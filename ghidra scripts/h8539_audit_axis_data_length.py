# H8/539F ECU - Whole-ROM Axis/Data-Length Mismatch Auditor
# @category MitsubishiECU
#
# PROBLEM THIS SOLVES: review5.md's "Lean Spool Trailing Time - Rich Side"
# trace (2026-08-10 session) found a table_2d_record at 0x11db8 whose real
# data span (measured by walking to the 0xFF sentinel / next real header)
# is only 9 bytes, but the ROM comment/scraper had it paired with a 14-
# element shared RPM axis at 0x2d2e6 -- a genuine mismatch, later shown to
# be a FALSE pairing entirely (the real caller,
# f2a0_f2c6_dual_timer_decay_and_f2c8_blend_dispatch, reads table_lookup_
# interp directly against a raw index register 0xF0CA with NO preceding
# axis_lookup_interp call at all -- so the 14-element axis was never
# really feeding this table in the first place).
#
# This raised an obvious follow-up: how many OTHER tables in this ROM have
# the same silent mismatch, either (a) genuinely paired with an axis whose
# declared count disagrees with the table's real measured data span, or
# (b) carrying an inherited/scraper-assumed axis pairing that a real
# caller trace would show is false (no axis_lookup_interp precedes it at
# all)? Doing this table-by-table by hand (as review5.md has been doing
# since SEED #1) does not scale to the ~85+ scraped table candidates in
# this ROM. This script automates the full pipeline in one pass:
#
#   1. SCAN the whole ROM for table_2d_record (02 00 <ptr>) and
#      table_3axis_record (03 00 <ptr> <ptr> <count>) header shapes.
#   2. MEASURE each candidate's real data span by walking forward from
#      the header until either an 0xFF sentinel byte or the start of
#      another recognised header shape is hit (same method used by hand
#      for 0x11db8/0x11dc2 in review5.md).
#   3. TRACE each candidate's real caller(s) via the literal-push
#      convention (mov:g.w #imm:16,@-SP :: mov:g.w #bank:8,@-SP :: pjsr),
#      reusing h8539_find_table_axis.py's exact method -- find real calls
#      to table_lookup_interp/table_3axis_interp_triple pushing this
#      table's address, then walk backward in the same function for the
#      nearest preceding axis_lookup_interp call.
#   4. IF a real axis pairing is found, READ that axis's own header
#      count field (offset+4, per the axis_lookup_record struct confirmed
#      in review5.md's STRUCT DEFINITIONS section) and COMPARE it against
#      the table's measured data span from step 2.
#   5. CLASSIFY each table into one of:
#        LENGTH-MATCH      - paired axis count == measured data span.
#                            Best available evidence the table is
#                            correctly sized (still not proof the pairing
#                            itself is real -- see NO-AXIS-CALL below).
#        LENGTH-MISMATCH   - paired axis count != measured data span.
#                            Exactly the Lean Spool Rich Side pattern --
#                            flag for manual review, same as review5.md's
#                            discipline (never auto-fix, only flag).
#        NO-AXIS-CALL      - a real table_lookup_interp/table_3axis_
#                            interp_triple caller was found, but with NO
#                            preceding axis_lookup_interp call in the same
#                            function. This means ANY axis currently
#                            attached to this table in XML/scraper output
#                            is unconfirmed at best, likely a false
#                            "nearest preceding axis-shaped header" guess
#                            (same failure mode as review5.md SEED #7a/#9/
#                            #10). The table's real bound is whatever
#                            range its raw index register can take --
#                            unrelated to any axis's element count.
#        NO-CALLER         - no real caller found via the literal-push
#                            convention at all. Matches SEED #7d
#                            methodology; strongest "not really a live 2D/
#                            3D lookup in this ROM revision" signal.
#
# This script only READS and REPORTS -- same discipline as
# h8539_find_table_axis.py and review5.md itself ("never guess", human
# reviews each flagged mismatch before it becomes an XML edit). Does not
# modify XML, Ghidra types, or bookmarks.
#
# IMPORT SETTINGS REQUIRED (same ROM setup as the other scripts):
#   Format:       Raw Binary
#   Language:     H8:BE:32:H8539F
#   Base Address: 0x00010000

from ghidra.program.flatapi import FlatProgramAPI

flat_api = FlatProgramAPI(currentProgram)
mem      = currentProgram.getMemory()
space    = currentProgram.getAddressFactory().getDefaultAddressSpace()
listing  = currentProgram.getListing()
refMgr   = currentProgram.getReferenceManager()
fnMgr    = currentProgram.getFunctionManager()

ROM_BASE = 0x00010000
ROM_END  = 0x0002FFFF

# Primitive dispatcher addresses (confirmed via decompile, review5.md
# STRUCT DEFINITIONS section, 2026-08-06). CPU-page-relative (bank 1).
AXIS_LOOKUP_INTERP        = 0x00014735   # axis_lookup_interp
TABLE_LOOKUP_INTERP       = 0x00014656   # table_lookup_interp (2D)
TABLE_3AXIS_INTERP_TRIPLE = 0x000148a2   # table_3axis_interp_triple (3D)

TABLE_BANK = 1
AXIS_BANK  = 2

MAX_BACKWARD_WALK = 400

# Data region to scan for header shapes. This ROM's table data has been
# observed (review5.md, review2.md) to live mostly in the 0x11000-0x14000
# and 0x2d000-0x2e000 ranges, but scan the whole ROM body to avoid missing
# anything -- cheap, this is a data scan not a disassembly.
SCAN_START = 0x00010000
SCAN_END   = 0x00030000

# Max bytes to walk forward past a header before giving up on measuring
# a data span (safety bound against runaway walks into misidentified
# regions).
MAX_DATA_SPAN_WALK = 512

# ------------------------------------------------------------------
# Helpers (byte-level)
# ------------------------------------------------------------------

def addr(offset):
    return space.getAddress(offset & 0xFFFFFFFF)

def read_byte(offset):
    try:
        return mem.getByte(addr(offset)) & 0xFF
    except Exception:
        return None

def read_bytes(offset, length):
    out = []
    for i in range(length):
        b = read_byte(offset + i)
        if b is None:
            return None
        out.append(b)
    return out

def read_u16be(offset):
    hi = read_byte(offset)
    lo = read_byte(offset + 1)
    if hi is None or lo is None:
        return None
    return (hi << 8) | lo

# ------------------------------------------------------------------
# Step 1: header shape detection
# ------------------------------------------------------------------
#
# table_2d_record: mode=0x02, mode_hi=0x00, index_src=ushort (2 bytes),
#                  data starts at offset+4.
# table_3axis_record: mode=0x03, mode_hi=0x00, x_ptr=ushort, y_ptr=ushort,
#                  nrows=byte, data starts at offset+7.
#
# Both index_src/x_ptr/y_ptr are expected to be RAM cells in the 0xF0xx-
# 0xF2xx range in every confirmed example so far (review5.md's pointer
# map) -- used here ONLY as a cheap plausibility filter to cut false
# positives, per the earlier conversation's own caution that "02 00 F0 xx"
# alone is a weak signature prone to matching coincidental data bytes.
# This is a HEURISTIC FILTER, not proof -- every candidate still goes
# through the real caller trace in step 3 before being trusted.

def looks_like_ram_ptr(v):
    return v is not None and 0xF000 <= v <= 0xF3FF

def is_table_2d_header(offset):
    b = read_bytes(offset, 4)
    if b is None:
        return False
    if b[0] != 0x02 or b[1] != 0x00:
        return False
    ptr = (b[2] << 8) | b[3]
    return looks_like_ram_ptr(ptr)

def is_table_3axis_header(offset):
    b = read_bytes(offset, 7)
    if b is None:
        return False
    if b[0] != 0x03 or b[1] != 0x00:
        return False
    x_ptr = (b[2] << 8) | b[3]
    y_ptr = (b[4] << 8) | b[5]
    return looks_like_ram_ptr(x_ptr) and looks_like_ram_ptr(y_ptr)

def is_axis_header(offset):
    """axis_lookup_record: ptr1, ptr2 (both RAM cells), count (ushort,
    clamped <=0xFF in practice per axis_lookup_interp's own decompile).
    Used only to recognise 'the next real structure starts here' during
    forward data-span walks, not scanned as a top-level candidate list
    (axes are already well covered by review5.md's own checklist)."""
    b = read_bytes(offset, 6)
    if b is None:
        return False
    ptr1 = (b[0] << 8) | b[1]
    ptr2 = (b[2] << 8) | b[3]
    count = (b[4] << 8) | b[5]
    return looks_like_ram_ptr(ptr1) and looks_like_ram_ptr(ptr2) and 0 < count <= 0xFF

def scan_for_headers():
    """Single forward pass over the scan range, collecting every offset
    that looks like a table_2d_record or table_3axis_record header."""
    candidates = []
    off = SCAN_START
    while off < SCAN_END:
        if is_table_3axis_header(off):
            candidates.append((off, "3D"))
            off += 7  # skip past this header, don't re-match inside it
            continue
        if is_table_2d_header(off):
            candidates.append((off, "2D"))
            off += 4
            continue
        off += 1
    return candidates

# ------------------------------------------------------------------
# Step 2: measure real data span (walk to sentinel or next header)
# ------------------------------------------------------------------

def measure_data_span(header_offset, kind):
    """Returns (data_start_offset, measured_length, stop_reason)."""
    data_start = header_offset + (7 if kind == "3D" else 4)
    pos = data_start
    steps = 0
    while steps < MAX_DATA_SPAN_WALK:
        b = read_byte(pos)
        if b is None:
            return (data_start, pos - data_start, "END-OF-ROM")
        if b == 0xFF:
            return (data_start, pos - data_start, "SENTINEL")
        if is_table_2d_header(pos) or is_table_3axis_header(pos) or is_axis_header(pos):
            return (data_start, pos - data_start, "NEXT-HEADER")
        pos += 1
        steps += 1
    return (data_start, pos - data_start, "MAX-WALK-EXCEEDED")

# ------------------------------------------------------------------
# Step 3: real caller / axis pairing trace
# (ported directly from h8539_find_table_axis.py -- see that script for
# the original documented rationale)
# ------------------------------------------------------------------

def containing_function(a):
    return fnMgr.getFunctionContaining(a)

def is_call_to(instr, target_offset):
    if instr is None:
        return False
    mnem = instr.getMnemonicString().lower()
    if "jsr" not in mnem and "call" not in mnem and "bsr" not in mnem:
        return False
    refs = instr.getReferencesFrom()
    for r in refs:
        if r.getReferenceType().isCall():
            to_off = r.getToAddress().getOffset()
            if to_off == (ROM_BASE + target_offset):
                return True
    return False

def read_preceding_push_pair(instr):
    prev1 = instr.getPrevious()
    prev2 = prev1.getPrevious() if prev1 is not None else None
    if prev1 is None or prev2 is None:
        return None
    try:
        bank_str = prev1.toString()
        imm_str  = prev2.toString()
        if "#0x" not in bank_str or "#0x" not in imm_str:
            return None
        bank = int(bank_str.split("#0x")[1].split(":")[0], 16)
        imm  = int(imm_str.split("#0x")[1].split(":")[0], 16)
        return (bank, imm)
    except Exception:
        return None

def find_table_call_sites(table_offset):
    results = []
    for target, is_3d in ((TABLE_LOOKUP_INTERP, False), (TABLE_3AXIS_INTERP_TRIPLE, True)):
        target_off = target - ROM_BASE if target >= ROM_BASE else target
        target_addr = addr(ROM_BASE + target_off)
        for ref in refMgr.getReferencesTo(target_addr):
            if not ref.getReferenceType().isCall():
                continue
            call_instr = listing.getInstructionAt(ref.getFromAddress())
            if call_instr is None:
                continue
            pushed = read_preceding_push_pair(call_instr)
            if pushed is None:
                continue
            bank, imm = pushed
            if bank == TABLE_BANK and imm == table_offset:
                results.append((call_instr, is_3d))
    return results

def find_preceding_axis_call(call_instr):
    fn = containing_function(call_instr.getAddress())
    if fn is None:
        return None
    fn_start = fn.getEntryPoint()
    axis_target_off = AXIS_LOOKUP_INTERP - ROM_BASE if AXIS_LOOKUP_INTERP >= ROM_BASE else AXIS_LOOKUP_INTERP
    cur = call_instr.getPrevious()
    steps = 0
    found = None
    other_axis_calls = 0
    while cur is not None and steps < MAX_BACKWARD_WALK:
        if cur.getAddress().getOffset() < fn_start.getOffset():
            break
        if is_call_to(cur, axis_target_off):
            pushed = read_preceding_push_pair(cur)
            if pushed is not None:
                bank, imm = pushed
                if bank == AXIS_BANK:
                    if found is None:
                        found = (imm, steps)
                    elif imm != found[0]:
                        # A second, DIFFERENT axis call also precedes this
                        # table call -- the returned match is nearest-call,
                        # not proven-unique. See review10.md.
                        other_axis_calls += 1
        cur = cur.getPrevious()
        steps += 1
    if found is None:
        return None
    return (found[0], found[1], other_axis_calls)

# ------------------------------------------------------------------
# Step 4: read paired axis's own declared count
# ------------------------------------------------------------------

def read_axis_count(axis_offset_bare):
    """axis_offset_bare is the bank-relative offset pushed into
    axis_lookup_interp (e.g. 0xd2e6 for 0x2d2e6). Reconstruct the full
    ROM address using the same bank*0x10000 + imm convention used
    throughout this project's disassembly (see h8539_find_table_axis.py).
    """
    # 2026-08-11 FIX: was double-counting ROM_BASE (0x00010000 + AXIS_BANK*0x10000
    # + offset), which computes an address one full bank too high and outside any
    # mapped block (e.g. axis_offset_bare=0xd7ca -> old formula gave 0x3d7ca,
    # unmapped/unreadable; correct formula gives 0x2d7ca, which reads back as a
    # real, sane axis_lookup_interp record: value_ptr=F0C0, axis_ptr=EF6A, count=5,
    # breakpoints 0000/4000/8000/C000/FF00 -- verified manually against the live
    # call site in isc_openloop_target_calc @ 0x17cb8 before applying this fix).
    # The bank multiply alone already reconstructs the full address; ROM_BASE must
    # not be added on top of it.
    full = (AXIS_BANK * 0x10000) + (axis_offset_bare & 0xFFFF)
    count = read_u16be(full + 4)
    return full, count

# ------------------------------------------------------------------
# Step 5: classify
# ------------------------------------------------------------------

def audit_table(header_offset, kind):
    measured_start, measured_len, stop_reason = measure_data_span(header_offset, kind)
    call_sites = find_table_call_sites(header_offset - ROM_BASE if header_offset >= ROM_BASE else header_offset)

    result = {
        "header": header_offset,
        "kind": kind,
        "measured_len": measured_len,
        "stop_reason": stop_reason,
        "status": None,
        "detail": None,
        "pairings": [],
    }

    if not call_sites:
        result["status"] = "NO-CALLER"
        result["detail"] = ("no table_lookup_interp/table_3axis_interp_triple call found "
                             "referencing this table via the literal-push convention.")
        return result

    any_paired = False
    any_mismatch = False
    any_ambiguous = False
    measured_len_exceeds_axis = False
    for call_instr, is_3d in call_sites:
        fn = containing_function(call_instr.getAddress())
        fn_name = fn.getName() if fn is not None else "<no function>"
        axis_result = find_preceding_axis_call(call_instr)
        if axis_result is not None:
            axis_offset, distance, other_axis_calls = axis_result
            axis_full_addr, axis_count = read_axis_count(axis_offset)
            any_paired = True
            if other_axis_calls > 0:
                any_ambiguous = True
            mismatch = (axis_count is not None and axis_count != measured_len)
            if mismatch:
                any_mismatch = True
                if axis_count is not None and measured_len > axis_count:
                    measured_len_exceeds_axis = True
            result["pairings"].append({
                "caller_function": fn_name,
                "call_site": call_instr.getAddress().toString(),
                "axis_offset": "0x%05X" % axis_full_addr,
                "axis_count": axis_count,
                "measured_table_len": measured_len,
                "mismatch": mismatch,
                "instructions_back": distance,
                "other_axis_calls": other_axis_calls,
            })
        else:
            result["pairings"].append({
                "caller_function": fn_name,
                "call_site": call_instr.getAddress().toString(),
                "axis_offset": None,
                "axis_count": None,
                "measured_table_len": measured_len,
                "mismatch": None,
                "instructions_back": None,
            })

    if any_ambiguous:
        result["status"] = "AXIS-AMBIGUOUS"
        result["detail"] = ("a real axis_lookup_interp call precedes this table's call, but "
                             "a DIFFERENT axis_lookup_interp call was also found earlier in "
                             "the same backward walk -- the nearest one is reported, but which "
                             "axis actually feeds this table cannot be determined by proximity "
                             "alone. Axis identity is UNVERIFIED regardless of whether length "
                             "happens to match; needs manual/dataflow-level confirmation before "
                             "trusting either the axis address or any length comparison against "
                             "it." + (" (length also disagrees: ROM says %s vs measured %d)" %
                                      (str([p["axis_count"] for p in result["pairings"] if p["mismatch"]]),
                                       measured_len) if any_mismatch else ""))
    elif any_mismatch:
        result["status"] = "LENGTH-UNVERIFIED"
        result["detail"] = ("paired axis count disagrees with measured table data span -- "
                             "this is NOT necessarily wrong: a table legitimately covers only "
                             "part of a shared axis's range (e.g. an 18-point 0-11000 RPM axis "
                             "where a given table only needs 0-4000 RPM), and this script has "
                             "no way to verify which subset of axis breakpoints the table's "
                             "data actually corresponds to, or to independently confirm the "
                             "paired axis address is the semantically-correct one for this "
                             "table (find_preceding_axis_call only confirms a real call "
                             "precedes this one in the same function, not that it's the RIGHT "
                             "axis if more than one is reachable). Needs manual value-level "
                             "check against a reference (e.g. known-good XML/tuning software) "
                             "before treating as either correct or a bug; NOT a confirmed defect."
                             + (" table_len > axis_count -- this direction IS structurally "
                                "suspicious (more table entries than axis breakpoints to index "
                                "them) and warrants closer look first."
                                if measured_len_exceeds_axis else ""))
    elif any_paired:
        result["status"] = "LENGTH-MATCH"
        result["detail"] = ("paired axis count matches measured table data span, AND no other "
                             "axis call was found ambiguously nearby -- highest-confidence "
                             "tier available, but still a proximity-based match, not formal "
                             "dataflow proof.")
    else:
        result["status"] = "NO-AXIS-CALL"
        result["detail"] = ("real caller(s) found, but none has a preceding axis_lookup_interp call -- "
                             "any axis currently attached to this table elsewhere (XML/scraper) is unconfirmed.")

    return result

# ------------------------------------------------------------------
# Main
# ------------------------------------------------------------------

def main():
    print("=" * 78)
    print("H8/539F Whole-ROM Axis/Data-Length Mismatch Auditor")
    print("=" * 78)
    print("Scanning 0x%05X-0x%05X for table_2d_record/table_3axis_record headers..." %
          (SCAN_START, SCAN_END))

    candidates = scan_for_headers()
    print("Found %d header candidates (heuristic shape match, unconfirmed by caller trace yet)" %
          len(candidates))
    print("")

    results = []
    for header_offset, kind in candidates:
        r = audit_table(header_offset, kind)
        results.append(r)

    ambiguous    = [r for r in results if r["status"] == "AXIS-AMBIGUOUS"]
    mismatches   = [r for r in results if r["status"] == "LENGTH-UNVERIFIED"]
    matches      = [r for r in results if r["status"] == "LENGTH-MATCH"]
    no_axis_call = [r for r in results if r["status"] == "NO-AXIS-CALL"]
    no_caller    = [r for r in results if r["status"] == "NO-CALLER"]

    print("-" * 78)
    print("AXIS-AMBIGUOUS (%d) -- more than one axis_lookup_interp call precedes this "
          "table's call; axis identity itself is unverified, not just length" % len(ambiguous))
    print("-" * 78)
    for r in ambiguous:
        print("Table 0x%05X (%s), measured=%d bytes (stopped: %s)" %
              (r["header"], r["kind"], r["measured_len"], r["stop_reason"]))
        for p in r["pairings"]:
            if p.get("other_axis_calls", 0) > 0:
                print("  [%s @ %s] nearest axis=%s count=%s vs measured=%d  "
                      "<-- %d OTHER axis call(s) also precede this one" %
                      (p["caller_function"], p["call_site"], p["axis_offset"],
                       p["axis_count"], p["measured_table_len"], p["other_axis_calls"]))
        print("")

    print("-" * 78)
    print("LENGTH-UNVERIFIED (%d) -- paired axis count disagrees with measured data span; "
          "NOT necessarily wrong, see per-entry note" % len(mismatches))
    print("-" * 78)
    for r in mismatches:
        print("Table 0x%05X (%s), measured=%d bytes (stopped: %s)" %
              (r["header"], r["kind"], r["measured_len"], r["stop_reason"]))
        for p in r["pairings"]:
            if p["mismatch"]:
                print("  [%s @ %s] axis=%s count=%s vs measured=%d  <-- MISMATCH" %
                      (p["caller_function"], p["call_site"], p["axis_offset"],
                       p["axis_count"], p["measured_table_len"]))
        print("")

    print("-" * 78)
    print("LENGTH-MATCH (%d) -- paired axis count agrees with measured data span" % len(matches))
    print("-" * 78)
    for r in matches:
        print("Table 0x%05X (%s), measured=%d bytes (stopped: %s)" %
              (r["header"], r["kind"], r["measured_len"], r["stop_reason"]))
        for p in r["pairings"]:
            print("  [%s @ %s] axis=%s count=%s vs measured=%d  -- CONFIRMED" %
                  (p["caller_function"], p["call_site"], p["axis_offset"],
                   p["axis_count"], p["measured_table_len"]))
        print("")

    print("-" * 78)
    print("LENGTH-MATCH (%d) -- paired axis count agrees with measured data span" % len(matches))
    print("-" * 78)
    for r in matches:
        print("Table 0x%05X (%s), measured=%d bytes (stopped: %s)" %
              (r["header"], r["kind"], r["measured_len"], r["stop_reason"]))
        for p in r["pairings"]:
            print("  [%s @ %s] axis=%s count=%s vs measured=%d  -- CONFIRMED" %
                  (p["caller_function"], p["call_site"], p["axis_offset"],
                   p["axis_count"], p["measured_table_len"]))
        print("")

    print("-" * 78)
    print("NO-AXIS-CALL (%d) -- real caller found, no axis pairing at all" % len(no_axis_call))
    print("-" * 78)
    for r in no_axis_call:
        print("Table 0x%05X (%s), measured=%d bytes (stopped: %s)" %
              (r["header"], r["kind"], r["measured_len"], r["stop_reason"]))
        for p in r["pairings"]:
            print("  [%s @ %s] no preceding axis_lookup_interp call" %
                  (p["caller_function"], p["call_site"]))
        print("")

    print("-" * 78)
    print("Summary: %d LENGTH-MATCH, %d LENGTH-UNVERIFIED, %d AXIS-AMBIGUOUS, "
          "%d NO-AXIS-CALL, %d NO-CALLER (of %d candidates)" %
          (len(matches), len(mismatches), len(ambiguous), len(no_axis_call), len(no_caller), len(results)))
    print("-" * 78)
    print("NOTE: header detection is a heuristic byte-shape scan and WILL include false")
    print("positives -- every result above already passed the real-caller trace filter")
    print("(literal-push convention), consistent with review5.md's discipline: never")
    print("trust shape alone, only a confirmed real caller. NO-CALLER entries were")
    print("dropped from the detailed printout above (usually the majority, mostly")
    print("false-positive header shapes) -- inspect results[] in-session for the full list.")

main()

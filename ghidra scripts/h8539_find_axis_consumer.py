# H8/539F ECU - Axis-to-Consumer Finder
# @category MitsubishiECU
#
# PROBLEM THIS SOLVES: h8539_find_table_axis.py answers "given a TABLE,
# which axis feeds it" by walking backward from a known table_lookup_interp/
# table_3axis_interp_triple call site. That's the wrong direction for
# review5.md SEED #7f (0x2d080, "Engine Speed"/RPM naming on 2d086): we
# already know the AXIS (0x2d080, confirmed real via throttle_body_update's
# literal push), but don't know what reads the {index,frac} result
# axis_lookup_interp writes back into that same cell. All prior manual
# search (get_xrefs_to, raw byte-pattern grep for D080, indexed-dispatch
# selector check, partial manual disassembly walk of the immediate call
# chain) came back empty or inconclusive -- see review5.md SEED #7f
# 2026-08-07 follow-ups for the exhausted-by-hand methods.
#
# WHAT THIS SCRIPT DOES, for a given axis address (or list of them):
#   1. Finds every pjsr call site to axis_lookup_interp (0x14735) whose
#      immediately-preceding literal push matches (AXIS_BANK, axis_offset)
#      -- same push-immediate convention as h8539_find_table_axis.py.
#   2. For each such call site, walks FORWARD through the same function's
#      instruction stream (not just to the next table_lookup_interp call --
#      ANY instruction that reads the axis's own value_ptr cell, since the
#      result is written back in place) looking for the next instruction
#      that references the axis address itself as a memory operand, and
#      separately checks whether a table_lookup_interp/table_3axis_interp_
#      triple call follows in the same function at all.
#   3. ALSO checks cross-function reads: since axis_lookup_interp writes
#      the result back into the SAME RAM cell used as the axis's own
#      value_ptr (offset+0 of axis_lookup_record, per review5.md STRUCT
#      DEFINITIONS), a DIFFERENT function entirely could read that cell
#      later (e.g. table_2d_record's index_src field pointing at it, or a
#      plain scalar read). This script reports every function in the
#      whole ROM whose disassembly references the axis RAM address
#      (axis_offset, not the axis_lookup_record's own ROM header address)
#      as an operand anywhere, not just immediately after the write.
#
# This is READ-ONLY, same discipline as h8539_find_table_axis.py -- does
# not modify XML or Ghidra state. Output is meant to be pasted into
# review5.md's per-axis trace notes for a human to evaluate.
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

AXIS_LOOKUP_INTERP        = 0x00014735   # axis_lookup_interp
TABLE_LOOKUP_INTERP       = 0x00014656   # table_lookup_interp (2D)
TABLE_3AXIS_INTERP_TRIPLE = 0x000148a2   # table_3axis_interp_triple (3D)

MAX_FORWARD_WALK = 400

# ---------------------------------------------------------------------
# EDIT THIS: axis addresses (RAM offsets, e.g. 0x2d080) to find consumers
# for. These are AXIS addresses (bank=2 pushes), not table addresses.
#
# IMPORTANT (found the hard way, 2026-08-07 debug session): the literal
# actually pushed before axis_lookup_interp is the bare 16-bit page
# offset ONLY (e.g. 0xd080), NOT the full address with the bank digit
# folded in (0x2d080) -- confirmed directly: disassembly at 0x217fb
# reads 'mov:g.w #0xd080:16,@-SP', bank pushed separately as
# 'mov:g.w #0x2:8,@-SP'. Enter addresses here in either form (full
# 0x2d080 or bare 0xd080) -- normalize_axis_offset() below strips the
# bank nibble the same way for both, so both forms behave identically.
# ---------------------------------------------------------------------
AXIS_ADDRESSES = [
    0x0002d080,   # "Engine Speed"/RPM @ 2d086 -- review5.md SEED #7f,
                   # real write confirmed (throttle_body_update), CONFIRMED
                   # zero consumers anywhere in ROM (2026-08-07 run)
    0x0002d146,   # "Error" @ Idle Error Correction -- review5.md SEED #3,
                   # real write confirmed (isc_f408_correction_calc via
                   # 0xD146 literal push), F0C0-writer question still open

    # --- 2026-08-14 [Claude/session]: Alternator candidate axes ---
    # From VR4's ROM-specific 20030013.xml (has real addresses, unlike
    # vr4base.xml which is address-less). "Alternator G Terminal Dead
    # Zone" table @ VR4 0x12574, RPM Y-axis header @ VR4 0x2d020 (data
    # starts 0x2d026, confirmed 10-elem clean 500-7000 RPM breakpoints
    # via direct memory read). "Alternator Charge Current" table @ VR4
    # 0x12527, Load X-axis header @ VR4 0x2d03a (data 0x2d040, 9 elem),
    # RPM Y-axis header @ VR4 0x2d00a (data 0x2d010, 8 elem). VR4 itself
    # has ZERO xrefs/literal-push hits anywhere in its own ROM for
    # 12574/12527/2d026/2d010/2d040 -- axis_lookup_interp caller sweep
    # (47 call sites) and raw byte-pattern search both came back empty.
    # Adding the RVR-side candidate axis (byte-identical RPM breakpoint
    # data found via search_byte_patterns at RVR 0x2d63c) here anyway,
    # since that address is already claimed by TCU/Knock tables and this
    # script's whole-ROM cross-function scan is the last real check
    # before calling Alternator a dead end in this project.
    0x0002d63c,   # RVR: identical RPM curve to VR4 Alt Dead Zone axis
                   # data (500-7000 RPM x10), but ALREADY claimed by
                   # TABLE_3D_000131FA_TCU (Axis1) and Knock Sensor
                   # Filter Maps 1-5 in 21000011...xml. Running here to
                   # get the FULL consumer list (script's cross-fn scan),
                   # not just the first caller found by hand.

    # --- ISC cluster orphan axes (RVRbase.xml SCALING_TABLE_* holding
    # pen, addresses given as DATA start = header+6, matching this
    # script's axis_offset convention) still sitting under generic
    # TABLE_2D_XXXXXXXX_ISC / TABLE_2D_XXXXXXXX names in RVR's
    # 21000011...xml as of 2026-08-14. Batch 1 of the sweep discussed
    # with the user -- cross-reference results against VR4's named ISC
    # tables (ISC Baro Compensation, ISCV Demand..., ISCV Initial
    # Steps..., Idle Error Correction (Manual/drive), etc.)
    0x0002d0a0,   # RPM, 16 elem -- feeds several "Open Loop Load"/ISC-
                   # adjacent 2D tables per 21000011...xml
    0x0002d0f6,   # RPM, 8/16 elem -- TABLE_2D_0001186C/11880/11894_Fueling
    0x0002d136,   # RPM, 8 elem -- TABLE_2D_00011E4A_TableLookup
    0x0002d200,   # RPM, 5 elem -- TABLE_2D_00012950_ISC
    0x0002d2a2,   # RPM, 4 elem -- TABLE_2D_00012858_ISC / 00012860_ISC
    0x0002d496,   # RPM, 8 elem -- TABLE_2D_000127FC_ISC
    0x0002d186,   # RPM, 6 elem -- TABLE_2D_0001284E_ISC
    0x0002d27a,   # Battery, 9 elem -- TABLE_2D_00012824_ISC (VR4 has no
                   # direct Battery-axis ISC table named; check whether
                   # this is really Coil Charge Time family instead)
]

def normalize_axis_offset(a):
    """Axis literal pushes only carry the low 16 bits (page offset);
    the bank (2 for axis records in this ROM) is pushed as a separate
    byte immediate and is NOT part of the 16-bit literal. Strip any
    full-address form down to the bare page offset so comparisons
    against the disassembled literal work regardless of which form
    the caller used in AXIS_ADDRESSES."""
    return a & 0xFFFF

TABLE_BANK = 1
AXIS_BANK  = 2

# ------------------------------------------------------------------
# Helpers (shared convention with h8539_find_table_axis.py)
# ------------------------------------------------------------------

def norm_offset(a):
    if a >= ROM_BASE:
        return a - ROM_BASE
    return a

def addr(offset):
    return space.getAddress((ROM_BASE + offset) & 0xFFFFFFFF)

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

def find_axis_call_sites(axis_offset):
    """Scan the whole ROM for pjsr calls to AXIS_LOOKUP_INTERP whose
    immediately-preceding literal push matches (AXIS_BANK, axis_offset).
    Returns a list of call_instr."""
    results = []
    target_off = AXIS_LOOKUP_INTERP - ROM_BASE if AXIS_LOOKUP_INTERP >= ROM_BASE else AXIS_LOOKUP_INTERP
    target_addr = addr(target_off)
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
        if bank == AXIS_BANK and imm == axis_offset:
            results.append(call_instr)
    return results

def instr_references_ram(instr, ram_offset):
    """True if instr's operands reference the raw RAM offset (as it
    would appear in a disassembly string, e.g. '@0xd080:16' or
    similar direct-page reference)."""
    if instr is None:
        return False
    s = instr.toString().lower()
    needle = "0x%x" % ram_offset
    return needle in s

def find_forward_consumer(call_instr, axis_offset):
    """Walk forward from the axis_lookup_interp call site, within the
    same function, looking for the next instruction referencing the
    axis's own RAM cell (axis_offset) as an operand -- that's a
    same-function consumer. Also flags whether a table_lookup_interp/
    table_3axis_interp_triple call appears anywhere later in the
    function (regardless of whether it's provably fed by this axis)."""
    fn = containing_function(call_instr.getAddress())
    if fn is None:
        return None
    fn_end = fn.getBody().getMaxAddress()
    table_off = TABLE_LOOKUP_INTERP - ROM_BASE if TABLE_LOOKUP_INTERP >= ROM_BASE else TABLE_LOOKUP_INTERP
    triple_off = TABLE_3AXIS_INTERP_TRIPLE - ROM_BASE if TABLE_3AXIS_INTERP_TRIPLE >= ROM_BASE else TABLE_3AXIS_INTERP_TRIPLE
    cur = call_instr.getNext()
    steps = 0
    found_table_call = None
    consumer_instr = None
    while cur is not None and steps < MAX_FORWARD_WALK and cur.getAddress().getOffset() <= fn_end.getOffset():
        if consumer_instr is None and instr_references_ram(cur, axis_offset):
            consumer_instr = cur
        if found_table_call is None and (is_call_to(cur, table_off) or is_call_to(cur, triple_off)):
            found_table_call = cur
        if consumer_instr is not None and found_table_call is not None:
            break
        cur = cur.getNext()
        steps += 1
    return {
        "consumer_instr": consumer_instr.getAddress().toString() if consumer_instr is not None else None,
        "table_call_instr": found_table_call.getAddress().toString() if found_table_call is not None else None,
    }

def scan_whole_rom_for_ram_refs(axis_offset, exclude_addrs):
    """Cross-function safety net: scan every defined function's
    instructions for a reference to axis_offset as an operand,
    skipping addresses already accounted for. This is the expensive
    path -- only run for axes with no same-function consumer found."""
    hits = []
    fn_iter = fnMgr.getFunctions(True)
    for fn in fn_iter:
        body = fn.getBody()
        instrs = listing.getInstructions(body, True)
        for instr in instrs:
            a = instr.getAddress()
            if a in exclude_addrs:
                continue
            if instr_references_ram(instr, axis_offset):
                hits.append((fn.getName(), a.toString(), instr.toString()))
    return hits

def trace_axis(axis_offset_raw):
    # Strip to the bare 16-bit page offset -- see normalize_axis_offset
    # docstring above for why (the disassembled literal never carries
    # the bank nibble; it's pushed as a separate byte).
    axis_offset = normalize_axis_offset(axis_offset_raw)
    call_sites = find_axis_call_sites(axis_offset)
    if not call_sites:
        return {
            "axis": axis_offset,
            "status": "NO-CALLER",
            "detail": "no axis_lookup_interp call found pushing this axis literally -- "
                      "unexpected if a write was already confirmed by hand; re-check the "
                      "manual finding before trusting this result.",
            "results": [],
        }
    per_call = []
    any_same_fn_consumer = False
    exclude = set()
    for call_instr in call_sites:
        exclude.add(call_instr.getAddress())
        fn = containing_function(call_instr.getAddress())
        fn_name = fn.getName() if fn is not None else "<no function>"
        fwd = find_forward_consumer(call_instr, axis_offset)
        if fwd["consumer_instr"] is not None:
            any_same_fn_consumer = True
        per_call.append({
            "caller_function": fn_name,
            "call_site": call_instr.getAddress().toString(),
            "same_fn_consumer": fwd["consumer_instr"],
            "same_fn_table_call": fwd["table_call_instr"],
        })
    cross_fn_hits = []
    if not any_same_fn_consumer:
        # Only pay for the expensive whole-ROM scan if the cheap
        # same-function walk found nothing.
        cross_fn_hits = scan_whole_rom_for_ram_refs(axis_offset, exclude)
    status = "PAIRED" if (any_same_fn_consumer or cross_fn_hits) else "NO-CONSUMER-FOUND"
    return {
        "axis": axis_offset,
        "status": status,
        "detail": "same-function consumer found" if any_same_fn_consumer else (
            "no same-function consumer; whole-ROM scan found %d cross-function reference(s)" % len(cross_fn_hits)
            if cross_fn_hits else
            "no same-function consumer AND whole-ROM instruction scan found zero references to "
            "this RAM cell anywhere else in the ROM. This is a strong dead-end signal -- either "
            "the result is read via a computed/indirect addressing mode this scan's string-match "
            "approach can't see, or the axis_lookup_interp call's result is genuinely unused "
            "(dead code / write-only diagnostic cell)."
        ),
        "results": per_call,
        "cross_fn_hits": cross_fn_hits,
    }

def main():
    print("=" * 70)
    print("H8/539F Axis-to-Consumer Finder")
    print("=" * 70)
    for a in AXIS_ADDRESSES:
        r = trace_axis(a)
        print("")
        print("Axis 0x%04X (bank 2 -> 0x2%04X) -> %s" % (r["axis"], r["axis"], r["status"]))
        print("  %s" % r["detail"])
        for p in r["results"]:
            print("  [call in %s @ %s]" % (p["caller_function"], p["call_site"]))
            print("    same-fn consumer instr: %s" % p["same_fn_consumer"])
            print("    same-fn table call:     %s" % p["same_fn_table_call"])
        if r.get("cross_fn_hits"):
            print("  Cross-function references found:")
            for fn_name, a_str, instr_str in r["cross_fn_hits"][:20]:
                print("    %s @ %s :: %s" % (fn_name, a_str, instr_str))
            if len(r["cross_fn_hits"]) > 20:
                print("    ... and %d more" % (len(r["cross_fn_hits"]) - 20))
    print("")
    print("=" * 70)

main()

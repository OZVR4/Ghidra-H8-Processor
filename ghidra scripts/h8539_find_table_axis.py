# H8/539F ECU - Table-to-Axis Pairing Finder
# @category MitsubishiECU
#
# PROBLEM THIS SOLVES: review5.md's axis-identity verification pass keeps
# hand-deriving "which axis feeds this table" call-by-call -- disassemble
# the real caller, find the pjsr @0x14735 (axis_lookup_interp) call, read
# its pushed literal, repeat. Slow, and several tables in RVR_base.xml
# currently sit with placeholder/unconfirmed axes specifically because
# nobody has done this trace for them yet (see review5.md SEED #7d,
# 0x2d0f6/0x2d100, as the worst case -- a genuine dead end found only
# after exhausting direct xrefs AND literal-constant search).
#
# WHAT THIS SCRIPT DOES, for a given table address (or a list of them):
#   1. Finds every pjsr call site to table_lookup_interp (0x14656) or
#      table_3axis_interp_triple (0x148a2) that pushes THIS table's
#      literal address immediately beforehand (same push-immediate
#      convention documented in review5.md and review2.md Method step 4:
#      mov:g.w #imm:16,@-SP :: mov:g.w #bank:8,@-SP :: pjsr).
#   2. For each such call site, walks BACKWARD through the same
#      function's instruction stream looking for the nearest preceding
#      pjsr @0x14735 (axis_lookup_interp) call, and reads ITS pushed
#      literal + bank byte -- that pair is the real axis address.
#   3. Reports one of three outcomes per table:
#        PAIRED   - found a preceding axis_lookup_interp call in the same
#                   function; reports the axis address (bank<<16 | imm)
#                   and how many instructions back it was found.
#        NO-AXIS-CALL - a real table_lookup_interp/table_3axis_interp_triple
#                   call was found, but no axis_lookup_interp call precedes
#                   it anywhere earlier in the same function. This is the
#                   genuine "placeholder" signal -- either the axis was
#                   set up by a DIFFERENT function (cross-function value
#                   passed in some other way, needs a human look), or the
#                   table is being indexed by something that was already
#                   sitting in the result RAM cell from a previous call
#                   elsewhere (stale-value read, also worth flagging).
#        NO-CALLER  - no table_lookup_interp/table_3axis_interp_triple
#                   call references this table address at all via the
#                   literal-push pattern. Matches review5.md's SEED #7d
#                   methodology exactly (Method step 3's fallback) --
#                   this is the strongest "is this table even really
#                   read as a 2D/3D lookup in this ROM revision" signal,
#                   stronger than a plain xref miss alone.
#
# This script only READS and REPORTS. It does not modify the XML or
# apply any Ghidra types/labels -- per review5.md's own discipline
# ("never guess", "only correct entries with CONFIRMED WRONG verdicts"),
# a human should look at each PAIRED/NO-AXIS-CALL/NO-CALLER result before
# it becomes an XML edit. Output is meant to be pasted straight into
# review5.md's per-axis trace notes.
#
# USAGE:
#   Edit TABLE_ADDRESSES below, then run. Addresses may be given as the
#   bare ROM offset (0x11cee) or CPU-page form (0x0001cee) -- both are
#   normalized against ROM_BASE.
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

# Max instructions to walk backward within the same function before
# giving up and reporting NO-AXIS-CALL. Generous -- real examples traced
# by hand in review5.md (throttle_target_ramp_update, ~140 instructions
# from function start to the axis/table pair) fit comfortably under this.
MAX_BACKWARD_WALK = 400

# ---------------------------------------------------------------------
# EDIT THIS: table addresses to trace (bare ROM offset, e.g. 0x11cee for
# "Boost Limit" per review5.md SEED #7d). Add one entry per table you
# want paired. Bank byte for the table push is fixed at 1 for all known
# examples so far (table_lookup_interp/table_3axis_interp_triple always
# take bank=1 CPU-page addresses) -- if a genuinely different bank shows
# up in a future ROM, TABLE_BANK below would need to become per-address.
# ---------------------------------------------------------------------
TABLE_ADDRESSES = [
    0x00011cee,   # "Boost Limit" -- review5.md SEED #7d, known dead end
    0x00011800,   # "MAF Scaling (2D)" -- known-good, sanity check
    0x00011840,   # "Barometric Pressure Compensation (3D)" -- known-good
    0x00011d86,   # parent of Post Start Enrich Trim vs Load -- known-good (axis 0x2d192)
    # 2026-08-06 [Claude]: "Decel Fuel Cut Delay" cluster, user-reported +4 offset
    # bug fixed in XML (address now points at table_2d_record data, header+4).
    # Direct literal-byte search (28fa/2906/2912/291e, and header addrs 128f6/
    # 12902/1290e/1291a) found ZERO hits anywhere in ROM, and Ghidra's own
    # get_xrefs_to confirmed zero references to all 8 addresses. Adding here to
    # run the same check through this script's more thorough backward-walk
    # method before concluding NO-CALLER via manual search alone.
    0x000128fa,   # "Decel Fuel Cut Delay - Neutral A/C Off"
    0x00012906,   # "Decel Fuel Cut Delay - In Gear A/C Off"
    0x00012912,   # "Decel Fuel Cut Delay - Neutral A/C On"
    0x0001291e,   # "Decel Fuel Cut Delay - In Gear A/C On"

    # --- 2026-08-14 [Claude/session]: VR4 Alternator candidates ---
    # RUN THIS BLOCK AGAINST THE VR4 PROGRAM ("20030013_legnum vr4.hex"),
    # NOT RVR. From VR4's ROM-specific 20030013.xml. Manual get_xrefs_to
    # and raw byte-pattern search for both addresses already came back
    # empty (see session notes), but this script's literal-push backward
    # walk is more thorough than either -- worth confirming NO-CALLER
    # properly before writing these off as unresolvable.
    0x00012574,   # "Alternator G Terminal Dead Zone" (VR4, 20030013.xml)
    0x00012527,   # "Alternator Charge Current" (VR4, 20030013.xml)

    # --- ISC cluster generic tables (RVR side) ---
    # RUN THIS BLOCK AGAINST THE RVR PROGRAM. Parent 2D table addresses
    # from 21000011_1997-2001_RVR_X3_Mt__4g63t_.xml, category="Unknown"
    # with an "_ISC"-style name fragment or plain TABLE_2D_XXXXXXXX,
    # paired against axes already run through h8539_find_axis_consumer.py
    # above. Goal: confirm/re-derive the axis pairing independently (this
    # script walks backward from the table call instead of forward from
    # the axis call) as a cross-check, and catch any table whose axis
    # isn't the one currently listed in the XML.
    0x000127fc,   # TABLE_2D_000127FC_ISC (axis currently listed: 2d496)
    0x00012824,   # TABLE_2D_00012824_ISC (axis currently listed: 2d27a, Battery)
    0x0001284e,   # TABLE_2D_0001284E_ISC (axis currently listed: 2d186)
    0x00012858,   # TABLE_2D_00012858_ISC (axis currently listed: 2d2a2)
    0x00012860,   # TABLE_2D_00012860_ISC (axis currently listed: 2d2a2)
    0x00012950,   # TABLE_2D_00012950_ISC (axis currently listed: 2d200)
    0x00011e4a,   # TABLE_2D_00011E4A_TableLookup (axis currently listed: 2d136)
]

TABLE_BANK = 1
AXIS_BANK  = 2

# ------------------------------------------------------------------
# Helpers
# ------------------------------------------------------------------

def norm_offset(a):
    """Normalize a table/axis address to a bare CPU-page offset,
    stripping ROM_BASE if the caller passed a full Ghidra address."""
    if a >= ROM_BASE:
        return a - ROM_BASE
    return a

def addr(offset):
    return space.getAddress((ROM_BASE + offset) & 0xFFFFFFFF)

def containing_function(a):
    return fnMgr.getFunctionContaining(a)

def is_call_to(instr, target_offset):
    """True if instr is a pjsr/call whose resolved target is
    ROM_BASE + target_offset."""
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
    """Given the pjsr instruction itself, walk back up to 2 instructions
    to find the 'mov:g.w #imm:16,@-SP' / 'mov:g.w #bank:8,@-SP' pair
    immediately before it. Returns (bank, imm) or None if the pattern
    doesn't match cleanly (e.g. computed/indirect push, not a literal)."""
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
    """Scan the whole ROM's instruction stream for pjsr calls to
    TABLE_LOOKUP_INTERP or TABLE_3AXIS_INTERP_TRIPLE whose immediately-
    preceding literal push matches (TABLE_BANK, table_offset). Returns a
    list of (call_instr, is_3d) tuples."""
    results = []
    for target, is_3d in ((TABLE_LOOKUP_INTERP, False), (TABLE_3AXIS_INTERP_TRIPLE, True)):
        target_off = target - ROM_BASE if target >= ROM_BASE else target
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
            if bank == TABLE_BANK and imm == table_offset:
                results.append((call_instr, is_3d))
    return results

def find_preceding_axis_call(call_instr):
    """Walk backward instruction-by-instruction from call_instr (a known
    table_lookup_interp/table_3axis_interp_triple pjsr), within the same
    function only, looking for the nearest earlier pjsr to
    AXIS_LOOKUP_INTERP. Returns (axis_offset, distance_instructions) or
    None if not found within MAX_BACKWARD_WALK or before leaving the
    function's start address."""
    fn = containing_function(call_instr.getAddress())
    if fn is None:
        return None
    fn_start = fn.getEntryPoint()
    axis_target_off = AXIS_LOOKUP_INTERP - ROM_BASE if AXIS_LOOKUP_INTERP >= ROM_BASE else AXIS_LOOKUP_INTERP
    cur = call_instr.getPrevious()
    steps = 0
    while cur is not None and steps < MAX_BACKWARD_WALK:
        if cur.getAddress().getOffset() < fn_start.getOffset():
            break
        if is_call_to(cur, axis_target_off):
            pushed = read_preceding_push_pair(cur)
            if pushed is not None:
                bank, imm = pushed
                if bank == AXIS_BANK:
                    return (imm, steps)
        cur = cur.getPrevious()
        steps += 1
    return None

def trace_table(table_offset_raw):
    table_offset = norm_offset(table_offset_raw)
    call_sites = find_table_call_sites(table_offset)
    if not call_sites:
        return {
            "table": table_offset,
            "status": "NO-CALLER",
            "detail": "no table_lookup_interp/table_3axis_interp_triple call found "
                      "referencing this table via the literal-push convention -- "
                      "matches review5.md SEED #7d methodology; strongly suggests "
                      "this table isn't read as a 2D/3D lookup in this ROM revision "
                      "at all (verify with the raw literal-constant byte search per "
                      "review2.md Method step 4 before concluding dead code).",
            "pairings": [],
        }
    pairings = []
    any_paired = False
    for call_instr, is_3d in call_sites:
        fn = containing_function(call_instr.getAddress())
        fn_name = fn.getName() if fn is not None else "<no function>"
        axis_result = find_preceding_axis_call(call_instr)
        if axis_result is not None:
            axis_offset, distance = axis_result
            any_paired = True
            pairings.append({
                "caller_function": fn_name,
                "call_site": call_instr.getAddress().toString(),
                "table_kind": "3D" if is_3d else "2D",
                "axis_offset": "0x%05X" % (ROM_BASE + axis_offset),
                "instructions_back": distance,
            })
        else:
            pairings.append({
                "caller_function": fn_name,
                "call_site": call_instr.getAddress().toString(),
                "table_kind": "3D" if is_3d else "2D",
                "axis_offset": None,
                "instructions_back": None,
            })
    status = "PAIRED" if any_paired else "NO-AXIS-CALL"
    detail = (
        "found %d real caller(s); see per-caller breakdown" % len(call_sites)
        if any_paired else
        "found %d real caller(s) of this table, but none has a preceding "
        "axis_lookup_interp call within %d instructions in the same function -- "
        "this table's index source is set up elsewhere (different function, or "
        "a stale/previously-written RAM cell). Needs a human look at the "
        "specific call site(s) listed below rather than further automated search."
        % (len(call_sites), MAX_BACKWARD_WALK)
    )
    return {
        "table": table_offset,
        "status": status,
        "detail": detail,
        "pairings": pairings,
    }

# ------------------------------------------------------------------
# Main
# ------------------------------------------------------------------

def main():
    print("=" * 70)
    print("H8/539F Table-to-Axis Pairing Finder")
    print("=" * 70)
    results = []
    for t in TABLE_ADDRESSES:
        r = trace_table(t)
        results.append(r)
        print("")
        print("Table 0x%05X -> %s" % (ROM_BASE + r["table"], r["status"]))
        print("  %s" % r["detail"])
        for p in r["pairings"]:
            if p["axis_offset"] is not None:
                print("  [%s call in %s @ %s] axis = %s (%d instr back)" %
                      (p["table_kind"], p["caller_function"], p["call_site"],
                       p["axis_offset"], p["instructions_back"]))
            else:
                print("  [%s call in %s @ %s] NO PRECEDING AXIS CALL FOUND" %
                      (p["table_kind"], p["caller_function"], p["call_site"]))
    print("")
    print("=" * 70)
    n_paired = sum(1 for r in results if r["status"] == "PAIRED")
    n_noaxis = sum(1 for r in results if r["status"] == "NO-AXIS-CALL")
    n_nocaller = sum(1 for r in results if r["status"] == "NO-CALLER")
    print("Summary: %d PAIRED, %d NO-AXIS-CALL, %d NO-CALLER (of %d tables)" %
          (n_paired, n_noaxis, n_nocaller, len(results)))
    print("=" * 70)

main()

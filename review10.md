AXIS-PAIRING VERIFICATION: THE table+2 RAM-POINTER FAMILY AND CONVENTION B
============================================================================

GOAL: extend the caller-trace axis-pairing check (h8539_import_tables_xml2.py
verify_table_pairing -- PAIRED/NOT-PAIRED/UNVERIFIED) to also confirm axis
pairing for tables where no second literal is pushed at the call site, using
xrefs and/or address computation instead of the literal-push convention. This
file documents what was found and where the investigation currently stands.
Completed fixes are in CHANGELOG.md, not here.

--------------------------------------------------------------------
BACKGROUND: TWO DIFFERENT table_lookup_interp CALLING CONVENTIONS EXIST
--------------------------------------------------------------------
Confirmed via live disassembly of multiple real call sites (not just the
decompiler's abstracted C output, which hides the difference):

Convention A (two literals -- what verify_table_pairing already detects):
  mov:g.w #<axis_offset>:16,@-SP
  mov:g.w #<axis_bank>:8,@-SP
  pjsr @0x14735:24          ; axis_lookup_interp
  ...
  mov:g.w #<table_offset>:16,@-SP
  mov:g.w #<table_bank>:8,@-SP
  pjsr @0x14656:24          ; table_lookup_interp
Real example: isc_openloop_target_calc @ 0x17cb8, axis record pushed as
(bank=2, offset=0xd7ca).

Convention B (single literal -- NOT detected by the current script):
  mov:g.w #<table_offset>:16,@-SP
  mov:g.w #<table_bank>:8,@-SP
  pjsr @0x14656:24          ; table_lookup_interp, ONLY ONE arg
Real examples: f2e0_f2ec_composite_correction_calc @ 0x246ee (table 0x11e94,
call site 0x24804) and isc_f356_correction_calc @ 0x25c54 (table 0x127d8,
call site 0x25c61) -- confirmed via full decompile + raw disassembly, zero
axis_lookup_interp call anywhere in either function body. This is not a
MAX_BACKWARD_WALK window miss; the call genuinely does not exist.

For Convention B, table_lookup_interp itself reads the axis pointer out of
the table's own header struct in ROM, not from a caller-supplied argument:
  table[0]   = mode byte (0x02 = 1D path, 0x03 = 2D/two-axis path)
  table[1]   = correction/offset byte
  table+2    = ushort, the "value_ptr" field -- dereferenced to get the
               actual index/lookup value (e.g. current RPM/load), NOT a
               pointer to the axis breakpoint array as first assumed
  table+4    = second pointer field (2D/mode-0x03 path only)
  table+6    = stride byte (2D path only)
Verified against table_lookup_interp's decompile (entry 0x14656) and cross-
checked field-for-field against axis_lookup_interp's own, already-documented
struct comment (axis_lookup_interp @ 0x14735: value_ptr/axis_ptr/axis_count/
axis_table, same field-0 role).

Example header bytes read live (table 0x11e94): 02 00 F0 C8 FA F9 F9 F9 --
mode=0x02 (1D), correction=0x00, value_ptr=0xF0C8 (an on-chip RAM cell).

--------------------------------------------------------------------
DATASHEET CHECK: 0xF0C8-CLASS CELLS ARE PLAIN ON-CHIP RAM, NOT REGISTERS
--------------------------------------------------------------------
Checked against datasheets/grep able datasheets/H8_539F_hardware.md (mode 7,
single-chip -- the relevant mode for this ROM): on-chip RAM spans
H'EE80-H'FE7F; on-chip registers occupy H'FE80-H'FFFF separately. 0xF0C8
falls inside the plain-RAM range, not the register range. This rules out
"populated directly by dedicated I/O hardware with no corresponding store
instruction" -- a cell in this range can only ever be written by a real
mov-class instruction somewhere in the ROM, even though we have not found
it yet.

--------------------------------------------------------------------
FIXED (see CHANGELOG.md): read_axis_count() ROM_BASE double-counting bug
--------------------------------------------------------------------
h8539_audit_axis_data_length.py's read_axis_count() double-counted ROM_BASE
in the bank*0x10000+offset formula; fixed and verified three ways. Full
details moved to CHANGELOG.md.

STILL OPEN: h8539_import_tables_xml2.py's verify_table_pairing/
find_table_call_sites/find_preceding_axis_call never resolves a full axis
address at all -- it only compares the raw (bank, imm) tuple against
TABLE_BANK/AXIS_BANK constants (lines 492, 511) to decide whether a pairing
exists. So today, nothing in the pairing-check output reports an actual
resolved axis address, only whether a pairing was found. Adding that (for
the PAIRED case only, where a real (bank, imm) exists to resolve) would be
new work -- see NEXT STEPS item 3 below.

--------------------------------------------------------------------
WHAT THE bank*0x10000+offset FIX DOES NOT SOLVE: Convention B / NOT-PAIRED
--------------------------------------------------------------------
For NOT-PAIRED and single-literal-UNVERIFIED tables (Convention B above),
there is no caller-supplied (bank, offset) pair to feed the formula in the
first place -- find_preceding_axis_call finds nothing because there is
genuinely nothing there, confirmed by full manual decompile of two real
examples (f2e0_f2ec_composite_correction_calc, isc_f356_correction_calc),
not a search-window limitation. The fix does not and cannot change any
NOT-PAIRED/UNVERIFIED verdict in this bucket.

For this bucket, the only remaining path is resolving table+2's stored
RAM-cell value (e.g. 0xF0C8) to find what writes it at runtime. This is an
EP-banked SEGMENTOP read (project's own spSegment userop); the SLEIGH/p-code
side is confirmed working (review.md item 18), so the writer search is no
longer gated on an unfinished SLEIGH change -- see NEXT STEPS below.

Methods tried against 0xF0C8 specifically, all negative, all real checks
(not omissions):
  1. get_xrefs_to(0xF0C8) -- zero results, twice (once mistakenly attributed
     to a missing memory block, corrected: the RAM 0xEE80-0xFFFF block does
     cover 0xF0C8, confirmed via list_segments; the zero-result stands on
     its own merits).
  2. search_byte_patterns for raw bytes "F0 C8" anywhere in the ROM -- every
     hit lands inside a table header struct (a READER's stored value_ptr
     field), never inside a real instruction as a literal push. One
     apparent code hit (0x26cd8) was checked and confirmed a false positive
     (mid-instruction byte coincidence inside bset.w @0xf3f0:16,0x8's own
     encoding at 0x26cd6 -- same false-positive class already documented
     project-wide for "EE 90" inside unrelated immediates).
  3. find_code_gaps() near the one real TCU-serial-byte handler
     (sci3_rx_process_tcu_byte @ 0x2a8d3) -- the 80-byte gap immediately
     after it (0x2a930-0x2a97f) is pure 0xFF padding, no hidden function.
     sci3_rx_process_tcu_byte's own decompile touches only 0xf39e/0xf3d0/
     0xf3ce -- nowhere near 0xF0C8. No support found for "written directly
     via serial from the TCU."

--------------------------------------------------------------------
NEXT STEPS
--------------------------------------------------------------------
1. Step 3b (EP-banked SEGMENTOP) is confirmed working at the SLEIGH/p-code
   level (review.md item 18) -- not a blocker anymore. Re-run the
   analyze_dataflow trace on 0x14665 and 0xF0C8-class reads across all
   NOT-PAIRED/single-literal-UNVERIFIED tables to confirm this actually
   unblocks the writer search via normal xref methods on the resolved
   address; item 18's fix was tested against a different function
   (flash_vpp_pulse_handshake_echo_verify), not this table family
   specifically.
2. Re-run h8539_audit_axis_data_length.py's Step 4/5 now that read_axis_count
   is fixed; diff against any previously-saved audit output to see which
   mismatch verdicts change.
3. Consider adding resolved-axis-address reporting to
   h8539_import_tables_xml2.py's verify_table_pairing for the PAIRED case
   only (using the now-fixed bank*0x10000+offset formula).

--------------------------------------------------------------------
FOLLOW-UP SESSION: h8539_find_convention_b_writers.py, A DIRECT-MODE
WRITER SEARCH ACROSS ALL 79 NOT-PAIRED/UNVERIFIED TABLES -- NO DP-RELATIVE
WRITER EXISTS, CONFIRMED PROJECT-WIDE, NOT TABLE-BY-TABLE
--------------------------------------------------------------------
Built a new script, h8539_find_convention_b_writers.py, to test one
untried angle against the whole NOT-PAIRED (31 tables) + PAIRING-
UNVERIFIED (48 tables) bookmark buckets = 79 unique table headers total.
The angle: all three static methods logged above (get_xrefs_to, byte-
pattern search, code-gap search) searched for a value_ptr RAM cell as an
IMMEDIATE VALUE being pushed/referenced. None specifically searched the
disassembly text for the RAM cell appearing as a DESTINATION memory
operand of a direct-mode (DP-relative, not EP-banked) write instruction.
A write using DP (already confirmed working per the step 1/2 notes above)
rather than EP would be invisible to all three prior methods but visible
to a plain "does any mov-family instruction's final operand mention this
address, prefixed with @" text scan.

RESULT: ran across all 79 candidates. Zero direct-mode writer candidates
found for any of them. This is a stronger and more useful negative than
the original two-table check -- it confirms the EP-banked SEGMENTOP path
is used UNIFORMLY across this entire table family for value_ptr writes,
not inconsistently. There is no subset of NOT-PAIRED/UNVERIFIED tables
that can be unblocked table-by-table via a DP-relative writer; the writer
for all 79 goes through the EP-banked SEGMENTOP path, not a per-table
DP-relative store.

WHAT THE SCRIPT DID SUCCEED AT (side effect, not the main goal): created
real header+2 -> value_ptr xrefs for ~74 of the 79 tables (the ones whose
value_ptr resolves inside a defined memory block), turning previously-
invisible-to-xref-search RAM pointers into first-class Ghidra references.
These are correct and safe -- value_ptr is a plain ROM-stored ushort, no
EP/segmentation involved in reading it, only in finding its WRITER.

FIVE TABLES FLAGGED AS SUSPECT, NOT GENUINE CONVENTION B CANDIDATES --
value_ptr fell outside any defined memory block for 5 of the 79. Full
bookmark export cross-checked against the run output; all 79
WRITER-NOT-FOUND bookmarks confirmed present and consistent with the
console summary. Named/labeled identities for the 5 suspects, pulled
from the bookmark export's existing Ghidra labels:
  0x12955  TABLE_2D_00012950_ISC              value_ptr=0xC00E  mode=0x00
  0x12AEB  TABLE_2D_00012AE6                  value_ptr=0xC480  mode=0x00
  0x12D75  TABLE_2D_00012D70_O2Load           value_ptr=0xC0F0  mode=0x00
  0x11F04  Decel_Fuel_Cut_Resume_Enrich       value_ptr=0x02C0  mode=0x02
  (0x12955/0x12AEB/0x12D75 share mode=0x00 and correction=0xF0 -- same
  suspect pattern across all three, worth checking as a group rather than
  independently; 0x11F04 is the odd one out, mode=0x02 but value_ptr
  still resolves off-chip)
Three of these (all mode=0x00, correction=0xF0) are OUTSIDE the
documented header layout (mode byte should be 0x02 for 1D or 0x03 for
2D/two-axis, per the BACKGROUND section above) -- strong signal these
either aren't real 2D/3D lookup tables at all, or this bookmarked address
isn't actually a table header (misaligned/wrong address), not that
value_ptr genuinely points off-chip. Decel_Fuel_Cut_Resume_Enrich
(0x11F04) has a plausible mode byte (0x02) but correction=0xC0 and
value_ptr=0x02C0 -- also worth a manual header-alignment check, since
0x02C0 doesn't look like a RAM address in this program's memory map
either. NOT yet manually reviewed -- flagged here so they don't get
silently treated as "just more blocked Convention B tables" in any
future summary. A mode-value sanity check (skip/flag anything outside
{0x02, 0x03} under its own bookmark category) is a planned but not yet
applied script improvement.

NOTABLE NAMED TABLES in the genuine Convention B (real mode 0x02/0x03
header) set worth prioritizing for the writer search in NEXT STEPS item 1 --
these already have meaningful XML/ECU-relevant names rather than generic
TABLE_2D_/BYTE_ARRAY_ placeholders, so they're likely higher-value targets
for axis confirmation:
  0x128F6  Decel_Fuel_Cut_Delay_Neutral_A_C_Off   value_ptr=0xF0C0
  0x12902  Decel_Fuel_Cut_Delay_In_Gear_A_C_Off   value_ptr=0xF0C0
  0x1290E  Decel_Fuel_Cut_Delay_Neutral_A_C_On    value_ptr=0xF0C0
  0x1291A  Decel_Fuel_Cut_Delay_In_Gear_A_C_On    value_ptr=0xF0C0
  0x11F6C  TABLE_2D_00011F6C_possible_Closed_Loop_Eligibility_Check
           value_ptr=0xF0C0


TOOL NOTES for h8539_find_convention_b_writers.py itself (useful if
picking this back up):
  - TABLE_HEADER_ADDRESSES is populated automatically from the
    H8539F-TABLE-NOT-PAIRED and H8539F-TABLE-PAIRING-UNVERIFIED bookmark
    categories (both left by h8539_import_tables_xml2.py) rather than a
    hardcoded list -- safe to re-run any time those categories change.
  - Self-healing: an early version of this script had the SAME ROM_BASE
    double-counting bug documented above for
    h8539_audit_axis_data_length.py's read_axis_count() (computed
    addr(value_ptr), which adds ROM_BASE a second time, instead of the
    bare RAM address). Fixed, and every run now also removes any stale
    bad xref left by the old buggy version before creating the correct
    one -- confirmed via get_xrefs_from that the bad 0001xxxx-prefixed
    xrefs are gone and only the correct 0000xxxx-prefixed ones remain,
    for both the originally-affected tables (0x11e94, 0x127d8).
  - Writer-candidate hits (if any future run finds some) are reported as
    text-match CANDIDATES only, never auto-converted to xrefs -- same
    manual-confirmation discipline as h8539_find_table_axis.py and
    h8539_find_axis_consumer.py, and for the same reason: a text match on
    an operand could still be a false positive of the same class already
    documented above ("EE 90" mid-instruction byte coincidence).

ADDITIONAL NEXT STEPS from this session:
  - Manually review the 5 mode=0x00/off-chip-value_ptr tables listed
    above before assuming they're just more blocked Convention B cases --
    they may be misclassified NOT-PAIRED/UNVERIFIED entries, wrong header
    addresses, or a different table type entirely.
  - NEW (optional): add a mode-value sanity check to
    h8539_find_convention_b_writers.py so future runs separate "genuine
    blocked Convention B table" from "header address looks wrong" instead
    of lumping both into one bookmark category.


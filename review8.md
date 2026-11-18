COMPUTED-CALL STATE-TABLE DISPATCH: f502_state_table_dispatch AND ITS
UNRESOLVED HANDLER TARGETS
====================================================================

GOAL: review6.md's zero-caller cohort sweep (17 functions, confirmed via
get_xrefs_to with zero references of any kind) was correct given what it
checked -- but "zero references Ghidra can see" is not the same as "zero
callers." This file documents a real, confirmed computed-call dispatch
mechanism that Ghidra's static xref analysis cannot see through, which
explains at least 4 of those 17 "dead" functions and additionally surfaces
16 more handler targets that are not even disassembled as functions yet.

--------------------------------------------------------------------
THE MECHANISM (confirmed, not guessed -- cross-checked against an existing
Ghidra bookmark at 0x27ed4/0x136e2 plus independent byte-level decode)
--------------------------------------------------------------------
f502_state_table_dispatch (0x27e9c) reads a state byte from RAM at 0xf502,
splits on `cmp:g.b #0xc,@0xf502` (0x27eb2) into two contiguous sub-tables,
computes `mulxu.w #6,Rn` to index by 6-byte record, and calls the resolved
handler via `pjsr @R0` at 0x27ed4 -- a genuine computed indirect SUBROUTINE
call, not a jump table Ghidra's own jump-table recovery would catch.

Two sub-tables:
  - state < 12: base 0x1369a, 12 records (states 0-11)
  - state >= 12: base 0x136e2, 9 records (states 12-20)

Record format, 6 bytes: [flags:16][bank:8, constant 0x02][pad:8, constant
0x00][handler-offset:16]. Real target address = bank<<16 | offset.

Companion function f502_state_index_update (0x27f60) almost certainly
writes the 0xf502 state byte that drives this dispatch -- not yet
investigated in this file, worth a look when picking this back up.

--------------------------------------------------------------------
FULL DECODED TABLE (both sub-tables, all 21 states, verified against the
live function list 2026-08-09)
--------------------------------------------------------------------
State < 12 (base 0x1369a):
  state 0  flags=0002  -> 0x27fb1  f17a_load_zone_and_ef96_f1d8_check   [FN EXISTS, 0 xrefs]
  state 1  flags=0040  -> 0x27fea  [NOT A FUNCTION -- undisassembled]
  state 2  flags=0080  -> 0x2800a  phase_dispatch_efa2_f500_f5ee_gate_check [FN EXISTS, 0 xrefs]
  state 3  flags=0101  -> 0x28038  FUN_00028038                          [FN EXISTS, 0 xrefs, unnamed]
  state 4  flags=0108  -> 0x28082  f17a_f13c_load_zone_and_f514_gate_check [FN EXISTS, 0 xrefs]
  state 5  flags=0140  -> 0x280be  [NOT A FUNCTION -- undisassembled]
  state 6  flags=0180  -> 0x280d3  [NOT A FUNCTION -- undisassembled]
  state 7  flags=0201  -> 0x280d8  [NOT A FUNCTION -- undisassembled]
  state 8  flags=0020  -> 0x280dd  [NOT A FUNCTION -- undisassembled]
  state 9  flags=0110  -> 0x28151  [NOT A FUNCTION -- undisassembled]
  state 10 flags=0120  -> 0x28253  [NOT A FUNCTION -- undisassembled]
  state 11 flags=0301  -> 0x28258  [NOT A FUNCTION -- undisassembled]

State >= 12 (base 0x136e2, index adjusted via adds.b 0xb8,R2 at 0x27ec2):
  state 12 flags=0002  -> 0x27fe5  [NOT A FUNCTION -- undisassembled]
  state 13 flags=0104  -> 0x2816c  [NOT A FUNCTION -- undisassembled]
  state 14 flags=0004  -> 0x28192  [NOT A FUNCTION -- undisassembled]
  state 15 flags=0008  -> 0x281a1  [NOT A FUNCTION -- undisassembled]
  state 16 flags=0102  -> 0x281b6  [NOT A FUNCTION -- undisassembled]
  state 17 flags=0204  -> 0x281e4  [NOT A FUNCTION -- undisassembled]
  state 18 flags=0208  -> 0x28210  [NOT A FUNCTION -- undisassembled]
  state 19 flags=0210  -> 0x28215  [NOT A FUNCTION -- undisassembled]
  state 20 flags=0220  -> 0x28234  [NOT A FUNCTION -- undisassembled]

VERIFICATION METHOD: independently byte-decoded both sub-tables directly
from ROM memory (ghidra:read_memory at 0x13690-0x136e2 and cross-checked
part 2 against the existing bookmark's own pasted per-state list at
0x136e2) -- the two sources agree on every value checked. Function-name
lookup via search_functions_enhanced sorted by address confirms the
function list jumps from f17a_f13c_load_zone_and_f514_gate_check (0x28082)
directly to eee4_eeda_bit6_update_from_f0f8_bit12 (0x28279) with NOTHING
in between -- all 16 of the states-1/5-20 targets sit in that gap as raw
undisassembled bytes, not functions.

--------------------------------------------------------------------
IMPACT ON PRIOR FILES
--------------------------------------------------------------------
review6.md's zero-caller-cohort conclusion for these 4 functions needs a
footnote, not a retraction: f17a_load_zone_and_ef96_f1d8_check,
phase_dispatch_efa2_f500_f5ee_gate_check, FUN_00028038, and
f17a_f13c_load_zone_and_f514_gate_check are NOT dead code. They are real
handlers reached only via f502_state_table_dispatch's computed pjsr, which
review6.md's get_xrefs_to sweep could not see (computed calls don't
register as static references the way direct calls or resolved jump
tables do). The other 13 functions in that cohort are unaffected by this
finding -- no evidence yet ties them to this or any other computed-call
mechanism, they remain correctly classified as dead pending a similar
check.

--------------------------------------------------------------------
PLAN: H8StateTableDispatchAnalyzer
--------------------------------------------------------------------
New analyzer, same conflict-safe/evidence-only posture as
H8FramePointerParamAnalyzer and H8FunctionPurgeAnalyzer (see those files'
own doc comments for the house style this follows):

1. Locate dispatch sites matching the confirmed idiom: a `cmp:g.b` against
   a RAM state byte, immediately followed by a `mulxu.w #0x6,Rn`
   size-6-record index computation, followed by a `pjsr @Rn`. Start from
   the two known, confirmed sites (0x27e9c region / table bases 0x1369a
   and 0x136e2) rather than a blind ROM-wide pattern scan for the first
   version -- generalize to a wider scan only after this known-good case
   is handled correctly end to end.
2. For each table, parse every 6-byte record: flags (2 bytes, informational
   only, not a param -- do not confuse with H8FramePointerParamAnalyzer's
   FP-offset params), bank (1 byte, expect constant 0x02 -- log and skip
   the record if it's ever anything else, do not guess), pad (1 byte,
   expect 0x00), offset (2 bytes). Real target = bank<<16 | offset.
3. For each resolved target address:
   a. If no Function exists there yet, disassemble at that address and
      create a Function (mirrors what a human would do via "Create
      Function" in the listing -- this is mechanical, the address is
      already unambiguous real evidence, not a guess).
   b. Add a COMPUTED_CALL reference from the pjsr instruction to the
      target, so Ghidra's own xref graph -- and any later
      H8FramePointerParamAnalyzer pass over these newly-defined functions
      -- sees a real caller instead of zero.
4. Log every record decoded and every action taken (function created /
   already existed / reference added / record skipped and why), same
   logging discipline as the two existing analyzers.
5. Explicitly OUT OF SCOPE for v1: general ROM-wide pattern scanning for
   other instances of this same dispatch idiom (flagged as a possible v2
   once this known case is proven correct); retyping the newly-created
   functions' return types or parameters (that's H8FramePointerParamAnalyzer's
   and review4/6.md's job, run those AFTER this analyzer, not instead of
   it); the flags field's meaning (informational bitmask, not touched).

--------------------------------------------------------------------
RUNNING LOG
--------------------------------------------------------------------
2026-08-09: File created. Discovered while investigating the person's
hypothesis that many "dead" zero-caller functions might actually run via
a background scheduler/interrupt/GPIO mechanism not visible to static
xref analysis. Confirmed correct -- found and fully decoded the
f502_state_table_dispatch computed-call mechanism (cross-checked against
an existing Ghidra bookmark, not a fresh guess), identified 4 previously
"dead" functions as real handlers, and surfaced 16 additional handler
targets with no Function defined at all. Scoped H8StateTableDispatchAnalyzer
to fix both problems (missing functions + missing references) for this
specific, fully-verified dispatch site. NEXT STEP: implement the analyzer
(H8StateTableDispatchAnalyzer.java), starting from the known table
addresses rather than a general pattern scan, per the PLAN section above.
Also flagged for later, not yet done: check f502_state_index_update
(0x27f60) to understand what actually drives state selection, and re-check
the other 13 zero-caller-cohort functions from review6.md against this or
any other computed-call mechanism before treating them as settled dead
code.
2026-08-09 (session 2): Implemented H8StateTableDispatchAnalyzer.java (see
that file's own doc comment for full detail). Scoped strictly to the two
known/verified table addresses per the PLAN above, not a general pattern
scan. Compiled successfully via javac against every jar under
ghidra_12.0.4_PUBLIC (exit code 0; a handful of unrelated bad-classpath-
element warnings for jars that don't exist in this 12.0.4 layout, and the
same [this-escape] setPriority()-in-constructor info warning the other
three analyzers already produce -- neither is a real problem). Compiled
alongside the other three analyzers in one pass to confirm nothing broke.
H8StateTableDispatchAnalyzer.class now sits in h8/build/classes/h8539f/
next to the other three .class files. NOT YET RUN inside Ghidra itself --
compiling is not the same as verifying correct runtime behavior.
NEXT STEP: run it against the RVR md352553 program (via Ghidra's Analysis
> One Shot menu, or by re-running full auto-analysis with it enabled) and
verify: (a) all 16 missing functions get created, (b) all 21 COMPUTED_CALL
references land correctly, (c) the 4 already-existing functions pick up
their new reference without any other change, (d) re-run
H8FramePointerParamAnalyzer afterward to see if the 16 newly-created
functions get real FP-relative parameter storage now that they're
reachable, which would be the real end-to-end validation of this whole
investigation.

2026-08-09 (session 2 cont.): CORRECTION -- compiling into the git repo's
own h8/build/classes was not sufficient. The live module Ghidra actually
loads analyzers from is C:\Users\j.brophy.CORKILLSYSTEMS\Downloads\
ghidra_12.0.4_PUBLIC\Ghidra\Processors\h8\lib\h8539f-analyzers.jar -- a
jar file, not loose .class files, and a completely separate location from
the git repo (this is why the git repo's build/classes dir existing at
all was a bit of a red herring; it's not what Ghidra reads). Backed up the
live jar to h8539f-analyzers.jar.bak-20260809-134432 (matching the backup
naming convention already present in that lib folder from prior sessions),
then rebuilt it from the git repo's freshly-compiled build/classes/h8539f
directory (all 4 source files compiled together in one javac pass, so
internally consistent) via `jar cf h8539f-analyzers.jar h8539f`. New jar
verified to contain all 5 class files: the 4 pre-existing ones plus
H8StateTableDispatchAnalyzer.class. Ghidra was NOT running during this
swap (CodeBrowser screenshot showing the missing analyzer was from before
this fix). NEXT STEP: restart Ghidra, reopen the RVR md352553 program, and
check Analysis Options for "H8 State Table Dispatch Analyzer" -- if it now
appears, run it and verify per the checklist two log entries up.

2026-08-09 (session 2 cont., BUG FOUND AND FIXED): Analyzer ran successfully
this time (confirmed via Ghidra log) but did nothing useful: all 21 records
failed the bank-byte check and were skipped, 0 functions created, 0
references added -- decompile output correctly reported as identical to
before, since nothing changed. Root cause: decodeRecord() had the pad and
bank byte positions swapped. My own manual byte trace earlier in this file
(record bytes "00 02 00 02 7f b1" -> address bytes are raw[3],raw[4],raw[5]
= 02,7f,b1, meaning bank=raw[3]) was correct, but when writing the Java I
transcribed it backwards as bank=raw[2], pad=raw[3] -- the actual layout is
[flags:16][pad:8][bank:8][offset:16], not [flags:16][bank:8][pad:8]
[offset:16] as both this file and the class doc comment originally stated.
Corrected in H8StateTableDispatchAnalyzer.java (decodeRecord swapped to
pad=raw[2], bank=raw[3]) and in both doc comments that described the
layout. Recompiled (exit 0) and redeployed to the live jar, backed up as
h8539f-analyzers.jar.bak-20260809-134936. NEXT STEP: restart Ghidra again,
re-run analysis, and this time expect real output -- 16 functions created,
21 COMPUTED_CALL references added (or fewer if some records still don't
match for a different, not-yet-seen reason -- watch the log closely rather
than assuming success). If any records STILL fail the bank check after
this fix, stop and re-derive the byte layout from scratch with a fresh
read_memory dump rather than patching the check further -- two wrong
transcriptions in a row would mean the manual derivation process itself
needs redoing, not another one-line fix.

2026-08-09 (session 2, SUCCESS -- end-to-end validated): Re-ran after the
pad/bank fix. Decompile output for all 21 targets now shows real function
bodies (previously these 16 were raw undisassembled bytes, and the other
4 existed but were unreachable per Ghidra's xref graph). All 21 handlers
follow the same shape: tri-state return (0/1/2, proceed/hold/deny-style),
reading the same status-word cluster already well-documented elsewhere in
this ROM (DAT_0001f1fe, DAT_0001efa2, g_status_flags_f0f8,
g_status_flags_f20e, DAT_0001f200, etc.) -- confirms
f502_state_table_dispatch is a genuine condition-gate state machine
consulted by whatever writes/reads state 0xf502, not dead weight. Still
unnamed (FUN_0002xxxx) -- naming these based on the flags each guards is
a reasonable follow-up but NOT done automatically by this analyzer (out
of scope per the SAFETY/SCOPE section -- naming requires understanding
what each flag means, which is human judgement, not mechanical
derivation). H8StateTableDispatchAnalyzer considered DONE for its stated
v1 scope: functions created, references added, mechanism fully explained.
REMAINING FOLLOW-UPS (not started):
  1. Name the 16 newly-created functions based on what each guards, once
     their flag meanings are understood (e.g. FUN_00027fea reads
     g_status_flags_f0f8 bit 6 and DAT_0001efa2, similar shape to its
     neighbor phase_dispatch_efa2_f500_f5ee_gate_check -- may be a related
     zone/phase check worth a matching name).
  2. Re-run H8FramePointerParamAnalyzer now that these 16 are real,
     reachable functions -- none of them take FP-relative parameters
     (all are `(void)` per the decompile shown), so no new CUSTOM_STORAGE
     signatures are expected from that pass for this batch, but worth
     confirming rather than assuming.
  3. review6.md's zero-caller-cohort verdict for the 4 previously-existing
     functions (f17a_load_zone_and_ef96_f1d8_check,
     phase_dispatch_efa2_f500_f5ee_gate_check, FUN_00028038 (now
     resolved, still unnamed),f17a_f13c_load_zone_and_f514_gate_check)
     should be formally corrected there, not just noted here.
  4. f502_state_index_update (0x27f60) still not investigated -- would
     explain what actually drives state selection and complete the
     picture of this subsystem.
  5. The other 13 functions in review6.md's original 17-function
     zero-caller cohort remain unaddressed -- no evidence yet ties them
     to this or any other computed-call mechanism.

2026-08-09 (session 2, NAMING PASS COMPLETE): Renamed and documented all
18 unnamed functions in the state table (17 created by
H8StateTableDispatchAnalyzer + the 1 pre-existing FUN_00028038 that just
never had a name). Names follow the project's existing convention
(f<addr>_<condition>_gate_check / _stub), derived directly from each
function's actual reads/writes -- not guessed. Each got a plate comment
describing its condition and, where relevant, cross-references to sibling
handlers sharing the same status word (DAT_0001f1fe in particular is
checked by 3 different states: 9, 14, 15 -- worth treating as a real
bitfield with known bit meanings if this subsystem gets revisited).
Final name list:
  0x27fe5  f502_state12_const_deny2_stub
  0x27fea  f0f8_bit6_efa2_phase_gate_check
  0x28038  f1ec_torque_f17a_multi_zone_gate_check  (was already a function,
           just unnamed -- only this one predates the analyzer)
  0x280be  f340_bit0_clear_check
  0x280d3  f502_state6_const_hold1_stub
  0x280d8  f502_state7_const_hold1_stub
  0x280dd  f1fe_f00c_timer_calc_gate_check  (largest/most complex, 116
           bytes -- writes DAT_0001f00c, a likely timer/count reload)
  0x28151  f1fe_bit5_clear_reset_f09a_check
  0x2816c  f1f2_bit7_f3a4_bit10_gate_check
  0x28192  f1fe_bit1_clear_check
  0x281a1  f1fe_bit6_clear_check
  0x281b6  ef92_f12e_range_gate_check
  0x281e4  flag102ea_f4d6_bit2_gate_check
  0x28210  f502_state18_const_hold1_stub
  0x28215  f1f2_bit13_f4ae_mask0a_gate_check
  0x28234  f1f2_bit13_f4ae_mask05_gate_check  (near-identical to state 19,
           different mask on same DAT_0001f4ae byte)
  0x28253  f502_state10_const_hold1_stub
  0x28258  f1f4_eedc_f202_gate_check
No parameter/return typing changes were needed or made: all 18 are (void)
with no FP-relative parameters (confirmed in the earlier decompile dump),
so there is nothing for H8FramePointerParamAnalyzer to do for this batch,
and Ghidra's own inferred return types (undefined2/bool) already match
the convention used by the 3 pre-existing named siblings, so those were
left alone rather than force a more specific type not actually evidenced
by the decompile.
REMAINING FOLLOW-UPS (updated):
  1. DONE -- naming (this entry).
  2. Confirmed not needed -- no FP params in this batch, nothing for
     H8FramePointerParamAnalyzer to add.
  3. review6.md's zero-caller-cohort verdict still needs formal correction
     there (not done yet -- only noted in this file so far).
  4. f502_state_index_update (0x27f60) still not investigated.
  5. The other 13 functions in review6.md's original 17-function
     zero-caller cohort remain unaddressed.
  6. NEW: DAT_0001f1fe is checked by 3 of these handlers (states 9, 14,
     15) plus written by state 8 -- worth formally documenting as a
     bitfield if a future session wants to push this further.

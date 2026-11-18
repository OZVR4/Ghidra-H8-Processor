CALL-SITE CONCAT/EXTRAOUT ARTIFACT AUDIT
function bodies still showing CONCAT/in_stack/extraout noise despite
correct return types
====================================================================

GOAL: review4.md and review6.md covered return-TYPE correctness (undefined
-> void/ushort/byte etc, ~95 functions total, both fully closed out). This
file covers a DIFFERENT, narrower defect: functions that already have (or
after review4/6 now have) a real, correctly-resolved return type, but whose
DECOMPILED BODY still contains decompiler-manufactured noise around call
sites to other already-typed functions -- specifically CONCAT11/CONCAT22
wrapping an extraout_var padding byte around a real captured value. This is
NOT a return-type bug (the callee is already typed correctly) and NOT
always a stack-parameter bug (in_stack_* is review6.md's territory when it
affects a function's OWN signature) -- it is the decompiler failing to
collapse `(undefined1)true_result` cleanly into the caller's own use of
that value, even when the callee's prototype is already right.

Scope is intentionally narrower than a blanket CONCAT/in_stack/extraout
sweep across the whole ROM -- those raw counts (92 CONCAT, 214 in_stack_*,
419 extraout_* occurrences file-wide) are mostly NOT bugs:
  - CONCAT usage inside the sat_math/mul/div primitive helpers (lines
    ~2100-3100: sat_add_u16, sat_sub_u16, sat_mul_u16, mul_u16_x2_or_
    sum_check_overflow, muldiv_s16_rounded_3op, etc) is genuine correct
    arithmetic -- these functions manually assemble 32-bit intermediate
    values from 16-bit halves on purpose. Do not touch these.
  - in_stack_* appearing in a function's OWN undefined-return investigation
    is review6.md's method (stack/return-overlap artifact, cause for
    skipping a retype) -- do not duplicate that work here.
  - extraout_R2/extraout_R1 register-propagation from a bare/tail call is
    already the documented review4/6 pattern for return-type verdicts --
    also not this file's job.

THIS file's scope: the CONCAT11(extraout_var, <callee_result>) idiom
specifically, where <callee_result> comes from a call to a function that
ALREADY has a resolved, non-undefined return type (bool/ushort/byte/etc).
Confirmed via ghidra:force_decompile on the first case
(isc_f0e6_bit0_update) that this is NOT a stale-decompiler-cache artifact
-- it persists after a forced re-decompile, so it's a real quirk in how
the current H8 SLEIGH/calling-convention model represents these call
sites, worth understanding properly rather than papering over.

--------------------------------------------------------------------
METHOD
--------------------------------------------------------------------
1. For each function below, decompile it and confirm the callee at the
   CONCAT11(extraout_var, X) site already has a real (non-undefined)
   return type -- if not, that's actually a review4/6-class return-type
   gap that surfaced late; handle it with THAT method instead (real
   caller capture/drop evidence), not this file's.
2. If the callee IS already correctly typed, the question is whether the
   CONCAT is: (a) cosmetically harmless -- the comparison/branch logic is
   still correct despite the ugly wrapper, just a readability issue safe
   to leave or annotate; or (b) actually misleading/wrong -- e.g. if
   extraout_var could carry garbage that changes the comparison result in
   some path. Case (b) would be a real correctness concern, not just
   cosmetics -- check the specific comparison (==0, !=0, etc) against
   what the callee guarantees about its full return register, not just
   the byte decompiler chose to name.
3. Where useful, cross-check with raw disassembly (disassemble_function)
   rather than trusting the decompile pseudocode alone, same discipline
   already established elsewhere in this project (see the
   o2_closed_loop_fuel_trim_compute existing comment: "Decompiler output
   for this function is currently untrustworthy... use disassembly for
   further analysis here, not decompile_function").
4. Record verdict per function: [B] BENIGN (cosmetic only, comparison
   logic unaffected, safe to leave or just comment) / [C] CONCERNING
   (comparison logic could be affected by extraout_var, needs a real fix
   or at minimum a flag) / [X] NOT THIS FILE'S BUG (callee turned out to
   still be untyped, or turned out to be a genuine arithmetic CONCAT) /
   [ ] not yet checked.
5. Do not blind-edit decompiler output. Any actual fix (e.g. a decompiler
   comment, a calling-convention adjustment) needs the same real-evidence
   discipline as review4.md/review6.md -- this file's job is diagnosis
   first, only fix what's demonstrably safe.

--------------------------------------------------------------------
KNOWN PRIOR CONTEXT (do not re-derive)
--------------------------------------------------------------------
- o2_closed_loop_fuel_trim_compute (line ~17204) already has an existing
  comment flagging its decompile as untrustworthy (CONCAT12/ZEXT24
  garbage) with an explicit instruction to use disassembly instead of
  decompile_function for this one. Its CONCAT11(extraout_var,bVar1) hit
  is downstream of that same known issue -- do not treat as a fresh
  finding, fold into the existing untrustworthy-decompile note instead.
- tcu_shift_ratio_buffer_update (0x2c16e) has an existing "clean
  disassembly, no decompiler artifacts - verified against raw
  disassemble_function output" comment, yet its own body contains
  `sVar2 = CONCAT11(extraout_var,1)` -- this is a real discrepancy
  between that comment's claim and the current decompile. Needs
  resolving: either the comment's "verified" claim needs updating, or
  this specific CONCAT is confirmed benign (sign/flag construction, not
  a real value corruption) and the comment's overall claim still holds.
  Check via disassembly directly, do not just trust either the comment
  or the pseudocode.
- tcu_shift_torque_and_knock_mgmt (0x28fff) is itself STILL undefined
  return type -- it did not appear in review6.md's original 80-function
  regex sweep or its zero-caller cohort, meaning it's a genuine gap in
  that sweep's coverage, not something deliberately skipped. Flag for a
  return-type check using review4/6's method (real caller capture/drop),
  separate from this file's CONCAT investigation -- do not conflate the
  two problems for this function.

--------------------------------------------------------------------
CHECKLIST (11 functions, found via grep for `CONCAT11\(extraout_var,`
against the current decompile export, 2026-08-09)
--------------------------------------------------------------------
[B] isc_f0e6_bit0_update (line ~7659) -- CHECKED 2026-08-09: callee
    knock_condition_eval confirmed bool __stdcall, real type, not a
    return-type gap. Disassembly of the callee's final branch shows
    `mov:i` (immediate move) setting the full return register to 0 or 1
    right before `prts` -- no partial-byte/leftover-garbage path exists,
    so extraout_var is always effectively 0 regardless of which branch
    was taken. CONCAT11(extraout_var,bVar2)==0 is behaviorally identical
    to bVar2==0 for both possible real values. BENIGN: cosmetic
    decompiler noise only, comparison logic is correct as compiled, no
    fix needed.
[B] f0ea_bit6_update_from_mode_check (line ~8684) -- CHECKED 2026-08-09:
    callee engine_mode_f512_bit4_check confirmed bool __stdcall, real
    type. Disassembly shows `mov:i` on the true path and `clr.w` (full
    word clear, not byte-only) on the false path -- both paths fully
    define the return register, extraout_var carries no real garbage.
    Same shape as isc_f0e6_bit0_update above. BENIGN.
[B] egr_f0e6_bit1_update (line ~8763) -- CHECKED 2026-08-09: callee
    egr_activity_condition_check confirmed bool __stdcall, real type.
    Same mov:i/clr.w full-register shape as the two functions above.
    BENIGN.
[B] f0ea_bit7_rpm_zone_update (line ~9829) -- CHECKED 2026-08-09: callee
    rpm_zone_enable_check confirmed bool __stdcall, real type. Same
    mov:i/clr.w full-register shape before prts. BENIGN.
[B] lambda_enable_flag_update (line ~14469) -- CHECKED 2026-08-09: callee
    lambda_enable_inhibit_check confirmed real type (uchar, not bool, but
    same mov:i/clr.w full-register disassembly shape as the bool cases
    above -- conclusion is the same regardless of the declared C type).
    BENIGN. SEPARATE FINDING (not this file's scope, flagging in passing):
    lambda_enable_flag_update's OWN return type is still undefined --
    another review6.md sweep gap alongside tcu_shift_torque_and_knock_mgmt,
    see KNOWN PRIOR CONTEXT. Needs its own review4/6-style check.
[B] f20e_f210_clear_chain_and_o2_closedloop_gate (line ~16398) -- CHECKED
    2026-08-09: callee o2_closed_loop_config_check confirmed bool
    __stdcall, real type, mov:i/clr.w full-register shape. BENIGN. See
    consolidated note below the third caller for the cross-check across
    all three callers of this same callee.
[B] o2_closed_loop_fuel_trim_compute (line ~17204) -- CHECKED 2026-08-09:
    callee o2_downstream_type34_check confirmed bool __stdcall, same
    clean mov:i/clr.w shape as every other case this session -- this
    SPECIFIC CONCAT11(extraout_var,bVar1) call site is BENIGN. Does NOT
    extend to the rest of this function's decompile: the existing
    comment's "untrustworthy... CONCAT12/ZEXT24 garbage, use disassembly
    not decompile_function" warning covers OTHER parts of this function's
    body (stack-var-as-address artifacts) unrelated to this specific
    call site, and that warning still stands. Leaving the existing
    caution in place -- this finding only clears the one CONCAT this
    file was checking, not the function as a whole.
[B] f26e_o2_closed_loop_output_blend_via_table (line ~17627) -- CHECKED
    2026-08-09: same callee o2_closed_loop_config_check, same shape.
    BENIGN.
[B] eeac_adaptive_trim_learn_update_via_table (line ~17880) -- CHECKED
    2026-08-09: same callee o2_closed_loop_config_check, same shape.
    BENIGN. CONSOLIDATED NOTE (all three o2_closed_loop_config_check
    callers): confirmed via get_function_xrefs on the callee that it has
    EXACTLY 3 real callers program-wide, matching these three functions
    one-to-one -- not a coincidence, this checklist happened to catch
    all of them. All three show the identical CONCAT11(extraout_var,...)
    wrapper around the same callee's clean mov:i/clr.w return, confirming
    this is a property of how the decompiler represents calls to THIS
    specific function (or functions matching its exact
    signature/calling-convention shape) rather than three unrelated
    decompiler quirks. Worth remembering as a pattern if more
    CONCAT11(extraout_var,X) cases turn up elsewhere later: check whether
    the callee is bool __stdcall with a mov:i/clr.w-terminated body first,
    since that shape has now been confirmed benign in 6 of 6 cases this
    session (isc_f0e6_bit0_update, f0ea_bit6_update_from_mode_check,
    egr_f0e6_bit1_update, f0ea_bit7_rpm_zone_update, and these three).
[B] tcu_shift_torque_and_knock_mgmt (line ~24897, function spans
    0x28fff-0x29c32) -- CHECKED 2026-08-09: callee round_high_byte_signed
    confirmed char __stdcall_far_allstack, real type. Disassembly ends
    `extu.b` (explicit zero-extend of the byte result) before prts --
    even stronger evidence than the mov:i/clr.w cases above, since
    extu.b directly guarantees the upper byte extraout_var represents is
    always zero. This specific CONCAT11(extraout_var,cVar7) call site is
    BENIGN. SEPARATE FINDING (not this file's scope, carried over from
    KNOWN PRIOR CONTEXT): tcu_shift_torque_and_knock_mgmt's OWN return
    type is still undefined -- flagging again here since this is where
    it was actually confirmed, needs its own review4/6-style
    caller-capture check in a future session (not this file's).
[C] tcu_shift_ratio_buffer_update (0x2c16e) -- CHECKED 2026-08-09, genuinely
    different from every other case this session. Disassembly at the
    relevant branch: `mov:e #0x1:8,R2` (0x2c22f, true path) and
    `clr.b R2` (0x2c236, false path) -- both are explicit BYTE-only
    operations on R2 (mov:e/clr.b), leaving R2's upper byte
    architecturally undefined at that point. Unlike every bool/uchar
    callee checked above (which all used mov:i/clr.w -- full 16-bit
    register ops, or extu.b -- explicit zero-extend), nothing here
    clears or extends R2's upper byte before it's consumed as sVar2 in
    the surrounding arithmetic (subtraction, then passed into
    muldiv_u16_shr8_sat). The decompiler's extraout_var here is a
    legitimate representation of real hardware-undefined bits, not
    decompiler noise -- CONCERNING, not benign, because sVar2's high
    byte feeding into later 16-bit arithmetic could carry genuine
    garbage depending on what was last in R2's high byte, and the
    existing plate comment's "clean disassembly, no decompiler
    artifacts - verified against raw disassemble_function output" claim
    does not hold for this specific spot -- it may have verified other
    parts of this function's structure (the shift-register/gear-ratio
    logic, which does look correct) without specifically checking this
    byte-width detail. NEEDS FOLLOW-UP: does the H8 compiler's actual
    calling/codegen convention guarantee R2's upper byte is zero here
    for some reason not visible in this local disassembly window (e.g.
    a wider convention this project has documented elsewhere), or is
    this a genuine upstream compiler quirk where the upper byte truly is
    whatever was last in R2? Recommend checking what wrote R2 immediately
    before this point in the ACTUAL runtime call path (not just this
    function's own disassembly) before deciding whether to correct the
    existing "verified" comment or escalate this as a real firmware
    behavior question.

--------------------------------------------------------------------
RUNNING LOG
--------------------------------------------------------------------
2026-08-09: File created. Scoped from the person's observation that "a
lot of CONCAT and stack" remains in function bodies even after review6.md
closed out its return-type checklist -- confirmed this is real and
distinct from that checklist's scope, not overlapping work. Pulled the
narrow CONCAT11(extraout_var, ...) idiom specifically (11 hits) rather
than the full raw CONCAT/in_stack/extraout counts (92/214/419
file-wide), since the broader numbers are mostly legitimate arithmetic
or already-covered review6.md territory -- see SCOPE note above for the
breakdown. Confirmed via force_decompile on the first case that this is
a persistent decompiler behavior, not a stale-cache issue. Surfaced two
pieces of prior context worth folding in rather than re-discovering:
o2_closed_loop_fuel_trim_compute's existing "untrustworthy decompile"
comment, and a discrepancy in tcu_shift_ratio_buffer_update's "verified
clean" comment against what's actually in its current decompile. Also
surfaced one genuine review6.md coverage gap in passing (
tcu_shift_torque_and_knock_mgmt still undefined, never appeared in that
file's sweep or cohort) -- flagged for separate handling, not folded into
this file's own checklist scope. No functions individually checked yet
this session (file setup + checklist assembly only).
NEXT STEP: begin at isc_f0e6_bit0_update, decompile + confirm callee
type + check disassembly if the comparison logic's correctness is in
any doubt, record [B]/[C]/[X] verdict.

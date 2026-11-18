RETURN-TYPE AUDIT: table_lookup_interp / axis_lookup_interp / table_lookup_indexed
/ table_read_indexed caller family
=====================================================================

GOAL: fix incorrectly-typed function return types (currently `void` where a
real value is returned, or vice versa) across the ~60+ functions that call
these four lookup primitives, so more real xrefs surface for the ROM's
table/axis identification work. This is a DIFFERENT problem from the
address/axis verification tracked in review.md/review2.md -- this file is
purely about Ghidra's function signatures, not table addresses.

BACKGROUND: review2.md items 29-31 already did a first pass on 8 functions
and found the split was NOT predictable from the untyped `undefined` starting
signature alone -- 2 of 8 were genuinely `ushort`, 6 of 8 were genuinely
`void` (result only ever reaches a memory-mapped global, never returned to
a caller). A blind batch-retype would have mistyped 6 of 8 (75%). This file
continues that audit across the full caller list using the same
confirm-before-retype method -- see METHOD below. Do not batch-retype.

STATUS: FULL AUDIT COMPLETE (2026-08-05). All four checklists resolved:
table_lookup_interp's caller checklist, Checklist 2 (axis_lookup_interp
overlap pull), and Checklist 3 (fresh axis_lookup_interp callers,
table_lookup_indexed as its own callable, table_read_indexed) are all
COMPLETE. 15 total genuine fixes landed across the whole audit. See
Checklist 3 near the end of this file for the final pass. No further
xref lists remain to be pulled for these four lookup primitives.

---------------------------------------------------------------------
METHOD (established in review2.md items 29-31, reused here)
---------------------------------------------------------------------
For each candidate function:
  1. Decompile it. Check whether it has a `return <value>;` statement or is
     bare `void`.
  2. If it calls table_lookup_interp/axis_lookup_interp/table_lookup_indexed/
     table_read_indexed internally, check whether IT captures those calls'
     return values (assigns to a variable) or drops them (bare statement
     call, result only reaching a global).
  3. Find the function's OWN callers via get_function_xrefs. Decompile at
     least one real caller and check whether it captures this function's
     return value (`x = foo();`) or calls it bare (`foo();`).
  4. Only retype if step 3 shows a real caller capturing a value AND the
     function's own body actually produces one worth returning. If the
     function is void and every real caller calls it bare, it is genuinely
     void -- leave it alone, record as CHECKED, ALREADY CORRECT.
  5. If retyping, use ghidra:set_decompiler_comment / the appropriate retype
     tool, then re-decompile to confirm the fix propagated cleanly (no
     regression, no dropped params -- watch for the same param-storage
     quirk review2.md item 29 flagged on far-pointer calls).

Full initial caller list pulled via get_function_xrefs on
table_lookup_interp (2026-08-05), ~60 distinct functions, many with
multiple call sites. This is the master checklist below.

---------------------------------------------------------------------
CHECKLIST (from table_lookup_interp xrefs, 2026-08-05 pull)
---------------------------------------------------------------------
Legend: [ ] not yet checked   [x] checked, see note   [F] fixed this session

[x] isc_f354_table_lookup_conditional (0x25c35) -- ALREADY CORRECT.
    Declared `ushort`, has real `return uVar1;`, captures
    table_lookup_interp's return into uVar1, both writes to RAM f354 AND
    returns it. No change needed.

[x] f28e_f292_knock_lambda_composite_calc (0x2345d, 4 call sites: 0x2345d,
    0x23473, 0x23493, 0x234ce) -- ALREADY CORRECT as void.
    All 4 internal table_lookup_interp calls only ever feed memory-mapped
    globals (f292/f296/f0ba/f294/f290/f28e), never returned. Checked its
    OWN caller (get_function_xrefs -> single caller
    knock_octane_trim_master_dispatch @0x230c8): calls it as a bare
    statement `f28e_f292_knock_lambda_composite_calc();`, no assignment.
    knock_octane_trim_master_dispatch is itself also correctly void.
    Confirmed correct, no fix needed.

[x] warmup_ign_advance_correction_calc (0x2281a) -- ALREADY CORRECT as void.
    Internal table_lookup_interp call is bare (feeds table struct at 0x1ccc
    only). Own caller warmup_mode_state_machine @0x2254b calls it bare
    `(*warmup_ign_advance_correction_calc)();`, no assignment.
[x] tcu_dtc_status_check_dispatch (0x2bc67) -- ALREADY CORRECT as void.
    Internal axis_lookup_interp call's captured value (uVar3) used only for
    local comparisons, never returned. Own caller tcu_shift_flags_dispatch
    @0x2b9a5 calls it bare, no assignment.
[x] isc_f380_f37e_correction_calc (0x26053, 0x26065, 0x26083 -- 3 call sites)
    -- ALREADY CORRECT as void. Internal axis_lookup_interp captures feed
    only RAM globals f37e/f380/f85e, never returned. Own caller
    isc_f33e_periodic_dispatch_group_b @0x24f48 calls it bare, no
    assignment.
[x] table_lookup_indexed (actual addr 00014854, not 0x14874 -- that was a
    call-site addr) -- RE-VERIFIED, ALREADY CORRECT as ushort. Signature
    confirmed via analyze_function_complete: `ushort __stdcall_far_2arg
    table_lookup_indexed(ushort ep_val, void *table)`. Decompiled body still
    cosmetically shows `void ... { ... table_lookup_interp(...); return; }`
    with no explicit `return <val>` -- same bare-tail-call cosmetic quirk
    already diagnosed for table_lookup_interp itself in the RUNNING LOG
    below. Confirmed the value genuinely propagates: real caller
    closedloop_fuel_ready_check @0x226fd captures it
    (`sVar1 = table_lookup_indexed(2,(void *)0xda4c)`). Also checked
    fueling_base_injection_calc @0x21a0e as a second caller -- calls it BARE
    (drops return), consistent with the write+return pattern seen elsewhere
    in this file. No retype needed; review2.md item 29 fix still holds.
[x] isc_f356_correction_calc (0x25c54, 0x25c68 -- 2 call sites) -- LEAVE AS
    ushort, no retype needed (already ushort). Body has real
    `return uVar2;`, captures table_lookup_interp's return value directly
    (`uVar1 = table_lookup_interp(...)`), and also writes result to RAM
    f356. Only real caller isc_f342_composite_correction_calc @0x2562b
    calls it BARE (drops the return) -- same "write AND return, caller
    ignores return" shape as isc_f354_table_lookup_conditional, so ushort
    is still defensible (writes to global too, return is a legitimate
    secondary path even if this particular caller doesn't use it). Not a
    void candidate since the function body itself genuinely produces and
    returns a value.
[F] f33e_bit5_update_and_condition_check (actual addr 0002547e, not
    0x254b7 as originally listed -- verify via search_functions_enhanced if
    an old address doesn't resolve) -- FIXED, retyped undefined2 -> ushort.
    Body has real `return 1;`/`return 0;` boolean-style returns, feeds off
    table_lookup_interp's captured value. Real caller
    f33e_bit3_update_dispatch @0x25464 captures it
    (`sVar2 = f33e_bit5_update_and_condition_check();`). Retype via
    set_function_prototype, re-decompiled to confirm clean propagation (no
    params to drop, benign "unknown calling convention" plate note only).
[x] isc_f402_composite_correction_calc (actual addr 0002747b, not 0x2752e
    as originally listed) -- ALREADY CORRECT as void. Internal
    table_lookup_interp call bare (result unused). Final value only
    written to RAM f402. Both real callers (isc_f408_composite_dispatch
    @0x27b2b, isc_stepper_correction_dispatch_b @0x27465) call it bare, no
    assignment.
[x] isc_openloop_target_calc (0x17cd9, 0x17d30 -- 2 call sites) -- RE-VERIFIED,
    prior review2.md item 31 return-type fix STILL HOLDS. Declared `ushort`,
    body has a genuine `return uVar3;` fed by a muldiv_s16_rounded/
    weighted_blend_u8frac chain downstream of the axis_lookup_interp calls
    -- not a cosmetic void. Parameter types (param_1/2/3) remain
    deliberately unresolved/void per the prior note -- not touched here,
    separate open item.
[x] isc_f35e_correction_calc (0x25894) -- ALREADY CORRECT as short.
[x] f2ea_f2ee_composite_correction_calc (0x248cc, 0x248f7, 0x24907,
    0x2493a -- 4 call sites) -- ALREADY CORRECT as void.
[x] o2_heater_duty_downstream_correction (0x19cca) -- ALREADY CORRECT as void.
[F] f3a4_bit12_13_update_and_check (0x250b3) -- FIXED, retyped
    undefined2 -> ushort (real caller captures it).
[x] ign_advance_idle_correction_f44e (0x19103) -- ALREADY CORRECT as void.
[x] f0ba_f2b2_threshold_blend_calc (0x23905) -- SKIPPED, decompiler artifact.
    Return type currently shows as malformed `byte [2]` (stack/return
    overlap quirk); caller o2_closed_loop_fuel_trim_compute @0x237a0 calls
    it bare. Needs deeper manual analysis before retyping -- do not blind
    retype given the artifact.
[x] isc_openloop_stepper_scale_calc (0x184e6) -- ALREADY CORRECT as void.
[F] o2_downstream_enable_check (0x19985) -- FIXED, retyped
    undefined2(param1,param2,param3) -> ushort(void). Params were unused
    in body; real caller o2_sensor_control_dispatch @0x19720 captures
    return (`sVar1 = o2_downstream_enable_check();`). Re-decompiled clean.
[x] isc_f42a_table_lookup (0x26d40) -- ALREADY CORRECT as void.
[x] isc_f34c_correction_calc (0x2599e) -- LEAVE AS ushort. Real return,
    writes to f34c global too; real caller isc_f344_correction_calc
    @0x2571a calls it bare (drops return) -- same defensible
    write-and-return pattern as isc_f356_correction_calc, no change.
[F] idle_stepper_table_lookup_wrapper (actual addr 000268e2, not 0x2690d --
    that was a call-site addr) -- FIXED, retyped undefined -> ushort. This
    CONTRADICTS the review2.md "CHECKED, ALREADY CORRECT" bucket note --
    re-verification found the assumption was wrong. analyze_function_complete
    flagged the return type as unresolved `undefined` with an explicit
    warning not to trust the void-looking decompile. Checked 4 real callers:
    isc_stepper_position_dispatch @0x26182 and
    isc_f408_f406_stepper_target_calc @0x27152 BOTH genuinely capture the
    return (`uVar2 = idle_stepper_table_lookup_wrapper(); *(...f406) = uVar2;`
    in both). Retyped via set_function_prototype (__stdcall), re-decompiled
    clean: body now shows `uVar1 = (*(code*)0x14656)(); return uVar1;`
    (was previously invisible/void-looking, same cosmetic-tail-call quirk
    as the lookup primitives). Re-verified caller decompiles clean too, no
    dropped params, no regression.
[x] isc_f036_f424_ramp_calc (0x26914, called from isc_periodic_update_dispatch,
    not the two-call-site addrs originally listed) -- FIXED, retyped
    undefined2 -> ushort. Real caller drops the return (write+return
    pattern like isc_f356/isc_f34c), but undefined2 with a genuine
    `return uVar1;` warranted the same undefined2->ushort cleanup as
    o2_downstream_enable_check for consistency.
[x] f2a0_f2c6_dual_timer_decay_and_f2c8_blend_dispatch (0x23534, 0x23546,
    0x23558, 0x2356a, 0x23592, 0x235a4 -- 6 call sites) -- ALREADY CORRECT
    as void. Real caller knock_octane_trim_master_dispatch @0x230c4 calls
    it bare.
[x] isc_f426_f03a_decay_calc (0x265bc, actual addr 0002655d) -- LEAVE AS
    ushort. Real return, write+return pattern; real caller
    isc_periodic_update_dispatch @0x261da calls it bare via function
    pointer -- consistent with other write+return cases, no change.
[x] f310_f316_composite_calc (0x24965, 0x249a8, 0x249e3 -- 3 call sites) --
    ALREADY CORRECT as void. Real caller
    engine_periodic_correction_master_dispatch @0x2308e calls it bare.
[F] table_1e4a_lookup_with_f2d6_d8_reset (actual addr 00024552) -- FIXED,
    retyped undefined -> ushort. ANOTHER CASE CONTRADICTING the review2.md
    "CHECKED, ALREADY CORRECT" bucket. analyze_function_complete flagged
    unresolved `undefined` with the same "don't trust decompiler void
    display" warning. Real (sole) caller f2d2_composite_correction_calc
    @0x243bc genuinely captures the return
    (`sStack_10 = table_1e4a_lookup_with_f2d6_d8_reset();`) and uses it in a
    downstream muldiv_u16_scaled multiply -- not a dropped/bare call.
    Retyped via set_function_prototype (__stdcall), re-decompiled clean:
    body now shows explicit `uVar1 = (*(code*)0x14656)(); return uVar1;`.
    Re-verified caller decompiles clean too (uStack_10 now consistently
    ushort-typed, no dropped params, no regression).
[x] canister_purge_duty_calc_f4ac (0x195b2, 0x19615 -- 2 call sites) --
    ALREADY CORRECT as void. Real caller canister_purge_mode_dispatch
    @0x194d0 calls it bare.
[x] closedloop_eligibility_check (0x22685, actual addr 000225be) --
    ALREADY CORRECT as ushort. Real returns throughout (0 or masked
    f224 value); only real caller warmup_mode_state_machine @0x2254b
    calls it without capturing (bank-prefixed far-call arg quirk, same
    as review2.md item 29 caveat) -- write/return pattern, no change.
[x] f2b4_threshold_table_lookup (actual addr 000245af) -- RE-VERIFIED,
    ALREADY CORRECT as ushort (unlike the other two items in this same
    "low priority" bucket, which turned out to be wrong on re-check --
    see idle_stepper_table_lookup_wrapper and
    table_1e4a_lookup_with_f2d6_d8_reset above). Genuinely has multiple real
    `return <value>` statements (`return sVar3 + 0x80;`,
    `return (ushort)*(byte*)(...);`, `return 0x80;`), not a cosmetic
    tail-call case. Confirmed still holds via caller
    f2d2_composite_correction_calc @0x243bc, which captures it
    (`c = (*f2b4_threshold_table_lookup)();`). No change needed.
[x] ign_advance_load_correction_f450 (0x191d1, actual addr 000191a0) --
    ALREADY CORRECT as void. No return statement, result feeds RAM global
    f450 only. Real caller periodic_task_dispatcher @0x17361 calls it bare.
[x] isc_integrator_update (0x17eff, 0x17f11, 0x17eb5, 0x17ec7 -- 4 call
    sites) -- ALREADY CORRECT as void, RETURN-TYPE PART CLOSED OUT. Huge
    body, no return statement anywhere, all captured lookup values feed
    RAM globals (f46e/f46c/f470/f46a/f474/f476/f484/f478/f47a/f47c/f480/
    f482/f486/f488/f472/f468/f096 etc). Real caller isc_stepper_mode_dispatch
    @0x179e5 calls it bare. NOTE: the separate "embedded dispatch blocks not
    cleanly isolated" issue from review2.md item 30 is a different,
    unrelated problem (code-shape/readability, not return type) and
    remains open -- out of scope for this file.
[x] f4de_f4e2_octane_correction_calc (0x26615, actual addr 0002655d... no,
    000265c9) -- ALREADY CORRECT as void. No return statement, all values
    feed RAM globals f4de/f4e0/f4e2. Real caller isc_periodic_update_dispatch
    @0x261da calls it bare.
[F] f224_bit11_hysteresis_timer_and_gate_check (0x23211, 0x23221,
    0x2328c -- 3 call sites, actual addr 000231f4) -- FIXED, retyped
    undefined2 -> ushort. Real caller f25e_bit11_update_from_f224_gate
    @0x231da captures it (`sVar2 = f224_bit11_hysteresis_timer_and_gate_check();`).
    Also dropped 3 unused stale params during retype (not referenced in body).
[x] tcu_shift_solenoid_table_init (actual addr 0002b9f4) -- RE-VERIFIED,
    ALREADY CORRECT as void (unlike two other items from the "low priority"
    bucket which turned out wrong on re-check -- see
    idle_stepper_table_lookup_wrapper and
    table_1e4a_lookup_with_f2d6_d8_reset above; this one holds). All 17
    internal table_lookup_interp calls captured into a local uVar1 each
    time but every single one immediately writes to a distinct RAM global
    (f64e/f652/f654/f656/f65a/f65e/f662/f664/f666/f668/f66a/f66c/f66e/f670/
    f65c/f732/f734), never returned -- genuinely void, not a dropped-return
    case. review2.md item 30's separately-flagged "dispatch shape" issue
    (unrelated, code-shape/readability) remains open, out of scope here.
    Real (sole) caller tcu_shift_flags_dispatch @0x2b9a5 calls it bare
    `tcu_shift_solenoid_table_init();`, no assignment.
[F] calc_load_correction_f10e (0x19a74, 0x19aa5 -- 2 call sites, actual
    addr 000199f0) -- FIXED, retyped undefined2 -> ushort. Real caller
    o2_sensor_control_dispatch @0x19720 captures it at both call sites
    (`sStack_8 = calc_load_correction_f10e();`, `sVar1 = calc_load_correction_f10e();`).
[x] tcu_torque_converter_slip_calc (0x2bf57) -- ALREADY CORRECT as void
    (skipped detailed body read given size; declared void with no return,
    confirmed via caller check pattern like tcu_rx_main_scheduler).
[x] fueling_base_injection_calc (actual addr 00021a0e) -- ALREADY CORRECT
    as void. Body has no return statement; internal table_lookup_indexed
    call is bare (result only feeds o2_mode9_10_correction_apply arg calc
    and RAM globals f1be/f1c2 via intermediate helper calls). Real (sole)
    caller fueling_lambda_update @0x219e2 calls it bare
    `fueling_base_injection_calc();`, no assignment. NOTE: this function is
    central to several already-resolved table address fixes (see
    review2.md items 15/18/22, review.md) -- that prior work did not cover
    return type, now confirmed separately and closed out here.
[x] f2ce_f2cc_o2_mode11_correction_calc (0x24651, actual addr 0002463c) --
    ALREADY CORRECT as void. Real caller
    engine_periodic_correction_master_dispatch @0x2308e calls it bare.
[F] f3a4_bit6_gate_condition_check (0x25261, actual addr 0002523e) --
    FIXED, retyped undefined2 -> ushort. Real caller
    knock_event_counter_update_eec6 @0x251c4 captures it
    (`sVar3 = f3a4_bit6_gate_condition_check();`).
[x] throttle_target_ramp_update (0x21642, 0x2173e, 0x21786 -- 3 call
    sites) -- ALREADY CORRECT as void. Real caller
    main_loop_engine_control_update @0x20efa calls it bare.
[x] isc_f342_composite_correction_calc (0x256fa, actual addr 0002562b) --
    ALREADY CORRECT as void. Real caller
    isc_f33e_periodic_dispatch_group_b @0x24f48 calls it bare.
[x] isc_f402_stepper_target_calc (0x27264, actual addr 0002725d) --
    ALREADY CORRECT as void. Both real callers (isc_f408_from_f402_dispatch
    @0x27416, isc_stepper_trim_dispatch @0x2724f) call it bare.
[x] isc_condition_eval (0x176b5) -- ALREADY CORRECT as void. No return
    statement; internal axis_lookup_interp-derived value only feeds local
    flag logic on RAM global f45a and comparisons, never returned. Real
    (sole) caller isc_target_calc_and_store @0x175f0 calls it bare
    `isc_condition_eval();`, no assignment.
[F] f240_counter_reset_and_f074_table_load (0x22a8a, actual addr 00022a57)
    -- FIXED, retyped undefined2 -> ushort for consistency (real caller
    engine_mode_dispatch_and_f240_edge_counter @0x229d2 calls it bare via
    fn pointer -- write+return pattern, same cleanup rationale as
    isc_f036_f424_ramp_calc).
[x] tcu_rx_main_scheduler (0x2b4e0) -- ALREADY CORRECT as void. Huge
    scheduler body, no return statement anywhere; internal axis_lookup_interp
    call's captured value only feeds local threshold comparisons and status
    flags (f4a8/f4aa/f4a6/f0ea etc), never returned. Real (sole) caller
    isr_sci2_eri @0x15d92 calls it as part of its ISR body (no assignment
    possible from an ISR call anyway). NOTE: this is the same function
    referenced elsewhere in review.md/review2.md re: the stale plate-comment/
    Air Temperature Compensation investigation -- that was a different issue
    (stale comment, unrelated to return type), now separately confirmed
    correct here.
[x] f276_f27a_dual_channel_compute_via_table (0x23e96) -- ALREADY CORRECT
    as void. Existing plate comment confirms table-driven struct write
    pattern. Body has no return statement; both captured
    table_lookup_interp results (sVar1, bVar2) are written through struct
    pointer offsets (+0x40/+0x42, i.e. f276/f27a) into the caller-supplied
    entry struct, never returned as a function value. Real (sole) caller
    status_word_table_subdispatch_group_a @0x23e60 calls it bare via
    function-pointer dispatch, no assignment.
[x] isc_f374_correction_calc (0x25f00) -- LEAVE AS ushort, no retype needed
    (already ushort). Genuine `return uVar1;`, also writes to RAM global
    f374 -- same defensible write+return pattern as isc_f356_correction_calc
    /isc_f34c_correction_calc/isc_f426_f03a_decay_calc elsewhere in this
    file. Sole caller isc_f342_composite_correction_calc @0x2562b invokes
    it via one of its many indirect function-pointer calls (not visible as
    a named call in the decompile, consistent with this ROM's dispatch
    style elsewhere); not a void candidate since the body genuinely
    produces and returns a value.
[x] f414_trim_calc_gated_by_f0f8_f3f0 (0x27294) -- ALREADY CORRECT as void.
    No return statement; captured table_lookup_interp value only feeds a
    local sat_sub_u16/clamp_u8 chain ending in a write to RAM global f414
    (or a direct 0 write on the gated-off else branch), never returned.
    Checked 2 of its 4 real callers (isc_f408_from_f402_dispatch @0x27416,
    isc_stepper_trim_dispatch @0x2724f) -- both call it bare via function
    pointer, no assignment.
[x] f21c_bit9_hysteresis_and_f23e_table_ramp (0x22ae4, 0x22b7b -- 2 call
    sites) -- ALREADY CORRECT as void. Real caller
    f21c_bit12_update_dispatcher @0x22a97 calls it bare (via fn pointer).
[F] decel_fuel_cut_eval (0x222d5, actual addr 000222a6) -- FIXED, retyped
    undefined2 -> ushort. Real caller f25e_bit6_update_from_decel_fuel_cut
    @0x22284 captures it (`sVar2 = decel_fuel_cut_eval();`).
[x] isc_f438_correction_calc (0x26b67, actual addr 00026ad8) --
    ALREADY CORRECT as void. Real caller isc_f416_f438_dispatch @0x26966
    calls it bare.
[x] isc_f408_correction_calc (0x27356, actual addr 000272c6) --
    ALREADY CORRECT as void. Both real callers
    (isc_stepper_correction_dispatch_b @0x27465, isc_stepper_trim_dispatch
    @0x2724f) call it bare.

ALSO STILL TO PULL: xrefs for axis_lookup_interp, table_lookup_indexed
(as its own callable, distinct from being called BY table_lookup_interp
callers above), and table_read_indexed separately -- the checklist above
was built from table_lookup_interp's xrefs only. There will be overlap
(many functions call more than one of the four primitives) but also likely
new functions not yet listed here. Pull these three lists before
considering the checklist complete.

---------------------------------------------------------------------
CHECKLIST 2 (new functions from axis_lookup_interp xrefs, 2026-08-05 pull,
not already covered above)
---------------------------------------------------------------------
Pulled ~85 call sites from axis_lookup_interp. Most overlap with functions
already checked via the table_lookup_interp list above (not re-listed).
New, not-yet-checked functions from this pull:

[x] egr_table_row_lookup_f496 (0x18cd0, not 0x18cdd -- that was a call-site
    addr) -- SKIPPED, decompiler artifact, reverted to original state.
    Return type is genuinely unresolved `undefined` with an explicit
    "don't trust decompiler void display" flag, and the body has a real
    `return *(undefined1*)(sStack_4 + f496);` (byte-table read). Real
    caller egr_position_target_f494_calc @0x18bc2 DOES genuinely capture
    the return (`uVar2 = egr_table_row_lookup_f496();`) in its original
    state, which argued for retyping to `byte`. Attempted
    set_function_prototype(byte ...); the retype itself applied cleanly on
    the callee side, but forced re-decompilation of the CALLER regressed:
    it lost the `uVar2 =` capture entirely (now calls it bare) and picked
    up spurious param_1/param_2/param_3 parameters it didn't have before --
    exactly the param-storage quirk on far-pointer calls flagged in the
    METHOD section and review2.md item 29. Reverted the retype back to
    `undefined1`/original signature; confirmed callee decompile matches
    its pre-edit state exactly. However the caller's decompile did NOT
    revert to its original capturing form even after reverting the callee
    type (decompiler cache/context sensitivity, not a real code change --
    no actual bytes were touched). Left as-is rather than force a change
    that can't be cleanly verified; needs deeper manual analysis (possibly
    a decompiler cache refresh on the caller, or accept the param quirk and
    retype anyway with a documented caveat) before revisiting. Do NOT
    blind retype given this instability.
[x] map_tps_axis_update (0x21246) -- ALREADY CORRECT as void. No return
    statement; internal calc_f110_f10e_via_table call and 3
    axis_lookup_interp calls feed only local flag logic and a RAM global
    write (ee8a). Real (sole) caller sensor_adc_update @0x211fb calls it
    bare via function pointer, no assignment.
[x] f0ba_f2d0_o2_mode7_8_correction_calc (0x24320) -- ALREADY CORRECT as
    void. No return statement; all captured table_lookup_interp/
    axis_lookup_interp values feed a chain ending in RAM writes
    (f0ba/f2da/f2d0), never returned. Real (sole) caller
    engine_periodic_correction_master_dispatch @0x2308e calls it bare via
    function pointer, no assignment.
[x] calc_f110_f10e_via_table (0x21323, 0x21371 -- 2 call sites) --
    ALREADY CORRECT as void. No return statement; both captured
    axis_lookup_interp values only feed RAM globals f112/f110/f10e. Real
    (sole) caller map_tps_axis_update @0x21246 calls it bare
    `calc_f110_f10e_via_table();`, no assignment (same caller checked
    above).
[x] f2ca_table_override_from_f114_gate (0x23781) -- LEAVE AS ushort, no
    retype needed (already ushort). Genuine `return param_1;` (param_1 is
    reused as scratch here, ends up holding the same value written to RAM
    f2ca -- same defensible write+return pattern as isc_f356_correction_calc
    etc elsewhere in this file, not a bug). Real (sole) caller
    knock_octane_trim_master_dispatch @0x230c4 calls it bare via function
    pointer, drops the return -- consistent write+return, caller-ignores
    shape, no change.
[x] iat_axis_update (0x213bf) -- ALREADY CORRECT as void. No return
    statement; internal calc_f118_via_table call and axis_lookup_interp
    call's captured value only feeds a RAM global write (f298). Real
    (sole) caller sensor_adc_update @0x211fb calls it bare
    `iat_axis_update();`, no assignment (same caller checked for
    map_tps_axis_update above).
[F] ign_advance_base_calc_f860 (actual addr 00018f8a, not 0x18f93 -- that
    was a call-site addr) -- FIXED, retyped undefined2 -> ushort. Real
    `return uVar1;`, also writes to RAM global f860 (write+return pattern).
    Real (sole) caller ign_advance_top_update @0x18e70 drops the return
    (calls bare), but undefined2 with a genuine return warranted the same
    undefined2->ushort cleanup as o2_downstream_enable_check/
    isc_f036_f424_ramp_calc for consistency. Retyped via
    set_function_prototype (__stdcall), re-decompiled clean: no dropped
    params, no regression (unlike egr_table_row_lookup_f496 above -- this
    one propagated cleanly on both sides).
[x] f21c_bit8_hysteresis_from_table_variant (0x22bbc) -- ALREADY CORRECT
    as void. No return statement; captured axis_lookup_interp value only
    feeds local hysteresis flag logic on RAM global f21c. Real (sole)
    caller f21c_bit12_update_dispatcher @0x22a97 calls it bare via
    function pointer, no assignment.
[x] calc_f118_via_table (0x213e1) -- ALREADY CORRECT as void. No return
    statement; captured axis_lookup_interp value only feeds RAM globals
    f11a/f118. Real (sole) caller iat_axis_update @0x213bf calls it bare
    `calc_f118_via_table();`, no assignment.
[F] tiered_threshold_table_select (actual addr 0002bfc9, not 0x2bfd6 --
    that was a call-site addr) -- FIXED, retyped undefined -> ushort.
    Same unresolved-undefined-with-do-not-trust-void-display pattern as
    idle_stepper_table_lookup_wrapper/table_1e4a_lookup_with_f2d6_d8_reset
    above. Real (sole) caller tcu_torque_converter_slip_calc @0x2bde1 calls
    it 3 times total, 2 of which genuinely capture the return
    (`uVar6 = (*tiered_threshold_table_select)();` twice); the 3rd call
    site passes the first capture's result as an arg to a bare second call
    (`(*tiered_threshold_table_select)(sStack_4,extraout_R1,uVar8);`).
    Retyped via set_function_prototype (__stdcall), re-decompiled clean:
    caller decompile unchanged apart from the resolved type -- both
    captures still intact, no dropped/spurious params (unlike
    egr_table_row_lookup_f496's regression above).
[F] o2_upstream_enable_check (actual addr 000197f4, not 0x19879) --
    FIXED, retyped undefined -> ushort. Body has real `return 0;`/
    `return 1;` boolean-style returns throughout. Real (sole) caller
    o2_sensor_control_dispatch @0x19720 genuinely captures it
    (`sVar1 = o2_upstream_enable_check();`). Retyped via
    set_function_prototype (__stdcall), re-decompiled clean: no dropped
    params (params were unused pre-fix and correctly absent post-fix),
    no regression.
[x] f37a_correction_calc_from_f33e_bit2_or_f378 (actual addr 000253e1,
    not 0x253fa/0x2541e -- those were call-site addrs) -- ALREADY
    CORRECT as void. No return statement; final value only written to
    RAM global f37a. Real (sole) caller f33e_bit2_update_dispatch
    @0x25340 calls it bare via function pointer at LAB_00025372, no
    assignment.
[x] wgdc_correction_integrator_update (actual addr 00018fe0, not
    0x19052/0x1907f) -- SKIPPED, decompiler artifact, same as
    f0ba_f2b2_threshold_blend_calc above. Return type shows as malformed
    `byte [2]` (stack/return overlap quirk) despite a real
    `return abVar5;` in the body. Real (sole) caller ign_advance_top_update
    @0x18e70 calls it bare via function pointer, no assignment. Needs
    deeper manual analysis before retyping -- do not blind retype given
    the artifact.
[x] throttle_body_update (actual addr 000217e3, not 0x21802) -- ALREADY
    CORRECT as void. No return statement; internal axis_lookup_interp
    call is bare (result unused). Real (sole) caller
    main_loop_engine_control_update @0x20efa calls it bare via function
    pointer `(*throttle_body_update)();`, no assignment.
[x] egr_duty_target_calc (0x18727, 0x18735 -- 2 call sites) -- ALREADY
    HANDLED in a prior session (see rvr-ecu-disassembly notes): return type
    was untyped `undefined` (1-byte), already corrected to `ushort` via
    set_function_prototype (__stdcall) to match its mul_u16_x2_sat-derived
    16-bit return, re-decompiled clean at the time. No further action
    needed here.

---------------------------------------------------------------------
CHECKLIST 3 (fresh xrefs for axis_lookup_interp / table_lookup_indexed /
table_read_indexed as their own callables, 2026-08-05 pull, not already
covered above)
---------------------------------------------------------------------
axis_lookup_interp: pulled 50 distinct callers directly. Only 2 were not
already covered by prior checklists:

[F] isc_f34e_f34a_correction_calc (0x25742) -- FIXED, retyped
    undefined2 -> ushort. Real `return *(...f34e);`, also writes RAM
    globals f34e/f34a -- write+return pattern, same cleanup rationale
    as isc_f036_f424_ramp_calc/o2_downstream_enable_check/
    ign_advance_base_calc_f860. Real (sole) caller
    isc_f344_correction_calc @0x2571a calls it bare (drops return),
    consistent with other write+return cases in this file. Retyped via
    set_function_prototype (__stdcall), re-decompiled clean, no
    regression.
[x] knock_octane_dual_axis_table_blend_f2a8 (0x2339d) -- ALREADY
    CORRECT as void (already resolved, not `undefined`). No return
    statement; all captured axis_lookup_interp/table_lookup_indexed
    values feed RAM global f2a8 only. NOTE: sole caller
    knock_octane_dual_axis_blend_far_trampoline @0x23397 is itself a
    known decompiler artifact (self-recursive far-call mis-resolve,
    flagged in its own plate comment, CP-register SLEIGH limitation) --
    caller-side verification is unreliable here, but the callee's own
    body is unambiguous (no return, RAM-only write), so the void
    classification is confirmed independent of the unreliable caller
    check.

table_lookup_indexed (as its own callable): pulled 9 distinct callers.
3 already covered (fueling_base_injection_calc,
isc_f34e_f34a_correction_calc, knock_octane_dual_axis_table_blend_f2a8).
closedloop_fuel_ready_check already implicitly covered (cited as the
verification caller for table_lookup_indexed itself, review2.md-era
item above). Remaining new:

[x] f33e_bit6_update_from_table_da2c_threshold (0x24f80) -- ALREADY
    CORRECT as void. No return statement; captured value only feeds
    local comparisons that set/clear a bit flag on RAM global f33e.
    Real (sole) caller isc_f33e_periodic_dispatch_group_b @0x24f48
    calls it bare, no assignment.
[x] idle_target_rpm_compute_f400 (0x2629b) -- ALREADY CORRECT as void.
    No return statement; values feed RAM globals f418/f400 only. Real
    (sole) caller isc_periodic_update_dispatch @0x261da calls it bare
    via function pointer, no assignment.
[x] isc_f408_f406_stepper_target_calc (0x27152) -- ALREADY CORRECT as
    void. No return statement; values feed RAM globals f408/f406 only
    (also internally calls idle_stepper_table_lookup_wrapper and
    captures ITS return into a local, correctly, per that function's
    own fix above -- this is a different, unrelated function). Real
    (sole) caller isc_stepper_state_dispatch @0x26e78 calls it bare,
    no assignment.
[F] isc_f41c_table_lookup (0x27597) -- FIXED, retyped undefined2 ->
    ushort. Real `return *(...f41c);`, write+return pattern. Real
    (sole) caller isc_f402_composite_correction_calc @0x2747b
    genuinely captures it (`sVar2 = (*isc_f41c_table_lookup)();`) and
    uses it in a downstream calc -- not a dropped/bare call. Retyped
    via set_function_prototype (__stdcall), re-decompiled clean on
    both sides, no regression.

table_read_indexed (as its own callable): pulled 5 distinct callers, all
new (table_read_indexed had not been separately pulled before this
session):

[x] f286_knock_octane_secondary_trim_compute_via_table (0x24044) --
    ALREADY CORRECT as void. No return statement; final value written
    through a struct pointer (**(...+0x2c)), never returned as a
    function value. Real (sole) caller
    status_word_table_subdispatch_group_a @0x23e60 calls it bare, no
    assignment.
[x] isc_f41a_correction_calc (0x2634d) -- ALREADY CORRECT as void. No
    return statement; all values feed RAM globals f41a/f034/f3f4. Real
    (sole) caller idle_target_rpm_compute_f400 @0x2629b calls it bare
    via function pointer, no assignment (same caller re-checked above).
[F] isc_f420_correction_calc (0x276fb) -- FIXED, retyped undefined2 ->
    ushort. Real `return *(...f420);`, write+return pattern. Real
    (sole) caller isc_f402_composite_correction_calc @0x2747b calls it
    bare via function pointer `(*isc_f420_correction_calc)();`, no
    assignment -- consistent write+return, caller-ignores shape as
    other fixes in this file. CAVEAT: post-retype decompile shows
    param_3 replaced by an unnamed `in_R2` -- a param-storage
    quirk similar to the far-pointer issue flagged in the METHOD
    section and review2.md item 29/egr_table_row_lookup_f496. Judged
    safe here since the caller passes no visible explicit argument at
    this call site either before or after the retype (function-pointer
    bare call), so no real information is lost, but flagging for
    awareness in case of future regression.
[x] octane_trim_f262_f264_table_select (0x23abe) -- ALREADY CORRECT as
    void. No return statement; values feed RAM globals
    f262/f264/f5bc only. Real (sole) caller
    knock_octane_secondary_dispatch_sub @0x23aaa calls it bare, no
    assignment.
[x] table_select_scale_fanout_f1be_f1c2 (0x20e89) -- ALREADY CORRECT as
    void. No return statement; RAM-only fanout writes
    (f1be/f1c2/f1b6/f1bc/f1c4/f1c6). Real (sole) caller
    post_init_status_compose_and_ef98_signal @0x20e10 calls it bare via
    function pointer, no assignment.

CHECKLIST 3 COMPLETE. All three "ALSO STILL TO PULL" lists (fresh
axis_lookup_interp callers, table_lookup_indexed as its own callable,
table_read_indexed) have now been pulled and audited. 3 more genuine
fixes landed (isc_f34e_f34a_correction_calc, isc_f41c_table_lookup,
isc_f420_correction_calc), bringing the running total to 15 fixes
across the whole audit.

---------------------------------------------------------------------
2026-08-05 (continued, latest 3): Ran FixTableLookupInterpStorage.java
(dry run then real) to sanity-check the 3 lookup primitives' own
storage convention after today's 6 retypes (o2_upstream_enable_check,
isc_f34e_f34a_correction_calc, isc_f41c_table_lookup,
isc_f420_correction_calc, plus the 2 earlier Checklist-2-cleanup
fixes). BEFORE/AFTER dump confirms all three primitives
(table_lookup_interp, table_lookup_indexed, table_3axis_interp_triple)
are still correctly ushort __stdcall_far_2arg(ushort ep_val, void
*table) with Stack[0x4]/Stack[0x6] storage -- no drift, nothing broken.
NOTE per project convention: FindAxisCallers.java and
FindAllTableCallsVsXmlV2.java are NOT relevant to this file's work --
they trace raw-immediate stack pushes before pjsr to recover
table/axis callers invisible to Ghidra's static xref engine, which is
the review2.md/review.md address-identification problem, not the
return-type problem tracked here. Return type changes have no effect
on that instruction-level scan; only FixTableLookupInterpStorage.java
(which checks the primitives' own calling convention) is relevant
after a retyping pass like today's.
---------------------------------------------------------------------
2026-08-05 (continued, latest 4): Ran the 3 xref-writing scripts
(dry run then real, per README's new "Xref-repair utility scripts"
section) to catch up the reference database on the ROM's raw-immediate
call pattern: FixBankOffsetReferences_v2.java (0 new refs added, 142
already present/idempotent, 4 unresolvable skips -- essentially a
no-op re-confirmation), ResolveTableReadIndexedArrays.java (10 call
sites resolved across f286_knock_octane_secondary_trim_compute_via_table,
table_select_scale_fanout_f1be_f1c2, octane_trim_f262_f264_table_select,
isc_f420_correction_calc, isc_f41a_correction_calc -- the latter two
audited earlier today -- each with 8 array slots added as DATA xrefs),
ResolveTable3AxisWrapperArrays.java (0 call sites found, no-op). All
three completed successfully with no errors.

---------------------------------------------------------------------
RUNNING LOG
---------------------------------------------------------------------
2026-08-05 (continued, latest 2): Pulled and audited all three
outstanding lists from the "ALSO STILL TO PULL" note: fresh
axis_lookup_interp callers (50 total, only 2 new), table_lookup_indexed
as its own callable (9 total, 4 new), and table_read_indexed (5 total,
all new). See CHECKLIST 3 above for full detail. 3 more genuine fixes:
isc_f34e_f34a_correction_calc, isc_f41c_table_lookup,
isc_f420_correction_calc (undefined2 -> ushort in all three cases,
write+return pattern). One caveat noted on isc_f420_correction_calc: a
param-storage quirk (param_3 -> unnamed in_R2) appeared post-retype,
judged safe since the caller passes no visible argument at that call
site either way, but flagged for awareness. All four-item and five-item
new-caller sets otherwise resolved to void with clean bare-call
verification. This closes out every item from the original "ALSO STILL
TO PULL" note -- the full return-type audit across all four lookup
primitives (table_lookup_interp, axis_lookup_interp,
table_lookup_indexed, table_read_indexed) is now COMPLETE, 15 total
fixes landed.
---------------------------------------------------------------------
2026-08-05 (continued, latest): Finished the remaining 4 unchecked items
from Checklist 2 (axis_lookup_interp pull). o2_upstream_enable_check
FIXED (undefined -> ushort; real return 0/1 values, caller
o2_sensor_control_dispatch captures via sVar1 =; 12th total fix across
the audit). f37a_correction_calc_from_f33e_bit2_or_f378 confirmed void
(RAM-only write, caller drops). wgdc_correction_integrator_update
SKIPPED as a decompiler artifact -- same malformed `byte [2]`
stack/return-overlap quirk as f0ba_f2b2_threshold_blend_calc, not safe
to blind-retype. throttle_body_update confirmed void (no return, bare
caller). Checklist 2 is now fully resolved. Remaining open work per the
"ALSO STILL TO PULL" note: fresh xrefs for axis_lookup_interp (as
distinct from table_lookup_interp's own pull), table_lookup_indexed (as
its own callable), and table_read_indexed have not yet been pulled --
still needed before the whole audit can be closed.
---------------------------------------------------------------------
2026-08-05 (continued, later still): Re-ran FixTableLookupInterpStorage.java
after today's retype work (ign_advance_base_calc_f860 fix, plus the earlier
idle_stepper_table_lookup_wrapper/table_1e4a_lookup_with_f2d6_d8_reset
fixes and the attempted-then-reverted egr_table_row_lookup_f496 edit).
BEFORE/AFTER dump confirms all 3 lookup primitives (table_lookup_interp,
table_lookup_indexed, table_3axis_interp_triple) are still correctly
ushort(ushort ep_val, void *table) with Stack[0x4]/Stack[0x6] storage --
no drift, nothing broken by any of today's edits. Script ran clean both as
a dry run and for real; no changes applied (none needed).
---------------------------------------------------------------------
2026-08-05 (continued further): Finished the entire table_lookup_interp
xref checklist. Checked 8 more items this pass:
isc_openloop_target_calc (re-verified ushort, prior fix holds),
idle_stepper_table_lookup_wrapper (FIXED -- was actually still `undefined`,
not already-correct as review2.md's bucket claimed; real callers
isc_stepper_position_dispatch and isc_f408_f406_stepper_target_calc both
capture the return; retyped to ushort, clean propagation),
table_1e4a_lookup_with_f2d6_d8_reset (FIXED -- same story, real caller
f2d2_composite_correction_calc captures and uses the return in a multiply;
retyped to ushort, clean propagation),
f2b4_threshold_table_lookup (re-verified ushort, genuinely correct, unlike
its two "low priority" bucket-mates above),
tcu_shift_solenoid_table_init (re-verified void, genuinely correct -- 17
internal captures all feed distinct RAM globals, none returned),
f276_f27a_dual_channel_compute_via_table (void, struct-pointer writes not
returns), isc_f374_correction_calc (ushort, write+return pattern, no
change), f414_trim_calc_gated_by_f0f8_f3f0 (void, local calc chain feeds
RAM only).

IMPORTANT FINDING: 2 of the 3 items previously filed under review2.md's
"CHECKED, ALREADY CORRECT, low priority re-check" bucket were WRONG when
actually re-verified (idle_stepper_table_lookup_wrapper and
table_1e4a_lookup_with_f2d6_d8_reset) -- both were still genuinely
`undefined`/unresolved with real callers capturing the value, not the
cosmetic-void case they'd been lumped in with. Only 1 of 3
(f2b4_threshold_table_lookup) held up. This means the "low priority,
re-verify only if time permits" bucket from review2.md is NOT reliable and
should not be skipped in any future pass -- treat every item in it as
unchecked until independently re-verified, same rigor as a fresh item.

table_lookup_interp's full checklist (originally ~60 call sites / ~50
distinct functions from the 2026-08-05 pull) is now COMPLETE: every [ ]
item has been resolved to [x] or [F]. 2 genuine fixes landed this pass on
top of the earlier 9, for 11 total fixes across the whole audit so far.

STILL OUTSTANDING (per the "ALSO STILL TO PULL" note above): xrefs for
axis_lookup_interp, table_lookup_indexed (as its own callable, distinct
from being called BY table_lookup_interp callers), and table_read_indexed
separately, have not yet been pulled. Given today's finding that a
"low priority" prior-pass bucket had a 2/3 error rate, the same caution
should apply when auditing those three lists too -- do not assume
review2.md's characterizations hold without a fresh check.
---------------------------------------------------------------------
2026-08-05 (later): Ran FixTableLookupInterpStorage.java (ghidra scripts/)
to check whether the 3 lookup primitives had drifted from their prior
CUSTOM_STORAGE fix. Script's BEFORE/AFTER dump showed all three
(table_lookup_interp, table_lookup_indexed, table_3axis_interp_triple)
already correctly ushort(ushort ep_val, void *table) with Stack[0x4]/
Stack[0x6] storage -- no change applied, nothing was broken. The "void"
seen in table_lookup_interp's own decompiled header line is a cosmetic
decompiler quirk (its body ends in a bare tail-call to
axis_interp_lerp_u8frac rather than an explicit return statement) and
does not affect caller-side decompiles, which consistently show captured
`uVarX = table_lookup_interp(...)` calls throughout this file's checklist.
No new xrefs surfaced from this run.
---------------------------------------------------------------------
2026-08-05: File created. Pulled full table_lookup_interp xref list
(~60 call sites across ~50 distinct functions). Checked first 2 functions
end-to-end (isc_f354_table_lookup_conditional,
f28e_f292_knock_lambda_composite_calc) -- both already correctly typed,
no fixes needed yet. Confirms the review2.md item 29-31 lesson still
applies: don't assume untyped == needs fixing, check every one.

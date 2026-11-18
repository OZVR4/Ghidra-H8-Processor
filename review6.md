RETURN-TYPE AUDIT PART 2: remaining untyped (undefined/undefined1/undefined2)
function return types outside the 4-lookup-primitive caller family
====================================================================

GOAL: review4.md audited and fixed return types across the ~60 callers of
table_lookup_interp/axis_lookup_interp/table_lookup_indexed/table_read_indexed
(15 fixes landed, COMPLETE 2026-08-05). That was a narrow, deliberately-scoped
slice. This file picks up the REST of the ROM: ~80 functions across the whole
decompile (RVR_1998_x3 4g63t 21000011 md352553.hex.c, 28721 lines, ~460
functions total) still have `undefined`/`undefined1`/`undefined2` return
types and have never been individually checked.

CRITICAL LESSON CARRIED FORWARD FROM review4.md (do not skip this):
review4.md found that a "low priority, probably already fine" bucket from an
earlier pass was WRONG 2 times out of 3 when actually re-verified. Untyped
does NOT reliably predict whether a function is genuinely void (result only
ever reaches a RAM global, dropped by every caller) or genuinely returns a
real value (ushort/short/byte, captured by at least one real caller). This
project's own conclusion, verbatim: "don't assume untyped == needs fixing,
check every one." Blind batch-retyping is explicitly rejected as a method.

--------------------------------------------------------------------
METHOD (per function, same discipline as review4.md)
--------------------------------------------------------------------
1. Decompile the function itself. Look at the end of the body: does it
   `return <value>;`, `return;` with no value, or fall off the end after
   only writing to RAM globals/struct fields?
2. Find every real caller (get_function_callers / get_xrefs_to on the
   function's address -- not just the ones visible in the same file region).
3. Decompile each caller at the call site. Does it capture the return value
   (`x = fn(...)`) or call it bare (`fn(...);`)? If ANY real caller captures
   it, the function needs a real return type. If ALL real callers drop it,
   void is correct (or the current undefined-but-effectively-void state is
   at least not actively wrong, though it should still be retyped cleanly
   to void for consistency/xref hygiene).
4. Watch for the review4.md-documented decompiler artifacts:
   - bare tail-calls into another function showing as `void` cosmetically
     even though the real return propagates (verify against a REAL captured
     caller, not the function's own header line)
   - malformed `byte [2]` stack/return-overlap quirks (seen on
     wgdc_correction_integrator_update, f0ba_f2b2_threshold_blend_calc in
     review4.md) -- do NOT blind-retype these, flag and skip
5. Record verdict per function: [F] FIXED (retyped, real caller evidence) /
   [x] CHECKED, ALREADY CORRECT (genuinely void or genuinely already typed
   right, re-verified with real caller evidence, not assumed) / [S] SKIPPED
   (decompiler artifact, unsafe to retype, documented why) / [ ] not yet
   checked.
6. Never batch-retype. Never trust a function's own untyped header as
   evidence either way -- only a real caller's capture/drop behavior counts.

--------------------------------------------------------------------
SCOPE NOTE
--------------------------------------------------------------------
This list excludes only functions CONFIRMED by direct text search of
review4.md to already be covered there (verified 2026-08-08, see RUNNING LOG
below -- an earlier naming-pattern guess at additional exclusions was WRONG
and has been corrected). Everything else pulled by this file's regex sweep
is in scope here.

--------------------------------------------------------------------
FULL CHECKLIST (80 functions pulled via regex sweep for
^undefined[124]? name( against the full decompile, 2026-08-08)
--------------------------------------------------------------------
Legend: [ ] not yet checked   [x] checked, correct as-is   [F] fixed this
session   [S] skipped (decompiler artifact)   [R4] confirmed by direct
review4.md text search to already be in scope there, excluded here

[S] weighted_blend_2term_u8frac_div255 (line 2883) -- SKIPPED 2026-08-08:
    zero callers found (ghidra:get_function_callers AND get_function_xrefs
    both return none -- not just no captured-return caller, no caller of
    any kind, no data ref either). Body already computes and returns a real
    `(short)(...)` value from three raw stack parameters
    (in_stack_0000000c, &stack0xfffe+6/8/0xc/0xa) -- classic malformed
    stack/return-overlap decompiler artifact per review4.md's documented
    pattern (same class as wgdc_correction_integrator_update,
    f0ba_f2b2_threshold_blend_calc). Cannot safely retype OR confirm void:
    no real caller exists to check capture-vs-drop against, and the raw
    stack-offset params mean even the decompiled body's own apparent
    return may not reflect real calling convention. Likely dead code or
    reached only via a call pattern this project's tooling doesn't resolve
    (same category of gap as several review5.md SEEDS). Leaving untyped.
    Flag for whoever next builds a broader unreached-function sweep.
[R4] axis_lookup_interp (line 3022) -- confirmed in review4.md text
[S] table_read_word_indexed_f1fc (line 3132) -- SKIPPED 2026-08-08: zero
    callers/refs (get_function_callers and get_function_xrefs both empty).
    Body is clean, no decompiler artifact -- `return *(undefined2*)(_bank +
    (DAT_0001f1fc & 7)*2);`, a genuine single-expression return, would be a
    safe ushort retype IF any caller existed. Notable: this sits right next
    to table_lookup_indexed/table_read_indexed (review4.md's fully-traced
    14-caller family, all real, all in the 0xd8ec-0xdbcc RAM-bank cluster)
    -- looks like a sibling/alternate accessor for the same
    DAT_0001f1fc-keyed dispatch that never got wired to a real call site in
    this ROM revision. Possibly genuinely dead code (consistent with
    review5.md SEED #8's own "genuinely dead/unused ROM data" theory for a
    different but similarly-shaped case). Leaving untyped, no real caller
    to confirm against.
[S] gear_indexed_table_lookup_f1fc (line 3224) -- SKIPPED 2026-08-08: zero
    callers/refs, same as table_read_word_indexed_f1fc immediately above
    (identical shape: `return *(undefined2*)(_bank + (DAT_0001f1fc & 7)*4);`
    -- same DAT_0001f1fc index, different stride (4 vs 2), still a clean
    single-expression return with no artifact).
    FOLLOW-UP 2026-08-08: checked the third family member,
    read_indexed_byte_f1fc (not in this file's checklist -- it already has
    an explicit calling convention so the regex sweep missed it -- but
    same DAT_0001f1fc-indexed shape, byte stride). It DOES have real
    callers (f28e_f292_knock_lambda_composite_calc,
    octane_level_init_or_force_from_gear_table) -- so the "whole family is
    dead" theory from the note above is WRONG. Revised: this looks
    stride-specific, not family-wide -- the byte-stride accessor is live,
    word-stride and dword-stride variants (this function and
    table_read_word_indexed_f1fc) are genuinely unreferenced in this ROM
    revision specifically, not because indexed access itself is unused.
    Consistent with byte-sized data being far more common than word/dword
    arrays in this ROM's calibration tables. Both still correctly left
    untyped/unfixed -- no real caller exists for either to confirm a
    retype against.
[F] peripheral_register_cold_init (line 3849, addr 0x151a2) -- FIXED
    2026-08-08: body is entirely RAM/register writes (peripheral cold-init
    sequence), ends with `return in_stack_0000000c;` where that variable is
    NEVER assigned anywhere in the function -- a garbage/uninitialized
    stack read, classic decompiler-manufactured fake return on a genuinely
    void function. analyze_function_complete's own return_type_warning
    flagged this exact pattern ("verify EAX at RET, do not trust decompiler
    void display") -- consistent with, not contradicting, the caller-based
    method. Sole real caller confirmed: peripheral_cold_init_wrapper_set_ef7a
    (only caller per get_function_callers), calls it bare
    (`peripheral_register_cold_init();`), drops any return value entirely.
    APPLIED in Ghidra via set_function_prototype (dry-run then real):
    undefined2 -> void.
[F] peripheral_register_warm_init (line 3918, addr 0x1528c) -- FIXED
    2026-08-08: same shape as peripheral_register_cold_init immediately
    above -- ends `return in_stack_00000012;`, variable never assigned,
    garbage-stack fake return. Sole real caller status_word_consumer_periodic
    calls it bare, drops return. APPLIED: undefined2 -> void.
[F] compose_status_words_f0f6_fc (line 3976, addr 0x15341) -- FIXED
    2026-08-08: body writes two RAM globals (DAT_0001f0f6, DAT_0001f0fc)
    via a long bitfield-compose chain, ends `return param_1;` -- an unused
    input parameter echoed straight back, not a real computed value (also
    the function's own body never reads param_1 at all). Two real callers
    confirmed via get_function_xrefs (both COMPUTED_CALL):
    periodic_status_compose_and_stall_check (bare `(*compose_status_words_f0f6_fc)();`,
    no args, no capture) and subsystem_unconditional_init_chain (bare
    `(*(code *)0x15341)();` -- same address, decompiler didn't resolve the
    symbol name inline but it's the same function). Both drop the return
    entirely. APPLIED: undefined2 -> void.
[F] compose_status_word_f102 (line 4130, addr 0x15477) -- FIXED 2026-08-08:
    body writes RAM global DAT_0001f102 via a bitfield-compose chain,
    same family as compose_status_words_f0f6_fc above, but with a
    stronger artifact: ends `return unaff_retaddr;` -- Ghidra's own name
    for "this is reading the return-address slot, not a real return
    value," an even clearer decompiler-artifact signal than the previous
    two garbage-stack-read cases. Two real callers confirmed via
    get_function_xrefs (both COMPUTED_CALL): subsystem_unconditional_init_chain
    and tcu_rx_main_scheduler, BOTH call it bare (`(*compose_status_word_f102)();`,
    no args despite the decompile showing 3 params, no capture) at lines
    13387 and 26207 respectively. APPLIED: undefined2 -> void.
    SEPARATE FINDING (not fixed here, flagging for its own pass): this
    function's body calls `latch_flag_read_fede_bit1(param_1, uVar2 & 0xfbff,
    param_3, param_2)` with 4 arguments, but latch_flag_read_fede_bit1's own
    definition (line 5542 in this file) is `bool latch_flag_read_fede_bit1(void)`
    -- a genuine calling-convention/argument-count mismatch, separate from
    any return-type question. Worth a dedicated look, possibly another
    decompiler artifact given how many are already documented near this
    status-word-compose cluster, but not something this pass's method
    (caller capture-vs-drop) resolves -- needs disassembly-level
    verification of what's actually passed at the call site, same
    discipline as review5.md's literal-push tracing.
[F] adc_read_sequence_main (line 4294, addr 0x1556d) -- FIXED 2026-08-08:
    body already has an extensive prior investigation comment (dated
    2026-07-12, unrelated to this pass) confirming 6 of 8 ADC channel
    writes against a MUT RequestID table with real consumers -- this is a
    genuinely important, well-understood RAM-writing function, not a
    trivial one. Ends `return unaff_retaddr;`, same return-address-artifact
    class as compose_status_word_f102 above (Ghidra's own naming makes
    clear it's not a real value). Two real callers confirmed via
    get_function_xrefs (both COMPUTED_CALL): subsystem_unconditional_init_chain
    and sensor_adc_update, BOTH bare `(*adc_read_sequence_main)();`, no
    capture. APPLIED: undefined2 -> void. NOTE for future work: the prior
    investigation comment itself flags 2 of 8 ADC channels (region around
    channels 0/2/9/0xA) as "NOT individually re-checked this pass, do not
    assume correct" -- unrelated to this return-type fix but worth
    surfacing since it's a real open item sitting in this function's own
    docstring.
[F] adc_channel_read_and_store_f16e (line 4372, addr 0x1566c) -- FIXED
    2026-08-08: RAM-only write to 0xf16e, `unaff_retaddr` artifact return.
    Sole real caller subsystem_unconditional_init_chain confirmed bare
    (`adc_channel_read_and_store_f16e();`, line 13382). APPLIED: undefined2
    -> void. Note: function's own name is already flagged in a prior
    triage comment as a "best-effort guess... verify against hardware
    manual before trusting fully" -- unrelated to this return-type fix,
    but a real open naming question left for whoever does hardware-manual
    cross-referencing next.
[F] adc_read_sequence_b (line 4398, addr 0x15689) -- FIXED 2026-08-08:
    already has a prior verified investigation comment (2026-07-12,
    confirms MUT Battery/TPS channel writes against the RequestID table,
    cross-references logging.txt). RAM-only writes, `unaff_retaddr`
    artifact return. Two real callers confirmed: subsystem_unconditional_init_chain
    (bare `(*(code *)0x15689)();` -- resolves to this exact address, matches
    the compose_status_word_f102 pattern of an unresolved-symbol computed
    call) and tcu_rx_main_scheduler (bare `(*adc_read_sequence_b)(0);` at
    line 26428, argument passed but return dropped). APPLIED: undefined2 ->
    void.
[F] adc_read_channel_6 (line 4416, addr 0x156ad) -- FIXED 2026-08-08:
    RAM-only write to 0xf15c, `unaff_retaddr` artifact return, same family
    as the other ADC-sequence functions above. Two real callers confirmed:
    subsystem_unconditional_init_chain and tcu_shift_torque_and_knock_mgmt,
    both bare `(*adc_read_channel_6)();`. APPLIED: undefined2 -> void.
[F] identity_passthrough_u16_b (line 4440, addr 0x156c8) -- FIXED 2026-08-08,
    AMBIGUOUS CASE flagged explicitly: body is `return param_1;` with NO
    decompiler artifact (unlike every ADC/status-word function above --
    this is a genuinely clean identity function, param in, same value out).
    In isolation this looks like a real `ushort identity_passthrough_u16_b(ushort)`.
    BUT: sole real caller (subsystem_unconditional_init_chain, only entry
    in get_function_xrefs) calls it as `identity_passthrough_u16_b();` --
    bare, NO argument passed, return dropped. Per this file's method (real
    caller behavior governs over the function's own apparent body), retyped
    to void since that's what's actually observed at the only real call
    site. Flagging the ambiguity rather than silently picking one: if a
    second caller ever surfaces that DOES pass an argument and capture the
    result, this verdict should be revisited -- the function's own body
    genuinely supports a real ushort(ushort) signature, it's just unused
    that way in this ROM revision. APPLIED: undefined2 -> void (with this
    caveat).
[F] mirror_status_f0e6_to_ports (line 4492, addr 0x156ce) -- FIXED
    2026-08-08: already has an extensive prior investigation comment
    (2026-07-22 session) confirming this mirrors 8 individual RAM flag
    bits (0xf0e6) onto real H8 physical output pins -- one of the most
    well-evidenced port-actuator functions found in this ROM to date
    (genuine hardware pin identities, not just RAM bookkeeping). Ends
    `return param_1;`, param never read in body, same unused-echo pattern
    as the status-word-compose functions above. Sole real caller
    (main_loop, only entry in get_function_xrefs) confirmed bare
    (`mirror_status_f0e6_to_ports();`, line 13278). APPLIED: undefined2 ->
    void. Existing investigation comment (physical pin table, ~20
    untraced xref sites to f0e6 flagged for future work) left completely
    intact -- this fix only touches the return type, not the function's
    documented behavior.
[F] update_port_fe82_indexed (line 4708, addr 0x158a7) -- FIXED 2026-08-08:
    single RAM write to 0xfe82 indexed off DAT_0001eed2, `return param_1;`
    with param never read (unused echo). Sole real caller
    tcu_rx_main_scheduler confirmed -- located the exact call site by
    address (0x158a7) inside that large function's decompile: bare
    `(*(code *)0x158a7)();` at line 26650, no capture. APPLIED: undefined2
    -> void.
[F] serial_fef9_duty_scale_and_set (line 4870) -- FIXED 2026-08-08: `unaff_retaddr`
    artifact return, same family as the ADC/status-word functions above.
    Sole real caller main_loop (line 13279) confirmed bare, no capture.
    APPLIED: undefined2 -> void.
[S] identity_passthrough_arg1_unused (line 4923) -- SKIPPED 2026-08-08: zero
    callers/xrefs anywhere in the program (already flagged as such in an
    existing comment). Body cleanly returns param_1 unchanged, no artifact --
    would be a safe ushort(ushort) retype IF a caller existed. Same category
    as weighted_blend_2term_u8frac_div255 / table_read_word_indexed_f1fc /
    gear_indexed_table_lookup_f1fc above. Leaving untyped, no real caller to
    confirm against.
[F] detect_sci1_param_change_reset (line 5117) -- FIXED 2026-08-08: both
    branches end `return param_1;`, unused-echo pattern (param never read).
    Sole real caller tcu_rx_main_scheduler (line 26778) confirmed bare, no
    args, no capture. APPLIED: undefined2 -> void.
[F] timer_block_cold_init (line 5143) -- FIXED 2026-08-08: RAM-only register
    init writes, `return param_1;` unused-echo. Sole real caller
    peripheral_register_cold_init (line 3899) confirmed bare
    (`(*timer_block_cold_init)();`), no capture. APPLIED: undefined2 -> void.
[F] isr_sci2_eri (line 5207) -- FIXED 2026-08-08: NEW CASE TYPE for this
    file -- a real interrupt handler (ends `rte`, not `prts`/`rts`), reached
    only via the hardware vector table (xref from 0x100e8), not any software
    call site. No software caller exists to check capture/drop by this
    file's normal method, but ISRs are inherently void by H8 interrupt
    calling convention (invoked by hardware trap, return value has nowhere
    to go). APPLIED: undefined2 -> void, on ISR-convention grounds rather
    than caller-capture evidence. Same reasoning applies to isr_sci3_rxi and
    isr_sci3_txi below -- flagging as a new verdict basis distinct from the
    rest of this file's method, worth a second opinion.
[F] timer_ff41_ff44_ff4a_cold_init (line 5232) -- FIXED 2026-08-08: RAM-only
    register init writes, `return param_1;` unused-echo, same shape as
    timer_block_cold_init above. Sole real caller
    peripheral_register_cold_init (line 3900) confirmed bare, no capture.
    APPLIED: undefined2 -> void.
[F] isr_sci3_rxi (line 5404) -- FIXED 2026-08-08: `rte`-terminated ISR,
    vector table xref from 0x100c4, no software caller. Same ISR-convention
    reasoning as isr_sci2_eri above. APPLIED: undefined2 -> void.
[F] isr_sci3_txi (line 5417) -- FIXED 2026-08-08: `rte`-terminated ISR,
    vector table xref from 0x100c8, no software caller. Same ISR-convention
    reasoning as isr_sci2_eri above. APPLIED: undefined2 -> void.
[F] propagate_flag_fe96_to_f0d8_db (line 5600) -- FIXED 2026-08-08: RAM-only
    writes, unused-echo `return param_1`. Two real callers confirmed
    (tcu_rx_main_scheduler line 13386, subsystem_unconditional_init_chain
    line 26204), both bare, no capture. APPLIED: undefined2 -> void.
[F] init_state_block_ff21 (line 5661) -- FIXED 2026-08-08: RAM-only writes,
    unused-echo `return param_1`. Two real callers confirmed
    (peripheral_register_cold_init line 3904, status_word_consumer_periodic
    line 13433), both bare, no capture. APPLIED: undefined2 -> void.
[F] peripheral_block_ff21_warm_init (line 5681) -- FIXED 2026-08-08: same
    ff21-block shape as init_state_block_ff21 above, unused-echo return.
    Sole real caller peripheral_register_warm_init (line 3934) confirmed
    bare, no capture. APPLIED: undefined2 -> void.
[F] isr_adi_adc (line 5746) -- FIXED 2026-08-08: `rte`-terminated ISR,
    vector table xref from 0x100a4, no software caller. Same ISR-convention
    reasoning as isr_sci2_eri/isr_sci3_rxi/isr_sci3_txi. APPLIED:
    undefined2 -> void.
[S] ff2a_set_clamped_to_ff26p2_atomic (line 5759) -- SKIPPED 2026-08-08:
    zero callers/xrefs. Body reads raw stack-offset params
    (&stack0xfffa+10) -- same malformed stack/return-overlap artifact class
    as weighted_blend_2term_u8frac_div255 documented earlier in this file
    and in review4.md. Cannot safely retype or confirm void, no real caller
    to check against. Leaving untyped.
[F] timer_ff51_ff54_arm_init (line 5788) -- FIXED 2026-08-08: RAM-only
    register init writes, unused-echo return, same shape as the other
    timer_*_cold_init/arm_init functions above. Sole real caller
    peripheral_register_cold_init (line 3905) confirmed bare, no capture.
    APPLIED: undefined2 -> void.
[F] peripheral_fec0_fed0_state_update (line 5921) -- FIXED 2026-08-08:
    RAM-only state writes, unused-echo `return param_1`. Sole real caller
    peripheral_register_warm_init (line 3937) confirmed bare, no capture.
    APPLIED: undefined2 -> void.
[F] serial_fec3_load_byte0_init (line 5976) -- FIXED 2026-08-08: RAM-only
    writes, `return param_1` where param_1 is never read in the body. Sole
    real caller tcu_rx_main_scheduler (line 26776) passes a real argument
    (uVar4) but drops the return entirely
    (`serial_fec3_load_byte0_init(uVar4);`). Per method, capture/drop on the
    return governs -- applied void, kept the genuinely-passed param.
    APPLIED: undefined2 -> void serial_fec3_load_byte0_init(undefined2).
[F] serial_fec3_load_next_byte (line 5991) -- FIXED 2026-08-08: RAM-only
    writes, unused-echo `return param_1`. Sole real caller isr_tpu5_tci5u
    (line 6325) confirmed bare `(*serial_fec3_load_next_byte)();`, no
    capture. APPLIED: undefined2 -> void.
[F] serial_fec5_byte_to_fifo (line 6024) -- FIXED 2026-08-08: same shape as
    serial_fec3_load_byte0_init above -- real caller tcu_rx_main_scheduler
    (line 26343) passes an argument (uVar4) but drops the return. APPLIED:
    undefined2 -> void serial_fec5_byte_to_fifo(undefined2).
[F] sci1_tx_response_feeder (line 6103) -- FIXED 2026-08-08: unused-echo
    `return param_1`, extensively documented autonomous SCI1 TX-FIFO feeder
    (see existing plate comment). Sole real caller sci1_protocol_state_machine
    (line 23290) confirmed bare `(*sci1_tx_response_feeder)();`, no capture.
    APPLIED: undefined2 -> void.
[F] init_state_sentinels_f5d2 (line 6332) -- FIXED 2026-08-08: RAM-only
    writes, unused-echo `return param_1`. Two real callers confirmed
    (peripheral_register_cold_init line 3908, status_word_consumer_periodic
    line 13435), both bare, no capture. APPLIED: undefined2 -> void.
[F] isr_ipu_ch2ch4_input_capture (line 6405) -- FIXED 2026-08-08:
    `rte`-terminated real interrupt handler (IPU channel 2/4 input capture,
    extensively documented -- confirmed cam-position/RPM signal chain),
    vector table xref from 0x100c0, no software caller. Same ISR-convention
    reasoning as isr_sci2_eri etc. APPLIED: undefined2 -> void.
[F] isr_tpu3_tgi3b (line 6961) -- FIXED 2026-08-08: `rte`-terminated ISR,
    vector table xref from 0x100f8, no software caller. Same ISR-convention
    reasoning. APPLIED: undefined2 -> void.
[S] sci1_tx_setup (line 7102) -- SKIPPED 2026-08-08: get_function_callers
    returns none -- the single xref (from 0x1389c) is a data reference, not
    a call site (confirmed via search-symbol: no call-site text anywhere in
    the decompile). Body also reads a raw stack offset
    (&stack0xfffc+10) -- same malformed stack/return-overlap artifact class
    as weighted_blend_2term_u8frac_div255 and ff2a_set_clamped_to_ff26p2_atomic
    above. Leaving untyped, no real caller to confirm against.
[x] adc_sensor_convert_single (line 7254) -- CHECKED, ALREADY CORRECT
    2026-08-08: extensively documented in an existing multi-session plate
    comment (MUT RequestID table lookup mechanism, fully traced 2026-07-12
    through 2026-08-07). Comment explicitly explains the undefined2 return
    type is deliberate -- the RequestID parameter arrives in a register
    (R1) rather than on the stack, a known H8 set_function_prototype
    limitation documented elsewhere in this project
    (functions_w4a51.md/boot_ram_block_copy), so a misleading stack-based
    signature was avoided on purpose. Two real callers confirmed
    (mut_configurable_reqid_backup_snapshot_5word,
    mut_configurable_reqid_periodic_snapshot_8word, 13 call sites total) --
    return value is genuinely used. No change needed, re-verified correct
    as-is.
[F] isc_stepper_enable_check (line 7764) -- FIXED 2026-08-08: AMBIGUOUS
    CASE TYPE (different from most of this file): genuine computed
    gate-check, no decompiler artifact -- body legitimately branches through
    a large boolean condition tree and returns 0 or 1 in a register
    (uVar4). NOT a void candidate. Sole real caller isc_condition_eval
    confirmed CAPTURING the result (`sVar3 = isc_stepper_enable_check();`,
    line 7735) into a short. APPLIED: undefined2 -> ushort (matches the
    register-boolean gate-check return convention used elsewhere in this
    project, e.g. the ~60-function table_lookup family retyped in
    review4.md). First real (non-void) retype of this batch.
[F] egr_valve_state_eval_f490 (line 8876) -- see RUNNING LOG batch 4 for
    full detail. FIXED: undefined1 -> void (real computed value, but sole
    caller drops it entirely).
[R4] egr_table_row_lookup_f496 (line 9297) -- confirmed in review4.md text
[F] ign_advance_rpm_zone_enable_check (line 9884) -- FIXED 2026-08-08:
    clean computed gate-check, returns 0/1, no artifact. Sole real caller
    f0e6_bit6_update confirmed CAPTURING (`sVar2 = ...`, line 9870).
    APPLIED: undefined2 -> ushort.
[F] canister_purge_feedback_active_check (line 10023) -- FIXED 2026-08-08:
    clean computed gate-check, returns 0/1, no artifact. Sole real caller
    canister_purge_gear_mode_select_f4a2 confirmed CAPTURING (`sVar1 = ...`,
    line 10011). APPLIED: undefined2 -> ushort.
[F] o2_sensor_gain_divisor (line 10840) -- FIXED 2026-08-08: body reads
    `in_stack_00000004` as if an incoming param, conditionally
    reassigned -- borderline stack-artifact shape, but a real caller DOES
    exist (tcu_rx_main_scheduler, line 26977), calling bare with no args,
    no capture. Return type set to void on that caller evidence; the
    internal in_stack_00000004 usage is a separate question this pass
    doesn't resolve, flagged in case it needs its own look.
    APPLIED: undefined2 -> void.
[F] o2_lambda_correction_apply (line 10907) -- FIXED 2026-08-08: same
    in_stack_00000004 shape as o2_sensor_gain_divisor immediately above,
    but OPPOSITE verdict -- sole real caller
    f28e_f292_knock_lambda_composite_calc confirmed CAPTURING
    (`uVar2 = o2_lambda_correction_apply();`, line 16927). Good contrast
    pair: identical body shape, different caller behavior, different
    verdict -- exactly what the method is for. APPLIED: undefined2 ->
    ushort.
[F] o2_sensor_mode_correction_select (line 10923) -- FIXED 2026-08-08:
    genuine multi-branch value dispatcher (several explicit
    `return <value>;` paths, no artifact). Sole real caller
    o2_closed_loop_fuel_trim_compute confirmed CAPTURING (`uVar1 = ...`,
    line 17192). APPLIED: undefined2 -> ushort.
[F] o2_mode7_8_correction_apply (line 10967) -- FIXED 2026-08-08: same
    in_stack_00000004 shape as o2_lambda_correction_apply. Sole real caller
    f0ba_f2d0_o2_mode7_8_correction_calc confirmed CAPTURING (`uVar1 = ...`,
    line 17861). APPLIED: undefined2 -> ushort.
[F] o2_mode9_10_correction_apply (line 10983) -- FIXED 2026-08-08: same
    in_stack_00000004 shape as the two above, but OPPOSITE verdict again --
    sole real caller fueling_base_injection_calc (line 14515-14516) passes
    a real computed argument but calls BARE, no capture
    (`o2_mode9_10_correction_apply((short)(...));`). Third example this
    batch of the identical-body/different-caller pattern. APPLIED:
    undefined2 -> void.
[S] o2_mode16_lambda_select (line 11014) -- SKIPPED 2026-08-08: zero
    callers/xrefs, same in_stack_00000004 family shape as
    o2_lambda_correction_apply/o2_mode7_8_correction_apply/
    o2_mode9_10_correction_apply above but no real caller to check
    capture-vs-drop against. Leaving untyped.
[F] o2_downstream_correction_apply (line 11188) -- FIXED 2026-08-08: same
    in_stack_00000004 family shape. Sole real caller egr_target_f448_update
    confirmed CAPTURING (`uVar2 = ...`, line 8738). APPLIED: undefined2 ->
    ushort.
[S] o2_mode65_lambda_select (line 11236) -- SKIPPED 2026-08-08: zero
    callers/xrefs, same family shape as o2_mode16_lambda_select above.
    Leaving untyped.
[F] sci1_rx_byte_poll_or_error (line 12493) -- FIXED 2026-08-08: genuine
    multi-branch byte dispatcher, real recursive-family callee
    (sci1_boot_handshake_and_bulk_rx), no artifact. 11 call sites found via
    search-symbol: most capture (`uVar1/uVar2/uVar3 = ...`), 2 call bare
    (lines 12368, 12380). Per method, any capturing real caller means a
    real return type is needed regardless of other bare callers. APPLIED:
    undefined1 -> byte sci1_rx_byte_poll_or_error(byte).
[F] sci1_boot_build_row_addr_frame (line 12752) -- FIXED 2026-08-08: ends
    `return 10;` (literal constant, genuine fixed frame-length value, no
    artifact). Two real callers confirmed CAPTURING (`uVar4 = ...`, lines
    12654/12681). APPLIED: undefined2 -> ushort.
[F] sci1_colon_frame_dispatch (line 12857) -- FIXED 2026-08-08:
    get_function_callers/xref metadata showed empty callers despite 3
    xrefs (indirect/computed calls not resolved into the callers list) --
    confirmed via search-symbol text search that all 3 real call sites
    (lines 11911, 11988, 12003) are direct calls that CAPTURE the result.
    Ends in an indirect jump-table dispatch to a real per-record-type
    handler, return value is that handler's genuine result. APPLIED:
    undefined1 -> byte.
[S] sci1_boot_ihex_colon_record_decode_dispatch (line 12904) -- SKIPPED
    2026-08-08: zero callers/xrefs, already documented in an existing
    triage comment as likely reached only via an unresolved indirect jump
    table (decompiler's own "could not recover jumptable" warning at the
    same 0x206cb site as sci1_colon_frame_dispatch above). No real caller
    to confirm against. Leaving untyped.
[F] check_cold_init_complete_sentinel_ef62 (line 13334) -- FIXED
    2026-08-08: clean multi-condition gate-check (RAM-range/checksum
    sentinel verification), returns 0/1, no artifact. Sole real caller
    subsystem_cold_init confirmed CAPTURING (`sVar1 = ...`, line 13313).
    APPLIED: undefined2 -> ushort.
[F] o2_temp_ready_check (line 15672) -- FIXED 2026-08-08: computed 0/1
    gate-check, no artifact -- BUT sole real caller warmup_mode_state_machine
    (line 15545) calls it bare, no capture. APPLIED: undefined2 -> void
    (caller-drop governs, same as egr_valve_state_eval_f490/
    o2_mode9_10_correction_apply pattern from earlier batches).
[F] warmup_complete_gate_check (line 15847) -- FIXED 2026-08-08: computed
    0/1 gate-check, reads &stack0xfffe+6 but as a genuine passed parameter
    (consistent link-frame usage, not a garbage artifact). Sole real caller
    warmup_mode_state_machine confirmed CAPTURING (`sVar1 = ...`, line
    15548). APPLIED: undefined2 -> ushort.
[F] f21c_bit1_and_struct_bit5_gate_check (line 16322) -- FIXED 2026-08-08:
    computed 0/1 gate-check, in_stack_00000004 used as a real dereferenced
    pointer parameter, not a garbage artifact. Sole real caller
    struct_bit11_toggle_from_gate_check confirmed CAPTURING (`sVar2 = ...`,
    line 16309). APPLIED: undefined2 -> ushort.
[F] f2f0_f2f8_ring_and_f27x_f2cc_periodic_init (line 16478) -- FIXED
    2026-08-08: real computed return (high word of a product, documented
    in existing low-confidence triage comment), but sole real caller
    subsystem_unconditional_init_chain (line 13395) calls it bare, no
    capture. APPLIED: undefined2 -> void.
[F] knock_octane_trim_gate_condition_check (line 17459) -- FIXED
    2026-08-08: clean multi-condition 0/1 gate-check, no artifact. Sole
    real caller knock_octane_trim_gate_dispatch confirmed CAPTURING
    (`sVar1 = ...`, line 17450). APPLIED: undefined2 -> ushort.
[F] o2_closed_loop_mode_selector_via_table (line 17605) -- FIXED
    2026-08-08: extensively documented multi-value dispatcher (enum codes
    1/2/4/8/0x10, consumed as a switch selector by
    f26e_o2_closed_loop_output_blend_via_table per existing plate comment).
    Sole real caller confirmed CAPTURING (`sVar2 = ...`, line 17545).
    APPLIED: undefined2 -> ushort.
[S] f33e_bit11_bit10_hysteresis_gate_check (line 19181) -- SKIPPED
    2026-08-08: zero callers/xrefs. Clean computed 0/1 gate-check, no
    artifact -- would be a safe retype IF a caller existed. Leaving
    untyped.
[F] isc_f4d6_gate_condition_check (line 19554) -- FIXED 2026-08-08: clean
    0/1 gate-check, no artifact. Sole real caller isc_f4d6_gated_offset_calc
    confirmed CAPTURING (`sVar1 = (*isc_f4d6_gate_condition_check)()`,
    line 19544, then compared against 0). APPLIED: undefined2 -> ushort.
    (This was one of the 7 functions wrongly guess-excluded in batch 1's
    naming-pattern mistake, later restored to the checklist -- now
    properly checked and confirmed to need fixing, validating that the
    correction was right.)
[F] isc_f36a_f36c_target_calc (line 19567) -- FIXED 2026-08-09: real computed
    return (`*(undefined2*)(bank+0xf36a)`, no artifact), but sole real
    caller isc_f342_composite_correction_calc calls it bare via
    `(*(code *)0x25a59)();`, no capture. APPLIED: undefined2 -> void.
[F] isc_f37c_ramp_target_calc (line 19605) -- FIXED 2026-08-09: same sole
    caller isc_f342_composite_correction_calc, called bare via
    `(*(code *)0x25aef)();`, no capture. Real computed return dropped.
    APPLIED: undefined2 -> void.
[F] isc_f352_table_lookup (line 19659) -- FIXED 2026-08-09: wrapper around
    table_lookup_interp, same sole caller isc_f342_composite_correction_calc,
    called bare via `(*(code *)0x25bfd)();`, no capture. APPLIED:
    undefined2 -> void.
[F] isc_f36e_f370_target_calc (line 19854) -- FIXED 2026-08-09: real
    computed return, no artifact. Same caller family
    (isc_f342_composite_correction_calc) but OPPOSITE verdict -- this call
    site is `uVar2 = (*isc_f36e_f370_target_calc)();`, genuinely captured.
    APPLIED: undefined2 -> ushort. Good contrast pair with the three
    isc_f342_composite_correction_calc callees immediately above: same
    caller function, four callees, three dropped and one captured --
    exactly the discipline this file's method exists to catch.
[S] f3f0_bit13_14_15_hysteresis_gate_check (line 20645) -- SKIPPED 2026-08-09:
    zero callers/xrefs. Clean computed 0/1 gate-check, no artifact -- would
    be a safe retype IF a caller existed. Same shape as
    f33e_bit11_bit10_hysteresis_gate_check above. Leaving untyped.
[F] isc_stepper_position_reached_check (line 21152) -- FIXED 2026-08-09:
    clean computed 0/1 gate-check, no artifact. Sole real caller
    isc_stepper_state_dispatch confirmed CAPTURING (`sVar2 =
    isc_stepper_position_reached_check();`, line 21206, verified via
    search-symbol against the correct RVR decompile). APPLIED: undefined2
    -> ushort.
[F] isc_f41e_table_lookup (line 21769) -- FIXED 2026-08-09: real computed
    return, no artifact. Sole real caller isc_f402_composite_correction_calc
    calls it with real arguments but bare, no capture
    (`isc_f41e_table_lookup(sVar2,extraout_R1,...,param_3);`). APPLIED:
    undefined2 -> void.
[S] f3f4_bit2_3_4_hysteresis_gate_check (line 21816) -- SKIPPED 2026-08-09:
    zero callers/xrefs, same hysteresis-gate-check shape as
    f3f0_bit13_14_15_hysteresis_gate_check above. Leaving untyped.
[S] f0f8_gated_constant_select_f3f4_neighbor (line 21856) -- SKIPPED
    2026-08-09: zero callers/xrefs. Clean value-select, no artifact, no
    real caller to confirm against. Leaving untyped.
[F] isc_f406_ramp_gate_condition_check (line 22091) -- FIXED 2026-08-09:
    clean 0/1 gate-check, no artifact. TWO real callers confirmed
    (isc_eeca_update_gate_condition_check, isc_f3fa_f3fe_dispatch), BOTH
    capturing (`sVar3 = ...` line 22178, `sVar4 = ...` line 22303, verified
    via search-symbol). APPLIED: undefined2 -> ushort.
[S] isc_table_read_byte_clamped_2eca (line 22353) -- SKIPPED 2026-08-09:
    zero callers/xrefs. in_stack_00000004 stack-artifact shape, same class
    as weighted_blend_2term_u8frac_div255. Leaving untyped.
[S] f17a_load_zone_and_ef96_f1d8_check (line 22742) -- SKIPPED 2026-08-09:
    zero callers/xrefs. Clean multi-value dispatcher, no artifact, no real
    caller to confirm against. Leaving untyped.
[S] phase_dispatch_efa2_f500_f5ee_gate_check (line 22763) -- SKIPPED
    2026-08-09: zero callers/xrefs. Multi-branch dispatcher that also
    propagates extraout_R2 from a callee -- no real caller to confirm
    calling convention against. Leaving untyped.
[S] FUN_00028038 (line 22786) -- SKIPPED 2026-08-09: zero callers/xrefs,
    still unnamed. Out of scope for a return-type-only pass either way --
    no caller to retype against, and the naming question is a separate
    task. Flagging again for whoever does function identification next.
[S] f17a_f13c_load_zone_and_f514_gate_check (line 22808) -- SKIPPED
    2026-08-09: zero callers/xrefs. Clean multi-value dispatcher, same
    shape as f17a_load_zone_and_ef96_f1d8_check above. Leaving untyped.

--------------------------------------------------------------------
RUNNING LOG
--------------------------------------------------------------------
2026-08-08: File created. Pulled full regex sweep of `^undefined[124]? name(`
across the complete decompile (80 matches). First attempt at cross-checking
against review4.md's scope used naming-pattern resemblance (e.g. "isc_" +
"table_lookup"/"target_calc") to guess ~7 additional exclusions -- this was
WRONG. Direct text search of review4.md for those 7 function names
(isc_f4d6_gate_condition_check, isc_f36a_f36c_target_calc,
isc_f37c_ramp_target_calc, isc_f352_table_lookup, isc_f36e_f370_target_calc,
isc_f41e_table_lookup, isc_f406_ramp_gate_condition_check) returned ZERO
matches -- none of them are actually mentioned anywhere in review4.md. All 7
restored to [ ] not yet checked. Only 2 exclusions are now confirmed by
direct text search: axis_lookup_interp and egr_table_row_lookup_f496. This
mirrors review4.md's own lesson almost exactly -- an assumption based on
naming pattern rather than direct verification would have caused 7 real
functions to be silently skipped. No functions individually checked yet
this session (file setup + exclusion-list correction only).
NEXT STEP: begin end-to-end checks starting from the top of the checklist,
same method as review4.md's own opening session (decompile function, find
real callers, check capture-vs-drop at each call site, record verdict).

2026-08-08 (batch 2): checked next 8 functions in list order. 7 fixed
(undefined2 -> void), 1 skipped (zero callers). Notable: isr_sci2_eri,
isr_sci3_rxi, isr_sci3_txi are genuine hardware interrupt handlers (`rte`
terminated, reached only via the vector table, no software caller) -- a
case type not previously seen in this file. Verdict basis for these three
is H8 ISR calling convention (return value has nowhere to go on `rte`), not
caller capture-vs-drop evidence, since no software caller exists to check.
Flagging this as worth a second opinion since it's a different justification
than the rest of the method. All other fixes this batch follow the
established unused-echo / unaff_retaddr-artifact pattern with confirmed
bare, non-capturing sole callers.
NEXT STEP: continue from propagate_flag_fe96_to_f0d8_db (line 5600).

2026-08-08 (batch 3): checked next 6 functions. 4 fixed to void, 1 skipped
(zero real callers, stack-offset artifact), 1 confirmed already correct
as-is (adc_sensor_convert_single -- pre-existing deliberate undefined2 due
to register-param limitation, re-verified not to need change). Notable:
isc_stepper_enable_check is the first REAL (non-void) retype this file has
produced -- a genuine gate-check function with no decompiler artifact,
returning 0/1, captured by its sole caller. Retyped to ushort rather than
void, confirming this file's method correctly distinguishes real-return
functions from the void-artifact majority when the evidence actually
points that way. Two more ISR cases found (isr_ipu_ch2ch4_input_capture,
isr_tpu3_tgi3b), same vector-table/no-software-caller reasoning as the
isr_sci2_eri/isr_sci3_rxi/isr_sci3_txi/isr_adi_adc cases from batch 2.
2026-08-08 (batch 3): checked next 6 functions. 4 fixed to void, 1 skipped
(zero real callers, stack-offset artifact), 1 confirmed already correct
as-is (adc_sensor_convert_single -- pre-existing deliberate undefined2 due
to register-param limitation, re-verified not to need change). Notable:
isc_stepper_enable_check is the first REAL (non-void) retype this file has
produced -- a genuine gate-check function with no decompiler artifact,
returning 0/1, captured by its sole caller. Retyped to ushort rather than
void, confirming this file's method correctly distinguishes real-return
functions from the void-artifact majority when the evidence actually
points that way. Two more ISR cases found (isr_ipu_ch2ch4_input_capture,
isr_tpu3_tgi3b), same vector-table/no-software-caller reasoning as the
isr_sci2_eri/isr_sci3_rxi/isr_sci3_txi/isr_adi_adc cases from batch 2.

2026-08-08 (batch 4, partial): checked egr_valve_state_eval_f490 (line
8876) -- FIXED to void. Notable/subtle case: body genuinely computes a
real 0-3 state value in a register (no decompiler artifact -- clean
`return uVar2;` off real branch logic), which in isolation looks exactly
like isc_stepper_enable_check above. BUT unlike that case, the sole real
caller (egr_sequence_control, line 8860) calls it bare with no capture --
confirmed by reading the caller's actual body, not just xref metadata.
Per method, caller capture/drop governs over the callee's own apparent
body shape, so void is correct despite the computed return value existing.
Worth remembering as a contrast pair with isc_stepper_enable_check: same
"clean computed return" shape, opposite verdict, entirely because of what
the real caller does with it -- exactly the discipline this file's method
is designed to enforce.
NEXT STEP: continue from ign_advance_rpm_zone_enable_check (line 9884)
(egr_table_row_lookup_f496, line 9297, already excluded [R4]).

2026-08-09 (batch 5): checked next 4 functions, all callees of the same
parent function isc_f342_composite_correction_calc (a single isc-composite
worker that calls out to 4 of this file's remaining unchecked functions in
sequence). 3 fixed to void (isc_f36a_f36c_target_calc, isc_f37c_ramp_target_calc,
isc_f352_table_lookup -- all real computed returns, all called bare/dropped
at their respective call sites within the parent), 1 fixed to ushort
(isc_f36e_f370_target_calc -- same parent, but this one call site captures
the result). Good internal contrast case: one caller function, four
different-verdict callees, decided purely by what happens at each
individual call site rather than any pattern in the callees' own bodies.
NEXT STEP: continue from f3f0_bit13_14_15_hysteresis_gate_check (line 20645).

2026-08-09 (batch 6, FINAL): checked the remaining 10 functions, completing
the full 80-function checklist. 3 fixed (isc_stepper_position_reached_check
-> ushort, isc_f41e_table_lookup -> void, isc_f406_ramp_gate_condition_check
-> ushort, all confirmed against real callers -- two of these verified via
search-symbol against the correct RVR decompile file specifically, since
this ROM has multiple similarly-named test decompiles in the project
(evo5, rvr) and grepping the wrong one would give a false positive). 7
skipped, all zero-callers/zero-xrefs cases: two more hysteresis-gate-check
siblings of f33e_bit11_bit10_hysteresis_gate_check, one more
in_stack-stack-artifact case, three clean multi-value/gate dispatchers with
no caller to confirm against, and the still-unidentified FUN_00028038
(flagged again for a future identification pass, separate from this
return-type work). CHECKLIST COMPLETE: all 80 functions from the original
regex sweep now have a recorded verdict ([F]/[x]/[S]/[R4]). Zero remain
unchecked. Final tally across the whole file: majority [F] fixed (mostly
undefined2->void on unused-echo/unaff_retaddr decompiler artifacts, several
genuine undefined2->ushort/byte real-value retypes on gate-checks and
dispatchers with capturing callers), a substantial [S] skipped minority (all
zero-caller/zero-xref cases where no real caller exists to confirm a retype
against -- consistently NOT retyped per this file's core method, left
untyped rather than guessed), 1 [x] re-verified already correct, 2 [R4]
excluded as already covered by review4.md. NEXT STEP (for a future
session, not this file): the zero-caller cohort accumulated across this
whole file (weighted_blend_2term_u8frac_div255, table_read_word_indexed_f1fc,
gear_indexed_table_lookup_f1fc, identity_passthrough_arg1_unused,
ff2a_set_clamped_to_ff26p2_atomic, sci1_tx_setup, o2_mode16_lambda_select,
o2_mode65_lambda_select, f33e_bit11_bit10_hysteresis_gate_check,
f3f0_bit13_14_15_hysteresis_gate_check, f3f4_bit2_3_4_hysteresis_gate_check,
f0f8_gated_constant_select_f3f4_neighbor, isc_table_read_byte_clamped_2eca,
f17a_load_zone_and_ef96_f1d8_check, phase_dispatch_efa2_f500_f5ee_gate_check,
f17a_f13c_load_zone_and_f514_gate_check, FUN_00028038) is now large enough
to be worth its own dedicated dead-code/unreached-function sweep, as
speculated back in the table_read_word_indexed_f1fc note near the top of
this file.

--------------------------------------------------------------------
ZERO-CALLER COHORT SWEEP (2026-08-09, same session)
--------------------------------------------------------------------
Follow-up requested immediately after the checklist above closed out.
GOAL: for each of the 17 zero-caller/zero-xref functions accumulated
across this file, determine via ghidra:get_xrefs_to (address-level,
authoritative -- catches data refs and jump-table entries that a
function-level xref summary can miss) whether each is genuinely dead
code, reached only via an unresolved indirect/computed call, or has some
other reference this pass's per-function checks didn't surface.

METHOD: get_xrefs_to on each function's own entry address. If a real
reference of ANY kind turns up (call, data, jump-table pointer), inspect
it; if truly zero references exist anywhere in the program, dead code is
confirmed rather than assumed.

RESULT: 16 of 17 have ZERO references of any kind anywhere in the
program -- not just no callers, no data refs or jump-table pointers
either. No evidence any of them are reached via an unresolved indirect
call: if that were happening, the jump table's own backing data array
would show as a reference to the function address, and none do. This is
stronger and more direct evidence than the speculative framing used when
each was individually skipped:
  weighted_blend_2term_u8frac_div255, table_read_word_indexed_f1fc,
  gear_indexed_table_lookup_f1fc, identity_passthrough_arg1_unused,
  ff2a_set_clamped_to_ff26p2_atomic, o2_mode16_lambda_select,
  o2_mode65_lambda_select, f33e_bit11_bit10_hysteresis_gate_check,
  f3f0_bit13_14_15_hysteresis_gate_check, f3f4_bit2_3_4_hysteresis_gate_check,
  f0f8_gated_constant_select_f3f4_neighbor, isc_table_read_byte_clamped_2eca,
  f17a_load_zone_and_ef96_f1d8_check, phase_dispatch_efa2_f500_f5ee_gate_check,
  f17a_f13c_load_zone_and_f514_gate_check, FUN_00028038.

1 of 17 (sci1_tx_setup) DOES have exactly one reference: a [DATA] xref
from address 0x1389c. Traced further: 0x1389c itself
(DAT_0001389c, an untyped pointer-sized primitive near the sci1 boot
protocol region already documented elsewhere in this project) has ZERO
inbound references of its own -- nothing reads/dereferences that pointer
slot anywhere Ghidra has resolved. So sci1_tx_setup's address sits in
memory as data (consistent with an unused function-pointer table entry,
as its own existing triage comment speculated), but nothing calls
through it. Same practical verdict as the other 16: no confirmed live
call path.

NOTABLE ASIDE: f33e_bit11_bit10_hysteresis_gate_check's body reads
PTR_vec_trapa4_00010b9e, a name suggestive of the H8 TRAPA4 software-
interrupt vector. Chased this in case it meant the function itself was
ISR-reached (same category as the isr_* functions fixed earlier in this
file) -- but PTR_vec_trapa4_00010b9e is just a calibration/threshold
DATA value the function reads (also read elsewhere by
isc_f356_correction_calc as ordinary data), not a vector-table call
mechanism. Does not change the dead-code verdict for this function.

CONCLUSION: no functions promoted out of the zero-caller cohort. All 17
remain correctly left untyped -- this sweep converts "no real caller
found by this file's per-function method" into "confirmed zero
references of any kind, checked directly," which is a strictly stronger
claim, but the practical outcome (leave untyped, no safe retype target)
is unchanged for all 17. No return-type changes applied this sweep.
Genuinely dead-code removal/documentation (as opposed to return-type
hygiene) is a separate decision outside this file's scope.

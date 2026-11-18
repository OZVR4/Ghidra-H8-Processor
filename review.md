Review document index
====================================================================
This project's review notes span multiple files. This section is the
table of contents -- what each file covers and whether it's still open.
Update this list whenever a new reviewN.md is started or an existing one
is closed out.

- review.md (this file) -- h8539f Sleigh implementation vs ana.cpp.
  OPEN. Items 1, 3, 6, 7, 8, 10, 11 still live here directly; items 4, 9,
  12, 14, 15, 18 are superseded and pointered to the files below; item 13
  is REOPENED (2026-08-17, 7 DAT_ globals remain unrenamed -- the
  2026-08-16 "CLOSED, zero hits" claim was inaccurate).

- review2.md -- RVR XML axis/table address verification (Batch 1/2 +
  appendix items 19-31). OPEN. ~31 items; still-open threads include
  item 30's dispatch-block isolation and ~50+ untyped functions in the
  table_lookup_interp/axis_lookup_interp caller family not yet audited.

- review3.md -- cspec-driven far-pointer call-argument fusion (a real
  Ghidra decompiler C++ patch, the #817-class bug). No longer plan-only:
  implemented and submitted as PR #9522 (issue #9450), an opt-in
  <farpointerjoin hislot joinsize> cspec element + ActionFarPointerJoin.
  Needs a core built with it; the clean master core cannot use it. No in-tree
  datatest yet (the PR offers one). The smoke-test noted in this file may be
  out of date. See todo.md item 11.

- review4.md -- return-type audit: table_lookup_interp/axis_lookup_interp/
  table_lookup_indexed/table_read_indexed caller family. COMPLETE
  (2026-08-05).

- review5.md -- axis identity/scaling verification against live ROM
  callers, picking up from review2.md's address-only pass. OPEN.

- review6.md -- return-type audit part 2, remaining untyped functions
  outside the 4-lookup-primitive family. CLOSED for its own scope (17
  zero-caller functions confirmed correctly left untyped, no further
  action there) -- but review7.md and review8.md both flag follow-up
  work this file didn't cover.

- review7.md -- call-site CONCAT/extraout artifact audit (functions with
  correct return types that still show decompiler noise). OPEN --
  checklist assembled, no functions individually verified yet.

- review8.md -- computed-call state-table dispatch: f502_state_table_
  dispatch and its unresolved handler targets. OPEN -- several named
  follow-ups outstanding (review6.md correction, f502_state_index_update,
  13 of 17 zero-caller functions still unaddressed, a candidate DAT_
  bitfield).

- review9.md -- sci1_colon_frame_dispatch jump table: emulation
  feasibility, the spSegment fix, and why DP still blocks recovery.
  OPEN -- consumer of a fully-decoded flash sector table still unfound;
  two named functions not yet checked.

- review10.md -- axis-pairing verification: the table+2 RAM-pointer
  family and Convention B. OPEN -- named next steps outstanding
  (manual review of 5 flagged tables, optional mode-value sanity check).

- mut_verification_status.md -- full detail backing review.md's item 1
  (MUT actuator-test mapping) and related SCI1/TCU findings. OPEN,
  referenced throughout review.md item 1.

- test/functions_evo5.md, test/functions_w4a51.md -- cross-binary
  function-matching checklists (successor to review.md's old item 12).
  OPEN.

- review11.md -- h8539_import_tables_xml2.py: behavior + implementation plan.

- review12.md -- RVR scaling/axis table verification: header-offset bug + correction batches.

- review13.md -- H8/500 paged/banked addressing issues. (Not the same as item 13 below.)

- review14.md -- TableHeader struct rollout across the RVR table region
  (0x2d080-0x2d8d6) and xref-based verification of the XML's header
  claims. OPEN -- 7 verification TODOs live there.


- review15.md -- register0x0e investigation. RESOLVED 2026-10: #9540/#9541 print-layer fix
  (920fcd6) + RdN and displacement-order language fixes; register0x 210 -> 0. See todo.md item 9.

- review16.md -- register0x0e print-layer fix. COMPLETE (landed 2026-08-26).

- review17-19.md -- auStack_/sat_add_u16 chain. HISTORICAL; cause since found (displacement
  order, RdN pointer store, param_fpN stack offsets). See todo.md.

- review.md item 19 (below) -- DAT_0001f1fe status/DTC-validity flags word
  mapping. OPEN, started 2026-08-17 as part of the DAT_ global renaming
  reopen (item 13).

- review20-24.md, review25-26.md (SUPERSEDED), review-9561-merge-ordering.md,
  review-calling-convention-regression.md -- muldiv_*/#9561 regression chain. Mostly
  SUPERSEDED by the above; read review-9561-merge-ordering.md before archiving.

 - todo.md, todo-r0-r5-displacement-order.md -- current work list (2026-10-26).

 - CHANGELOG.md, h8539f-instruction-table.md -- reference.

Review: h8539f Sleigh implementation vs ana.cpp (IDA H8/500 module)
====================================================================

Scope: h8539f.slaspec + h8539f-branch.sinc, h8539f-arith.sinc,
h8539f-logic.sinc, h8539f-bit.sinc, h8539f-mem.sinc, cross-checked against
source/ida-sdk-main/src/module/h8500/ana.cpp.

Completed items (fixed bugs, resolved bookmarks, retracted false leads) have
been moved to CHANGELOG.md. This file tracks only open/in-progress work.
Compressed 2026-07-23 to current state only -- superseded theories, reverted
trial fixes, and session-by-session narration have been dropped in favor of
final conclusions. Git history has the full story if it's ever needed.

Verified correct
----------------
- Branch/control instructions (JMP/JSR/PJMP/PJSR/BSR/RTS/PRTS/PRTD/RTE/RTD/
  LINK/UNLK/TRAPA/TRAP:VS/SLEEP/BPT/SCB) in h8539f-branch.sinc.
- DADD/DSUB nested MAP3->MAP6 dispatch (traced byte-by-byte against ana.cpp's
  tables[] resolution loop) -- correct.
- arith.sinc: add:g/sub/subx/adds/addx (opcode 4/5/6/20/22), add:q
  (+1/+2/-1/-2), swap, btst (Rs and #imm4), clr/neg/tst/not
  (opcode_special 0x11-0x16), cmp:e/cmp:i, shal/shll/shar/shlr/rotl/rotr/
  rotxl/rotxr, mulxu/divxu, tas -- all match A3/A4/A3tail/A4tail rows.
- logic.sinc: or/and/xor EA forms (opcode 8/10/12), andc/orc/xorc CR8/CR16
  forms (opcode 9/11/13) for the MAP3/MAP4 tail groups that ARE implemented.
- mem.sinc: mov:l/mov:s/mov:e/mov:i/mov:f, ldm.w/stm (with reglist!=0 guard
  matching ana.cpp's Op2.reg rejection), xch.b/xch.w (correctly coexisting
  with mov:g store at the same opcode=18 via mode=10 vs mode=11-15 split),
  mov:g load/store/immediate forms, ldc/stc EA-sourced forms for all six
  control registers (CR8 in {1,3,4,5,7}, CR16=0).
- 0x05/0x0D/0x15/0x1D MAP4-literal-prefix family (aa8/aa16-then-dispatch):
  IS implemented (tst.b/tst.w/clr.b/clr.w via opcode_special gates, plus the
  mov:g.w literal-prefix escape) and confirmed working against the live ROM
  at multiple addresses. See CHANGELOG for the fixed cmp:"g.w"/BNOT bugs that
  came out of this investigation.

Open items
----------

1. MUT Mode5 actuator-test RequestID mapping + physical output pin ID --
   IN PROGRESS, full detail in mut_verification_status.md.
   Confirmed: output_relay_flags_f0e6 (RAM 0xf0e6) is a real 8-bit
   relay/output flag word mirrored to ports by mirror_status_f0e6_to_ports
   (0x156ce).

   BIT NUMBERING CORRECTED (2026-07-26): the two entries below marked
   "bit3"/"bit7" in earlier notes were WRONG -- both were pin-name-correct
   but bit-number-wrong (an old miscount, never independently re-verified
   until the "~20 untraced writer sites" sweep below closed every remaining
   xref and forced a re-check of the full bit map against
   mirror_status_f0e6_to_ports itself). Corrected, full bit map:
   bit0 (isc_f0e6_bit0_update, P1DR.4, knock-gated, not SCI1-reachable),
   bit1 (EGR solenoid, P2DR.7, egr_f0e6_bit1_update -- has two writers, one
   unconditional at cold-init (engine_state_f498_init), the other the real
   RPM/warmup-gated ongoing control (egr_f0e6_bit1_update); the "interaction"
   is just normal init-then-steady-state override, not actually mysterious),
   bit2 (canister purge, P4DR.7, purge_f0e6_bit2_update -- CONFIRMED
   forceable via SCI1 cmd 0xD1, the strongest real actuator-test candidate
   found on this ROM. NOTE 2026-07-26: there is a SEPARATE, still-
   unresolved DUTY-CYCLE purge path too -- canister_purge_duty_calc_f4ac
   (F4AC) -- gated in part by a live TCU serial link (see item 6's TCU
   sub-note below). Not confirmed whether F4AC and this on/off F0E6.bit2
   path drive the same physical solenoid or two different ones), bit3
   (PCDR.3, INVERTED -- NOT EGR, previously
   unmentioned. Two writers found: knock_octane_f0e6_bit3_gate_dispatch
   (RPM/knock-pattern-gated, see below) and warmup_state_f594_f0e6_init
   (NEW 2026-07-26, unconditional set at warmup-state init, same
   init-then-steady-state pattern as bit1) -- plausibly a MIL/check-engine
   or knock-warning lamp), bit4+bit5 (EGR valve position, P4DR.4/PCDR.6,
   egr_f0e6_valve_bits_update, orchestrated by egr_mode_dispatch/
   egr_sequence_control), bit6 (PCDR.7, via dispatcher cmd 0xD2), bit7
   (P1DR.5, ign_advance_f0e6_bit7_update -- previously unmentioned,
   forceable via SCI1 cmd 0xD8, ignition-adjacent per inherited naming,
   unconfirmed). Separately,
   f0e6 bits 8-11 are an unrelated O2 sensor upstream/downstream control
   mode field (via o2_sensor_control_dispatch). bit13 (P6DR.2): CHECKED
   2026-07-23 via xref trace (corroborates a prior 2026-07-22 byte-pattern
   sweep) -- no directly-encoded writer exists anywhere in this ROM
   (mirror_status_f0e6_to_ports reads/forwards it to P6DR.2 faithfully, but
   nothing else touches bit 0x2000). Consistent with this ROM's known
   static-analysis blind spot for bank-prefixed/indirect RAM writes -- not
   proof no writer exists at all, just that none is reachable via a literal-
   address instruction. Low priority to pursue further absent a new lead.
   Separately, SCI1 cmd 0xCC (CORRECTED 2026-07-26 -- was misattributed to
   0xCD in earlier notes; re-verified via direct read_memory at the table
   entry, 0x13770 = 00 FF 20 00) sets f510 bit13 (a different bit13,
   different RAM word, read by o2_upstream_enable_check) -- a fourth SCI1-
   reachable actuator chain, but it doesn't terminate in a physical pin the
   way the other three do. This correction was part of a larger fix: the
   whole cmd_c0_d8_actuator_bit_table CC-D8 region was off-by-one in all
   prior notes (full corrected table and the real f00e/f25a gate mechanism
   now in the sci1_meta_cmd_dispatch_c0_ff plate comment and
   mut_verification_status.md) -- this also retroactively resolved two
   previously-flagged "table anomalies" (the D3 dual-bit entry and the D8
   pattern-break), which turn out to be the same single fact seen from two
   wrong offsets: the real dual-bit entry is D2, not D3, and D8 correctly
   continues an uninterrupted 13-entry bit sweep from CC down to D8 with no
   break at all.

   FIFTH chain found 2026-07-26, and the strongest one yet: cmd 0xCD (f510
   bit12) or 0xCE (f510 bit11) -> check_mode_gate_f510 (0x18ae5) -> forces
   egr_position_target_f494_calc's output (f494) to a fixed ROM calibration
   value -> scaled and written to T6GR1 (ROM 0xff88-89), a REAL, confirmed
   H8 Timer 6 PWM duty/compare register (not an inferred port pin). This is
   the ACTIVE EGR control mechanism on this ROM -- confirmed by reading ROM
   config byte 0x102e4 directly, value is 2, selecting this PWM variant
   over the relay-style two-bit sequence (which is present in the ROM but
   dead code here). Timer 6 is corroborated as a genuinely configured PWM
   channel by its cold-boot init function (period + duty register pair
   both set with matching scale). Strongest physical-actuator evidence
   found in this whole investigation, though the exact vehicle-side wiring
   of Timer 6's output pin is still unconfirmed. Full detail in
   mut_verification_status.md "FIFTH SCI1 ACTUATOR-FORCE CHAIN".

   Separately found 2026-07-26 (not SCI1-related, but same actuator-tracing
   thread, triggered by a question about RAM cell F4CA): a checksummed TCU
   (transmission control unit) serial RX frame inside tcu_rx_main_scheduler
   (0x2aa36) -- F4C4/F4C6/F4C8/F4CA + checksum F4CC, latched into confirmed
   copies F4BC/F4BE/F4C0/F4C2 on a checksum match. Confirmed genuinely
   serial (F4C2 is also written directly by serial_fec5_byte_to_fifo,
   inside the known SCI1/2/3 UART register range). F4BC/F4BE feed directly
   into canister_purge_duty_calc_f4ac (item 1's bit2 purge note above) --
   i.e. canister purge duty is influenced by live TCU-link data, not just
   engine-side state. All cells renamed in Ghidra (tcu_rx_frame_raw_byte0-
   3_*, tcu_rx_frame_checksum_f4cc, tcu_rx_confirmed_byte0-3_*, tcu_rx_
   frame_state/timeout_counter_*); f226_bit7_update_from_f4c2_check
   renamed to tcu_link_confirmed_flag_f226_bit7_update. Full detail in
   mut_verification_status.md "TCU SERIAL RX FRAME FOUND".

3. XML table verification design -- NOT YET IMPLEMENTED. Community EcuFlash
   XML table defs and the ROM byte-pattern scraper are both individually
   fallible (same failure mode as the 0x13898 false positive above), so a
   new Step 5d ("XML Cross-Verification") is designed to sit between XML
   labeling and auto-analysis in h8539_ecu_master_setup_new.py: for each XML
   <table>, recompute the expected header address, run the byte-signature
   check against it, cross-check the XML's declared axis RAM pointer against
   the one actually in the header, run a length-bounded data-body sanity
   check using the XML's own element count, and check for a real Ghidra code
   xref to the header -- plus item 2's table-of-tables catalog pattern as an
   extra corroborating signal. Classify VERIFIED/SUSPECT/REJECTED (agreement
   is corroboration, not automatic truth); only VERIFIED tables get typed
   and protected from auto-analysis. 1D-scalar tables (no header signature
   to check) default to SUSPECT unless otherwise corroborated. Not yet
   wired into the script -- `verify_xml_table()` and the Step 5d call site
   are the next concrete step.

4. 0x20843 reachability -- superseded. Full investigation (isolation tests,
   SleighDebugLogger breakthrough, CP-context lead, #817 corroboration) now
   lives in review9.md.


5. RPM identity -- FINAL STATE (2026-07-15). Best candidate: P54/T2IOC1,
   polled by isr_ipu_ch2ch4_input_capture (0x168e3, renamed from an earlier
   isr_sci3_eri mislabel -- it touches zero SCI/serial registers). Structural
   evidence (simple single-edge period-delta, no missing-tooth sync logic,
   unlike the confirmed crank ISR isr_tpu3_tgi3a which does implement
   missing-tooth sync) favors this being the 2-tooth camshaft position
   signal rather than crank-angle, giving 1 pulse per crank revolution (a
   real if coarse RPM-equivalent). Two independent downstream consumers
   confirmed, corroborating a genuine engine-speed signal: F5DE -> ... ->
   engine_torque_pct_scale_calc (0x2184b), and F5D6/F5D8 ->
   tcu_shift_ratio_buffer_update (0x2c16e, via tcu_periodic_dispatch
   0x2c12b). This is inference from ISR structure, not a proven pin-level
   trace -- no connector-pin-to-H8-chip-pin schematic exists for this ECU,
   so board-level wiring confirmation is closed at this confidence level;
   do not resume searching for one. Open: absolute RPM scale/units (timer
   tick rate + the 0x10 divisor's real-world meaning) not yet derived.

6. SCI1 command dispatcher (sci1_meta_cmd_dispatch_c0_ff, 0x28869) -- its
   0xC0-0xD8 actuator bit-table (ROM 0x13740) is decoded. Confirmed:
     - f516 bits 2/5/6/8 (cmds C3/C4/FA) -> f516_request_latch_f09a_calib_
       select (0x27e1a), a calibration selector for output f09a.
     - f516 bit7 (cmd FC) -> eee0_eeee_diag_flags_reset_dispatch (0x282c1)
       -> startup_phase_reset_eed6_eefa_bulk (0x28413) -- a live, on-demand
       trigger, not just a startup path. Independently corroborated by the
       user-supplied XML's own "FC = Clear EFI Diagnostic Codes" label.
     - f516 bit11 -> f588_duty_gate_f516_bit11_set (0x28b62), inside
       sci1_periodic_phase_dispatch_f526 (0x28b2f). That parent function has
       a genuine decompiler jump-table reconstruction failure
       (halt_baddata()/"Could not recover jumptable" on switch cases
       0/2/4/6) -- a real open item on its own merits, unrelated to the
       page-banking work in item 9.
     - f516 bit1/bit12 writer -- CLOSED (2026-07-26): search_byte_patterns
       for the literal address bytes across the whole ROM found 27 touches
       of RAM f516 (vs 10 via plain xref lookup; the rest were individual
       instructions in already-known multi-branch functions, plus a
       previously-undefined jump-table case (0x28cc8) now disassembled and
       confirmed not to touch f516, plus one false positive at 0x232d0 --
       coincidental bytes inside an unrelated pointer table, ruled out by
       reading the table directly). None of the 26 genuine hits set bit1 or
       bit12. Same conclusion as bit13 above: closed as "no writer reachable
       via literal addressing," not "confirmed no writer exists" -- the
       ROM's bank-prefixed/indirect blind spot still applies. Full detail
       and method in mut_verification_status.md.
   FIXED (2026-07-23): sci1_periodic_phase_dispatch_f526's body was wrongly
   absorbing 0x2800c-0x28b51 (2.9KB before its own entry at 0x28b2f) due to
   a stale computed-jump xref from the switch at 0x28b50 -- same root cause
   as the earlier sci1_boot_switch_case4_frag_20640/0x20640 fix (see git
   history). Verified via the actual jump-table bytes at 0x28b52 (8 entries,
   all in 0x28xxx, matching the plate comment) that 0x2800c was never a real
   target. Deleted and recreated the function at 0x28b2f; body is now
   correctly 0x28b2f-0x28b51 (79 bytes), matching the 8-entry switch. The
   freed-up region 0x2800a-0x28081 turned out to contain two real,
   previously-absorbed functions (FUN_0002800a, FUN_00028038/now
   FUN_00028082 after further splitting) -- see new item 11 below.
   FUN_0002f490 CHECKED 2026-07-23: NOT an orphaned duplicate function --
   it's unprogrammed 0xFF flash filler (64 bytes read, all 0xFF), same
   category as the other confirmed-blank regions in item 11. The stale
   xref from the switch at 0x28b50 pointing to it is a leftover of the
   exact same stale-computed-jump-xref bug already fixed for this switch
   (0x2800c) -- the real jump table at 0x28b52 (read directly, 8 words) only
   targets 0x28b62/8b89/8ba8/8c5a/8c78/8c83/8cba/8cc8, all in 0x28xxx, none
   at 0x2f490. Closed -- no action needed beyond noting the stale xref if a
   future cleanup pass touches this switch's xref table.
   Open: any reader for f510/f512, physical meaning of any bit, the <0xC0
   pointer table, and SCI1-vs-SCI3 reachability. Full detail
   in mut_verification_status.md.

7. MUT BLANK/actuator sweep -- mostly resolved, full detail in
   mut_verification_status.md. Confirmed: EECA/EECC (mode-shadow copies,
   isc_eeca_eecc_eece_correction_calc), F3FB/F3FD (real low bytes of
   F3FA/F3FC, not scraper artifacts), F971/F972 (real low/high bytes of
   F970/F973, closing that cluster). Reusable technique for whoever
   continues this: per-address xref search on BLANK cells is a dead end
   (known indirect/bank-prefixed blind spot) -- search from the PRODUCER
   side instead (search_functions on "calc"/"duty"/"target"/"stepper",
   cross-check write addresses against the table), prioritizing BLANK rows
   adjacent to already-CONFIRMED cells.
   Separately (EFI/TCU XML cross-check): ReqID 0x1C/F187 = ECULoad (was
   BLANK, now fixed); ReqID 0x00 has a third conflicting claim (gmas/Airflow
   vs LoadMUT2Byte) on top of an existing GalantLegnum conflict, likely
   genuinely ROM-revision-dependent, not a single stable value. NEW SCOPE,
   NOT YET WORKED: the TCU-side XML has its own 13-entry MUT RequestID
   scheme (AT_ID1-3, AT_Temp, gear states, shaft-speed pulses, AT_Brake,
   AT_ClearDTC), entirely separate from every tcu_* function traced so far --
   no TCU-side equivalent of adc_sensor_convert_single/the 0x2fad0 table has
   been located yet.

8. Unify H8/520, H8/538, H8/539F onto one shared instruction-decode core --
   confirmed via IDA's ana.cpp (a single generic H8/500-family decoder that
   never branches on chip model) that the full 63-instruction ISA and
   control-register set are identical across all three; only the memory map
   and peripheral set differ, and that's already isolated per-chip in each
   .pspec. Current state: h8539f.slaspec is the mature, actively-verified
   implementation; h8520.slaspec is a fully independent 1827-line grammar
   with the same CR8/CR16 unguarded-fallback bug h8539f already fixed, and
   no `sleep` instruction at all; h8538f.slaspec is an empty stub, not
   usable as-is. DECISION: hold off on porting until h8539f's own open items
   are resolved, then factor the branch/arith/logic/bit/mem .sinc files out
   as a shared core with each chip keeping only a thin wrapper .slaspec plus
   its own .pspec -- this retroactively fixes h8520's divergence rather than
   hand-porting each fix individually. Intentionally out of scope for the
   port itself: which operating mode (minimum/maximum) the real H8/538 and
   H8/520 target ROMs actually run in (doesn't affect the shared decode
   grammar, only each chip's .pspec context-register defaults); a full
   instruction-by-instruction completeness audit of h8520.slaspec against
   h8539f's .sinc files (not yet done -- the datasheet says the ISA is
   identical, but that's not been mechanically verified).

9. SP24/FP24/DP/EP page-register banking vs Ghidra's stack-frame analyzer --
   superseded. Full investigation (SP/FP fix, EP/DP constresolve limits,
   R0:R1 revert) now lives in review9.md.

10. "Unable to track spacebase fully for stack" warning -- STATUS: real
    mechanism identified via live pcode inspection; no fix implemented yet;
    not upstream-filing material (same general class as Ghidra's own x86-16
    issues #817/#5199).

    Affects at least 18 structurally-unrelated functions: isr_sci1_eri,
    isr_tpu5_tci5u, isr_tpu5_tgi5a_sci1_rx_poll,
    isr_tpu5_tgi5b_sci1_tie_clear, axis_interp_lerp_u8frac,
    axis_lookup_interp, div_s32_s16_rounded, div_u16_rounded, div_u16_sat,
    div_u32_u16_rounded, div_u32_u16_sat, muldiv_s16_rounded,
    muldiv_s16_rounded_3op, muldiv_u16, muldiv_u16_scaled,
    f26e_cluster_accumulator_unrelated_to_ihex, sci1_boot_build_data_frame,
    weighted_blend_u8frac. Ruled out as causes: the R0:R1 cspec work (item 9
    above); EP/DP dynamic banking (only 3 of the 18 touch DP/EP dynamically
    at all -- the other 15, including isr_sci1_eri, don't); which return
    instruction is used (rts/prts/rte all appear on both warned and clean
    functions); default_proto vs an explicit calling convention (three
    default_proto, rte-ending sibling ISRs -- isr_sci2_eri, isr_nmi,
    isr_adi_adc -- decompile perfectly clean).

    MECHANISM, confirmed against Ghidra's actual decompiler source
    (coreaction.cc, ActionStackPtrFlow::analyzeExtraPop / StackSolver): the
    analysis only runs when a function's prototype has extrapop="unknown"
    (true for anything on default_proto in this cspec). It builds a system
    of linear equations from every stack-pointer-derived varnode, including
    CPUI_INDIRECT ops at call sites to callees whose own extrapop is
    unresolved (which get a guessed equation, not a derived one). The
    warning fires per-unsolved-variable when that system can't be solved.

    RE-DIAGNOSED VIA LIVE PCODE (2026-07-23), isr_sci1_eri specifically:
    Instruction.getPcode() on the raw SLEIGH output (before any decompiler
    analysis runs) for "stc.b EP,@-SP" shows:
      SP = SP - 1                                     (INT_SUB)
      ptr24_tmp = CALLOTHER(spSegment, TP, SP)         (unique, 4 bytes)
      tmp = COPY(EP)
      STORE(ram, ptr24_tmp, tmp)
    and the identical shape recurs for every push/pop in the function --
    both pjsr-embedded pushes and all four rte-embedded pops -- eight
    occurrences, zero exceptions. SP's own storage is clean (plain
    INT_SUB/INT_ADD, one of the solver's recognized shapes), but the
    STORE/LOAD *address* -- what the solver actually needs to resolve, to
    alias each access back to a stack slot -- is CALLOTHER(spSegment, TP,
    SP) in every case, and CALLOTHER isn't one of the solver's 5 recognized
    op shapes (INT_ADD/COPY/INDIRECT/MULTIEQUAL/INT_AND). This is the
    concrete, per-address confirmation of the general mechanism above, for
    this specific function.

    UNTRIED FIX CANDIDATE: TP is a tracked_set context register with a
    compile-time-constant default of 0. spSegment is declared only as a
    bare `define pcodeop spSegment;` (h8539f.slaspec line 323) with no
    <callotherfixup>/pcode injection defined anywhere in the .pspec or
    .cspec -- nothing currently tells the decompiler how to expand it.
    Adding a callotherfixup that expands spSegment(TP, SP) to concrete
    arithmetic (e.g. zext(SP) when TP==0, falling back to the full segmented
    form otherwise) would make the address transparent to constant folding
    and the spacebase solver in the common case, without any upstream
    Ghidra change. NOT YET IMPLEMENTED OR TESTED -- this is the next
    concrete step if pursued.

    Treated as cosmetic/expected in the meantime: decompiled output is
    still numerically correct on every function checked so far (e.g.
    isr_sci1_eri's own body decompiles to a correct, if warning-flagged,
    call+return). Revisit filing upstream only if the callotherfixup
    experiment surfaces something not already covered by #817/#5199, or if
    a function's decompiled output is ever found to be actually wrong (not
    just warned).

11. Function-boundary / undefined-code-gap sweep (2026-07-23) -- prompted by
    the item 6 fix above. Used find_code_gaps (84 gaps ROM-wide initially) and
    manually inspected raw bytes at each gap before acting -- size/heuristic
    flags alone were not reliable (many "orphaned instruction" gaps were
    genuine 0xFF erased-flash filler or repeating-constant data tables, not
    missed code; conversely many "no orphaned instructions" gaps were real
    unbound code). Also checked for mid-function prts/rte occurrences
    ROM-wide (search_byte_patterns on 1119 and 0a0a) as a possible signal of
    wrongly-absorbed function boundaries elsewhere -- none found; every real
    rte/prts hit landed exactly at a correct function's tail. The apparent
    extra 0a0a hits were inside a data table (0x12995, an arithmetic
    progression 00/0a/14/1e...), a reminder that raw byte-pattern search
    needs manual byte inspection before acting, same lesson as item 3.

    22 new functions created and verified this session (each: dry-run,
    create, get_function_by_address to confirm sane non-overlapping bounds):
      FUN_0002800a (0x2800a, 46b), FUN_00028082 (0x28082, 60b, formerly
        FUN_00028038's absorbed tail), FUN_00027fb1 (0x27fb1, 52b),
      FUN_00024cd2 (0x24cd2, 36b) -- also caused Ghidra to auto-discover
        FUN_00024d36 immediately after it,
      FUN_00014079 (71b), FUN_000140fc (47b), FUN_0001431f (77b),
      FUN_000143e8 (69b), FUN_00014502 (9b), FUN_0001460d (72b),
      FUN_00014838 (28b), FUN_0001497b (44b, gap RESOLVED 2026-07-23 --
        see below),
      FUN_000164f8 (13b), FUN_00016505 (13b, second small function found
        immediately after FUN_000164f8), FUN_00016786 (63b),
      FUN_0001a146 (22b), FUN_0001a2d0 (22b), FUN_00022898 (85b),
      FUN_000254dd (103b), FUN_0002561d (14b), FUN_00026982 (135b),
      FUN_00027644 (143b, gap RESOLVED 2026-07-23 -- see below),
        FUN_000278ef (68b), FUN_00027bc6 (37b).
    Gap count dropped 84 -> 68 after this pass, all as expected shrinkage
    (no new gaps appeared elsewhere -- ROM-wide total is stable).

    Confirmed genuine 0xFF filler / real data tables, NOT missed code (left
    untouched): the isr/flag-check cluster's small 1-15 byte gaps
    (0x1519d, 0x175ef, 0x193ec, 0x185f3, 0x19191, 0x1a2fc, etc.), the boot
    region's 0x20d42/0x20de3/0x21edf/0x22f92/0x24e5d/0x260e5/0x28e90/
    0x28f23/0x2a930/0x2b832, and the three huge regions 0x10152-0x12170
    (repeating f9c4/f9ba-style pointer table), 0x1a49a-0x1ffff, and
    0x2cee4-0x2ffff (both confirmed 0xFF filler/reserved flash space).

    NOT YET RESOLVED, flagged as higher-risk (real, varied, non-filler
    bytes present but deliberately not touched given this region's
    documented fragility -- see item 4's BUG 5 class of issue):
      0x20208-0x202b1 (170b), 0x2031e-0x2036c (79b, borders the
        scratch_pjmp_demo experimental stub), 0x20747-0x20771 (43b),
      0x2077d-0x2077e (2b) -- all inside/adjacent to
        sci1_boot_ihex_colon_record_decode_dispatch's boot-record parsing;
      0x20134-0x2014b (24b) and 0x20a14-0x20a7f (108b) -- ambiguous,
        short/irregular byte patterns that could be small data tables
        (flash constants) rather than code, not confidently classified
        either way.
    RESOLVED 2026-07-23: 0x149a7-0x149c7 (33b) and 0x276d3-0x276fa (40b),
    the two residual slivers left by the two partial fixes above -- same
    dry-run-create-verify method as the rest of this pass. Both raw byte
    reads showed varied, non-repeating bytes (not filler); both dry-run
    creates produced clean, exact-size bodies with no overlap warnings;
    both decompile cleanly with no decompiler garbage/warnings:
      gear_indexed_table_lookup_f1fc (0x149a7, 33b) -- gear-indexed table
        lookup (uses f1fc, the same gear-index register as the SCI1
        dispatcher's EC-EF case), reads *(stack_param + (f1fc & 7)*4).
      f0f8_gated_constant_select_f3f4_neighbor (0x276d3, 40b) -- small
        mode-selected constant picker gated on f0f8 bits 4/0, structurally
        similar to the neighboring f3f4_bit2_3_4_hysteresis_gate_check
        (0x27644).
    Neither has any incoming xref yet (get_xrefs_to: none found for either)
    -- same situation as several of the other 22 functions from this pass,
    likely reached via computed/indirect calls given FUN_000149a7's own
    gear-index-table-lookup shape. Not itself concerning; call-site tracing
    not yet done for either, low priority given the physical meaning of
    f1fc/f0f8 in this context is already established elsewhere.

    TO CONTINUE: same method (read real bytes first, dry-run create,
    verify bounds against neighbors) -- do NOT run Ghidra's built-in
    "Function Start Search" analyzers over this region; per manual
    inspection during this session they add nothing for already-defined
    code and risk re-triggering the same heuristic mis-splits that caused
    the original absorption bugs (item 6, and originally
    sci1_boot_switch_case4_frag_20640/0x20640).

----------------------------------------------------------------------------------
12. Cross-binary function matching -- superseded. Full checklists and
    ongoing matching work now live in test/functions_evo5.md and
    test/functions_w4a51.md.

13. DAT_ global address renaming -- REOPENED (2026-08-17): the 2026-08-16
    "CLOSED, zero hits" claim below was WRONG. Live list_globals(filter="DAT_")
    just returned 7 remaining unnamed globals: DAT_0000f00e, DAT_0000f012,
    DAT_0000f014, DAT_0001f1fe, DAT_0000f38c, DAT_0000f3cc, DAT_0001ef90.
    Do not trust "zero hits" without re-running list_globals live first.
    NOT YET INVESTIGATED this session -- next step is the same method as the
    batches below (get_bulk_xrefs under both bank-prefix forms, decompile the
    owning function, name from confirmed behavior, no blind guessing).

    Prior (inaccurate) closure note retained for its batch history below:
    Two batches:
    - 0x1ef04/0x1ef0c/0x1ef0e/0x1ef1c: a 6-slot TCU gear-ratio rolling
      history buffer (init sentinel 0x8000/0, shifted in from F5FA-F610 on
      each shift event) plus a status/latch word, both consumed by
      tcu_gear_shift_history_reset (0x2b880) and tcu_gear_ratio_calc
      (0x2cac4). Named g_tcu_gear_ratio_hist_slot0_lo/slot2_lo/slot2_hi/
      status_flags.
    - 36 addresses across 0x0000ee82-0x0000eff2: traced via get_bulk_xrefs
      (both bank-prefix forms checked per the project's own documented
      blind spot) to their owning functions and decompiled for confirmed
      behavior before naming -- no blind guessing. Covers: knock-event
      counter and octane-level MUT output (eec6/eec8, cross-verified
      against knock_event_counter_update_eec6 and
      octane_level_compute_from_knock_counter's existing plate comments);
      ISC stepper reload state (eed0/eed4, matched to the existing
      isc_stepper_position_eed0 name already present at the 0001-prefix
      form); the F09A calibration-select latch chain (eeea/ef6e, per
      f516_request_latch_f09a_calib_select's plate comment); TCU
      torque-converter-slip flags (eedc/eef8/eefa); RPM-capture-ISR
      per-channel settle counters (ef66/ef68, isr_ipu_ch2ch4_input_capture);
      and a ~20-address cluster entirely inside tcu_rx_main_scheduler
      (scheduler tick counters, ramp trackers, status gates, and the TCU
      RX-frame timeout/reinit state already documented in that function's
      plate comment). Full address-to-name mapping is in the Ghidra
      project itself (list_globals), not duplicated here.
    Two names (g_tcu_rx_sched_state_ee84, g_tcu_rx_sched_state_ef80) are
    lower-confidence -- both confirmed to live inside tcu_rx_main_scheduler
    but the decompiler folded their specific access pattern away from a
    plain-text DAT_ee84/ef80 reference, so the exact per-field role
    couldn't be pinned down the way the others were. Accurately located,
    generically named. Revisit via live pcode/disassembly (not decompiler
    text) if higher precision is ever needed here.

14. DP-banked CONCAT decompiler noise -- concluded done, but related
    return-type/CONCAT cleanup work continues in review4.md, review6.md,
    and review7.md.

15. Prototype-pass CONCAT/extraout_/unaff_ regression -- superseded. Full
    investigation and remaining call-site CONCAT/extraout artifact audit
    now live in review7.md.


ADDENDUM to item 4 -- superseded. The 0x20843/SP-segmented-EA/CP-context
investigation (isolation tests, SleighDebugLogger breakthrough, #817
corroboration) now continues in review9.md.

----------------------------------------------------------------------------------
17. Idle Stepper Look-up Table XML address bug -- FIXED (2026-07-27). Both
    duplicate XML entries declared address="12992", which pointed at an 8-byte
    runtime lookup-context header (mode byte + 2 RAM pointer refs), not the
    real data grid -- confirmed via idle_stepper_table_lookup_wrapper
    (0x268e2) and a direct ROM read. VERIFIED FIX: the real 8x27 ISCVSteps grid
    starts at 0x1299a (12992+8); both XML entries corrected, logged in
    CHANGELOG.md. PRECEDENT for later items: this "mode byte + RAM pointers"
    header-record shape (seen at 0x11ef0, 0x12992, 0x2d5be/0x12a72) is the
    standard calling convention for the whole table_lookup_interp/
    table_3axis_interp_triple family, not a bug -- a neighboring table (WGDC
    Correction vs TPS, 12a72) uses the same shape correctly and needed no fix.
    Only the Idle Stepper table needed its XML address adjusted; do not
    blanket-apply the same +8 offset elsewhere without individually confirming
    corrupted EcuFlash rendering first.

    RELATED, SAME SESSION: investigated table_3axis_interp_triple (genuine
    3-argument trilinear primitive, distinct from EcuFlash's "3D" table-type
    label) -- only ONE real call site chain found (via
    f250_f252_channel_snapshot_update -> engine_periodic_correction_master_dispatch),
    passing table pointer 0x1ef0 (EP-banked, physical bank not yet resolved) and
    secondary arg 1. NOT YET RESOLVED: the table's first mode byte read via
    `cmp:g.b #0x3,@R4` in the function's disassembly determines whether this call
    takes the true 3-axis path or a simpler 2-point linear-interp fallback --
    determining which requires resolving the live EP value at the call site
    (0x24a3f), not yet done. An earlier same-session flat-address read at
    0x11ef0 assuming no banking is RETRACTED as unreliable -- do not reuse those
    byte values. Also retracted: a mistaken link between this table and the
    unrelated SCALING_OUT_F0CA label (that label comes from a different,
    already-existing scanner-found scaling table elsewhere in ROM, not from this
    one). TO CONTINUE: trace back from 0x24a3f to resolve the actual EP/bank
    value in effect, then re-read the table header correctly banked before
    concluding anything about its real axis/mode structure.

18. table_lookup_interp parameter binding + call-site argument visibility --
    superseded. Full investigation, fixes, and the cspec-driven far-pointer
    call-argument fusion plan now live in review3.md.

19. DAT_0001f1fe status/DTC-validity flags word -- OPEN, started 2026-08-17.
    Picked as the highest-traffic item among the 7 DAT_ globals reopened in
    item 13 above (60+ xrefs across ~40 functions when this work began --
    fueling, ISC, ignition, O2 sensor, EGR, knock/octane, DTC status).

    CONFIRMED (via live decompile, this session):
    - bit0, bit1, bit2, bit3: each an independent fault/validity condition,
      confirmed by dtc_status_flags_f204_f206_build (0x1a36f) which ORs each
      bit directly into a local usDtcBits fault bitfield alongside IAT/
      coolant/TPS-load-out-of-range checks and an f500==0 check, then maps
      usDtcBits through a large set of OR conditions into the real DTC output
      words DAT_0001f204/DAT_0001f206. Exact per-bit physical meaning (which
      sensor/condition each of bit0-3 corresponds to) NOT YET traced back
      further -- next step is finding what SETS each of these 4 bits.
    - bit3 additionally gates fueling logic directly: confirmed via
      f1fe_bit3_clear_and_f1f2_bit4_set_check (0x242c1), which returns true
      only when f1fe bit3 is clear AND g_status_flags_f1f2 bit4 is set --
      i.e. bit3 has at least two independent consumers (DTC builder +
      fueling gate), not a single-purpose flag.
    - bit5: confirmed via f1fe_bit5_clear_reset_f09a_check (0x28151), part of
      f502_state_table_dispatch state 9 (already documented, see review8.md
      and the H8StateTableDispatchAnalyzer batch). Returns bit5-clear as a
      bool; when bit5 IS set, also clears DAT_0001f09a -- cooperates with
      state 8's f1fe_f00c_timer_calc_gate_check, which only READS f09a,
      suggesting a shared timer/counter reset handshake between these two
      dispatch states.
    - bit1, bit6: referenced by name only so far (f1fe_bit1_clear_check
      @0x28192, f1fe_bit6_clear_check @0x281a6) -- not yet individually
      decompiled/confirmed this session.

    NOT YET DONE:
    - Find the writer(s) for DAT_0001f1fe itself (get_bulk_xrefs across both
      bank-prefix forms per this project's documented blind spot, or grep the
      .c dump per review.md's SESSION 2026-08-07 methodology notes -- do not
      trust get_xrefs_to alone).
    - Decompile the remaining ~35 unchecked caller functions from the full
      xref list (isc_stepper_enable_check, o2_upstream/downstream_enable_check,
      egr_sequence_control, ign_advance_table_lookup, isc_integrator_update,
      manifold_heat_enable_flag_update, map_tps_axis_update, iat_axis_update,
      and others -- full list was captured via get_xrefs_to(0x1f1fe) this
      session, not yet copied in here to avoid duplicating a live query;
      re-run get_xrefs_to if this list needs regenerating).
    - Once enough consumers are traced, name the individual bits (per-bit
      renamed constants or a plate comment bit-map on the global itself,
      matching the style already used for g_status_flags_f0e6 in item 1
      above) and rename DAT_0001f1fe to a real global name.
    - The other 6 DAT_ globals from item 13's reopen (f00e -- 9 xrefs all in
      sci1_meta_cmd_dispatch_c0_ff; f3cc/f38c -- 18 xrefs each, both in
      tcu_shift_torque_and_knock_mgmt/tcu_shift_timing_state_machine; f012/
      f014 -- low traffic; 0001ef90 -- not yet checked) still need the same
      treatment once this one is closed out.

----------------------------------------------------------------------------------
Build/test workflow
----------------------------------------------------------------------------------
Use this every time a .sinc/.slaspec grammar change needs compiling and
testing against the live ROM. No need to re-ask for these paths again.

- Compiler:  C:\Users\j.brophy.CORKILLSYSTEMS\Downloads\ghidra_12.0.4_PUBLIC\support\sleigh.bat
- Source (edit here):    C:\Users\j.brophy.CORKILLSYSTEMS\github\Ghidra-H8-Processor\h8\data\languages
- Ghidra install target: C:\Users\j.brophy.CORKILLSYSTEMS\Downloads\ghidra_12.0.4_PUBLIC\Ghidra\Processors\h8\data\languages

Steps:
1. Edit the .sinc/.slaspec files in the github source folder (never edit the Ghidra
   install copy directly -- it gets overwritten/regenerated).
2. Compile via run-command:
   & "C:\Users\j.brophy.CORKILLSYSTEMS\Downloads\ghidra_12.0.4_PUBLIC\support\sleigh.bat" "C:\Users\j.brophy.CORKILLSYSTEMS\github\Ghidra-H8-Processor\h8\data\languages\h8539f.slaspec"
   Exit code 0 = success (WARN lines about unreferenced tables/NOP constructors are
   pre-existing and expected -- only ERROR lines or a nonzero exit code mean the
   grammar actually broke). This produces h8539f.sla next to the .slaspec source.
3. Copy ALL changed language files into the Ghidra install, not just h8539f.sla --
   BUG FOUND 2026-07-21: copying only the .sla left a stale .pspec installed (the
   .pspec/.cspec/.sinc files are read separately at runtime, not compiled into the
   .sla), which silently made a source-level fix (a new <segmentop> block) invisible
   in the already-open Ghidra project even after reanalyze/close/reopen -- wasted a
   full test cycle before the mismatch was caught by comparing file timestamps.
   Always copy the full set together:
   $src = "...\github\Ghidra-H8-Processor\h8\data\languages"
   $dst = "...\ghidra_12.0.4_PUBLIC\Ghidra\Processors\h8\data\languages"
   Copy-Item "$src\h8539f.sla"    "$dst\h8539f.sla"    -Force
   Copy-Item "$src\h8539f.pspec"    "$dst\h8539f.pspec"    -Force
   Copy-Item "$src\h8539f.cspec"    "$dst\h8539f.cspec"    -Force
   Copy-Item "$src\h8539f.slaspec"  "$dst\h8539f.slaspec"  -Force
   Copy-Item "$src\h8539f-arith.sinc"  "$dst\h8539f-arith.sinc"  -Force
   Copy-Item "$src\h8539f-bit.sinc"    "$dst\h8539f-bit.sinc"    -Force
   Copy-Item "$src\h8539f-branch.sinc" "$dst\h8539f-branch.sinc" -Force
   Copy-Item "$src\h8539f-logic.sinc"  "$dst\h8539f-logic.sinc"  -Force
   Copy-Item "$src\h8539f-mem.sinc"    "$dst\h8539f-mem.sinc"    -Force
   Sanity check after copying (both listings should show matching timestamps for
   every h8539f* file): Get-ChildItem "$src" | Select Name,LastWriteTime
   and the same command against $dst.
4. run the script  ReloadSleighLanguage.java in Ghidra to refresh the open project with the new sleigh files.
5. Test in the Ghidra project as normal

CAVEAT ADDED 2026-08-02: steps 1-4 above only reload .sla/.pspec/.sinc content.
.cspec is loaded once per Ghidra session/project and is NOT hot-swapped by
ReloadSleighLanguage.java -- confirmed by direct test (see item 18 continuation
below): a full compile+copy+reload cycle produced a byte-identical decompile
until Ghidra itself was actually closed and reopened. If a change touches
.cspec (tracked_set defaults, calling conventions, pentry storage), a full
Ghidra restart is required before testing, not just steps 2-4.

18 (continuation) -- superseded. The EP-input SEGMENTOP limitation, the
table_lookup_interp/table_lookup_indexed/table_3axis_interp_triple
param-storage fixes, and the #817-style far-pointer/call-site annotation
work are now tracked in review3.md (cspec-driven far-pointer call-argument
fusion plan).

SESSION 2026-08-07: MUT DISPATCH/PACK-AND-SEND TRACE, METHODOLOGY
CORRECTIONS, AND A BETTER VERIFICATION TOOL
============================================================
Long session continuing from "trace where the ECU packs and sends MUT
IDs." Full detail lives in mut_verification_status.md; this entry
summarizes the METHODOLOGY findings specifically, since they apply to
all future sessions/tools, not just this one topic.

WHAT WAS TRACED (see mut_verification_status.md for full detail):
- sci1_periodic_status_frame_build_f54a_f566 (renamed from
  diagnostic_snapshot_f54a_f566_build): the periodic (every-200-tick)
  SCI1 status/handshake frame builder. All 14 fields traced to a source;
  3 confirmed as real known values (engine_torque_pct_f17a,
  mut_battery_voltage_f13a, coolant_temp_scaled_f130), 2 traced but
  unnamed (F18C injector-PW sibling, F10E table/axis output), 3
  (F552/F554/F55A) traced to a shared muldiv_s16_rounded_3op() call whose
  inputs are a 100-tick snapshot-and-reset of an accumulator
  (accum_latch_100tick_f33a/f33c, renamed) inside
  fuel_pw_and_airvol_compute -- exact physical meaning of THAT
  accumulator not pinned down.
- Separately traced the ACTUAL Mode-2 MUT RequestID path
  (adc_sensor_convert_single, the 0x2fad0 table reader): found it has
  exactly 2 callers, both internal periodic snapshot/backup-logging
  functions (renamed mut_configurable_reqid_backup_snapshot_5word /
  _periodic_snapshot_8word), which pull their RequestID list from a
  RAM record fed by a boot-time ROM copy that turned out to be all-0xFF
  (unprogrammed) on this ROM. Traced who else could populate that record
  at runtime (eeprom_backup_table_write_dispatch, called every main_loop
  pass) and searched exhaustively for an EEPROM/serial driver -- found
  none. Working conclusion (per user's steer): this ECU likely has no
  onboard EEPROM, the whole mechanism is vestigial/inherited from a
  shared platform codebase, never populated on real hardware. CONCLUSION:
  there is no live, wire-driven "answer any RequestID you're asked for"
  handler on this ROM -- the sub-0xC0 command table @0x1fad0 that would
  normally hold one is entirely unprogrammed (separate, earlier finding).
- Used the above work as a base to confirm 6 MUT table RequestIDs as
  genuinely dead/unimplemented cells: 0x18 F21D, 0x19 F217, 0x20 F179,
  0x2E F1E7, 0x44 EEDF (MAT Scaled), 0x45 EEE1 (MAP Scaled).

METHODOLOGY LESSONS (the important part for future sessions/tools):

1. get_xrefs_to / get_bulk_xrefs must be checked under BOTH the 0000-
   and 0001-address-bank-prefix forms of an address before concluding no
   writer exists. This bit twice this session: F17A and F33A/F33C both
   showed "zero xrefs" under one prefix but had real writers findable
   under the other. Root cause: this ROM's H8/500 code accesses the same
   physical RAM cell under different bank-tag prefixes depending on
   which bank was active at the access site (same idiom as the
   `bVar<<0x10 | 0xfxxx` pattern already documented elsewhere in this
   project) -- Ghidra's xref index appears to track these separately
   rather than unifying them to one physical address.

2. search_byte_patterns on a raw 2-byte address literal is UNRELIABLE
   used alone -- H8/500 instructions are variable-length and a 2-byte
   substring can coincidentally appear inside a completely unrelated
   instruction's encoding (demonstrated concretely with F21D: a
   byte-pattern hit at 0x2be19 looked like a real reference but
   get_assembly_context proved it wasn't even an instruction boundary --
   it was a byte-offset overlap inside a different instruction targeting
   a different address entirely, F71C). MANDATORY: verify every
   byte-pattern hit with get_assembly_context (or equivalent) to confirm
   it lands on a genuine instruction boundary referencing the intended
   address, before treating it as a real reference either way (a hit
   OR a non-hit both need this check -- a real hit could also coincide
   with data/table regions that aren't code references, as seen with the
   EEDF/EEE1/F217/F179/F1E7 table-entry hits, which were real matches
   but were table DATA, not code writers).

3. BEST METHOD FOUND THIS SESSION, should be the PRIMARY method going
   forward: the project has a full decompiled-C dump of the entire ROM
   at test/rvr/RVR_1998_x3 4g63t 21000011 md352553.hex.c (28,456 lines,
   confirmed current -- reflects today's Ghidra renames already). Grep
   this file directly for a target symbol/address FIRST. It is immune to
   BOTH failure modes above simultaneously: it's flat decompiled text
   (no bank-prefix split, so can't miss a writer the way get_xrefs_to
   did), and it matches on actual decompiled address/symbol text rather
   than raw instruction-encoding bytes (so can't produce the
   coincidental-byte-overlap false positives search_byte_patterns did).
   Verified by grepping "f17a" and finding the exact known-good writer
   line in plain text (`*(undefined2 *)((uint)bVar2 << 0x10 | 0xf17a) =
   uVar1;`). Re-ran all 6 "confirmed dead" cells through this file as a
   third independent check -- all came back 0 matches, giving much
   higher confidence than either of the other two methods alone.
   RECOMMENDATION: for any future "does X have a writer / who references
   Y" question in this project, grep the .c dump first; use
   get_xrefs_to second (checking both bank prefixes) to jump from a hit's
   line number to the live Ghidra function name/address; reserve
   search_byte_patterns for cases where the .c dump doesn't exist yet or
   needs regenerating, and always verify its hits against instruction
   boundaries before trusting them.

4. Général process note: several conclusions this session were stated
   too confidently on a first pass and had to be walked back after
   direct pushback/re-checking (the "fixed literal RequestID" claim, the
   "dead end" framing on the EEPROM mechanism, the F21D byte-pattern
   dismissal). Worth building in the habit of treating a first
   xref/byte-pattern search as provisional, especially when it produces
   a clean "definitely nothing here" result, rather than writing it up
   as final until cross-checked by a second, structurally different
   method (per point 3 above, that second method should now be the .c
   dump grep by default).

SCRIPT ADDED 2026-08-07: FindRealWritersAcrossBankForms.java
============================================================
Saved in "ghidra scripts/FindRealWritersAcrossBankForms.java" (copy into
Ghidra's own ghidra_scripts search path before running, per the existing
documented gotcha for all scripts in this project).

WHY: get_xrefs_to was confirmed this session to have a real, reproducible
blind spot -- it indexes references per literal address-space-prefix
string ("0000fXXX" vs "0001fXXX") and does NOT unify them even when they
are the same physical RAM byte. Verified directly: get_xrefs_to(0001f130)
returns 17 correct references; get_xrefs_to(0000f130) returns ZERO for
the exact same physical cell, in the same session, immediately after.
"Check both prefixes" (the mid-session workaround) is a partial
mitigation, not a fix -- it still misses any write using a THIRD
addressing form, e.g. a runtime-computed bank-relative store like
(uint)bVar<<0x10 | 0xfXXX, which is exactly how F17A's and F33A/F33C's
real writers were built (found only by decompiling/disassembling
candidate functions directly, not via get_xrefs_to under either prefix).

WHAT IT DOES: single pass over every instruction in the program.
  - Pass 1 (MEM-REF, high confidence): checks every instruction's already-
    resolved memory references, normalizes the target address to its bare
    16-bit RAM offset (strips the bank-prefix distinction Ghidra's live
    xref index fails to unify), and reports a hit if it matches a target.
    This alone reproduces everything get_xrefs_to would find under BOTH
    prefixes, in one query.
  - Pass 2 (IMM-ONLY, lower confidence, needs manual review): separately
    scans every instruction's operands for the bare 16-bit target value
    appearing as a plain scalar immediate, REGARDLESS of whether Ghidra
    recognized it as a memory reference. This is the pass that would have
    caught F17A/F33A/F33C without prior knowledge of where to look --
    those writers use a computed-bank store where the low 16 bits still
    appear as a literal, just not as something Ghidra's operand analysis
    resolved into a formal Reference.
  - Does NOT auto-classify Pass 2 hits as real -- deliberately, per the
    F21D false-positive already found this session (a byte-pattern match
    that turned out to be a coincidental sub-instruction byte overlap,
    not a real reference). Every Pass 2 hit needs a human to confirm it's
    a genuine address-forming immediate and not a coincidental literal
    (loop bound, unrelated constant, etc.) before treating it as a real
    writer.
  - Optional CONFIRM_AND_ANNOTATE list: once a hit is manually confirmed,
    add it there and re-run to have the script create a real Ghidra
    memory reference, so future get_xrefs_to queries (under either
    prefix) will find it going forward -- closes the gap for that
    specific cell permanently rather than needing this script re-run
    every time.

NOT YET RUN this session -- written and reviewed but not executed inside
Ghidra (requires copying into ghidra_scripts path and running via the
GUI or a script-runner tool, not yet done). Default TARGET_OFFSETS list
is pre-loaded with the 6 cells this session called "confirmed dead"
(f21d, f217, f179, f1e7, eedf, eee1) specifically so running it is the
natural next step to re-verify those conclusions with a method immune to
both known blind spots at once (the prefix-index bug AND the byte-
pattern-false-positive risk), especially given the EvoScan log
contradiction found the same session (real live MATScaled/MAPScaled data
existing despite "no writer found" for EEDF/EEE1).

NOT YET DONE: actually run this script in Ghidra and reconcile its
output against both the .c dump grep results (which agreed "0 matches"
for all 6 targets, but is ALSO potentially blind to computed-bank-store
writers if the decompiler folds them into a form that doesn't textually
contain the literal) and the EvoScan log question.

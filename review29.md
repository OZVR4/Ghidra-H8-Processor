# Session 29 (2026-10-28 evening): the "hybrid grammar hole" was stale instruction prototypes in project `new`, not a grammar or sleigh bug

"Verified" = observed (compile output, live headless Ghidra, text of the exports).
"Unverified" = inference. Follows review28.md and handoff-r0r5-hybrid-grammar.md (this supersedes its "Open problem" section and next step 1).

## Summary

1. The sign-split hybrid grammar (`lang_hyb4`) works as designed. After a fresh parse every R0-R5 16-bit displacement instruction in the RVR ROM has the intended shape: sign bit clear -> old shape (`spSegment(PG,Rn)+disp`, first op CALLOTHER), sign bit set -> new shape (`off=Rn+disp; spSegment(PG,off)`, first op INT_ADD). 90 of 90 correct.
2. The "hole" (positive `mov:g.w @(d,R0|R3)` showing the new shape, `or.w` and `mov:g.b` showing the old shape) was an artifact of the measurement. Project `new` stores instruction prototypes, shared between instructions with the same constructor tree. It was disassembled when positive and negative displacements had identical trees, so they share prototypes; after the .sla changed, the stored representative decides the pcode shape for every instruction sharing it. Which representative a family got explains "depends on the instruction, not the bits" and why moving the alias to bit (0,0) seemed to change the pattern.
3. No sleigh bug, no upstream report. Do not file anything.
4. Rule from now on: after ANY .slaspec/.sinc change, run `scripts\ReDisasm.java` before measuring pcode shape or decompiling an existing project (`-postScript ReDisasm.java -postScript DecompAll.java <out>`).

## Evidence (verified)

- Installed `ghidra_mod` h8539f.sla is byte-identical to a fresh recompile of `lang_hyb4` (120846 bytes, same hash). The 48 R0-R5 EA constructors (slaspec 635-682) are all split; `disp16_banked` is used only by `jmp`; `mov:g.w` and `or.w` both go through `eaw_disp16`.
- `DumpAt.java` (full pcode, stored prototype): `mov:g.w @(0x329e:16,R0),R3` (f8 32 9e 83) INT_ADD first, but `or.w @(0x329e:16,R0),R3` (f8 32 9e 43) CALLOTHER first, same EA bytes.
- `DumpFresh.java` (clear + re-disassemble in memory, `-readOnly`, nothing saved), all 90 R0-R5 d16 instructions from `shape_mod3.txt`:

| mnemonic | disp sign | stored first op | fresh first op | count |
|---|---|---|---|---|
| mov:g.w | positive | INT_ADD | CALLOTHER | 26 |
| mov:g.w | positive | CALLOTHER | CALLOTHER | 1 |
| mov:g.w | negative | INT_ADD | INT_ADD | 32 |
| mov:g.b | positive | CALLOTHER | CALLOTHER | 19 |
| mov:g.b, clr.w, sub.w, or.w (neg) | negative | INT_ADD | INT_ADD | 1 + 4 + 3 + 1 |
| or.b, or.w, tst.b | positive | CALLOTHER | CALLOTHER | 1 + 1 + 1 |

Only the 26 `stored=INT_ADD, fresh=CALLOTHER` rows differ; after the fresh parse nothing is wrong.

## Full-ROM comparison with fresh parse (RVR 21000011, 741 functions, cspec align=1 for all three, verified)

`rvr_fresh\` (ReDisasm.java first; 25962 instructions re-parsed, 0 failed, 0 length changes) vs `rvr_fp\` (stale):

| Metric | old | new | hybrid | new-old | hybrid-old | (stale: hybrid-old) |
|---|---|---|---|---|---|---|
| decompile FAIL | 1 | 0 | 0 | -1 | -1 | -1 |
| `undefined*` | 1574 | 1637 | 1574 | +63 | **+0** | +10 |
| pointer casts | 4127 | 4212 | 4132 | +85 | **+5** | +15 |
| `(short)` casts | 85 | 126 | 107 | +41 | +22 | +22 |
| `halt_baddata` | 7 | 8 | 8 | +1 | +1 | +1 |
| `switch(` | 0 | 2 | 2 | +2 | +2 | +2 |
| `in_stack_`, stack arrays, register0x | same | same | same | 0 | 0 | 0 |

- The only failure is `sci1_post_handshake_status_broadcast_dispatch` (0x28b2f) under the OLD grammar (it reads the junk table); new and hybrid have 0 failures, and both get the real `switch`.
- The hybrid now matches the old grammar on `undefined*` exactly and is within +5 on pointer casts, versus +63 / +85 for the plain new grammar.
- The stale run also shifted the old and new columns a little (old `undefined*` 1562 -> 1574), so ALL earlier absolute numbers measured on project `new` after a grammar swap (review28 tables, `rvr\`, `rvr_fp\`, `multi\`, `multi2\`) are slightly off. Their conclusions look unaffected (Regression 1 = cspec `align="2"`, `align="1"` resolves calls to odd-address functions), but `multi\`/`multi2\` (the other ROMs, 2x2 and callres) were NOT re-run with ReDisasm.

## Leftovers explained (verified counts, cause partly unverified)

- tcu_solenoid_diagnostic_calc (0x2c2d9) `undefined*`: old 11, new 18, hybrid 11. The earlier "+7" was the stale artifact.
- `(short)` +22: 17 functions, +1/+2 each (tcu_gear_ratio_calc, sci1_meta_cmd_dispatch_c0_ff, sci1_boot_ihex_colon_record_decode_dispatch, knock_octane_pattern_match_state_machine, isr_sci1_eri_reset_timeout, ...). In tcu_gear_ratio_calc the lines are `*(ushort *)((short)&UNK_0000ef04 + sStack_4)`: a negative displacement (0xef04 = -0x10fc) taking the new, hardware-correct 16-bit-wrap shape. Unverified: that all 22 are negative-displacement forms; only the top function was inspected.
- 8 functions differ from old on `undefined*` by +1/-1 (net 0); not inspected.

Known semantic limit (unchanged, reading): for non-negative displacements the hybrid keeps the old behaviour, so an Rn + disp that overflows 16 bits goes into the next page instead of wrapping (identical to final2).

## State of the rig

- `ghidra_mod` language folder: HYBRID grammar + patched cspec (align=1), restored by run_fresh.ps1. Untouched: `ghidra_stock`, the repo grammar and cspec, `optional/r0r5-add-before-segment/`, project `new`, review27.md, review28.md, the handoff.
- New in `%TEMP%\r0r5`: `scripts\DumpAt.java` (pcode at given addresses), `scripts\DumpFresh.java` (stored vs fresh first op), `scripts\ReDisasm.java`, `run_fresh.ps1`, `rvr_fresh\`, `short_diag.py`, `fresh_addrs.txt`, `dumpat_out.txt`, `fresh_out.txt`, `lang_chk_220800\` (scratch recompile, deletable).

## Next steps (not done; items 1-2 need your OK, they change the repo)

1. Put `<funcptr align="1" />` in the repo `h8539f.cspec` (after checking git history for why `align="2"` was added; review28 item 4 is still open). It fixes 0x28b2f and resolves calls to odd-address functions, but changes 233 of 741 functions, so review a sample first.
2. Decide where the hybrid goes: with fresh numbers (0 failures, `undefined*` +0, pointer casts +5 against old) it can replace the default grammar; otherwise update `optional/r0r5-add-before-segment/`.
3. Re-run the 2x2 (grammar x cspec) comparison for all ROMs with ReDisasm (`multi\` / `multi2\` drivers, add `-postScript ReDisasm.java` before DecompAll).
4. Optionally look at the `(short)` sites beyond the top function and the 8 +/-1 `undefined*` functions.
5. Update `todo.md` item 2 and `todo-r0-r5-displacement-order.md` (still pending from review27).



## Clean 2x2 (grammar x cspec) on all five ROMs, fresh parse (verified; run 2026-10-29, driver run_2x2_fresh.ps1, summary sum_2x2_fresh.py, output multi3\)

All 20 runs (5 ROMs x 4 configs) ran ReDisasm.java first: 0 failed, 0 length changes (instructions: 25962 / 24137 / 31831 / 22032 / 23877). "stock" = stock grammar + cspec align=2; "cspec-only" = stock grammar + align=1; "grammar-only" = hybrid + align=2; "both" = hybrid + align=1.

| ROM | metric | stock | cspec-only | grammar-only | both |
|---|---|---|---|---|---|
| RVR 21000011 | FAIL / undefined* / ptr casts / (short) / unresolved calls | 0 / 1669 / 3819 / 97 / 686 | 1 / 1574 / 4127 / 85 / 1 | 1 / 1655 / 3813 / 115 / 684 | 0 / 1574 / 4132 / 107 / 1 |
| RVR 21000012 | same | 3 / 1807 / 3111 / 71 / 688 | 4 / 1775 / 3091 / 66 / 0 | 4 / 1794 / 3104 / 82 / 686 | 3 / 1780 / 3095 / 80 / 0 |
| RVR w4a51 | same | 3 / 3718 / 7566 / 121 / 948 | 3 / 3792 / 8220 / 197 / 0 | 4 / 3711 / 7564 / 177 / 948 | 3 / 3786 / 8220 / 253 / 0 |
| legnum vr4 | same | 2 / 1585 / 3061 / 83 / 595 | 2 / 1472 / 3240 / 77 / 0 | 2 / 1586 / 3063 / 110 / 595 | 1 / 1477 / 3244 / 104 / 0 |
| EVO5 | same | 1 / 1781 / 3521 / 90 / 665 | 2 / 1692 / 3718 / 80 / 0 | 2 / 1762 / 3486 / 118 / 663 | 1 / 1694 / 3722 / 108 / 0 |

Reading (verified counts; the interpretation is mine):

- cspec align=1 is the big win: unresolved indirect calls `(*name)()` drop from 595-948 per ROM to 0-1 on every ROM.
- Neither change is safe alone: the switch function (0x28b2f RVR 11 and 12, 0x28b21 EVO5) is ok under stock, FAILS under cspec-only AND under grammar-only, and is ok again under both. 0x19876 (w4a51) fails only under grammar-only. So they must be applied together (consistent with Regression 1 in review28).
- "both" vs stock: failures equal or better on every ROM (legnum 2 -> 1; 0x28acb ok only under both); `undefined*` down on four ROMs (-95, -27, -108, -87), up +68 on w4a51; pointer casts up (+313, -16, +654, +183, +201).
- Failures that remain under "both" (adc_sensor_convert_multi on w4a51/legnum/EVO5, byte_sum_banked and sum_byte_array on w4a51, 0x17160/0x17206/0x1722b on RVR 12) fail in all four configs, so they are independent of both changes.
- Grammar effect on (short) casts: +18 / +11 / +56 / +27 / +28 (hybrid vs stock grammar at align=2); expected from negative displacements now wrapping at 16 bits. Unverified that all are negative-displacement forms.
- Outlier to spot-check: w4a51 "both" has (short) 121 -> 253, `&stack0x` 750 -> 912 and ptr casts +654 (about +9%); not inspected, cause unknown (more functions resolving under align=1 is one possibility).

Not resolved: why `align="2"` was put in the repo cspec. `git` calls are blocked by the pulsar run-command guard here and the repo notes searched so far do not say; run `git log -S"funcptr align" -- h8/data/languages/h8539f.cspec` yourself (or give the OK for `confirm:true`).


## Session 29 continued (2026-10-29/30): final4 diff, calling-convention gap, H8PreservedRegsAnalyzer

Final3 -> final4 (user's GUI exports) diff, read through (verified from the diff text): `align="1"` makes calls land on the right
function (final3 called `noop_return_void`, `stub_empty_0x2a8d1`, `stub_empty_0x2c129`, `stub_empty_0x26b8b` where final4 calls
`adc_channel_convert_and_read`, `sci3_rx_process_tcu_byte`, `tcu_periodic_dispatch`, `isc_f428_condition_calc`), hundreds of
`(*fn)()` became direct calls, about 60 `TableHeader_*` globals and typed table arguments appeared, and
`sci1_post_handshake_status_broadcast_dispatch` decompiles to an 8-case `switch` that matches the hand-derived table
(case 3 sends 0x55, case 5 sends 0xEF/0xF8/0x08 0x08, case 7 sends 0x85); its plate comment saying "ignore the decompiled
body" is now outdated. `(int)0xf814 + 0x33` instead of `0xf847` also appears in a fresh parse of the stock grammar, so it is the
cost of keeping the old form for non-negative displacements, not a regression.

Findings, in order:
- Lost registers: the default prototype lists R0, R1, R2 as killedbycall, but `adc_sensor_convert_single` (0x171c3) and about
  345 of 741 RVR functions save and restore R1 and/or R2 (stm/ldm or mov push/pop; verified by script scan). That produced
  `extraout_R2`, `(int)extraout_R2 + 2` and `(int)1 + 1` in the ADC/MUT frame builders.
- Stack pop is already handled: H8ExtraPopAnalyzer / H8FunctionPurgeAnalyzer set the purge size of 726/741 functions (675 are 4 bytes);
  my headless runs use `-noanalysis`, so they never ran there, but the values are stored in the project.
- Negative result: shrinking every `maxsize="512"` stack-parameter slot to 32 changed 1 function and no stack array.
- The stack arrays moved when register preservation was modelled (9 -> 4), so at least some come from the same clobber assumption.

H8PreservedRegsAnalyzer result on RVR 21000011, fresh parse, `align="1"` for both (305 functions switched):

| Metric | before | after |
|---|---|---|
| `extraout_R2` | 307 | 230 |
| `extraout_R1` | 115 | 117 |
| `undefined*` | 1574 | 1534 |
| pointer casts | 4133 | 4112 |
| `&stack0x` | 20 | 7 |
| stack-array declarations | 9 | 4 |
| `unaff_*` | 123 | 167 |
| decompile failures | 0 | 0 |

Watch items (not investigated): `unaff_*` +44 and 17 functions with more `in_stack_`/`unaff_`/`&stack0x` names (largest:
`tcu_shift_timing_state_machine` +6); 3 junk base addresses remain in `mut_configurable_reqid_periodic_snapshot_8word` and 1 each in
`table_select_scale_fanout_f1be_f1c2`, `isc_f41a_correction_calc`, `isc_f420_correction_calc`. Only RVR 21000011 was measured.
The analyzer run is byte-identical to the equivalent one-off script run. Not tested: the revert path and the Dry run option.

Installed: `ghidra_stock` has the new `h8539f-analyzers.jar` (old one kept as `h8539f-analyzers.jar.bak-20261030-224227` in the same lib
folder) and the cspec with the 15 clones; the repo has the same cspec, `H8PreservedRegsAnalyzer.java`, and the two new class files
in `h8/build/classes/h8539f/`. Verified from `ghidra_stock` headless: analyzer discovered, disabled by default, 305 set, none skipped.
The analyzer is disabled by default; enable it under Analysis > Auto Analyze after restarting Ghidra.

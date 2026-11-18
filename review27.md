# Session 27 (2026-10-26): R0-R5 displacement order converted (add-then-segment); final3 vs final2: stack/register metrics unchanged, but ONE new decompile failure (0x28b2f) and a readability regression (more undefined*/pointer casts)

"Verified" = observed (compile output, file hashes, live Ghidra, the exports).
"Unverified" = inference. Companion notes: `todo.md` item 2, `todo-r0-r5-displacement-order.md`.

## Summary for whoever picks this up next

1. **Did** the R0-R5 displacement conversion planned in `todo-r0-r5-displacement-order.md`:
   `spSegment(PAGE, Rn + disp)` instead of `spSegment(PAGE, Rn) + disp`
   (PAGE = DP for R0-R3, EP for R4/R5). Compiled, deployed to repo and live install,
   exported as `test/rvr/...final3.hex.c`.
2. **Compared final3 against final2.** The stack / register / spacebase metrics are
   unchanged. Two regressions: **one function now fails to decompile** (0 -> 1 "Unable to
   decompile", 0x28b2f; Regression 1) and **more `undefined*` types and pointer casts**
   (+54 / +72) in about 20 functions (Regression 2).
3. **Not done / not known:** why that function fails. Cause is open (see below).

The new grammar was later committed as an optional copy (see "Committed as an optional copy" below).
Ghidra project `new` data was not touched.

## What changed (verified)

- `h8539f.slaspec` lines 635-638: 4 constructors (eab_disp8, eaw_disp8, eab_disp16, eaw_disp16)
  replaced by 24 (one per register R0..R5).
- `h8539f-arith.sinc`: 46 inline `Rn_banked & Rn < 6` constructors (clr/neg/tst/not/shal/shll/
  shar/shlr/rotl/rotr/rotxl/rotxr, b/w, disp8/disp16) replaced by 276.
  (An earlier estimate of 48 was a miscount. `rotxr.w` has no inline disp forms; it uses the
  shared `eaw_disp8/16` operands, so the slaspec change covers it.)
- Diff check on the temp copy: every removed line carried the `Rn_banked & Rn < 6 &` guard,
  every added line is a per-register `is Rn = N &` constructor, and no `Rn < 6` or
  `Rn_banked + disp` remains in either file.
- Display text is unchanged (literal "R0".."R5", same text `Rn_banked` printed).
- New shape, copied from the FP/SP forms:

      sinc:    :clr.b "@("^disp8^":8,R0)" is Rn = 0 & mode = 14 & sz = 0; disp8; opcode_special=0x13
               { local eaoff:2 = R0 + disp8; local easeg:4 = spSegment(DP, eaoff); tmp = easeg; ... }
      slaspec: eab_disp8: "@("^disp8^":8,R4)" is Rn = 4 & mode = 14 & sz = 0; disp8
               { local off:2 = R4 + disp8; local a:4 = spSegment(EP, off); export *:1 a; }

## Lesson: two-constructor split from the plan does NOT compile (verified)

The plan suggested `Rn < 4` (DP) and `Rn >= 4 & Rn < 6` (EP) with the bare `Rn` field in the
display and body. sleigh rejects that:

- bare `^Rn^` in the display, or `Rn + disp` in the body, next to a relational `Rn < N`
  constraint -> "syntax error".
- `Rn = 0` with bare `^Rn^` -> "Constraining currently undefined operand Rn".
- `Rn = N` plus a LITERAL register name in display and body compiles (tested for R0/DP and
  R4/EP) and is the shape the FP/SP constructors already use. So: six explicit constructors.

## Compile / deploy (verified)

- Baseline and new both compile, exit 0. The only difference in compiler output is
  "unnecessary extensions/truncations" 44 -> 54. The 10 extra are the existing `shal.b`
  "Unnecessary Truncation" warning repeated: 2 original shal.b disp lines became 12.
  Not caused by the displacement change.
- `eab_disp8_2`, `eaw_disp8_2`, `eab_disp16_2`, `eaw_disp16_2` (slaspec ~766-769) still use
  `Rn_banked2 + disp` but are UNREFERENCED (sleigh warns "Unreferenced table"). Left alone.
- Deployed: repo `h8/data/languages` and install
  `...\ghidra_12.2_DEV_20260818\ghidra_stock\Ghidra\Processors\h8\data\languages`.
  All h8539f source files are byte-identical between the two. Install `h8539f.sla`
  recompiled in place (96084 bytes) and byte-identical to the one compiled and tested in a
  temp copy. The repo's own `h8539f.sla` was NOT regenerated (todo item 15 still open).
- Backups beside the originals in BOTH places: `h8539f.slaspec.bak_r0r5`,
  `h8539f-arith.sinc.bak_r0r5`, and `h8539f.sla.bak_r0r5` (install only).
- Ghidra was not running during the deploy (only the MCP bridge process).
- The deploy script first aborted at the compile step because `$ErrorActionPreference='Stop'`
  turns sleigh's stderr WARN lines into a terminating error in PowerShell 5.1. Sources had
  already been copied; the compile and parity check were re-run without Stop. Use
  `*>` redirection and check `$LASTEXITCODE` instead. Note `-match 'ERROR'` is
  case-insensitive and also matches "NativeCommandError".
- Helper scripts (in `%TEMP%\r0r5`, not in the repo): `convert2.py` (the conversion),
  `cmp.py` (final2 vs final3 metrics and per-function split), `neutral.py`.

## final3 vs final2 (verified, text comparison of the exports)

| Metric | final | final2 | final3 |
|---|---|---|---|
| `register0x` | 0 | 0 | 0 |
| stack arrays (`a\w?Stack_`) | 43 | 55 | 55 |
| `in_stack_` | 340 | 163 | 163 |
| `in_stack_00000004` | 256 | 79 | 79 |
| "Restarted to delay" | 2 | 2 | 2 |
| `&stack0x` | 14 | 21 | 21 |
| spacebase warnings | 15 | 15 | 15 |
| "Unable to decompile" | - | 0 | **1** |
| `halt_baddata` | 11 | 11 | 6 |
| `unaff_FP` | 15 | 13 | 9 |

(My stack-array regex gives 55; the 46 in `todo.md` used a different regex. Final2 and
final3 are equal on both.)

730 chunks in both exports, 59 changed. CORRECTION to the first version of this note: the
failing function has no `name(...)` signature line in either export (final2 has a real body
under a bare-name header at line 24394, final3 has the failure stub at line 24344), so a
splitter keyed on signatures folds its text into the previous function,
`f510_f512_f00e_f514_reset`. That chunk is therefore NOT a second problem: it is the failed
function's lost body (its apparent -14 undefined / -9 casts / -2 `(short)` and the
`halt_baddata` / `unaff_FP` drops are that body disappearing). The other 58 changed chunks
are neutral on the stack / register / failure metrics only, NOT on `undefined*` or casts
(see Regression 2).

### The other 58 changed chunks, stack/register-neutral (14 read, 44 not read)

Of the 14 read, all are address-expression or declaration changes:
- bank-0 references became the real bank-1 variable, e.g. `uVar5 = DAT_0000f84a` ->
  `DAT_0001f84a & 0x101` (eeprom_backup_table_write_dispatch, channel_validity_gated_handler_dispatch).
  Consistent with the direct-addressed `@0xf84a:16` uses elsewhere.
- `(int)uStack_4 + -0x7b6` -> `(short)&DAT_0000f84a + uStack_4`
  (init_copy_const_block_via_memcpy_banked, isc_sci1_eri_reset_timeout, serial_fec5_byte_to_fifo,
  sci1_tx_response_feeder).
- variable declaration reordering / renumbering.
Unverified for the other 44. `mut_configurable_reqid_periodic_snapshot_8word` now shows
`*(short *)0xf84c` (no bank shown; DP is not in constresolve so a constant DP offset is not
folded; same limitation as before, just a different print).

## Regression 1: decompile failure in sci1_post_handshake_status_broadcast_dispatch (0x28b2f)

Verified:
- final2: decompiled (with the `halt_baddata` warnings that the old plate comment calls
  spurious). final3 and the live session:
  "Low-level Error: Bad instruction count exceeded ... No pcode could be generated at
  address r0x00028b8e". Same error from `decompile_function` and `get_function_pcode`
  in the live project, so it is not an export artifact.
- 0x28b8e is MID-INSTRUCTION. The `bne #0x28d1f:16` is at 0x28b8d, bytes `36 01 8f`
  (next instruction 0x28b90). Case 1 starts at 0x28b89 (`btst.w 0x5,@0xf102:16`).
- The function's switch: `shll.w R4; ldc.b 0x2,EP; mov:g.w @(-0x74ae:16,R4),R4; jmp @R4`
  (guard `cmp:i #0x7:16,R4; bhi #0x28d1b`). `-0x74ae` = 0x8b52.
- The real 8-entry jump table is at **0x28b52**: `8b62 8b89 8ba8 8c5a 8c78 8c83 8cba 8cc8`.
  Every target disassembles cleanly.
- OLD semantics (`spSegment(EP,R4) + sext(disp)`) resolves that load to **0x18b52**,
  which holds ordinary code bytes (`f490 800c fffc 50ee ...`), not a table. That is the
  source of the spurious `halt_baddata` in final2.
- NEW semantics (`spSegment(EP, R4 + disp)`) resolves it to the real table at 0x28b52.
  So the language change is correct here; the failure appears because the decompiler now
  follows the real table.
- `jmp @Rn_banked` is `goto [Rn_banked];` (h8539f-branch.sinc line ~48), i.e. it uses EP for
  R4/R5 on purpose, per its own comment.

Tested and refuted:
- Hypothesis "the decompiler reads table entries past the 8th (bound not recognised) and
  one equals 0x8b8e". The bytes after the table (0x28b62...) contain no `8b 8e` at any
  alignment.

Experiments run later the same day, on a TEMP COPY of project `new` (headless; the original
project was not touched; the 12:51 timestamps on it were the user reopening Ghidra).
Scripts are in `%TEMP%\r0r5\scripts` (DumpSwitchPcode.java, ClearStaleSwitchRefs.java):
- Reproduces headless: decompileCompleted=false, plus decompiler warnings
  "pcode error at 00028c8a: Unable to resolve constructor" and the same at 00028b8e.
  So there are TWO bad addresses (0x28b8e and 0x28c8a), not one.
- Listing p-code (verified): `mov:g.w @(-0x74ae:16,R4),R4` = `unique = R4 + 0x8b52;
  callother(0, EP, unique); R4 = LOAD(callother result)`; `jmp @R4` = `callother(0, EP, R4);
  BRANCHIND`. The instruction containing 0x28b8e is the 3-byte bne at 0x28b8d; nothing
  references 0x28b8e. The function body is only [0x28b2f-0x28b51]; the case code lives in
  other functions (0x28b89 is sci1_handshake_status_byte_builder_and_f516_hibit_writer).
- The saved project still has 8 stale COMPUTED_JUMP refs (src=ANALYSIS) from the jmp at
  0x28b50 to 0x2f490, 0x2800c, 0x2fffc, 0x250ee, 0x20640, 0x21df4, 0x29090, 0x20f10 (the
  bytes at the OLD wrong table 0x18b52 with EP=2). Deleting all 8 in the temp copy did NOT
  change the failure: hypothesis "stale refs cause it" REFUTED. They are still junk worth
  cleaning once the real cause is found.

Unknown:
- Why the decompiler targets 0x28b8e and 0x28c8a. Language vs decompiler switch recovery
  is not determined. No A/B on a rebuilt older language was done.

## Regression 2: more `undefined*` types and pointer casts (counts verified, cause inferred)

| Metric | final2 | final3 | delta |
|---|---|---|---|
| `undefined*` | 2781 | 2835 | +54 |
| pointer casts (my regex) | 3798 | 3870 | +72 |
| `(short)` casts | 96 | 132 | +36 |
| `unaff_` | 96 | 84 | -12 |
| `switch(` | 2 | 0 | -2 |

`test/rvr/ghidra diff final 3.txt` (an external comparison of the same exports; it calls the
file final3.hex-2.c) reports the same line counts, `undefined*`, `unaff_` and `switch`
figures and a larger cast figure (+151, different regex). Its claim that final3 "drops the
function" is wrong: the function is present as the failure stub. Its advice to treat that
as a cleanup should be weighed against it being a decompile failure.

Excluding the failed function's lost body (inferred from the chunk it was folded into), the
like-for-like change in the other changed functions is about +60 `undefined*`, +81 casts and
+38 `(short)`, spread over roughly 21-23 functions. Biggest:
status_word_composite_multibit_update_via_table (undefined 6 -> 23, casts +18),
f26e_o2_closed_loop_output_blend_via_table (+7 / +7), tcu_solenoid_diagnostic_calc (+7 / +7),
table_lookup_interp (undefined +4, `(short)` +9), table_3axis_interp_triple (+4, `(short)` +9),
isc_stepper_blend_from_struct, eeac_adaptive_trim_learn_update_via_table.

Typical change (status_word_composite_multibit_update_via_table, table_lookup_interp):

    final2:  uVar3 = **(ushort **)((int)in_stack_00000004 + 6);
    final3:  uVar3 = *(ushort *)*(undefined2 *)(in_stack_00000004 + 6);
    final2:  undefined2 in_stack_00000004;      final3:  short in_stack_00000004;
    final2:  *(char *)((int)table + 1)          final3:  *(char *)((short)table + 1)

Unverified reading: with the 16-bit add done before the segment, pointer arithmetic is on
`short` and the loaded 16-bit pointer prints as `*(undefined2 *)` instead of a typed
pointer-to-pointer. Looks semantically equivalent (a 16-bit offset into the data page) but
is less readable and loses pointer typing. Not confirmed whether the R0-R5 change or the
earlier FP/SP change (fc00e4f / 9e0f902) drives it; no A/B was run.

## A/B result (13:05, live Ghidra, OLD language restored in the install)

The old sources (`*.bak_r0r5`, hash-identical to the tested baseline) were put back in the
install and `h8539f.sla` deleted; Ghidra recompiled on start ("compiling language").
Live decompiles of project `new` under the OLD language:
- sci1_post_handshake_status_broadcast_dispatch (0x28b2f): decompiles again, with the
  final2 look (junk table via `(DAT_0001f526 << 1) + -0x74ae`, `halt_baddata`, "bad
  instruction data"). So the decompile failure is caused by the R0-R5 change, not by stale
  project data.
- status_word_composite_multibit_update_via_table (0x22d12) and table_lookup_interp
  (0x14656): back to final2's form (`**(ushort **)((int)in_stack_00000004 + 6)`,
  `undefined2 in_stack_00000004`, `**(ushort **)((int)table + 2)`). So the readability
  regression is also caused by the R0-R5 change. Only these 2 were checked live; for the
  rest, final2 is the old-language reference (no full old-language re-export yet).

Trade-off (reading, not verified for every site): the new semantics are the hardware-correct
ones. They change the resolved bank where the displacement is >= 0x8000 (negative as
signed), e.g. the jump table at 0x28b52 instead of 0x18b52 and `DAT_0001f84a` instead of
`DAT_0000f84a`, but they cost pointer typing in many functions and, in 0x28b2f, a decompile
failure.

State after this test: install = OLD sources (Ghidra has rebuilt the .sla); the new versions
are saved in the install as `h8539f.slaspec.new_r0r5`, `h8539f-arith.sinc.new_r0r5` and
`h8539f.sla.new_r0r5`. The repo's default sources were then restored to HEAD (9e0f902, identical
to the install), so repo == install again; see the next section for where the NEW sources went.

## Committed as an optional copy (2026-10-26 evening)

- Commit `6780a4c` on `main` (local; NOT yet pushed, the push from this machine failed because
  the bundled git has no GitHub login: "could not read Username"; push from GitHub Desktop).
  Remote `origin/main` is still `9e0f902`.
- Adds `optional/r0r5-add-before-segment/` with `h8539f.slaspec`, `h8539f-arith.sinc`
  (byte-identical to the tested NEW versions) and a `README.md`, plus a section and file-layout
  entry in the root `readme.md`. The default grammar files are unchanged (hash-identical to
  the tested OLD baseline).
- Placed outside `h8/data/languages` so nothing that scans that folder compiles it.
- Considered and not done: one source with `@ifdef R0R5_ADD_FIRST` plus a second `.ldefs`
  language entry (a thin wrapper `.slaspec` that defines the switch). Was being tested in a
  temp copy (`convert_dual.py`, `flatten_check.py` in `%TEMP%\r0r5`) when the user chose the
  simple placeholder; nothing from it was compiled or committed.

## Diagnosis of Regression 1 (evening of 2026-10-26, `ghidra_mod` rig)

Rig: `Downloads\ghidra_12.2_DEV_20260818\ghidra_mod` is a hard-linked copy of `ghidra_stock` (no extra
disk; C: has about 2 GB free) with the NEW grammar compiled in (its h8539f.sla is byte-identical to the
one tested earlier). The stock install was not touched. Project: a fresh copy of `new` in
`%TEMP%\r0r5\proj2`, run with analyzeHeadless while Ghidra was closed. Scripts: DiagBadAddrs.java
(plus DumpSwitchPcode.java, ClearStaleSwitchRefs.java) in `%TEMP%\r0r5\scripts`.

Verified:
- p-code at 0x28b4c under the new grammar: `INT_ADD R4, 0x8b52` (2 bytes) -> `CALLOTHER spSegment(EP, .)`
  -> `LOAD`.
- 0x28b8e is byte +1 of the 3-byte `bne #0x28d1f:16` at 0x28b8d; 0x28c8a is byte +1 of the 6-byte
  `mov:g.w #0xef:16,@0xf54a:16` at 0x28c89. Both are inside
  sci1_handshake_status_byte_builder_and_f516_hibit_writer (0x28b89).
- No flow, fall-through or p-code constant in 0x28b2f-0x28d30 targets either address (0 hits), so they
  are not produced by a branch in the listing.
- The 8 real table targets are all valid instruction starts, in OTHER functions: 0x28b62
  f588_duty_gate_f516_bit11_set; 0x28b89, 0x28ba8, 0x28c5a, 0x28c78, 0x28c83, 0x28cba
  sci1_handshake_status_byte_builder_and_f516_hibit_writer; 0x28cc8
  sci1_send_final_handshake_byte_85_and_park_phase_machine.
- With the `jmp @R4` at 0x28b50 flow-overridden to CALL_RETURN (no switch recovery) the function
  decompiles (override reverted afterwards). The trigger is the switch recovery.
- Tests that did NOT change the failure (same two addresses each time):
  A. deleting the 8 stale COMPUTED_JUMP refs (earlier);
  B. replacing `spSegment` in the one constructor used at 0x28b4c (`eaw_disp16`, R4) with inline
     `(zext(EP) << 16) | zext(off)`;
  C. giving that instruction the OLD shape with the CORRECT address:
     `local d:2 = disp16; spSegment(EP, R4) + zext(d)` (= 0x28b52 + R4).
  So the failure does not depend on the segment op or on the 16-bit add. What changed is the table the
  decompiler reads: the new grammar reads the real table at 0x28b52 and follows it into the case code,
  while the old grammar read junk at 0x18b52 and never followed the real targets.
- The two `switch(` hits in final2 were this function's real `switch(DAT_0001f526)` and a mention in
  its plate comment, so `switch(` 2 -> 0 is only this function. "Could not recover jumptable" is 6 in
  both exports (unchanged).
- Existing `disp8_banked` / `disp16_banked` (slaspec ~560-576) already use add-then-segment
  (`addrLo = R4 + disp; spSegment(EP, addrLo)`), so the form itself is not new to the grammar.

Reading: Regression 1 looks like a decompiler switch-recovery problem exposed by correct addressing,
not a p-code-shape bug in the new constructors. Not proven.

Unknown: why the decompiler asks for byte +1 of an instruction while following the correct table.
Ideas: decompiler debug dump (decomp_dbg) of 0x28b2f; look for another switch whose targets lie in
other functions and see if it behaves the same; a manual jump-table override.
Regression 2 (undefined*/casts) is NOT diagnosed here; whether the p-code shape drives it is untested
(a test on the two sample functions with the OLD shape for all R0-R5 forms would show it).

## Next steps (not done)

1. Diagnose 0x28b8e / 0x28c8a (stale refs are ruled out). Options: manual jump-table
   override at 0x28b50 with the 8 real targets (try in the temp copy first); A/B against the
   pre-change language (`*.bak_r0r5`) on a temp copy; compare against the patched core
   (branch `base_`, 53929579f8).
2. Decide whether to accept the failure meanwhile (the final2 body was already unusable per
   the old plate comment), and whether the readability regression (Regression 2) is
   acceptable. Find out whether R0-R5 or the earlier FP/SP change drives it (A/B on a
   temp copy).
3. Read the remaining 44 neutral function diffs, or spot-check the ones touching R4/R5 disp forms.
4. Finish the R0-R5 checks in the verify list: run on the stock (unpatched) core as well as the
   patched one; look at the 6 functions that still have stack arrays.
5. Once the cause is fixed, replace the default `h8539f.slaspec` / `h8539f-arith.sinc` with the
   corrected version and remove `optional/r0r5-add-before-segment/` and its readme.md section.
   Also decide about regenerating/committing the repo `h8539f.sla` (todo item 15).
6. Update `todo.md` item 2 (conversion done, regression above) and
   `todo-r0-r5-displacement-order.md` (its two-constructor suggestion does not compile).

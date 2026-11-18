# Session 28 (2026-10-27): R0-R5 regression work - sign-split hybrid grammar fixes Regression 2; Regression 1 (0x28b2f) still open, cause not yet found

"Verified" = observed (compile output, live headless Ghidra, text of the exports).
"Unverified" = inference. Follows review27.md. Draft bug report: `ghidra-issue-draft-segmentop-switch-recovery.md`.

## Summary

1. Regression 2 (more `undefined*`, pointer casts, `(short)` casts) is fixed by splitting every
   R0-R5 displacement constructor by the SIGN of the displacement (hybrid grammar, below). Full-ROM
   numbers are in the table. This is a grammar-level workaround that restores the old typing, not a
   statement that the new semantics are wrong.
2. Regression 1 (decompile failure at 0x28b2f) is NOT fixed by the hybrid and the cause is NOT known.
   It must be diagnosed in the decompiler (switch recovery) and reported to Ghidra if confirmed.
   The datasheet does not need another look: the addressing semantics are settled (review27 / the
   todo-r0-r5 plan, Table 1-10).

## The hybrid grammar (verified)

For every R0-R5 `@(disp8/16,Rn)` constructor:

- displacement sign bit clear: old shape `spSegment(PAGE, Rn) + disp` (keeps pointer typing)
- displacement sign bit set:   new shape `spSegment(PAGE, Rn + disp)` (hardware-correct wrap)

Counts: 24 constructors in `h8539f.slaspec` (eab/eaw_disp8/16 x R0..R5) and 276 inline forms in
`h8539f-arith.sinc` (the generator asserts both counts). The sign test is a 1-bit alias field
`disp8sg = (7,7)` / `disp16sg = (15,15)` in the same token, compared with `= 0` / `= 1`.

What did NOT work (sleigh):
- `disp < 0` / `>= 0` on the signed field that is also an operand: "syntax error" (same restriction
  as a bare `Rn < N`, review27).
- An unsigned multi-bit alias with `disp8u < 0x80`: compiles but did not finish in more than 6
  minutes of CPU (stopped; a normal compile takes under 1 second). Plain equality on a 1-bit field
  compiles in seconds.

Compile (verified): 120846 bytes, no errors; "66 unnecessary extensions/truncations" (old 44, new 54).

Known semantic limit (reading, not tested): for non-negative displacements the hybrid keeps the OLD
behaviour, so an Rn + disp that overflows 16 bits goes into the next page instead of wrapping. That
is identical to final2, so it is not a new regression.

## Full-ROM comparison, RVR 21000011 (741 functions, verified)

Method: headless `DecompAll.java` (every function, 60 s each) on a copy of project `new`, program
`RVR_1998_x3 4g63t 21000011 md352553.hex`, under three grammars: old = ghidra_stock, new = NEW
grammar in ghidra_mod, hybrid = hyb4 in ghidra_mod. The new-vs-old deltas reproduce review27
(+72 pointer casts, +36 `(short)`, `halt_baddata` 11 -> 6), so the method matches the exports.

| Metric | old | new | hybrid | new-old | hybrid-old |
|---|---|---|---|---|---|
| decompile FAIL | 0 | 1 | 1 | +1 | +1 |
| `undefined*` | 1657 | 1703 | 1652 | +46 | -5 |
| pointer casts | 3813 | 3885 | 3816 | +72 | +3 |
| `(short)` casts | 97 | 133 | 115 | +36 | +18 |
| `in_stack_` | 163 | 163 | 163 | 0 | 0 |
| stack arrays | 55 | 55 | 55 | 0 | 0 |
| `register0x` | 0 | 0 | 0 | 0 | 0 |
| `halt_baddata` | 11 | 6 | 6 | -5 | -5 |
| `unaff_` | 102 | 90 | 90 | -12 | -12 |
| `switch(` | 2 | 0 | 0 | -2 | -2 |
| spacebase warnings | 16 | 16 | 16 | 0 | 0 |

- The only failing function is `sci1_post_handshake_status_broadcast_dispatch` (0x28b2f) in both
  new and hybrid. `switch(` 2 -> 0 is that one function (review27).
- Biggest hybrid improvements over new: status_word_composite_multibit_update_via_table (23 -> 6),
  f26e_o2_closed_loop_output_blend_via_table (8 -> 1), eeac_adaptive_trim_learn_update_via_table
  (6 -> 2), isc_stepper_blend_from_struct (4 -> 0), table_3axis_interp_triple and table_lookup_interp
  (5 -> 1).
- Hybrid leftovers vs old: `(short)` +18, and tcu_solenoid_diagnostic_calc (0x2c2d9) undefined
  11 -> 18 (+7). Unverified: probably the negative-displacement forms that correctly use the new
  shape. Not looked at.
- Sample probe (`DecompSample.java`, same grammar swap): 0x22d12 line
  `uVar3 = **(ushort **)((int)in_stack_00000004 + 6);` is back to final2's form under the hybrid
  (new grammar: `*(ushort *)*(undefined2 *)(in_stack_00000004 + 6)`).

PITFALL: `analyzeHeadless -process` with no program name runs the script on ALL 6 programs in the
project, each overwriting the same output file. A first comparison used the last program (787
functions, not the RVR ROM) and was thrown away. Always pass `-process "<program name>"`.

## State of the rig (2026-10-27 ~13:35)

- Install `ghidra_mod\Ghidra\Processors\h8\data\languages` holds the HYBRID grammar (slaspec,
  arith.sinc, .sla). Backups beside them: `*.bak_hyb`, `*.bak_all3` (the NEW grammar).
- `ghidra_stock` (old grammar), the repo default grammar, `optional/r0r5-add-before-segment/`, and
  project `new` were not touched. ghidra_mod and ghidra_stock files are separate (1 hard link each).
- Scratch in `%TEMP%\r0r5`: `lang_hyb4` (the full hybrid sources and compiled .sla), `lang_t`
  (NEW baseline), `hyb4_all.py` (generator), `cmp3.py` (metrics), `scripts\DecompAll.java`,
  `scripts\DecompSample.java`, `run_rvr3.ps1`, `rvr\` (valid results). `all_*.txt` and
  `all3_progress.txt` directly in `r0r5` are the INVALID mixed-program runs.

## Regression 1: where the diagnosis stands

Verified earlier (review27) and again here: with the new semantics the decompiler reads the real
8-entry table at 0x28b52 and then fails with "No pcode could be generated at address r0x00028b8e"
(and 0x28c8a), both mid-instruction. The hybrid does not change this because the displacement is
negative (-0x74ae) so it takes the new shape. Flow-overriding `jmp @R4` to CALL_RETURN makes the
function decompile. Three grammar variants of the load address and deleting the stale refs did not
change the failure.

Unverified reading: the decompiler's jump-table recovery mishandles a BRANCHIND whose target is a
user-defined SEGMENTOP result (`spSegment(EP, R4)`), or follows case targets in other functions
incorrectly. Nothing has been read in the decompiler source yet.

## Next steps (not done)

1. Diagnose in the decompiler: read the jump-table recovery path (JumpBasic / JumpTable /
   EmulateFunction in the decompiler's jumptable.cc and the emulator) and how SEGMENTOP is evaluated
   there; run with decompiler debug output (decomp_dbg) on 0x28b2f; check the stock core as well as
   the patched one (review27 verify-list item 4 is still open).
2. Reproduce on an unmodified Ghidra release, then cut a minimal reproducer program and finish the
   draft issue (`[CONFIRM]` items in it).
3. Decide where the hybrid goes: update `optional/r0r5-add-before-segment/` (preferred, it does not
   fix 0x28b2f) or replace the default grammar after the Ghidra answer.
4. Check the hybrid leftovers (`(short)` +18, tcu_solenoid_diagnostic_calc +7).
5. Update `todo.md` item 2 and `todo-r0-r5-displacement-order.md` (still pending from review27).

## UPDATE (2026-10-27 evening): ROOT CAUSE OF REGRESSION 1 FOUND - it is OUR cspec, not a Ghidra bug

This supersedes the "cause not yet found" wording above and the "Next steps" list (steps 1-2).
The draft Ghidra issue is WITHDRAWN (see its header). Do not file it.

### Cause (verified)

`h8539f.cspec` line 15 has `<funcptr align="2" />`. In the decompiler (Architecture::decodeFuncPtrAlign,
architecture.cc ~1050) the align value is turned into a bit count: align=2 is binary 10, so the first
set bit is bit 1 and `funcptr_align` = 1. JumpBasic::buildAddresses (jumptable.cc ~1486-1497) then does
`mask = (mask >> bit) << bit; addr &= mask;` on EVERY recovered switch target, i.e. it clears bit 0.
H8/500 code is byte aligned (the grammar says `define alignment=1;`), and 2 of the 8 case targets of
0x28b2f are odd:

- 0x28b89 -> 0x28b88 (one byte before the real target, so it lands inside the preceding instruction; mid instruction)
- 0x28c83 -> 0x28c82 (mid instruction)

Evidence chain:
- `max_baddata` is 4 (architecture.cc:1426, flow.cc:550 countBadData): the "Bad instruction count
  exceeded" error means the flow met MORE THAN 4 undecodable addresses; the message only shows the last.
- Raising max_baddata to 200 for one run (DecompHighBad.java, read-only) made 0x28b2f decompile and
  printed: "Instruction at (ram,0x00028c83) overlaps instruction at (ram,0x00028c82)" and "Removing
  unreachable block" for 0x28b89, 0x28b90, 0x28c83, 0x28c89, 0x28c91, 0x28c97, 0x28ca4: the decompiler
  was following 0x28b88 / 0x28c82 instead of the real targets.
- Listing facts (DumpCases.java): all 8 case targets decode cleanly with correct lengths and
  fall-throughs; 0x28b8e and 0x28c8a are inside the instructions at 0x28b8d (bne, 3 bytes) and 0x28c89
  (mov.w, 6 bytes). Only the two ODD targets are affected, the six even ones are not.
- Earlier "refuted" ideas stay refuted: stale refs, segment op, 16 bit add, table over-read.
  Also ruled out here: context-register decoding (targetBase/targetReg are set only locally inside
  single constructors), SLEIGH alignment (=1), SegmentOp::execute (the injected body
  `(zext(base)<<16)|zext(inner)` is fine).
- Why the OLD grammar never showed it: it read junk at 0x18b52 and never reached the real odd targets.

### Test of the fix (verified, ghidra_mod only; ghidra_stock and the repo were NOT changed)

Edit: `<funcptr align="2" />` -> `<funcptr align="1" />` (align=1 gives 0 bits = no masking). Done
byte-exact (Latin-1 round trip) in `ghidra_mod\...\h8539f.cspec`, backup `h8539f.cspec.bak_fp`.
Full-ROM decompile of the RVR 21000011 program (741 functions), results in `%TEMP%\r0r5\rvr_fp`:

| Metric | old grammar | new grammar | hybrid | new-old | hybrid-old |
|---|---|---|---|---|---|
| decompile FAIL | 1 | 0 | 0 | -1 | -1 |
| `undefined*` | 1562 | 1625 | 1572 | +63 | +10 |
| pointer casts | 4120 | 4205 | 4135 | +85 | +15 |
| `(short)` casts | 85 | 126 | 107 | +41 | +22 |
| `halt_baddata` | 6 | 7 | 7 | +1 | +1 |
| `switch(` | 0 | 2 | 2 | +2 | +2 |
| `in_stack_`, stack arrays, register0x, spacebase | same | same | same | 0 | 0 |

ALL THREE columns use the FIXED cspec, so this is the like-for-like grammar comparison.
- 0x28b2f decompiles under new and hybrid (and now with its real `switch`). The "old" grammar now
  FAILS on it (1 failure): that is expected, it reads the junk table; the old grammar is the one being replaced.
- The hybrid is still clearly better than the new grammar on readability (undefined +10 vs +63,
  pointer casts +15 vs +85), i.e. the sign-split hybrid is still worth keeping for Regression 2.

### The cspec change alone is big (verified, needs your decision)

Comparing the stock cspec with the fixed cspec on the SAME old grammar (cspecdiff.py): 233 of 741
functions change, including parameter lists (e.g. tcu_shift_torque_and_knock_mgmt gets `(void)` and
different locals). Probable reason (not checked in the source): funcptr_align is also used in
coreaction.cc:1259 and ruleaction.cc:9903, so call targets to ODD addresses (functions at 0x28b2f,
0x28b89 ... are odd) were being masked to the wrong even address in the stock config. Unverified:
whether every one of the 233 changes is an improvement. Absolute metrics are therefore NOT comparable
with the earlier stock-cspec runs in `rvr\` (old: undefined 1657, pointer casts 3813, short 97, 0 fails).

### State of the rig now

- `ghidra_mod` language folder: HYBRID grammar + patched cspec (align=1). Backups: `*.bak_hyb`,
  `*.bak_all3` (NEW grammar), `h8539f.cspec.bak_fp` (align=2).
- Untouched: `ghidra_stock`, the repo default grammar and cspec, `optional/r0r5-add-before-segment/`,
  project `new`.
- New scratch in `%TEMP%\r0r5`: `rvr_fp\` (valid results), `run_fp.ps1`, `cspecdiff.py`,
  `scripts\DumpCases.java`, `scripts\DecompHighBad.java`, `dumpcases_out.txt`, `highbad_out.txt`.

### Next steps (replace the earlier list)

1. Decide on the cspec fix: set `<funcptr align="1" />` (or remove the element) in the repo
   `h8/data/languages/h8539f.cspec`. It fixes 0x28b2f and probably improves calls to odd-address
   functions, but it changes 233 functions, so review a sample first (especially functions whose
   prototype changed) before it becomes the default.
2. Decide where the hybrid goes: with the cspec fix it has 0 failures and near-old readability, so
   it can replace the default grammar, or stay in `optional/` until the cspec change is reviewed.
3. Check the hybrid leftovers: `(short)` +22 and tcu_solenoid_diagnostic_calc (0x2c2d9) undefined +7.
4. Why was `align="2"` added? Check git history/comments for the cspec line before changing it.
5. Update `todo.md` item 2 and `todo-r0-r5-displacement-order.md` (still pending from review27).
6. Optional upstream note (not a bug): a mis-set funcptr align gives a misleading failure
   ("Bad instruction count exceeded" shows only the LAST bad address). Not worth an issue by itself.


## UPDATE (2026-10-28): further testing results (2x2 and call resolution)

Correction: cmpmulti.py / cmp2x2.py compared the wrong field for FAIL, so my earlier "no new decompile failures" claim was unfounded. True counts come from `DECOMPALL done ... failures=N` and failnames.py:
- The switch function (0x28b2f RVR 21000011/12, 0x28b21 EVO5) fails under either change alone and decompiles under both. Legnum 0x28acb now decompiles too.
- Six unrelated functions (adc_sensor_convert_multi, byte_sum_banked, sum_byte_array, FUN_00017160, FUN_00017206, FUN_0001722b) fail identically in every config.
- No program has more failures under "both" than stock.

Unresolved indirect calls, count of `(*name)()` (callres.py):

| ROM | stock | cspec-only | grammar-only | both |
|---|---|---|---|---|
| RVR 21000011 | 686 | 1 | 684 | 1 |
| RVR 21000012 | 688 | 0 | 686 | 0 |
| RVR w4a51 | 948 | 0 | 948 | 0 |
| Legnum | 595 | 0 | 595 | 0 |
| EVO5 | 665 | 0 | 663 | 0 |

Interpretation: funcptr align=2 clears bit 0 of constant call targets, so every call to an odd-address function stays an unresolved indirect call. align=1 resolves them to direct calls. This explains the undefined/stack-array/in_stack_ shifts (resolved calls expose their stack args). It is a real improvement, not a regression. The grammar change is independent and benign except (short) casts +18..52.

Recommendation: set `<funcptr align="1" />` in h8539f.cspec and adopt the hybrid grammar. Not yet applied to the repo.

## UPDATE (2026-10-28 evening): HYBRID GRAMMAR HAS A HOLE (not yet understood) - SUPERSEDED, WRONG: see CORRECTION at the end and review29.md

Source reading (Ghidra decompiler): TypeOpSegment::propagateType (typeop.cc ~2561) only propagates pointer types between segmentop input slot 2 and the output, and only when the incoming type is already TYPE_PTR. RuleLoadVarnode::checkSpacebase ignores the page part. So `spSegment(PG, Rn) + disp` keeps pointer typing, `spSegment(PG, Rn + disp)` can lose it. Not yet proven to be a Ghidra defect.

Whole-ROM pcode-shape tally (RVR 21000011, hybrid installed, DumpShape2.java): the sign split does NOT behave as designed for word accesses.
- d8 forms (all R0-R5): all positive -> old shape (correct).
- d16 byte forms: positive -> old shape (correct); negative -> new shape (correct).
- d16 mov:g.w via eaw_disp16: positive displacements get the NEW shape (26 of ~30 cases, e.g. 0x2c64b `mov:g.w @(0x32f0:16,R0),R1`). or.w with the same displacement (0x329e) gets the old shape, so it is instruction/constructor dependent, not a function of the displacement bits.
- These are exactly the pointer-table lookups, and explain tcu_solenoid_diagnostic_calc: `*(short **)(idx<<1 + 0x32f0)` became `(short *)*(undefined2 *)(idx*2 + 0x32f0)`.
- Moving the sign alias to bit (0,0) (lang_x1) changed which forms misbehave (words all old shape incl. negative; some bytes new shape), so the 1-bit alias test is not reliably selecting constructors. Cause unknown: either bit numbering/alias semantics in big-endian SLEIGH, or a sleigh constructor-matching problem. ghidra_mod restored to hyb4 afterwards.

Not safe to call the hybrid grammar finished. Next: minimal reproduction in a tiny language, then decide whether it is our grammar or a sleigh bug (the latter would be a valid upstream report).


## CORRECTION (2026-10-28 late, session 29): the "hole" was stale prototypes, and the hybrid numbers above are contaminated

Full details and evidence: review29.md. What changes here:

- The "HYBRID GRAMMAR HAS A HOLE" section above is WRONG. The sign-split hybrid grammar works as designed: after a fresh parse all 90 R0-R5 d16 instructions in the RVR ROM have the right shape (positive -> old, negative -> new). Not a sleigh bug; no upstream report; its next step (minimal reproduction in a tiny language) is not needed.
- Cause: project `new` stores instruction prototypes shared between instructions with the same constructor tree; they were created when positive and negative displacements had identical trees, so after the .sla changed the stored representative decided the pcode shape for every instruction sharing it. The pcode-shape tallies (DumpShape*.java) and the (0,0) alias experiment (lang_x1) were measuring that, so their conclusions are void.
- Every number in this file measured on project `new` after a grammar swap (the tables above, rvr\, rvr_fp\, multi\, multi2\) was taken without re-disassembly and is slightly off. Re-measured with `scripts\ReDisasm.java` run first (RVR 21000011, cspec align=1): hybrid-old `undefined*` +0 (was +10), pointer casts +5 (was +15), `(short)` +22 (unchanged), 0 failures; tcu_solenoid_diagnostic_calc undefined old 11 / new 18 / hybrid 11 (the "+7" leftover was stale). old and new columns also moved (old `undefined*` 1562 -> 1574).
- Unchanged: Regression 1 = cspec `align="2"` (switch function fails under either change alone, decompiles with both); `align="1"` resolves calls to odd-address functions. The other ROMs (multi\, multi2\) were not re-run with ReDisasm.
- New rule: after any .slaspec/.sinc change, run ReDisasm.java before measuring pcode or decompiling an existing project.

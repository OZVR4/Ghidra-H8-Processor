# TODO: convert R0-R5 displacement addressing to "add disp, then segment"

Written 2026-10-25. Nothing below has been compiled or tested; the "verified" and
"NOT verified" lists say what was actually checked.

## Why

H8/500 Programming Manual (datasheets/h8539f/H8_500_programming.pdf):
- PDF p.28, Table 1-10 (1), mode 3 `@(d:8,Rn)` / `@(d:16,Rn)`: Rn contents + disp
  form one 16-bit "Result" (bits 15-0). The page register (DP/TP/EP) supplies
  bits 23-16 separately.
- PDF p.29, Table 1-10 (2), note 2: DP for R0-R3, EP for R4/R5, TP for R6/R7.
- The diagram does NOT state in words that the sum wraps within 16 bits. Do not
  quote it as an explicit statement.

So the faithful P-code is `spSegment(PAGE, Rn + disp)`, not `spSegment(PAGE, Rn) + disp`.
The second form also leaves the nested SEGMENTOP-inside-INT_ADD shape that
RuleLoadVarnode::vnSpacebase does not unwrap (PR #9564 / issue #9561).

## Already done (FP/SP)

- Commit fc00e4f: shared FP/SP displacement operands.
- Commit 9e0f902: h8539f-arith.sinc inline single-operand forms (tst/clr/neg/not/
  shift/rotate) on @(disp8/16,FP|SP). Commit message says generic forms are now
  "limited to Rn<6". Its stated results: stack arrays 237 -> 34, register0x 210 -> 0,
  0 failures. It does not say which core those were measured on.

## Still on the old order (to convert)

h8/data/languages/h8539f.slaspec, lines ~635-638, all guarded `Rn < 6`:

    eab_disp8:  "@("^disp8^":8,"^Rn_banked^")"   is Rn_banked & Rn < 6 & mode = 14 & sz = 0; disp8  { tmp = (Rn_banked + disp8);  export *:1 tmp; }
    eaw_disp8:  ... mode = 14 & sz = 1 ... export *:2 tmp;
    eab_disp16: "@("^disp16^":16,"^Rn_banked^")" is Rn_banked & Rn < 6 & mode = 15 & sz = 0; disp16 { tmp = (Rn_banked + disp16); export *:1 tmp; }
    eaw_disp16: ... mode = 15 & sz = 1 ... export *:2 tmp;

`Rn_banked` already exports the segmented pointer (R4/R5: `spSegment(EP, Rn)` at
lines ~583-584), so `Rn_banked + disp` adds AFTER the segment.

Also expected, but NOT read: the "generic forms" in h8/data/languages/h8539f-arith.sinc
that commit 9e0f902 left limited to Rn<6.

## Reference patterns to copy

- FP/SP, lines ~641-648:
      eab_disp8: "@("^disp8^":8,FP)" is Rn = 6 & mode = 14 & sz = 0; disp8 { local off:2 = FP + disp8; local a:4 = spSegment(TP, off); export *:1 a; }
- R4/R5 EP, lines ~564-565 and ~573-574 (`disp8_banked`, `disp16_banked`): a separate
  constructor family; already correct order.

## Suggested change

Replace the four generic `Rn < 6` constructors with explicit per-register ones
(or a helper) so each does:

    local off:2 = Rn + disp;  local a:4 = spSegment(PAGE, off);  export *:N a;

with PAGE = DP for R0-R3 and EP for R4/R5. Keep the display strings identical
(`@(disp:8,R0)` etc.) so disassembly text does not change. Watch for constructor
ambiguity if the old generic ones are left in place. Do the same in
h8539f-arith.sinc.

## Check before starting

- Find stragglers: search all .slaspec/.sinc for `Rn_banked +`, `Rn_banked2 +`,
  and `spSegment(...) +`. The `spSegment(...) +` search over h8539f.slaspec only
  found nothing else, but the .sinc files were not searched.
- R0-R3: how `Rn_banked` is defined for them was NOT read. They appear to use
  DP via the macro near line 448 (`result = spSegment(DP, reg)`).
- Two copies of h8539f.slaspec exist with the same line numbers:
  the repo (github\Ghidra-H8-Processor\h8\data\languages) and
  Downloads\ghidra_12.2_DEV_20260818\ghidra_stock\Ghidra\Processors\h8\data\languages.
  Edit the one that is actually deployed and keep them in sync.

## Verify after

1. Compile with sleigh; confirm no new constructor conflicts.
2. Full-ROM decompile before/after, as in 9e0f902: count stack arrays
   (`auStack_`), `register0x` cases, and decompile failures.
3. Run on a stock, unpatched core as well as the patched one.
4. Look at the 34 stack arrays that 9e0f902 left, to see whether any are R0-R5 related.

## Expected effect

Hardware-faithful P-code for R0-R5 displacement access. It should not change stack
symptoms, since DP/EP are not stack pointers. It still disagrees with Table 1-10 today.

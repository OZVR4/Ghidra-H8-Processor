# Optional: R0-R5 displacement added before the segment (PLACEHOLDER)

**Update 2026-10-29: superseded; kept for reference.** The default files in `h8/data/languages/` are now a *hybrid* of this variant and the old form: for R0-R5 16-bit displacements whose sign bit is clear, the original `spSegment(DP|EP,Rn)+disp` form is kept; with the sign bit set, the form described below (add in 16 bits, then apply the segment) is used. The default cspec is now `<funcptr align="1" />`.

What the 2026-10-29 re-measurements (fresh instruction parse, five ROMs) changed in the "Known regressions" section below:

- The 0x28b2f failure has a cause: it fails when only the grammar change or only `align="1"` is applied, and decompiles when both are. Same for 0x28b21 (EVO5).
- The typing regression of this unsplit variant is real (RVR 21000011: `undefined*` +63, pointer casts +85 versus the old grammar); the hybrid brings it to +0 / +5, which is why the hybrid is the default.
- Earlier numbers were taken from stale instruction prototypes stored in the project and are slightly off. After changing a `.slaspec`, clear and re-disassemble before measuring.

See `review28.md` (Correction section) and `review29.md`.

**Status: experimental placeholder. This is NOT the default.** It is kept here so it can be
tried, or replaced, until the underlying issue is corrected properly.

## What this is

Two drop-in replacements for the files of the same name in `h8/data/languages/`:

- `h8539f.slaspec`
- `h8539f-arith.sinc`

They are the default files as of commit `9e0f902` with one change: the R0-R5 displacement
addressing forms. Nothing else differs.

## What changed

The generic `Rn_banked & Rn < 6` displacement constructors are split into one constructor per
register (R0..R5) that adds the displacement in 16 bits and then applies the segment:

    spSegment(DP, Rn + disp)    for R0-R3
    spSegment(EP, Rn + disp)    for R4, R5

instead of `spSegment(DP|EP, Rn) + disp`. The FP/SP forms already work this way. 4
constructors in `h8539f.slaspec` and 46 in `h8539f-arith.sinc` become 24 and 276. Display
text is unchanged.

## Why

The default form adds a sign-extended displacement to the 32-bit segmented address, so a
displacement of 0x8000 or more (negative as signed) lands in the wrong bank. Example in the
RVR ROM (21000011 md352553): `mov:g.w @(-0x74ae:16,R4),R4` at 0x28b4c reads its jump table
from 0x18b52 (ordinary code bytes) with the default files, instead of the real table at
0x28b52.

## Known regressions (why it is not the default)

Measured by comparing decompiler exports of the RVR ROM with and without this change. The
function failure and the typing change were also re-checked live, with the default files, on
three functions.

- Stack, register and spacebase metrics are unchanged.
- `sci1_post_handshake_status_broadcast_dispatch` (0x28b2f) no longer decompiles ("Bad
  instruction count exceeded", no p-code at 0x28b8e; the decompiler also warns at 0x28c8a).
  With the default files it decompiles, although from the wrong jump table. Cause not found.
- About 20 functions show more `undefined*` types and pointer casts (`undefined*` +54,
  pointer casts +72, `(short)` casts +36). For example
  `**(ushort **)((int)p + 6)` becomes `*(ushort *)*(undefined2 *)(p + 6)`. Mechanism not
  confirmed.

## Use it

1. Keep copies of your current `h8539f.slaspec` and `h8539f-arith.sinc` (or use git).
2. Copy both files from this folder over the ones in `h8/data/languages/` and, if you run an
   installed copy, over the ones in `<GhidraInstall>\Ghidra\Processors\h8\data\languages\`.
3. Delete the compiled `h8539f.sla` next to them. Ghidra recompiles it the next time it
   starts ("compiling language"), or compile it yourself as described in the main `readme.md`.
4. Restart Ghidra. Programs already in a project keep their analysis; decompile again to see
   the effect.

## Go back

Put the original two files back (`git checkout -- h8/data/languages/h8539f.slaspec
h8/data/languages/h8539f-arith.sinc`), delete `h8539f.sla`, and restart Ghidra.

If the default files change after `9e0f902`, these copies will need re-merging.

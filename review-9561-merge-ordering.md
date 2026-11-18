# 9561 regression: Merge ordering defect in muldiv_u16_scaled

## Status (2026-09-14, updated — supersedes the 2026-09-08 status below)

**SIGSEGV: FIXED and confirmed.** `muldiv_u16_scaled` no longer crashes the decompiler.

**Decompilation: NO LONGER CRASHES, BUT OUTPUT IS CONFIRMED WRONG** (user-confirmed
2026-09-14). This is worse than the 2026-09-08 status below suggests: that status
described the crash turning into a *visible* `LowlevelError`. What actually happens
now is `muldiv_u16_scaled` decompiles with NO error and NO warning beyond the usual
spacebase/deadcode-delay pair, but returns the wrong value (`uVar5`, a pre-division
intermediate) instead of the real scaled-division result, plus an additional
unattributed `in_R1` read. I.e. the `isDead()` fallback silenced the loud failure
(crash / LowlevelError) and left a quiet, more dangerous one (wrong answer, no
diagnostic). See "2026-09-14 update" section near the end for the full picture and
why this changes the diagnosis.

**Also newly confirmed (2026-09-14): for `muldiv_u16` (the non-`_scaled` sibling),
full-SeqNum instrumentation of `CoverBlock::getUIndex` shows `targOp->isDead()` is
FALSE for BOTH of its INDIRECT ops.** This means the crash-fix's `isDead()`-true
fallback branch — the mechanism this document blames for the ordering inversion —
is NOT the code path producing muldiv_u16's bad output. The two functions may still
share a common upstream cause (see below), but not via the literal `isDead()`
branch. This narrows, rather than confirms, the original "same root cause" claim
made in the 2026-09-08 cross-confirmation section.

## Original status (2026-09-08, kept for history — see update above)

**Decompilation: STILL FAILING**, now with a normal, non-fatal `LowlevelError`
("Unable to force merge of op at 0x00014203:ed") instead of a crash. Root cause
of this remaining failure is fully understood and precisely located (see below).
Not yet fixed.

Full investigation history (multiple prior sessions, many ruled-out theories,
debugger tooling notes, etc.) is archived in `review-calling-convention-regression.md`
in this same directory. This document only carries forward what's current and
confirmed, so a fresh session doesn't need to read ~2800 lines of superseded
theories to get up to speed.

## The original crash (fixed)

**File:** `Ghidra/Features/Decompiler/src/decompile/cpp/merge.cc`, `Merge::trimOpOutput`

**Bug:** `PcodeOp::getOpFromConst(op->getIn(1)->getAddr())` decoded a stale/dangling
IOP-encoded pointer (a `CPUI_INDIRECT`'s second input can correctly report `IPTR_IOP`
space while still encoding the address of a `PcodeOp` that has since been deleted).
The decoded pointer was used unconditionally as `afterop`, which
`Funcdata::opInsertAfter`'s inlined `PcodeOp::isMarker()` call then dereferenced
without any liveness check, causing a SIGSEGV (confirmed via direct in-process
stack capture; exact faulting instruction identified as
`test byte ptr [r8+8],40h`, the first instruction of the inlined `isMarker` check).

**Fix (applied, minimal, upstream-ready — 11 lines added, 2 removed, single file):**

```cpp
if (op->code() == CPUI_INDIRECT && op->numInput() > 1 && op->getIn(1)->getSpace()->getType() == IPTR_IOP) {
  PcodeOp *indirectSource = PcodeOp::getOpFromConst(op->getIn(1)->getAddr());
  afterop = indirectSource->isDead() ? op : indirectSource; // Insert copyop AFTER source of indirect, unless stale
}
else
  afterop = op;
```

This follows the same pattern already used at `heritage.cc`'s
`removeRevisitedMarkers`, which validates the same `getOpFromConst` result via
`isDead()` before trusting it — `trimOpOutput` was the one call site (of 25 in
the codebase) that didn't.

**Confirmed:** rebuilt, `force_decompile` on `muldiv_u16_scaled` no longer produces
"Decompiler process died" / SIGSEGV. `segvHandler` diagnostic (see below) no longer
fires for this function.

## The remaining problem: a use-before-define P-code ordering defect

Once the SIGSEGV was fixed, `muldiv_u16_scaled` still fails to decompile — now
with `Merge::mergeOp` throwing `LowlevelError("Unable to force merge of op at
0x00014203:ed")`. This has been traced all the way down to a concrete, confirmed
mechanism:

### The actual defect

For `muldiv_u16_scaled`'s second `divxu.w` instruction (P-code address `0x14203`,
disassembly: `divxu.w @(0xc:8,FP),R2` — the second of two chained divides sharing
the stack parameter `c` as divisor), the P-code sequence numbers are:

| seq | op | effect |
|---|---|---|
| `0xed` | `CPUI_INDIRECT` | defines `outvn` (a size-2 `unique` temp); **reads `invn`** |
| `0x153` | `CPUI_COPY` | **defines `invn`** |
| `0x154` | `CPUI_COPY` | reads `outvn` |

`invn` is read by the op at `0xed` but not actually defined until `0x153` —
**a genuine, literal use-before-define ordering bug.** This is why
`Cover::addRefPoint` computes an inverted Cover range (`0x14203:153-0x14203:ed`,
start > end) for `invn`, why the backward-recursion fallback fills whole
predecessor blocks (`begin-end`) rather than a tight range, and why
`Merge::mergeOp`'s Cover-intersection test correctly fails — every one of these
downstream symptoms is a correct, honest report of a genuinely malformed input,
not a bug in Cover or Merge themselves.

### Why: our own crash fix's fallback position is the direct cause

`Funcdata::opInsertAfter` (`funcdata_op.cc`) already has its own separate
`isDead()`-guarded check, independent from the one added to `trimOpOutput`:

```cpp
void Funcdata::opInsertAfter(PcodeOp *op,PcodeOp *prev)
{
  if (prev->isMarker()) {
    if (prev->code() == CPUI_INDIRECT) {
      Varnode *invn = prev->getIn(1);
      if (invn->getSpace()->getType()==IPTR_IOP) {
        PcodeOp *targOp = PcodeOp::getOpFromConst(invn->getAddr());
        if (!targOp->isDead())
          prev = targOp;
      }
    }
  }
  list<PcodeOp *>::iterator iter = prev->getBasicIter();
  ...
}
```

When `trimOpOutput`'s fix falls back to `afterop = op` (the `CPUI_INDIRECT`
itself, because the decoded IOP target is dead), `opInsertAfter` receives this
`op` as `prev`. Since `prev` is itself a `CPUI_INDIRECT`, this function's own
check re-fires — finds the same dead target, leaves `prev` unchanged — and the
new trim-COPY is inserted immediately after `prev`'s existing position via
`prev->getBasicIter(); iter++`. **Confirmed via direct instrumentation**
(`opInsertAfter op_pc=0x14203 prev_pc=0x14203 prev_seq=0xed prev_opcode=61`):
the trim-COPY is placed right after the `CPUI_INDIRECT` at seq `0xed`, which is
*before* the real, legitimate definition of `invn` at seq `0x153`. That's the
exact, direct mechanism producing the ordering inversion.

**This means the `isDead()` guard's crash-safety property is correct and should
be kept, but its fallback position (`afterop = op`) is not always semantically
correct** — it prevents the crash but can leave the P-code in a genuinely invalid
order for cases like this one.

## Next step (status as of 2026-09-14 — item 3 below has new evidence, not yet resolved)

Find a correct insertion point for the fallback case instead of `op` itself.
Candidate directions, roughly in order of promise:

1. When the decoded IOP target is dead, search forward from `op` for wherever
   the relevant Varnode is actually (re)defined, and insert after that instead
   of after `op`'s own position.
2. Consider whether this is better prevented upstream — e.g. in
   `Heritage::bumpDeadcodeDelay` or wherever `CPUI_INDIRECT`/`CPUI_COPY` pairs
   like this get created/reordered in the first place — rather than patched at
   the trim/insert site after the fact.
3. Confirm whether this ordering defect is itself pre-existing and merely
   exposed by 9561 (as the crash was), or newly introduced by 9561's changed
   `RuleLoadVarnode::vnSpacebase` recursion. **Still not definitively
   determined**, but see the 2026-09-14 update below: both affected functions
   share the `"Unable to track spacebase fully for stack"` warning, which
   points at upstream P-code construction (spacebase/INDIRECT modeling for
   the `__stdcall_far_allstack` family) rather than at `merge.cc`/`cover.cc`
   specifically. Not yet proven, but this is now the leading hypothesis
   ahead of any `merge.cc`-local fix.

## 2026-09-14 update: isDead() ruled out for muldiv_u16; wrong output confirmed for muldiv_u16_scaled after the crash fix

This update consolidates findings from a parallel investigation thread (full
chronological detail in review20.md through review24.md in this directory,
now considered historical/superseded by this summary) plus direct log
evidence gathered this session.

### Finding 1: `targOp->isDead()` is FALSE for both of muldiv_u16's INDIRECTs

Full-SeqNum debug logging was added to `CoverBlock::getUIndex` (cover.cc),
gated to the address range `0x141ba`-`0x141da` (muldiv_u16), logging
`targOp->isDead()` plus a full basic-block op dump for each INDIRECT
encountered. Result, for both INDIRECTs:

```
[muldiv_u16 getUIndex] ENCLOSING INDIRECT op at 0x000141c2 ... targOp->isDead()=FALSE ...
[muldiv_u16 getUIndex] ENCLOSING INDIRECT op at 0x000141ce ... targOp->isDead()=FALSE ...
```

This means the `isDead()`-true fallback branch that this document's original
(2026-09-08) analysis blames for `muldiv_u16_scaled`'s ordering inversion is
**not the mechanism producing muldiv_u16's bad output** — for muldiv_u16,
`isDead()` never evaluates true, so that code path is simply not exercised.
Whatever produces muldiv_u16's wrong `uVar4 = b; return uVar4;` output must
be a different mechanism than the one described in "Why: our own crash fix's
fallback position is the direct cause" above, even though the symptom shape
(spurious value substituted for a real computed result) looks similar.

The block dumps also show a structural oddity worth investigating further:
the two INDIRECTs' target ops live in blocks using two different order-
numbering schemes — one block numbers ops ascending from a small base
(e.g. `0x400002`, `0x600002`, ... `0x12800002`), the other numbers ops
**descending from `0xffffffff`** (e.g. `0x1ffffffe`, `0x3ffffffc`, ...
`0xfffffff0` for the RETURN). This may itself be a clue (possibly indicating
one block was constructed by a different pass, e.g. delay-slot/CROSSBUILD
injection) but has not yet been chased down.

### Finding 2: muldiv_u16_scaled still produces wrong output after the isDead() crash fix

User-confirmed 2026-09-14: `muldiv_u16_scaled` (the function this document's
crash fix targets) no longer crashes and no longer throws `LowlevelError`,
but its decompiled output is wrong. Current decompile:

```c
/* WARNING: Unable to track spacebase fully for stack */
/* WARNING: Restarted to delay deadcode elimination for space: register */

ushort __stdcall_far_allstack_32ret muldiv_u16_scaled(ushort a,ushort b,ushort c,ushort d)
{
  short sVar1;
  ushort uVar2;
  ushort in_R1;
  ushort uVar4;
  undefined2 uVar3;
  ushort uVar5;
  ushort uVar6;
  undefined1 *puVar7;
  undefined1 auStack_84 [128];

  sVar1 = (short)((uint)a * (uint)c >> 0x10);
  uVar4 = in_R1;
  uVar6 = b;
  puVar7 = auStack_84 + 0x82;
  uVar3 = (short)((uint)a * (uint)c);
  uVar6 = (ushort)((uint)uVar6 * (uint)*(ushort *)(puVar7 + 10) >> 0x10);
  uVar2 = sVar1 + (ushort)CARRY2(uVar6,uVar4);
  if ((*(short *)(puVar7 + 0xc) == 0) || (*(ushort *)(puVar7 + 0xc) <= uVar2)) {
    return 0xffff;
  }
  uVar5 = uVar6 + uVar4;
  uVar3 = (short)(CONCAT22(uVar2,uVar6 + uVar4) / (uint)*(ushort *)(puVar7 + 0xc));
  return uVar5;
}
```

Notable: `in_R1` is printed as a bare, unattributed register input (not even
disguised as a parameter copy the way muldiv_u16's `uVar4 = b` was); `uVar3`
(the real scaled-division result, computed via `CONCAT22(...)  / ...`) is
computed but never returned; `uVar5` (`uVar6 + uVar4`, a pre-division sum)
is returned instead. This is the same general shape of bug as muldiv_u16
(real result computed and discarded; a pre-computation intermediate returned
instead), obtained via a genuinely different assembly-level structure (two
chained `divxu.w` instructions and R1/R2/R3 register juggling — see
disassembly below — vs muldiv_u16's single `divxu.w`).

Disassembly (`ghidra:disassemble_function` at `0x141dc`):

```
000141dc: link FP,#0x0:8
000141de: mov:g.w R3,@-SP
000141e0: mov:g.w R2,@-SP
000141e2: mov:f.w @(0x6:8,FP),R0
000141e4: mov:f.w @(0x8:8,FP),R2
000141e6: mulxu.w @(0xa:8,FP) ,R0
000141e9: mulxu.w @(0xa:8,FP) ,R2
000141ec: add:g.w R1,R2
000141ee: addx.w 0x0,R0
000141f2: tst.w @(0xc:8,FP)
000141f5: beq #0x0001420c:8
000141f7: cmp:g.w @(0xc:8,FP),R0
000141fa: bhs #0x0001420c:8
000141fc: mov:g.w R2,R1
000141fe: divxu.w @(0xc:8,FP) ,R0
00014201: mov:g.w R0,R2
00014203: divxu.w @(0xc:8,FP) ,R2
00014206: mov:g.w R1,R0
00014208: mov:g.w R3,R1
0001420a: bra #0x00014212:8
0001420c: mov:i #0xffff:16,R0
0001420f: mov:i #0xffff:16,R1
00014212: mov:g.w @SP+,R2
00014214: mov:g.w @SP+,R3
00014216: unlk FP
00014217: prts
```

Note the real assembly moves `R2` into `R1` at `0x141fc` (before either
division), then moves `R1` into `R0` (the return register) at `0x14206`
*after* both `divxu.w`s. So the assembly genuinely does return a value that
was computed before the divisions — this is NOT simply "decompiler picks
the wrong operand", the real control/data flow is more subtle than
muldiv_u16's case and has not yet been fully re-derived from P-code. Do not
assume this is proven to be the identical bug to muldiv_u16 without doing
that derivation; it is best treated as a second, structurally distinct
symptom of a suspected common upstream cause, not confirmed identical.

### Recommendation for whoever continues this

- **Do not spend further time on the `isDead()`/fallback-position theory as
  the explanation for muldiv_u16's bug specifically** — Finding 1 rules it
  out directly via log evidence. **It IS confirmed as the mechanism for
  muldiv_u16_scaled's bug — see Finding 3 below, resolved 2026-09-14.**
- **The most likely unifying lead for muldiv_u16 (not muldiv_u16_scaled) is
  upstream P-code construction for the `__stdcall_far_allstack` family**,
  specifically whatever produces the `"Unable to track spacebase fully for
  stack"` warning — both functions hit it, both use FP-relative stack access
  via `LINK FP,#0x0`, and both substitute a pre-computation value for a real
  computed result at return. This has not yet been traced to a specific
  `.cspec`/`.slaspec` construct or a specific decompiler pass; that trace is
  the concrete next step for muldiv_u16. (Original review22.md FIFTH UPDATE
  first raised this hypothesis; it has not yet been chased into the actual
  spec files.)
- Still outstanding from every prior review across this entire investigation
  (review19 through review24, and this document): the `sat_add_u16` and
  sibling-function regression check has never been done.

### Finding 3 (2026-09-14, resolved): muldiv_u16_scaled's isDead() IS TRUE — confirms the fallback-position mechanism, does NOT explain muldiv_u16

After widening the `getUIndex` debug instrumentation's address range from
`0x141ba`-`0x141da` (muldiv_u16 only) to `0x141ba`-`0x14217` (covering both
muldiv_u16 and muldiv_u16_scaled) and rebuilding/redeploying decompile.exe,
fresh logs show, for muldiv_u16_scaled's INDIRECT at P-code address `0x141e9`:

```
[muldiv_u16 getUIndex] ENCLOSING INDIRECT op at 0x000141e9 enclosing_seq=1de00002 enclosing_time=e6
targOp=... targOp->isDead()=TRUE targOp->getAddr()=0x000141e9 targOp->code()=COPY
targOp->getSeqNum().getOrder()=1e000002 targOp->getTime()=3b
```

**`isDead()` is TRUE here** — unlike muldiv_u16, where it was FALSE for both
INDIRECTs (Finding 1). This means:

- **muldiv_u16_scaled's wrong output is plausibly explained by the exact
  mechanism this document's original (2026-09-08) analysis identified**: the
  crash-fix's `isDead()`-true fallback (`afterop = op`, i.e. insert relative
  to the INDIRECT itself rather than its — now-dead — indirect-source op)
  produces an ordering inversion, which is a real, confirmed, still-present
  defect in the fallback's *positioning* logic, independent of whether the
  crash itself is fixed.
- **muldiv_u16's wrong output is NOT explained by this mechanism** — it
  never touches the `isDead()`-true branch at all (Finding 1). Despite the
  visually similar symptom (a pre-computation value substituted for a real
  result at RETURN), the two functions' bugs go through genuinely different
  code paths in `getUIndex`/`mergeOp`, and should be treated as two separate
  (if possibly related-at-a-deeper-level) defects, not one unified bug.
- This resolves the "not yet proven identical" caveat from the 2026-09-14
  update above: they are now **known non-identical at the `isDead()`
  mechanism level**. A shared upstream cause (e.g. both being downstream of
  spacebase-tracking failure for this calling convention) remains an open,
  unconfirmed hypothesis, but any fix for one should not be assumed to fix
  the other.

**Concrete next step, muldiv_u16_scaled specifically:** implement item 1 from
the "Next step" list above (search forward from `op` for the Varnode's real
definition when the decoded IOP target is dead, instead of falling back to
`op` itself) and re-verify against this function's decompile — the fix
target and expected before/after are now precisely characterized.

**Concrete next step, muldiv_u16 specifically:** unrelated to the above;
pursue the spacebase/`.cspec`/`.slaspec` trace instead, since the `isDead()`
mechanism is now conclusively ruled out for this function.

**Tooling notes from this session, for future reference:**
- The debug instrumentation's address-range guard was scoped too narrowly
  (`0x141ba`-`0x141da`, muldiv_u16 only) to ever catch muldiv_u16_scaled
  (`0x141dc`-`0x14217`) — always check debug-log address filters cover every
  function you intend to exercise, not just the one that motivated adding
  the instrumentation.
- Confirmed again this session: a `decompile.exe` rebuild can report a
  client-side timeout while completing successfully server-side — verify via
  `LastWriteTime` on the build output rather than assuming timeout=failure.
- Confirmed again this session: copying the rebuilt `decompile.exe` to the
  deployed path can fail with a locked-file error while a wrapping
  `Write-Output "copy succeeded"` still prints — always verify via matching
  `LastWriteTime` on both source and destination after copying, never trust
  a wrapping script's own success message.
- The Ghidra MCP connector dropped (`WinError 10061 / connection actively
  refused`) after the deployed `decompile.exe` was replaced while a live
  PyGhidra process was using it; required a full PyGhidra restart to recover
  — consistent with the existing tooling note above about intermittent
  connector drops.

## MAJOR CORRECTION (2026-09-14, same day, after Finding 3): the spacebase-tracking hypothesis was already investigated and explained in `h8539f.cspec` — this was missed by every prior session including this one

`__stdcall_far_allstack` / `__stdcall_far_allstack_32ret` are NOT decompiler-
inferred labels — they are hand-authored `<prototype>` entries in
`Ghidra-H8-Processor/h8/data/languages/h8539f.cspec` (lines 233-296 as of this
writing), maintained with extensive inline change-history comments. This file
is the one review22.md's FIFTH UPDATE recommended reading but, per that
document's own text, never actually got read. It should have been read before
concluding the spacebase warning was an unexplained, fresh lead.

**The comment block immediately preceding these prototypes (dated 2026-07-22,
i.e. weeks before this investigation's review19-24 sessions) already
diagnosed and worked through the exact symptom this investigation treated as
new:**

- `muldiv_u16_scaled` and five sibling functions (`sat_add_s16`,
  `sat_sub_u16_clamp0`, `muldiv_s16_rounded_3op`, `div_u32_u16_rounded`,
  `select_min_xy_triple`) genuinely return a 32-bit value across the R0:R1
  register pair, which the original 2-byte single-R0-pentry
  `__stdcall_far_allstack` couldn't express.
- Three different fixes were tried for this (join+solo-pentry register
  join; a real overlapping R0R1 register in `h8539f.slaspec`; falling back
  to a plain 2-byte R0 and accepting a "dropped R1 half" warning). **All
  three were confirmed, via controlled A/B diffing of full pre/post
  decompile dumps across the whole ROM, to trigger `"Unable to track
  spacebase fully for stack"` / `"Restarted to delay deadcode elimination
  for space: register"` on 18+ unrelated functions** (line 204-211) —
  i.e. this warning pair, which review22.md's FIFTH UPDATE treated as a
  fresh, specific clue about muldiv_u16, is a KNOWN, ALREADY-DOCUMENTED,
  currently-accepted side effect of this convention's register-pair/
  killedbycall shape, not a new discovery.
- The final, currently-deployed choice (plain 2-byte R0 output pentry,
  accepting the warning on just the affected math helpers) is a deliberate,
  documented trade-off, not an oversight.

**More importantly, the deployed `__stdcall_far_allstack`
(non-`_32ret`) prototype's `<killedbycall>` block explicitly lists R0, R1,
AND R2:**

```xml
<prototype name="__stdcall_far_allstack" extrapop="4" stackshift="2">
  ...
  <killedbycall>
    <register name="R0"/>
    <register name="R1"/>
    <register name="R2"/>
  </killedbycall>
  <unaffected>
    <register name="R3"/>
    <register name="R4"/>
    <register name="R5"/>
    <register name="FP"/>
    <register name="SP"/>
    <register name="SP24"/>
  </unaffected>
</prototype>
```

**This is a far more concrete, mechanistic candidate for muldiv_u16's
spurious R1 INDIRECTs than the vague "LINK FP,#0 might confuse spacebase
tracking" framing this document previously carried forward from review22.**
`muldiv_u16` uses this exact prototype (confirmed: its decompile signature
is `__stdcall_far_allstack muldiv_u16(...)`, the non-`_32ret` variant) and
declaring R1 `killedbycall` is precisely the kind of declaration that would
cause Ghidra's SSA construction to conservatively insert `INDIRECT` markers
for R1 — not because of a spacebase-tracking failure, but because the
*convention itself declares R1 to be clobbered by any call*, real or
otherwise. Whether muldiv_u16's specific spurious INDIRECTs are actually
caused by this `killedbycall` declaration (as opposed to some other part of
the convention, or the spacebase warning being an unrelated but co-occurring
symptom) has NOT yet been verified — this is a stronger hypothesis than the
one it replaces, not a confirmed fix.

**Revised next step for muldiv_u16 (supersedes the spacebase/.cspec-file-
location step above, since the file has now been found and read):**

1. Check whether `muldiv_u16`'s function body contains any real or apparent
   call site (it doesn't, per review22's own disassembly finding — "there is
   no CALL/JSR instruction anywhere in this function"). If genuinely no call
   exists, `killedbycall` should not fire for it at all under normal
   circumstances — determine what actually triggers the INDIRECT insertion
   in a function with no calls despite `killedbycall` being declared for its
   convention. This may point at generic decompiler logic (e.g. a
   convention being applied more broadly than at call sites — for instance
   to the function's own entry/exit as a defensive measure) rather than
   anything H8-specific.
2. Try temporarily removing R1 from `<killedbycall>` for
   `__stdcall_far_allstack` (NOT `_32ret`, since that one only affects the
   6 documented math helpers, not muldiv_u16 — confirm muldiv_u16 doesn't
   use `_32ret` first) in a scratch/experimental copy of `h8539f.cspec`,
   rebuild, and check whether muldiv_u16's spurious INDIRECTs and `uVar4 = b`
   symptom disappear. This is a fast, cheap, directly falsifiable test of
   the new hypothesis, unlike the previous "read the spec and see if
   anything looks wrong" framing.
3. If removing R1 from `killedbycall` fixes muldiv_u16 but reintroduces a
   different symptom elsewhere (per the 2026-07-22 comment's own warning
   that this convention has already been tuned against exactly this kind
   of regression), that in itself would be useful evidence about how tight
   the trade-off space here really is.
4. Still outstanding from every prior review across this entire
   investigation (review19 through review24, review-9561, and this
   correction): the `sat_add_u16` and sibling-function regression check
   has never been done. Given this update, that check should now also
   specifically include the 6 functions named in the 2026-07-22 `.cspec`
   comment (`sat_add_s16`, `sat_sub_u16_clamp0`, `muldiv_u16_scaled`,
   `muldiv_s16_rounded_3op`, `div_u32_u16_rounded`, `select_min_xy_triple`),
   since those are the functions the convention's own author already knows
   are affected by this trade-off.

## Finding 4 (2026-09-15): killedbycall hypothesis FALSIFIED for muldiv_u16 by direct experiment

Ran the exact experiment proposed in the "Revised next step for muldiv_u16"
section above. Added a scratch prototype `__stdcall_far_allstack_noR1kill`
to `h8539f.cspec` — byte-identical to `__stdcall_far_allstack` except R1 is
removed from `<killedbycall>` and added to `<unaffected>` instead — applied
it to `muldiv_u16` only via `set_function_prototype`, and force-redecompiled.

**Result: no change whatsoever.** Same `uVar4 = b; ... return uVar4;` bug,
same three warnings (`Unable to track spacebase fully for stack`, and both
`Restarted to delay deadcode elimination` variants), byte-for-byte identical
output apart from the cosmetic calling-convention name in the signature.

This directly falsifies the `killedbycall`-driven-INDIRECT hypothesis from
the "MAJOR CORRECTION" section above. Removing R1 from `killedbycall`
entirely did not touch the bug. **The spurious R1 substitution in muldiv_u16
is not caused by this convention's `killedbycall` declaration.**

This also means:
- The `.cspec`-level explanation (whether via the old spacebase-vagueness
  framing or the newer, more specific `killedbycall` framing) is now
  twice-falsified for muldiv_u16 specifically: `isDead()` was ruled out
  by direct log evidence (Finding 1), and now `killedbycall` is ruled out
  by direct experiment (Finding 4). Both of this document's two leading
  hypotheses for muldiv_u16 have failed concrete tests.
- `muldiv_u16_scaled`'s bug remains separately, positively explained by the
  `isDead()`-fallback mechanism (Finding 3) — that finding is untouched by
  this result, since it was reached by direct log evidence, not by analogy
  to muldiv_u16.
- muldiv_u16's actual cause is still unknown. It is not `isDead()`-fallback
  ordering, and it is not `killedbycall`-driven INDIRECT insertion. The
  warnings it shares with muldiv_u16_scaled and the six documented sibling
  functions may still point at a real common upstream issue in P-code
  construction for FP-relative stack access — but the specific mechanism
  (killedbycall) proposed in the "MAJOR CORRECTION" section is now known
  NOT to be it, and that section's confidence in the killedbycall theory
  should be read as superseded by this negative result.

**Next step for muldiv_u16 (supersedes item 2 in the "Revised next step"
list above, which is now completed and negative):** Since neither
`isDead()`-fallback nor `killedbycall` explains it, get the actual P-code
for muldiv_u16 (`get_function_pcode`) and trace, ins-by-ins, where the
Varnode that ends up printed as `uVar4` (register:02, i.e. R1) actually
comes from and where the INDIRECT(s) touching it are inserted, rather than
continuing to test named hypotheses about *why* an INDIRECT might appear.
Direct tracing, not further hypothesis-guessing, is now the appropriate
next move — two plausible, well-reasoned hypotheses have both failed
concrete tests, which is a signal to stop guessing and start reading the
raw P-code for this specific function.

Housekeeping: the `__stdcall_far_allstack_noR1kill` scratch prototype is
still applied to muldiv_u16 and still present in `h8539f.cspec` as of this
writing. Since the experiment is complete and negative, per the prototype's
own "EXPERIMENTAL" comment it should be removed from the .cspec and
muldiv_u16 reset to `__stdcall_far_allstack` — not yet done as of this
update.

## Finding 5 (2026-09-15): root cause identified via raw P-code — write-write collision on register:2 (R1) at 0x141c2

Per Finding 4's own recommendation, pulled raw P-code for muldiv_u16
(`get_function_pcode`, address `0x141ba`) instead of testing further named
hypotheses. This gives a direct, mechanical answer.

At P-code address `0x141c2`, TWO separate operations both write output
`register:2` (R1 — the register that ends up printed as `uVar4`):

**Write A (the real, correct one):** the actual multiply `a*b` is computed
in full 32-bit precision (`INT_ZEXT` both operands to 4 bytes, `INT_MULT`),
then the low 16 bits are extracted (`SUBPIECE ... → unique:a2702`) and
copied into `register:2`:
```
SUBPIECE(unique:a2700, 0) → unique:a2702   [low 16 bits of 32-bit a*b]
COPY(unique:a2702) → register:2            [merge_group=0]
```

**Write B (the spurious one):** a raw COPY of the `b` parameter's OWN stack
slot (`stack:6`, i.e. the same storage `b` itself lives in) is made, then
fed through an `INDIRECT` (cause constant `0x24` = 36 decimal, presumably
referencing some other PcodeOp's SeqNum/order as the "cause") that ALSO
writes `register:2`:
```
COPY(stack:6) → unique:10000014            [merge_group=1]
INDIRECT(unique:10000014, 0x24) → register:2   [merge_group=1]
```

Both writes target the same Varnode (`register:2`, i.e. R1) at the exact
same P-code address (`0x141c2`). This is a genuine write-write collision,
not a subtle ordering/merge heuristic issue — two real, distinct pcode ops
assign the same output location at the same instruction. Whichever one the
decompiler's merge/Cover logic picks as "the" value of R1 going forward
determines whether the eventual decompile shows the correct multiply result
or the spurious stack-6 copy. Currently it picks Write B (confirmed by the
decompile output: `uVar4 = b` reflects exactly the stack:6-sourced INDIRECT
chain, not the SUBPIECE/COPY multiply-result chain).

This explains why both previously-tested hypotheses failed: **`isDead()` and
`killedbycall` are both about how an INDIRECT's ordering/position is decided
once it exists — neither explains why a competing INDIRECT writing R1 from
`stack:6` was created in the first place.** That's a prior, separate
question: something during P-code generation for this function inserts an
`INDIRECT` that treats `b`'s own stack storage as also flowing into
`register:2`, seemingly modeling some indirect effect that doesn't actually
occur in this function (there is no call, and no real instruction copies
`b`'s stack slot into R1 at this point per the disassembly reviewed
earlier in this investigation).

**Next step:** identify what PcodeOp corresponds to `INDIRECT`'s cause
constant `0x24` (36 decimal) — this is very likely encoding a reference
(iop / affecting-op SeqNum-order) to whatever earlier operation is
responsible for inserting this INDIRECT, and finding that op is the direct
route to the actual root cause (most plausibly: something related to the
`LINK FP,#0x0` / spacebase setup at `0x141ba`-`0x141c2`, i.e. the two
`PTRSUB`/`PTRADD` ops seen at the very start of this same P-code dump,
which touch `register:e` — worth checking whether `0xe` and `0x2` are
related in the register file, or whether the INDIRECT's real trigger is
elsewhere). This is now a precise, mechanical question rather than a
hypothesis about calling-convention behavior, and should be pursued in
`heritage.cc`/wherever `CPUI_INDIRECT` ops get their cause/iop metadata
assigned, not in `cover.cc`/`merge.cc` (which only consume this metadata,
per Findings 1 and 4).

## Temporary diagnostics currently in the tree (all need removal before final submission)

These are all gated behind `#ifdef _WINDOWS` and write to files under
`C:\Users\j.brophy.CORKILLSYSTEMS\`. None are needed for the `isDead()` fix
itself (that fix has no diagnostic code in the final, upstream-ready version —
see `git diff Ghidra/Features/Decompiler/src/decompile/cpp/merge.cc` for a check).
Diagnostics currently added ON TOP of that clean fix, for continuing this
specific investigation:

- `cover.cc`: `Cover::addRefRecurse` (logs `setAll()` block-fill events) and
  `Cover::addRefPoint` (logs ref/def op details for the two known offending
  Varnodes, offsets `0x1000001b`/`0x1000001d`).
- `funcdata_op.cc`: `Funcdata::opInsertAfter` (logs resolved `prev` for
  `op_pc=0x14203`).
- `merge.cc`: `Merge::mergeOp`'s failing-merge branch (logs both Varnodes,
  sizes, spaces, and printed Cover ranges).

All write to distinct files (`cover_diag.txt`, `opinsert_diag.txt`,
`mergefail_diag.txt`) for easy correlation. Clear each with
`Remove-Item`/`Clear-Content` before a fresh triggering run, since they append.

**Tooling note:** `str_replace`-style single-string-match edits proved unreliable
on `cover.cc` specifically this session (silently failed to apply a change while
reporting success, and separately spliced code into the middle of a doc comment).
The reliable alternative that worked every time: read the file as a PowerShell
line array (`Get-Content` returns one string per line), find the insertion point
via `[System.Collections.Generic.List[string]].IndexOf()` on an exact-match
marker string, verify the following line matches what's expected as a sanity
check, then `InsertRange()` and `Set-Content`. No regex, no escaping tricks.

## Finding 6 (2026-09-15): muldiv_u16_scaled's parameters are bound one stack word too early — likely a SEPARATE bug from Findings 1-5

Prompted by a question about whether `__stdcall_far_allstack_32ret` is even
the correct prototype shape for a 4-parameter function. It is generic
enough to handle any parameter count (single unbounded all-stack `<pentry>`,
offset 4 to 512 bytes — Ghidra carves individual params out of this blob
per the function's own declared signature, not per any count baked into
the `.cspec`), so parameter *count* isn't the issue. But checking the
actual bound offsets against the real disassembly finds a real, distinct
bug:

`get_function_variables` on `muldiv_u16_scaled` (`0x141dc`) shows:
```
a: Stack[0x4]:2
b: Stack[0x6]:2
c: Stack[0x8]:2
d: Stack[0xa]:2
```

But the disassembly (already captured earlier in this document, "Finding
2" section) shows the real instructions reading:
```
000141e2: mov:f.w @(0x6:8,FP),R0    ; real first param at FP+6, not FP+4
000141e4: mov:f.w @(0x8:8,FP),R2    ; real second param at FP+8, not FP+6
...
000141f7: cmp:g.w @(0xc:8,FP),R0    ; a FOURTH real stack access at FP+0xc,
000141fe: divxu.w @(0xc:8,FP) ,R0   ; with no corresponding pentry at all
00014203: divxu.w @(0xc:8,FP) ,R2   ; (Ghidra's model stops at 0xa)
```

**Every parameter Ghidra has bound is one stack word (2 bytes) earlier than
where the function actually reads it, and there is a real fourth access at
FP+0xc that Ghidra's 4-parameter model doesn't cover at all** (it would
need a 5th `ushort` parameter, or the existing `d` needs to move to 0xc
and a real value needs to occupy 0xa — not yet determined which).

This is consistent with `muldiv_u16_scaled`'s prologue pushing two EXTRA
registers before its stack-parameter region starts, which `muldiv_u16`
(the non-`_scaled` sibling, whose bindings ARE correct — see disassembly
in the main investigation, FP+6/+8/+0xa/+0xc was not its shape, it takes
3 params not 4) does not do:
```
000141dc: link FP,#0x0:8
000141de: mov:g.w R3,@-SP     ; extra push #1 (2 bytes)
000141e0: mov:g.w R2,@-SP     ; extra push #2 (2 bytes)
000141e2: mov:f.w @(0x6:8,FP),R0   ; first real param read starts AFTER these
```

`__stdcall_far_allstack_32ret`'s `<pentry>` hardcodes `offset="4"` as the
start of the all-stack region, which is correct for a plain `LINK FP,#0x0`
frame with no extra saves, but wrong for `muldiv_u16_scaled` specifically,
whose frame has 4 extra bytes of register-save before the real parameter
region begins at FP+6.

**This is very likely a genuine, independent contributor to
`muldiv_u16_scaled`'s wrong output, separate from Finding 3's `isDead()`
fallback-ordering issue.** With every parameter read from the wrong slot,
`a`, `b`, `c`, `d` as Ghidra presents them are NOT the real argument values
— `a` (Stack[0x4]) is actually whatever the caller placed one word before
the true first argument (quite possibly the low or high half of a value,
or garbage, depending on what the caller's stack layout puts there), and
the function's real 4th argument (at FP+0xc) has no parameter binding at
all and shows up as raw stack dereferences (`*(ushort *)(puVar7 + 0xc)`)
instead of a named parameter.

**This means `muldiv_u16_scaled`'s wrong-output diagnosis in Findings 2-3
is incomplete.** Finding 3 correctly identified a real `isDead()`-fallback
ordering bug affecting one INDIRECT, but this offset bug means the
function's parameters were ALSO mismodeled the whole time, independent of
that ordering issue. Fixing only the `isDead()` fallback (per the existing
"Next step, muldiv_u16_scaled specifically" recommendation) would not by
itself produce fully correct output — the parameter-offset issue would
need fixing too, either by giving `muldiv_u16_scaled` its own prototype
with the correct base offset (6, not 4) and a 5th pentry for the FP+0xc
value, or by using `set_variable_storage` to override just this function's
parameter bindings without touching the shared `.cspec` prototype (safer,
since — per the 2026-07-22 comment history and Finding 4's own experiment
tonight — shared-prototype edits in this `.cspec` have repeatedly caused
wide, hard-to-predict regressions across unrelated functions).

**Not yet checked:** whether this same offset issue affects any of the
other 5 sibling functions sharing `__stdcall_far_allstack_32ret`
(`sat_add_s16`, `sat_sub_u16_clamp0`, `muldiv_s16_rounded_3op`,
`div_u32_u16_rounded`, `select_min_xy_triple`) — if their prologues also
push extra registers before their real parameter region (as
`muldiv_u16_scaled`'s does), they likely have the identical offset bug.
This should be checked before deciding whether to fix this via a shared
prototype change or per-function `set_variable_storage` overrides.

## Finding 7 (2026-09-15): Finding 6's mechanism was WRONG — this is a systematic offset-4-vs-6 bug affecting ALL SIX documented functions, unrelated to extra register pushes

Checked all 5 remaining functions named in the 2026-07-22 `.cspec` comment
against their real disassembly. Result, tabulated (FP offset of first real
parameter read vs. extra register pushes in the prologue before that read):

| Function | Extra prologue pushes | Real first param at | Ghidra binds first param at |
|---|---|---|---|
| `sat_add_s16` (0x1401f) | none | FP+6 | Stack[0x4] |
| `sat_sub_u16_clamp0` (0x14045) | none | FP+6 | (not re-checked, same shape as sat_add_s16) |
| `muldiv_u16_scaled` (0x141dc) | R3, R2 (4 bytes) | FP+6 | Stack[0x4] |
| `muldiv_s16_rounded_3op` (0x14247) | R3, R2 (4 bytes) | FP+6 | (uses `_32ret`, same base offset issue expected) |
| `div_u32_u16_rounded` (0x1417d) | R4, R3, R2 (6 bytes) | FP+6 | (uses `_32ret`, same base offset issue expected) |
| `select_min_xy_triple` (0x14471) | R2,R3,R4,R5 via `stm` (8 bytes) | FP+6 | (uses `_32ret`, same base offset issue expected) |

**The extra-pushes theory in Finding 6 is WRONG.** `sat_add_s16` has ZERO
extra prologue pushes and its real first parameter is still at FP+6, not
FP+4 — identical to `muldiv_u16_scaled`, which has two extra pushes. The
number of extra pushes has no bearing on where the real parameters land;
every one of these six functions reads its first real stack parameter at
FP+6, unconditionally. This makes sense in retrospect: on this ABI, extra
`mov Rn,@-SP` pushes happen AFTER `LINK FP,#0x0` and therefore adjust SP,
not FP-relative offsets — FP is fixed at function entry regardless of
what's pushed afterward. Finding 6's causal story (extra pushes shifting
parameter offsets) was incorrect; the correction is simpler and worse in
scope than Finding 6 suggested.

**The real, simple, systematic bug: `__stdcall_far_allstack` AND
`__stdcall_far_allstack_32ret` both declare their all-stack `<pentry>`
starting at `offset="4"`, but on this hardware/ABI, with a plain
`LINK FP,#0x0` frame (extrapop=4, stackshift=2), the real first stack
parameter is consistently at FP+6, not FP+4.** This affects EVERY function
using either prototype — which per the 2026-07-22 `.cspec` comment is
`muldiv_u16_scaled` and the 5 named siblings, i.e. this is not a
`muldiv_u16_scaled`-specific bug at all, it's a defect in the shared
prototype declaration itself, silently mis-binding parameters for 6+
documented functions (likely more — `__stdcall_far_allstack`, the
non-`_32ret` variant used by e.g. `muldiv_u16` per Finding 5/6's own
earlier checks, was NOT found to have this problem — `muldiv_u16` itself
has no declared parameters bound via a stack pentry at all in the P-code
dump reviewed in Finding 5, it reads `stack:4` and `stack:6` directly,
matching FP+4/FP+6 relative to ITS OWN link+0 frame with no extra pushes —
so this offset-4-vs-6 issue may be specific to how `_32ret`'s consumers
happen to be typed/parameter-bound versus how `__stdcall_far_allstack`'s
consumers are, not automatically also broken. This distinction is NOT yet
fully resolved and needs direct checking, not assumption, before drawing
a final conclusion about scope).

**This finding needs consolidation with Finding 6 before any fix is
attempted** — Finding 6's specific claim (extra pushes → offset shift) is
superseded by this table, but Finding 6's core observation (parameters are
bound at the wrong offset, one word too early, for `muldiv_u16_scaled`)
remains correct, just for a different and simpler reason than originally
stated. The fix candidate list in Finding 6 (per-function
`set_variable_storage` override vs. shared prototype base-offset change) is
still the right shape of fix, but given how many functions are now known
to share this exact pentry base, the shared-prototype fix (changing
`offset="4"` to `offset="6"` in both `__stdcall_far_allstack` and
`__stdcall_far_allstack_32ret`'s `<input><pentry>`) may now be the more
appropriate one, IF it doesn't reintroduce the kind of wide unrelated
regression the 2026-07-22 comment and this session's own Finding 4
experiment both warned shared-prototype edits in this `.cspec` tend to
cause. Any such change needs the same full-ROM before/after diff discipline
used in the 2026-07-22 history before being trusted.

**Immediate next step:** confirm whether `__stdcall_far_allstack` (non-
`_32ret`) has the same offset-4-vs-6 mismatch for any function that
actually uses it with declared stack parameters (muldiv_u16 itself doesn't
exercise this cleanly, since its P-code reads `stack:4`/`stack:6` directly
rather than through named/typed parameters in the dump reviewed) — this
determines whether the fix belongs on `_32ret` alone or on the base
`__stdcall_far_allstack` too.

## Finding 8 (2026-09-15): Finding 7's table was built on an unverified assumption — the .cspec prototype is NOT the cause; something function-specific is

Checked `__stdcall_far_allstack` directly by finding a function actually
assigned it in the live database: `search_functions_enhanced` with
`calling_convention=__stdcall_far_allstack` returns exactly one hit,
`FUN_0001400f` (0x1400f) — an unnamed, unanalyzed-name function, NOT one of
the 6 functions named in the 2026-07-22 `.cspec` comment. Its disassembly:

```
0001400f: link FP,#0x0:8
00014011: clr.w R0
00014013: mov:f.w @(0x6:8,FP),R1
00014015: add:g.w @(0x8:8,FP),R1
00014018: addx.w 0x0,R0
0001401c: unlk FP
0001401d: prts
```

No extra prologue pushes, real params at FP+6/FP+8 — same shape as every
other function checked. But `get_function_variables` on this SAME function
shows it bound CORRECTLY:
```
param_fp1: Stack[0x6]:2
param_fp2: Stack[0x8]:2
```

**This directly contradicts Finding 7's conclusion.** The exact same
`offset="4"` pentry, on a function with the exact same real-parameter shape
(FP+6 start, no extra pushes) as `sat_add_s16` and `muldiv_u16_scaled`,
produces a CORRECT binding here, but an INCORRECT one for
`muldiv_u16_scaled`. Since the `.cspec` declaration is identical in both
cases (confirmed by direct re-read of both `<prototype>` blocks — see
Finding 7's table, both are `offset="4"`, `align="2"`, `stackshift="2"`,
`extrapop="4"`), **the `.cspec` prototype itself cannot be the cause of the
offset mismatch.** Finding 7's core conclusion ("systematic offset-4-vs-6
bug in the shared prototype declaration") is WRONG and should not be acted
on — do not edit `offset="4"` to `offset="6"` in either prototype based on
Finding 7; Finding 8 falsifies the premise that led to that suggested fix.

Also discovered while checking this: `search_functions_enhanced` with
`calling_convention=__stdcall_far_allstack_32ret` likewise returns only ONE
hit — the same `FUN_0001400f`, not `muldiv_u16_scaled` or any of the named
siblings — despite `muldiv_u16_scaled`'s own decompile signature printing
`__stdcall_far_allstack_32ret muldiv_u16_scaled(...)` earlier this session
(see Finding 2). This is either a bug/staleness in
`search_functions_enhanced`'s convention filter (consistent with several
other tool-caching oddities noted elsewhere in this document), or evidence
that `muldiv_u16_scaled`'s prototype assignment is itself in some unusual
or overridden state not reflected by a normal convention lookup. This
was NOT resolved before the session ended — Finding 7's whole premise
(which of the 6 named sibling functions use which convention) was never
independently verified against the live database and should not be trusted
without redoing this check properly.

**Corrected picture:** `muldiv_u16_scaled`'s wrong parameter binding
(Finding 6) is real and confirmed directly via its own
`get_function_variables` output. But its CAUSE is not a shared `.cspec`
defect (Finding 8 rules that out) — it's something specific to this
function's own analysis state: possibly a leftover custom
`set_variable_storage`/prototype override from an earlier session (the
`.cspec` history mentions several abandoned experiments — join+solo-pentry,
real R0R1 register, etc. — any of which could have left a per-function
override in the database that was never cleared), possibly something
related to the same INDIRECT/spacebase issue from Findings 1-5 confusing
the parameter-recovery pass specifically for this function, or possibly
something else not yet identified. Finding 6's raw observation (wrong
offsets, confirmed via `get_function_variables`) stands; Finding 7's
explanation for it does not; the real explanation is still open.

**Next step:** check whether `muldiv_u16_scaled` has a per-function
prototype override (as opposed to inheriting cleanly from the `_32ret`
model) — this is directly checkable and would explain a function-specific
binding wrong at a function-shared convention is right. Do NOT attempt to
"fix" this via a `.cspec` edit until that's resolved; Finding 8 already
shows a `.cspec`-level fix would be aimed at the wrong target.

## Finding 9 (2026-09-15): DATASHEET-CONFIRMED root cause of the FP+6 offset — extrapop is wrong for far calls, not a decompiler bug at all

Per direct instruction, checked the project's own datasheets
(`datasheets/grep able datasheets/`) rather than continuing to guess or
experiment blind. This resolves Finding 8's open question definitively and
overturns the entire offset-4-vs-6 investigation (Findings 6-8): **this was
never a decompiler defect. It's a straightforward, datasheet-confirmable
`extrapop`/pentry-offset error in the `.cspec`, correct to fix at the
`.cspec` level after all — just via a different attribute than Finding 7
guessed.**

From `H8_500_programming.md` (base H8/500 ISA reference): `LINK FP,#xx`
breaks down as `MOV.W FP,@–SP` / `MOV.W SP,FP` / `ADDS.W #xx,SP` — i.e.
`LINK FP,#0` pushes exactly ONE word (the old FP) before establishing the
new frame. For a NEAR call (`JSR`/`RTS`), the call itself pushes only PC (1
word), so after `LINK FP,#0`: `FP+0`=old FP, `FP+2`=return PC, `FP+4`=first
real argument. This matches `extrapop="4"` — which is why `FUN_0001400f`
(Finding 8), confirmed further down to be a NEAR-called function, binds
correctly with the existing `offset="4"` pentry.

From `H8_539F_hardware.md` (H8/539F-specific, maximum-mode/far-call
addendum): **`PJSR` (page jump to subroutine — a FAR call) operation is
explicitly documented as `PC → @–SP, CP → @–SP, <EA> → PC`** — i.e. a far
call pushes TWO words (PC, then CP), not one, before the callee's own
`LINK FP,#0` adds the third (old FP). So for any FAR-called function:
`FP+0`=old FP, `FP+2`=CP, `FP+4`=PC, **`FP+6`=first real argument** — not
FP+4. `PRTS`'s operation (`@SP+→CP, @SP+→PC`, popping CP before PC,
consistent with LIFO order matching PJSR's push order) confirms the same
pairing from the return side.

**This is exactly, precisely the FP+6 offset seen in EVERY function checked
in Findings 6-8** (`sat_add_s16`, `sat_sub_u16_clamp0`, `muldiv_u16_scaled`,
`muldiv_s16_rounded_3op`, `div_u32_u16_rounded`, `select_min_xy_triple`) —
all of them return via `prts`/use far-call semantics, all of them have
their real first parameter at FP+6, and this is now explained by the
datasheet's own documented `PJSR`/`PRTS` operation, not by any decompiler
defect, ordering bug, or `.cspec` authoring mistake in the pentry `offset`
attribute alone.

**The actual, datasheet-justified fix: `extrapop` is the wrong value for
these far-call prototypes.** `__stdcall_far_allstack`,
`__stdcall_far_allstack_32ret`, `__stdcall_far`, and `__stdcall_far_2arg`
(all currently `extrapop="4"`, all named as "far" conventions, all backing
functions that return via `prts`/`pjsr`-style far calls) should have
`extrapop="6"` to correctly account for the extra CP word a far call pushes
beyond a near call's plain PC. The `<pentry>` `offset="4"` inside each of
these `<input>` blocks should correspondingly become `offset="6"`. This
is now a concrete, datasheet-justified, mechanically explainable fix
candidate — a genuine correction to the SLEIGH/cspec model matching real
hardware behavior, not another patch-over-symptom.

**Why Finding 8's `FUN_0001400f` test didn't catch this:** `FUN_0001400f`
uses the SAME `.cspec` prototype names as the broken functions, but that
doesn't mean it's actually reached via a far/`pjsr` call at its own entry
— the `.cspec` prototype models a calling convention's *stack layout*
assuming a particular call mechanism, but whether a given function is
ACTUALLY invoked via `jsr` (near) or `pjsr` (far) depends on how ITS
CALLERS reach it, which isn't verified by checking the callee's own
prologue alone (both near- and far-called functions can look identical
internally — `LINK FP,#0`, then read `@(0x6:8,FP)`, etc. — the only
difference is what's already on the stack from the call itself, which the
callee's own code never re-derives). **This needs verification**: check
`FUN_0001400f`'s actual call sites (xrefs) to see whether it's reached via
`jsr`/`bsr` (near, 1-word return address) or `pjsr` (far, 2-word CP+PC) —
if near, that fully explains why Finding 8's test function bound correctly
under `offset="4"` despite using the SAME prototype name as far-called,
FP+6-offset functions like `muldiv_u16_scaled`. If this is confirmed, it
would mean the six documented math-helper functions and `FUN_0001400f`
should NOT share the same prototype at all — the near-called ones need
`extrapop="4"`, the far-called ones need `extrapop="6"`, and the current
`.cspec` conflates the two under names that say "far" but are declared
with near-call arithmetic.

## Finding 11 (2026-09-15): Finding 9/10's fix APPLIED and CONFIRMED CORRECT for the parameter-offset half of muldiv_u16_scaled's bug

Added `__stdcall_far_allstack_32ret_farcall` (extrapop=6, pentry offset=6)
as a new, separate prototype in `h8539f.cspec` per Finding 9/10's
recommendation — did not edit `__stdcall_far_allstack_32ret` in place.
Applied it to `muldiv_u16_scaled` via `set_function_prototype`, then
force-redecompiled.

**Result: parameter binding is now correct.** `get_function_variables`
confirms:
```
a: Stack[0x6]:2
b: Stack[0x8]:2
c: Stack[0xa]:2
d: Stack[0xc]:2
```
matching the real disassembly exactly (`@(0x6:8,FP)`, `@(0x8:8,FP)`,
`@(0xa:8,FP)`, `@(0xc:8,FP)`). The previously-unbound 4th stack access at
FP+0xc — shown before this fix as a raw `*(ushort *)(puVar7 + 0xc)`
dereference with no parameter identity — is now the properly named
parameter `d`. This is a direct, confirmed, working fix for the
parameter-offset defect described in Findings 6-10.

**The decompile is still not fully correct — but for the OTHER, already-
known, separate reason (Finding 3), not because this fix failed:**
```c
ushort __stdcall_far_allstack_32ret_farcall muldiv_u16_scaled(ushort a,ushort b,ushort c,ushort d)
{
  ...
  ushort in_stack_00000004;
  ...
  sVar1 = (short)((uint)in_stack_00000004 * (uint)b >> 0x10);
  uVar4 = in_R1;
  uVar6 = a;
  ...
  uVar3 = (short)((uint)in_stack_00000004 * (uint)b);
  ...
  uVar5 = uVar6 + uVar4;
  uVar3 = (short)(CONCAT22(uVar2,uVar6 + uVar4) / (uint)*(ushort *)(puVar7 + 0xc));
  return uVar5;
}
```
`in_stack_00000004` (reading the now-unused Stack[0x4] slot, which per
Finding 9 is really the CP word pushed by the calling `pjsr` — not a real
argument at all) and `in_R1` are still present, and the function still
returns `uVar5` (a pre-division sum) rather than the real division result
in `uVar3`. This is Finding 3's `isDead()`-fallback ordering defect,
untouched by tonight's fix — it was never in scope for this specific fix,
and its presence here is expected, not a regression or a sign this fix is
wrong.

**Status: the parameter-offset defect (Findings 6, 9, 10) is FIXED and
CONFIRMED for `muldiv_u16_scaled`. The isDead()-fallback ordering defect
(Finding 3) remains OPEN and unfixed for the same function.
`muldiv_u16`'s separate write-write-collision defect (Finding 5) remains
OPEN and unfixed, and is unrelated to this fix (muldiv_u16 uses the plain
`__stdcall_far_allstack`, 3 parameters, and per Finding 5's own P-code dump
was already reading `stack:4`/`stack:6` directly rather than through named
parameters — worth re-checking whether muldiv_u16 ALSO needs the
farcall/extrapop=6 treatment, since it's presumably also pjsr-called; not
yet checked.**

**Not yet done:**
- The other 5 functions named in the 2026-07-22 `.cspec` comment
  (`sat_add_s16`, `sat_sub_u16_clamp0`, `muldiv_s16_rounded_3op`,
  `div_u32_u16_rounded`, `select_min_xy_triple`) have not had this same fix
  applied yet, despite Finding 7's disassembly check showing they have the
  identical FP+6 real-parameter-start shape. Each should be verified
  far-called (check for a real `pjsr` targeting it, as done for
  `muldiv_u16_scaled`) before reassigning, per Finding 9's own caution
  against assuming by name/shape alone.
- Whether `muldiv_u16` itself (plain `__stdcall_far_allstack`, not
  `_32ret`) needs an equivalent `__stdcall_far_allstack_farcall`
  (extrapop=6) variant has not been checked — its own P-code (Finding 5)
  reads raw `stack:4`/`stack:6` rather than going through named parameters,
  so this fix's effect on it, if any, is unverified.
- The full-ROM before/after diff discipline the 2026-07-22 history
  recommends for any shared-prototype-adjacent change has NOT been run for
  this new prototype. Given it's an ADDITIVE new prototype rather than an
  in-place edit, and applied to only one function so far, the blast radius
  is much smaller than past incidents in this file's history, but this
  should still be verified before considering the fix final.
- The scratch `__stdcall_far_allstack_noR1kill` prototype from Finding 4
  is still applied to `muldiv_u16` and still present in the `.cspec` —
  still not reverted/cleaned up as of this writing.

## Finding 10 (2026-09-15): confirmed via real xrefs — these ARE genuinely far-called (pjsr); Finding 8's "working" counterexample was never actually a counterexample

Per the concern that these functions might simply be forced into the wrong
convention family rather than needing a numeric tweak to a shared one,
checked real call sites directly instead of reasoning from `.cspec` text.

**`muldiv_u16_scaled` (0x141dc) is confirmed genuinely far-called.**
`get_function_xrefs` lists 13 call sites; disassembling one directly
(`f2ea_f2ee_composite_correction_calc` at 0x24884) shows an unambiguous,
statically-resolved instruction:
```
000248e1: pjsr @0x141dc:24
```
This is real `pjsr` (far call, 24-bit target), not an indirect/computed
call — `[COMPUTED_CALL]` in the xref listing was Ghidra's classification
for a `pjsr @absolute:24` target, not evidence of an indirect call. This
directly confirms Finding 9's mechanism: this function is reached via
`pjsr`, so its real stack layout after `LINK FP,#0` genuinely includes the
extra CP word `pjsr` pushes, landing real parameters at FP+6 as observed
all along.

**Finding 8's `FUN_0001400f` counterexample is corrected: it was never
evidence that `offset="4"` works.** `FUN_0001400f` has ZERO callers
(`get_function_xrefs` returns no references at all — it may be dead code,
an unreferenced library remnant, or a mis-identified function boundary).
Its own disassembly ends in `prts` (far return) and reads its first real
parameter at FP+6, exactly like every far-called math helper — Finding 8
mischaracterized this as "the same offset=4 pentry produces a correct
FP+6 binding here," but FP+6 was never what `offset="4"` should produce;
FP+6 is only correct under `offset="6"` (Finding 9). Since nothing calls
`FUN_0001400f`, there is no real call-site evidence to explain why its
auto-analyzed binding happened to land on the observed values — most
likely Ghidra's own parameter-recovery heuristics (independent of the
`.cspec` prototype's declared offset) inferred the binding empirically
from the function body's own stack accesses in this one case, which
would not generalize to functions where that heuristic fails (as it
apparently does for `muldiv_u16_scaled`). This was never a genuine
"working" case to compare against; Finding 8's conclusion (that the
`.cspec` prototype isn't the cause) is now superseded by Finding 9/10 —
the `.cspec` far-call prototypes' `extrapop`/`offset` genuinely are wrong,
Finding 8 just picked an uninformative comparison function to test it
against.

**This confirms the framing behind tonight's question directly: these
functions were not suffering from a numeric typo in a convention that
otherwise fit them — they were being bound under a convention
(`extrapop="4"`) that assumes NEAR-call stack semantics, while every real
call site reaches them via a FAR call (`pjsr`).** The fix is still a
`.cspec`-level change (not SLEIGH, not C++ decompiler source — see prior
answer on that), but it should be understood as "give far-called functions
a prototype whose numbers actually describe far-call stack semantics,"
not "patch a shared number." Per Finding 9's own recommendation, this
should land as a distinct, correctly-parameterized prototype rather than
an in-place edit to the existing `__stdcall_far_allstack*` names, both to
avoid the wide blast-radius regressions this `.cspec`'s history has
repeatedly shown from in-place shared-prototype edits, and because it's
now clear the existing names were themselves misapplied (claiming to be
"far" conventions while carrying near-call arithmetic) rather than merely
imprecise.

**2026-09-15 note: the table and read-before-define framing below is
SUPERSEDED by Finding 5 above and should not be trusted as the mechanism.**
This section predates Findings 1, 4, and 5 in this same document. Finding 1
already showed `isDead()` is FALSE for both of muldiv_u16's INDIRECTs (which
this section doesn't address either way). Finding 5, from directly reading
raw P-code via `get_function_pcode` rather than reasoning about SeqNum order
numbers, found a different and more concrete mechanism: a genuine
write-write collision, where two distinct ops (a correct SUBPIECE/COPY of
the real multiply result, and a spurious COPY/INDIRECT chain sourced from
`stack:6`) both write output `register:2` at the SAME P-code address
(`0x141c2`), not an INDIRECT-reads-before-its-input-is-defined ordering
problem between two different addresses/orders as described below. The two
framings may be compatible (the "order 143 reads before order 210 defines"
observation below could be a symptom of the same underlying dual-write, seen
from the SeqNum-order angle rather than the raw-P-code angle), but this has
NOT been verified, and the specific claim that `unique:10000014` "is not
actually defined until a LATER-ordered COPY" does not match what Finding 5's
raw P-code dump shows (there, `unique:10000014` is defined by a COPY from
`stack:6` at the SAME address, then immediately consumed by the INDIRECT
that also writes `register:2` — no later-order COPY redefines it in the
dump reviewed). Whoever continues this should re-derive this section's table
directly from `get_function_pcode` output (as Finding 5 did) rather than
trust the order-number claims below at face value. Kept below for historical
context only.

An independent, multi-session investigation into a DIFFERENT but sibling
function, `muldiv_u16` (not `_scaled`), converged on what appears to be
**the same underlying ordering defect described above**, arrived at from a
completely different angle (a wrong-looking `uVar4 = b; return uVar4;`
display bug, rather than a `LowlevelError` crash). Full chronological detail
is in `review22.md` in this same directory; this note records only the
cross-confirmation.

`muldiv_u16` has two `CPUI_INDIRECT` ops on register `R1` (`register:02`) at
P-code addresses `0x141c2` and `0x141ce`, each reading a `unique` temp that
is not actually defined until a LATER-ordered `CPUI_COPY`:

| INDIRECT (reads too early) | its input | actually defined by |
|---|---|---|
| `(0x141c2, order 143)` | `unique:10000014` | `COPY@(0x141c2, order 210)` |
| `(0x141ce, order 144)` | `unique:10000016` | `COPY@(0x141ce, order 211)` |

Same shape as this document's `0x00014203:ed` case: an `INDIRECT` at a
LOWER order number reading a Varnode whose real defining `COPY` has a
HIGHER order number. In `muldiv_u16`'s case this doesn't throw a
`LowlevelError` (the Cover-intersection apparently still finds a mergeable,
if wrong, result rather than failing outright) — instead it silently
produces a merged HighVariable whose data, traced all the way back, turns
out to be the wrong value (an untouched copy of an early parameter,
`b`, instead of the real computed result), which is what shows up as the
incorrect `uVar4 = b;` / `return uVar4;` text. This may mean the ordering
defect has (at least) two distinct failure modes depending on whether the
resulting Cover ends up unmergeable (crash-turned-`LowlevelError`, this
function's `_scaled` sibling) or mergeable-but-wrong (silent bad output,
`muldiv_u16`).

Also independently corroborates this document's suspicion (section "Why:
our own crash fix's fallback position is the direct cause" / next-step item
3) that the root trigger is spacebase/stack-pointer-tracking related: every
decompile of `muldiv_u16` in the other investigation thread carried the
warning `"Unable to track spacebase fully for stack"`, and the function
uses the same `LINK FP,#0x0` / FP-relative-parameter-access pattern as
`muldiv_u16_scaled`. This is consistent with, though doesn't by itself
prove, this document's hypothesis 3 (that 9561's `RuleLoadVarnode::
vnSpacebase` recursion change is the actual trigger, exposing a pre-existing
or newly-introduced ordering issue in how `CPUI_INDIRECT`/`CPUI_COPY` pairs
get created for stack/FP-relative accesses).

**Recommendation for whoever picks this up next:** treat this document
(review-9561) as the primary, authoritative one for the ordering defect
itself — it has the more precise mechanism and a concrete next-step list
already. Use `muldiv_u16` (this cross-confirmation) as a second, non-crashing
test case for validating any fix, since it reproduces the same root cause
without depending on the crash path at all — a fix here should make
`muldiv_u16`'s `INDIRECT` ops at 143/144 no longer read from a
not-yet-defined Varnode, and the decompiled output should stop showing
`uVar4 = b;`/`return uVar4;` and instead show the real division-result
expression.

## Finding 12 (2026-09-15): Finding 5's cause-constant `0x24` decodes to a
## `CPUI_STORE` with a genuinely unresolved pointer — the INDIRECT itself is
## correct, conservative behavior; the open question moves one level back

Picked up Finding 5's own stated next step: identify what PcodeOp the
INDIRECT's cause constant at `0x141c2` decodes to. Traced this live via gdb
against `decomp_test_dbg.exe` (see `review25.md`/`review26.md` in this
directory for full session mechanics, tooling notes, and — importantly — a
mid-investigation mislabeling that this finding corrects; those two files
should now be treated as superseded scratch work, not read as current).

**Mechanism, confirmed live and reproducibly (with `set disable-randomization
on` in gdb — without it, breakpoint hit-order in this harness varies run to
run due to heap-layout/ASLR effects on `varstack`'s iteration order; any
trace result from this investigation obtained without that setting should be
treated as unverified until re-run with it):**

- `PcodeOp::getOpFromConst(vnnew->getDef()->getIn(1)->getAddr())` on the
  `register:2` INDIRECT at `0x141c2` decodes to a `PcodeOp` at the same
  address, opcode value **3**. Earlier tracing this session mislabeled
  opcode 3 as `CPUI_LOAD` — it is actually **`CPUI_STORE`**
  (`opcodes.hh`: `CPUI_COPY=1, CPUI_LOAD=2, CPUI_STORE=3`). This correction
  matters: a `LOAD` and a `STORE` imply different, non-interchangeable
  mechanisms in `heritage.cc`, and the STORE identification is what actually
  explains the INDIRECT (see below) — the LOAD framing in review26.md does
  not.
- This STORE has `output = 0x0` (STOREs never have an output Varnode — this
  is normal, not evidence of a dead/incomplete op) and 3 inputs (space-id,
  pointer, value — the correct `CPUI_STORE` shape).
- Inspected the STORE's pointer input (`op->getIn(1)`) directly:
  `isConstant=0, isFree=1, isWritten=0`. **The STORE's destination pointer
  is a genuine, unresolved free Varnode** at this point in the heritage
  pass — not a stale/dangling reference, not corrupted state, just an
  address the decompiler legitimately cannot yet resolve.
- Traced the INDIRECT-creation call site responsible:
  `Heritage::guardStores` (`heritage.cc:1539-1560`). For every address range
  being heritaged, this function walks all `CPUI_STORE` ops in the function
  and inserts an `indirect_store`-flagged `INDIRECT` on that range whenever
  either the STORE targets the containing space via a spacebase pointer, or
  — the branch that fires here — `spc == storeSpace`, i.e. the STORE writes
  directly into the same address space as the range being heritaged
  (`register`), with **no narrowing by resolved offset**. This is different
  machinery from `LoadGuard`/`StoreGuard`'s `establishRange`/`finalizeRange`
  range-refinement (confirmed those never fire for this STORE at all — they
  only apply to the indexed-stack-pointer guard path, not this
  same-space-conservative path).

**Conclusion: the INDIRECT on `register:2` at `0x141c2` is correct,
intentional, conservative behavior given a free/unresolved STORE pointer —
not a decompiler defect at this site.** When a STORE's destination can't yet
be resolved to a specific address, `guardStores` must assume (correctly, by
design) that it could write anywhere in its target space, including
`register:2`, and insert an INDIRECT accordingly. Finding 5's framing of
this as "a genuine write-write collision" undersold this — Write B isn't a
malformed or erroneous op, it's the intended conservative guard for a
not-yet-resolved STORE.

**This means Finding 5's own open question ("what PcodeOp does `0x24`
decode to") is now answered, but the real open question moves one level
back and is now sharper than anything in Findings 1-11:**

**Why does this STORE's destination pointer remain unresolved at the point
`guardStores` runs, for a value that (per the disassembly already gathered
in this document) should be perfectly statically resolvable — there is no
computed/indexed addressing anywhere in `muldiv_u16`'s instruction stream?**
If the pointer should have resolved earlier and didn't, that's the actual
bug — either in why this particular STORE's pointer computation stays free
longer than it should, or in heritage pass ordering/timing that runs
`guardStores` before that resolution has had a chance to happen. If it's
expected to still be free at this point and get resolved in a later
heritage pass/restart, then the real bug is downstream: something isn't
cleaning up this now-unnecessary INDIRECT once the STORE's real destination
is known not to be `register:2` after all.

**Concrete next step:** identify which real instruction/PcodeOp this STORE
corresponds to (cross-reference its address and inputs against
`muldiv_u16`'s full P-code dump, not just the `register:2` INDIRECT chain
already captured in Finding 5), and determine whether its pointer is: (a)
inherently unresolvable at this stage by design (e.g. it's a spacebase/stack
pointer under construction, in which case check why `usesSpacebasePtr()`
reads false for it — Finding 12's own trace showed `usesSpacebasePtr=0` for
this STORE, which is itself slightly surprising given the function's
`LINK FP,#0x0` frame and worth double-checking), or (b) a genuine
resolution failure that a fix should address. Do not assume either answer
without checking — this is now a precise, mechanical question, same as
Finding 5 characterized its own next step, and should be pursued with a
**deterministic** gdb trace (`set disable-randomization on`) against
`decomp_test_dbg.exe`, not against the live PyGhidra/decompile.exe path
used for Findings 1-11.

## Finding 13 (2026-09-15): resolved — the unresolved STORE pointer is a
## normal artifact of heritage's multi-pass/restart structure, not a bug

Followed Finding 12's own concrete next step: traced what defines the
`0x141c2` STORE's stored value (`in2`) and its pointer (`in1`), to determine
whether the pointer's unresolved state at guard-insertion time is expected
or a genuine failure.

**What was found:** the STORE's stored value is defined by a `CPUI_SUBPIECE`
(opcode 63) truncating a 4-byte value down to 2 bytes — structurally
matching Finding 5's own "Write A" description (the real multiply result,
truncated to its low 16 bits). But at the specific point this trace
observes it, that 4-byte input is **itself still free/unresolved**
(`isWritten=0`) — i.e. the multiply hasn't been computed yet either, at this
point in analysis.

Checked how many times `Heritage::heritage()` (the top-level per-pass entry
point) runs for this function: **12 times**, confirmed via a breakpoint at
`heritage.cc:2751`. Heritage is address-range-incremental and
restart-driven (consistent with the `"Restarted to delay deadcode
elimination for space: register"` warning already known from this
document's earlier findings) — different passes heritage different address
ranges and can run before other parts of the function's data-flow are fully
resolved.

**Conclusion: the free STORE pointer and free SUBPIECE input observed in
Finding 12 are simply a snapshot of an early, incomplete heritage pass —
not a stuck or failed resolution.** By the time the final decompile output
is produced, the real multiply chain (per Finding 5's own P-code dump) is
fully resolved. This closes out Finding 12's open question: there is no
resolution-failure bug at the `guardStores`/STORE-pointer level. The
INDIRECT on `register:2` is legitimate, temporary, conservative machinery
that existed during intermediate heritage passes and should not itself be
the target of any fix.

**What this means for the investigation as a whole:** Findings 5 and 12-13
together show that the mechanism creating the competing write on
`register:2` is ordinary, correct decompiler machinery reacting to
legitimately-incomplete intermediate state — not a bug in `heritage.cc`'s
INDIRECT/guard logic itself. The real defect, if `muldiv_u16`'s wrong output
is to be explained at the same mechanistic level as `muldiv_u16_scaled`'s
(Finding 3's `isDead()`-fallback ordering issue), must lie in **how the
final Cover/merge pass chooses between the two competing definitions of
`register:2` once heritage is complete** — i.e. back in `cover.cc`/
`merge.cc` territory, the same files already implicated for
`muldiv_u16_scaled`, rather than in `heritage.cc`'s INDIRECT-creation logic.
This is a return to, and narrowing of, the direction Finding 9 in the
"MAJOR CORRECTION" section originally set aside in favor of the
`.cspec`/calling-convention thread — that thread (Findings 9-11) is
confirmed correct and complete for `muldiv_u16_scaled`'s parameter-offset
half of its bug, but does not touch `muldiv_u16` at all (per Finding 7's own
note that `muldiv_u16` uses the plain `__stdcall_far_allstack` and reads
`stack:4`/`stack:6` directly, not through named parameters).

**Concrete next step for `muldiv_u16` specifically:** with `heritage.cc`
now cleared across Findings 1, 4, 5, and 12-13, pursue the **final Cover
range / `Merge::mergeOp` selection logic** for how it picks which of
`register:2`'s two writers (SUBPIECE-sourced multiply-result vs.
STORE/INDIRECT-guarded chain) becomes the live value at the point of RETURN
— this is the same class of question Finding 3 already answered for
`muldiv_u16_scaled` (fallback insertion positioning in `opInsertAfter`), and
`muldiv_u16`'s equivalent mechanism, per Finding 1, is confirmed to NOT be
that exact code path (`isDead()` never true for `muldiv_u16`) — so this
needs its own trace of `Merge::mergeOp`/`Cover::addRefPoint` for this
specific function, deterministically (`set disable-randomization on`),
rather than assuming the `muldiv_u16_scaled` mechanism transfers over.

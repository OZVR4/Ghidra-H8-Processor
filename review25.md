> **SUPERSEDED (2026-09-15):** see `review-9561-merge-ordering.md`, Finding
> 12, for the current, corrected, authoritative status of this
> investigation. This file (and `review26.md`) are kept for historical
> session detail only.

# Session 25 (2026-09-15): trimOpOutput ordering fix landed; SEGMENTOP hypothesis tested and reverted; forward-trace from RETURN corroborates Finding 5 (write-write collision) over the mergeOpcode Cover-boundary bug

## Summary for whoever picks this up next

Three things happened this session, in order:

1. **Applied and confirmed a real fix** for the `trimOpOutput` fallback-position
   defect described in `review-9561-merge-ordering.md` (Finding 3). This is a
   genuine improvement, landed in the shared decompiler source, and safe to
   keep.
2. **Tested and reverted a speculative fix** to `Heritage`'s stack-pointer
   discovery walk (the `CPUI_SEGMENTOP` case in the function containing
   `generateLoadGuard`/`generateStoreGuard`). The hypothesis was reasonable
   but directly falsified by rebuilding and re-decompiling -- zero effect on
   `muldiv_u16`'s output, and the change was reverted rather than left in
   shared spacebase-tracking code with unverified regression risk.
3. **Used the existing `decomp_test_dbg.exe` + gdb harness** (from
   `review20.md`) to forward-trace `muldiv_u16`'s `RETURN` op's actual printed
   HighVariable identity, using debug instrumentation already present in
   `printc.cc::opReturn`. This produced a new, load-bearing result: it
   corroborates **Finding 5** (the `stack:6`-sourced write-write collision at
   `0x141c2`, from `review-9561-merge-ordering.md`) as the real mechanism, and
   effectively rules out the `mergeOpcode`/`Cover::intersectList` boundary bug
   that `review20.md` spent significant effort tracing and fixing -- that fix
   works exactly as designed but has **no effect on this symptom**, matching
   what `review20.md` itself already concluded before this session started.

**Net position on the two open bugs, updated:**

- `muldiv_u16_scaled`'s `isDead()`-fallback ordering defect (Finding 3): now
  actually fixed in `merge.cc::trimOpOutput`. The `LowlevelError` this used to
  throw is gone. The function's wrong-value symptom (`return uVar5;` instead
  of the real division result) persists, but per the P-code inspected this
  session, that specific function *also* exhibits the same three-way
  `INDIRECT`-into-overlapping-registers shape as `muldiv_u16`'s write-write
  collision (Finding 5) at its own multiply/scale step (`0x141e9`). It is very
  likely the *same* underlying collision mechanism, not a second
  `trimOpOutput`-adjacent bug, and should be re-tested once Finding 5's actual
  cause is fixed rather than pursued separately.
- `muldiv_u16`'s write-write collision (Finding 5): still open, but now
  corroborated end-to-end from cause to printed symptom (see below). This is
  the one real remaining thread, and the next concrete step is scoped and
  ready to pick up with the gdb harness.

## 1. `trimOpOutput` fallback-position fix (landed, confirmed)

File: `Ghidra/Features/Decompiler/src/decompile/cpp/merge.cc`,
`Merge::trimOpOutput`.

Old code (the crash-fix from before this session, already in
`review-9561-merge-ordering.md`):

```cpp
if (op->code() == CPUI_INDIRECT && op->numInput() > 1 && op->getIn(1)->getSpace()->getType() == IPTR_IOP) {
  PcodeOp *indirectSource = PcodeOp::getOpFromConst(op->getIn(1)->getAddr());
  afterop = indirectSource->isDead() ? op : indirectSource; // Insert copyop AFTER source of indirect, unless stale
}
else
  afterop = op;
```

New code (this session):

```cpp
if (op->code() == CPUI_INDIRECT && op->numInput() > 1 && op->getIn(1)->getSpace()->getType() == IPTR_IOP) {
  PcodeOp *indirectSource = PcodeOp::getOpFromConst(op->getIn(1)->getAddr());
  if (!indirectSource->isDead())
    afterop = indirectSource;	// Insert copyop AFTER source of indirect
  else {
    // The op that originally caused this INDIRECT is gone (stale/dangling
    // IOP reference). Falling back to afterop = op is not always safe: op
    // (the INDIRECT itself) can appear earlier in program order than the
    // real definition of its own input Varnode (op->getIn(0)), e.g. when a
    // later CPUI_COPY is what actually defines that Varnode. In that case
    // inserting the trim-COPY right after op places it before the input's
    // real definition, producing a use-before-define ordering defect that
    // Cover/Merge correctly detect further down the pipeline (see 9561
    // ordering-defect investigation). Prefer inserting after the input
    // Varnode's own defining op, when that op is alive, since that is
    // guaranteed to come no earlier than the input's real definition.
    PcodeOp *inputDef = op->getIn(0)->getDef();
    afterop = (inputDef != (PcodeOp *)0 && !inputDef->isDead()) ? inputDef : op;
  }
}
else
  afterop = op;
```

**Confirmed via the live PyGhidra/`decompile.exe` path** (rebuilt with
`:Decompiler:decompileWin_x86_64Executable`, deployed to
`...\ghidra_12.2_DEV\Ghidra\Features\Decompiler\os\win_x86_64\decompile.exe`,
verified via matching `LastWriteTime`/size, PyGhidra restarted to reconnect):
`muldiv_u16_scaled` force-decompiles cleanly with no `LowlevelError`, matching
the pre-existing crash-fix's behavior but now also handling the ordering
case Finding 3 identified. This is a real, keepable fix.

**Caveat:** the person's earlier per-function `set_function_prototype`
override from Finding 11 (the `_farcall` prototype, extrapop=6) was not
present after this session's PyGhidra restart -- database-level per-function
changes don't survive a program reopen unless saved. `muldiv_u16_scaled`'s
decompile in this session shows the pre-Finding-11 parameter offsets again
(`Stack[0x4]` etc.). This is a tooling/workflow note, not a regression: the
`.cspec`-level prototype definition itself is still on disk and intact; only
the per-function assignment needs redoing (or the program needs saving after
applying it) before Finding 11 shows up again in a fresh session.

## 2. `CPUI_SEGMENTOP` hypothesis in `heritage.cc` (tested, reverted)

Hypothesized that `Heritage`'s stack-offset discovery walk incorrectly
treats a `CPUI_SEGMENTOP`'s output as inheriting the same tracked stack
offset as its input, when a far/segmented pointer computation should be
treated as leaving the tracked address space entirely. This looked plausible
because both `muldiv_u16` and `muldiv_u16_scaled` dereference a far parameter
via exactly `PTRADD -> SEGMENTOP -> LOAD`, and the spurious `INDIRECT`s
shadowing the real computed value seemed like they could be downstream of
this.

**Tested directly:** changed the `CPUI_SEGMENTOP` case to `break` immediately
instead of falling through to the `COPY`/`INDIRECT` handling, rebuilt,
redeployed, reconnected PyGhidra, force-decompiled `muldiv_u16`. Result: the
P-code at `0x141c2` was **byte-for-byte identical** before and after --
including the exact same three `INDIRECT`s with cause constant `0x24` and the
same spurious `COPY(stack:6) -> INDIRECT -> register:2` chain. The collision
happens at `0x141c2` (the multiply), which is *before* the `SEGMENTOP`/LOAD at
`0x141c4` even executes, so the hypothesis was structurally unable to be the
cause. **Reverted** back to the original code rather than leave a no-benefit
change with real regression risk (this is shared spacebase-tracking logic;
the `.cspec` history already documents wide, hard-to-predict regressions from
changes in this neighborhood).

Net: this was a legitimate hypothesis worth testing quickly, tested cleanly,
falsified cleanly, and cleanly reverted. No residue left in the tree from
this attempt.

## 3. Forward-trace from RETURN via the `decomp_test_dbg.exe` + gdb harness

`review20.md` already built a fast, PyGhidra-free reproduction harness
(`decomp_test_dbg.exe`, built from the same shared source tree via a plain
MinGW g++ command, driven by a standalone XML test case at
`C:\Users\j.brophy.CORKILLSYSTEMS\decomp_test_build\muldiv_repro_tests\`).
That session used it to find and fix a genuine `Cover::intersectList`
level-2 boundary-touch bug (via `mergeTestBoundary` reused in
`Merge::mergeOpcode`), confirmed working exactly as designed via gdb, but
found the fix had **zero effect** on the actual wrong-output symptom -- i.e.
that bug, while real, is not what's producing `muldiv_u16`'s bad return
value. `review20.md` ended with an untried next step: trace *forward* from
the `RETURN` op to find the actual printed identity, rather than only
backward from symptom to merge chain.

This session picked that up. `printc.cc::PrintC::opReturn` already has debug
instrumentation (from a still-earlier session, still gated and harmless)
that logs, at print time, the `RETURN` op's input Varnode, its `HighVariable`,
that HighVariable's `numInstances()`, and the `getNameRepresentative()`
Varnode's own defining op.

Rebuilt `decomp_test_dbg.exe` from the current shared source tree (which
still has the `mergeTestBoundary` fix from `review20.md` and now also this
session's `trimOpOutput` fix), reran the standalone XML repro, and read the
log:

```
[opReturn] retOp=0x000141da:6a retVn=0x1987d0fab60 retVn->getAddr()=0x00 retVn->getDef()=141d7 high=0x1987d1b71e0 high->numInstances()=1 nameRep=0x1987d0fab60 nameRep->getAddr()=0x00 nameRep->getDef()=141d7
[opReturn] retOp=0x000141da:c1 retVn=0x1987d1b47b0 retVn->getAddr()=0x00 retVn->getDef()=141d7 high=0x1987d1b7390 high->numInstances()=1 nameRep=0x1987d1b47b0 nameRep->getAddr()=0x00 nameRep->getDef()=141d7
```

(Two log lines rather than one, with different object addresses but
identical `getDef()`, are consistent with the decompiler's print pipeline
running its emission logic more than once, e.g. a measurement pass and a
real emission pass -- not evidence of two distinct bugs.)

**Key facts from this trace:**

- `high->numInstances()==1` in both cases. By the time printing happens,
  there is **no merge ambiguity** for this Varnode -- it is not a case of
  multiple candidate definitions sharing one `HighVariable` and the wrong one
  being chosen as the name representative. There is only one instance, and
  it *is* the one that gets read at `RETURN`.
- That sole instance is defined by the op at `0x141d7`, which (per the
  disassembly already on file) is a straightforward register-to-register
  `COPY` near the function's exit.
- The XML-only, symbol-free repro's own printed output shows this Varnode
  labeled `in_stack_00000006` -- i.e. it traces, through that `0x141d7` COPY,
  back to `stack:6` (parameter `b`'s storage), not to the multiply/divide
  computation.

**What this rules in and out:**

- **Rules out** the `mergeOpcode`/`Cover::intersectList` boundary bug as the
  cause of this specific symptom, confirming (via an independent method) what
  `review20.md` already concluded when its correctly-working fix didn't
  change the output.
- **Corroborates Finding 5** from `review-9561-merge-ordering.md`: the
  genuine write-write collision at `0x141c2`, where a real
  `SUBPIECE`/`COPY`-based write of the true multiply result to `register:2`
  coexists with a spurious `COPY(stack:6) -> INDIRECT(...,0x24) -> register:2`
  at the same P-code address. The forward trace shows the `0x141d7` COPY's
  source is, transitively, the same `stack:6`-rooted value -- i.e. the
  spurious write from the `0x141c2` collision is the one that survives SSA
  construction and propagates all the way to `RETURN`. This is a fully
  connected causal chain now: **`stack:6` -> spurious `INDIRECT` at `0x141c2`
  (wins the collision) -> `register:2`'s surviving definition -> read by the
  `COPY` at `0x141d7` -> printed at `RETURN`.**

## Next step (not yet attempted, explicitly deferred by the person this session)

Trace, live via the `decomp_test_dbg.exe` + gdb harness, the actual SSA
renaming/collision-resolution logic that decides which of the two `0x141c2`
writes becomes `register:2`'s live definition. This is `heritage.cc` territory
(the rename pass, not the guard-creation code investigated and ruled out
this session), and per Finding 5's own recommendation, the INDIRECT's cause
constant (`0x24` at this address, in the numbering scheme of whichever build
is current) is very likely encoding a reference to whatever earlier op is
responsible for creating this spurious INDIRECT in the first place -- finding
that op via gdb, live, on the same minimal repro, is the concrete next move.

Suggested approach, following the exact pattern that worked in `review20.md`:

1. `gdb --args decomp_test_dbg.exe -sleighpath "...\ghidra_12.2_DEV" -path
   "...\muldiv_repro_tests" datatests`
2. Breakpoint somewhere in the heritage rename pass that decides between
   competing definitions at the same address for the same storage location
   (not yet identified by file/line this session -- first task is finding the
   right function via source reading or a `Funcdata::opInsertAfter`/
   `Heritage`-adjacent grep, the same way `merge.cc:390` and
   `variable.cc:1243` were found last session).
3. Once at that breakpoint, inspect which `PcodeOp*` the `0x24` cause constant
   on the spurious `INDIRECT` actually decodes to (via
   `PcodeOp::getOpFromConst`, callable directly from gdb the same way
   `mergeTestBoundary` was called directly in `review20.md`), to identify the
   real originating op.
4. Determine why that op's effect is chosen over the legitimate multiply
   result's `COPY`/`SUBPIECE` chain, and design a fix analogous in spirit to
   `mergeTestBoundary`/this session's `trimOpOutput` fix: correct the
   resolution at its actual decision point, not by guessing at guard-creation
   code upstream (which this session already tried once and disproved for
   this exact symptom).
5. Validate via the same harness before ever touching the live
   PyGhidra/`decompile.exe` path, exactly as `review20.md` and this session
   both did.

## State of the tree as of end of this session

- `merge.cc::trimOpOutput`: **fixed, keep.** (Section 1 above.)
- `merge.cc::mergeOpcode`'s `mergeTestBoundary` reuse: still present from
  `review20.md`, confirmed still compiling and still firing correctly. Real
  fix for a real (separate) bug; keep.
- `heritage.cc`'s `CPUI_SEGMENTOP` case: **reverted to original, no net
  change this session.**
- `printc.cc::opReturn`'s debug instrumentation: still present, still gated,
  still harmless -- used this session, still worth keeping until the
  investigation concludes, per the existing "REMOVE before any
  upstream-facing change" comment already on it.
- Live PyGhidra `decompile.exe`: currently deployed build reflects the
  `trimOpOutput` fix only (the `SEGMENTOP` experiment was rebuilt/redeployed
  transiently but the revert was rebuilt and redeployed last, so the
  currently-live binary is clean). `muldiv_u16_scaled`'s per-function
  prototype override from Finding 11 needs reapplying in this program session
  (or saving after applying) since it did not survive the PyGhidra restarts
  this session required.
- `decomp_test_dbg.exe`: rebuilt this session from current shared source
  (includes both the `mergeTestBoundary` and `trimOpOutput` fixes). Confirmed
  working, confirmed producing the forward-trace log used in Section 3.

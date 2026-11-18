# auStack_ grouping regression -- investigation continuation (from review16.md)

Status: OPEN INVESTIGATION, NOT YET FIXED.
Full prior history/rationale for the register0x0e/SEGMENTOP fix itself: see
review16.md (sections 1-6, CONFIRMED COMPLETE, landed as
NationalSecurityAgency/ghidra#9541 / commit aadd7ee).
This file continues ONLY the auStack_ grouping regression thread that began
in review16.md's final two sessions (2026-08-26, "auStack_ regression
investigation" and "auStack_ grouping is a real structural change"). That
investigation is explicitly OUT OF SCOPE for PR #9541 and must land as its
own separate, independent fix per review16.md's own established practice.

## Problem (carried over from review16.md)

Whole-ROM regression pass on `RVR_1998_x3 4g63t 21000011 md352553.hex`
(pre- vs post-register0x0e-fix export) showed:
- `stack0x` occurrences: 355 (old) -> 20 (new)
- `auStack_` occurrences: 16 (old) -> 214 (new)
- `register0x0e` occurrences: 1 (old) -> 1 (new), unaffected

Example, `sat_add_u16`:
```
OLD: result = a + *(ushort *)((int)&stack0xfffe + 8);
NEW: result = a + *(ushort *)((int)(auStack_102 + 0x80) + 8);
```
Both old and new fail to print parameter `b` by its real, correctly-declared
name (confirmed via saved debug.xml: `b` is a locked, typelocked `ushort`
Symbol at stack offset 0x6). This is a pre-existing Symbol-*lookup* failure,
not something introduced by the register0x0e fix -- but the register0x0e fix
changed which of two fallback rendering paths the failure lands on
(`stack0xNNNN` implicit-reference vs `auStack_NNN[128]` grouped-array), and
the `+0x80` term proves the new path is a real structural regrouping of
multiple stack locals into one array Symbol, not just a cosmetic rename.

## Section 7 recap and correction (2026-08-26, continued in this session)

review16.md's last entry proposed a `__stdcall_far_allstack` `extrapop=4`
(current h8539f.cspec) vs `extrapop=6` (locked in `sat_add_u16`'s saved
debug.xml prototype) mismatch as the likely mechanism, reasoning that a
stale extrapop could misalign frame-layout recovery and trigger the
regrouping.

**This theory does NOT hold up under direct source tracing and should be
considered ruled out as stated.** Traced the actual code:

- `ScopeLocal`'s stack-local range bounds (`range` / `paramRange` in
  `MapState`, used throughout `varmap.cc`'s `addRange`/`adjustOutOfRange`)
  come from `FuncProto::getLocalRange()` / `getParamRange()`
  (fspec.hh:838/1538), which return `ProtoModel::localrange` /
  `paramrange`.
- Those fields are set either by `ProtoModel::defaultLocalRange()` /
  `defaultParamRange()` (fspec.cc:2263-2309 -- large generic
  half-of-stack-space defaults, computed purely from `stackgrowsnegative`
  and the stack space's size) or decoded verbatim from `<localrange>` /
  `<paramrange>` XML elements in the cspec (fspec.cc:2597-2646). **Neither
  path reads or depends on `extrapop` at all.**
- `getExtraPop()` / `effective_extrapop` are used elsewhere entirely: at
  CALL sites, by the *caller*, to inject a stack-pointer-adjustment
  constant after a call returns (coreaction.cc:1516-1523,
  `ActionStackPtrFlow`-adjacent). This affects how a caller's SP is
  adjusted post-call, not how the callee's (`sat_add_u16`'s) own local
  scope bounds or its own Varnodes' stack-relative offsets are computed.

**Conclusion: the extrapop=4-vs-6 discrepancy, even if real, cannot directly
cause `sat_add_u16`'s own stack-local grouping to change via the mechanism
proposed.** It may still be worth explaining (see Not-yet-done below) but is
not the root-cause candidate going forward.

## Actual grouping mechanism, located

The code that decides whether several stack locals get swept into ONE
`auStack_NNN[128]`-style array Symbol, vs. resolved individually by name, is
in `varmap.cc` (ghidra checkout,
`Ghidra/Features/Decompiler/src/decompile/cpp/varmap.cc`), NOT in
`buildVariableName` (varmap.cc:623 -- that function only synthesizes a name
for whatever data-type/range decision has already been made upstream of it).

Key pieces, all in `RangeHint` (varmap.cc lines ~30-373):
- `RangeHint::reconcile()` (~62-103) -- decides whether two overlapping
  ranges' data-types can coexist without destroying type information (e.g.
  array-of-unknown absorbing a smaller fixed reference).
- `RangeHint::preferred()` (~128-159) -- when two ranges start at the same
  offset, decides which one's data-type wins (locked > unlocked, fixed >
  open, more-specific type ordering).
- `RangeHint::attemptJoin()` / `RangeHint::absorb()` (~172-304) -- the
  actual array-growing logic: if `this` is an open array-typed range and
  `b` lines up on the element stride, `b` gets absorbed and `highind`
  (the array's known index bound) grows.
- `RangeHint::merge()` (~315-373) -- top-level entry point called when two
  ranges in the sorted `maplist` overlap; decides via `contain()` +
  `reconcile()` + `preferred()` whether to keep `this`'s type, take `b`'s
  type, or give up and fall back to an `TYPE_UNKNOWN` sized primitive
  (`resType == 2`, the "Concede confusion about types" branch,
  varmap.cc:356-370).

Feed path: `MapState::gatherVarnodes` (not yet read this session) and
`MapState::gatherSymbols` (varmap.cc:1170-1186) populate `maplist` with one
`RangeHint` per Varnode/Symbol reference in the function via `addRange()`
(varmap.cc:1019-1045), bounded by `range`/`paramRange` (the
`getLocalRange()`/`getParamRange()` ruled out above as extrapop-sensitive,
but still the actual boundary check in `adjustOutOfRange`,
varmap.cc:973-1010). `MapState::initialize()` (varmap.cc:1190-1215) sorts
the list and calls `reconcileDatatypes()` (varmap.cc:1086-1122) before the
main iteration (not yet located/read this session -- likely
`ScopeLocal::restructureVarnode` or similar, walks `maplist` pairwise and
calls `RangeHint::merge()` on overlaps).

## Next steps

1. **Locate and read the main iteration loop** that walks the sorted
   `maplist` and actually invokes `RangeHint::merge()`/`attemptJoin()` on
   overlapping entries (likely `ScopeLocal::restructureVarnode` in
   varmap.cc, not yet located/read this session -- grep for `merge(` call
   sites and/or `restructureVarnode` in varmap.cc/varmap.hh).
2. **Instrument or otherwise directly observe** (not infer) the actual
   `RangeHint` list `MapState::gatherVarnodes` builds for `sat_add_u16`,
   pre- vs post-register0x0e-fix, to see directly which Varnode(s)'
   offsets/types changed and triggered the new grouping decision. This is
   the concrete replacement for the ruled-out extrapop theory -- go look at
   the actual RangeHint list rather than continuing to theorize from
   symptoms.
   - Candidate mechanism worth checking directly: the register0x0e fix's
     `linkAnnotationSymbolAtUsepoint` (funcdata_varnode.cc, added in
     aadd7ee) now mints new annotation Symbols for `SP`/`FP` at usepoints
     that previously had none. If `sat_add_u16` contains any
     SEGMENTOP-consuming self-referential or cross-register arithmetic
     (even incidentally, not necessarily on the exact `b`-parameter access
     path), a newly-created Symbol could shift what `gatherSymbols()` sees
     and change how overlapping ranges resolve. Confirm or rule this out
     directly before assuming it, the same way the vnAnnotation/vnImplied
     mystery in review16.md was only resolved by a direct live flag dump,
     not by source-reading alone.
3. Once the real mechanism is identified, confirm whether the underlying
   `b`-parameter Symbol-lookup failure itself (pre-existing, present in the
   OLD export too) is fixable at the same time, is a separate issue, or is
   out of scope entirely -- don't conflate the "which fallback branch"
   regression with the "lookup fails at all" pre-existing bug; both are
   real but may need separate fixes.
4. Per review16.md's established practice (Step 4's "own separate, clean
   commit" rule; the self-referential vs cross-register split for #9541),
   whatever fix comes out of this MUST land as its own independent patch,
   not folded into PR #9541.
5. Lower priority / not blocking: the extrapop=4-vs-6 discrepancy on
   `__stdcall_far_allstack` observed between `h8539f.cspec` and
   `sat_add_u16`'s locked debug.xml prototype is still real and unexplained
   (why did 6 get saved when the cspec has said 4 since this prototype was
   introduced?) even though it's now been ruled out as the direct cause of
   the auStack_ regrouping. Worth a short, independent investigation later,
   but not on the critical path for this bug anymore.
6. Carry-over items from review16.md's own final "Not yet done" list that
   are still open and unrelated to this specific thread (WARNING count
   604 untriaged, the `register0x0e + 6` call-argument case deferred by the
   user, PR #9541 cleanup/reconciliation) are NOT restated here -- see
   review16.md's own final entries for those; this file only tracks the
   auStack_ thread going forward.

## Grouping mechanism traced end-to-end -- root-cause candidate identified,
## but contradicts review16.md's own live-test result

Read the actual iteration loop and the AliasChecker feed path in varmap.cc
that review16.md's final entry flagged as the next concrete step:

- `ScopeLocal::restructure(MapState &state)` (varmap.cc:1425-1459) is the
  loop that walks the sorted `maplist` and calls `RangeHint::merge()` /
  `attemptJoin()` on overlapping ranges -- this is what actually decides to
  materialize a grouped array Symbol rather than resolving locals
  individually.
- `MapState::gatherVarnodes` (varmap.cc:1257-1339), `gatherOpen`
  (varmap.cc:1344-1382), and `gatherSymbols` (varmap.cc:1170-1186) are the
  three sources that populate `maplist` before `restructure` runs, called in
  that order from `ScopeLocal::restructureVarnode` (varmap.cc:1389-1417).
- `gatherOpen` calls `checker.gather(&fd,spaceid,false)`
  (`AliasChecker::gather`, varmap.cc:767-779), which walks forward from the
  function's raw stack-pointer INPUT Varnode
  (`fd->findSpacebaseInput(space)`) through every additive-chain descendant
  -- `AliasChecker::gatherAdditiveBase` (varmap.cc:816-883) explicitly
  follows `CPUI_INT_ADD`, `CPUI_INT_SUB`, `CPUI_PTRADD`, `CPUI_PTRSUB`, AND
  `CPUI_SEGMENTOP` (varmap.cc:867) as part of the same additive-sum walk.
  Each terminal sum result becomes an "open" (array-like, unknown-size)
  `RangeHint` seed via `addRange(offset,ct,0,RangeHint::open,minItems)`
  (varmap.cc:1371), with `minItems` set to at least 3 when an index Varnode
  was found (varmap.cc:1364-1370). An "open" RangeHint is exactly the kind
  `RangeHint::attemptJoin`/`absorb` grow into a combined array by absorbing
  neighboring fixed-size locals (varmap.cc:172-304) -- this is the direct
  mechanism that would produce `auStack_NNN[128]`.

**This is a plausible, well-supported root-cause candidate: since
`SEGMENTOP` is explicitly walked by `gatherAdditiveBase`, and the
register0x0e fix changes what Symbol (if any) gets attached to SEGMENTOP's
self-referential/cross-register input at print time via
`linkAnnotationSymbolAtUsepoint`, it seemed reasonable that this could shift
what `AliasChecker` discovers as a pointer base and change the open-range
seeding that leads to grouping.**

**However: a doc comment already present in the current source
(funcdata_varnode.cc:1298-1327, printc.cc:1949-1957/2019-2022, dated
"CHANGED 2026-08-25") describes exactly this theory and states a fix was
already applied for it** -- removing `addSymbol()` from
`linkAnnotationSymbolAtUsepoint` so it only looks up an existing Symbol
(via `queryProperties`) and never mints a new one, with
`pushSegmentRegisterExpression`/`pushCrossRegisterExpression` falling back
to a plain-text `pushAtom` of the register's real name (via
`getRegisterName`) on NULL, matching `pushAnnotation`'s own existing
fallback mechanism, rather than creating a competing Symbol at the base
register's canonical storage address (the same address the stack-frame
grouping pass uses as its own anchor).

**This is precisely the fix review16.md's own final session ("Fix attempted
based on that theory") implemented and live-tested -- and found to make NO
difference: `auStack_` count stayed at 214 after a full
rebuild+reinstall+restart+re-export cycle, confirmed via both the export
diff and a direct `force_decompile` on `sat_add_u16`.** The doc comment in
the current source describes the theory and the fix as if it were the
resolution, but does not (as of this read) reflect that live testing already
falsified it. This is a real discrepancy between what the code comments
claim and what review16.md's own measurement showed -- worth flagging
explicitly rather than re-trusting the comment's framing.

**Conclusion, consistent with this project's own repeated lesson (the
vnAnnotation/vnImplied mystery, the deployment-path confusion, etc.): do
not treat this doc comment's stated theory as confirmed just because it
reads correctly and is already implemented. It was tested live and refuted.
The actual cause of the grouping is still unknown and must be found by
direct measurement, not further source-reading.**

## Next steps (supersedes item 2 in the section above)

1. **Do not re-attempt or re-theorize the linkAnnotationSymbolAtUsepoint
   angle further** -- it's implemented correctly per its own design intent
   and was live-tested to have zero effect on the regression. Continuing to
   read that code path is very unlikely to produce a different answer than
   review16.md's session already found.
2. **Directly instrument `AliasChecker::gatherAdditiveBase` and/or
   `MapState::gatherOpen`/`addRange`** for `sat_add_u16` specifically (the
   same direct-measurement approach that resolved the vnAnnotation/vnImplied
   mystery in review16.md -- a synchronous file-based diagnostic, NOT a
   `warningHeader()`-based one given that mechanism's own documented timing
   failure earlier in this project's history) to capture the actual list of
   `AddBase` entries and/or `RangeHint`s built pre- vs post- the
   register0x0e fix (aadd7ee), for this one function. This is the concrete
   "go look at the actual RangeHint list" step from review17.md's original
   plan, now made specific: instrument `gatherAdditiveBase`'s `addbase`
   result and `gatherOpen`'s `addRange` calls, not `linkAnnotationSymbolAtUsepoint`.
3. Compare that captured list against the OLD (pre-aadd7ee) build's
   equivalent list for the same function, if a pre-fix build/binary is
   still available to re-test against, or reason carefully from the known
   pcode diff (SEGMENTOP's input Varnode shape changed between builds even
   where the register0x0e fix's print-time Symbol never differs) if not.
4. Once the actual differing Varnode/RangeHint is identified, re-evaluate
   whether the fix belongs in `AliasChecker` (e.g. excluding SEGMENTOP from
   the additive-chain walk when it's gated by
   `isSelfReferentialRegisterArithmetic`/`isCrossRegisterArithmetic`, since
   those inputs are print-time-only annotation constructs, not real pointer
   arithmetic a human would read as "indexing into an array"), in
   `gatherOpen`'s `minItems`/array-seeding logic, or somewhere in
   `RangeHint::merge`/`attemptJoin` itself.
5. All other items from the "Next steps" list above (separate patch from
   #9541, don't conflate the grouping regression with the pre-existing
   `b`-parameter lookup failure, etc.) still apply unchanged.

## Diagnostic instrumentation added (2026-08-26, same session)

Added a synchronous `std::ofstream` diagnostic directly into
`MapState::gatherOpen` (varmap.cc:1355-1378), gated to `fd.getName() ==
"sat_add_u16"` to keep output small. For every `AddBase` entry
`gatherAdditiveBase`/`AliasChecker::gather` produced, it logs: the alias
offset, the base Varnode's own address/size, whether an index Varnode was
found (the thing that sets `minItems=3` and triggers open-array seeding),
the base Varnode's current data-type name, and its `isAnnotation()` /
`isImplied()` flags (included because the earlier vnAnnotation/vnImplied
mystery in review16.md turned out to hinge on exactly this distinction, so
capturing it here up front avoids a second round-trip if it turns out to
matter again).

Output path: `C:\Users\j.brophy.CORKILLSYSTEMS\gatherOpen_diag.txt`
(append mode -- matches the working precedent from review16.md's
`recurse_diag.txt` diagnostic, same user path convention, NOT `/tmp` which
doesn't exist in this Windows build environment). Added `#include
<fstream>` (varmap.cc:17-18, wasn't previously included in this file).

**Next step: rebuild, copy, force-decompile `sat_add_u16` ONCE, then read
`gatherOpen_diag.txt` directly.** This will show, for the first time via
direct measurement rather than inference, exactly which Varnode(s)
`gatherAdditiveBase` treats as pointer bases and whether any of them are
annotation/implied SEGMENTOP-related Varnodes -- the concrete fact needed
to confirm or rule out the SEGMENTOP-walk theory from the section above.

**Reminder for whoever picks this up next:** per review16.md's own
established practice, remove BOTH the diagnostic code (the block above and
the `<fstream>` include) AND delete the diagnostic output file once the
answer is captured -- neither belongs in any upstream submission or repo
state.

## Diagnostic measured -- SEGMENTOP/annotation theory ruled out by direct
## measurement; grouping traced to non-local AddBase entries (2026-08-26)

Rebuilt, reinstalled, ran `force_decompile` on `sat_add_u16` once, and read
`gatherOpen_diag.txt`.

**Decompile output confirms the regression is still present post-rebuild**
(unchanged from prior sessions):
```
ushort __stdcall_far_allstack sat_add_u16(ushort a,ushort b)
{
  ushort result;
  ushort auStack_102 [128];
  result = a + *(ushort *)((int)(auStack_102 + 0x80) + 8);
  ...
}
```

**Diagnostic output, full pass (`addbase.size()=4`):**
```
[0] offset=0xfffe addr=0xe     size=2 baseType=undefined2 isAnnotation=no isImplied=no
[1] offset=0xfffe addr=0xe9300 size=4 baseType=undefined4 isAnnotation=no isImplied=no
[2] offset=0x4    addr=0x21d00 size=4 baseType=undefined4 isAnnotation=no isImplied=no
[3] offset=0x10006 addr=0x15c00 size=4 baseType=undefined4 isAnnotation=no isImplied=no
```
`isAnnotation=no` and `isImplied=no` on every entry, across every repeated
pass in the log (the diagnostic fires multiple times per single
`force_decompile`, alternating between the 4-entry full pass and several
1-entry passes showing only `[3]`). Entry `[3]`'s `baseType` field is blank
on some passes and `undefined4` on others for the identical Varnode
(addr=0x15c00) -- a real inconsistency, not yet explained.

**Conclusion: the SEGMENTOP-annotation-Symbol theory is now ruled out by**
**direct measurement, not just by the earlier live-test result.** No
annotation or implied Varnode is entering the AddBase list for this
function at all.

**Disassembly of `sat_add_u16` (00014000) obtained for cross-check:**
```
00014000: link FP,#0x0:8
00014002: mov:f.w  @(0x6:8,FP),R0
00014004: add:g.w  @(0x8:8,FP),R0
00014007: bhs      #0x0001400c:8
00014009: mov:i    #0xffff:16,R0
0001400c: unlk FP
0001400d: prts
```
Only two FP-relative operand accesses in the entire function body (offsets
0x6 and 0x8), no SEGMENTOP, no cross-register arithmetic, no additive chain
of any complexity. This does not obviously produce four AddBase entries on
its own.

**Program's address space layout checked:** only one physical space exists
(`ram`, flat, 0-0xffffffff). No separate register/stack space is reported
at this level, so the diagnostic's raw `baseVn addr` values (0xe, 0xe9300,
0x21d00, 0x15c00) can't be disambiguated by space name alone.

**Working hypothesis, NOT yet confirmed:** entry `[0]` (offset 0xfffe,
addr 0xe, size 2) plausibly corresponds to the real FP/stack-pointer input
Varnode (address 0xe matching FP's likely register storage address, size 2
matching the 16-bit register width) and is the legitimate stack base this
function's own code would produce. Entries `[1]`-`[3]` do not obviously
correspond to anything in `sat_add_u16`'s own two-instruction body and are
suspected to be non-local -- possibly leaking in from caller context, a
wider-than-expected `AliasChecker::gather` walk, or global/static aliasing
candidates incidentally sharing an offset -- but this has NOT been traced
to a specific source location yet and should not be treated as confirmed.

## Next steps (supersedes item 2 in the "Next steps" section above)

1. Do not re-open the SEGMENTOP/annotation-Symbol angle -- ruled out twice
   now (live test in review16.md, direct measurement above).
2. Determine what entries `[1]`, `[2]`, `[3]` in the AddBase list actually
   are and where they come from, given they don't correspond to anything
   visible in `sat_add_u16`'s own disassembly. Likely needs instrumenting
   `AliasChecker::gather`/`gatherAdditiveBase` itself (one level up from
   `gatherOpen`) to capture what Varnode each entry originates from, not
   just the terminal base/offset already captured.
3. Explain the blank-vs-`undefined4` `baseType` inconsistency on entry
   `[3]` across passes -- may or may not be related to the root cause, but
   is a real, reproducible anomaly worth a one-line note either way.
4. Confirm which entry (if any) is the legitimate FP/SP base vs. which are
   spurious, then re-evaluate where the actual fix belongs (AliasChecker
   scope, gatherOpen's minItems/seeding logic, or RangeHint::merge/
   attemptJoin) per item 4 of the prior "Next steps" section.
5. All prior carry-over items (separate patch from #9541, don't conflate
   with the pre-existing `b`-parameter lookup failure, remove diagnostic
   code + output file once no longer needed) still apply unchanged.

## SEGMENTOP confirmed present in the additive chain -- corrects the
## "ruled out" conclusion above (2026-08-26, same session)

Added a second diagnostic, one level up from `gatherOpen`: a synchronous
`std::ofstream` logger inside `AliasChecker::gatherAdditiveBase`'s
descend-loop (varmap.cc, right after `op = *iter;`), logging every pcode op
visited while walking additive descendants from the spacebase input. Since
`gatherAdditiveBase` is `static` (no `fd` member access -- first build
attempt failed with C2597/C3867/C2568 on `fd` for exactly this reason, since
this function has no `this`), the diagnostic is gated on `startvn->getOffset()
== 0xe` instead (the known spacebase input address for `sat_add_u16` from
the `gatherOpen` diagnostic above). Output:
`C:\Users\j.brophy.CORKILLSYSTEMS\gatherAdditiveBase_diag.txt`. Caveat: this
gate is by raw offset, not function identity, so entries from any other
function whose spacebase also lands at offset 0xe would interleave in the
same file -- not ruled out, though the results below are internally
consistent enough to be read at face value.

**Rebuilt, force-decompiled `sat_add_u16` once, read the output. One
representative pass (addresses match the four AddBase entries from the
`gatherOpen` diagnostic exactly):**
```
descend from vn addr=0xe     -> op INT_SUB    at 0x14000 out=yes
descend from vn addr=0xe     -> op SEGMENTOP  at 0x14000 out=yes
descend from vn addr=0xe     -> op COPY       at 0x14000 out=yes
descend from vn addr=0xe9300 -> op STORE      at 0x14000 out=no   [terminal, = AddBase[1]]
descend from vn addr=0xc     -> op INT_ADD    at 0x14002 out=yes
descend from vn addr=0xc     -> op SEGMENTOP  at 0x14004 out=yes
descend from vn addr=0x21b00 -> op SEGMENTOP  at 0x14002 out=yes
descend from vn addr=0x11000 -> op INT_ADD    at 0x14004 out=yes
descend from vn addr=0x21d00 -> op LOAD       at 0x14002 out=yes  [terminal, = AddBase[2]]
descend from vn addr=0x15c00 -> op LOAD       at 0x14004 out=yes  [terminal, = AddBase[3]]
```

**This directly contradicts the "ruled out by direct measurement" framing**
**two sections above and must be treated as superseding it.** `SEGMENTOP`
appears repeatedly and directly in the additive chain leading to all three
non-spacebase `AddBase` terminals (`0xe9300`, `0x21d00`, `0x15c00`) -- the
earlier "ruled out" conclusion only checked `isAnnotation()`/`isImplied()`
flags on the terminal AddBase Varnodes themselves, which is a different,
narrower question than whether SEGMENTOP is part of the walk at all. It is
part of the walk. What IS still correctly ruled out (unchanged) is the
specific `linkAnnotationSymbolAtUsepoint`-mints-a-new-Symbol mechanism --
that fix was live-tested with no effect. But the broader fact that
`AliasChecker::gatherAdditiveBase` treats `CPUI_SEGMENTOP` as just another
additive op and chains straight through it (varmap.cc:867-868, `case
CPUI_PTRSUB: case CPUI_SEGMENTOP:` falling into the same output-Varnode
push as `INT_ADD`/`PTRADD`) is real, confirmed by direct measurement, and
back in play as the root-cause mechanism -- just not via the annotation-
Symbol angle.

Note `sat_add_u16`'s disassembly (link/mov/add/bhs/mov/unlk/prts) contains
no literal SEGMENTOP-producing instruction; SEGMENTOP here must be
synthesized by the H8/539F SLEIGH semantics for the `@(disp,FP)`-style
addressing mode itself (consistent with review16.md's original
register0x0e/SEGMENTOP work), not by anything visible in the assembly.

## Next steps (supersedes the "Next steps" section immediately above)

1. Test whether gating `gatherAdditiveBase`'s SEGMENTOP case (varmap.cc:868)
   to exclude self-referential/cross-register SEGMENTOP uses -- the
   `isSelfReferentialRegisterArithmetic`/`isCrossRegisterArithmetic` idea
   floated earlier in this file (originally in the context of the
   already-ruled-out annotation-Symbol angle) -- actually changes the
   `auStack_` count when applied at the `gatherAdditiveBase` walk itself
   rather than at `linkAnnotationSymbolAtUsepoint`. This is now the
   concrete, most-directly-supported experiment given the measurement
   above.
2. If that has no effect either, the next candidate is excluding SEGMENTOP
   from `gatherAdditiveBase`'s walk unconditionally (not gated by
   self-referential/cross-register) and observing whether `sat_add_u16`
   still produces 4 AddBase entries or drops to just the legitimate
   spacebase one -- a coarser test to confirm SEGMENTOP-walking is
   necessary-and-sufficient before building the properly-scoped fix.
3. Still explain the blank-vs-`undefined4` `baseType` inconsistency on
   AddBase[3] noted in the prior section -- not yet investigated further
   this session.
4. All other carry-over items (separate patch from #9541, don't conflate
   with the pre-existing `b`-parameter lookup failure, remove BOTH
   diagnostic blocks AND the experimental SEGMENTOP gate itself once no
   longer needed -- the gate is still an active code change in
   `varmap.cc`, not just a diagnostic, and needs its own decision on
   keep/revert once the real fix is known) still apply unchanged.

## Experiment 1 result: gate has a real, measurable effect but does NOT
## fix the grouping by itself (2026-08-26, same session)

Implemented the gate from item 1 above: in `AliasChecker::gatherAdditiveBase`'s
`CPUI_SEGMENTOP` case (varmap.cc), split it out from `CPUI_PTRSUB` and added
an inline shape-check (self-referential: SEGMENTOP's consumed Varnode is
defined by `INT_ADD`/`INT_SUB` against a constant with the same storage
address as itself; cross-register: same shape but base is a *different*
`IPTR_PROCESSOR`-space register) mirroring
`isSelfReferentialRegisterArithmetic`/`isCrossRegisterArithmetic` in
printlanguage.cc (both `static`, not callable from varmap.cc, hence
inlined). On match, treat the SEGMENTOP as a non-additive terminal
(`nonadduse=true`) instead of chaining through it.

**First build attempt tested the wrong Varnode** -- checked
`op->getIn(0)` re-fetched fresh instead of the loop's own `vn` (the
Varnode actually being tested, matching how
`isSelfReferentialRegisterArithmetic(vn)` is really called at
printlanguage.cc:597). Rebuilt with this bug present, `sat_add_u16` was
unchanged (`auStack_102[128]` still present, gate never fired -- confirmed
by re-reading `gatherAdditiveBase_diag.txt`, which showed the exact same
unfiltered SEGMENTOP chain as before). Not a real negative result, just a
broken test -- corrected to test `vn` directly and rebuilt again.

**Second build (corrected gate) result:** `sat_add_u16`'s decompile output
changed from
```
result = a + *(ushort *)((int)(auStack_102 + 0x80) + 8);
```
to
```
result = a + (auStack_102 + 0x80)[4];
```
-- syntax changed (array-index vs. pointer-cast-and-dereference), but
**the `auStack_102[128]` grouping is still present.** The `+0x80` offset
is unchanged. The regression is NOT fixed by this gate alone.

**However, the gate does have a real, confirmed effect on the AddBase set**
(from `gatherOpen_diag.txt`, comparing the last full pass to earlier ones):
```
BEFORE (all passes): addbase.size()=4 -- [0]=0xe [1]=0xe9300 [2]=0x21d00 [3]=0x15c00
AFTER  (last pass):  addbase.size()=3 -- [0]=0xe [1]=0x21b00 [2]=0x15c00
```
Entry `0xe9300` (previously `[1]`) is gone entirely. Entry `0x21d00`
(previously `[2]`, the terminal beyond the `SEGMENTOP` chain through
`0x21b00`/`0x11000`) is also gone -- but `0x21b00` (previously only a
mid-chain hop, not a terminal) now appears as a NEW terminal in its place,
at a smaller size (2 bytes vs the old chain's 4-byte terminal). `0x15c00`
(previously `[3]`) is unaffected and still present in every pass. Cross-
checked directly against `gatherAdditiveBase_diag.txt`'s last full trace
(lines 149-182 of that file): the `0xc -> SEGMENTOP -> 0x21b00 -> SEGMENTOP`
sub-chain that used to continue on to `0x11000 -> INT_ADD -> 0x21d00 -> LOAD`
now stops at `0x21b00` itself becoming the terminal -- i.e. the SECOND
`SEGMENTOP` in that two-SEGMENTOP sub-chain (the one at `0x21b00`, matching
the gate's self-referential/cross-register shape) is the one being
excluded, not the first.

**Conclusion: the gate is mechanically working as designed (confirmed by**
**both diagnostics agreeing) and does change what `AliasChecker` reports --**
**but excluding just this one SEGMENTOP instance is not sufficient to stop**
**the grouping.** `0x15c00` (offset 0x10006) survives untouched in every
pass, before and after the gate, and is the most likely remaining driver
of the `auStack_102[128]` array-seed -- it was never part of the excluded
chain. The `0xe9300` disappearance is a secondary, not-yet-explained
side-effect (it wasn't the SEGMENTOP the gate's shape-check should have
matched, based on the chain reconstruction from the previous section) and
should be understood before concluding anything about it.

## Next steps (supersedes the "Next steps" section immediately above)

1. Trace WHY `0xe9300` disappeared even though the gate's match was on the
   `0x21b00` SEGMENTOP specifically, not the `0xc -> SEGMENTOP` one nearer
   `0xe9300` in the earlier chain reconstruction -- these two chains may
   share more structure than the previous section's reconstruction showed;
   re-examine `gatherAdditiveBase_diag.txt` line-by-line for the full new
   chain shape rather than only comparing terminal sets.
2. `0x15c00` (offset 0x10006) is untouched by this gate and is now the
   prime suspect for whatever is still seeding the `auStack_102[128]`
   grouping. Trace its own chain back to see whether it, too, passes
   through a SEGMENTOP that the current gate's shape-check does NOT catch
   (e.g. a variant not covered by the self-referential/cross-register
   shape as currently checked), or whether it's structurally unrelated to
   SEGMENTOP at all and the root cause for THIS terminal lies elsewhere
   (e.g. `gatherOpen`'s `minItems`/seeding logic itself, or
   `RangeHint::merge`/`attemptJoin`, per item 4 of the section before
   last).
3. Once `0x15c00`'s chain is understood, decide whether to broaden the
   gate's shape-check to cover it, or conclude the SEGMENTOP-exclusion
   angle is a dead end for this particular terminal and pivot to gating
   `gatherOpen`'s array-seeding logic directly instead.
4. All other carry-over items (separate patch from #9541, don't conflate
   with the pre-existing `b`-parameter lookup failure, remove BOTH
   diagnostic blocks AND the experimental SEGMENTOP gate itself once no
   longer needed -- the gate is still an active code change in
   `varmap.cc`, not just a diagnostic, and needs its own decision on
   keep/revert once the real fix is known) still apply unchanged.

## Experiment 2 result: root-cause traced to gatherOpen/minItems seeding,
## NOT AliasChecker's AddBase count (2026-08-26, same session)

Added a third diagnostic (`gateShape_diag.txt`) inside the gate itself,
dumping the shape of whatever op defines the Varnode a SEGMENTOP consumes.
This isolated the exact reason `0xc`'s SEGMENTOP was surviving the original
gate: **`0xc`'s def is `CPUI_COPY` with 1 input, not `INT_ADD`/`INT_SUB`
against a constant** -- the gate's shape-check only covered the arithmetic
case and never matched a bare register copy. Broadened the gate: a
SEGMENTOP consuming a Varnode defined by `CPUI_COPY` (1 input) from another
processor-space (register) Varnode is now also treated as non-additive.

**Rebuilt, force-decompiled `sat_add_u16` once more. Result: STILL
UNCHANGED** -- `auStack_102[128]` grouping persists, `+0x80` offset
identical, decompile output identical to the previous (post-Experiment-1)
build. Confirmed by re-checking `gatherOpen_diag.txt`: the gate change DID
have a further, different, real effect on the AddBase composition (the
last several passes now settle at 3 entries with `0xc` surviving on its own
as `[1]` rather than folding into `0x21b00`'s branch as before -- meaning
`0xc`'s own SEGMENTOP is now correctly excluded from continuing the walk,
matching the fix's intent) -- but **no combination of AddBase changes
achieved through this line of experimentation (4 -> 3 entries via two
successive gate fixes) has changed the actual decompiler output.**

**Conclusion: this closes out the "test whether excluding SEGMENTOP from**
**the walk changes the count" line of investigation (steps 1-3 of the**
**prior "Next steps") with a clear, if negative, result.** The `AddBase`
count IS being reduced by these gates, cleanly and as intended, but the
`auStack_` array-seeding behavior does not depend on `AddBase.size()` the
way this investigation assumed. **The root cause is very likely NOT in
`AliasChecker::gatherAdditiveBase` at all** -- it must be downstream, in
however `MapState::gatherOpen` (or whatever consumes `addbase` after
`AliasChecker::gather()` returns -- possibly `gatherOpen`'s own
`minItems`/open-range seeding logic, or `RangeHint::merge`/`attemptJoin`)
decides to seed/merge an open array range, since that decision is clearly
not gated purely on how many AddBase terminals exist.

This matches the caution already flagged in the "Diagnostic measured"
section far above this one: `gatherOpen`'s own logic (varmap.cc:1355-1378,
the ORIGINAL diagnostic site, not `gatherAdditiveBase`) was never actually
instrumented to show *why* it decides to seed an open `auStack_` array from
whatever AddBase entries it receives -- only WHAT AddBase entries it
receives. That is the real gap.

## Next steps (supersedes the "Next steps" section immediately above)

1. STOP further experimentation inside `AliasChecker::gatherAdditiveBase` --
   two independent gate fixes there, producing two different real AddBase-
   count reductions (4->3 via two different mechanisms), have had zero
   effect on the actual grouping. This angle is exhausted for now.
2. Move the investigation to `MapState::gatherOpen` itself (varmap.cc, the
   ORIGINAL diagnostic site from the very first section of this file) --
   specifically the code between where it receives `addbase`/`alias` and
   where it actually calls whatever seeds/creates the open array range
   (likely something calling into `RangeHint`, possibly
   `RangeHint::merge`/`attemptJoin` as speculated earlier in this file).
   Instrument THAT decision point directly: does it key off `addbase.size()`,
   off the specific offsets/sizes present, off `alias[]` entries, or off
   something else entirely (e.g. unrelated logic in a different
   decompiler pass that runs on `sat_add_u16` regardless of what
   `AliasChecker` reports)?
3. Given AddBase composition has now been changed twice with no effect,
   seriously consider testing whether the `auStack_` grouping is even
   caused by anything AliasChecker-related, versus being driven by a
   completely separate mechanism (e.g. Symbol/HighVariable merging in a
   later decompiler action, unrelated to `varmap.cc` at all). A fast,
   cheap sanity check: temporarily stub `gatherOpen` to always seed
   nothing (or use a trivially large/absurd `minItems` threshold) and see
   if `sat_add_u16`'s grouping disappears at all -- if it doesn't, the
   mechanism is confirmed to be elsewhere entirely and this whole
   `varmap.cc` investigation branch should be abandoned in favor of
   tracing a different decompiler action/pass.
4. Still explain the blank-vs-`undefined4`/`short` `baseType` inconsistency
   on the `0xe`/`0x15c00` entries across passes -- persists unchanged
   through both experiments, still not investigated.
5. All carry-over items unchanged: separate patch from #9541, don't
   conflate with the pre-existing `b`-parameter lookup failure, remove ALL
   THREE diagnostic blocks (`gatherOpen`, `gatherAdditiveBase`,
   `gateShape_diag.txt`'s block) AND the two experimental SEGMENTOP gate
   changes in `varmap.cc` once the real fix is known -- none of the
   current `gatherAdditiveBase` changes should be assumed to be part of
   the eventual fix; they may need reverting entirely once the true
   mechanism is found downstream.

## Ground truth confirmed from saved debug.xml (2026-08-26, same session)

User supplied the saved `sat_add_u16` debug.xml directly. Confirms and adds
precision to facts this file had previously only inferred indirectly:

- **`a` is locked at stack offset `0x4`, `b` is locked at stack offset
  `0x6`** (both typelocked/namelocked `ushort`, size 2) -- NOT `0x6`/`0x8`
  as the disassembly's `@(0x6,FP)`/`@(0x8,FP)` operands might suggest at a
  glance. The 2-byte gap between the FP-relative displacement in the
  disassembly and the locked stack-symbol offset is consistent with `link
  FP` itself consuming 2 bytes of stack (the saved FP), i.e. the
  disassembly's FP-relative frame and the locked Symbol's stack-relative
  frame are offset from each other by exactly 2 bytes. Worth keeping this
  distinction explicit going forward so the two numbering schemes don't get
  conflated.
- **`extrapop="6"` confirmed directly** in the saved `<prototype>` element,
  exactly as this file's already-ruled-out Section 7 theory described. No
  new information here beyond confirming the number was read correctly the
  first time.
- `result` is NOT a stack local at all -- it's stored at **`register
  offset=0x0 size=2`** (the return-value register), valid only in range
  `0x14004-0x14004`. Not previously called out explicitly in this file.
- `readonly="on"` and full symbol/type locking throughout confirm this XML
  is a frozen reference snapshot for comparison purposes, not something to
  re-derive or treat as editable.

No action item changes as a result of this -- it's ground-truth confirmation
filed for reference, not a new lead.

## Experiment 3 (decisive): gatherOpen seeding ANYTHING for this function
## causes the grouping -- confirms the mechanism, narrows the remaining gap
## (2026-08-26, same session)

Per item 3 of the "Experiment 2" next-steps, ran the sanity check directly:
temporarily made `gatherOpen`'s per-AddBase loop `continue` (skip
`addRange(...)` entirely) whenever `fd.getName() == "sat_add_u16"`, for
every entry in `addbase`, regardless of composition. Explicitly marked
TEMPORARY in the source comment -- this is a diagnostic probe, not a
candidate fix, since it would suppress ALL open-array seeding for the
function including any entries that might legitimately need it.

**Rebuilt, force-decompiled `sat_add_u16` once. Result: the `auStack_`**
**grouping is COMPLETELY GONE:**
```
ushort __stdcall_far_allstack sat_add_u16(ushort a,ushort b)
{
  ushort result;
  result = a + *(ushort *)((int)(SP - 2) + 8);
  if (CARRY2(a,*(ushort *)((int)(SP - 2) + 8))) {
    result = 0xffff;
  }
  return result;
}
```
No array Symbol at all -- falls back to raw `SP - 2` arithmetic instead of
any stack-local Symbol reference.

**This is decisive and requires correcting Experiment 2's framing.**
Experiment 2 concluded the root cause was "likely NOT in
`AliasChecker::gatherAdditiveBase` at all" and pointed at `gatherOpen`
possibly having independent logic unrelated to what `AliasChecker` reports.
That was based on an incomplete read: a direct read of `gatherOpen`'s
current body (varmap.cc:1483-1509) shows it has **no additional filtering
logic of its own** -- every single entry in `addbase` unconditionally
becomes an `addRange(offset,ct,0,RangeHint::open,minItems)` call, with no
per-entry gate in between. So `gatherOpen` is not an independent mechanism
with its own hidden logic; it is a pure pass-through from `AliasChecker`'s
AddBase list to `RangeHint` seeding. This experiment confirms that pass-
through IS the trigger (seeding nothing -> no grouping at all) -- which
means the correct fix genuinely does belong in `AliasChecker`, preventing
the wrong Varnodes from ever becoming AddBase terminals in the first
place, same direction as Experiments 1-2, just incomplete so far.

**Cross-checked against the latest `gatherAdditiveBase_diag.txt` capture**
(collected in the same build as this experiment, before the gatherOpen
suppression was added -- i.e. reflects the state AFTER both Experiment 1's
and Experiment 2's gates): the chain `descend from vn addr=0xe -> op
SEGMENTOP at 0x14004` (matching the gate's own self-referential shape
check: SEGMENTOP consuming `0xe` directly, defined by `INT_ADD` against a
constant with base address `0xe` == own address) is STILL immediately
followed by `descend from vn addr=0x11000 -> op INT_ADD at 0x14004`, i.e.
**this SEGMENTOP is NOT being excluded by the existing gate, even though it
appears to match the gate's own self-referential condition on paper.**
This is a real, unresolved discrepancy between what the gate's logic should
do and what the diagnostic shows it actually doing -- the walk continues
past this SEGMENTOP into `0x11000 -> 0x15c00`, which is exactly the
untouched terminal Experiment 1 already flagged as the prime remaining
suspect. Whether this is because `nonadduse=true` doesn't actually stop
`0x11000` from being reached via a DIFFERENT, unblocked path (a second
producer of the same Varnode), or because the gate's shape-check is subtly
not matching this specific case despite appearing to on inspection, has NOT
been determined and needs direct instrumentation of the gate's own
`gateOut` boolean at this exact call site (not just the shape inputs, which
`gateShape_diag.txt` already covers) to resolve.

## Next steps (supersedes the "Next steps" section immediately above)

1. **The `gatherOpen`-is-a-red-herring framing from Experiment 2 is now**
   **corrected -- return focus to `AliasChecker::gatherAdditiveBase` and**
   **specifically the `0xe -> SEGMENTOP @14004 -> 0x11000 -> 0x15c00`**
   **chain**, which by all appearances should already be caught by the
   existing self-referential gate (confirmed via `gateShape_diag.txt`'s
   entries 4-7 showing this exact SEGMENTOP's def as `INT_ADD` against a
   constant with `in0-addr=0xe` matching its own consumed Varnode's
   address) but demonstrably is not being stopped.
2. Add a diagnostic directly at the `gateOut` decision point in the
   SEGMENTOP case (not just the shape dump `gateShape_diag.txt` already
   has) that logs the actual boolean result AND, critically, whether
   `nonadduse=true` for this iteration actually prevented `subvn`
   (`op->getOut()`) from being independently reachable via a different
   queued entry -- i.e. check whether the SAME `0x11000` Varnode has more
   than one producing op/is reached from more than one path in the queue,
   which would explain how it survives even when this specific SEGMENTOP
   is correctly gated.
3. Once `0x11000`/`0x15c00`'s true surviving path is identified and
   closed, re-run the full grouping test (not the seed-nothing sanity
   check) to confirm the grouping actually disappears with `gatherOpen`
   restored to normal (i.e. revert the temporary "skip everything" probe
   from this experiment once a real, scoped `AliasChecker`-level fix
   exists -- the seed-nothing version must never be the shipped fix, since
   it would break legitimate open-array cases in other functions).
4. All carry-over items unchanged and now more urgent given how close this
   is: separate patch from #9541, don't conflate with the pre-existing
   `b`-parameter lookup failure, remove ALL diagnostic blocks (`gatherOpen`
   dump, `gatherAdditiveBase` dump, `gateShape_diag.txt`, AND this
   experiment's temporary seed-nothing probe) plus the two experimental
   SEGMENTOP gate changes, replacing them with whichever final,
   properly-scoped fix closes the `0x11000`/`0x15c00` gap, once known.
5. Still unexplained and unblocking: the blank-vs-`undefined4`/`short`
   `baseType` inconsistency noted several sections back.

## Experiment 4: user's own fixes (operand-order + PTRSUB/"CROSSBUILD"
## alias) close most but not all of the 0x11000/0x15c00 gap
## (2026-08-26, same session, picked up mid-session by the user directly)

User made two further fixes to the SEGMENTOP gate directly (not fully
narrated turn-by-turn in this file, reconstructed from reading the
resulting source):
1. **Operand-order fix** in the `INT_ADD`/`INT_SUB` branch: the constant
   operand isn't guaranteed to be in `getIn(1)` -- this SLEIGH's
   addressing-mode pcode can put it in either slot. Now checks both slots
   before concluding "not a constant-offset case."
2. **PTRSUB/"CROSSBUILD" alias fix**: `gateShape_diag.txt` had shown one
   surviving base (`addr=0x21b00` region) was defined by an op that
   `get_opname()` prints as `"CROSSBUILD"`. An earlier attempt wrongly
   treated this as a literal `CPUI_CROSSBUILD` enum value -- **does not
   exist, build failed with C2065** (same class of mistake as the
   CPUI_LABEL one below). Per `opcodes.cc`'s own doc comment ("PTRSUB =
   CROSSBUILD"), `"CROSSBUILD"` is `get_opname()`'s display-alias for
   `CPUI_PTRSUB` in this table. Corrected to check `CPUI_PTRSUB` and gate
   it the same way as the other print-time-plumbing cases.

Confirmed via `gateResult_diag.txt` (space-annotated version) after these
fixes: `gateOut=true` now on nearly every SEGMENTOP occurrence for
`sat_add_u16`, including most `def-op=LABEL` ones (see below for what that
alias resolves to) -- a real, large improvement over Experiment 3's state.
**But two specific occurrences still slip through with `gateOut=false`:
`vn addr=0xe` and `vn addr=0x10000002` (both `space=unique` for the
latter), both pushing `subvn addr=0x11000 space=unique`.**

## CPUI_LABEL does not exist -- "LABEL" is CPUI_PTRADD's display alias
## (build failure + correction, 2026-08-26, same session)

Diagnosed the two remaining `gateOut=false` cases via `gateResult_diag.txt`:
both have `def-op=LABEL`. First attempt added a `case`/`if` branch checking
literal `CPUI_LABEL` -- **does not exist, build failed with C2065:
undeclared identifier.** Checked `opcodes.cc`'s own doc comment directly
(same file/pattern that resolved the CROSSBUILD confusion): **`"LABEL"` is
`get_opname()`'s display-alias for `CPUI_PTRADD`**, not a real opcode.
Corrected the branch to check `segInOpc == CPUI_PTRADD` (note: this is
`segInDef->code()`, the op DEFINING the SEGMENTOP's consumed Varnode -- NOT
the same as the outer switch's own pre-existing `case CPUI_PTRADD:`, which
is `op->code()`, the currently-visited op; same enum name, different
Varnode/op being tested, do not conflate them).

**Rebuilt with the corrected `CPUI_PTRADD` branch. Result: STILL NOT FULLY
FIXED.** `sat_add_u16` decompile output changed shape again (from
`ushort auStack_102[128]` / `(auStack_82+0x80)[4]`-style expression to
`undefined1 auStack_82[128]` with `*(ushort*)(...)` pointer-cast syntax
again -- offset also shifted `0x102`->`0x82`, type degraded `ushort`->
`undefined1`) but **the grouping is still present.** Re-checked
`gateResult_diag.txt`: the `CPUI_PTRADD` branch DOES now correctly gate
most `def-op=LABEL` occurrences (`gateOut=true`), but **the same two
specific occurrences (`vn addr=0xe`, `vn addr=0x10000002`, both pushing
`subvn addr=0x11000`) still show `gateOut=false` despite an apparently
identical `def-op=LABEL`.** File size/timestamp on `gateResult_diag.txt`
checked directly (40 lines, single write timestamp) to rule out a stale-log
artifact; user confirmed this was a from-scratch full rebuild, not a
binary-swap-without-restart. This is a genuine, reproducible discrepancy,
not a build/deployment mistake.

## Current diagnostic in flight -- not yet run (2026-08-26, end of session)

Added a new diagnostic (`gateOpInputs_diag.txt`) to resolve the above
discrepancy, on the theory that `vn`/`segIn` might not be the operand whose
`getDef()` actually matters for this specific SEGMENTOP call site (i.e.
this SEGMENTOP may have a different `numInput()`/operand-slot layout than
the ones that gate correctly). Placed right after `segInDef` is computed
(same `if (startvn->getOffset() == (uintb)0xe)` gate as the other
diagnostics -- same known caveat: leaks entries from any other function in
the ROM whose spacebase also happens to land at 0xe, must be filtered/
deleted after use). For every SEGMENTOP visited, dumps `op->numInput()` and
EVERY input's address + defining-op name (not just the one input this
session has been calling `segIn`/`vn`), flagging which input index (if any)
matches `segIn`.

**This diagnostic has been added to source but NOT YET BUILT OR RUN.**
This is the concrete next action for a fresh session/chat to pick up.

## Status summary for continuation in a new chat

**Where things stand:** the `AliasChecker::gatherAdditiveBase` SEGMENTOP
gate (varmap.cc, inside `case CPUI_SEGMENTOP:`) now correctly excludes
5 of what were originally ~7-8 distinct SEGMENTOP call-site shapes seen
across this investigation: self-referential/cross-register `INT_ADD`/
`INT_SUB`-against-constant (either operand order), bare `CPUI_COPY` of a
processor-space register, `CPUI_PTRSUB` (display-aliased "CROSSBUILD"),
and `CPUI_PTRADD` (display-aliased "LABEL") -- covering `0xc`, `0x21b00`,
and most `0xe`/`0x10000002` occurrences. **Two specific occurrences of what
appears to be the exact same `CPUI_PTRADD`("LABEL") shape still resolve
`gateOut=false`** and are the sole remaining known path by which `0x11000`
(and downstream `0x15c00`) survive into the AddBase list, which Experiment
3 already proved decisively is sufficient on its own to cause the
`auStack_` grouping (seeding zero AddBase entries for this function makes
the grouping vanish completely).

**Immediate next action:** rebuild with the `gateOpInputs_diag.txt`
diagnostic already in place (see section above), run `force_decompile` on
`sat_add_u16` (address `00014000` in
`RVR_1998_x3 4g63t 21000011 md352553.hex`) once, then read
`C:\Users\j.brophy.CORKILLSYSTEMS\gateOpInputs_diag.txt` directly (e.g. via
`run-command` -> `Get-Content -TotalCount 40`, same pattern used
successfully earlier this session) to see whether the two failing
`0xe`/`0x10000002` SEGMENTOP occurrences have a different `numInput()` or
operand-slot layout than the ones that already gate correctly, which
would explain why `segIn->getDef()` isn't the right thing being checked
for this specific call-site shape.

**Known caveats to keep in mind:**
- `get_opname()`'s display strings are NOT 1:1 with opcode identity --
  confirmed twice now (`"CROSSBUILD"`=`CPUI_PTRSUB`, `"LABEL"`=
  `CPUI_PTRADD`, per `opcodes.cc`'s own doc comment listing all such
  aliases: `MULTIEQUAL=BUILD`, `INDIRECT=DELAY_SLOT`, `PTRADD=LABEL`,
  `PTRSUB=CROSSBUILD`). Before adding any new gate branch based on a
  diagnostic's `def-op=` string, check `opcodes.cc`'s doc comment FIRST
  for whether that string is an alias, rather than assuming a literal
  `CPUI_<STRING>` enum exists -- this has caused two separate build
  failures (C2065) this session alone.
- All diagnostic file paths in this investigation are gated on
  `startvn->getOffset() == (uintb)0xe` because `AliasChecker::
  gatherAdditiveBase` is `static` (no `fd` member access) -- this leaks
  entries from any other function in the whole ROM whose spacebase also
  happens to land at offset `0xe`. Read diagnostic files with this in mind
  (the `sat_add_u16` pass is reliably the first ~10-40 lines per rebuild
  cycle in this session's experience, but this is not guaranteed).
- `gatherOpen_diag.txt`, `gatherAdditiveBase_diag.txt`, and
  `gateShape_diag.txt` were deleted by the user partway through this
  session (per explicit request) and are no longer being written by
  current source (the `gateShape_diag.txt`-writing block was removed and
  replaced by `gateOpInputs_diag.txt` in the "Current diagnostic in
  flight" section above). Only `gateResult_diag.txt` and the new
  `gateOpInputs_diag.txt` are live diagnostics as of this session's end.
- The Experiment 3 `gatherOpen`-seed-nothing probe (temporary, gated to
  `fd.getName() == "sat_add_u16"`) was added AND already reverted within
  this session -- confirmed removed, do not re-add it as a candidate fix.
- Total diagnostic/experimental code currently live in `varmap.cc` that
  will need cleanup once the real fix is found: the `<fstream>` include:
  the `gatherAdditiveBase_diag.txt`-writing block in the descend-loop (the
  `if (startvn->getOffset()==0xe) {...}` right after `op = *iter;`, PRE-
  DATES this session's SEGMENTOP-gate work, may or may not still be
  present -- verify at line ~836-848 in a fresh read since these line
  numbers have drifted repeatedly this session as diagnostics were
  added/removed); the entire `CPUI_SEGMENTOP` case body's gate logic AND
  its `gateOpInputs_diag.txt`/`gateResult_diag.txt` diagnostic writes
  (currently spans roughly varmap.cc:887-1050+, verify exact range in a
  fresh read -- line numbers are not stable references in this file
  anymore, always re-grep for anchoring comments like "FIX 2026-08-26" or
  "DIAGNOSTIC 2026-08-26" before editing). None of this diagnostic/gate
  code should be assumed to be the final shape of the real fix -- it may
  need significant simplification once `0x11000`'s true surviving path is
  fully understood.
- Ground truth from saved debug.xml (see section above): `a`/`b` locked at
  stack offset `0x4`/`0x6` (NOT `0x6`/`0x8`, that's the FP-relative
  disassembly frame, offset by 2 for the saved FP); `extrapop=6` confirmed
  (already ruled out as a direct cause); `result` lives in a register, not
  on the stack.
- This entire investigation is OUT OF SCOPE for PR #9541 (already-landed,
  confirmed-complete register0x0e/SEGMENTOP annotation fix, commit
  aadd7ee) and must land as its own separate, independent patch once
  resolved, per review16.md's own established practice.

## MAJOR PIVOT: SLEIGH source + datasheet confirm the SEGMENTOP chain is
## LEGITIMATE effective-address computation, not noise -- the gating
## approach was built on a mistaken premise (2026-08-27, new session)

Per user's suggestion, read the actual SLEIGH source
(`h8539f.slaspec`/`h8539f-mem.sinc`) and the original H8/500-series
disassembler (`source\ida-sdk-main\src\module\h8500\ana.cpp`) plus the
Hitachi H8/500 Programming Manual (grepable text at
`datasheets\grep able datasheets\H8_500_programming.md`), instead of
continuing to infer the addressing-mode semantics from pcode-shape
diagnostics alone.

**Found the exact constructor `sat_add_u16`'s `@(0x6:8,FP)`/`@(0x8:8,FP)`**
**operands go through**, at `h8539f.slaspec:1341-1348`:
```
r6b_disp8: "@("^disp8^":8","FP"^")" is disp8 {
	local ptr24:4 = spSegment(TP, FP + disp8);
	export *:1 ptr24;
}
r6w_disp8: "@("^disp8^":8","FP"^")" is disp8 {
	local ptr24:4 = spSegment(TP, FP + disp8);
	export *:2 ptr24;
}
```
This is a dedicated, `FP`-only constructor (distinct from the generic
`Rn_banked2`/`eaw_disp8_2` family also present in the file) that computes
`FP + disp8` **inline as the SEGMENTOP's own argument expression** --
exactly matching the `SEGMENTOP` consuming an `INT_ADD`-against-constant
Varnode shape every diagnostic this session has been observing and trying
to gate out.

**Cross-checked against the H8/500 Programming Manual (authoritative
datasheet, not just the SLEIGH port's own comments):**
- Section on page registers, "(6) Stack Page Register (TP)": *"The stack
  page register combines with R6 (Frame pointer) or R7 (Stack pointer) to
  generate a 24-bit stack address... It is used to calculate effective
  addresses in the register indirect addressing mode using R6 or R7."*
- Note 2 under Table 1-10 (Effective Address Calculation): confirms the
  page-register pairing explicitly -- *"DP for register-indirect addressing
  with R0, R1, R2, or R3... EP for register-indirect addressing with R4 or
  R5... TP for register indirect addressing with R6 or R7."*
- Table 1-10 itself shows addressing mode #3 (`@(d:8,Rn)`, disp8-relative --
  exactly the mode `sat_add_u16` uses) always combines the page register
  with the displaced register value to form the real 24-bit effective
  address, for every register class uniformly.
- Cross-checked `ana.cpp`'s `ds8`/`ds16` (h8500/ana.cpp:219-227) too: the
  original disassembler treats `@(disp,Rn)` as one uniform `o_displ`
  operand type at the decode level, `x.reg = reg & 7` with no per-register
  special-casing visible there -- meaning the DP/EP/TP page-register split
  is a documented *semantic* fact from the manual, applied correctly by
  this SLEIGH port, not something `ana.cpp`'s bare operand decode would
  show structurally on its own. The two sources are consistent, not
  contradictory: `ana.cpp` decodes the operand shape, the manual supplies
  the addressing semantics the SLEIGH port correctly encodes.

**Conclusion: `spSegment(TP, FP + disp8)` is a real, datasheet-accurate,**
**hardware-correct 24-bit effective-address computation for `sat_add_u16`'s**
**stack-local access -- not print-time-only plumbing, not an artifact to be**
**excluded.** This directly undercuts the premise of every gating
experiment this session (Experiments 1 and 2, and the corrected
CPUI_PTRADD/CPUI_PTRSUB/CPUI_COPY branches built on top of them): treating
this SEGMENTOP as a "non-additive terminal" to stop `AliasChecker`'s walk
was excluding a *legitimate* pointer computation from consideration, not
filtering out noise. That the decompile output partially changed in
response to those gates (`auStack_102`->`auStack_82`, type changes,
`AddBase` count reductions) was a side-effect of breaking a real part of
the address-tracking, not evidence of correctness.

**This does not mean the SEGMENTOP-exclusion gates had zero diagnostic**
**value** -- Experiment 3's cleaner, decisive test (temporarily suppressing
ALL of `gatherOpen`'s `addRange` calls for this function) remains valid and
conclusively proved the grouping mechanism lives in
`gatherOpen`/`RangeHint`'s consumption of the `AddBase` list, not in some
unrelated decompiler pass. That finding stands. What changes is the
interpretation of the SEGMENTOP walk itself: it should NOT be excluded from
`AliasChecker`, since it's the real address computation. The bug is more
likely in how `RangeHint::merge`/`attemptJoin`/`gatherOpen`'s
`minItems`-seeding logic interprets or groups the *correct* SEGMENTOP-
derived addresses into an oversized open array, not in which Varnodes reach
`AddBase` in the first place.

## Next steps (supersedes every prior "Next steps" section -- start here)

1. **Revert the experimental `CPUI_SEGMENTOP` gating in**
   **`AliasChecker::gatherAdditiveBase`** (varmap.cc, currently spans
   roughly the `case CPUI_SEGMENTOP:` block -- re-grep for the "FIX
   2026-08-26"/"DIAGNOSTIC 2026-08-26" comment markers left throughout this
   session rather than trusting any specific line range, since line numbers
   have drifted repeatedly). Restore it to unconditionally chain through
   SEGMENTOP the same way `CPUI_PTRSUB` does (i.e. back to the very first,
   pre-Experiment-1 state: `case CPUI_PTRSUB: case CPUI_SEGMENTOP:` sharing
   one unconditional push, no gate). This premise is no longer believed
   correct and should not be built on further.
2. Remove all now-obsolete diagnostics tied to the reverted gate
   (`gateUnified_diag.txt`'s writing block and the `bool gateOut` scaffold
   around it). The `gatherAdditiveBase_diag.txt`-equivalent op-trace
   diagnostic (if still present near the top of the descend-loop, gated on
   `startvn->getOffset()==0xe`) can stay if useful for the next step, or be
   removed too -- no longer the active investigation site either way.
3. **New investigation target: `RangeHint::merge()` / `attemptJoin()` /**
   **`absorb()`** (varmap.cc, roughly lines 62-373 per the much-earlier
   "Actual grouping mechanism, located" section of this file -- re-grep to
   confirm current line numbers) **and/or `MapState::gatherOpen`'s**
   **`minItems`/seeding logic itself** (varmap.cc, the `addRange(offset,ct,
   0,RangeHint::open,minItems)` call). With the legitimate SEGMENTOP-derived
   addresses restored and flowing normally, instrument THIS layer directly:
   log each `RangeHint`'s offset/size/type immediately before and after
   `merge()`/`attemptJoin()` is called on it for `sat_add_u16`, to see
   exactly which two (or more) legitimate, correct address computations are
   being merged into one oversized array, and why `attemptJoin`'s
   stride/alignment logic considers them joinable when they should not be.
4. Given the confirmed real addresses now understood (`0xe`=FP itself,
   `0xc`=some other register feeding a second SEGMENTOP, `0x15c00`/`0x21d00`-
   style terminals = real `TP`-banked 24-bit pointers), a useful cross-check
   before instrumenting further: manually work out what stack offset/size
   each of the 3-4 legitimate `AddBase` entries for `sat_add_u16` SHOULD
   correspond to (given the function only has two real locals -- the `a`/`b`
   parameters at offsets 0x4/0x6 per the confirmed debug.xml ground truth
   several sections above) and see if the grouping decision is conflating
   parameter-space accesses with local-stack-space accesses, or
   double-counting the same real access through two different SSA/pcode
   paths (which the `gatherAdditiveBase_diag.txt` traces earlier this
   session already hinted at -- e.g. `0xe9300`/`0x21d00`/`0x15c00` all
   appearing as separate terminals for what may functionally be the same
   two real memory accesses, reached via different pcode chains).
5. All standing carry-over items unchanged: separate patch from #9541,
   don't conflate with the pre-existing `b`-parameter lookup failure, and
   this entire thread remains its own independent patch target.

## Full revert executed, clean baseline reconfirmed, new investigation
## started at RangeHint::merge (2026-08-27, same session)

Per the pivot above, reverted `varmap.cc` completely via `git checkout --`
(user had a backup of the pre-revert state first) rather than manually
reconstructing the original text -- safer given how much the file had
drifted across this session's many diagnostic/gate edits. Verified clean:
`case CPUI_PTRSUB: case CPUI_SEGMENTOP:` back to one unconditional shared
push (varmap.cc:866-867 post-revert), zero grep hits for
`diag|DIAGNOSTIC|review17` anywhere in the file, `#include <fstream>`
removed. Rebuilt, reinstalled, force-decompiled `sat_add_u16` once:
**output matches the very first capture from the start of this
investigation exactly** -- `ushort auStack_102[128]`,
`(int)(auStack_102 + 0x80) + 8` pointer-cast syntax. Confirmed clean
baseline before starting the new investigation.

**New instrumentation added at the actual suspected mechanism:**
`RangeHint::merge()`'s `resType==2` ("confuse") branch (varmap.cc, now
~line 357 post-revert -- re-grep `RangeHint::merge` to confirm current
line number, this file's numbering has drifted repeatedly all session).
This is the branch reached when two overlapping `RangeHint`s can't be
reconciled and neither is type-locked: it computes
`diff = b->sstart - sstart`, grows `size` to cover the union of both
ranges, and **if that combined size isn't a clean 1/2/4/8, falls back to
`size=1` and `rangeType=open`** -- which is exactly the "byte-array of
unknown length" shape that prints as `auStack_NNN[128]`. This is now the
leading hypothesis: `sat_add_u16`'s two real, legitimate, SEGMENTOP-derived
parameter accesses (`a`/`b`, both real size-2 `ushort` per the confirmed
debug.xml ground truth several sections above) are reaching this branch,
their combined span isn't landing on a clean power-of-two, and `merge()`
gives up and produces one oversized `open` array covering both instead of
keeping them as two separate size-2 locals.

Diagnostic (`mergeConfuse_diag.txt`) logs `start`/`sstart`/`size` for both
`this` and `b` on entry to the branch, plus the resolved `diff`/final
`size`/`rangeType` after the branch's own logic runs. Gated on
`sstart`/`b->sstart` both being in `[0, 0x20)` -- `RangeHint::merge` has no
`Funcdata` reference available to gate on function name directly (same
limitation `AliasChecker::gatherAdditiveBase` had), so this uses a
stack-offset heuristic instead, chosen to bracket `sat_add_u16`'s known
real locals (offsets ~0x4-0x8) without repeating the 379k-line whole-ROM
leak the raw-address gates produced earlier this investigation. Not a
perfect filter -- other small-stack-frame functions could also pass through
occasionally -- but should be manageable to eyeball/filter manually if so.

**Not yet built or run this session** -- next action for continuation is:
rebuild, reinstall, `force_decompile` on `sat_add_u16` (address `00014000`
in `RVR_1998_x3 4g63t 21000011 md352553.hex`) once, then read
`C:\Users\j.brophy.CORKILLSYSTEMS\mergeConfuse_diag.txt` directly (e.g. via
`run-command` -> `Get-Content -TotalCount 40`) to see the actual
`start`/`size` values colliding and confirm or refute the hypothesis
above.

## Status summary for continuation (supersedes the "Status summary" section
## from earlier this file -- that one is now historical/superseded by the
## pivot above)

**Where things stand:** the SEGMENTOP-gating line of investigation
(Experiments 1-3, the `AliasChecker::gatherAdditiveBase` gates) is
CLOSED/ABANDONED -- confirmed via SLEIGH source (`h8539f.slaspec:1341-1348`,
the `r6b_disp8`/`r6w_disp8` constructors) and the Hitachi H8/500
Programming Manual (datasheet) that the SEGMENTOP/`spSegment(TP,FP+disp8)`
chain is legitimate, datasheet-correct effective-address computation, not
noise to exclude. All of that experimental code has been fully reverted via
`git checkout --` and a clean baseline reconfirmed. **Do not re-open the
SEGMENTOP-gating angle.**

**Current investigation target:** `RangeHint::merge()`'s `resType==2`
branch (varmap.cc, ~line 357 as of this session -- always re-grep
`RangeHint::merge` to confirm, do not trust cached line numbers from this
file). Diagnostic code is in place but NOT YET BUILT/RUN. Hypothesis:
combining `sat_add_u16`'s two real parameter accesses produces a
non-power-of-two combined size, triggering the `size=1`/`rangeType=open`
fallback that manifests as `auStack_NNN[128]`.

**Immediate next action:** rebuild, reinstall, `force_decompile` on
`sat_add_u16` once, read `mergeConfuse_diag.txt`.

**Known caveats carried forward:**
- `get_opname()`'s display strings are not 1:1 with opcode identity
  (`PTRADD`="LABEL", `PTRSUB`="CROSSBUILD", `MULTIEQUAL`="BUILD",
  `INDIRECT`="DELAY_SLOT" per `opcodes.cc`'s own doc comment) -- if this
  comes up again for any reason, check the doc comment before assuming a
  literal `CPUI_<STRING>` enum exists.
- Functions in the varmap.cc `RangeHint`/`AliasChecker` layer generally
  lack a `Funcdata` reference to gate diagnostics on function name
  directly -- stack-offset or Varnode-address heuristics are the fallback,
  and all such gates leak somewhat across functions sharing similar
  addresses/offsets. Always check output volume (`Get-Item ... Length`)
  before assuming a diagnostic capture is scoped to just the function of
  interest.
- Ground truth from saved debug.xml (see much-earlier section): `a`/`b`
  locked at stack offset `0x4`/`0x6` (word/`ushort`, size 2 each); FP-
  relative disassembly frame (`@(0x6,FP)`/`@(0x8,FP)`) is offset by +2 from
  the stack-symbol frame due to `link FP` consuming 2 bytes for the saved
  FP; `extrapop=6` confirmed but ruled out as a direct cause; `result`
  lives in a register, not on the stack.
- This entire investigation is OUT OF SCOPE for PR #9541 (already-landed,
  confirmed-complete register0x0e/SEGMENTOP annotation fix, commit
  aadd7ee) and must land as its own separate, independent patch once
  resolved.


---

## Update 2026-08-27 (session continued): gatherOpen/AliasChecker dead ends, real signal found in gatherAdditiveBase

### Summary of the two dead ends first

1. **RangeHint::merge() resType==2 "confuse" branch** � instrumented, rebuilt,
   ran force_decompile on sat_add_u16. `mergeConfuse_diag.txt` was NEVER
   created. Confirmed via targeted filesystem search (recursive under the
   user profile) that the file does not exist anywhere. This branch never
   fires for sat_add_u16 at the offsets in question. **Hypothesis falsified.**

2. **MapState::gatherOpen() / addRange() seeding** � instrumented with a
   gate on offset<0x20. `gatherOpen_diag.txt` showed only ONE addRange call
   for the whole function:
   ```
   offset=0x4 ctSize=-1 hasIndex=0 minItems=-1
   ```
   (appeared twice, once per force_decompile run since instrumenting).
   Only offset 0x4 (a's slot) appears � offset 0x6/0x8 (b's slot) never
   shows up as its own addRange call. ctSize=-1 means ct was null (unknown
   array element type). hasIndex=0/minItems=-1 means AliasChecker did NOT
   classify this as an indexed access. **This ruled out the "two open
   ranges collide and merge" theory** � there's only ever one open range
   seeded, so nothing is colliding; b's slot is simply never surfaced as
   its own alias base at this layer.

### Root-cause tracing moved to AliasChecker::gatherAdditiveBase()

Given (1) and (2), the investigation moved one layer deeper/upstream, into
`AliasChecker::gatherAdditiveBase()` � the BFS that walks all additive
pcode ops (INT_ADD/INT_SUB/PTRADD/PTRSUB/SEGMENTOP/COPY) from the function's
single spacebase input Varnode, and produces the AddBase entries that
gatherOpen() later turns into addRange() calls.

Instrumented with three log points (all writing to
`gatherAdditiveBase_diag.txt`):
- BFS entry: startvn address
- Per-queue-pop ("visit"): the varnode address/size being expanded
- Per-descendant-op: the opcode name and address of every op examined

**Build note:** the first two attempts at this instrumentation corrupted
varmap.cc due to stale/drifting line-number anchors in the pulsar
str_replace tool (positional afterLine inserts landing in the wrong place
after earlier edits shifted the file, once splicing new code into the
middle of the function's doc comment, and once creating a duplicate
diagOut4 block causing a redefinition compile error). Recovered by reading
the live file directly via pulsar run-command/PowerShell, diffing against
the known-correct structure, and doing surgical, sanity-checked
line-indexed replacements (each script asserts the exact expected content
at the target index before touching anything, and aborts otherwise). Final
state verified clean by reading the entire function back before rebuilding.
**Lesson for future sessions: prefer whole-function-body replacement or
PowerShell-with-assertions over incremental positional str_replace once a
file has already been edited once in the same session � anchors drift.**

### The actual finding

After a clean rebuild, force_decompile on sat_add_u16, and reading
gatherAdditiveBase_diag.txt, the freshest trace (one full BFS run) is:

```
--- gatherAdditiveBase start, startvn addr=0xe ---
  visit vn addr=0xe size=2
    descend op=CROSSBUILD addr=0x14000
  visit vn addr=0x10000002 size=2
    descend op=LABEL addr=0x14000
  visit vn addr=0xe size=2
    descend op=SEGMENTOP addr=0x14004
  visit vn addr=0x11000 size=4
    descend op=INT_ADD addr=0x14004
  visit vn addr=0x15c00 size=4
    descend op=LOAD addr=0x14004
```

Key observations:
- There is exactly **one spacebase input** (addr=0xe), as expected � one
  BFS call per decompile, confirmed across ~14 cumulative log entries (this
  file appends across runs; earlier entries in the full log are from
  repeated force_decompile calls this session, not multiple concurrent
  BFS starts).
- The BFS visits `CROSSBUILD` and `LABEL` ops. Neither is in
  gatherAdditiveBase's explicit switch cases (COPY/INT_SUB/INT_ADD/
  PTRADD/PTRSUB/SEGMENTOP) � both fall through to `default:
  nonadduse=true`, meaning **the BFS does not recurse into their
  descendants** (only the explicit additive-op cases push new AddBase
  queue entries).
- There is only **ONE terminal LOAD** in the entire traversal.

**Conclusion: AliasChecker/gatherAdditiveBase/gatherOpen/RangeHint::merge
are all behaving correctly given their input.** The bug is NOT in any of
these decompiler analysis passes � they are faithfully reporting that the
pcode they were handed already contains only a single combined address
computation (one SEGMENTOP -> INT_ADD -> LOAD chain) for what should be
two independent stack-slot accesses (a at Stack[0x4], b at Stack[0x6]).

This pushes the root cause further upstream, to wherever this pcode is
generated/normalized � i.e., before AliasChecker ever runs. CROSSBUILD is
a SLEIGH-specific pseudo-op used for delay-slot / cross-instruction code
generation, which was the initial reason to suspect the processor's
.sinc/.slaspec constructor pattern for this instruction's addressing mode.

**Correction per user (this session): the SLEIGH spec itself is believed
to be correct, cross-checked against ana.cpp and the vendor datasheets.**
So the working theory is narrowed: the SLEIGH constructors are producing
correct semantics per-instruction, but something in how the delay-slot /
CROSSBUILD-driven pcode gets assembled or merged for this specific
instruction pair (the two stack parameter loads) collapses them into one
LOAD before AliasChecker sees it. This is likely in Ghidra's core
pcode-emission/delay-slot-merging logic (funcdata pcode generation,
possibly around CROSSBUILD resolution / Funcdata::opInsertBefore or
similar delay-slot flattening), NOT in the H8 SLEIGH spec itself.

### Status summary (supersedes all earlier ones)

- Clean baseline confirmed and reproducible via force_decompile on
  sat_add_u16 (address 0x14000): produces the bogus
  `ushort auStack_102[128]` / pointer-cast-arithmetic output.
- RangeHint::merge() resType==2 hypothesis: FALSIFIED (instrumented,
  never fires).
- gatherOpen()/addRange() collision hypothesis: FALSIFIED (only one open
  range ever seeded; nothing collides).
- gatherAdditiveBase() BFS: instrumented and traced successfully. Shows
  the bug is NOT in AliasChecker's traversal logic � it's being handed
  pcode that has already merged a's and b's address computations into a
  single SEGMENTOP/INT_ADD/LOAD chain, with CROSSBUILD/LABEL ops (not
  followed further by the BFS) somewhere in the ancestry.
- SLEIGH spec itself: believed correct per user, cross-checked against
  ana.cpp and datasheets. Rules out ".sinc/.slaspec constructor is wrong"
  as the next step.
- **Next step: investigate Ghidra core's delay-slot / CROSSBUILD pcode
  merging (pre-AliasChecker, likely in Funcdata's pcode generation /
  delay slot flattening), NOT the H8 SLEIGH spec.** Consider instrumenting
  wherever CROSSBUILD ops get resolved/expanded during pcode generation to
  see if/where the two separate stack loads get combined into one.
- All instrumentation (mergeConfuse diag in RangeHint::merge, the
  gatherOpen_diag in MapState::gatherOpen, and the three
  gatherAdditiveBase_diag log points in AliasChecker::gatherAdditiveBase)
  is still live in the current build and safe to leave in for now, gated
  cheaply (offset<0x20 checks / always-on for the small gatherAdditiveBase
  logs) � should be stripped before this becomes a real PR.
- This entire investigation remains OUT OF SCOPE for PR #9541
  (already-landed, confirmed-complete register0x0e/SEGMENTOP annotation
  fix, commit aadd7ee) and must land as its own separate, independent
  patch once resolved.

---

## Update 2026-08-27 (session continued further): standalone offline test harness -- WORKING repro achieved

### Motivation

The edit->rebuild->force_decompile->grep-a-log cycle against full Ghidra was
too slow and error-prone for productive iteration (each cycle = full native
rebuild + Ghidra reload). Built a standalone offline decompiler test harness
instead, using Ghidra''s own built-in data-test framework (test.cc /
testfunction.cc), which runs isolated .xml repros against the real
decompiler core in-process, no Java/GUI/network round-trip, ~seconds per run.

### What was built

Added a new `decompile_test` native executable component to
`Ghidra/Features/Decompiler/buildNatives.gradle`, alongside the existing
`decompile` and `sleigh` components. Builds `ghidra_test.exe` at
`Ghidra/Features/Decompiler/build/os/win_x86_64/ghidra_test.exe`.

Source list = same core decompiler sources as `decompile`, PLUS:
- `ifacedecomp.cc`, `ifaceterm.cc`, `interface.cc` (console/interface layer,
  normally commented out in the plain `decompile` build)
- `testfunction.cc`, `test.cc` (the actual data-test framework + its main())
- `libdecomp.cc` (startDecompilerLibrary, capability registration)
- `sleigh_arch.cc` (SleighArchitecture, scanForSleighDirectories)
- `xml_arch.cc` (XML architecture capability -- REQUIRED for binaryimage
  test files; without this you get "Missing XML architecture capability")
- `loadimage_xml.cc` (LoadImageXml -- required alongside xml_arch.cc)
- `inject_sleigh.cc` (PcodeInjectLibrarySleigh)
- `pcodeparse.cc`, `grammar.cc` (needed for PcodeSnippet / console command
  parsing -- NOT the same as the SLEIGH-spec-compiler grammar, safe to
  include alone)
- zlib compiled from source (src/decompile/zlib/*.c) with LOCAL_ZLIB
  defined, same pattern as the existing sleigh component, because zlib.h
  isn''t otherwise on the include path for a fresh component

Explicitly did NOT include slgh_compile.cc / slghparse.cc / slghscan.cc --
these belong to the standalone SLEIGH-spec compiler frontend and
slgh_compile.cc has its own unconditional main() that collides with
test.cc''s main(). Runtime .sla loading does not need them.

Also had to add: a diagnostic dump in FunctionTestCollection::runTests
(testfunction.cc, right after "string result = bulkout.str();") that
unconditionally writes bulkout (== the raw print C output, i.e. what
stringmatch assertions actually scan) to
C:\Users\j.brophy.CORKILLSYSTEMS\harness_bulkout_diag.txt, appending each
run. Without this there is NO way to see the actual decompiled text --
stringmatch results only tell you pass/fail per assertion, never the
underlying content, and test.cc/testfunction.cc don''t otherwise expose it
on success.

### Invocation

cd Ghidra\Features\Decompiler\build\os\win_x86_64
.\ghidra_test.exe -sleighpath "C:\Users\j.brophy.CORKILLSYSTEMS\github\Ghidra-H8-Processor\h8\data\languages" -path "C:\Users\j.brophy.CORKILLSYSTEMS\github\ghidra\Ghidra\Features\Decompiler\src\decompile\datatests"

Notes on flags/behavior discovered the hard way:
- -sleighpath <dir> works even though the dir isn''t laid out as
  .../Ghidra/Processors/<module>/data/languages/ (the pattern
  SleighArchitecture::scanForSleighDirectories normally expects) --
  there''s a fallback (sleigh_arch.cc ~line 610) that just uses the given
  rootpath directly as the languages dir if the Ghidra/Processors
  structure isn''t found. So pointing straight at
  Ghidra-H8-Processor\h8\data\languages works fine.
- "-path <dir> datatests <basename>" (filtering to one named file) did NOT
  reliably work in this build -- consistently returned "0/0 tests" even
  for files confirmed present and well-formed on disk. Root cause not
  fully diagnosed (possibly a real bug in the basename-filter argv
  handling, or something in how -sleighpath interacts with it). WORKAROUND
  that reliably works: point -path at a directory and run the whole
  batch (no trailing filename), then grep the output for your test''s
  stringmatch names. Also discovered a scratch directory OUTSIDE the real
  datatests folder intermittently returned 0 tests even for multiple
  files with valid content -- not fully explained, possibly related to
  Pulsar buffer caching interfering with what''s actually on disk (see
  next point). Simplest reliable approach: just add/edit files directly
  inside the real src/decompile/datatests/ folder.
- CAUTION: Pulsar''s MCP file buffers can silently resurrect deleted files
  (re-saving a stale open buffer back to disk after Remove-Item), causing
  confusing "file exists" / stale-content problems. If a file must be
  truly gone, use pulsar-edit-mcp-server:list-open-files and close-file
  (save:false) BEFORE deleting on disk, or expect it to come back.

### The repro file: satadd_debug.xml

Located at
Ghidra\Features\Decompiler\src\decompile\datatests\satadd_debug.xml.

Key ingredients that took several failed attempts to get right:
1. arch="H8:BE:32:H8539F" (confirmed via get_current_program_info in the
   live Ghidra session).
2. Raw bytes for sat_add_u16, offset 0x14000. IMPORTANT BUG IN EARLIER
   ATTEMPT: originally used a 14-byte read
   (17008806ee0820240358ffff0f11), which truncates the final prts
   instruction (2 bytes: 1119) to 1 byte, causing the SLEIGH disassembler
   to fail immediately with "Control flow encountered bad instruction
   data" / "Bad instruction - Truncating control flow here" -- a totally
   different, uninteresting failure mode that produces a halt_baddata()
   stub instead of real decompiled output, and silently explains why
   every earlier stringmatch probe came back empty. CORRECT bytes (15
   bytes, confirmed via disassemble_bytes showing prts as 2 bytes ending
   at 0x1400e): 17008806ee0820240358ffff0f1119.
3. parse line extern uint2 sat_add_u16(uint2 a,uint2 b); BEFORE lo fu /
   decompile -- without an explicit prototype declaration, the harness''s
   bare decompile command does not run whatever parameter-recovery/
   signature-naming analysis full interactive Ghidra normally runs
   automatically, so parameters never get proper names/types and none of
   the interesting bug-marker strings appear at all (this was the second
   red herring -- an "empty-looking" decompile that had nothing to do
   with the actual bug). NOTE: use Ghidra''s internal type names
   (uint2/int4/int8 etc, NOT ushort/int) in parse line -- ushort causes
   "Did not apply tests" (parse failure).
4. option readonly on -- copied from other working datatests
   (pointerrel.xml), keeps the harness from trying to write back to the
   loaded image.

### CONFIRMED: harness reproduces the real bug exactly

Full satadd_debug.xml decompiled output (from harness_bulkout_diag.txt,
via the print-C dump instrumentation):

uint2 sat_add_u16(uint2 a,uint2 b)
{
  uint2 uVar1;
  xunknown1 in_TP;
  uint2 in_stack_00000004;
  uint2 auStack_102 [128];

  uVar1 = in_stack_00000004 + *(uint2 *)((int4)(auStack_102 + 0x80) + 8);
  if (CARRY2(in_stack_00000004,*(uint2 *)((int4)(auStack_102 + 0x80) + 8))) {
    uVar1 = 0xffff;
  }
  return uVar1;
}

This matches the live-Ghidra bug shape exactly: the bogus auStack_102[128]
open array, the (int4)(auStack_102 + 0x80) + 8 pointer-cast arithmetic
reading b''s slot, AND additionally shows that a itself never gets bound
to the declared prototype parameter either -- it surfaces as a raw
in_stack_00000004 placeholder, which is a detail live Ghidra''s fuller
analysis pipeline was papering over (still worth investigating: does full
Ghidra''s parameter-ID pass just relabel this, or does something upstream
of that pass actually behave differently with vs without a manually
declared prototype -- OPEN QUESTION for next session).

stringmatch results confirm this precisely:
- PRESENT auStack -> Success (bug marker present)
- Canary param a / Canary param b (checking for "ushort a"/"ushort b")
  -> FAIL (expected -- signature uses uint2 not ushort, and parameter a
  isn''t bound to the name "a" at all, see above)
- PRESENT param_1 / PRESENT param_2 -> FAIL (parameters aren''t named
  param_1/param_2 either in this pipeline -- moot, superseded by the
  auStack confirmation)

### Status summary (supersedes all earlier ones)

- Working, fast, offline repro harness now exists: ghidra_test.exe +
  satadd_debug.xml, runs in seconds, confirmed reproduces the exact bug.
  This should be the primary tool for all further investigation and fix
  attempts on this bug -- no more full-Ghidra-rebuild cycles needed for
  iteration (only needed once a real fix is ready to verify against the
  full live binary/UI as a final check).
- Standing instrumentation still live and unharmed by this work: the
  RangeHint::merge diagnostic (falsified hypothesis), the gatherOpen
  diagnostic (falsified hypothesis), and the three gatherAdditiveBase
  diagnostics (real signal: single BFS, PTRSUB/PTRADD misidentified via
  get_opname''s CROSSBUILD/LABEL display aliases, see previous update).
- Found but NOT yet reconciled: "varmap - backup.cc" in the same
  directory as the live varmap.cc, containing a MORE ADVANCED prior
  investigation from 2026-08-26 (a day before this session) with a
  partially-built gateOut mechanism in gatherOpen targeting the exact
  same 0xe->SEGMENTOP->0x11000 chain. That work was NOT determined to be
  a dead end (no such comment found anywhere in the file) -- it looks
  like an interrupted, promising thread that this session''s "clean
  baseline" revert lost. STILL UNRESOLVED: whether to restore/continue
  that gateOut work or continue independently; needs a decision next
  session.
- Next step: use the new harness to rapidly iterate on either (a)
  restoring/completing the gateOut approach from varmap - backup.cc, or
  (b) a fresh fix attempt in gatherAdditiveBase/gatherOpen informed by
  today''s BFS trace, testing each attempt in seconds via satadd_debug.xml
  instead of full rebuilds.
- All caveats about the harness''s "-path <dir> <basename>" filtering not
  working, and Pulsar buffer resurrection of deleted files, carry forward
  -- see Invocation section above.
- This entire investigation remains OUT OF SCOPE for PR #9541
  (already-landed, confirmed-complete register0x0e/SEGMENTOP annotation
  fix, commit aadd7ee) and must land as its own separate, independent
  patch once resolved.

---

## HANDOFF 2026-08-27 (end of session) -- read this first in a new conversation

**One-line status**: Root cause not yet found. A fast offline repro harness
now exists and works. Two candidate directions are queued up, neither
started. No fix has been attempted yet.

### To get oriented fast, in order:
1. Read this file top-to-bottom once (it''s long but each "Update"/"Status
   summary" section supersedes the previous -- the LAST status summary
   before this handoff is the authoritative one).
2. Confirm the harness still works before doing anything else:
   cd C:\Users\j.brophy.CORKILLSYSTEMS\github\ghidra\Ghidra\Features\Decompiler\build\os\win_x86_64
   .\ghidra_test.exe -sleighpath "C:\Users\j.brophy.CORKILLSYSTEMS\github\Ghidra-H8-Processor\h8\data\languages" -path "C:\Users\j.brophy.CORKILLSYSTEMS\github\ghidra\Ghidra\Features\Decompiler\src\decompile\datatests" 2>&1 | Select-String "SatAdd|Canary|Total"
   Should show "Success -- SatAdd #1", "Success -- Canary function name",
   "Total tests applied = 13", "Total passing tests = 6". If not, something
   changed -- re-read the "standalone offline test harness" section above.
3. To see full decompiled C output of any test run, read (it accumulates
   across runs, check the LAST "=== bulkout for ... satadd_debug.xml"
   block):
   C:\Users\j.brophy.CORKILLSYSTEMS\harness_bulkout_diag.txt

### The bug, restated simply
sat_add_u16(ushort a, ushort b) at address 0x14000 in the H8 firmware
decompiles to a bogus ushort auStack_102[128] open array with pointer-cast
arithmetic ((int)(auStack_102 + 0x80) + 8) instead of two clean ushort
parameters. Root cause is somewhere in AliasChecker/RangeHint''s open-range
handling in varmap.cc, upstream of where two falsified hypotheses were
ruled out (see below). Confirmed via the harness that even a manually
declared prototype (parse line extern uint2 sat_add_u16(uint2 a,uint2 b))
does not fix it and parameter "a" itself doesn''t bind to its declared name
either -- surfaces as in_stack_00000004.

### Falsified (don''t re-investigate these)
- RangeHint::merge() resType==2 "confuse" branch: instrumented, never
  fires for this function.
- MapState::gatherOpen()/addRange() "two ranges collide" theory: only ONE
  open range is ever seeded (offset 0x4, ct=null, no index) -- nothing
  collides.
- "CROSSBUILD/LABEL are delay-slot ops the BFS skips" theory: WRONG, this
  build''s get_opname() just prints PTRSUB as "CROSSBUILD" and PTRADD as
  "LABEL" -- these are real, normal ops the BFS DOES handle correctly.
  Don''t be fooled by get_opname output again.

### Two queued, unstarted next steps (pick one)
A) UNRESOLVED THREAD: "varmap - backup.cc" sits next to the live varmap.cc
   in the same directory
   (C:\Users\j.brophy.CORKILLSYSTEMS\github\ghidra\Ghidra\Features\Decompiler\src\decompile\cpp\).
   It contains a MORE ADVANCED, seemingly-not-abandoned investigation from
   2026-08-26 (the day before this whole session) with a partially-built
   gateOut mechanism in gatherOpen targeting the exact same
   0xe->SEGMENTOP->0x11000 chain this session independently rediscovered.
   No comment in that file says it was wrong or abandoned -- it just stops
   mid-experiment. Worth reading in full and deciding whether to finish it.
B) Fresh attempt informed by today''s gatherAdditiveBase BFS trace: the
   single BFS from spacebase (addr 0xe) produces exactly one terminal LOAD
   via SEGMENTOP->INT_ADD->LOAD, meaning a''s and b''s addresses are already
   combined into one load before AliasChecker ever sees them. Root cause
   is upstream of AliasChecker (SLEIGH spec itself believed CORRECT per
   user, cross-checked against ana.cpp/datasheets -- don''t suspect the
   .slaspec/.sinc). Next unexamined layer: how pcode for this instruction
   sequence gets generated/combined before AliasChecker runs.

Either direction: USE THE HARNESS (satadd_debug.xml) to test each change
in seconds. Do not fall back to full-Ghidra force_decompile rebuild cycles
except as a final verification once a fix looks promising.

### Known tooling gotchas (see full sections above for detail)
- pulsar str_replace tool anchors DRIFT after multiple sequential edits to
  the same file in one session -- caused two file corruptions this
  session. Prefer whole-function-body replacement, or PowerShell with an
  explicit sanity-check assertion on exact expected line content BEFORE
  writing, especially on a 2nd+ edit to the same file.
- Pulsar buffers can silently resurrect deleted files. Use
  list-open-files + close-file(save:false) before relying on a delete.
- ghidra_test.exe "-path <dir> datatests <basename>" filtering is
  unreliable -- run the whole datatests directory and grep output instead.
- Byte-count-off-by-one killed an entire investigation thread earlier
  today (missed last byte of a 2-byte prts instruction -> "bad instruction
  data" -> looked like decompile was producing nothing). Always verify
  instruction-exact byte counts via disassemble_bytes, not a guessed
  length on read_memory.

### Still out of scope
This entire investigation is OUT OF SCOPE for PR #9541 (already-landed,
confirmed-complete register0x0e/SEGMENTOP annotation fix, commit
aadd7ee) and must land as its own separate, independent patch once
resolved.

## File closed -- auStack_ investigation continues in review18.md (2026-08-28)

This file had grown too large to read in as a single file -- see the
pointer note above (line 7 of this file, referring back to review16.md)
for the same reason this split is happening again now.

**What got resolved since the handoff above:**
- The missing rebuild command for `ghidra_test.exe` is now documented:
  `cd C:\Users\j.brophy.CORKILLSYSTEMS\github\ghidra` then
  `.\gradlew.bat :Decompiler:decompile_testExecutable` (fast incremental
  rebuild of just the `decompile_test` component, not a full Ghidra
  rebuild). This was missing from the "Invocation" section above and cost
  real time next session -- make sure review18.md keeps this visible near
  the top.
- Added `space=<name>` to the `gatherAdditiveBase` diagnostic (both the
  per-call start line and the per-visit line) to settle direction B's
  open question about whether repeated `startvn addr=0xe` log lines were
  the same address space or a coincidental offset collision from another
  function's spacebase (a possibility this file itself flagged, see line
  ~901 above). **Confirmed: same space (`register`) every time** -- not a
  collision. Both calls are genuinely the same logical spacebase for
  `sat_add_u16` itself.
- With that ruled out, direction B produced a real, sharper finding: the
  *first* `gatherAdditiveBase` call for `sat_add_u16` sees a "wide"
  pre-simplification p-code shape (`INT_SUB`+`SEGMENTOP`+`COPY` at
  0x14000, plus a distinct `vn@0xc` branch reaching a separate terminal
  LOAD at 0x21d00 via 0x14002). Every subsequent call in the same
  decompilation sees an already-collapsed shape (`INT_ADD` at 0x14000,
  `vn@0xc` and the 0x21d00 LOAD gone entirely, `SEGMENTOP@0x14004` now
  hanging directly off `vn@0xe`). **This means the merge is not happening
  inside AliasChecker/gatherAdditiveBase at all** -- something rewrites
  the p-code graph between the first and second call, inside the same
  decompilation. AliasChecker is just faithfully walking whatever graph
  it's handed each time, exactly as this file's own "Conclusion" section
  (line ~1260 above) already argued from a different angle.
- Practical note: `gatherAdditiveBase_diag.txt` and similar diagnostic
  files must be deleted before each harness run (`Remove-Item` needs
  `confirm:true` in Pulsar's `run-command`, since it's a destructive op)
  or old entries accumulate and make the freshest trace hard to isolate.

**Guidance for future sessions:** read `review18.md` first -- it starts
from a short recap of this new lead rather than repeating this file's
full chronological log. Only come back to this file if you need the
history of the two falsified hypotheses (RangeHint::merge "confuse"
branch, and the SEGMENTOP-exclusion gate experiments) or the harness
build/repro-file work in detail.

## AMENDMENT (2026-08-29): review18.md now covers root-cause diagnosis
## and a crash incident -- read it before assuming this file is current

Since the closing section above was written, `review18.md` accumulated
substantial further work in the same investigation thread. Summary for
anyone landing on this file first:

- **A likely root cause was found**, backed by direct source reading and
  empirical toggle-testing (not guesswork): `RuleLoadVarnode`'s
  `vnSpacebase`/`checkSpacebase` (ruleaction.cc) only unwraps one level
  of `SEGMENTOP` when resolving whether a LOAD/STORE's pointer is a
  stack/spacebase access. `sat_add_u16`'s parameter `b` needs a chain
  three levels deep (`INT_ADD <- SEGMENTOP <- INT_ADD <- ... raw input`),
  which the existing one-level unwrap can't reach -- explaining why `a`
  resolves cleanly (only two levels needed) while `b` falls through to
  the generic merged `auStack_102[128]` blob. `RuleAddMultCollapse`,
  this file's and review18.md's earlier suspect, was directly
  toggle-tested and **falsified** (zero effect on output).
- **A fix attempt CRASHED one of the PR #9541 regression tests**
  (`register0x0e_crossregister_fix.xml`, access violation) and was
  **immediately and fully reverted** -- `ruleaction.cc`/`ruleaction.hh`
  confirmed back to pristine, full test suite confirmed passing again
  (`Total passing tests = 7`, no crash) before review18.md was updated.
  The crash is analyzed in depth in review18.md (likely cause: calling
  `getIn(2)` on a SEGMENTOP outside the narrow context where
  `checkSpacebase` already safely does this), with concrete guidance for
  a future attempt, including a hard requirement to test against both
  `register0x0e_*.xml` files on every iteration going forward, not just
  the target bug's own Canary tests.
- This investigation remains, as always, entirely separate from and must
  not regress PR #9541 (commit aadd7ee) -- this file's repeated
  reminders of that held up as directly load-bearing this time, not just
  boilerplate caution.

**Read `review18.md` in full** for the actual diagnosis, the crash
incident, and the next concrete steps -- this amendment is a pointer,
not a substitute for that detail.

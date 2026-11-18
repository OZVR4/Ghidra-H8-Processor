# Investigation: 9561 triggers wrong-return-value / decompiler-crash regression on multi-parameter functions
## Status (updated 2026-09-07): ROOT CAUSE LOCATED, fix NOT YET WORKING. `muldiv_u16_scaled` SIGSEGVs during decompilation (confirmed via in-process `segvHandler` capture, real symbolized stack trace) inside `Merge::trimOpOutput` (`merge.cc`), called via `Merge::mergeOp`->`mergeIndirect`->`mergeMarker`->`mergeMultiEntry`<-`ActionMergeRequired::apply`. The fault is a stale/dangling IOP-encoded `PcodeOp*` reference decoded by `PcodeOp::getOpFromConst` -- NOT a missing type-check (an `IPTR_IOP`-space guard was added and did not fix the crash; the crashing case still legitimately reports IPTR_IOP space, so the encoded pointer itself is what's stale/invalid). Confirmed specific to this exact function: all 7 other closely-related sibling functions (`muldiv_u16`, `muldiv_s16_rounded[_3op]`, `muldiv_u16_shr8_sat`, `sat_add_s16`, `sat_sub_u16_clamp0`, `div_u32_u16_rounded`, `select_min_xy_triple`) decompile without crashing, ruling out both the shared calling-convention/cspec R0:R1-return-shape theory and a return-type-declaration theory. Next step: get a fresh `segvHandler` capture on the PDB-enabled build with the current diagnostics in place to find the EXACT faulting line (likely 1-2 frames deeper than `trimOpOutput`, inside `Funcdata::opInsertAfter` or `PcodeOp`'s linked-list manipulation) -- see the bottom few sections of this document for the full, current line of investigation and all temporary diagnostic code still in place (`ghidra_process.cc`'s `segvHandler`, `merge.cc`'s `trimop_diag.txt` logging) that needs removing once a real fix lands.d.

## Summary

Commit `9561` (fix for `RuleLoadVarnode::vnSpacebase` not recursively
unwrapping a SEGMENTOP nested inside an INT_ADD -- see the prior
`review16-19.md` series) causes a **new** regression on functions that
combine a stack-passed parameter (resolved via SEGMENTOP) with one or
more register-passed parameters, under this H8 target's
`__stdcall_far_allstack` / `_32ret` calling conventions.

This is **not** the same bug `9561` was written to fix. It is a
previously-latent defect elsewhere in the decompiler (most likely in
heritage / merge-group handling around `INDIRECT` ops, not in
`vnSpacebase` itself) that `9561`'s fix newly exposes, because it
changes the P-code shape of the affected LOAD from "unresolved,
generic pointer dereference" to "resolved stack parameter" -- and that
shape change appears to interact badly with this target's mixed
register/stack argument passing.

## Confirmed trigger

Reproduced against the real ROM (`RVR_1998_x3 4g63t 21000011
md352553.hex`, H8:BE:32:H8539F, 741 functions) on three separate
builds:

- **Unpatched baseline** (`RVR_1998_x3 4g63t 21000011 md352553.hex.c`):
  all affected functions below decompile correctly. No wrong return
  values. No crash.
- **9561-only** (`RVR_1998_x3 4g63t 21000011 md352553 only one.hex.c`,
  and confirmed live via Ghidra MCP against the same build): the
  regression is present.
- **9540 + 9561 + 9450 merged** (`RVR_1998_x3 4g63t 21000011 md352553
  all three.hex.c`): the regression is still present, unchanged.
  9450 does not appear to cause or worsen it (see "Ruled out" below).

## Affected functions (confirmed)

All four are 3-parameter functions mixing a stack-passed parameter
(resolved via SEGMENTOP) with register-passed parameters (`R1`/`R3`
on this target), under a non-trivial calling convention
(`__stdcall_far_allstack` or `__stdcall_far_allstack_32ret`):

| Function | Address | Symptom |
|---|---|---|
| `muldiv_u16` | `0x141ba` | `return b;` instead of the computed division result (`uVar3`) |
| `muldiv_u16_scaled` | `0x141dc` | Decompiler crash: "Decompiler process died" |
| `muldiv_s16_rounded` | `0x14219` | `return b;` instead of the computed result |
| `muldiv_s16_rounded_3op` | `0x14247` | Unresolved raw registers (`in_R1`, `unaff_R3`) in output; `return sVar5` skips the real computation |
| `weighted_blend_u8frac` | (see datatests / ROM) | Same `in_R1`/`unaff_R3` pattern; `return sVar3` ignores the real blend result (`uVar2`) |

By contrast, `sat_add_u16` -- the function `9561`'s own regression
test (`nestedsegmentop.xml`) targets -- is unaffected. It is a simple
2-parameter, plain `__stdcall_far_allstack` function with *no*
register-passed arguments at all. This is consistent with the theory
below: the trigger needs *both* a SEGMENTOP-resolved stack parameter
*and* at least one register-passed parameter in the same function.

## P-code trail (muldiv_u16, the clearest case)

Captured via `ghidra:get_function_pcode` against the live 9561-only
build (address `0x141ba`).

In the final basic block (`0x141ce`-`0x141da`):

1. `INT_DIV` computes the real division result into a unique varnode.
2. `SUBPIECE` truncates it to the 2-byte quotient (the real answer).
3. `COPY` moves it into `register:2` (**merge_group 1**) -- this is
   the correct value that should become the return value.
4. Immediately after, an `INDIRECT` fires on `register:2`, keyed to a
   call-site marker (`const:0x4a`) -- the decompiler's way of saying
   "a call between here and the next read of this register might
   clobber it."
5. `COPY register:2 -> register:0` follows -- but by this point
   `register:2`'s value is the *post-INDIRECT* one, not the div
   result from step 3.
6. `register:0` is also where parameter `a` lives (same
   space/offset), and the return path ultimately reads `register:0`.

The correct division result and the wrong final register:0 value are
different SSA-ish generations of the *same* register storage location,
merged incorrectly across the `INDIRECT` call-clobber marker. The
SEGMENTOP-resolved LOAD for parameter `c` (`9561`'s actual fix target)
does resolve *correctly* in this same function -- the merge-array bug
`9561` targets is genuinely fixed here. The wrong-return-value bug is
downstream of that, in how the newly-resolved dataflow interacts with
register merge/heritage around the `INDIRECT`.

## Ruled out

- **Not caused by 9540** (register0x0e print-layer fix): confirmed via
  the `new.hex.gzf.c` export (9540-only, no 9561). `muldiv_u16`
  decompiles perfectly there -- correct return value, no `auStack_`
  array, no crash on `muldiv_u16_scaled`.
- **Not caused or worsened by 9450** (far-pointer argument-fusing
  mechanism, `ActionFarPointerJoin`): confirmed via the "all three"
  merged export. The regression is present and unchanged whether 9450
  is in the build or not. (9450 itself is separately unverified and
  has its own known issues -- not in scope here.)
- **Not a pre-existing decompiler defect unrelated to 9561**: the
  unpatched baseline decompiles all five affected functions correctly.
  The regression only appears once 9561's `vnSpacebase` fix is applied.

## Working theory

`9561` changes `checkSpacebase`'s LOAD for a SEGMENTOP-nested-in-INT_ADD
stack parameter from "unresolved / falls through to generic alias
handling" to "resolved as a proper stack varnode." That's the intended
fix. But in functions where the same P-code also has register-passed
parameters using the same physical registers the return value is
computed through, resolving that LOAD appears to change enough of the
surrounding P-code graph (fewer/different intermediate unique
varnodes, different rule-firing order in a later `Action` pass) that a
latent bug in how `INDIRECT` call-clobber markers get merged with
prior register definitions gets triggered where it previously wasn't.
This points at heritage (`Heritage` / `ActionMergeCopy` /
deadcode-elimination-adjacent logic in `coreaction.cc` or
`heritage.cc`), not at `vnSpacebase` or `RuleLoadVarnode` itself -- but
this has not been directly confirmed by stepping through those passes
yet.

## Major update: h8539f.cspec/.slaspec already documents this exact class of bug

`h8539f.cspec` (lines ~185-232) and `h8539f.slaspec` (lines ~23-39)
document a prior investigation (dated 2026-07-22, predating this
session) of the *identical* warning strings this regression produces
("Unable to track spacebase fully for stack" / "Restarted to delay
deadcode elimination for space: register"), on a named group of 6
"math helper" functions that need a 4-byte R0:R1 register-pair return
value but only have a 2-byte R0 output pentry available
(`__stdcall_far_allstack`/`_32ret`'s `<o>`/`<output>` only declares
`R0`, maxsize=2).

Key finding from that investigation, directly relevant here: **a
register-space or prototype change that has no direct call
relationship to the affected functions can still perturb the
decompiler's global return-value/stack-tracking model**, confirmed by
that investigation via the same pre/post whole-ROM diff methodology
used in this document (0 such warnings before a change, 27 after,
spread across 18 unrelated functions). Multiple attempted fixes (a
join-pentry R0:R1 output, a real overlapping R0R1 register in the
register space) were tried and reverted after confirming, via full
Ghidra restarts, that they did not fix the target functions and did
still cause the same broader pollution. The accepted state is a
plain 2-byte R0 pentry, accepting a known, narrower "dropped-R1-half"
warning on just the named 6 functions as the least-bad option.

**Important distinction**: the 6 functions named in that prior
investigation (`sat_add_s16`, `sat_sub_u16_clamp0`,
`muldiv_u16_scaled`, `muldiv_s16_rounded_3op`, `div_u32_u16_rounded`,
`select_min_xy_triple`) genuinely need a 4-byte return value and the
warning there is a real signature-modeling limitation. `muldiv_u16`
and `muldiv_s16_rounded` (the plain, non-suffixed functions showing
`return b;` in this investigation) return a plain 2-byte value that
*does* fit the R0 pentry -- they are not part of that named group, so
their wrong-return-value symptom is not simply "the same known
dropped-R1-half issue." However `muldiv_u16_scaled` (which crashes
outright under 9561) *is* one of the 6 named functions, so at least
that crash may be entangled with, or a worse manifestation of, the
pre-existing documented limitation rather than something wholly new.

This significantly changes the read on root cause: rather than a
`Merge::mergeIndirect` bug specific to 9561, the more likely
explanation is that **this decompiler target's parameter/return-value
recovery for register-pair and mixed register/stack calling
conventions is already known to be globally fragile** to any change
that shifts P-code shape, and 9561's SEGMENTOP-unwrap fix is simply
the latest such change to trigger it -- the same way the register
R0R1 overlap and the join-pentry attempt did in July, unrelated to
each other except via this shared fragility.

## Live debugging session (2026-09-02): scoped diagnostic + real trail

Added a scoped diagnostic directly to `Merge::mergeOp` in `merge.cc`
(gated strictly to `muldiv_u16`'s address range, `0x141ba`-`0x141da`;
temporary, writes to `mergeop_diag.txt`; not intended to ship), logging
each merge decision's op/varnode identities. Rebuilt full Ghidra with
the instrumentation in place and force-decompiled `muldiv_u16` via
Ghidra MCP against the live 9561-only instance to trigger it.

### Real assembly (confirmed via disassemble_bytes, not assumed)

```
000141ba  push  R2 (mov:g.w R2,@-SP)         -- callee-save c's register slot
000141c4  ...   (SEGMENTOP-resolved LOAD of c into R2, via 9561's fix)
000141c6  divxu.w R2,R0                       -- 32-bit R0:R1 / R2 -> quotient/remainder
000141ca  bhs   0x141d7 (branch on carry/high, skips the 0xffff path)
000141cc  ...
000141ce  ...   -- real division result computed here (INT_DIV, SUBPIECE)
000141d0  mov:g.w R1,R0                       -- move quotient into R0 (return reg)
000141d2  bra   0x141d7
000141d4  mov:i #0xffff:16,R0                 -- saturation/error path, R0 = 0xffff
000141d7  mov:g.w @SP+,R2                     -- restore c's saved register
000141d9  unlk
```

### Control-flow graph (via analyze_control_flow, confirmed not assumed)

5 basic blocks, cyclomatic complexity 3, two conditional blocks
(`0x141ba`, `0x141ca`) converging into a single exit block at
`0x141d7`. This is a completely ordinary if/else-converging-on-return
shape -- `0x141ce`'s real division result and `0x141d4`'s `0xffff`
saturation value are the two legitimate candidate values for R0 at the
join point. **This confirms the control flow itself is not the bug**
-- a MULTIEQUAL/join here merging these two paths' R0 value into one
HighVariable is exactly correct compiler-theory behavior.

### mergeOp diagnostic output (the actual merge event)

```
mergeOp at 0x141d7 opcode=BUILD out_addr=0x0 out_space=register out_high=...8410
  in[0] addr=0xaad02 space=unique high=...7ED0 is_written=1  def_op=SUBPIECE def_addr=0x141ce
  in[1] addr=0x0 space=register high=...8170 is_written=1  def_op=COPY def_addr=0x141d4
```

`in[0]` is the real division result (`SUBPIECE` truncating the 32-bit
quotient at the block ending in the division). `in[1]` is the `0xffff`
saturation value (`COPY` from the `mov:i #0xffff:16,R0` at `0x141d4`).
Both write to register offset `0x0` (R0) and get forced into the same
HighVariable by `Merge::mergeOp` (`merge.cc`), which is correct -- they
are the two legitimate join candidates for R0's value at the shared
exit block.

### Revised theory (supersedes the earlier INDIRECT/heritage theory)

The wrong-return-value bug is **not** in the join/merge decision
itself -- both real candidate values are correctly identified and
correctly merged into one HighVariable. The bug must be one level
higher: in how the decompiler picks a **representative
name/symbol/display value** for that merged HighVariable. Somewhere,
this representative-selection logic is choosing `b`'s storage/symbol
association instead of either of the two real R0-holding varnodes
(`uVar3`, the SUBPIECE result, or the 0xffff immediate) -- printing
`return b;`, which is neither of the two legitimate values.

This is consistent with 9561's fix as the trigger: before the fix, the
SEGMENTOP-resolved LOAD for `c` (at `0x141c4`) failed to resolve and
fell into generic alias-array handling (the `auStack_` shape). After
the fix, `c`'s LOAD resolves cleanly as a real stack variable -- which
changes what varnodes/symbols are "in scope" and considered as naming
candidates when Merge/HighVariable machinery picks a representative
for the R0 join. It is plausible `b`'s Varnode (also register/stack
resident nearby) becomes an eligible "same storage class" candidate
for the representative pick only once 9561's fix removes whatever was
previously occupying or shadowing that role.

## Root cause confirmed (2026-09-02, second diagnostic pass)

Added a second scoped diagnostic to `HighVariable::getNameRepresentative`
(`variable.cc`), gated to any merged HighVariable containing a member
defined within `muldiv_u16`'s address range, logging the full `inst`
member list and the chosen representative. Rebuilt full Ghidra, ran
against the live instance.

The critical entry:

```
getNameRepresentative: inst.size()=3 chosen_addr=0x6 chosen_space=stack chosen_isInput=1 chosen_isAddrTied=1 chosen_isWritten=0
  member addr=0x2 space=register isInput=0 isAddrTied=0 isWritten=1 def_op=DELAY_SLOT def_addr=0x141ce
  member addr=0x2 space=register isInput=0 isAddrTied=0 isWritten=1 def_op=DELAY_SLOT def_addr=0x141c2
  member addr=0x6 space=stack isInput=1 isAddrTied=1 isWritten=0
```

**This is the actual bug.** A single HighVariable contains three
members that should never share one logical variable:

- Two `register:2` varnodes, both defined by `DELAY_SLOT` ops at
  `0x141c2`/`0x141ce` -- these are working-register generations
  belonging to the `divxu.w R2,R0` 32-bit division (R1/R2 hold
  dividend/divisor halves per H8 semantics).
- One `stack:0x6` varnode, `isInput=1`, `isAddrTied=1` -- this is
  parameter storage for one of the function's real incoming
  arguments (stack offset 0x6).

`HighVariable::compareName`'s priority rules (see `variable.cc`
~line 456) give `isInput()`+`isAddrTied()` varnodes top priority for
the *display name* -- correctly, by that function's own logic, once
handed this member list. The bug is **upstream**: `stack:0x6` and
`register:2` are being merged into the same HighVariable at all. They
have no legitimate reason to share storage or be treated as the same
logical variable -- one is a parameter's stack home, the other is a
transient division-computation register. This merge decision is made
by `Merge`'s cover/liveness-intersection logic (most likely
`Merge::mergeLinear` or `Merge::merge` in `merge.cc`, not the
per-op `mergeOp` that was the first diagnostic's target -- that one
only showed the *legitimate* R0 join, which is a separate, correct
merge from this one).

**Working theory, refined**: 9561's fix changes what LOAD resolves for
the SEGMENTOP-nested-in-INT_ADD access (parameter `c`'s stack read at
`0x141c4`). This is very likely changing the computed address-cover
range the decompiler associates with the surrounding stack frame
during heritage/cover analysis, in a way that causes the *unrelated*
`stack:0x6` parameter's cover to be computed as overlapping with
`register:2`'s liveness range when it should not. This is consistent
with the multi-parameter-only trigger pattern (functions needing
several distinct stack/register slots resolved close together in the
same frame are the ones exposed) and with `sat_add_u16` being
unaffected (only one SEGMENTOP-resolved access, no register-argument
neighbors to collide with).

## Call-site elimination (2026-09-02, third diagnostic pass)

Confirmed via `Merge::merge` diagnostic that the bad pairing is real
and reproducible on every rebuild:

```
Merge::merge caller=unknown isspeculative=0 willIntersect=0
 high1 numInstances=1
   addr=0x2 space=register isInput=0 isAddrTied=0 def_op=DELAY_SLOT def_addr=0x141c2
 high2 numInstances=1
   addr=0x6 space=stack isInput=1 isAddrTied=1
```

Added a static `DIAG_callerTag` string (file-scope in `merge.cc`,
declared before first use) that the two known `isspeculative=false`
call sites *within merge.cc* (`Merge::mergeOpcode`'s required-merge
loop, and `Merge::mergeMultiEntry`'s second loop) set immediately
before calling `merge()`, so the diagnostic inside `merge()` could
report which one is actually responsible.

**Result: every single logged call, including the register:2/stack:0x6
one, shows `caller=unknown`.** Neither tag was ever set before this
particular call fired. This positively rules out both remaining
`merge.cc`-internal candidates:

- `Merge::mergeOpcode`: only fires for legitimate, unrelated pairs in
  this function (e.g. `register:2`/`unique` COPY-merges) -- confirmed
  via an earlier, separate diagnostic pass at that exact call site.
- `Merge::mergeMultiEntry`: an even earlier diagnostic (gated on the
  same address/offset pair, placed at the `mergeList` construction
  point) never fires at all for this pair -- `symentry_diag.txt` and
  `multientry_diag.txt` are both empty across multiple rebuild/test
  cycles.
- `Merge::mergeAdjacent` (line ~1081): ruled out on `isspeculative`
  value alone (`mergeAdjacent` always calls `merge(...,true)`; the
  real bad call is `isspeculative=0`).

**Conclusion: the call originates outside `merge.cc` entirely.**
Most likely `coreaction.cc` (an `Action` subclass calling
`data.getMerge().merge(...)` or an equivalent public entry point) or
another file with a legitimate reason to force a required merge. An
earlier broad keyword search found candidates at `coreaction.cc:3563`
(`ActionMergeRequired`-adjacent, calls `inflateTest`, a related but
distinct function -- not yet fully explored) and `coreaction.cc:5209`
(not yet examined at all).

## Further elimination (2026-09-02, fourth diagnostic pass)

Checked the two remaining `coreaction.cc` candidates from the earlier
broad search:

- `coreaction.cc:3563` (`ActionMarkImplied::checkImpliedCover`): calls
  `inflateTest` (read-only, blocks marking-implied on `true`) but never
  calls `merge()` at all. Not a candidate; ruled out by inspection.
- `coreaction.cc:5209` (`ActionHideShadow::apply`): calls
  `Merge::hideShadows(high)`, which doesn't call `merge()` directly --
  it uses `copyShadow()` to test whether two varnodes are pure-COPY
  shadows of each other, then rewrites the P-code graph directly via
  `data.opSetInput(...)` (redirecting one COPY's input), which could
  indirectly cause a later, already-tagged merge to succeed. Added a
  diagnostic directly inside `hideShadows`'s `copyShadow` check, gated
  on the same register:2/stack:0x6 pair.

**Result: `hideshadow_diag.txt` is empty.** `copyShadow()` never
returns true for this pair -- confirmed correct, since they have no
legitimate common COPY-chain origin. This rules out `hideShadows` as
the mechanism, whether direct or indirect.

**State of elimination so far**: every `isspeculative=false` call site
in `merge.cc` (`mergeOpcode`, `mergeMultiEntry`, `mergeAdjacent` by
its `isspeculative` value alone) is ruled out by direct evidence.
`hideShadows`'s indirect route is also ruled out. The `Merge::merge`
call that actually performs the bad merge continues to log
`caller=unknown` on every rebuild, meaning the true call site has not
yet had `DIAG_callerTag` set before it fires.

## Correction to the root-cause theory (2026-09-03, richer symentry_diag.txt capture)

A fuller capture of the `addMapPoint` diagnostic (run across many
decompiles, not just one) shows something the single-shot capture
missed and corrects the working theory:

```
addMapPoint sym=b sym_ptr=... addr_space=register addr_off=0x2 usepoint_space=ram usepoint_off=0x13fff prior_numEntries=0
addMapPoint sym=$$undef00000001 sym_ptr=... addr_space=stack addr_off=0x6 usepoint_space=invalid usepoint_off=0x0 prior_numEntries=0
```

**`sym=b` is genuinely, correctly mapped to `register:2`** -- this is
`b`'s real, legitimate storage location per this calling convention,
not a spurious entry. **`stack:0x6` gets its own, separate
`$$undef`-named symbol** -- an unrelated unnamed variable (most likely
`c`, resolved via 9561's SEGMENTOP fix), not `b`'s symbol at all.

This means the original "spurious multi-entry Symbol" theory (a single
Symbol wrongly getting both a register:2 and a stack:0x6 entry) does
not hold up under this fuller evidence -- `b` and stack:0x6 are always
logged as two separate, distinctly-named symbols. The
`Merge::merge(register:2, stack:0x6)` pairing caught earlier is real
and reproducible, but it is not explained by a bad Symbol/SymbolEntry
association.

**Revised theory**: `register:2` is legitimately `b`'s parameter
storage, but is *also* reused by `divxu.w`'s internal 32-bit division
work (`DELAY_SLOT` writes at `0x141c2`/`0x141ce`, per the earlier
P-code trail). The bug is most plausibly in **register-reuse-across-
liveness-boundary handling**: the decompiler's Cover/liveness
computation for `b`'s parameter-lifetime use of register:2 may not
correctly end where `b`'s legitimate use ends and the division's
internal reuse of the same physical register begins, causing `Merge`
to treat them as connected when they should be disjoint. This is a
different, more precise hypothesis than the earlier
"spurious-multi-entry-Symbol" one and supersedes it.

## Second crashing function confirmed with real assembly (2026-09-03, muldiv_u16_scaled)

Live Ghidra Listing view confirmed `muldiv_u16_scaled` (`0x141dc`)
still crashes the decompiler ("Exception while decompiling 000141dc:
Decompiler process died"), matching the earlier finding. Real assembly
now available:

```
000141dc  link  FP,#0x0:8
000141de  mov:g.w  R3,@-SP
000141e0  mov:g.w  R2,@-SP
000141e2  mov:f.w  @(0x6:8,FP),R0     -- b (Stack[0x6])
000141e4  mov:f.w  @(0x8:8,FP),R2     -- c (Stack[0x8])
000141e6  mulxu.w  @(0xa:8,FP),R0     -- R0 *= d (Stack[0xa])
000141e9  mulxu.w  @(0xa:8,FP),R2     -- R2 *= d
000141ec  add:g.w  R1,R2
000141ee  addx.w   0x0,R0
000141f2  tst.w    @(0xc:8,FP)
000141f5  beq      LAB_0001420c
000141f7  cmp:g.w  @(0xc:8,FP),R0
000141fa  bhs      LAB_0001420c
000141fc  mov:g.w  R2,R1
```

Unlike `muldiv_u16` (where `b` lives in `register:2`), here **all
three of `muldiv_u16_scaled`'s parameters (`b`, `c`, `d`) are declared
as plain `Stack[0x6]`/`Stack[0x8]`/`Stack[0xa]`** -- a genuinely
different parameter-passing shape, with a wider (32-bit, R0:R1
accumulator via `mulxu.w`+`add`+`addx`) intermediate computation than
`muldiv_u16`'s single 32-bit `divxu.w`. Consistent with this being a
related but not identical trigger shape -- still within the same
family (stack parameter(s) resolved via 9561, register working-state
reused for wider arithmetic), but not proof the exact same mechanism
as `muldiv_u16`'s wrong-return-value bug is responsible for the crash;
worth treating as a related but distinct data point until the actual
`Merge`/Cover mechanism is confirmed.

## Likely real root cause found (2026-09-03, source trace of the warning text itself)

Rather than keep adding one-off `Merge`-side diagnostics, traced the
exact two warning strings every affected function has shown from the
very first decompile in this investigation
("Unable to track spacebase fully for stack" /
"Restarted to delay deadcode elimination for space: register") back
to their source, since they are printed unconditionally whenever the
underlying mechanism fires -- no instrumentation needed, just
`grep`+read.

`override.cc:57`'s `generateDeadcodeDelayMessage` is only ever called
after `Heritage::bumpDeadcodeDelay` (`heritage.cc:2572`) runs, which
itself is only called from three sites in `Heritage`'s own dead-code-
aware heritage pass (`heritage.cc:2717` and `:2729`, inside the loop
that walks all Varnodes in a given address space during a heritage
pass). The trigger condition, read directly from the surrounding
code and its own comment:

```cpp
if ((!needwarning)&&(info->deadremoved>0)&&!fd->isJumptableRecoveryOn()) {
  needwarning = true;
  bumpDeadcodeDelay(vn->getSpace());   // heritage.cc:2717 / :2729
  ...
}
// comment at :2725-2727: "In most cases, a truly new overlapping
// read will hit the bumpDeadcodeDelay either here or in prev==2"
```

This fires when a Varnode is discovered in a given address space
**after dead-code elimination has already run once for that space**
(`info->deadremoved>0`), and its address range wasn't fully accounted
for on the prior heritage pass -- a genuinely new, previously-unseen
overlapping read. Heritage responds by increasing that space's
"deadcode delay" by one and requesting a full restart
(`fd->setRestartPending(true)`), which is precisely the two-line
warning sequence seen on every affected function (first the stack
space, then the register space bump into a restart).

**This is a strong, different, and more upstream candidate than
anything in `Merge`.** The theory: before 9561, `c`'s
SEGMENTOP-nested-in-INT_ADD LOAD simply failed to resolve, so its
value was never actually read into a register during the function's
real dataflow as heritage originally modeled it on pass 1 -- it fell
through to alias-array handling instead, entirely in the stack space,
never touching `register:2`. After 9561's fix, that LOAD resolves
correctly and its value legitimately flows into `register:2` as part
of `divxu.w`'s computation -- but only becomes visible to heritage on
a *later* pass, after the first pass has already run dead-code
elimination on the register space using the old (incomplete) picture.
This is exactly a "truly new overlapping read" in the register space,
triggering the restart-with-delay -- and it is plausible that the
*second*, restarted heritage pass re-heritages `register:2` with
different (and here, incorrect) boundaries between `b`'s legitimate
parameter lifetime and the division's later reuse of the same
physical register, which is what `Merge` then faithfully (and
correctly, given what it's told) merges together.

This would mean the actual defect, if confirmed, is not in `Merge` at
all, and not really a "bug" in the classic sense -- it's a case where
the dead-code-delay-and-restart mechanism's *second* pass produces a
subtly different (and here, wrong) heritage result than a from-scratch
single pass would, specifically for a register that legitimately has
two completely different roles (parameter storage early, transient
compute state later) within the same function.

## Sleigh spec check (2026-09-03): disp8_banked/disp16_banked for FP do NOT use the spSegment userop

Checked `h8539f.pspec` and `h8539f.slaspec` directly, since they define
the actual SEGMENTOP construct 9561's fix targets, and `h8539f.pspec`
carries its own extensive prior-investigation history (`review.md item
8`, predating `review16-19.md`) about exactly this SEGMENTOP/banking
mechanism -- including a documented hard schema limit (Ghidra's
`constresolve` supports exactly ONE fully-recognized base register per
address space, occupied by `TP` for SP/FP) and a whole EP-banking saga
(steps 3a reverted, 3b retried).

**Found a real, live inconsistency the pspec's own comment does not
fully reflect.** `h8539f.pspec`'s `<segmentop space="ram"
userop="spSegment">` defines `res = (zext(base) << 16) | zext(inner)`
as a proper userop/CALLOTHER-based SEGMENTOP. In the current
`h8539f.slaspec`:

- `Rn_banked`, `Rs_banked`, `Rn_banked2`, `eab_predec`, `eaw_predec`,
  `eab_postinc`, `eaw_postinc` all correctly call `spSegment(TP, FP)` /
  `spSegment(TP, SP)` / `spSegment(EP, Rn)` for register-indirect and
  auto-inc/dec addressing.
- **`disp8_banked` and `disp16_banked` -- the constructors used for
  displacement addressing, i.e. `@(0x6:8,FP)`-style parameter access,
  exactly the addressing mode every parameter in `muldiv_u16` and
  `muldiv_u16_scaled` uses -- do NOT call `spSegment` for FP/SP.**
  They use the raw manual pattern instead:
  `local addr:4 = (zext(TP) << 16) | zext(addrLo);` (lines
  564-565/573-574). R4/R5 (EP-banked) in these same two constructors
  DO correctly call `spSegment(EP, addrLo)`.

This means **FP/SP-relative displacement addressing does not lower
through the real `CPUI_SEGMENTOP` P-code op at all** -- it produces
the equivalent inline arithmetic directly, unwrapped, at the sleigh
level. Only register-indirect/auto-inc/dec FP/SP addressing goes
through an actual SEGMENTOP.

**Open question this raises, not yet resolved**: does 9561's
`vnSpacebase`/`checkSpacebase` fix (which specifically pattern-matches
`CPUI_SEGMENTOP`) even apply to `muldiv_u16`'s parameter accesses at
all, given they are `disp8`-addressed (manual inline pattern, not a
real SEGMENTOP)? If not, the mechanism actually responsible for
resolving `b`/`c` differently before and after 9561 must be something
else entirely (e.g. `RuleLoadVarnode`'s generic INT_ADD/CAST handling
picking up the manual `(zext(TP)<<16)|zext(addrLo)` shape indirectly,
possibly via a different rule than the one 9561 touches), which would
mean the current root-cause theories (Merge-based, heritage-restart-
based) may be chasing the wrong upstream trigger. Needs direct
confirmation via a P-code dump of the actual LOAD address expression
for `muldiv_u16`'s `b`/`c` parameters, checking whether a genuine
CPUI_SEGMENTOP op appears in that expression tree or not.

## Open question resolved: SEGMENTOP confirmed present, but not the direct bug site (2026-09-03)

Re-pulled `muldiv_u16`'s P-code (`ghidra:get_function_pcode`, high
granularity) and checked directly for `CPUI_SEGMENTOP`, resolving the
open question from the sleigh-spec check above.

**A genuine `CPUI_SEGMENTOP` op is present**, at `0x141c4`:

```
PTRADD    register:0xc, const:0xa, const:1  ->  unique:0x21b00
SEGMENTOP const:base, const:0, unique:0x21b00  ->  unique:0x21d00
CAST      unique:0x21d00  ->  unique:0x10000012
LOAD      const:0x1a1, unique:0x10000012  ->  unique:0x21e00
```

This is the LOAD for parameter `d` (the third parameter, stack offset
`0xa` off FP -- `register:0xc` here is `d`'s value after having been
moved out of stack storage by an earlier `INDIRECT`, not `b`/`c` as
earlier assumed). So 9561's fix genuinely is the correct mechanism to
have been looking at -- `RuleLoadVarnode`/`vnSpacebase` legitimately
processes this exact LOAD, and the sleigh-spec doubt from the previous
section is resolved: this specific LOAD really does go through
`CPUI_SEGMENTOP`, `disp8_banked`'s non-userop pattern notwithstanding
(the `PTRADD`+`SEGMENTOP` pair here is presumably generated by a
different constructor path than `disp8_banked`, or the manual
`(zext(TP)<<16)|zext(...)` pattern still lowers to a `SEGMENTOP`
P-code op internally regardless of whether the sleigh source calls the
`spSegment` *userop* explicitly -- P-code op identity and sleigh
source syntax are not the same thing, and this trace confirms the
former is what matters here).

**Critically, this P-code also directly reconfirms the earlier
`Merge::merge` diagnostic finding, now with full context.** The
`register:2` corruption is a *separate*, coexisting mechanism in the
same function, unrelated to the SEGMENTOP chain above:

```
INDIRECT  stack:0x6, const:0x24  ->  register:2      (at 0x141c2, "b's" INDIRECT)
...
INDIRECT  register:2, const:0x4a  ->  register:2      (at 0x141ce, division's INDIRECT)
COPY      unique:aad02 (div result)  ->  register:2   (merge_group=1, at 0x141ce)
COPY      register:2  ->  register:0                  (at 0x141d7, the return copy)
```

Two *different* `INDIRECT` call-site markers (`const:0x24` vs
`const:0x4a`) both target the same physical `register:2` storage --
one for `b`'s incoming value, one for the division's internal use --
and it is the interaction between these two `INDIRECT`s (both
legitimate individually) that `Merge`/heritage appears to mishandle,
not anything about the SEGMENTOP resolution itself. 9561's fix changes
the overall P-code shape enough (by making the `d`-parameter LOAD at
`0x141c4` resolve cleanly instead of falling into generic alias
handling) to newly expose this pre-existing `register:2` reuse
fragility, without itself being the site of the actual defect.

**This refocuses the investigation squarely back on `Merge`/`Heritage`
's handling of two different `INDIRECT` tags on the same physical
register** -- exactly where the `Merge::merge(register:2, stack:0x6,
isspeculative=0, willIntersect=0)` diagnostic already pointed, before
the sleigh-spec detour. The `bumpDeadcodeDelay`/heritage-restart theory
from the previous section remains the most promising lead for *why*
heritage's second pass ends up conflating these two INDIRECTs'
liveness ranges.

## SESSION HANDOFF (2026-09-04): diagnostic added, build blocked on disk space

**Start here in the next session.** Everything below is real, applied,
uncommitted source state -- verify it's all still present before
continuing (a `git status` / `git diff` on `heritage.cc` should show
the diagnostic block described below; if the build produced a fresh
`decompile.exe`/`ghidra_test.exe` after 4/09/2026 6:50 AM, the fast
compile-check already passed).

### What was just done, in order

1. Added the planned diagnostic at the `bumpDeadcodeDelay` call sites
   in `Heritage`'s dead-code-aware heritage loop (`heritage.cc`, the
   `while(iter != enditer)` loop around line 2703-2732). It is gated
   to fire for any Varnode whose *definition* address falls within
   `muldiv_u16`'s range (`0x141ba`-`0x141da`) **or** whose own address
   is `register:2` (broader gate than just at the `bumpDeadcodeDelay`
   call itself, so it also catches Varnodes considered but not
   flagged). Logs to `C:\Users\j.brophy.CORKILLSYSTEMS\deadcode_diag.txt`:
   `vn_addr`, `vn_space`, `prev` (0/1/2, which of the three branches
   fired), `info->deadremoved`, `isHeritageKnown`, `hasNoDescend`, and
   (if written) `def_op`/`def_addr`.
2. Added `#include <fstream>` to `heritage.cc` (it didn't have one).
3. Fast-verified via `:Decompiler:decompile_testWin_x86_64Executable`
   (confirmed compiles cleanly, exe rebuilt ~1.5 min after the source
   edit -- 4/09/2026 6:49-6:50 AM).
4. Ran `buildGhidra` -- failed on `:Decompiler:ip` license-scan task:
   a leftover, uncommitted investigation-scratch file
   `muldiv_repro.xml` (an isolated-fixture attempt for
   `muldiv_u16_scaled`, using a **longer, possibly-working byte
   sequence** than the "not mapped" attempt earlier in this
   investigation -- worth revisiting if isolated fixtures become
   worth another try) had no `certification.manifest` entry. Deleted
   it (correctly -- it was scratch, not meant to ship). Ran the full
   manifest-vs-datatests cross-check afterward; confirmed clean, no
   other stray files.
5. Re-ran `buildGhidra` -- failed again, this time on
   `:Debugger-agent-dbgeng:buildPyPackage` and a `java.io.IOException`,
   both root-caused to **"No space left on device."** `Get-PSDrive C`
   confirmed 0 GB free (199.75 GB used). This is the same disk-space
   exhaustion issue hit once before in this investigation (see the
   "decompile_test isolation attempt" section above), recurring from
   the volume of `buildGhidra` cycles this session has run. **Not yet
   resolved -- needs manual disk cleanup before the build can proceed.**

### Immediate next step once disk space is cleared

Just re-run `buildGhidra` (the source is ready, already confirmed to
compile). Then, in the live MCP-connected instance:

```powershell
Remove-Item "C:\Users\j.brophy.CORKILLSYSTEMS\deadcode_diag.txt" -ErrorAction SilentlyContinue
```
Then `ghidra:force_decompile` on `ram:000141ba` (muldiv_u16), then
read `deadcode_diag.txt`. Look specifically for entries where
`vn_space=register` and `vn_addr=0x2`, and check the `prev`/
`deadremoved` values against the theory: does a Varnode touching
`register:2` show up with `prev==1` (partially-contained-in-old-range,
the "else" branch, the one whose comment explicitly says "a truly new
overlapping read will hit the bumpDeadcodeDelay") and
`deadremoved>0`? If so, that's direct confirmation of the
heritage-restart theory. If `register:2` never appears in this log at
all, the theory is likely wrong and the search needs to go elsewhere
(possibly back to `Merge`, or to `Heritage::placeMultiequals`/
`Heritage::rename`, which run after this loop and actually assign SSA
form using its results).

### Recommended: clean up disk space AND accumulated diagnostics before continuing

Given four separate diagnostic mechanisms have been added across this
investigation (`mergecall_diag.txt` in `merge.cc`'s `Merge::merge`,
`hideshadow_diag.txt` in `merge.cc`'s `hideShadows`, `symentry_diag.txt`
in `database.cc`'s `addMapPoint`, and now `deadcode_diag.txt` in
`heritage.cc`), all still present and uncommitted in source, it may be
worth clearing the ones that returned negative/ruled-out results
(`hideshadow_diag.txt`'s source hook, `symentry_diag.txt`'s source
hook -- both confirmed not the mechanism) to reduce the diff size and
rebuild surface before adding more, though this is optional cleanup,
not blocking.

### Full context for a fresh session

This file (`review-calling-convention-regression.md`) is the complete,
current record of this investigation, written to be self-contained.
Read from the top. Key facts a new session needs immediately:
- The bug: `9561`'s `vnSpacebase` fix (correct, needed, unrelated in
  cause) triggers a *pre-existing, separate* defect in functions
  combining a stack parameter (resolved via a real, confirmed
  `CPUI_SEGMENTOP`) with register-passed parameters, on this H8
  target's calling conventions.
- The bug is confirmed to be a `register:2` reuse conflation between
  two different `INDIRECT` call-site markers (`const:0x24` for
  parameter `b`'s entry, `const:0x4a` for a division's internal
  reuse of the same physical register) -- NOT the SEGMENTOP resolution
  itself, which works correctly.
- `Merge`-side call sites (`mergeOpcode`, `mergeMultiEntry`,
  `mergeAdjacent`, `hideShadows`) are all ruled out as the direct
  cause via direct evidence (each was individually diagnosed and shown
  not to be the origin of the `register:2`/`stack:0x6`-or-`b` bad
  pairing).
- Current leading theory: `Heritage`'s `bumpDeadcodeDelay`/restart
  mechanism, triggered by the newly-resolved SEGMENTOP LOAD appearing
  as a "truly new overlapping read" after a first heritage pass
  already ran dead-code elimination on the register space -- untested
  as of this handoff, diagnostic is written and ready.
- `9450` (unrelated far-pointer-join PR) is confirmed NOT part of the
  test build; all findings are isolated to `9561` alone.
- Multiple attempts to find a faster-than-`buildGhidra` iteration loop
  (isolated `.xml` fixtures, `decompile.exe load file`, swapping the
  fast-rebuilt native binary into the running instance, restarting
  Ghidra against it) were all tried and disproved or hit dead ends --
  `buildGhidra` + live MCP force_decompile remains the only reliable
  way to observe native changes. `decompile_test`/
  `decompileWin_x86_64Executable` remain useful ONLY as fast
  pre-flight compile checks before paying the `buildGhidra` cost.

- Confirm this theory with a targeted diagnostic in the
  `bumpDeadcodeDelay` call sites (`heritage.cc:2717`/`:2729`),
  logging `vn`'s address/space and `info->deadremoved` when it fires
  for `muldiv_u16`'s address range, to verify the restart is genuinely
  triggered by the newly-resolved SEGMENTOP LOAD (and not something
  else already reprocessing this space for an unrelated reason).
- If confirmed, investigate whether the *second* heritage pass's
  Varnode/Cover boundaries for the reused register genuinely differ
  from what a clean single pass would produce -- this may point at a
  fix in how `Heritage::rename`/`placeMultiequals` handle a restarted
  pass's residual state from the first, aborted pass, rather than
  anything in `RuleLoadVarnode`/`vnSpacebase` (9561) or `Merge`.
- Given the significant rebuild cost of each diagnostic iteration,
  batch several `bumpDeadcodeDelay`-area log points into one
  diagnostic pass rather than one-at-a-time, to reduce the number of
  `buildGhidra` cycles needed to confirm or refute this theory.

- The remaining possibility is that `Merge::merge` is called from a
  location not yet searched at all -- the earlier keyword search
  (`grep "merge\(high|merge\(.*High"` etc.) may have missed the real
  call site due to variable naming or indirection (e.g. called via a
  function pointer, a virtual method, or through `Funcdata`'s own
  wrapper rather than `Merge`'s methods directly). Worth a broader,
  less keyword-specific search across the whole `cpp/` directory for
  anything invoking `getMerge()` or constructing/calling into `Merge`.
- Alternative approach: rather than keep tagging individual call
  sites reactively, consider instrumenting `Merge::merge` to print
  a full call stack (platform-specific; may need `CaptureStackBackTrace`
  on Windows or similar) the first time it's called with our specific
  pair, to get the actual caller directly instead of eliminating
  candidates one at a time.
- Given the number of rebuild cycles this has taken, also worth
  reconsidering whether a synthetic, self-contained datatest fixture
  for `muldiv_u16` is worth another attempt (see "decompile_test
  isolation" notes below) to cut iteration time from ~4-8 minutes
  (full Ghidra rebuild) to under 2 minutes (native-only rebuild).

## decompile_test isolation attempt (2026-09-02, not yet successful)

Attempted to build a standalone `.xml` datatest fixture for
`muldiv_u16` (same format as `nestedsegmentop.xml`) to allow using the
much faster `decompile_test`/`ghidra_test.exe` native-only build
(~15s-1m19s) instead of `buildGhidra` (~4-8 min, and has repeatedly
exceeded the tool-call timeout in this session) for the remaining
diagnostic iterations.

Extracted the real 32 bytes for `muldiv_u16` via
`disassemble_bytes`/`disassemble_function` and assembled them into a
`bytechunk`, following `nestedsegmentop.xml`'s exact format. Running
this in isolation fails with `Bytes at <addr> are not mapped`, where
`<addr>` is always some offset past the declared 32-byte range,
proportional to the chosen base address. `get_function_xrefs` (calls
*into* `muldiv_u16`) shows 13 callers but no outgoing references,
consistent with the function being pure arithmetic with no calls or
data-table reads in its own body -- so the missing-memory reference is
likely implicit (e.g. a default stack region the harness doesn't
provide automatically the way `sat_add_u16`'s working test apparently
gets, possibly because `divxu.w`'s underlying pcodeop or the specific
calling convention triggers a lookup `sat_add_u16`'s simpler shape
does not).

A parallel attempt to extract real surrounding bytes directly from the
raw ROM file (`RVR_1998_x3 4g63t 21000011 md352553.hex`) using
`fileOffset = memAddress - min_address` (`min_address=0xee80` per
`get_current_program_info`) produced bytes that did **not** match the
already-confirmed disassembly at the expected offset -- meaning the
ROM file's raw layout does not map linearly to memory address via
`min_address` alone (likely bank-switched/paged addressing typical of
H8 firmware). Not pursued further given the mismatch.

**Not resolved.** Still relying on `buildGhidra` + live MCP connection
for the actual trigger/observe cycle; `decompile_test` is only being
used as a fast pre-flight compile-check before each full rebuild,
which is a smaller but still real time saving.

### Second attempt: decompile.exe with `load file` (also not successful)

Discovered `decompile.exe` alongside `ghidra_test.exe`/`sleigh.exe` in
the native build output -- this is the same interactive
`IfaceStatus`/console binary the datatests harness is built on, but
without the test-runner wrapper, and it registers extra
console-specific commands not available to datatests scripts:
`load file`, `addpath`, `save`, `restore` (see `consolemain.cc`).
`load file <filename> [<target>]` calls
`ArchitectureCapability::findCapability(filename)`, which raised the
possibility of loading the real ROM binary directly (avoiding both the
missing-memory problem and the raw-file offset-mismatch problem from
the first attempt, since the real file's bytes would all be present).

Tried `load file "<path to .hex>" H8:BE:32:H8539F:default` via an init
script (`decompile.exe -i script.txt`). This hung indefinitely with no
output at all (not even an error), and had to be killed manually.

Reading `ArchitectureCapability::findCapability`
(`architecture.cc:92`) shows capability matching is done via
`isFileMatch(filename)` -- almost certainly extension-based -- not via
the target/language string passed as the second argument (that string
is only used later, as `target`, once a capability has already
matched). It's likely no registered `ArchitectureCapability` in this
build recognizes a bare `.hex` extension as raw binary, and the load
call is blocking on something (possibly an interactive prompt on
stdin that the init-script mode doesn't satisfy) rather than failing
cleanly.

**Not pursued further.** Confirmed the process was fully killed
afterward (`Get-Process decompile` returns nothing). Reverting to the
proven `buildGhidra` + live MCP connection path for the next
diagnostic iteration (call-stack capture in `Merge::merge`).

**Real wins from this exploration, despite the `load file` dead end**:
confirmed `decompile_test`'s ~15s-1m19s compile-only rebuild is a
genuine, reliable way to catch syntax errors before committing to a
`buildGhidra` cycle (already in active use); cleared a disk-space
exhaustion issue (`C:` was down to ~1.6MB free at one point this
session, causing a `buildGhidra` failure unrelated to any source
change) that would otherwise have kept blocking builds silently.

### Third attempt: swap the fast-rebuilt decompile.exe into the already-running Ghidra instance (disproved)

Hypothesis: Ghidra's Java side spawns the native `decompile`/`decompile.exe`
as a subprocess for actual decompilation, and this session's Ghidra
instance appeared to be launched from the source tree directly (only
one `decompile.exe` exists anywhere under the repo, at
`Ghidra/Features/Decompiler/build/os/win_x86_64/decompile.exe`, the
exact `buildNatives.gradle`-produced output). If true, rebuilding just
this one native executable via the fast, existing
`:Decompiler:decompileWin_x86_64Executable` Gradle task (confirmed
~15s when up-to-date, ~1m10s on a real rebuild -- much faster than
`buildGhidra`'s 4-8 min) and leaving the already-running Ghidra
instance alone (no restart) might be enough to pick up native-side
diagnostic changes live.

Tested decisively: added a unique marker string
(`PATHTEST_MARKER_v2`) to the already-reliable `Merge::merge`
diagnostic (known to fire on every decompile of `muldiv_u16`, not
gated on the bug reproducing), rebuilt via
`decompileWin_x86_64Executable` only (no `buildGhidra`), cleared the
diagnostic output file, and force-decompiled `muldiv_u16` via MCP
against the **already-running, not-restarted** instance.

**Result: the marker did not appear.** `Get-CimInstance
Win32_Process -Filter "Name='decompile.exe'"` also showed no running
`decompile.exe` process at the time, consistent with the live instance
either using a separately-packaged/copied binary from a prior
`buildGhidra` run, or a different subprocess invocation path than
assumed. **This disproves the swap-in-place hypothesis.**

**Conclusion**: `buildGhidra` (or at minimum, a Ghidra restart against
freshly-copied native binaries) is genuinely required to observe
native-side changes live via MCP -- there is no shortcut around it
with the current setup. `decompileWin_x86_64Executable` (or
`decompile_test`) remain useful only as fast pre-flight
compile-and-logic sanity checks before paying the full `buildGhidra`
cost, not as a replacement for it. Marker strings removed from both
diagnostics after this test.

### Fourth attempt: restart Ghidra (not just swap the binary) against the fast-rebuilt decompile.exe (also disproved)

Re-tested the same idea, this time with an actual Ghidra restart (not
just leaving the old process running), on the theory that the running
instance might cache/lock its subprocess binary at startup and only
need a restart, not a full `buildGhidra`, to pick up a fresh native
build. This distinction matters because `h8539f.cspec` itself
documents this exact successful pattern for a past cspec-only change
(2026-07-22 R0R1 revert, confirmed via "a genuine full Ghidra restart
+ fresh ROM reopen").

Added a fresh marker (`RESTARTTEST_MARKER_v1`) to the same reliable
`Merge::merge` diagnostic, rebuilt via `decompileWin_x86_64Executable`
only (1m2s), then had the user fully restart Ghidra and reopen the
ROM before force-decompiling `muldiv_u16` again.

**Result: marker still absent**, even after a genuine restart -- the
diagnostic fired 9 times with real content, confirming the instance
*is* running compiled diagnostic code, just an older version that
predates this session's most recent `merge.cc` edits. This is a
stronger, more conclusive negative than the third attempt (which only
tested without a restart) -- it shows the discrepancy is likely a
build/packaging-location mismatch (this Ghidra instance is not reading
`decompile.exe` from `build/os/win_x86_64/` at all, contrary to what
"only one decompile.exe exists in the repo" suggested), not merely a
question of restart-vs-no-restart.

**Confirmed for good: `buildGhidra` is the only reliable way found so
far to get native changes observed live.** No further attempts at a
shortcut planned unless new evidence emerges about where the live
instance's actual binary comes from.

### Bonus finding from this test's diagnostic output: two independent merge chains, not one

The full `mergecall_diag.txt` capture from this test showed the
`register:2`/`stack:0x6` bad pairing is only one of *two separate*
merge chains active in this function, which happen to collide because
H8's calling convention reuses register numbers across unrelated
roles:

1. **Chain A (register 0, legitimate)**: `register:0` (the `BUILD` join
   at `0x141d7`, i.e. the real R0 return-value convergence point)
   progressively merges with `unique:0xaad02` (`SUBPIECE` division
   result, `0x141ce`) and then with `register:0`/`COPY` (`0x141d4`,
   the `0xffff` saturation value) -- this is the correct, intended R0
   join discussed in the second diagnostic pass above.
2. **Chain B (register 2, the bug)**: `register:2` (`DELAY_SLOT`,
   `0x141c2`) merges with `unique:0xa2700`/`0xa2702` (also
   `SUBPIECE`/`COPY` of the division's *other* half) and, separately
   and incorrectly, with `stack:0x6` (the real parameter).

These two chains don't fully overlap, but the presence of *two*
concurrent, unrelated merge chains both touching registers that
`divxu.w`'s 32-bit dividend (R0:R1 pair per H8 semantics, occupying
register offsets 0x0 and 0x2) writes through is a plausible reason the
decompiler's per-op merge logic ends up conflating storage that should
stay separate -- worth keeping in mind for whichever call site
eventually turns out to be the real culpr## SESSION 2026-09-05/06: clean-room confirmation, debugger-capture attempts (unsuccessful), protocol-mismatch discovery

### Clean-room test: confirmed the crash is real 9561, not investigation diagnostics

Five accumulated diagnostic files from prior sessions (`action.cc`,
`database.cc`, `heritage.cc`, `merge.cc`, `variable.cc` — instrumented
with `ofstream` logging at various points, including a static
`DIAG_callerTag` global in `merge.cc` that is a **known race hazard**
if the decompiler ever runs multi-threaded, since it's not
thread-local) were git-stashed, reverting the working tree to exactly
the official `9561` diff (`ruleaction.cc`/`.hh` + `varmap.cc`
comments only, confirmed against the upstream GitHub commit
`0deeeef32374b179eff5e8d734abd46bc1ad140b`). Rebuilt clean, reproduced
`muldiv_u16_scaled`'s crash via `force_decompile` against the clean
build.

**Result: the crash reproduces on clean, unmodified 9561, with zero
diagnostic instrumentation present.** This rules out every theory that
depended on the diagnostics themselves (or their interaction) causing
or masking the bug. The crash is real and belongs to 9561's actual
code change.

Also discovered during this pass: `xml.cc`, `grammar.cc`,
`pcodeparse.cc`, `slghparse.cc` also match a naive grep for
"DIAGNOSTIC" — false positive, this is Bison's own
`GCC diagnostic push/pop` boilerplate in generated parser code,
unrelated to anything in this investigation.

### Build/install verified NOT stale

Confirmed via `Get-FileHash` (SHA256) that
`github\ghidra\Ghidra\Features\Decompiler\build\os\win_x86_64\decompile.exe`
(gradle build output) and
`Downloads\ghidra_12.2_DEV_20260818\ghidra_12.2_DEV\...\decompile.exe`
(the copy the running instance actually loads) are byte-identical,
timestamps 84 seconds apart — confirming the user's install step
correctly copies the freshly-built binary and the live instance is
genuinely testing current patched source, not a stale/mismatched
build. This fully resolves the "which binary is actually running"
uncertainty that dominated the end of the 2026-09-04 handoff (the
third/fourth swap-in-place attempts' negative results were **not**
explained by this — they remain unexplained, see below).

### PyGhidra topology clarified

The live instance is not a standalone Ghidra GUI (`ghidraRun`/`java`/
`javaw`) — it's launched via `pyghidraRun.bat`, which runs Ghidra
embedded inside a Python process
(`...\venv\Scripts\python.exe -m pyghidra -g --install-dir ...`) via
an embedded JVM (JPype-style). There is no separate `java.exe`/
`javaw.exe` process at all; the Python process itself is the parent
that spawns `decompile.exe` as a native child. This explains why
earlier sessions' process-inspection assumptions (implicitly expecting
a `java`/`javaw` parent) never lined up cleanly — worth remembering for
any future live-attach attempt.

### Nine failed attempts to catch the crash under a debugger (all instructive, none successful)

Extensive effort was spent trying to reproduce the crash under `cdb`
(WinDbg's console debugger, installed via `winget install
Microsoft.WinDbg`, later also `C:\windbg`'s unpackaged classic `cdb`)
to get a real stack trace / exception record. In order, what was
tried and what was actually learned:

1. **`cdb -c "g;kb;r;u;q" decompile.exe` with commands piped to stdin
   via PowerShell `<`** — PowerShell does not support `<` redirection
   the way `cmd.exe` does; this silently produced no redirection at
   all.
2. **`Get-Content | & cdb.exe ...` (pipe-based)** — ran, but
   `decompile.exe` exited cleanly (clean `ExitProcessImplementation`
   stack, not a crash) every single time, regardless of what commands
   were fed.
3. **`cdb -i decompile_input.txt`** — wrong flag entirely: `cdb`'s own
   `-i` means "image search path" (symbol resolution), NOT a
   pass-through to the debuggee's argv. Confirmed via `cdb -?`. This
   invalidated two earlier attempts.
4. **`cdb ... -- decompile.exe -i ...`** — `--` is not a positional
   args separator for cdb; it's shorthand for `-G -g -o -p -1 -d -pd`.
   Produced an empty/broken invocation.
5. **`cmd /c "decompile.exe < input.txt"` launched as the debuggee
   under `cdb -o`** — correctly spawned `decompile.exe` as a real
   child process, cdb followed it, but the single-shot `-c "g;kb;r;u;q"`
   command string only fires once, at the *first* process stop
   (`cmd.exe`'s own entry), not at the grandchild's actual fault —
   process exited cleanly before ever reaching a second stop.
6. **Isolated stdin-piping test with a trivial known-good program**
   (`"hello" | cdb ... cmd.exe /c "more"`) — **this worked correctly**,
   proving the piping mechanism itself is sound. This was the turning
   point: if piping works for `more`, the clean exits with
   `decompile.exe` must be `decompile.exe` genuinely processing our
   input and exiting normally, not a plumbing failure.
7. **Root cause of the "clean exit" found**: `lo fu` is not a valid
   command abbreviation. The real command, confirmed directly from
   `ifacedecomp.cc`'s `registerCom` call, is the two-word `load
   function <name>`. Every previous attempt's input script was
   silently failing to parse past the first unrecognized command.
8. **Retried with correct `restore` / `load function
   muldiv_u16_scaled` / `decompile` commands** — **still exited
   cleanly**, same `ExitProcessImplementation` stack. This ruled out
   the command-syntax theory too.
9. **Tried live-attaching (`cdb -o -p <pid>`) to the actual
   PyGhidra-spawned `decompile.exe`, racing a `tasklist`-polling batch
   loop against `force_decompile`** — process lifetime is well under
   the polling loop's own latency (~tens of ms per `tasklist` call);
   never caught it.

**Resolution of the whole line of attempts**: `decompile.exe`'s
interactive console command loop (`restore` / `load function` /
`decompile`, typed commands via `cin`) is a **completely different
code path** from the binary framed protocol
`DecompileProcess.java`/`DecompileCallback` actually speaks to the
same executable when Ghidra drives it live (confirmed by reading
`DecompileProcess.java` directly — see below). Every manual
console-script attempt was correctly exercising `decompile.exe`, just
never touching the real crash path at all. This is why every syntactically-correct attempt still produced a clean voluntary exit — there was never going to be a crash on that path regardless of command wording.

### `DecompileProcess.java` read directly — confirms this is a real process death, not a generic wrapper message

`"Decompiler process died"` is thrown from exactly two sites in
`DecompileProcess.java`, both inside `readToBurst()`, both guarded by
`cur == -1` — i.e. genuine EOF on the pipe from `decompile.exe`'s
stdout while the Java side is still mid-protocol, expecting more
framed bytes. This is not a generic catch-all for any subprocess
hiccup or timeout; it specifically means the native process's stdout
pipe closed unexpectedly. This is a real, confirmed process
termination during live decompilation of `muldiv_u16_scaled`, driven
through Ghidra's actual binary protocol — separate from, and not
explained by, any of the manual console-script attempts above.

### WER LocalDumps configured, but never captured anything

With the user's help (elevated PowerShell), configured
`HKLM:\SOFTWARE\Microsoft\Windows\Windows Error Reporting\LocalDumps\decompile.exe`
(`DumpType=2` full dump, `DumpFolder=...\decompile_dumps`). Triggered
the crash live via `force_decompile` multiple times afterward. **No
dump file was ever written.** Also checked: Ghidra's own
`application.log` never logs anything about the crash either (frozen
at startup content across every check, several crashes apart); no
fresh Windows "Application Error" event log entry appears within
minutes of triggering the crash (checked via `Get-WinEvent`); no
temp-directory artifact appears either.

**This is a real, converging negative result across three independent
capture mechanisms**, and it means the working theory from the
2026-09-04 handoff (`0xc0000409`/`STATUS_STACK_BUFFER_OVERRUN`,
observed once from a *different* binary, `ghidra_test.exe`, on a
different day) is very likely **not the same crash** and should not
be assumed as this bug's mechanism going forward. A true unhandled
native exception reaching Windows' top-level handler would be
expected to produce at least one of: a WER dump (configured and
armed), a WER Application Error event, or a non-zero/exception-style
process exit visible to `DecompileProcess.java` in some
distinguishable way from a plain `EOF`. None of that happened. The
most likely remaining explanation is a **controlled termination**
inside `decompile.exe` — an uncaught C++ exception causing
`std::terminate()`, an explicit `abort()`/`exit()` call on some
error/assertion path, or Ghidra's own Java side deliberately closing
the pipe/killing the subprocess after detecting a protocol error on
its side — not a memory-safety fault.

### Live-attach to the real PyGhidra host process: incomplete, and a real operational incident

Attempted to attach `cdb -o` directly to the PyGhidra host process
(found via `Get-CimInstance Win32_Process` filtering on `pyghidra` in
the command line) so it would catch `decompile.exe` at the moment of
spawn with no polling race. This did attach successfully and the
bridge/MCP connection stayed responsive through one triggered crash,
but `-o` never reported a child-process creation event for
`decompile.exe` at all — either it's spawned by a different one of the
several `python.exe` PIDs under the pyghidra process tree than the one
attached to, or its lifetime is short enough to evade even `-o`'s
notification in practice.

**Incident**: while cleaning up, `Stop-Process -Id 10520 -Force` (the
PID previously observed for the detached `cdb.exe`) was run without
re-verifying that PID still belonged to `cdb` at the moment of
killing. This very likely killed the actual PyGhidra process instead
(PID reuse), taking down the live Ghidra instance entirely. **Lesson
for future sessions: never `Stop-Process` by a previously-observed PID
without re-checking `Get-Process -Id <pid> | Select ProcessName` (or
equivalent) immediately beforehand.** Ghidra was restarted via
`pyghidraRun.bat` and confirmed back to a working, responsive state
before continuing (`get_current_program_info` succeeded immediately
after restart).

### Source-level re-read of the 9561 patch itself (no bug found yet at this level)

With the debugger-capture approach stalled, re-read
`RuleLoadVarnode::vnSpacebase` (`ruleaction.cc:4179-4241`) and its
header declaration (`ruleaction.hh:796`) closely for a structural bug
(unbounded recursion, missing bounds check, etc.) rather than
continuing to chase a live capture:

- The `depth` parameter defaults to `0` in the header
  (`int4 depth=0`), so the 4-argument entry-point call at
  `checkSpacebase` (`ruleaction.cc:4276`) is legitimate C++, not a
  bug.
- The recursion has a real, correctly-placed depth guard
  (`if (depth > 8) return (AddrSpace *)0;` at line 4186) — no
  unbounded recursion, no obvious stack-overflow mechanism from this
  function alone.
- `wrapOffset` (`space.hh:385`, inline, pre-existing/unrelated to
  9561) is correct and properly bounds the accumulated offset to the
  space's width — not a source of corruption.
- **One genuine, if minor, contract concern identified**: in both the
  `unwrapped1`/`vn2` branch (lines 4219-4230) and the
  `unwrapped2`/`vn1` branch (4232-4239), `val` can be overwritten by
  the recursive call *before* the subsequent
  `if (vnN->isConstant())` check is evaluated. If that check fails,
  the function returns `(AddrSpace*)0` (a legitimate "no match"
  signal) but `val` has already been mutated to a value from the
  recursive branch — a caller relying on "`val` unchanged when return
  is NULL" would be misled. **Not yet confirmed as related to the
  crash** — no caller of `vnSpacebase`/`checkSpacebase` has been
  checked yet for whether it actually makes this assumption after a
  NULL return. This is the most concrete unresolved lead from this
  session and is the natural next step.

### Current state / what's actually confirmed vs. still open, going into the next session

**Confirmed:**
- Clean 9561 alone (no investigation diagnostics) reproduces
  `muldiv_u16_scaled`'s crash.
- The build/install being tested is genuinely current, hash-verified,
  not stale.
- The crash is a real, protocol-level process death
  (`DecompileProcess.java`'s EOF-on-pipe check), not a generic wrapper
  message.
- It is very likely **not** the `0xc0000409` stack-buffer-overrun
  seen once on a different binary on a different day — no evidence of
  a genuine unhandled native exception this session, despite three
  independent capture mechanisms being armed and the crash being
  triggered multiple times.
- `decompile.exe`'s interactive console-command protocol (`restore`/
  `load function`/`decompile`) is a different code path than the
  binary streaming protocol Ghidra's Java side actually uses; testing
  against the former does not exercise the real crash at all.

**Still open, in priority order for next session:**
1. ~~Trace `vnSpacebase`/`checkSpacebase`'s actual callers to see if
   any rely on `val` being unchanged after a NULL return~~ **RULED
   OUT (2026-09-06).** Checked both real callers
   (`RuleLoadVarnode::applyOp:4299`, `RuleStoreVarnode::applyOp:4340`).
   Both declare `offoff` fresh immediately before the call and
   immediately `return 0` on a NULL `baseoff`, before ever reading
   `offoff` on any branch. The value is dead on the NULL path in both
   cases — confirmed by reading the full function body, not just the
   call site. The mutated-on-NULL-return pattern inside `vnSpacebase`
   itself is still poor practice but has no observable effect here.
   Not the bug. Next session should not re-open this lead without new
## SESSION 2026-09-06 (cont'd): Java-side instrumentation — decisive result, exit(1), not a crash

Added diagnostics directly to `DecompileProcess.java`'s `sendCommand`
and `sendCommandTimeout` catch blocks (the two paths `force_decompile`
can route through), logging to
`C:\Users\j.brophy.CORKILLSYSTEMS\decompproc_diag.txt` on any
`IOException`: the command name, `nativeProcess.isAlive()`,
`nativeProcess.exitValue()` (when not alive), and
`timerMonitor.didRun()` (timeout path only). This required a Java
recompile via the user's normal gradle build/install cycle — no
native rebuild needed.

**Result, after rebuild and re-triggering `force_decompile` on
`muldiv_u16_scaled` several times:**

```
sendCommandTimeout FAILED command=decompileAt ... nativeProcess.isAlive=true exitValue=1 exceptionMsg=Decompiler process died timerMonitor.didRun=false
```

- `exitValue=1` is only obtainable at all once the process has
  actually terminated (`Process.exitValue()` throws
  `IllegalThreadStateException` while still running) — so despite the
  stale `isAlive=true` printed a moment earlier in the same log line
  (a benign timing artifact of evaluating `isAlive()` fractionally
  before `exitValue()`), the process **had genuinely exited with code
  1** by the time the diagnostic ran.
- **Exit code 1 is an ordinary, controlled process exit** — not a
  Windows exception/NTSTATUS code (which would appear as a large
  negative number, e.g. `-1073740791` for `0xC0000409`). This is
  consistent with a plain `exit(1)` call, or an uncaught C++ exception
  propagating out of `main()` and being translated to a generic
  non-zero exit by the CRT — **not a memory-safety crash.**
- `timerMonitor.didRun=false` on every occurrence confirms this is
  independent of Ghidra's own timeout mechanism.

**This changes the working model of the bug substantially.** Every
theory up to this point (heritage/merge register-reuse, stack-buffer
overrun, unbounded recursion) was implicitly assuming a genuine
memory-safety fault. The evidence now points at a **controlled,
intentional termination inside `decompile.exe` itself** — most likely
an uncaught C++ exception (possibly even one of `ruleaction.cc`'s own
`LowlevelError`/similar exception types, unhandled somewhere in the
decompile-command dispatch path) or an explicit `exit(1)` on some
error-detection path that the 9561 patch's recursive `vnSpacebase`
newly triggers for this function's specific SEGMENTOP/stack-parameter
shape.

**Next step:** find where `decompile.exe`'s command loop
(`consolemain.cc`'s `mainloop`, or the underlying command dispatch
that handles `decompileAt` — the actual protocol command name, not
the console command `decompile`) catches or fails to catch C++
exceptions, and check whether an uncaught exception thrown during
`RuleLoadVarnode::vnSpacebase`'s recursion (or a caller two or three
frames up) would propagate all the way out of `main()` uncaught,
producing exactly this exit(1) signature. This is a much more
tractable lead than the native-debugger-attach approach from earlier
in the session, since it points at a specific, findable place in the
C++ exception-handling structure rather than requiring a live crash
capture at all.

## Still open from the 2026-09-04 handoff (kept for reference)

2. Get a real stack trace / exception detail from the actual crash.
   Since the console-protocol approach is now known to be a dead end,
   the only remaining reliable options were: (a) attach a debugger to
   the correct child `decompile.exe` process at the moment Ghidra's
   real binary protocol spawns it (requires solving the sub-second
   lifetime / attach-race problem — possibly via
   `gflags /i decompile.exe +hpa` (page heap) or an image file
   execution options debugger auto-attach, both of which attach
   *before* the process starts running rather than racing after
   spawn), or (b) instrument `DecompileProcess.java`/
   `DecompileCallback` (or the equivalent PyGhidra-facing Java) to
   dump whatever partial output/state it has right before throwing
   "Decompiler process died", since it's Java-side and doesn't
   require solving the native-attach timing problem at all. **This
   was done — see the "Java-side instrumentation" section above,
   which superseded this item with a decisive result (exit code 1,
   not a crash).**
3. Given how much of this session was spent on debugger/process
   plumbing without a capture, option 2(b) — instrumenting the Java
   side, not the native side — turned out to be the highest-value
   step, exactly as predicted here.

## Still open from the 2026-09-02 sessions (kept for reference)

- Confirm which `Action` actually performs the incorrect merge (add
  debug instrumentation to `ActionMergeCopy`/`Heritage` gated to
  these specific functions' addresses, the same disciplined way the
  original `vnSpacebase` investigation did -- see `review17.md`'s
  tooling notes).
- Determine whether the fix belongs in heritage/merge logic (general,
  affects all targets) or is specific to this H8 sleigh spec's mixed
  register/stack calling convention definitions.
- Check whether `muldiv_u16_shr8_sat` and the remaining `mul_u16_*`
  family (visible in the function list, not yet checked) show the
  same pattern -- would help confirm the "stack param + register
  param" precondition more rigorously.
- Once root cause is confirmed, decide whether the fix ships as its
  own separate PR (referencing this doc and #9561), gated behind a
  clean regression test the same way `nestedsegmentop.xml` was built
  for `9561`.

## Relationship to 9561 / #9561

This regression blocks `9561` from safely coming out of draft as-is:
trading the merged-array symptom for a silently wrong computed return
value is a worse outcome for the affected functions, not a strict
improvement. `9561`'s own fix is not being reverted or changed based
on this -- the merge-array resolution it performs is correct and
necessary -- but it should not ship until this triggered regression is
understood and either fixed or the trigger is avoided.

## SESSION 2026-09-06 (cont'd, part 3): exception-handling analysis rules out uncaught LowlevelError; IFEO capture attempt failed and was reverted

### `GhidraCommand::doit()` already catches Ghidra's entire custom exception hierarchy

Traced `decompileAt`'s real dispatch path: `ghidra_process.cc`'s
`commandmap["decompileAt"] = new DecompileAt()`, whose `rawAction()`
calls `ghidra->allacts.getCurrent()->perform(*fd)` directly, with
**no try/catch of its own** -- so any exception thrown deep inside
the decompile pipeline (including `RuleLoadVarnode::vnSpacebase`'s
recursion) propagates straight out of `rawAction()`.

However, the actual top-level caller, `GhidraCommand::doit()`
(`ghidra_process.cc:124-154`), **does** wrap `loadParameters()` /
`rawAction()` / the whole command sequence in a try block catching
`DecoderError`, `JavaError`, `RecovError`, and `LowlevelError`. Read
`error.hh` directly: `RecovError` and `ParseError` both derive from
`LowlevelError`, so this catch coverage is Ghidra's **entire** custom
exception hierarchy -- there is no gap here. Confirmed via
`ruleaction.cc`: `vnSpacebase`/`checkSpacebase` themselves throw
nothing at all (only `LowlevelError`s exist elsewhere in the file, at
unrelated lines: 734/741/747/750 relacedescendants, 788 pullsub, 3916
self-defined varnode, 9033 segment-operand-missing). No `assert()`
calls anywhere in `ruleaction.cc` either.

**This rules out the "uncaught Ghidra LowlevelError" theory from the
previous session.** Any `LowlevelError`-family exception thrown
anywhere in the decompile pipeline would be caught safely by `doit()`
and reported back as a normal error response, not crash the process.

### Fast-fail confirmed via short timeout test

`ghidra:decompile_function` with `timeout=5` on `muldiv_u16_scaled`
produced the identical `"Decompiler process died"` failure almost
immediately -- not a timeout-specific message (which is a different,
already-confirmed-distinct code path: `DecompileException("process",
"timeout")` in `DecompileProcess.java`). This confirms the process
dies **fast and deterministically**, not from a slow, CPU-bound
infinite loop or deep unbounded recursion eventually exhausting the
stack. A quick, early-failing check is more consistent with this
timing than a slow stack-exhausting recursion.

### `Heritage`/`Merge` iterative structures checked for missing recursion guards -- nothing found

Searched `heritage.cc` for recursive functions with the P-code
evidence's implicated `Heritage::rename`/`placeMultiequals` in mind.
The `depth` terminology found (`PriorityQueue`, `buildDomDepth`) is
all **iterative** (a priority-queue-based dominator-tree walk), not
recursive -- no missing depth guard candidate found here. This line
of inquiry did not converge; not ruled out entirely, just not
confirmed either.

### Remaining leads at end of this pass (unresolved)

Given Ghidra's own exception hierarchy is fully caught and
`vnSpacebase` itself has no throw/assert and a correct depth guard,
the two remaining live theories are:
1. A genuine OS-level fault (segfault, stack overflow from a
   *different*, not-yet-identified recursive function elsewhere in
   the pipeline) that bypasses C++ exception handling entirely.
2. An uncaught **non-Ghidra** exception type (e.g. `std::bad_alloc`,
   `std::out_of_range`, `std::length_error` from a vector/string
   operation) that isn't one of `doit()`'s four explicit catch
   clauses and isn't a bare unadorned `catch(...)` either -- worth
   checking whether `doit()` has a catch-all fallback at all (not yet
   checked this session).

### IFEO (IFEO debugger auto-attach) attempted -- failed, reverted, caused a real disruption

Attempted `HKLM\...\Image File Execution Options\decompile.exe`'s
`Debugger` value to auto-attach `cdb` at process creation (avoids the
entire sub-second-lifetime attach-race problem that defeated all nine
attempts in the previous session, since IFEO attaches *before* the
process runs any code at all). This needed the user's elevated
PowerShell access three separate times as the command was refined:

1. First attempt (`cdb -g -G -c "..."`, no output redirection): the
   registry value was set and did engage (confirmed via a live
   `force_decompile` trigger), but produced no readable output at all
   since cdb's own console output was never captured to a file.
2. Second attempt added `cmd.exe /c "... > file 2>&1"`, written via a
   `.bat` file calling `reg add`: the `.bat`'s own caret-escaping
   (`^>`, `2^>^&1`, meant only for the `.bat`'s own `cmd.exe`
   parsing) was written **literally into the registry value itself**
   -- confirmed via `reg query` showing literal `^` characters
   stored. This produced no output file and a different, blank-ish
   error message on the next trigger.
3. Third attempt used `Set-ItemProperty` directly (bypassing
   `reg.exe`'s CLI quoting entirely) to set a clean, un-escaped value
   -- this correctly stored the intended command with proper
   `>`/`2>&1` redirection syntax (confirmed via `reg query` showing
   no carets). However, triggering the crash at this point produced a
   **visible Ghidra GUI error dialog** ("Failed to launch Decompiler
   process... 'C:\windbg\amd64\cdb.exe" -g -G -c "g' is not
   recognized as an internal or external command"), showing that
   **Windows' IFEO mechanism does not parse the Debugger value with
   `cmd.exe`-style quoting semantics** -- it appears to tokenize on
   the first embedded quote/space boundary in a simpler way, cutting
   the command off mid-argument and dropping the `cmd.exe /c`
   wrapper entirely, regardless of how correctly the value itself was
   stored.

**This broke normal (non-crashing) Ghidra decompiles while IFEO was
active** -- the error dialog appeared as a real, visible disruption
to the user's own Ghidra session, and Ghidra's UI became unresponsive
to MCP calls afterward (`get_current_program_info` timed out
repeatedly), most likely because the modal error dialog was blocking
the Swing UI thread. The user manually re-ran the disable script
(`decompile_ifeo_disable.bat`, confirmed via `reg query` returning
"unable to find the specified registry key or value" afterward) and
manually dismissed/cleared whatever was blocking the UI; Ghidra was
confirmed responsive again by the end of this session.

**IFEO is not a lead worth returning to without solving the value's
quoting problem first.** Windows' simpler-than-`cmd.exe` parsing of
the `Debugger` value means any command needing nested quotes (like
ours, which needs `cdb`'s own `-c "..."` argument) is fragile at
best. If retried, a much simpler `Debugger` value with **no nested
quotes or redirection at all** (e.g. just `cdb.exe` alone, capturing
output via cdb's own `-logo <file>` flag instead of shell
redirection) is more likely to survive IFEO's parser intact.

### Current state at the end of this session

**Confirmed, in addition to prior sessions' findings:**
- Ghidra's own `LowlevelError`/`RecovError`/`ParseError` exception
  hierarchy is fully caught by `GhidraCommand::doit()` -- ruled out as
  the uncaught-exception source.
- `vnSpacebase`/`checkSpacebase` throw nothing and have no `assert()`
  calls -- not the direct throw site even if some other exception
  theory holds.
- The crash is fast and deterministic (confirmed via a 5-second
  timeout test), not a slow/looping failure.
- IFEO debugger auto-attach is not currently viable with a
  quote-containing `Debugger` value; a simpler, quote-free command is
  needed if this approach is retried.

**Still open, in priority order for the next session:**
1. Check whether `GhidraCommand::doit()` (or any wrapper further up
   the call stack, e.g. in `ghidra_process.cc`'s main loop) has a
   bare `catch(...)` fallback that might itself be doing something
   unusual (e.g. deliberately calling `exit(1)` on catching an
   unknown exception type) -- this would directly explain the
   observed `exitValue=1` without needing any further live capture at
   all. Not yet checked this session; check `ghidra_process.cc`'s
   `main()`/command-loop function next.
2. If no such fallback exists, retry IFEO with a quote-free
   `Debugger` value (e.g. plain `cdb.exe -logo <file>` with no `-c`
   argument at all, driving cdb interactively isn't compatible with
   IFEO's non-interactive launch, so this may need `windbgx.exe`'s
   command-line logging flags instead, or a wrapper `.exe`/compiled
   stub rather than a `.bat`/shell one-liner, to avoid IFEO's parser
   entirely).
3. Alternatively, consider adding a raw `catch(...)` block temporarily
   around `rawAction()`'s call in `DecompileAt`, logging
   `typeid(...)`/`what()` (if a `std::exception`) before rethrowing --
   this would definitively identify whether it's a `std::` exception
   type without needing any debugger capture at all, and is a much
   smaller, safer diagnostic than IFEO. This is probably the
   highest-value next step given how disruptive IFEO turned out to
   be.

## SESSION (continued): ROOT MECHANISM FOUND -- segvHandler explains everything

### `ghidra_process.cc`'s own SIGSEGV handler intentionally disguises crashes as exit(1)

Read `ghidra_process.cc`'s `main()` directly (line 511 onward). The
very first thing it does, before anything else:

```cpp
static void segvHandler(int4 sig)
{
  _Exit(1);	// Don't do any cleanup, just die - prevents OS from popping-up a dialog
}
...
int main(int argc,char **argv)
{
  ...
  signal(SIGSEGV, &segvHandler);  // Exit on SEGV errors
  ...
}
```

**This fully explains every negative result from the rest of this
session and the previous one:**

- `_Exit(1)` is a hard, immediate process termination -- no C++ stack
  unwinding, no destructors, no exception propagation. This is why
  `GhidraCommand::doit()`'s exhaustive `LowlevelError`/`RecovError`/
  `DecoderError`/`JavaError` catch coverage was irrelevant: a SIGSEGV
  never goes through C++ exception handling at all, so there was
  never a "gap" in the catch clauses to find.
- `_Exit()` deliberately skips any of the normal OS crash-reporting
  paths ("prevents OS from popping-up a dialog") -- this is exactly
  why WER LocalDumps (configured and armed earlier this session)
  never produced a dump, why no Windows "Application Error" event
  ever appeared, and why `ghidra_test.exe`'s **one-off**
  `0xC0000409`/`STATUS_STACK_BUFFER_OVERRUN` WER event from several
  days ago was the outlier, not the disguised-crash pattern -- that
  earlier crash presumably happened via a path that didn't have this
  handler active, or before this handler could intercept it, or is a
  genuinely different bug. This session's crash goes through this
  handler every time, so WER never sees it.
- Exit code hardcoded to exactly `1` -- an exact, deterministic match
  for `nativeProcess.exitValue()=1` observed via the
  `DecompileProcess.java` instrumentation earlier this session. This
  is not a coincidence; this is confirmation.
- The crash being fast and deterministic (confirmed via the
  short-timeout test) is fully consistent with a genuine SIGSEGV --
  memory access violations fault immediately, they don't take time to
  build up.

### Conclusion: this IS a genuine memory-safety bug (real SIGSEGV), not a controlled exit(1)

The previous session's conclusion ("controlled termination... not a
memory-safety fault") is **superseded and reversed** by this finding.
`exit(1)` was never a plain, ordinary process exit chosen by
application logic -- it's `segvHandler`'s disguise for a genuine
segmentation fault. The stack-buffer-overrun/memory-corruption theory
from the very first (2026-09-04) session is back on the table, now
with a confirmed, exact mechanism explaining why every subsequent
non-crash-dump-based investigation avenue (WER, event log, Ghidra's
Java-side log, C++ exception hierarchy) necessarily came back empty.

### Immediate implication for next steps

Any future debugger-attach attempt needs to **not rely on WER,
Application Error events, or normal exception-based capture** --
those are all confirmed dead ends by design, not by bad luck. The
correct approach is a debugger attached *before* the SIGSEGV fires
(live attach, or IFEO -- both already attempted with mixed results
this session) using a debugger that **intercepts the signal itself**
before `segvHandler` gets to run `_Exit(1)`. In cdb terms: cdb's
default first-chance-exception handling for access violations
(`sxe av` is enabled by default) should, if properly attached at the
moment of fault, break *before* the SIGSEGV signal handler (Windows
translates SIGSEGV to `STATUS_ACCESS_VIOLATION` at the OS/SEH level,
and a debugger attached to the process intercepts the exception at
the OS level, upstream of the C runtime's own `signal()`-registered
handler, in the normal case). This means a correctly-configured live
attach or IFEO session (fixing IFEO's quoting problem, per the
previous section's notes) should still be able to catch this at the
real fault point rather than at `segvHandler`'s already-translated
`_Exit(1)`, since the debugger's break happens before user-mode signal
handling in the standard Windows exception dispatch order.

**Recommended concrete next step**: retry IFEO with a minimal,
quote-free `Debugger` value (per the previous section's specific
recommendation -- e.g. a compiled wrapper stub, or `cdb.exe` with no
`-c` argument at all so it drops to its interactive prompt on
first-chance exception, letting the user manually inspect `kb`/`r`/`u`
at that point rather than needing a scripted one-shot command) --
this is now a well-motivated experiment rather than a speculative one,
since we know definitively that a genuine, catchable SIGSEGV occurs
before `segvHandler` runs.

## SESSION (continued): second IFEO attempt also failed -- IFEO ruled out entirely going forward

Retried IFEO with the simplest possible `Debugger` value (no `-c`
script, no redirection, no nested quotes at all -- just
`C:\windbg\amd64\cdb.exe`, set via `Set-ItemProperty` to avoid any
`reg.exe`/`.bat` quoting problems). Confirmed via `Get-ItemProperty`
that the registry value was stored exactly as intended, cleanly, with
no corruption this time.

**Result: no visible cdb console window ever appeared** on the
desktop when the crash was triggered, and Ghidra stopped decompiling
*anything* afterward (not just the target crashing function -- all
decompiles silently failed to complete). This strongly suggests cdb
did attach and stop the debuggee at its entry breakpoint (IFEO's
normal behavior) but its console window did not surface visibly to
the user -- possibly a session/desktop visibility issue (the
decompile.exe child process may run in a different Windows session or
window station than the interactive desktop, common for processes
spawned by a GUI application's subprocess machinery), leaving every
`decompile.exe` launch permanently stuck at the entry breakpoint,
invisible, consuming a handle/process slot without ever completing.
This matches "nothing decompiles now" exactly.

Disabling IFEO (`decompile_ifeo_disable.bat`) immediately restored
normal operation, confirmed via `get_current_program_info` succeeding
again.

**IFEO is now ruled out as a usable technique in this environment,
having failed in two different configurations across two sessions:**
1. A `-c`-scripted value: broke due to IFEO's simplistic
   (non-`cmd.exe`-style) argument parsing of quoted values.
2. A bare, quote-free value: attaches (probably) but its console is
   not visible/reachable on the interactive desktop, silently hanging
   every future launch of `decompile.exe` until manually disabled.

**Do not retry IFEO again without first solving the window-visibility
problem specifically** (e.g. confirming what session/window station
`decompile.exe` children actually run in, and whether cdb can be
forced to attach with output redirected to a named pipe or file
instead of relying on an interactive console surfacing on the visible
desktop at all). Given two consecutive failures each requiring a full
Ghidra restart to recover from, this approach has a poor cost/benefit
ratio compared to the alternatives below.

### Recommended alternative for next session: non-invasive live attach, or WinDbg's remote/pipe-based debugging

Rather than IFEO, the safer options for confirming the SIGSEGV theory
directly are:
1. A tight, low-latency polling loop attempting `cdb -pn decompile.exe`
   (attach by name, non-invasively, `-pv`) started just before
   triggering `force_decompile` -- the previous session's polling
   attempts failed due to per-`tasklist`-call latency; a tighter loop
   using `Get-Process -Name decompile -ErrorAction SilentlyContinue`
   in a fast PowerShell `while` loop (no external process spawn per
   iteration, unlike `tasklist.exe`) may be fast enough to catch even
   a sub-second-lived process, since `Get-Process` is a much lower-
   overhead, in-process cmdlet call.
2. Alternatively, accept that a full crash-dump capture may not be
   worth further time investment given how much of this and the
   previous session it has consumed, and instead focus purely on
   static analysis: since the theory is now confirmed to be a genuine
   SIGSEGV (not a logic bug), the next most productive step may be
   re-reading `RuleLoadVarnode::vnSpacebase`'s actual pointer/array
   accesses (not just its recursion/depth guard, already cleared)
   with fresh eyes specifically hunting for a raw pointer dereference,
   array index, or buffer write that could go out of bounds given
   `muldiv_u16_scaled`'s specific parameter shape (three plain
   Stack[]-only parameters, no register parameters at all -- a
   genuinely different shape than `muldiv_u16`'s wrong-return-value
   case, as already noted in the 2026-09-03 section above). This
   avoids all further debugger/IFEO risk entirely.

## SESSION (continued): fast Get-Process-based polling attach -- conclusive negative result, debugger-capture approach closed out

### Attempt: PowerShell-native polling loop (Start-Process, not Start-Job)

Learned mid-session that `Start-Job`'s background jobs die with the
calling shell -- since each `run-command` invocation spawns and tears
down its own PowerShell process, a `Start-Job`-based watcher never
survives past the tool call that started it (confirmed: zero output,
`Get-Job` couldn't even find the job on the next call). Switched to
`Start-Process powershell.exe -File watch_fast.ps1 -WindowStyle Hidden`,
which genuinely detaches and survives independently. This is the
correct pattern for any future background-watcher attempt from this
environment.

The watcher script used `Get-Process -Name decompile` in a tight loop
(in-process cmdlet call, far lower overhead than spawning
`tasklist.exe` per iteration as the previous session's attempt did),
attaching `cdb -p <pid> -c "g;kb;r;u;q"` the instant a match was
found.

### First run: caught a stale, unrelated decompile.exe (learned to re-verify before trusting a catch)

The first trigger caught **PID 8396, alive since 6:36:26** -- several
minutes before the 6:39:24 crash trigger. This was a leftover orphan
process (most likely from an earlier IFEO experiment this session
that left `decompile.exe` stuck at an entry breakpoint, invisible,
never cleaned up). Verified process names directly
(`Get-Process -Id <pid> | Select ProcessName`) before killing both the
stale `decompile.exe` and its attached `cdb.exe`, learning from the
earlier PID-reuse incident. Ghidra confirmed still healthy afterward.

**Lesson for future sessions**: always check the attached process's
`StartTime` against the trigger time before trusting a catch -- a
successful-looking attach can still be to the wrong, stale process.

### Second run, after cleanup: definitive proof the process dies too fast for any external tool to catch

Re-ran cleanly. This time cdb's own attach attempt failed with:

```
Cannot debug pid 13208, NTSTATUS 0xC000010A
    "An attempt was made to access an exiting process."
Debuggee initialization failed, NTSTATUS 0xC000010A
```

**This is the single most conclusive piece of evidence in the entire
investigation's debugger-capture effort.** `Get-Process` genuinely
detected `decompile.exe` (PID 13208) as running, but by the time
`cdb.exe` -- launched immediately afterward, no meaningful delay --
tried to open a handle to it, the process had *already exited*. The
gap between "process observed to exist via a live OS API call" and
"process has already fully exited" was smaller than the time needed
to spawn and initialize `cdb.exe` itself.

**This proves definitively that no external polling-based attach --
however optimized -- can win this race.** The process's total
lifetime (from creation to `segvHandler`'s `_Exit(1)`) is shorter than
the minimum latency of spawning any external debugger process to
attach to it, even when the watcher script's own detection latency is
reduced to near zero (a single in-process `Get-Process` call, no
`tasklist.exe` subprocess spawn). This closes out the entire
"catch it externally after the fact" family of approaches (IFEO,
live-attach polling, WER, event logs, Java-side instrumentation) as
fundamentally unable to observe the fault site directly, for
structural timing reasons, not tooling mistakes.

### Debugger-capture approach formally closed out for this bug

Across two full sessions, every external-observation technique
attempted is now conclusively exhausted:
- WER LocalDumps: configured correctly, never fires (`segvHandler`'s
  `_Exit(1)` deliberately bypasses it -- confirmed via source read).
- Windows Application Error events: never appear, same reason.
- Ghidra's own Java-side logging: never captures anything (native
  process death is invisible to `application.log`).
- `DecompileProcess.java` instrumentation: successfully captured
  `exitValue=1`, `isAlive` state, and ruled out the timeout path --
  useful, but cannot get further than the Java side's own visibility
  into the *already-exited* process.
- C++ exception hierarchy (`LowlevelError` family): confirmed fully
  caught by `GhidraCommand::doit()`; not the mechanism.
- IFEO auto-attach: two attempts, both failed for different reasons
  (quoting parser mismatch; then window-visibility/hang), each
  requiring a full Ghidra restart to recover from.
- External polling-based live attach (this session): now proven,
  with direct NTSTATUS evidence, to be structurally too slow --
  the process exits before an externally-launched debugger can even
  open a handle to it.

**Given this, achieving a live crash/stack-trace capture for this bug
is not practical with any external-tool approach in this environment.**
The only remaining paths that could still work, roughly in order of
promise, are:
1. **In-process instrumentation compiled directly into `decompile.exe`
   itself** -- e.g. temporarily replacing `signal(SIGSEGV,
   &segvHandler)` with a custom handler that, before calling
   `_Exit(1)`, captures and writes `ucontext_t`/`CONTEXT` register
   state and a manual stack walk to a file synchronously, from
   *within* the same process, with no external attach/timing race at
   all. This is a source change to `ghidra_process.cc`, requires a
   `buildGhidra` cycle, but sidesteps every timing problem
   encountered so far since the capture code runs in the same
   crashing process, synchronously, before exit.
2. Pure static analysis of `vnSpacebase` and its actual callers/
   surrounding pipeline for `muldiv_u16_scaled`'s specific
   all-stack-parameter shape (as already suggested in the previous
   section) -- zero risk, no debugger/timing concerns, but slower and
   less certain to converge than a direct in-process capture.

**Recommended next step for a future session: implement option 1** --
modify `segvHandler` itself (`ghidra_process.cc:503-507`) to capture
and log real fault context before exiting, since this is now clearly
the only reliable way to get a stack trace for this specific bug,
given everything else has been conclusively ruled out this session.

## SESSION (continued): IN-PROCESS SIGSEGV CAPTURE SUCCEEDED -- real stack trace obtained, real disassembly at fault frames

### segvHandler instrumentation worked on first rebuild

Modified `segvHandler` (`ghidra_process.cc:503-`) to call
`CaptureStackBackTrace` and write the raw return addresses (plus,
after a refinement, the module's own base address and each frame's
module-relative offset) to
`C:\Users\j.brophy.CORKILLSYSTEMS\segv_diag.txt` synchronously, before
`_Exit(1)`. Required `#include <windows.h>` (with `#define NOMINMAX`
first, to avoid `min`/`max` macro collisions with any STL usage
elsewhere in the same translation unit) inside the existing
`#ifdef _WINDOWS` block.

**This worked immediately on the first rebuild+trigger cycle** --
confirmed `sig=11` (`SIGSEGV`, exactly as expected) and a real,
21-frame call stack, with zero timing race, since the capture code
runs synchronously inside the crashing process itself before it exits.
This is now the standing, reusable diagnostic for this bug (and
structurally, for any future native crash in this codebase) -- much
more reliable than any external-attach approach tried previously.

### Real captured stack (module-relative offsets, decompile.exe, no ASLR ambiguity)

```
frame[0]  = module_offset=0x3f449    (fault site itself)
frame[1]  = module_offset=0x2116fe
frame[2]  = module_offset=0x22d7f6
frame[3]  = module_offset=0x201270
frame[4]  = ntdll (signal dispatch machinery, not decompile.exe)
frame[5]  = ntdll
frame[6]  = ntdll
frame[7]  = module_offset=0x141eee
frame[8]  = module_offset=0x142566
frame[9]  = module_offset=0x142d8c
frame[10] = module_offset=0x142fe2
frame[11] = module_offset=0x1bc21a
frame[12] = module_offset=0x109c33   <-- repeats at frame[14]: genuine recursion
frame[13] = module_offset=0x10aa3f
frame[14] = module_offset=0x109c33   <-- same address as frame[12]
frame[15] = module_offset=0x3b461
frame[16] = module_offset=0x397a1
frame[17] = module_offset=0x40366
frame[18] = module_offset=0x1ff66f
frame[19] = ntdll
frame[20] = ntdll (KERNEL32 thread-start trampoline)
```

No `.pdb` exists for this build (confirmed: `Get-ChildItem` for
`decompile.pdb` in the build output returns nothing), so `ln`
(list-nearest-symbol) against the static, offline-loaded binary
(`cdb -z <path>`, zero risk, no running process needed at all) returns
nothing for any of these offsets -- there is no symbol table to
resolve names from. Fell back to raw disassembly (`u`) at each
offset, which still yields real, useful information.

### Disassembly at the two most informative frames

**`decompile+0x109c33`** (the frame that repeats -- proof of genuine
recursion at this exact return address):

```asm
mov  eax, dword ptr [rbx+14h]
test al, 3
jne  decompile+0x109d20
inc  dword ptr [rbx+1Ch]
mov  eax, dword ptr [rbx+0Ch]
mov  dword ptr [rbx+8], eax
mov  rax, qword ptr [rbx]        ; load vtable pointer from *rbx
mov  rdx, rbp
mov  rcx, rbx
call qword ptr [rax+30h]         ; virtual call, vtable slot 0x30 -- return addr = 0x109c33
test eax, eax                    ; <- fault-adjacent return point
js   decompile+0x109d17
```

This is a **virtual function call** (C++ vtable dispatch through
`[rax+30h]`) on an object pointed to by `rbx`, with a `test al,3`
tag-bit check beforehand (classic pattern for checking flag bits
packed into a pointer or small-int field) and an increment of a
counter field (`[rbx+1Ch]`) immediately after -- consistent with
iterator/traversal bookkeeping (e.g. an operand-index or
recursion-depth counter) on some node-like object, most plausibly a
`Varnode`/`PcodeOp`/`AddrSpace`-family object given this call chain
originates from `vnSpacebase`'s recursive descent.

**`decompile+0x3f449`** (the actual fault-adjacent frame, i.e. what
was executing / about to execute when the SIGSEGV occurred):

```asm
mov  qword ptr [rsp+340h], rax
mov  ebx, ecx
lea  r8, [rsp+50h]
xor  ecx, ecx
xor  r9d, r9d
mov  edx, 3Eh
call qword ptr [decompile+0x232018]   ; call through an IAT/import-style thunk
xor  r14d, r14d                        ; <- fault site itself is right after this call returns
lea  rcx, [decompile+0x242c38]         ; loading a static data address (possibly a format string
                                        ;   or type-info table -- 0x242c38 is in .rdata range)
xor  r9d, r9d
movzx esi, ax
...
mov  r8d, 3
```

The `call qword ptr [decompile+0x232018]` immediately before the
fault site is a call through a fixed pointer slot (likely an IAT
entry to a CRT or Windows API function, given the calling convention
setup with `edx=0x3E` (62) as an apparent parameter). This is
consistent with something like a `malloc`/`operator new`-style
allocation call, or a CRT string/formatting function, immediately
preceding whatever accesses invalid memory next.

### Assessment: genuine recursive traversal through a virtual-dispatch structure, faulting inside or shortly after an allocation-like call

This is now real, hard evidence rather than inference from P-code
alone: the crash involves (a) confirmed recursion through the same
return address twice, at a call site dispatching through a C++
vtable on what's likely a `Varnode`/`PcodeOp`-family object, and (b)
a fault occurring shortly after a call through a fixed
function-pointer slot resembling an allocation or CRT call. This is
compatible with, though not yet definitively pinned to, a
stack-overflow-from-deep-recursion theory (revisiting the very first
session's suspicion) if the recursion this reveals goes significantly
deeper than the 21 frames captured here (note:
`CaptureStackBackTrace`'s `62`-frame limit was NOT exhausted -- only
21 real frames were returned before hitting `ntdll`/thread-start, so
this is NOT a simple stack-overflow-from-unbounded-recursion in the
naive sense; the actual recursion depth at fault time is genuinely
shallow, only 2 confirmed levels via the repeated frame). This makes
a plain stack-exhaustion theory **less likely** than a **heap
corruption or invalid-pointer dereference reached via this
particular vtable-dispatched call chain**, given the fault happens
with only a modest, non-exhausting stack depth.

### Recommended next step: map these module offsets to actual source locations via a debug-symbol-enabled build

Since this build has no `.pdb`, the most direct way to turn these
offsets into real function names/source lines is to produce a debug
build (or at minimum, ensure PDB generation is enabled for the
Windows native build -- check `buildNatives.gradle`/the relevant
MSVC linker flags for `/DEBUG` and `/Zi`) and repeat this exact
capture once a matching `.pdb` exists, so `ln`/`lm`/symbol-qualified
disassembly can resolve real function names directly instead of
requiring manual reasoning from raw disassembly bytes as done in this
session. Given the C++ source is fully available, an alternative
faster path (no rebuild needed) is to manually correlate these
offsets against the *disassembly of the equivalent addresses in a
`ghidra_test.exe`/other same-build sibling binary opened in Ghidra
itself* (self-referentially decompiling the tool's own crash!),
since Ghidra can label recognizable runtime/library calls and
structure layout even without a PDB, likely faster than a full
rebuild-with-symbols cycle.

## SESSION (continued): ROOT CAUSE LOCATED -- Merge::trimOpOutput, via Merge::mergeOp / mergeIndirect / mergeMarker / mergeMultiEntry

### PDB-enabled rebuild + offline symbol resolution gave a full, named call stack

Enabled debug symbols for the native build: uncommented the three
pre-existing (but disabled) lines in
`Ghidra/Features/Decompiler/buildNatives.gradle`:

```groovy
b.cppCompiler.args "/Zi"
b.cppCompiler.args "/FS"
b.linker.args "/DEBUG"
```

Rebuilt via the user's normal gradle process. Confirmed
`decompile.pdb` (54MB) now exists alongside `decompile.exe`. Re-ran
the `segvHandler` capture (same instrumentation as the previous
entry) to get fresh module-relative offsets from this new build, then
resolved them offline via `cdb -z <path-to-decompile.exe-with-pdb>
-cf <ln-commands-file>` -- no running/crashing process needed for this
step at all, since `cdb -z` just loads the binary+PDB statically for
symbol lookup.

**Confirmed first: the same genuine-recursion signature persists in
this new build** (`module_offset=0x1fc183` repeats at frame 12 and
14, exactly matching the pre-symbols build's pattern) -- this is a
real, reproducible property of the bug, not an artifact of one
particular compilation.

### Full, resolved call stack (bottom of relevant frames to top / fault site)

```
main
  -> __scrt_common_main_seh -> invoke_main -> mainCRTStartup
  -> GhidraCommand::doit
  -> DecompileAt::rawAction
  -> ActionRestartGroup::apply
  -> Action::perform                        [repeats -- see below]
  -> ActionMergeRequired::apply
  -> Action::perform                        [same address as above -- confirmed recursion]
  -> Merge::mergeMultiEntry
  -> Merge::mergeMarker
  -> Merge::mergeIndirect
  -> Merge::mergeOp
  -> Merge::trimOpOutput    <-- FAULT SITE
```

The `Action::perform` repetition is expected, ordinary Ghidra
architecture (nested `ActionGroup`/`ActionRestartGroup` composite
actions calling into child actions, which are themselves
`Action`-derived and call `Action::perform` again) -- **not** itself
a bug. The genuinely informative part of this stack is the bottom:
**the crash is inside `Merge::trimOpOutput`**, reached via
`Merge::mergeOp` -> `Merge::mergeIndirect` -> `Merge::mergeMarker` ->
`Merge::mergeMultiEntry`, all invoked from `ActionMergeRequired::apply`.

### This is a direct, hard confirmation of the very first session's original suspicion

The investigation began because `Merge`'s behavior on
`muldiv_u16_scaled` looked wrong after the 9561 patch (register/stack
merge-array symptoms, `Merge::merge(register:2, stack:0x6)` pairing a
transient division register with an unrelated stack parameter, per
the 2026-09-03/04 sessions' P-code tracing). Multiple subsequent
theories (heritage restart timing, stack-buffer overrun in
`vnSpacebase` itself, uncaught C++ exceptions, protocol mismatches)
were explored and each ruled out in turn -- but the crash's actual,
now-confirmed location is exactly in the `Merge` subsystem the
original investigation was already looking at, specifically in
`trimOpOutput`, which was **not** one of the four caller sites
(`mergeOpcode`, `mergeMultiEntry`, `mergeAdjacent`, `hideShadows`)
ruled out during the earlier `DIAG_callerTag` investigation --
`mergeMultiEntry` **is** on that list and **is** in this real stack,
so the earlier diagnostic's "caller=unknown" finding (attributed at
the time to a static-global race condition in the diagnostic itself)
may actually have been correctly pointing at `mergeMultiEntry` all
along, just corrupted/unreadable due to the diagnostic's own known
race hazard.

### Immediate next step: read `Merge::trimOpOutput` and `Merge::mergeOp` directly for the actual faulting access

This is now a normal, tractable source-reading task -- no more
debugger/timing/capture work needed. The next session should:
1. Read `Merge::trimOpOutput`'s full implementation in `merge.cc`
   directly, looking for a pointer/array access that could be invalid
   given a Varnode/PcodeOp shape newly produced by 9561's recursive
   `vnSpacebase` unwrap (e.g. a SEGMENTOP-derived stack varnode that
   now has different size/type/def-chain characteristics than
   `trimOpOutput` expects).
2. Cross-reference against `Merge::mergeOp`'s call site immediately
   before `trimOpOutput` (module_offset 0x26be96 in the pre-PDB
   build) to see exactly what arguments/state are passed in.
3. Given `mergeMultiEntry` is confirmably in the real call chain, it
   is worth revisiting the original `DIAG_callerTag`-based
   `merge.cc` diagnostics from the very first (2026-09-02/04)
   sessions with this now-confirmed call path in mind -- the earlier
   diagnostic infrastructure (once its known static-global race is
   fixed) may still be useful for inspecting the actual Varnode/
   PcodeOp values flowing through `mergeMultiEntry` ->
   `mergeIndirect` -> `mergeOp` -> `trimOpOutput` for this specific
   function, now that the exact call path is confirmed rather than
   theorized.

This closes out the "where does it crash" question definitively. The
remaining work is "why does trimOpOutput fault given what 9561
produces" -- a much narrower, well-scoped question than anything this
investigation has had to work with until now.

## SESSION (continued): EXACT BUG FOUND -- Merge::trimOpOutput is missing an IPTR_IOP guard present at every other call site

### `merge.cc:664` is the only one of 25 `getOpFromConst` call sites with no space-type check first

Read `Merge::trimOpOutput` directly (`merge.cc:656-682`). The fault
line is:

```cpp
void Merge::trimOpOutput(PcodeOp *op)
{
  ...
  if (op->code() == CPUI_INDIRECT)
    afterop = PcodeOp::getOpFromConst(op->getIn(1)->getAddr()); // Insert copyop AFTER source of indirect
  else
    afterop = op;
  ...
  data.opInsertAfter(copyop,afterop);   // afterop dereferenced here if garbage
}
```

Searched every call site of `PcodeOp::getOpFromConst` across the
entire codebase (25 total, in `coreaction.cc`, `cover.cc`,
`double.cc` x5, `funcdata_op.cc`, `funcdata_varnode.cc`,
`heritage.cc` x3, `merge.cc` x4 (including this one),
`op.cc` x2, `ruleaction.cc` x3, `transform.cc`, `typeop.cc`).

**`trimOpOutput` (merge.cc:664) is the ONLY one of these 25 sites that
does not first check
`op->getIn(1)->getSpace()->getType() == IPTR_IOP`** before calling
`getOpFromConst`. Every other call site -- including the other three
in this exact same file (`merge.cc:464`, `merge.cc:554`, and
`merge.cc:814`) -- guards this call with an explicit IOP-space check
first (either as a separate `if`, or folded into the surrounding
conditional). `trimOpOutput` only checks
`op->code() == CPUI_INDIRECT`, which is necessary but not
sufficient: a `CPUI_INDIRECT`'s second input is *supposed* to always
be in IOP space by construction, but nothing enforces that
invariant at this specific call site the way it's enforced
everywhere else.

### Why this is the crash, precisely

`PcodeOp::getOpFromConst` decodes an `Address` value as if it were a
`PcodeOp*` pointer, valid only when that address genuinely lives in
Ghidra's internal IOP (indirect-op) address space -- a space
specifically used to embed pointers as encoded constants. If
`op->getIn(1)` is, for whatever reason, NOT in IOP space at this call
site (e.g. it has been resolved/rewritten to a real stack-space
constant by some interaction with 9561's newly-recursive
`vnSpacebase` unwrap upstream), `getOpFromConst` will silently
reinterpret an ordinary numeric constant as a raw pointer. The
resulting garbage `PcodeOp*` is then used as `afterop`, which is
dereferenced shortly after via `data.opInsertAfter(copyop,afterop)`
-- a direct, unguarded pointer dereference on attacker-uncontrolled
but genuinely invalid data. This is a textbook SIGSEGV: no exception,
no assert, immediate and deterministic given the right input shape --
matching every empirical property confirmed via the `segvHandler`
capture this session (fast, deterministic, no exception, signal 11).

### This closes the entire investigation's root-cause question

The call chain confirmed via the symbolized crash dump
(`ActionMergeRequired::apply` -> `Merge::mergeMultiEntry` ->
`Merge::mergeMarker` -> `Merge::mergeIndirect` -> `Merge::mergeOp` ->
`Merge::trimOpOutput`) is now fully explained end-to-end: some
CPUI_INDIRECT PcodeOp reaches `trimOpOutput` (via this exact merge
path, triggered because `muldiv_u16_scaled`'s specific
all-stack-parameter shape causes `ActionMergeRequired` to force a
merge here) whose input 1 is not actually in IOP space, and the
missing guard lets a garbage pointer through.

### Recommended fix

Add the same guard `trimOpOutput`'s siblings already have:

```cpp
if (op->code() == CPUI_INDIRECT && op->getIn(1)->getSpace()->getType() == IPTR_IOP)
    afterop = PcodeOp::getOpFromConst(op->getIn(1)->getAddr());
else
    afterop = op;
```

This is the minimal, surgical fix consistent with the pattern used
everywhere else in the codebase. It does not change 9561 itself (which
remains correct and necessary) -- it fixes a separate, pre-existing
latent gap in `Merge::trimOpOutput` that 9561's changes happen to
newly expose for this function's specific Varnode shape. Recommend
implementing this fix, rebuilding, and re-triggering
`force_decompile` on `muldiv_u16_scaled` to confirm the SIGSEGV no
longer occurs and that the decompilation output is now correct (not
just non-crashing) before considering this fully resolved.

## SESSION (continued): REFINED root cause -- not a missing IOP-space guard, but a STALE/DANGLING IOP-encoded PcodeOp reference

### First fix attempt (IPTR_IOP guard) did not resolve the crash -- diagnostics added to find out why

Added the guard recommended in the previous entry
(`op->code() == CPUI_INDIRECT && op->getIn(1)->getSpace()->getType() == IPTR_IOP`),
rebuilt, retested: **crash persisted, identical fault-frame offset**.
Added incremental diagnostics (written to
`C:\Users\j.brophy.CORKILLSYSTEMS\trimop_diag.txt`) to find the real
fault site within `trimOpOutput`, since the naive guard clearly wasn't
sufficient:

1. Logged `op->numInput()` before ever accessing `getIn(1)` -- ruled
   out `getIn(1)` being an out-of-bounds vector access:
   `numInput=2` logged safely multiple times. (Also discovered
   `PcodeOp::getIn(int4)` is a raw, unchecked `inrefs[slot]` array
   index with no bounds checking at all -- worth keeping in mind for
   any other `getIn()` call site audited in the future.)
2. Logged `vn = op->getOut()` right before its first dereference --
   ruled out a null-output theory: `vn` was a real, valid-looking
   heap pointer both times it was logged, never null.
3. Logged `afterop` immediately before the actual `data.opInsertAfter
   (copyop, afterop)` call (the last unguarded use of a
   possibly-IOP-decoded pointer), including whether the IOP-decode
   branch was taken at all (`afterop == op` vs not).

### Result: IOP-decode branch fires and PRODUCES REAL-LOOKING POINTERS three times, all of which succeed safely

```
trimOpOutput about to opInsertAfter copyop=... afterop=0000018D27EA6320 (afterop==op? NO-IOP-DECODED)
trimOpOutput about to opInsertAfter copyop=... afterop=000001F911A8AAF0 (afterop==op? NO-IOP-DECODED)
trimOpOutput about to opInsertAfter copyop=... afterop=000001C551795AD0 (afterop==op? NO-IOP-DECODED)
```

All three of these calls complete successfully (confirmed: further
diagnostic log lines appear immediately after each, for subsequent
`trimOpOutput` invocations) -- **none of these three logged,
IOP-decoded instances is the one that crashes.** The actual crash
happens on some later, unlogged call to `trimOpOutput` where
`getOpFromConst` must be producing a pointer that is NOT valid --
critically, the crash happens *before* our own `opInsertAfter`
diagnostic write executes, meaning the dereference inside
`data.opInsertAfter` (or possibly inside `getOpFromConst` itself, or
in whatever `PcodeOp` method is called on the decoded pointer as part
of insertion) faults on that specific call, silently, with no
opportunity to log.

### Refined understanding: this is NOT a missing space-type guard -- it's a genuinely STALE/DANGLING IOP reference

The evidence now points to something more specific than "the guard
was missing": `op->getIn(1)` genuinely **is** reporting `IPTR_IOP` as
its space type in the crashing case too (our guard does not prevent
the crash, meaning the crashing call also passes the `IPTR_IOP` check)
-- but the *address value encoded in that IOP constant no longer
corresponds to a live, valid `PcodeOp*`*. In other words: some
`CPUI_INDIRECT`'s second input still legitimately claims to be an
IOP-space reference, and its `getSpace()->getType()` check correctly
passes, but the specific `PcodeOp` it was originally pointing to has
since been deleted, freed, or replaced (most likely as a side effect
of 9561's recursive `vnSpacebase` unwrap triggering additional
Rule-driven simplification/cleanup passes that remove or replace
PcodeOps without updating every INDIRECT still holding a stale
iop-reference to the removed op). Dereferencing that now-dangling
pointer produces the observed SIGSEGV.

**This means the original `getOpFromConst`-missing-check theory from
the previous entry was on the right general track (this call site
genuinely is fragile) but the actual defect is different in kind: it
is not (solely) a missing type-check, but a case of an IOP-encoded
reference becoming stale/dangling due to some other part of the
pipeline (most likely PcodeOp removal/cleanup logic) failing to
notify or update this specific INDIRECT.** The `IPTR_IOP` guard added
this session is still a reasonable defensive check (matches the
pattern used at the other 24 call sites) and should likely be kept,
but it does not by itself fix this bug, since the crashing case still
passes that check.

### Recommended next step for a future session

1. Instrument `getOpFromConst` itself (`op.cc`) to validate the
   decoded pointer before returning it -- e.g. check it against a
   known-live-PcodeOp registry/set if one exists in `Funcdata`, or at
   minimum wrap the eventual dereference point with a diagnostic that
   fires precisely at the crash rather than one call removed from it
   (the current diagnostics all fire *before* the actual fault,
   meaning we still haven't captured the exact faulting instruction
   inside `opInsertAfter`'s own call chain -- worth getting a fresh
   `segvHandler`-based stack capture now that PDB symbols exist, to
   see exactly which function/line the fault is really inside, since
   it may be one or two frames deeper than `trimOpOutput` itself,
   e.g. inside `Funcdata::opInsertAfter` or `PcodeOp`'s own linked-list
   manipulation methods).
2. Separately, investigate whether 9561's changes cause any PcodeOp
   deletion/replacement pass to run that wouldn't have run (or would
   have produced different, still-valid results) prior to the patch,
   specifically for `CPUI_INDIRECT` ops -- this is the more
   fundamental question of *why* a stale IOP-reference is being left
   behind at all, as opposed to merely detecting/guarding against the
   already-stale pointer defensively after the fact.
3. All temporary diagnostics added to `merge.cc` this session
   (`numInput` logging, `vn` logging, `afterop`/opInsertAfter logging,
   all writing to `trimop_diag.txt`) should be removed once no longer
   needed, along with the earlier `segvHandler` instrumentation in
   `ghidra_process.cc`, before any final fix is considered ready to
   ship -- none of this is intended to be permanent, production code.

## SESSION (continued): cspec/sleigh review -- muldiv_u16_scaled sits on a pre-existing, independently-documented fragile edge case

### h8539f.cspec's own history flags this exact function as calling-convention-fragile, unrelated to 9561

Reviewed `h8539f.cspec` directly. Comments dated 2026-07-22 (predating
9561 entirely) document that `muldiv_u16_scaled` (along with
`sat_add_s16`, `sat_sub_u16_clamp0`, `muldiv_s16_rounded_3op`,
`div_u32_u16_rounded`, `select_min_xy_triple`) genuinely returns a
32-bit value across the R0:R1 register pair on every path, but no
available prototype shape in this cspec can express that correctly:

- Declaring the return as `ushort` against the 2-byte R0 pentry drops
  the R1 half entirely (confirmed via decompiler warnings + broken
  parameter binding, specifically named for `muldiv_u16_scaled`).
- Declaring it as `uint` makes Ghidra fall back to a bogus
  hidden-return-pointer convention that doesn't match the hardware.
- A `join` pentry construct (`piece1="R0" piece2="R1"`) was tried, but
  the two throwaway solo R0/R1 pentries Ghidra's schema requires to
  support the join **polluted the decompiler's global return-value
  candidate model**: ~18 unrelated functions across the ROM (with NO
  call relationship to any of the 6 affected math helpers) started
  showing **"Unable to track spacebase fully for stack"** and
  **"Restarted to delay deadcode elimination for space: register"**
  warnings (0 before, 27 after, confirmed via full before/after
  decompile-dump diffing).
- A genuine overlapping register (`R0R1`, defined directly in
  `h8539f.slaspec`, same technique as any AX/AL-style register alias)
  was tried next, on the theory that a "real" register (not a
  join/solo-pentry construct) couldn't cause the same global
  pollution. **This also failed**: confirmed via a full Ghidra
  restart + fresh ROM reopen that `isr_sci1_eri` and
  `muldiv_u16_scaled` both still showed byte-for-byte identical
  pollution symptoms. R0R1 was fully reverted and removed from
  `h8539f.slaspec` (confirmed clean via this session's own grep --
  only historical comments remain, no live register definition).
- The cspec currently falls back to a plain 2-byte `ushort` R0 output
  pentry for these 6 functions (`__stdcall_far_allstack_32ret`,
  despite its name, only actually declares a 2-byte R0 output --
  the "32ret" naming reflects intent/history, not the pentry itself),
  accepting the known dropped-R1-half warning as a smaller, contained,
  already-understood problem that doesn't spread to unrelated
  functions.

### Why this matters for the 9561 crash investigation

**This is independent, pre-existing evidence that `muldiv_u16_scaled`
specifically sits on a documented fragile edge in this cspec's
calling-convention modeling for exactly the same subsystems this
whole investigation has been tracing** -- spacebase tracking, heritage
restart/deadcode-elimination delay, and (implicitly) the merge logic
that consumes their output. The symptom class (spacebase/heritage
warnings spreading to unrelated functions) is strikingly close to the
Merge/Heritage misbehavior this investigation started from in the
very first (2026-09-02) session, and it was already known to occur
**without 9561 in the picture at all** (all dated 2026-07-22, over a
month before 9561 existed).

**This raises a real, not-yet-ruled-out possibility**: the SIGSEGV in
`Merge::trimOpOutput` may not be purely a 9561-introduced regression
in the generic decompiler -- it may be that `muldiv_u16_scaled`'s
return-value binding (currently: a 2-byte R0 pentry standing in for a
genuinely 4-byte R0:R1 register-pair return) was ALREADY producing an
irregular/borderline PcodeOp shape for this function pre-9561 (silent
R1-half-drop, as documented), and 9561's recursive `vnSpacebase`
change is what finally pushes that pre-existing irregularity from "a
warning + wrong output" into "a hard crash via a stale IOP reference
in Merge". Both could be true simultaneously: `trimOpOutput`'s lack of
IOP-liveness validation is a genuine, generically-fragile latent bug
(worth fixing regardless), AND this specific function's return-value
modeling may be an unusually good trigger for it precisely because of
its already-known irregular register/stack shape.

### Recommended next step

Before concluding the fix is purely `merge.cc`-side, it is worth
testing whether the crash is specific to functions with this
documented R0:R1-split return-value shape, or whether it reproduces
on a different, cleanly-modeled all-stack-parameter function with no
known cspec irregularities. The cspec comments name five OTHER
functions in the same boat (`sat_add_s16`, `sat_sub_u16_clamp0`,
`muldiv_s16_rounded_3op`, `div_u32_u16_rounded`,
`select_min_xy_triple`) -- if `force_decompile` also crashes on some
or all of these (predicted, if the cspec-shape theory is right) but
NOT on unrelated all-stack-parameter functions without this specific
return-value irregularity, that would strongly confirm the cspec's
pre-existing fragile modeling is a necessary precondition, not just
an unrelated coincidence, and should be considered alongside (or
possibly instead of) a pure `Merge::trimOpOutput` fix.

## SESSION (continued): cspec-shape theory TESTED AND RULED OUT -- crash is specific to muldiv_u16_scaled, not the R0:R1-return-shape family

### All 5 other named sibling functions decompile successfully; only muldiv_u16_scaled crashes

Tested `force_decompile` against all 5 other functions named in the
`h8539f.cspec` 2026-07-22 comments as sharing the same documented
R0:R1-split-return irregularity:

| Function | Address | Result |
|---|---|---|
| `sat_add_s16` | 0x1401f | **Succeeds cleanly**, no warnings |
| `sat_sub_u16_clamp0` | 0x14045 | **Succeeds cleanly**, no warnings |
| `div_u32_u16_rounded` | 0x1417d | Succeeds, but with both documented warnings ("Unable to track spacebase fully for stack" / "Restarted to delay deadcode elimination for space: register") and visibly garbled/wrong output (uninitialized `uVar4` used before being set) |
| `muldiv_s16_rounded_3op` | 0x14247 | Same as above: succeeds, both warnings present, clearly garbled output (stray `puVar6`/stack-garbage reads, `unaff_R3`) |
| `select_min_xy_triple` | 0x14471 | **Succeeds cleanly**, no warnings, output looks correct |

**None of these five crash.** Two reproduce the pre-existing,
independently-documented spacebase/heritage warnings and wrong output
(consistent with the cspec's own 2026-07-22 history), but none hit a
SIGSEGV. Only `muldiv_u16_scaled` itself crashes.

### Conclusion: the cspec R0:R1-return-shape theory from the previous entry is RULED OUT as a necessary precondition

This is a clean, decisive negative result. `muldiv_u16_scaled` shares
its calling convention and its documented return-value modeling
irregularity with all five of these functions, yet is the only one
that crashes -- so the shared cspec irregularity is **not** what
triggers the SIGSEGV. Whatever makes `muldiv_u16_scaled` different
must be something specific to *this* function's own P-code shape
(e.g. its specific SEGMENTOP/stack-parameter arrangement, its
particular sequence of arithmetic operations feeding into the
CPUI_INDIRECT that reaches `Merge::trimOpOutput`, or some other
per-function detail not shared by its five siblings) -- not a
category-level property of the whole "R0:R1-split-return,
all-stack-parameter" function family.

**This refocuses the investigation squarely back onto
`Merge::trimOpOutput`'s stale-IOP-reference bug (previous entry) as
the primary, and now more clearly isolated, root cause** -- the cspec
angle was a reasonable and worthwhile hypothesis to test (given the
suggestive historical parallel), but it is now cleanly ruled out
rather than left as an open, confusing alternative. The next step
remains as stated in the previous entry: get a fresh `segvHandler`
stack capture on the PDB-enabled build to find precisely which
function/line the fault is really inside (likely one or two frames
deeper than `trimOpOutput`, inside `Funcdata::opInsertAfter` or
`PcodeOp`'s own linked-list manipulation), now knowing definitively
that the trigger is specific to `muldiv_u16_scaled`'s own P-code
shape rather than a broader calling-convention category.

## SESSION (continued): retyping the return value tested -- does not affect the crash

Tried changing `muldiv_u16_scaled`'s declared return type from
`ushort` to `uint` (via `set_function_prototype`, same calling
convention `__stdcall_far_allstack_32ret`) to test whether the
crash was sensitive to the return-type declaration itself (echoing
the cspec's own historical uint-vs-ushort experiments for its
siblings). **Crash still occurred, identical symptom.** Reverted to
the original `ushort` return type (confirmed restored).

This further confirms the crash is not about how the return value is
*declared* -- it's about the function's own underlying P-code shape
(specific instruction sequence / stack-parameter layout / whatever
CPUI_INDIRECT ends up reaching `Merge::trimOpOutput` with a stale IOP
reference), independent of any type annotation applied on top.

## SESSION (continued): confirmed muldiv_u16_scaled is the ONLY crashing function among all directly-related siblings

Tested `force_decompile` against every other function in the `mul`/
`div`-family group visible via `search_functions name_pattern=muldiv`
and the original `muldiv_u16` that started the whole investigation:

| Function | Address | Result |
|---|---|---|
| `muldiv_u16` | 0x141ba | Succeeds, both warnings present, returns `b` (the original wrong-answer symptom from session 1) -- no crash |
| `muldiv_u16_scaled` | 0x141dc | **CRASHES (SIGSEGV)** |
| `muldiv_s16_rounded` | 0x14219 | Succeeds, both warnings present, garbled/wrong output -- no crash |
| `muldiv_s16_rounded_3op` | 0x14247 | Succeeds, both warnings present, garbled/wrong output -- no crash (already tested previous entry) |
| `muldiv_u16_shr8_sat` | 0x14387 | Succeeds cleanly, no warnings, output looks correct -- no crash |

Combined with the previous entry's five cspec-named siblings
(`sat_add_s16`, `sat_sub_u16_clamp0`, `div_u32_u16_rounded`,
`select_min_xy_triple`, plus `muldiv_s16_rounded_3op` already
counted above), **`muldiv_u16_scaled` is confirmed as the only
function, among every closely-related sibling tested this session, that
actually crashes.** Everything else in this function family either
decompiles cleanly or reproduces the older warning/garbled-output
symptom class without ever hitting a SIGSEGV.

Attempted to enumerate and test literally every function in the
binary (741 total) via `search_functions` with a wildcard pattern,
but the tool's `name_pattern` does not support a bare wildcard/regex
match-all (`.*` returned no results) -- a full-binary sweep was not
attempted given the cost/risk of triggering hundreds of native
process launches just to search for a possible second crashing
function, especially with no further specific reason (beyond general
completeness) to suspect one exists. Given how tightly scoped and
specific the confirmed root cause already is (a stale IOP reference
reaching `Merge::trimOpOutput`, apparently tied to this exact
function's own precise P-code shape rather than any shared
category), it is plausible `muldiv_u16_scaled` is genuinely unique in
triggering this in the current ROM. If a systematic full-binary sweep
is wanted in the future, it would need either a working wildcard
search or an external enumeration (e.g. iterating `list_functions`'
full output programmatically) rather than `search_functions`'
apparent regex-anchor-based matching.

## SESSION (continued): crash confirmed 100% deterministic and stable across 50+ repeated triggers; diagnostic files cleaned up

After the auto-analysis dialog incident, `segv_diag.txt` had grown to
over 50 separate SIGSEGV captures (from repeated triggers, likely
including auto-analysis's own internal retry attempts). Every single
one shows the **identical module-relative offsets** at every frame
(`0x5a1f9`, `0x44c5xx`, `0x49d2xx`, `0x3f54xx`, `0x26bxxx`,
`0x1fc183`/`0x1fcf8f` repeating pair, `0x55b61`, `0x53e21`, `0x5b116`,
`0x3f2xxx`), varying only in PID and ASLR-shifted absolute addresses
as expected. **This confirms the crash is fully deterministic and
stable** -- not a race condition, not intermittent, not sensitive to
timing or system load. Diagnostic files (`segv_diag.txt`,
`trimop_diag.txt`) were becoming large from repeated triggers and
have been noted for cleanup; the underlying fault signature itself
requires no further reconfirmation.

## SESSION (continued): expanded sweep across structurally diverse functions -- muldiv_u16_scaled remains the sole crasher

User supplied the full symbol table for this address range
(0x14000-0x14880+, ~50 functions). Tested a broader, more
structurally diverse sample beyond the immediate mul/div/muldiv
family already covered:

| Function | Address | Result |
|---|---|---|
| `sat_add_u16` | 0x14000 | Succeeds cleanly, no warnings |
| `div_u16_sat2` | 0x140fc | Succeeds, both warnings present, no crash |
| `axis_lookup_interp` | 0x14735 | Succeeds, both warnings present, no crash -- notably the LARGEST and most structurally complex function tested this session (pointer-heavy, loops, binary-search-style table walk, ~80 real call sites per its own embedded documentation comment) |
| `mul_u16_x2_sat_alt` | 0x14293 | Succeeds cleanly, no warnings |
| `table_lookup_indexed` | 0x14854 | Succeeds cleanly, no warnings, calls into another function (`table_lookup_interp`) |

**`muldiv_u16_scaled` remains the only function, across every test run
this session (13 total: 8 direct mul/div/cspec siblings + these 5
structurally diverse additions), that crashes.** This includes the
single largest, most control-flow-complex function available in this
address range (`axis_lookup_interp`), which decompiles fine despite
being a much more demanding test of the decompiler's general
robustness than any of the small arithmetic helpers. This further
reinforces that the trigger is something narrowly specific to
`muldiv_u16_scaled`'s own exact P-code shape (most likely the precise
combination of its SEGMENTOP-resolved stack access alongside its
particular arithmetic/control-flow sequence), not a general
robustness issue with the decompiler, this ROM, or this cspec.

## SESSION (continued): EXACT FAULTING INSTRUCTION IDENTIFIED -- Funcdata::opInsertAfter's inlined PcodeOp::isMarker call, first instruction

### Disassembled Funcdata::opInsertAfter directly via offline symbol resolution (cdb -z, PDB-enabled build)

Found `Merge::trimOpOutput` and `Merge::mergeOp` are both FULLY INLINED
(no standalone symbol at all -- `x decompile!Merge::trimOpOutput`
returns nothing) into their own callers, which is why earlier
`ln`/ symbol lookups on raw crash-frame addresses were unable to name
them directly; only the disassembly-based approach from the prior
entry actually worked. `Funcdata::opInsertAfter`, by contrast, IS a
real, separately-emitted function (`decompile!ghidra::Funcdata::
opInsertAfter`, confirmed at `decompile+0x1c47e0`), confirmed via
`x decompile!*opInsertAfter*` to also appear as an INLINE CALLER
inside `Merge::trimOpOutput` specifically at `trimOpOutput+0x26e` --
directly confirming, at the disassembly level, that `trimOpOutput`
really does call into this exact code.

Disassembled `Funcdata::opInsertAfter` directly (`u decompile+0x1c47e0 L60`):

```asm
decompile!ghidra::PcodeOp::isMarker [inlined in decompile!ghidra::Funcdata::opInsertAfter]:
00000001`401c47e0 test    byte ptr [r8+8],40h      <-- FAULTING INSTRUCTION
00000001`401c47e5 mov     r10,rdx
00000001`401c47e8 mov     r11,rcx
00000001`401c47eb je      opInsertAfter+0x34
00000001`401c47ed mov     rax,qword ptr [r8]        ; dereferences afterop again (vtable-style)
00000001`401c47f0 cmp     dword ptr [rax+10h],3Dh
00000001`401c47f4 jne     opInsertAfter+0x34
00000001`401c47f6 mov     rax,qword ptr [r8+50h]    ; another unguarded afterop field access
00000001`401c47fa mov     r9,qword ptr [rax+8]
00000001`401c47fe mov     rax,qword ptr [r9+10h]
...
```

### This is the exact fault, confirmed at the instruction level

`opInsertAfter(copyop, afterop)`'s FIRST ACTION is an inlined call to
`PcodeOp::isMarker(afterop)`, whose first instruction is
`test byte ptr [r8+8],40h` -- `r8` holds `afterop` (per the x64
calling convention, third integer argument), and this instruction
reads a byte at `afterop+8` with **zero prior validation that
`afterop` is a live, valid pointer**. Given `afterop` here is exactly
the value produced by `PcodeOp::getOpFromConst(op->getIn(1)->
getAddr())` back in `trimOpOutput` -- confirmed via this session's
earlier diagnostics to sometimes be a stale/dangling pointer despite
correctly reporting `IPTR_IOP` space -- this `test byte ptr
[r8+8],40h` is very likely the literal, exact faulting CPU
instruction. Several more unguarded dereferences of the same pointer
follow immediately after (`[r8]`, `[r8+50h]`, chained further through
`[rax+8]`, `[r9+10h]`, etc.), any one of which would produce an
identical SIGSEGV if `afterop` is invalid -- so the crash could be at
this exact first instruction or one of these next few, but all are
part of the same unguarded-dereference chain immediately inside
`opInsertAfter`'s inlined `isMarker` check.

### Complete picture, source-to-assembly

1. `Merge::trimOpOutput` (`merge.cc`) calls
   `PcodeOp::getOpFromConst(op->getIn(1)->getAddr())` for a
   `CPUI_INDIRECT` whose second input reports `IPTR_IOP` space (this
   session's `IPTR_IOP` guard does not prevent this, since the
   crashing case genuinely has that space type) but whose encoded
   address no longer corresponds to a live `PcodeOp` -- i.e. a
   stale/dangling IOP reference, root-caused to something upstream
   (most likely a PcodeOp deletion/replacement pass, possibly
   triggered by 9561's changed `vnSpacebase` recursion, that removes
   or replaces a `PcodeOp` without updating every `CPUI_INDIRECT`
   still holding a stale iop-pointer to it) -- exact upstream
   mechanism not yet identified.
2. This garbage pointer is passed as `afterop` into
   `Funcdata::opInsertAfter(copyop, afterop)`.
3. `opInsertAfter` immediately inlines `PcodeOp::isMarker(afterop)`,
   whose very first instruction unconditionally dereferences
   `afterop` with no null/validity check -- `test byte ptr
   [r8+8],40h` -- producing the SIGSEGV.

### Recommended fix, now precisely scoped

The most direct, targeted fix is validating the `getOpFromConst`
result in `trimOpOutput` before using it as `afterop` -- e.g.
confirming the decoded pointer is still a live member of the
function's own PcodeOp list/set before trusting it (Ghidra's
`Funcdata` likely already has some notion of "is this PcodeOp still
alive" that could be reused here, such as checking it against the
op's own tracked list membership rather than trusting a raw decoded
address). This is a more surgical, correct fix than merely adding
back the `IPTR_IOP` space check (already tried, insufficient) since
the real defect is pointer *liveness*, not pointer *space
classification*. A future session should look at how other
`getOpFromConst` call sites (there are 24 others) protect against
this same staleness risk, if any do, as a model for the fix here --
or, if none do, this may indicate the underlying PcodeOp-removal
pass itself needs to notify/update stale INDIRECT references rather
than trying to defensively validate at every consumption site.

## SESSION (continued): FIX CONFIRMED WORKING -- SIGSEGV eliminated, replaced by a graceful, catchable LowlevelError

### Implemented the isDead()-validated fix in Merge::trimOpOutput

```cpp
if (op->code() == CPUI_INDIRECT && op->numInput() > 1 && op->getIn(1)->getSpace()->getType() == IPTR_IOP) {
  PcodeOp *indirectSource = PcodeOp::getOpFromConst(op->getIn(1)->getAddr());
  afterop = (indirectSource->isDead()) ? op : indirectSource; // Insert copyop AFTER source of indirect, unless that source is stale
}
else
  afterop = op;
```

This follows the exact pattern already used at
`heritage.cc:267-270`'s `removeRevisitedMarkers`, which validates the
same `getOpFromConst` result via `targetOp->isDead()` before trusting
it. `trimOpOutput` previously had no such check.

### Result: rebuilt, retriggered `force_decompile` on `muldiv_u16_scaled`

```
{"error":"Decompilation did not complete for function muldiv_u16_scaled. Reason: Low-level Error: Unable to force merge of op at 0x00014203:ed"}
```

**No more "Decompiler process died". No more SIGSEGV.** This is now a
genuine, caught `LowlevelError` -- confirmed earlier this session
that `GhidraCommand::doit()` catches this exception type cleanly and
reports it back through the normal error-response protocol, exactly
as observed here. The `segvHandler` diagnostic
(`C:\Users\j.brophy.CORKILLSYSTEMS\segv_diag.txt`) was NOT triggered
by this run -- consistent with there being no crash at all this time.

**This new error is an expected, legitimate downstream consequence of
the fix, not a new bug.** It's `Merge::mergeOp`'s own explicit
`throw LowlevelError("Unable to force merge of op at " +
op->getSeqNum())` (source confirmed at `merge.cc`, near line 799),
fired when a merge genuinely cannot be completed even after all trim
attempts. This is precisely the correct behavior once `trimOpOutput`
declines to (incorrectly) insert the trim-COPY after a stale/dead
op -- falling back to `afterop = op` per the fix -- and the merge
subsequently fails its Cover-intersection test for a different,
legitimate reason. The decompiler now fails safely and explains why,
rather than crashing.

### Current status: crash eliminated; underlying merge-failure symptom is now the next (much smaller, non-fatal) question

`muldiv_u16_scaled` still does not produce a full decompilation --
but the failure mode is now a normal, informative,
recoverable-by-Ghidra's-own-error-handling condition instead of a
process-ending SIGSEGV. This is the practical resolution of the
crash investigation this whole document has tracked across multiple
sessions. Whether `muldiv_u16_scaled` can be made to fully decompile
(by addressing why this particular merge is now failing, likely still
related to 9561's changed `vnSpacebase` behavior producing a Cover
shape `mergeOp` can't reconcile) is a separate, follow-on question --
much narrower in scope and no longer safety-critical, since the
practical effect of the original bug (crashing the whole decompiler
process on this function) is resolved.

### Cleanup still needed before this fix is considered final/shippable

The temporary diagnostics added to `merge.cc` this session
(`numInput`/`vn`/`opInsertAfter` logging to `trimop_diag.txt`, all
still present in the same function) and `ghidra_process.cc`'s
`segvHandler` stack-capture instrumentation should be removed once no
further live capture is needed -- neither is intended to ship. The
core `isDead()` fix itself should remain.

## CORRECTION (2026-09-08): "fixed" was the wrong framing -- crash is gone, but there is still NO decompiled output

To be precise and avoid overstating progress: the `isDead()` fix
eliminates the SIGSEGV, confirmed via the screenshot of the actual
Decompiler panel in Ghidra showing:

```
Low-level Error: Unable to force merge of op at 0x00014203:ed
```

**This is not a working decompilation. There is no C code for
`muldiv_u16_scaled` at all** -- the practical, user-facing outcome
(being able to read this function's decompiled source) remains
unresolved. The earlier "FIX CONFIRMED WORKING" framing in this
document should be read narrowly: it means the *crash* is fixed
(the native process no longer dies), not that the *function* now
decompiles. These are different things and the distinction matters
-- from the user's perspective, `muldiv_u16_scaled` is still
completely unusable, just via a different failure mode now.

**Next real objective**: get `Merge::mergeOp` to actually succeed for
this function's specific `op at 0x00014203`, not merely fail
gracefully instead of crashing. This means understanding why the
Cover-intersection test at `merge.cc`'s `mergeOp` (the final
`if (!merge(...)) throw LowlevelError(...)` around line 796-799) is
failing for this particular PcodeOp, likely still traceable to
9561's changed `vnSpacebase` recursion producing a Cover/Varnode
shape that doesn't fit this function's specific
all-stack-parameter, SEGMENTOP-resolved layout.

## SESSION (continued): identified exactly what op at 0x00014203 is -- the second of two chained divxu.w instructions

Disassembled `muldiv_u16_scaled` in full:

```asm
000141dc: link FP,#0x0:8
000141de: mov:g.w R3,@-SP
000141e0: mov:g.w R2,@-SP
000141e2: mov:f.w @(0x6:8,FP),R0
000141e4: mov:f.w @(0x8:8,FP),R2
000141e6: mulxu.w @(0xa:8,FP),R0
000141e9: mulxu.w @(0xa:8,FP),R2
000141ec: add:g.w R1,R2
000141ee: addx.w 0x0,R0
000141f2: tst.w @(0xc:8,FP)
000141f5: beq #0x0001420c
000141f7: cmp:g.w @(0xc:8,FP),R0
000141fa: bhs #0x0001420c
000141fc: mov:g.w R2,R1
000141fe: divxu.w @(0xc:8,FP),R0
00014201: mov:g.w R0,R2
00014203: divxu.w @(0xc:8,FP),R2      <-- op at 0x00014203, the failing merge target
00014206: mov:g.w R1,R0
00014208: mov:g.w R3,R1
0001420a: bra #0x00014212
0001420c: mov:i #0xffff,R0
0001420f: mov:i #0xffff,R1
00014212: mov:g.w @SP+,R2
00014214: mov:g.w @SP+,R3
00014216: unlk FP
00014217: prts
```

**`op at 0x00014203` is the SECOND of two sequential `divxu.w`
(16-bit unsigned divide) instructions.** The function computes
`a*d + (b*d >> 16)` (two `mulxu.w`+`add:g.w`/`addx.w`), then performs
TWO chained divides by `c`: the first at `0x141fe` divides the
initial 32-bit product by `c`, its result is moved `R0->R2` at
`0x14201`, and the second `divxu.w` at `0x14203` divides that
intermediate result by `c` again. This is a genuine dependent-op
chain sharing the same physical register (`R2`) across two
side-effecting instructions in immediate succession -- exactly the
shape that would naturally produce a `CPUI_INDIRECT` linking the
second division's input to the first division's result, which is a
very plausible reason `Merge::mergeIndirect`/`mergeOp` would need to
force a merge here at all, and a plausible place for 9561's changed
`vnSpacebase` resolution (which affects how `@(0xc:8,FP)` -- the
`c` parameter's SEGMENTOP-resolved stack access, used as the divisor
in BOTH `divxu.w` instructions) to produce a Cover shape the merge
logic can't reconcile.

### Next step

Get P-code for this function (via `force_decompile`'s partial output
if available, or a lower-level P-code dump tool) specifically around
these two `divxu.w` ops and their shared `@(0xc:8,FP)` operand, to see
exactly what Cover/HighVariable conflict `mergeOp`'s final
`if (!merge(...))` check is hitting. This is a much more concrete,
scoped target than before -- we now know precisely which two
instructions and which shared stack parameter (`c`, at `Stack[0x8]`)
are involved.

## SESSION (continued): merge failure root cause narrowed to a Cover intersection between two unique-space temporaries

Added targeted diagnostics directly at `mergeOp`'s failing
`if (!merge(...))` call, logging both Varnodes' addresses, sizes,
spaces, and HighVariable instance counts.

### Result

```
mergeOp FAIL op=... pc_offset=0x14203 slot=0
  outvn=... offset=0x1000001d size=2 space=unique
  invn=...  offset=0x1000001b size=2 space=unique
  outHigh numInstances=1, inHigh numInstances=1
```

Both the failing merge's output and input Varnodes are **freshly
allocated `unique`-space temporaries** (Ghidra's internal scratch
register space), each size 2, each belonging to a `HighVariable` with
only one instance (i.e. neither has been merged with anything else
yet -- this is the very first attempt to merge them). `pc_offset`
confirms this is the second `divxu.w` at `0x14203`
(`divxu.w @(0xc:8,FP),R2`), and `slot=0` is its first input -- the
dividend, which is `R2` holding the prior division's result (moved
there at `0x14201: mov:g.w R0,R2`).

### Root cause of the failure

`Merge::merge()`'s failure is exactly
`testCache.intersection(high1,high2)` returning true -- i.e. these
two temporaries' Covers (live ranges) genuinely overlap according to
Ghidra's own live-range analysis. Since `mergeOp`'s calling context
(`mergeIndirect`/`mergeMarker`) is trying to FORCE this merge because
`divxu.w`'s output and input must share the same storage as a
hardware/ABI constraint the P-code models via a CPUI_INDIRECT/
MULTIEQUAL-style marker, an intersection here means the Cover
computation itself is producing an overlap where, given this is the
very first, fresh instance of each temporary, none should exist.

**This is a distinct bug from the one already fixed.** The
`isDead()` fix corrected `trimOpOutput`'s handling of a stale IOP
reference (a pointer-safety bug). This new issue is a Cover/liveness
computation problem -- something in the pipeline (plausibly still
tied to 9561's changed `vnSpacebase` recursion, since this shared
`@(0xc:8,FP)` stack access -- parameter `c` -- feeds BOTH divxu.w
instructions and is exactly the kind of SEGMENTOP-resolved access
9561 changes) is causing these two adjacent, dependent divide ops'
live ranges to be computed as overlapping when they should be
sequential and non-overlapping.

### Stopping point for this session

This is real, precise, well-scoped progress: the crash is fixed, and
the remaining "no decompiled output" symptom is now narrowed to a
specific Cover-intersection question between two named temporaries at
two named P-code op sites, with the exact source line
(`merge.cc`'s `Merge::merge()` -> `testCache.intersection()`) that
performs the failing check identified. A future session should trace
`HighIntersectTest::intersection()` (the `testCache` member's
underlying implementation) directly, or add Cover-printing
diagnostics to `mergeOp`/`mergeIndirect` to see the actual Cover
ranges computed for these two temporaries before the intersection
check runs, to determine whether the Cover computation itself is
wrong (a real, distinct bug) or whether the two ranges are correctly
computed as genuinely overlapping (in which case the bug is further
upstream, in how the CPUI_INDIRECT/MULTIEQUAL linking the two
divxu.w ops was constructed in the first place).

## SESSION (2026-09-08, continued): Cover data captured -- reveals a genuinely abnormal, overly-broad Cover on the failing input Varnode

### Clean rebuild from scratch: reverted ALL prior changes, reapplied ONLY the isDead() merge.cc fix

Given the confusion from the previous "everything broke" incident, did a
full `git checkout --` on all 7 previously-modified files to return to
a byte-for-byte clean 9561 baseline, then reapplied exactly one change:
the `isDead()` liveness guard in `Merge::trimOpOutput` (see the final,
upstream-ready version of this fix, with a self-contained comment not
referencing this document, committed to `merge.cc`). Confirmed via
`git diff --stat` this is now a single-file, 11-line-added/2-line-removed
change -- the true minimal fix, with no leftover diagnostics from
earlier sessions. This is the version intended for actual submission.

Root cause of the earlier "everything broke" confusion: a real,
substantive (non-diagnostic) behavioral change to
`RuleLoadVarnode::vnSpacebase`'s SEGMENTOP-selector-resolution logic
had been made directly in the working tree while "trialing" a fix
attempt, without being tracked/reasoned about as a deliberate
experiment. It was suspected as the cause of the broadened crash
(reaching `axis_lookup_interp`) but reverting it alone did NOT fix
that crash -- the broadened-crash episode's true cause was not
conclusively identified before this clean-slate revert was done
instead. Given the clean revert immediately restored full working
state (confirmed via re-test below), further root-causing that
specific episode was deprioritized in favor of moving forward
correctly.

### Fresh, correctly-captured Cover data for the current LowlevelError

Re-added the mergeOp-failure diagnostic (Cover-printing this time, not
just addresses) on top of the clean, minimal `isDead()` fix. Rebuilt,
triggered `force_decompile` on `muldiv_u16_scaled`, read
`mergefail_diag.txt` via direct `Get-Content` (NOTE: Pulsar's own
`read-file` tool returned stale/cached content for this file across
multiple attempts in this session -- always cross-check with a raw
`Get-Content` via `run-command` when a file is being written to
frequently by an external process).

```
mergeOp FAIL op=... pc_offset=0x14203 slot=0
  outvn=... offset=0x1000001d size=2 space=unique
  invn=...  offset=0x1000001b size=2 space=unique
  outHigh numInstances=1, inHigh numInstances=1
  outCover: 2: 0x00014203:ed-0x00014203:154

  inCover: 0: begin-end
1: begin-end
2: 0x00014203:153-0x00014203:ed
```

### This is a genuinely abnormal Cover -- likely the real root cause of the merge failure

Two distinct red flags:

1. **`inCover` blocks 0 and 1 are both `begin-end`** -- meaning
   Ghidra's liveness analysis considers this input temporary (the
   dividend feeding the second `divxu.w` at 0x14203) live across the
   ENTIRE function, from its first block to its last. This is highly
   abnormal for what should be a small, purely local temporary
   holding one division's result for immediate reuse a few
   instructions later. A correctly-scoped Cover for this kind of
   temporary should span only the narrow range between the first
   divide (0x141fe) and the second (0x14203), not the whole function.
2. **`inCover` block 2's range is inverted**: `0x14203:153` to
   `0x14203:ed` -- but `0x153` (339) is numerically greater than
   `0xed` (237). A Cover block's start sequence number is greater
   than its end sequence number, which is backwards. Combined with
   `outCover`'s single block (`0xed` to `0x154`, i.e. 237 to 340,
   correctly ordered), the two ranges are guaranteed to intersect
   under any reasonable overlap test, explaining the merge failure
   directly and mechanically.

**Working theory**: something in how the two chained, dependent
`divxu.w` instructions (0x141fe then 0x14203, sharing R2 and both
using the SEGMENTOP-resolved `@(0xc:8,FP)` stack parameter `c` as
divisor) get linked via CPUI_INDIRECT is causing the second
temporary's Cover to be computed incorrectly -- either picking up an
erroneous `begin-end` (whole-function) block from a different, unrelated
code path (possibly a merge/join with some other, genuinely
whole-function-scoped variable, like a parameter), or the sequence
numbering itself becoming inconsistent for this specific op due to
9561's changed `vnSpacebase` recursion altering the order/identity of
P-code ops generated for this SEGMENTOP-resolved access.

### Recommended next step

Trace where `inCover`'s three blocks actually come from -- specifically
block 2's inverted range and blocks 0/1's `begin-end` scope. This
likely requires instrumenting `Cover`/`HighVariable`'s construction
methods (`Cover::addRefPoint`, `Cover::merge`, wherever these blocks
are added) rather than only observing the final printed result, to
see which specific operation contributes the erroneous begin-end
blocks and the inverted range, and whether this traces back to the
same SEGMENTOP/vnSpacebase interaction already implicated throughout
this investigation, or to a separate defect in Cover construction
itself that 9561 merely exposes for this function's specific shape.

## SESSION (continued): traced the abnormal Cover to addRefPoint's normal backward-recursion, now pointing at a CFG/heritage-shape question

### Instrumented Cover::addRefPoint and Cover::addRefRecurse directly

Added temporary diagnostics logging every `addRefPoint` call for the
two known offending Varnodes (`0x1000001b`, `0x1000001d`, both size-2
`unique` temporaries) and every `addRefRecurse` block-fill event.

### Result

```
  addRefRecurse setAll() on block=1 (no existing cover)
  addRefRecurse setAll() on block=1 (no existing cover)
  addRefRecurse setAll() on block=1 (no existing cover)
  addRefRecurse setAll() on block=0 (no existing cover)
  addRefRecurse setAll() on block=1 (no existing cover)
addRefPoint vn_off=0x1000001b ref_pc=0x14203 ref_seq=0xed ref_block=2 ref_opcode=61
  addRefRecurse setAll() on block=1 (no existing cover)
  addRefRecurse setAll() on block=0 (no existing cover)
addRefPoint vn_off=0x1000001d ref_pc=0x14203 ref_seq=0x154 ref_block=2 ref_opcode=1
```

`ref_opcode=61` is `CPUI_INDIRECT`; `ref_opcode=1` is `CPUI_COPY`. Both
reads occur in **block 2** (`ref_block=2`) -- but immediately
beforehand, `addRefRecurse` fills in (`setAll()`, the mechanism behind
the `begin-end` blocks seen in the earlier Cover dump) **blocks 0 and
1**, which are NOT where the read happens at all.

### This is addRefPoint's normal, correctly-designed mechanism -- not a bug in Cover construction itself

Re-reading `Cover::addRefPoint`/`addRefRecurse`'s source: this
backward-fill behavior is intentional and correct in general -- a
variable's live range must extend backward through every CFG
predecessor block that has no closer definition point, so the value
is guaranteed live along all paths that can reach the read. The
diagnostic confirms this mechanism is firing exactly as designed,
**not malfunctioning**.

**This reframes the real question**: why does block 2 (containing the
`CPUI_INDIRECT`/`CPUI_COPY` reads of these temporaries) have blocks 0
and 1 as CFG predecessors with no intervening definition, when by the
function's actual assembly
(see the disassembly captured earlier this investigation),
block 2 should logically only be reachable via the direct
fall-through path after the `beq`/`bhs` zero/overflow checks at
0x141f5/0x141fa -- NOT via the `0x1420c` fallback path (which just
loads 0xffff/0xffff and jumps straight to the epilogue, never
touching the division temporaries at all). If Ghidra's actual basic-
block graph has block 2 receiving an edge from the fallback block (0
or 1) that shouldn't exist, or is missing an intermediate block that
would normally contain the definition, that would fully explain both
the whole-block covers and the resulting merge failure -- and would
point to a genuine CFG/heritage construction issue rather than a
Cover-printing or intersection-test bug.

### Recommended next step

Get the actual basic-block graph/CFG for `muldiv_u16_scaled` as
Ghidra currently sees it (block count, each block's start/end
address, and its predecessor/successor edges) and compare against the
disassembly's real control flow to check for a spurious edge or a
missing block. `get_function_signature` earlier reported
`basic_block_count=5` for this function; the next step is to get
each of those 5 blocks' actual address ranges and edges (a
`ghidra:analyze_control_flow`-style call, if available, or by reading
the P-code block structure directly via a targeted diagnostic in
`funcdata.cc`/`block.cc`) to see exactly which blocks 0/1/2 correspond
to in the real assembly, and whether the edge from the fallback path
into block 2 is genuinely spurious.

## SESSION (continued): CFG hypothesis CHECKED AND RULED OUT -- the control-flow graph is actually correct

### Got the real basic-block structure via analyze_control_flow

```
Block 0: 0x141dc-0x141f6 (27 bytes) -- 2 successors (fallthrough to Block 1, branch to Block 3)
Block 1: 0x141f7-0x141fb (5 bytes)  -- 2 successors (fallthrough to Block 2, branch to Block 3)
Block 2: 0x141fc-0x1420b (16 bytes) -- 1 successor (Block 4) -- CONTAINS BOTH divxu.w INSTRUCTIONS
Block 3: 0x1420c-0x14211 (6 bytes)  -- 1 successor (Block 4) -- the 0xffff/0xffff fallback
Block 4: 0x14212-0x14218 (7 bytes)  -- exit
```

**The previous entry's hypothesis (a spurious edge from the fallback
path, Block 3, into Block 2) is WRONG and is retracted.** Block 3 does
NOT feed into Block 2 at all -- it goes straight to Block 4,
bypassing Block 2 entirely. Block 2's only predecessor is Block 1;
Block 1's only non-branch predecessor is Block 0. So the backward
walk from Block 2 (where the divxu.w reads happen) through Block 1 to
Block 0 is walking the SINGLE, ONLY linear path that can reach Block
2 at all -- there is no divergence, no spurious edge, nothing
structurally wrong with this CFG. `addRefRecurse` filling Blocks 0
and 1 with `setAll()` is therefore mechanically CORRECT given this
graph: if there's no earlier, more specific definition point found
along that single path, conservatively covering the whole of both
predecessor blocks is the algorithm's designed fallback behavior.

### This means the real question is different: why is there no earlier, more specific definition point?

Given the CFG itself is fine, the abnormal breadth of the Cover is
better explained by: the temporary's actual DEFINING PcodeOp (not
just its later read) is either missing, or placed somewhere
`addRefPoint`'s search doesn't find efficiently along this path --
forcing the conservative whole-block fallback where a tighter,
correct Cover should have been possible. This points back toward
9561's changed `vnSpacebase` recursion potentially affecting *where*
or *how* the relevant defining PcodeOp for the SEGMENTOP-resolved
stack parameter access is placed in the P-code graph for this
specific two-divide sequence, rather than a CFG edge problem.

### Recommended next step (revised)

Rather than continuing to guess at the CFG, get the actual raw P-code
op-iter listing for Block 2 specifically (via
`ghidra:get_function_pcode` with `granularity: "basic"`, which is
lighter-weight and was specifically noted as suited for this kind of
per-block inspection) and see the actual defining op for both size-2
unique temporaries (`0x1000001b`, `0x1000001d`) and exactly where
each is placed relative to the two divxu.w-derived CPUI_INDIRECT
markers, rather than continuing to hypothesize about CFG structure
that has now been checked and found to be correct.

## SESSION (continued): TRUE ROOT CAUSE FOUND -- a genuine use-before-define P-code sequencing defect

### Added def-point logging alongside the existing ref-point logging in Cover::addRefPoint

(Note: `str_replace` proved unreliable and corrupted `cover.cc` twice
in this session -- once splicing code into a doc comment, once
failing to apply a `const` fix while reporting success. Switched to
PowerShell line-array insertion with an exact-match guard
(`$lines.IndexOf(...)`, `InsertRange`, verified against the expected
following line before inserting) for all further edits to this file,
which worked reliably. This technique should be preferred over
`str_replace` for any further edits to files that have shown this
corruption pattern.)

### Result -- the real defect, found directly

```
addRefPoint vn_off=0x1000001b ref_pc=0x14203 ref_seq=0xed  ref_block=2 ref_opcode=61
  defOp pc=0x14203 def_seq=0x153 def_block=2 def_opcode=1
addRefPoint vn_off=0x1000001d ref_pc=0x14203 ref_seq=0x154 ref_block=2 ref_opcode=1
  defOp pc=0x14203 def_seq=0xed  def_block=2 def_opcode=61
```

(`ref_opcode=61` = `CPUI_INDIRECT`, `ref_opcode=1` = `CPUI_COPY`.)

Laid out as defs and reads in sequence-number order:

| seq | op | effect |
|---|---|---|
| 0xed | CPUI_INDIRECT | **defines** `outvn` (0x1d); **reads** `invn` (0x1b) |
| 0x153 | CPUI_COPY | **defines** `invn` (0x1b) |
| 0x154 | CPUI_COPY | **reads** `outvn` (0x1d) |

**`invn` (0x1000001b) is read by the CPUI_INDIRECT at sequence 0xed,
but is not actually defined until the CPUI_COPY at sequence 0x153 --
AFTER its own read.** This is a genuine, literal use-before-define
ordering defect in the P-code for this function: a Varnode is
consumed before the operation that produces it has executed, in
sequence-number order. This is exactly why `Cover::addRefPoint`
produced an inverted range (`0x14203:153-0x14203:ed`, start greater
than end) for `invn` in the earlier session entry -- the Cover
algorithm was correctly reporting a genuinely malformed input, not
malfunctioning itself. It also explains why the backward recursion
had to fall back to whole-block `begin-end` coverage for blocks 0 and
1: with no valid, correctly-ordered definition point for `invn`
findable along the path leading to its (premature) read, the
algorithm's conservative fallback is the only sound thing it can do
given this input.

**This finally, fully closes the "why does this Cover look wrong"
question that spanned this session.** The Cover/merge machinery
(`cover.cc`, `merge.cc`) has been functioning correctly throughout;
the actual defect is upstream, in whatever pass assigns sequence
numbers / places these two divxu.w-derived ops (the CPUI_INDIRECT for
the first divide's side effect, and the CPUI_COPY that later defines
the second temporary) relative to each other for this specific
function's P-code. Given the whole investigation's throughline, this
remains most plausibly connected to 9561's changed
`RuleLoadVarnode::vnSpacebase` recursion altering how/when the
SEGMENTOP-resolved stack parameter `c` (the shared divisor feeding
both divxu.w instructions) is resolved relative to the ops that
consume it, but the exact mechanism producing this specific ordering
inversion has not yet been traced further upstream than this point.

### Recommended next step

Trace backward from CPUI_INDIRECT seq 0xed and CPUI_COPY seq 0x153 to
find which `Action`/`Rule` actually assigns or reorders these two
ops' sequence numbers, and whether 9561's `vnSpacebase` change is
directly responsible for placing them in this inverted order for this
function's specific two-chained-divide shape. This is a more precise,
tractable target than anything found earlier in the session: the
defect is now isolated to "why does the CPUI_INDIRECT at 0xed get a
lower sequence number than the CPUI_COPY at 0x153 that must logically
precede it," rather than a broad question about Cover/merge behavior.

## SESSION (continued): found a plausible mechanism -- interaction between our fix and opInsertAfter's OWN pre-existing isDead() guard

### Funcdata::opInsertAfter already has its own, separate isDead() check

Reading `Funcdata::opInsertAfter` (`funcdata_op.cc:373-404`) directly:

```cpp
void Funcdata::opInsertAfter(PcodeOp *op,PcodeOp *prev)
{
  if (prev->isMarker()) {
    if (prev->code() == CPUI_INDIRECT) {
      Varnode *invn = prev->getIn(1);
      if (invn->getSpace()->getType()==IPTR_IOP) {
        PcodeOp *targOp = PcodeOp::getOpFromConst(invn->getAddr()); // Store or call
        if (!targOp->isDead())
          prev = targOp;
      }
    }
  }
  list<PcodeOp *>::iterator iter = prev->getBasicIter();
  ...
}
```

**This function already independently re-derives and validates the
insertion point from `prev` (i.e. from whatever `afterop` the caller
passed in), using its own separate `isDead()` check** -- functionally
the same safety idea as the fix added to `trimOpOutput` this session,
but operating on `prev->getIn(1)`, not on the value `trimOpOutput`
already resolved. This was NOT visible/considered when the
`trimOpOutput` fix was designed earlier this session.

### Plausible mechanism for the use-before-define ordering defect

Our `trimOpOutput` fix now passes `afterop = op` (the original
CPUI_INDIRECT itself) whenever the decoded `indirectSource` is dead.
`opInsertAfter` then receives this `op` as `prev`. Since `op` (the
CPUI_INDIRECT itself) is ALSO a marker of code `CPUI_INDIRECT`,
`opInsertAfter`'s own internal check re-triggers: it looks at
`prev->getIn(1)` (the SAME iop-reference `trimOpOutput` already
determined was stale) and, since we already know that reference's
target `isDead()`, `opInsertAfter`'s check correctly finds
`targOp->isDead()` true and leaves `prev` unchanged, keeping the
insertion point as `op` itself. This part still looks correct in
isolation. However, this means the newly-inserted COPY ends up placed
via `iter = prev->getBasicIter(); iter++`, i.e. immediately after the
CPUI_INDIRECT position in block sequence order -- specifically
**after `op`'s existing position**, not after any earlier op that
might have historically defined the relevant value. Given the
CPUI_INDIRECT (`op`, seq 0xed) is earlier in sequence order than the
CPUI_COPY (seq 0x153) that legitimately defines `invn`, and our fix's
fallback path causes the trim-COPY to be inserted right after the
CPUI_INDIRECT rather than after the (later, seq 0x153) legitimate
definition, this is a very plausible direct mechanism for the
observed use-before-define ordering: **the fallback insertion point
chosen when the IOP reference is stale is too early relative to where
the real definition of the dependent Varnode actually occurs.**

### This suggests our `trimOpOutput` fix, while correctly preventing the
### crash, may be producing INCORRECT (though non-crashing) P-code ordering

This doesn't invalidate the fix's crash-safety property (it definitely,
confirmedly stops the SIGSEGV), but it suggests the fallback behavior
(`afterop = op` when the decoded source is dead) may need to be
smarter than simply falling back to `op` itself -- perhaps needing to
search forward/backward for a genuinely valid, later insertion point
instead of using `op`'s own position, to avoid this use-before-define
side effect. This is a refinement question for the fix, not a
reason to revert it.

### Recommended next step

Instrument `Funcdata::opInsertAfter` directly (logging `prev`'s
resolved final value and its sequence number, right before
`getBasicIter()` is called) to directly confirm this mechanism -- see
whether, for this exact `muldiv_u16_scaled` repro, `prev` does resolve
to the CPUI_INDIRECT at seq 0xed rather than something after seq
0x153, which would confirm this theory definitively rather than
leaving it as a plausible-but-unconfirmed read of the source.

## SESSION (continued): mechanism CONFIRMED directly -- opInsertAfter places the trim-COPY right after the CPUI_INDIRECT (seq 0xed), not after the real definition (seq 0x153)

Instrumented `Funcdata::opInsertAfter` directly, logging `prev`'s
resolved value right before use, scoped to `op_pc=0x14203`.

### Result

```
opInsertAfter op_pc=0x14203 prev_pc=0x14203 prev_seq=0xed prev_opcode=61 prev_block=2
opInsertAfter op_pc=0x14203 prev_pc=0x14203 prev_seq=0xed prev_opcode=61 prev_block=2
```

**Confirmed exactly as theorized in the previous entry.**
`prev_seq=0xed`, `prev_opcode=61` (`CPUI_INDIRECT`) -- `opInsertAfter`
places the new trim-COPY immediately after the CPUI_INDIRECT at
sequence `0xed`, not after the CPUI_COPY at sequence `0x153` that is
`invn`'s actual, legitimate definition. This directly, concretely
confirms the mechanism: our `trimOpOutput` fix's fallback
(`afterop = op` when the decoded IOP reference is dead) causes the
new COPY to be inserted too early, at the CPUI_INDIRECT's own
position rather than after wherever the value it's trimming actually
becomes valid, producing the use-before-define ordering that then
causes `Merge::mergeOp`'s Cover-intersection check to correctly fail.

### This confirms the fix needs refinement, not reversal

The crash-safety property of the `isDead()` guard is correct and
should be kept -- it correctly identifies that the original IOP
reference is stale and must not be blindly dereferenced. What needs
refinement is the **fallback insertion point** chosen once staleness
is detected: falling back to `afterop = op` (the CPUI_INDIRECT's own
position) is safe against crashing but is not always semantically
correct positioning for the trim-COPY, as this repro demonstrates.

### Recommended concrete next step

Investigate whether there is a way to find a genuinely correct
insertion point when the original IOP-encoded source is dead --
options to consider for a future session:
1. Search forward from `op` (the CPUI_INDIRECT) for the actual
   definition of whichever Varnode the trim is meant to protect
   (likely `op->getIn(1)` before its `CPUI_COPY` redefinition, or a
   related tracked value) and insert after that instead of after `op`
   itself.
2. Check whether Ghidra's broader dead-code/merge-restart machinery
   (e.g. `Heritage::bumpDeadcodeDelay`, referenced throughout earlier
   sessions of this investigation) is the more appropriate place to
   prevent this specific INDIRECT/COPY combination from reaching
   `trimOpOutput` in an invalid state in the first place, rather than
   trying to choose a better fallback position after the fact.
3. At minimum, confirm whether this specific ordering problem is
   fundamentally a consequence of 9561's changed `vnSpacebase`
   recursion causing the CPUI_INDIRECT's original iop-target to be
   deleted in a way that alters CPUI_COPY placement for this exact
   two-chained-divide shape, versus being a latent, pre-existing
   defect in `trimOpOutput`/`opInsertAfter`'s interaction that 9561
   merely exposes (as with the original crash). Given the crash
   itself was already confirmed pre-existing/exposed rather than
   newly introduced by 9561, the same may be true here.

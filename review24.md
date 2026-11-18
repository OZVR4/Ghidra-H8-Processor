# review24.md — muldiv_u16, ruling out merge.cc mechanisms (2026-09-14, continued)

Continues directly from review23.md. Read review19/20/21 in full this
session for the first time in this thread's continuation -- review20.md in
particular already covers almost all of this ground exhaustively and should
be treated as the primary source of truth for the merge-mechanism
investigation; do not re-derive what it already concluded.

## Critical thing review20.md already established that review22/23 didn't know

Quoting review20.md directly (paraphrased): after exhaustively instrumenting
every `Merge::merge`-family call site and confirming via live gdb Cover-data
inspection that BOTH `mergeTestBoundary` fixes (mergeOp's INDIRECT path,
mergeOpcode's COPY path) correctly detect and block their respective boundary
collisions, **no other Merge::merge-family call was ever observed firing for
the uVar4/division-result pair, despite extensive instrumentation.** review20
explicitly concluded the likely fix is "either a HighVariable-splitting
correction (if grouping is the issue) or a nameRepresentative selection fix
(if naming choice is the issue), NOT a change to the underlying
Cover-intersection threshold."

This session (continuing review22/23's independent rediscovery of the same
symptom) should have started from that conclusion rather than re-searching
for a bridging merge call site. Recorded here so a future session doesn't
repeat the same detour a third time.

## What was ruled out this session

1. **`buildDominantCopy`** (merge.cc:1315, the only call to
   `HighVariable::merge` that bypasses `Merge::merge`'s testCache gate
   entirely) -- instrumented directly. **Never called at all** for this
   decompile of muldiv_u16 (debug log file never created). Not the mechanism.
2. **`markRedundantCopies`/`checkCopyPair`** (merge.cc:1276/1437, marks a
   COPY `data.opMarkNonPrinting()` when judged redundant against an earlier
   COPY into the same HighVariable) -- read in full, not yet instrumented,
   but structurally ruled out by inspection: `checkCopyPair` is only ever
   invoked between COPYs sharing the exact same source Varnode (grouped by
   `rootVn` in the caller). `141c2`'s COPY sources from `b`; `141d7`'s COPY
   sources from the division-result SUBPIECE (`141ce`) -- different sources,
   so they are never compared against each other by this mechanism at all.
3. **`getNameRepresentative`/`compareName`** (variable.cc:492-547) -- read in
   full. Its tie-break criteria (nameLock > unaffected > persist > input >
   addrTied > protoPartial > not-internal-space > written > earlier-defined)
   would indeed favor `141c2` (earlier) over `141ce`/`141d7` as the display
   name IF the two are legitimately merged. But on reflection this is a red
   herring on its own: if `141c2` and `141ce`/`141d7` are validly merged
   (non-overlapping live ranges reusing one storage slot), printing both
   assignments in sequence (`uVar4=b; ... uVar4=divisionresult; ...
   return uVar4;`) would be completely correct C regardless of which
   instance's identity is used for the name. **The display-name choice is
   not sufficient on its own to explain a missing assignment statement.**

## Refined hypothesis for a new session

The real open question is not "which merge combined them" (review20 already
showed exhaustively that no observed merge call explains it) or "which name
gets picked" (insufficient on its own) but: **why does the `uVar4 =
divisionresult;` assignment never get PRINTED at all**, even though our
`opCopy` log (review22.md) proves the RETURN's Varnode is correctly wired to
the division result at the data-flow level?

Leading candidate: a copy-propagation/"mark implied" action pass entirely
outside `merge.cc` (e.g. `RuleCopyPropagate`, or whatever pass sets a
PcodeOp's implied/non-printing flag when its output is judged trivially
substitutable) decided the `141d7` COPY is foldable into its one descendant
(the RETURN), forward-substituting the division-result Varnode directly into
RETURN's input and marking `141d7` non-printing -- all correct at the
data-flow level, but silently removing the only statement that would show
`uVar4` being reassigned. This would fully explain every observed symptom
without requiring any merge/Cover mechanism to be at fault at all.

## Concrete next step for a new session

1. Find where a PcodeOp's "non-printing"/"implied" flag actually gets read
   by `PrintC` to decide whether to emit a statement for it -- likely a flag
   check in `PrintC::emitExpression`/`opCopy`'s caller (the statement-level
   emit loop, not `opCopy` itself, since `opCopy` we already instrumented
   only handles the RHS expression, not whether the statement gets emitted
   at all). Search for `isNonPrinting`/`nonprinting` reads.
2. Search for all *setters* of that flag across the codebase (not just
   `markRedundantCopies`'s `opMarkNonPrinting` call, which we've now ruled
   out for this pair) -- likely candidates: `RuleCopyPropagate` in
   ruleaction.cc, or an action in `action.cc` responsible for "implied"
   variable marking.
3. Instrument whichever setter(s) are found the same way as this session's
   other dumps, filtered/logged for muldiv_u16, to catch the actual call
   that silences `141d7`'s COPY.
4. Once found, the fix will very likely be a correctness check inside that
   pass (e.g. "don't mark implied/non-printing if [some condition specific
   to this boundary-adjacent storage-reuse case]"), analogous in spirit to
   the `mergeTestBoundary` pattern, but in an entirely different subsystem
   than everything investigated in review20-24. Do not assume it belongs in
   merge.cc just because that's where this investigation has spent the most
   time.
5. Still outstanding from every prior review: the `sat_add_u16` and
   sibling-function regression check, never done in any session across
   review20-24.

## Source-tree state additions this session

- **`merge.cc`**: `buildDominantCopy`'s debug dump (this session, ~line
  1401-1424) can be safely removed in a future cleanup pass -- confirmed
  never fires for muldiv_u16, contributed nothing, ruled out the hypothesis
  it was testing. `mergeTestBoundary`'s debug dump (review23.md, ~line 201)
  should be KEPT for now since it's cheap and already proven useful evidence
  once. Both `mergeTestBoundary` fixes (mergeOp INDIRECT, mergeOpcode COPY)
  remain validated and should be kept regardless of how this investigation
  concludes -- they fix a real, independently-confirmed bug class (see
  review20.md's live-gdb-confirmed capstone finding), even though they don't
  fully explain muldiv_u16's remaining symptom.
- No new rebuild was done this session past review23.md's
  `mergeTestBoundary`-instrumented binary (still the live deployed build;
  the `buildDominantCopy` instrumentation added and tested this session was
  built/deployed/tested -- see the "ruled out" section above -- so the
  currently-deployed decompile.exe DOES include it, it's just inert/
  harmless since the function is never called for this case).
- Read review19.md and review20.md in full this session (review19 is a
  different, already-fully-fixed bug -- sat_add_u16's SEGMENTOP/
  RuleLoadVarnode address-unwrapping issue -- useful only for its tooling
  lessons, already being followed). review20.md is essential reading for
  anyone continuing this specific investigation; do not skip it again.

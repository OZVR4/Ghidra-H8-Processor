# review23.md — muldiv_u16, mergeTestBoundary instrumentation result (2026-09-14, continued)

Continues directly from review22.md's "immediate next step" #1: instrument
`mergeTestBoundary` itself to see whether it's even reached for the
141c2/141ce boundary collision, and what it returns.

## What was done

Added a debug dump directly inside `Merge::mergeTestBoundary` (merge.cc:201),
logging both HighVariables' `numInstances()`, name-representative
address/def, and the function's own return value, on every call. Writes to
`C:\Users\j.brophy.CORKILLSYSTEMS\muldiv_u16_mergeboundary_debug.txt`.

Rebuild/deploy note: the gradle build (`--rerun-tasks`) timed out client-side
at the 4-minute wait again, but — as in the previous session — had actually
completed successfully server-side (confirmed via the build output's
timestamp, ~3.5 minutes old when checked). Lesson reinforced: **always verify
via file timestamp rather than assuming a client-side timeout means the
command failed.** Also hit a locked-file Copy-Item failure because PyGhidra
was still running (holding decompile.exe open); resolved by having the user
fully close PyGhidra first, then the copy succeeded and the deployed file's
timestamp was confirmed to match the fresh build.

## The result: the guard works correctly and gets bypassed anyway

Ran `force_decompile` on muldiv_u16 (0x141ba) again (still shows the bug,
unchanged — no behavior change was expected from this instrumentation-only
build). The debug log's lines 19 and 23 are the exact pair in question:

```
[mergeTestBoundary] high_out=...887E0  repAddr()=0x02  repDef()=141ce   (real division result)
                   | high_in =...87510  repAddr()=0x02  repDef()=141c2   (early "uVar4 = b")
                   | result=true(BLOCKED)
```//`
(appears twice, at numInstances()=1 and again at numInstances()=2 for
high_in, both times BLOCKED)

**`mergeTestBoundary` is being called for exactly this pair, and it correctly
rejects the merge both times.** This rules out both hypotheses from
review22.md's "why this isn't yet fixed" section:
- Not "never reached" (it's reached, twice).
- Not "returns false when it should return true" (it correctly returns
  true/BLOCKED both times).

Yet we already know from review22.md's `opCopy` log that these two DO end up
in the same HighVariable (`numInstances()=4` on the final merged group). So
the direct 141c2-vs-141ce merge decision is being correctly blocked, but the
two are getting unified anyway — almost certainly **transitively**, through a
third/fourth HighVariable that each one separately and validly merges into,
such that the union-find-style result groups them together without
`mergeTestBoundary` (or any single boundary check) ever being asked about the
final 141c2-141ce combination directly.

Supporting evidence for a transitive merge: the full log shows a chain of
BLOCKED calls at storage 0x02 across a *different* set of addresses (141e6,
141e9, 141fe, 14203 — appears to be from a separate function's decompile,
possibly `muldiv_u16_scaled` or similar, sharing the same debug log file from
an earlier/interleaved decompile in the same session) forming its own
0x02-storage merge chain with the identical pattern: an instance grows from
numInstances()=1 to 2 across repeated calls, each time correctly BLOCKED
against a new candidate at the same address. This is consistent with
HighVariable merging normally proceeding via many pairwise
`mergeTestBoundary`/`mergeTestRequired`-gated steps that build up a group
incrementally -- the open question is which *specific* step is the one that
lets 141c2 and 141ce end up in the same final group despite every direct
pairwise check between them (repeated twice) being blocked.

## Immediate next step for a new session

1. Instead of instrumenting a single function, log a global trace of every
   *actual accepted* merge (i.e. every place `HighVariable::merge()` itself
   is actually invoked -- not just the trim-decision helper functions) for
   muldiv_u16 specifically: both HighVariables' repAddr/repDef *before* the
   merge call. This will show the literal sequence of pairwise unions that
   builds up the final group step by step, which will reveal the bridging
   HighVariable (call it `A`) that separately absorbs both 141c2 and 141ce's
   groups.
2. Search for `HighVariable::merge(` call sites (not `mergeTest*` helper
   calls, the actual union operation) across merge.cc and variable.cc --
   these are the real candidates for where the guard might be skipped
   entirely.
3. Once the bridging HighVariable and its merge call site are found, decide
   whether a `mergeTestBoundary`-style second opinion can be added there
   too, narrowly, without touching the general `intersectList` level=2
   threshold (same caution as always -- confirmed to cause regressions on
   sat_add_u16 if loosened globally).
4. Still outstanding from review21/22: the `sat_add_u16` and sibling-function
   regression check, not yet done in any session.

## Current source-tree state additions this session

- **`merge.cc`**: `mergeTestBoundary` now has the debug dump described above,
  at merge.cc:201 (function grew from 8 to 31 lines). Same
  "remove before upstream-facing change" status as all other debug
  instrumentation in this investigation.
- **`decompile.exe`** (build output and deployed copy) rebuilt and confirmed
  redeployed (timestamps verified matching) with this instrumentation on top
  of everything review21/22.md already listed.
- New debug log file:
  `C:\Users\j.brophy.CORKILLSYSTEMS\muldiv_u16_mergeboundary_debug.txt`
  (in addition to the existing `muldiv_u16_opcopy_debug.txt` and the
  `opReturn` dump's log from review20/21.md -- none of these have been
  cleared between runs, so they accumulate entries across sessions/decompiles
  of possibly multiple functions; keep this in mind when reading them, as
  seen with the apparent interleaved-function entries noted above).

# review21.md — muldiv_u16 investigation, fresh handoff (2026-09-14)

This is a condensed, self-contained handoff. Full blow-by-blow history (P-code
traces, every dead end, every instrumentation attempt) lives in review20.md
(3836 lines) — refer back to it for detailed rationale on anything summarized
here. This file exists so a new session doesn't have to read all of review20.md
to get productive again.

## The bug, in one sentence

`muldiv_u16` (and sibling functions) compute the correct result internally, but
the decompiler's printed C source shows the function returning the second
parameter (`b`) instead of the division result — a **display/HighVariable
naming bug**, not a real data-flow or computation bug.

## Ground truth: the actual computation has always been correct

Confirmed via `mcp__ghidra__get_function_pcode` (granularity=high) against the
live binary. Traced the RETURN op's real SSA def-use chain by hand:

```
RETURN (0x141da) reads register[0]
  <- COPY (0x141d7): register[0] = register[2]
    <- register[2]'s LAST def before 0x141d7 is COPY (0x141ce):
       register[2] = SUBPIECE(INT_DIV(dividend, divisor))
       (the genuine division result)
```

No p-code edge anywhere connects `b` (stack offset 0x6) to the return value.
**Anyone using raw p-code, emulation, or disassembly gets the correct answer.**
The bug is confined to human-readable decompiler *source* output.

## Two real fixes applied and kept (both in `merge.cc`)

Root cause: `Cover::intersectList`'s `level=2` pre-filter (used by
`Merge::merge`'s `testCache.intersection()` check) silently misses a genuine
**boundary-touching** Cover collision — one variable's live range stops at
address X, another's starts at exactly that same X, same block index. This was
confirmed **live in a debugger** (see below) — not inferred: `blockisect` comes
back empty despite the exact touch being visible in the input Cover dump
immediately beforehand.

There's an existing helper, `Merge::mergeTestBoundary(HighVariable*,
HighVariable*)`, already applied to `mergeOp`'s CPUI_INDIRECT trim-loop path
(this fix predates this session and is validated/kept). This session found and
fixed a **second, independent** call site with the same defect:

**Fix (this session): `Merge::mergeOpcode`, guarding its CPUI_COPY
required-merge.** In the `mergeTestRequired(...)` block, before the existing
`merge(vn1->getHigh(), vn2->getHigh(), false)` call, added:
```cpp
if (!mergeTestBoundary(vn1->getHigh(),vn2->getHigh())) {
  // ... existing merge() call ...
}
```
Skips the merge entirely on a detected boundary collision (mergeOpcode has no
trim/split fallback the way mergeOp does, so "don't merge" is the only safe
action). **Confirmed live via gdb**: `mergeTestBoundary` correctly returns
`true` (block) for the exact traced colliding pair and `false` (allow) for an
unrelated pair in the same run.

**Effect of this fix on the live binary:** changed the output from `return b;`
(direct) to `uVar4 = b; ... return uVar4;` (one more hop). **The user-visible
bug is NOT fully fixed** — see "still open" below.

## New tool built this session: standalone decompiler test harness

No PyGhidra, no pipe protocol, no timing races. This is now the preferred way
to debug the decompiler core.

- **Binary**: `C:\Users\j.brophy.CORKILLSYSTEMS\decomp_test_build\decomp_test_dbg.exe`
- **Built from**: the project's own pre-existing (but never wired up on
  Windows) `Ghidra/Features/Decompiler/src/decompile/cpp/Makefile` and
  `test.cc` (a plain `argc/argv` console entry point — NOT
  `ghidra_process.cc`'s pipe-protocol `main()` used by the real
  `decompile.exe`/PyGhidra).
- **Compiler**: MinGW g++ directly (`C:\mingw64\bin\g++.exe`), not WSL (WSL is
  broken on this machine per the user).
- **Build command** (from `Ghidra/Features/Decompiler/src/decompile/cpp/`):
  ```
  C:\mingw64\bin\g++.exe -std=c++17 -g -O0 -DCPUI_DEBUG -Wno-sign-compare
    -o decomp_test_dbg.exe <94 files, see filelist3.txt> -lws2_32 -lz
  ```
  Full file list preserved at
  `C:\Users\j.brophy.CORKILLSYSTEMS\decomp_test_build\filelist3.txt`.
  Takes ~5-6 min single-threaded, no incremental caching (worth fixing if many
  more iterations are needed — see "process notes" below).
- **Portability fixes needed** (all in the live source tree, all safe to
  keep): dropped `bfd_arch.cc`/`loadimage_bfd.cc`/`analyzesigs.cc`/
  `codedata.cc` (BFD dependency, not needed — raw bytes supplied directly);
  did not define `__TERMINAL__` (avoids `ifaceterm.hh`'s POSIX
  `<termios.h>` requirement — script-driven, not interactive, so this is
  fine); `filemanage.cc` got a genuine, permanent MinGW portability fix
  (guarded `dirent.d_type` usage, which doesn't exist at all on MinGW, behind
  `#if defined(__MINGW32__) || defined(__MINGW64__)`, always taking the
  existing `stat()`-based fallback path on that platform).
- **Debugger**: `C:\mingw64\bin\gdb.exe` — works cleanly, no timing issues, no
  crashes. This is a completely different experience from cdb/WinDbg against
  the real `decompile.exe` (see "abandoned approaches" below).
- **Running it**:
  ```
  decomp_test_dbg.exe -sleighpath "<Ghidra install root>" -path "<dir with .xml>" datatests
  ```
  `-sleighpath` must be the Ghidra install root (e.g. `...\ghidra_12.2_DEV`),
  not the `Processors` subfolder — `SleighArchitecture` scans subdirectories
  itself.
- **Repro XML**: `C:\Users\j.brophy.CORKILLSYSTEMS\decomp_test_build\muldiv_repro_tests\muldiv_repro.xml`
  — real H8/539F bytes and language ID pulled directly from the live binary
  via `read_memory`/`get_current_program_info`. **Important limitation**: this
  minimal repro has no prototype/parameter setup, so it falls back to
  "irregular input" naming (`in_stack_00000006` instead of `b`) and does
  **not** reproduce the `uVar4` symptom — only the original, now-fixed
  `mergeOpcode` collision. A prototype-aware repro (see "next step" below)
  is still needed to reproduce the *current* remaining bug in this harness.
- **Debugging output**: `FunctionTestCollection::runTests` (`testfunction.cc`)
  swallows all console output (including `print C`) into an internal
  `ostringstream` used only for `stringmatch` checks. A temporary
  `std::cerr` dump was added right after `string result = bulkout.str();` to
  see the actual decompiled text — needed `#include <iostream>` (not
  transitively available) and explicit `std::` qualification. **Still in
  place, should be reverted eventually but is harmless.**

## Live-debugger confirmation of the root cause (the smoking gun)

Using `decomp_test_dbg.exe` + gdb, set a breakpoint at `merge.cc:1243` (the
`a->getCover().intersectList(blockisect, b->getCover(), 2)` call inside
`HighIntersectTest::intersection`). Dumped both input Covers immediately
before the call:
```
a->getCover(): 1 block  -- [2] = {start = 0xdc5350, stop = 0xd91240}
b->getCover(): 3 blocks -- ... [2] = {start=0x0, stop=0xdc5350}
```
`b`'s block `[2]` stops at **exactly** `0xdc5350`; `a`'s block `[2]` starts at
that **exact same** value. After the call: `blockisect` is **empty** (size 0).
No remaining inference anywhere in this causal chain — this was watched
executing directly, not reconstructed from symptoms.

## What's still open (the actual next step)

The `mergeTestBoundary`-in-`mergeOpcode` fix changed the output but did not
remove the bug. Traced forward from `PrintC::opReturn` (`printc.cc:783`) in
the live, fixed `decompile.exe`:

- Added debug logging there (`std::ofstream` to
  `muldiv_u16_opreturn_debug.txt`), dumping the RETURN op's input Varnode,
  its defining PcodeOp address, its HighVariable, `numInstances()`, and
  `getNameRepresentative()`.
- **Result: the RETURN statement is completely innocent.** Both RETURN sites
  in the function show `retVn->getDef()=141d7` (matches the ground-truth
  p-code trace exactly), `numInstances()=1` (never merged/grouped with
  anything), and `nameRep == retVn` (no naming-selection issue). If `uVar4`
  really is this HighVariable's assigned name, the RETURN statement is
  printing the truth.
- **Conclusion: the bug is in the preceding assignment statement,
  `uVar4 = b;`, not the RETURN.** Something makes that COPY's printed
  right-hand side resolve to `b` when the p-code says its source should be
  the division-result SUBPIECE.

### Immediate next step for a new session

1. Find and instrument the COPY/assignment-printing code path in `printc.cc`
   (look for `PrintC::opCopy` or wherever a plain assignment statement gets
   emitted — this hasn't been located yet this session) with the same kind
   of identity dump used successfully for `opReturn`: source Varnode, its
   def, its HighVariable, `numInstances()`, `getNameRepresentative()`.
2. Run it against the live binary (same `force_decompile` on `0x141ba` /
   `muldiv_u16`, same log-file pattern) and see whether the `uVar4 = b;`
   line's HighVariable shows `numInstances() > 1` (a grouping bug — two
   genuinely different SSA values sharing one HighVariable) or a
   `nameRep` that doesn't match its own defining op (a
   naming-selection bug).
3. Cross-reference against the raw P-code dump (via
   `mcp__ghidra__get_function_pcode`) to identify exactly which Varnode is
   really being printed as `b` on that line.
4. Design a fix scoped as narrowly as the two `mergeTestBoundary` fixes
   already applied — do not touch `Cover::intersectList`'s global threshold
   (confirmed elsewhere in review20.md history that loosening it globally
   causes regressions).
5. Once a fix candidate exists, verify via the **live PyGhidra
   `force_decompile` loop** (fast and reliable now — environment issues are
   resolved) that `uVar4`/`return uVar4` disappears entirely, leaving only
   the mathematically correct division-result return.
6. **Do the standing regression check that has not yet been done this
   session**: `sat_add_u16` (the established control function) has not been
   re-verified against either `mergeTestBoundary` fix yet. Do this before
   considering anything finished. Also re-check the other named sibling
   functions from early in review20.md (`muldiv_u16_scaled`,
   `muldiv_s16_rounded_3op`, `muldiv_s16_rounded`, `weighted_blend_u8frac`).

## Abandoned approaches (don't retry these)

- **cdb/WinDbg attached to the live `decompile.exe`**: extremely fragile.
  `decompile.exe` is a sub-second, one-shot, no-argument, pipe-protocol child
  process spawned by PyGhidra — cdb's own attach/initialize overhead
  routinely exceeds the target's entire lifetime. One attempt to attach cdb
  to the *parent* PyGhidra process (with `-o` for child-process debugging)
  **crashed PyGhidra** when cdb was killed (Windows terminates the debuggee
  when its debugger dies, since `DebugSetProcessKillOnExit(FALSE)` was never
  set). Do not attach any debugger directly to the live PyGhidra process
  again. A `MULDIV_DEBUG_WAIT` environment-variable gate exists in
  `ghidra_process.cc`'s real `main()` (polls `IsDebuggerPresent()`, calls
  `DebugBreak()`) as a way to get a real attach window on the *child* only —
  currently unset/cleared. If re-enabled, PyGhidra must be restarted for the
  child to inherit it, and it only affects `decompile.exe`, never the parent.
- **WSL**: broken on this machine per the user, don't retry.
- **`decomp_test_dbg.exe`'s own `test.cc`/`main()` route was initially
  confused with `consolemain.cc`** — that file exists in the tree but is
  **not** part of any actual build target (confirmed via grep — zero
  references in `buildNatives.gradle`). A debug-wait gate was mistakenly
  added there first and had to be reverted; the real entry point for the
  live `decompile.exe` is `ghidra_process.cc`'s `main()`.

## Current source-tree state (nothing committed to git)

All changes live in the working tree at `C:\Users\j.brophy.CORKILLSYSTEMS\github\ghidra`:

- **`merge.hh` / `merge.cc`**: both `mergeTestBoundary` fixes (mergeOp path,
  pre-existing/validated; mergeOpcode path, this session/validated-but-
  insufficient-alone). Plus a `g_mergeCallSiteTag` global debug marker
  (harmless, still present, tags which of 9 call sites is executing —
  useful for future symbol-inherit tracing, safe to keep or remove).
- **`variable.cc`**: symbol-inherit debug log in `HighVariable::mergeInternal`
  (writes to `muldiv_u16_syminherit_debug.txt`), extended to print
  `g_mergeCallSiteTag`. Harmless, currently produces empty output for the
  remaining bug (confirmed the remaining issue is NOT a symbol-inheriting
  merge).
- **`printc.cc`**: new debug log in `PrintC::opReturn` (writes to
  `muldiv_u16_opreturn_debug.txt`). Needed `#include <fstream>` added.
- **`filemanage.cc`**: genuine MinGW portability fix (see above) — this one
  is NOT debug instrumentation, fine to keep/upstream independently of
  everything else.
- **`testfunction.cc`**: temporary `std::cerr` dump of full captured test
  output (only relevant to `decomp_test_dbg.exe`, not the real
  `decompile.exe`). Needed `#include <iostream>`.
- **`ghidra_process.cc`**: `MULDIV_DEBUG_WAIT`-gated debugger-wait block in
  the real `main()`. Currently a no-op (env var unset). Needs `<windows.h>`
  under `#ifdef _WINDOWS`.
- **`buildNatives.gradle`**: switched the MSVC (`VisualCpp`) branch to a real
  debug build — `/Od /Zi /FS` instead of `/O2 /Oy /GL`, `/DEBUG` linker flag
  instead of `/LTCG`. Original release settings preserved as comments. This
  means **the live `decompile.exe` deployed to PyGhidra is currently an
  unoptimized debug build**, not the normal release build — fine for
  continued investigation, but worth remembering/reverting before any
  performance-sensitive use or final upstream-facing change.

All of the above debug instrumentation (everything except the
`mergeTestBoundary` fixes themselves and the `filemanage.cc` portability fix)
should be removed before preparing any upstream-facing change, per the
established convention throughout review20.md.

## Environment notes

- `MULDIV_DEBUG_WAIT` user environment variable: currently **unset/cleared**.
- PyGhidra needs a full restart for any environment variable change to take
  effect (child processes inherit at parent-launch time only).
- The live decompile.exe at
  `C:\Users\j.brophy.CORKILLSYSTEMS\Downloads\ghidra_12.2_DEV_20260818\ghidra_12.2_DEV\Ghidra\Features\Decompiler\os\win_x86_64\decompile.exe`
  currently has the debug build + both fixes + the opReturn debug log
  deployed and working (confirmed via matching timestamps/sizes with the
  gradle build output).

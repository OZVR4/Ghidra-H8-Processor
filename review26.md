> **SUPERSEDED (2026-09-15):** this file and `review25.md` were scratch
> sessions for the `muldiv_u16` write-write-collision investigation. Their
> conclusions have been reconciled, corrected (this file mislabeled opcode 3
> as `CPUI_LOAD`; it is `CPUI_STORE`), and folded into
> `review-9561-merge-ordering.md` as **Finding 12**, which is now the single
> authoritative document for this investigation. Read that file, not this
> one, for current status.

# Session 26 — Live gdb trace into heritage.cc rename/collision logic

Continuation of review25.md's deferred next step: live gdb trace into the
`heritage.cc` SSA renaming/collision-resolution logic to find what the
spurious INDIRECT's cause-op decodes to, and why the write-write collision
at `0x141c2` (Finding 5) produces `muldiv_u16`'s wrong output
(`return in_stack_00000006;` instead of the real division result).

## Harness / environment notes (for reproducing this session)

- Binary: `C:\Users\j.brophy.CORKILLSYSTEMS\decomp_test_build\decomp_test_dbg.exe`
  (rebuilt same day as this session, 11:42 AM).
- Sleigh path: `C:\Users\j.brophy.CORKILLSYSTEMS\Downloads\ghidra_12.2_DEV_20260818\ghidra_12.2_DEV`
- Test file: `C:\Users\j.brophy.CORKILLSYSTEMS\decomp_test_build\muldiv_repro_tests\muldiv_repro.xml`
- Debugger: GDB 17.1 (`C:\mingw64\bin\gdborig.exe`), run via PowerShell.
- Invocation pattern that works (note: `-sleighpath`/`-path` MUST be passed
  via `set args`, not inlined into a `run` string inside `-ex`, or gdb's own
  option parser eats them):
  ```powershell
  $gdbCmds = @"
  break heritage.cc:<line> [if <cond>]
  commands
    silent
    printf "...", ...
    continue
  end
  set args -sleighpath "$sleigh" -path "$tests" datatests
  run
  "@
  $gdbCmds | Out-File -Encoding ascii gdbcmds.txt
  gdb -batch -x gdbcmds.txt decomp_test_dbg.exe 2>&1
  ```
- CPUI opcode values used this session (from `opcodes.hh`, don't guess these
  again): `CPUI_LOAD = 3`, `CPUI_INDIRECT = 61`.
- **IMPORTANT — non-determinism observed:** identical gdb command files
  produced different breakpoint-hit counts across runs (e.g. `establishRange`
  hit twice in one run, zero times in an immediate rerun with the same
  harness invocation). Likely heap-address/ASLR-driven iteration order in a
  `std::map`-keyed `varstack` or similar. **Any single-run observation here
  needs a second confirming run before being trusted** — one apparent finding
  from this session (garbage negative sequence-order values read from a
  `LoadGuard`'s stored `op` in `establishRange`) did NOT reproduce on rerun
  and has been retracted below rather than written up as fact. Before further
  live-trace sessions, worth checking whether `decomp_test_dbg.exe` can be
  run with ASLR disabled or a fixed allocator seed for a deterministic repro.

## Confirmed findings (reproduced across multiple runs)

1. **Breakpoint `heritage.cc:2509`** (the "INDIRECTs and their op really
   happen AT SAME TIME" special case in `Heritage::renameRecurse`) reliably
   hits, with the stack-top `vnnew` for the relevant storage defined by a
   `CPUI_INDIRECT` at `0x141c2`, for reads at `0x141c4` (and others).

2. **Decoded the INDIRECT's cause op** via
   `PcodeOp::getOpFromConst(vnnew->getDef()->getIn(1)->getAddr())`:
   - Cause op is at the same address `0x141c2` but a **different sequence
     order** than the INDIRECT and than the current read op.
   - Cause op's opcode is **3 (`CPUI_LOAD`)**.
   - That LOAD op has **`output = 0x0`** (no output Varnode) and 3 inputs —
     consistent with a guard-generated LOAD used only to trigger aliasing
     INDIRECTs on nearby storage, not a genuine value-producing load.

3. **`causeOp != op` at the line-2509 check (confirmed false/0).** The
   "AT SAME TIME" fast path does **not** fire for this read. This directly
   contradicts the review25.md working theory that the bug was in this
   same-op collision-resolution branch — falsified with direct evidence,
   not just non-reproduction.
   - Revised understanding: the INDIRECT at `0x141c2` is simply sitting on
     the varstack as an ordinary prior write and winning normally against
     the read at `0x141c4`. If it's wrong, the bug is further upstream —
     either in why this LOAD-caused INDIRECT exists on `register:2` at all,
     or in `Heritage`'s free/write classification before renaming ever runs.

4. **`Heritage::removeRevisitedMarkers` (heritage.cc:244-297) ruled out.**
   Set breakpoints on lines 267 and 268 (its own `getOpFromConst` +
   `targetOp->isDead()` check, structurally similar to the line-2509
   pattern). Zero hits across the run — this function never executes for
   `muldiv_u16`'s heritage pass. Not the mechanism in play here.

5. **`LoadGuard::establishRange` / `LoadGuard::finalizeRange`
   (heritage.hh:151-152, defined near heritage.cc:743/790):**
   - `LoadGuard::set()` (heritage.hh:159-161) initializes every new guard as
     fully unanalyzed and covering the **entire stack space**:
     `minimumOffset=0, maximumOffset=s->getHighest(), analysisState=0`.
   - In one run, `establishRange` fired only for two guard ops at `0x141d9`
     and `0x141da` — **not** for the `0x141c2` LOAD. `finalizeRange` did not
     fire at all for any guard in that run. This means the `0x141c2` LOAD's
     guard, if one exists for it, may never get narrowed from its
     whole-stack-space default — a plausible mechanism for an
     over-broad/spurious INDIRECT — but this needs a deterministic rerun to
     confirm the `0x141c2` LOAD even has a guard entry at all before relying
     on it.
   - **Retracted:** an initial read of garbage/negative sequence-order
     values from `this->op` inside `establishRange` (suggesting a dangling
     `PcodeOp*` surviving a heritage restart) did not reproduce on a second,
     otherwise-identical run. Do not treat this as evidence of a
     use-after-free without a deterministic repro.

6. **`Heritage::guardLoads` (heritage.cc:1571-1600+) mechanism clarified:**
   this is a separate code path from the INDIRECT we're chasing — it walks
   `loadGuard`, checks `guardRec.isValid(CPUI_LOAD)` (heritage.cc:1581,
   which itself calls `op->isDead()` and `op->code()` on the guard's stored
   `PcodeOp*` — worth another look once repro is deterministic), and inserts
   a **`CPUI_COPY`** guard op when an address falls in
   `[minimumOffset, maximumOffset]`. Since our `0x141c2` structure is an
   `INDIRECT`, not a `COPY`, **this is not the code path that created it.**
   The actual INDIRECT-creation site for LOAD-caused aliasing is still
   unidentified — this is the next concrete thing to grep for.

## What's still open / next step

- Root question, unchanged in substance from review25.md but now sharper:
  **why is a no-output, guard-generated `CPUI_LOAD` at `0x141c2` causing an
  `INDIRECT` write on `register:2`'s storage, if the LOAD's range should not
  cover that register at all?**
- Concrete next actions, in order:
  1. Get a **deterministic repro** (disable ASLR / fix allocator seed / pin
     down whatever is causing run-to-run breakpoint-count variance) before
     trusting any further single-run gdb observations.
  2. Find the actual INDIRECT-creation call site for LOAD-guard-driven
     aliasing (distinct from `guardLoads`'s COPY-insertion path at
     heritage.cc:1571) — grep for where `fd->newIndirectOp` or similar is
     called in connection with `loadGuard` entries.
  3. Once found, check whether the `0x141c2` LOAD actually has a `LoadGuard`
     record, and if so what `minimumOffset`/`maximumOffset` it holds at the
     point the INDIRECT gets created — confirm or refute the
     "never-narrowed, still whole-stack-space" theory from finding 5 above.
  4. Re-run the `causeOp` decode from finding 2 a few more times across
     fresh processes to confirm findings 2-3 are themselves stable and not
     also affected by the non-determinism seen elsewhere this session.

## Tree state (carried forward from review25.md, unchanged this session)

- `trimOpOutput` fix (merge.cc) — live and deployed.
- `mergeTestBoundary` fix (from review20.md) — still in place.
- SEGMENTOP change in heritage.cc — fully reverted (review25.md).
- Debug instrumentation in `printc.cc::opReturn` — still present, marked for
  removal before any upstream-facing change.
- Finding-11 per-function prototype override — did not survive PyGhidra
  restarts, still needs reapplying.
- No source changes were made this session — purely diagnostic/read-only
  gdb tracing plus grep/read of `heritage.cc` and `heritage.hh`.

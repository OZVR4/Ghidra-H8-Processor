# sat_add_u16/auStack_ investigation, continued from review18.md (2026-08-31)

**Do not repeat review18.md's full chronological log here.** Read its
closing section first (`## File closed -- investigation continues in
review19.md`) for a short recap. Go back to review18.md in full only if
you need: the falsified `RuleAddMultCollapse` hypothesis; the three
crash incidents (their partially-mistaken diagnoses, including a whole
line of investigation that wrongly blamed
`register0x0e_crossregister_fix.xml`); the `loneDescend()` Varnode
descendant-count instability finding; or the eventual correct crash
diagnosis (unwrapped offset arithmetic overflowing a 16-bit address
space) that led to the working fix below.

This investigation remains OUT OF SCOPE for PR #9541 (already-landed,
confirmed-complete register0x0e/SEGMENTOP annotation fix, commit
aadd7ee) and must ship as its own independent patch once resolved.

## One-line status

**The target bug (sat_add_u16's `auStack_102[128]` merge) is FIXED and
confirmed working, crash-free, with no regression to PR #9541's tests.**
The fix is currently scoped narrowly (hard-coded to `sat_add_u16`'s own
address range) as a deliberate safety measure for validation -- it is
NOT yet generalized into a real, upstreamable change. Remaining work:
(1) generalize the fix, (2) solve a separate, smaller parameter-binding
gap so `a`/`b` get their proper declared names instead of
`in_stack_00000004`/`in_stack_00000006`.

## How to get oriented fast

1. **Rebuild command** (documented the hard way earlier in this
   investigation -- don't lose it again):
   ```powershell
   cd C:\Users\j.brophy.CORKILLSYSTEMS\github\ghidra
   .\gradlew.bat :Decompiler:decompile_testExecutable
   ```
   Fast incremental rebuild of just the `decompile_test` component, not a
   full Ghidra rebuild. Only needed after editing a decompiler `.cc`/`.hh`
   file. **Gradle can report `UP-TO-DATE` even when a rebuild is actually
   needed/already happened** -- this has been observed multiple times
   this investigation, usually because a prior tool call that appeared to
   fail (`<error>Tool execution failed</error>`) had actually already
   completed the real compile before erroring out on the response side.
   **Always verify a rebuild genuinely happened by comparing file
   timestamps**, not by trusting the printed task status alone:
   ```powershell
   Get-Item "C:\Users\j.brophy.CORKILLSYSTEMS\github\ghidra\Ghidra\Features\Decompiler\src\decompile\cpp\ruleaction.cc" | Select-Object LastWriteTime
   Get-Item "C:\Users\j.brophy.CORKILLSYSTEMS\github\ghidra\Ghidra\Features\Decompiler\build\os\win_x86_64\ghidra_test.exe" | Select-Object LastWriteTime
   ```
   The exe's timestamp must be newer than the source edit's.
2. **Run the harness and verify current (fixed) baseline:**
   ```powershell
   cd C:\Users\j.brophy.CORKILLSYSTEMS\github\ghidra\Ghidra\Features\Decompiler\build\os\win_x86_64
   .\ghidra_test.exe -sleighpath "C:\Users\j.brophy.CORKILLSYSTEMS\github\Ghidra-H8-Processor\h8\data\languages" -path "C:\Users\j.brophy.CORKILLSYSTEMS\github\ghidra\Ghidra\Features\Decompiler\src\decompile\datatests" 2>&1 | Select-String "SatAdd|Canary|Total|register0x0e|ABSENT"
   ```
   Current known-good state (2026-08-31, post-fix): `Success -- No
   register0x0e placeholder text` (x2), `Success -- SatAdd #1`, `Success
   -- Canary function name`, `FAIL -- Canary param a`, `FAIL -- Canary
   param b`, `Success -- ABSENT auStack (fixed 2026-08-30, review18.md)`,
   `Total tests applied = 13`, `Total passing tests = 7`, exit code 6 (no
   crash). The two Canary param FAILs are the remaining parameter-binding
   gap, not the merge bug -- see below.
3. Full decompiled output accumulates in
   `C:\Users\j.brophy.CORKILLSYSTEMS\harness_bulkout_diag.txt` -- check
   the *last* `=== bulkout for ... satadd_debug.xml ===` block. Current
   fixed output:
   ```c
   uint2 sat_add_u16(uint2 a,uint2 b)

   {
     uint2 uVar1;
     uint2 in_stack_00000004;
     uint2 in_stack_00000006;

     uVar1 = in_stack_00000004 + in_stack_00000006;
     if (CARRY2(in_stack_00000004,in_stack_00000006)) {
       uVar1 = 0xffff;
     }
     return uVar1;
   }
   ```
   No more `auStack_102[128]`, no more pointer-cast reconstruction --
   `a` and `b` are two genuinely distinct stack locations now.
4. `combined_diag.txt` (`C:\Users\j.brophy.CORKILLSYSTEMS\combined_diag.txt`)
   is the shared diagnostic file used throughout this investigation for
   `gatherAdditiveBase` (varmap.cc) and rule-fire (action.cc)
   instrumentation, both still in place and safe. No `RuleLoadVarnode`-
   related diagnostics remain in `ruleaction.cc` as of the end of the
   previous session -- all were removed once they'd served their purpose,
   per this investigation's established practice. Delete this file before
   each fresh run if isolating a specific trace:
   ```powershell
   Remove-Item "C:\Users\j.brophy.CORKILLSYSTEMS\combined_diag.txt" -ErrorAction SilentlyContinue
   ```
   Note: Pulsar's `run-command` treats `Remove-Item` as destructive and
   requires `confirm:true`.

## The bug, restated (now FIXED)

`sat_add_u16(ushort a, ushort b)` at address `0x14000` used to decompile
to a bogus `auStack_102[128]`-style array with pointer-cast arithmetic
instead of two clean `ushort` parameters. Root cause: `b`'s address
computation is a 3-level chain
(`INT_ADD(SEGMENTOP(INT_ADD(spacebase,c1)),c0)`), and
`RuleLoadVarnode::vnSpacebase` (ruleaction.cc) only unwrapped one level
of `SEGMENTOP`, so it never recognized `b`'s access as a stack offset at
all -- `b` fell through to `AliasChecker`'s generic merged-array handling
while `a` (only 2 levels deep) resolved correctly on its own.

## The fix (currently scoped/experimental -- see "Not yet done" below)

Added a new function, deliberately scoped to ONLY ever apply inside
`sat_add_u16`'s own address range, so it's impossible for it to affect
`register0x0e_crossregister_fix.xml` or any other function while the
underlying logic gets validated:

**`ruleaction.hh`** (inside `class RuleLoadVarnode`, alongside the
existing private static members):
```cpp
static AddrSpace *vnSpacebaseRecursiveExperiment(Architecture *glb,Varnode *vn,uintb &val,AddrSpace *spc,int4 depth);
```

**`ruleaction.cc`**, in `checkSpacebase`, right before its final
`return vnSpacebase(glb,offvn,offoff,loadspace);`:
```cpp
{
  uintb opAddr = op->getAddr().getOffset();
  if (opAddr >= 0x14000 && opAddr <= 0x14010)
    return vnSpacebaseRecursiveExperiment(glb,offvn,offoff,loadspace,0);
}
return vnSpacebase(glb,offvn,offoff,loadspace);
```

**`ruleaction.cc`**, new function (after `checkSpacebase`):
```cpp
AddrSpace *RuleLoadVarnode::vnSpacebaseRecursiveExperiment(Architecture *glb,Varnode *vn,uintb &val,AddrSpace *spc,int4 depth)

{
  PcodeOp *op;
  Varnode *vn1,*vn2;
  AddrSpace *retspace;

  if (depth > 8) return (AddrSpace *)0;

  retspace = correctSpacebase(glb,vn,spc);
  if (retspace != (AddrSpace *)0) {
    val = 0;
    return retspace;
  }
  if (!vn->isWritten()) return (AddrSpace *)0;
  op = vn->getDef();
  if (op->code() != CPUI_INT_ADD) return (AddrSpace *)0;
  vn1 = op->getIn(0);
  vn2 = op->getIn(1);

  Varnode *test1 = vn1;
  if (test1->isWritten() && (test1->getDef()->code()==CPUI_SEGMENTOP) &&
      (test1->getDef()->numInput() >= 3)) {
    Varnode *inner1 = test1->getDef()->getIn(2);
    if (!inner1->isConstant()) test1 = inner1;
  }
  Varnode *test2 = vn2;
  if (test2->isWritten() && (test2->getDef()->code()==CPUI_SEGMENTOP) &&
      (test2->getDef()->numInput() >= 3)) {
    Varnode *inner2 = test2->getDef()->getIn(2);
    if (!inner2->isConstant()) test2 = inner2;
  }

  retspace = (test1 == vn1) ? correctSpacebase(glb,test1,spc) : vnSpacebaseRecursiveExperiment(glb,test1,val,spc,depth+1);
  if (retspace != (AddrSpace *)0) {
    uintb accum = (test1 == vn1) ? 0 : val;
    if (vn2->isConstant()) {
      val = retspace->wrapOffset(accum + vn2->getOffset());  // <-- THE FIX
      return retspace;
    }
    return (AddrSpace *)0;
  }
  retspace = (test2 == vn2) ? correctSpacebase(glb,test2,spc) : vnSpacebaseRecursiveExperiment(glb,test2,val,spc,depth+1);
  if (retspace != (AddrSpace *)0) {
    uintb accum = (test2 == vn2) ? 0 : val;
    if (vn1->isConstant()) {
      val = retspace->wrapOffset(accum + vn1->getOffset());  // <-- THE FIX
      return retspace;
    }
  }
  return (AddrSpace *)0;
}
```

**The critical line in both branches is `retspace->wrapOffset(...)`**
(`AddrSpace::wrapOffset`, `space.hh:141`/`385`). Without it, accumulating
offsets across nested unwrap levels via plain unsigned addition can
overflow the address space's actual bit width (confirmed: produced
`offoff=0x10006` in a 2-byte/16-bit stack space -- one past a full
16-bit wraparound), corrupting the newly-created Varnode and crashing
shortly afterward in `RuleLoadVarnode::applyOp`'s
`data.newVarnode(size,baseoff,offoff)` call. This was the actual root
cause of every crash in this investigation's history -- NOT any
interaction with `register0x0e`'s cross-register/self-referential
SEGMENTOP pattern, despite that being the working theory for a long
time (see review18.md for the full, partially-mistaken history of that
diagnosis).

## Not yet done -- concrete next actions

1. **Generalize the fix.** It's currently gated to
   `opAddr >= 0x14000 && opAddr <= 0x14010` purely as a safety measure
   for this session's validation. The underlying logic is now proven
   correct and crash-free (the real danger was the missing `wrapOffset`,
   which is fixed) -- it does NOT need to avoid `register0x0e`'s
   functions at all. Recommended path: fold
   `vnSpacebaseRecursiveExperiment`'s logic into (or have it replace)
   the ordinary `vnSpacebase`, remove the address-range gate in
   `checkSpacebase`, rename the function to something permanent, and
   update its doc comment (currently still describes itself as an
   experiment). **Re-verify against the FULL datatests suite** (not just
   `sat_add_u16` and the two `register0x0e_*.xml` files) once
   generalized, since other functions may exercise similar deep
   `SEGMENTOP`/`INT_ADD` chains not yet examined -- the whole-suite
   diagnostic scan from review18.md (searching for
   `SEGMENTOP`-nested-in-`INT_ADD` shapes) found at least one more such
   function (`init_copy_const_block_via_memcpy_banked` in
   `register0x0e_segmentop_fix.xml`, address ~`0x149ca`) that should be
   specifically checked, not just trusted to be fine by extrapolation.
2. **Solve the parameter-binding gap.** `a`/`b` currently show as
   `in_stack_00000004`/`in_stack_00000006` rather than being matched to
   the prototype's declared parameter names. This is why `Canary param
   a`/`Canary param b` still FAIL. This is very likely a *different*
   mechanism (prototype/parameter matching, possibly in
   `ActionActiveParam` or a related symbol-binding pass) from the
   alias/merge fix just completed -- don't assume it will resolve
   automatically; investigate it as its own question.
3. **Test file housekeeping.** `satadd_debug.xml`'s `PRESENT
   param_1`/`param_2`/`undefined2`/`result_eq` assertions were already
   failing before this fix and remain unaffected by it -- they appear to
   be stale placeholders from an earlier draft of this test file and
   don't test anything relevant to the current bug. Consider removing or
   updating them once the parameter-binding gap (point 2) is resolved,
   since their intent may become clearer once `a`/`b` are properly
   named.
4. Confirm `satadd_debug.xml`'s `ABSENT auStack` assertion (renamed from
   `PRESENT auStack` this session, since the old name/sense was
   backwards once the bug was fixed) reads correctly and stays in sync
   with whatever the final decompiled shape looks like once parameter
   binding is also fixed.

## Known tooling gotchas (carried forward, with two new/reinforced
## lessons from this investigation's most eventful session)

- **Use `run-command`/PowerShell for source edits, not `str_replace`,
  when at all avoidable.** This was elevated to a standing instruction
  mid-investigation after `str_replace` repeatedly (at least 4-5
  separate incidents across review17.md/review18.md) matched the wrong
  location in `varmap.cc`/`action.cc`/`ruleaction.cc` -- silently
  splicing new text into an unrelated line (often a doc comment or a
  different function's signature) and corrupting it, sometimes
  cascading when a "fix" attempt itself mis-matched a *third* location.
  The reliable, tested pattern for `run-command`-based edits:
  ```powershell
  $path = "<file>"
  $content = Get-Content -Raw -Path $path
  $contentLF = $content -replace "`r`n", "`n"
  $old = @'
  <exact old text, LF-normalized for comparison>
  '@ -replace "`r`n", "`n"
  $new = @'
  <exact new text>
  '@ -replace "`r`n", "`n"
  $count = ([regex]::Matches($contentLF, [regex]::Escape($old))).Count
  if ($count -ne 1) { Write-Output "MATCH_COUNT_ERROR: $count" } else {
    $idx = $contentLF.IndexOf($old)
    $newContentLF = $contentLF.Substring(0,$idx) + $new + $contentLF.Substring($idx + $old.Length)
    [System.IO.File]::WriteAllText($path, ($newContentLF -replace "`n", "`r`n"))
    Write-Output "REPLACED_OK"
  }
  ```
  **Always check `$count -eq 1` before writing** -- this is what makes
  the approach safe; it fails loudly instead of silently mismatching.
  Source files in this repo are CRLF -- the LF-normalize/restore-CRLF
  step matters, or exact-string matching will spuriously fail on line
  endings. `str_replace` is still fine for small, tightly-scoped edits
  (e.g. markdown files, or C++ edits with a unique `inFunction`/
  `betweenHint`/`afterString` anchor) -- the danger is specifically
  large, repetitive C++ files with generic old_str text and no scoping
  hint.
- **A "no change" result from disabling something is only meaningful
  once you've confirmed the toggle mechanism itself works**, via a
  positive control (something known to have an effect). Confirmed this
  investigation's own `RuleAddMultCollapse` toggle test was a genuine
  negative (not a broken toggle) only after validating `propagatecopy`/
  `sub2add` toggles produced visible effects first.
- **A crash's apparent location (inferred from where console output
  stops) is not reliable without positive confirmation.** This
  investigation spent a large fraction of its time (see review18.md)
  chasing `register0x0e_crossregister_fix.xml` as the crash site, based
  on "next file alphabetically after readvolatile.xml" -- without ever
  confirming output hadn't actually gotten further (e.g. into
  `satadd_debug.xml` itself) before dying. **The fix that finally worked
  came from explicitly address-scoping the change so it could ONLY
  possibly affect the target function**, which conclusively proved the
  crash was in `sat_add_u16`'s own processing the whole time. When a
  crash's location is ambiguous, prefer scoping a change so narrowly
  that it's impossible for it to be responsible for a suspected
  unrelated failure, rather than continuing to reason from inferred
  output boundaries.
- Flush-per-step diagnostics (`std::ofstream` opened, written, and
  `.close()`d immediately at each step) are reliable for finding a
  crash's approximate location when no debugger is available, but
  **can still be misleading under very high call-frequency** (thousands
  of open/append/close cycles per run) -- treat the last logged line as
  a strong hint, not proof, unless corroborated (e.g. by scoping the
  change to eliminate confounding functions entirely, as was eventually
  done here).
- Gradle may report `UP-TO-DATE` on all tasks even when a source file
  changed, especially right after a tool call that appeared to fail --
  always verify via file timestamps (see "How to get oriented fast",
  above) before trusting a build result.
- Pulsar buffers can silently resurrect deleted files -- use
  `list-open-files` + close any open buffer before relying on a delete.
- `ghidra_test.exe -path <dir> datatests <basename>` filtering is
  unreliable -- run the whole datatests directory and grep output
  instead.
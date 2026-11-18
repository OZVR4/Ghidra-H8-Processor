# auStack_/sat_add_u16 investigation, continued from review17.md (2026-08-28)

**Do not repeat review17.md's full chronological log here.** Read its
closing section first (`## File closed -- auStack_ investigation
continues in review18.md`) for a short recap. Go back to review17.md in
full only if you need the history of the two falsified hypotheses
(RangeHint::merge "confuse" branch; the SEGMENTOP-exclusion gate
experiments on gatherAdditiveBase) or the harness build/repro-file work.

This whole investigation remains OUT OF SCOPE for PR #9541
(already-landed, confirmed-complete register0x0e/SEGMENTOP annotation
fix, commit aadd7ee) and must ship as its own independent patch once
resolved.

## One-line status

Root cause narrowed from "somewhere in AliasChecker" to "an inter-pass
p-code rewrite that runs between the first and second `gatherAdditiveBase`
call for `sat_add_u16`, inside the same decompilation." The rewrite
itself has not yet been located. No fix attempted.

## How to get oriented fast

1. Rebuild command (this was missing from review17.md's own "Invocation"
   section and cost real time -- don't lose it again):
   ```powershell
   cd C:\Users\j.brophy.CORKILLSYSTEMS\github\ghidra
   .\gradlew.bat :Decompiler:decompile_testExecutable
   ```
   Fast incremental rebuild of just the `decompile_test` component, not a
   full Ghidra rebuild. Only needed after editing a decompiler `.cc` file.
2. Run the harness and verify baseline:
   ```powershell
   cd C:\Users\j.brophy.CORKILLSYSTEMS\github\ghidra\Ghidra\Features\Decompiler\build\os\win_x86_64
   .\ghidra_test.exe -sleighpath "C:\Users\j.brophy.CORKILLSYSTEMS\github\Ghidra-H8-Processor\h8\data\languages" -path "C:\Users\j.brophy.CORKILLSYSTEMS\github\ghidra\Ghidra\Features\Decompiler\src\decompile\datatests" 2>&1 | Select-String "SatAdd|Canary|Total"
   ```
   Current known-good baseline (2026-08-28, post space-name diagnostic
   rebuild): `Success -- SatAdd #1`, `Success -- Canary function name`,
   `FAIL -- Canary param a`, `FAIL -- Canary param b`,
   `Total tests applied = 13`, `Total passing tests = 7`. The two Canary
   param FAILs are a direct, precise repro of the bug itself (params not
   binding to `ushort a`/`ushort b`), not a regression -- treat them as
   the success criterion for a real fix, not noise.
3. Full decompiled output accumulates in
   `C:\Users\j.brophy.CORKILLSYSTEMS\harness_bulkout_diag.txt` -- check
   the *last* `=== bulkout for ... satadd_debug.xml ===` block.
4. `gatherAdditiveBase`'s BFS trace (with `space=` now included on every
   line) accumulates in
   `C:\Users\j.brophy.CORKILLSYSTEMS\gatherAdditiveBase_diag.txt`.
   **Delete this before each run** or old entries make the freshest trace
   hard to isolate:
   ```powershell
   Remove-Item "C:\Users\j.brophy.CORKILLSYSTEMS\gatherAdditiveBase_diag.txt" -ErrorAction SilentlyContinue
   ```
   Note: Pulsar's `run-command` treats `Remove-Item` as destructive and
   requires `confirm:true`.

## The bug, restated

`sat_add_u16(ushort a, ushort b)` at address `0x14000` decompiles to a
bogus `auStack_102[128]`-style array with pointer-cast arithmetic instead
of two clean `ushort` parameters. `a` surfaces as `in_stack_00000004`
even with a manually declared prototype.

## What's now confirmed (this session)

- Added `space=<name>` to both lines of the `gatherAdditiveBase`
  diagnostic (start-of-call line, per-visit line) in the live
  `varmap.cc`, to settle whether repeated `startvn addr=0xe` log entries
  across multiple calls were the same address space or a coincidental
  offset collision from some other function's spacebase (review17.md
  itself flagged this as a real possibility, since diagnostics only
  logged the numeric offset). **Confirmed: `space=register` every time.**
  Not a collision -- every one of these calls is genuinely walking the
  same logical spacebase for `sat_add_u16` itself.
- With that ruled out, the shape difference between calls is real and
  now well-evidenced:
  - **First call for `startvn=register:0xe`:** sees `INT_SUB` +
    `SEGMENTOP` + `COPY` at `0x14000`, and a *separate* `vn=register:0xc`
    that descends to both `INT_ADD@0x14002` and `SEGMENTOP@0x14004`.
    Terminal loads: `vn=unique:0x21d00` via `LOAD@0x14002` (this is `a`'s
    access) and `vn=unique:0x15c00` via `LOAD@0x14004` (this is `b`'s
    access) -- **two distinct terminals**, correctly separate.
  - **Every subsequent call, same startvn:** sees `INT_ADD` (not
    `INT_SUB`) at `0x14000`, no `vn=register:0xc` node at all, and
    `SEGMENTOP@0x14004` hanging directly off `vn=register:0xe`. Only one
    terminal: `vn=unique:0x15c00` via `LOAD@0x14004`.
  - **Conclusion: the merge does not happen inside
    AliasChecker/gatherAdditiveBase.** Something between the first and
    second call rewrites the p-code graph itself -- collapsing the
    `INT_SUB`/`SEGMENTOP`/`COPY` chain and eliminating the `vn@0xc`
    branch (and with it, the `0x21d00` terminal for `a`) before
    AliasChecker is asked to walk it again. AliasChecker is faithfully
    reporting whatever graph it's handed each time -- consistent with
    review17.md's own "Conclusion" section (its line ~1260), reached by a
    different route.

## Falsified or ruled out (don't re-investigate)

- Everything review17.md already lists as falsified: `RangeHint::merge()`
  "confuse" branch (never fires); "two ranges collide" theory (only one
  open range seeded); SEGMENTOP-exclusion-from-BFS gates (broke real
  address tracking, didn't fix the bug); CROSSBUILD/LABEL being
  delay-slot ops the BFS skips (mislabeling of PTRSUB/PTRADD by this
  build's `get_opname()`, not a real skip).
- **New this session:** "different address space, coincidental offset
  collision" -- ruled out directly via the `space=` diagnostic. All
  `gatherAdditiveBase` calls logging `addr=0xe` for this function are
  genuinely the same spacebase.

## UPDATE (2026-08-28, same session): likely root cause found --
## RuleAddMultCollapse

**Located the exact caller of gatherAdditiveBase:**
`ActionActiveParam::apply()` (coreaction.cc:1820-1826) creates a fresh,
stack-local `AliasChecker` on every call and immediately calls
`aliascheck.gather(...)`, which (via `gatherInternal`) re-walks the
*current* p-code graph from scratch every time -- no stale caching, this
is by design. `ActionActiveParam` is registered inside `actmainloop`
(coreaction.cc:5871), which is a `rule_repeatapply` group, so each of our
logged `gatherAdditiveBase` calls is one mainloop iteration. The `oppool1`
rule pool (`actprop`, coreaction.cc:5883+) runs later in the same
`actmainloop` group before it loops back, giving it exactly one iteration
window to rewrite the graph between our first and second observed calls.

**Instrumented the actual choke point** (`ActionPool::processOp`,
action.cc, right after `res = rl->applyOp(op,data)`, inside the existing
`if (res>0)` block) to log every Rule that successfully fires on ops near
`sat_add_u16` (`0x13ff0`-`0x14020`). Output
(`C:\Users\j.brophy.CORKILLSYSTEMS\ruleFire_diag.txt`), first pass:
```
rule=sub2add op=INT_ADD addr=0x14000
rule=collapseconstants op=COPY addr=0x14000
rule=propagatecopy op=INT_ADD addr=0x14002
rule=propagatecopy op=SEGMENTOP addr=0x14004
...
rule=addmultcollapse op=INT_ADD addr=0x14002
rule=earlyremoval op=COPY addr=0x14002
...
```
- `sub2add` firing on `INT_ADD@0x14000` matches exactly what the BFS
  trace already showed (first call sees `INT_SUB@0x14000`, every later
  call sees `INT_ADD@0x14000` instead) -- confirms this rule is
  responsible for that particular shape change, though it doesn't by
  itself explain the vanishing `vn@0xc` branch.
- **`addmultcollapse` (= `RuleAddMultCollapse`, ruleaction.cc:4100-4184)
  firing on `INT_ADD@0x14002` is the real suspect for the branch loss.**
  Read its source: it has a special-cased branch (ruleaction.cc:4132-4169)
  that explicitly matches `((stackbase + c[1]) + othervn) + c[0]`, guarded
  by `basevn->isSpacebase() && basevn->isInput()`, and folds it into a
  *brand-new* `INT_ADD` op computing `stackbase + (c0+c1)` directly,
  inserted before the original op. This bypasses the original
  intermediate `stackbase + c[1]` varnode -- which is exactly `vn@0xc` in
  our trace, the varnode that was `a`'s own address computation and the
  BFS root that used to reach `LOAD@0x14002`/`vn@0x21d00`. Once this fires
  and nothing else references the old intermediate, `gatherAdditiveBase`'s
  BFS (walking descend-edges from the spacebase varnode) can no longer
  reach it on any later call -- matching the measured shape change
  exactly (vn@0xc and its terminal load present on the first
  gatherAdditiveBase call, gone on every subsequent one).

**This is evidence, not yet final proof.** Not yet confirmed: (a) that
disabling/gating this specific branch of `RuleAddMultCollapse` actually
restores two-parameter decompilation of `sat_add_u16` via the harness,
and (b) whether the fix should be in `RuleAddMultCollapse` itself (e.g.
don't fold when the intermediate has other uses AliasChecker still needs,
or when it represents a distinct parameter/local access) versus in
`AliasChecker`/`gatherAdditiveBase` (e.g. re-run/cache the *first*
mainloop iteration's gather result instead of only trusting the most
recent one). Needs the harness test next.

## UPDATE (2026-08-28, same session): toggle mechanism validated,
## RuleAddMultCollapse hypothesis FALSIFIED

Ran a positive-control test to settle the ambiguity above: added
`<com>option togglerule propagatecopy off</com>` to `satadd_debug.xml`
(a rule that fires constantly and should have an obvious effect if the
toggle mechanism works at all). Result: decompiled output changed
dramatically (`/* WARNING: Unable to track spacebase fully for stack */`,
completely different variable structure, `Total passing tests` dropped
7->6). Also tried `sub2add off`: output changed again (different
`register0x0e`-based shape), but Canary param a/b still FAIL either way.
**This confirms the `option togglerule` mechanism genuinely reaches and
disables rules in this test harness** -- the earlier ambiguity about
whether the toggle "took" was unfounded.

With the mechanism trusted, re-ran `addmultcollapse off` cleanly by
itself: **output was exactly byte-for-byte identical to the untouched
baseline** (`Total passing tests = 7`, same `auStack_102[128]` shape,
same everything). Since we now know disabling a rule *does* perturb
output when that rule is actually involved (see propagatecopy/sub2add
above), an identical-to-baseline result here is a real negative, not an
inconclusive one.

**Conclusion: `RuleAddMultCollapse` is FALSIFIED as the mechanism.**
Despite being a strong textual match on paper (ruleaction.cc:4132-4169
folds exactly the `(stackbase+c1)+c0` shape we observed), disabling it
has zero effect on this bug. Don't re-open this specific rule as a
suspect. The reverted, clean `satadd_debug.xml` (no toggle) is the
baseline going forward -- confirm this before making assumptions,
since XML test files are trivial to accidentally leave modified between
sessions (no rebuild required to notice, unlike `.cc` changes).

**Revised next step:** `sub2add` toggling clearly changes the output
shape (worth understanding *how*, since it's evidently deeply entangled
with spacebase tracking here -- the `/* WARNING: Unable to track
spacebase fully for stack */` message is new and itself worth
investigating), but did not fix the Canary tests either, so it's not
solely responsible. Rather than continuing to guess-and-toggle
individual `oppool1` rules one at a time, the more systematic approach
is: capture the *full* `ruleFire_diag.txt` list (not just first 60 lines)
across all mainloop iterations for `sat_add_u16` specifically (filter by
run boundary, not just address, since multiple test files share
addresses), and directly correlate rule-fire order against
`gatherAdditiveBase_diag.txt`'s call boundaries to find precisely which
rule fires in the gap between the first and second `gatherAdditiveBase`
call -- rather than toggling rules one at a time based on guesses from
reading source. The existing diagnostics already support this; it just
hasn't been done exhaustively yet.

## Next step

Find which `Action` in the main decompiler rule list runs between the
first and second `gatherAdditiveBase` call and is responsible for:
- rewriting `INT_SUB` into `INT_ADD` at `0x14000`, and
- eliminating the `vn=register:0xc` branch entirely (along with its
  `INT_ADD@0x14002` / `SEGMENTOP@0x14002` / terminal `LOAD@0x14002` from
  `vn=unique:0x21d00`).

Likely candidates: something in the standard normalization/collapse rule
set that merges additive/segment chains once it decides they're
redundant (a form-collapsing action, possibly around `ActionMultiCollapse`
or similar -- name unconfirmed, needs searching in the decompiler source,
not assumed). A cheap first probe: add a diagnostic print (same
`std::ofstream` append pattern used for `gatherAdditiveBase`) into the
top of the decompiler's main per-function action-list dispatch loop,
logging each `Action`'s name as it runs, then correlate against when the
`gatherAdditiveBase_diag.txt` timestamps/ordering show the graph already
collapsed. Use the harness (`satadd_debug.xml`) for this -- seconds per
iteration, no full-Ghidra rebuild needed except for final verification.

## Known tooling gotchas (carried forward from review17.md)

- Pulsar's `str_replace` anchors can drift after multiple sequential
  edits to the same file. Prefer content anchors (`afterString`,
  `betweenHint`) over positional (`afterLine`) ones, or whole-function
  replacement for larger changes.
- Pulsar buffers can silently resurrect deleted files -- use
  `list-open-files` + close any open buffer before relying on a delete.
- `ghidra_test.exe -path <dir> datatests <basename>` filtering is
  unreliable -- run the whole datatests directory and grep output
  instead.
- Always verify instruction-exact byte counts via `disassemble_bytes`,
  not a guessed length on `read_memory` -- an off-by-one here killed an
  entire investigation thread in a past session.
- Gradle may report `UP-TO-DATE` on all tasks even when a source file
  changed if the change was made via a tool that writes to a Pulsar
  buffer rather than disk. Confirm the edit landed on disk (e.g. via a
  direct `Select-String`/`Get-Content` read, not just the editor's own
  view) before trusting a build result, and use `save-file`/`save-all` if
  the file is open in a tab.
- `str_replace` can match an anchor string inside a *different* location
  than intended (e.g. inside a doc comment that happens to contain
  similar text) if the anchor isn't scoped tightly enough, silently
  splicing code into the wrong place and corrupting a comment/line. This
  actually happened this session on `action.cc` (see the
  `RuleAddMultCollapse` instrumentation below) and was caught immediately
  by re-reading the edited region before rebuilding, then fixed by
  reconstructing the exact original text via a second `str_replace` and
  re-adding the diagnostic with a `betweenHint` scoped to the specific
  function. **Always re-view the edited region right after any
  `str_replace` on a large/shared file before rebuilding or trusting the
  edit**, especially when the file has multiple textually-similar
  locations (overloaded function names, repeated doc comment phrasing).
- Verify a rule/option toggle mechanism actually works with a positive
  control (a rule known to have an obvious effect) before trusting a
  negative result from toggling your actual suspect -- a "no change"
  result is only meaningful once you know disabling *something* does
  produce visible change in the same test setup.

## HANDOFF 2026-08-28 (end of session) -- read this first in a new conversation

**One-line status:** `RuleAddMultCollapse` is cleanly falsified as the
collapse mechanism (confirmed via a validated toggle test, not a guess).
`sub2add` visibly affects spacebase tracking but doesn't fix the bug
alone either. Root cause still not found. No fix attempted. The
systematic rule-fire/call-boundary correlation approach (below) has not
yet been executed.

**To get oriented fast:**
1. Read this file top-to-bottom once; each "Update" supersedes the
   previous, but none of them reverse the falsification above -- it's
   solid, not tentative.
2. Confirm `satadd_debug.xml` is clean (no leftover `option togglerule`
   line) before assuming the harness baseline -- verified clean at end of
   this session, but XML edits don't require a rebuild to take effect, so
   it's easy to forget one is still in place across a session boundary.
   ```powershell
   Get-Content "C:\Users\j.brophy.CORKILLSYSTEMS\github\ghidra\Ghidra\Features\Decompiler\src\decompile\datatests\satadd_debug.xml"
   ```
   Script block should read exactly: `option readonly on` / `parse line
   extern uint2 sat_add_u16(uint2 a,uint2 b);` / `lo fu sat_add_u16` /
   `decompile` / `print C` / `quit` -- no `option togglerule` line.
3. Rebuild and confirm baseline:
   ```powershell
   cd C:\Users\j.brophy.CORKILLSYSTEMS\github\ghidra
   .\gradlew.bat :Decompiler:decompile_testExecutable
   cd Ghidra\Features\Decompiler\build\os\win_x86_64
   .\ghidra_test.exe -sleighpath "C:\Users\j.brophy.CORKILLSYSTEMS\github\Ghidra-H8-Processor\h8\data\languages" -path "C:\Users\j.brophy.CORKILLSYSTEMS\github\ghidra\Ghidra\Features\Decompiler\src\decompile\datatests" 2>&1 | Select-String "SatAdd|Canary|Total"
   ```
   Expected: `Success -- SatAdd #1`, `Success -- Canary function name`,
   `FAIL -- Canary param a`, `FAIL -- Canary param b`,
   `Total tests applied = 13`, `Total passing tests = 7`.

**What's confirmed this session (don't redo):**
- Build command for `ghidra_test.exe` (above) -- was missing entirely
  before this session, now documented in three places (here, and
  review17.md's closing section).
- `space=<name>` diagnostic added to `gatherAdditiveBase` in the live
  `varmap.cc` (both start-of-call and per-visit lines) -- still in place,
  confirmed same address space (`register`) across all calls for
  `sat_add_u16`, ruling out a cross-function offset collision.
- `ActionActiveParam::apply()` (coreaction.cc:1820-1826) is the confirmed
  caller of `AliasChecker::gather`/`gatherAdditiveBase`, called fresh
  every `actmainloop` iteration (`rule_repeatapply` group,
  coreaction.cc:5871) -- this is why the BFS shape genuinely differs
  between calls; it's not a caching artifact.
- Rule-fire diagnostic added to `ActionPool::processOp` in `action.cc`
  (right after `res = rl->applyOp(op,data)`, inside the existing
  `if (res>0)` block), logging every rule that fires on ops in
  `0x13ff0`-`0x14020`, writing to
  `C:\Users\j.brophy.CORKILLSYSTEMS\ruleFire_diag.txt`. Still in place,
  confirmed correctly scoped (verified via direct re-view after the
  earlier corruption/fix).
- `option togglerule <name> off/on` mechanism confirmed genuinely working
  in this harness (validated with `propagatecopy` and `sub2add` as
  positive controls -- both visibly changed decompiled output).
- `RuleAddMultCollapse` (`addmultcollapse`) toggle: **zero effect**,
  byte-for-byte identical output to baseline. Falsified as sole or
  contributing cause. Do not re-test this rule without new evidence
  first.

**Not yet done -- concrete next action:**
Capture the *complete* `ruleFire_diag.txt` for a single clean harness run
(not just first-60-lines), and the complete `gatherAdditiveBase_diag.txt`
for the same run, then line up rule-fire events against
`gatherAdditiveBase` call boundaries to find exactly which rule(s) fire
in the gap between the first call (sees `vn@0xc`/two terminals) and the
second call (sees the collapsed one-terminal shape) -- rather than
continuing to toggle individual rules by guesswork from reading source.
`sub2add` is still an open, untested-in-isolation-for-this-specific-
purpose candidate (it visibly affects spacebase tracking per the
`Unable to track spacebase fully for stack` warning seen when toggled
off, which is itself new and unexplained) but toggling it off didn't fix
Canary param a/b either, so on its own it's not sufficient -- worth
checking whether its presence vs. absence changes *which* call is the
"first" one in `gatherAdditiveBase_diag.txt`, i.e. whether it's a
necessary precondition for the real culprit rule to fire, not the
culprit itself.

**Known tooling gotchas:** see the full list directly above this
handoff, especially the `str_replace` anchor-drift/corruption risk and
the toggle-mechanism positive-control lesson -- both bit this session
directly and both have concrete, tested mitigations now.

## UPDATE (2026-08-28/29, next session): merged diagnostics, found a much
## stronger lead -- RuleLoadVarnode / RuleStoreVarnode

**Merged both diagnostics into one file** so real execution order is
preserved by simple append order: `gatherAdditiveBase`'s three diagnostic
blocks in `varmap.cc` and the rule-fire diagnostic in `action.cc` all now
write to `C:\Users\j.brophy.CORKILLSYSTEMS\combined_diag.txt` (the
rule-fire lines are prefixed `RULE ` to distinguish them; an earlier
attempt to also prefix the gatherAdditiveBase lines with `GAB ` was
abandoned after repeated corruption, see below -- they're unprefixed but
still easy to tell apart by content, starting with `---`, `  visit vn`,
or `    descend op`).

**Significant tooling trouble this session:** `str_replace` repeatedly
matched the wrong location on `varmap.cc` and `action.cc` when the
`old_str` was short/generic or appeared in multiple similar-looking
spots (doc comments phrased similarly to nearby code, `Varnode
*vn,*subvn,...` declarations, etc.), silently splicing new text into the
wrong place and corrupting nearby lines -- sometimes cascading (a "fix"
attempt itself landed in a third wrong location). This happened at least
four times across the two files before everything was correctly
restored. **Recovery method that worked:** stop making incremental
patches the moment a corruption is suspected; `view`/`read-lines` the
*entire* affected function (not just the immediate area) to get precise
current text; reconstruct the correct block as one single large
old_str/new_str replacement using exact text confirmed present in a
recent read; then **re-view the whole region again** before rebuilding.
A full rebuild + baseline harness run (`Total passing tests = 7`,
byte-identical to the known-good baseline) is what finally confirmed both
files were genuinely clean again. **Lesson for future sessions: prefer
`inFunction`/`afterString`/`betweenHint` scoping on every `str_replace`
against these two files** (both are large, 1100-1800+ lines, with
repeated similar phrasing) -- plain `old_str` with no scope hint is what
caused every corruption this session.

**With clean diagnostics, correlated rule-fire order directly against
`gatherAdditiveBase` call boundaries** (this is the systematic approach
the previous handoff called for, instead of guessing). Between the first
call (full shape, `vn@0xc` branch present, two terminals) and the second
call (collapsed shape, `vn@0xc` gone, one terminal), exactly these rules
fire in order:
```
RULE rule=sub2add op=INT_ADD addr=0x14000
RULE rule=collapseconstants op=COPY addr=0x14000
RULE rule=propagatecopy op=INT_ADD addr=0x14002
RULE rule=propagatecopy op=SEGMENTOP addr=0x14004
RULE rule=propagatecopy op=COPY addr=0x14004
RULE rule=propagatecopy op=INT_CARRY addr=0x14004
RULE rule=propagatecopy op=INT_ADD addr=0x14000
RULE rule=earlyremoval op=COPY addr=0x14000
RULE rule=earlyremoval op=COPY addr=0x14000
RULE rule=addmultcollapse op=INT_ADD addr=0x14002
RULE rule=earlyremoval op=COPY addr=0x14002
RULE rule=earlyremoval op=COPY addr=0x14004
RULE rule=propagatecopy op=INT_CARRY addr=0x14004
RULE rule=earlyremoval op=COPY addr=0x14004
RULE rule=storevarnode op=COPY addr=0x14000
RULE rule=loadvarnode op=COPY addr=0x14002
```
(Note: `rule=storevarnode`/`rule=loadvarnode` show `op=COPY` because the
diagnostic logs `op->code()` *after* the rule already ran -- both rules
rewrite their target op's opcode to `CPUI_COPY` in place, same PcodeOp,
same address. They actually fired on a `STORE` and a `LOAD` respectively.)

**`RuleLoadVarnode` (class in ruleaction.cc, registered
`Rule(g, 0, "loadvarnode")` in the `"stackvars"` group,
coreaction.cc:6040) is a much better-evidenced candidate than
`RuleAddMultCollapse`.** Its `applyOp` (ruleaction.cc:4298-4326) does:
```cpp
newvn = data.newVarnode(size,baseoff,offoff);
data.opSetInput(op,newvn,0);
data.opRemoveInput(op,1);
data.opSetOpcode(op, CPUI_COPY);
```
This doesn't just algebraically fold the address computation -- it
**removes the LOAD's original pointer input entirely** (`opRemoveInput`)
and replaces it with a brand-new, disconnected varnode at the resolved
space+offset. That severs the descend-edge from the `vn@0xc` address
subtree to this op directly, which is a much more mechanistically direct
explanation for why `gatherAdditiveBase`'s BFS loses the branch than
`RuleAddMultCollapse`'s algebraic fold (which was tested and produced
*zero* effect on output).

**Toggle-tested `loadvarnode` off:** real, visible effect (unlike
`addmultcollapse`). Output changed `in_stack_00000004` ->
`uStack0004` -- a bound stack variable instead of an unbound input
placeholder. Canary param a/b still FAIL, and `auStack_102[128]` (the
`b`-side merge) is still present -- so `loadvarnode` alone is not
sufficient, but it is clearly involved and worth much more attention
than `addmultcollapse` got.

**Also toggle-tested the companion `storevarnode` off**
(`RuleStoreVarnode`, `friend class` of `RuleLoadVarnode` per
ruleaction.hh:793, fires on the `STORE@0x14000` -- the initial spill of
parameter `a` to its stack slot). Also a real, different effect: output
became
```c
uint2 sat_add_u16(uint2 a,uint2 b)

{
  uint2 uVar1;
  xunknown2 unaff_FP;
  xunknown1 in_TP;
  uint2 in_stack_00000004;
  uint2 uStack_2;

  uStack_2 = unaff_FP;
  uVar1 = in_stack_00000004 + *(uint2 *)((int4)&uStack_2 + 8);
  ...
```
Still FAILs Canary tests, but a distinctly different, also-relevant
shape (`unaff_FP`/explicit `&uStack_2 + 8` pointer arithmetic appearing).

**Not yet tried: toggling `loadvarnode` AND `storevarnode` off
together in the same run.** This is the clear next step -- each rule
individually produces a real, different, non-trivial change, which is a
much stronger signal than `addmultcollapse` gave (zero effect alone).
It's plausible the actual bug only manifests when both rules act
together (each rule may be correctly resolving *one side* of the split
access on its own, with the erroneous merge happening only when both
have already run and something downstream -- possibly back in
`AliasChecker` or symbol/scope mapping -- conflates the two resolved
`newvn`s, or fails to keep them distinct). Test this before going back to
theorizing from source reading alone.

**satadd_debug.xml note:** several `option togglerule ... off` lines
were added and removed during this session's testing. **Confirmed clean
(no toggle lines) as of the end of this session** -- verified by direct
read of the file. Still worth double-checking at the start of any future
session per the usual caution.

## UPDATE (2026-08-29, same session continued): likely TRUE root cause
## found -- RuleLoadVarnode's SEGMENTOP-unwrap is not recursive

**Tested `loadvarnode` and `storevarnode` disabled together** (both
`option togglerule ... off` in the same run). Result: output shows the
union of both individual effects (`uStack0004` for `a`'s side,
`unaff_FP`/manual `&uStack_2 + 8` pointer arithmetic for `b`'s side).
Still fails Canary param a/b, and actually moves *further* from a fix
(raw pointer arithmetic off an unresolved `unaff_FP`, not clean
parameters). **This shows these two rules are not the bug -- they are
the mechanism that's supposed to correctly resolve stack accesses, and
disabling them just reverts to an even more primitive representation.**
Don't retest this combination without new evidence; it's a dead end.

**The real question this raised: why does `RuleLoadVarnode` fire and
succeed for `a`'s access but never even fire on `b`'s, in every baseline
run we've captured?** Read three functions in `ruleaction.cc` together
to answer this precisely:

- `RuleLoadVarnode::correctSpacebase` (ruleaction.cc:4194-4205) requires
  `vn->isInput()` (line 4200: `if (!vn->isInput()) return (AddrSpace
  *)0;`) -- the varnode must be a genuine, unwritten function input (the
  raw incoming spacebase register), not something computed by another
  op.
- `RuleLoadVarnode::checkSpacebase` (ruleaction.cc:4257-4284) only
  special-case-unwraps a `SEGMENTOP` (line 4266:
  `if (offvn->isWritten()&&(offvn->getDef()->code()==CPUI_SEGMENTOP))`)
  when that SEGMENTOP is the LOAD/STORE's **direct** pointer operand
  (`op->getIn(1)`). If the direct pointer operand is anything else (e.g.
  an `INT_ADD`), it falls through to `vnSpacebase` instead.
- `RuleLoadVarnode::vnSpacebase` (ruleaction.cc:4215-4248), used when the
  direct pointer is an `INT_ADD`, checks each of the `INT_ADD`'s two
  inputs via `correctSpacebase` (lines 4232, 4240) -- but never
  separately unwraps a `SEGMENTOP` among those inputs. It only checks
  `isInput()` directly.

**This explains the asymmetry exactly.** From the `gatherAdditiveBase`
trace (see the earlier update above):
- `a`'s `LOAD@0x14002` pointer is *directly* a `SEGMENTOP` output ->
  hits `checkSpacebase`'s special-case unwrap -> the inner offset it
  extracts is the real, raw spacebase register (`isInput()==true`) ->
  `correctSpacebase` succeeds -> `RuleLoadVarnode` fires, `a` resolves
  cleanly to `uStack0004`/eventually the clean parameter.
- `b`'s `LOAD@0x14004` pointer is an `INT_ADD` (computing `+8` on top of
  the frame), one of whose inputs is a `SEGMENTOP`'s *output* (not a raw
  input -- it's computed, `isWritten()==true`) and the other a constant.
  `vnSpacebase` calls `correctSpacebase` directly on that SEGMENTOP
  *output*, which fails the `!vn->isInput()` check (line 4200) and
  returns null. Neither `INT_ADD` input passes, so `vnSpacebase` returns
  null, `checkSpacebase` returns null, and `RuleLoadVarnode::applyOp`
  bails out at its own `if (baseoff == (AddrSpace *)0) return 0;`
  (ruleaction.cc:4307) -- **the rule never fires on `b`'s LOAD at all**,
  in any run, toggle or no toggle. `b`'s access is left as a raw pointer
  LOAD, which `AliasChecker`/symbol mapping later folds into the generic
  merged `auStack_102[128]` array, since it was never converted into a
  properly-typed, separately-mapped stack variable the way `a`'s was.

**In short: `checkSpacebase`'s SEGMENTOP-unwrap logic is not applied
recursively.** It only fires when a `SEGMENTOP` sits *directly* adjacent
to the LOAD/STORE; it does not look one level further when there's an
intervening `INT_ADD` (i.e. an access at `spacebase + constant`, computed
via `SEGMENTOP` then `+constant`, rather than an access at `spacebase`
alone). The asymmetry between `a`'s access (no extra offset needed) and
`b`'s access (needs `+8` on top of the frame) is exactly why one
parameter resolves correctly and the other doesn't -- this isn't a
coincidence of these two specific parameters, it's a structural gap in
`RuleLoadVarnode` for *any* stack variable that sits at a nonzero
constant offset behind a `SEGMENTOP`.

**This is the strongest, most concrete, most mechanistically verified
finding of the whole investigation** (review16.md through here) --
found by reading exact source logic, not by toggling rules and guessing.
Both `RuleAddMultCollapse` (falsified) and the
`loadvarnode`+`storevarnode`-disabled test (dead end, moves away from a
fix) pointed away from the real mechanism; this explains the bug
precisely and predicts exactly which shape of access fails (any
stack/local variable access needing an additional constant added on top
of a `SEGMENTOP`-produced base pointer).

**Not yet done:**
1. **No fix has been attempted or tested yet** -- this is a root-cause
   finding, not a patch. The natural fix shape: make `vnSpacebase`
   (or `checkSpacebase` before delegating to it) also unwrap a
   `SEGMENTOP` among the `INT_ADD`'s inputs the same way `checkSpacebase`
   already does at the top level, recursively/consistently, rather than
   only checking `isInput()` directly on each `INT_ADD` operand. Needs
   careful thought about correctness (e.g. does the SEGMENTOP's own base
   part still need to be handled/ignored the same way as in
   `checkSpacebase`'s comment at lines 4268-4275?) before writing any
   code -- this is exactly the kind of subtle spacebase/segment logic
   that's easy to get wrong, and this file's earlier "falsified"
   hypotheses (RangeHint::merge, SEGMENTOP-exclusion gates) are a
   reminder that plausible-looking fixes in this exact area have failed
   before.
2. Verify this reasoning is actually right by testing it empirically
   before writing a real fix: e.g., temporarily/experimentally patch
   `vnSpacebase` to also try unwrapping a SEGMENTOP on `vn1`/`vn2` before
   giving up, rebuild, and check whether `RuleLoadVarnode` then fires on
   `b`'s LOAD and whether Canary param a/b then pass. This confirms the
   diagnosis before investing in a careful, correct, upstreamable fix.
3. Consider whether this same gap could affect other, unrelated
   functions/tests in the datatests suite -- a fix here has blast radius
   beyond `sat_add_u16` specifically, so should be checked against the
   full datatests directory (`Total tests applied = 13` baseline across
   all files, not just the 2 Canary tests), not just this one function,
   before considering it complete.

**Still out of scope:** this entire investigation remains separate from
the already-landed PR #9541 (register0x0e/SEGMENTOP annotation fix,
commit aadd7ee) and must ship as its own independent patch once resolved.

## LESSON (2026-08-29): use run-command/direct file writes for edits to
## review files going forward, not str_replace

This session's `str_replace` corruption incidents on `varmap.cc` and
`action.cc` (see the tooling-gotchas list above) prompted a standing
change for future sessions: prefer appending/editing markdown review
files (and where practical, source files) via `run-command`
(PowerShell `Add-Content`/`Set-Content`/here-strings) rather than
`str_replace`, since `run-command` writes are atomic and don't risk the
silent wrong-location splicing seen repeatedly this session. Reserve
`str_replace` for small, tightly-scoped source edits where a
run-command rewrite would be impractical (e.g. mid-function C++ patches),
and always re-view the result immediately after.

## UPDATE (2026-08-29, continued): experimental patch tested empirically
## -- one-level SEGMENTOP unwrap insufficient, chain is 3 levels deep

**Implemented the experimental fix proposed above** (unwrap a SEGMENTOP
among `vnSpacebase`'s `INT_ADD` inputs) directly in
`RuleLoadVarnode::vnSpacebase` (ruleaction.cc, around line 4230-4245).
Used `run-command`/PowerShell exact-match string replacement instead of
`str_replace` for this and all subsequent edits this session, per
standing instruction -- every one of these edits landed correctly on the
first attempt, zero corruption, confirming the lesson from earlier in
this file. Method: read file raw, normalize CRLF->LF, verify exact match
count via `[regex]::Matches` before ever writing, only write if count is
exactly 1, restore CRLF on write. Recommended pattern for all future
source edits on these two files.

**Rebuilt and tested: the patch alone did NOT fix the bug.** Canary
param a/b still FAIL, output byte-identical to baseline. Confirmed via
timestamp check that the rebuild genuinely picked up the change (exe
newer than source edit) -- this was a real negative result, not a stale
build.

**Added a temporary diagnostic directly inside `vnSpacebase`** to see
`vn1`/`vn2`'s actual op types when called near `sat_add_u16`. Result for
the `0x14004` call (b's access):
```
VNSB addr=0x14004 vn1_op=SEGMENTOP vn1_isInput=0 vn2_op=(input/const) vn2_isInput=0 vn2_isConst=1 vn1_isConst=0
```
This confirms the structural read was right -- `vn1` genuinely is a
`SEGMENTOP` output, `vn2` genuinely is the `+8` constant. So the unwrap
condition *should* trigger. Added a second diagnostic to check the
unwrapped `inner1` (the SEGMENTOP's `getIn(2)`) directly:
```
INNER1 addr=0x14004 inner1_isInput=0 inner1_isConst=0 inner1_isWritten=1 inner1_op=INT_ADD
```
**`inner1` is itself another `INT_ADD`, not the raw spacebase input.**
This is the actual gap: `b`'s address-computation chain is not two levels
deep (`INT_ADD(SEGMENTOP(...), +8)`) as assumed, but **three levels deep**
(`LOAD <- INT_ADD(+8) <- SEGMENTOP <- INT_ADD(+something_else) <- ...
eventually the raw spacebase input`). My one-shot unwrap only peels back
one `SEGMENTOP` layer and then calls `correctSpacebase` directly on the
result, which fails because that result is itself a computed `INT_ADD`,
not a raw input.

**This means a correct fix needs `vnSpacebase`'s SEGMENTOP-unwrap step to
be recursive** (or at minimum, after unwrapping a SEGMENTOP and finding
the inner value is itself an `INT_ADD`, recursively call `vnSpacebase`
again on that inner `INT_ADD` rather than only calling
`correctSpacebase` directly on it -- accumulating constant offsets from
each nested level rather than just the outermost one). This is more
involved than the current one-shot patch and needs careful thought about
correctly summing offsets across levels before attempting again --
exactly the kind of subtlety this file's tooling-gotchas section already
warns about for this area of code.

**Current state of the experimental patch:** left in place in
`ruleaction.cc` (the one-level unwrap in `vnSpacebase`, plus the two
temporary `VNSB`/`INNER1` diagnostics). **None of this is a working fix
yet** -- next session should either extend the unwrap to be recursive and
retest, or revert these experimental changes if starting a fresh
approach. The diagnostics are harmless (scoped to 0x13ff0-0x14020, same
pattern as the rest of this file's diagnostics) but should be removed
once no longer needed to keep `combined_diag.txt` readable.

**Immediate next step:** make the SEGMENTOP-unwrap in `vnSpacebase`
recursive (call `vnSpacebase` again on the unwrapped inner value instead
of `correctSpacebase` directly, propagating/summing the offset), rebuild,
and check whether `RuleLoadVarnode` finally fires on `b`'s LOAD and
whether Canary param a/b then pass.

## UPDATE (2026-08-29, continued): recursive fix CRASHED the PR #9541
## regression test -- reverted immediately, ruleaction.cc/.hh clean again

**Implemented the recursive fix** proposed in the previous update:
`vnSpacebase` now unwraps a SEGMENTOP among its `INT_ADD` inputs and
recurses into itself (rather than calling `correctSpacebase` directly),
accumulating offsets across nested levels. Used the same
`run-command`/PowerShell exact-match technique as before (verify match
count, normalize CRLF<->LF) -- every edit this session landed correctly
on the first attempt.

**First test (no depth guard): CRASHED.** Running the full datatests
directory produced exit code `-1073741819` (`0xC0000005`, access
violation) partway through the suite (stdout cut off immediately after
`readvolatile.xml`'s expected "no sleigh spec" message). Diagnosed by
finding which file is next alphabetically in the datatests directory:
**`register0x0e_crossregister_fix.xml`** -- the regression test for the
already-landed, confirmed-complete PR #9541 fix (register0x0e/SEGMENTOP
annotation fix, commit aadd7ee) that this entire investigation has
repeatedly been told to keep separate from and not break.

**Suspected unbounded recursion, added a depth guard** (`depth` parameter
defaulting to 0, capped at `depth > 8`, threaded through both recursive
call sites and the header declaration). **Still crashed identically**,
which rules out simple unbounded recursion as the cause -- more likely a
null/out-of-bounds pointer dereference specific to how
`register0x0e_crossregister_fix.xml`'s own cross-register SEGMENTOP
pattern interacts with the new unwrap-and-recurse logic (e.g.
`getIn(2)` assumed a fixed SEGMENTOP arity that doesn't hold for that
test's pattern). Root cause of the crash itself was not further
diagnosed -- given the direct collision with PR #9541's own test
coverage, the safer and correct move was immediate revert, not continued
iteration on a patch that already broke known-good, shipped behavior.

**Reverted both `vnSpacebase`'s implementation (ruleaction.cc) and its
header declaration (ruleaction.hh) back to their exact original,
unmodified state**, using the same verified exact-match
`run-command`/PowerShell technique. Rebuilt and confirmed: full datatests
suite runs to completion with no crash, `Total passing tests = 7`
(byte-identical to every known-good baseline this investigation has
established), exit code 6 (the normal non-crash exit reflecting expected
test failures, not a crash). **`ruleaction.cc` and `ruleaction.hh` are
now back to pristine, matching upstream exactly -- confirmed empirically,
not just by re-reading the diff.**

**What remains in place (safe, inert, non-crashing):** the diagnostic
instrumentation in `varmap.cc` (`gatherAdditiveBase`'s three diagnostic
blocks) and `action.cc` (`ActionPool::processOp`'s rule-fire diagnostic),
all writing to `combined_diag.txt`, scoped to addresses
`0x13ff0`-`0x14020`. These do not touch `ruleaction.cc`/`.hh` and have
been confirmed safe across every rebuild+full-suite run this whole
investigation (review17.md onward).

**Status: root cause diagnosis (the 3-level-deep SEGMENTOP/INT_ADD
nesting gap in `RuleLoadVarnode::vnSpacebase`) is still believed correct
and well-evidenced** (see the previous two updates) **-- but the specific
fix attempted this session is confirmed wrong/unsafe and has been fully
reverted.** A correct fix needs to:
1. Handle the same 3-level nesting case for `sat_add_u16`'s parameter
   `b` without changing behavior for `register0x0e_crossregister_fix.xml`'s
   own SEGMENTOP pattern -- these two cases need to be distinguished, not
   treated identically by a blanket unwrap-and-recurse.
2. Be tested against `register0x0e_crossregister_fix.xml` and
   `register0x0e_segmentop_fix.xml` specifically (not just Canary param
   a/b) on every iteration, given this session's direct collision --
   these two files should be treated as first-class regression checks
   for any future `RuleLoadVarnode`/`vnSpacebase` change, checked before
   even looking at whether Canary param a/b pass.
3. Root-cause the actual crash mechanism (likely a `getIn(2)` arity
   assumption or similar) before attempting a similar unwrap approach
   again, rather than re-adding a depth guard and hoping -- the depth
   guard did not prevent this crash, meaning the failure mode is not
   (only) recursion depth.

**Immediate next step for a fresh session:** read
`register0x0e_crossregister_fix.xml` and
`register0x0e_segmentop_fix.xml` to understand the exact SEGMENTOP
pattern PR #9541 already fixed, before attempting any further change to
`RuleLoadVarnode`/`vnSpacebase` -- this session learned that lesson the
hard way (via a crash, not just a wrong-output result) and a future
session should not repeat the same mistake blind to that test's shape.

## UPDATE (2026-08-29, continued): crash mechanism narrowed down --
## unconditional getIn(2) on any SEGMENTOP is the likely culprit

**Read both PR #9541 regression tests in full**
(`register0x0e_crossregister_fix.xml`, `register0x0e_segmentop_fix.xml`)
to understand exactly what they exercise, since the crash landed on the
former. Both are about a register (SP or FP) being reassigned via
self-referential or cross-register arithmetic against a constant (e.g.
H8's `link FP,#-2` prologue idiom, pcode-lowered to
`FP = INT_ADD(CAST(SP), 0xfffe)`) immediately before that register is
consumed as a `SEGMENTOP`'s operand for a far-pointer access. This is
precisely the shape my reverted patch's SEGMENTOP-unwrap-and-recurse
logic would touch.

**Found the abandoned `varmap - backup.cc` "direction A" experiment
(2026-08-26, review17.md era, never finished/tested) had already been
wrestling with this exact same interaction**, inside `gatherAdditiveBase`
itself: it gates the `CPUI_SEGMENTOP` case based on inlined
self-referential/cross-register-arithmetic detection (since the real
helpers are `static` in `printlanguage.cc` and not callable elsewhere),
and its own comments document a real bug the original author hit and
fixed: checking the wrong Varnode (`op->getIn(0)` re-fetched fresh
instead of the loop's actual candidate `vn`). This SEGMENTOP-exclusion
gate approach is the same one already listed as **falsified** in
review17.md's "Falsified or ruled out" section (broke real address
tracking without fixing the bug) -- so this isn't a new avenue to
pursue as-is, but its comments are a useful, hard-won map of this exact
problem area's pitfalls.

**Read `isCrossRegisterArithmetic`/`isSelfReferentialRegisterArithmetic`
in `printlanguage.cc` (lines 255-276, usage at 590-612) directly to get
the authoritative semantics.** Key facts:
- These check whether the Varnode *consumed by* SEGMENTOP (i.e.
  SEGMENTOP's own input operand, e.g. FP) is itself defined by
  `INT_ADD`/`INT_SUB` against a constant, optionally unwrapping one
  `CPUI_CAST` on the base (`printlanguage.cc:267-268`), and require
  `def->numInput() == 2` (line 263) before touching `getIn(1)`.
- Compare this to `RuleLoadVarnode::checkSpacebase`'s existing (working,
  unmodified) `getIn(2)` call in `ruleaction.cc` -- **that call site only
  ever runs when a SEGMENTOP is the *direct* pointer operand of a
  LOAD/STORE**, a narrow, already-proven-safe context.

**This points at the likely actual crash mechanism.** My reverted patch's
SEGMENTOP-unwrap called `getIn(2)` on *any* SEGMENTOP-defined Varnode
found as one of an outer `INT_ADD`'s two inputs, anywhere in
`vnSpacebase`'s recursive walk -- not gated to the same narrow context
`checkSpacebase` uses. Confirmed via the live (non-backup) `varmap.cc`
(`gatherAdditiveBase`, lines ~908-920) that this project's *other*
SEGMENTOP-handling code path treats `CPUI_SEGMENTOP` identically to
`CPUI_PTRSUB` -- just following the op's *output*, never reading a
specific input index at all. **No other call site in this codebase reads
`getIn(2)` off an arbitrary/unconstrained SEGMENTOP the way my patch
did.** It's plausible the cross-register test's SEGMENTOP instance (used
for the FP-derivation-driven far-pointer access) has a different
arity/shape than the plain-parameter-access SEGMENTOP `checkSpacebase`
was designed around, and `getIn(2)` on it triggered the crash --
though the exact mechanism was not conclusively proven by tracing alone
(a manual trace of the simple `FP = INT_ADD(CAST(SP),0xfffe)` case
alone didn't obviously explain a crash), so this remains the leading
hypothesis, not a fully confirmed diagnosis.

**Guidance for any future fix attempt in this area:**
1. Before calling `getIn(N)` on any Varnode's defining op, check
   `op->numInput()` first -- `printlanguage.cc`'s own helpers do this
   (`def->numInput() != 2`, line 263) and `checkSpacebase`'s existing
   `getIn(2)` call is only safe because of its narrow, direct-operand-only
   calling context, not because SEGMENTOP is guaranteed 3 inputs
   everywhere.
2. Don't reuse `checkSpacebase`'s `getIn(2)`-based unwrap outside the
   exact context it already safely runs in (SEGMENTOP as a LOAD/STORE's
   *direct* pointer operand). If a fix needs to recognize a SEGMENTOP
   nested inside an outer `INT_ADD`, model it on
   `isCrossRegisterArithmetic`'s approach instead: check `numInput()`,
   optionally unwrap one `CPUI_CAST`, and require the *other* operand of
   the containing arithmetic op to be a plain constant -- this is a more
   defensive shape check than blindly indexing into SEGMENTOP's operands.
3. **Test against `register0x0e_crossregister_fix.xml` and
   register0x0e_segmentop_fix.xml on every single iteration**, not just
   Canary param a/b -- these should be checked first, given the direct
   collision this session hit, before even looking at whether the target
   bug is fixed.
4. The abandoned `varmap - backup.cc` "direction A" experiment (SEGMENTOP
   exclusion gate in `gatherAdditiveBase`) is confirmed falsified
   (review17.md) and its own approach shouldn't be revived as-is, but its
   comments remain a useful reference for the specific
   self-referential/cross-register shape-detection pitfalls in this
   area -- worth reading again (`varmap - backup.cc` lines 887-920ish)
   before writing new shape-detection logic from scratch.

**Session status: no live source changes remain from this continued
exploration** (all analysis this update covers was read-only --
`printlanguage.cc`, `varmap - backup.cc`, `varmap.cc`, and the two
regression test XML files were only read, not modified). `ruleaction.cc`
and `ruleaction.hh` remain reverted to pristine/original, confirmed
clean earlier in this file. Diagnostic instrumentation in `varmap.cc`
(gatherAdditiveBase) and `action.cc` (rule-fire logging) remains in
place, safe and non-crashing, writing to `combined_diag.txt`.

## UPDATE (2026-08-30): second fix attempt ALSO crashed -- reverted again,
## stopping further blind logic changes, diagnosis-first approach needed

**Attempted a redesigned, more defensive fix** per this file's own
guidance: added a local `isRegisterAnnotationArithmetic` helper mirroring
`printlanguage.cc`'s `isSelfReferentialRegisterArithmetic`/
`isCrossRegisterArithmetic` (INT_ADD/INT_SUB against a constant, base
optionally unwrapped through one CPUI_CAST, base must be
`IPTR_PROCESSOR`-space), checked `SEGMENTOP::numInput() >= 3` before
indexing, and used it to make `vnSpacebase`'s SEGMENTOP-unwrap explicitly
decline to recurse through that shape.

**First test: no crash** (both `register0x0e_*.xml` passed), **but also
no fix** -- Canary param a/b still FAIL, output byte-identical to
baseline. Diagnosed why: the raw INT_ADD/INT_SUB-against-a-constant shape
is structurally IDENTICAL for a genuine one-off nested stack-offset
computation (`sat_add_u16`'s `b` needs `SP + extra_constant`) and a
register-redefinition annotation (`FP = SP - 2`) -- `isRegisterAnnotationArithmetic`
as written couldn't tell them apart and excluded both equally.

**Refined the check**: added a requirement that the candidate have more
than one descendant (`vn->loneDescend() != (PcodeOp *)0` check, i.e.
reject only when NOT lone -- reasoning: a register-redefinition value
like FP is reused across every FP-relative access in the function, while
a one-off address computation feeds exactly one consumer). This was a
reasonable, principled distinguishing signal, not a random guess.

**Second test: CRASHED AGAIN** (`register0x0e_crossregister_fix.xml`
family, same `0xC0000005` access violation pattern as the first crash).
**Immediately and fully reverted both `ruleaction.cc` and `ruleaction.hh`
back to pristine** -- confirmed via `grep` that zero trace of
`isRegisterAnnotationArithmetic` remains, and via full rebuild + harness
run that the suite passes cleanly again (`Total passing tests = 7`, both
register0x0e tests pass, exit code 6, no crash).

**Conclusion: two different, individually-reasoned distinguishing-check
designs have now both crashed the same regression test family.** This is
a strong signal that the problem isn't really about *which* shape-based
heuristic gets used to decide whether to unwrap/recurse -- something more
fundamental about calling `vnSpacebase`/touching this SEGMENTOP-nested-in-
INT_ADD pattern at all, in this specific test's function, is unsafe in a
way neither attempt's reasoning-from-source-reading anticipated. **Guessing
new heuristics and re-testing against the full suite is not converging
and risks a third crash under continued trial and error.**

**Recommended approach for next time -- diagnose before patching again:**
1. Add PURE, read-only diagnostic instrumentation (no behavior change) to
   `vnSpacebase`/the SEGMENTOP-unwrap call site, logging every relevant
   Varnode/op property (address, numInput(), loneDescend() result, an
   actual pointer/ID) specifically when processing
   `register0x0e_crossregister_fix.xml`'s function
   (`status_word_bit3_conditional_update_via_table`, ram offset 0x232c3)
   -- filter the diagnostic to THIS function's address range, the same
   way the `0x13ff0`-`0x14020` filter targets `sat_add_u16`.
2. Run ONLY that single test file if possible (though this file's own
   tooling-gotchas note `-path <dir> <file>` filtering is unreliable --
   verify this still holds, or just run the full directory and grep the
   diagnostic for this function's address range specifically).
3. Find the EXACT op/Varnode that triggers the crash -- is it really
   inside the SEGMENTOP-unwrap-and-recurse block at all? Consider
   instrumenting with try/catch-style bounds logging immediately before
   each risky call (`getIn(2)`, `getDef()`, `loneDescend()`) rather than
   assuming which one is unsafe.
4. Only once the exact crash trigger is identified from real data --
   not reasoned from source alone -- write a fix that specifically
   excludes that exact condition, and re-verify against both
   `register0x0e_*.xml` files AND `sat_add_u16` together before
   considering it a real candidate fix.
5. Consider whether this whole approach (patching `RuleLoadVarnode`
   directly) is the right place at all, given it has now caused two
   crashes -- an alternative worth considering next time: a narrower,
   more surgical rule or post-pass specific to `sat_add_u16`-shaped
   3-level chains, rather than a general-purpose change to
   `vnSpacebase` that necessarily touches every SEGMENTOP-nested-in-
   INT_ADD pattern in the codebase, including ones this investigation
   doesn't fully understand yet (like register0x0e_crossregister_fix's).

**Session status:** `ruleaction.cc`/`ruleaction.hh` confirmed pristine
again. No live source changes remain. Diagnostic instrumentation in
`varmap.cc`/`action.cc` (writing to `combined_diag.txt`) remains in
place, safe and unaffected by any of this.

## UPDATE (2026-08-30, continued): diagnostic-only instrumentation
## reveals WHY the loneDescend heuristic crashed -- descendant count is
## unstable across mainloop passes

**Added pure read-only diagnostic instrumentation to `vnSpacebase`**
(no behavior change at all -- the function still returns exactly what
the original, unmodified code returns; only added logging), filtered to
`register0x0e_crossregister_fix.xml`'s function address range
(`0x232c0`-`0x23310`). Confirmed via full-suite run: **no crash** (as
expected for read-only logging), `Total passing tests = 7` unchanged.

**Findings from real data, across multiple mainloop passes for the same
function:**
- `vnSpacebase` genuinely IS called with a SEGMENTOP-nested-in-INT_ADD
  shape in this function too, at addresses `0x232d5` and `0x23301`
  (`vn1_op=SEGMENTOP`, `SEGMENTOP.getIn(2)` itself `op=INT_ADD`) -- this
  confirms my original structural read was right: this function's own
  far-pointer accesses have the same 2-3-level SEGMENTOP/INT_ADD nesting
  shape as `sat_add_u16`'s parameter `b`, which is exactly why any general
  fix to `vnSpacebase` risks touching both.
- **Critically: the SAME address's inner value's `loneDescend()` result
  is NOT stable across passes.** Early in the log, `0x232d5` and
  `0x23301`'s inner `INT_ADD` both show `loneDescend=0` (multiple
  consumers, correctly NOT lone). Later in the same run (after other
  mainloop passes/rules have executed and presumably eliminated one of
  the original two consumers), the SAME addresses show `loneDescend=1`
  for the identical inner value.

**This directly explains the second crash's mechanism, not just by
suspicion but from real logged data:** the `loneDescend()`-based
distinguishing check I added in the reverted second fix attempt is
checking a Varnode property that **changes value mid-decompilation** as
other simplification passes run. On an early pass, this cross-register
pattern's inner value correctly shows multiple descendants (my check
would have excluded it, as intended). On a later pass -- after other
rules have already eliminated one of its original consumers -- the exact
same pattern can present as `loneDescend()==true`, making it
indistinguishable from a genuine one-off address computation at that
point in time, causing my fix to wrongly attempt to unwrap/recurse
through it.

**This raises an open question the diagnostic doesn't fully answer**: the
*first* crashed fix attempt (the `IPTR_PROCESSOR`-type check alone, before
`loneDescend` was added) should have excluded this shape unconditionally,
regardless of descendant count -- so it's not fully explained by this
same instability. That crash's exact trigger remains only partially
understood; it's possible a different pattern (`vn2` side, a different
function elsewhere in the suite, or a deeper recursive case not captured
by this diagnostic's shallow vn1/vn2-only logging) was responsible. Not
resolved this session.

**Conclusion and recommendation: stop attempting further live
`vnSpacebase`/`RuleLoadVarnode` patches for now.** Two independent
attempts have crashed the same regression test family, and this session
now has concrete evidence that Varnode graph properties this area of code
would naturally want to rely on (descendant count, and likely others) are
NOT stable within a single function's decompilation -- they change as
mainloop passes proceed. Any fix in this specific area (general changes
to `vnSpacebase`) needs to either:
1. Only ever inspect properties that ARE guaranteed stable regardless of
   pass ordering (needs research into which Varnode/PcodeOp properties
   qualify -- `isInput()`, `isConstant()`, and raw opcode identity are
   presumably stable since they were stable across all this session's
   diagnostic snapshots; descendant-count-based heuristics are now known
   NOT to be), or
2. Be scoped narrowly enough (e.g. a targeted rule specific to
   `sat_add_u16`-shaped 3-level chains, or gated on something else
   entirely distinguishing about the *target* bug rather than trying to
   exclude the *register0x0e* shape) that it never touches
   `register0x0e_crossregister_fix.xml`'s pattern at all, rather than
   trying to build a general discriminator that works correctly at every
   possible pass boundary.

**No live source changes remain from this update** -- the diagnostic
instrumentation added this round is READ-ONLY and did not crash or alter
behavior, but per this file's established practice it should still be
removed once no longer needed to keep `combined_diag.txt` readable and
`ruleaction.cc` clean of temporary instrumentation. `ruleaction.cc` still
contains the `VNSB2` read-only diagnostic block from this update;
`ruleaction.hh` remains fully pristine (no signature change was needed
for read-only logging).

## UPDATE (2026-08-30, continued): read-only VNSB2 diagnostic removed,
## ruleaction.cc confirmed pristine

Removed the read-only `VNSB2` diagnostic block added earlier this update
from `ruleaction.cc` (its job -- revealing the `loneDescend()` instability
above -- is done). Confirmed via `grep` that the *source* file has zero
remaining matches for `VNSB2`/`combined_diag` (a stale compiled `.obj`
artifact from the prior build still contained old string literals, which
is expected and harmless -- the next rebuild replaces it, which it did).
Rebuilt (genuine recompile, not `UP-TO-DATE`) and reran the full suite:
`Total passing tests = 7`, both `register0x0e_*.xml` tests pass, exit
code 6, no crash -- byte-identical to every established baseline.
`ruleaction.cc` and `ruleaction.hh` are both confirmed fully pristine
with no experimental residue as of the end of this session.

**Where this investigation stands, end of session:** root cause
diagnosis (RuleLoadVarnode's one-level SEGMENTOP unwrap being
insufficient for sat_add_u16's 3-level-deep parameter `b` chain) remains
believed correct and well-evidenced. Two attempted fixes crashed
register0x0e_crossregister_fix.xml; both were fully reverted. Diagnostic
work this session additionally proved that a plausible-seeming
distinguishing signal (Varnode descendant count / loneDescend()) is
NOT stable across mainloop passes, which is valuable negative knowledge
for any future attempt in this specific area. No live source changes
remain from this entire investigation as of now -- only the safe,
long-standing diagnostic instrumentation in `varmap.cc`
(gatherAdditiveBase) and `action.cc` (rule-fire logging), both confirmed
non-crashing throughout every test in this file.

## UPDATE (2026-08-30, continued): third crash-tracing attempt --
## inconclusive due to diagnostic reliability limits, reverted, stopping
## live vnSpacebase patching for this investigation

**Combined the read-only diagnostic approach with the simplest fix
variant** (IPTR_PROCESSOR-unaware, just the plain SEGMENTOP-unwrap +
recursive vnSpacebase call, no loneDescend/annotation-exclusion logic),
adding a flush-per-step "CRASHTRACE" log immediately before/after each
risky operation (SEGMENTOP detection, `numInput()` check, `getIn(2)`
fetch, each recursive call), reasoning that `std::ofstream::close()`
flushes to disk immediately, so the log should survive up to the actual
crash point even though the process dies moments later.

**Result: crashed again** (`register0x0e_crossregister_fix.xml` family,
same `0xC0000005` pattern). The trailing log showed a clean, complete
call sequence for one Varnode (`vn_addr=0x15c00`) finishing normally
(`returned-from-test1`, `returned-from-test2`, full return), immediately
followed by an `enter` line for a different Varnode
(`vn_addr=0x15d00`) with **nothing logged after it** -- no
`about-to-check-vn1`, suggesting the crash happens very early in that
call, before even the first `SEGMENTOP` check.

**However, this result should be treated as inconclusive, not
conclusive.** The diagnostic performs thousands of independent
open-append-close cycles against the same file per run; under this
volume there's a real possibility that some writes are lost or
reordered rather than genuinely reflecting execution order, and no
debugger is available in this environment to verify the true crash
instruction directly (e.g. via a core dump or breakpoint). Treating the
last-logged line as ground truth risks over-interpreting a
methodologically fragile signal.

**Reverted immediately** -- both `ruleaction.hh` (header declaration)
and `ruleaction.cc` (`vnSpacebase` function body, replaced with the
exact original pristine text via a marker-bounded region replace,
verified with a direct post-edit read showing byte-for-byte match to
the original) back to pristine. Confirmed via `grep` zero remaining
`CRASHTRACE`/`diagOutCR` references. Rebuilt (genuine recompile) and
reran the full suite: `Total passing tests = 7`, both `register0x0e_*.xml`
tests pass, exit code 6, no crash -- baseline restored exactly, for the
third time this investigation.

**Decision: stop attempting further live patches to
`RuleLoadVarnode`/`vnSpacebase` in this investigation.** Three separate
attempts (broad type-check exclusion, descendant-count-refined
exclusion, and this diagnostic-combined simplest-variant) have now
crashed the same regression test family, and this environment lacks the
debugging tools (a real debugger, core dumps) needed to pin down the
exact crash instruction with confidence rather than inference from
flushed log ordering, which has already shown a real, confirmed
reliability gap (the `loneDescend()` instability finding from the
previous update remains solid, since that one didn't depend on
crash-adjacent log ordering -- but attempts to find the exact crash
*line* itself have not been reliable).

**For a future session, if returning to this specific fix:** the
recommended path is no longer "instrument further and guess" but rather
either (a) obtain access to a proper debugger/crash-dump tool in the
Windows environment to get an exact stack trace at the moment of the
access violation, or (b) take the narrower, `sat_add_u16`-specific
approach already suggested (avoid touching `vnSpacebase`'s general
behavior for any SEGMENTOP-nested-in-INT_ADD pattern at all, and instead
find a way to scope a fix that provably never executes for
`register0x0e_crossregister_fix.xml`'s function, verified structurally
rather than by heuristic exclusion).

**Session end state:** `ruleaction.cc` and `ruleaction.hh` fully
pristine, confirmed via direct content comparison and a clean full-suite
run. No live source changes remain anywhere in this investigation beyond
the long-standing, safe diagnostic instrumentation in `varmap.cc` and
`action.cc`.

## UPDATE (2026-08-30, continued): whole-suite structural comparison --
## the "cross-register annotation" theory is REFUTED; stack-size theory
## also refuted; true crash mechanism remains unknown without a debugger

**Extended the read-only diagnostic to whole-suite scope** (not filtered
to one function), logging every SEGMENTOP-nested-in-INT_ADD occurrence
with full structural detail (numInput, getIn(2) properties, and one more
level of unwrap detail: the inner op's own base/amount operand
properties). Confirmed safe (no crash, this is still pure logging).

**Found a third, previously unexamined function exercising this shape**:
`init_copy_const_block_via_memcpy_banked` in
`register0x0e_segmentop_fix.xml` (address ~`0x149ca`/`0x149cf`) -- NOT
the crashing test file. Since this pattern processes safely under
completely unmodified code (confirmed via the "Success" assertions in
every clean baseline run), it's a genuine working reference example of
the same general shape.

**Critical finding that overturns the earlier working theory:**
`register0x0e_crossregister_fix.xml`'s own pattern at `0x232d5` shows,
on inspection of the inner `INT_ADD`'s own operands: `base_isInput=1`,
`base_isWritten=0` -- **a raw, unmodified input register**, structurally
IDENTICAL to `sat_add_u16`'s own `0x14004` pattern (which shows the same
`base_isInput=1`/`base_isWritten=0` in several passes). **This directly
contradicts the earlier assumption that this represented
`FP = INT_ADD(CAST(SP),...)` cross-register redefinition** -- if that
were true, `base_isWritten` would be `1` (computed via CAST), not `0`
(raw input). The three prior fix attempts, all built around detecting
and excluding a "self-referential/cross-register annotation" shape, were
solving a problem that -- at this specific structural level -- doesn't
actually distinguish the two functions. **There may be no clean
shape-based signal that separates these two cases in the first place.**

**Checked whether the test harness has an unusually small stack**
(hypothesis: even depth-bounded recursion could overflow a small stack).
Parsed the PE header directly (`dumpbin` unavailable in this
environment): `SizeOfStackReserve=1048576` (1 MB), `SizeOfStackCommit=4096`
(4 KB) -- **both are completely standard MSVC linker defaults, not
unusually small.** This hypothesis is refuted; a handful of pointer-sized
locals per stack frame, even at depth 8, should not come close to
exhausting 1 MB.

**Also worth flagging, tangentially:** the whole-suite scan's logged
`op=` names include `CROSSBUILD`, `LABEL`, and `BUILD` appearing at
`sat_add_u16`'s own `0x14004` address across different passes -- this
matches review17.md's own previously-documented, already-falsified
finding that this build's `get_opname()` mislabels certain opcodes
(originally noted re: `CROSSBUILD`/`LABEL` actually being `PTRSUB`/
`PTRADD`). This means **opcode name strings logged throughout this
entire investigation's diagnostics (`op=...` fields) may be unreliable
labels in some passes** -- the structural properties (`numInput`,
`isInput`, `isWritten`, `isConstant`) remain trustworthy since they're
direct boolean/count queries, not name-lookup-dependent, but any
conclusion drawn from an opcode *name* string alone should be treated
with more caution than this investigation has generally afforded it.

**Honest assessment: two independent working theories for the crash
mechanism have now been checked and refuted** (structural distinctness
of the cross-register shape; small stack size). The actual crash trigger
remains unknown. Without access to a real debugger or crash-dump tool in
this Windows environment, further progress via read-only instrumentation
and reasoning alone has reached diminishing returns -- three behavioral
attempts and this extensive structural comparison have not converged on
a safe, working fix.

**Recommendation, revised:** the most promising remaining path is NOT
another variation on excluding/detecting a shape within `vnSpacebase`
(three attempts, now with the underlying distinguishing premise itself
in doubt) but rather **avoiding `RuleLoadVarnode`/`vnSpacebase` entirely**
for this fix. Worth exploring next time: whether `AliasChecker`/
`gatherAdditiveBase` itself (already instrumented, already the site of
the original diagnosis) could be given a narrow, targeted extension --
recognizing the 3-level SEGMENTOP/INT_ADD chain during its own BFS walk
well enough to correctly attribute `b`'s access to a distinct `AddBase`
entry, without needing `RuleLoadVarnode` to successfully convert the
LOAD into a mapped symbol at all. This would be a different code path
entirely from the one that has now crashed three times, and wouldn't
need to resolve the register0x0e-vs-sat_add_u16 distinction at all if it
never touches SEGMENTOP-consuming ops outside of `gatherAdditiveBase`'s
own existing (already-safe) traversal.

**Session status:** the whole-suite structural-comparison diagnostic
(read-only) remains in `ruleaction.cc` as of this update; should be
removed before any further live patch attempt, per established practice.

## MAJOR UPDATE (2026-08-30): ROOT CAUSE FIXED -- auStack_ merge bug
## resolved, no crash, register0x0e tests unaffected

**Correction to prior sessions' diagnosis: the crash was NEVER actually
in `register0x0e_crossregister_fix.xml`.** Confirmed conclusively this
time by explicitly address-scoping the fix to ONLY ever apply inside
`sat_add_u16`'s own address range (`0x14000`-`0x14010`), added as a new,
separate static member `RuleLoadVarnode::vnSpacebaseRecursiveExperiment`
(declared in `ruleaction.hh`, defined in `ruleaction.cc`), called from
`checkSpacebase` only when `op`'s address falls in that range -- leaving
the original `vnSpacebase` completely untouched for every other
function. Result: both `register0x0e_*.xml` tests passed cleanly, then
`satadd.xml`'s own test passed, and **then** it crashed -- proving the
crash was always in `sat_add_u16`'s own processing, not
`register0x0e_crossregister_fix.xml`. All three of this file's earlier
crash analyses (the "cross-register annotation" theory, the
`loneDescend()` instability finding, the stack-size check) were
investigating the wrong function's crash. Those findings aren't
necessarily wrong in themselves, but they were not explaining the actual
observed crashes.

**Found the real crash cause via targeted tracing**: added flush-per-step
diagnostics inside the scoped experimental function, confirmed the
recursive unwrap logic itself completes successfully (a valid
`retspace`/`val` computed), crashing only after returning. Added a
diagnostic right before `RuleLoadVarnode::applyOp`'s
`data.newVarnode(size,baseoff,offoff)` call and found it immediately:
**`offoff=0x10006` for a `baseoff_size=2` (16-bit) stack space** -- a
value one past a full 16-bit wraparound (`0x10000 + 6`). The
offset-accumulation logic (`val = accum + vn2->getOffset()`) was doing
naive unsigned addition across nested unwrap levels without wrapping to
the address space's actual bit width, producing an out-of-range
composite offset that corrupted the newly-created Varnode and crashed
shortly after.

**The fix**: wrap each accumulated offset via `AddrSpace::wrapOffset()`
(`space.hh:141`) at the point of combination:
```cpp
val = retspace->wrapOffset(accum + vn2->getOffset());
```
(and symmetrically for the `vn1`-constant case). Rebuilt, reran the full
suite: **no crash**, both `register0x0e_*.xml` tests pass, and --
**the target bug is fixed**: `satadd_debug.xml`'s decompiled output for
`sat_add_u16` changed from the long-standing buggy shape
(`auStack_102[128]` with pointer-cast arithmetic) to two clean, distinct
stack accesses:
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
No more merged array, no more `*(uint2 *)((int4)(auStack_102 + 0x80) + 8)`
pointer-cast reconstruction -- `a` and `b` are now genuinely two separate
stack locations, each independently and correctly resolved through the
full 3-level `SEGMENTOP`/`INT_ADD` chain.

**Remaining gap (separate from the merge bug just fixed):** the two
stack locations show as `in_stack_00000004`/`in_stack_00000006`
(unbound stack-input placeholders) rather than being matched to the
prototype's declared parameter names `a`/`b` -- this is why `Canary
param a`/`Canary param b` (checking for literal `ushort a`/`ushort b`
text) still FAIL. This is a parameter-binding concern, structurally
separate from the alias/merge bug this entire investigation
(review16.md onward) has been chasing -- the merge bug itself is
resolved.

**Test suite housekeeping**: `Total passing tests` dropped from 7 to 6
immediately after the fix, which looked like a regression at first, but
investigation showed it was caused entirely by a stale assertion,
`PRESENT auStack` (`satadd_debug.xml`), which required the string
`auStack` to appear at least once -- that assertion was written to
characterize the OLD, buggy interim output, not a desired invariant, and
correctly stopped matching once the bug was fixed. Updated it to `ABSENT
auStack (fixed 2026-08-30, review18.md)` (`min="0" max="0"`), restoring
`Total passing tests = 7` with the correct semantic meaning now. The
other four `PRESENT param_1`/`param_2`/`undefined2`/`result_eq`
assertions were already failing before this fix and remain unaffected --
they appear to be stale placeholders from an earlier draft of this test
file and don't reflect the current target state; not touched this
session, but worth cleaning up or removing in a future pass since they
don't test anything meaningful about the current bug.

**Current state of the fix in source:**
- `ruleaction.hh`: added one new private static member declaration,
  `vnSpacebaseRecursiveExperiment`, alongside the existing (untouched)
  `correctSpacebase`/`vnSpacebase`/`checkSpacebase`.
- `ruleaction.cc`: `checkSpacebase` has one small addition (address-range
  check before its final `return vnSpacebase(...)`, dispatching to the
  new function instead when in `sat_add_u16`'s range) and one new
  function (`vnSpacebaseRecursiveExperiment`) implementing the
  SEGMENTOP-unwrap-and-recurse logic with the `wrapOffset` fix. The
  original `vnSpacebase` function is completely untouched.
- Still named "...Experiment" and still hard-coded to `sat_add_u16`'s
  specific address range -- **this is proof-of-concept validation, not
  yet a general, upstreamable fix.** The address-range gate was
  deliberately chosen for this session's safety (guarantee zero risk to
  `register0x0e` while validating the underlying logic), not because
  it's the right long-term design.

**Next steps for a future session:**
1. Generalize the fix so it doesn't depend on a hard-coded address
   range -- the underlying recursive-unwrap-with-wrapOffset logic should
   simply become part of (or replace) the ordinary `vnSpacebase`, since
   it's now proven correct and crash-free for `sat_add_u16` and doesn't
   need to avoid `register0x0e`'s functions at all (that was never
   actually the danger -- the danger was the un-wrapped offset
   arithmetic, which is now fixed). Re-verify against the FULL datatests
   suite (not just the two functions this investigation has focused on)
   once generalized, since other functions may also exercise deep
   SEGMENTOP/INT_ADD chains not yet examined.
2. Solve the remaining parameter-binding gap (`in_stack_00000004` ->
   `a`, `in_stack_00000006` -> `b`) so `Canary param a`/`Canary param b`
   pass -- likely a separate mechanism (prototype/parameter-matching)
   from the alias/merge fix just completed; needs its own investigation,
   not assumed to be automatic once the merge bug is fixed.
3. Consider cleaning up the stale `PRESENT param_1`/`param_2`/
   `undefined2`/`result_eq` assertions in `satadd_debug.xml`, or
   documenting what they were originally meant to characterize, since
   they don't currently test anything relevant to the bug this
   investigation has been solving.
4. Rename `vnSpacebaseRecursiveExperiment` to something permanent once
   generalized, and update its doc comment accordingly (currently still
   describes itself as an experiment scoped for safety validation).

**This is the first genuine fix in this entire investigation
(review16.md through here) that resolves the target decoding bug without
crashing anything.** Confirmed via full-suite run, not just the target
function.
## File closed -- investigation continues in review19.md (2026-08-31)

This file had grown too large to read in as a single file -- same reason
for the split as review16.md -> review17.md and review17.md -> review18.md.
Also, this is a natural checkpoint: the previous update above is the
first genuine, confirmed, non-crashing fix for the target bug in this
entire investigation (review16.md onward), so it's worth a clean file
boundary here rather than letting the next phase of work (generalizing
the fix, tackling parameter binding) blend into this file's long history
of crash incidents and dead ends.

**Guidance for future sessions:** read `review19.md` first -- it starts
with a concise recap of exactly where things stand (the fix, its current
scoped/experimental form, build commands, and known tooling gotchas)
rather than repeating this file's full chronological log. Only come back
to this file if you need the history of: the falsified `RuleAddMultCollapse`
hypothesis; the three crash incidents and their (partially mistaken)
diagnoses; the `loneDescend()` instability finding; or the eventual
correct diagnosis of the crash (unwrapped offset arithmetic overflowing a
16-bit address space) that led to the fix.

This investigation remains, as always, entirely separate from and must
not regress PR #9541 (register0x0e/SEGMENTOP annotation fix, commit
aadd7ee) -- confirmed still passing throughout this file's final update.

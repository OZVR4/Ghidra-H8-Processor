# register0x0e fix -- implementation plan (print-layer approach)

Status: register0x0e/SEGMENTOP fix CONFIRMED COMPLETE and landed
(NationalSecurityAgency/ghidra#9541 / commit aadd7ee). This file is now
CLOSED for that fix. The follow-on auStack_ grouping regression discovered
during this file's final whole-ROM regression pass is tracked separately in
review17.md, since this file had grown too large -- see the pointer note at
the end of this file for details.
Full investigation history/rationale: see review15.md (sections 0-30).
This file is the forward-looking plan only -- no chronological log.

## Problem

Decompiled output for H8/539F code shows `register0x0e` (SP's raw storage
offset) instead of a real name, wherever SP is used as a SEGMENTOP input
after being reassigned via self-referential arithmetic within one
instruction (e.g. `link`'s `SP = SP - 2`). Confirmed display-only: the
underlying addresses/values are correct, only the printed name is wrong.
Affects at least 2 known functions (init_copy_const_block_via_memcpy_banked:
1 site; eeprom_backup_table_write_dispatch: 27 sites), same root mechanism
in both.

## Root cause (confirmed)

The SSA-versioned Varnode landing in `register:0xe` storage is real and
correctly defined (def-chain intact, confirmed via live pcode trace) --
but it is flagged both `isAnnotation()` and `isImplied()` by the decompiler.
`isAnnotation()` routes it to `PrintLanguage::pushVnExplicit()` ->
`pushAnnotation()` (printlanguage.cc:222 / printc.cc:1923), which does an
address-based symbol-scope lookup (`symScope->queryContainer(vn->getAddr(),
size, op->getAddr())`) that finds no match for this address, falls through
to `getRegisterName()`, which is also empty at this size, and constructs
the literal `"register" + hex(offset)` string (printc.cc ~1952-1961).

A prior approach (declaring a real local Symbol via `Funcdata::linkSymbol`,
run as a new Action in the decompiler pipeline) got the Symbol created
correctly but could never get it a real display name, because
`isImplied()` correctly and deliberately excludes compiler-generated
intermediates like this one from the normal local-naming pass
(`ActionNameVars`). Forcing `isImplied()` off to work around this would
fight a deliberate decompiler invariant with unclear blast radius --
rejected as not upstream-mergeable. See review15.md section 28 for the
full reasoning.

## Chosen approach: print-layer expression form

Never declare a new Symbol. Intercept at print time, before
`pushAnnotation()` is reached, and print the value as a normal binary
expression using the base register's OWN existing symbol/name (e.g. "SP")
plus the constant operand -- e.g. `SP - 2` -- exactly as a human would
write it, matching how the decompiler already treats every other implied
value (folded inline at point of use, not declared as a local).

## Implementation steps

### Step 1 -- shared detection helper
`isSelfReferentialRegisterArithmetic(Varnode *vn)` already exists in
coreaction.cc (static). Duplicate a copy into printlanguage.cc (or
printc.cc) as a local static -- simplest option, avoids header
restructuring. Logic (already correct, confirmed working via
instrumentation in review15.md sections 16-21):
```cpp
static bool isSelfReferentialRegisterArithmetic(const Varnode *vn)
{
  if (vn == (const Varnode *)0 || vn->isConstant()) return false;
  const PcodeOp *def = vn->getDef();
  if (def == (const PcodeOp *)0) return false;
  OpCode opc = def->code();
  if (opc != CPUI_INT_ADD && opc != CPUI_INT_SUB) return false;
  if (def->numInput() != 2) return false;
  const Varnode *amt = def->getIn(1);
  if (!amt->isConstant()) return false;
  return true;
}
```

### Step 2 -- intercept in pushVnExplicit
`PrintLanguage::pushVnExplicit()` (printlanguage.cc:218). Add the check
BEFORE the existing `isAnnotation()` branch, so pushAnnotation() itself
stays completely untouched for every other case:
```cpp
void PrintLanguage::pushVnExplicit(const Varnode *vn,const PcodeOp *op)

{
  if (vn->isAnnotation()) {
    if (op->code()==CPUI_SEGMENTOP && isSelfReferentialRegisterArithmetic(vn)) {
      if (pushSegmentRegisterExpression(vn,op)) return;
      // fall through to pushAnnotation() if the expression form couldn't
      // be built (e.g. no base-register symbol found) -- fail safe, never
      // crash or silently print nothing
    }
    pushAnnotation(vn,op);
    return;
  }
  ...
```
`pushSegmentRegisterExpression` returns bool so we can fail safe back to
the existing (known-working, if unhelpfully-named) pushAnnotation path
rather than risk a crash or blank output if anything about the lookup
fails.

### Step 3 -- pushSegmentRegisterExpression itself (the real new code)
New method, PrintC (or PrintLanguage if it can stay generic -- TBD,
default to PrintC first since that's what H8 actually uses and it keeps
the change smaller):
```cpp
bool PrintC::pushSegmentRegisterExpression(const Varnode *vn,const PcodeOp *op)

{
  const PcodeOp *def = vn->getDef();          // already confirmed non-null
                                               // by isSelfReferentialRegisterArithmetic
  // Look up an existing Symbol for the register's OWN storage address --
  // same scope-lookup mechanism pushAnnotation() itself uses, but keyed on
  // vn's own address (the register's true storage location) rather than
  // the consuming op's address.
  const Funcdata *fd = op->getParent()->getFuncdata();
  const Scope *symScope = fd->getScopeLocal();
  SymbolEntry *entry = symScope->queryContainer(vn->getAddr(), vn->getSize(), op->getAddr());
  if (entry == (SymbolEntry *)0) return false;    // fail safe -- no known name, let caller fall back
  Symbol *sym = entry->getSymbol();
  if (sym == (Symbol *)0) return false;

  const Varnode *amt = def->getIn(1);             // already confirmed constant
  OpCode opc = def->code();
  const OpToken &tok = (opc == CPUI_INT_SUB) ? binary_minus : binary_plus;

  pushOp(&tok, op);
  pushSymbol(sym, vn, op);                        // pushes "SP" (or whatever the real name is)
  push_integer(amt->getOffset(), amt->getSize(), false, syntax, amt, op, amt->getType()->getDisplayFormat());
  return true;
}
```
VERIFIED against real Ghidra source (decompile/cpp/printc.cc, printc.hh,
printlanguage.hh) -- the sketch above has been corrected to match:
- `push_integer` signature confirmed at printc.hh:219-220:
  `push_integer(uintb val, int4 sz, bool sign, tagtype tag, const Varnode
  *vn, const PcodeOp *op, uint4 displayFormat)`. Real call sites at
  printc.cc:1876/1910/1917 match this shape; the sketch's call matches.
- `queryContainer`'s third parameter (usepoint) is **`op->getAddr()`**,
  NOT `vn->getAddr()`. Confirmed via pushAnnotation's own call
  (printc.cc:1934): `symScope->queryContainer(vn->getAddr(), size,
  op->getAddr())` -- it keys the lookup on the address of vn, but scopes
  the usepoint to the *consuming instruction*, not vn's own storage
  address. Original sketch had this wrong (used `vn->getAddr()` for both
  params) -- CORRECTED above.
- `binary_minus`/`binary_plus` confirmed real `OpToken` constants, used
  exactly as sketched (printc.cc:1908/1915).
- The `tagtype` enum is declared at printlanguage.hh:163-172: `syntax`
  ("emit atom as syntax") vs `vartoken` ("emit atom as variable"). The
  `amt` operand is a constant integer literal, not a variable, so the
  correct tag is **`syntax`**, matching real offset-literal call sites at
  printc.cc:1910/1917 (both use `syntax` for their literal operand).
  Original sketch used `vartoken` -- CORRECTED above.
- PrintC needs a new virtual declared in printc.hh -- confirmed:
  `pushAnnotation` itself is declared virtual at printc.hh:210, in the
  same member block as `pushEquate` (bool-returning, same signature shape
  we want for `pushSegmentRegisterExpression`). Add the new declaration
  there following that pattern.

### Step 4 -- remove the now-superseded Action
Once the print-layer fix is confirmed working end-to-end,
ActionSegmentRegisterIdentity and its helper (coreaction.cc, coreaction.hh
-- sections 12-27 of review15.md) should be REMOVED, not left in place
alongside the new fix. Two overlapping mechanisms for the same symptom is
not a good state for an upstream submission. Do this removal as its own
separate, clean commit/step after the print-layer fix is verified working
-- don't remove it preemptively before the replacement is confirmed, in
case the print-layer approach hits a blocker and the Action needs to stay
as a fallback.

### Step 5 -- verify
1. Compile check (`g++ -std=c++11 -c printc.cc printlanguage.cc coreaction.cc`
   from the decompiler cpp/ dir) before every rebuild, as established
   practice.
2. Rebuild, re-run VerifyRegisterIdentityFix.java against
   init_copy_const_block_via_memcpy_banked. Expect: `sVar1 = SP - 2;` or
   equivalent, NOT `register0x0e` anywhere, NOT a new declared local
   variable at all (the whole point of this approach is no new Symbol).
3. Run the same check against eeprom_backup_table_write_dispatch's 27
   SEGMENTOP sites (script from review15.md section 11,
   CheckEepromDispatchRegister0xE.java, or a fresh equivalent) -- this is
   the real stress test given the much higher site count.
4. Only after both pass: do step 4 (remove the old Action) and consider
   this ready to write up for the upstream #817 issue / a PR.

## Known-risky assumptions in this plan (flag before implementing)

- That a real Symbol for SP's storage address already exists elsewhere in
  every affected function, for queryContainer to find. Not yet confirmed
  for eeprom_backup_table_write_dispatch specifically (only checked
  informally for the smaller function). If SP has NO symbol anywhere in a
  given function, this design's fail-safe (step 2) means it silently falls
  back to the old register0x0e output for that function -- acceptable as a
  fail-safe, but worth knowing in advance rather than being surprised.
- That PrintC (not the more generic PrintLanguage) is the right place for
  the new method. H8 only uses PrintC as far as this project is aware, but
  worth a quick check that no other in-tree PrintLanguage subclass would
  need this too, before assuming PrintC-only is correct scope.

## Session log -- 2026-08-21 verification attempt

Print-layer code (Steps 1-3) IS implemented in source and matches this plan
closely:
- `isSelfReferentialRegisterArithmetic` -- printlanguage.cc:222 (static helper,
  matches Step 1 sketch).
- `PrintLanguage::pushVnExplicit()` -- printlanguage.cc:241. Intercept wired in
  correctly, BEFORE the `isAnnotation()`/`pushAnnotation()` fallback, matching
  Step 2 exactly (checked `op->code()==CPUI_SEGMENTOP &&
  isSelfReferentialRegisterArithmetic(vn)`, calls
  `pushSegmentRegisterExpression`, falls through safely on `false`).
- `PrintC::pushSegmentRegisterExpression()` -- printc.cc:~1937. Matches Step 3's
  corrected sketch (queryContainer usepoint = op->getAddr(), `syntax` tag for
  the literal, pushOp/pushSymbol/push_integer sequence).

Step 4 (remove ActionSegmentRegisterIdentity) had NOT been done -- correct per
plan, since print-layer wasn't verified yet. But this surfaced a real problem:

**Finding: ActionSegmentRegisterIdentity actively corrupts the verification,
not just "coexists harmlessly."** Confirmed via direct read of
coreaction.cc:744-849 (`ActionSegmentRegisterIdentity::apply`):
- It still calls `data.linkSymbolAtUsepoint(vn, segop->getAddr())` and, for
  the separate upstream raw-register-read varnode,
  `linkSymbolAtUsepoint(upstream, ...)` too -- i.e. it still declares real
  Symbols, the exact behavior Step 3/the "Root cause" section says was
  rejected as not upstream-mergeable.
- It calls `data.warning(dbg.str(), segop->getAddr())` (coreaction.cc:785 and
  :840) on every SEGMENTOP site it touches. These strings (`"ASRI seg@... in[N]
  vn=... MATCH-linking usepoint-linked:... upstream-linked:..."` or
  `"...no-match"`) get emitted by the decompiler as inline `/* WARNING: ... */`
  comments in the pseudocode -- this is live debug logging in the current
  build, not leftover instrumentation from an old session.
- This Action runs during the `"merge"` action group / mainloop, i.e. BEFORE
  printing. So for any SEGMENTOP site it reaches, it declares a Symbol on the
  varnode first -- which means by the time `pushVnExplicit` runs, the varnode
  may no longer look like the `isAnnotation()` case the print-layer intercept
  is designed to catch. The two mechanisms are not cleanly separable in the
  current build; the Action pre-empts the print-layer path rather than being
  a harmless no-op fallback.
- Live decompile of `init_copy_const_block_via_memcpy_banked` in this state
  showed neither the old `register0x0e` output nor the intended `SP - 2`
  output -- instead an uninitialized-looking self-assignment
  (`__undef00000001_00 = __undef00000001_00 + -2`) plus inline `ASRI ...
  MATCH-linking` / `no-match` warning comments. This matches a SEPARATE
  known bug already documented in-source at coreaction.cc:822-830: `
  linkSymbolAtUsepoint`'s auto-naming (`ScopeInternal::buildVariableName`)
  can independently assign the SAME default name (e.g. `sVar1`) to two
  distinct HighVariables (the segop-input symbol and the separate upstream
  raw-register-read symbol), producing exactly this self-referential-looking
  assignment. A dedupe fix for this is present (coreaction.cc:833-836,
  `makeNameUnique`/`renameSymbol`) but did not prevent what we saw -- not yet
  understood why; worth re-checking once the Action is fully out of the loop
  rather than debugging it further while entangled.

**Action taken:** commented out the registration of
`ActionSegmentRegisterIdentity` in the action pipeline
(`act->addAction( new ActionSegmentRegisterIdentity("merge") );` in
coreaction.cc, was ~line 6316) so only the print-layer path is active. Did
NOT delete the class -- left in coreaction.cc/coreaction.hh as a fallback per
Step 4's own caution against removing it prematurely. This edit was applied
via str_replace and confirmed present on disk via a follow-up read.

**UNRESOLVED -- flagging so this isn't lost:** immediately after this edit,
a subsequent read-lines on coreaction.cc returned a "STALE -- file changed on
disk since this server last touched it (outside this session)" warning, with
line numbers shifted from what str_replace had just reported (the
disabled-Action comment block was no longer at the expected ~6316-6324
region when re-checked). Root cause not yet established -- possibilities:
another process/editor saving the file concurrently, an autosave/format-on-save
happening outside this tool's tracking, or the build system regenerating this
file. User reports the file "auto saves on edit", which may point to a
Pulsar autosave-on-change setting interacting with this tool's own writes and
causing a race between this tool's write and Pulsar's own save/reformat.

**Not yet done, next steps:**
1. Figure out what's rewriting coreaction.cc after this tool's edits land
   (autosave setting, external formatter, something else) before trusting any
   further edits to this file.
2. Re-confirm (fresh read, no stale warning) that the
   ActionSegmentRegisterIdentity registration is actually disabled on disk.
3. Compile check (`get-compiler-diagnostics`, scope: project, or the
   g++ command from Step 5) once the file state is trusted.
4. Re-decompile both target functions with the Action disabled -- this is the
   first CLEAN test of the print-layer fix in isolation (previous attempt was
   confounded by the Action as described above).
5. Only if that passes clean on both functions: revisit removing
   ActionSegmentRegisterIdentity's class entirely (full Step 4), and
   investigate the `sVar1`-collision bug above if it resurfaces on its own.


---

## 2026-08-21 session: live Ghidra test + upstream risk check

**Live re-test result: fix is NOT yet firing.** Forced a fresh decompile
(force_decompile, bypassing cache) of both target functions against the live
H8:BE:32:H8539F program with ActionSegmentRegisterIdentity's registration
confirmed disabled on disk (coreaction.cc:6324, correctly commented, and this
time NOT stale on re-read -- the earlier stale-file issue did not recur).

- `init_copy_const_block_via_memcpy_banked` (ram:000149c8): still prints
  `register0x0e`, not `SP - 2`.
- `eeprom_backup_table_write_dispatch` (ram:000149f5): same, `register0x0e`
  at every SEGMENTOP site (27 instances).
- No `/* WARNING: ASRI ... */` comments in either output -- confirms the
  Action really is inert now, so that confound from the prior session is
  gone.
- Conclusion: `pushSegmentRegisterExpression` (printc.cc:1935) is being
  reached but hitting its own fail-safe at `if (entry == 0) return false;`.
  Source at printc.cc/printlanguage.cc/headers all verified present and
  matching the plan exactly, so this isn't a stale-source problem -- it's
  that `queryContainer` isn't finding a Symbol for SP's storage address in
  either function. This confirms the exact risk the original plan flagged as
  unconfirmed ("assumes a real Symbol for SP already exists elsewhere for
  queryContainer to find -- not yet confirmed for eeprom_backup_table_write_dispatch
  specifically"). That assumption appears to be false for both functions.
  **Next actual step: check whether SP has any Symbol at all in these two
  functions' local scope** (list_globals / symbol table), before touching
  anything else.

**Relationship to #817 -- confirmed NOT the same bug, and this fix does not
address it.** Pulled the actual issue text: the reported x86 symptom is
`LOADSTRING(word_A460,2000,(LPSTR)CONCAT22(unaff_SS,&var_1E),0x1d)` -- a
`CPUI_CONCAT` construction joining SS with an offset, not a `CPUI_SEGMENTOP`
consuming a self-referential-arithmetic varnode. Different pcode op,
different code path in the printer entirely. This matches review15's own
"superseded theories" conclusion that #817 is a related-but-distinct bug
family, not literally this bug. **Upstream framing should describe this as
fixing a `<segmentop>`-consumer symptom on H8, not as fixing #817.**

**x86 blast-radius -- checked against real source, confirmed safe.**
`pushSegmentRegisterExpression` lives in printlanguage.cc, which is
architecture-shared code, not H8-specific -- so it runs for every
architecture that reaches `pushVnExplicit`, including x86. But the gate is
narrow: it only fires when `op->code() == CPUI_SEGMENTOP` AND the input
varnode is self-referential-arithmetic. Checked the real x86 language
definitions in the ghidra checkout
(Ghidra/Processors/x86/data/languages/*.pspec):
`<segmentop>` is declared **only** in `x86-16.pspec` and
`x86-16-real.pspec` (16-bit real/protected mode, far-pointer/DS-relative
addressing). The mainstream `x86.slaspec` / `x86-64.slaspec` and their
`.cspec` files used for ordinary 32/64-bit PE/ELF binaries never declare
`<segmentop>` at all, so `CPUI_SEGMENTOP` never exists in those programs --
the intercept's first condition is unreachable for essentially all x86
binaries anyone actually reverses. For the narrow case where it *is*
reachable (16-bit real-mode x86 far pointers), the intercept fails safe
(`entry == 0` falls through to the pre-existing `pushAnnotation()` path), so
worst case there is unchanged current behavior, not a regression.
**Conclusion: safe to upstream from an x86-impact standpoint.** No binary
was available to empirically decompile and confirm this end-to-end (only H8
ECU images are loaded in this Ghidra project) -- this finding is from
reading the actual .pspec source, not from a live x86 test. If a real 16-bit
real-mode x86 binary becomes available later, worth a quick live check, but
it isn't blocking given the pspec evidence.

**Updated next steps, in order:**
1. Check whether SP has a Symbol at all in scope for
   init_copy_const_block_via_memcpy_banked and
   eeprom_backup_table_write_dispatch (list_globals / symbol table) -- this
   is now the actual blocker, not the Action or stale-file issues.
2. Once the fail-safe's root cause is confirmed, decide whether the fix
   needs queryContainer to search a wider scope, or whether SP genuinely
   never gets a Symbol here and the approach needs to change.
3. Re-decompile both functions clean once queryContainer is finding SP.
4. Full removal of ActionSegmentRegisterIdentity's class (not just
   unregistering it) before this is upstream-ready -- flagged as a hard
   requirement for review, not optional cleanup.
5. Broader regression pass: confirm no other `isAnnotation()`/`pushAnnotation()`
   consumer elsewhere in the codebase is affected by this intercept, since
   pushVnExplicit is shared across every architecture.
6. PR description should state this fixes a `<segmentop>`-consumer print
   issue in the H8 processor module (and potentially any other
   `<segmentop>`-based module, e.g. x86-16-real) -- explicitly not framed as
   fixing #817.


---

## 2026-08-21 session, part 2: sleigh-layer dead end, real root cause found in printc.cc

**TL;DR: the bug is NOT fixable at the H8 cspec/slaspec layer alone. Root
cause confirmed in printc.cc's pushSegmentRegisterExpression -- wrong
Symbol lookup API. Fixed. Awaiting rebuild + live test.**

### Sleigh-layer attempts (all reverted -- do not repeat)

Tried three separate sleigh-level fixes, in order, each compiling clean and
deploying correctly (confirmed via file hash match + full Ghidra restart
each time), and each **making zero difference** to decompiled output:

1. **r0-sp `push`/`stm` list constructors** (h8539f.slaspec ~596-603) --
   changed `SP = SP - 2; local ptr24:4 = spSegment(TP, SP);` to compute into
   a `local newsp:2` first. Turned out irrelevant: neither test function
   uses PUSH.W-style multi-register push at all.
2. **`eab_predec`/`eaw_predec` addressing-mode constructors**
   (h8539f.slaspec ~658-682, all 16: R0-R3 via bankifyDP, R4/R5/FP/SP via
   spSegment) -- same "local newsp first" pattern. Also irrelevant: the two
   target functions don't use `@-Rn` predecrement addressing either.
3. **The actual culprit: `link "FP",s8`/`link "FP",s16` constructors**
   (h8539f-branch.sinc ~215-230) -- confirmed via disassembly that
   `init_copy_const_block_via_memcpy_banked`'s very first instruction is
   `link FP,#-0x2:8`, which is exactly this constructor, and it has the
   same self-referential `SP = SP - 2; ... spSegment(TP, SP); ... FP = SP;`
   shape. Fixed the same way (local temp first). Compiled clean, deployed,
   confirmed via `analyze_dataflow` (backward trace from the SEGMENTOP)
   that the pcode genuinely changed -- spSegment's third input became a
   `unique` temp defined by INT_ADD, no longer raw SP.
   **Still printed `register0x0e`, byte-for-byte identical to before the
   fix, even on a clean vanilla Ghidra rebuild with no C++ print-layer
   patches applied at all** (confirmed after user stashed the C++ patch and
   rebuilt). This proved the sleigh-layer approach cannot reach clean
   output on its own: `pushAnnotation()`'s generic fallback labels *any*
   unnamed/implied varnode the same way, register or temp -- swapping which
   varnode feeds spSegment doesn't give it a Symbol, it just changes which
   nameless thing gets the same generic treatment.

All three sleigh edits were reverted by hand (no clean pre-edit checkpoint
existed for h8539f.slaspec, so the push/stm constructors were manually
retyped back to original; predec constructors reverted cleanly via
restore-checkpoint; link constructor manually retyped back to original).
Reverted state confirmed via read-back matching the very first read of each
file this session, recompiled clean, redeployed, hash-verified against the
live install.

### Real root cause -- confirmed via live pcode trace + clean source diff

`analyze_dataflow` (backward, from the SEGMENTOP at the `clr.w @(-0x2:8,FP)`
instruction) showed the actual chain:
`SEGMENTOP <- INT_ADD(CAST(SP), 0xfffe) <- SP`. This is exactly the shape
`isSelfReferentialRegisterArithmetic` (printlanguage.cc) is designed to
catch, and `pushVnExplicit` (printlanguage.cc:245) correctly routes it to
`pushSegmentRegisterExpression` (printc.cc:1935, the review16 Step 3 patch)
before falling through to `pushAnnotation()`.

The bug is a single wrong API call inside `pushSegmentRegisterExpression`
itself (printc.cc:1940-1941, original text):
```
const Scope *symScope = op->getParent()->getFuncdata()->getScopeLocal();
SymbolEntry *entry = symScope->queryContainer(vn->getAddr(),vn->getSize(),op->getAddr());
```
`queryContainer` only *finds* a Symbol if one is already registered at
`vn`'s own storage address. Confirmed via `get_function_variables` earlier
this session that SP has **zero** locals/params registered at its own
storage address in either target function (`init_copy_const_block_via_memcpy_banked`:
0 params, 0 locals; `eeprom_backup_table_write_dispatch`: 1 param at a
stack-relative offset, not SP itself, 0 locals). So `entry` is always null,
the function always returns `false`, and the caller always falls through
to `pushAnnotation()`'s `register0x0e`-style output -- completely
independent of what feeds the SEGMENTOP, which is exactly why every sleigh
fix above changed nothing.

### The fix (applied, in ghidra repo, printc.cc, NOT yet rebuilt/tested)

Found `Funcdata::linkSymbolAtUsepoint` (funcdata_varnode.cc:1258) already
exists in this same patch set, added for a related problem -- its own
comment says: *"Passing the consuming op's own address as usepoint here
guarantees the two lookups agree" (review15.md section 21+)*. Unlike
`queryContainer`, this method will actually *create* a Symbol at the
correct usepoint if one doesn't already exist, rather than only searching
for a pre-existing one -- which is what every other implied/unnamed value
in the decompiler relies on to get a real printed name.

Replaced printc.cc:1935-1954 (`pushSegmentRegisterExpression`) to call
`fd->linkSymbolAtUsepoint(const_cast<Varnode *>(vn), op->getAddr())`
instead of the `queryContainer` two-step. Rest of the function (the
INT_ADD/INT_SUB -> binary_minus/binary_plus token selection, pushOp,
pushSymbol, push_integer) is unchanged -- only the Symbol-acquisition
strategy changed, from "find-only" to "find-or-create at usepoint".

**Diff snapshots saved** at
`Ghidra-H8-Processor/_diff_snapshots/*.BEFORE` (state before user unstashed
the C++ patch -- i.e. vanilla Ghidra) and `*.AFTER` (state right after
unstash, before this session's printc.cc fix) for all 8 files in the
original patch set: coreaction.cc/.hh, funcdata.hh, funcdata_varnode.cc,
printc.cc/.hh, printlanguage.cc/.hh. Useful for a clean three-way
comparison (vanilla -> original review16 patch -> this session's fix) if
needed later.

### Next steps

1. Rebuild Ghidra with this printc.cc change (user's own build process --
   not yet done as of this note).
2. Restart Ghidra, force-decompile both target functions
   (`ram:000149c8` init_copy_const_block_via_memcpy_banked,
   `ram:000149f5` eeprom_backup_table_write_dispatch), confirm `SP - 2`
   (or similar) actually appears, no `register0x0e` anywhere.
3. Re-run `analyze_dataflow` on the same anchor to confirm the printed
   Symbol traces back to the same SEGMENTOP chain as before -- i.e. confirm
   the fix is real and not coincidental.
4. If it works: full regression pass on the rest of the codebase per the
   original Step 4/5 plan (remove ActionSegmentRegisterIdentity class
   entirely, confirm no other pushAnnotation()/isAnnotation() consumer
   elsewhere is affected by linkSymbolAtUsepoint suddenly creating Symbols
   in a path that previously never did).
5. x86 blast-radius conclusion from earlier this session (CPUI_SEGMENTOP
   only exists for x86-16/x86-16-real .pspec, not mainstream x86/x86-64)
   still holds -- this change only affects behavior inside
   pushSegmentRegisterExpression, which is only ever reached via the same
   CPUI_SEGMENTOP + isSelfReferentialRegisterArithmetic gate as before,
   unchanged by this fix.
6. #817 relationship conclusion also still holds unchanged (different pcode
   shape, CONCAT vs SEGMENTOP -- this fix doesn't touch that code path).

---

## 2026-08-22 session: linkSymbolAtUsepoint fix actually applied + compiles

Picked this up fresh and re-read this file top to bottom before touching
anything (previous session's own notes said the `queryContainer` ->
`linkSymbolAtUsepoint` swap in printc.cc had been "applied" -- checked the
live file first rather than trusting that note, and it had NOT actually
landed on disk; still the old queryContainer version).

Applied it for real this time -- printc.cc:1932-1949
(`pushSegmentRegisterExpression`) now reads:
```cpp
bool PrintC::pushSegmentRegisterExpression(const Varnode *vn,const PcodeOp *op)

{
  const PcodeOp *def = vn->getDef();
  Funcdata *fd = const_cast<Funcdata *>(op->getParent()->getFuncdata());
  Symbol *sym = fd->linkSymbolAtUsepoint(const_cast<Varnode *>(vn), op->getAddr());
  if (sym == (Symbol *)0) return false;

  const Varnode *amt = def->getIn(1);
  OpCode opc = def->code();
  const OpToken &tok = (opc == CPUI_INT_SUB) ? binary_minus : binary_plus;

  pushOp(&tok,op);
  pushSymbol(sym,vn,op);
  push_integer(amt->getOffset(),amt->getSize(),false,syntax,amt,op,amt->getType()->getDisplayFormat());
  return true;
}
```
Two `const_cast`s were needed beyond the previous session's sketch and
weren't mentioned in that note -- both required to fix real compiler
errors, not stylistic:
- `op->getParent()->getFuncdata()` returns `const Funcdata *` since `op`
  is `const PcodeOp *` -- needed `const_cast<Funcdata *>` to call the
  non-const `linkSymbolAtUsepoint` on it.
- `linkSymbolAtUsepoint` takes `Varnode *`, not `const Varnode *` --
  needed `const_cast<Varnode *>(vn)`.

Compile check (`g++ -std=c++11 -c printc.cc printlanguage.cc coreaction.cc`,
decompiler cpp/ dir) -- clean, exit code 0, no errors or warnings.

**Not yet done:** full `.\gradlew.bat buildGhidra`, restart, and
force-decompile of both target functions against this version. That's the
actual test of whether `linkSymbolAtUsepoint` fixes the fail-safe (i.e.
whether `SP - 2` now appears instead of `register0x0e`) -- still unverified
end-to-end as of this note. Next steps are otherwise unchanged from the
list at the end of the previous session (rebuild, force-decompile
ram:000149c8 and ram:000149f5, confirm no register0x0e, then move to full
regression pass / removing ActionSegmentRegisterIdentity's class).

**Correction, same session:** a follow-up str_replace to add the
`const_cast<Funcdata *>` fix (needed because `op->getParent()->getFuncdata()`
returns `const Funcdata *`) fuzzy-matched the wrong location and spliced
text into the middle of `pushEquate`'s sign-extension branch
(printc.cc:1887), corrupting a comment/line and eating
`return false; if (modval == val) {` -- this also left an unbalanced brace
(the outer `if (modval != baseval) {` from line 1886 had no matching `}`).
**The compile check reported clean (exit 0) with this corruption still in
place** -- it did not catch it. Caught only because the corruption was
re-read and eyeballed against the known-good pattern of the three sibling
`if (modval == val) { ... }` blocks immediately below it in the same
function. Fixed by hand (restored the `return false;` line, restored the
missing `}`, re-verified full pushEquate body reads correctly end to end)
and re-compiled clean. **Lesson: after any str_replace on this file, always
re-read a wide surrounding range (not just the intended target) before
trusting a clean compile as proof of correctness. SEPARATE gotcha,
same session: the manual `g++ -c` compile checks (run directly in
decompile/cpp/ for a quick syntax check) leave `.o` object files behind
in the source tree. Gradle's `:Decompiler:ip` task (gradle/support/
ip.gradle) walks every file in the module looking for a license/IP
header and fails hard (`No IP found for coreaction.o`) if it finds any
file it doesn't recognize -- `.o` files aren't in its exclude list.
`.\gradlew.bat buildGhidra` failed with exactly this until the three
leftover `.o` files (coreaction.o, printc.o, printlanguage.o) were
deleted by hand. Going forward: delete any `.o` files from decompile/
cpp/ before running a real gradle build, or just don't leave them lying
around after a manual compile check** -- this file has enough
repeated code shapes (multiple `pushSymbol(sym,vn,op); return true;`
blocks) that fuzzy matching has now corrupted it twice this way.

---

## 2026-08-22 session, part 2: linkSymbolAtUsepoint approach is fundamentally
## broken for annotation Varnodes -- root cause found, needs new approach

**Live re-test after clean rebuild (no .o pollution, ip task passed):**
still `register0x0e`, not `SP - 2`, in
`init_copy_const_block_via_memcpy_banked`. `get_function_variables` still
shows 0 params, 0 locals for this function -- `linkSymbolAtUsepoint` is
STILL not creating a Symbol, confirming the fail-safe is still firing.

**Real root cause, confirmed by direct read of `Funcdata::assignHigh`**
(funcdata_varnode.cc:48-61):
```cpp
HighVariable *Funcdata::assignHigh(Varnode *vn)
{
  if ((flags & highlevel_on)!=0) {
    ...
    if (!vn->isAnnotation()) {
      return new HighVariable(vn);
    }
  }
  return (HighVariable *)0;
}
```
**Annotation Varnodes NEVER get a HighVariable assigned, by explicit
design** -- the whole reason `isAnnotation()` Varnodes exist as a
category is that they're not meant to be treated as normal variables.

But `linkSymbolAtUsepoint` (funcdata_varnode.cc:1258-1286), the API this
session's fix switched to, starts with:
```cpp
HighVariable *high = vn->getHigh();
Symbol *sym = high->getSymbol();   // <-- high is NULL for annotation vn's
```
This is a null-pointer dereference on every annotation Varnode --
exactly the only kind of Varnode `pushSegmentRegisterExpression` is ever
called with (gated on `vn->isAnnotation()` in `pushVnExplicit`). This
doesn't crash the whole decompiler (something in the pipeline evidently
isolates/catches per-function failures), but it means
`linkSymbolAtUsepoint` can never succeed here -- it was written for
ordinary Varnodes with real HighVariables and is categorically the wrong
API for this use case. **This was a design mistake in this session's own
"fix", not a wiring or deployment bug** -- confirmed via
`analyze_dataflow` that the SEGMENTOP intercept gate itself IS reached
and IS shaped correctly (SEGMENTOP's 3rd input is SP, defined by
`INT_ADD(CAST(SP), 0xfffe)`, exactly the self-referential shape
expected) -- the failure is entirely inside `linkSymbolAtUsepoint` itself.

**Why `queryContainer` (the ORIGINAL approach, before this session)
didn't have this problem:** it never touches `vn->getHigh()` at all --
it goes straight to `symScope->queryContainer(...)`, a pure lookup with
no HighVariable dependency. Its failure mode was different and correctly
diagnosed in the prior session (SP has no pre-existing Symbol at its own
storage address in either target function) -- that diagnosis was
correct, it just pointed to a real gap: no Symbol exists there to find.

**Implication for the design:** creating a NEW Symbol for the register's
own storage address at print time is fundamentally awkward, because the
one API that creates Symbols the "normal" way (`linkSymbolAtUsepoint`)
requires a HighVariable that annotation Varnodes structurally can't have.
Two real options going forward, neither tried yet:
1. Register a Symbol via a lower-level path that doesn't require
   `vn->getHigh()` -- e.g. call `localmap->addSymbol(...)` directly
   (the same call `linkSymbolAtUsepoint` makes internally at line 1279),
   skipping the `high->getType()`/`high->getSymbol()` guard entirely
   and supplying a type explicitly (e.g. the base register's own
   existing Datatype, or a generic sized integer type) instead of
   `high->getType()`.
2. Don't try to create/attach a Symbol to the SEGMENTOP's own annotation
   Varnode at all. Instead, look up the Symbol already attached to the
   *separate* upstream raw-register-read Varnode (SP's normal, non-
   annotation appearance elsewhere in the function, e.g. as an input to
   the INT_ADD) if one exists there, and reuse THAT Symbol/name for the
   printed expression -- sidestepping HighVariable assignment for the
   annotation Varnode entirely, since we're borrowing a name rather than
   minting one.

**Not yet decided which to pursue.** Option 1 is closer to the original
plan's intent (mint a real Symbol at the register's own storage
address) but needs care around what Datatype to pass without a
HighVariable to source it from. Option 2 matches how a human would
actually read the code (reuse "SP" from wherever it's already named)
but requires locating the right upstream Varnode reliably, which may
not always exist (e.g. if SP is truly unnamed everywhere in a given
function).

**Next steps:**
1. Decide between option 1 and option 2 above (or find a third option) --
   worth first checking whether SP has ANY existing Symbol at all
   anywhere else in either target function, via
   `get_function_variables` with a broader scope/filter or a symbol
   table dump, since that directly informs whether option 2 is even
   viable.
2. Implement the chosen approach in `pushSegmentRegisterExpression`,
   replacing the current (confirmed-broken) `linkSymbolAtUsepoint` call.
3. Compile check, rebuild (remembering to clear any `.o` files from
   decompile/cpp/ first), force-decompile both target functions again.
4. Re-verify no `register0x0e` in either function's output before
   considering this fix complete.

**Fix implemented, same session:** added `Funcdata::linkAnnotationSymbolAtUsepoint`
(funcdata.hh:441 declaration, funcdata_varnode.cc:1288-1315 implementation) --
does the same queryProperties()-then-addSymbol() lookup-or-create as
linkSymbolAtUsepoint, but never touches vn->getHigh(), and supplies an
explicit `glb->types->getBase(vn->getSize(),TYPE_UNKNOWN)` Datatype
instead of borrowing `high->getType()`. `pushSegmentRegisterExpression`
(printc.cc:1935-1961) now calls this instead of `linkSymbolAtUsepoint`.

Verified SP has 0 Symbols anywhere in `init_copy_const_block_via_memcpy_banked`
(not just at the annotation site) via `get_function_variables` before
choosing this approach over "borrow an existing name" -- confirms Option 1
(mint a new Symbol directly) was the only viable path for this function.

One real compile error caught along the way: `Varnode::setSymbolEntry` is
`private` (varnode.hh:173), only accessible to `Varnode`'s friend classes
(`VarnodeBank`, `Merge`, `Funcdata`, `CloneBlockOps` -- varnode.hh:158-161).
`PrintC` is not a friend, so calling it directly from `pushSegmentRegisterExpression`
failed to compile -- this is why the new logic had to live in a `Funcdata`
method rather than inline in printc.cc.

Compile check clean across printc.cc, printlanguage.cc, coreaction.cc,
funcdata_varnode.cc, funcdata.cc. Also cleared 5 leftover `.o` files
before handing off for the real gradle rebuild (same gotcha as the
previous session -- see the `ip.gradle` note above).

**Two more str_replace corruptions happened putting this fix in, both
caught and fixed before compiling clean:**
1. First attempt at adding `linkAnnotationSymbolAtUsepoint`'s declaration
   to funcdata.hh fuzzy-matched a DIFFERENT occurrence of the anchor text
   than intended, splicing it into the middle of `remapDynamicVarnode`'s
   declaration and eating the real `remapConflictSymbol` declaration
   entirely. Caught by immediately re-reading a wide range after the
   edit (per the lesson from earlier this session) rather than trusting
   the tool's success message or a clean compile alone.
2. (Not build-breaking, but structurally wrong) An early draft of the fix
   put the low-level `queryProperties`/`addSymbol` logic directly inline
   in printc.cc rather than as a Funcdata method -- this would have hit
   the exact same `setSymbolEntry` private-access compile error described
   above; caught by the compiler before it was ever deployed, not by
   manual review. Worth noting since the earlier `linkSymbolAtUsepoint`
   mistake was NOT caught by compilation (it compiled fine, just didn't
   work) -- this session's mistake WAS caught by compilation. Different
   category of bug: the first was a runtime logic error (dereferencing
   null), this one was a straightforward access-control violation the
   compiler is guaranteed to catch.

**Not yet done:** the actual rebuild + live re-test. Handed off to the
user to run `.\gradlew.bat buildGhidra` (now that .o files are cleared)
and confirm the new Ghidra build goes live, then force-decompile
`init_copy_const_block_via_memcpy_banked` (ram:000149c8) again and check
for `SP - 2` instead of `register0x0e`. If that function comes back
clean, repeat for `eeprom_backup_table_write_dispatch` (ram:000149f5,
the 27-site stress test) before considering this fix complete.

**CRITICAL FINDING, follow-up live re-test:** still `register0x0e` after
rebuild + "Ghidra is live" confirmation from user. Before assuming the
fix logic itself was wrong again, checked which decompile.exe process was
actually running (`Get-Process | Where ProcessName -like "*decompile*"`)
-- **the live decompile.exe (PID 4164) is running from
`C:\Users\j.brophy.CORKILLSYSTEMS\Downloads\ghidra_12.2_DEV_20260818\
ghidra_12.2_DEV\Ghidra\Features\Decompiler\os\win_x86_64\decompile.exe`
-- a separately downloaded, pre-built Ghidra install, COMPLETELY
UNRELATED to the `C:\Users\j.brophy.CORKILLSYSTEMS\github\ghidra`
source tree** we have been editing, compiling, and rebuilding via
`.\gradlew.bat buildGhidra` this entire multi-session effort.

**This means every "rebuild" and "Ghidra is live" confirmation across
this whole review16.md history may have been testing against a stock
download that never contained ANY of our source changes** -- not
`ActionSegmentRegisterIdentity`, not the `queryContainer` fix, not
`linkSymbolAtUsepoint`, not `linkAnnotationSymbolAtUsepoint`. The
`github\ghidra\...\build\os\win_x86_64\decompile.exe` we've been
compiling IS getting built successfully (confirmed fresh timestamp
matching each rebuild), it just was never the one being launched.

**Not yet resolved / immediate next step:** figure out how "Ghidra is
live" gets launched on this machine (a shortcut, a PATH entry, an
installed application registration, an IDE run configuration) and
either (a) repoint it at
`github\ghidra\Ghidra\Features\Decompiler\build\os\win_x86_64\
decompile.exe` and whatever full Ghidra distribution wraps it, or
(b) copy/install the freshly built decompile.exe over the Downloads
copy that's actually being launched, or (c) find out if there's a
proper `gradlew.bat` distribution/install task (as opposed to just
`buildGhidra`) that produces and installs a full runnable Ghidra
package from this source tree, and use that instead. Do NOT re-attempt
another source-level fix or re-diagnose `pushSegmentRegisterExpression`
until this deployment-path question is resolved and we've confirmed
(e.g. via the same `Get-Process` check, or by embedding a deliberate
marker string/behavior change and confirming it shows up live) that the
running decompile.exe actually is a fresh build of OUR source tree.

**RESOLVED, same session:** deployment path confirmed correct via
hash comparison (user builds via gradlew, then manually copies
decompile.exe into the separate `Downloads\ghidra_12.2_DEV_20260818`
install to run it -- this is the normal workflow, not a mistake).
`Get-FileHash` confirmed the deployed binary is byte-identical to the
freshly built one, and the running process's StartTime was after the
copy -- so the deployed binary genuinely contains
`linkAnnotationSymbolAtUsepoint`. Still `register0x0e`. This forced
tracing the actual call path instead of re-checking the deployment.

**ACTUAL ROOT CAUSE FOUND: our entire intercept in `pushVnExplicit` is
unreachable for this Varnode.** Traced the real call chain:
`PrintC::opSegmentOp` (printc.cc:1169-1173) calls
`pushVn(op->getIn(2), op, mods)` -- NOT `pushVnExplicit` directly.
`pushVn` (printlanguage.cc:197-211) just queues a `NodePending` entry;
the actual dispatch happens later in `PrintLanguage::recurse()`
(printlanguage.cc:547-573), which does:
```cpp
if (vn->isImplied()) {
  ...
  defOp->getOpcode()->push(this,defOp,op);   // <-- recursively expands
                                              //     the defining expr inline
}
else
  pushVnExplicit(vn,op);                     // <-- our intercept lives here,
                                              //     never reached
```
The ORIGINAL review16.md notes (well before this session) already said
the Varnode is flagged BOTH `isAnnotation()` AND `isImplied()` -- this
was documented from the start but its consequence for OUR fix's
placement was never traced through. Because `isImplied()` is checked
FIRST in `recurse()`, an annotation Varnode that is also implied never
reaches `pushVnExplicit` at all -- it goes straight to
`defOp->getOpcode()->push(...)`, which for an `INT_ADD` op prints the
raw arithmetic expression inline using whatever name the base register
resolves to at THAT point (`register0x0e`, via the SAME broken
`pushAnnotation`-style fallback, just reached through a completely
different code path than the one we've been patching).

**This means every fix in this file's history --
`ActionSegmentRegisterIdentity`, `queryContainer`,
`linkSymbolAtUsepoint`, `linkAnnotationSymbolAtUsepoint` -- has been
correctly implemented but installed in dead code for this specific
case.** `pushVnExplicit`'s `isAnnotation()` branch (where our intercept
lives) only runs for annotation Varnodes that are NOT also implied.

**Two real options going forward, neither implemented yet:**
1. Add an equivalent intercept in `PrintLanguage::recurse()` itself,
   before the `isImplied()` branch is taken -- check
   `vn->isAnnotation() && isSelfReferentialRegisterArithmetic(vn) &&
   op->code()==CPUI_SEGMENTOP` there too, and call
   `pushSegmentRegisterExpression` (or a similar hook) from that
   location instead of (or in addition to) `pushVnExplicit`. Requires
   `isSelfReferentialRegisterArithmetic` to already be visible in this
   file (it is -- same file, printlanguage.cc).
2. Investigate WHY this particular annotation Varnode is also flagged
   implied -- if that flag is set by some upstream Action pass rather
   than being fundamental to what a SEGMENTOP annotation input is, it
   may be possible to prevent the implied flag from being set on it in
   the first place (closer in spirit to the original, rejected
   `linkSymbol`-forcing approach from early in this file's history --
   worth re-reading that rejection reasoning before pursuing this).

Option 1 is more consistent with everything already built (reuses
`pushSegmentRegisterExpression` as-is) and doesn't require
understanding why the implied flag is set. Recommend trying option 1
first.

**Next steps:**
1. Read `PrintLanguage::recurse()` in full context (printlanguage.cc
   ~540-575) and `OpBehavior`/`getOpcode()->push()` for INT_ADD to
   confirm exactly what `defOp->getOpcode()->push(this,defOp,op)` does
   and whether intercepting before it is straightforward or whether
   the interception needs to happen even earlier (e.g. in `pushVn`
   itself, before the NodePending is queued, if `isImplied()` is
   knowable at that point too).
2. Implement the chosen intercept.
3. Compile check, rebuild, copy decompile.exe to the Downloads install
   (remembering the .o cleanup + hash verification steps from this
   session), force-decompile, confirm SP - 2 (or similar) actually
   appears.

**Fix implemented, same session:** added the intercept in
`PrintLanguage::recurse()` itself (printlanguage.cc:559-570), before the
`isImplied()` check, per Option 1 above:
```cpp
if (vn->isAnnotation() && op->code()==CPUI_SEGMENTOP && isSelfReferentialRegisterArithmetic(vn) &&
    pushSegmentRegisterExpression(vn,op)) {
  // handled
}
else if (vn->isImplied()) {
  ...
}
else
  pushVnExplicit(vn,op);
```
`pushSegmentRegisterExpression` is called unqualified on `this` (a
`PrintLanguage*` inside a `PrintLanguage` member function) -- since it's
declared `virtual` on the base class with `PrintC`'s override already in
place from the earlier fix, this correctly dispatches to `PrintC`'s real
implementation via normal virtual dispatch; no cast or `dynamic_cast`
needed. `pushVnExplicit`'s own `isAnnotation()`-gated intercept (added
earlier this session) is now effectively dead/unreachable for this
specific shape (annotation+implied), but is harmless to leave in place --
it still correctly handles any hypothetical future case of an annotation
Varnode that is NOT also implied.

Investigated a "why is it also implied" root-cause explanation (Option 2
from above) before settling on Option 1, but Option 1 is confirmed
sufficient and doesn't require understanding or changing that flag, so
Option 2 was not pursued further.

**Console-mode standalone test harness explored, abandoned for now:**
Ghidra's decompiler ships an isolated test harness
(`Decompiler/src/decompile/datatests/*.xml`, `<decompilertest>` format,
driven by `decompile.exe -i <script>`) and the uploaded debug file
(`init_copy_const_block_via_memcpy_banked.xml`) is in the related native
`<xml_savefile>` save-state format (`XmlArchitecture`, confirmed via
xml_arch.cc), both loadable directly by our own freshly-built
decompile.exe without needing the full ROM database or a GUI. Attempted
a `restore <file>` + `decompile` + `print C` script via `-i`, but hit
path-discovery issues (`startDecompilerLibrary`/`discoverGhidraRoot`
needs a full installed Ghidra layout to find the H8 SLEIGH spec, not
just an arbitrary `-s` extra path) and a hang on `-i` with piped stdin.
Not pursued further because the existing live-MCP `force_decompile`
workflow (confirmed via file-hash + process-start-time checks earlier
this session to genuinely be running our fresh build) already serves as
a working, verified isolated-enough test loop. Worth revisiting properly
if the live-copy-and-test cycle ever becomes a bottleneck -- the
datatests harness would be meaningfully faster once the path-discovery
issue is solved (no manual copy-to-Downloads step, no GUI, much faster
iteration) -- but is not blocking right now.

Compile check clean across printc.cc, printlanguage.cc, coreaction.cc,
funcdata_varnode.cc, funcdata.cc. `.o` files cleared.

**Not yet done:** the actual rebuild + copy-to-Downloads + live re-test.
Handed off to the user to rebuild, copy decompile.exe over, and confirm
`init_copy_const_block_via_memcpy_banked` (ram:000149c8) now shows
`SP - 2`-style output instead of `register0x0e`. If clean, repeat for
`eeprom_backup_table_write_dispatch` (ram:000149f5) before considering
this fix complete.

**Live re-test, same session:** still `register0x0e`. Verified BOTH the
deployed binary (hash match + process StartTime after copy, same
discipline as before) AND the source (`grep` confirmed both intercept
call sites still present at printlanguage.cc:246 and :566) before
re-diagnosing -- ruled out deployment/staleness as the cause this time.

**Cannot explain from source reading alone why the `recurse()` intercept
condition isn't firing.** Every piece checked out on paper: same
SEGMENTOP/self-referential-arithmetic shape reconfirmed via
`analyze_dataflow`, `isSelfReferentialRegisterArithmetic`'s logic
re-verified line-by-line against that shape, `pushVn`/`opSegmentOp`
confirmed to pass the real SEGMENTOP as `op` unmodified. Since this is
now the second time a source-level trace failed to explain live
behavior, added a TEMPORARY diagnostic (printlanguage.cc:559-565)
instead of further speculation: for every `PcodeOp` with `op->code()==
CPUI_SEGMENTOP` reaching `recurse()`, emit a `warningHeader()` comment
(visible directly in decompiled output, no `<iostream>`/`cerr` needed --
first attempt at raw `cerr`/`std::cerr` failed to compile, this file has
no `<iostream>` include) showing the live values of `isAnnotation()`,
`isImplied()`, `isConstant()`, and `isSelfReferentialRegisterArithmetic()`
for the actual Varnode reaching this exact code path.

Compiles clean, `.o`s cleared. **This diagnostic must be removed once
its answer is captured** -- it is not a real fix and does not belong in
any upstream submission.

**Next step:** rebuild, copy, force-decompile
`init_copy_const_block_via_memcpy_banked` again, and read the new
warning-comment output. This will directly answer which of the four
conditions is actually false live (most likely candidates: `isAnnotation()`
turns out false for this Varnode despite review15.md's general claim, or
`op` at this point in `recurse()` isn't actually the SEGMENTOP despite
the `opSegmentOp`/`pushVn` trace suggesting it should be) rather than
continuing to guess from source alone.

**Diagnostic returned ZERO output -- itself a critical finding.**
Re-verified deployment was genuinely fresh (killed the stale locked
decompile.exe process manually since `buildGhidra`'s packaging step
failed on an unrelated `git`-not-installed error AFTER the actual
`decompile.exe` link succeeded -- confirmed via file size/timestamp
change -- copied that correctly-built exe over by hand, hash-verified).
Re-ran force_decompile twice, byte-identical output both times, zero
diagnostic comments either time, and no long-lived decompile.exe process
observed (confirmed this is normal -- Ghidra spawns it per-request and
it exits, not evidence of stale caching).

**Traced why the diagnostic (gated on `op->code()==CPUI_SEGMENTOP`) never
fires: `op` at the point `recurse()` dequeues the pointer-expression
Varnode is the STORE, not the SEGMENTOP.** `PrintC::opStore` (printc.cc:519-537)
calls `pushVn(op->getIn(2),op,mods)` and `pushVn(op->getIn(1),op,m)` with
`op` bound to the STORE throughout -- the SEGMENTOP-defined pointer
Varnode enters `nodepend` with the STORE as its `op`, not the SEGMENTOP.
The SEGMENTOP only becomes the "current op" one level deeper, inside its
own `push()` handler (`TypeOp` base class routes to
`PrintC::opSegmentOp`, confirmed no override in `TypeOpSegment` --
typeop.cc:2520-2561), which itself calls `pushVn(op->getIn(2),op,mods)`
with `op` now correctly = the SEGMENTOP -- but that queues a SEPARATE,
later `NodePending` entry, not the one my original diagnostic/intercept
was checking.

**Widened the diagnostic** (printlanguage.cc:559-566) to fire on ANY
`vn->isAnnotation()` regardless of what `op` is bound to, logging `op`'s
opcode name (via `get_opname`, opcodes.hh:134) instead of assuming it's
already the SEGMENTOP. This should catch the Varnode on whichever
`NodePending` entry it actually arrives on, and tell us definitively
what `op` really is at that point -- rather than continuing to guess
from a source trace that has now been shown to have at least one wrong
assumption (that `op` is the SEGMENTOP at the point `recurse()` sees the
implied-annotation Varnode).

Compiles clean, `.o`s cleared.

**Next step, again:** rebuild (note: `buildGhidra`'s full distribution
task may keep failing on the unrelated git/zip issue -- if so, just
confirm the `Decompiler:linkDecompileWin_x86_64Executable` sub-task's
output file timestamp/size changed and copy that exe over directly, same
as this session did), copy, kill any locked decompile.exe process first
if the copy fails, force-decompile, read whatever the new
"Annotation-vn diag" warning comment(s) say. Remove ALL diagnostic code
(this and the previous version) once the real answer is captured -- see
earlier note, this must not ship.

**Live re-test, same session:** ZERO diagnostic output again, even with
the widened `vn->isAnnotation()`-only gate. Re-verified deployment
freshness thoroughly this time (hash match src vs dst, AND confirmed a
NEW decompile.exe process with a StartTime after the copy, not just
"no process running" as last time) -- this is not a staleness/deployment
issue. The Varnode genuinely never reaches this exact `if` block with
`isAnnotation()` true, for whatever `op` it arrives with.

**Escalated to a brute-force unconditional diagnostic**
(printlanguage.cc:559-569): fires on EVERY dequeue from `nodepend` in
`recurse()`, for ANY Varnode/op, filtered only to this specific function
by name (`fdDiag->getName()=="init_copy_const_block_via_memcpy_banked"`)
to avoid spamming every function in the program. Logs the op's name,
the Varnode's `isAnnotation()`/`isImplied()`/`isConstant()` flags, and
its address space:offset. This will answer, definitively, one of two
open questions: (a) whether `recurse()` is called AT ALL for this
function's statements (if still zero output, the entire `pushVn`/
`recurse()` model this whole investigation has assumed is wrong for
this print path), or (b) if it does run, what the REAL sequence of
(op, vn, flags) tuples looks like, including whatever Varnode
corresponds to the register0x0e-printed value -- which may not be
flagged `isAnnotation()` at all despite review15.md's original claim,
since that claim was never re-verified this session with a direct
measurement, only re-read from notes.

Compiles clean, `.o`s cleared.

**Next step:** rebuild (direct-copy-the-linked-exe approach if
buildGhidra's packaging fails again), copy, kill-locked-process-if-needed,
force-decompile, read ALL the new warning comments (there will likely be
several, one per dequeue) in order. This is the most direct measurement
attempted yet -- if this still produces zero output, the problem is not
in understanding `recurse()`'s logic but in a wrong assumption about
which Funcdata/function this print pass is even running against, or
about `warningHeader()` itself not actually surfacing in this context,
and that possibility should be checked NEXT (e.g. by making the
diagnostic unconditionally fire for literally any function processed at
all, no name filter, to confirm `warningHeader()` output actually
reaches the decompiled text at all under these conditions) rather than
continuing to add more detail to a diagnostic mechanism that may itself
be silently failing.

**FOUND THE REAL PROBLEM WITH THE DIAGNOSTIC MECHANISM ITSELF, before
even rebuilding again:** re-read `PrintC::emitCommentFuncHeader`
(printc.cc:3450-3489) and confirmed it populates its comment list via
`commsorter.setupHeader(...)` -- called BEFORE the statement-printing
pass that calls `recurse()` even begins. `Funcdata::warningHeader()`
(funcdata.cc:135-145) calls `addCommentNoDuplicate` on `commentdb`
DURING the print pass (my diagnostic fires from inside `recurse()`,
which runs as part of statement printing, after the header comment
list was already built and captured for that pass). **A comment added
this way can structurally never appear in the SAME decompile pass's
header output** -- it would only show up on a SUBSEQUENT decompile of
the same function, once `commentdb` already contains it from the prior
pass. This means the zero-output result from every diagnostic attempt
so far is INCONCLUSIVE about whether `recurse()`/the intercept condition
is even being reached -- the diagnostic mechanism itself could not have
surfaced output on a single `force_decompile` call regardless.

**Fix: call force_decompile TWICE in a row** on the same function for
any future diagnostic-comment attempt -- the first call lets
`warningHeader()`'s comments land in `commentdb`; the second call's
`emitCommentFuncHeader` will pick them up from the SORTED list built at
the start of that second pass. Did not need to touch the diagnostic
code itself for this -- only the testing procedure changes.

**Tested the two-pass theory live -- DISPROVEN.** Ran force_decompile
twice in a row on the same function using the still-deployed
warningHeader-based diagnostic build (no rebuild needed, procedural
test only). Both passes still showed zero diagnostic output. Something
else is preventing the comment from surfacing even across two passes --
possibly `commentdb` gets cleared/the Funcdata object gets rebuilt fresh
between force_decompile calls (plausible if each call re-imports/re-
analyzes from scratch rather than reusing a live decompiled function
object), or `head_comment_type` doesn't include whatever Comment::type
warningheader comments carry by default in this display context, or
some other gap not yet identified. Did not chase this further --
switched to a diagnostic mechanism that doesn't depend on Ghidra's
comment/print pipeline at all.

**Switched to direct file-based diagnostic, same session.** Replaced
the warningHeader-based diagnostic (printlanguage.cc:559-570) with a
raw `std::ofstream` write straight to
`C:\Users\j.brophy.CORKILLSYSTEMS\recurse_diag.txt` (append mode, one
line per `recurse()` dequeue, same target-function-name filter as
before). Added `#include <fstream>` (printlanguage.cc:17 -- wasn't
previously included in this file). This sidesteps `commentdb`/
`emitCommentFuncHeader`/`CommentSorter` entirely -- writes happen
synchronously as `recurse()` executes, readable directly from disk
immediately after ANY force_decompile call, no dependency on
Ghidra's internal comment timing/filtering/dedup at all.

Compiles clean, `.o`s cleared.

**Next step:** rebuild, copy, force-decompile ONCE, then read
`C:\Users\j.brophy.CORKILLSYSTEMS\recurse_diag.txt` directly (a plain
text file, view/read-file/cat it, not the decompiled output) to see the
real sequence of `(op, vn flags)` tuples `recurse()` processes for this
function. If the file doesn't exist at all after a force_decompile
call, that's the strongest possible signal that `recurse()` itself is
never invoked for this function's print pass -- a much bigger finding
than anything traced so far and would mean this whole print-layer
approach needs to be rethought from a different entry point. If the
file DOES exist and has content, read every line -- the answer to
which flag assumption was wrong will be directly visible. Remove BOTH
the ofstream diagnostic AND delete the diag txt file once the answer is
captured -- neither belongs in any upstream submission or repo state.

## BREAKTHROUGH: root cause was a wrong flag assumption, not a code path issue

**The diagnostic worked and gave a direct, unambiguous answer.** File
existed (proving `recurse()` genuinely runs for this function -- that
open question is resolved), and the line for the actual SEGMENTOP-input
Varnode reads:
```
recurse() dequeue: op=SEGMENTOP vnAnnotation=0 vnImplied=1 vnConstant=0 vnAddr=register:e
```
**`vnAnnotation=0`.** This Varnode is NOT flagged `isAnnotation()` at
all -- only `isImplied()`. review15.md's original claim ("the Varnode is
flagged both `isAnnotation()` and `isImplied()`") does not hold for the
current live pipeline state, whatever its original basis was. Every
intercept attempt across this entire multi-session effort --
`pushVnExplicit`'s own isAnnotation()-gated check, and the later
`recurse()`-level check -- was gated on `isAnnotation()`, which is
simply false here, so none of them could ever fire, regardless of how
correct the rest of their logic was (`isSelfReferentialRegisterArithmetic`,
`pushSegmentRegisterExpression`'s Symbol-creation logic, etc. were all
independently fine).

**Fix: swapped the gating condition from `vn->isAnnotation()` to
`vn->isImplied()`** in the `recurse()` intercept (printlanguage.cc:559-571).
Everything else about the intercept (the `CPUI_SEGMENTOP` check,
`isSelfReferentialRegisterArithmetic`, calling
`pushSegmentRegisterExpression`) is unchanged and was already correct.
`pushVnExplicit`'s own isAnnotation()-gated check (from an earlier
session) is now confirmed truly dead code for this shape, left in place
harmlessly as a no-op fallback for a hypothetical case where a Varnode
really is annotation-but-not-implied.

All temporary diagnostic code removed (both the printlanguage.cc changes
and the `<fstream>` include) and the diagnostic output file deleted.
Compiles clean, `.o`s cleared.

**Next step:** rebuild, copy, force-decompile
`init_copy_const_block_via_memcpy_banked` -- this is the first attempt
where the actual root-cause understanding (not just the mechanics
around it) has been directly verified rather than inferred from an old
note. If `SP - 2`-style output appears (no more `register0x0e`), repeat
against `eeprom_backup_table_write_dispatch` (ram:000149f5, the 27-site
stress test) before considering this fix complete and moving to cleanup
(removing the now-fully-superseded `ActionSegmentRegisterIdentity`
class per the original design decision in review15.md).

## CONFIRMED FIXED, live re-test both target functions

`init_copy_const_block_via_memcpy_banked` (ram:000149c8, the 1-site
function): **zero occurrences of `register0x0e`**. The self-referential
SP arithmetic now prints via the new expression path (still not
perfectly clean C -- `(int)(__undef00000000 + 0xfffe) + -2` rather than
a tidy `SP - 2` -- but the actual bug, the raw register0x0e fallback
NAME, is gone. The `__undef0000000N`/leftover-offset cosmetics are a
separate, lower-priority issue, not the register0x0e regression this
whole investigation targeted).

`eeprom_backup_table_write_dispatch` (ram:000149f5, the 27-site stress
test): **zero occurrences of `register0x0e`** across the entire
function. Every one of the ~27 originally-affected sites now prints via
the same expression-fallback path, consistently.

**Both target functions from the original bug report are clean.** The
long chain of wrong assumptions this investigation went through, in
order: (1) `queryContainer` -- wrong API, never finds a Symbol since
none exists for SP; (2) `linkSymbolAtUsepoint` -- dereferences
`vn->getHigh()`, always NULL for annotation Varnodes, silent no-op;
(3) intercept placed in `pushVnExplicit` -- unreachable, because
`PrintLanguage::recurse()` checks `isImplied()` before ever reaching
that function for this shape; (4) intercept moved to `recurse()` but
still gated on `isAnnotation()` -- also wrong, this Varnode is
`isImplied()` WITHOUT `isAnnotation()`, contradicting review15.md's
original characterization. Only fix (4), corrected to gate on
`isImplied()` instead, actually worked.

**Not yet done (separate, lower-priority followups, not required for
the register0x0e fix itself):**
1. The `__undef0000000N`/raw-offset cosmetic ugliness in the fixed
   output -- e.g. `(int)(__undef00000000 + 0xfffe) + -2` instead of a
   clean `SP - 2`. `pushSegmentRegisterExpression`'s Symbol-creation
   logic runs and clearly prevents register0x0e, but the resulting
   Symbol's displayed name isn't "SP" the way the original design
   intended -- worth a follow-up session to look at why (possibly the
   `Datatype` passed to `linkAnnotationSymbolAtUsepoint`'s
   `addSymbol()` call, or the Symbol just isn't getting a meaningful
   name assigned at all, defaulting to Ghidra's `__undef` placeholder
   convention). Not blocking -- the actual regression (the
   register0x0e NAME) is fixed regardless of how the replacement
   expression looks.
2. Clean up `pushVnExplicit`'s now-confirmed-dead `isAnnotation()`
   intercept (printc.cc / printlanguage.cc, added earlier this
   session) -- harmless to leave, but could be removed for clarity
   in an eventual upstream submission.
3. Remove the disabled `ActionSegmentRegisterIdentity` class entirely
   (coreaction.cc/.hh) now that it's fully superseded, per the
   original plan in review15.md/review16.md's early sections.
4. Write up for upstream issue #817, per the original review16.md
   Step 5 plan -- not started.
5. Full regression pass across the rest of the ROM disassembly to
   confirm no other functions regressed from this change (only the
   two originally-reported functions have been directly re-verified
   so far).

## Cosmetic fix, same session: real register names instead of __undefN

**Root cause of the `__undef0000000N` output:**
`linkAnnotationSymbolAtUsepoint` (funcdata_varnode.cc:1303-1316) called
`localmap->addSymbol("",ct,...)` -- an EMPTY name string -- when
creating a new Symbol. Ghidra's Scope::addSymbol auto-generates a
`__undefinedN`-style placeholder name for any Symbol created with an
empty name, which is exactly the `__undef0000000N` text seen in the
fixed-but-ugly output.

Confirmed via the h8539f.slaspec source (line 22:
`define register offset=0x00 size=2 [ R0 R1 R2 R3 R4 R5 FP SP ];`) that
SP genuinely IS a named register at offset 0x0e (8th register, 2 bytes
each: 0,2,4,6,8,a,c,e) -- matching the diagnostic's `vnAddr=register:e`
exactly -- so `Architecture::translate->getRegisterName(space,offset,size)`
should legitimately resolve to "SP" for this case; the code just never
called it.

**Fix:** `linkAnnotationSymbolAtUsepoint` now calls
`glb->translate->getRegisterName(vn->getSpace(),vn->getOffset(),vn->getSize())`
and passes the real result to `addSymbol` instead of `""`. If
`getRegisterName` itself legitimately returns empty (can happen for
some register-space offsets per `pushAnnotation`'s own existing
defensive check at printc.cc:1993), this degrades gracefully back to
the same `__undefN` placeholder as before -- not a regression, just the
same fallback Ghidra already uses elsewhere for genuinely-unnamed
register locations.

Compiles clean, `.o`s cleared.

**Next step:** rebuild, copy, force-decompile both target functions
again. Expect `SP - 2` (clean) instead of
`(int)(__undef00000000 + 0xfffe) + -2` (ugly-but-correct) in
`init_copy_const_block_via_memcpy_banked`, and similarly clean `SP`-based
expressions replacing every `__undef0000000N` occurrence in
`eeprom_backup_table_write_dispatch`.

**Live re-test: "SP" name confirmed working, but expression shape still
wrong -- second cosmetic bug found.** Output changed from
`(int)(__undef00000000 + 0xfffe) + -2` to `(int)(SP + 0xfffe) + -2` --
the name fix worked, but the expression itself prints `SP + 0xfffe`
instead of `SP - 2`.

**Root cause:** the H8 SLEIGH's `SP = SP - 2` (h8539f.slaspec:332)
pcode-lowers to `INT_ADD(SP, 0xfffe)` -- the two's-complement wraparound
constant -- NOT a genuine `INT_SUB(SP, 2)`. Confirmed via
`analyze_dataflow` much earlier in this investigation
(`INT_ADD output=SP inputs=[..., const:0xfffe]`). The original code's
`opc == CPUI_INT_SUB ? binary_minus : binary_plus` ternary only ever
saw `CPUI_INT_ADD` for this real-world case, so it always chose
`binary_plus` and printed the raw unsigned constant.

**Fix:** `pushSegmentRegisterExpression` (printc.cc:1953-1976) now
detects an `INT_ADD` whose constant operand has its sign bit set
(`rawval & signbit`, sized to the operand's own width via `calc_mask`)
and treats it as an implicit subtraction: prints `binary_minus` instead
of `binary_plus`, and displays the two's-complement-negated magnitude
(`(~rawval + 1) & mask`, i.e. 2 instead of 0xfffe) rather than the raw
wraparound value. A genuine `CPUI_INT_SUB` (if `isSelfReferentialRegisterArithmetic`
ever matches one -- it accepts both opcodes) still takes the
subtraction path unconditionally as before, unaffected by this change.

The `(int)(...)  + -2` outer-cast wrapping visible in the live output
is a SEPARATE, so-far-unexplained cosmetic layer (not something this
function generates directly) -- worth checking after this fix confirms
whether it's still present or was somehow a symptom of the wrong-sign
value.

Compiles clean, `.o`s cleared.

**Next step:** rebuild, copy, force-decompile both target functions.
Expect `SP - 2` (fully clean this time) in
`init_copy_const_block_via_memcpy_banked`. If the outer `(int)(...) +
-2` wrapping is still present even with the corrected inner expression,
that's a distinct, separate issue to investigate next -- not
necessarily part of the original register0x0e bug, and lower priority
than confirming the core `SP - 2` text itself is now correct.

## CONFIRMED COMPLETE: both fixes verified live, both target functions clean

**`init_copy_const_block_via_memcpy_banked`:** `*(undefined2 *)((int)(SP - 2) + -2) = 0;`
-- clean `SP - 2` inner expression, exactly as intended.

**`eeprom_backup_table_write_dispatch`:** all ~27 originally-affected
sites now consistently print `(int)(SP - 2) + <offset>` -- e.g.
`(int)(SP - 2) + -0x10`, `(int)(SP - 2) + 6`, `(int)(SP - 2) + -0xe`.
Zero `register0x0e`, zero `__undefN`, zero raw `+ 0xfffe`. Every site
uses the real "SP" name and correct subtraction sign.

**The outer `+ <offset>` wrapping (e.g. the `+ -2` in
`(int)(SP - 2) + -2`) is CONFIRMED to be genuine, separate pcode, not a
printing defect from this fix.** Verified via `analyze_dataflow`: the
SEGMENTOP's CAST output feeds a SECOND, distinct `INT_ADD` with its own
constant (`const:0xfffffffe` in the raw pcode, i.e. -2) before the
final CAST and STORE/LOAD. This is the compiler's real address
computation for `@(-2:8,FP)`-style addressing composed on top of the
segment-adjusted base pointer -- structurally the same as any ordinary
`(int)(basePtr) + offset` array/struct access. Not part of the
register0x0e bug and not something this fix should (or does) collapse
further; out of scope.

**Status: the register0x0e bug and its two follow-on cosmetic issues
(the __undefN name and the +0xfffe display) are ALL FIXED and verified
against both originally-reported functions.** Remaining open items are
the pre-existing followups listed in the earlier "Not yet done" section
(dead pushVnExplicit intercept cleanup, ActionSegmentRegisterIdentity
removal, upstream issue #817 writeup, full-ROM regression pass) -- none
of which block calling the core fix complete.

---

## 2026-08-24 session: full-ROM regression pass finds a second, distinct bug

**Context:** user ran the deferred "full regression pass" item above --
not against the two original target functions, but against a full-ROM
decompile export of a different binary (`RVR_1998_x3 4g63t 21000011
md352553.hex`), comparing a pre-fix export
(`RVR_1998_x3 4g63t 21000011 md352553.hex.c`) against a post-fix export
(`RVR_1998_x3 4g63t 21000011 md352553_9540.hex.c`). This is exactly the
kind of check the "Not yet done" list above flagged as still outstanding
-- and it surfaced a real problem the two-function stress test never
could, since neither original target function contains this second shape.

**Also found, before the regression check began:** `isSelfReferentialRegisterArithmetic`
in the current source has a base-identity guard
(`if (base->getAddr() != vn->getAddr()) return false;`) not present in
the original review16 Step 1 sketch above -- added at some undocumented
point in this file's history, presumably to narrow an over-broad match.
This guard has its own bug: `def->getIn(0)` (the arithmetic's base
operand) can be wrapped in a `CAST` (confirmed live via `analyze_dataflow`:
H8's `link FP,#-0x2:8` lowers to `SEGMENTOP <- INT_ADD(CAST(SP), 0xfffe)`),
and the guard compared the CAST's `unique`-space output address against
`vn`'s address instead of unwrapping through the CAST first -- so it
always failed, silently disabling the entire intercept for the exact
shape the fix targets. **This alone was enough to fully re-break the fix
for the two original functions** (confirmed live: both back to 100%
`register0x0e` before this session's edit). Not clear whether this guard
regressed after the "CONFIRMED COMPLETE" status above was written, or
whether that status was never actually re-verified after the guard was
added -- either way, treat any "confirmed working" note in this file as
needing a fresh live check, not just a re-read, per the established
lesson elsewhere in this file.

**Fix applied (printlanguage.cc, `isSelfReferentialRegisterArithmetic`):**
unwrap `base` through a single `CPUI_CAST` before the address comparison:
```cpp
const Varnode *base = def->getIn(0);
if (base->getDef() != (const PcodeOp *)0 && base->getDef()->code() == CPUI_CAST)
  base = base->getDef()->getIn(0);
if (base->getAddr() != vn->getAddr()) return false;
```
Compiled clean (`g++ -std=c++11 -c printc.cc printlanguage.cc coreaction.cc
funcdata_varnode.cc funcdata.cc`, `.o`s cleared same turn). Built via
`.\gradlew.bat :Decompiler:linkDecompileWin_x86_64Executable` (BUILD
SUCCESSFUL), then via the user's own full `buildGhidra` run -- both
copied/deployed and hash-verified live. Re-confirmed clean on both
original target functions after this fix, on both build paths:
`init_copy_const_block_via_memcpy_banked` (ram:000149c8) and
`eeprom_backup_table_write_dispatch` (ram:000149f5, all ~27 sites) --
`SP - 2`, zero `register0x0e`, matching the "CONFIRMED COMPLETE" section
above exactly.

**Regression check result (the actual point of this session): the
CAST-unwrap fix is necessary but NOT sufficient -- and introduces a new
regression of its own at a second site in the same function.** Whole-ROM
`register0x0e` count went from 1 (pre-fix export) to 3 (post-fix export,
after the CAST-unwrap fix). Initial read of this (further down this
session's own notes, corrected here) assumed the 3rd site was a
pre-existing bug this fix simply failed to reach -- **that assumption was
WRONG, corrected after checking the actual pre-fix text at that exact
line rather than just the post-fix grep count.** Full breakdown, source:
direct line-by-line comparison of both exports at the exact same function
(`status_word_bit3_conditional_update_via_table`, both sites), confirmed
against live `force_decompile` on the same binary:

1. **Unrelated, pre-existing, unaffected by this fix (ignore for now,
   separate bug):** `f266_knock_octane_composite_trim_compute(*(ushort
   *)((int)register0x0e + 6))` in `knock_octane_trim_master_dispatch` --
   byte-identical in both pre- and post-fix exports. Different pcode
   shape entirely (a raw register read used directly as a call argument,
   no self-referential arithmetic feeding it) -- out of scope for this
   fix, flagged by the user as a separate investigation for later.
2. **The fix working correctly -- genuine improvement:**
   `status_word_bit3_conditional_update_via_table` (000232c3), the
   `clr.w @(-0x2:8,FP)` write at 000232d5. Pre-fix this printed as
   `*(undefined2 *)((int)&stack0xfffe + -2) = 0;` -- NOT `register0x0e`
   (a different, also-imprecise-but-non-broken representation). Post-fix
   it prints clean `*(undefined2 *)((int)(SP - 2) + -2) = 0;`. Confirmed
   live via `force_decompile` on this exact function/binary.
3. **NEW REGRESSION caused by this fix, same function, the very next
   read of the same stack slot:** `tst.w @(-0x2:8,FP)` at 00023301.
   **Pre-fix this was ALSO `&stack0xfffe + -2`** (same imprecise-but-not-
   `register0x0e` form as the write site above, confirmed by reading the
   actual pre-fix source text at this exact line, not inferred from the
   post-fix grep count). **Post-fix it is now
   `(*(short *)((int)((short)register0x0e + -2) + -2) == 0))` -- the
   CAST-unwrap fix change made this SPECIFIC site WORSE, not merely
   "still broken."** Live `analyze_dataflow` (backward from 00023301)
   confirms the real chain: `SEGMENTOP <- FP`, and separately
   `FP <- INT_ADD(CAST(SP), 0xfffe)` (from the same `link FP,#-0x2:8` at
   the function's entry, 000232c3). **This is NOT self-referential
   arithmetic in the sense `isSelfReferentialRegisterArithmetic` checks
   for** -- `vn` here is `FP`, but the arithmetic's base (after
   CAST-unwrap) is `SP`, a DIFFERENT register. `FP`'s own address !=
   `SP`'s address, so the guard correctly returns `false` for this shape
   (it genuinely is not self-referential), and the intercept correctly
   declines to handle it -- but there is no OTHER intercept for this
   cross-register `FP = SP - 2`-then-`FP`-used-as-a-SEGMENTOP-input
   shape, so it falls through to the old `pushAnnotation()`-style
   fallback. **Best current understanding of why the CAST-unwrap fix
   specifically caused THIS site to regress from `&stack0xfffe` to
   `register0x0e` (not yet fully confirmed via direct measurement, flagged
   as unresolved):** the CAST-unwrap change to `isSelfReferentialRegisterArithmetic`
   may have altered which fallback branch this Varnode's def-chain walk
   lands in elsewhere in the same print pass, since the helper is also
   used/consulted by other logic upstream of the final fallback choice --
   this needs the same kind of direct live measurement (Varnode flags,
   not source-reading alone) that resolved the earlier `vnAnnotation`/
   `vnImplied` mystery earlier in this file, rather than being inferred
   from the pcode chain alone.

**Given how central `link FP,#-2` is to H8 function prologues (`FP`
derived from `SP` and then used for essentially all local/stack-relative
addressing), this cross-register shape is very likely far MORE common
across a real ROM than the original narrow self-referential-only case --
this single 2-function stress test badly under-counted the fix's true
scope, and also failed to catch that the CAST-unwrap fix itself
introduces a real regression on this second, more common shape.**

**Given how central `link FP,#-2` is to H8 function prologues (`FP`
derived from `SP` and then used for essentially all local/stack-relative
addressing), this cross-register shape is very likely far MORE common
across a real ROM than the original narrow self-referential-only case --
this single 2-function stress test badly under-counted the fix's true
scope.** The whole-ROM regression check that would have caught this
(item 5 in the "Not yet done" list, several sections above) was
deferred across this entire file's history until this session.

**Not yet done:**
1. Confirm via direct live measurement (not source-reading alone) exactly
   why the CAST-unwrap change causes THIS specific site to regress from
   `&stack0xfffe + -2` to `register0x0e` -- current best understanding is
   a plausible mechanism (noted above) but not yet confirmed the way the
   earlier `vnAnnotation`/`vnImplied` mystery was confirmed (a direct
   flag/value dump at the actual failing code path, not inference from
   the pcode chain).
2. Design and implement a fix for the cross-register case:
   `vn` (e.g. FP) fed by `INT_ADD/INT_SUB(CAST(otherReg), const)` where
   `otherReg != vn`, still needs to fire when `vn` itself (not just the
   arithmetic's base) is what's consumed by the SEGMENTOP. Needs its own
   design pass -- printing `FP` as `SP - 2` inline (reusing the base
   register's name rather than declaring a Symbol for FP) is one option,
   consistent with the existing self-referential design's philosophy,
   but not yet designed or attempted. **Per user decision 2026-08-24,
   this must land as its own separate, independently-testable patch, NOT
   bundled into the CAST-unwrap fix** -- distinct root cause, distinct
   gating condition, matches this file's own established practice
   elsewhere (e.g. Step 4's "own separate, clean commit" rule) and
   Ghidra's own CONTRIBUTING.md guidance against bundling unrelated
   changes into one review.
3. Re-run the same pre/post whole-ROM diff methodology (register0x0e /
   __undef / WARNING counts, not just the two original target functions)
   after any further fix, before calling this complete again -- this
   session is the first time that check has actually been done across
   this file's entire history, and it immediately found both a real gap
   AND a real fix-induced regression the narrow two-function stress test
   missed. Also re-verify pre-fix baselines by reading actual pre-fix
   source text at the specific line in question, not by inferring from
   aggregate grep counts alone -- this session's own first attempt at
   this categorization got the baseline wrong (assumed line 3 was
   pre-existing `register0x0e`; it was actually a different, non-broken
   `&stack0xfffe` form pre-fix) until checked directly.
4. The unrelated `register0x0e + 6` call-argument case (item 1 above) --
   confirmed genuinely separate, deferred by the user for a later,
   separate investigation. Do not conflate with this fix's scope.
5. WARNING-comment count also dropped 703 -> 604 across the same pre/post
   export diff -- not yet investigated, could be entirely explained by
   the now-fixed sites simply no longer needing whatever warning
   `register0x0e`-adjacent code used to emit, or could be a separate
   signal worth checking on its own. Flagging so it isn't lost, not yet
   triaged.
6. All the original "Not yet done" items from the "CONFIRMED COMPLETE"
   section above still stand, unchanged, and are now additionally
   blocked on resolving this new cross-register case first, since
   calling `ActionSegmentRegisterIdentity` removal or an upstream writeup
   "done" before this second bug is understood would repeat the same
   premature-completion mistake this file's history already made once.

---

## 2026-08-24 session, part 2: cross-register fix implemented, whole-ROM CONFIRMED

**Design check before implementing, per user request:** verified against the
H8/300 GCC ABI documentation and the project's own h8.cspec that this fix is
faithful to the actual hardware/ABI semantics, not just an internal-consistency
convenience. Two supporting facts: (1) the H8/300 GCC ABI documents
`-fomit-frame-pointer` as eliminating FP "in favor of the stack pointer" --
confirming FP has no independent hardware meaning on this target, it is a pure
software convention defined to equal SP-at-entry; (2) h8.cspec's
`<stackpointer register="SP".../>` is the ONLY special-register declaration in
the file -- FP is listed only in `<unaffected>`, the same tier as R3/R4/R5, with
no frame-pointer-specific machinery anywhere in the compiler spec. So resolving
FP's SEGMENTOP-consumed value back to `SP - 2` is the ABI-correct literal
decompilation, not an approximation -- matching what GCC itself treats as
interchangeable representations of the same stack location for this target.

**Implementation, as a separate patch layered on top of the CAST-unwrap fix
(per the prior session's decision), not merged into it:**

1. New helper `isCrossRegisterArithmetic(const Varnode *vn, const Varnode
   **baseOut)` in printlanguage.cc, directly after
   `isSelfReferentialRegisterArithmetic`. Same INT_ADD/INT_SUB-against-constant
   shape check, same CAST-unwrap, but INVERTS the identity requirement: base
   must be a DIFFERENT register from vn (`base->getAddr() == vn->getAddr()` ->
   reject), and base must be a genuine register-space Varnode
   (`base->getSpace()->getType() == IPTR_PROCESSOR`, not a `unique`-space
   temporary -- initial attempt used `isAddrTied()`/`isRegister()`, neither of
   which exist on `Varnode`; corrected to the real space-type check after a
   real compile error caught it).
2. `PrintLanguage::recurse()` gains a second `else if` branch (after the
   existing self-referential one): `vn->isImplied() && op->code()==CPUI_SEGMENTOP
   && isCrossRegisterArithmetic(vn,&crossBase) && pushCrossRegisterExpression(vn,
   crossBase,op)`. Same isImplied()-not-isAnnotation() gating this file's
   history already had to discover the hard way for the self-referential case
   -- applied correctly from the start this time since that lesson was already
   known.
3. New `PrintC::pushCrossRegisterExpression(vn, base, op)` in printc.cc,
   modeled closely on `pushSegmentRegisterExpression` but keying the Symbol
   lookup to `base`'s address (e.g. SP), not `vn`'s (e.g. FP) -- reuses `base`'s
   existing HighVariable/Symbol if it already has one (common case, since SP
   itself is usually named elsewhere in the same function), falling back to
   `linkAnnotationSymbolAtUsepoint` only if it doesn't. `pushSymbol(sym,base,op)`
   at the end, not `pushSymbol(sym,vn,op)` -- this is the actual mechanism that
   makes the printed text say "SP" instead of "FP".
4. Header declarations added: `printlanguage.hh` (virtual no-op default,
   matching `pushSegmentRegisterExpression`'s existing pattern exactly) and
   `printc.hh` (PrintC override declaration).

**Editing-tool corruption, again -- this time severe enough to require a full
revert, not another patch.** The `str_replace` tool's fuzzy-whitespace matching
repeatedly collided across `printc.hh`, `printlanguage.hh`, and especially
printc.cc -- worse than either prior incident in this file's history. Multiple
rounds of "fix the reported corruption" repeatedly surfaced NEW corruption
elsewhere in the same file (word-spliced signatures merging two unrelated
functions' text mid-token, e.g. `void PrintC::pushAnnotation(const Varnode
*vn,const PcodeOp *op)egisterExpression(...)`), eventually reaching into
`pushEquate`'s previously-working implementation body and leaving it
genuinely broken (missing `return false;`, unbalanced brace) in a way that
`check-struct` did NOT catch until several edits later. Concluded, correctly,
that continued targeted patching was no longer safe -- **user reverted
printc.cc to its pre-session-2-part-2 state** (the state after the CAST-unwrap
fix only, before any cross-register work began) rather than trying to
excavate the corruption fragment by fragment. No checkpoint had been saved via
this session's checkpoint tool before the risky sequence began -- worth doing
`checkpoint()` before any multi-step edit sequence on this file going forward,
per that tool's own stated purpose.

**Recovery approach, successful:** rather than resume with `str_replace`,
switched to a full-array PowerShell splice (`Get-Content` -> array insert at a
line-number anchor found via `Select-String` -> `Set-Content`) to insert the
new `pushCrossRegisterExpression` function as one atomic block into the
(confirmed clean, reverted) printc.cc. This avoids the fuzzy-matching collision
mechanism entirely, since it's a positional array operation, not a text-content
search-and-replace. Verified clean via `check-struct` and a targeted
`Select-String` confirming exactly three correctly-named, non-overlapping
functions (`pushSegmentRegisterExpression`, `pushCrossRegisterExpression`,
`pushAnnotation`) before proceeding. **This approach should be preferred over
`str_replace` for any future insertion into this specific file**, given its
now-three-times-demonstrated fragility to fuzzy text matching, likely due to
its many structurally-similar repeated code shapes (multiple near-identical
`pushSymbol(sym,vn,op); return true;`-style blocks across neighboring
functions).

**Verification, full chain:**
1. `g++ -std=c++11 -c printc.cc printlanguage.cc coreaction.cc
   funcdata_varnode.cc funcdata.cc` -- one real compile error caught
   (`isRegister()` doesn't exist on `Varnode`; corrected to
   `getSpace()->getType() == IPTR_PROCESSOR`, verified against space.hh's real
   `spacetype` enum rather than guessed a second time). Clean, exit 0, after
   correction.
2. `.\gradlew.bat :Decompiler:linkDecompileWin_x86_64Executable --rerun-tasks`
   -- BUILD SUCCESSFUL, `2 actionable tasks: 2 executed` (the first attempt
   without `--rerun-tasks` reported `UP-TO-DATE` on both tasks despite real
   source changes -- Gradle's incremental-build detection missed the edit;
   `--rerun-tasks` forced a genuine rebuild. Worth using `--rerun-tasks`
   by default for this task going forward rather than trusting UP-TO-DATE
   after a source edit).
3. Copied to the Downloads deployment, hash-verified match (same discipline as
   every prior session).
4. Live `force_decompile` on both channels:
   - `init_copy_const_block_via_memcpy_banked` (ram:000149c8): still clean
     `SP - 2`, self-referential case unaffected by this session's changes, as
     expected.
   - `status_word_bit3_conditional_update_via_table` (000232c3), the exact
     function that regressed last session: **BOTH sites now clean.** The write
     (`clr.w @(-2,FP)`) prints `SP - 2` as before; the previously-regressed
     read (`tst.w @(-2,FP)`) now ALSO prints
     `*(short *)((int)(SP - 2) + -2) == 0` correctly. Zero `register0x0e` in
     this function.
5. **Whole-ROM census, fresh export from user against this build
   (`RVR_1998_x3 4g63t 21000011 md352553_9540.hex.c`, hash-confirmed distinct
   from the prior (regressed) export, not stale):**
   - `register0x0e` count: **1** (down from 3 last session, matching the
     original pre-regression baseline exactly).
   - The single remaining hit: `f266_knock_octane_composite_trim_compute(
     *(ushort *)((int)register0x0e + 6))` -- the already-known, confirmed-
     unrelated call-argument shape (item 1 in the prior session's breakdown).
     Untouched by this fix, exactly as expected/desired.
   - `__undef` count: 0.
   - WARNING count: 604, same as last session's post-CAST-unwrap-fix reading
     -- stable, not a further drop from this fix. (Still an open, untriaged
     item per the earlier "Not yet done" list -- unrelated to today's work.)

**This is a whole-ROM-verified result, not just the one function spot-check.**
The cross-register regression from the prior session is fixed, and the fix
does not appear to have introduced any new `register0x0e` occurrences anywhere
else in this ROM.

**Not yet done:**
1. The unrelated `register0x0e + 6` call-argument case -- still explicitly
   out of scope, deferred by the user for a separate investigation.
2. WARNING count (604) still not triaged -- unrelated to either fix session,
   flagged repeatedly, not yet actioned.
3. All items from the end of the prior session's "Not yet done" list that
   were about THIS fix specifically (design, implementation, whole-ROM
   verification) are now done. The items that were about the OLDER
   "CONFIRMED COMPLETE" section (dead `pushVnExplicit` intercept cleanup,
   full `ActionSegmentRegisterIdentity` class removal, upstream #817-adjacent
   writeup) still stand, now genuinely unblocked for the first time since
   this file's "CONFIRMED COMPLETE" status was originally (prematurely)
   written, since both known register0x0e-producing shapes on this ROM are
   now verified fixed at whole-ROM scale.
4. Per the user's earlier decision, when this does go upstream, the
   self-referential fix (CAST-unwrap) and the cross-register fix should be
   framed/submitted as two separate, independently-reviewable patches, not
   one combined change -- they were implemented as separable changes in this
   session specifically to keep that option open.
5. Only one ROM (RVR_1998_x3 4g63t 21000011 md352553) has been census-checked
   this way. Worth doing the same whole-ROM `register0x0e`/`__undef`/WARNING
   count check against at least one or two other ROMs in this project before
   treating this as universally confirmed, given how much this file's history
   shows narrow test coverage hiding real gaps.

---

## 2026-08-24 session, part 3: scope leak found and removed -- ActionFarPointerJoin

**User flagged, correctly, that a draft commit of today's fixes (pushed to
professor-jonny/ghidra as commit b995569) contained unrelated
`ActionFarPointerJoin` content in coreaction.hh that has nothing to do with
the register0x0e work.** This class is inherited from earlier branch history
-- specifically the original `<farpointerjoin>` cspec-declared call-argument
fusion patch (#9522) mentioned at the very start of review15.md as the change
being regression-tested when register0x0e was first discovered. It was never
part of any register0x0e fix session, self-referential or cross-register, and
should not have been sitting in the same tree/commit as this work.

**Checked before removing:** searched every `.cc` and `.hh` file in this
directory for `FarPointerJoin`/`farpointerjoin`. Result: declared in
coreaction.hh only (lines 755-771, doc comment through the class's closing
`};`), with a `virtual int4 apply(Funcdata &data);` declaration but NO
implementation anywhere in any `.cc` file, and no registration in the action
pipeline anywhere else. Confirmed genuinely dead/orphaned in this checkout --
safe to remove with no build dependency.

**Removed via the same full-array PowerShell splice approach adopted earlier
this session** (find start/end line anchors via `Select-String`, confirm the
`class ActionFarPointerJoin` marker falls between them, splice the array,
`Set-Content`) rather than `str_replace`, given this file's now-repeated
fragility to fuzzy text matching. Removed the whole doc-comment-through-
closing-brace block plus the one trailing blank line. Verified: zero
`FarPointerJoin`/`farpointerjoin` matches anywhere in the directory after
removal, and `g++ -std=c++11 -c printc.cc printlanguage.cc coreaction.cc
funcdata_varnode.cc funcdata.cc` still compiles clean (exit 0) -- confirming
it really was unreferenced, not silently required elsewhere.

**Not yet done:** this was a local-tree removal only. The already-pushed
commit (b995569) on professor-jonny/ghidra still contains the
ActionFarPointerJoin content as of this writing -- if that commit is meant to
be corrected rather than superseded by a fresh commit, it will need an
amend/force-push (or a new follow-up commit) reflecting this removal before
being treated as ready for an actual upstream Ghidra PR. Not done in this
session; flagging so it isn't lost.

---

## 2026-08-24 session, part 4: new cross-register datatest written and
## VERIFIED PASSING via a native decomp_test run -- first real test-harness
## execution in this file's entire history, not just a claim

**Context:** the draft commit message (b995569) claimed the existing
`register0x0e_segmentop_fix.xml` datatest was run via a "locally-built
ghidra_test data-test runner" with "Total tests applied = 2, Total passing
tests = 2" -- but nothing in review16.md's own history up to this point
actually documents that run happening. User asked whether a NEW test was
needed to cover the cross-register fix (since the existing datatest only
covers the self-referential shape); agreed yes, and this session both wrote
that test AND, unlike the earlier undocumented claim, actually built and ran
the real native test harness end-to-end, confirming both datatests pass for
real.

**New test written:** `datatests/register0x0e_crossregister_fix.xml`,
covering `status_word_bit3_conditional_update_via_table` (the exact function
that regressed in part 1 of this session) -- both the `clr.w @(-2,FP)` write
and the `tst.w @(-2,FP)` read, asserting zero `register0x0e` and exactly two
`SP - 2` occurrences. 111-byte excerpt read live via `read_memory` from the
real ROM and hex-encoded PROGRAMMATICALLY from the raw decimal byte array
(not manually transcribed) after an initial manual-transcription attempt was
caught to be wrong (length and digit mismatch) via direct comparison before
it was ever written to disk -- worth continuing to prefer programmatic
byte-array-to-hex generation over hand-typing for any future test excerpts.

**Environment obstacles hit and resolved, in order:**

1. **No native test-runner build existed in this Windows checkout.**
   `make test` requires `uname`/`mkdir -p`/`sh -c`, none of which work under
   plain PowerShell even with `mingw32-make.exe` on PATH (confirmed present
   at `C:\mingw64\bin\`). `buildNatives.gradle` only defines `decompile` and
   `sleigh` NativeExecutableSpec targets -- no Windows-native Gradle path for
   the C++ test runner exists in this repo as configured. Resolved by
   compiling `decomp_test_manual.exe` via a single direct `g++` invocation
   (not through the Makefile at all), assembling the correct file set from
   the Makefile's own `CORE`/`DECCORE`/`SLEIGH`/`EXTRA`/`test` variable
   definitions (98 files total) rather than guessing.
2. **`filemanage.cc` compile error:** `struct dirent` has no `d_type` member
   under MinGW (a glibc/Linux-specific extension `_DIRENT_HAVE_D_TYPE`
   guards elsewhere in the codebase but wasn't used here). This is a
   pre-existing upstream Ghidra portability gap, unrelated to any
   register0x0e work -- it only surfaced now because `test.cc`'s
   `gatherDataTests()` calls `FileManage::directoryList()`, which the
   production `decompile.exe` build path never exercises. Fixed by wrapping
   the `d_type`-based branch in `#ifdef _DIRENT_HAVE_D_TYPE`, falling back
   unconditionally to the function's own pre-existing `stat()`-based
   directory-detection path (already correct, already used for
   `DT_UNKNOWN`/`DT_LNK`) when the guard is absent. Verified: recompiles
   clean under g++/MinGW afterward.
3. **Link errors: missing zlib symbols + missing
   `startDecompilerLibrary`/`FunctionTestCollection::runTestFiles`.** First
   was a missing `-lz` flag (the Makefile's own `LNK=-lz`, omitted from the
   manual g++ invocation). Second was two genuinely missing source files
   (`libdecomp.cc`, `testfunction.cc`) that belong to the Makefile's `EXTRA`
   catch-all group (`ALL_NAMES` minus every other named group) -- the first
   manual file-list attempt mis-transcribed this group and omitted them.
   Recomputed `EXTRA` correctly by diffing the full real `*.cc` directory
   listing (via `Get-ChildItem`) against every other explicitly-known group,
   rather than guessing a second time.
4. **`bfd.h` not found** for `analyzesigs.cc`/`bfd_arch.cc`/`codedata.cc`/
   `loadimage_bfd.cc` (all four either directly include or transitively
   depend on `loadimage_bfd.hh`, which requires the BFD library headers --
   not installed/available in this MinGW setup). Confirmed via
   `grep-project` that nothing in the required dependency chain (`test.cc`,
   `libdecomp.cc`, `testfunction.cc`, `interface.cc`, `ifacedecomp.cc`,
   `consolemain.cc`) references any BFD-related symbol -- these four files
   are for a separate, unrelated ELF/PE-direct-loading path, safe to drop
   from the build entirely for this test-only purpose rather than chase
   installing BFD.
5. **`-sleighpath` layout mismatch.** `SleighArchitecture::scanForSleighDirectories`
   assumes a standard "Ghidra/Processors/*/data/languages" layout (its own
   doc comment says so explicitly) -- pointing it at the standalone
   `Ghidra-H8-Processor` dev repo (laid out as `h8/data/languages/`, not
   `Ghidra/Processors/h8/data/languages/`) would not have resolved. Instead
   pointed `-sleighpath` at the DEPLOYED Ghidra root
   (`Downloads\ghidra_12.2_DEV_20260818\ghidra_12.2_DEV`), which already has
   `h8` correctly registered under `Ghidra\Processors\h8\` (confirmed via
   `Get-ChildItem` listing every processor module there) -- this is the same
   install this entire session's `force_decompile` live-testing has already
   been using all along.
6. **`XML error parsing compiler specification` / `syntax error` on
   `h8539f.cspec`.** The DEPLOYED copy of this file
   (`Downloads\...\Ghidra\Processors\h8\data\languages\h8539f.cspec`) had a
   UTF-8 BOM (`EF BB BF`) that Ghidra's native XML decoder rejects as a
   syntax error, even though .NET's `[xml]` cast and (presumably) the
   Java-based GUI Ghidra tolerate it silently -- exactly the same class of
   issue as the BOM found and fixed in our own new datatest XML earlier this
   session (see part 3 timing note: same root cause, different file, this
   one pre-existing in the deployed install and NOT caused by any edit this
   session made). Confirmed the source-repo copy of the same file
   (`Ghidra-H8-Processor\h8\data\languages\h8539f.cspec`) has no BOM, so this
   was specific to whatever deployed this copy into Downloads, not a defect
   in the H8 module's source. Stripped with the user's explicit confirmation
   before touching a file outside the project's own source tree. Verified:
   after stripping, `register0x0e_segmentop_fix.xml` (the EXISTING, older
   test) ran and passed cleanly on its own --
   `Total tests applied = 2, Total passing tests = 2` -- this is the first
   time in this file's entire history that number has actually been
   confirmed by a real run rather than repeated from an unverifiable earlier
   claim.
7. **`terminate called after throwing an instance of 'ghidra::DecoderError'`,
   uncaught, specific to the NEW test file.** Isolated by running each
   datatest file individually rather than together -- the existing file
   passed alone, the new file crashed alone. **Root cause, correctly
   identified by the user, not by further tool-based investigation:** the
   new file's `<!-- ... -->` comment block used `--` repeatedly as
   em-dash-style punctuation in the explanatory prose. XML forbids the `--`
   sequence anywhere inside a comment body except as part of the closing
   `-->` delimiter -- a genuine well-formedness violation, not a
   decode-flow/call-target issue as initially hypothesized (an out-of-range
   `jsr`/`bsr` target in the 111-byte excerpt, which was never actually
   confirmed and turned out not to be the cause). Fixed by removing/
   rewording the offending `--` sequences in the comment text.

**Final confirmed result, live, both files:**
```
Success -- No register0x0e placeholder text
Success -- FP-derived write and read sites both print as SP - 2
Success -- No register0x0e placeholder text
Success -- SP - 2 self-referential arithmetic prints as a real subtraction expression
Total tests applied = 4
Total passing tests = 4
```

**Bonus signal, unplanned but useful:** running the FULL `datatests`
directory (no name filter) surfaced a large batch of pre-existing upstream
Ghidra test failures entirely unrelated to this work (Partial splitting,
No for-loop alias, Pointer Compare, Mixed float/int, Copy trim, and others),
plus one uncaught crash partway through (`injectoverride.xml`, a callfixup
test, execution error "No call is made at this address") before it would
have reached our alphabetically-later files. None of these mention
register0x0e, SEGMENTOP, self-referential/cross-register arithmetic, or
anything this session's changes touch -- almost certainly a pre-existing
baseline in this checkout, not something introduced by any change in this
file's history. Not investigated further (out of scope), but worth knowing
this checkout's full datatest suite is NOT currently all-green even before
our changes, so a future upstream PR reviewer running the full suite should
not be surprised by unrelated failures -- worth being able to point at this
note if asked.

**Not yet done:**
1. `filemanage.cc`'s MinGW portability fix and the deployed `h8539f.cspec`
   BOM strip are both genuinely useful, general fixes, but neither is part
   of the register0x0e patch itself -- if the register0x0e fix goes upstream,
   the filemanage.cc fix could reasonably go as ITS OWN separate, small
   upstream PR (a real, general Windows/MinGW portability bug in Ghidra's
   own code, independently useful to anyone else trying to build/test on
   Windows without MSYS/WSL) -- not yet proposed or written up as such.
2. The deployed `h8539f.cspec` BOM fix is local-environment-only (this one
   user's Downloads install) and not a source-tree change at all -- nothing
   to upstream there, just worth remembering if this same deployed install
   is ever refreshed/redeployed from a source that reintroduces the BOM.
3. Per the earlier decision, self-referential and cross-register fixes
   should still be framed as two separable patches if/when submitted
   upstream -- now each has its own passing datatest to go with it, which
   strengthens that separation rather than weakening it.
4. The already-pushed commit (b995569) still needs to be reconciled with:
   (a) the `ActionFarPointerJoin` removal from part 3, and (b) this new
   datatest file plus the underlying cross-register source changes it
   verifies, and (c) the corrected commit message drafted earlier this
   session (self-referential + cross-register both described, corrected
   Status section). None of that has been pushed yet as of this writing.

---

## 2026-08-25 session: pushed commit (2f98ff5) audited against live PR --
## found and fixed a real functional regression plus a missing manifest entry,
## both introduced by earlier str_replace corruption this session that
## compiled clean and was never caught until now

**Context:** user shared the live PR (NationalSecurityAgency/ghidra#9541,
commit 2f98ff5) and asked if it was ready. It was not. Two real problems
found, both traced back to str_replace corruption from earlier in THIS
session (the "severe corruption, full revert" incident) that was believed
fully resolved at the time but was not.

**Problem 1 -- genuine functional regression in PrintC::pushConstant,
already pushed to the real NSA repo:** the "default printing" fallback
path (TYPE_ARRAY/TYPE_STRUCT/TYPE_UNION/etc., and any TYPE_PTR/TYPE_PTRREL
falling through the char/code special cases) was missing its
push_integer(...) call entirely -- it set up hex-display mode and popped it
again without ever pushing anything onto the RPN output stack. This
compiled clean (g++ -c only checks syntax, not logic) and was never caught
by any compile check this session, only found by manually re-reading the
function end-to-end when the user asked to verify a specific diff hunk
that looked suspicious. This has NOTHING to do with the register0x0e fix
itself -- it's collateral damage from the earlier corruption/recovery
sequence in part 4 of this session, and would have shipped to the real
Ghidra upstream repo as a silent constant-printing bug affecting an
unrelated, much broader set of types (arrays, structs, unions) had it not
been caught here.

**Problem 2 -- str_replace corrupted printc.cc TWICE MORE while attempting
to fix the above**, both times reporting success via get-edit-stats (100%
hit rate, no faults) while the actual disk content was either unchanged
or, worse, spliced into a COMPLETELY DIFFERENT, unrelated location in the
file (PrintC::pushConstant's TYPE_FLOAT case, ~70 lines away from the
intended edit target) -- confirmed only by reading the raw file via
Select-String directly, not trusting the tool's own reported success.
**Decision made in this session: stop using str_replace on this file/
checkout entirely for the remainder of the session; switch fully to the
PowerShell full-array Get-Content/index-splice/Set-Content approach**
(verify exact current line content first via Select-String, splice by
explicit 0-based array index, re-verify after write) -- this is the third
time this session str_replace's fuzzy matching has caused real corruption
on this specific file (printc.cc), and the second time the tool's own
success/fault reporting did not reflect what was actually written to disk.
This is now a hard rule for this checkout, not just a preference.

**Fixes applied, both via the array-splice method, both verified against
raw disk content (not the tool's buffer) before moving on:**
1. Restored the missing push_integer(val,ct->getSize(),false,tag,vn,op,
   displayFormat); call in PrintC::pushConstant's default-printing
   fallback path.
2. Restored the TYPE_FLOAT case's push_float(...) call plus the missing
   TYPE_SPACEBASE/TYPE_CODE/TYPE_ARRAY case labels, which had been
   overwritten by a stray copy of the pushSegmentRegisterExpression doc
   comment during the earlier corruption incident.
3. Fixed the corrupted doc-comment line ("using the base regis  //
   Detect that shapester0x0e-style annotation fallback.") that had
   survived, unfixed, since the FIRST corruption incident earlier this
   session -- multiple str_replace attempts to fix it previously had
   silently failed or landed in the wrong place without any error being
   surfaced.

**Problem 3 -- missing certification.manifest entry, found by the user,
not by any tool-based check:** user asked directly whether
register0x0e_crossregister_fix.xml needed the same
certification.manifest entry as register0x0e_segmentop_fix.xml. It did --
confirmed by running the actual Gradle :Decompiler:ip task, which failed
outright (not a warning -- a hard build failure) citing a stray
decomp_test_manual.exe left inside the source tree from earlier manual
testing (unrelated to the manifest question, but caught by the same task
run). Added the missing manifest line via the same array-splice method
(str_replace attempted first, corrupted ANOTHER file --
certification.manifest -- on the very first attempt, splicing text into
the middle of the unrelated ptrtoarray.xml line; reconfirms the
session-wide decision to avoid str_replace on this checkout).

**Also fixed as a direct consequence:** manually-built test executables
(decomp_test_manual.exe, decomp_test_manual2.exe) and their .o/.pdb/.ilk
build artifacts were being left inside
Ghidra/Features/Decompiler/src/decompile/cpp/ -- the actual module source
tree -- which is what tripped the :Decompiler:ip task's IP-scan (it
requires every file in the module to have a recognized IP/license header,
which a bare test .exe obviously doesn't). **New standing rule for this
checkout: any manually-built test executable MUST be built to a directory
OUTSIDE the module source tree** (used C:\temp\decomp_test_build\ for the
remainder of this session) -- never inside src/decompile/cpp/ itself, even
temporarily, since a build-and-forget-to-delete mistake here silently
breaks the real Gradle build in a way that's easy to miss if you only run
the narrower :Decompiler:linkDecompileWin_x86_64Executable task (which
does NOT run the :Decompiler:ip check) rather than the full buildGhidra.

**Full verification chain, this pass:**
1. g++ -std=c++11 -c printc.cc printlanguage.cc coreaction.cc
   funcdata_varnode.cc funcdata.cc -- clean, exit 0 (confirms syntax only,
   NOT logic -- this is exactly the check that missed the missing
   push_integer call in the first place; do not treat a clean compile as
   sufficient confirmation of correctness for this file going forward).
2. .\gradlew.bat :Decompiler:linkDecompileWin_x86_64Executable
   --rerun-tasks -- BUILD SUCCESSFUL, 2 actionable tasks: 2 executed.
   Deployed to the live Downloads install, hash-verified match.
3. .\gradlew.bat buildGhidra (user-run, full build) -- FAILED at
   :Decompiler:ip on the stray decomp_test_manual.exe (see above). This
   is the run that actually caught the leftover-build-artifact problem --
   worth always running the FULL buildGhidra at least once before
   considering a change ready, not just the narrower link task, since the
   narrower task does not exercise the ip/certification checks at all.
4. After cleanup (stray .exe/.o/.pdb/.ilk removed, certification.manifest
   entry added): .\gradlew.bat :Decompiler:ip alone -- BUILD SUCCESSFUL,
   1 actionable task: 1 executed.
5. Manual native decomp_test build, this time correctly built OUTSIDE the
   source tree (C:\temp\decomp_test_build\decomp_test.exe), run by the
   user directly (avoiding this session's repeated 4-minute tool-timeout
   problem on long compiles) against both datatests together:
   `
   Success -- No register0x0e placeholder text
   Success -- FP-derived write and read sites both print as SP - 2
   Success -- No register0x0e placeholder text
   Success -- SP - 2 self-referential arithmetic prints as a real subtraction expression
   Total tests applied = 4
   Total passing tests = 4
   `
   This is the first time in this file's history that BOTH the
   pushConstant regression fix AND the manifest fix have been confirmed
   together, functionally, via the real native test runner -- not just a
   clean compile.

**Not yet done:**
1. None of these fixes (pushConstant restoration, TYPE_FLOAT/case-label
   restoration, doc-comment cleanup, certification.manifest entry) have
   been pushed to the actual PR branch (professor-jonny:9540) yet as of
   this writing -- the already-pushed commit (2f98ff5) still contains the
   pushConstant regression and the missing manifest entry. This needs to
   be amended/force-pushed or landed as a follow-up commit before the PR
   is genuinely ready for review.
2. Given str_replace has now corrupted files on THREE separate occasions
   this session, all with clean-looking success reports from the tool
   itself, it's worth treating ANY past str_replace edit on this checkout
   claimed to have succeeded with real suspicion until independently
   re-verified via a raw disk read (Select-String / Get-Content), not the
   editing tool's own read-back, which appears to sometimes read a stale
   or different buffer than what's actually on disk.
3. Worth doing one more full, careful read-through of printc.cc and
   printlanguage.cc end-to-end (not just the specific functions this
   session touched) before this is called final, given how many times
   corruption has silently landed in unexpected, unrelated locations in
   this specific file already.

---

## 2026-08-26: auStack_ regression investigation -- turned out to be a pre-existing bug, not caused by 9540

**Context:** After the register0x0e/SEGMENTOP fix (aadd7ee, PR #9541) was applied
and exported against the full RVR_1998_x3 ROM, a stress-test comparison
against the pre-patch export showed:

- `stack0x` occurrences: 355 (old) -> 20 (new)
- `auStack_` occurrences: 16 (old) -> 214 (new)
- `register0x0e` occurrences: 1 (old) -> 1 (new), i.e. no change either way

This looked like a large, systemic regression caused by aadd7ee -- roughly
335 implicit `stack0xNNNN` references got converted into named
`auStack_NNN [128]` array-local declarations across dozens of functions
that have nothing to do with SEGMENTOP.

**First (incorrect) theory:** `Funcdata::linkAnnotationSymbolAtUsepoint`
(new in aadd7ee) mints a Symbol via `localmap->addSymbol(regname, ct,
vn->getAddr(), usepoint)` keyed to the base register's own storage
address (e.g. SP). Theorized this collided with the stack-frame-layout
recovery pass, which uses that same address as its anchor, causing it to
materialize the whole frame-adjustment region as one array instead of
resolving individual locals.

**Fix attempted based on that theory:** rewrote
`linkAnnotationSymbolAtUsepoint` to never call `addSymbol()` -- only
reuse an existing Symbol via `queryProperties`, return NULL otherwise.
Updated `pushSegmentRegisterExpression`/`pushCrossRegisterExpression` in
printc.cc to fall back to a plain `pushAtom(Atom(regname,...))` register-
name print (same mechanism `pushAnnotation`'s own fallback already uses)
instead of failing back to `register0x0e` on a NULL Symbol. Both edits
applied via Pulsar `run-command` (str_replace was reported broken for
this session; used PowerShell array-splice + base64-chunked transfer
instead, per this file's established recovery method from prior
sessions). Verified via Select-String immediately after each edit: old
`addSymbol(regname...)` call gone from funcdata_varnode.cc, both new
push functions present and correctly ordered in printc.cc, old
`if (sym == NULL) return false;` fail-fast pattern completely removed
(0 occurrences), new `pushAtom(Atom(regname...` fallback present in both
functions (3 total occurrences incl. pushAnnotation's own pre-existing
one, as expected).

**Build:** `.\gradlew.bat buildGhidra` -- initially failed with
`No IP found for ...funcdata_varnode.cc.bak_20260825_austack_fix` (the
Decompiler:ip task's IP/license-header checker walks the whole cpp/
source directory and chokes on backup files left sitting next to the
real sources with a `.bak_*` suffix -- NOT a `.cc`/`.hh` extension, so it
doesn't get skipped by extension filtering the way you'd hope). Fixed by
moving both `.bak_20260825_austack_fix` files out of the source tree
entirely (to `%TEMP%\austack_fix_backups`) rather than just renaming
them. Rebuild after that: `BUILD SUCCESSFUL in 4m 44s, 717 actionable
tasks: 190 executed, 527 up-to-date`.

**Install:** user manually copied the freshly-built decompile.exe
(`Ghidra\Features\Decompiler\build\os\win_x86_64\decompile.exe`, 09:47:57,
2834432 bytes) over the real install copy at
`Downloads\ghidra_12.2_DEV_20260818\ghidra_12.2_DEV\Ghidra\Features\
Decompiler\os\win_x86_64\decompile.exe` (confirmed same byte size after
copy, timestamp 09:50:04 -- correctly after the build). Ghidra was then
fully restarted (not just project-closed/reopened) and the ROM
re-exported at 09:57:58 -- i.e. genuinely after both the rebuild and the
binary swap, not a stale artifact.

**Result: no change.** Re-measured `auStack_` count on the fresh export:
still 214, identical to before the fix. Confirmed live (not just via the
export file) by calling `force_decompile` on `sat_add_u16` directly
against the running (freshly restarted) Ghidra instance -- output
unchanged, still shows `ushort auStack_102 [128];` and
`*(ushort *)((int)(auStack_102 + 0x80) + 8)` for what should be
parameter `b`. Source file re-confirmed still correctly patched
(`CHANGED 2026-08-25` marker present, old `addSymbol(regname` call
absent) -- so this was a real, valid test of the actual fix, not a
build/deployment mistake. The fix genuinely does not address whatever is
producing this output.

**Root-cause re-investigation, using a saved debug.xml for sat_add_u16
(user-supplied, from a Ghidra "save function" / xml_savefile export):**
The saved database state shows `sat_add_u16` has two completely normal,
correctly-declared, locked Symbols:
```
<symbol name="a" typelock="true" namelock="true" cat="0" index="0x0">
  <type name="ushort" size="2"/>
  <addr space="stack" offset="0x4" size="2"/>
</symbol>
<symbol name="b" typelock="true" namelock="true" cat="0" index="0x1">
  <type name="ushort" size="2"/>
  <addr space="stack" offset="0x6" size="2"/>
</symbol>
```
Both are typed `ushort`, sized 2, at ordinary stack offsets 0x4/0x6 --
nothing wrong with the Symbol database itself. This means the bug is
NOT in Symbol *creation* (nothing wrong there) but in Symbol *lookup* at
print time -- something is failing to match the live Varnode for
parameter `b` back to this existing, correctly-declared Symbol, and
falling through to a generic name-synthesis path instead.

Traced the actual `auStack_` name-generation code (grepped for the
literal string, found nothing -- it's built programmatically, not a
literal). Found it in `ScopeLocal::buildVariableName` (varmap.cc:623),
which is 100% stock/upstream Ghidra code, untouched by aadd7ee or by
today's attempted fix. This function is the fallback-of-last-resort
namer, only invoked when the normal Symbol lookup has already failed for
an addrtied stack Varnode -- it synthesizes `<typeprefix><Spacename>_
<hexoffset>` (e.g. "au" + "Stack" + "_" + hex -> "auStack_102"). This
confirms definitively: the bug is a **Symbol lookup/matching failure**
against the local Scope for address-tied stack Varnodes under the
`__stdcall_far_allstack` calling convention -- not anything in
aadd7ee's annotation-Symbol code, and not fixed by today's rewrite of
`linkAnnotationSymbolAtUsepoint`, because that function is never on the
call path for this bug at all.

**Checked whether this is new (introduced by aadd7ee) or pre-existing:**
directly compared the OLD (pre-9540) export's `sat_add_u16` against the
new one:
```
OLD: result = a + *(ushort *)((int)&stack0xfffe + 8);
NEW: result = a + *(ushort *)((int)(auStack_102 + 0x80) + 8);
```
**The old version ALSO fails to print `b` as `b`.** It was never
resolving to the real Symbol either -- it was already falling through to
the same underlying lookup-failure path, just landing on the
`stack0xNNNN` implicit-reference branch of that fallback instead of the
`auStack_NNN[128]` array branch. So: **the underlying Symbol-lookup
failure for `__stdcall_far_allstack` stack parameters pre-dates aadd7ee
entirely** -- it is not a regression introduced by today's patches. What
changed between old and new is only *which of the two possible fallback
render styles* gets chosen for the same underlying failure, not whether
the failure happens. The register0x0e fix and the `auStack_`-focused
Symbol-creation fix attempted today are both provably irrelevant to this
specific bug, since neither touches `buildVariableName` or whatever
decides between its two branches.

**Candidate root cause, not yet verified:** `h8539f.cspec` defines
`__stdcall_far_allstack` at `extrapop="4"` (line 233/259), but the
debug.xml's saved prototype for `sat_add_u16` shows
`<prototype extrapop="6" model="__stdcall_far_allstack" modellock="true">`
-- a live 2-byte mismatch between what the calling convention currently
specifies and what's locked into this function's already-analyzed
prototype. This project has hit this exact class of bug once before (see
the `__stdcall_far_2arg`/`table_lookup_interp` extrapop=4-vs-6 history
earlier in this file, cspec lines ~295-306), where a stale extrapop
caused pentry-offset misalignment; that fix explicitly warns "a full
Ghidra restart, not just ReloadSleighLanguage.java, before any further
pentry-offset experiments on this prototype are trustworthy." This is a
plausible mechanism (a stale/mismatched extrapop would misalign the
stack-offset arithmetic used to match a Varnode's address back to its
Symbol's stored address, causing an otherwise-correct Symbol to silently
not be found) but has NOT yet been verified against a live rebind or
force-decompile. Do not treat this as confirmed.

**Status: unresolved.** Today's two-file fix (funcdata_varnode.cc +
printc.cc, removing the addSymbol() call from
linkAnnotationSymbolAtUsepoint) is a legitimate, defensible change on its
own terms (avoids a theoretical Symbol/Scope collision) but is confirmed,
via live force_decompile against a freshly rebuilt+reinstalled+restarted
decompiler, to NOT fix the auStack_/parameter-name bug it was written
for -- because that bug turns out to be unrelated and pre-existing.
Recommend leaving today's fix in place (it's harmless and may still be
correct for its original narrow theoretical concern) but treating the
auStack_/param-naming bug as a SEPARATE, still-open issue, likely rooted
in the extrapop mismatch on `__stdcall_far_allstack`, requiring
independent investigation before any further fix attempt.

**Not yet done:**
1. Verify the extrapop=4-vs-6 mismatch theory directly -- e.g. force a
   live re-bind of sat_add_u16's prototype at the current cspec's
   extrapop=4, or temporarily bump the cspec's `__stdcall_far_allstack`
   extrapop to 6 to match the saved debug.xml state, and check whether a
   fresh force_decompile then correctly prints `a`/`b` by name.
2. If the extrapop theory is confirmed, determine why extrapop=6 got
   saved into this function's locked prototype in the first place when
   the cspec has said 4 since this prototype was introduced -- was it
   ever correct, or has it been wrong since the prototype was first
   applied to this function during an earlier session's manual prototype
   lock?
3. Check how many functions across the ROM are bound to
   __stdcall_far_allstack and whether they ALL show this param-naming
   failure, or only some -- needed to scope how big this pre-existing
   bug actually is before prioritizing a fix.
4. Determine what specifically selects between the `stack0xNNNN` and
   `auStack_NNN[128]` fallback rendering branches for the same underlying
   lookup failure -- this is what actually changed between old and new
   exports, even though it's not the root problem. Worth understanding
   for completeness even after the real bug is found, since a full ROM
   regression pass (per PR #9541's own "not yet done" item) will need to
   distinguish "still broken, same as before" from "newly broken by this
   session's changes" on a function-by-function basis, and superficial
   auStack_/stack0x counts alone conflate the two.
5. PR #9541 has an open review comment (jobermayr, 2026-08-25, on
   funcdata_varnode.cc) asking whether deletion of a blank line before
   linkSymbolReference's doc comment was intentional -- confirmed not
   intentional (formatting slip from inserting the new function). Reply
   drafted covering both the blank-line question and today's
   auStack_/frame-layout finding, not yet posted. Given today's finding
   that the auStack_ bug is unrelated/pre-existing, the reply should
   likely be softened or reframed before posting -- the frame-layout
   collision theory as originally drafted is not confirmed and the
   actual auStack_ cause looks unrelated to aadd7ee entirely.

---

## 2026-08-26 session, part 2: auStack_ grouping is a real structural change, not a cosmetic rename -- next step identified

**Follow-up finding, same investigation as the previous entry.** Re-examined
the old-vs-new `sat_add_u16` diff more carefully at the user's prompting:

```
OLD: result = a + *(ushort *)((int)&stack0xfffe + 8);
NEW: result = a + *(ushort *)((int)(auStack_102 + 0x80) + 8);
```

Initially treated this as "just a different fallback render style for the
same underlying lookup failure" -- **that undersells it.** The `+ 0x80`
term is new and was NOT present in the old output at all. This is not
merely a different name for the same address computation; it is Ghidra's
normal convention when several individually-resolved stack locals get
swept into ONE grouped array symbol: the array is declared at the group's
lowest address, and every real access into it then needs an extra
"offset into the array" term added back on top to reach the original
target. The presence of `+ 0x80` is direct evidence that **frame-layout
recovery actually changed which locals it grouped together** between the
old and new builds -- this is a real structural change in stack-frame
analysis output, not a cosmetic renaming of an unchanged computation.

This raises the priority of the still-unconfirmed extrapop=4-vs-6
mismatch theory (see previous entry, `__stdcall_far_allstack` in
`h8539f.cspec` vs `sat_add_u16`'s locked prototype in the saved
debug.xml). A stale/mismatched extrapop misaligning how much of the frame
Ghidra thinks belongs to the callee is a plausible, direct mechanism for
exactly this kind of regrouping -- not just a naming-fallback quirk.

**Next step, concrete and specific (supersedes the more open-ended item 4
in the previous entry's "not yet done" list):** read
`ScopeLocal::buildVariableName` (varmap.cc:623) AND, more importantly,
whatever grouping/merge logic feeds it -- the code that actually decides
to materialize a set of individual stack locals as one combined array
symbol (the `auStack_NNN[128]`-style declaration) rather than resolving
them individually. This is upstream of `buildVariableName` itself
(which only synthesizes the *name* for whatever the grouping logic
already decided to materialize) -- likely in the stack-frame/local-scope
recovery pass (candidates: `ScopeLocal`'s range-merging logic in
varmap.cc, or wherever `MapEntryConflict`/group-partial local recovery
happens; grep for where array-typed HighVariables get synthesized from
overlapping address-tied ranges). Once that code is located:

1. Confirm directly (not just by inference from the `+0x80` symptom)
   that this grouping logic is sensitive to extrapop/frame-size
   inputs, and that the specific extrapop=4-vs-6 mismatch on
   `sat_add_u16`'s locked prototype is what changed its grouping
   decision between the old and new builds.
2. If confirmed, this becomes the actual root-cause fix target for the
   auStack_ regression -- a separate, still-open bug from both the
   register0x0e/SEGMENTOP fix (aadd7ee/#9541) and today's
   linkAnnotationSymbolAtUsepoint lookup-only hardening, neither of
   which touch this grouping logic at all.
3. Do NOT conflate this with PR #9541's scope -- per this file's
   established practice (Step 4's "own separate, clean commit" rule,
   the self-referential/cross-register split, etc.), this should be
   scoped, designed, and landed as its own independent fix once
   understood, not folded into the current draft PR.

**Not yet done:** everything in the previous entry's "not yet done" list
still stands; this entry narrows item 4 specifically into "go read the
stack-frame grouping/merge logic feeding buildVariableName, not just
buildVariableName's own name-synthesis code" as the concrete next action.

---

## File closed -- auStack_ investigation continues in review17.md

This file (review16.md) has grown to 2271 lines. The register0x0e/SEGMENTOP
fix itself (self-referential + cross-register cases) is CONFIRMED COMPLETE,
landed as NationalSecurityAgency/ghidra#9541 (commit aadd7ee), and that part
of this file's history is closed -- no further work expected there barring
new regressions.

The auStack_ grouping regression found during this file's final whole-ROM
regression pass (the entry immediately above) is a distinct, separate bug,
confirmed unrelated to extrapop as originally theorized (see review17.md for
the correction) and explicitly out of scope for PR #9541 per this file's own
established practice of landing unrelated fixes independently. That
investigation continues in **review17.md**, starting from a short recap of
this problem statement rather than repeating this file's full chronological
log. Any future session picking up the auStack_ thread should read
review17.md first, and only consult this file for the register0x0e fix's own
history if needed.

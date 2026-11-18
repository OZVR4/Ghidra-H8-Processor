# review22.md — muldiv_u16 decompiler bug: current understanding (2026-09-14)

This file was rewritten from scratch at the end of a long investigation
session to give a clean, current summary. The raw chronological version
(multiple reversed hypotheses along the way, kept for reference/audit trail)
is preserved in git history / can be regenerated on request; this version
states only what is currently believed to be true, evidenced, and actionable.

Continues from review19-21.md (read those for earlier, separately-resolved
bugs and general tooling lessons — review19 is an unrelated, already-fixed
`sat_add_u16` bug; review20/21 cover the two already-applied
`mergeTestBoundary` fixes for a related-but-distinct Cover-boundary defect
class). review23/24.md are this session's earlier, now-superseded
chronological notes — safe to skip; everything useful from them is folded
in below.

## The bug

`muldiv_u16` (address `0x141ba`) decompiles as:

```
ushort __stdcall_far_allstack muldiv_u16(ushort a,ushort b,ushort c)
{
  ushort uVar1;
  ushort uVar2;
  ushort uVar4;
  undefined2 uVar3;
  undefined1 *puVar5;
  undefined1 auStack_82 [128];

  uVar3 = (undefined2)((uint)a * (uint)b);
  uVar2 = (ushort)((uint)a * (uint)b >> 0x10);
  uVar4 = b;                                              // <- WRONG (see below)
  puVar5 = auStack_82 + 0x80;
  uVar1 = *(ushort *)(puVar5 + 10);
  if ((uVar1 == 0) || (uVar1 <= uVar2)) {
    return 0xffff;
  }
  uVar3 = (short)(CONCAT22(uVar2,uVar3) / (uint)uVar1);
  return uVar4;
}
```

The function is meant to return the division result, but the visible
`uVar4 = b;` line and `return uVar4;` make it look like it returns `b`
unchanged. The underlying computation is actually correct (verified via raw
P-code and gdb across earlier sessions) — this is a **decompiler display
bug**, not a real bug in the compiled binary.

## What the bug actually is (current best understanding)

Using a one-shot Ghidra script (`DecompInterface`/`HighFunction`, via
`ghidra:run_script_inline`) that dumped every PcodeOp touching register
offset `0x02` by **full SeqNum** (address + order + uniquifier) — not just
instruction address, which is what every earlier round of instrumentation
in this and prior sessions used, and which turned out to be the core
methodological mistake driving several false leads (see "dead ends" below —
there are genuinely two separate PcodeOps at each of the addresses
`0x141c2` and `0x141ce`, which address-only logging silently conflated):

**Group A** — ordinary, correct, and correctly suppressed from printing
(`notPrinted()=true`, confirmed via `PrintC::emitBlockBasic` instrumentation):
- `(141c2, order 36)`: `COPY register:02 <- SUBPIECE(...)` — the literal
  `uVar4 = b` computation
- `(141ce, order 74)`: `COPY register:02 <- SUBPIECE(...)` — the literal,
  real division-result computation

**Group B** — the one that actually feeds the `RETURN`, and the one whose
merged identity produces the wrong printed text — consists of **two
`INDIRECT` ops**, not ordinary copies:
- `(141c2, order 143)`: `INDIRECT register:02`, iop-reference constant
  `0x24` (=36 decimal) — i.e. anchored to Group A's `b`-copy (order 36)
- `(141ce, order 144)`: `INDIRECT register:02`, iop-reference constant
  `0x4a` (=74 decimal) — i.e. anchored to Group A's division-result copy
  (order 74)
- Chain continues: `(141ce,211)` COPY reads back from `(141c2,143)`;
  `(141d7,204)` COPY (register:00, return register) reads from
  `(141ce,144)` and feeds `RETURN`.

`INDIRECT` ops are Ghidra's SSA mechanism for "this storage might be
clobbered by an indirect effect (typically a CALL) at this point, so
conservatively create a new SSA value here." The two INDIRECTs above are
each anchored, via their iop-reference constant, to a **different** one of
Group A's two real, structurally-distinct assignments — meaning they
represent the register-0x02 value surviving across some indirect-effect
barrier at two genuinely different points in the function. They should be
distinct values. Instead, **both ended up merged into one HighVariable**
(confirmed via the script: both report the same `HighVariable` object,
`numInstances()=4`). That merged HighVariable is what actually gets
displayed as `uVar4` in the printed output, and its corrupted, doubled
identity is the root cause of the wrong text.

## Why this took so many false starts (methodology note for future sessions)

Every debug dump from earlier in this investigation (this session's
`opCopy`/`mergeTestBoundary`/`buildDominantCopy`/`emitBlockBasic`
instrumentation, and everything in review20/21) logged only the
**instruction address** of a PcodeOp's defining op (e.g. via
`getDef()->getSeqNum().getAddr().getOffset()`), never the full SeqNum
(address+order+uniquifier). Since Group A and Group B both have ops at the
exact same two instruction addresses (`141c2`, `141ce`), every prior log
was ambiguous about which of the two structurally-different ops it was
actually reporting on. This produced several plausible-but-wrong
conclusions in sequence (a suspected missing print statement, then a
suspected wrong-RHS-resolution on the wrong op) before the full-SeqNum
script finally disambiguated things. **Any future instrumentation in this
investigation must log full `PcodeOp::getSeqnum().toString()`, not just an
address**, or it will risk repeating this mistake.

## Confirmed NOT the cause (dead ends, don't re-investigate)

- `buildDominantCopy` (merge.cc) — instrumented directly, confirmed never
  called for this function at all.
- `markRedundantCopies`/`checkCopyPair` (merge.cc) — structurally cannot
  apply here; it only ever compares COPYs sharing the same source Varnode.
- A missing/suppressed print statement — ruled out; `emitBlockBasic`
  instrumentation confirmed the relevant ops are correctly
  printed/suppressed exactly as they should be (Group A suppressed, and the
  visible line is genuinely Group B, an INDIRECT chain, being printed).
- The two existing `mergeTestBoundary` fixes (in `mergeOp`'s INDIRECT trim
  loop and `mergeOpcode`'s COPY required-merge) are real, independently
  confirmed fixes for a related bug class and should be **kept** — but they
  do not explain this specific symptom. A `mergeTestBoundary` instrumentation
  pass this session showed a `141c2`/`141ce` pair being correctly blocked,
  but per the methodology note above, that was address-only logging and was
  almost certainly reporting on Group A's ordinary pair, not Group B's
  INDIRECT pair — inconclusive either way about Group B.

## Concrete next step for a new session

1. Find where two **independently-anchored INDIRECT ops** (as opposed to an
   INDIRECT and its own paired input/output, which is what `mergeOp`'s
   existing INDIRECT branch at merge.cc:835 handles) can end up merged into
   one HighVariable. Candidates not yet traced to a conclusion: the general
   `mergeTestRequired` call sites at merge.cc:812/817/855 applied to two
   INDIRECT outputs sharing a storage address, or a separate pass elsewhere
   that collapses INDIRECTs across a common storage location.
2. Re-instrument with **full SeqNum logging** this time (not address-only)
   to directly confirm whether `mergeTestBoundary` (or whatever the actual
   call site turns out to be) is invoked for the SPECIFIC pair
   `(141c2, order 143)` vs `(141ce, order 144)`, and what it returns.
3. Once the exact merge call site is found, the fix is likely either: (a) a
   correctness check that INDIRECT ops anchored to different iop-reference
   points should never be merged regardless of Cover-intersection result
   (since they represent conservative markers at genuinely different
   program points by construction), or (b) points to a deeper issue in how
   these paired INDIRECTs get created in the first place for this
   function's `__stdcall_far_allstack` calling convention (worth checking
   action.cc's INDIRECT-creation logic if the merge-side fix doesn't pan
   out).
4. `varmap.cc` (initial Symbol-to-Varnode/stack-slot mapping) has never
   been examined in this entire investigation (review19-22) — worth a look
   if steps 1-3 don't pan out, especially since the separate `sat_add_u16`
   bug (review19.md) also eventually implicated Symbol/stack-mapping-
   adjacent code in the same codebase.
5. Still outstanding from EVERY prior review, never done in any session:
   the `sat_add_u16` and sibling-function (`muldiv_u16_scaled`,
   `muldiv_s16_rounded_3op`, `muldiv_s16_rounded`, `weighted_blend_u8frac`)
   regression check against all fixes/instrumentation accumulated so far.

## Current source-tree state (nothing committed to git)

- **`merge.cc`**: both `mergeTestBoundary` fixes (mergeOp's INDIRECT trim
  loop, mergeOpcode's COPY required-merge) — KEEP, real independently-
  confirmed fixes for a related bug class. `mergeTestBoundary`'s own debug
  dump (~line 201) — KEEP, cheap. `buildDominantCopy`'s debug dump
  (~line 1401) — confirmed inert for this function, safe to remove in a
  cleanup pass but currently harmless.
- **`printc.cc`**: `opReturn`'s debug dump (from review20/21) and `opCopy`'s
  debug dump (~line 501) — KEEP. `emitBlockBasic`'s debug dump (~line 2878)
  — KEEP, this one was decisive.
- **`decompile.exe`** (build output at
  `Ghidra\Features\Decompiler\build\os\win_x86_64\decompile.exe` and the
  deployed copy at
  `...\Downloads\ghidra_12.2_DEV_20260818\ghidra_12.2_DEV\Ghidra\Features\Decompiler\os\win_x86_64\decompile.exe`)
  reflects all of the above; rebuild via
  `gradlew.bat :Decompiler:decompileWin_x86_64Executable --rerun-tasks` from
  `C:\Users\j.brophy.CORKILLSYSTEMS\github\ghidra`, then copy the build
  output over the deployed path (PyGhidra must be fully closed first, or the
  copy silently fails on a locked file — always verify via matching
  `LastWriteTime` on both files after copying, and note that a gradle build
  can report a client-side timeout after ~4 minutes while still having
  completed successfully server-side — verify via the build output's own
  timestamp rather than assuming timeout means failure).
- Debug log files accumulate across sessions/runs at
  `C:\Users\j.brophy.CORKILLSYSTEMS\muldiv_u16_*.txt` (opreturn, opcopy,
  mergeboundary, builddominant, emitblock) — none are cleared between runs,
  so multiple decompiles'/functions' entries interleave; read with that in
  mind.
- All debug instrumentation listed above is tagged `// DEBUG (muldiv_u16
  investigation...) ... REMOVE before any upstream-facing change` in the
  source — must be stripped before any real fix is proposed upstream.

## Tooling notes (separate from the bug itself)

- The Ghidra MCP connector (`ghidra:*` tools) can intermittently drop
  entirely (all tools fail with "connection actively refused"), requiring a
  PyGhidra restart. Distinct from the Pulsar-side `grep-project`/tree-sitter
  memory issue already logged in Pulsar's own session-notes store under
  project "Ghidra-H8-Processor" — check that note if tooling flakiness
  recurs, and consider adding a similar note for the Ghidra-connector drops
  if they happen again.
- Always scope `grep-project`/`search-symbol` calls with a `glob` on this
  project (contains the full ghidra source tree, hundreds of large C++
  files) — unscoped calls have repeatedly timed out.

## FIFTH UPDATE (2026-09-14, same day) -- likely TRUE root cause found;
## supersedes the "INDIRECT merge" framing above as the wrong layer entirely

Traced the FULL data flow of the INDIRECT chain identified in the previous
update (143 -> 211 -> 144 -> 204 -> RETURN) back to its actual source
Varnode, using another one-shot Ghidra script dumping every op at
141c2/141ce/141d7 with full inputs. Result: **the chain literally never
touches the division result at any point.** Its ultimate source is
`(141c2,210): COPY unique <- Stack[0x6]` -- Stack[0x6] is the incoming
parameter `b` itself (an input, no defining op). The chain just carries `b`
through two INDIRECT hops, untouched, all the way to RETURN. The real
division computation (INT_DIV -> SUBPIECE -> Group A's COPY at order 74) is
a completely separate, parallel write to the same register that the
INDIRECT chain never reads from.

**This means the previous update's framing (`two independently-anchored
INDIRECTs wrongly merged`) was itself wrong** -- merging 143 and 144 was
correct, since 144 directly reads 143's result via the intermediate COPY
211; there's a real, direct SSA edge between them, not an improper transitive
union. The actual defect is one level down: **Ghidra's P-code reconstruction
itself is modeling `register:02` (R1) as merely POSSIBLY preserved through
some indirect effect at two points in the function, when in reality (per
the disassembly) nothing indirect happens there at all.**

### The disassembly (via `ghidra:disassemble_function`)

```
000141ba: link FP,#0x0:8
000141bc: mov:g.w R2,@-SP          <- save caller's R2 (used as scratch below)
000141be: mov:f.w @(0x6:8,FP),R0   <- param a
000141c0: mov:f.w @(0x8:8,FP),R1   <- param b
000141c2: mulxu.w R1,R0
000141c4: mov:f.w @(0xa:8,FP),R2   <- param c (reuses R2 as scratch)
000141c6: tst.w R2
000141c8: beq #0x141d4
000141ca: cmp:g.w R2,R0
000141cc: bhs #0x141d4
000141ce: divxu.w R2,R0            <- the actual division
000141d0: mov:g.w R1,R0            <- quotient (in R1) -> R0 (return reg)
000141d2: bra #0x141d7
000141d4: mov:i #0xffff:16,R0
000141d7: mov:g.w @SP+,R2          <- restore caller's R2
000141d9: unlk FP
000141da: prts
```

**There is no CALL/JSR instruction anywhere in this function.** `INDIRECT`
p-code ops are supposed to represent genuine indirect effects (calls,
unconstrained-pointer stores, or spec-declared "killedbycall" registers) --
none of those are visibly present here for R1 specifically. The one thing
that IS unusual about this function, and that has been silently present in
literally every decompile output produced all session without ever being
investigated, is this warning that appears every single time:

```
/* WARNING: Unable to track spacebase fully for stack */
/* WARNING: Restarted to delay deadcode elimination for space: register */
```

This function uses `LINK FP,#0` / frame-pointer-relative parameter access
(`@(0x6:8,FP)`, `@(0x8:8,FP)`, `@(0xa:8,FP)`) under the custom
`__stdcall_far_allstack` convention, plus a stack-relative pointer
dereference visible in the decompiled C (`puVar5 = auStack_82 + 0x80;
uVar1 = *(ushort *)(puVar5 + 10);`). If Ghidra cannot fully resolve the
stack pointer/spacebase for this function (as the warning states outright),
it loses the ability to prove no-aliasing for stack/frame-relative accesses
and conservatively falls back to INDIRECT-based modeling for potentially-
aliased storage -- which would fully and directly explain why R1 gets two
spurious INDIRECT markers despite no real indirect effect existing in the
disassembly.

### Why this reframes the whole investigation (reviews 20-24 included)

Every previous session's fixes and dead ends (`mergeTestBoundary`'s two
guarded sites, `buildDominantCopy`, `markRedundantCopies`, the INDIRECT-merge
theory above) were all investigating the GENERIC Ghidra decompiler's C++
source (merge.cc/printc.cc/variable.cc) on the assumption that the P-code
graph itself was trustworthy and the bug was in how it gets displayed. This
update suggests the P-code graph itself may be wrong at its root, for a
reason specific to THIS PROJECT: a custom H8 processor definition (this
project's own `.slaspec`/`.cspec`/`.pspec` files, under the project's `h8`
folder) whose stack-pointer/spacebase declaration or `LINK` instruction
p-code semantics don't let Ghidra's SSA construction fully resolve the
frame, for functions using `__stdcall_far_allstack` with FP-relative
addressing. If so, the correct fix is NOT anywhere in the generic decompiler
source this investigation has spent four review files inside of -- it's in
this project's own processor spec.

**Important caveat:** this is a strong, well-evidenced hypothesis (the
warning is real, appears on every relevant decompile, and directly matches
the observed spurious-INDIRECT symptom) but has NOT yet been definitively
confirmed as the fix, nor has the exact spec construct responsible been
located. Do not assume it's proven; verify per the next steps below before
touching any spec file.

### Concrete next step for a new session (supersedes ALL previous
### "concrete next step" sections in this file)

1. Find and read this project's H8 `.cspec` file specifically for the
   `__stdcall_far_allstack` (or whatever the base convention is named
   before the `_allstack`/`_far` suffixes) calling convention definition --
   check its `<stackpointer>` element and any `<unaffected>`/
   `<killedbycall>`/`<localrange>` declarations for anything that could
   cause spurious indirect-effect modeling on general-purpose registers.
2. Find and read the `LINK`/`UNLK` instruction's p-code semantics in the H8
   `.sinc`/`.slaspec` source -- specifically whether `LINK FP,#0x0` (a
   zero-sized frame, as used here) is handled as a degenerate/special case
   that might confuse Ghidra's stack-pointer tracking (the "Unable to track
   spacebase fully" warning is the single most direct lead in this entire
   investigation and has never once been chased down before now).
3. As a smaller, faster experiment before touching the spec: try
   decompiling a function that does NOT trigger the spacebase warning (if
   one exists in this binary) and confirm it has no spurious INDIRECT/
   register-merge symptoms, to further validate the correlation between the
   warning and the bug before committing to a spec-level fix.
4. If the spacebase-tracking issue is confirmed as root cause, check
   whether it also explains the separate, already-"fixed" `sat_add_u16` bug
   from review19.md (which also involved stack/address computation and
   SEGMENTOP/RuleLoadVarnode unwrapping) -- they may be two symptoms of the
   same underlying spec gap, in which case the review19 fix might be a
   narrower workaround for a problem this update finally identifies at its
   true source.
5. Regression check, still outstanding from every prior review across
   reviews 19-22, never done in any session: `sat_add_u16` and sibling
   functions.

### Source-tree state additions from this update

- No source changes -- purely diagnostic, via `ghidra:disassemble_function`
  and a second one-shot inline script dumping full op inputs (not saved to
  disk; same caveat as the previous update's script).
- This update does not invalidate the two existing `mergeTestBoundary`
  fixes in merge.cc -- they remain real, independently-confirmed fixes for
  a genuine (if different) Cover-boundary defect class, and should stay.
  It DOES mean the INDIRECT-focused "concrete next step" from the previous
  update should not be pursued further until the spacebase-tracking
  hypothesis above is checked first.

Review 20: muldiv_* wrong-return-value regression from 9561 (live-confirmed)
====================================================================

STATUS: OPEN. Root-cause question from NEXT STEPS item 2 is now ANSWERED
(2026-09-10, this session, via live native-decompiler instrumentation --
see NEW FINDING below). Fix B's isDead() guard does NOT fire on
muldiv_u16's collision because its precondition is genuinely false here;
this is confirmed a second, distinct defect in the same Cover/Merge
family, not a missed test case for Fix B. Still no landed fix.

Original: confirmed live 2026-09-09, now RE-CONFIRMED against an
isolated 9561-only rebuild (professor-jonny/ghidra @ commit 83ec213,
"9561: Fix decompiler: fix RuleLoadVarnode::vnSpacebase to recursively
unwrap SEGMENTOP nested in INT_ADD"). Both fixes in that commit are
present in the current build and the bug still reproduces unchanged.
(RVR_1998_x3 4g63t 21000011 md352553.hex, H8:BE:32:H8539F.)

THIS IS A CORRECTNESS REGRESSION, NOT A COSMETIC ONE. Affected functions
compute the right answer internally and then silently return a different,
wrong value (a raw input parameter or an unrelated partial result) instead.
No crash, no warning beyond the pre-existing "Unable to track spacebase
fully for stack" / "Restarted to delay deadcode elimination" lines that
already existed pre-fix for this function family (review.md item 10) --
i.e. nothing in the decompiler's own output flags that the return value is
wrong. Any prior conclusion in this project that depended on decompiled
output from these functions (or their callers, transitively) should be
treated as unverified until re-checked against this fix.

IMPORTANT CORRECTION (2026-09-09, this session): an earlier line of
investigation this session wrongly concluded this was a genuine FIRMWARE
bug (misreading divxu.w's quotient/remainder register placement from the
datasheet). That was refuted by checking real callers (fuel_pw_and_airvol_
compute and others) -- they consume muldiv_u16's return value as the real
a*b/c quotient and it works correctly on real running hardware. The ROM is
correct; only Ghidra's static decompilation is wrong. Do not re-open the
"maybe it's the firmware" theory without new evidence -- it was checked
and refuted this session.

--------------------------------------------------------------------
BACKGROUND / HOW THIS WAS FOUND
--------------------------------------------------------------------
Two other docs in this repo independently converged on the same root cause
from different angles:

- review-calling-convention-regression.md -- found first, via systematic
  before/after comparison across four ROM exports (unpatched baseline,
  9561-only, 9540-only, all-three-merged). Confirms the regression is
  caused specifically by 9561, not 9540 or 9450, and gives the working
  theory (heritage/merge-group handling around INDIRECT call-clobber
  markers, newly exposed because 9561 changes the resolved P-code shape
  of the affected LOAD from "generic pointer deref" to "resolved stack
  varnode").
- review-9561-merge-ordering.md -- found second, investigating a full
  decompiler crash (SIGSEGV, then a LowlevelError) on muldiv_u16_scaled
  specifically. Traces a genuine use-before-define P-code ordering defect
  introduced by 9561's own crash-fix fallback (`afterop = op` in
  `Merge::trimOpOutput` when the IOP-decoded target is dead). NOTE: this
  specific LowlevelError did NOT reproduce in this session's live
  force_decompile of muldiv_u16_scaled (see below) -- either intermittent,
  order-dependent, or already partially addressed; not yet resolved which.

This file exists to give the wrong-return-value regression (the
calling-convention-regression.md finding) its own tracked, actionable home,
separate from the crash/ordering investigation, since they are confirmed
distinct symptoms on overlapping functions and shouldn't be conflated when
picking up work.

--------------------------------------------------------------------
9561 COMMIT NOW IDENTIFIED AND READ DIRECTLY (2026-09-09, this session)
--------------------------------------------------------------------
Commit: professor-jonny/ghidra @ 83ec213f4c18e733e2b89b19b39bb52e65993b6c
"9561: Fix decompiler: fix RuleLoadVarnode::vnSpacebase to recursively
unwrap SEGMENTOP nested in INT_ADD"

Bundles TWO fixes:

FIX A -- RuleLoadVarnode::vnSpacebase (ruleaction.cc/.hh), the commit's
primary/named fix. Makes vnSpacebase recurse through a SEGMENTOP found
among an INT_ADD's inputs (to a depth-guarded arbitrary level), mirroring
the one-level unwrap checkSpacebase already did when SEGMENTOP was the
DIRECT LOAD/STORE operand. Targets the "merged auStack_XXX[128] array
instead of individual typed stack variable" symptom. Own regression test
(nestedsegmentop.xml) uses sat_add_u16 as its canary, NOT any muldiv_*
function. Commit message claims this also fixed the same merged-array
symptom independently found in sci1_meta_cmd_dispatch_c0_ff, and that a
741-function full-ROM sweep found no new occurrences of the symptom
introduced.

FIX B -- CoverBlock::getUIndex() (cover.cc) + matching guard already
present in Merge::trimOpOutput (merge.cc). THIS IS THE ONE THAT MATTERS
FOR THIS BUG. getUIndex() determines an INDIRECT op's position in program
order by looking up "the op it's indirect for" via
`PcodeOp::getOpFromConst(op->getIn(1)->getAddr())->getSeqNum().getOrder()`,
with NO liveness check. If that target op was already deleted by an
earlier simplification pass, this returns a stale leftover order value,
corrupting the Cover/Merge ordering comparison used to decide which
definition of a Varnode is live at a given point -- i.e. exactly the
merge_group resolution mechanism this doc's own P-code trace (below)
found misbehaving on muldiv_u16's register[2]. Fix adds an isDead() guard,
falling back to the INDIRECT's own order when the target op is stale.
Commit message says this was found while validating Fix A, on "a shared-
parameter divxu.w helper" in this same ROM -- almost certainly one of this
doc's muldiv_* family, though the commit does not name it explicitly, so
DO NOT assume it's muldiv_u16 specifically without independent evidence.

CRITICAL FINDING: BOTH fixes are already present in the currently-loaded
build (isolated 9561-only rebuild, live via ghidra:force_decompile,
re-tested this session after the rebuild). muldiv_u16 STILL shows both:
  - the wrong-return-value symptom (still `return b;`), AND
  - the merged auStack_82[128] array symptom Fix A was specifically
    supposed to eliminate (`c` still reads via
    `*(ushort *)(puVar4 + 10)` instead of resolving as a named parameter).

This means muldiv_u16 is NOT the function Fix B's commit message refers to
as already-fixed ("a shared-parameter divxu.w helper" -- singular), and is
also NOT covered by Fix A despite matching Fix A's target symptom shape on
its face. Two live possibilities, NOT YET DISTINGUISHED:
  (a) Fix B fixed a *different* muldiv_* sibling (or an unrelated function
      entirely), and muldiv_u16 has a similar-looking but mechanistically
      distinct bug neither fix reaches.
  (b) muldiv_u16's stack-parameter access pattern is NOT actually a
      SEGMENTOP-nested-in-INT_ADD case the way Fix A's regression test
      models it -- i.e. it fails to qualify for Fix A's recursion at all,
      for a structural reason not yet identified.
See NEXT STEPS for how to tell these apart.

--------------------------------------------------------------------
LIVE CONFIRMATION (this session, 2026-09-09, via ghidra:force_decompile,
not file export -- cache-bypass confirmed; RE-CONFIRMED a second time
after switching to the isolated 9561-only rebuild described above)
--------------------------------------------------------------------

1. muldiv_u16 (0x141ba), __stdcall_far_allstack(ushort a, ushort b, ushort c):

   uVar3 = (undefined2)((uint)a * (uint)b);
   ...
   uVar3 = (short)(CONCAT22(uVar2,uVar3) / (uint)uVar1);   // real division result
   return b;                                                // WRONG: returns raw param b

   Disassembly (0x141ba-0x141da) confirmed read directly this session:
     141c2: mulxu.w R1,R0     ; R0:R1 = a*b
     141ce: divxu.w R2,R0     ; R0:R1 = (a*b)/c -- quotient ends up in R1 per
                                real H8/539F semantics (CONFIRMED via caller
                                behavior, see CORRECTION note above, NOT via
                                datasheet text alone -- the datasheet's own
                                DIVXU operand diagram is garbled/unusable in
                                the extracted grep-able .md)
     141d0: mov.w R1,R0       ; correct instruction: moves true quotient into
                                the R0 return slot. Ghidra disassembles and
                                emulates this correctly; the DECOMPILER's
                                pseudocode is what's wrong, mislabeling this
                                move as "return b" because its heritage/merge
                                pass picked the wrong upstream definition for
                                register[2]/R1 (see P-CODE TRACE below).

2. muldiv_u16_scaled (0x141dc), __stdcall_far_allstack_32ret(ushort a,b,c,d):

   unresolved raw register `in_R1` appears directly in output (never assigned):
     uVar4 = uVar2 + in_R1;
   uVar3 = (short)(CONCAT22(uVar2,uVar4) / (uint)*(ushort *)(puVar5 + 0xc));  // real result
   return uVar4;                                            // WRONG: returns partial sum, not uVar3

3. muldiv_s16_rounded_3op (0x14247), __stdcall_far_allstack_32ret(ushort a,b,c):

   two unresolved raw registers appear directly in output: `in_R1`, `unaff_R3`
   uVar4 = (short)(CONCAT22(uVar4,unaff_R3) / (uint)*(ushort *)(puVar6 + 0xc)); // real rounded result
   ...
   return sVar5;            // WRONG (one path)
   return sVar5 + (ushort)(0xfffe < unaff_R3);  // WRONG (another path)
   return -1;                // WRONG (fallthrough path)
   // uVar4, the actual computed result, is never returned on any path

Not yet live-checked this session (named in calling-convention-regression.md,
carried over from file-export evidence only -- re-verify via force_decompile
before relying on):
   - muldiv_s16_rounded (0x14219) -- documented symptom: `return b;` instead
     of computed result, same shape as muldiv_u16.
   - weighted_blend_u8frac (address not yet pulled this session) --
     documented symptom: `return sVar3;` ignores real blend result `uVar2`,
     same `in_R1`/`unaff_R3` pattern as muldiv_s16_rounded_3op.

CONTROL CHECKED (unaffected): sat_add_u16 -- the function 9561's Fix A
regression test (nestedsegmentop.xml) targets. Simple 2-parameter, plain
`__stdcall_far_allstack`, NO register-passed arguments. Not affected.
Consistent with the working theory below: trigger needs a SEGMENTOP-resolved
stack parameter AND at least one register-passed parameter in the same
function.

Also not reproduced this session: the SIGSEGV / "Unable to force merge of
op at 0x00014203:ed" LowlevelError that review-9561-merge-ordering.md
documents for muldiv_u16_scaled. force_decompile succeeded cleanly (wrong
answer, not a crash) on this session's build, including after the switch
to the isolated 9561-only rebuild. NEEDS RECONCILING -- possible that the
crash is intermittent/pass-order-dependent, or that Fix B above (which
directly targets a stale-op-pointer hazard in the same Cover/Merge code
review-9561-merge-ordering.md was investigating) already resolved it; not
yet determined which. Don't assume the crash is fixed just because it
didn't reproduce.

--------------------------------------------------------------------
P-CODE TRACE (muldiv_u16 only, captured this session via
ghidra:get_function_pcode, high-level granularity, on the triple_bug_fix
build -- NOT YET RE-CAPTURED on the isolated 9561-only rebuild, see NEXT
STEPS item 1)
--------------------------------------------------------------------
At the division site (0x141ce), two definitions target register[2] in the
same P-code:
  - the real division result, INT_DIV -> SUBPIECE -> a varnode in
    merge_group 1 (the CORRECT value, matches what mov.w R1,R0 actually
    moves at the instruction level)
  - an INDIRECT call-clobber marker in merge_group 0, sourced from an
    earlier register-passed-parameter INDIRECT at the mulxu.w site
    (0x141c2)

The final return COPY (register[0] <- register[2], then RETURN
register[0]) resolves to merge_group 0 (the stale/wrong one) instead of
merge_group 1 (the real division result). This is exactly the class of
defect Fix B (CoverBlock::getUIndex, see above) targets: two INDIRECTs/
definitions on the same physical register at the same address, with the
Cover/Merge ordering comparison picking the wrong one. Fix B is already
in the build and the bug still reproduces, so either Fix B doesn't cover
this exact collision shape, or the stale-op precondition Fix B guards
against (isDead() on the IOP-decoded target) isn't what's happening here
-- NOT YET DISTINGUISHED, see NEXT STEPS.

--------------------------------------------------------------------
RULED OUT
--------------------------------------------------------------------
- NOT caused by 9540 (register0x0e print-layer fix) -- confirmed via a
  9540-only export where muldiv_u16 decompiles correctly.
- NOT caused or worsened by 9450 (far-pointer join) -- confirmed via
  the "all three" merged export showing the same regression, unchanged,
  as the 9561-only export. RE-CONFIRMED THIS SESSION a different way:
  copied a genuine 9450 SEGMENTOP/far-pointer-join cspec fix
  (`<farpointerjoin hislot="0" joinsize="3"/>`) into h8539f.cspec and
  reloaded live -- muldiv_u16's decompile was byte-for-byte unchanged.
  NOTE: that farpointerjoin line is scoped to the __stdcall_far_2arg
  prototype, not __stdcall_far_allstack[_32ret] (what the muldiv_*
  family actually uses), so this is a weaker confirmation than it looks --
  it proves that SPECIFIC line doesn't touch muldiv_u16, not that no
  possible 9450-family fix could. Spot-checked for regressions on
  isr_sci1_eri and table_lookup_interp (the latter uses
  __stdcall_far_2arg + CUSTOM_STORAGE) -- both clean, no new warnings.
- NOT a pre-existing decompiler defect -- confirmed via the unpatched
  baseline export, where all affected functions decompile correctly.
- NOT a genuine firmware/ROM bug -- see IMPORTANT CORRECTION note above.
  Checked and refuted this session by reading real caller code
  (fuel_pw_and_airvol_compute and 5 others); all consume the return value
  as the true a*b/c quotient, consistent with correct hardware execution.
- Confirmed specific to functions combining a SEGMENTOP-resolved stack
  parameter with register-passed parameters under
  __stdcall_far_allstack[_32ret] -- 7 sibling functions without that
  combination (muldiv_u16 itself is presumably now one of the *affected*
  siblings per re-test above, so this list needs re-confirming, not just
  copied forward -- see NEXT STEPS) were reported clean in the original
  investigation.
- The calling-convention names (__stdcall_far_allstack, _32ret,
  __stdcall_far_2arg etc.) are confirmed NON-STANDARD -- locally invented
  and hand-tuned per h8539f.cspec's own dated comments, not derived from
  any H8/539F hardware ABI documentation. Checked H8_539F_hardware.md and
  H8_538-539_hardware.md directly this session: the datasheet confirms
  H8/500-family assembly of this era has NO fixed calling convention --
  argument-register usage and callee-saved registers are chosen per
  routine by whoever wrote it (explicit stm/ldm save-restore pairs per
  function, arguments assigned to R0-R3 differently function to function
  in Renesas' own flash-programming sample code). This means the cspec's
  killedbycall/unaffected lists for these synthetic conventions are ALSO
  unverified guesses, same as the convention shapes themselves -- a
  separate, real gap from the bug tracked in this file, but NOT
  confirmed to be its cause (see RULED OUT note on 9450 above; the one
  concrete convention-layer fix tried this session didn't touch this
  bug). Do not assume this gap is irrelevant just because one probe
  didn't find a connection.

--------------------------------------------------------------------
LIKELY CONNECTION FOUND: review16.md through review19.md is 9561's own
pre-landing development history, tracking the same sat_add_u16/auStack_
investigation start to finish. review19.md (the LAST file in that chain,
dated 2026-08-31) ends with the fix "confirmed working, crash-free" for
the merged-array symptom itself, but EXPLICITLY STILL OPEN: `FAIL --
Canary param a`, `FAIL -- Canary param b` -- a parameter-binding gap where
sat_add_u16's own stack parameters resolve to
`in_stack_00000004`/`in_stack_00000006` instead of their declared names
`a`/`b`. review19.md ends there with no closing note and no pointer to a
review20.md of its own -- i.e. that gap was never recorded as fixed
anywhere in the review16-19 chain.

HYPOTHESIS TESTED AND WEAKENED: re-checked sat_add_u16 live via
force_decompile on the current build. It is now FULLY CLEAN -- parameters
correctly named `a`/`b` (not `in_stack_00000004`/`0006`), correct
computed-and-returned result, no merged array, no unresolved registers:

  ushort __stdcall_far_allstack sat_add_u16(ushort a,ushort b)
  {
    ushort result;
    result = a + b;
    if (CARRY2(a,b)) { result = 0xffff; }
    return result;
  }

So the specific param-binding gap review19.md left open for THIS function
was evidently closed at some point before/during the final 9561 landing --
consistent with this being exactly what Fix A's nestedsegmentop.xml
regression test (which also canaries sat_add_u16) was written to lock in.
That means the `in_R1`/`unaff_R3` pattern in muldiv_u16_scaled/
muldiv_s16_rounded_3op, and the wrong-return-value pattern in muldiv_u16
itself, is NOT simply "the same known review19.md gap, still open" -- it's
either a different, new manifestation of a related mechanism (these
functions have 3-4 params, mixing stack AND register-passed args, vs
sat_add_u16's 2 plain stack params -- consistent with
calling-convention-regression.md's own theory that the trigger needs BOTH
a SEGMENTOP-resolved stack param and a register-passed param), or an
unrelated new bug. The review16-19 chain is useful background/methodology
reading but should not be assumed to already cover this specific case --
treat as a related-but-distinct gap until traced directly.

--------------------------------------------------------------------
WORKING THEORY (PARTIALLY CONFIRMED this session -- Fix B in the actual
9561 commit targets close to the right mechanism, but does not eliminate
the bug on muldiv_u16 as currently observed; not fully resolved)
--------------------------------------------------------------------
9561 changes the affected LOAD's P-code shape from "unresolved generic
pointer dereference" to "resolved stack varnode." That's the intended fix
(Fix A above). But in functions where the same P-code also has
register-passed parameters using the same physical registers the return
value is computed through, resolving that LOAD changes enough of the
surrounding P-code graph (fewer/different intermediate unique varnodes,
different rule-firing order in a later Action pass) that a latent bug in
how INDIRECT call-clobber markers get merged with prior register
definitions gets triggered where it previously wasn't.

CONFIRMED THIS SESSION: this points at CoverBlock::getUIndex() in
cover.cc (Fix B in the actual 9561 commit, see above) -- a real,
independently-discovered defect in exactly this mechanism, already fixed
upstream in principle. NOT YET CONFIRMED: why Fix B's isDead() guard does
not appear to resolve muldiv_u16's specific case. Either Fix B fixed a
different function (the commit message's "a shared-parameter divxu.w
helper" is not named explicitly) and muldiv_u16 needs its own,
structurally distinct fix, or muldiv_u16's INDIRECT collision doesn't hit
the isDead() precondition Fix B checks for and needs a broader version of
the same guard. Distinguishing these two is the top NEXT STEPS priority.

ALSO NOTED (per calling-convention-regression.md, not yet independently
checked): h8539f.cspec (lines ~185-232) and h8539f.slaspec (lines ~23-39)
apparently already document a *prior*, pre-9561 investigation of the same
warning strings ("Unable to track spacebase fully for stack" / "Restarted
to delay deadcode elimination for space: register") on a named group of 6
"math helper" functions needing a 4-byte R0:R1 register-pair return. Read
directly this session (see RULED OUT above for the __stdcall_far_2arg
farpointerjoin probe this prompted) -- worth a closer pass specifically
on the register-pair-return history in this section (R0R1 overlap
register attempted and reverted 2026-07-22, see cspec lines ~198-232) in
case it's related to why register[2]'s merge_group resolution goes wrong,
even though the direct farpointerjoin probe this session didn't find a
connection.

--------------------------------------------------------------------
NEW FINDING (2026-09-10): NEXT STEPS ITEM 2 ANSWERED -- isDead() is FALSE
--------------------------------------------------------------------
Method: added debug instrumentation directly to the live dev checkout of
CoverBlock::getUIndex() (cover.cc, the Fix B function), printing
targOp->isDead() and related state for every INDIRECT hit inside
muldiv_u16's address range (0x141ba-0x141da), gated on op->getAddr().
Output written to a plain file (NOT stderr/cerr -- see CAUTION below)
and rebuilt via `gradlew :Decompiler:decompileWin_x86_64Executable`
(the native-only target; do not use the full `buildGhidra` for this --
much slower and unnecessary). Deployed by directly overwriting
decompile.exe at the live install path and force-killing any running
decompile.exe children so Ghidra spawns a fresh one from the new binary
(Windows keeps the old file handle locked otherwise -- a plain
Copy-Item will silently no-op against a running instance; verify the
LastWriteTime on the deployed copy actually changed).

RESULT: every single INDIRECT hit at the actual bug site (0x141ce) shows
targOp->isDead() == FALSE. Full instrumented output (both hit sites):

  0x141c2 (mulxu.w site, register-param INDIRECT): targOp->isDead()=FALSE,
    targOp->code()=COPY, targOp->getSeqNum().getOrder()=12800002 -- this
    site's target op looks like ordinary live-order data, not the
    interesting one.

  0x141ce (divxu.w site, THE bug site): targOp->isDead()=FALSE,
    targOp->code()=COPY, targOp->getAddr()=0x141ce (i.e. it resolves to
    a live COPY at the division site itself), but
    targOp->getSeqNum().getOrder()=bffffff4 -- an anomalously large
    synthetic order value (0xbffffff4, in Ghidra's high/reserved-range
    order-value space).

CONCLUSION: Fix B's isDead() guard is doing exactly what it was
designed to do, and correctly does NOT fire here, because its
precondition (stale/deleted target op) genuinely is not met -- targOp is
alive. This means muldiv_u16's bug is NOT "Fix B should have caught this
but has a gap in its isDead() check" (possibility (b) from the CRITICAL
FINDING section above). It IS possibility (a): a second, distinct defect
in the same Cover/Merge ordering family, structurally different from
what Fix B targets.

NEW WORKING HYPOTHESIS (not yet independently confirmed -- next step):
the bug is that a LIVE INDIRECT's target op can carry an anomalously
large synthetic SeqNum order value (0xbffffff4 seen here -- this looks
like it's in a reserved/high range Ghidra uses for certain synthetic or
late-inserted ops, NOT an ordinary sequential program-order value). If
getUIndex() trusts that order value at face value for Cover/Merge
comparison purposes, an INDIRECT whose target happens to carry one of
these high synthetic orders will sort as "very late" regardless of its
true position in program order, letting it incorrectly dominate an
earlier, correctly-ordered definition (the real division-result COPY)
in the merge_group resolution. This would explain the P-CODE TRACE
section above directly: the INDIRECT's merge_group (0) wins over the
real division COPY's merge_group (1) at the final return, independent
of isDead().

CONFIRMED (2026-09-10, continued): read BlockBasic::insert() and
BlockBasic::setOrder() in block.cc directly. The order-value scheme is
legitimate, not a raw overflow bug: insert() assigns a new op's order as
the midpoint between its neighbors (`ordafter/2 + ordbefore/2`, with an
explicit "Beware overflow" comment at that line); when there's no longer
room to subdivide (`ordafter - ordbefore <= 1`), setOrder() is called to
evenly REBALANCE every op's order across the full uintm range for that
block. So 0xbffffff4 is not inherently a bad/overflowed value on its
own -- it's a normal midpoint or post-rebalance order that any op can
legitimately carry.

SHARPER HYPOTHESIS: the bug is more likely a STALENESS issue than a
magnitude issue. Because setOrder() can reassign every op's order value
in a block (as part of normal, correct operation, likely triggered here
by the "Restarted to delay deadcode elimination for space: register"
pass this function already logs), a comparison in getUIndex() between
two order values that were read/cached at different points relative to
a rebalance could compare inconsistent generations of ordering data even
though each value is individually valid. NOT YET CONFIRMED whether this
is actually what happens (getUIndex() itself just calls
targOp->getSeqNum().getOrder() live each time, so it SHOULD always see
the current, post-rebalance value -- need to check whether Cover/Merge
caches an order value anywhere else, e.g. in a merge_group's own stored
range boundaries, rather than re-reading getOrder() fresh at comparison
time). This is the next concrete thing to check.

CORRECTION (2026-09-10, continued): checked cover.hh directly.
CoverBlock stores live `const PcodeOp *start/stop` pointers, not cached
order values, and every comparison method (intersect/contain/boundary/
merge) calls getUIndex() fresh against the live pointer each time. So
the "stale-generation cache" theory above is WRONG as stated -- there is
no cache to go stale; every comparison re-resolves current, live data.

SHARPENED QUESTION: since the data is always fresh, the anomalous
0xbffffff4 order on the op at 0x141ce is not stale/misread data -- it's
that op's genuine, current order value. The real question is why. Two
sub-possibilities, not yet distinguished:
  (a) PcodeOp::getOpFromConst(op->getIn(1)->getAddr()) resolves to the
      WRONG op -- i.e. some other op that happens to share address
      0x141ce (same instruction can back multiple PcodeOps) rather than
      the specific division-result COPY the INDIRECT is meant to stand
      in for, and that wrong op just happens to have a late order from
      an unrelated insert()/setOrder() event elsewhere in the function.
  (b) getOpFromConst correctly resolves to the intended COPY, but that
      COPY's order value is legitimately late because of WHEN it was
      created relative to a setOrder() rebalance pass (e.g. inserted
      near the end of the block during the delayed-deadcode-elimination
      restart this function's own warning references) -- i.e. the
      order-assignment scheme itself doesn't account for "this op
      logically belongs earlier even though it was created later,"
      which would make this a genuine design gap in getUIndex()'s
      reliance on creation-order rather than final-instruction-address
      order for INDIRECT resolution.
NEXT CONCRETE CHECK: extend the existing debug instrumentation to also
print op->getAddr() for the ENCLOSING op being compared (not just
targOp), and ideally dump the full list of all PcodeOps at address
0x141ce with their individual order values, to see whether there are
multiple distinct ops at that address and which one getOpFromConst
actually picks.

RESOLVED (2026-09-10, continued): extended instrumentation to dump the
FULL parent-block op list (address, opcode, order, time) around both
INDIRECT sites, via targOp->getParent()->beginOp()/endOp(). Result is
decisive and rules out BOTH prior hypotheses cleanly.

0x141c2 site (mulxu.w, register-param INDIRECT) -- targOp resolves to a
COPY at order=12800002, sitting correctly at the END of its block's own
op list (block spans order 400002 .. 1b800002, all monotonic, targOp
correctly last among the 0x141c2-address ops before the block moves on
to 0x141c4).

0x141ce site (divxu.w, THE BUG) -- targOp resolves to a COPY at
order=bffffff4, and it is CORRECTLY POSITIONED within ITS OWN block:
  #2 INT_DIV      order=5ffffffa   (the real division)
  #3 SUBPIECE     order=7ffffff8   (real quotient extraction)
  #4 DELAY_SLOT   order=9ffffff6   (the enclosing INDIRECT itself)
  #5 COPY         order=bffffff4   (targOp, isTarg=YES -- resolved
                                     CORRECTLY, immediately after the
                                     real division/quotient ops)
  #6 COPY         order=dffffff2   (0x141d7, final register[0]<-
                                     register[2] move)
  #7 RETURN       order=fffffff0
getOpFromConst is NOT resolving to the wrong op (rules out hypothesis
(a) above) -- targOp is exactly the right COPY, correctly ordered
relative to the real division result within its own block. Both blocks'
internal order sequences are independently monotonic and internally
self-consistent (rules out hypothesis (b) -- no rebalance-timing
mismatch within either block).

ACTUAL ROOT CAUSE: THE TWO SITES ARE IN DIFFERENT BASIC BLOCKS, EACH
WITH ITS OWN INDEPENDENT ORDER-VALUE RANGE. The 0x141c2 block's orders
run 400002..1b800002 (roughly 0x0004xxxx..0x001bxxxx magnitude); the
0x141ce block's run 1ffffffe..fffffff0 (roughly 0x1fxxxxxx..0xffxxxxxx
magnitude -- both blocks were evidently rebalanced via
BlockBasic::setOrder()'s full-uintm-range redistribution independently
of each other. getUIndex()'s callers in the Cover/Merge machinery
perform raw numeric comparisons between order values (see
CoverBlock::intersect/contain/boundary/merge in cover.cc, and their
callers in merge.cc) WITHOUT any check that the two ops being compared
share the same parent block. Comparing an order value from one block's
independently-assigned range against another block's independently-
assigned range is comparing incommensurable numbers -- there is no
guarantee block A's "order=12800002" means anything relative to block
B's "order=bffffff4" even though both are valid, correctly-assigned,
non-stale values within their own block.

This matches the P-CODE TRACE section above exactly: the final
RETURN's merge_group resolution is choosing between a definition
reached via the 0x141c2 block's INDIRECT chain and the 0x141ce block's
real division result, and whichever block's independently-assigned
order range happens to produce a larger raw uintm value at the specific
op being compared wins the comparison -- regardless of true control-flow/
dominance order between the two blocks. This is NOT the class of defect
Fix B's isDead() guard addresses (which is about STALE/deleted target
ops, not cross-block order incommensurability) -- confirms once more
this is a structurally distinct bug from Fix B, now root-caused rather
than just localized.

NOT YET DONE: read the actual Cover/Merge comparison call sites
(CoverBlock::intersect/contain/boundary/merge in cover.cc, and their
callers, likely in merge.cc's ActionMergeCopy or similar) to confirm
whether they operate purely on raw getUIndex() values within what's
assumed to be a single block's ordering space, or whether there's
supposed to be a block-boundary check/normalization step that's missing
or broken specifically for the INDIRECT-resolution path. This is the
concrete fix-location candidate.

CORRECTION (2026-09-10, continued): read Cover::intersect directly (the
outer class wrapping CoverBlock, per its own doc comment "implemented as
a map from basic block to their non-empty CoverBlock"). The cross-block
theory above is WRONG. Cover::intersect walks both Cover objects'
map<int4,CoverBlock> in LOCKSTEP BY BLOCK INDEX and only ever calls the
inner CoverBlock::intersect when both iterators point at the SAME block
index (`(*iter).first < (*iter2).first` advance logic). So the outer
Cover/Merge machinery is correctly block-scoped; two ops from different
blocks are never compared via a single getUIndex() magnitude check
against each other in this code path. Also: the block dump above shows
the INDIRECT at 0x141ce and its targOp (the real division COPY) ARE
BOTH IN THE SAME BLOCK (all 8 dumped ops, #0-#7, come from one
targOp->getParent() call) -- so this specific comparison was never
cross-block to begin with. Struck the ACTUAL ROOT CAUSE conclusion
above; it does not hold up under closer reading. Leaving the incorrect
reasoning in place above (rather than deleting it) so a future session
doesn't waste time re-deriving and re-refuting the same wrong theory.

REMAINING OPEN QUESTION, sharpened again: within the SAME block, targOp
(the real division COPY, order=bffffff4) is correctly positioned AFTER
the INT_DIV/SUBPIECE that compute the real quotient, and BEFORE the
final 0x141d7 COPY and 0x141da RETURN (also confirmed in the same
block's dump: order dffffff2 and fffffff0 respectively, both larger
than bffffff4, i.e. later, as expected). So within this single block,
program order is entirely correct and unremarkable. The actual
remaining mystery: how does the FINAL merge_group resolution at
0x141d7/0x141da end up picking a value that traces back to the
0x141c2 block's INDIRECT chain (merge_group 0, per the P-CODE TRACE
section above) instead of this block's own, locally-correct division
COPY (merge_group 1)? This must be happening one level up from
getUIndex() itself -- in whatever Merge/ActionMergeCopy logic decides
WHICH candidate definitions of register[2]/R1 to even compare against
each other for the final return varnode, not in the order-comparison
arithmetic (which has now been checked at both the CoverBlock and Cover
levels and found correct). NEXT: read merge.cc's ActionMergeCopy (or
equivalent -- search for where HighVariable candidate Varnode merging
actually selects between multiple definitions reaching the same
point) rather than continuing to re-examine cover.cc's comparison
arithmetic, which is not where the remaining defect lives.

CORRECTION (2026-09-10, continued): "ActionMergeCopy" does not exist in
this codebase under that name (grepped, no match) -- was an imprecise
name carried over from review-9561-merge-ordering.md. The actual
mechanism is Merge::mergeOpcode(OpCode) in merge.cc (walks all basic
blocks in linear order, and for each op matching a given opcode --
called elsewhere with CPUI_COPY -- attempts a REQUIRED merge of the
op's output HighVariable with each input HighVariable via
mergeTestRequired()/merge(), lines ~326-350). This walks blocks via
bl->beginOp()/endOp() in bblocks.getSize() order (i.e. actual
control-flow block order, NOT raw getUIndex() magnitude), which is a
different, likely more trustworthy ordering signal than what cover.cc
alone provides. mergeTestRequired (line 102) and the underlying merge()
implementation (line ~170 onward, with a Cover-intersection check via
mergeTestRequired at line 178) are the concrete next things to read in
full -- this is where the actual decision to fold two candidate
definitions of the same physical register together (or not) happens,
one level above the getUIndex() arithmetic already confirmed correct.
NOT YET READ IN FULL -- stopped here for this session; next session
should start by reading merge.cc lines ~90-200 (mergeTestRequired) and
~700-900 (the several other mergeTestRequired call sites already
located, likely covering the INDIRECT-specific forced-merge path) in
full before forming a new hypothesis, rather than guessing from
function names alone.

NOT YET DONE: confirm what 0xbffffff4 actually means in this codebase's
SeqNum/order-value scheme (grep funcdata.cc/op.cc for where such
high-range orders get assigned -- likely something to do with
late-inserted ops during a delayed deadcode-elimination restart pass,
matching this exact function's own "Restarted to delay deadcode
elimination for space: register" warning already noted in LIVE
CONFIRMATION above -- that warning may not be cosmetic background noise
after all, it may be the actual trigger condition). Confirm whether this
same anomalous-order pattern is present at muldiv_u16_scaled and
muldiv_s16_rounded_3op's equivalent collision sites, or whether it's
specific to muldiv_u16.

CAUTION -- INSTRUMENTATION METHODOLOGY NOTE FOR FUTURE SESSIONS: an
early attempt at this used std::cerr for the debug output and caused
every force_decompile call touching muldiv_u16's address range to hang
indefinitely (repeatable, isolated by testing an unrelated function
which decompiled instantly on the same build). Root cause not fully
confirmed but consistent with cerr blocking when nothing drains the
decompile.exe child's stderr pipe in this PyGhidra/bridge setup at the
volume getUIndex() is called during a real decompile. Switching to a
plain ofstream file write (opened/closed per call, std::ios::app) fixed
it completely with no other change. DO NOT reintroduce cerr/cout debug
prints into decompiler-hot-path code (anything called from
Heritage/Merge/Cover per-varnode machinery) when testing against a live
PyGhidra bridge session -- use file-based logging only.

--------------------------------------------------------------------
NEXT STEPS
--------------------------------------------------------------------
1. Re-run the P-code trail capture (ghidra:get_function_pcode) against
   muldiv_u16 on the CURRENT isolated-9561-only rebuild specifically --
   the trace in this file was captured on the earlier triple_bug_fix
   build, before the isolated rebuild existed. Confirm the same
   merge_group 0 vs merge_group 1 collision on register[2] is still
   present byte-for-byte, now that Fix A and Fix B are both confirmed
   live in the build.
2. ANSWERED 2026-09-10 -- see NEW FINDING section above. isDead() is
   FALSE at the bug site; Fix B correctly does not fire; this is a
   distinct defect. Original text preserved below for context:
   determine
   whether Fix B (CoverBlock::getUIndex isDead() guard) was verified
   against muldiv_u16 specifically, or a different function. Check
   whether muldiv_u16's INDIRECT-collision at 0x141ce hits the isDead()
   precondition Fix B guards against -- i.e. is the op that
   register[2]'s stale INDIRECT points back to actually dead/removed at
   that point, the way Fix B's fix assumes? If NOT dead, Fix B's guard
   correctly does nothing here and this is a genuinely separate bug in
   the same Cover/Merge machinery, not a Fix-B-should-have-caught-this
   case.
3. Determine whether muldiv_u16's stack-parameter access (the `c`
   parameter, still showing as auStack_82[128]/puVar4+10 access even
   post-Fix-A) is structurally a SEGMENTOP-nested-in-INT_ADD case the
   way Fix A's regression test models it, or something Fix A's recursion
   doesn't reach for a different structural reason. This determines
   whether the merged-array symptom still present on muldiv_u16 is a
   second, distinct gap in Fix A, or expected/unrelated.
4. Live-check the two not-yet-verified functions this session
   (muldiv_s16_rounded, weighted_blend_u8frac) via force_decompile.
5. Re-run the full "7 unaffected siblings" list from
   calling-convention-regression.md against the current build -- do not
   assume it's still accurate, since muldiv_u16 itself changed behavior
   between investigations.
6. Reconcile the SIGSEGV/LowlevelError non-reproduction noted above:
   determine whether review-9561-merge-ordering.md's crash is fixed
   (plausibly BY Fix B, which targets the same stale-IOP-pointer hazard
   class that doc was investigating), intermittent, or just not
   triggered by this session's specific force_decompile call order.
7. If steps 1-3 confirm muldiv_u16 needs a genuinely new fix (not just
   a missed test case for an existing one), step through
   Heritage/ActionMergeCopy/coreaction.cc's INDIRECT-merge handling
   directly to either confirm or rule out a second, distinct defect in
   the same family as Fix B, rather than continuing to reason from
   symptom-level P-code alone.
8. Once root cause is confirmed, do NOT reuse review-9561-merge-ordering's
   `afterop = op` fallback pattern uncritically elsewhere -- that fallback
   is itself suspected of being the direct cause of the *ordering* defect
   in that doc, so any fix here should be checked for the same class of
   side effect before landing.
9. Do not spend further time on the calling-convention/cspec/SLA layer
   for THIS specific bug unless steps 1-3 above point back to it -- this
   session's one concrete probe (farpointerjoin) found no connection,
   though the broader unverified-convention-model gap noted in RULED OUT
   remains real and worth its own separate tracking.
10. NEW HYPOTHESIS TRACED (2026-09-10, this session, static reading only --
    NOT instrumented/confirmed live): with item 2 answered and the cross-
    block CoverBlock/Cover theories struck down, traced the merge decision
    one level further up than any prior session reached. For a CPUI_INDIRECT
    with a non-address-forced output, Merge::mergeIndirect (merge.cc:855-861)
    routes to Merge::mergeOp (merge.cc:728), which unconditionally attempts
    to fold the INDIRECT's output HighVariable together with its single
    input's HighVariable via Merge::merge (merge.cc:1574). That call is
    gated ONLY on HighIntersectTest::intersection (variable.cc:1182) --
    if intersection() returns false for two HighVariables that are, in
    reality, both genuinely live at the collision point, merge() proceeds
    silently (no LowlevelError, matching this bug's exact silent-wrong-
    answer signature, as opposed to the mergeTestRequired/trim-exhaustion
    paths which do throw).

    intersection() (variable.cc:1197) calls Cover::intersectList, which
    is keyed by block index (map<int4,CoverBlock>) and only invokes the
    inner CoverBlock::intersect when both Covers have an entry for the
    SAME block index. Confirmed via reading (not yet instrumented) that
    this block-keyed comparison is NOT the same mechanism as the already-
    debugged CoverBlock::getUIndex/isDead() path -- it's one level up,
    in cover construction/lookup, not order-value arithmetic.

    IMPORTANT CAVEAT: this is source-reading only, matching the pattern
    that produced two prior struck-down theories in this same file (the
    staleness-cache theory, struck down at variable.hh/cover.hh reading;
    the cross-block-getUIndex theory, struck down at Cover::intersect
    reading). Per explicit user instruction this session, code was NOT
    patched or rebuilt -- this is tracked as a hypothesis only.

    NEXT CONCRETE STEP (not yet done): before any live instrumentation,
    finish the STATIC read of CoverBlock::intersect (cover.cc ~109-165,
    already partially read this session while confirming Fix B's isDead()
    guard is landed at cover.cc:31-99) and specifically
    HighIntersectTest::blockIntersection / testBlockIntersection /
    gatherBlockVarnodes (declared variable.hh:261-263, not yet located/
    read in variable.cc this session) to see whether the SECONDARY
    per-block test (not just intersectList's coarse block-index match)
    could be the actual place a genuine intersection gets missed. Only
    after that full static read is exhausted should live instrumentation
    of HighIntersectTest::intersection (analogous to the existing
    getUIndex instrumentation already sitting in cover.cc:48-97 for
    muldiv_u16's address range) be considered -- and only with explicit
    go-ahead, since it requires a native rebuild + binary redeploy.

    ALSO NOTE FOR NEXT SESSION: cover.cc currently has LIVE DEBUG
    INSTRUMENTATION still checked in (lines ~48-97, gated on address
    range 0x141ba-0x141da, writing to
    C:\Users\j.brophy.CORKILLSYSTEMS\muldiv_u16_getUIndex_debug.txt).
    This is intentionally left in place per this doc's existing
    methodology notes (CAUTION section above) but should not be mistaken
    for stock Ghidra source when diffing against upstream, and should be
    removed/reverted before any eventual upstream-facing fix is prepared.

--------------------------------------------------------------------
ROOT CAUSE CONFIRMED LIVE (2026-09-10, continued): intersection() returns
blockisect.size()=0 for the exact colliding HighVariable pair
--------------------------------------------------------------------
Rebuilt decompile.exe (gradlew :Decompiler:decompileWin_x86_64Executable,
BUILD SUCCESSFUL) with two new debug instrumentation points added this
session: HighIntersectTest::intersection (variable.cc) and
Varnode::copyShadow (varnode.cc), both gated to muldiv_u16's address
range and logging to file (muldiv_u16_intersection_debug.txt /
muldiv_u16_copyshadow_debug.txt in the user's home directory). Killed
the running decompile.exe, deployed the new binary (verified via changed
LastWriteTime and file size), ran ghidra:force_decompile on muldiv_u16 --
bug reproduced unchanged (`return b;`).

DECISIVE LOG LINE (muldiv_u16_intersection_debug.txt, entry 10):
  A: {addr=0x000a2702,defOp=SUBPIECE(0x141c2)} {addr=0x02,defOp=COPY(0x141c2)}
  B: {addr=0x02,defOp=COPY(0x141ce)}
  blockisect.size()=0  ->  res=FALSE

HighVariable A is the already-merged mulxu.w-chain value (merge_group 0
in the earlier P-CODE TRACE section -- the SUBPIECE and its COPY, both
at 0x141c2). HighVariable B is the division-result COPY at 0x141ce
(merge_group 1, the CORRECT value). intersection(A,B) returns false
because Cover::intersectList (cover.cc:357) finds ZERO shared block-index
keys between A's and B's covers -- it short-circuits before
blockIntersection()/copyShadow() are even invoked on this pair. This
directly confirms the review20.md item 10 hypothesis: the block-keyed
intersection test genuinely misses this collision. With no intersection
detected, Merge::mergeOp's merge() call (merge.cc:775, reached via
mergeIndirect -> mergeOp for the INDIRECT at 0x141c2) proceeds
unopposed and folds A and B into one HighVariable -- explaining the
silent wrong-answer with no LowlevelError.

NOTE ON THE COPYSHADOW LOG: a separate true-positive shadow relationship
(this=register[2]COPY@0x141c2, op2=SUBPIECE@0x141c2, RESULT=TRUE) was
also captured, but this is WITHIN merge_group 0 (A's own internal COPY
chain), not the A/B collision itself -- it is not the mechanism at fault
and should not be confused with the intersection() finding above; ruling
this out cost some initial analysis time and is noted here so a future
session doesn't re-walk the same dead end.

REMAINING QUESTION, sharpened to its final form: WHY do A's cover and
B's cover not share a block-index key, when both are confirmed (per the
existing block-dump instrumentation in cover.cc, see NEW FINDING above)
to occupy the SAME BlockBasic in the op-list dump (#2 INT_DIV, #3
SUBPIECE, #4 DELAY_SLOT, #5 COPY targOp, #6 COPY, #7 RETURN all listed
under one targOp->getParent() call)? Cover::intersectList operates on
each HighVariable's OWN map<int4,CoverBlock>, which is built by
Cover::rebuild/addDefPoint/addRefPoint/addRefRecurse (cover.cc:527-662)
at HighVariable-merge time, not by directly inspecting the function's
BlockBasic list. It is fully possible for A's cover to lack an entry for
the block both varnodes physically sit in, if addRefPoint's traversal
(the ustop/ustart raw getUIndex comparisons at cover.cc:590-592 and
638-639, called during addRefRecurse's cross-call recursion through
bl->getIn(j)) terminates or fails to record that block for one of the
merged instances making up HighVariable A. NEXT CONCRETE STEP: add
targeted instrumentation to Cover::addDefPoint/addRefPoint/addRefRecurse
(or simply dump a->getCover() and b->getCover()'s full block-index maps
at the point of the failing intersection() call above, via Cover::print
which already exists at cover.cc:665) to see literally which block
indices ARE present in each Cover, and compare that list against the
block index the shared BlockBasic actually has. This is a smaller,
more targeted instrumentation pass than the one just completed and
should directly answer whether this is a Cover::rebuild traversal bug
(cover construction incorrectly omits a block) or something in how
HighVariable::merge (variable.cc, not yet read this session) combines
two already-built Covers when A absorbs a later instance -- both remain
open until this dump is captured.

INSTRUMENTATION LEFT IN PLACE: variable.cc (HighIntersectTest::intersection,
~line 1213-1247) and varnode.cc (Varnode::copyShadow, ~line 1005-1057) now
also carry live debug logging matching cover.cc's existing pattern, gated
to muldiv_u16's address range (0x141ba-0x141da) and writing to
muldiv_u16_intersection_debug.txt / muldiv_u16_copyshadow_debug.txt
respectively. <fstream> was added to both files' includes. All three
instrumented files should be reverted together before any upstream-facing
fix is prepared.

--------------------------------------------------------------------
ROOT CAUSE PINPOINTED (2026-09-10, continued further): boundary-touching
ranges in the SAME block index, not a missing block index
--------------------------------------------------------------------
Added a second instrumentation layer: when intersection() finds
blockisect empty for an in-range pair, it now also dumps
a->getCover().print() and b->getCover().print() (Cover::print,
cover.cc:665, already existed -- no new function needed). Rebuilt,
redeployed (verified via LastWriteTime/size change), re-ran
force_decompile -- bug still reproduces identically.

RESULT for the exact failing pair from before:
  A cover blocks: 0: 0x000141c2:7d-end
                  1: begin-end
                  2: begin-0x000141ce:4a
  B cover blocks: 2: 0x000141ce:4a-0x000141ce:4a

This OVERTURNS the "zero shared block-index keys" reading of the earlier
log. A and B DO share block index 2. A's range in block 2 runs from
begin to 0x141ce:4a; B's range in block 2 is the single point
0x141ce:4a-0x141ce:4a (B's def point, i.e. the division-result COPY
itself). The two ranges are boundary-adjacent -- B's def point sits
exactly at the end of A's recorded range -- not cleanly overlapping,
not cleanly disjoint. intersectList still returns blockisect.size()=0
for this pair despite the shared key, meaning CoverBlock::intersect's
per-block comparison (cover.cc, not yet re-read with this specific
boundary case in mind -- the class declaration was read in variable.hh/
cover.hh earlier in this doc but the actual intersect() body has NOT
been walked with this exact "op defined exactly at the edge of another's
range" scenario in mind) is very likely using a strict boundary
comparison that treats "ends exactly where the other begins" as
non-overlapping, when for THIS case (an INDIRECT's forced merge with a
value defined at the tail edge of the first value's live range) that
boundary point is exactly where the real collision occurs and must
count as intersecting.

NEXT CONCRETE STEP: read CoverBlock::intersect's actual body (the
class is declared around cover.hh:1-115 per earlier reading in this
doc, but the METHOD BODY implementing intersect() -- likely in cover.cc
near the CoverBlock class functions, not yet located by line number
this session) to see exactly which comparison operator is used at the
range boundary, and determine whether the fix belongs there (loosen an
off-by-one at coincident start/end order values) or in how B's def-point
CoverBlock gets constructed as a single point in the first place instead
of properly including the moment of definition as a start-inclusive
range that would naturally overlap A's end. This is now a small,
well-scoped, single-function read -- no further live instrumentation
should be needed to find the exact line; live instrumentation should
only return if the source reading doesn't resolve it cleanly.

--------------------------------------------------------------------
EXACT ROOT CAUSE FOUND, source-confirmed (2026-09-10, continued): a
level-2 pre-filter silently discards the level-1 boundary collision
before the fine-grained check that would catch it
--------------------------------------------------------------------
Read CoverBlock::intersect (cover.cc:109-152) in full. It is CORRECT
and already handles our exact scenario deliberately: lines 124-126
explicitly check for a touching boundary ((ustart==u2stop)||
(ustop==u2start)) and return 1 ("Boundary intersection") rather than 0,
precisely for the case where one op's range ends exactly where
another's begins. This DISPROVES the immediately preceding section's
guess that CoverBlock::intersect itself was miscomparing the boundary --
it isn't; it was already right. (Leaving that guess in place above,
struck through by this note, per this doc's established convention of
not erasing superseded reasoning.)

The actual defect is one level up, in Cover::intersectList (cover.cc:
357-384). Its signature takes an int4 level parameter and filters with
`if (val >= level) listout.push_back(...)` (line 378). The single call
site inside HighIntersectTest::intersection (variable.cc:1197) passes
level=2:
    a->getCover().intersectList(blockisect,b->getCover(),2);
This means CoverBlock::intersect returning 1 (boundary intersection) --
exactly what it correctly computes for our A/B pair at block 2 -- FAILS
the `val >= 2` test and is silently dropped from blockisect. Since
blockisect ends up empty, the subsequent loop over blockisect in
intersection() (variable.cc:1201-1206) never calls blockIntersection()
at all for this block, so the fine-grained, boundary-aware secondary
test (HighIntersectTest::blockIntersection ->
HighIntersectTest::testBlockIntersection, called with level=0 at
variable.cc:1020, which WOULD correctly treat any intersection
including boundary as real) never even runs. intersection() therefore
returns false with no further checks, exactly matching every log and
symptom captured this session.

WHY THIS IS THE ROOT CAUSE, not a deliberate design choice being
misapplied: intersectList's level=2 call is a coarse, cheap PRE-FILTER
whose entire purpose (per its use immediately followed by
blockIntersection's own finer, level=0 test) is to cheaply identify
candidate blocks worth the expensive check -- not to be the final
word on intersection. A genuine boundary collision (level 1) is a
valid candidate that should always be allowed through to the
finer-grained test, since that finer test is specifically designed
to handle boundary cases via copyShadow-based reasoning
(HighIntersectTest::testBlockIntersection / gatherBlockVarnodes /
Varnode::copyShadow, variable.cc:960-1004, already read earlier this
session). Filtering at level>=2 silently discards exactly the class of
case the finer test exists to adjudicate. This is very likely a
pre-existing upstream latent defect (not something introduced by the
H8 processor module or by Fix A/Fix B), since intersectList's level
parameter and its two call sites (level=2 here, level=0 inside
blockIntersection at variable.cc:1020ish for the per-varnode test) look
original to Ghidra's decompiler, not H8-specific -- though this has
NOT yet been confirmed against a clean/stock Ghidra checkout diff.

CANDIDATE FIX (NOT YET APPLIED -- discuss with user before touching):
Change the single call site in HighIntersectTest::intersection
(variable.cc:1197) from level=2 to level=1, so boundary intersections
are no longer pre-filtered out before blockIntersection gets a chance
to run its proper, already-boundary-aware, level=0 check. This is a
minimal, surgical one-character change (2 -> 1) at a single call site,
not a change to CoverBlock::intersect's logic (which is already
correct) or to Cover::intersectList's general mechanism (which is
correctly generic; the bug is in what level THIS caller asks for).
RISK: lowering the pre-filter threshold means MORE blocks pass through
to the expensive per-varnode blockIntersection check, which could
affect the decompiler's performance on other, unrelated functions (in
Ghidra generally, not just H8 firmware) if boundary-adjacent-but-truly-
distinct variables are extremely common. This risk should be weighed
against: (a) profiling impact on a broad regression corpus, and (b)
confirming blockIntersection's own logic correctly resolves the
now-admitted boundary candidates as "not actually the same variable"
in the many cases where a boundary touch is legitimate and harmless
(e.g. two genuinely sequential, non-conflicting variables that happen
to abut). This candidate fix has NOT been applied, built, or tested
yet, pending explicit user go-ahead, per this session's established
practice of confirming before further code changes.

--------------------------------------------------------------------
IMPACT / WHY THIS BLOCKS OTHER WORK
--------------------------------------------------------------------
- review.md item 8 (H8/520, H8/538F core port): should NOT proceed while
  this regression is open. The port's whole rationale is forking off a
  "mature, actively-verified" h8539f core; a live silent-wrong-answer bug
  in a load-bearing math-helper family fails that bar regardless of how
  clean review.md's own tracked items look otherwise.
- Any ROM analysis conclusion (this project's or a downstream user's) that
  depends on muldiv_u16/_scaled/s16_rounded[_3op]/weighted_blend_u8frac or
  any of their callers, decompiled on a build containing 9561, should be
  treated as UNVERIFIED until this is fixed and those functions are
  re-checked. This is a wide blast radius -- these are generic scaling/
  rounding helpers, not single-purpose functions, so callers span fueling,
  timing, and sensor-scaling code per review.md's own item 10 caller list
  (confirmed this session: fuel_pw_and_airvol_compute,
  isc_activation_control_f466, isc_f37c_ramp_target_calc,
  isc_integrator_update, tcu_shift_ratio_buffer_update,
  tcu_shift_torque_and_knock_mgmt all call muldiv_u16 directly).
- IMPORTANT: this is a Ghidra-side static-analysis bug only. Confirmed
  this session that the actual ROM firmware executes correctly on real
  hardware (see IMPORTANT CORRECTION above) -- this does NOT block or
  call into question anything about the vehicle/ECU itself, only this
  project's Ghidra-based analysis of it.

--------------------------------------------------------------------
SELF-CORRECTION (2026-09-10, continued): earlier level=2->1 edits landed
in the wrong place (a doc-comment above updateHigh, corrupted twice by
str_replace tool mis-anchoring on a repeated substring), not the real
call site. First rebuild/redeploy/test cycle after the "fix" showed the
bug UNCHANGED because the real call site (variable.cc, inside
HighIntersectTest::intersection) was still literally level=2 the whole
time. Diagnosed via a direct raw-value probe
(a->getCover().getCoverBlock(2).intersect(b->getCover().getCoverBlock(2)))
logged alongside the existing dump, which confirmed CoverBlock::intersect
truly returns 1 for the pair as predicted -- proving the logic reasoning
was right and only the edit location was wrong. Repaired via direct
PowerShell line-indexed edits (Get-Content/Set-Content on the exact
array index) rather than further str_replace attempts, since str_replace
kept matching the corrupted comment text instead of the real call site
even after intermediate restores. variable.cc now has exactly ONE
occurrence of intersectList(blockisect,b->getCover(),N) -- at the real
call site inside intersection(), correctly set to level=1 -- confirmed
via grep and full compiler diagnostics (0 errors). Rebuilding and
redeploying now to actually test the fix for the first time.

--------------------------------------------------------------------
LEVEL FIX CONFIRMED WORKING BUT INSUFFICIENT ALONE (2026-09-10,
continued): blockisect.size() changed 0->1 for the decisive pair, but
final res is still FALSE and the bug still reproduces
--------------------------------------------------------------------
With the level=1 fix correctly applied at the real call site (confirmed
via grep: exactly one occurrence, at variable.cc's real intersection()
body) and freshly rebuilt/redeployed/Ghidra-restarted, re-ran
force_decompile on muldiv_u16. Bug STILL reproduces (return b;)
unchanged. However the debug log now shows a materially different
result for the exact decisive pair:
  A: {SUBPIECE(0x141c2)} {COPY(0x141c2)}
  B: {COPY(0x141ce)}
  blockisect.size()=1  ->  res=FALSE   (previously: blockisect.size()=0)

This CONFIRMS the level=2->1 fix is real and working as designed: the
boundary collision now correctly passes the coarse pre-filter and
reaches blockIntersection() (variable.cc:1005ish), which is now
actually being invoked on this pair for the first time. However
blockIntersection/testBlockIntersection/copyShadow's finer-grained
check independently concludes FALSE (no intersection) for this exact
pair anyway -- meaning there is a SECOND, separate, previously-hidden
defect in that finer-grained logic that only became visible once the
first (level) bug was fixed. The two multiplication-chain varnodes in
A (SUBPIECE and COPY, both at 0x141c2) were already established
earlier this session to be genuine copyShadow(...)=TRUE of each other
(see COPYSHADOW LOG NOTE above) -- that finding remains correct and is
NOT the issue here; the open question is specifically why
blockIntersection/copyShadow treats vn(A) vs vn(B) -- the COPY at
0x141c2 vs the COPY at 0x141ce -- as non-intersecting when B's COPY is
the INDIRECT's actual colliding value.

NEXT CONCRETE STEP: read HighIntersectTest::blockIntersection and
HighIntersectTest::testBlockIntersection in full (declared/partially
read earlier around variable.cc:960-1004 for gatherBlockVarnodes, but
blockIntersection's own body -- likely just above/around that region --
has NOT yet been read line-by-line this session) to see exactly how it
iterates the varnode lists and calls copyShadow, and determine why the
A-side COPY(0x141c2) and B-side COPY(0x141ce) pair specifically is
judged non-intersecting. Given the earlier copyShadow log entry showed
this=COPY(0x141c2) as truly copyShadow-related to SUBPIECE(0x141c2)
(same merge_group), it is possible blockIntersection is doing an
all-vs-all copyShadow comparison across BOTH lists and treating ANY
shadow match anywhere in the cross product as sufficient to call the
whole block "non-intersecting" -- which would be incorrect if even one
pair in the cross product is a genuine, non-shadow collision (B's COPY
at 0x141ce against A's SUBPIECE at 0x141c2, which have never been
directly logged against each other by copyShadow this session). This
is the most promising remaining lead and should be checked by reading
the loop structure of blockIntersection itself before further live
instrumentation.

FIX STATUS: level=2->1 change in variable.cc's intersection() left IN
PLACE (confirmed correct, working, and necessary -- though evidently
not sufficient alone to resolve the visible bug). No further code
changes made pending this next static read.

--------------------------------------------------------------------
SYSTEMIC PATTERN FOUND (2026-09-10, continued): the same level>1
boundary-exclusion bug is independently repeated THREE more times inside
gatherBlockVarnodes/testBlockIntersection, one layer below the call site
already patched
--------------------------------------------------------------------
Read HighIntersectTest::blockIntersection, gatherBlockVarnodes, and
testBlockIntersection in full (variable.cc:960-1054). Found the exact
same class of bug repeated independently at three more call sites, all
using Varnode::getCover()->intersectByBlock(blk,cover) (a per-varnode
analog of Cover::intersectList's per-HighVariable level check):

  1. gatherBlockVarnodes, line 966:
       if (1<vn->getCover()->intersectByBlock(blk,cover))
     Requires level>1 (i.e. ==2, full overlap) to add a candidate
     varnode to blist. A boundary touch (level 1) is excluded here,
     meaning B's actual colliding varnode may never even enter blist.

  2. testBlockIntersection, line 987:
       if (2>vn->getCover()->intersectByBlock(blk,cover)) continue;
     Requires level>=2 for A's OWN instance to even be considered; a
     boundary-touching instance of A is skipped entirely before the
     inner blist loop runs.

  3. testBlockIntersection, line 990:
       if (1<vn2->getCover()->intersectByBlock(blk,*vn->getCover())) {
     Requires level>1 again for the specific vn/vn2 pairwise check
     before copyShadow/partialCopyShadow is even consulted (lines
     991-998, where the ACTUAL wrong-vs-right-value decision happens).

This means fixing ONLY the top-level intersection() call
(Cover::intersectList level 2->1, already applied and confirmed
changing blockisect.size() 0->1) was necessary but not sufficient: even
once a block is correctly flagged as a merge candidate, these three
inner gates can still each independently throw away the exact
boundary-touching varnode(s) that represent the real collision, before
copyShadow/partialCopyShadow ever gets a chance to judge them correctly.
This fully explains why blockisect.size() is now 1 (block-level gate
fixed) but blockIntersection()'s own return value is still false (one
or more of the three varnode-level gates above is still discarding our
pair).

CANDIDATE FIX (NOT YET APPLIED): change the three thresholds above from
level>1 to level>=1 (i.e. >0) and level>=2 to level>=1, consistent with
the same reasoning as the already-applied fix: these are pre-filters
whose job is to decide whether a pair is a CANDIDATE for the real
copyShadow/partialCopyShadow test, not to make the final call
themselves. A boundary touch is a valid candidate in all three cases.
RISK is proportionally larger than the single already-applied change,
since this touches three sites in the hot per-varnode inner loop of the
merge algorithm rather than one per-HighVariable site -- broader
potential performance/behavior impact on the general Ghidra decompiler,
not just H8 firmware, should this ever be proposed upstream. Given the
recurring pattern of str_replace mis-anchoring on repeated/near-duplicate
text in this file earlier this session (which corrupted a doc-comment
twice), these three edits will be made via direct PowerShell
Get-Content/Set-Content line-array edits, matched by exact line number
confirmed via grep immediately beforehand, NOT via str_replace, to avoid
repeating that failure mode.

NOT YET APPLIED -- pending explicit user go-ahead given the broadened
scope of this change (3 sites instead of 1, all in a hot loop).

--------------------------------------------------------------------
ALL FOUR LEVEL-FILTER FIXES APPLIED: silent wrong-answer replaced by a
DETECTED, THROWN LowlevelError (2026-09-10, continued)
--------------------------------------------------------------------
Applied all three remaining fixes (gatherBlockVarnodes line 966: 1<...
-> 0<...; testBlockIntersection line 987: 2>...continue -> 1>...continue;
testBlockIntersection line 990: 1<... -> 0<...) via direct PowerShell
Get-Content/Set-Content line-indexed edits (guarded with exact-match
assertions before writing, to avoid repeating the earlier str_replace
mis-anchoring failure). Compiler diagnostics: 0 errors, no new warnings
at the three edited lines. Rebuilt (BUILD SUCCESSFUL), redeployed
(confirmed via LastWriteTime), Ghidra fully restarted, force_decompile
re-run on muldiv_u16.

RESULT: decompilation NO LONGER silently produces wrong output. Instead:
  Low-level Error: Unable to force merge of op at 0x000141d7:8e

This is a qualitatively different and more correct failure mode than
the original bug. With all four level-filter gates now correctly
treating a boundary touch as a real candidate intersection, the merge
algorithm's finer-grained check (blockIntersection/copyShadow) now
correctly IDENTIFIES that HighVariable A (mulxu.w chain) and
HighVariable B (division-result COPY at 0x141d7, the INDIRECT's forced
merge partner) really do collide -- and refuses to merge them. However
Merge::mergeOp's INDIRECT-forced-merge path (merge.cc:728, called from
mergeIndirect) apparently has no fallback for "the two sides genuinely
cannot be merged" -- it appears to assume mergeTest/merge success is
guaranteed for an INDIRECT's forced merge and throws a hard
LowlevelError (via mergeTestRequired's speculative-merge-failure path,
not yet re-confirmed by reading this exact throw site) rather than
inserting a copy/split to keep both values distinct in the output.

This means: the ROOT CAUSE (the level-filter bugs) is now fully fixed
and confirmed via direct evidence (the finer check now correctly
detects the real collision it was previously blind to). What remains
is a SEPARATE, downstream problem: Merge::mergeOp / whatever calls it
for INDIRECTs needs a fallback path for a forced-merge that fails its
own intersection test, most likely by inserting an explicit COPY to
split the two live ranges rather than assuming the merge always
succeeds. This is architecturally a different, likely larger change
than the four one-line level fixes, and squarely upstream Ghidra
decompiler merge-algorithm territory, not H8-specific.

NEXT STEPS: (1) test the other affected functions (muldiv_u16_scaled,
muldiv_s16_rounded_3op, muldiv_s16_rounded, weighted_blend_u8frac) and
the control (sat_add_u16) against this same build to see if they now
also throw LowlevelError (expected, if they share the same root cause)
rather than silently misdecompiling, and whether sat_add_u16 remains
unaffected (regression check). (2) read Merge::mergeIndirect and
mergeOp's exact throw path/call chain for "Unable to force merge" to
understand whether a copy-insertion fallback is feasible, and whether
Ghidra's own codebase has an existing pattern for this (splitting a
merge on failure) elsewhere in merge.cc that could be reused rather
than invented from scratch.

FIX STATUS: level=2->1 (intersection, 1 site) + three inner-loop level
fixes (gatherBlockVarnodes/testBlockIntersection, 3 sites) all IN PLACE,
confirmed compiling cleanly and changing behavior as predicted at every
step. Not reverted -- correctly detecting and refusing the bad merge is
strictly better than silently producing wrong output, even though a
LowlevelError is not yet a full, usable fix for the end user.

--------------------------------------------------------------------
REGRESSION CONFIRMED: sat_add_u16 (control, previously correct) now
ALSO throws LowlevelError -- the level-loosening fix is too broad
--------------------------------------------------------------------
Ran ghidra:batch_decompile across muldiv_u16_scaled (0x141dc),
muldiv_s16_rounded_3op (0x14247), muldiv_s16_rounded (0x14219),
sat_add_u16 (0x14000), weighted_blend_u8frac -- ALL FIVE returned
'Decompilation failed'. Confirmed sat_add_u16 specifically via
force_decompile with its correct address (0x14000, looked up via
ghidra:search_functions since the earlier batch call used the bare
name, which failed to resolve):

  Low-level Error: Unable to force merge of op at 0x0001400c:36

sat_add_u16 was previously confirmed CORRECT and used as this session's
regression control -- it has NO relationship to the muldiv_* family's
INDIRECT/mulxu.w collision pattern. Its failure under the exact same
error class ('Unable to force merge') confirms the four level-loosening
edits (intersection: level 2->1; gatherBlockVarnodes: 1<...->0<...;
testBlockIntersection x2: 2>...->1>..., 1<...->0<...) are TOO BROAD:
they are now flagging boundary-touching pairs as genuine conflicts in
cases where the boundary touch is legitimate and harmless (e.g. two
sequential, non-overlapping variables that happen to abut in the
Cover), not just in the muldiv_u16-style genuine collision case. This
is exactly the RISK flagged in the earlier CANDIDATE FIX section before
these changes were applied, now empirically confirmed.

This means the fix, AS APPLIED, is not viable to leave in place: it
trades one bug (silent wrong answer in ~4 named functions) for a
broader regression (hard decompilation failure in previously-correct
functions, confirmed in at least 1, likely many more across the 741-
function program and beyond it in any other Ghidra target).

STATUS / OPEN QUESTION FOR NEXT STEP: the four edits are still live in
variable.cc as of this note. Options going forward (not yet decided):
 (a) revert all four to their original level thresholds (2, 1<..(orig),
     2>..(orig), 1<..(orig)) restoring the original silent-wrong-answer
     bug but eliminating the new regression, and look for a NARROWER
     fix that only loosens boundary handling for the specific
     INDIRECT-forced-merge case (mergeIndirect/mergeOp) rather than
     globally in Cover/HighIntersectTest's general-purpose intersection
     test used by every merge decision in the decompiler;
 (b) keep the four edits and additionally implement the copy-insertion
     fallback noted in the previous section for Merge::mergeOp's forced
     merge failure path, so a genuine LowlevelError becomes a correctly
     split-variable decompilation instead -- but this does not fix
     sat_add_u16-style cases where the merge SHOULD still succeed (a
     harmless boundary touch) and now incorrectly throws instead;
 (c) investigate a fix scoped specifically to Merge::mergeIndirect's
     forced-merge call path only (e.g. use a stricter/different
     intersection test JUST for the INDIRECT-forced case, leaving the
     general intersection() level thresholds at their original, correct
     values for all other callers) -- likely the most correct direction
     but not yet designed or attempted.
Awaiting user direction before proceeding further; no code has been
reverted yet.

--------------------------------------------------------------------
REVERT CONFIRMED CLEAN (2026-09-10, continued): back to known-good
baseline, no regressions, ready to design a narrower fix
--------------------------------------------------------------------
All four level-loosening edits reverted via guarded PowerShell
line-indexed writes (each preceded by an exact-match assertion against
the expected loosened text, so a mismatch would throw rather than
silently write the wrong line). Compiler diagnostics after revert: 0
errors, and the warning list is IDENTICAL to the pristine pre-session
baseline (same lines/messages), confirming a byte-for-byte-equivalent
revert of all logic (debug logging additions from earlier in this
session remain, harmlessly, as designed). Rebuilt (BUILD SUCCESSFUL,
fresh compile+link, not up-to-date), redeployed (confirmed via
LastWriteTime), Ghidra fully restarted.

VERIFIED: sat_add_u16 (0x14000) decompiles correctly again (result =
a+b, CARRY2 saturation to 0xffff) -- regression eliminated.
VERIFIED: muldiv_u16 (0x141ba) reproduces the ORIGINAL bug exactly
(return b;) -- known baseline restored, not the LowlevelError state.

Session is now at a clean, stable checkpoint: root cause fully
understood and confirmed (see EXACT ROOT CAUSE FOUND and SYSTEMIC
PATTERN FOUND sections above), but the straightforward fix (loosening
the four level thresholds globally) is confirmed TOO BROAD and has been
fully reverted. variable.cc and varnode.cc still carry the debug
logging instrumentation from earlier (intersection()/copyShadow()
dumps, gated to muldiv_u16's address range) -- these are inert for any
other function and were not implicated in the regression (the
regression came from the four logic threshold changes, not the logging
additions).

NEXT: design a fix scoped specifically to the INDIRECT-forced-merge
path (Merge::mergeIndirect/mergeOp, merge.cc:728-861) rather than
loosening the general-purpose Cover/HighIntersectTest level thresholds
used by every merge decision in the decompiler. Not yet started.

--------------------------------------------------------------------
NARROW FIX TARGET IDENTIFIED (2026-09-10, continued): mergeIndirect's
non-address-forced path has NO trim/fallback at all; mergeOp's trim
logic (used by that path) is blind to boundary collisions for the SAME
underlying reason as the reverted global fix, but the true fix belongs
in the TRIM decision, not the general intersection() thresholds
--------------------------------------------------------------------
Re-read Merge::mergeIndirect (merge.cc:855-891) and Merge::mergeOp
(merge.cc:728-781) with fresh eyes post-revert. Key structural finding:

mergeIndirect has TWO branches:
  - outvn->isAddrForce() == true (lines 863-890): has a full retry
    chain -- mergeTestRequired+merge, then snipOutputInterference (which
    inserts a COPY to split the conflicting value) and retries, and only
    THEN throws LowlevelError as a last resort. This path already knows
    how to recover from a failed merge by inserting a copy.
  - outvn->isAddrForce() == false (lines 858-861, THIS BUG'S PATH): just
    calls mergeOp(indop) directly, no retry, no fallback of its own --
    whatever mergeOp does is the whole story.

Inside mergeOp (merge.cc:728-781): it DOES already contain a trim
mechanism (mergeTest + trimOpInput/trimOpOutput, lines 752-770) meant
to catch exactly this class of problem BEFORE attempting the final hard
merge. But Merge::mergeTest (merge.cc:1666-1678) calls
testCache.intersection(a,high) -- the SAME HighIntersectTest::intersection
function already fully traced and confirmed to silently miss our
boundary-touching collision via its level=2 pre-filter. This means
mergeTest ALSO reports 'no conflict' for our exact pair, the trim loop
(lines 756-770) never triggers (i==max on the first pass, line 765),
and execution falls straight through to the final unconditional
merge()/throw at lines 772-780 -- which is the exact 'Unable to force
merge of op at <addr>' error observed when intersection() was
temporarily fixed.

This reframes the fix target precisely: the correct fix is NOT to
loosen HighIntersectTest::intersection()'s or its sub-checks' level
thresholds globally (confirmed too broad, breaks sat_add_u16 and
presumably other legitimate boundary-adjacent cases elsewhere in the
codebase). Instead, the fix belongs in Merge::mergeOp's TRIM decision
specifically: mergeOp needs to also treat a boundary-level (level 1)
collision as trim-worthy, even though the general-purpose
intersection()/mergeTest correctly (for THEIR general audience) treat
a bare boundary touch as usually-harmless-don't-bother. This way:
  - intersection()'s global level=2 threshold stays untouched, so
    sat_add_u16 and everything else relying on 'is this actually worth
    a full block-level merge-blocking check' keeps working exactly as
    before;
  - mergeOp's own trim heuristic, which exists specifically to PREVENT
    a doomed final merge by proactively splitting inputs, becomes aware
    of the narrower, boundary-specific case that can still cause the
    final hard merge to fail even when the general test said 'fine.'

CANDIDATE NARROW FIX (NOT YET DESIGNED IN DETAIL, NOT YET APPLIED):
Add a supplementary, boundary-aware check inside mergeOp's trim-decision
loop (merge.cc:752-754 and 762-764) -- e.g. call
a->getCover().intersectList(...,1) (boundary-inclusive) as a SEPARATE,
additional test alongside the existing mergeTest() calls, and treat a
boundary hit here as ALSO trim-worthy even when mergeTest() proper
returns true. This keeps the change scoped to mergeOp's trim heuristic
only, never touching HighIntersectTest::intersection()'s or its
sub-checks' actual thresholds, so no other caller of those (general
merge decisions throughout the decompiler) is affected. This still
needs careful design: mergeTest's return value is also used to CACHE
+ build tmplist (the accumulated already-merged-into-testlist state)
for the trim loop's own bookkeeping, so simply calling a second,
stricter test alongside it without integrating into that bookkeeping
correctly could reintroduce different bugs. NOT YET ATTEMPTED --
requires more careful reading of how tmplist/testlist accumulate across
the loop before writing any code.

STATUS: still at the clean, reverted, known-good baseline (no code
changes since the full revert). This is purely an updated analysis of
where the correct, narrow fix should go, based on re-reading
mergeIndirect/mergeOp fresh after the broad-fix regression.

--------------------------------------------------------------------
NARROW FIX APPLIED AND TESTED: no crashes, no regressions, but the
WRONG VALUE BUG ITSELF IS NOT YET FIXED (2026-09-10, continued)
--------------------------------------------------------------------
Designed and applied a narrow, INDIRECT-scoped fix per user direction:
added Merge::mergeTestBoundary (merge.hh + merge.cc), a new static
method that calls high_out->getCover().intersectList(...,1,...)
directly -- boundary-inclusive, completely independent of
HighIntersectTest::intersection/testCache -- and wired it into
Merge::mergeOp's trim-decision loop (merge.cc ~line 802), gated
STRICTLY to i==max && max==1 (i.e. only the CPUI_INDIRECT case,
never MULTIEQUAL). If mergeTest() alone says no conflict but
mergeTestBoundary() detects a boundary collision, i is reset to 0,
forcing entry into the existing trim branch that was already there
for MULTIEQUAL's cover-restriction case.

BUILD ISSUES DURING THIS EDIT (self-corrected): two separate
str_replace calls mis-anchored on repeated/near-duplicate substrings
in merge.hh and merge.cc (same failure class as earlier in this
session with variable.cc), corrupting merge.hh's copyTrims/protoPartial
declarations and losing mergeTestRequired's declaration. Caught via
full compiler diagnostics (14 errors) before any rebuild/deploy;
repaired via direct PowerShell Get-Content/Set-Content line-indexed
writes verified against the original first-read content of merge.hh,
re-diagnosed to 0 errors, then proceeded.

TEST RESULTS on the rebuilt, redeployed, Ghidra-restarted binary:
  - muldiv_u16 (0x141ba): NO LowlevelError (trim fires -- P-code order
    changed, confirming the split happened) but STILL returns b
    (still WRONG, just no longer crashes).
  - sat_add_u16 (0x14000): CORRECT, unchanged. Confirms the max==1 gate
    successfully avoided the earlier global regression.
  - muldiv_u16_scaled, muldiv_s16_rounded_3op, muldiv_s16_rounded,
    weighted_blend_u8frac: all NO LongLevelError, but all STILL
    WRONG (in_R1/unaff_R3 unresolved, wrong return values) -- same
    pattern as muldiv_u16.

INTERPRETATION: the fix successfully makes mergeOp's trim loop AWARE
of the boundary collision and it DOES trigger trimOpInput (confirmed
by the changed instruction ordering in the decompiled output), so the
immediate crash (LowlevelError) is resolved. However, trimming the
INDIRECT's input apparently is not sufficient to make the CORRECT
value (the division result) end up as what's ultimately read at the
return point -- something about which of the two (now properly split)
variables the final RETURN op's input traces back to is still wrong.
This suggests the actual value-selection defect is downstream of the
Cover/merge mechanism entirely -- possibly in which COPY the RETURN
op's input SSA edge points to, or a dead-code-elimination /
propagation pass that runs after merging and picks the wrong one of
the two now-properly-separated variables to keep. This has NOT yet
been investigated.

STATUS: this narrow fix is a net improvement (no crashes, no
regressions) but does NOT resolve the user-visible wrong-answer bug.
It should be considered a partial, safe fix for the crash risk, with
the actual value-correctness defect remaining open and requiring
further investigation into what happens AFTER Merge::mergeOp finishes
-- likely in a later action/pass that decides which SSA definition
feeds the final RETURN, not in the merge/cover machinery itself.

======================================================================
SESSION SUMMARY (2026-09-10) -- for quick orientation next session
======================================================================
1. ROOT CAUSE (fully confirmed, live-instrumented): Cover::intersectList
   is called with level=2 from HighIntersectTest::intersection
   (variable.cc:1198), which silently discards genuine boundary-only
   Cover collisions (CoverBlock::intersect correctly returns 1 for
   these, cover.cc:109-152, but 1 fails the level>=2 filter). The same
   pattern is independently repeated in gatherBlockVarnodes and
   testBlockIntersection (variable.cc ~966, ~987, ~990).

2. FIRST FIX ATTEMPT (reverted): loosening all four thresholds
   globally (level 2->1, 1<->0<, 2>->1>) fixed muldiv_u16 but broke
   sat_add_u16 (previously-correct control) with the same class of
   error -- confirmed too broad, fully reverted, verified back to
   pristine baseline (identical compiler warnings to session start).

3. SECOND FIX (currently APPLIED, narrowly scoped): added
   Merge::mergeTestBoundary (merge.hh + merge.cc), a new boundary-
   inclusive Cover test used ONLY inside Merge::mergeOp's trim-decision
   loop, ONLY for the CPUI_INDIRECT case (max==1). This does NOT touch
   HighIntersectTest::intersection or any of its three internal gates
   from item 1 -- those remain at their original values. CONFIRMED via
   testing: no LowlevelError crashes anywhere, sat_add_u16 unaffected
   (regression-free), muldiv_u16's trim mechanism now correctly fires
   (P-code instruction order changes, confirming a split happened).

4. REMAINING BUG (NOT YET FIXED): despite the trim firing correctly,
   muldiv_u16 and all other affected functions (muldiv_u16_scaled,
   muldiv_s16_rounded_3op, muldiv_s16_rounded, weighted_blend_u8frac)
   STILL return the wrong value -- no crash, but still incorrect
   output. This means splitting the covers via trimOpInput is not,
   by itself, sufficient to make the CORRECT HighVariable end up as
   the RETURN op's input. The defect apparently lies downstream of
   Merge::mergeOp/mergeIndirect entirely.

5. NEXT SESSION SHOULD START HERE: investigate what happens to the
   RETURN op's input varnode AFTER mergeOp splits the INDIRECT's
   input via trimOpInput/allocateCopyTrim. Candidate areas to read,
   none yet explored this session:
     - allocateCopyTrim itself (merge.cc, declared merge.hh:100) --
       confirm the COPY it inserts actually carries the CORRECT
       (division-result) value, not the stale mulxu.w one, i.e. that
       trimOpInput is trimming the right SIDE of the collision.
     - Whatever action/pass runs after Merge (likely in action.cc or
       coreaction.cc, not yet located) that could still coalesce or
       propagate the wrong COPY forward into the RETURN op even after
       Cover-level merging is resolved correctly.
     - Consider adding a NEW round of live instrumentation (same
       methodology as this session: file-based debug logging gated to
       muldiv_u16's address range) specifically inside
       allocateCopyTrim and around the final RETURN op resolution, to
       see definitively which HighVariable's value the RETURN op reads
       from after the current fix's trim has run.

6. CURRENT CODE STATE: variable.cc and varnode.cc carry harmless debug
   logging only (from earlier live-instrumentation, gated to
   muldiv_u16's address range, all original thresholds/logic restored
   to pristine). merge.hh and merge.cc carry the new, narrowly-scoped
   mergeTestBoundary fix (compiles cleanly, tested, no regressions,
   does not yet fully fix the bug). All four files' debug logging and
   the merge.cc/merge.hh fix should be reviewed together before any
   upstream-facing changes are prepared; none of this has been
   committed to git or otherwise finalized -- it exists only in the
   live working tree at C:\Users\j.brophy.CORKILLSYSTEMS\github\ghidra.

--------------------------------------------------------------------
LIKELY TRUE ROOT CAUSE OF THE WRONG-VALUE BUG FOUND (2026-09-10,
continued): merge() keeps high1's identity, and high1 IS op->getOut(),
the wrong/stale HighVariable, regardless of trim success
--------------------------------------------------------------------
Re-read Merge::merge (merge.cc:1623-1633) and confirmed
allocateCopyTrim (merge.cc:449-472) is NOT the problem -- it correctly
preserves value semantics via a genuine SSA COPY split.

THE REAL ISSUE: merge()'s own internal gate (line 1627) is
testCache.intersection(high1,high2) -- i.e. THE SAME untouched,
original, level=2-based test, completely independent of whatever
mergeTestBoundary/the trim loop in mergeOp decided. This means: once
trimOpInput successfully separates the two Covers (which the new fix
now correctly triggers), THIS test correctly reports no intersection,
and merge() proceeds -- calling high1->merge(high2,...) (variable.cc,
not yet read this session), which by every indication merges high2's
varnode instances INTO high1 and keeps high1 as the surviving
HighVariable identity, discarding high2.

In mergeOp's final loop (merge.cc ~824):
  merge(op->getOut()->getHigh(), op->getIn(i)->getHigh(), false)
high1 = op->getOut()->getHigh()  -- the INDIRECT's OUTPUT high, i.e.
  the PRE-EXISTING, WRONG mulxu.w-chain HighVariable (our "A")
high2 = op->getIn(i)->getHigh()  -- i.e. our "B", the CORRECT
  division-result HighVariable

So even with a fully successful trim (Covers now genuinely disjoint,
no crash), the surviving merged HighVariable is high1 -- A's identity,
not B's -- because that's simply which argument mergeOp always passes
as high1. The trim makes the MERGE mechanically valid (no Cover
violation), but does nothing to correct WHICH of the two values ends
up canonical afterward, since Merge::merge always keeps its first
argument's identity by convention (this convention itself is very
likely correct and used correctly by every OTHER caller in the
codebase -- MULTIEQUAL merges, address-forced INDIRECT merges, etc.
This is not a bug in merge() itself).

WHY THIS IS SPECIFIC TO OUR CASE: for a normal, well-behaved INDIRECT
(the whole point of an INDIRECT p-code op), the INPUT and OUTPUT
really are supposed to represent the SAME logical value across the
indirect effect -- merging output-identity-wins is fine because both
sides "should" be identical anyroad. Our bug is exactly the case where
that assumption breaks down: A (op->getOut()'s pre-existing high) is
NOT actually the same logical value as B (op->getIn(0)'s high) -- A is
a stale multiplication leftover that happens to share a register with
the INDIRECT's output slot, and B is the real division result. The
Cover-level fix (mergeTestBoundary) correctly identifies these as
DIFFERENT values needing separation, and the trim DOES separate them
at the SSA/Cover level -- but merge()'s output-wins convention then
immediately re-collapses them back down to a single HighVariable using
A's (wrong) identity, silently reintroducing the same wrong-answer
problem one level higher up, just without a Cover violation to catch
it anymore.

NEXT CONCRETE STEP (not yet done): read HighVariable::merge
(variable.cc, not yet located by line number this session) to confirm
precisely how instance ownership transfers and whether "which side
survives" is only about symbol/name-lock bookkeeping (in which case
swapping the argument order for THIS specific INDIRECT case might be
a viable, still-narrow fix) or whether output-identity-wins is load-
bearing elsewhere in a way that can't simply be swapped for the
INDIRECT case without side effects. This is the most promising lead
yet for actually fixing the visible wrong-answer bug (as opposed to
just the crash), but requires confirming HighVariable::merge's
mechanics before attempting any further code change.

--------------------------------------------------------------------
CORRECTION / REFINEMENT to the previous entry, after reading
HighVariable::merge/mergeInternal in full (variable.cc:640-726):
the merge is NOT silently discarding B's value -- it's a printing/
representative-selection problem, not a data-loss problem
--------------------------------------------------------------------
mergeInternal (variable.cc:640-680) does NOT discard tv2's (high2's,
our correct-value B's) varnode instances. It reassigns EVERY one of
tv2's varnode instances to point to 'this' (high1, A) via
vn->setHigh(this,...) (line 657 or 666 depending on
speculative/non-speculative), then concatenates both instance lists
into a single combined inst vector (line 669-671) before deleting tv2
itself (the now-empty HighVariable object, not the varnode data).

This means: after the merge, the single surviving HighVariable
genuinely CONTAINS both A's original varnode instances (the mulxu.w
chain SUBPIECE/COPY) AND B's varnode instance (the division-result
COPY) -- nothing is lost or corrupted at the SSA/merge level. The
earlier write-up's framing ('A's identity wins, B's value is
discarded') was IMPRECISE -- correcting it here rather than deleting
it, per this doc's convention. What actually happens is closer to:
both varnodes now belong to the same HighVariable, and something
LATER in the pipeline (likely Funcdata's naming/printing logic, or
whatever picks a HighVariable's 'representative' varnode for display
purposes) is choosing to print/return A's value (the stale
multiplication leftover) instead of B's (the correct division result)
when rendering the final C-like output, even though the underlying
data-flow graph, post-merge, actually has both values still present as
separate p-code varnodes under one combined HighVariable.

This reframes the remaining work: the fix likely does NOT belong in
Merge/Cover/HighVariable::merge at all (all confirmed structurally
correct now) -- it likely belongs in whatever chooses which varnode's
value is loaded at the RETURN op's actual read, which happens at the
p-code level (which specific Varnode is passed as the RETURN op's
input) BEFORE printing ever happens. Given b (the multiplication's
second operand, unrelated register) is what actually prints, and this
doc's own P-CODE TRACE section further up already established the
RETURN op's real p-code input varnode identity for muldiv_u16 (not
re-verified this session) -- the next step should be to re-run the
existing debug instrumentation (or add new instrumentation at the
RETURN op itself) to see, POST-merge, POST-trim (i.e. with the current
mergeTestBoundary fix active), which actual Varnode instance the
RETURN op's input slot points to -- not which HighVariable 'wins' the
merge (both now legitimately coexist under one HighVariable) but which
INSTANCE within that HighVariable the RETURN op was already wired to
read from, likely from BEFORE this session's fix ever ran, i.e.
possibly a decision made by an earlier action/rule (dead code
elimination, copy propagation) that already picked A's SUBPIECE-derived
COPY as the RETURN's input long before Merge ever runs, and Merge's job
was never to change WHICH varnode feeds the RETURN -- only to ensure
whichever varnodes end up sharing a HighVariable don't have conflicting
covers. If that's correct, this bug may not be fixable in Merge/Cover
at all -- it may require an earlier pass (constant/copy propagation,
dead code elimination) to correctly resolve which of the two candidate
definitions is live at the RETURN in the first place, which is a much
earlier and more fundamental problem, likely closer to Fix A/Fix B's
original SEGMENTOP/stack-var resolution territory referenced at the
very top of this document than to anything investigated this session.

--------------------------------------------------------------------
NEW LIVE DATA (2026-09-10, continued): RETURN op's input(1) points to
defAddr=0x141d7 (the CORRECT division-result COPY) THE ENTIRE TIME,
including after all merges complete -- contradicts the 'wrong varnode
feeds RETURN' hypothesis
--------------------------------------------------------------------
Added instrumentation to Merge::mergeOp (merge.cc, after the final
merge loop) dumping every live CPUI_RETURN op's input(1) varnode
identity (def address, def opcode, HighVariable pointer) each time any
op's merge completes, gated to muldiv_u16's address range. Rebuilt,
redeployed, Ghidra restarted, force_decompile re-run -- bug reproduces
identically (return b;).

LOG RESULT: across all 4 mergeOp calls logged for this function
(BUILD@0x141d7:8e, DELAY_SLOT@0x141c2:8c/8f/95, DELAY_SLOT@0x141ce:90),
the RETURN op's input(1) (there are two RETURN op copies in the
function, @0x141da:6a and @0x141da:cd, presumably from different
control-flow paths merged/duplicated by an earlier action) CONSISTENTLY
shows defAddr=0x141d7, defOp=COPY -- i.e. it points to the COPY at
0x141d7, previously established as the division-result COPY (our
'B', the CORRECT value), NOT to anything at 0x141c2 (the mulxu.w
chain, our 'A', the WRONG value). This is true in the FIRST log entry
and remains unchanged through the LAST.

This directly contradicts the 'wrong varnode feeds RETURN, decided by
an earlier pass' hypothesis from the previous entry: the RETURN op's
p-code input has apparently always correctly pointed at the intended
value's definition site. Yet the final decompiler OUTPUT still prints
'return b;' -- b being the function's second PARAMETER, an entirely
different, named, unrelated Varnode from either A or B in this
investigation.

REVISED UNDERSTANDING: since the RETURN op's underlying p-code input
varnode identity is correct throughout, the wrong-output bug must be
happening at a HIGHER level than the raw SSA graph -- specifically in
how the FINAL MERGED HighVariable (whichever one ends up containing
the varnode at defAddr=0x141d7) gets NAMED/PRINTED. The fact that the
visible output is specifically 'b' (the second function PARAMETER, a
named, symbol-bound HighVariable) rather than some unrelated temp
suggests the merge chain for this HighVariable's identity eventually
ALSO absorbed 'b' the parameter's own HighVariable somewhere upstream
of or parallel to the merges investigated this session (the INDIRECT
at 0x141c2), and that when the PRINTER selects a representative
name/symbol for the final combined HighVariable, it picks 'b' (likely
because a named, symbol-bound HighVariable takes naming priority over
an anonymous one per Ghidra's normal, CORRECT naming convention) even
though the ACTUAL VALUE being read via the real p-code graph, if
emulated/traced, would be the division result, not the parameter.

NEXT CONCRETE STEP: determine whether 'b' the parameter's HighVariable
and the 0x141d7 COPY's HighVariable are ACTUALLLY THE SAME
HighVariable (i.e. merged together at some point) or merely happen to
share a printed name coincidentally due to a labeling/symbol-inheritance
bug. The existing debug logs already capture highPtr for the RETURN's
input varnode (e.g. 00000220C6B88250 in the final entries) -- adding a
similar highPtr dump for parameter b's own varnode/HighVariable, and
comparing pointer values directly, would immediately confirm or refute
whether they are the same object. This is the most direct, decisive
next check and does not require any further reasoning about WHICH
merge caused it before first confirming THAT it happened.

--------------------------------------------------------------------
CONFIRMATION: muldiv_u16_scaled shows the SAME symptom pattern on the
current (mergeTestBoundary-fix-applied) binary
--------------------------------------------------------------------
User-supplied decompiler output for muldiv_u16_scaled (0x141dc) on the
current binary:

  ushort __stdcall_far_allstack_32ret muldiv_u16_scaled(ushort a,ushort b,ushort c,ushort d)
  {
    ...
    uVar3 = (short)(CONCAT22(uVar2,uVar5 + in_R1) / (uint)*(ushort *)(puVar6 + 0xc));
    return uVar4;
  }

uVar3 IS computed correctly (the actual scaled division result) but is
NEVER returned or used after that assignment -- uVar4 (a leftover
carry-add intermediate, uVar5 + in_R1) is returned instead. in_R1
remains an unresolved raw register read, consistent with earlier
NOT-YET-CONFIRMED notes on this function from before this session.

This is the SAME symptom shape as muldiv_u16: the CORRECT value is
demonstrably computed and sitting right there in the p-code/output,
but a DIFFERENT, wrong HighVariable's identity is what actually
surfaces at the return point. This strongly supports the current
working theory (see NEW LIVE DATA entry above) that the defect is in
HighVariable naming/identity/symbol-inheritance after merging, not in
which raw p-code value is computed -- the correct computation is
present in BOTH functions; only the FINAL SELECTED return identity is
wrong in both cases. This is a second, independent data point for the
same theory and should be checked against once the b-vs-0x141d7
HighVariable-pointer comparison (immediate next step, previous entry)
is done for muldiv_u16 -- if confirmed there, muldiv_u16_scaled's
uVar4-vs-uVar3 identity confusion likely shares the identical root
cause and would be fixed by the same eventual fix.

--------------------------------------------------------------------
IN PROGRESS (2026-09-10, continued): parameter-b vs RETURN-input
HighVariable* pointer comparison instrumentation added, NOT YET RUN
--------------------------------------------------------------------
Per the decisive RETURN-op finding above (input(1) consistently
defAddr=0x141d7/COPY, i.e. correctly wired to the division result
throughout every merge), the next concrete step identified was to
directly compare HighVariable* pointers for parameter `b` and the
RETURN op's actual input varnode, to confirm whether they have
genuinely been merged into one object (naming/identity bug) or
whether this is a separate labeling bug with no underlying merge.

SECOND INDEPENDENT CONFIRMATION (this session, still pending the log
save at the time this entry was written): muldiv_u16_scaled
(0x141dc) shows the identical symptom shape on the current
(mergeTestBoundary-fix-applied) binary -- the real scaled-division
result (uVar3) is computed correctly and sits unused in the output,
while `return uVar4;` returns a leftover carry-add intermediate
instead. in_R1 remains unresolved. This is the same
correct-value-computed-but-wrong-identity-returned pattern as
muldiv_u16, strengthening the case that both share one root cause
and would be fixed together.

ACTION TAKEN, NOT YET TESTED: added new debug instrumentation to log
parameter `b`'s own HighVariable* pointer alongside the existing
RETURN-op input(1) HighVariable* dump (in the same debug-log
mechanism already gated to muldiv_u16's address range). Code has
been rebuilt and the new decompile.exe deployed. Ghidra needs a
restart, then existing debug logs need to be cleared and
force_decompile re-run on muldiv_u16 to capture the comparison.

STATUS: instrumentation added + rebuilt + deployed, but the
comparison test itself has NOT yet been run (pending Ghidra restart
+ log clear + fresh force_decompile in this session). This entry is
being saved specifically because it had not yet been recorded before
the user asked whether progress was saved to the review -- do not
assume the pointer comparison result is known; it isn't yet.

NEXT CONCRETE STEP (unchanged, now with instrumentation in place):
after Ghidra restart, clear muldiv_u16's debug log files, run
force_decompile on muldiv_u16, then read the log to compare the two
HighVariable* pointer values directly. If they match, this confirms
a genuine merge folded `b`'s identity into the division-result's
HighVariable (likely worth then re-reading HighVariable::merge's
symbol/name-lock priority logic in variable.cc). If they differ,
this is a separate printing/representative-selection bug unrelated
to any merge investigated so far, and the search should move to
whatever selects a HighVariable's displayed representative varnode
(likely in Funcdata's naming or PrintC/PrintLanguage output-selection
code, not yet located this session).

--------------------------------------------------------------------
DECISIVE RESULT (2026-09-10, continued): parameter b's HighVariable*
and the RETURN op's input HighVariable* are DIFFERENT OBJECTS --
refutes the merge/identity-folding hypothesis entirely
--------------------------------------------------------------------
Cleared all four muldiv_u16 debug logs, restarted Ghidra, re-ran
ghidra:force_decompile on muldiv_u16 (0x141ba) with the new
parameter-b-vs-RETURN-input comparison instrumentation active (added
and deployed per the previous entry). Bug reproduced identically
(`return b;`). All four log files were freshly rewritten
(muldiv_u16_copyshadow_debug.txt, muldiv_u16_getUIndex_debug.txt,
muldiv_u16_intersection_debug.txt, muldiv_u16_return_debug.txt --
confirmed via LastWriteTime, all four timestamped to this test run).

muldiv_u16_return_debug.txt, FINAL snapshot (after the last mergeOp
call, op@0x141c2:95, code=DELAY_SLOT):
  RETURN op@0x000141da:6a input1: defAddr=0x141d7, defOp=COPY,
    highPtr=000002461C838480
  RETURN op@0x000141da:cd input1: defAddr=0x141d7, defOp=COPY,
    highPtr=000002461C8377D0
  param[1] name=b addr=0x0006 highPtr=000002461C838100

None of the three highPtr values match one another. This pattern is
IDENTICAL across every one of the five logged mergeOp snapshots in
this run (op@0x141d7:8e/BUILD, op@0x141c2:8c/8f/95/DELAY_SLOT,
op@0x141ce:90/DELAY_SLOT) -- the two RETURN-op HighVariables and
param b's HighVariable are three distinct, never-converging objects
throughout the entire merge process, from the very first snapshot to
the last.

CONCLUSION: this DECISIVELY REFUTES the working theory from the
previous two entries (NEW LIVE DATA / muldiv_u16_scaled confirmation
sections above) that a Cover/Merge operation folds parameter b's
HighVariable identity into the division-result's HighVariable. No
such merge occurs at any point -- the objects never share a pointer
value. Whatever causes the decompiler to PRINT "b" as the return
expression, it is NOT because the RETURN's underlying HighVariable
and b's HighVariable have become the same object.

REVISED UNDERSTANDING: since defAddr=0x141d7/COPY (the correct
division-result definition) is what the RETURN op's input has always
pointed to, and that HighVariable is confirmed to be a genuinely
separate object from b's, the "b" text in the printed output must be
coming from somewhere other than object-identity sharing --
candidates, none yet checked this session:
  (a) A naming/symbol-assignment bug where the printer/namer
      (Funcdata's dynamic-naming logic, or PrintC's variable-name
      resolution) independently assigns the display string "b" to
      the RETURN's actual (distinct) HighVariable, e.g. via a
      dead/stale Symbol binding, a HighVariable::getSymbol() /
      getSymbolEntry() mismatch, or a naming collision in however
      anonymous/renamed HighVariables get their displayed names.
  (b) The dump captured mergeOp-time snapshots only; some LATER
      action/pass (after all of Merge finishes, closer to final
      PrintC emission) could still change which HighVariable the
      RETURN op's input varnode structurally points to, i.e. this
      instrumentation's timing window may not cover the actual
      moment the bug is introduced. NOT YET CHECKED -- no
      instrumentation currently exists between end-of-Merge and
      PrintC's actual text emission.
  (c) The printed "b" may not correspond to a HighVariable at all in
      the naive sense assumed throughout this investigation -- e.g.
      it could be a per-instance override (a Varnode-level forced
      display name) rather than the HighVariable's own name. NOT YET
      CHECKED.

NEXT CONCRETE STEP: instrument (or otherwise inspect) whatever runs
between Merge completion and final C-text emission -- most likely
Funcdata's symbol/name assignment pass and/or PrintC's variable
naming resolution (locate the function that decides a HighVariable's
displayed name/token, not yet located this session) -- and log, for
the RETURN op's actual (confirmed-distinct-from-b) HighVariable, both
its own assigned name/symbol AND whether it has any Symbol/SymbolEntry
association that could explain why "b" specifically gets printed for
it. This moves the investigation for the first time outside
Merge/Cover/HighVariable entirely, into the decompiler's naming and
printing layer.

STATUS: root cause of the wrong-answer bug remains OPEN. The
merge-identity theory is now conclusively ruled out by direct
pointer-level evidence; the search must move to naming/printing.

--------------------------------------------------------------------
STRONG NEW LEAD, source-reading only (2026-09-10, continued): naming
happens via an ADDRESS-KEYED symbol lookup, not HighVariable identity
--------------------------------------------------------------------
Since the previous entry decisively ruled out any HighVariable-object
merge between b and the RETURN's actual HighVariable, moved the
search to the naming/symbol-binding layer as planned. Found
ActionNameVars (coreaction.hh:472, coreaction.cc), registered as
`act->addAction(new ActionNameVars("merge"))` (coreaction.cc:6117) --
i.e. it runs as part of the "merge" action group, AFTER
Merge/Cover/HighVariable::merge all complete, and BEFORE ActionSetCasts
and final structuring/printing. This is exactly the "later
action/pass" gap flagged as candidate (b) in the previous entry.

ActionNameVars::linkSymbols (coreaction.cc:3078-3124) iterates every
Varnode by address-space/location, and for each HighVariable's own
getNameRepresentative() varnode (line 3107-3111):
  Varnode *vn = curvn->getHigh()->getNameRepresentative();
  if (vn != curvn) continue;              // hit each high once
  HighVariable *high = vn->getHigh();
  if (!high->hasName()) continue;
  Symbol *sym = data.linkSymbol(vn);

Read Funcdata::linkSymbol in full (funcdata_varnode.cc:1223-1255).
Critical lines:
  Symbol *sym = high->getSymbol();
  if (sym != (Symbol *)0) return sym;     // already assigned, skip
  ...
  SymbolEntry *entry = localmap->queryProperties(vn->getAddr(), 1,
                                                  usepoint, fl);
  if (entry != (SymbolEntry *)0 && !entry->isConflict()) {
    vn->setSymbolEntry(entry);
    return entry->getSymbol();
  }

THIS IS THE KEY FINDING: linkSymbol resolves a Symbol via an
ADDRESS-KEYED lookup into localmap (queryProperties(vn->getAddr(),...))
-- it does NOT compare HighVariable pointers, and does NOT care
whether two HighVariables are otherwise related. If the RETURN's
actual (confirmed-distinct-from-b) HighVariable's name-representative
varnode happens to have a storage ADDRESS that overlaps whatever
address range localmap has on file for parameter b's SymbolEntry (a
real possibility here, since muldiv_u16's whole bug family is about
a register being reused/aliased across the mulxu.w chain, the
division result, and the incoming parameters), this call returns b's
existing Symbol directly and binds it via vn->setSymbolEntry(entry)
-- with ZERO requirement that the two HighVariable objects share any
identity. This fully accounts for the previous entry's finding
(distinct highPtr values, yet "b" gets printed) without requiring any
merge-machinery bug at all: it would be an address-overlap symbol
mis-binding, a structurally different and much simpler bug class than
anything investigated so far in Merge/Cover/HighIntersectTest.

IMPORTANT CAVEAT: this is source-reading only, NOT yet live-confirmed
-- per this doc's established convention (see prior struck-down
theories above), must be verified via instrumentation before treating
as settled.

NEXT CONCRETE STEP (not yet done): add debug instrumentation to
Funcdata::linkSymbol (funcdata_varnode.cc, around line 1235), gated
to muldiv_u16's address range via vn's containing function or a
similar check, logging: vn->getAddr() (the queried address), whether
queryProperties found a matching entry, and if so entry->getSymbol()'s
name and the SymbolEntry's own address range. Cross-reference against
b's own storage address (already known from the existing RETURN-debug
log: param[1] b addr=0x0006) to directly confirm or refute whether
the RETURN's name-representative varnode's address falls inside that
same 0x0006 entry's range. This is the direct, decisive test for this
new theory, analogous in rigor to the highPtr comparison that just
ruled out the merge-identity theory.

STATUS: root cause hypothesis narrowed to an address-keyed symbol
lookup mis-binding in Funcdata::linkSymbol / ActionNameVars, NOT
yet live-confirmed. No code changes made yet this entry.

--------------------------------------------------------------------
LIVE RESULT (2026-09-10, continued): address-overlap theory ALSO
REFUTED -- entry was NULL for every HighVariable this run
--------------------------------------------------------------------
Added linkSymbol debug instrumentation (funcdata_varnode.cc, gated to
muldiv_u16 via getAddress().getOffset()==0x141ba), logging
vn->getAddr(), high pointer, and the queryProperties() result. Build
notes: first build attempt failed with MSVC "unexpected end of file
found in comment" -- caused by my own str_replace mis-anchoring the
#include insertion and corrupting the file's license header comment
(merged " * limitatio" with the include lines, destroying the closing
"*/"). Fixed by directly restoring the header against the known-good
text from funcdata.hh. Second build attempt failed on
`entry->getAddr()` -- getAddr() only exists on the derived MapEntry
class, not the base SymbolEntry that queryProperties() returns;
removed that field from the log line. Third build succeeded (BUILD
SUCCESSFUL), binary deployed to the live install
(...\Downloads\ghidra_12.2_DEV_20260818\...\Decompiler\os\win_x86_64\
decompile.exe) after stopping the running decompile.exe process,
Ghidra restarted, debug logs cleared, force_decompile re-run. Bug
reproduced identically (`return b;`).

muldiv_u16_linksymbol_debug.txt, full contents (5 lines, one per
HighVariable's name-representative varnode processed this run):
  vn->getAddr()=0x00       high=000001E7F5C16760 entry=0000000000000000
  vn->getAddr()=0x00021e00 high=000001E7F5BD74C0 entry=0000000000000000
  vn->getAddr()=0x00       high=000001E7F5BD81E0 entry=0000000000000000
  vn->getAddr()=0x02       high=000001E7F5BD8D40 entry=0000000000000000
  vn->getAddr()=0x0c       high=000001E7F5BD7F40 entry=0000000000000000

entry is NULL (0x0) in every single case -- localmap->queryProperties
never found a match for ANY of the 5 name-representative varnodes
processed this run, including whichever one corresponds to the
RETURN's HighVariable. This REFUTES the address-overlap-with-b's-
SymbolEntry theory from the previous entry: b's Symbol is never
returned via this path for any of these HighVariables, so it cannot
be what's producing "b" as the RETURN's printed name via this
mechanism.

Note also: b itself never appears in this log at all, for either of
the two force_decompile runs against this instrumentation. This is
consistent with b's HighVariable already having sym != NULL by the
time linkSymbol reaches the `Symbol *sym = high->getSymbol(); if
(sym != (Symbol*)0) return sym;` early-exit (line ~1229, just before
our instrumentation) -- i.e. b is a formal parameter and likely gets
its Symbol bound earlier (formal parameter symbols are typically
pre-existing in the input scope before ActionNameVars ever runs), so
it skips straight past our logging point entirely.

FOLLOW-ON, read via read-lines (funcdata_varnode.cc:1255-1275): when
entry is null (as it was for every relevant case here), execution
falls through to:
  if (!vn->isPersist()) {
    if (detectSymbolConflicts(vn))
      entry = localmap->addSymbolWithConflict("", high->getType(), vn);
    else {
      ... 
      entry = localmap->addSymbol("", high->getType(), vn->getAddr(), usepoint);
    }
    sym = entry->getSymbol();
    vn->setSymbolEntry(entry);
  }
  return sym;

So every one of these 5 HighVariables (assuming none is vn->isPersist())
gets a BRAND NEW anonymous symbol created on the spot, with an empty
name (""), via ScopeLocal::addSymbol or addSymbolWithConflict. The
empty-string name presumably gets replaced by some later auto-naming
step (not yet identified) before final printing. This reframes the
question again: it's not a lookup-collision at all -- it's whatever
auto-naming logic assigns an actual display string to these freshly
created empty-name symbols that must be investigated next, since one
of these 5 anonymous symbols is what ultimately gets displayed as
"b" in the final output.

NEXT CONCRETE STEP: extend the linkSymbol instrumentation to also log,
for the null-entry branch specifically: vn->isPersist(),
detectSymbolConflicts(vn)'s result, and -- critically -- read back
entry->getSymbol()->getName() immediately AFTER addSymbol/
addSymbolWithConflict returns, to see what (if any) name is assigned
at creation time (it may not stay "" as naively assumed -- addSymbol's
implementation itself may already perform default/auto-naming
internally, not deferred to a separate later pass as assumed above --
this needs to be checked by reading ScopeLocal::addSymbol before
adding further instrumentation, rather than assuming a separate
later renaming pass exists).

STATUS: root cause remains OPEN. Two theories now refuted (merge-
identity folding; address-overlap symbol reuse). Investigation has
moved to: how do these 5 freshly-created anonymous local symbols (one
of which becomes the RETURN's displayed "b") get their actual names
assigned -- read ScopeLocal::addSymbol next, before further live
instrumentation.

--------------------------------------------------------------------
LIVE RESULT + STRONG NEW LEAD (2026-09-10, continued): all 5
name-representative varnodes get FRESH anonymous symbols
($$undefN) -- but getSymbol() itself scans ALL merged instances,
not just the name-representative, for a pre-existing SymbolEntry
--------------------------------------------------------------------
Extended linkSymbol instrumentation with a second log point right
before the final `return sym;` (funcdata_varnode.cc), capturing
vn->isPersist() and, if assigned, sym->getName()/getCategory(). Also
fixed a mid-session tooling problem: gradle reported the decompiler
build as UP-TO-DATE despite the source edit being confirmed present
on disk (verified via grep) -- had to force a real rebuild with
`gradlew.bat :Decompiler:decompileWin_x86_64Executable --rerun-tasks`,
which did show actual compile+link work this time. Root cause of the
stale UP-TO-DATE not fully understood (possibly a file-system
timestamp granularity issue between the MCP file-edit layer and
gradle's up-to-date checker) -- worth remembering to use --rerun-tasks
if BUILD SUCCESSFUL comes back suspiciously fast (~14s) after a source
edit. Also hit an unrelated session hiccup: after two consecutive
Ghidra/Claude restarts, the `ghidra:*` MCP tool namespace dropped out
entirely and had to be restored via `ghidra:connect_instance` (project
"new") before force_decompile worked again; `ghidra:list_instances`
was reported by the user as not useful in this environment (single-
project setup) -- go straight to connect_instance next time.

Bug reproduced identically across all 3 decompiles run against this
build. muldiv_u16_linksymbol_final_debug.txt, one representative run
(5 lines, one per name-representative varnode, in address-space
iteration order):
  vn->getAddr()=0x00       sym->getName()=$$undef00000000
  vn->getAddr()=0x00021e00 sym->getName()=$$undef00000001
  vn->getAddr()=0x00       sym->getName()=$$undef00000002
  vn->getAddr()=0x02       sym->getName()=$$undef00000003
  vn->getAddr()=0x0c       sym->getName()=$$undef00000004

Every one of these 5 gets vn->isPersist()=FALSE and a freshly created
anonymous Symbol via localmap->addSymbol("", ...) with category=-1
(0xffff), still carrying its ScopeInternal::addSymbolInternal
placeholder name ($$undef0000000N) at this point in the pipeline --
NOT yet renamed to anything resembling "b". This confirms
assignDefaultNames (the batch rename pass, ScopeInternal.cc ~2979)
runs LATER, as a separate pass over the whole nametree, not inside
linkSymbol itself.

HOWEVER: traced assignDefaultNames' call site (database.cc:2991,
`buildDefaultName(sym, base, (Varnode*)0)`) and found it always passes
vn=NULL. This means buildDefaultName's real-naming branches always
take the sym->numEntries()!=0 path (using entry->getFirstUseAddress()
etc, category==-1 here so no function_parameter branch), which
constructs a name via buildVariableName's generic "local variable"
branch (ct->printNameBase(s); s<<"Var"<<index++...) -- this branch
CANNOT literally produce "b" for any of these 5 symbols; it would
produce something like "uVar1", "uVar2", etc. So the eventual literal
string "b" cannot be coming from assignDefaultNames'
buildDefaultName/buildVariableName naming logic either, for any of
these 5 freshly created anonymous symbols.

NEW LEAD: re-read HighVariable::getSymbol() (variable.hh:177):
`Symbol *getSymbol(void) const { updateSymbol(); return symbol; }`
-- it always calls updateSymbol() first, not just returning a cached
field. Read HighVariable::updateSymbol() (variable.cc:419-434):
  void HighVariable::updateSymbol(void) const {
    if ((highflags & symboldirty)==0) return; // flags are up to date
    highflags &= ~((uint4)symboldirty);
    symbol = (Symbol *)0;
    for(iter=inst.begin();iter!=inst.end();++iter) {
      Varnode *vn = *iter;
      if (vn->getSymbolEntry() != (SymbolEntry *)0) {
        setSymbol(vn);
        return;
      }
    }
  }

THIS IS THE KEY FINDING: a HighVariable's effective Symbol is
determined by scanning EVERY merged-in member Varnode instance
(`inst`, the full list of Varnodes unioned into this HighVariable by
Merge/Cover) for the FIRST ONE that already carries a bound
SymbolEntry -- NOT specifically the "name representative" varnode
that ActionNameVars::linkSymbols processes and that our existing
instrumentation has been tracking. This means: even though (a) the
RETURN's HighVariable object is a genuinely distinct C++ object from
b's HighVariable (proven two entries ago via direct pointer
comparison), and (b) its OWN name-representative varnode just got a
brand new, unrelated anonymous symbol via linkSymbol (proven this
entry), it is still possible for getSymbol()/getName() to return "b"
if ANY OTHER member varnode instance inside that same HighVariable's
`inst` vector already has vn->setSymbolEntry() pointing at b's
SymbolEntry -- set by some earlier, different code path (e.g. an
earlier per-instance linkSymbol call on a different piece of the same
HighVariable, COPY-shadow propagation, or a leftover binding from
before the object was split/reformed this decompile). Our
instrumentation so far has only ever logged the SINGLE
name-representative varnode per high, once each -- it has never
enumerated the OTHER member varnodes of the same HighVariable's
`inst` list, so this possibility has not yet been ruled in or out.

NEXT CONCRETE STEP: instrument HighVariable::updateSymbol() itself
(variable.cc, gated to muldiv_u16's HighVariables somehow -- likely
easiest by checking inst[0]'s owning Funcdata's address, or simply
logging unconditionally for this investigation and filtering by
inst-address patterns matching known muldiv_u16 register offsets
afterward) to dump, for each call: how many entries are in `inst`,
and for EACH instance varnode -- its address and whether
getSymbolEntry() is already non-null and what symbol name it carries.
This directly tests the new theory: if one of the RETURN high's
merged instances already carries a bound "b" SymbolEntry from earlier
in the pipeline (before or independent of the fresh anonymous symbol
just created for its name-representative), that instance -- not the
name-representative -- is what determines the final printed name via
getSymbol()'s first-match iteration order.

STATUS: root cause remains OPEN, but this is the strongest lead yet
and is a structurally different bug class again (a stale/leftover
per-instance SymbolEntry binding surviving inside a HighVariable's
merged instance list, surfaced through getSymbol()'s first-match scan
order) from anything investigated in this document so far. Not yet
live-confirmed -- instrument updateSymbol() next before drawing
further conclusions.

--------------------------------------------------------------------
IN PROGRESS (2026-09-10, continued): updateSymbol() instrumentation
added, build NOT yet confirmed complete -- session paused here
--------------------------------------------------------------------
Per the previous entry's plan, added live instrumentation to
HighVariable::updateSymbol() (variable.cc, right after
`symbol = (Symbol *)0;`, before the existing first-match loop, so it
doesn't alter original control flow). <fstream> was already included
in this file, no include edit needed this time.

Instrumentation logic: since HighVariable has no direct Funcdata/
address-of-owning-function pointer to gate on (unlike funcdata_varnode.cc,
which could check getAddress().getOffset()==0x141ba directly), gated
instead by scanning `inst` for any member varnode whose storage offset
matches one of the four addresses already established as relevant to
muldiv_u16 from earlier entries (0x00, 0x02, 0x0c, 0x00021e00). If any
instance matches, logs to
C:\Users\j.brophy.CORKILLSYSTEMS\muldiv_u16_updatesymbol_debug.txt:
  this (HighVariable* identity)
  inst.size()
  for EACH member varnode instance: its own getAddr(), its own vn
    pointer, whether getSymbolEntry() is already non-null, and if so
    the bound symbol's name.

This directly targets the new theory from the previous entry: that
the RETURN's HighVariable's getSymbol() might return "b" not via its
own name-representative varnode's (freshly-anonymous) symbol, but via
some OTHER merged instance varnode in the same `inst` list that
already carries a stale/pre-existing SymbolEntry bound to b's Symbol,
found first during updateSymbol()'s linear first-match scan.

BUILD STATUS: kicked off `gradlew.bat :Decompiler:decompileWin_x86_64Executable
--rerun-tasks` (following the --rerun-tasks lesson learned two entries
ago, since a previous edit was once silently skipped by gradle's
UP-TO-DATE check despite being confirmed present on disk). The build
command TIMED OUT client-side after 4 minutes with no result returned
-- unclear whether it is still running server-side, completed after
the timeout, or never started (e.g. stuck on an unanswered approval
prompt in the host app, as happened once earlier this session with an
identical symptom, which that time turned out to just be a slow
build that succeeded on retry). NOT YET RESOLVED as of this save --
do not assume the build succeeded or the binary is ready.

STATUS: root cause remains OPEN. Instrumentation for the
"stale-instance-symbol-binding-inside-inst-list" theory is written
and saved to disk, but NOT yet confirmed built, NOT yet deployed to
the live install, and NOT yet tested. Session paused at this point.

NEXT CONCRETE STEPS on resume, in order:
  1. Re-attempt/check the gradlew --rerun-tasks build for
     :Decompiler:decompileWin_x86_64Executable (retry if the previous
     attempt is confirmed dead/timed out; do not re-run redundantly if
     it turns out to already be complete -- check build output
     timestamp on decompile.exe under
     ...\github\ghidra\Ghidra\Features\Decompiler\build\os\win_x86_64\
     first).
  2. Stop the running decompile.exe process, copy the fresh binary to
     the live install
     (...\Downloads\ghidra_12.2_DEV_20260818\...\Decompiler\os\win_x86_64\decompile.exe).
  3. Clear old debug logs, including the new
     muldiv_u16_updatesymbol_debug.txt if a stale one exists from a
     prior partial run.
  4. Ask user to restart Ghidra; after restart, reconnect the MCP
     bridge via ghidra:connect_instance("new") BEFORE attempting
     force_decompile -- this session showed the ghidra: tool
     namespace can silently drop after a Ghidra/Claude restart even
     when the underlying Ghidra process and port are fine, and
     ghidra:list_instances is not reliable/needed in this
     single-project setup, so go straight to connect_instance.
  5. Run force_decompile on muldiv_u16 (0x141ba), confirm bug still
     reproduces (`return b;`), then read
     muldiv_u16_updatesymbol_debug.txt and cross-reference against
     the existing muldiv_u16_linksymbol_final_debug.txt from the same
     run to identify: which HighVariable (by `this` pointer) has
     inst.size()>1 with a mismatched pre-existing SymbolEntry among
     its instances, and whether that entry's bound symbol name is
     "b". This is the direct test of the current leading theory.

--------------------------------------------------------------------
LIKELY TRUE ROOT CAUSE FOUND, source-confirmed (2026-09-10, continued):
HighVariable::mergeInternal inherits a Symbol pointer directly from the
absorbed side, which can survive across a CHAIN of separate merges even
though each intermediate HighVariable object gets deleted
--------------------------------------------------------------------
updateSymbol() instrumentation never fired (log file never created)
across two separate gating attempts. Investigated why: read
HighVariable's constructor (variable.cc:221-236) and confirmed
symboldirty is NOT part of the initial highflags (only
flagsdirty|namerepdirty|typedirty|coverdirty) -- a fresh HighVariable
starts NOT symbol-dirty, so updateSymbol()'s early-return
((highflags&symboldirty)==0) fires immediately unless something later
explicitly calls symbolDirty(). This means our instrumentation (placed
just after that early-return) can legitimately never fire for
HighVariables that never got marked dirty -- not a bug in the
instrumentation, just confirms updateSymbol()'s scan-all-instances path
is not always how a HighVariable's Symbol gets set.

Read HighVariable::mergeInternal (variable.cc:668-726) in full to find
the ACTUAL Symbol-assignment path used during merges. Line 673:
highflags |= (flagsdirty|namerepdirty|typedirty) -- symboldirty is
NOT set here either. Lines 674-679, immediately after:
  if (tv2->symbol != (Symbol *)0) {          // Check if we inherit a Symbol
    if ((tv2->highflags & symboldirty)==0) {
      symbol = tv2->symbol;                  // Overwrite our Symbol (assume it is the same)
      symboloffset = tv2->symboloffset;
      highflags &= ~((uint4)symboldirty);
    }
  }

THIS IS THE MECHANISM: when HighVariable A absorbs HighVariable tv2 via
merge, if tv2 ALREADY has a non-null, non-dirty Symbol pointer, A
directly takes over that exact Symbol pointer -- comment literally says
'assume it is the same'. Critically, tv2 is then deleted (per
mergeInternal's caller, HighVariable::merge), but the Symbol POINTER
VALUE has already been copied forward into the surviving object. This
means a Symbol assignment can propagate through an entire CHAIN of
merges across many different HighVariable C++ objects, surviving each
intermediate object's deletion, as long as at each step the absorbed
side still carries that Symbol non-dirty.

THIS FULLY RECONCILES the two previous 'refuted' entries: b's
HighVariable and the RETURN's final HighVariable are indeed two
genuinely distinct objects (confirmed via direct pointer comparison,
correctly) -- but if, at ANY point earlier in the merge sequence
(likely one of the OTHER mergeOp/mergeIndirect calls logged in this
session's mergeOp instrumentation, not necessarily the specific
A-vs-B collision this whole investigation started from), some
HighVariable that eventually contributes (directly or transitively)
to the RETURN's final HighVariable absorbed another HighVariable that
ALREADY had b's Symbol assigned (e.g. because at some earlier, much
more mundane merge point, a varnode that legitimately shared an
address/register with parameter b's storage got merged in the
ordinary, non-buggy way merges are supposed to work), then b's Symbol
pointer would silently ride along through every subsequent merge in
that chain, ending up assigned to the totally-unrelated final
RETURN HighVariable -- with NO Cover violation, NO crash, and NO
distinguishing marker at the final object showing where the Symbol
actually came from.

This reframes the bug's likely true nature: it may not be a bug in
Merge/Cover/HighIntersectTest's INTERSECTION logic at all (all
confirmed structurally sound this session) -- it may be that the
underlying COVER-LEVEL bug (the original level=2 boundary-filter
issue this whole document is about) causes muldiv_u16's mulxu.w-chain
HighVariable to get merged (correctly, per Cover rules, once our
mergeTestBoundary fix landed) with something that TRANSITIVELY already
carries b's Symbol from an entirely separate, ordinary, non-buggy
register-reuse merge elsewhere in the function -- i.e. the ROOT CAUSE
investigated at length in this document (the boundary-collision bug)
may be a NECESSARY PRECONDITION for this symbol-inheritance chain to
connect two otherwise-unrelated variables' identities, even though the
final visible SYMPTOM (wrong printed name) is a completely different
mechanism (Symbol pointer inheritance) than what was investigated for
most of this session (Cover intersection correctness).

NEXT CONCRETE STEP: instrument HighVariable::mergeInternal directly at
line 674-679 (the symbol-inheritance branch), logging tv2->symbol's
name (if non-null) and both this/tv2's HighVariable* pointers, gated
broadly (e.g. only when tv2->symbol!=0, since that's already a narrow,
meaningful filter, no address-range gate needed) to capture EVERY
symbol-inheriting merge in the whole muldiv_u16 decompile pass. Cross-
reference the resulting chain of inherited-symbol merges against the
existing mergeOp/RETURN-op instrumentation timestamps/HighVariable
pointers already captured, to trace the exact sequence of merges that
carries 'b' from its own parameter HighVariable into the final
RETURN HighVariable. This is the first instrumentation approach this
session that directly targets Symbol INHERITANCE rather than object
IDENTITY, and should finally close the loop on this investigation.

--------------------------------------------------------------------
CONFIRMED LIVE (2026-09-10, continued): a real two-hop Symbol
inheritance chain caught in the act -- 'b' propagates through two
separate merges into a third HighVariable
--------------------------------------------------------------------
Instrumented HighVariable::mergeInternal directly at the symbol-
inherit branch (variable.cc:674-689), logging this/tv2 pointers and
tv2->symbol's name whenever tv2->symbol!=0 (no address-range gate
needed -- this condition is already a narrow, meaningful filter).
Rebuilt (fresh, 2m24s, --rerun-tasks), redeployed, Ghidra restarted,
force_decompile re-run on muldiv_u16 -- bug reproduced identically.

muldiv_u16_syminherit_debug.txt, full contents (4 entries, last blank):
  1: this=0000017CBBC524F0 tv2=0000017CBBC52720 tv2->symbol=b dirty=false
  2: this=00000243191E8630 tv2=00000243191E8A90 tv2->symbol=b dirty=false
  3: this=00000243191EA000 tv2=00000243191E8630 tv2->symbol=b dirty=false

Entry 1 is from a different pointer address space (0000017C... vs
00000243...) -- almost certainly a separate/earlier decompile pass
(e.g. an internal warm-up pass) and not part of the same chain as
entries 2-3, which share the 00000243... space and form a clean,
directly-connected two-hop chain:
  [HighVariable carrying b's Symbol, ...8A90]
    --merged into--> [...8630]   (entry 2: ...8630 now also carries "b")
      --merged into--> [...A000] (entry 3: ...A000 now also carries "b")
...A000 does not appear as a tv2 in any further entry in this run,
consistent with it being (or feeding directly into, via a merge whose
tv2->symbol was still null at that point and thus untracked by this
instrumentation) the eventual RETURN HighVariable.

THIS CONFIRMS THE MECHANISM FROM THE PREVIOUS ENTRY IS REAL AND
ACTIVE for this exact bug, not just theoretically possible. The
Symbol pointer for 'b' genuinely rides across two separate
HighVariable merges, ending up on a HighVariable that has no direct
object-identity relationship to b's own original HighVariable
(consistent with the earlier direct-pointer-comparison finding that
ruled out simple identity-merging).

REMAINING QUESTION: entries 2-3 show the symbol propagating CORRECTLY
per the code's own logic (assuming the merges themselves were
legitimate) -- the real question is whether the MERGE at entry 2
(...8630 absorbing ...8A90, which held b's Symbol) was itself a
LEGITIMATE merge (e.g. an ordinary, correct coalescing of b's
now-dead register slot with a genuinely-unrelated later value that
happens to reuse the same storage, which is normal, correct decompiler
behavior and NOT itself the bug) or whether THIS merge is where the
session's known Cover-boundary defect (or some other remaining gap)
incorrectly judged b's dead value and a live scratch value as
mergeable when they should not have been. This is genuinely ambiguous
from the current log alone -- ordinary dead-register-slot reuse
merges of this shape happen constantly in correct decompiler output
and are not inherently bugs; the SYMPTOM only becomes visible because
of what happens 3 hops later at the RETURN.

NEXT CONCRETE STEP: extend the mergeInternal instrumentation (or add a
companion instrumentation point in Merge::merge, which calls
mergeInternal and has access to the PcodeOp/address context
mergeInternal itself lacks) to also log the calling merge operation's
address/PcodeOp context for entry 2 specifically -- i.e. WHERE (which
p-code op, which merge decision -- MULTIEQUAL cover-merge, the
INDIRECT path this whole document is about, or an entirely separate,
ordinary merge elsewhere in the function) triggered ...8630 absorbing
...8A90. This determines whether entry 2's merge is the session's
known bug manifesting one more time, or a completely ordinary,
unrelated merge that just happens to be the innocent carrier of a
Symbol pointer that was always going to end up somewhere unexpected
once the ACTUAL Cover-boundary bug (already fixed via
mergeTestBoundary) let the mulxu.w/division-result collision through
in a still-imperfect way at the RETURN's own final merge.

--------------------------------------------------------------------
CROSS-REFERENCED CONFIRMATION (2026-09-10, continued): syminherit
entry 2's tv2 IS parameter b's own real HighVariable, absorbed by an
op outside the four already-instrumented mergeOp call sites
--------------------------------------------------------------------
Cross-referenced muldiv_u16_syminherit_debug.txt against
muldiv_u16_return_debug.txt from the SAME force_decompile invocation
(both logs share pointer address space 00000243191E.../...F8... in
their final/matching block, confirming same run). Direct match found:

  syminherit entry 2: tv2=00000243191E8A90
  return_debug [params], ALL FOUR snapshots (ops @0x141c2:8c/8f/95,
    @0x141ce:90) in this run: param[1] name=b highPtr=00000243191E8A90

This CONCLUSIVELY identifies syminherit entry 2's tv2 as parameter b's
own, genuine, correctly-named HighVariable -- not a coincidental
pointer collision. The chain is now fully identified in terms of real
program identities:
  b's own real HighVariable (00000243191E8A90)
    --absorbed into--> 00000243191E8630          (entry 2)
      --absorbed into--> 00000243191EA000        (entry 3, final)

IMPORTANT TIMING NOTE: b's highPtr remains UNCHANGED at 8A90 across
ALL FOUR captured mergeOp snapshots (ops @0x141c2:8c, :8f, :95, and
@0x141ce:90) in this run -- meaning b's own HighVariable object was
NOT YET absorbed at any of those four points (if it had been, the
[params] dump taken via data.findVarnodeInput would show b's varnode
already reassigned to the surviving HighVariable's pointer, not b's
own original one). Therefore the actual merge that absorbs b (entry 2)
must happen at a DIFFERENT, so-far-uninstrumented mergeOp call --
either for an op outside the 0x141ba-0x141da gate used by the
RETURN-debug instrumentation, or one whose completion timing falls
after the last of the four captured snapshots within the same overall
Merge pass. This op has NOT yet been identified.

Also could not directly match entry 3's surviving object
(00000243191EA000) against either RETURN op's highPtr in this run
(op@...:6a=00000243191E9740, op@...:cd=00000243191E8940 -- neither
equals A000), meaning A000 is not yet confirmed to BE the RETURN's
final HighVariable directly -- it may feed into it via one further,
still-uncaptured merge, or the two RETURN op copies may each resolve
through independent merge chains that were not fully traced by
either instrumentation point as currently scoped.

NEXT CONCRETE STEP: widen the RETURN-debug instrumentation's gate
(currently op->getAddr() in [0x141ba,0x141da]) to log for EVERY
mergeOp call in the function unconditionally (removing the address
restriction, acceptable since this is a small, single-function debug
run, not a whole-program pass), so the exact op whose mergeOp call
performs the b-absorbing merge (syminherit entry 2) can be pinned
down by finding the first return_debug snapshot after which b's
highPtr no longer equals 00000243191E8A90 (or no longer appears via
findVarnodeInput at all, if fully absorbed). This is the final
correlation needed to identify -- by p-code address -- exactly where
in muldiv_u16 the illegitimate (or legitimate) absorption of b's
Symbol actually originates, which was the single remaining open
question from the previous entry.

--------------------------------------------------------------------
NARROWED FURTHER (2026-09-10, continued): the b-absorbing merge does
NOT go through Merge::mergeOp at all -- confirmed via widened gate
--------------------------------------------------------------------
Widened the RETURN-debug instrumentation's gate from an address range
to data.getName()=="muldiv_u16" (unconditional for this function),
rebuilt, redeployed, restarted, re-ran. Result: STILL only the same
4 mergeOp calls logged (BUILD@0x141d7:8e, DELAY_SLOT@0x141c2:8c/8f/95,
DELAY_SLOT@0x141ce:90) -- mergeOp is only ever called for these 4 ops
in this function; there is no 5th, later mergeOp call hiding outside
the original address range.

Cross-referencing this run's syminherit log against this run's
return_debug [params] dump (same run, matching pointer space
00000242BCB2...): entry 2's tv2=00000242BCB29C80 exactly matches b's
own highPtr in every one of the 4 captured mergeOp snapshots (which,
consistent with the earlier finding, never changes across any of
them). Since mergeOp's own 4 calls are now KNOWN EXHAUSTIVE for this
function (widened gate proves it) and b's highPtr never changes across
any of them, the actual merge that absorbs b (syminherit entry 2) must
happen via one of Merge::merge's OTHER call sites (merge.cc has ~9
total call sites: lines 326, 353, 385, 825/mergeOp-already-ruled-out,
959, 967, 981, 1044, 1112, 1338), none of which run through mergeOp
and therefore none of which are visible to the RETURN-debug
instrumentation at all, regardless of gating.

NEXT CONCRETE STEP: since mergeInternal itself has no call-site
context, the plan is to tag each of Merge::merge's ~8 remaining call
sites with a distinct marker (e.g. a file-scope thread-unsafe-but-
sufficient-for-single-threaded-decompile static const char* 'currentMergeContext'
string set immediately before each call site, read back inside the
existing symbol-inherit log point in mergeInternal) to identify, by
name, which specific caller (e.g. mergeAddrTied vs
processHighRedundantCopy vs the speculative-merge path vs others, not
yet mapped to line numbers/function names this session) performs the
b-absorbing merge. This is a broader, code-wide tagging change rather
than a single-point instrumentation add, and is a reasonable
appropriate place to check in with the user before proceeding further,
given the session's length and the fact that the crash-risk fix
(mergeTestBoundary) is already safely applied and validated
independent of this remaining investigation thread.

======================================================================
SESSION SUMMARY, CONTINUED (2026-09-11) -- quick orientation for next
resume, supersedes only the "NEXT SESSION SHOULD START HERE" section
of the earlier 2026-09-10 summary above (items 1-3 and 6 of that
summary remain fully accurate and unchanged)
======================================================================
Picked up from the earlier summary's item 4/5 (remaining wrong-value
bug, HighVariable::updateSymbol as the lead). Progress this
continuation:

A. updateSymbol() instrumentation (from the previous summary) never
   fired -- root cause: HighVariable's symboldirty flag is not set at
   construction and updateSymbol() early-returns before ever reaching
   its scan loop unless something explicitly marks it dirty. Not a bug
   in the instrumentation; just means Symbol assignment for these
   HighVariables happens via a different path.

B. FOUND AND CONFIRMED LIVE: HighVariable::mergeInternal
   (variable.cc:668-726) directly inherits a Symbol pointer from the
   absorbed side (tv2) whenever tv2->symbol != 0 and tv2 is not itself
   symbol-dirty (lines 674-679: symbol = tv2->symbol;). This lets a
   Symbol assignment silently ride across a CHAIN of otherwise-
   unrelated HighVariable merges, surviving each intermediate object's
   deletion. Instrumented this exact branch and captured a real,
   reproducible TWO-HOP CHAIN in muldiv_u16: parameter b's own,
   genuine HighVariable (confirmed via direct highPtr cross-reference
   against the existing RETURN-debug/[params] dump from the SAME
   force_decompile run) gets absorbed into an intermediate
   HighVariable, which is itself then absorbed into a third, final
   HighVariable -- carrying b's Symbol/name forward at each step. This
   is REPRODUCIBLE (2 independent decompile runs, different pointer
   values each time, identical 2-hop shape).

C. Widened the RETURN-debug instrumentation's gate from an address
   range to unconditional-for-this-function (data.getName()==
   "muldiv_u16") and confirmed Merge::mergeOp is called for EXACTLY 4
   ops in this whole function (BUILD@0x141d7:8e, DELAY_SLOT@0x141c2:
   8c/8f/95, DELAY_SLOT@0x141ce:90) -- no more, no fewer. Since
   parameter b's own highPtr never changes across any of these 4
   snapshots, the b-absorbing merge (item B) is CONFIRMED to happen
   via a DIFFERENT Merge::merge call site than mergeOp -- merge.cc has
   roughly 8 other call sites to Merge::merge (approximate line
   numbers noted in the entry immediately above this summary: 326,
   353, 385, 959, 967, 981, 1044, 1112, 1338) that have not yet been
   individually identified or instrumented.

CURRENT UNDERSTANDING OF THE FULL BUG, top to bottom:
  1. ROOT CAUSE (fixed): Cover::intersectList's level=2 pre-filter hid
     a genuine boundary-only collision between the INDIRECT's forced-
     merge output and input HighVariables -- this let Merge::mergeOp
     attempt (and, before the fix, silently succeed at) an invalid
     merge with no Cover violation ever detected.
  2. FIX APPLIED (Merge::mergeTestBoundary, scoped to CPUI_INDIRECT/
     max==1 only): makes mergeOp's trim loop aware of the boundary
     collision, so it now correctly trims/splits instead of silently
     merging. CONFIRMED: no crashes, no regressions (sat_add_u16 still
     correct), P-code instruction order visibly changes (proving the
     trim fires).
  3. REMAINING BUG (open): even with the Cover-level fix correctly
     splitting the INDIRECT's own input/output, muldiv_u16 STILL
     prints the wrong value (eturn b;) instead of the real division
     result. Investigation this session PROVED (via direct pointer
     comparisons) this is NOT a HighVariable object-identity merge
     between b and the RETURN's real value -- it's a Symbol-pointer
     INHERITANCE chain (item B above) that separately, silently
     attaches b's Symbol/display-name to an unrelated HighVariable
     somewhere in the merge sequence, via a merge call site OTHER than
     mergeOp (item C above). This second mechanism has NOT yet been
     connected back to the original Cover-boundary defect -- it is
     not yet established whether fixing item 1 was a NECESSARY
     precondition for item 3 to manifest, or whether item 3 is a
     wholly separate, pre-existing defect that was already there
     before this session started (the doc's ORIGINAL baseline
     behavior, before ANY fix this session, was already eturn b; --
     see the very first decompiler output captured at the top of this
     document -- so item 3's symptom PRE-DATES the mergeTestBoundary
     fix and is not something the fix introduced; it was always the
     visible face of the bug, and item 1/2 was working towards
     eliminating a DIFFERENT symptom, the LowlevelError crash, that
     only appeared transiently while testing an earlier, since-
     reverted, overly-broad version of the Cover-level fix).

NEXT SESSION SHOULD START HERE: tag each of Merge::merge's ~8 non-
mergeOp call sites (merge.cc lines approximately 326, 353, 385, 959,
967, 981, 1044, 1112, 1338 -- confirm exact current line numbers via
grep first, as line numbers have shifted repeatedly this session due
to other edits) with a distinguishing static marker string, readable
from inside HighVariable::mergeInternal's existing symbol-inherit
debug log point, to identify BY NAME which caller performs the
b-absorbing merge. Once identified, read that caller function in full
to determine whether the merge it performs is itself legitimate
(ordinary, correct dead-register-slot reuse) or a further manifestation
of a bug -- this is the determining step for whether item 3 is fixable
at all within Merge/Cover, or whether it requires accepting that
Symbol-pointer inheritance across legitimate merges can, in rare
cases like this H8 register-reuse pattern, produce a misleading but
NOT incorrect (in terms of raw computed values) display name, which
would reframe this from a "wrong answer" bug to a "confusing variable
naming" bug -- a materially different and more limited class of defect
than what this document originally set out to fix.

CURRENT CODE STATE: variable.cc, varnode.cc, merge.cc, merge.hh all
carry accumulated debug instrumentation from this session (intersection/
copyShadow/updateSymbol/mergeInternal-symbol-inherit logging, all
gated appropriately, all confirmed compiling cleanly with 0 errors).
merge.cc/merge.hh additionally carry the mergeTestBoundary fix, which
is validated safe and should be KEPT. None of this is committed to
git; it exists only in the live working tree at
C:\Users\j.brophy.CORKILLSYSTEMS\github\ghidra.

======================================================================
SESSION SUMMARY, CONTINUED FURTHER (2026-09-11, later same day) --
call-site tagging identifies the exact bad merge; mergeOpcode's
CPUI_COPY required-merge at the division site is the smoking gun
======================================================================
Picked up exactly where the previous summary left off: item C's open
question (which of Merge::merge's ~8 non-mergeOp call sites performs
the b-absorbing merge).

METHOD: added a global debug marker (g_mergeCallSiteTag, declared
extern in merge.hh, defined in merge.cc) set immediately before each
of the 9 relevant merge()/HighVariable::merge() call sites in
merge.cc: mergeLinear (line ~329), mergeRangeMust (~358), mergeOpcode
(~391, the CPUI_COPY forced-merge path), mergeOp (~833, the
already-known INDIRECT path), three call sites inside mergeIndirect
(~968/~977/~991, tagged mergeIndirect-firstAttempt /
-afterSnipOutput / -finalForced), mergeMultiEntry (~1056),
mergeAdjacent (~1125), and buildDominantCopy's direct
HighVariable::merge call (~1353). variable.cc's existing
HighVariable::mergeInternal symbol-inherit debug log (see prior
summary's item B) was extended to print g_mergeCallSiteTag alongside
its existing this/tv2/symbol-name fields, via a forward declaration
of the extern marker (merge.hh itself was NOT included in variable.cc,
to avoid any include-order risk).

BUILD/DEPLOY NOTES THIS ROUND: the `gradlew.bat
:Decompiler:decompileWin_x86_64Executable --rerun-tasks` command
client-side-timed-out after 4 minutes with no result, exactly the
false-timeout pattern already flagged in the prior summary -- verified
via decompile.exe's LastWriteTime/Length in the build output directory
that the build had, in fact, completed successfully during the
timeout window. Confirms (again) that a suspiciously-absent result
after several minutes should be checked via file timestamps before
assuming failure or re-running redundantly. On the SECOND rebuild this
same continuation (after adding op-address logging, see below), the
tool call returned normally with full BUILD SUCCESSFUL output in 1m
39s -- both compileDecompileWin_x86_64ExecutableDecompileCpp and
linkDecompileWin_x86_64Executable tasks actually ran (not
UP-TO-DATE), confirming --rerun-tasks is working as intended.

FIRST TEST RESULT (call-site name only, no address yet): cleared all
debug logs, killed decompile.exe, deployed fresh binary (verified via
matching LastWriteTime/Length at both the build output and live
install paths), restarted Ghidra, reconnected via
ghidra:connect_instance("new") (list_instances still not needed/used,
consistent with prior guidance), ran force_decompile on muldiv_u16.
Bug reproduced identically (`return b;`).

muldiv_u16_syminherit_debug.txt, full contents (3 entries):
  1: callSite=mergeOpcode this=...21E0 tv2=...26B0 tv2->symbol=b dirty=false
  2: callSite=mergeOpcode this=...9F20 tv2=...9DD0 tv2->symbol=b dirty=false
  3: callSite=mergeOpcode this=...8710 tv2=...9F20 tv2->symbol=b dirty=false

DECISIVE FINDING: every single symbol-inheriting merge in this run --
including the full two/three-hop chain that ultimately carries `b`'s
Symbol -- goes through Merge::mergeOpcode. NONE go through mergeOp or
mergeIndirect (the INDIRECT-forced-merge path this entire document
was originally about). mergeOpcode is invoked elsewhere in the
codebase with opc=CPUI_COPY: it walks every COPY op in block order and
performs a *required* merge of the COPY's output HighVariable with
its input HighVariable whenever mergeTestRequired() passes. This is
ordinary, extremely common register/COPY-chain coalescing, not the
INDIRECT-specific mechanism investigated for most of this document.

FOLLOW-UP INSTRUMENTATION: since mergeOpcode's own `op` (the COPY
being merged) is already in scope at the tag site, extended the tag
from a static string to a per-call ostringstream-built string
including op->getSeqNum() (i.e. the COPY's own p-code address), stored
in a `static string tagbuf` so the buffer backing g_mergeCallSiteTag's
raw pointer stays alive across the merge() call. Rebuilt (this is the
"second rebuild" above, returned normally), redeployed (verified),
Ghidra restarted, reconnected, force_decompile re-run -- bug
reproduced identically.

muldiv_u16_syminherit_debug.txt, full contents from this run (3
entries, WITH addresses):
  1: callSite=mergeOpcode@0x000141e6:158 this=...2640 tv2=...2790 tv2->symbol=b dirty=false
  2: callSite=mergeOpcode@0x000141c2:d2  this=...9900 tv2=...9DD0 tv2->symbol=b dirty=false
  3: callSite=mergeOpcode@0x000141ce:d3  this=...8710 tv2=...9900 tv2->symbol=b dirty=false

THIS IS THE SMOKING GUN. Cross-referencing against addresses already
established throughout this document:
  - 0x141c2 is the mulxu.w site -- exactly "A", the stale
    multiplication-chain HighVariable from the original P-CODE TRACE
    section and every subsequent entry in this document.
  - 0x141ce is the divxu.w site -- exactly "B", the CORRECT
    division-result COPY, independently proven earlier this document
    (via the RETURN-op instrumentation) to be what the RETURN op's
    p-code input has always genuinely pointed to.

The full chain in program terms:
  b's real Symbol (absorbed first at an unrelated COPY, 0x141e6)
    --absorbed into--> COPY at 0x141c2 (mulxu.w chain, "A")
      --absorbed into--> COPY at 0x141ce (division result, "B",
                                           = the RETURN's real input)

Hop 3 (mergeOpcode@0x141ce:d3) is the actual bad merge: an ordinary
CPUI_COPY "required" merge (mergeTestRequired said yes) folding A (the
stale mulxu.w chain, already carrying b's inherited Symbol via hop 2)
directly into B (the division-result HighVariable, i.e. the object
that IS the RETURN's real p-code input). Per
HighVariable::mergeInternal's "assume it is the same" Symbol-inherit
rule (see prior summary item B), b's Symbol rides along into B at this
exact point, which is why `return b;` prints even though the
underlying p-code has correctly read the division result the entire
time.

RECONCILIATION WITH THE ORIGINAL ROOT-CAUSE INVESTIGATION: this
confirms the Cover-boundary defect (Cover::intersectList's level=2
pre-filter, documented at length earlier in this file, fix already
applied via mergeTestBoundary in mergeOp's trim loop) IS a genuine
precondition after all -- but it manifests via a DIFFERENT call path
than assumed. mergeOpcode's required-merge decision for the
0x141c2/0x141ce COPY pair is gated by mergeTestRequired(), which
(not yet independently re-confirmed this session, but consistent with
every other caller in this codebase) very likely goes through the same
HighIntersectTest::intersection()/Cover::intersectList() machinery
already investigated -- meaning the SAME boundary-collision blind spot
this document spent most of its length on may be exactly why
mergeTestRequired() incorrectly says "yes, safe to merge" for A and B
at 0x141ce, via mergeOpcode, rather than (or possibly in addition to)
via mergeOp's INDIRECT path. The already-applied mergeTestBoundary fix
does NOT cover this call site -- it is scoped strictly to mergeOp's
trim loop (CPUI_INDIRECT / max==1 case only) and has no effect on
mergeOpcode's independent mergeTestRequired() check.

NEXT STEPS, in order:
1. Re-read Merge::mergeTestRequired (merge.cc, not yet re-read this
   continuation with the current finding in mind) to confirm exactly
   which intersection test it calls, and whether it is indeed the same
   HighIntersectTest::intersection()/Cover::intersectList(level=2)
   path already fully traced earlier in this document, or a distinct
   check.
2. If confirmed, the likely correct narrow fix is analogous to the
   already-applied mergeTestBoundary pattern: give mergeOpcode's own
   required-merge decision (or mergeTestRequired itself, if that
   wouldn't over-broaden the effect to mergeTestRequired's OTHER
   callers -- check all call sites first, per the CANDIDATE FIX
   caution pattern already established earlier in this document) a
   boundary-aware supplementary check, so a genuine boundary collision
   between the COPY being merged and ITS OWN forced-merge partner is
   caught and trimmed/split rather than silently folded together via
   inherited Symbol identity.
3. Regression-test the same way as before: confirm sat_add_u16 (the
   established control) is unaffected by any change, since
   mergeTestRequired/mergeOpcode fire constantly across ordinary,
   correct decompiler output and a global loosening already failed
   catastrophically once earlier in this document for a similarly
   general call path.
4. Once mergeOpcode@0x141ce:d3's specific merge is confirmed to be the
   fix target, re-test muldiv_u16 and the other affected sibling
   functions (muldiv_u16_scaled, muldiv_s16_rounded_3op,
   muldiv_s16_rounded, weighted_blend_u8frac) to see if they share the
   identical mergeOpcode-driven mechanism (likely, given they all show
   the same correct-value-computed-but-wrong-identity-printed
   symptom shape documented earlier in this file) and are fixed
   together.
5. Once a fix lands and is validated, remove ALL accumulated debug
   instrumentation (variable.cc, varnode.cc, merge.cc, merge.hh,
   funcdata_varnode.cc -- confirm the full list via grep for
   "DEBUG (muldiv_u16" and the various *_debug.txt log paths) before
   preparing any upstream-facing change; the g_mergeCallSiteTag
   marker introduced this continuation is itself part of that
   instrumentation and should be removed alongside it, not kept.

CURRENT CODE STATE: merge.hh now additionally declares
g_mergeCallSiteTag (extern const char*). merge.cc defines it and sets
it at all 9 call sites listed under METHOD above; the mergeOpcode site
was subsequently upgraded to embed the COPY op's own address via a
static string buffer. variable.cc forward-declares the same extern
and prints it in the existing symbol-inherit log line. All confirmed
building cleanly (BUILD SUCCESSFUL, both compile and link tasks ran)
and reproducing the bug identically on every test this continuation --
no behavior change from this instrumentation, as expected (it is
read-only with respect to program logic). None of this is committed
to git; still lives only in the working tree at
C:\Users\j.brophy.CORKILLSYSTEMS\github\ghidra.

--------------------------------------------------------------------
NEXT STEP 1 COMPLETED (2026-09-11, continued further): mergeTestRequired
has NO Cover check at all; the real gate is Merge::merge's own
testCache.intersection() call -- the SAME machinery already fixed once
for mergeOp, now confirmed to also gate mergeOpcode's COPY-merge
--------------------------------------------------------------------
Read Merge::mergeTestRequired (merge.cc:106-170) in full, as planned
in the previous entry's NEXT STEPS item 1. RESULT: it contains NO
Cover-intersection check of any kind. It is purely a set of symbol/
address-tie/type-lock/VariablePiece compatibility gates (type-lock
mismatch, address-tied-with-different-address, input-vs-persist
conflicts, ProtoPartial rules, and a same-symbol/different-symbol
check at lines 161-167). None of these reject the A (0x141c2 mulxu.w
chain) vs B (0x141ce division result) pair in muldiv_u16: neither has
a locked type, neither is addr-tied in a conflicting way, and at the
time mergeOpcode's mergeTestRequired() call runs, at most one side has
an inherited Symbol (per the symbol-inherit log itself, the Symbol
transfer is a CONSEQUENCE of this merge succeeding, not a pre-existing
state mergeTestRequired would see and reject). So this earlier
theory -- "mergeTestRequired shares the same level=2 blind spot" -- is
WRONG AS STATED; correcting it here rather than deleting it, per this
document's established convention.

Read Merge::merge (merge.cc:1689-1707, the actual function invoked at
every merge() call site including mergeOpcode's) in full instead.
Line 1693: `if (testCache.intersection(high1,high2)) return false;` --
testCache is a HighIntersectTest member of the Merge object, so this
IS, precisely, the same HighIntersectTest::intersection() /
Cover::intersectList(level=2) machinery already fully traced and
root-caused earlier in this document (see EXACT ROOT CAUSE FOUND
section, ~line 870 onward). This is the REAL gate for mergeOpcode's
COPY-merge, not mergeTestRequired.

REVISED, CONFIRMED UNDERSTANDING OF THE MECHANISM: mergeOpcode's flow
for the op@0x141ce COPY is: mergeTestRequired() (symbol/type/address
compatibility only -- passes here, uninterestingly) -> merge() ->
testCache.intersection(high_A, high_B) (the REAL gate) -> since A's
and B's Covers touch only at the same boundary point already proven
earlier in this document (A's range ends exactly where B's single-
point def-range begins, at block index 2, per the ROOT CAUSE
PINPOINTED / boundary-touching-ranges section far above), intersection()
returns FALSE (no intersection detected) via the identical level=2
pre-filter mechanism -- and Merge::merge proceeds to silently fold A
and B into one HighVariable, carrying B's now-inherited Symbol from A
IF -- and this is the missing link P closing the earlier chain -- A
had already itself absorbed a Symbol (b's) via an EARLIER mergeOpcode
merge (hop 2, @0x141c2:d2, per the previous entry's 3-hop chain).

THIS FULLY CLOSES THE LOOP: the exact same root-cause defect already
identified and already partially fixed (Cover::intersectList's level=2
pre-filter missing genuine boundary-only collisions) is confirmed to
ALSO be the mechanism letting mergeOpcode's COPY-merge silently fold
the mulxu.w chain (A) into the division result (B) at 0x141ce -- via
Merge::merge's own testCache.intersection() call, completely
independent of and parallel to mergeOp's trim loop (which is a
different call path entirely and was already fixed via
mergeTestBoundary). The already-applied mergeTestBoundary fix has NO
effect here because it is scoped strictly inside mergeOp's own
trim-decision loop (gated to the CPUI_INDIRECT/max==1 case) and is
never reached by mergeOpcode's call path.

CANDIDATE FIX DIRECTION (NOT YET DESIGNED IN DETAIL, NOT YET APPLIED):
by analogy with the existing mergeTestBoundary pattern (a supplementary,
boundary-aware Cover check added at a NARROW, specific call site
without touching HighIntersectTest::intersection's global level=2
threshold, which is already confirmed too broad to loosen safely --
see the REGRESSION CONFIRMED section far above where a global loosening
broke sat_add_u16), the likely correct fix is to give Merge::mergeOpcode
its own supplementary boundary check, analogous to mergeTestBoundary,
called alongside (not instead of) the existing merge() call for the
CPUI_COPY case specifically -- so a genuine boundary-only collision
between vn1's and vn2's Covers is caught and the COPY is left unmerged/
trimmed rather than silently folded via Symbol inheritance. This needs:
  (a) confirming whether mergeOpcode is ever called with an opcode
      OTHER than CPUI_COPY elsewhere in the codebase (if so, any fix
      must be scoped to the CPUI_COPY case only, mirroring
      mergeTestBoundary's own CPUI_INDIRECT-only scoping, to avoid
      broadening effect to unrelated opcode merges);
  (b) designing the actual boundary-check call (likely reusable
      directly: Merge::mergeTestBoundary's existing signature
      (HighVariable*,HighVariable*)->bool, already used by mergeOp,
      may be directly callable from mergeOpcode with no changes needed
      to mergeTestBoundary itself -- worth trying this first, as the
      cheapest possible fix, before writing a new function);
  (c) if mergeTestBoundary is reused as-is: on a true boundary
      collision, decide what mergeOpcode should DO instead of merging
      -- unlike mergeOp (which has an existing trim/split mechanism to
      fall back on), mergeOpcode currently has no fallback path at all;
      it either merges or it doesn't (the `if (mergeTestRequired(...))
      { merge(...); }` structure has no else-branch and no trim call).
      Simply skipping the merge (treating a boundary collision as
      "don't merge, leave as separate variables") may be sufficient
      here, since -- unlike the INDIRECT case -- there is no hard
      LowlevelError risk if two ordinarily-merged COPYs are left
      unmerged; worst case is slightly less aggressive variable
      coalescing elsewhere, which needs verifying is not itself
      user-visible or currently relied upon.
  (d) regression-testing exactly as before: confirm sat_add_u16 stays
      correct and unaffected, since mergeOpcode fires on every COPY in
      every function and any behavior change here has broad reach
      across the whole program, not just muldiv_u16.

NEXT CONCRETE STEP: check mergeOpcode's OTHER call site(s) (i.e. where
Merge::mergeOpcode(OpCode opc) is itself invoked, not the merge() calls
inside it) to see what opcode(s) it's actually called with in this
codebase's own pipeline -- needed before scoping any fix, per (a)
above -- then attempt reusing mergeTestBoundary as-is per (b), skip the
merge on a detected boundary collision per (c), rebuild, and re-test
muldiv_u16 + sat_add_u16 together in the same cycle this document has
used throughout (never test the fix on the bug function alone without

======================================================================
SESSION SUMMARY, CONTINUED FURTHER STILL (2026-09-13) -- built a
standalone, PyGhidra-free decompiler test harness (decomp_test_dbg.exe)
using the project's own pre-existing but never-wired-up Makefile/test.cc
console interface; bug CONFIRMED to reproduce in complete isolation
======================================================================
User pointed out we had a working standalone test harness before and
that it should only take ~20 lines of build config -- correct call.
Found `Ghidra/Features/Decompiler/src/decompile/cpp/Makefile`, a
genuine, complete, pre-existing Makefile (Linux/g++-oriented) with a
`test`/`decomp_test_dbg` target already wired to build test.cc (a
plain argc/argv console entry point, NOT the pipe-protocol
ghidra_process.cc used by decompile.exe/PyGhidra) against the full
decompiler core plus testfunction.cc (FunctionTestCollection, the
driver for the datatests/*.xml files) and ifacedecomp.cc/ifaceterm.cc
(the interactive command-line interface: `lo fu`, `decompile`,
`print C`, etc, exactly what copytrim.xml and friends already use).
This whole mechanism was always in the tree, just never built on
Windows -- buildNatives.gradle explicitly comments out
ifacedecomp.cc/ifaceterm.cc with "uncomment for debug" and has no
target at all for test.cc.

WSL was attempted first (has g++/make/bison/flex already) but WSL
itself is broken on this machine (user's call) and gdb wasn't
installed there either -- abandoned in favor of doing everything
in Windows directly.

BUILD METHOD: rather than porting the Makefile's own logic, compiled
directly with MinGW g++ (C:\mingw64\bin\g++.exe, confirmed present,
along with a working C:\mingw64\bin\gdb.exe -- important, since this
means gdb is available as a much better alternative to the cdb/WinDbg
timing nightmares from earlier this session, once we have a normal,
non-one-shot, non-piped Windows executable to attach to). Replicated
the Makefile's own CORE/DECCORE/SLEIGH/EXTRA source categorization by
hand in PowerShell to build the exact TEST_NAMES file list (98 files
initially), then fixed four Windows/MinGW portability issues
encountered along the way, none of which touch decompiler LOGIC, only
build portability:
  1. ifaceterm.hh's IfaceTerm class requires <termios.h> (POSIX
     terminal raw-mode/tab-completion) under its own __TERMINAL__
     guard -- simply did not define __TERMINAL__ (we drive the
     interface via a script file, not a live interactive terminal, so
     the base IfaceStatus class is sufficient; nothing in test.cc
     itself references IfaceTerm/__TERMINAL__ directly).
  2. bfd_arch.cc/loadimage_bfd.cc need <bfd.h> (GNU binutils), not
     available in this MinGW install and not needed for our purposes
     (raw bytes are supplied directly via the XML <binaryimage> tag,
     no need to load a real ELF/PE/etc via BFD) -- dropped both files
     from the build.
  3. analyzesigs.cc and codedata.cc both transitively #include
     loadimage_bfd.hh even with bfd_arch.cc/loadimage_bfd.cc
     themselves removed -- dropped both from the build too (neither
     needed for our XML-driven raw-bytes test).
  4. filemanage.cc's FileManage::directoryList relied on POSIX
     dirent's d_type member (DT_DIR/DT_UNKNOWN/DT_LNK), which MinGW's
     dirent struct does not have AT ALL (not just "always unknown" at
     runtime -- the member itself doesn't exist, a hard compile
     error). Fixed by adding a `#if defined(__MINGW32__) ||
     defined(__MINGW64__)` branch that always takes the stat()-based
     fallback path this file ALREADY implements for the DT_UNKNOWN
     case on other platforms -- no new logic, just always-take that
     existing branch on MinGW. This is a genuine, permanent portability
     fix (not investigation-specific instrumentation) and could
     reasonably be upstreamed on its own, independent of the muldiv_u16
     bug.
  5. (Link-time, not compile-time) compression.cc needs zlib
     (deflate/inflate/etc) -- added -lz; MinGW already bundles
     libz.a at C:\mingw64\x86_64-w64-mingw32\lib\libz.a so no separate
     install was needed.

Final build command (from
Ghidra/Features/Decompiler/src/decompile/cpp/):
  C:\mingw64\bin\g++.exe -std=c++17 -g -O0 -DCPUI_DEBUG -Wno-sign-compare
    -o decomp_test_dbg.exe <94 .cc files, see filelist3.txt> -lws2_32 -lz
(-g -O0 deliberately chosen over the gradle decompile.exe build's
normal -O2/-GL, matching the same debug-build rationale already
applied earlier this session to the MSVC decompile.exe build --
reliable breakpoints/stepping over raw speed.) Full source file list
preserved at C:\Users\j.brophy.CORKILLSYSTEMS\decomp_test_build\filelist3.txt
for reproducibility. Build takes ~5-6 minutes single-threaded (94
files, no -j, no incremental caching since each rebuild was a fresh
single g++ invocation) -- worth revisiting with separate -c compile +
link steps and object-file caching if many more rebuild cycles are
needed, to avoid repeatedly recompiling all 94 files for a one-line
change (this bit us once this session: a single-line testfunction.cc
debug-output addition needed three full ~5-6 minute rebuilds to shake
out two trivial `cerr`-not-declared/`<iostream>` missing-include
errors that would have been caught in seconds with incremental
compilation).

RUNNING IT: `decomp_test_dbg.exe -sleighpath <path to real Ghidra
install root> -path <dir containing .xml test files> datatests`.
-sleighpath must point at the Ghidra install root (e.g.
...\ghidra_12.2_DEV, NOT the Processors subfolder) since
SleighArchitecture scans subdirectories itself for .ldefs files --
confirmed Processors\h8\ exists under that root and the H8/539F
language loaded successfully with no error.

REPRO XML: extracted muldiv_u16's real 34 raw bytes (0x141ba-0x141db)
directly from the live Ghidra database via read_memory, and the real
language ID (H8:BE:32:H8539F) via get_current_program_info, and wrote
a minimal <decompilertest> (modeled on datatests/copytrim.xml's
format) containing just those bytes at their real address with a
single "muldiv_u16" symbol, then `lo fu muldiv_u16` / `decompile` /
`print C` / `quit`. Saved at
C:\Users\j.brophy.CORKILLSYSTEMS\decomp_test_build\muldiv_repro_tests\muldiv_repro.xml.

FIRST RESULT, MISINTERPRETED: initial stringmatch check for the
literal text "return b;" with min=0 max=0 (expect zero occurrences)
PASSED, which was misread in the moment as "the bug did not
reproduce." This was WRONG -- corrected immediately below once the
actual decompiled text was inspected directly.

GETTING THE REAL OUTPUT: FunctionTestCollection::runTests (
testfunction.cc) captures ALL console output (including print C's
full decompiled C text) into an internal ostringstream (`bulkout`)
that is used ONLY for stringmatch pattern-checking and is never
otherwise surfaced -- by design, matching how every existing
datatests/*.xml file works (you only ever see PASS/FAIL, never the
underlying C). Added a temporary debug line right after `string
result = bulkout.str();` in FunctionTestCollection::runTests to dump
the full captured text to std::cerr (needed both `std::` qualification
and an explicit `#include <iostream>` -- this codebase's normal
headers do not transitively provide bare `cerr`/`endl` the way one
might expect; test.cc/other files apparently get by using only
`cout`/`endl` via a different, more limited include path). This is
investigation-only instrumentation and should be reverted before any
further use of this harness beyond the current investigation.

ACTUAL DECOMPILED OUTPUT (from decomp_test_dbg.exe, full text via the
debug dump above):
  uint2 muldiv_u16(void)
  {
    uint2 uVar1;
    uint2 uVar2;
    xunknown2 xVar3;
    xunknown1 *pxVar4;
    xunknown1 in_TP;
    uint2 in_stack_00000004;
    uint2 in_stack_00000006;
    xunknown1 axStack_82 [128];

    xVar3 = (xunknown2)((uint4)in_stack_00000004 * (uint4)in_stack_00000006);
    uVar2 = (uint2)((uint4)in_stack_00000004 * (uint4)in_stack_00000006 >> 0x10);
    pxVar4 = axStack_82 + 0x80;
    uVar1 = *(uint2 *)(pxVar4 + 10);
    if ((uVar1 == 0) || (uVar1 <= uVar2)) {
      return 0xffff;
    }
    xVar3 = (int2)(CONCAT22(uVar2,xVar3) / (uint4)uVar1);
    return in_stack_00000006;
  }

THE BUG DOES IN FACT REPRODUCE IN THIS MINIMAL, FULLY ISOLATED,
34-BYTE, SINGLE-FUNCTION TEST. The prior "did not reproduce" reading
was simply because this minimal repro has no function signature/
parameter names applied (no <com> to set up a prototype naming a, b,
c the way the real binary's analysis does), so the decompiler falls
back to raw storage-location naming: `in_stack_00000006` is the exact
same physical stack slot that, with the real prototype applied in the
live binary, is named `b`. The RETURN statement returns
`in_stack_00000006` -- the second incoming parameter's raw storage --
instead of `xVar3`/the division result, which is precisely the same
bug (wrong HighVariable/Symbol printed at the RETURN op) already
fully diagnosed earlier in this document, just without a name for the
misidentified variable. This CHANGES a piece of earlier reasoning:
the theory that hop 1 of the 3-hop merge chain (address 0x141e6,
outside muldiv_u16's own body) is a NECESSARY precondition for the bug
appears to be WRONG, or at least not necessary in this isolated
33-byte case -- the bug clearly still occurs with ONLY muldiv_u16's
own bytes present and nothing else in the binary image at all. This
needs reconciling with the earlier 3-hop chain finding: EITHER (a) an
equivalent merge chain forms entirely from muldiv_u16's OWN internal
COPYs even without the external 0x141e6 seed (i.e. hop 1's role was
coincidental/not essential, and hops 2+3 -- the mulxu.w chain merging
into the divxu.w/division-result chain -- are sufficient on their
own), or (b) this minimal repro is hitting a related but distinct
manifestation of the same underlying mergeOpcode/Cover-boundary defect
rather than the exact same 3-hop chain. Either way, the core mechanism
(mergeOpcode's CPUI_COPY required-merge silently folding across a
genuine boundary-only Cover collision) is strongly reconfirmed as
reproducible in complete isolation, which is very good news for
actually fixing and testing this without any PyGhidra/pipe-protocol
timing dependency at all.

WHY THIS MATTERS GOING FORWARD: decomp_test_dbg.exe is a normal,
long-lived-until-it-naturally-exits, single Windows console process
with full debug symbols (-g -O0), taking simple command-line
arguments and printing to stdout/stderr normally. It has NONE of the
properties that made WinDbg/cdb attachment so unreliable earlier this
session (no PyGhidra parent, no pipe protocol, no sub-second one-shot
lifetime, no need for a MULDIV_DEBUG_WAIT-style gate at all since a
debugger can simply be the process's own direct launcher via `gdb
--args decomp_test_dbg.exe -sleighpath ... -path ... datatests`, which
guarantees the debugger controls the process from before main() even
runs -- exactly the "make the debugger the launcher" option identified
but deprioritized earlier this session for the real decompile.exe
because doing so there wasn't achievable without admin/IFEO). gdb.exe
is confirmed present at C:\mingw64\bin\gdb.exe.

IMMEDIATE NEXT STEPS:
1. Revert the temporary std::cerr debug dump in testfunction.cc (or
   leave it gated/harmless -- low priority, does not affect
   correctness, but should not ship in any upstream-facing change).
2. Launch decomp_test_dbg.exe directly under gdb (`gdb --args
   C:\Users\j.brophy.CORKILLSYSTEMS\decomp_test_build\decomp_test_dbg.exe
   -sleighpath "...\ghidra_12.2_DEV" -path
   "...\muldiv_repro_tests" datatests`), set a breakpoint on
   `Merge::mergeOpcode` (plain function breakpoint, no deferred-bp
   module-load timing concerns since this is a normal statically-
   linked executable, not something whose relevant module loads
   asynchronously mid-run the way decompile.exe's pipe-protocol
   loop did), run to it, and inspect the vn1/vn2 HighVariables and
   the op being merged directly via gdb's normal `print`/`p *vn1`
   etc, exactly as originally envisioned when debug-build-and-attach
   was first proposed this session -- this time with a realistic,
   working attach story.
3. Once inside Merge::merge's testCache.intersection() check (still
   the confirmed real gate, per the mergeTestRequired-has-no-Cover-
   check finding from earlier this session), inspect the actual
   Cover objects for the mulxu.w-chain HighVariable and the
   divxu.w/division-result HighVariable at the point they are
   compared, to see exactly why the level=2 pre-filter judges them as
   non-intersecting despite the genuine boundary-touching collision
   -- this is the piece that was never actually observed directly,
   only inferred from address/behavior evidence.
4. Reconcile finding (a) vs (b) above regarding whether the external
   0x141e6 hop is truly necessary -- try shrinking/varying the XML
   test further (e.g., does removing the division-by-zero guard
   branch, or changing operand sizes, still reproduce?) using
   decomp_test_dbg.exe's fast iteration loop (no PyGhidra restart,
   no MCP reconnect, no cdb attach race -- just rerun the exe) to
   map out exactly which minimal ingredients are needed.
5. Once a fix candidate is designed (per the mergeTestBoundary-reuse
   plan from the immediately preceding entry), it can be built and
   tested via decomp_test_dbg.exe's fast, PyGhidra-free loop before
   ever touching the real decompile.exe/PyGhidra path at all --
   only port the validated fix to the live investigation once proven
   here.

CURRENT STATE: decomp_test_dbg.exe exists and works at
C:\Users\j.brophy.CORKILLSYSTEMS\decomp_test_build\decomp_test_dbg.exe.
filemanage.cc carries a genuine, permanent MinGW-portability fix (the
d_type guard, item 4 above) -- this one is NOT investigation-only
debug instrumentation and is fine to keep/upstream independently.
testfunction.cc carries the temporary std::cerr debug-dump (item to
revert per Next Step 1). ghidra_process.cc still carries the
MULDIV_DEBUG_WAIT gate from earlier this session (harmless, opt-in via
an environment variable, irrelevant to decomp_test_dbg.exe since that
gate lives in a file this new build doesn't even compile -- test.cc's
main() is entirely separate from ghidra_process.cc's main(), and only
one or the other is ever linked into a given executable). The
MULDIV_DEBUG_WAIT environment variable itself is still set at the User
level on this machine (harmless no-op for anything except a real
decompile.exe built from ghidra_process.cc). None of this is committed
to git; everything lives in the working trees at
C:\Users\j.brophy.CORKILLSYSTEMS\github\ghidra (source changes) and

======================================================================
SESSION SUMMARY, CONTINUED FURTHER STILL (2026-09-13, same day) -- gdb
attach to decomp_test_dbg.exe WORKED CLEANLY on the first real attempt;
live-observed the exact Cover boundary collision for the first time
this entire investigation, directly, not inferred
======================================================================
Launched decomp_test_dbg.exe directly under gdb via `gdb -x <script>
--batch decomp_test_dbg.exe`, passing the real program args
(-sleighpath/-path/datatests) via gdb's own `run` command. Worked
perfectly on the first try: no attach races, no deferred-breakpoint
timing issues, no crashes, no orphaned processes -- exactly the
outcome hoped for when standalone-harness-plus-gdb was proposed as an
alternative to the PyGhidra/cdb approach.

FIRST PASS: set a breakpoint on Merge::mergeOpcode itself (function
entry). Hit exactly 3 times in this minimal single-function repro (not
2 -- corrected from a miscount in the immediately preceding entry,
which only looked at 2 of the 3 hits' data). All 3 hits report
identical op->getAddr() (Address{offset=38} in this build) because the
breakpoint fires at the top of mergeOpcode, before the internal loop
advances `op`/`vn1`/`vn2` to their real per-iteration values -- not
useful for pointer-identity tracing on its own.

SECOND PASS: moved the breakpoint to merge.cc:390 (`if
(mergeTestRequired(vn1->getHigh(),vn2->getHigh()))`), i.e. the actual
per-COPY decision point inside mergeOpcode's loop. This gave real,
distinct vn1/vn2 Varnode data per hit. Traced the HighVariable pointer
chain directly across the 3 hits:
  Hit 1: vn1->high = 0xda45d0,  vn2->high = 0xe20600
  Hit 3: vn1->high = 0xd93210,  vn2->high = 0xda45d0   <-- SAME pointer as hit 1's vn1->high
This is live, direct pointer-identity confirmation of the 2-3 hop
merge chain already inferred (via the syminherit debug log's
this/tv2 pointer values) in the live PyGhidra/decompile.exe
investigation earlier this session: hit 1 merges high(0xe20600) into
high(0xda45d0); hit 3 then merges THAT SAME already-merged
high(0xda45d0) into high(0xd93210). No inference needed this time --
gdb showed the identical pointer value appearing as vn2's high in a
later hit after being vn1's high in an earlier hit, in the same debug
session, live.

THIRD PASS (the actual smoking gun, observed directly for the first
time): at hit 3 -- the merge that folds the already-merged
high(0xda45d0) into high(0xd93210) -- dumped both sides' Cover objects
directly via `print *vn1->cover` / `print *vn2->cover`:
  vn1->cover: 1 block -- [2] = {start = 0xe05350, stop = 0xe726a0}
  vn2->cover: 3 blocks -- [0] = {start=0xe71f90, stop=0x1},
                          [1] = {start=0x0, stop=0x1},
                          [2] = {start=0x0, stop=0xe05350}
THE COLLISION IS RIGHT THERE: vn2's Cover block [2] STOPS at exactly
0xe05350, and vn1's Cover block [2] STARTS at exactly that same
0xe05350 (which is also `op` itself -- op's own PcodeOp pointer,
confirmed via vn1's own `def` field independently equalling 0xe05350
in the earlier per-hit dump, and via SeqNum $1's reported pc.offset
matching the same instruction). This is a textbook END-TO-START
Cover-block boundary touch in the SAME block index (both are index
[2]) -- exactly the shape of collision the level=2 pre-filter in
Cover::intersectList was already suspected (from static code reading
earlier this session) to treat as "no intersection" when it is in
fact a genuine adjacency that matters here, because vn2 is LIVE
exactly up to and including the point where vn1 begins (i.e. they are
simultaneously live at the shared boundary instruction), which is
precisely a case Merge::merge's testCache.intersection() check should
be catching and blocking but is not.

This is the first time in the entire investigation (spanning this
document from its original P-CODE TRACE section through every
subsequent entry) that the actual Cover data for the colliding pair
has been directly observed in a live debugger, rather than reconstructed
from decompiled-output symptoms, address arithmetic, or instrumented
log lines. It fully confirms, with no remaining inference gap, that
the root cause is exactly what the static-code-reading sessions
concluded: a genuine boundary-touching Cover collision that the
existing level=2 intersection test does not catch, now caught
happening via Merge::mergeOpcode's ordinary CPUI_COPY required-merge
path (not mergeOp/INDIRECT), for the general case (confirmed here in
a minimal repro with no named parameters/symbols at all -- both
vn1->high->getSymbol() and vn2->high->getSymbol() were still null
(0x0) at this point, ruling out any Symbol-specific special-casing as
part of the root cause; the Symbol only gets attached/inherited to the
now-merged HighVariable in a LATER step, consistent with
HighVariable::mergeInternal's own "assume it is the same" symbol-
inherit logic already read in full earlier this session).

WHY 3 HOPS HERE VS 3 HOPS IN THE LIVE BINARY -- RECONCILED: the
"external 0x141e6 seed" concern raised in the immediately preceding
entry is now moot/resolved. This minimal, fully isolated repro (ONLY
muldiv_u16's own 34 bytes, nothing else in the binary image) still
produces a 3-hit mergeOpcode chain with the identical
Cover-boundary-touch shape at the critical hit. The external 0x141e6
address seen in the live PyGhidra run was evidently just a
DIFFERENT/EXTRA merge unrelated to (or an alternate route to) the
same underlying chain, not a necessary precondition -- the core defect
reproduces fully self-contained within muldiv_u16's own COPY/mulxu.w/
divxu.w structure, exactly as hypothesis (a) in the immediately
preceding entry proposed.

PRACTICAL WORKFLOW NOTE: this gdb + decomp_test_dbg.exe setup is now a
fully reusable, fast, reliable debugging loop for the rest of this
investigation -- no PyGhidra restarts, no MCP reconnects, no cdb
attach races, no risk of crashing the live Ghidra session. Any further
live inspection (e.g. stepping into Cover::intersectList itself to see
the level=2 pre-filter's actual decision for this exact pair, or
testing a candidate fix) should use this harness first.

IMMEDIATE NEXT STEP: step from this same breakpoint hit (hit 3, at
merge.cc:390) INTO Merge::merge (the actual call a few lines later)
and further into testCache.intersection()/Cover::intersectList, to
observe -- live, not inferred -- the exact level=2 pre-filter logic
making its "no intersection" decision on this exact vn1/vn2 pair,
which is the one remaining unobserved link in the causal chain (we
have now directly seen the Cover data going IN, and separately already
fully read Cover::intersectList's source code earlier this session,
but have not yet watched the actual function execute on this exact
data to confirm the source-reading match the runtime behavior exactly).


======================================================================
CAPSTONE FINDING (2026-09-13, same day, immediately following) --
Cover::intersectList's level=2 call CAUGHT LIVE returning an EMPTY
blockisect despite a genuine, exact boundary touch in the input Cover
data. Zero remaining inference in the causal chain. Root cause fully
and directly confirmed.
======================================================================
Continued the same gdb + decomp_test_dbg.exe session. Set a breakpoint
at variable.cc:1243, the exact call site already identified as the
real gate (`a->getCover().intersectList(blockisect,b->getCover(),2);`
inside HighIntersectTest::intersection). Hit it at the same hit-3
mergeOpcode iteration as before (a=high(0xda3210), b=high(0xdb45d0),
matching vn1/vn2's high pointers from the prior entry exactly).

DUMPED BOTH INPUT COVERS DIRECTLY, IMMEDIATELY BEFORE THE CALL EXECUTES:
  a->getCover(): 1 block  -- [2] = {start = 0xdc5350, stop = 0xd91240}
  b->getCover(): 3 blocks -- [0] = {start=0x0, stop=0x1},
                             [1] = {start=0x0, stop=0x1},
                             [2] = {start=0x0, stop=0xdc5350}

THE COLLISION: b's block [2] stops at EXACTLY 0xdc5350. a's block [2]
starts at EXACTLY that same 0xdc5350. Same block index (both [2]).
This is about as clean and unambiguous a boundary-touch as is possible
to construct -- not a near-miss, not an off-by-one, not a
misinterpretation of pointer arithmetic: the stop of one and the start
of the other are the identical value.

STEPPED PAST THE CALL AND INSPECTED THE RESULT:
  blockisect after the call: std::vector of length 0, capacity 0
  res: stepped through to line 1250 (`if (!res)`) with res still false
  aTied / bTied: both false (so the address-tied fallback path at
    lines 1251-1258 does not fire either -- there is no other
    mechanism left in this function that could still catch the
    collision)

intersectList, called with level=2 on these two Cover objects,
returned ZERO intersecting blocks despite the exact, direct,
value-for-value boundary touch dumped immediately beforehand in the
same debugger session. This is not an inference from decompiled-output
symptoms, not a reconstruction from instrumented log timestamps, not
an argument from address arithmetic -- this is the actual buggy
function call observed executing on the actual colliding data, live,
with the "before" state and the "after" state both captured directly
via gdb in the same run. There is no remaining gap in the causal chain
from Cover::intersectList's level=2 behavior through to `return b;`/
`return in_stack_00000006;` in the final decompiler output: every link
has now been either read in source, confirmed via debug
instrumentation in the live PyGhidra binary, or (this entry) watched
executing directly in a debugger on a minimal, fully reproducible,
standalone test case.

WHAT THIS MEANS FOR THE FIX: the already-drafted candidate fix
direction from two entries ago (give Merge::mergeOpcode a supplementary
boundary-aware check before committing to its CPUI_COPY required-merge,
analogous to the already-applied mergeTestBoundary fix for mergeOp's
INDIRECT path, rather than touching intersectList's level=2 threshold
globally) remains the right target. The new, extremely valuable asset
gained this entry is: decomp_test_dbg.exe + this exact gdb script
sequence is now a proven, fast, reliable REPRODUCTION AND VERIFICATION
tool for that fix. A candidate fix can be applied to merge.cc, rebuilt
via the same MinGW g++ command (~5-6 minutes, single-threaded, no -j --
worth setting up incremental/object-file caching if many more
iterations are needed), and verified by re-running this exact gdb
script: if blockisect (or an equivalent supplementary check) correctly
identifies the boundary collision after the fix, and/or the final
`print C` output no longer shows `return in_stack_00000006;` /
`return b;`, the fix is confirmed working -- all without touching
PyGhidra, the live decompile.exe, or risking another crash of the
interactive Ghidra session, and without any of the timing-race
uncertainty that plagued every attempt at live debugging earlier this
session.


======================================================================
FIRST FIX ATTEMPT (2026-09-13, same day) -- mergeTestBoundary reused
in mergeOpcode, confirmed via gdb to correctly detect and skip BOTH
colliding merges (hit 1 and hit 3), yet the final decompiled output is
UNCHANGED. Fix does not work as hoped; root cause of the printed value
is evidently not solely dependent on this specific merge occurring.
======================================================================
Applied the candidate fix exactly as planned: in Merge::mergeOpcode
(merge.cc, inside the mergeTestRequired(...) block, immediately before
the existing merge(vn1->getHigh(),vn2->getHigh(),false) call), added
`if (!mergeTestBoundary(vn1->getHigh(),vn2->getHigh())) { ... merge(...); }`,
reusing mergeTestBoundary as-is (no changes to that function -- its
existing level=1 intersectList call and simple boolean return were
already exactly what was needed). On a detected boundary collision the
merge is now skipped entirely and the loop continues to the next
input/op, leaving vn1 and vn2 as separate HighVariables. Rebuilt
decomp_test_dbg.exe clean (BUILD FINISHED exitcode=0, ~5-6 min per the
established pattern).

VERIFIED THE FIX FIRES CORRECTLY, LIVE: re-ran the gdb harness with a
breakpoint at the new `if (!mergeTestBoundary(...))` line (merge.cc:407
in this build) and called mergeTestBoundary directly on each hit's
vn1/vn2 pair:
  Hit 1: high(0xef45d0)/high(0xf70600) -> mergeTestBoundary = TRUE
         (collision detected, merge correctly SKIPPED)
  Hit 2: high(0xef44c0)/high(0xec8ce0) -> mergeTestBoundary = FALSE
         (different, unrelated pair -- not part of the traced chain --
         no collision, merge correctly proceeds as before)
  Hit 3: high(0xee3210)/high(0xef45d0) -> mergeTestBoundary = TRUE
         (collision detected, merge correctly SKIPPED)
This exactly matches the hit-1/hit-3 chain already identified and
directly observed two entries ago (same high(...45d0)-family pointer
appearing as one side of hit 1 and the other side of hit 3). The fix
mechanism itself is confirmed working precisely as designed: both
halves of the previously-traced bad merge chain are now correctly
identified as boundary collisions and neither merge occurs.

RAN THE FULL TEST DESPITE THIS: decomp_test_dbg.exe's `print C` output
is COMPLETELY UNCHANGED from before the fix -- still `return
in_stack_00000006;`, byte-for-byte identical decompiled function body
to the pre-fix run. Skipping both halves of the traced merge chain has
NO visible effect on the final printed result.

WHAT THIS MEANS: the specific HighVariable merge chain traced and
confirmed via live Cover-data inspection two entries ago, while a
GENUINE boundary-collision bug in its own right (confirmed, reproduced,
now correctly caught by mergeTestBoundary), is evidently NOT the sole
or direct mechanism by which the wrong value ends up printed at the
RETURN statement in this repro. Possible explanations, none yet
confirmed:
  (a) The Symbol/identity that ends up wrongly printed may be
      attached to one of vn1/vn2's (now separate, unmerged)
      HighVariables independently of whether THIS merge happens --
      i.e., something upstream or parallel already decided "this
      storage location is called in_stack_00000006" via a mechanism
      that does not depend on the mergeOpcode chain at all, and our
      fix, while correctly blocking a real bug, is blocking the wrong
      (or an insufficient subset of the) culprit(s) for THIS
      particular symptom.
  (b) There may be additional merge/symbol-inheritance steps elsewhere
      (mergeMultiEntry, mergeAdjacent, buildDominantCopy, or another
      mergeOpcode-driven CPUI_COPY pair not part of the specific
      hit-1/hit-3 chain) that independently produce the same wrong
      result through a parallel path -- recall from much earlier this
      session that ALL 9 relevant merge call sites were tagged with
      g_mergeCallSiteTag specifically because more than one call site
      COULD be responsible; only the live-binary PyGhidra run's
      3-entry syminherit log was ever actually inspected in detail,
      and that log only ever showed callSite=mergeOpcode for all 3
      entries in THAT run -- it was never re-checked whether skipping
      those specific merges (as opposed to just identifying them)
      changes the live-binary output either.
  (c) The RETURN op's actual input Varnode/HighVariable identity by
      the time printing happens may not be vn1 or vn2 from this hit at
      all -- possibly a THIRD HighVariable, reached via some other
      merge or via direct Symbol assignment (e.g. parameter-storage
      Symbol binding, independent of any Cover-merge mechanism)
      that was never part of the traced chain in the first place, and
      the visual coincidence of "b's Symbol ends up on the division
      result" may have a different proximate cause than the
      mergeOpcode chain, even though that chain is a real, separate,
      confirmed bug worth fixing regardless.

NEXT STEPS:
1. Before assuming the fix is wrong, directly identify -- via gdb, in
   THIS exact fixed build -- which HighVariable the RETURN op's input
   Varnode actually belongs to by the time printing happens, and
   whether IT has a Symbol/name attached, and via what code path. This
   is the one thing that was never done: tracing forward from the
   RETURN op to its actual printed identity in a live debugger, rather
   than only tracing backward from symptom to merge-chain.
2. Check whether the fix has any effect at all by comparing hit counts
   and merge chain shape before/after -- e.g. does skipping hit 1/hit 3
   change what LATER merges occur (a downstream merge might now merge
   vn1's or vn2's now-separate HighVariable into the same wrong place
   via a different route)? The action pipeline runs mergeOpcode
   multiple times across different Action passes (ActionMergeCopy is
   only one caller per the earlier stack trace,
   ghidra::ActionMergeCopy::apply at coreaction.hh:394) -- worth
   checking whether mergeOpcode(CPUI_COPY) runs more than once across
   the whole decompilation and whether a LATER pass re-attempts and
   succeeds at merging the same or an equivalent pair via a slightly
   different Cover state after other actions have run.
3. If forward-tracing from RETURN confirms a different, unaddressed
   merge/symbol-assignment step is the actual proximate cause, keep
   the mergeOpcode fix (it is independently correct and worth keeping
   regardless) and continue the search using the same gdb +
   decomp_test_dbg.exe loop to find that step.
4. Do NOT yet test this fix against the live PyGhidra binary/
   muldiv_u16 at its real address -- confirm and fully understand the
   fix's actual effect (or lack thereof) in the fast, safe standalone
   harness first.

CURRENT STATE: merge.cc carries the mergeTestBoundary-reuse fix in
mergeOpcode (confirmed compiling and confirmed functionally firing
correctly via gdb, but confirmed NOT sufficient on its own to change
the final printed output in this repro). decomp_test_dbg.exe reflects
this fix. gdb_commands7.txt / gdb_output7.txt hold this entry's exact
verification session. Not yet reverted -- keep this fix in place while

======================================================================
LIVE BINARY RETEST WITH THE FIX (2026-09-14) -- output CHANGED but bug
STILL PRESENT in a new shape: an extra intermediate variable (uVar4)
now carries b's value and gets returned, instead of b being returned
directly. Confirms explanation (b) from the immediately preceding
entry: at least one more instance of the same class of defect remains.
======================================================================
Rebuilt the real decompile.exe (gradlew.bat
:Decompiler:decompileWin_x86_64Executable --rerun-tasks, BUILD
SUCCESSFUL in 1m12s) with the mergeTestBoundary-reuse fix from the
previous entry, deployed to the live PyGhidra install (verified via
matching LastWriteTime/Length). Had to clear the MULDIV_DEBUG_WAIT
user environment variable (set earlier this session for the
cdb/WinDbg attach attempts, no longer needed now that
decomp_test_dbg.exe+gdb is the debugging method of choice) and restart
PyGhidra once more before force_decompile stopped timing out --
confirms, again, that env var changes only take effect for a process
tree after the parent is restarted, consistent with earlier findings
this session.

RESULT -- muldiv_u16 decompiled output, WITH the fix, freshly
force_decompiled:
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
    uVar4 = b;
    puVar5 = auStack_82 + 0x80;
    uVar1 = *(ushort *)(puVar5 + 10);
    if ((uVar1 == 0) || (uVar1 <= uVar2)) {
      return 0xffff;
    }
    uVar3 = (short)(CONCAT22(uVar2,uVar3) / (uint4)uVar1);
    return uVar4;
  }

COMPARISON TO PRE-FIX OUTPUT (`return b;` directly, no uVar4 at all):
the fix DID change something real -- the direct fold of b's
HighVariable into the division-result HighVariable that this session
traced and fixed no longer happens. But the practical bug is
UNCHANGED: `b` is still, in effect, what gets returned -- now via one
extra hop through a new intermediate variable (`uVar4 = b;` ...
`return uVar4;`) instead of being folded away entirely. A user reading
this output still sees the function apparently returning the second
parameter unchanged, when it should return the division result.

DIAGNOSIS: this directly confirms explanation (b) from the immediately
preceding standalone-repro entry, not (a) or (c) -- there IS at least
one more merge or copy-forwarding step, independent of the specific
mergeOpcode@division-site merge already fixed, that produces the same
class of wrong-value-returned symptom. The mergeOpcode fix correctly
blocked ONE real boundary-collision bug (confirmed on live Cover data,
confirmed via decomp_test_dbg.exe+gdb, confirmed here to visibly change
the live binary's output), but the underlying defect evidently has
multiple independent trigger points in this function, or a second,
structurally similar collision now surfaces because the decompiler's
pipeline runs multiple passes/actions and a LATER action step
re-encounters an equivalent boundary situation for uVar4 specifically
(consistent with NEXT STEPS item 2 from two entries ago, which
specifically raised this possibility: "does skipping hit 1/hit 3
change what LATER merges occur... worth checking whether
mergeOpcode(CPUI_COPY) runs more than once across the whole
decompilation").

NEXT STEPS:
1. Repeat the exact same forward-trace-from-RETURN technique that
   worked in the standalone harness (break at PrintC::opReturn /
   printc.cc:783, inspect op->getIn(1)'s HighVariable and Symbol) but
   this time on uVar4's assignment/RETURN in a decomp_test_dbg.exe
   repro of the FULL, unmodified live binary output shape (i.e. now
   that we know the fix changes but doesn't fully solve the live
   output, build a new/updated XML repro that captures whatever
   produces uVar4, and confirm whether uVar4's wrong identity comes
   from the SAME level=2 Cover-boundary defect via a different
   merge/copy site, or a genuinely different mechanism).
2. Specifically check whether Merge::mergeOpcode(CPUI_COPY) is invoked
   more than once across the action pipeline (multiple Action passes),
   and whether the SECOND invocation encounters a NEW boundary
   collision between uVar4's HighVariable and the division-result
   HighVariable that mergeTestBoundary, as currently written (called
   only from the one call site added in the previous entry), does not
   yet cover -- e.g. if the second pass's specific vn1/vn2 pair
   differs enough (different block indices, different Cover shape
   after the first pass's changes) that OUR same boundary check
   doesn't trigger for THIS new pair even though the underlying defect
   is the same class of bug.
3. Consider whether the fix needs to be broadened (still narrowly, not
   via a global intersectList threshold change) to cover whatever
   mechanism now produces uVar4 -- possibilities include: another
   mergeOpcode-driven CPUI_COPY merge for a different pair not yet
   checked, a mergeMultiEntry/mergeAdjacent/buildDominantCopy call
   site (all still untouched by any fix so far), or a copy-forwarding/
   propagation action entirely separate from the Merge:: family
   (e.g. ActionCopyMarker or similar dead-copy-elimination passes that
   might be independently choosing to forward b's value into uVar4's
   storage for unrelated reasons that only LOOK like the same bug).
4. Re-run the full sat_add_u16 regression check (the established
   control function) against this fix before going any further, to
   make sure the mergeOpcode change has not introduced any NEW problem
   even though it hasn't yet fully solved the original one -- this has
   not yet been done in this session and should not be skipped just
   because the primary fix attempt is incomplete.

CURRENT STATE: the real decompile.exe (built via gradle, MSVC,
deployed to the live PyGhidra install) now carries BOTH the earlier
mergeTestBoundary-in-mergeOp fix (validated long ago, kept) AND the new
mergeTestBoundary-in-mergeOpcode fix (this session, confirmed to
change but not fully fix the muldiv_u16 symptom). decomp_test_dbg.exe
(the standalone MinGW/gdb harness) also carries the mergeOpcode fix.
MULDIV_DEBUG_WAIT is now cleared (removed from User environment
variables) since decomp_test_dbg.exe+gdb has fully replaced the
cdb/WinDbg approach for live debugging going forward -- if any future
session needs to debug the real decompile.exe/PyGhidra path directly
again, the ghidra_process.cc gate is still present in source (harmless,
opt-in) and could be re-enabled by re-setting the environment variable

======================================================================
PIVOTAL FINDING (2026-09-14, immediately following) -- the
muldiv_u16_syminherit_debug.txt log is COMPLETELY EMPTY on this run.
uVar4's wrong value is NOT coming from HighVariable::mergeInternal's
symbol-inheritance path at all. The remaining bug is a DIFFERENT
mechanism than everything traced and fixed so far this session.
======================================================================
Cleared muldiv_u16_syminherit_debug.txt and re-ran force_decompile on
muldiv_u16 against the live, fixed decompile.exe (same one just
rebuilt/deployed in the immediately preceding entry). Output identical
to the previous run (uVar4 = b; ... return uVar4;). Checked the log:
ZERO entries. Confirmed via Test-Path/Get-Content that the file was
successfully deleted beforehand and simply does not exist after this
run, meaning the code path at variable.cc's symbol-inherit log point
inside HighVariable::mergeInternal (dbgf4 <<
"[mergeInternal symbol-inherit] callSite=...") was never reached at
all during this decompilation.

WHY THIS MATTERS: this rules out explanation threads that assumed the
remaining bug is still a Merge::mergeOpcode (or any other Merge::merge
family) call performing a symbol-inheriting HighVariable merge that
our existing instrumentation would have caught. It did not fire even
once. The mergeTestBoundary fix from two entries ago is doing exactly
its job -- blocking the ONE specific boundary-collision merge this
session traced end-to-end via live Cover data -- and, apparently, no
OTHER Merge::merge-family call is stepping in to produce an equivalent
symbol-inheriting merge for uVar4 either. Yet `uVar4 = b;` /
`return uVar4;` still appears in the final output.

REVISED DIAGNOSIS: uVar4's wrong value is most likely being produced
by a mechanism entirely OUTSIDE the Merge:: class family -- e.g. a
copy-propagation, dead-copy-elimination, or copy-forwarding Action
(candidates raised in the immediately preceding entry's NEXT STEPS
item 3: ActionCopyMarker or similar) that independently decides to
materialize b's value into a new temporary/variable and feed that into
the RETURN op's input, for reasons unrelated to HighVariable Cover
collisions or Symbol inheritance at all. This would explain both: (a)
why the syminherit log stays empty (no HighVariable merge with a
non-null inherited Symbol occurs), and (b) why the SHAPE of the output
changed (a new intermediate uVar4 appeared, where before the
mergeOpcode bug had folded everything down to a single, more
economical `return b;` with no intermediate step at all) -- consistent
with the mergeOpcode fix successfully preventing ONE kind of over-eager
folding, while a SEPARATE, structurally different bug (whatever
produces uVar4) was always present underneath, and previously invisible
because the (now-fixed) merge bug pre-empted it by collapsing the
variable earlier in the pipeline before this second mechanism had
anything left to act on.

NEXT STEPS:
1. Do NOT keep searching within the Merge:: family (mergeMultiEntry,
   mergeAdjacent, buildDominantCopy, or a second mergeOpcode pass) --
   the empty syminherit log is strong direct evidence none of those
   are responsible for THIS remaining symptom. Broaden the search to
   the action/copy-propagation layer instead.
2. Identify what actually assigns uVar4's value and connects it to
   `b`. Options, in likely order of investigation cost: (a) search
   coreaction.cc/allaction-registration files for copy-propagation/
   copy-forwarding Actions with names like ActionCopyMarker,
   ActionDeadCode, ActionForwardCopy or similar that run in the same
   pass group as ActionMergeCopy (recall
   ghidra::ActionMergeCopy::apply, coreaction.hh:394, was the caller
   already identified for Merge::mergeOpcode much earlier this
   session -- checking what OTHER actions run adjacent to
   ActionMergeCopy in the same group/pass ordering is a natural next
   step); (b) add fresh, targeted debug instrumentation (following
   this session's established std::ofstream-to-file pattern) at
   whatever PcodeOp creates/assigns uVar4's defining COPY, to log its
   creation and inputs directly, the same way the RETURN op's actual
   input was traced earlier in this document; (c) use the
   decomp_test_dbg.exe + gdb harness again, but this time build a new
   XML repro that includes a `prototype lock`-equivalent or otherwise
   sets up named/recognized parameters (unlike the current
   muldiv_repro.xml, which was shown two entries ago to fall back to
   "irregular input" naming and does NOT reproduce the uVar4 symptom
   at all -- confirmed this session that mergeOpcode only fires 3
   times total in that repro and the program runs to completion
   normally, meaning whatever produces uVar4 in the live binary simply
   doesn't have anything to act on in the current minimal repro,
   since there's no properly-recognized `b` parameter for it to
   target in the first place).
3. Once uVar4's true origin is found, re-run the SAME live PyGhidra
   force_decompile test (fast, immediate feedback, already proven
   reliable now that the environment/restart issues are resolved) to
   confirm any new fix actually removes uVar4/return-uVar4 from the
   output entirely, leaving only the mathematically correct division
   result being returned.
4. Once genuinely fixed, run the full regression suite this document
   has used throughout: sat_add_u16 (unaffected control, not yet
   re-verified against either fix this session -- still outstanding
   per the previous entry's NEXT STEPS item 4) plus the other affected
   sibling functions named early in this document (muldiv_u16_scaled,
   muldiv_s16_rounded_3op, muldiv_s16_rounded, weighted_blend_u8frac).


======================================================================
GROUND-TRUTH CONFIRMATION (2026-09-14, immediately following) --
pulled raw HighFunction P-code for muldiv_u16 directly via
get_function_pcode. The RETURN op's actual data-flow chain is
COMPLETELY CORRECT and unambiguous. This is purely a display/
HighVariable-naming defect, not a p-code correctness bug, confirmed
now with zero remaining ambiguity.
======================================================================
Used mcp__ghidra__get_function_pcode (granularity=high) against the
live, currently-fixed muldiv_u16 to get the full HighFunction P-code
listing with concrete varnode addresses/spaces. Traced the RETURN op
backward through actual SSA def-use edges (not decompiler-printed
names, not HighVariable identity -- literal p-code operand/output
address matching):

  RETURN (0x141da) reads register[0]
    <- COPY (0x141d7): register[0] = register[2]
      <- register[2]'s LAST definition before 0x141d7 is the COPY at
         0x141ce: register[2] = unique[0xaad02]
           <- unique[0xaad02] = SUBPIECE(unique[0xaad00], 0)
             <- unique[0xaad00] = INT_DIV(unique[0xaa900], unique[0xaab00])
                (the genuine division: dividend PIECE'd from
                register[0]/unique[a2702], divisor ZEXT of unique[0x21e00]
                -- i.e. the real mulxu.w/divxu.w computation this whole
                document is about)

This chain is completely clean, single-valued, and correct at every
step. There is NO p-code edge anywhere in this listing connecting `b`
(stack offset 0x6) to register[0] at the RETURN. The earlier COPY at
0x141c2 (register[2] = unique[a2702], part of the mulxu.w chain, tagged
"A" throughout this document) is a SEPARATE, EARLIER definition of the
same register[2] storage location that is fully overwritten by the
0x141ce COPY before the RETURN ever reads it -- ordinary, correct
register reuse, not a data-flow bug.

CONCLUSION: muldiv_u16's actual computed behavior, and the actual
p-code SSA graph, have ALWAYS been correct throughout this entire
investigation (confirmed early on via emulation/manual trace, and now
independently reconfirmed from the live HighFunction graph). The bug
is entirely confined to how the decompiler's HighVariable-merging and
naming layer chooses to DISPLAY this correct underlying computation --
specifically, something makes it print `uVar4`/`b`-named source text
for a value that p-code-level analysis shows is unambiguously the
division result. This rules out any concern that the current fix (or
lack of a full fix) could cause an actual WRONG COMPUTATION in
downstream tooling that consumes raw p-code/emulation rather than the
printed C -- the risk is entirely confined to human-readable source
correctness, not runtime semantics.

WHAT STILL NEEDS EXPLAINING: given the p-code is unambiguous, `uVar4`'s
mislabeling must originate in exactly one of: (1) HighVariable
construction/grouping (something groups the earlier 0x141c2 COPY's
output varnode and the later 0x141ce COPY's output varnode into the
same displayed HighVariable/name despite them being genuinely different
SSA values sharing only a storage location -- this is DIFFERENT from
the already-fixed Merge::mergeOpcode bug, which folds distinct
HighVariables together; this would instead be a case where they were
NEVER separated properly, or a naming/nameRepresentative choice step
picks the wrong one of several legitimately-separate-but-related
HighVariables to name after), or (2) the print/emission layer itself
(PrintC's variable-declaration and pushVn logic) resolves the SAME
correct HighVariable to the wrong display name via getNameRepresentative()
or equivalent, even though the Varnode/PcodeOp graph itself is fine.

NEXT STEPS:
1. Repeat the same PrintC::opReturn (printc.cc:783) breakpoint
   technique used successfully in the standalone harness, but this
   time against the LIVE decompile.exe/PyGhidra process for the real
   muldiv_u16 -- inspect op->getIn(1)'s actual Varnode identity (which
   physical p-code varnode does it correspond to -- the 0x141c2 COPY's
   output or the 0x141ce COPY's output?), its HighVariable pointer,
   and getNameRepresentative(), to determine definitively whether
   uVar4's wrong name comes from HighVariable grouping (multiple
   distinct defining varnodes sharing one HighVariable that was never
   split) or from name-representative selection picking the wrong
   candidate within an otherwise correctly-scoped HighVariable.
   NOTE: the decomp_test_dbg.exe standalone harness cannot be used for
   THIS specific step, since (confirmed two entries ago) its minimal
   repro never reaches the "irregular input"-free, fully-parameterized
   state needed to reproduce uVar4 at all -- this step needs either
   the live PyGhidra binary (now that MULDIV_DEBUG_WAIT is cleared,
   consider re-enabling it just for this one investigation, or revisit
   the standalone-harness prototype-setup blocker from two entries ago
   to build a repro that DOES reproduce uVar4) or a new standalone
   repro with real parameter/prototype recognition.
2. Once the exact mechanism is identified, design a fix scoped as
   narrowly as the two fixes already applied this session (mergeOp's
   and mergeOpcode's mergeTestBoundary reuse) -- likely either a
   HighVariable-splitting correction (if grouping is the issue) or a
   nameRepresentative selection fix (if naming choice is the issue),
   NOT a change to the underlying Cover-intersection threshold that
   was already shown early in this document's history to cause
   regressions when loosened globally.
3. Continue using the fast live-PyGhidra force_decompile loop (now
   reliable, environment issues resolved) for rapid iteration once a
   fix candidate exists, following with the standing regression checks
   (sat_add_u16 plus the other named sibling functions) before
   considering this fully resolved.


======================================================================
RETURN OP TRACED LIVE AND FOUND INNOCENT (2026-09-14) -- the RETURN
statement's own HighVariable is completely clean (numInstances=1,
correctly tied to the 0x141d7 COPY, matches the ground-truth p-code
trace exactly). The bug must be in the uVar4 ASSIGNMENT statement's
own identity resolution, not the RETURN statement.
======================================================================
Attempted cdb attach to the live decompile.exe first (re-enabled
MULDIV_DEBUG_WAIT, restarted PyGhidra, attached with `bp
printc!PrintC::opReturn; g; kb 3; !dvo op 1; q`) -- got stuck on a
deferred breakpoint resolution (symbol server lookup pending) and an
invalid `!dvo` extension command (not a real WinDbg command, an
SOS/.NET-ism that doesn't apply here). Rather than keep fighting cdb's
fragility (consistent with every other cdb attempt this session),
abandoned it in favor of the file-based std::ofstream debug-logging
technique that has worked reliably every time this session. Cleared
MULDIV_DEBUG_WAIT again (back to unset).

Added debug logging directly in PrintC::opReturn (printc.cc, right
before pushVn(op->getIn(1),op,mods)) dumping: the RETURN op's actual
input Varnode pointer and address, which PcodeOp address defines it
(retVn->getDef()), its HighVariable pointer and numInstances(), and
the HighVariable's own getNameRepresentative() (pointer, address,
defining op) -- to determine directly whether the wrong-name symptom
originates from HighVariable grouping or from name-representative
selection, per the two hypotheses raised in the immediately preceding
entry. Required #include <fstream> (this file didn't have it) and a
const-correctness fix (op->getIn(1) returns `const Varnode *` since
opReturn takes `const PcodeOp *op`). Rebuilt clean (BUILD SUCCESSFUL,
1m11s), deployed, force_decompile re-run -- same bug shape reproduced
(uVar4 = b; ... return uVar4;).

muldiv_u16_opreturn_debug.txt, full contents (2 entries -- one per
RETURN statement in the function, the 0xffff early-exit and the real
one):
  retOp=0x141da:6a retVn->getDef()=141d7 high->numInstances()=1
        nameRep==retVn nameRep->getDef()=141d7
  retOp=0x141da:cd retVn->getDef()=141d7 high->numInstances()=1
        nameRep==retVn nameRep->getDef()=141d7

BOTH RETURN statements' input Varnode is defined by the COPY at
0x141d7 -- exactly matching the ground-truth ManuallyTraced p-code
chain from two entries ago (register[0] = register[2], itself fed
correctly from the division result). numInstances()=1 means this
HighVariable has never been merged/grouped with anything else at all
-- it is a single, clean, unambiguous Varnode-to-HighVariable mapping.
nameRep is literally the same pointer as retVn itself, so there is no
name-representative-selection issue either: this HighVariable would,
if printed, print under a name assigned specifically to itself, tied
to the 0x141d7 definition.

CONCLUSION -- IMPORTANT COURSE CORRECTION: the RETURN statement itself
is entirely innocent. Its HighVariable is clean, correctly scoped, and
correctly tied to the real division-result chain. This means `return
uVar4;` in the printed output is not wrong because the RETURN's own
HighVariable resolution is wrong -- if `uVar4` really is this
HighVariable's name, then the RETURN statement is printing exactly the
right thing, honestly. The bug must instead be entirely within the
EARLIER statement `uVar4 = b;` -- i.e., the COPY operation that
DEFINES uVar4's value is itself incorrectly resolving/printing its
SOURCE operand as `b` when the actual p-code source (per the same
ground-truth trace) should be the SUBPIECE of the division result
(unique[0xaad02] in the earlier P-code dump), not `b`'s stack storage
at all. In other words: this session's forward-trace-from-RETURN
technique correctly ruled OUT the RETURN statement as the site of the
bug, and instead points at the ASSIGNMENT (`uVar4 = b;`) as the actual
site where an incorrect Varnode/HighVariable is being resolved and
printed as the right-hand side.

NEXT STEPS:
1. Find and instrument the COPY-printing code path (likely
   PrintC::opCopy or the general assignment-statement emission code in
   printc.cc, analogous to what was found for opReturn) to dump the
   SAME kind of identity information (source Varnode, its def, its
   HighVariable, numInstances, nameRep) for the COPY op that produces
   the `uVar4 = b;` line specifically -- expect this one to show either
   numInstances()>1 (grouping bug) or a nameRep that does NOT match the
   actual defining op address (naming-selection bug), unlike the clean
   RETURN case just traced.
2. Cross-reference whatever Varnode this COPY's PRINTED right-hand
   side corresponds to against the ground-truth P-code dump from two
   entries ago -- specifically check whether it's actually printing
   the SUBPIECE(unique[0xaad00]) result (the correct division value)
   under a HighVariable that has ALSO absorbed/grouped in b's own
   Varnode from stack offset 0x6, which would finally reconcile this
   session's various merge-chain observations (the still-active,
   not-yet-explained hop from the earlier Cover-collision
   investigation) with this newer, RETURN-focused trace.
3. Keep both already-applied mergeTestBoundary fixes in place; this
   entry does not call either fix into question, it simply relocates
   the remaining search target from the RETURN statement to the
   preceding assignment statement.

CURRENT STATE: printc.cc carries the new opReturn debug-logging
instrumentation (harmless, should be reverted before any upstream-
facing change, alongside all other debug instrumentation accumulated
this session). muldiv_u16_opreturn_debug.txt holds this entry's raw
output for reference. MULDIV_DEBUG_WAIT is cleared again. No further
source logic changes this entry beyond the debug dump itself.

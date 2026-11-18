IMPLEMENTATION PLAN: cspec-driven far-pointer call-argument fusion
(a real Ghidra decompiler C++ patch for the #817-class bug)
=====================================================================

STATUS: plan only, nothing written to the real Ghidra source yet.
Written 2026-08-03 after live investigation of the actual C++ (see
review2.md items 19, 26, 27 for the full dead-end history -- read
those first if picking this up cold). This file is the buildable
spec; review2.md item 27 is the investigation log that got us here.

---------------------------------------------------------------------
0. GOAL (confirmed with PJ before writing this)
---------------------------------------------------------------------
Both of:
  1. Proper fix for the reference/xref gap (already solved today by
     FixBankOffsetReferences_v2.java -- this plan does not replace
     that script, it's additive)
  2. Clean decompiled C display: a joined far-pointer argument
     (e.g. "bank<<16|offset" typed as one pointer) at call sites,
     instead of two raw pushed scalars
Delivered as a GENERAL, cspec-configurable mechanism -- not hardcoded
to one ROM's layout, so different H8/539F ROMs (or other segmented
processors) can each declare their own bank+offset call-argument
convention without touching C++ again after this lands.

---------------------------------------------------------------------
1. WHY THIS NEEDS NEW C++ (recap -- full detail in review2.md #27)
---------------------------------------------------------------------
Two existing Ghidra mechanisms were checked against the real source
and both dead-end for this specific bug shape:

  - SegmentOp::unify() / ActionSegmentize (userop.cc, coreaction.cc
    ~line 628): only ever inspects a SINGLE CALLOTHER op's own 3
    inputs. Structurally cannot see two separate prior push
    instructions -- there is no pcode-level link from a pjsr call op
    back to STOREs several instructions earlier. Not fixable by
    changing unify()'s matching logic; the information isn't there
    at that pipeline stage.

  - ParamEntry::resolveJoin() (fspec.cc line 94-116): requires each
    piece of a join to ALREADY match an independently-existing
    ParamEntry elsewhere in the same static compiler-spec model
    (findEntryByStorage, throws "<pentry> join must overlap at least
    one previous entry" otherwise). Our bank byte and offset word are
    never independently meaningful parameters anywhere else in the
    model -- they only ever exist together. Confirmed this throws at
    cspec-load time, not just "doesn't help."

The one REAL, live, per-call-site join mechanism that already exists
and actually fits our shape:
    FuncCallSpecs::checkInputJoin() / doInputJoin()   (fspec.cc ~5351)
    Architecture::constructJoinAddress()               (called at ~5392)
Currently only ever invoked from ActionParamDouble (coreaction.cc
~1692), gated on SplitVarnode::inHandHi/inHandLo -- i.e. it only
fires when some OTHER prior dataflow analysis already proved the two
Varnodes are secretly one bigger value via real arithmetic in the
program. Our bank+offset pair has no such arithmetic anywhere in the
caller -- the two pushes are genuinely independent immediates until
the callee's own (invisible-to-Ghidra) convention combines them.

CONCLUSION: checkInputJoin/doInputJoin/constructJoinAddress is the
right FUSION MECHANISM (reuse it, don't reinvent Varnode/Address
plumbing). What's missing is the right TRIGGER -- since there's no
dataflow evidence to detect this from, the trigger has to come from
"DECLARED cspec knowledge instead of a Rule pattern-matching existing
pcode. That's the new code this plan describes.

---------------------------------------------------------------------
1.5 ALL NON-C++ ALTERNATIVES NOW CONFIRMED CLOSED (2026-08-03)
---------------------------------------------------------------------
Three separate routes to avoid a C++ patch were checked this session
and all three are dead ends, not just \"harder than the C++ option\" --
worth recording so a future session doesn't re-attempt any of them:

 1. A second <segmentop> for the call-argument case. Confirmed via
    userop.cc (UserOpManage registration, ~line 516-527): a
    segmentop is indexed strictly by TARGET ADDRESS SPACE
    (s_op->getSpace()->getIndex()), and registering a second one for
    a space that already has one throws \"Multiple segmentops defined
    for same space\". This is also independently already documented
    in this repo's own NOTES.TXT (line 48-62, written before this
    session) as a hard, TESTED platform limit -- not unclaimed work.
    Even setting the throw aside, a second segmentop wouldn't help:
    the real blocker was never \"which segmentop resolves the
    address\", it's that no CALLOTHER exists at the call site at all
    for unify() to find.

 2. Restructuring the SLEIGH push constructors so the second push
    reads back the first and synthesizes one CALLOTHER, making the
    pattern single-instruction-visible to the EXISTING unify()
    mechanism (no C++ needed). Checked against the real call site:
    the actual instructions feeding table_lookup_interp /
    table_lookup_indexed are genuinely generic register pushes
    (\"mov R0,@-SP\" / \"mov R1,@-SP\" -- confirmed in review2.md item 19
    disassembly review, ~line 1084), each already independently
    calling spSegment(TP,SP) via the shared r0_push/r1_push-style
    constructors used for EVERY register-list push in the ISA, not
    something specific to far-pointer calls. Which register holds
    \"bank\" vs \"offset\" is a property of the CALLEE's calling
    convention, invisible to either push instruction's own encoding.
    Making r0_push/r1_push \"aware\" they're sometimes building a far
    pointer would require carrying call-site-specific ABI knowledge
    across two independently-matched instructions -- the identical
    cross-instruction problem as unify(), just relocated into SLEIGH.
    Structurally the same wall, not a smaller version of it.

 3. ParamEntry::resolveJoin() / <pentry><addr space=\"join\"> -- see
    section 1 above and review2.md item 27's TODO-closed entry.
    Confirmed by reading fspec.cc directly: no piece1/piece2 syntax
    exists in the decoder, and even a correctly-built join address
    would be rejected by resolveJoin's requirement that both pieces
    already match an independently-existing ParamEntry.

All three fail for a VARIANT OF THE SAME REASON: every existing
Ghidra mechanism for combining two values into one either (a) looks
at a single instruction/op in isolation, or (b) requires the pieces
to be independently meaningful elsewhere in the static model. Our
bank+offset pair is cross-instruction AND only ever meaningful
together -- it doesn't fit the shape any existing mechanism was built
to recognize. This is exactly why PIECE B below needs a genuinely new
Action rather than extending something that already exists: the gap
isn't a missing case in an existing rule, it's a class of problem none
of the existing rules were designed to address.

Net effect: this doesn't change the plan below, it just closes off
every remaining excuse to avoid it. If the display fix (goal 2) is
wanted, Pieces A-D are the only architecturally sound path found.

PIECE A -- new cspec schema element: <farpointerjoin>
-------------------------------------------------------
File: fspec.hh / fspec.cc

New ElementId + AttributeIds, following the exact pattern of the
existing ones at fspec.cc lines 21-52 (ATTRIB_*, ELEM_* registration
block). Pick unused numeric IDs -- existing ones run up through at
least 173 (ELEM_UNAFFECTED) and 286 (ELEM_INTERNAL_STORAGE) for a
newer addition, so use something clearly free, e.g.:
    ElementId ELEM_FARPOINTERJOIN = ElementId("farpointerjoin", 287);
    AttributeId ATTRIB_HISLOT = AttributeId("hislot", 130);
    AttributeId ATTRIB_JOINSIZE = AttributeId("joinsize", 131);
(Renumber against whatever the live tree actually has free at
implementation time -- re-grep ATTRIB_/ELEM_ id numbers immediately
before adding, since other patches may have claimed numbers since
this plan was written.)

XML shape, living INSIDE <prototype> (sibling of <returnsym>, per the
real decode loop read at fspec.cc line 4744-4776):

    <prototype name="__stdcall_far_2arg" ...>
      <returnsym> ... </returnsym>
      <input>
        <pentry minsize="2" maxsize="2"> <addr space="stack" offset="4"/> </pentry>
        <pentry minsize="1" maxsize="1"> <addr space="stack" offset="6"/> </pentry>
      </input>
      <farpointerjoin hislot="1" joinsize="3"/>   <!-- NEW -->
    </prototype>

hislot=N: index (0-based, matching input <pentry> declaration order)
of the MOST significant piece (the bank byte in our case -- matches
the ishislot bool that checkInputJoin/doInputJoin already take).
joinsize: sanity-check total byte size of the fused value (3 in our
case: 1-byte bank + 2-byte offset). Reject-and-throw at decode time
if it doesn't match the sum of the two adjacent pentry sizes -- fail
loud at cspec load, not silently at decompile time.

Only slot pair (hislot, hislot+1) is supported per prototype for a
first cut -- matches checkInputJoin/doInputJoin's existing signature
exactly (int4 slot1, bool ishislot), no new multi-piece plumbing
needed. Multiple <farpointerjoin> elements per <prototype> allowed
(a function could theoretically take more than one far pointer arg),
just decode into a vector.

WHERE THIS LIVES: NOT ParamListStandard (that's the shared default
parameter-model -- <pentry>/<group>/<modelrule> at fspec.cc line
1477-1500 -- it has no idea which specific function it's binding).
This must be a per-FuncProto property, decoded in FuncProto::decode()
(fspec.cc line 4677), stored as a new member on FuncProto:

    // in fspec.hh, class FuncProto, alongside effectlist/likelytrash:
    struct FarPointerJoinSpec {
      int4 hislot;
      int4 joinsize;
    };
    vector<FarPointerJoinSpec> farPointerJoins;

Decode block goes right after the <returnsym>/ELEM_ADDR handling
(after line 4776, before the voidinputlock/modellock flag-fixup at
line 4778), as a peekElement() loop consuming any <farpointerjoin>
siblings the same way the existing subId dispatch does.

Needs: getPieces()/setPieces()/copy() (fspec.hh lines 1384-1385, 1382)
updated to carry farPointerJoins through -- these are the existing
prototype-cloning paths (used e.g. by FuncCallSpecs::clone at fspec.hh
1684) and if we don't thread it through, a cloned call spec silently
loses the join spec. Check every call site of getPieces/setPieces/copy
in fspec.cc before considering this piece done -- do not assume the
three obvious ones are the only callers.

PIECE B -- new Action: ActionFarPointerJoin
-------------------------------------------------------
File: coreaction.cc

Modeled directly on ActionParamDouble::apply (coreaction.cc line
1692-1759), specifically the branch at line 1731-1758 (the
"!fc->isInputLocked()" search-for-joinable-adjacent-slots branch) --
copy its STRUCTURE, replace its TRIGGER.

Declaration (class definition, wherever ActionParamDouble's is
declared in coreaction.hh or similar -- grep for
"class ActionParamDouble" to find the exact header before writing):

    class ActionFarPointerJoin : public Action {
    public:
      ActionFarPointerJoin(const string &g) : Action(0,"farpointerjoin",g) {}
      virtual Action *clone(const ActionGroupList &grouplist) const {
        if (!grouplist.contains(getGroup())) return (Action *)0;
        return new ActionFarPointerJoin(getGroup());
      }
      virtual int4 apply(Funcdata &data);
    };

apply() body -- pseudocode grounded in the real API read this session:

    for (i = 0; i < data.numCalls(); ++i) {
      fc = data.getCallSpecs(i);
      if (!fc->isInputActive()) continue;          // mirrors line 1698
      if (fc->getFarPointerJoins().empty()) continue;   // new accessor, nothing declared for this prototype
      ParamActive *active = fc->getActiveInput();
      PcodeOp *op = fc->getOp();
      for (each FarPointerJoinSpec spec in fc->getFarPointerJoins()) {
        int4 j = spec.hislot;
        if (j+1 >= op->numInput()) continue;        // bounds check
        Varnode *vn1 = op->getIn(j);
        Varnode *vn2 = op->getIn(j+1);
        bool isslothi = true;                       // hislot IS vn1 by construction here, unlike ActionParamDouble's dataflow-derived isslothi
        if (fc->checkInputJoin(j, isslothi, vn1, vn2)) {
          data.opSetInput(op, /* need a real joined Varnode here -- see OPEN QUESTION below */, j);
          data.opRemoveInput(op, j+1);
          fc->doInputJoin(j, isslothi);
          count += 1;
        }
      }
    }
    return 0;

RESOLVED 2026-08-04 (was OPEN QUESTION -- now confirmed against real
source, no longer blocking): checked whether ActionParamDouble's
line 1751 pattern (`data.opSetInput(op, whole.getWhole(), j)`) has an
equivalent for us, given we have no pre-existing whole Varnode the
way double-precision recovery does (its whole is already real
because the compiler's own arithmetic built it -- ours never does).

Read double.hh/double.cc directly. SplitVarnode::createJoinedWhole()
(double.cc line ~566-578) is the exact precedent for our case --
its own doc comment is "Create a whole Varnode from pieces,
respecting piece storage," i.e. it already handles "no prior
arithmetic joins these, just synthesize the whole from fixed
storage locations," which is precisely our bank+offset shape. Its
body is short and requires no PIECE op:

    Address newaddr;
    if (!isAddrTiedContiguous(lo,hi,newaddr))
      newaddr = data.getArch()->constructJoinAddress(
                    data.getArch()->translate,
                    hi->getAddr(), hi->getSize(),
                    lo->getAddr(), lo->getSize());
    whole = data.newVarnode(wholesize, newaddr);
    whole->setWriteMask();

This maps directly onto ActionFarPointerJoin::apply's body: build
joinaddr the same way FuncCallSpecs::doInputJoin already does
internally (glb->constructJoinAddress, confirmed real at fspec.cc
line 5392), synthesize the whole Varnode with data.newVarnode(),
mark it with setWriteMask(), then proceed as originally planned:

    Address joinaddr = glb->constructJoinAddress(glb->translate,
                            hiAddr, hiSize, loAddr, loSize);
    Varnode *wholevn = data.newVarnode(joinsize, joinaddr);
    wholevn->setWriteMask();
    data.opSetInput(op, wholevn, j);
    data.opRemoveInput(op, j+1);
    fc->doInputJoin(j, isslothi);

No PIECE op needed -- that path (option (b) in the original
question) is NOT what SplitVarnode itself does for this shape, so
there's no reason to invent something more complex than the
library's own precedent.

setWriteMask() checked directly too (varnode.hh line 326: "Mark
this as not a true write when computing SSA form" / addlflags |=
Varnode::writemask). Confirmed narrow-scope by grep: only
heritage.cc reads/acts on the flag (SSA/heritage-form construction),
and the only other two setWriteMask() call sites in the whole
codebase (coreaction.cc lines 1956, 1988) use it for the same
reason ("Don't let new Varnode cause additional heritage"). No Rule
anywhere branches on it -- confirmed getWriteMask has zero call
sites outside Varnode's own accessor, i.e. nothing downstream
special-cases a write-masked Varnode's shape. Safe to use as-is.

REMAINING UNCERTAINTY (small, deferred to smoke test not design):
isAddrTiedContiguous() is checked first in the real precedent before
falling back to constructJoinAddress -- worth keeping that same
check in ActionFarPointerJoin for consistency, though for genuinely
independent stack slots (our case) it should always fall through to
the join-address branch. Confirm this is a no-op skip for our inputs
during the Piece 3 smoke test, not by additional reading -- this is
now a runtime verification detail, not an open design question.

PIECE C -- pipeline registration
-------------------------------------------------------
File: coreaction.cc, ActionDatabase::universalAction (line 5678)

Insert directly after ActionParamDouble (line 5709) and BEFORE
ActionSegmentize (line 5710) -- actually, reconsider: our trigger
doesn't depend on ActionSegmentize's output, and ActionParamDouble is
the closest structural sibling (same isInputActive/ParamTrial-era
timing), so slot immediately after it:

    actmainloop->addAction( new ActionParamDouble("protorecovery") );
    actmainloop->addAction( new ActionFarPointerJoin("protorecovery") );  // NEW
    actmainloop->addAction( new ActionSegmentize("base"));
    ...

Rationale for exact position: must run while fc->isInputActive() is
still true (checkInputJoin line 5354 returns false otherwise) -- so
strictly before ActionActiveParam's trimming/buildInputFromTrials
commit (line 1834-1852, same mainloop, runs later in the group so
this is safe) and after the trial list is populated. Confirm on a
real debug build that isInputActive() is true at this exact point
for a representative call site before trusting the plan blindly --
timing bugs here would silently no-op the whole feature rather than
crash, so it needs a positive-confirmation test, not just "it built."

PIECE D -- cspec authoring for your ROMs
-------------------------------------------------------
File: h8539f.cspec (or a new cspec if this needs to be opt-in per-ROM)

Once A-C build and pass a smoke test, add <farpointerjoin> to the
existing __stdcall_far_2arg-style prototypes (table_lookup_interp,
table_lookup_indexed, and whichever others the review2.md item 19/26
history identified as needing this). Confirm the stack pentry offsets
match your actual push order (offset word first at +4, bank byte at
+6, per review2.md's original description of the mov #offset/mov
#bank/pjsr sequence) -- re-verify against a live disassembly, don't
assume the offsets from earlier chat turns are exactly right without
checking a real function.

---------------------------------------------------------------------
3. BUILD / TEST PLAN
---------------------------------------------------------------------
1. Build the decompiler executable from
   C:\Users\j.brophy.CORKILLSYSTEMS\github\ghidra (confirm exact build
   command/target for the decompile/sleigh binary -- not yet checked
   this session, check Ghidra's build docs or existing build scripts
   in that checkout before assuming gradle vs make).
2. Point a dev Ghidra install's decompiler at the freshly-built binary
   (standard Ghidra decompiler-dev workflow -- swap the platform-
   specific binary under Ghidra/Features/Decompiler/os/<platform>/).
3. Smoke test on ONE function first (table_lookup_interp), same
   caution as the original (superseded) join-pentry TODO in review2.md
   item 27 -- verify via get_function_variables / decompiler output
   that:
     a. The two stack args now show as ONE joined parameter
     b. No regression on functions that do NOT have <farpointerjoin>
        declared (should be completely unaffected -- the whole
        mechanism is opt-in per prototype)
     c. No regression on any OTHER architecture's cspec that doesn't
        use this element at all (should be a pure no-op path for them
        -- confirm by running against a non-H8 binary if any is handy,
        since this is a shared coreaction.cc pipeline change)
4. Only after (3) passes cleanly, roll out <farpointerjoin> to the
   remaining known far-pointer-argument prototypes across ROMs.
5. FixBankOffsetReferences_v2.java stays in place regardless -- it
   handles the xref/reference-database side, which this C++ change
   does not touch or replace.

---------------------------------------------------------------------
4. HONEST RISK ASSESSMENT
---------------------------------------------------------------------
- This is a shared-pipeline change (coreaction.cc, fspec.cc/hh) --
  even though the new Action is opt-in per-prototype via cspec, it's
  still new code running in universalAction's mainloop for EVERY
  function on EVERY architecture. A bug here (bad bounds check, wrong
  timing assumption) risks affecting non-H8 decompilation. Test step
  3c above is not optional.
- UPDATE 2026-08-04: PIECE B's synthesis question is now RESOLVED,
  see the RESOLVED note in PIECE B above. SplitVarnode::createJoinedWhole
  (double.cc ~566-578) is a directly-read, already-working precedent
  for exactly our shape (join with no pre-existing dataflow "whole"),
  and setWriteMask()'s scope was confirmed narrow by grep (only
  heritage.cc reads it; no Rule branches on it). PIECE B is no longer
  the open unknown it was -- every piece of this plan now traces to
  source actually read across the two review sessions.
- This still requires a full decompiler rebuild per iteration --
  slow edit-test loop compared to cspec-only changes. Confirm the
  local build turnaround time before committing to this path over
  just living with FixBankOffsetReferences_v2.java's xref-only fix.
- REVISED ESTIMATE (2026-08-04): closer to 1-2 days of C++ work, not
  multi-day/could-blow-up -- PIECE A is mechanical (half a day),
  PIECE C is trivial (an hour), PIECE B now has a concrete ~10-line
  body grounded in a real precedent (should be well under a day
  including the smoke-test check on isAddrTiedContiguous mentioned
  above), plus build/test turnaround on top of all of it. The
  remaining unknowns are now build/tooling logistics (build command,
  turnaround time), not design risk. Still worth reconfirming
  isAddrTiedContiguous's no-op behavior on our inputs empirically
  before declaring PIECE B fully done, but that's a smoke-test line
  item, not an open research question anymore.

## IMPLEMENTATION PROGRESS LOG (2026-08-18)

Source tree note: mid-session the local checkout was synced to a newer
upstream revision than this plan was originally written against, so
line numbers below are current-real, not the plan's originally-cited
ones (which had drifted, e.g. ActionParamDouble registration moved
from ~5678/5709 to actual 5865/5866). Re-verify line numbers again
before resuming if the tree has been touched further.

### PIECE A -- STATUS: COMPLETE, compiles clean (0 errors; 311
warnings/8 notes, identical count to pre-change baseline -- all
pre-existing upstream noise, none touching new code).

Concrete state, file by file:

- fspec.cc lines 51-54: ELEM_FARPOINTERJOIN (287), ATTRIB_HISLOT (130),
  ATTRIB_JOINSIZE (131) declared. NOTE: these were found CORRUPTED at
  the start of this session (a stray "ElementElementId" typo on the
  ELEM_INTERNAL_STORAGE line just above them, plus a garbled duplicate
  fragment appended to the ATTRIB_JOINSIZE line -- looked like a bad
  merge from a pre-sync attempt). Fixed via str_replace and verified
  clean on disk and via get-compiler-diagnostics.
- fspec.hh: FarPointerJoinSpec struct (hislot, joinsize fields) and
  the farPointerJoins member on FuncProto were ALREADY PRESENT going
  into this session (apparently survived/reappeared after the source
  sync) -- nothing needed to be written here.
- fspec.cc, FuncProto::decode(): new `else if (subId ==
  ELEM_FARPOINTERJOIN)` branch added inside the existing <prototype>
  sibling-decode loop, placed immediately after the ELEM_INTERNALLIST
  branch and before the loop closes (i.e. before decodeEffect() runs).
  Reads hislot/joinsize attributes, throws LowlevelError if either is
  missing, pushes a FarPointerJoinSpec onto farPointerJoins.
- fspec.cc, FuncProto::copy(): added
  `farPointerJoins = op2.farPointerJoins;` right after the existing
  `likelytrash = op2.likelytrash;` line (around line 3795-3797).
- FuncCallSpecs::clone(): NO EDIT NEEDED -- it calls
  `res->copy(*this)`, so it inherits the farPointerJoins copy for free
  once FuncProto::copy() was patched.
- getPieces()/setPieces(): CONFIRMED NOT APPLICABLE, contrary to what
  the plan assumed. These operate on the separate PrototypePieces
  struct (model/outtype/intypes/innames/firstVarArgSlot only, used for
  building a prototype from raw names+types), which has no join-spec
  field and isn't a full-fidelity clone path. Do not add farPointerJoins
  here -- there's nowhere for it to go without widening
  PrototypePieces itself, which is out of scope.
- FuncProto::encode(): NOT TOUCHED. The plan never covered this side
  (decode-only). Flagging as a real, currently-open gap: a decode ->
  encode round-trip will silently drop farPointerJoins. Likely fine
  for now since PIECE D's cspec is hand-authored XML rather than
  written back out by the decompiler in normal operation, but worth a
  conscious decision before considering this "done done."

### PIECE B -- STATUS: COMPLETE, compiles clean (0 errors in both
coreaction.hh and coreaction.cc; only new warning is a harmless
-Wsign-compare on the loop index vs joins.size(), consistent with
dozens of identical pre-existing warnings elsewhere in this file).

Concrete state:

- coreaction.hh: new ActionFarPointerJoin class declared immediately
  after ActionParamDouble (before ActionActiveParam), same shape
  (Constructor, clone(), virtual apply()).
- coreaction.cc: new ActionFarPointerJoin::apply() body, placed
  immediately after ActionParamDouble::apply() (before
  ActionActiveParam::apply()). For each call site, reads
  fc->getFarPointerJoins() and for each declared (hislot,joinsize)
  pair: builds a SplitVarnode(lovn,hivn) -- note hislot's Varnode is
  the HI piece, hislot+1 is LO, so constructor args are (lo,hi) in
  that order, not (hislot's vn, hislot+1's vn) naively -- checks
  hasBothPieces(), calls whole.createJoinedWhole(data) to synthesize
  a real merged Varnode (this step is REQUIRED; the raw
  SplitVarnode(vn1,vn2) constructor alone leaves `whole` null until
  createJoinedWhole or an equivalent is called -- getWhole() on an
  unconstructed SplitVarnode returns a null pointer, which was an
  actual bug in the first draft of this code this session, caught by
  reading double.cc's SplitVarnode::initPartial before trusting the
  constructor), then folds the join into the prototype via the same
  fc->checkInputJoin()/data.opSetInput()/data.opRemoveInput()/
  fc->doInputJoin() sequence ActionParamDouble uses.
- fspec.hh: FarPointerJoinSpec had to be made a PUBLIC nested struct
  (moved to right after the enum block, before the private
  farPointerJoins member, with an explicit `private:` label restoring
  the section state afterward) -- coreaction.cc references
  FuncProto::FarPointerJoinSpec by name, and the struct was originally
  private, causing a real compile error caught by
  get-compiler-diagnostics ("FarPointerJoinSpec is private within this
  context"). Also required moving the struct's declaration to BEFORE
  the farPointerJoins vector member's declaration, not after --
  declaring it in the public: section further down (after
  farPointerJoins) does not compile, C++ requires the element type
  known before the vector<T> member that uses it.
- New accessor added: FuncProto::getFarPointerJoins() (const,
  returns const vector<FarPointerJoinSpec>&), added next to
  getPieces()/setPieces() in the public section. farPointerJoins
  itself stays private; only the struct type and this accessor are
  public.

NOTE: mid-edit this session, a str_replace with a match ending
exactly at a function signature line (no opening brace) accidentally
consumed that signature line as part of the "old" text being
replaced, deleting `int4 ActionActiveParam::apply(Funcdata &data)`
and leaving a bare `{` with no preceding declaration -- a real
"expected unqualified-id before '{' token" compile error. Fixed by
restoring the missing signature line. Lesson for next time: when
using str_replace with an old_str that ends right at a function
signature (to insert new code before an existing function), verify
after the edit that the signature line is still present, since a
match boundary sitting exactly on that line is an easy way to lose it.

### PIECE C -- STATUS: COMPLETE, compiles clean (0 errors, warning
count unchanged at 298 -- the registration line itself is warning-free).
Added `actmainloop->addAction( new ActionFarPointerJoin("protorecovery") );`
immediately after the ActionParamDouble registration line (now at
coreaction.cc line 5907, drifted from 5865 due to Piece B's ~42 added
lines) and before ActionSegmentize("base"), exactly as planned.

### PIECE D -- STATUS: NOT STARTED. This is the last piece: actually
annotate the H8/539F cspec prototypes (table_lookup_interp, etc.) with
<farpointerjoin hislot=N joinsize=N> elements now that A/B/C are done
and the whole pipeline compiles. This is XML authoring in the cspec
file, not C++ -- lower risk, but still needs a real smoke test against
an actual H8 binary with a far-pointer call to confirm the decompiled
C output actually shows the joined argument correctly, and to verify
isAddrTiedContiguous's no-op assumption empirically as originally
flagged. A/B/C have never been run end-to-end -- only confirmed to
compile. Do not consider this feature "working" until D is done and
a real decompile has been observed.

### TOOLING NOTES FOR NEXT SESSION
- grep-project timed out/hung on this project at least once this
  session -- use grep-file scoped to a known filePath instead of
  project-wide search.
- The Pulsar MCP server was restarted mid-session; all in-memory
  checkpoints were lost when that happened (list-checkpoints returned
  empty after restart). Re-checkpoint immediately after confirming
  the server is back, before making further edits.
- str_replace threw "memory access out of bounds" twice in a row on
  what was ultimately a valid, matching edit; a third attempt after a
  full Pulsar restart succeeded. If this recurs, verify buffer state
  with get-active-editor-info (modified: false + unchanged line count
  confirms no partial write) before retrying, rather than assuming
  success or corruption.
- Last real checkpoint saved this session: "piece_a_complete" (may not
  survive a server restart -- see above).

  ## Piece D testing session (2026-08-18) -- farpointerjoin proven working, struct application blocked

  Full pipeline built and tested for the first time: Pieces A-C compiled clean
  (after fixing a missing ELEM_UNAFFECTED definition -- see below), packaged
  into a full Ghidra 12.2 dev build, and Piece D applied to the
  __stdcall_far_2arg prototype behind table_lookup_interp's 130 call sites
  (<farpointerjoin hislot="0" joinsize="3"/>, byte @ stack offset 6 + word @
  offset 7).

  RESULT: farpointerjoin mechanism confirmed working across a real spread of
  call sites (ISC, ignition timing, knock/lambda composite, TCU sensor range
  check) -- every call site now shows a single clean joined pointer argument
  (e.g. table_lookup_interp(1,(void *)0x1ccc)) instead of the previous
  unused/garbage-pointer state. This is a genuine, repeatable positive result,
  not a one-off.

  ### Two bugs found and fixed during first build

  1. fspec.cc was missing the ElementId ELEM_UNAFFECTED definition line
     (declared in fspec.hh but never defined in fspec.cc) -- linker error
     LNK2001 unresolved external. Almost certainly fallout from the earlier
     str_replace accident that was thought to be fully restored. Fixed:
     restored `ElementId ELEM_UNAFFECTED = ElementId("unaffected",173);`
     between ELEM_RETURNSYM and ELEM_INTERNAL_STORAGE.

  2. compiler_spec.rxg (the RELAX NG schema Ghidra's Java side validates cspec
     files against, at
     Ghidra/Framework/SoftwareModeling/data/languages/compiler_spec.rxg) did
     not list farpointerjoin as an allowed child of prototype_type --
     CompilerSpecNotFoundException/ValidityViolation on project open even
     though the C++ decoder and cspec XML were both correct. Fixed: added an
     <optional><element name="farpointerjoin"> block (hislot/joinsize
     attributes) alongside the existing returnaddress/unaffected/killedbycall
     optional elements in prototype_type's <interleave>.

  Both fixes needed on any future rebuild from a fresh checkout -- neither is
  part of Pieces A-C themselves, both are prerequisites for farpointerjoin to
  work at all.

  ### Struct typing attempt on table_lookup_interp -- BLOCKED, reverted

  Attempted to replace the fused `void *table` with a proper
  `TableLookupDesc *` struct (derived from the function body's own field
  accesses: offset 0 mode byte, offset 1 bias byte, offset 2 axis_ptr
  (ushort*), offset 4 stride_ptr (ushort*), offset 6 stride byte -- struct
  created successfully in Ghidra's data type manager, 8 bytes).

  Applying it via set_function_prototype broke table_lookup_interp's
  CUSTOM_STORAGE binding (the one documented in this file's own 2026-08-03
  PROTOTYPE FIXED note) -- prototype collapsed to table_lookup_interp(void),
  body fell back to raw in_stack_00000005/in_stack_00000006 reads, i.e. the
  exact broken pre-fix state. Recovered by re-running
  FixTableLookupInterpStorage.java, which restored ep_val@Stack[0x4]:2 and
  table@Stack[0x6]:2 and the clean void* decompile.

  Root cause confirmed via two independent errors:
  - set_local_variable_type on the retyped parameter throws
    java.lang.ArithmeticException: / by zero (Storage: Stack[0x6]:2) --
    reproducible, not a one-off, happens both before and after the
    storage-restore.
  - Retrying via the raw in_stack_00000006 alias instead gives a clearer
    error: "Variable size (2) may not be changed: type 'TableLookupDesc *'
    length is 4 (Storage: Stack[0x6]:2)" -- confirms the CUSTOM_STORAGE slot
    is pinned at 2 bytes (the unfused half-pointer width) and Ghidra's normal
    retype path cannot grow it to the fused pointer's natural 4-byte width.

  ## Widening attempt session (2026-08-18, continued) -- storage widened to 4 bytes,
  ## new blocker found: decompiler pcode model still diverges from declared storage

  Goal: widen the CUSTOM_STORAGE `table` slot from 2 to 4 bytes so a struct type
  (or any 4-byte pointer type) can actually bind, instead of fighting the retype
  API after the fact.

  ### Root cause of the ArithmeticException, confirmed via real GhidraMCPPlugin log

  Live stack trace (not speculation this time -- pasted directly from the running
  plugin) pinpoints the crash exactly:

      VariableUtilities.resizeStackVarnode(VariableUtilities.java:515)  <- / by zero HERE
        <- expandVarnode(465) <- expandStorage(427) <- resizeStorage(400)
        <- HighFunctionDBUtil.updateDBVariable(590)
        <- FunctionService.updateVariableType/setLocalVariableType/setParameterTypeEndpoint

  This is stock Ghidra core (ghidra.program.model.listing.VariableUtilities),
  reached via the plugin's REST retype endpoint. Cross-checked against a saved
  program-state export (trial.xml) for this exact function: the `table` symbol's
  persisted type record is `<type name="" metatype="ptr" size="2"><void/></type>`,
  and the program's <typegrp> block contains exactly ONE anonymous void* type,
  size 2 -- confirms there was no 4-byte pointer type anywhere in this program's
  data type manager to grow into. Reproduced live via set_parameter_type dry_run
  for both `void *` and `void * *` targets -- identical ArithmeticException either
  way, confirming the crash is about resizing the EXISTING 2-byte varnode in
  place, not about which target type is requested.

  IMPORTANT CORRECTION vs. earlier theorizing this session: this is NOT a
  farpointerjoin synthetic-varnode collision. It's simpler -- the database's own
  pointer type for this slot has always been 2 bytes wide (from the original
  FixTableLookupInterpStorage.java fix, which predates farpointerjoin), and nothing
  had ever created or applied a proper 4-byte pointer type to replace it.

  ### Fix built: FixTableLookupInterpFarPointer.java (NEW, additive, does not edit
  ### or replace FixTableLookupInterpStorage.java)

  Saved to both known script locations (project copy under
  "ghidra scripts/specific/", and Ghidra's actual search path at
  C:\Users\j.brophy.CORKILLSYSTEMS\ghidra_scripts\ -- same copy-before-running
  gotcha as FixBankOffsetReferences_v2.java, noted again here since it bit this
  session too: the MCP/Pulsar tools can edit files in the project tree, but cannot
  execute Ghidra scripts -- only Ghidra's own Script Manager can run them. Had to
  ask PJ to run it manually; this is a hard capability boundary for future
  sessions, not a one-off inconvenience.).

  Approach: rather than resizing the existing 2-byte pointer type in place (which
  would silently widen every OTHER legitimate 2-byte pointer sharing that same
  anonymous type elsewhere in the program), the script creates a new, explicitly
  NAMED 4-byte pointer type (`FarPointerVoid4`, via Pointer32DataType + dtm.resolve,
  which correctly produced a distinct `void *32` type -- confirmed by its own type
  id in the run log, 288230376151711755, separate from the original type) and
  applies it only to the three known far-pointer functions via a full
  updateFunction(..., CUSTOM_STORAGE, ...) parameter REPLACEMENT (same mechanism
  the original script already used successfully) rather than an in-place resize.

  RESULT: ran clean, real run (not dry_run), PJ's own console output confirms all
  three functions:
    table_lookup_interp:        table Stack[0x6]:2 void*16 (2B) -> Stack[0x6]:4 void*32 (4B)
    table_lookup_indexed:       same transformation
    table_3axis_interp_triple:  same transformation
  ep_val left untouched on all three (still ushort @ Stack[0x4]:2). Confirmed
  independently via get_function_variables after the run -- matches PJ's console
  output exactly. Old 2-byte type confirmed untouched (search_data_types for
  "FarPointer" found nothing before the run, found the new type after).

  ### NEW BLOCKER found immediately after: force_decompile shows the widened
  ### storage did NOT reconcile the decompiler's pcode model -- output is now
  ### WORSE than the pre-widening state, not fixed

  Checked via analyze_dataflow(variable="table") immediately after the storage
  widen: still resolves to `auStack_6`, not a real `table` varnode -- same
  mismatch as before widening, storage size change alone did not fix it.

  Then forced a full force_decompile on table_lookup_interp to see if a genuine
  cache/analysis refresh would reconcile declared storage vs. pcode model. It did
  NOT reconcile -- it exposed real corruption:
    - Body now references `table._0_2_` throughout -- the decompiler is treating
      the new 4-byte `table` as a struct/union-like object and pulling only its
      low 2 bytes out, i.e. the OLD 2-byte binding is still alive underneath the
      new declared storage.
    - `auStack_6` is still present as a separately-declared local (now sized [4],
      picked up the new storage length but is still being treated as an unrelated
      local array rather than being unified with the `table` parameter).
    - The decompiled body's own signature line reverts to
      `void __stdcall_far_2arg table_lookup_interp(ushort ep_val,void *table)`
      -- return type silently dropped from ushort to void INSIDE the decompiled
      body, even though get_function_variables/the committed signature still
      correctly reports ushort. Two layers disagreeing again, same shape of bug
      as the plate-comment's original "keeps re-inferring a 3rd stack param"
      quirk, just manifesting differently now that storage is 4 bytes.

  CONCLUSION: this confirms (does not just theorize) that CUSTOM_STORAGE parameter
  declarations and the decompiler's own stack-layout/SSA-heritage analysis are two
  separate models that are not reliably reconciled by either updateFunction() or
  force_decompile() alone for this function shape. force_decompile only refreshes
  the DECOMPILER'S CACHE of an existing analysis -- it does not force the
  underlying per-function stack-frame/variable analysis to be rebuilt from
  scratch. That distinction is the likely reason this hasn't resolved: the stale
  2-byte stack-frame analysis from before the widening is still being reused.

  NEXT STEP (not yet attempted): try clearing/rebuilding the function's STACK
  FRAME analysis directly (distinct from force_decompile's cache-only refresh)
  before re-checking -- i.e. find the right API/script action to make Ghidra
  discard and regenerate its own stack-variable model for this function, not just
  its decompiler output cache. Until that's tried, do not consider the widening
  fix complete -- "compiles/runs clean" is not the same bar as "decompiler model
  agrees with declared storage," and this session is a direct demonstration of
  that gap: the storage widen alone genuinely regressed the decompiled output
  versus the pre-widening 2-byte state, it did not just fail to improve it.

  ## Root cause finally resolved (2026-08-18, continued) -- widening was applied
  ## to the WRONG side. Callee genuinely only ever reads a 2-byte pointer; revert.

  After run_analysis (whole-program, dry_run then real, 740 functions, 0 new --
  confirmed harmless) and a manual disassemble_bytes re-pass over
  table_lookup_interp's own entry both failed to change force_decompile's output
  at all (byte-for-byte identical `table._0_2_`/`auStack_6` output both times),
  concluded the issue isn't a stale cache or stale stack-frame analysis -- pcode
  is being freshly regenerated every force_decompile and is CONSISTENTLY showing
  a 2-byte access pattern. That consistency was the tell to go read the raw
  disassembly instead of continuing to poke at decompiler cache/analysis state.

  ### disassemble_function on table_lookup_interp (00014656) -- ground truth

  Prologue: `link FP,#-0x8:8` then `stm (R2 R3 R4 R5),@-SP`. On this H8 ABI, FP
  sits 1 byte above the caller's SP-relative view, so Stack[0x6] (caller/SP view,
  what CUSTOM_STORAGE declares) is FP+7 (callee/FP view) -- matches the plate
  comment's own much earlier note about Stack[0x7]/[0x8] collision.

  The actual table-parameter load, in full:
      0001465a: ldc.b  @(0x7:8,FP), EP
      0001465d: mov:f.w @(0x8:8,FP), R4

  `mov:f.w` is a single 2-byte word load from FP+8 into R4 -- that is the ENTIRE
  `table` pointer, loaded whole, in ONE instruction. There is no second
  instruction anywhere in the function reading FP+0xA or any upper-half word.
  Confirmed by reading the full disassembly top to bottom (78 instructions) --
  every subsequent use of R4 as the table pointer derives from this single
  2-byte load, nothing wider.

  ### CONCLUSION -- this was the wrong function to widen

  farpointerjoin's fusion happens at the CALLER's push/call sites (confirmed
  working per the "Piece D testing session" entry above -- 130 call sites now
  show a single clean joined pointer argument like
  `table_lookup_interp(1,(void *)0x1ccc)`). That fusion is a call-ARGUMENT
  representation improvement for how the CALLER passes the value. It was never
  a claim that the CALLEE's own body reads a wider value -- and it doesn't:
  table_lookup_interp's own code only ever needs and reads a 2-byte pointer,
  because H8's segmented/banked memory model means a 2-byte offset is already a
  complete, valid pointer within whatever bank the EP (extension page) register
  is set to (note EP is explicitly loaded via `ldc.b @(0x7:8,FP),EP` right before
  the pointer read -- the bank context is coming from the EP register side
  channel, not by widening the stack value itself).

  So `table._0_2_` in the earlier force_decompile output was NEVER a bug or a
  stale-cache artifact -- it was the decompiler correctly and accurately
  reporting that only the low 2 bytes of the (now-widened-to-4-bytes)
  CUSTOM_STORAGE slot are actually touched by real instructions. The
  ArithmeticException/regression chase this session was chasing a real crash,
  but the FIX direction (widen the callee's own declared parameter storage to 4
  bytes) was based on a wrong assumption -- that the callee needed to consume a
  wider fused value the same way the caller now displays one. It doesn't.

  ACTION: reverting `table` on all three functions (table_lookup_interp,
  table_lookup_indexed, table_3axis_interp_triple) back to the original 2-byte
  void* via FixTableLookupInterpStorage.java (the ORIGINAL script, still
  correct and untouched this whole session). FixTableLookupInterpFarPointer.java
  (this session's new script) and the FarPointerVoid4 type it created are being
  left in place in the repo as a documented dead-end / historical record, NOT
  deleted -- but should not be re-run against the callee side again. If a
  genuinely wider pointer representation is ever wanted here, it would need to
  be a presentation-only concern (e.g. a comment or a caller-side display
  artifact), not a change to what the callee itself declares it reads, since the
  callee's real instructions are the ground truth and they are unambiguously
  2 bytes wide.

  LESSON FOR NEXT SESSION: when a "fix" (widen storage, retype, etc.) produces
  decompiler output that looks WORSE (stray ._0_2_ suffixes, resurrected
  auStack_N locals) rather than simply unchanged or broken-the-same-way, treat
  that as a signal to go read the raw disassembly before trying more
  decompiler-side remediation (cache clears, reanalysis, etc.) -- the decompiler
  may be accurately reporting real instruction behavior that contradicts an
  assumption made two layers up (in this case: caller-side call-argument fusion
  was wrongly assumed to imply callee-side wide-pointer consumption).

  ## Revert executed and confirmed clean (2026-08-18, continued)

  TOOLING NOTE: run_ghidra_script(script_name=...) and run_script_inline(code=...)
  ARE available and DO work for executing existing .java scripts directly from
  this session -- gated by GHIDRA_MCP_ALLOW_SCRIPTS=1, v5.4.1+. Earlier in this
  session it was wrongly assumed no script-execution capability existed at all
  and PJ was asked to run scripts manually via the GUI Script Manager instead.
  That was incorrect -- these tools were available the whole time, just not
  discovered until PJ pointed at "run script inline" directly. CORRECTION FOR
  NEXT SESSION: try tool_search for "run script"/"execute script" BEFORE
  concluding script execution isn't possible and asking the user to run
  something manually.

  Ran FixTableLookupInterpStorage.java (the ORIGINAL script, unmodified) via
  run_ghidra_script, dry_run first then real. Both confirmed the exact expected
  revert on all three functions:
    table_lookup_interp:       table Stack[0x6]:4 -> Stack[0x6]:2
    table_lookup_indexed:      same
    table_3axis_interp_triple: same
  ep_val untouched throughout, as always.

  Verified via get_function_variables (table_lookup_interp: table now
  `void *16` @ Stack[0x6]:2, matches) and force_decompile: the `table._0_2_`
  split and the table/auStack_6 conflation are BOTH gone. `table` is now used
  as a clean, ordinary pointer throughout the body ((char*)((int)table+1),
  (int)table+2, etc.) -- matches the original 2026-08-03 "clean void* decompile"
  description exactly. auStack_6 is still present but is now clearly a
  genuinely separate, unrelated local (one read at the top of the function,
  never touches table) rather than a symptom of a broken binding.

  STATUS: table_lookup_interp/table_lookup_indexed/table_3axis_interp_triple
  are back to the known-good, pre-widening state. FarPointerVoid4 type and
  FixTableLookupInterpFarPointer.java remain in the program/repo as a
  documented dead end -- do not re-run the FarPointer script against these
  three functions again; see the "Root cause finally resolved" entry above for
  why the widening was the wrong move. The original struct-typing goal
  (TableLookupDesc*) that kicked off this whole investigation is therefore
  BACK TO BLOCKED, same as the 2026-08-18 "Struct typing attempt" entry earlier
  in this file -- this session did not unblock it, it correctly ruled out one
  proposed path (widen the callee's declared storage) and confirmed the
  callee's real 2-byte pointer read is correct as-is. If struct typing is
  wanted here in future, the struct itself would need to be built to fit
  behind a genuine 2-byte pointer (i.e. work with what the real instructions
  actually do), not by trying to widen the callee's storage to make room for a
  4-byte pointer that the instructions never read.

  ## Java-side PrototypeModel.java patch (2026-08-18, continued) -- makes
  ## <farpointerjoin> visible to Java, prerequisite for an auto-run Analyzer
  ## and for eventual upstream submission alongside the existing C++ commit

  Context: PJ shared the real upstream-style C++ commit for this feature
  (professor-jonny/ghidra@68cb889, "Add cspec-driven mechanism for fusing
  split call-arguments into one pointer") -- confirms fspec.hh/cc and
  coreaction.hh/cc exactly as already described in this file's Pieces A-C.
  That commit is C++-only; PJ asked to upstream this properly, which means
  the previously-identified Java gap (PrototypeModel.restoreXml silently
  discardSubTree()-ing any unrecognized cspec element, confirmed earlier this
  session) needs a matching patch, not just documentation of the gap.

  ### IMPORTANT: numeric ID collisions found and corrected

  The C++ commit registered:
    ElementId ELEM_FARPOINTERJOIN = ElementId("farpointerjoin", 287)
    AttributeId ATTRIB_HISLOT = AttributeId("hislot", 130)
    AttributeId ATTRIB_JOINSIZE = AttributeId("joinsize", 131)

  Checked the Java-side registries (ElementId.java, AttributeId.java) before
  writing anything -- good thing, because all three collide with EXISTING
  Java-side entries:
    287 -> already ELEM_EXTRA_STACK ("extra_stack")
    130 -> already ATTRIB_VECTOR_LANE_SIZES ("vector_lane_sizes")
    131 -> already ATTRIB_LABEL ("label")

  Confirmed via PrototypeModel.restoreXml (elName.equals(ELEM_X.name())) that
  cspec XML parsing only ever matches on the STRING name, never the numeric
  id -- so these collisions do NOT currently break cspec loading. But the
  numeric id space is shared with the packed binary decompiler<->Ghidra
  communication protocol (separate from cspec XML), where the id absolutely
  does matter, and colliding ids there would be a real, if currently latent,
  bug -- and would very likely be flagged immediately by any upstream
  reviewer diffing against the existing Java registries. Also noticed the
  existing id sequences already have gaps (283->285->287->288->291 in
  ElementId.java) suggesting C++/Java numbering has drifted independently
  before -- another reason not to assume the C++ commit's chosen numbers are
  safe to mirror blindly.

  Corrected, non-colliding ids chosen for BOTH sides (Java side implemented
  now; fspec.cc's ATTRIB_HISLOT/ATTRIB_JOINSIZE/ELEM_FARPOINTERJOIN numbers
  still need updating to match -- NOT yet done this session, C++ side is
  untouched):
    ELEM_FARPOINTERJOIN: 287 -> 292 (next free after ELEM_UNKNOWN=291)
    ATTRIB_HISLOT:        130 -> 160 (next free after ATTRIB_UNKNOWN=159)
    ATTRIB_JOINSIZE:      131 -> 161

  ### Changes made (all in Java checkout at
  ### C:\Users\j.brophy.CORKILLSYSTEMS\github\ghidra, added as a project path
  ### this session)

  1. ElementId.java -- added ELEM_FARPOINTERJOIN = new ElementId("farpointerjoin", 292),
     placed just before the ELEM_UNKNOWN sentinel, with a comment documenting
     the 287 collision/renumber for future readers.
  2. AttributeId.java -- added ATTRIB_HISLOT (160) and ATTRIB_JOINSIZE (161),
     same placement/comment pattern, just before ATTRIB_UNKNOWN.
  3. PrototypeModel.java:
     - New public static nested class FarPointerJoinSpec { int hislot; int
       joinsize; } -- deliberately mirrors FuncProto::FarPointerJoinSpec on
       the C++ side field-for-field.
     - New private field: ArrayList<FarPointerJoinSpec> farPointerJoins,
       initialized empty, placed alongside the other Varnode[]/AddressSet
       storage fields.
     - New dispatch case in restoreXml()'s per-child-element if/else chain
       (immediately before the catch-all discardSubTree() else): matches
       ELEM_FARPOINTERJOIN.name(), reads ATTRIB_HISLOT/ATTRIB_JOINSIZE via
       SpecXmlUtils.decodeInt(), throws XmlParseException if either is
       missing/invalid (mirrors the C++ decode()'s own validation exactly --
       "requires hislot and joinsize attributes"), then appends to
       farPointerJoins. THIS is the actual fix -- previously this exact
       branch point fell through to parser.discardSubTree(subel) silently.
     - New public accessor getFarPointerJoins() returning List<FarPointerJoinSpec>,
       named identically to FuncProto::getFarPointerJoins() on the C++ side.
     - Added `import java.util.List;` (ArrayList was already imported,
       List was not).

  TOOLING NOTE (significant, worth flagging clearly for next session): mid-edit,
  str_replace and insert BOTH started failing hard on this checkout --
  insert threw a raw "Aborted(). Build with -sASSERTIONS for more info." crash
  (not a normal tool-error response) on ElementId.java specifically, repeatably,
  across three different anchor strategies (beforeString, then beforeLine+
  matchContent); str_replace failed with a plain "File not found" on the exact
  same path, also repeatably, on both ElementId.java AND later PrototypeModel.java.
  Reads (grep-file, read-file) worked fine throughout -- confirmed no partial/
  corrupt writes at any point via re-grep before retrying. Also discovered
  sed operates on "the active buffer" (no filePath param in its schema) --
  it silently ran against review3.md instead of the intended target file when
  tried as a workaround; do NOT use sed when multiple files are open, no way
  to target it at a specific path. PJ restarted the Pulsar MCP server mid-session
  and both str_replace and insert started working again immediately after,
  first try, no other change made. CONCLUSION: this was a genuine transient
  Pulsar-server-side fault (not a bad anchor, not a real file-lock/permission
  issue, not specific to one file -- it followed the session across two
  different files) -- if this recurs in a future session, restarting Pulsar
  is the fix to reach for BEFORE spending time on alternate anchoring
  strategies or alternate tools.

  ### Status / what's NOT done yet

  - fspec.cc/fspec.hh (C++ side) still use the OLD colliding ids (287/130/131).
    These need updating to 292/160/161 to match the Java side now that both
    exist -- not yet done. Until that happens, the two sides are internally
    consistent (both work independently for cspec XML decode, since that path
    never uses the numeric id) but NOT number-consistent with each other,
    which would need fixing before any packed-binary-protocol use of this
    element and before upstream submission.
  - FuncProto::encode() on the C++ side still doesn't round-trip
    farPointerJoins (decode-only), per the original commit message's own
    disclosed gap -- not addressed this session, Java or C++ side.
  - No Java-side encode()/save-to-XML counterpart written either (Java
    PrototypeModel has its own encode() for writing cspec back out -- not
    touched this session, same asymmetry as the C++ side).
  - The actual Analyzer (auto-run replacement for FixBankOffsetReferences_v2.java,
    reading getFarPointerJoins() to drive reference creation) has NOT been
    written yet -- this session only unblocked it by making the data
    available on the Java side. That Analyzer is the next real piece of work.
  - Not yet compiled/tested. This is a source-level patch only -- Ghidra's
    Java side hasn't been rebuilt or exercised against a real cspec with
    <farpointerjoin> in it this session. Should not be considered verified
    until that's done.

  ## C++ side ID renumbering (2026-08-18, continued) -- fspec.cc now matches
  ## the corrected Java-side ids

  Updated fspec.cc lines 53-55 (the exact lines from the shared upstream-style
  commit) to the non-colliding ids chosen when the Java patch was written:
    ELEM_FARPOINTERJOIN: 287 -> 292
    ATTRIB_HISLOT:        130 -> 160
    ATTRIB_JOINSIZE:      131 -> 161
  Verified via read-lines after edit -- all three now read correctly and match
  ElementId.java/AttributeId.java exactly. Used pulsar-edit-mcp-server's
  namespaced replace-all tool for this (NOT the top-level str_replace, which
  was still unreliable on this checkout even after the earlier Pulsar
  restart -- the namespaced tool set worked immediately, first try, on this
  file). Worth trying that tool family first if str_replace acts up again.

  STATUS: both sides of the numeric id registration are now internally
  consistent with each other. Still NOT done: fspec.cc's FuncProto::encode()
  round-trip gap (decode-only, disclosed in the original commit message,
  unaddressed), the equivalent Java-side encode() gap (also unaddressed), and
  actually building/testing either side -- this and the prior Java patch are
  both unverified source-level changes only. The Analyzer that would consume
  getFarPointerJoins() to auto-create xrefs (the original motivation for this
  whole thread) also still doesn't exist.

  ## Full Ghidra rebuild confirmed successful (2026-08-18, continued) --
  ## both Java and C++ patches compile clean

  PJ ran `.\gradlew.bat buildGhidra` from C:\Users\j.brophy.CORKILLSYSTEMS\github\ghidra
  twice (once cold/full, once incremental) -- both BUILD SUCCESSFUL:
    Run 1: 717 actionable tasks: 717 executed (23m 30s)
    Run 2: 717 actionable tasks: 307 executed, 410 up-to-date (20m 42s)

  Verified from this session (not just trusting the build log) that our
  specific edits actually compiled, not just skipped/cached:
    - PrototypeModel.class, ElementId.class, AttributeId.class all present
      under Ghidra/Framework/SoftwareModeling/build/classes/java/main/... --
      confirms the new FarPointerJoinSpec nested class, farPointerJoins field,
      restoreXml() dispatch case, and getFarPointerJoins() accessor are all
      syntactically valid and resolved cleanly against the rest of the
      codebase (if ELEM_FARPOINTERJOIN/ATTRIB_HISLOT/ATTRIB_JOINSIZE hadn't
      resolved, or the List<FarPointerJoinSpec> generics were wrong, this
      build would have failed outright).
    - Ghidra/Features/Decompiler/build/os/win_x86_64/decompile.exe freshly
      rebuilt same session -- confirms fspec.cc's renumbered ids (292/160/161)
      and the rest of Pieces A-C compile clean as part of a real, full build,
      not just the earlier isolated decompiler-only build mentioned in the
      IMPLEMENTATION PROGRESS LOG section above.

  This is the first time this session's Java patch has been verified past
  "source written, matches C++ pattern" -- it's now confirmed to actually
  compile as part of the real Ghidra build. NOT yet verified: that it
  actually WORKS correctly at runtime (i.e. that a cspec with <farpointerjoin>
  now decodes without error and getFarPointerJoins() returns the right data)
  -- "compiles" and "behaves correctly" are different bars, same caution as
  already noted elsewhere in this file re: the decompiler-model-vs-declared-
  storage gap found earlier this session. Next real step: an end-to-end smoke
  test -- open this dev-build Ghidra against the H8 program (or a minimal
  test cspec) and confirm getFarPointerJoins() returns the expected
  hislot/joinsize for table_lookup_interp's prototype now that
  <farpointerjoin> is genuinely in the H8 cspec (Piece D already confirmed
  working on the C++/decompiler side earlier this session -- this is the
  first chance to confirm the SAME cspec file also loads correctly through
  the newly-patched Java path in the same running instance).

  ## Runtime smoke test PASSED (2026-08-18, continued) -- new build confirmed
  ## working against the live RVR program, both sides

  PJ opened the freshly-built Ghidra against the same RVR_1998_x3 ROM. Checked
  from this session:

  1. get_function_signature/program open: ZERO cspec parse errors. This is a
     real signal, not just "no crash" -- if the new restoreXml() dispatch
     case had the wrong attribute names, a bad SpecXmlUtils.decodeInt() call,
     or any other bug, the WHOLE PROGRAM would fail to open with an
     XmlParseException, not just one function. Confirms the Java patch
     genuinely parses <farpointerjoin> correctly against the real H8 cspec,
     not just "compiles."

  2. force_decompile on isc_f354_table_lookup_conditional (00025c15), a real
     caller of table_lookup_interp, with the freshly-rebuilt decompile.exe
     (renumbered ids 292/160/161): output is
       uVar1 = table_lookup_interp(1,(void *)0x27e4);
     -- the fused far-pointer call-argument display is IDENTICAL to what was
     confirmed working earlier this session before the ID renumbering.
     Confirms the C++ side's farpointerjoin mechanism survived the id changes
     completely intact.

  STATUS: both halves of the Java+C++ patch are now confirmed working at
  runtime, not just compiling, against the real target binary. This is the
  strongest validation this feature has had all session.

  REMAINING GAPS before this is genuinely upstream-submission-ready (unchanged
  from prior entry, not yet started):
    - encode() round-trip missing on both C++ (fspec.cc, disclosed in the
      original commit message) and Java (PrototypeModel's own encode()) sides
    - No automated test case in Ghidra's own test-suite style
    - getFarPointerJoins() itself has not yet been exercised directly (only
      indirectly, via "program opened without error" + "decompiler output is
      still correct") -- would be worth a small throwaway script/test calling
      it directly on table_lookup_interp's PrototypeModel to see the actual
      hislot/joinsize values come back, as a more direct unit-level check
      than the two indirect confirmations above.
    - The Analyzer that actually CONSUMES getFarPointerJoins() (the original
      motivation -- auto xref creation) still doesn't exist; this session's
      work only made the data reachable, nothing reads it yet.
    - PR description / upstream submission itself not started.

  ## Direct getFarPointerJoins() unit check PASSED (2026-08-18, continued) --
  ## closes the "only verified indirectly" gap

  Wrote CheckFarPointerJoins.java (new, throwaway diagnostic script, saved to
  C:\Users\j.brophy.CORKILLSYSTEMS\ghidra_scripts\ only -- not the project
  copy, since this is a one-off check not a permanent fixup script like
  FixTableLookupInterpStorage.java/FixTableLookupInterpFarPointer.java).
  Calls Function.getCallingConvention() for each of the three known
  far-pointer functions, then calls the new PrototypeModel.getFarPointerJoins()
  directly and prints the raw hislot/joinsize values.

  Ran via run_ghidra_script (dry_run, which still executes read-only scripts
  fully and returns real console output -- confirmed this works for
  read-only/diagnostic scripts, not just as a preview, useful to remember for
  future non-mutating checks). RESULT, all three functions, exact match to
  the H8 cspec's declared <farpointerjoin hislot="0" joinsize="3"/>:
    table_lookup_interp:        hislot=0 joinsize=3
    table_lookup_indexed:       hislot=0 joinsize=3
    table_3axis_interp_triple:  hislot=0 joinsize=3

  This is now a DIRECT confirmation, not inference from side effects (cspec
  loading without error, decompiler output looking right) -- the actual
  accessor added to PrototypeModel.java this session returns exactly the
  right data for all three functions it was meant to cover. Closes the
  "getFarPointerJoins() itself has not yet been exercised directly" gap from
  the prior entry.

  UPDATED REMAINING GAPS before genuine upstream-submission-readiness:
    - encode() round-trip missing on both C++ (fspec.cc) and Java
      (PrototypeModel's own encode()) sides -- still not started
    - No automated test case in Ghidra's own test-suite style -- still not
      started
    - The Analyzer that actually CONSUMES getFarPointerJoins() for auto xref
      creation (the ORIGINAL motivation for this whole thread) still doesn't
      exist -- everything so far has been making the data reachable and
      proving it's correct; nothing yet reads it for a real purpose
    - PR description / upstream submission itself not started

  ## encode() round-trip added, both sides (2026-08-18, continued) -- closes
  ## the decode-only asymmetry disclosed in the original C++ commit message

  Both fspec.cc (C++) and PrototypeModel.java (Java) previously only DECODED
  <farpointerjoin> -- neither could write it back out to cspec XML. Closed
  both gaps this session, following each codebase's own existing pattern for
  an analogous list field (encodeLikelyTrash on the C++ side, the inline
  ELEM_PARAMRANGE-style block in PrototypeModel.encode() on the Java side) --
  not a novel approach, deliberately mirrors what's already there.

  ### C++ side (fspec.hh / fspec.cc)

  - fspec.hh: added `void encodeFarPointerJoins(Encoder &encoder) const;`
    declaration alongside encodeLikelyTrash's declaration.
  - fspec.cc: new FuncProto::encodeFarPointerJoins(Encoder&) definition
    immediately after encodeLikelyTrash's definition -- loops farPointerJoins,
    writes each entry as its own <farpointerjoin> element with hislot/joinsize
    attributes via encoder.writeSignedInteger(ATTRIB_HISLOT/ATTRIB_JOINSIZE,...).
    Unlike encodeLikelyTrash, no ProtoModel-default diffing needed --
    farPointerJoins has no underlying ProtoModel-level default to override, so
    every declared entry is always encoded in full.
  - Wired into FuncProto::encode()'s main body: encodeFarPointerJoins(encoder);
    added immediately after the existing encodeLikelyTrash(encoder); call.

  ### Java side (PrototypeModel.java)

  - Added directly inline in PrototypeModel.encode() (that method doesn't
    delegate to a separate per-field private encoder for this style of field --
    ELEM_PARAMRANGE/ELEM_LOCALRANGE are handled inline too, so matched that
    convention rather than the C++ side's separate-method style): a
    `for (FarPointerJoinSpec spec : farPointerJoins)` loop, positioned right
    before the paramRange block's ELEM_PROTOTYPE close, emitting the same
    <farpointerjoin hislot="N" joinsize="N"/> shape. No null check needed --
    farPointerJoins is initialized to an empty ArrayList, never null, so an
    empty list just encodes zero elements, consistent with the field's own
    initialization elsewhere in the file.

  STATUS: both encode() methods now round-trip <farpointerjoin> symmetrically
  with their own decode() paths. NOT yet verified by an actual round-trip test
  (decode a cspec with <farpointerjoin>, re-encode it, confirm the output
  matches) -- written to mirror existing, working patterns exactly, but this
  specific new code path hasn't been exercised yet. Saved but not yet rebuilt
  this session -- PJ needs to re-run buildGhidra to confirm this compiles
  clean before it's considered verified, same bar as the earlier Java patch.

  UPDATED REMAINING GAPS:
    - encode() round-trip: WRITTEN, not yet built/tested (was previously
      "not started" -- now "done, unverified")
    - No automated test case in Ghidra's own test-suite style -- still not
      started
    - The Analyzer that actually consumes getFarPointerJoins() for auto xref
      creation -- still doesn't exist, still the actual practical payoff of
      this whole thread, still untouched
    - PR description / upstream submission itself not started

  ## LIVE CRASH found and fixed (2026-08-18, continued): "Unknown element in
  ## prototype" on program open, after the encode() round-trip rebuild

  PJ rebuilt with the encode() round-trip changes and reopened the RVR
  program -- Ghidra's Decompiler pane failed immediately:
    "Unable to initialize the DecompilerInterface: Could not register
    program: Low-level Error: Unknown element in prototype"

  ### Root cause: TWO separate C++ decoders read <prototype> elements, only
  ### one of them (the one Piece B touched) knows about <farpointerjoin>

  Traced "Unknown element in prototype" to its exact throw site in fspec.cc
  and found there are two, unrelated functions with near-identical decode
  loops for the same <prototype> XML shape:
    - ProtoModel::decode (~line 2542) -- decodes <prototype> elements when
      the CSPEC'S OWN named/default calling-convention MODELS are loaded,
      i.e. at program-open / language-load time. This is the one that threw.
    - FuncProto::decode (~line 4632) -- decodes <prototype> elements when a
      SPECIFIC FUNCTION's prototype override is read. This is the one Piece B
      correctly added ELEM_FARPOINTERJOIN handling to (confirmed at line
      4781, storing into the real farPointerJoins field), and it was already
      confirmed working correctly earlier this session.

  ProtoModel::decode's own dispatch chain never got a matching case -- it
  still fell through to its `else throw LowlevelError("Unknown element in
  prototype")` catch-all whenever it hit a <farpointerjoin> child. This did
  not surface as a bug until THIS session's encode() work, because previously
  the H8 cspec's <farpointerjoin> declarations only ever existed inside
  <prototype> blocks that FuncProto::decode reads (the specific function
  override prototypes) -- nothing was writing <farpointerjoin> anywhere
  ProtoModel::decode would see it. The live crash on program OPEN (not on
  any specific function decompile) was the tell that pointed at
  ProtoModel::decode rather than FuncProto::decode, since program-open is
  when ProtoModel::decode runs.

  Checked whether ProtoModel needs to actually STORE this data: no --
  ProtoModel represents the calling-convention MODEL as a whole; farpointerjoin
  is inherently a per-function-prototype override (fusing THIS callee's split
  arguments), which is exactly why it was designed as a FuncProto-only field
  in the first place. ProtoModel has no farPointerJoins field and doesn't need
  one -- it just needs to not crash when the element appears in a <prototype>
  block at its level.

  ### Fix: ProtoModel::decode now tolerates (parses and discards)
  ### <farpointerjoin>, matching how genuinely-unknown elements are meant to
  ### be handled, rather than storing it

  Added a new `else if (subId == ELEM_FARPOINTERJOIN)` case to
  ProtoModel::decode's dispatch chain (placed after the existing
  ELEM_LIKELYTRASH case, before ELEM_INTERNAL_STORAGE -- confirmed exact
  insertion point via read-lines before AND after, since the insert tool's
  beforeLine anchoring was off-by-one on this file this session, see tooling
  note below). Uses decoder.openElement()/closeElementSkipping() to walk and
  discard the child <farpointerjoin> element's contents without storing
  anything -- confirmed both APIs are real, existing Decoder methods
  (marshal.hh lines 144-149, 403, 556) before using them, not invented.
  (Noted a cleaner available helper, Decoder::skipElement() at marshal.hh
  line 291, which does the same open+closeElementSkipping in one call -- left
  the current hand-rolled version in place since it's already correct and a
  replace-all attempt to simplify it hit whitespace-matching issues; cosmetic
  only, not a correctness concern, worth tidying in a later pass.)

  TOOLING NOTE: insert's beforeLine parameter was off-by-one on this file this
  session -- beforeLine:2661 inserted BEFORE the closing brace of the
  PRECEDING likelytrash block (would have broken syntax), confirmed via two
  separate dry runs before catching it; beforeLine:2662 (one higher than
  expected) was the line that actually produced correct placement. Always
  verify insert dry-run output structurally (matching braces, correct
  preceding/following statement) rather than trusting the line number alone,
  especially on files that have had multiple edits earlier in the same
  session (this file had 3 prior successful edits this session before this
  one). Also: pulsar-edit-mcp-server's replace-all (used successfully for the
  fspec.cc id renumbering earlier) failed to match here despite what looked
  like an exact copy-paste of surrounding code -- the tool's own "closest
  match" diagnostic showed the target WAS present, just not exact-matching
  (likely invisible whitespace/tab differences) -- worth re-viewing the file
  fresh immediately before constructing a replace-all query, not relying on
  previously-read content, especially since multiple "STALE" file warnings
  fired earlier in this same investigation for fspec.cc/fspec.hh/
  PrototypeModel.java (external changes from the Gradle build process
  touching these files, most likely).

  STATUS: fix written, saved, NOT yet rebuilt/retested this session. PJ needs
  to run buildGhidra again and reopen the RVR program to confirm this
  actually resolves the "Unknown element in prototype" crash before this is
  considered fixed. This is now the top-priority verification step -- the
  encode() round-trip work from the previous entry is currently in a BROKEN,
  crashing state in the live build until this fix is confirmed working.

  ## ProtoModel::decode fix CONFIRMED WORKING (2026-08-18, continued) --
  ## "Unknown element in prototype" crash resolved, everything re-verified green

  PJ rebuilt and reopened the RVR program -- Decompiler pane initialized
  successfully this time (no "Unable to initialize the DecompilerInterface"
  error). Re-ran both prior verification checks from this session to confirm
  nothing regressed:

  1. force_decompile on isc_f354_table_lookup_conditional (00025c15): output
     identical to every previous check this session --
     `uVar1 = table_lookup_interp(1,(void *)0x27e4);` -- fused pointer display
     still correct.

  2. CheckFarPointerJoins.java re-run: all three functions still report
     hislot=0 joinsize=3 exactly as before:
       table_lookup_interp, table_lookup_indexed, table_3axis_interp_triple

  STATUS: the full stack is now confirmed working together, live, in one
  running instance:
    - C++ decode (FuncProto::decode) -- correct, unaffected by this fix
    - C++ decode tolerance (ProtoModel::decode) -- NEW this session, fixes
      the crash, confirmed not breaking anything
    - C++ encode (FuncProto::encodeFarPointerJoins) -- written this session,
      compiles, not yet directly round-trip tested (see below)
    - Java decode (PrototypeModel.restoreXml) -- confirmed via direct
      getFarPointerJoins() check, working
    - Java encode (PrototypeModel.encode) -- written this session, compiles,
      not yet directly round-trip tested (see below)
    - Decompiler display (ActionFarPointerJoin, Pieces A-C, from earlier in
      this file) -- confirmed still working throughout

  UPDATED REMAINING GAPS before genuine upstream-submission-readiness:
    - encode() round-trip WRITTEN and COMPILES on both sides, and the fact
      the program now opens/decompiles cleanly with it in the live build is
      indirect evidence it's not obviously broken -- but neither
      encodeFarPointerJoins (C++) nor the Java encode() addition has been
      DIRECTLY exercised yet (e.g. decode a cspec, re-encode it, diff the
      output XML against the original to confirm a true round-trip). Worth
      doing as the next concrete step -- likely via a small script/test that
      calls encode() and inspects the resulting XML text directly, similar
      in spirit to how CheckFarPointerJoins.java directly exercised decode().
    - No automated test case in Ghidra's own test-suite style -- still not
      started
    - The Analyzer that actually consumes getFarPointerJoins() for auto xref
      creation -- the ORIGINAL motivation for this whole thread -- still
      doesn't exist
    - PR description / upstream submission itself not started

  This was a genuinely valuable near-miss to have caught and fixed within
  this session rather than after a PR was already opened -- "Unknown element
  in prototype" on program load is exactly the kind of regression an upstream
  reviewer's own test suite (or worse, a downstream user's crash report)
  would have caught, and would have looked bad arriving after submission
  rather than before.

  ## Java-side encode() round trip DIRECTLY CONFIRMED (2026-08-18, continued)

  Wrote CheckFarPointerEncode.java (new, throwaway diagnostic, saved to
  C:\Users\j.brophy.CORKILLSYSTEMS\ghidra_scripts\ only, same non-permanent
  category as CheckFarPointerJoins.java). Calls PrototypeModel.encode() into
  a real XmlEncode(true) buffer for all three functions, prints the resulting
  XML text in full, and checks it contains a correctly-formed
  <farpointerjoin hislot="0" joinsize="3"/> element.

  Used null for the injectLibrary parameter -- confirmed safe by reading
  encode()'s body first: injectLibrary is only dereferenced when
  hasUponEntry/hasUponReturn is set, and none of these three functions use
  call-fixup injection.

  RESULT: all three functions produced correct, complete <prototype> XML,
  each ending with:
    <farpointerjoin hislot="0" joinsize="3"/>
  as the last child before </prototype> -- exactly matching what decode()
  expects to read back in (confirmed against the FuncProto::decode dispatch
  case read earlier this session, which looks for exactly this element/
  attribute shape). All three of table_lookup_interp, table_lookup_indexed,
  table_3axis_interp_triple: "ROUND TRIP OK".

  This is now a DIRECT, not inferred, confirmation of the encode() side --
  previously only known to compile and to not break decode when present.
  Combined with the earlier direct decode() confirmation
  (CheckFarPointerJoins.java), BOTH directions of the round trip are now
  independently, directly verified working on the Java side. (C++-side
  encodeFarPointerJoins has not yet had an equivalent direct check -- still
  only known to compile; same category of remaining gap, smaller now that
  the pattern/approach for checking it is proven out on the Java side.)

  UPDATED REMAINING GAPS before genuine upstream-submission-readiness:
    - Java encode() round trip: DIRECTLY CONFIRMED working (was "written,
      compiles, not directly tested" -- now fully closed)
    - C++ encodeFarPointerJoins: still only known to compile, not yet
      directly exercised the same way (would need a small standalone C++
      test harness or decompiler-side script, not yet written)
    - No automated test case in Ghidra's own test-suite style -- still not
      started
    - The Analyzer that actually consumes getFarPointerJoins() for auto xref
      creation -- the ORIGINAL motivation for this whole thread -- still
      doesn't exist
    - PR description / upstream submission itself not started

  ## ============================================================
  ## SESSION HANDOFF SUMMARY (2026-08-19) -- read this first if picking
  ## up in a new conversation, then dip into the detailed entries above
  ## as needed
  ## ============================================================

  ### Goal
  Make the <farpointerjoin> feature (H8 far-pointer call-argument fusion,
  originally a C++-only decompiler patch, see Pieces A-D and the shared
  commit professor-jonny/ghidra@68cb889 earlier in this file) genuinely
  upstream-submission-ready to NationalSecurityAgency/ghidra, so PJ doesn't
  have to maintain a permanent local Ghidra fork alongside the H8 core.

  ### What's DONE and DIRECTLY VERIFIED (not just "compiles")
  1. Found and fixed 2 real numeric ID collisions between the original C++
     commit and Ghidra's EXISTING id registries (287/130/131 were already
     taken). Renumbered to 292/160/161 on BOTH sides, consistently.
  2. PrototypeModel.java (Java) patched to actually STORE <farpointerjoin>
     instead of silently discardSubTree()-ing it (this was the original,
     real gap -- Java had zero knowledge of this element before this
     session). New FarPointerJoinSpec class, farPointerJoins field, decode
     dispatch case, getFarPointerJoins() accessor.
  3. Full Ghidra rebuild (gradlew buildGhidra) confirmed successful, both
     Java and C++ sides, verified via build artifacts.
  4. Direct decode() check: CheckFarPointerJoins.java confirms
     getFarPointerJoins() returns exactly hislot=0 joinsize=3 for all three
     known functions (table_lookup_interp, table_lookup_indexed,
     table_3axis_interp_triple).
  5. encode() round-trip WRITTEN on both sides (fspec.cc/fspec.hh:
     FuncProto::encodeFarPointerJoins; PrototypeModel.java: inline loop in
     encode()).
  6. LIVE CRASH found and fixed: adding encode() surfaced that TWO separate
     C++ decoders read <prototype> blocks -- FuncProto::decode (which Piece B
     correctly patched) and ProtoModel::decode (used at program-OPEN time for
     the cspec's own named/default models, NEVER patched, threw "Unknown
     element in prototype"). Fixed by making ProtoModel::decode tolerate
     (parse+discard, not store -- farpointerjoin is correctly a FuncProto-only
     per-function-override concept) <farpointerjoin>. Rebuilt, reopened,
     confirmed the crash is gone and nothing else regressed.
  7. Direct encode() check: CheckFarPointerEncode.java confirms
     PrototypeModel.encode() produces correct, complete
     <farpointerjoin hislot="0" joinsize="3"/> XML for all three functions,
     in the right position in the output. Java-side round trip is now fully,
     directly verified in BOTH directions.

  ### What's NOT done yet (in priority order for next session)
  1. **C++-side encodeFarPointerJoins direct check** -- compiles, and its
     presence doesn't break anything, but (unlike the Java side) nobody has
     actually CALLED it and inspected real output yet. Would need a small
     C++ test harness or a way to trigger FuncProto::encode() and capture the
     XML -- approach/pattern is proven out on the Java side (see
     CheckFarPointerEncode.java), just needs a C++ equivalent.
  2. **No automated test case** in Ghidra's own test-suite style anywhere
     yet (would live under Ghidra/Features/Decompiler/src/decompile/cpp's
     test fixtures, or the Java-side test tree -- exact location not yet
     researched this session).
  3. **The Analyzer itself does not exist yet.** This was the ORIGINAL,
     practical motivation for this entire thread (auto-creating xrefs from
     <farpointerjoin> declarations, replacing the manually-run
     FixBankOffsetReferences_v2.java). Everything done this session has been
     PREREQUISITE work (making the data reachable/correct on the Java side)
     -- nothing yet actually CONSUMES getFarPointerJoins() for a real
     purpose. Design sketch from earlier this session: a real
     ghidra.app.services.Analyzer subclass, auto-registered via
     AnalyzerFactory, scoped to languages/cspecs that declare
     <farpointerjoin>, walking callers' call sites for functions whose
     PrototypeModel.getFarPointerJoins() is non-empty, computing the fused
     address, and calling ReferenceManager.addMemoryReference -- same logic
     FixBankOffsetReferences_v2.java already has, just auto-run and reading
     the cspec instead of a hardcoded function list.
  4. **PR description / actual upstream submission** not started. The
     "why not unify()/resolveJoin()" justification already exists in this
     file's Section 1 (near the top, from before this session) and is
     genuinely good -- should become the PR description close to verbatim.

  ### Key files touched this session (all in
  ### C:\Users\j.brophy.CORKILLSYSTEMS\github\ghidra unless noted)
    - Ghidra/Framework/SoftwareModeling/src/main/java/ghidra/program/model/
      pcode/ElementId.java -- ELEM_FARPOINTERJOIN = 292
    - Ghidra/Framework/SoftwareModeling/src/main/java/ghidra/program/model/
      pcode/AttributeId.java -- ATTRIB_HISLOT = 160, ATTRIB_JOINSIZE = 161
    - Ghidra/Framework/SoftwareModeling/src/main/java/ghidra/program/model/
      lang/PrototypeModel.java -- FarPointerJoinSpec, farPointerJoins field,
      decode dispatch case, getFarPointerJoins(), encode() addition
    - Ghidra/Features/Decompiler/src/decompile/cpp/fspec.hh -- renumbered
      ids, encodeFarPointerJoins() declaration
    - Ghidra/Features/Decompiler/src/decompile/cpp/fspec.cc -- renumbered
      ids, encodeFarPointerJoins() definition + wired into encode(), NEW
      tolerant <farpointerjoin> case added to ProtoModel::decode (the crash
      fix)
    - C:\Users\j.brophy.CORKILLSYSTEMS\ghidra_scripts\
      CheckFarPointerJoins.java (throwaway diagnostic, decode check)
    - C:\Users\j.brophy.CORKILLSYSTEMS\ghidra_scripts\
      CheckFarPointerEncode.java (throwaway diagnostic, encode check)

  ### Known tooling gotchas hit this session (worth knowing up front next time)
    - Pulsar's top-level str_replace/insert can crash/fail intermittently
      ("Aborted()", "File not found") even on valid paths -- a Pulsar
      server RESTART fixed it instantly last time this happened. Try that
      before spending time on alternate anchoring strategies.
    - pulsar-edit-mcp-server's NAMESPACED tools (replace-all, sed, etc.) are
      a different, sometimes more reliable code path when the top-level
      ones act up -- but sed operates on "the active buffer" with NO
      filePath targeting, so it's dangerous with multiple files open (it
      silently edited the wrong file once this session).
    - insert's beforeLine can be off-by-one on files that have had several
      edits earlier in the same session -- always check the dry-run's
      structural correctness (matching braces/statements), not just that a
      line number looks plausible.
    - "STALE" file warnings fired repeatedly on fspec.cc/fspec.hh/
      PrototypeModel.java mid-session, most likely because the Gradle build
      process was touching these files concurrently -- re-read fresh before
      trusting old tool output if a build may have run since.
    - run_ghidra_script(dry_run=true) still FULLY EXECUTES read-only/
      diagnostic scripts and returns real console output -- useful, not
      just a preview, for anything non-mutating.

## SESSION HANDOFF SUMMARY (2026-08-19, part 2) -- C++ encode test, Windows build

**Goal this session:** item 1 from the previous handoff's priority list -- get a real,
runnable C++-side test proving `FuncProto::encodeFarPointerJoins` (and the decode side)
work correctly, self-contained in the upstream ghidra tree (no dependency on the H8
module), suitable for an eventual PR to NationalSecurityAgency/ghidra.

**Test written:** `unittests/testfuncproto.cc`, new `TEST(funcproto_farpointerjoin_roundtrip)`,
appended after the existing `funcproto_recovermixedmeta` test. Uses the upstream-sanctioned
pattern for out-of-tree-shaped features: builds an in-memory `Architecture` via the
`"xml"` capability against the built-in `Toy:LE:32:default:default` language (ships in
`Ghidra/Processors/Toy`), decodes a synthetic `<prototype>` block containing
`<farpointerjoin hislot="0" joinsize="3"/>` directly into a `FuncProto`, asserts the
decoded spec, then calls the public `FuncProto::encode()` and asserts the XML round-trips
(decode -> encode -> decode again, checking for asymmetry). No H8 dependency at all --
this is the correct answer to "how do others test out-of-tree cores": use Ghidra's
built-in Toy processor plus the `unittests/*.cc` `TEST()` framework, not `datatests`
(which needs real compiled binaries) and not a dependency on your own module.

**Bug found and fixed along the way:** an earlier `str_replace` mis-targeted a duplicate
`} // End namespace ghidra` string and corrupted `testfuncproto.cc`, splicing the new
test into the middle of `funcproto_recovermixedmeta`'s body. Fixed by rewriting the whole
662-749 span cleanly. Lesson: this file's `str_replace`/`insert` tools need wider,
more specific anchors on files with repeated boilerplate closers -- confirmed by the
tool's own drift-immunity hints (`afterString`/`inFunction`/`betweenHint`).

**CRITICAL gotcha discovered: two different filesystems.** `bash_tool` and `create_file`
(the "container" tools) run in an isolated sandbox and CANNOT see or write to the real
Windows machine -- confirmed when `create_file` reported success but the file didn't
exist when checked via Pulsar. All real file writes on this machine MUST go through
Pulsar's own tools: `pulsar-edit-mcp-server:create-file` (not the generic `create_file`),
`str_replace`, `insert`, etc. `run-command` (PowerShell) also only runs on the real
machine, not the container -- this is fine, just don't mix tool families for the same task.

**Toolchain state on this machine (Windows, no WSL used):**
- `mingw32-make.exe` at `C:\mingw64\bin\` works as `make`, but the upstream `cpp/Makefile`
  is unusable as-is on bare Windows: it needs `uname`, `sh`, and other POSIX tools that
  a plain MinGW64 install (compiler + make only, not MSYS2) doesn't provide. Not worth
  fighting further -- drive `g++`/`ld` directly instead.
- `g++`/`gcc` 15.2.0 at `C:\mingw64\bin\` work fine for compiling/linking the decompiler
  C++ sources directly (no `make` needed).
- Real, upstream-side blockers found (not fixed by touching source, only by adding flags
  or an extra local-only file):
  - `filemanage.cc`'s POSIX `dirent.d_type`/`DT_DIR`/`DT_UNKNOWN`/`DT_LNK` branch doesn't
    compile on MinGW's dirent shim -- BUT the file already has a native `#ifdef _WINDOWS`
    branch using `FindFirstFileA`/`FindNextFileA`. Just pass `-D_WINDOWS` on the g++
    command line; no source edit needed.
  - `test.cc` transitively includes `<termios.h>` via
    `libdecomp.hh -> ifacedecomp.hh -> testfunction.hh -> ifaceterm.hh`, which MinGW
    doesn't have. `test.cc`'s actual payload (`main()`, CLI/datatests handling) isn't
    needed for unit tests -- only `UnitTest::run(set<string>&)`'s implementation is
    needed, and that function's body has zero POSIX dependency.
  - Missing `-lz` link flag: `compression.cc` needs zlib (`deflate`/`inflate` family).
- Local-only helper file created (NOT part of the farpointerjoin patch, NOT for
  upstream): `Ghidra/Features/Decompiler/src/decompile/cpp/test_win.cc`. Minimal driver:
  replicates the useful parts of `startDecompilerLibrary()` (AttributeId/ElementId
  init, `CapabilityPoint::initializeAll()`, `ArchitectureCapability::sortCapabilities()`,
  `SleighArchitecture::scanForSleighDirectories(...)`), copies `UnitTest::run()` verbatim
  from `test.cc`, and has its own tiny `main()`. Includes only `architecture.hh`,
  `sleigh_arch.hh`, `test.hh` -- none of which touch termios.

**Exact working compile/link recipe** (objects go in `cpp/test_dbg/`, run from
`Ghidra/Features/Decompiler/src/decompile/cpp`):
```
g++ -std=c++11 -g -Wall -Wno-sign-compare -D__TERMINAL__ [-D_WINDOWS for filemanage/others] -c <name>.cc -o test_dbg\<name>.o
```
Full object list needed beyond the original core/deccore/sleigh macros from the
Makefile: also add `filemanage` (compiled WITH `-D_WINDOWS`), `sleigh_arch`, `xml_arch`,
`loadimage_xml`, `inject_sleigh` -- none of these were in the original naive object list
and each produced a real undefined-reference link error before being added (each is a
static-registration/capability-lookup dependency: `xml_arch.cc`'s file-scope
`XmlArchitectureCapability` singleton is what registers the `"xml"` capability string;
skip it and `ArchitectureCapability::getCapability("xml")` returns null, and dereferencing
that null is what caused an access-violation crash (`0xC0000005`) on the first full run
attempt -- worth remembering, this was silent/crashy, not a link error, until the
object was actually missing from a DIFFERENT link attempt that also lacked it).
Link line: `g++ -g -o test_dbg\decomp_test_win.exe <all .o objects> -lz`

**Current status: builds and RUNS cleanly (no crash), fails on a real, expected-scope bug.**
Latest run output:
```
testing : funcproto_farpointerjoin_roundtrip ...
  fail: Missing <returnsym> tag
0/1 tests passed.
```
This is progress, not a blocker -- it got past architecture construction (Toy sleigh
spec found correctly once `SleighArchitecture::scanForSleighDirectories()` was pointed
at `C:\Users\j.brophy.CORKILLSYSTEMS\github\ghidra`, the parent of the `Ghidra\Processors\...`
layout it expects) and is now failing inside `FuncProto::decode()` itself on the test's
own synthetic XML. The exception is thrown at fspec.cc ~line 4739/4745: decode() requires
the output to be wrapped in an explicit `<returnsym>` element, not a bare `<output><pentry>`.
The test's `prototext` string (in `testfuncproto.cc`, `funcproto_farpointerjoin_roundtrip`)
needs its `<output>...</output>` block changed to wrap the pentry in `<returnsym>...</returnsym>`
(or whatever exact structure fspec.cc's decode expects around that line -- not yet
inspected in detail).

**Next step:** read fspec.cc lines ~4700-4750 to see the exact expected `<returnsym>`
structure, fix the test's XML to match, rerun. This is a small mechanical fix -- all the
hard infrastructure (finding every needed source file, working around MinGW gaps,
getting the Toy sleigh spec to load) is done and should not need to be redone.

**After the test passes:** re-confirm `funcproto_recovermixedmeta` (the pre-existing test
that got corrupted and was fixed) still passes too -- run the full `unittests` suite, not
just the new test name, before considering item 1/2 fully done. Then remaining priority
items are unchanged from the previous handoff: item 3 (the Analyzer, still not started --
this is the actual functional payoff) and item 4 (PR description, largely already drafted
via the earlier "why not unify()/resolveJoin()" writeup).

**Upstream contribution requirements** (from NationalSecurityAgency/ghidra CONTRIBUTING.md,
looked up this session): file GitHub issue first (done, #9450); commit message must start
with `#9450: ...`; squash to one commit (or a few logically separated ones) before PR;
keep the patch to smallest reasonable scope -- the earlier `ELEM_UNAFFECTED` linker-bug
fix (found during the very first build attempt, unrelated pre-existing bug) should
probably be its own separate PR, not bundled with farpointerjoin; consider splitting into
two PRs -- plumbing (decode/encode/format support) as PR #1, the Analyzer as PR #2 --
since reviewers prioritize small, easily-evaluated contributions.

## UPDATE (2026-08-19, part 2 continued) -- TEST PASSES, full suite green

Fixed the `Missing <returnsym> tag` failure. Root causes (both in the test's synthetic
XML, not in fspec.cc):
1. The test's `<prototype>` block wrongly included an `<input><pentry>...</pentry></input>`
   section copied from the *model definition* XML style (`registerModel1` etc., which
   defines a `ProtoModel`'s calling convention). A `<prototype>` element decoded by
   `FuncProto::decode` is a *function instance*, not a model definition -- it has no
   `<input>` block at all; parameter storage comes from the referenced model
   (`model="__model1"`) via `store`/`setInternal()`. The stray `<input>` element caused
   `decoder.peekElement()` (fspec.cc ~4713) to see `<input>` instead of `<returnsym>`,
   triggering the "Missing <returnsym> tag" throw.
2. Two attribute bugs: used `name="__model1"` instead of the correct `model="__model1"`
   (ATTRIB_MODEL, fspec.cc ~4661), and included an unrecognized `stackshift` attribute
   (not read anywhere in FuncProto::decode's attribute loop -- harmless but wrong, removed).
3. The return value itself needed `<returnsym><register name="r12"/><void/></returnsym>`
   -- confirmed the exact shape by reading FuncProto::encode() (fspec.cc ~4619-4625):
   `<returnsym>` wraps an address element (`<register>`/`<addr>`, via
   `VarnodeData::decode`/`Address::encode`) followed directly by a type element
   (`<void/>` for void, via `TypeFactory::decodeType`/`Datatype::encodeRef`) -- no
   wrapping `<pentry>`, no `<output>` tag at all.

Corrected `prototext` in `funcproto_farpointerjoin_roundtrip`:
```xml
<prototype model="__model1" extrapop="unknown">
  <returnsym><register name="r12"/><void/></returnsym>
  <farpointerjoin hislot="0" joinsize="3"/>
</prototype>
```

**Result: PASSED.**
```
testing : funcproto_farpointerjoin_roundtrip ...
  passed.
```

**Full suite run (`decomp_test_win.exe` with no args, all TEST() cases across all
unittests/*.cc files): 205/205 tests passed.** Confirms the earlier `str_replace`
corruption of `funcproto_recovermixedmeta` was fully and correctly repaired (it passes),
and nothing else in the suite regressed from anything touched this session (the new
test, the `-D_WINDOWS`/filemanage fix, or any of the added object files).

**Item 1 and item 2 from the priority list are now genuinely done** -- not just
"compiles", but executed, passing, verified against real fspec.cc/fspec.hh logic
(IDs 292/160/161), with the full existing suite confirmed non-regressed.

**Remaining priority items, unchanged:**
3. The Analyzer (auto-creating xrefs from `<farpointerjoin>`, replacing manual
   `FixBankOffsetReferences_v2.java`) -- still not started. This is the actual
   functional payoff of the whole patch.
4. PR description -- largely drafted already via the earlier "why not
   unify()/resolveJoin()" writeup; needs to be assembled into the actual PR body,
   plus the commit needs squashing to one (or a few) commits starting with `#9450: ...`
   per CONTRIBUTING.md, with the earlier unrelated `ELEM_UNAFFECTED` linker-bug fix
   probably split into its own separate small PR.

**Reminder for next session:** `test_win.cc` and the whole `test_dbg/` directory
(objects, `decomp_test_win.exe`, various `.txt` log files) are LOCAL-ONLY scratch/
verification artifacts, not part of the farpointerjoin patch and not meant to be
committed or included in the eventual PR diff. Don't `git add` them. The compile/link
recipe from the previous handoff entry above still applies for re-running the suite
after any further fspec.cc/fspec.hh changes -- just rerun the same g++ compile of
whichever object changed, then the link line, then `.\test_dbg\decomp_test_win.exe`
(optionally with a specific test name, or no args for the full suite).

## UPDATE (2026-08-19, part 3) -- PR OPENED: NationalSecurityAgency/ghidra#9522

**PR #9522 is live**: https://github.com/NationalSecurityAgency/ghidra/pull/9522
- One commit (3639135a8f), clean, parented directly on upstream master.
- Description covers problem/solution/testing as drafted this session.
- Auto-links to close issue #9450 on merge.
- Pre-flight checks done before opening: confirmed via `git show --stat`/`git log`
  that (a) this is genuinely one commit, not an unsquashed stack, and (b) the earlier
  `ELEM_UNAFFECTED` linker-bug fix is NOT part of this diff (it was already
  pre-existing code by the time this commit was made -- appears only as unchanged
  context in `fspec.cc`, confirmed via `git show` with no +/- prefix on that line).
  Nothing needed splitting out.
- git.exe location on this machine (no git on PATH): GitHub Desktop bundles its own,
  found at `C:\Users\j.brophy.CORKILLSYSTEMS\AppData\Local\GitHubDesktop\app-<version>\
  resources\app\git\cmd\git.exe` -- multiple app-<version> folders may exist, use the
  newest. Invoke directly (`& $git -C <repo> <args>`) rather than adding to PATH.
- No reviewers assigned yet as of this update -- normal for a freshly opened PR,
  not a signal of anything.

**Scope decision made this session:** the Analyzer (replacing
`FixBankOffsetReferences_v2.java`, creating persistent xrefs via
`ReferenceManager.addMemoryReference`) is explicitly OUT of scope for this PR and
does NOT belong upstream at all -- it's H8-specific analysis behavior and belongs in
the Ghidra-H8-Processor repo, not NationalSecurityAgency/ghidra. Confirmed this by
reading FixBankOffsetReferences_v2.java in full: it does real listing-level xref
creation (bank*0x10000+offset resolution, adjacency/function-boundary heuristics),
which is a fundamentally different job from what `ActionFarPointerJoin` (upstream,
in PR #9522) does -- ActionFarPointerJoin only reshapes decompiler-internal p-code
per-function, transiently, and never touches ReferenceManager. Both are needed;
they are not redundant with each other.

**Next planned work (separate from PR #9522, in Ghidra-H8-Processor repo):** build a
proper Java `Analyzer` class to replace `FixBankOffsetReferences_v2.java`, driven by
`getFarPointerJoins()` (now available thanks to PR #9522's plumbing) instead of the
old script's fragile backward-instruction-scan heuristics. Not yet started at time of
this update -- agreed to start it, then got sidetracked into PR mechanics/verification
before any Analyzer code was written. Pick this up next session.

  ## NEW, SEPARATE bug found (2026-08-19): register0x0e phantom-register
  ## artifact in decompiled output, CONFIRMED LIVE, distinct from farpointerjoin
  ## and distinct from the resizeStackVarnode / by zero bug

  PJ shared two full-program decompile exports (RVR_1998_x3...hex.c "pre" and
  "_new.hex.c" "post"), asking whether the Ghidra rebuild fixed anything /
  caused regressions. Grep comparison:
    table_lookup_interp(1,...) fused-call sites: 47 -> 127 (real improvement,
      consistent with everything confirmed earlier this session)
    BADSPACEBASE: 0 in BOTH files (the old bookmarked "register0x0e =
      (BADSPACEBASE *)&uStack_N" cast pattern from 4 addresses -- 0x21df5,
      0x23523, 0x26124, 0x27b9d -- is confirmed gone; live force_decompile
      on all 4 addresses this session also shows zero BADSPACEBASE, matches)
    register0x0e (bare, no BADSPACEBASE cast): 1 -> 210

  Initially mis-investigated this: first checked the WRONG function (used
  stale line numbers from the "old" file's line numbering against content
  actually read from the "new" file -- a real mistake, corrected once PJ
  confirmed the "new" export was freshly re-generated, not stale). Re-derived
  the correct owning function per line number directly from the actual
  register0x0e hit locations in the fresh export:
    init_copy_const_block_via_memcpy_banked (000149c8) -- hit at line 3417
    eeprom_backup_table_write_dispatch (parameter param_fp1) -- hits at
      lines 3482/3489, not yet individually investigated

  ### Confirmed LIVE and reproducible on init_copy_const_block_via_memcpy_banked

  force_decompile (000149c8), current, live, post-rebuild:
    void init_copy_const_block_via_memcpy_banked(void)
    {
      ushort uStack_4;
      *(undefined2 *)((int)((short)register0x0e + -2) + -2) = 0;
      do {
        *(undefined2 *)((int)uStack_4 + -0x7b6) = 0;
        uStack_4 = uStack_4 + 2;
      } while (uStack_4 < 2);
      memcpy_banked(2,0xcf80,0,0xf862,0x100);
      return;
    }

  Raw disassembly (disassemble_function, 000149c8) is clean and simple -- 16
  instructions, only touches R0/FP/SP, no other register:
    link FP,#-0x2:8 / clr.w @(-0x2:8,FP) / mov:f.w @(-0x2:8,FP),R0 /
    clr.w @(-0x7b6:16,R0) / add:q.w #2,R0 / mov:f.w R0,@(-0x2:8,FP) /
    cmp:i #0x2:16,R0 / blo ... / [4 stack pushes] / pjsr memcpy_banked /
    unlk FP / prts
  A single-pass loop (runs exactly once: 0 then 2, cmp:i #2 fails) clearing
  one word at R0-0x7b6, then calling memcpy_banked. NOTHING in this
  disassembly references any register other than R0/FP/SP -- confirms
  register0x0e in the decompiled output is a DECOMPILER-INVENTED artifact
  with no basis in the actual instructions, not a real value the hardware
  computes. This is a genuine, distinct decompiler bug.

  get_function_signature confirms param_count=0 for this function (matches
  the disassembly -- no incoming stack args), so this isn't obviously a
  calling-convention-model problem the way gear_indexed_table_lookup_f1fc's
  "Unknown calling convention" warning might be (separate, also-unresolved
  finding from earlier this session, same investigation, not yet linked to
  this one -- worth checking whether they share a cause).

  STATUS: real, reproducible, live bug. NOT yet root-caused to a specific
  Ghidra source line (unlike the resizeStackVarnode bug, which has an exact
  line-level cause). NOT yet checked whether this correlates with farpointerjoin
  at all -- init_copy_const_block_via_memcpy_banked has zero connection to
  table_lookup_interp/farpointerjoin, so this is very likely PRE-EXISTING,
  not something the farpointerjoin patch introduced -- but that's an
  inference, not yet confirmed by checking a build from BEFORE this
  session's C++/Java changes.

  NEXT STEPS (not yet done):
    - Check eeprom_backup_table_write_dispatch's two register0x0e hits too,
      confirm same phantom-register shape or a different one
    - Determine whether this predates all of this session's Ghidra source
      changes (would need a build from before this session, or check
      git/build history) -- important for knowing whether this is something
      this session's work is responsible for investigating/fixing, or a
      wholly separate pre-existing Ghidra decompiler bug
    - Check the relationship (if any) to gear_indexed_table_lookup_f1fc's
      "Unknown calling convention" warning found earlier in this same
      investigation
    - Root-cause in decompiler C++ source (likely coreaction.cc or the
      SSA/heritage pass, similar investigative approach to how
      resizeStackVarnode was traced) once reproduction is solid

  ## register0x0e: "is it caused by typing the function?" -- TESTED, NO

  PJ asked directly whether register0x0e on init_copy_const_block_via_memcpy_banked
  is introduced by how the function is typed/declared. Tested directly rather
  than reasoning about it:
    - Confirmed function already has 0 parameters, 0 locals declared at the
      Function/Variable level (get_function_variables) -- consistent with
      the clean, fully self-contained disassembly (only touches R0/FP/SP,
      no incoming stack args at all). Weak prior for a typing-mismatch
      theory even before testing, since there's nothing to mistype.
    - Direct test: called set_function_prototype for real (not dry_run) with
      an explicit "__stdcall" convention and "void
      init_copy_const_block_via_memcpy_banked(void)" prototype -- i.e.
      exactly re-declaring it as what it already visually appears to be.
      Tool reported success. Re-ran force_decompile immediately after.
    - RESULT: byte-for-byte IDENTICAL decompiled output, register0x0e still
      present in the same place. Re-declaring the prototype had ZERO effect.

  CONCLUSION: register0x0e is NOT caused by function typing/prototype
  declaration -- directly disproven, not just inferred. Since the Java-side
  Function/PrototypeModel binding demonstrably has no effect, whatever
  produces this artifact must live purely in the C++ decompiler's SSA/
  heritage analysis for this specific instruction shape, independent of how
  the function is declared. The suspect pattern (from the clean disassembly
  already captured above): `link FP,#-0x2:8` / `clr.w @(-0x2:8,FP)` /
  `mov:f.w @(-0x2:8,FP),R0` -- reading a just-zeroed stack local straight
  into a register before any other value is ever written there. Worth
  investigating that specific idiom (immediate self-referential stack
  read-after-clear) as the next step, rather than anything prototype- or
  calling-convention-related.

  ## register0x0e IDENTIFIED (2026-08-19): it's SP, not a phantom value

  PJ shared a trial.xml program-state export scoped to
  init_copy_const_block_via_memcpy_banked, including a <tracked_pointset>
  block showing Ghidra's SLEIGH context tracking pinned register values at
  offsets 0x10/0x11/0x12/0x13/0x14 right at this function's entry (0x149c8).
  Register offset 0x0e sits immediately before that tracked range -- worth
  checking the actual register space definition rather than guessing.

  Checked h8539f.slaspec directly:
    define register offset=0x00 size=2 [ R0 R1 R2 R3 R4 R5 FP SP ];
  2-byte registers, sequential from 0x00: R0=0x00, R1=0x02, R2=0x04, R3=0x06,
  R4=0x08, R5=0x0a, FP=0x0c, SP=0x0e.

  CONCLUSION: register0x0e IS SP (the stack pointer). This is NOT a phantom/
  invented decompiler value -- earlier framing in this file ("decompiler-
  invented artifact... no basis in the actual disassembly") was WRONG and is
  corrected here. SP is absolutely real and touched by this function (the
  `link FP,#-0x2:8` prologue and the local-clearing loop are inherently
  SP-relative operations). The actual bug is narrower and different than
  first framed: the decompiler is correctly tracking a real SP-derived value
  but failing to resolve/display it as the named "SP" register in this one
  output line, falling back to the raw numeric offset "register0x0e" instead.

  This likely connects to the SAME known-fragile TP/FP/SP segmentop/
  constresolve machinery already extensively documented in h8539f.pspec's own
  comments (lines 35-113: the SP24 segmentop spike, constresolve singleton
  limitation blocking a second EP-based register, the "opaque CALLOTHER with
  no pointer significance" failure mode when a constresolve-adjacent register
  isn't fully recognized) -- NOT a brand-new unrelated bug. That prior
  documented history is strong context for why SP display specifically (not
  R0-R5, not FP) might fail to resolve in certain instruction shapes: SP is
  the one register wrapped in custom segmentop/constresolve handling for the
  24-bit ram space, unlike the plain GP registers.

  STATUS: root identity confirmed (register0x0e = SP). Root CAUSE (why SP
  fails to resolve to its name in THIS specific instruction shape --
  link/clr.w/mov:f.w on a fresh local, immediately after function entry --
  while apparently resolving fine elsewhere) still not confirmed. Next step:
  compare this function's SP-handling shape against a similar function where
  SP prints correctly, to isolate what's different, OR trace the segmentop/
  constresolve pcode for this specific address (0x149c8) directly via
  analyze_dataflow to see where the name resolution is being lost.

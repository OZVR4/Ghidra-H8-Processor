# review15.md — register0x0e investigation (condensed 2026-08-21)

Original 30-section, ~1950-line working log condensed for readability.
Every confirmed fact, root cause, and API gotcha below is preserved from
the original; only step-by-step narration of dead ends and rebuild-by-
rebuild retries has been compressed. Current plan/status: review16.md.

## Background

Found while verifying the farpointerjoin patch (#9522) caused no
regressions. SP (H8 stack pointer, register offset 0x0e) sometimes prints
as raw `register0x0e` instead of its name. Confirmed real (not a phantom
decompiler value) and confirmed display-only (underlying addresses/values
are correct). Distinct from farpointerjoin and from the unrelated
VariableUtilities division-by-zero bug (review3.md).

Confirmed facts:
- SP = register offset 0x0e (h8539f.slaspec line 22, sequential register
  layout: R0..R5, FP=0x0c, SP=0x0e).
- Reproduces live on `init_copy_const_block_via_memcpy_banked` (0x149c8):
  `*(undefined2 *)((int)((short)register0x0e + -2) + -2) = 0;`
- NOT caused by function typing (tested directly, no change).
- 210 occurrences across the program; confirmed present in at least 2
  functions (see stress-test note below).
- FP (register 0x0c), which goes through the identical `spSegment(TP,...)`
  call as SP, shows **zero** occurrences — ruled out a blanket
  spSegment/constresolve gap as the cause; something SP-specific.

## Investigation (superseded theories, kept for context only)

Several theories were explored and each ruled out by direct evidence
before the real root cause was found — noted briefly since they explain
why later reasoning avoided re-treading them:
- **EP/constresolve theory**: SP not being in `<constresolve>` (only TP
  is) looked promising by analogy with an earlier EP-banking failure, but
  didn't explain why FP (same mechanism) stays clean. Superseded.
- **#817 same-bug-as-upstream theory**: register0x0e looked structurally
  similar to upstream Ghidra issue #817's `unaff_SS` symptom (open since
  2019, same general segmented-register-identity problem, x86). Genuinely
  useful context (this project's own farpointerjoin work already cites
  #817), but not the same literal bug — see Root Cause below.
- **translate.cc constructJoinAddress/getRegisterName-combined-size
  theory**: a plausible-looking candidate mechanism found via full-repo
  grep. Tested directly via a real pcode dump and disconfirmed — no
  JoinRecord/PIECE op appears anywhere near the actual affected varnode.

**Key structural finding that led to the real cause:** `link`/`unlk`'s
pcode (h8539f-branch.sinc) reassigns SP to a value computed from itself
twice within one instruction (`SP = SP - 2` ... `SP = SP + sext(s8)`),
unlike clean `@(disp,FP)` addressing which reads FP once. Every
register0x0e occurrence traced back to exactly this `SP = SP - 2` step.

## Root cause (confirmed via live pcode/HighVariable trace, not inference)

The SSA-versioned Varnode landing in `register:0xe` storage after `link`'s
`SP = SP - 2` step is real, correctly defined (its def-chain is fully
intact — confirmed via `Varnode.getDef()` walks and later via a live
`CPUI_SEGMENTOP`-input dump), but it never receives a Symbol, so it prints
as the raw register offset. Full confirmed chain, each link source-verified:

1. `link`'s `INT_ADD` (`SP = SP - 2`) produces a new Varnode at
   `register:0xe`. Its own first input may be an intermediate `unique`
   temporary rather than a direct register read — an early implementation
   attempt assumed otherwise and this caused a real bug (see Implementation
   History).
2. `Funcdata::assignHigh()` DOES create a `HighVariable` for it
   unconditionally, once high-level analysis is on — this is not the gap.
3. `HighVariable::HighVariable()` (variable.cc:220) only attaches a Symbol
   if `vn->getSymbolEntry()` is already non-null at construction time — for
   a bare register-space write with no declared local/parameter, it's
   null, so no Symbol is attached, ever, by this path.
4. **Print-time fallback, the literal source of the string:**
   `PrintC::pushAnnotation()` (printc.cc:1923) queries the symbol scope by
   address (`symScope->queryContainer(vn->getAddr(), size, op->getAddr())`
   — NOT via `vn->getHigh()->getSymbol()`). No match exists, falls through
   to `getRegisterName()` (also empty at this size), and constructs the
   literal `"register" + hex(offset)` string (printc.cc ~1952-1961).
   `pushAnnotation()` is reached because the varnode is flagged
   `isAnnotation()` — confirmed a distinct flag from `isImplied()`
   (varnode.hh:236 vs 237) — via `PrintLanguage::pushVnExplicit()`
   (printlanguage.cc:218), pushAnnotation's only call site.

Contrast: R0 (a loop counter) stays named throughout, because the
stack-based local-variable-recovery pass registers a real address-mapped
entry for it — a mapping that never gets created for a bare mid-expression
register write like SP's here.

**Scale confirmed:** `eeprom_backup_table_write_dispatch` has **28
occurrences across 27 distinct SEGMENTOP sites** (not the "2 hits" an
earlier coarse grep suggested) — same mechanism throughout, confirmed via
live trace. Other register-space varnodes in the same function (loop
counters, etc.) resolve correctly, confirming the bug is narrowly scoped
to this specific self-referential-arithmetic-into-SEGMENTOP-input shape,
not register-space varnodes generally. This function is the real stress
test for any fix (many more sites, more varied control flow) — and is
separately documented elsewhere in this project as likely vestigial
(no EEPROM on this ECU), which doesn't affect its validity as a test case.

## Design decision: declared-Symbol fix rejected in favor of print-layer fix

A first implementation (`ActionSegmentRegisterIdentity`, a new Action
calling `Funcdata::linkSymbol()`/`linkSymbolAtUsepoint()` to attach a real
Symbol to the affected varnode) was built, iterated on extensively, and
ultimately got as far as **fully eliminating `register0x0e` from the
output** — but produced Symbols that never resolved to a real display
name (`$$undef00000001`-style raw placeholders instead of `sVar1`).

Root cause of that gap: `ActionNameVars` (the pass that would normally
assign a real name to a blank-named Symbol) deliberately skips any
Varnode where `isImplied()` is true (`HighVariable::hasName()`,
variable.cc:731) — and the affected varnode genuinely is an "implied"
(compiler-generated intermediate) value by the decompiler's own
definition. This is not a bug or an ordering gap; declaring a local
Symbol for an implied value is fundamentally fighting a deliberate
decompiler invariant.

**Decision (2026-08-21): pursue a print-layer expression-form fix
instead** — intercept in `PrintLanguage::pushVnExplicit()` before
`pushAnnotation()` is reached, and print the value as a normal inline
expression (`SP - 2`) using the base register's own existing name,
without ever declaring a new Symbol. This is confined to printc.cc,
doesn't fight `isImplied()`, and matches how the decompiler already
treats every other implied value. Full implementation plan: **review16.md**.

The `ActionSegmentRegisterIdentity` Action (coreaction.cc/.hh) remains
compiled and functionally working at the Symbol-table level, but is
expected to be **removed** once the print-layer fix is verified working —
two overlapping mechanisms for one symptom is not a good state to submit
upstream.

## Implementation history — bugs found and fixed (for future reference)

The first (declared-Symbol) implementation went through ~11 rebuild/test
cycles. Listed here because each uncovered a real, reusable lesson about
this codebase — not for the narrative:

1. **Wrong pcode-shape assumption**: detection logic required the
   defining `INT_ADD`'s own first input to be the same register as the
   output. Real H8 lowering routes through a `unique` temporary first.
   Fixed by checking only the output's own shape, not its input's.
2. **IPTR_PROCESSOR guard bug**: added a space-type guard using a
   diagnostic script's "spaceType" reading — which was Java's
   `AddressSpace` type constant, not the C++ `IPTR_*` enum (two unrelated
   type systems). Removed; wasn't load-bearing anyway.
3. **`data.beginOp(CPUI_SEGMENTOP)` silently empty**: `PcodeOpBank::begin
   (OpCode)`/`end(OpCode)` only special-cases `CPUI_STORE`, `CPUI_LOAD`,
   `CPUI_RETURN`, `CPUI_CALLOTHER` — every other opcode, including
   `CPUI_SEGMENTOP`, silently returns an empty range (`default: break`).
   This caused 4 consecutive false-negative test runs before being found
   by reading `op.cc` directly. Fixed by iterating `beginOpAlive()`/
   `endOpAlive()` (all ops, unconditionally) with an explicit opcode
   filter.
4. **Fatal `getHigh()` crash**: `Varnode::getHigh()` throws
   `LowlevelError("Requesting non-existent high-level")` if called before
   `HighVariable`s are assigned. The Action ran inside `actmainloop`
   (`rule_repeatapply`), which completes entirely *before*
   `ActionAssignHigh` (a separate, later top-level `"merge"`-group Action)
   ever runs — so `isHighOn()` was never true there. Real fix: relocated
   the Action's registration to run on top-level `act`, directly after
   `ActionAssignHigh`, matching the placement this project's own
   `ActionFarPointerJoin` already uses (not inside `actmainloop`).
5. **`pushAnnotation`'s usepoint-bound lookup vs `linkSymbol`'s looser
   borrow**: `linkSymbol()` can borrow an existing overlapping Symbol
   regardless of whether that entry's own usepoint covers the consuming
   op's address; `pushAnnotation()`'s lookup is strictly bound to the
   consuming op's own address. Fixed via a new `Funcdata::
   linkSymbolAtUsepoint(vn, explicitUsepoint)`, passing the SEGMENTOP's
   own address explicitly.
6. **Second, distinct unnamed Varnode**: the SEGMENTOP input and the
   varnode feeding an upstream CAST turned out to be two separate Varnode
   instances, only one of which the Action originally reached. Added a
   bounded backward walk (`findUpstreamRawRegisterRead`, 4-hop max through
   CAST/COPY) to find and link the second one too.
7. **Name collision**: linking both varnodes independently sometimes gave
   them the identical auto-generated name (`sVar1 = (short)sVar1 + -2;`),
   because Ghidra's default-name generation isn't safe against two
   blank-named Symbols created back-to-back without seeing each other.
   Fixed with an explicit post-link uniqueness check
   (`makeNameUnique`/`renameSymbol`) inside the Action itself.
8. **`$$undef` placeholder never resolving** — see Design Decision above;
   this is what led to abandoning the declared-Symbol approach.

## Tooling lessons (apply to any future large-file editing in this repo)

- **On coreaction.cc (6000+ lines) specifically**, the Pulsar
  `str_replace`/`insert` tools repeatedly matched the wrong location or
  silently duplicated content when given an anchor that wasn't freshly
  re-verified unique against current file content. This happened multiple
  times and required full `git revert` recovery twice. **Established
  practice**: for edits to this file, use `run-command`/PowerShell with an
  explicit pre-write uniqueness check
  (`([regex]::Matches($content,[regex]::Escape($anchor))).Count -eq 1`)
  and a disk-level verification read immediately after every write — never
  trust a tool's own "success" report, and never use a whole-file
  `-replace` (even for a seemingly-trivial substitution) since short
  substrings collide with unrelated real code (`endl` inside `endLoc` is a
  real example that corrupted ~2 dozen call sites in one pass).
- **`restore-checkpoint` only resets the in-editor buffer, not the file on
  disk.** After a checkpoint restore, `read-lines` reported clean content
  while the actual file on disk still had corruption. Git revert is the
  only real safety net for this file; checkpoints are a convenience for
  quick rollback within a session, not a substitute.
- **A `str_replace`/`insert` "not found"/failed dry-run result is not
  proof nothing was written** — one incident saw a dry-run report no match
  on a call whose near-identical follow-up had already landed corrupted
  content. Always re-grep the specific changed text on disk after any
  edit to this file, regardless of what the tool reported.
- **Compile-check `.o` cleanup must happen in the same turn as the
  compile check**, not "later" — leftover `.o`/`.exe` files inside the
  tracked `Decompiler/src/decompile/cpp/` directory cause `buildGhidra`'s
  `:Decompiler:ip` task to fail outright (it can't attribute IP for
  license-header-less binary objects). Happened twice.
- **`run_script_inline`'s generated wrapper can corrupt itself** (a
  double-wrapping bug), and since Ghidra compiles the whole
  `ghidra_scripts/` directory as one OSGi bundle, one broken file there
  blocks every script from loading — including freshly-written, valid
  ones. Prefer writing `.java` diagnostic scripts directly via
  `run-command`/`Set-Content` with a disk-verified read-back, same
  discipline as the C++ source files.
- **Verify a "new build is live" claim directly**, not by trusting a
  timestamp alone — several iterations were nearly derailed by binary/
  hash confusion across 3 separate Ghidra installs on the same machine.
  The reliable check: read the binary's actual bytes and search for a
  known string unique to the latest change (e.g. a new warning-message
  literal), not just file timestamps or SHA256 hashes (which can
  legitimately match across installs after a normal unzip-over-old-build
  update).
- **A compiler error is a better signal than manual source reading** for
  catching an assumed-not-verified API (e.g. `Varnode::isRegister()`,
  which doesn't exist) — compile early and often, even for exploratory
  C++ changes on unfamiliar internal APIs.

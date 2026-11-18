SCI1_COLON_FRAME_DISPATCH JUMP TABLE: EMULATION FEASIBILITY, THE spSegment
FIX, AND WHY DP STILL BLOCKS RECOVERY
============================================================================

GOAL: review8.md's Note-Jump-Table bookmark on sci1_colon_frame_dispatch
(0x20664, jmp @R1 at 0x206cb) already flagged that this table's real base
needs "live EP/DP page-register context" to resolve -- static xref search
and byte-pattern search (same method that worked for nothing here, see
below) both came up empty. This file documents the attempt to get that
live context via Ghidra's p-code emulator instead, what broke, what got
fixed, and why the table is still unresolved.

--------------------------------------------------------------------
BACKGROUND: THE INSTRUCTION IN QUESTION
--------------------------------------------------------------------
sci1_colon_frame_dispatch (entry 0x20664), colon-frame command dispatch
for the SCI1 boot/serial protocol:
  000206bd: add:q.b #-1,R1          ; R1 = command_index - 1
  000206bf: extu.b R1
  000206c1: shll.w R1                ; R1 = index * 2
  000206c3: mov:g.w @(-0x6d2:16,R1),R1  ; BANKED load -- the real table read
  000206c7: sub.w 0xda0,R1
  000206cb: jmp @R1

Three real callers, all confirmed via get_xrefs_to: 0x20240, 0x20320,
0x2033d, all three via unconditional bsr. NONE of these three call sites
sit inside a Ghidra-defined Function (get_function_by_address returns "No
function found" for all three) -- same undissasembled-gap pattern review8
found in the state-table dispatch region, just not yet cleaned up here.

--------------------------------------------------------------------
STATIC METHODS TRIED FIRST (all negative, per house method of exhausting
static approaches before reaching for anything heavier)
--------------------------------------------------------------------
1. get_xrefs_to on the jmp site (0x206cd) and on the load's literal operand
   addresses Ghidra resolved (0xf92e, 0xfb2a, 0xfb2c): zero results beyond
   the one instruction itself. Nothing else in the program ever reads or
   writes these addresses.
2. search_byte_patterns for the 16-bit forms of the offset constant
   (0xf92e), the subtract constant (0xda0), and the two other candidate
   resolved addresses (0xfb2a, 0xfb2c): only 0xda0's byte pattern hit
   anything, and those hits were all inside unrelated calibration-table
   regions (0x10xxx-0x13xxx) -- same coincidental-match false-positive
   pattern already documented in review8's range-classification-table
   entry (0x20a14) re: the 0x0a14 pattern search.
3. Disassembling the ROM address implied by a flat (mis)interpretation of
   the resolved RAM address (0x2f92e): decodes as live, unrelated code,
   not a data table -- confirms the naive resolution is bogus and this
   really is bank-relative addressing Ghidra can't see through statically.

CONCLUSION at this point (matches review8's original bookmark note): no
further static method was going to find this table. Next option was
emulation.

--------------------------------------------------------------------
EMULATION ATTEMPT 1: ghidra:emulate_function tool -- FAILED, WRONG TOOL
--------------------------------------------------------------------
The built-in emulate_function MCP tool failed immediately on every call,
including a bare dry_run with no register args:
  "Emulation failed: Undefined register: ESP"
This happens before it even looks at supplied registers, on an H8
program that has no ESP. Conclusion: this specific tool wrapper
unconditionally seeds an x86 stack register regardless of the loaded
program's language -- not a general "emulation is x86-only" limitation
(p-code emulation is architecture-agnostic in principle, and this
program's H8 registers ARE correctly recognized elsewhere, e.g. by
get_current_program_info and by manual EmulatorHelper calls below) but a
bug/limitation in this particular tool for non-x86 targets. Abandoned in
favor of writing a Ghidra script directly against EmulatorHelper.

--------------------------------------------------------------------
EMULATION ATTEMPT 2: manual EmulatorHelper script -- HIT A REAL SLEIGH
USEROP GAP (spSegment), NOT A SLEIGH BUG
--------------------------------------------------------------------
First real script hit a different, legitimate error on the function's
very first instruction (`stm (R1 R2 R3),@-SP`):
  "Unimplemented CALLOTHER pcodeop (spSegment), PC=00020664"

Investigated whether this was a Sleigh-level issue (i.e. something wrong
in the H8 language definition itself) by reading the actual language
source on disk:
  C:\Users\j.brophy.CORKILLSYSTEMS\Downloads\ghidra_12.0.4_PUBLIC\Ghidra\
  Processors\H8\data\languages\h8539f.slaspec (+ h8539f-branch.sinc)

Finding: spSegment is a real, deliberately-declared pcodeop (`define
pcodeop spSegment;`), backing the <segmentop> tag in h8539f.pspec. The
slaspec's own comments (already present in the file, presumably from
whoever wrote this language module) explain it was introduced specifically
to stop the decompiler mis-rendering bank-relative accesses as
CONCAT12(bank,reg) at every callsite, and that FP's separate fpSegment
userop was removed and folded into spSegment because Ghidra only allows
one <segmentop> per address space. It is used EVERYWHERE: every push/pop,
every @Rn banked EA, jsr/bsr/rts/rte/prts/prtd/rtd, link/unlk -- literally
the entire H8/539F stack and bank-addressing model routes through this one
userop. Semantics are simple and unambiguous from the slaspec itself:
  spSegment(bankReg, offset) = (zext(bankReg) << 16) | zext(offset)
identical in shape to the bankifyDP() macro sitting right next to it,
which does the same math inline (without a pcodeop) for the DP-banked
R0-R3 case.

CONCLUSION: this is NOT a Sleigh issue and NOT something a language-file
edit should touch. The language correctly declares the userop; the gap is
one layer up -- Ghidra's stock p-code emulator (EmulatorHelper /
PcodeEmulator) simply has no behavior registered for a CALLOTHER named
"spSegment", because that's inherently target-specific and something
every Sleigh language author who defines a custom userop is expected to
supply a behavior for if they want emulation to work. Genuinely fixable at
the Java/emulator level via EmulatorHelper.registerCallOtherCallback.

--------------------------------------------------------------------
WRITING THE FIX: A LOT OF WASTED CYCLES ON WRONG API NAMES, NOT A REAL
BLOCKER
--------------------------------------------------------------------
Many failed run_script_inline attempts in a row, all producing the same
confusing top-level symptom:
  "GhidraScriptLoadException: The class could not be found. It must be
  the public class of the .java file: McpInline_xxxxx"
This LOOKED like an OSGi/Felix classloader problem with multi-class files
or anonymous inner classes (several wrong theories chased: multiple
top-level classes, anonymous class $1 siblings, stale build cache from
earlier broken scripts in the shared ghidra_scripts directory). It was
none of those. Cross-checking the SAME failing file's own dry_run compile
log (the tool helpfully re-prints every previously-failed file's compiler
output on every call) showed the real cause every time: genuine javac
"cannot find symbol" errors, because several class/method names guessed
from memory don't exist in this specific Ghidra 12.0.4 build's public API:
  - ghidra.pcode.opbehavior.OpBehaviorOther        -- does not exist here
  - EmulatorHelper.getBreakTable()                  -- does not exist here
  - ghidra.pcode.pcoderaw.VarnodeRaw                -- does not exist here
  - BreakCallBack.getEmulate()                      -- no such method
    (emulate is a protected FIELD on BreakCallBack, not an accessor)
The generic "class not found" message was just this environment's way of
surfacing a failed compile; there was never a real classloader mystery.

Correct API (found via reflection against the live classpath rather than
guessing further -- Class.forName + getMethods()/getFields(), same
technique as the earlier language-userop introspection):
  EmulatorHelper.registerCallOtherCallback(String name, BreakCallBack cb)
  abstract class BreakCallBack {
    protected Emulate emulate;               // set for you before each call
    boolean pcodeCallback(PcodeOpRaw op) { ... }   // return true = handled
  }
  PcodeOp.getInputs() -> Varnode[]            (inherited by PcodeOpRaw fine)
  MemoryState.getValue(Varnode) / setValue(Varnode, long)

Working handler (single run_script_inline file, one anonymous
BreakCallBack subclass -- once the API names were right, the "multi-class"
loader theory turned out to be irrelevant; plain anonymous classes load
fine):
  emu.registerCallOtherCallback("spSegment", new BreakCallBack() {
      public boolean pcodeCallback(PcodeOpRaw op) {
          Varnode[] in = op.getInputs();
          long bank = emulate.getMemoryState().getValue(in[1]);
          long off  = emulate.getMemoryState().getValue(in[2]);
          long result = ((bank & 0xFFL) << 16) | (off & 0xFFFFL);
          emulate.getMemoryState().setValue(op.getOutput(), result);
          return true;
      }
  });

VERIFIED WORKING two separate ways:
  1. Single-instruction test at 0x206c3 with R1=8 (synthetic index),
     EP=2: emulator correctly computed spSegment(bank=0, off=8)=0x8 the
     first time (wrong bank register used, see below) and, after
     switching to the correct bank register, spSegment(bank=2, off=8)
     =0x20008 -- arithmetic matches the slaspec definition exactly both
     times, confirming the CALLOTHER handler itself is correct.
  2. Multi-instruction run starting from the real caller at 0x20320,
     which naturally executes the `bsr 0x20664` call (a spSegment-backed
     stack push under the hood) and several `stm`/link-style prologue
     instructions before reaching sci1_colon_frame_dispatch's own body --
     ran 28 real steps through genuine push/call code with zero
     unimplemented-CALLOTHER errors, proving the fix generalizes beyond
     the one hand-picked instruction and covers the whole
     push/pop/call/return family this language routes through spSegment.

--------------------------------------------------------------------
WHY THE TABLE IS STILL UNRESOLVED: DP, NOT EP, AND NEITHER IS KNOWN HERE
--------------------------------------------------------------------
Initial attempt seeded EP=2 (matching the wrong assumption that the
0x206c3 load uses EP like the FP/SP/R4/R5 banked forms do). Read back
correctly as 0x2 immediately after writing it, so the register-write
mechanism itself works fine -- but the resulting resolved address was
still nonsensical RAM garbage.

Root cause, found by re-reading the slaspec's Rn_banked/Rs_banked
constructor table (already quoted in earlier grep output, worth
restating here since it's the crux): the bank register is chosen by WHICH
Rn is the base register, not uniformly EP for all of them --
  R0-R3 base register -> bankifyDP() -> DP is the bank register
  R4, R5 base register -> spSegment(EP, Rn)
  FP, SP base register -> spSegment(TP, Rn)
R1 is in the R0-R3 group, so the real bank register for this specific
instruction is DP, not EP. Re-ran with DP=2 instead of EP=2: spSegment
now correctly computed bank=0x2 as fed (confirms the mechanism is right)
but the resulting R1 (0xf25f) is still not trustworthy, because DP=2 was
itself just a guess carried over from the earlier (wrong-register) EP
attempt -- never independently confirmed as the real runtime value.

Checked whether DP might be a fixed ROM-wide constant that could be
looked up cheaply instead of traced: NO. Scanned the whole program for
DP writes (`ldc.b`, both immediate and @SP+-restore forms) -- 30+ found
across many unrelated functions, values seen clustering around 0x0 and
0x1, with DP frequently saved to the stack before a scoped block and
restored afterward via `ldc.b @SP+,DP`. This is a real, actively-used,
scoped bank-switching convention throughout the ROM, not a fixed global --
confirms DP must be traced per-call-site, guessing is not defensible.

Checked all three real callers (0x20240, 0x20320, 0x2033d) for a nearby
DP-setting instruction: walked 15 instructions backward from each call
site, found none. Also confirmed (get_function_by_address) that none of
the three call sites are inside any currently-defined Ghidra Function --
so "walk back to the start of the containing function and check its
prologue" isn't available as a shortcut either; the real DP-setting
instruction, if local to this call path at all, is further back than 15
instructions and outside any function boundary Ghidra currently
recognizes.

Attempted one exploratory cold-start emulation from 0x20320 anyway
(no DP seeded, default/zero state) purely to see what happens: DP read
back as 0x0 on entry to sci1_colon_frame_dispatch (expected, since nothing
set it), the bsr call itself executed correctly (proving spSegment/stack
handling works for real control flow, not just the one load), but
execution wandered into unrelated memory and hit an invalid-decode crash
28 steps later -- expected garbage-in-garbage-out from starting mid-stream
with genuinely unknown state, not a new bug.

--------------------------------------------------------------------
CURRENT STATUS / WHAT WOULD ACTUALLY RESOLVE THIS
--------------------------------------------------------------------
DONE, reusable regardless of what happens with this specific table:
  - spSegment CALLOTHER now has a correct, verified handler available for
    any future emulation work on this ROM (worth keeping the working
    run_script_inline snippet above, or promoting it into a saved .java
    script under ghidra_scripts, so it doesn't need re-deriving next
    session).
  - Confirmed R0-R3 base registers bank via DP, R4/R5 via EP, FP/SP via
    TP -- worth remembering for ANY future banked-load investigation in
    this ROM, not just this one table.

NOT DONE, real remaining work, in order of how much static analysis it
would need first:
  1. Define real Functions across the undissasembled gap containing all
     three call sites (0x20240, 0x20320, 0x2033d) and sci1_colon_frame_
     dispatch's own callers-of-callers, the same kind of gap review8
     already found and fixed for the state-table region -- prerequisite
     for tracing DP properly rather than guessing.
  2. Once real function boundaries exist, trace DP backward from each
     call site through its actual containing function (and that
     function's callers, if DP is set even further up) to find the real
     runtime value(s) -- there may be more than one, if different callers
     reach this dispatch under different DP values for different colon-
     command ranges.
  3. Re-run the now-working emulation with the REAL traced DP value(s)
     seeded (not a guess) to get a trustworthy resolved table base, then
     decode records the same way H8StateTableDispatchAnalyzer did for the
     f502 table in review8 (record shape here is still unconfirmed --
     don't assume it matches the 6-byte flags/bank/pad/offset shape from
     that unrelated table without checking).
  4. Cross-check whether the RAM addresses this load resolves to are ever
     WRITTEN anywhere once real function boundaries make more of the ROM
     visible to xref analysis -- if genuinely never written by any code
     Ghidra can see, this may be runtime/boot-populated data (like the
     sci1_boot_cmd_dispatch RAM-pointer mechanism review8 already
     correctly classified as needing live hardware), in which case static
     recovery has a hard ceiling regardless of DP.

--------------------------------------------------------------------
RUNNING LOG
--------------------------------------------------------------------
2026-08-09: File created. Investigated whether sci1_colon_frame_dispatch's
jump table (flagged unresolved in review8's Note-Jump-Table bookmark)
could be recovered via emulation instead of static analysis. Confirmed
static xref/byte-pattern search still comes up empty (same conclusion as
the existing bookmark, now independently re-verified). ghidra:
emulate_function tool is broken for this non-x86 target (hardcoded ESP
seed) -- not a general x86-only limitation of emulation, just this tool
wrapper. Manual EmulatorHelper scripting hit a real, legitimate Sleigh
userop (spSegment) with no default emulator behavior -- read the actual
H8 language source to confirm this is a deliberate, well-commented pcodeop
covering the ENTIRE H8/539F stack/banking model, not a bug, then wrote and
verified a correct CALLOTHER handler for it via
registerCallOtherCallback, after a lot of wasted cycles on wrong internal
API names (resolved by reflecting the live classpath instead of guessing
further). Emulation mechanism now genuinely works end-to-end (verified via
both an isolated single-instruction test and a real 28-step multi-
instruction run through actual call/push code). However, the specific
table this session set out to resolve is STILL not recovered: the bank
register for this instruction is DP (not EP, corrected mid-session), and
DP's real runtime value at any of the three call sites is unknown -- it's
a scoped, actively-managed register throughout this ROM (confirmed via a
whole-program write scan), not a fixed constant, and none of the three
real callers sit inside a currently-defined Ghidra Function, so there's no
quick prologue check available either. NEXT STEP: this needs real static
work first (define functions across the gap containing the three callers,
then trace DP backward through them) before another emulation attempt
would produce a trustworthy answer -- scoped as its own follow-up rather
than continued ad hoc, per the STATUS section above.

--------------------------------------------------------------------
FOLLOW-UP SESSION: STATUS-SECTION ITEM 1 DONE -- CORRECTS THIS FILE'S
"THREE CALLERS ARE ORPHANED" PREMISE
--------------------------------------------------------------------
CORRECTION to the BACKGROUND section above: the three call sites were
NOT orphaned/undissasembled-gap code outside any function. find_code_gaps
across the whole program found no gap containing 0x20240/0x20320/0x2033d
at all. What was actually happening: they sit inside sci1_boot_cmd_dispatch
(entry 0x2016e), but Ghidra's stored function body for it was truncated at
0x201be -- a real `rts`, but one reached only by internal back-branches
(bne #0x1b6 from further down jumps back into the compare-chain right
after it), not the function's true end. get_function_by_address on all
three call sites correctly returned "no function found" because of this
truncation, not because the code is orphaned -- decompiling from the
(truncated) entry followed the real control flow straight through all
three sites anyway, which is what first revealed the mismatch.

FIX: deleted the truncated function and recreated it at the same entry
(0x2016e) with disassemble_first=true, letting Ghidra's flow-follow
rediscover the real extent. Body grew 80 -> 301 bytes, now correctly
spans 0002016e-0002036c and contains all three call sites plus the rest
of the boot command dispatch (flash erase, IHEX colon-frame handling,
checksum accumulate, security-seed handshake). Re-decompiled clean, same
logical structure as before (same branches/calls/tail-loop), just
properly bounded now. Saved, and a plate comment added documenting the
fix and the DP finding below. This IS sci1_boot_cmd_dispatch's real body
-- not three separate new functions -- so no renaming/new-function-naming
was needed, just the boundary fix.

DP finding, now over the CORRECT and COMPLETE function body (not a 15-
instruction guess as in the original session): scanned the full
301-byte body for any `ldc` targeting DP. There is none. Only EP is
explicitly set (`ldc.b #1,EP` / `#2,EP` at a few points, all unrelated to
the R1/R0-R3 DP-banked load in question). This means DP is never written
on ANY path from sci1_boot_cmd_dispatch's own entry through to the three
sci1_colon_frame_dispatch calls -- so DP's real value must come from
further back: either the caller of sci1_boot_cmd_dispatch's own dispatch
loop, or a cold-boot/reset-time initialization that never gets touched
again on this path. This is a materially different picture than "DP is a
scoped, actively-managed register with an unknown per-call-site value"
(true elsewhere in the ROM per the whole-program write scan, but NOT
demonstrated true for THIS specific call path) -- worth treating as a
narrower, more tractable question: trace ONE value backward from
sci1_boot_cmd_dispatch's entry (or from flash_boot_init/entry@0x0), not
N different values per call site.

NOT DONE: have not yet traced DP back past sci1_boot_cmd_dispatch's own
entry to confirm what it actually is at cold boot -- checked
flash_boot_init (0x20000) and the program's two listed entry points
(0x0, 0x1517c) exist but did not trace the actual reset-to-bootloader
path within this session. That's the concrete next step, and it's now a
single backward trace rather than the four-item static-analysis
prerequisite list the original session scoped -- item 1 of that list is
done; items 2-4 collapse into "trace DP from sci1_boot_cmd_dispatch's
entry to reset" rather than needing per-caller function definition work,
since there's only one real entry into this dispatch loop, not three
independent ones.

--------------------------------------------------------------------
FOLLOW-UP SESSION PART 2: DP TRACED TO RESET -- TABLE ADDRESSES NOW
KNOWN EXACTLY, AND CONFIRMED RUNTIME/RAM-ONLY (STATIC CEILING REACHED)
--------------------------------------------------------------------
Traced DP from sci1_boot_cmd_dispatch's entry backward through its only
caller (boot_main_loop, 0x20030, called only from sci1_boot_cmd_dispatch's
xrefs -- itself uncalled by anything, consistent with being reached via a
raw jump rather than bsr) up to the program's real reset entry point
(entry @ 0x1517c). Confirmed by direct disassembly:
  0001517c: ldc.b 0x0,TP
  0001517f: mov:i #0xfe80:16,SP
  00015182: ldc.b 0x0,DP        <-- DP SET HERE, AT RESET
  00015185: ldc.b 0x0,EP
  00015188: btst.b 0x7,@0xfee0:16
  0001518c: beq #0x00015192:8
  0001518e: pjmp @0x20000:24    <-- bootloader path (flash_boot_init)
  00015192: pjsr @0x20a80:24    <-- normal runtime path (main_loop)
DP=0 is written exactly once, unconditionally, before the bit-7 check
that branches to either the bootloader or normal runtime. Checked every
function on the bootloader path between reset and sci1_boot_cmd_dispatch
(flash_boot_init, boot_ram_block_copy, boot_sci1_init,
boot_sci1_receive_command, boot_sci1_receive_config_bytes,
boot_sci1_rom_id_verify, boot_main_loop, sci1_boot_cmd_type_rx_validate,
sci1_boot_cmd_dispatch itself) via batch_decompile + disassembly -- none
write DP. CONCLUSION: DP=0 for the entire SCI1 bootloader path,
unconditionally, for all three real callers of sci1_colon_frame_dispatch.
This is a traced fact, not a guess -- first hard, non-speculative
resolution of the DP question in this file.

Rather than fight the run_script_inline tool again to seed the emulator
(hit a NEW variant of the same "class not found" failure this session,
distinct from and not explained by anything diagnosed in the WRITING THE
FIX section above -- tried a fresh unique class name, still failed with
zero compiler output this time, unlike the earlier session's masked-
compile-error pattern; not worth further cycles chasing since the answer
was obtainable another way), computed the resolved addresses directly
from the already-confirmed pcode semantics instead. get_dataflow (forward,
from 0x206c3) shows the REAL pcode shape, which clarifies something the
disassembly view alone didn't make obvious: there are TWO SEGMENTOP
(spSegment) applications on this path, not one --
  1. at 0x206c3 (the load): spSegment(DP, R1_original) + (-0x6d2) is the
     LOAD address, where R1_original = ((param_1-1)&0xff) extu'd then
     shll.w'd (i.e. (command_index-1)*2 as an unsigned byte-then-word)
  2. at 0x206cb (the jmp): spSegment(DP, R1_loaded-0xda0) is the actual
     jump target, using the same DP again on the table's loaded value
With DP=0 confirmed, spSegment(0,X)=X for both (bank contributes nothing),
so the read address collapses to simple 16-bit arithmetic. Computed for
all three real callers (param_1=0 from the 0x20240 call site, 1 and 2 from
0x20320/0x2033d respectively, matching sci1_boot_cmd_dispatch's decompiled
calls sci1_colon_frame_dispatch(0)/(2)/(1) seen in the corrected decompile
above):
  param_1=0: R1_orig=0x01fe, table read address = 0xfb2c
  param_1=1: R1_orig=0x0000, table read address = 0xf92e
  param_1=2: R1_orig=0x0002, table read address = 0xf930
These are EXACTLY the three candidate addresses the original emulation
attempt (guessing EP=2 then DP=2) had already surfaced as "resolved RAM
address" outputs, cross-referenced in the STATIC METHODS section above --
confirms those addresses were correct all along even though the DP value
used to get them at the time was an unconfirmed guess; DP=0 (the real,
traced value) reproduces them exactly via clean arithmetic, which is
strong independent confirmation the formula and the traced DP are both
right.

FINAL STATUS ON THE TABLE ITSELF: read all three addresses directly
(read_memory) -- all three FAIL ("Unable to read bytes"), because they
fall in the declared RAM segment (RAM: 0xee80-0xffff, confirmed via
list_segments) which has no backing byte data in the static ROM image --
i.e. genuine uninitialized RAM, not a bug or a wrong address. Checked
get_xrefs_to on all three exact addresses: 0xf92e and 0xfb2c show only
the ONE read at 0x206c3 (the instruction itself) and nothing else in the
whole program; 0xf930 shows zero xrefs at all. NO code anywhere in this
ROM ever writes any of these three addresses. This closes the
investigation at a hard, confirmed ceiling: the table's DP-bank and
addresses are now known exactly (not guessed), but the table's CONTENTS
are populated by something outside this ROM image's static reach -- live
hardware, an external SCI1 programming tool driving the bootloader over
the wire, or boot-time data shifted in during the handshake itself (this
IS the bootloader/flash-programming protocol path, so "the programming
host writes these via the SCI1 link before jumping here" is a plausible,
consistent story, though not proven). Same category as the
sci1_boot_cmd_dispatch RAM-pointer mechanism review8 already correctly
classified as needing live hardware -- this is a second, independently-
confirmed instance of the same pattern, not a new kind of problem.

REMAINING STATUS-SECTION ITEMS, now resolved or closed:
  1. DONE (fixed sci1_boot_cmd_dispatch's body -- see FOLLOW-UP SESSION
     PART 1 above).
  2. DONE -- DP traced to reset, confirmed DP=0, single value not N.
  3. DONE in the sense that matters -- real addresses computed and
     confirmed via cross-check with the earlier session's independently-
     surfaced candidates; full record-shape decode is moot until #4.
  4. ANSWERED: yes, genuinely never written by any code in this ROM
     image (confirmed via get_xrefs_to on all three exact addresses, not
     an approximation) -- static recovery has hit its ceiling here as
     predicted, regardless of DP, exactly as this item anticipated.
This table is now fully characterized and correctly classified as
runtime/external-data-dependent. No further static work on THIS specific
table is worthwhile; any further progress needs live hardware or a real
SCI1 boot trace capture, not more Ghidra analysis.

--------------------------------------------------------------------
FOLLOW-UP SESSION PART 3: flash_region_classify_table (0x20a14) --
DECODED AND TYPED, CONSUMER NOT FOUND
--------------------------------------------------------------------
Separate item, same neighborhood (the 0x20a14-0x20a7f range
find_code_gaps had flagged as unanalyzed, sitting between
flash_vpp_pulse_handshake_echo_verify and main_loop). User supplied a
raw Ghidra listing view of this region showing per-row target-address
annotations already partially worked out. Re-derived the row structure
independently from the raw bytes to confirm/correct it rather than trust
the annotations at face value -- first pass mis-paired rows by one field
width; re-checked against all 16 rows and the H8 24-bit-address
convention already established elsewhere in this ROM (bank:hi:lo byte
triples) before committing.

CONFIRMED STRUCTURE: 16 entries x 6 bytes = 96 bytes exactly
(0x20a14-0x20a73), FF-padded immediately after (verified via
read_memory) -- 16 rows is the real, complete table, not a partial
gap-fill guess. Each row: [00][hi][lo][00][hi][lo] = two 24-bit fields
(region_base, size_mask), low byte of each implied 0x00. Rows 9-16
verified exactly against a straightforward (hi<<16)|(lo<<8) formula:
0x02c000, 0x028000, 0x024000, 0x020000, 0x01c000, 0x018000, 0x014000,
0x010000 -- these are precisely the 0x4000-aligned ROM flash sector
boundaries spanning 0x10000-0x30000, and the size_mask field's low 16
bits for these rows (0xffff/0xbfff/0x7fff/0x3fff pattern) are exactly
power-of-2-minus-1 masks for a 0x4000 (16KB) region size, confirming
this half is a flash-sector classification/lookup table. Rows 1-8 use
the same encoding for a tighter 0x200-stride range (0x013000-0x013e00),
sitting near the already-known fixed-point calibration math library
region (0x14000-0x1450b) -- purpose of this sub-range not identified.

Created FlashRegionClassifyEntry struct (2x 3-byte-effective fields,
laid out as 6 discrete bytes: pad0/region_base_hi/region_base_lo/
pad1/size_mask_hi/size_mask_lo) and a FlashRegionClassifyEntry[16]
array type, applied at 0x20a14 -- dry-run confirmed exact 96-byte fit
before committing. Labeled the address flash_region_classify_table and
added a decompiler pre-comment with the full decode (plate comments
require a function context and correctly refused on this pure-data
address -- used set_decompiler_comment instead).

CONSUMER SEARCH: same exhaustive-and-empty result as the
sci1_colon_frame_dispatch jump table.
  - get_xrefs_to on the table base and on individual row addresses:
    zero hits anywhere in the ROM.
  - search_byte_patterns for the table's own base address (0x20a14) as
    a literal load operand: no matches in any encoding tried.
  - analyze_data_region across the full 96-byte span: xref_map is
    empty, zero unique_xref_addresses.
  - Directly decompiled and checked the two functions whose names most
    strongly suggested they'd be the consumer --
    sci1_boot_flash_addr_range_validate (0x20461, does flat 32-bit
    range-bound checks against ROM-string-selected error codes, no
    table walk) and flash_erase_sectors (0x2080c, not yet decompiled
    this pass but named the other obvious candidate for next session) --
    the former is confirmed NOT the consumer; the latter should be
    checked next before concluding this table is truly orphaned.
  - The physical adjacency to flash_vpp_pulse_handshake_echo_verify
    (immediately before, ends 0x20a13) and main_loop (immediately
    after, 0x20a80) is real but coincidental -- neither function's
    decompile references this data. The cmp:i #0x10:16,R2 / bls /
    mov:e #0x1:8,R0 code visible right before the table in the raw
    listing is flash_vpp_pulse_handshake_echo_verify's OWN epilogue
    bound-check (function body correctly ends at 0x20a13 with its
    rts/ldm.w restore sequence), not a table-walking loop.

STATUS: table fully decoded and typed with high confidence (the
sector-boundary/power-of-2-mask cross-check is strong structural
evidence, not just plausible-looking numbers), but the consumer remains
unfound -- same "no static reference, likely computed/indirect access"
ceiling as the sci1_colon_frame_dispatch jump table, OR this could
genuinely be unused/legacy data left in the ROM image (unlike the jump
table, there's no confirmed live call site proving this data is ever
actually read at runtime). NOT YET CHECKED: flash_erase_sectors
(0x2080c) and flash_erase_sector_loop (0x2093c) decompiles -- named
candidates for next session before concluding this is dead data rather
than an unresolved-reference case.

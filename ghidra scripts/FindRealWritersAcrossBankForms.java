import ghidra.app.script.GhidraScript;
import ghidra.program.model.address.*;
import ghidra.program.model.listing.*;
import ghidra.program.model.scalar.Scalar;
import ghidra.program.model.symbol.*;
import ghidra.program.model.mem.Memory;
import java.util.*;

/**
 * Whole-program writer/reader search for a list of target RAM addresses,
 * built to close the specific blind spots found during the 2026-08-07 MUT
 * session:
 *
 * 1. get_xrefs_to (and Ghidra's live xref index generally) indexes a
 *    reference under whatever address-space-prefix string the memory
 *    reference actually uses. Confirmed on this ROM: the same physical RAM
 *    cell (0xf130) is referenced under BOTH "0000f130" and "0001f130" forms
 *    depending on which bank was active at the access site, and querying
 *    only one prefix silently misses real references under the other. This
 *    script normalizes every resolved address to its OFFSET WITHIN THE RAM
 *    SPACE (stripping the bank-prefix distinction) before comparing against
 *    targets, so it can't be fooled by that split the way a live per-prefix
 *    xref query can.
 *
 * 2. Some writes don't use a static operand at all -- they use a
 *    RUNTIME-COMPUTED address of the form (uint)bVar<<0x10 | 0xfXXX,
 *    typically compiled as something like:
 *        mov.b  bVar, R0        ; or similar bank-selector load
 *        <shift/or sequence>
 *        mov:g.w R1,@ER0        ; indirect store through a computed pointer
 *    A pure disassembly-literal scan (this script, and search_byte_patterns)
 *    CANNOT see through that -- the low 16 bits (e.g. 0xf17a) still appear
 *    somewhere in the instruction stream as an immediate almost always,
 *    because compilers building that address normally OR/ADD a compile-time
 *    constant low-word onto a runtime-computed high word rather than
 *    computing the whole 32-bit value with no literal at all. So: this
 *    script ALSO reports every instruction referencing the bare 16-bit
 *    offset as an immediate ANYWHERE (mov, cmp, add, or, etc.), not just
 *    confirmed memory-reference operands, as a lower-confidence secondary
 *    signal -- exactly the kind of hit that turned out to be F17A's and
 *    F33A/F33C's real writers, which a strict "is this a memory reference
 *    operand" filter would still have missed depending on how the
 *    instruction disassembles.
 *
 * 3. Every hit is reported with enough context (containing function, full
 *    instruction text, mnemonic) to manually verify it -- this script does
 *    NOT auto-classify hits as "real" the way FixBankOffsetReferences did
 *    for call targets, because distinguishing a genuine address-forming
 *    immediate from a coincidental unrelated literal (e.g. a loop bound
 *    that happens to equal 0xf17a) requires human judgement, not a fixed
 *    rule. This is intentional: the false-positive already found this
 *    session (byte-pattern match landing mid-instruction) came from
 *    OVER-trusting an automated classifier. This script produces candidates
 *    for review, not final answers.
 *
 * 4. Optionally creates a real Ghidra memory reference at any hit the user
 *    confirms via the CONFIRM_AND_ANNOTATE list below, so future
 *    get_xrefs_to calls (under either prefix) will find it -- same pattern
 *    as FixBankOffsetReferences_v2, applied here to data references instead
 *    of call targets.
 */
public class FindRealWritersAcrossBankForms extends GhidraScript {

    // Target addresses to search for, as bare 16-bit RAM offsets (no bank
    // prefix -- this script normalizes both 0000fXXX and 0001fXXX forms to
    // this same offset before comparing). Edit this list, or leave empty to
    // be prompted interactively.
    static final String[] TARGET_OFFSETS = {
        "f21d", "f217", "f179", "f1e7", "eedf", "eee1"
    };

    // After reviewing script output, list any (targetOffset, fromAddress)
    // pairs you've manually confirmed are real address-forming references,
    // to have this script create a proper Ghidra memory reference for them.
    // Format: "f17a:00028dee" (address in hex, no "0x" prefix). Leave empty
    // to skip annotation and just report candidates.
    static final String[] CONFIRM_AND_ANNOTATE = {
        // "f17a:00028dee",
    };

    @Override
    public void run() throws Exception {
        Listing listing = currentProgram.getListing();
        AddressFactory af = currentProgram.getAddressFactory();
        Memory mem = currentProgram.getMemory();
        ReferenceManager refMan = currentProgram.getReferenceManager();

        List<String> targets = new ArrayList<>(Arrays.asList(TARGET_OFFSETS));
        if (targets.isEmpty() || allBlank(targets)) {
            String input = askString("Target RAM offsets",
                "Comma-separated 16-bit hex offsets, no bank prefix (e.g. f17a,f33a):");
            targets = Arrays.asList(input.toLowerCase().split("\\s*,\\s*"));
        }

        // Normalize targets to plain longs for fast comparison.
        Map<Long, String> targetVals = new LinkedHashMap<>();
        for (String t : targets) {
            try {
                targetVals.put(Long.parseLong(t.trim(), 16), t.trim());
            } catch (NumberFormatException e) {
                println("Skipping unparsable target: " + t);
            }
        }

        Map<Long, Integer> memRefHitCounts = new LinkedHashMap<>();
        Map<Long, Integer> immOnlyHitCounts = new LinkedHashMap<>();
        for (Long v : targetVals.keySet()) {
            memRefHitCounts.put(v, 0);
            immOnlyHitCounts.put(v, 0);
        }

        println("=== Scanning all instructions for " + targetVals.size() + " target offset(s) ===");
        println("Targets: " + targetVals.values());
        println("");

        InstructionIterator instrs = listing.getInstructions(true);
        while (instrs.hasNext()) {
            Instruction insn = instrs.next();
            Function fn = listing.getFunctionContaining(insn.getAddress());
            String fnName = (fn != null) ? fn.getName() : "UNKNOWN";

            // Pass 1: does this instruction have a genuine MEMORY REFERENCE
            // (as Ghidra's operand analysis already resolved it) landing on
            // one of our targets, regardless of which bank-prefix form the
            // resolved address uses? This catches everything get_xrefs_to
            // would find under EITHER prefix, in one pass.
            Reference[] refsFrom = insn.getReferencesFrom();
            for (Reference r : refsFrom) {
                Address to = r.getToAddress();
                if (to == null || !to.getAddressSpace().getName().toLowerCase().contains("ram")) continue;
                long offsetInSpace = to.getOffset() & 0xFFFFL; // low 16 bits, strips bank prefix
                if (targetVals.containsKey(offsetInSpace)) {
                    String label = targetVals.get(offsetInSpace);
                    memRefHitCounts.put(offsetInSpace, memRefHitCounts.get(offsetInSpace) + 1);
                    println("[MEM-REF] " + label + " (resolved " + to + ") <- " + fnName +
                        " @ " + insn.getAddress() + " : " + insn.toString() +
                        "  [" + r.getReferenceType() + "]");
                }
            }

            // Pass 2: does this instruction contain the bare 16-bit target
            // value as a plain SCALAR IMMEDIATE anywhere in its operands,
            // regardless of whether Ghidra resolved it as a memory
            // reference? This is the lower-confidence secondary signal that
            // catches computed-bank-store address formation (the "or 0xfXXX
            // onto a runtime-shifted bank register" idiom) that pass 1 can
            // miss entirely if Ghidra didn't recognize it as a reference.
            int numOps = insn.getNumOperands();
            for (int op = 0; op < numOps; op++) {
                Object[] opObjs = insn.getOpObjects(op);
                if (opObjs == null) continue;
                for (Object o : opObjs) {
                    if (o instanceof Scalar) {
                        long val = ((Scalar) o).getUnsignedValue();
                        if (targetVals.containsKey(val)) {
                            String label = targetVals.get(val);
                            immOnlyHitCounts.put(val, immOnlyHitCounts.get(val) + 1);
                            println("[IMM-ONLY, VERIFY MANUALLY] " + label + " <- " + fnName +
                                " @ " + insn.getAddress() + " : " + insn.toString());
                        }
                    }
                }
            }
        }

        println("");
        println("=== Summary ===");
        for (Map.Entry<Long, String> e : targetVals.entrySet()) {
            long v = e.getKey();
            println(String.format("  %-6s : %d confirmed memory-reference hit(s), %d immediate-only candidate(s) needing manual review",
                e.getValue(), memRefHitCounts.get(v), immOnlyHitCounts.get(v)));
        }
        println("");
        println("NOTE: zero hits in BOTH categories for a target is much stronger evidence of");
        println("'no writer exists' than a get_xrefs_to check under one or two prefixes alone,");
        println("since this scan covers every instruction in the program once, normalized to a");
        println("single address form. It is still not absolute proof -- a write could theoretically");
        println("use a fully runtime-computed address with no compile-time-constant low word at");
        println("all, which no static-literal scan (this script, byte-pattern search, or the .c");
        println("dump grep) can detect.");

        // Optional: create real references for manually-confirmed hits.
        if (CONFIRM_AND_ANNOTATE.length > 0) {
            println("");
            println("=== Annotating confirmed hits ===");
            for (String entry : CONFIRM_AND_ANNOTATE) {
                String[] parts = entry.split(":");
                if (parts.length != 2) { println("Bad entry, skipping: " + entry); continue; }
                long targetVal;
                try {
                    targetVal = Long.parseLong(parts[0].trim(), 16);
                } catch (NumberFormatException e) {
                    println("Bad target offset in entry, skipping: " + entry);
                    continue;
                }
                Address fromAddr;
                try {
                    fromAddr = af.getAddress(parts[1].trim());
                } catch (Exception e) {
                    println("Bad from-address in entry, skipping: " + entry);
                    continue;
                }
                Address toAddr;
                try {
                    toAddr = af.getAddress(String.format("%08x", targetVal));
                } catch (Exception e) {
                    println("Could not resolve target address for entry, skipping: " + entry);
                    continue;
                }
                if (!mem.contains(toAddr)) {
                    println("Resolved target " + toAddr + " outside defined memory, skipping: " + entry);
                    continue;
                }
                boolean alreadyThere = false;
                for (Reference r : refMan.getReferencesFrom(fromAddr)) {
                    if ((r.getToAddress().getOffset() & 0xFFFFL) == targetVal) { alreadyThere = true; break; }
                }
                if (alreadyThere) { println("Already present, skipping: " + entry); continue; }

                refMan.addMemoryReference(fromAddr, toAddr, RefType.WRITE, SourceType.USER_DEFINED, 0);
                println("Reference added: " + fromAddr + " -> " + toAddr + " (from " + entry + ")");
            }
        }
    }

    private boolean allBlank(List<String> l) {
        for (String s : l) if (s != null && !s.trim().isEmpty()) return false;
        return true;
    }
}

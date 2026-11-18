import ghidra.app.script.GhidraScript;
import ghidra.program.model.address.*;
import ghidra.program.model.listing.*;
import ghidra.program.model.scalar.Scalar;
import ghidra.program.model.symbol.*;
import ghidra.program.model.mem.Memory;
import java.util.*;

/**
 * ROM-agnostic version. Two changes from the original that make it portable:
 *
 * 1. TARGETS are supplied by name via askString (or edit TARGET_NAMES below
 *    for headless use) instead of hardcoded addresses -- resolved at runtime
 *    via the symbol table. Works on any binary regardless of where these
 *    functions happen to land, as long as they're named/labeled.
 *
 * 2. VALIDITY CHECK no longer relies on a hardcoded bank allowlist ({1,2}),
 *    which was this-ROM-specific and would silently reject legitimate call
 *    sites on a ROM with a different bank layout. Replaced with two checks
 *    that generalize to any ROM:
 *      a. ADJACENCY: the two "mov"-family immediates supplying offset/bank
 *         must be on instructions with nothing but each other between them
 *         and the call -- if the scan crosses into unrelated code (e.g. an
 *         arg-less wrapper with no real pushes of its own), it now bails
 *         instead of grabbing stale immediates from further up.
 *      b. FUNCTION-BOUNDARY: the backward scan never leaves the function
 *         containing the call site. This alone would have caught the
 *         table_3axis_interp_triple_wrapper false positive from item 27.
 *    Memory containment (mem.contains) remains as the final sanity check,
 *    since a resolved address outside any defined block is wrong on any ROM.
 */
public class FixBankOffsetReferences_v2 extends GhidraScript {

    // Edit this list, or leave empty to be prompted interactively.
    static final String[] TARGET_NAMES = {
        "table_lookup_interp", "table_lookup_indexed", "table_3axis_interp_triple"
    };

    @Override
    public void run() throws Exception {
        Listing listing = currentProgram.getListing();
        AddressFactory af = currentProgram.getAddressFactory();
        Memory mem = currentProgram.getMemory();
        ReferenceManager refMan = currentProgram.getReferenceManager();
        SymbolTable symTab = currentProgram.getSymbolTable();

        List<String> targetNames = new ArrayList<>(Arrays.asList(TARGET_NAMES));
        if (targetNames.isEmpty() || allBlank(targetNames)) {
            String input = askString("Target functions",
                "Comma-separated function names (bank+offset far-pointer callees):");
            targetNames = Arrays.asList(input.split("\\s*,\\s*"));
        }

        int totalFixed = 0, totalSkippedNoAdjacency = 0, totalSkippedCrossFunction = 0;
        int totalSkippedOutOfRange = 0, totalSkippedUnresolved = 0, totalAlreadyPresent = 0;

        for (String name : targetNames) {
            List<Function> matches = new ArrayList<>();
            for (Symbol s : symTab.getSymbols(name)) {
                Function f = listing.getFunctionAt(s.getAddress());
                if (f != null) matches.add(f);
            }
            if (matches.isEmpty()) { println(name + ": not found, skipping"); continue; }

            for (Function target : matches) {
                Address targetAddr = target.getEntryPoint();
                ReferenceIterator refs = refMan.getReferencesTo(targetAddr);
                List<Address> callSites = new ArrayList<>();
                while (refs.hasNext()) callSites.add(refs.next().getFromAddress());

                println("=== " + name + " @ " + targetAddr + " (" + callSites.size() + " call sites) ===");

                for (Address callSite : callSites) {
                    Function caller = listing.getFunctionContaining(callSite);
                    String callerName = (caller != null) ? caller.getName() : "UNKNOWN";

                    Instruction cur = listing.getInstructionAt(callSite);
                    List<Long> imms = new ArrayList<>();
                    List<Address> immAddrs = new ArrayList<>();
                    boolean crossedFunctionBoundary = false;
                    boolean adjacencyBroken = false;

                    // Walk backward one instruction at a time collecting the two
                    // "mov #imm,@-SP" immediate pushes that supply this call's
                    // offset/bank pair. Instructions that don't touch the stack
                    // (register moves, frame spills like "mov:f.w R0,@(-N:8,FP)")
                    // are transparent filler and don't break adjacency -- compilers
                    // routinely interleave a spill between the second push and the
                    // call. What DOES break adjacency is anything that itself
                    // pushes onto @-SP without being a qualifying immediate push
                    // (e.g. "mov R0,@-SP", a register push) -- that's a THIRD
                    // stack argument that would displace what "top of stack" means
                    // for this call, and grabbing immediates from further back
                    // past it would be wrong. This is also how the arg-less
                    // wrapper false positive (item 27, table_3axis_interp_triple_
                    // wrapper) gets caught: it has no pushes of its own at all, so
                    // the scan crosses straight into its caller's function body
                    // and the function-boundary check below stops it.
                    for (int i = 0; i < 16 && cur != null; i++) {
                        Instruction prev = cur.getPrevious();
                        if (prev == null) break;

                        // Never leave the function containing the call site.
                        if (caller != null && !caller.getBody().contains(prev.getAddress())) {
                            crossedFunctionBoundary = true;
                            break;
                        }

                        String mn = prev.getMnemonicString();
                        boolean isQualifyingMov = false;
                        boolean pushesStack = false;
                        if (mn.startsWith("mov")) {
                            Object[] opObjs = prev.getOpObjects(0);
                            if (opObjs != null && opObjs.length > 0 && opObjs[0] instanceof Scalar) {
                                isQualifyingMov = true;
                                long val = ((Scalar) opObjs[0]).getUnsignedValue();
                                imms.add(0, val);
                                immAddrs.add(0, prev.getAddress());
                            } else {
                                // A "mov ...,@-SP" whose source is NOT a plain
                                // immediate (register, memory-indirect, etc.) is
                                // still a real stack push -- e.g. table_lookup_
                                // indexed's own internal call at 0x14874 pushes
                                // R0/R1 (runtime array-slot values), not literals.
                                pushesStack = mentionsPredecrementSP(prev);
                            }
                        } else {
                            pushesStack = mentionsPredecrementSP(prev);
                        }

                        if (!isQualifyingMov) {
                            if (pushesStack) {
                                // A real (non-immediate) push interleaved in here
                                // means the two values we've collected, if any,
                                // aren't actually this call's top-of-stack args.
                                if (imms.isEmpty()) { break; }
                                adjacencyBroken = true;
                                break;
                            }
                            // Non-stack-affecting filler (register move, frame
                            // spill, flag test, etc.) -- transparent, keep walking
                            // back without counting it against adjacency.
                            if (imms.isEmpty() && (mn.equals("pjsr") || mn.equals("jsr") || mn.equals("bsr"))) {
                                // Hit a preceding call with nothing collected yet:
                                // this call site has no immediate pushes at all.
                                break;
                            }
                        }

                        cur = prev;
                        if (imms.size() >= 2) break;
                    }

                    if (crossedFunctionBoundary) {
                        totalSkippedCrossFunction++;
                        println("  " + callerName + " @ " + callSite +
                            " : SKIPPED (scan crossed into a different function -- likely" +
                            " an arg-less caller/wrapper, not a real push site)");
                        continue;
                    }

                    if (imms.size() < 2) {
                        totalSkippedUnresolved++;
                        println("  " + callerName + " @ " + callSite +
                            " : SKIPPED (could not resolve two adjacent immediates" +
                            " before reaching a non-push instruction or function start)");
                        continue;
                    }

                    if (adjacencyBroken) {
                        totalSkippedNoAdjacency++;
                        println("  " + callerName + " @ " + callSite +
                            " : SKIPPED (immediates not tightly adjacent to the call --" +
                            " likely picked up unrelated code, not this call's real args)");
                        continue;
                    }

                    long offset = imms.get(0);
                    long bank = imms.get(1);
                    Address offsetPushAddr = immAddrs.get(0);

                    long realAddr = bank * 0x10000L + offset;
                    Address realAddress;
                    try {
                        realAddress = af.getAddress(String.format("%08x", realAddr));
                    } catch (Exception e) {
                        totalSkippedOutOfRange++;
                        continue;
                    }

                    if (!mem.contains(realAddress)) {
                        println("  " + callerName + " @ " + callSite +
                            " : SKIPPED (resolved 0x" + String.format("%08x", realAddr) +
                            " falls outside any defined memory block, bank=" + bank + ")");
                        totalSkippedOutOfRange++;
                        continue;
                    }

                    boolean alreadyThere = false;
                    for (Reference r : refMan.getReferencesFrom(offsetPushAddr)) {
                        if (r.getToAddress().equals(realAddress)) { alreadyThere = true; break; }
                    }
                    if (alreadyThere) { totalAlreadyPresent++; continue; }

                    refMan.addMemoryReference(offsetPushAddr, realAddress, RefType.DATA, SourceType.USER_DEFINED, 0);
                    println("  " + callerName + " @ " + callSite + " : reference added " +
                        offsetPushAddr + " -> " + realAddress);
                    totalFixed++;
                }
            }
        }

        println("");
        println("TOTAL new references added: " + totalFixed);
        println("TOTAL already present (idempotent skip): " + totalAlreadyPresent);
        println("TOTAL skipped (crossed function boundary): " + totalSkippedCrossFunction);
        println("TOTAL skipped (immediates not adjacent to call): " + totalSkippedNoAdjacency);
        println("TOTAL skipped (resolved address outside defined memory): " + totalSkippedOutOfRange);
        println("TOTAL skipped (could not resolve two immediates at all): " + totalSkippedUnresolved);
    }

    private boolean allBlank(List<String> l) {
        for (String s : l) if (s != null && !s.trim().isEmpty()) return false;
        return true;
    }

    // True if this instruction's destination operand is a predecrement-SP push
    // (i.e. "...,@-SP"), regardless of what the source operand is (immediate,
    // register, or memory-indirect all count). Detected via the mnemonic's
    // operand representation rather than a raw string match on the full
    // instruction text, since Ghidra formats "@-SP" identically for the
    // pre-decrement addressing mode across mov variants on this architecture.
    private boolean mentionsPredecrementSP(Instruction insn) {
        int numOps = insn.getNumOperands();
        for (int op = 0; op < numOps; op++) {
            String repr = insn.getDefaultOperandRepresentation(op);
            if (repr != null && repr.contains("-SP")) return true;
        }
        return false;
    }
}

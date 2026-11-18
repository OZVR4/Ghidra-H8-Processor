// Whole-program version of FixBankOffsetReferences.java / FindBatch2Callers.java /
// FindAxisCallers.java: rather than a hardcoded target-function list (bank+offset
// pushed right before a call to a SPECIFIC known function like table_lookup_interp)
// or a hardcoded target-ADDRESS list (report-only, per-investigation), this scans
// every instruction in the program for the literal
//     mov:g.w #offset16,@-SP
//     mov:g.b #bank8,@-SP
// pair immediately preceding a pjsr/jsr/bsr, resolves bank*0x10000+offset, and -- if
// it lands inside a defined memory block and the bank is plausible (1 or 2, per
// review.md/review2.md's own observed-bank findings) -- writes a real memory
// reference from the offset-push instruction to the resolved address.
//
// This covers BOTH value tables (table_lookup_interp/indexed/table_3axis_interp_triple)
// AND axis tables (axis_lookup_interp and anything else reached the same way) in one
// pass, with no need to enumerate target functions or target addresses up front --
// unlike FixBankOffsetReferences.java (fixed function list) and
// FindAxisCallers.java/FindBatch2Callers.java (fixed address list, report-only).
//
// Idempotent: skips any (from,to) pair that's already a reference. Safe to re-run.
// @category H8539F

import ghidra.app.script.GhidraScript;
import ghidra.program.model.address.Address;
import ghidra.program.model.address.AddressFactory;
import ghidra.program.model.listing.Instruction;
import ghidra.program.model.listing.InstructionIterator;
import ghidra.program.model.listing.Function;
import ghidra.program.model.listing.Listing;
import ghidra.program.model.mem.Memory;
import ghidra.program.model.scalar.Scalar;
import ghidra.program.model.symbol.Reference;
import ghidra.program.model.symbol.ReferenceManager;
import ghidra.program.model.symbol.RefType;
import ghidra.program.model.symbol.SourceType;

import java.util.HashSet;
import java.util.Set;

public class FixImmediatePushReferences extends GhidraScript {
    @Override
    public void run() throws Exception {
        Listing listing = currentProgram.getListing();
        AddressFactory af = currentProgram.getAddressFactory();
        Memory mem = currentProgram.getMemory();
        ReferenceManager refMan = currentProgram.getReferenceManager();

        // Observed real banks (review.md item 27 / review2.md item 23): 1 and 2 only.
        Set<Long> plausibleBanks = new HashSet<>();
        plausibleBanks.add(1L);
        plausibleBanks.add(2L);

        int totalFixed = 0;
        int totalAlreadyPresent = 0;
        int totalSkippedBadBank = 0;
        int totalSkippedOutOfRange = 0;

        InstructionIterator it = listing.getInstructions(true);

        Long pendingOffset = null;
        Address pendingOffsetAt = null;

        while (it.hasNext()) {
            Instruction insn = it.next();
            String mnem = insn.getMnemonicString();
            String rep = insn.toString();

            if (mnem.startsWith("mov") && rep.contains("@-SP")) {
                Scalar sc = null;
                try { sc = insn.getScalar(0); } catch (Exception e) {}
                if (sc != null) {
                    long val = sc.getUnsignedValue();
                    if (rep.contains(":16")) {
                        pendingOffset = val;
                        pendingOffsetAt = insn.getAddress();
                    }
                    else if (rep.contains(":8") && pendingOffset != null) {
                        long bank = val;
                        long offset = pendingOffset;
                        Address offsetPushAddr = pendingOffsetAt;

                        if (!plausibleBanks.contains(bank)) {
                            totalSkippedBadBank++;
                        } else {
                            long realAddr = bank * 0x10000L + offset;
                            Address realAddress = null;
                            try {
                                realAddress = af.getAddress(String.format("%08x", realAddr));
                            } catch (Exception e) {
                                realAddress = null;
                            }

                            if (realAddress == null || !mem.contains(realAddress)) {
                                totalSkippedOutOfRange++;
                            } else {
                                boolean alreadyThere = false;
                                for (Reference r : refMan.getReferencesFrom(offsetPushAddr)) {
                                    if (r.getToAddress().equals(realAddress)) { alreadyThere = true; break; }
                                }
                                if (alreadyThere) {
                                    totalAlreadyPresent++;
                                } else {
                                    refMan.addMemoryReference(offsetPushAddr, realAddress,
                                        RefType.DATA, SourceType.USER_DEFINED, 0);
                                    Function caller = listing.getFunctionContaining(offsetPushAddr);
                                    String callerName = (caller != null) ? caller.getName() : "UNKNOWN";
                                    println("  " + callerName + " @ " + offsetPushAddr +
                                        " : reference added -> " + realAddress);
                                    totalFixed++;
                                }
                            }
                        }
                        pendingOffset = null;
                        pendingOffsetAt = null;
                    }
                }
            }
            // Any call-family instruction resets the pending state -- we only want
            // offset/bank pairs that are genuinely back-to-back, immediately
            // preceding each other (mirrors FixBankOffsetReferences.java's
            // backward-scan break condition, just applied going forward).
            if (mnem.equals("pjsr") || mnem.equals("jsr") || mnem.equals("bsr")) {
                pendingOffset = null;
                pendingOffsetAt = null;
            }
        }

        println("");
        println("TOTAL new references added: " + totalFixed);
        println("TOTAL already present (skipped, idempotent): " + totalAlreadyPresent);
        println("TOTAL skipped (implausible bank): " + totalSkippedBadBank);
        println("TOTAL skipped (resolved address outside defined memory): " + totalSkippedOutOfRange);
    }
}

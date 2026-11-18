import ghidra.app.script.GhidraScript;
import ghidra.program.model.address.*;
import ghidra.program.model.listing.*;
import ghidra.program.model.scalar.Scalar;
import ghidra.program.model.symbol.*;
import java.util.*;

/**
 * Annotates call sites of table_lookup_interp, table_lookup_indexed, and
 * table_3axis_interp_triple with an EOL comment showing the real,
 * bank-adjusted ROM address (bank*0x10000 + offset), computed from the two
 * literal immediates pushed immediately before each call.
 *
 * Background: these three functions take (ep_val/bank, table/offset) as two
 * SEPARATE arguments. Confirmed (review.md item 26) that Ghidra's decompiler
 * does NOT recognize bank*0x10000+offset as a combined far pointer -- it
 * just shows two plain integers at the call site. This script adds the
 * missing human-readable annotation directly at each call site.
 *
 * Method: reuses the backward-immediate-scan technique from review.md items
 * 19/22 (walk backwards from the call instruction collecting the last two
 * "mov"-family immediates pushed before it, stopping at the previous
 * call/pjsr/jsr/bsr).
 */
public class AnnotateBankOffsetCallSites extends GhidraScript {
    @Override
    public void run() throws Exception {
        Listing listing = currentProgram.getListing();
        AddressFactory af = currentProgram.getAddressFactory();

        String[] targetAddrs = { "00014656", "00014854", "000148a2" };
        String[] targetNames = { "table_lookup_interp", "table_lookup_indexed", "table_3axis_interp_triple" };

        int totalAnnotated = 0;
        int totalSkipped = 0;

        for (int t = 0; t < targetAddrs.length; t++) {
            Address targetAddr = af.getAddress(targetAddrs[t]);
            Function target = getFunctionAt(targetAddr);
            if (target == null) {
                println(targetNames[t] + " @ " + targetAddrs[t] + ": function not found, skipping");
                continue;
            }

            ReferenceIterator refs = currentProgram.getReferenceManager().getReferencesTo(targetAddr);
            List<Address> callSites = new ArrayList<>();
            while (refs.hasNext()) {
                callSites.add(refs.next().getFromAddress());
            }

            println("=== " + targetNames[t] + " (" + callSites.size() + " call sites) ===");

            for (Address callSite : callSites) {
                Function caller = listing.getFunctionContaining(callSite);
                String callerName = (caller != null) ? caller.getName() : "UNKNOWN";

                Instruction cur = listing.getInstructionAt(callSite);
                List<Long> imms = new ArrayList<>();
                for (int i = 0; i < 8 && cur != null; i++) {
                    cur = cur.getPrevious();
                    if (cur == null) break;
                    String mn = cur.getMnemonicString();
                    if (mn.startsWith("mov")) {
                        Object[] opObjs = cur.getOpObjects(0);
                        if (opObjs != null && opObjs.length > 0 && opObjs[0] instanceof Scalar) {
                            long val = ((Scalar) opObjs[0]).getUnsignedValue();
                            imms.add(0, val);
                        }
                    }
                    if (mn.equals("pjsr") || mn.equals("jsr") || mn.equals("bsr")) break;
                    if (imms.size() >= 2) break;
                }

                if (imms.size() < 2) {
                    println("  " + callerName + " @ " + callSite + " : could not resolve bank/offset, skipping");
                    totalSkipped++;
                    continue;
                }

                long offset = imms.get(0);
                long bank = imms.get(1);
                long realAddr = bank * 0x10000L + offset;
                Address realAddress = af.getAddress(String.format("%08x", realAddr));

                String comment = String.format("-> 0x%08x (bank=%d, offset=0x%x)", realAddr, bank, offset);

                CodeUnit cu = listing.getCodeUnitAt(callSite);
                if (cu != null) {
                    String existing = cu.getComment(CodeUnit.EOL_COMMENT);
                    if (existing == null || !existing.contains("-> 0x")) {
                        String newComment = (existing == null || existing.isEmpty())
                            ? comment
                            : existing + " | " + comment;
                        cu.setComment(CodeUnit.EOL_COMMENT, newComment);
                        println("  " + callerName + " @ " + callSite + " : " + comment);
                        totalAnnotated++;
                    } else {
                        println("  " + callerName + " @ " + callSite + " : already annotated, skipping");
                    }
                }
            }
        }

        println("");
        println("TOTAL annotated: " + totalAnnotated + ", skipped (unresolved): " + totalSkipped);
    }
}

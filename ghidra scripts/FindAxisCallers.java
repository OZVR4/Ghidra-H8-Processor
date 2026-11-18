// Generic "who calls this address" tracer -- finds the function that pushes a given
// address as a literal immediate (mov:g.w #0xNNNN,@-SP / mov:g.b #bank,@-SP) immediately
// before a pjsr, regardless of what kind of address it is (axis, table, or anything else).
//
// This is the SAME core scan logic as FindBatch2Callers.java (unmodified algorithm),
// just generalized to take any address list rather than being hardcoded to Batch 2 table
// addresses. Written because get_xrefs_to/get_bulk_xrefs are structurally blind to this
// addressing pattern -- every axis/table in this ROM is reached via a raw 16-bit immediate
// push, never an address-mode operand, so Ghidra's static xref engine never records a data
// reference no matter how well-typed the calling function is (confirmed 2026-08-03 by
// checking get_xrefs_to on an address with an already-known, hand-verified caller and
// getting zero hits regardless). See review2.md item 23 for the investigation that produced
// this script.
//
// Edit TARGET_ADDRS below before each run -- this is a scratch/investigation tool, not a
// one-shot batch script like FindBatch2Callers.java's fixed table list.
//
// @category H8539F

import ghidra.app.script.GhidraScript;
import ghidra.program.model.address.Address;
import ghidra.program.model.listing.Instruction;
import ghidra.program.model.listing.InstructionIterator;
import ghidra.program.model.listing.Function;
import ghidra.program.model.scalar.Scalar;

import java.util.HashMap;
import java.util.Map;

public class FindAxisCallers extends GhidraScript {

    // Edit this list per investigation. Defaults to the 20 unattributed
    // H8539F-SCRAPED-TABLE axis addresses from review2.md item 23.
    static final int[] TARGET_ADDRS = new int[] {
        0x2D0C0, 0x2D0D2, 0x2D146, 0x2D15E, 0x2D192, 0x2D1B4, 0x2D20A, 0x2D218,
        0x2D248, 0x2D2CC, 0x2D302, 0x2D37C, 0x2D3A8, 0x2D3C2, 0x2D3DA, 0x2D412,
        0x2D428, 0x2D45C, 0x2D472, 0x2D4A6
    };

    @Override
    public void run() throws Exception {
        Map<Integer, String> found = new HashMap<>();

        InstructionIterator it = currentProgram.getListing().getInstructions(true);
        Integer pendingOffset = null;

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
                        pendingOffset = (int) val;
                    }
                    else if (rep.contains(":8") && pendingOffset != null) {
                        int bank = (int) val;
                        for (int a : TARGET_ADDRS) {
                            int wantBank = (a >> 16) & 0xFF;
                            int wantOff = a & 0xFFFF;
                            if (bank == wantBank && pendingOffset == wantOff) {
                                Function f = getFunctionContaining(insn.getAddress());
                                String fname = f != null ? f.getName() : "UNKNOWN";
                                String key = String.format("%06X", a);
                                String entry = String.format("%s @ %s (in %s)", key, insn.getAddress(), fname);
                                found.merge(a, entry, (o, n) -> o + " | " + n);
                            }
                        }
                        pendingOffset = null;
                    }
                }
            }
        }

        StringBuilder sb = new StringBuilder();
        for (int a : TARGET_ADDRS) {
            String key = String.format("%06X", a);
            if (found.containsKey(a)) {
                sb.append(key).append(": ").append(found.get(a)).append("\n");
            }
            else {
                sb.append(key).append(": NO CALLER FOUND\n");
            }
        }
        println(sb.toString());
    }
}

// For each Batch 2 "Unknown"-category table address, scans the whole program for
// literal mov-to-stack instructions matching that address's (bank, offset) pair,
// the same pattern ResolveTableReadIndexedArrays.java walks backward from a known
// call site. Here we scan forward across all instructions instead, since we don't
// have a known call site to start from -- we're looking for ANY literal push of
// each target address anywhere in the program.
// @category H8539F

import ghidra.app.script.GhidraScript;
import ghidra.program.model.address.Address;
import ghidra.program.model.listing.Instruction;
import ghidra.program.model.listing.InstructionIterator;
import ghidra.program.model.listing.Function;
import ghidra.program.model.scalar.Scalar;

import java.util.HashMap;
import java.util.Map;

public class FindBatch2Callers extends GhidraScript {
    @Override
    public void run() throws Exception {
        int[] addrs = new int[] {
            0x10D73, 0x11834, 0x1186C, 0x11880, 0x11894, 0x118A0, 0x11D20, 0x11D4C,
            0x11DEC, 0x11E4A, 0x11E6A, 0x11E86, 0x11E94, 0x11EA0, 0x11EC8, 0x11ED6,
            0x11EE2, 0x11F16, 0x11F30, 0x11F3C, 0x11F4A, 0x11F5C, 0x11F64, 0x11F6C,
            0x127A8, 0x127D8, 0x127FC, 0x12824, 0x1284E, 0x12858, 0x12860, 0x1286C,
            0x128D2, 0x128DE, 0x128EA, 0x12926, 0x12950, 0x1297A, 0x12986, 0x12AE6,
            0x12B06, 0x12B94, 0x12CBC, 0x12CD2, 0x12CE8, 0x12CF4, 0x12D00, 0x12D70,
            0x12DD6, 0x12DE4, 0x12DF2, 0x12E4A, 0x12EA2, 0x12EAE, 0x12EBC, 0x12ECE,
            0x12EDC, 0x12EEA, 0x12EF8, 0x12F06, 0x12F14, 0x1313E, 0x1314C, 0x13154,
            0x1315C, 0x13168, 0x13174, 0x1317C, 0x13188, 0x13190, 0x13198, 0x131A0,
            0x131A8, 0x131B0, 0x131B8, 0x131C0, 0x131CC, 0x131D4, 0x131E0, 0x131EC,
            0x131F6, 0x13202, 0x1320A, 0x13212, 0x1321C, 0x13236, 0x13250, 0x1325E,
            0x1326C, 0x1327A, 0x2D3C0, 0x2D5EC, 0x2D668, 0x2D796
        };

        // Build a map from offset(16-bit) -> list of target full addresses that share it,
        // since bank is almost always 1 (0x1xxxx range) or 2 (0x2xxxx range) for these.
        Map<Integer, Integer> offsetToFullAddr = new HashMap<>();
        for (int a : addrs) {
            int bank = (a >> 16) & 0xFF;
            int off = a & 0xFFFF;
            offsetToFullAddr.put(off | (bank << 24), a); // unique key
        }

        Map<Integer, String> found = new HashMap<>();

        InstructionIterator it = currentProgram.getListing().getInstructions(true);
        Integer pendingOffset = null;
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
                        pendingOffset = (int) val;
                        pendingOffsetAt = insn.getAddress();
                    }
                    else if (rep.contains(":8") && pendingOffset != null) {
                        int bank = (int) val;
                        for (int a : addrs) {
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
        for (int a : addrs) {
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

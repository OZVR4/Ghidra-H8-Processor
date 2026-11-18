// Reads the 4-byte header (mode, axis-ptr bank, axis-ptr hi, axis-ptr lo) at each
// Batch 2 "Unknown"-category table address, for review2.md Batch 2 naming pass.
// @category H8539F

import ghidra.app.script.GhidraScript;
import ghidra.program.model.address.Address;
import ghidra.program.model.mem.Memory;

public class DumpBatch2Headers extends GhidraScript {
    @Override
    public void run() throws Exception {
        int[] addrs = new int[] {
            0x10D73, 0x11834, 0x1186C, 0x11880, 0x11894, 0x118A0, 0x11D20, 0x11D4C,
            0x11DEC, 0x11E4A, 0x11E6A, 0x11E86, 0x11E94, 0x11EA0, 0x11EC8, 0x11ED6,
            0x11EE6, 0x11F16, 0x11F30, 0x11F3C, 0x11F4A, 0x11F5C, 0x11F64, 0x11F6C,
            0x127A8, 0x127D8, 0x127FC, 0x12824, 0x1284E, 0x12858, 0x12860, 0x1286C,
            0x128D2, 0x128DE, 0x128EA, 0x12926, 0x12950, 0x1297A, 0x12986, 0x12AE6,
            0x12B06, 0x12B94, 0x12CBC, 0x12CD2, 0x12CE8, 0x12CF4, 0x12D00, 0x12D70,
            0x12DD6, 0x12DE4, 0x12DF2, 0x12E4A, 0x12EA2, 0x12EAE, 0x12EBC, 0x12ECE,
            0x12EDC, 0x12EEA, 0x12EF8, 0x12F06, 0x12F14, 0x1313E, 0x1314C, 0x13154,
            0x1315C, 0x13168, 0x13174, 0x1317C, 0x13188, 0x13190, 0x13198, 0x131A0,
            0x131A8, 0x131B0, 0x131B8, 0x131C0, 0x131CC, 0x131D4, 0x131E0, 0x131EC,
            0x131F6, 0x13202, 0x1320A, 0x13212, 0x1321C, 0x13236, 0x13250, 0x1325E,
            0x1326C, 0x1327A, 0x2D3C4, 0x2D5F0, 0x2D66C, 0x2D796
        };
        Memory mem = currentProgram.getMemory();
        StringBuilder sb = new StringBuilder();
        for (int a : addrs) {
            Address addr = toAddr(a);
            try {
                byte mode = mem.getByte(addr);
                byte b1 = mem.getByte(addr.add(1));
                byte hi1 = mem.getByte(addr.add(2));
                byte lo1 = mem.getByte(addr.add(3));
                int ptr1 = ((hi1 & 0xFF) << 8) | (lo1 & 0xFF);
                String line = String.format("%06X mode=%d bank1=%d ptr1=%04X", a, mode & 0xFF, b1 & 0xFF, ptr1);
                sb.append(line).append("\n");
            }
            catch (Exception e) {
                sb.append(String.format("%06X ERROR %s", a, e.getMessage())).append("\n");
            }
        }
        println(sb.toString());
    }
}

// Scans for direct "mov ADDR,Rn" loads (the confirmed real mechanism for 1D
// scalar table reads -- no wrapper function exists, unlike 2D/3D tables).
// Uses insn.getScalar() for reliable operand extraction (handles DP/bank
// prefixes correctly, unlike raw disassembly text which can show a
// bank-relative short form). Filters to single-xref ROM addresses in the
// known 1D scalar range, excludes RAM (0xF0xx+) targets and anything within
// 8 bytes of an existing XML entry.
// @category H8539F

import ghidra.app.script.GhidraScript;
import ghidra.program.model.address.Address;
import ghidra.program.model.listing.Instruction;
import ghidra.program.model.listing.InstructionIterator;
import ghidra.program.model.listing.Function;
import ghidra.program.model.scalar.Scalar;

import java.io.BufferedReader;
import java.io.FileReader;
import java.util.HashSet;
import java.util.Set;
import java.util.TreeMap;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

public class Find1DScalarCandidatesV3 extends GhidraScript {

    @Override
    public void run() throws Exception {
        String xmlPath = "C:\\Users\\j.brophy.CORKILLSYSTEMS\\github\\Ghidra-H8-Processor\\test\\rvr\\xml\\ecuflash\\new\\21000011_1997-2001_RVR_X3_Mt__4g63t_.xml";
        Set<Integer> xmlAddrs = new HashSet<>();
        Pattern p = Pattern.compile("address=\"([0-9A-Fa-f]+)\"");
        try (BufferedReader r = new BufferedReader(new FileReader(xmlPath))) {
            String line;
            while ((line = r.readLine()) != null) {
                Matcher m = p.matcher(line);
                while (m.find()) {
                    try { xmlAddrs.add(Integer.parseInt(m.group(1), 16)); } catch (Exception e) {}
                }
            }
        }
        println("Parsed " + xmlAddrs.size() + " distinct XML addresses.");

        // Known 1D scalar constants cluster in these ROM ranges per the
        // bookmark list (0x108xx-0x10fxx, 0x114xx, 0x12fxx). RAM state is
        // 0xF0xx-0xF7xx and above -- excluded entirely since it's a totally
        // different address space concern (confirmed by DAT_/RAM naming
        // convention seen throughout this session).
        int[][] ranges = { {0x10800, 0x10FFF}, {0x11400, 0x11600}, {0x12F00, 0x13000} };

        TreeMap<Integer, String> hits = new TreeMap<>();

        InstructionIterator it = currentProgram.getListing().getInstructions(true);
        while (it.hasNext()) {
            Instruction insn = it.next();
            String mnem = insn.getMnemonicString();
            if (!mnem.toLowerCase().startsWith("mov")) continue;
            String rep = insn.toString();
            if (rep.contains("@-SP") || rep.contains("@SP")) continue; // stack push/pop
            if (rep.contains("R") == false) continue; // must load into a register

            Scalar sc = null;
            try { sc = insn.getScalar(0); } catch (Exception e) {}
            if (sc == null) continue;
            long val = sc.getUnsignedValue();

            boolean inRange = false;
            for (int[] rg : ranges) {
                if (val >= rg[0] && val <= rg[1]) { inRange = true; break; }
            }
            if (!inRange) continue;

            int addrVal = (int) val;
            boolean nearXml = false;
            for (int d = -8; d <= 8; d++) {
                if (xmlAddrs.contains(addrVal + d)) { nearXml = true; break; }
            }
            if (nearXml) continue;

            Function f = getFunctionContaining(insn.getAddress());
            String fname = f != null ? f.getName() : "???";
            String key = String.format("%06X", addrVal);
            String entry = String.format("%s @ %s [%s]", fname, insn.getAddress(), rep.trim());
            hits.merge(addrVal, entry, (o, n) -> o + " | " + n);
        }

        println("Direct mov-load candidates in known 1D ranges, not near XML: " + hits.size());
        println("");
        for (java.util.Map.Entry<Integer, String> e : hits.entrySet()) {
            println(String.format("%06X : %s", e.getKey(), e.getValue()));
        }
    }
}

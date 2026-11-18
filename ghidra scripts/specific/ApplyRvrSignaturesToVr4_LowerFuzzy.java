// ApplyRvrSignaturesToVr4_LowerFuzzy.java
// Run this with the VR4 ECU ROM as the ACTIVE program.
// Third pass, complementary to ApplyRvrSignaturesToVr4.java (score 1.0) and
// ApplyRvrSignaturesToVr4_Fuzzy.java (score 0.85-0.99).
// Reads C:\Users\j.brophy.CORKILLSYSTEMS\github\Ghidra-H8-Processor\rvr_function_signatures.csv
// plus the fixed RVR-source-address -> VR4-target-address match table below:
// 56 matches from bulk_fuzzy_match at threshold 0.70-0.85 (lower-confidence
// band). Every single pair was individually verified via side-by-side
// decompile comparison (RVR vs VR4) before inclusion -- see project chat
// history 2026-08-14. This band has a materially higher false-positive rate
// than the 0.85+ band, so nothing here was auto-applied on score alone.
//
// Candidates examined and EXCLUDED from this list (do not re-add without
// re-verifying):
//   - fueling_openloop_output_copy (RVR 21af3 -> VR4 24357): CONFIRMED FALSE
//     POSITIVE. RVR copies 2 source values into 4 destinations; VR4 zeroes
//     6 unrelated cells. Different operation, coincidental branch shape.
//   - sci1_post_handshake_status_broadcast_dispatch (RVR 28b2f -> VR4 28acb):
//     EXCLUDED, not confirmed either way. Both sides' decompiles are
//     unreliable (RVR has documented halt_baddata()/bad-jump-table warnings
//     in its own plate comment; VR4 side shows severe overlapping-instruction
//     and unreachable-block warnings). Do not trust a fuzzy score derived
//     from broken decompilation. If ever revisited, verify via raw
//     disassembly at each jump-table target, not decompile_function.
//   - o2_closed_loop_fuel_trim_compute (RVR 237a0): RVR's own plate comment
//     already flags its decompile as untrustworthy (stack-var-as-address
//     artifacts). Excluded for the same reason as above.
//   - f2ea_f2ee_composite_correction_calc (RVR 24884 -> VR4 2448e): both
//     sides show "Exceeded maximum restarts" decompiler warnings -- same
//     unreliable-decompile class, excluded.
//   - Several dozen other 0.70-0.85 candidates not listed above were spot-
//     checked and are semantically unrelated matches (short/simple functions
//     coincidentally structurally similar) -- not included here at all,
//     see chat history for the discarded list.
//
// Applies rename + prototype to the active program ONLY.
// @category H8Processor
import ghidra.app.script.GhidraScript;
import ghidra.program.model.listing.*;
import ghidra.program.model.address.Address;
import ghidra.program.model.symbol.SourceType;
import ghidra.app.util.parser.FunctionSignatureParser;
import ghidra.program.model.data.FunctionDefinitionDataType;
import ghidra.app.cmd.function.ApplyFunctionSignatureCmd;
import java.io.*;
import java.util.*;

public class ApplyRvrSignaturesToVr4_LowerFuzzy extends GhidraScript {

    static class Sig {
        String name;
        String returnType;
        String params;
        String convention;
    }

    // Minimal RFC4180-ish CSV line splitter (handles quoted fields with commas).
    List<String> splitCsvLine(String line) {
        List<String> out = new ArrayList<>();
        StringBuilder cur = new StringBuilder();
        boolean inQuotes = false;
        for (int i = 0; i < line.length(); i++) {
            char c = line.charAt(i);
            if (inQuotes) {
                if (c == '"') {
                    if (i + 1 < line.length() && line.charAt(i + 1) == '"') {
                        cur.append('"');
                        i++;
                    } else {
                        inQuotes = false;
                    }
                } else {
                    cur.append(c);
                }
            } else {
                if (c == '"') {
                    inQuotes = true;
                } else if (c == ',') {
                    out.add(cur.toString());
                    cur.setLength(0);
                } else {
                    cur.append(c);
                }
            }
        }
        out.add(cur.toString());
        return out;
    }

    @Override
    public void run() throws Exception {
        if (currentProgram == null) {
            println("ERROR: no current program");
            return;
        }
        println("Applying to: " + currentProgram.getName());

        String csvPath = "C:\\Users\\j.brophy.CORKILLSYSTEMS\\github\\Ghidra-H8-Processor\\rvr_function_signatures.csv";
        Map<String, Sig> bySrcAddr = new HashMap<>();
        try (BufferedReader r = new BufferedReader(new FileReader(csvPath))) {
            String header = r.readLine(); // skip header
            String line;
            while ((line = r.readLine()) != null) {
                if (line.trim().isEmpty()) continue;
                List<String> parts = splitCsvLine(line);
                if (parts.size() < 5) continue;
                Sig s = new Sig();
                String addr = parts.get(0);
                s.name = parts.get(1);
                s.returnType = parts.get(2);
                s.params = parts.get(3);
                s.convention = parts.get(4);
                bySrcAddr.put(addr, s);
            }
        }
        println("Loaded " + bySrcAddr.size() + " signatures from CSV");

        // src RVR address -> dst VR4 address (56 verified 0.70-0.85 fuzzy matches)
        List<String[]> pairs = new ArrayList<>();
        pairs.add(new String[]{"00014edd", "00014ecb"});
        pairs.add(new String[]{"0001504b", "00015039"});
        pairs.add(new String[]{"00014dd8", "00014dc6"});
        pairs.add(new String[]{"000149f5", "000149f5"});
        pairs.add(new String[]{"000151a2", "00015204"});
        pairs.add(new String[]{"0001528c", "000152ee"});
        pairs.add(new String[]{"000158d7", "000159bc"});
        pairs.add(new String[]{"00015a06", "00015a4a"});
        pairs.add(new String[]{"0001651f", "0001655f"});
        pairs.add(new String[]{"00018689", "00017a39"});
        pairs.add(new String[]{"000186fa", "00017a86"});
        pairs.add(new String[]{"00018bc2", "00017ff7"});
        pairs.add(new String[]{"00018cd0", "00018105"});
        pairs.add(new String[]{"000191a0", "00018600"});
        pairs.add(new String[]{"00019240", "00018800"});
        pairs.add(new String[]{"000194d0", "00018f00"});
        pairs.add(new String[]{"00019bbc", "000194d6"});
        pairs.add(new String[]{"00019d4b", "00019669"});
        pairs.add(new String[]{"0001a30c", "00019716"});
        pairs.add(new String[]{"00020b5e", "00020c58"});
        pairs.add(new String[]{"00020d8b", "00020f0b"});
        pairs.add(new String[]{"00020e00", "00021000"});
        pairs.add(new String[]{"00020efa", "000210bd"});
        pairs.add(new String[]{"00020fe5", "000211a8"});
        pairs.add(new String[]{"000211fb", "00021380"});
        pairs.add(new String[]{"000217e3", "000217a2"});
        pairs.add(new String[]{"00022442", "0002241a"});
        pairs.add(new String[]{"0002297f", "000229c6"});
        pairs.add(new String[]{"00022a57", "00022a76"});
        pairs.add(new String[]{"00022a97", "00022aaa"});
        pairs.add(new String[]{"00022ab3", "00022ac6"});
        pairs.add(new String[]{"00023027", "00023021"});
        pairs.add(new String[]{"0002339d", "00023369"});
        pairs.add(new String[]{"00023d4f", "00023d3d"});
        pairs.add(new String[]{"00023fc7", "00024231"});
        pairs.add(new String[]{"000245af", "0002425b"});
        pairs.add(new String[]{"000249f2", "00024656"});
        pairs.add(new String[]{"00024fc9", "00024d45"});
        pairs.add(new String[]{"0002586f", "000255da"});
        pairs.add(new String[]{"00025d27", "000259cb"});
        pairs.add(new String[]{"0002629b", "00025f93"});
        pairs.add(new String[]{"0002634d", "00026045"});
        pairs.add(new String[]{"00026914", "000265ce"});
        pairs.add(new String[]{"00026f82", "00026d7c"});
        pairs.add(new String[]{"0002705b", "00026e55"});
        pairs.add(new String[]{"000270b5", "00026eaf"});
        pairs.add(new String[]{"0002725d", "00027043"});
        pairs.add(new String[]{"000272c6", "000270a0"});
        pairs.add(new String[]{"00027597", "00027358"});
        pairs.add(new String[]{"000275c2", "0002737f"});
        pairs.add(new String[]{"00027933", "000276d4"});
        pairs.add(new String[]{"00027959", "000276fa"});
        pairs.add(new String[]{"00027a20", "000277b1"});
        pairs.add(new String[]{"00027b9d", "0002792e"});
        pairs.add(new String[]{"00027c99", "00027b19"});
        pairs.add(new String[]{"00028869", "00028803"});

        int renamed = 0, prototyped = 0, skipped = 0, errors = 0;

        for (String[] pair : pairs) {
            if (monitor.isCancelled()) break;
            String srcAddrStr = pair[0];
            String dstAddrStr = pair[1];
            Sig sig = bySrcAddr.get(srcAddrStr);
            if (sig == null) {
                println("SKIP (no CSV entry) src=" + srcAddrStr);
                skipped++;
                continue;
            }
            Address dstAddr = currentProgram.getAddressFactory().getAddress(dstAddrStr);
            Function dstFunc = currentProgram.getFunctionManager().getFunctionAt(dstAddr);
            if (dstFunc == null) {
                println("SKIP (no dst function) dst=" + dstAddrStr);
                skipped++;
                continue;
            }
            try {
                dstFunc.setName(sig.name, SourceType.USER_DEFINED);
                renamed++;

                if (sig.convention != null && !sig.convention.isEmpty()
                        && sig.returnType != null && !sig.returnType.isEmpty()
                        && sig.params != null) {
                    String protoStr = sig.returnType + " " + sig.name + "(" + sig.params + ")";
                    try {
                        FunctionSignatureParser parser = new FunctionSignatureParser(
                                currentProgram.getDataTypeManager(), null);
                        FunctionDefinitionDataType fddt = parser.parse(
                                dstFunc.getSignature(), protoStr);
                        ApplyFunctionSignatureCmd cmd = new ApplyFunctionSignatureCmd(
                                dstAddr, fddt, SourceType.USER_DEFINED);
                        if (cmd.applyTo(currentProgram)) {
                            dstFunc.setCallingConvention(sig.convention);
                            prototyped++;
                        } else {
                            println("WARN prototype apply failed for " + sig.name + ": " + cmd.getStatusMsg());
                        }
                    } catch (Exception pe) {
                        println("WARN prototype parse/apply exception for " + sig.name + ": " + pe.getMessage());
                    }
                }
            } catch (Exception e) {
                println("ERROR on " + srcAddrStr + " -> " + dstAddrStr + ": " + e.getMessage());
                errors++;
            }
        }

        println("DONE. renamed=" + renamed + " prototyped=" + prototyped
                + " skipped=" + skipped + " errors=" + errors);
    }
}

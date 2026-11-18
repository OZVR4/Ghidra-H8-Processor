// ApplyRvrSignaturesToVr4_Fuzzy.java
// Run this with the VR4 ECU ROM as the ACTIVE program.
// Second pass, complementary to ApplyRvrSignaturesToVr4.java.
// Reads C:\Users\j.brophy.CORKILLSYSTEMS\github\Ghidra-H8-Processor\rvr_function_signatures.csv
// plus the fixed RVR-source-address -> VR4-target-address match table below:
// 109 matches from bulk_fuzzy_match at threshold 0.85 (score < 1.0, i.e. NOT
// byte-identical but structurally identical logic with only embedded RAM
// addresses / table bases / constants differing between the two ROM builds).
// Every pair below was individually verified via side-by-side decompile
// comparison (RVR vs VR4) before inclusion in this list -- see project chat
// history 2026-08-14 for the verification pass. Excluded from the original
// 117-candidate 0.85+ set:
//   - mul_u16_dbl_sat, sci_fifo_f58a_advance_and_load_fed3,
//     fedd5_fifo_rx_store_byte: real functions but VR4 target already
//     correctly named by a prior pass; applying would overwrite a correct name
//   - engine_mode_f512_bit4_check, efbc_timer_target_select_from_f2c6_thresholds,
//     f1fe_bit1_clear_check: CONFIRMED FALSE POSITIVES -- decompiled and found
//     to have genuinely different logic (different registers/bitmasks/branch
//     structure) despite high fuzzy-match score; short/simple functions are
//     the highest false-positive-risk category for this technique
//   - rpm_sync_ready_check: unverified due to large address discontinuity,
//     excluded out of caution rather than confirmed bad
//   - f110_f17a_threshold_gate_check: verified GOOD but target 00024e87 was
//     already renamed by the first pass (f0ec_bit1_threshold_gate_check from
//     RVR 0002510f) -- would collide/overwrite, so dropped here
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

public class ApplyRvrSignaturesToVr4_Fuzzy extends GhidraScript {

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

        // src RVR address -> dst VR4 address (109 verified 0.85-0.99 fuzzy matches)
        List<String[]> pairs = new ArrayList<>();
        pairs.add(new String[]{"00014854", "00014854"});
        pairs.add(new String[]{"0001497b", "0001497b"});
        pairs.add(new String[]{"00014d6f", "00014d5d"});
        pairs.add(new String[]{"0001556d", "000155cf"});
        pairs.add(new String[]{"0001578f", "00015847"});
        pairs.add(new String[]{"000158a7", "0001594b"});
        pairs.add(new String[]{"00015b3f", "00015b83"});
        pairs.add(new String[]{"00015c0a", "00015c4e"});
        pairs.add(new String[]{"00015dd0", "00015e14"});
        pairs.add(new String[]{"00015e01", "00015e45"});
        pairs.add(new String[]{"00015e79", "00015ebd"});
        pairs.add(new String[]{"00016410", "00016454"});
        pairs.add(new String[]{"0001655a", "0001659a"});
        pairs.add(new String[]{"00016631", "00016671"});
        pairs.add(new String[]{"000166d7", "00016717"});
        pairs.add(new String[]{"0001682b", "000167bc"});
        pairs.add(new String[]{"00016890", "00016820"});
        pairs.add(new String[]{"00016f6b", "00016efa"});
        pairs.add(new String[]{"00017160", "000170cf"});
        pairs.add(new String[]{"000171c3", "00017132"});
        pairs.add(new String[]{"000174d4", "00017414"});
        pairs.add(new String[]{"00018848", "00017c8b"});
        pairs.add(new String[]{"00018a55", "00017e98"});
        pairs.add(new String[]{"000192ac", "0001886c"});
        pairs.add(new String[]{"00019364", "00018914"});
        pairs.add(new String[]{"00019560", "00018f90"});
        pairs.add(new String[]{"00019b5e", "00019472"});
        pairs.add(new String[]{"00019cb3", "000195d1"});
        pairs.add(new String[]{"0001a15c", "000290e1"});
        pairs.add(new String[]{"0001a31a", "00019724"});
        pairs.add(new String[]{"0001a36f", "00019779"});
        pairs.add(new String[]{"00020024", "00020024"});
        pairs.add(new String[]{"00020f28", "000210eb"});
        pairs.add(new String[]{"000211c8", "0002134d"});
        pairs.add(new String[]{"0002127f", "000213e2"});
        pairs.add(new String[]{"00021414", "0002154b"});
        pairs.add(new String[]{"0002153b", "0002162e"});
        pairs.add(new String[]{"00021928", "000218e7"});
        pairs.add(new String[]{"00021942", "00021909"});
        pairs.add(new String[]{"00021aa5", "00021a75"});
        pairs.add(new String[]{"00021b2d", "00021aeb"});
        pairs.add(new String[]{"00021df5", "00021d7f"});
        pairs.add(new String[]{"00021fa0", "00021f86"});
        pairs.add(new String[]{"00021fff", "00021fe5"});
        pairs.add(new String[]{"0002204a", "00022030"});
        pairs.add(new String[]{"0002207f", "00022065"});
        pairs.add(new String[]{"00022111", "000220e9"});
        pairs.add(new String[]{"00022225", "00022209"});
        pairs.add(new String[]{"00022386", "0002238f"});
        pairs.add(new String[]{"000224a6", "00022462"});
        pairs.add(new String[]{"000224e9", "000224a5"});
        pairs.add(new String[]{"000225be", "0002258a"});
        pairs.add(new String[]{"00022746", "00022704"});
        pairs.add(new String[]{"00022bb3", "00022bca"});
        pairs.add(new String[]{"00022c11", "00022c28"});
        pairs.add(new String[]{"00022c60", "00022c71"});
        pairs.add(new String[]{"00022c89", "00022c9a"});
        pairs.add(new String[]{"00022d12", "00022d23"});
        pairs.add(new String[]{"00023193", "0002317d"});
        pairs.add(new String[]{"000231f4", "000231d8"});
        pairs.add(new String[]{"000232c3", "000232a7"});
        pairs.add(new String[]{"000238d8", "0002377f"});
        pairs.add(new String[]{"00024d3a", "000249fc"});
        pairs.add(new String[]{"00024e80", "00024c00"});
        pairs.add(new String[]{"00024fa0", "00024d1c"});
        pairs.add(new String[]{"0002502d", "00024da9"});
        pairs.add(new String[]{"000250fb", "00024e73"});
        pairs.add(new String[]{"000252d3", "00025055"});
        pairs.add(new String[]{"000252f9", "0002507b"});
        pairs.add(new String[]{"000253e1", "0002515b"});
        pairs.add(new String[]{"0002547e", "000251f8"});
        pairs.add(new String[]{"00025544", "000252be"});
        pairs.add(new String[]{"000255c0", "0002533a"});
        pairs.add(new String[]{"000259df", "000256c8"});
        pairs.add(new String[]{"00025a59", "00025742"});
        pairs.add(new String[]{"00025c15", "000258fa"});
        pairs.add(new String[]{"00025e8b", "00025b4e"});
        pairs.add(new String[]{"00025f45", "00025c08"});
        pairs.add(new String[]{"0002601a", "00025cdd"});
        pairs.add(new String[]{"00026124", "00025e24"});
        pairs.add(new String[]{"00026224", "00025f20"});
        pairs.add(new String[]{"00026251", "00025f49"});
        pairs.add(new String[]{"0002655d", "00026255"});
        pairs.add(new String[]{"000266e2", "000263c2"});
        pairs.add(new String[]{"00026a99", "00026823"});
        pairs.add(new String[]{"00026bc7", "000269c3"});
        pairs.add(new String[]{"00026d4d", "00026b49"});
        pairs.add(new String[]{"00026f67", "00026d61"});
        pairs.add(new String[]{"00027032", "00026e2c"});
        pairs.add(new String[]{"0002721a", "00027002"});
        pairs.add(new String[]{"000273b8", "00027192"});
        pairs.add(new String[]{"0002785a", "00027607"});
        pairs.add(new String[]{"00027990", "00027721"});
        pairs.add(new String[]{"00027a81", "00027812"});
        pairs.add(new String[]{"00027bc6", "000265a9"});
        pairs.add(new String[]{"00027c80", "00027b00"});
        pairs.add(new String[]{"00027d55", "00027bd5"});
        pairs.add(new String[]{"00027e1a", "00027c9a"});
        pairs.add(new String[]{"00027f60", "00027e4d"});
        pairs.add(new String[]{"000282c1", "0002824c"});
        pairs.add(new String[]{"00028557", "000284dc"});
        pairs.add(new String[]{"0002a7f7", "0002ac07"});
        pairs.add(new String[]{"0002a83b", "0002ac4b"});
        pairs.add(new String[]{"0002a8d3", "0002ace3"});
        pairs.add(new String[]{"0002a997", "0002ae0f"});
        pairs.add(new String[]{"00029c68", "0002a191"});
        pairs.add(new String[]{"00029f40", "0002a44b"});
        pairs.add(new String[]{"0002c16e", "0002c15e"});
        pairs.add(new String[]{"000149c8", "000149c8"});

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

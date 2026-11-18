// ApplyRvrSignaturesToEvo5.java
// Run this with 22580006_EVO5_Stock.hex as the ACTIVE program.
// Reads C:\Users\j.brophy.CORKILLSYSTEMS\github\Ghidra-H8-Processor\rvr_function_signatures.csv
// (produced by ExportFunctionSignatures.java against RVR) plus the fixed
// RVR-source-address -> EVO5-target-address match table below (260 unique
// 1:1 matches from bulk_fuzzy_match at score 1.0; collisions where
// multiple RVR names mapped to one EVO5 address were excluded per
// 2026-08-03 decision). Applies rename + prototype to EVO5 ONLY --
// never opens a second program, so it can't hit the "wrong/detached
// program" bug from the two-program script.
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

public class ApplyRvrSignaturesToEvo5 extends GhidraScript {

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

        // src RVR address -> dst EVO5 address (260 unique 1:1 matches, score 1.0)
        List<String[]> pairs = new ArrayList<>();
        pairs.add(new String[]{"00014000", "00014000"});
        pairs.add(new String[]{"0001401f", "0001401f"});
        pairs.add(new String[]{"00014036", "00014036"});
        pairs.add(new String[]{"00014045", "00014045"});
        pairs.add(new String[]{"0001405c", "0001405c"});
        pairs.add(new String[]{"0001406f", "0001406f"});
        pairs.add(new String[]{"000140c0", "000140c0"});
        pairs.add(new String[]{"000140dc", "000140dc"});
        pairs.add(new String[]{"0001412b", "0001412b"});
        pairs.add(new String[]{"00014151", "00014151"});
        pairs.add(new String[]{"0001417d", "0001417d"});
        pairs.add(new String[]{"000141ba", "000141ba"});
        pairs.add(new String[]{"000141dc", "000141dc"});
        pairs.add(new String[]{"00014219", "00014219"});
        pairs.add(new String[]{"00014247", "00014247"});
        pairs.add(new String[]{"00014293", "00014293"});
        pairs.add(new String[]{"000142b4", "000142b4"});
        pairs.add(new String[]{"000142f3", "000142f3"});
        pairs.add(new String[]{"0001436c", "0001436c"});
        pairs.add(new String[]{"00014387", "00014387"});
        pairs.add(new String[]{"000143bf", "000143bf"});
        pairs.add(new String[]{"0001442d", "0001442d"});
        pairs.add(new String[]{"0001443c", "0001443c"});
        pairs.add(new String[]{"0001444c", "0001444c"});
        pairs.add(new String[]{"00014471", "00014471"});
        pairs.add(new String[]{"000144b4", "000144b4"});
        pairs.add(new String[]{"000144bf", "000144bf"});
        pairs.add(new String[]{"000144c6", "000144c6"});
        pairs.add(new String[]{"000144d1", "000144d1"});
        pairs.add(new String[]{"000144da", "000144da"});
        pairs.add(new String[]{"000144ef", "000144ef"});
        pairs.add(new String[]{"0001450b", "0001450b"});
        pairs.add(new String[]{"0001451a", "0001451a"});
        pairs.add(new String[]{"0001452d", "0001452d"});
        pairs.add(new String[]{"0001454c", "0001454c"});
        pairs.add(new String[]{"0001456d", "0001456d"});
        pairs.add(new String[]{"00014599", "00014599"});
        pairs.add(new String[]{"000145c5", "000145c5"});
        pairs.add(new String[]{"00014735", "00014735"});
        pairs.add(new String[]{"000147c6", "000147c6"});
        pairs.add(new String[]{"0001481c", "0001481c"});
        pairs.add(new String[]{"00015319", "00015319"});
        pairs.add(new String[]{"00015341", "00015341"});
        pairs.add(new String[]{"0001553d", "0001553d"});
        pairs.add(new String[]{"00015555", "00015555"});
        pairs.add(new String[]{"00015561", "00015561"});
        pairs.add(new String[]{"000156c2", "000156c2"});
        pairs.add(new String[]{"000156c8", "000156c8"});
        pairs.add(new String[]{"000156ce", "000156ce"});
        pairs.add(new String[]{"00015857", "00015857"});
        pairs.add(new String[]{"00015877", "00015877"});
        pairs.add(new String[]{"00015897", "00015897"});
        pairs.add(new String[]{"0001589d", "0001589d"});
        pairs.add(new String[]{"000158a7", "000158a7"});
        pairs.add(new String[]{"000158d7", "000158d7"});
        pairs.add(new String[]{"00015a3a", "00015a3a"});
        pairs.add(new String[]{"00015a76", "00015a76"});
        pairs.add(new String[]{"00015aff", "00015aff"});
        pairs.add(new String[]{"00015b0d", "00015b0d"});
        pairs.add(new String[]{"00015b90", "00015b90"});
        pairs.add(new String[]{"00015c6c", "00015c6c"});
        pairs.add(new String[]{"00015d36", "00015d36"});
        pairs.add(new String[]{"00015dd0", "00015dd0"});
        pairs.add(new String[]{"00015e01", "00015e01"});
        pairs.add(new String[]{"00015e17", "00015e17"});
        pairs.add(new String[]{"00015edb", "00015edb"});
        pairs.add(new String[]{"00015f1a", "00015f1a"});
        pairs.add(new String[]{"00015f71", "00015f71"});
        pairs.add(new String[]{"00016029", "00016029"});
        pairs.add(new String[]{"00016059", "00016059"});
        pairs.add(new String[]{"0001609d", "0001609d"});
        pairs.add(new String[]{"000160d9", "000160d9"});
        pairs.add(new String[]{"00016110", "00016110"});
        pairs.add(new String[]{"00016140", "00016140"});
        pairs.add(new String[]{"0001615c", "0001615c"});
        pairs.add(new String[]{"000161e6", "000161e6"});
        pairs.add(new String[]{"00016222", "00016222"});
        pairs.add(new String[]{"0001626c", "0001626c"});
        pairs.add(new String[]{"00016297", "00016297"});
        pairs.add(new String[]{"00016410", "00016410"});
        pairs.add(new String[]{"0001643a", "0001643a"});
        pairs.add(new String[]{"0001645e", "0001645e"});
        pairs.add(new String[]{"0001646c", "0001646c"});
        pairs.add(new String[]{"00016488", "00016488"});
        pairs.add(new String[]{"000164b7", "000164ce"});
        pairs.add(new String[]{"000164da", "000164f1"});
        pairs.add(new String[]{"0001651f", "00016536"});
        pairs.add(new String[]{"0001655a", "00016571"});
        pairs.add(new String[]{"00016600", "00016617"});
        pairs.add(new String[]{"00016631", "00016648"});
        pairs.add(new String[]{"00016680", "00016696"});
        pairs.add(new String[]{"000166d7", "000166ed"});
        pairs.add(new String[]{"00016716", "0001672c"});
        pairs.add(new String[]{"00016865", "0001687b"});
        pairs.add(new String[]{"00016890", "000168a6"});
        pairs.add(new String[]{"000168d9", "000168ef"});
        pairs.add(new String[]{"00016aa3", "00016ab9"});
        pairs.add(new String[]{"00016b83", "00016b99"});
        pairs.add(new String[]{"00016c09", "00016c1f"});
        pairs.add(new String[]{"00016c13", "00016c29"});
        pairs.add(new String[]{"00016c33", "00016c49"});
        pairs.add(new String[]{"00016c4f", "00016c65"});
        pairs.add(new String[]{"00016d40", "00016d56"});
        pairs.add(new String[]{"00016e1c", "00016e32"});
        pairs.add(new String[]{"00016f3b", "00016f51"});
        pairs.add(new String[]{"00017154", "00017174"});
        pairs.add(new String[]{"00017160", "00017180"});
        pairs.add(new String[]{"000171c3", "000171e3"});
        pairs.add(new String[]{"00017206", "00017226"});
        pairs.add(new String[]{"0001722b", "0001724b"});
        pairs.add(new String[]{"00017256", "00017276"});
        pairs.add(new String[]{"000174a1", "000174a1"});
        pairs.add(new String[]{"000174d4", "000174d4"});
        pairs.add(new String[]{"0001792f", "0001792f"});
        pairs.add(new String[]{"0001799f", "0001799f"});
        pairs.add(new String[]{"0001842b", "0001842b"});
        pairs.add(new String[]{"000185de", "000185de"});
        pairs.add(new String[]{"00018628", "00018628"});
        pairs.add(new String[]{"00018689", "00018689"});
        pairs.add(new String[]{"000189c0", "000189c0"});
        pairs.add(new String[]{"00018a55", "00018a55"});
        pairs.add(new String[]{"00018ae5", "00018ae5"});
        pairs.add(new String[]{"00018b17", "00018b17"});
        pairs.add(new String[]{"00018b63", "00018b63"});
        pairs.add(new String[]{"00018e93", "00018e93"});
        pairs.add(new String[]{"000191f0", "00019270"});
        pairs.add(new String[]{"000192ac", "0001932c"});
        pairs.add(new String[]{"00019b2a", "00019a4a"});
        pairs.add(new String[]{"00019b5e", "00019a7e"});
        pairs.add(new String[]{"00019d0e", "00019c2e"});
        pairs.add(new String[]{"00019d4b", "00019c6b"});
        pairs.add(new String[]{"00019f35", "00019fb5"});
        pairs.add(new String[]{"00019f77", "00019ff7"});
        pairs.add(new String[]{"00019ff2", "0001a072"});
        pairs.add(new String[]{"0001a0a7", "0001a127"});
        pairs.add(new String[]{"0001a130", "0001a1b0"});
        pairs.add(new String[]{"0001a15c", "0001a1dc"});
        pairs.add(new String[]{"0001a1b9", "0001a239"});
        pairs.add(new String[]{"0001a1cc", "0001a24c"});
        pairs.add(new String[]{"0001a1e5", "0001a265"});
        pairs.add(new String[]{"0001a205", "0001a285"});
        pairs.add(new String[]{"0001a23e", "0001a2be"});
        pairs.add(new String[]{"0001a254", "0001a2d4"});
        pairs.add(new String[]{"0001a261", "0001a2e1"});
        pairs.add(new String[]{"0001a2a4", "0001a324"});
        pairs.add(new String[]{"0001a2ba", "0001a33a"});
        pairs.add(new String[]{"0001a2e6", "0001a366"});
        pairs.add(new String[]{"0001a31a", "0001a41a"});
        pairs.add(new String[]{"0001a36f", "0001a46f"});
        pairs.add(new String[]{"00020024", "00020024"});
        pairs.add(new String[]{"00020048", "00020048"});
        pairs.add(new String[]{"000201c0", "000201c0"});
        pairs.add(new String[]{"000202b2", "000202b2"});
        pairs.add(new String[]{"00020379", "00020379"});
        pairs.add(new String[]{"0002039f", "0002039f"});
        pairs.add(new String[]{"000203bd", "000203bd"});
        pairs.add(new String[]{"000205a7", "000205a7"});
        pairs.add(new String[]{"00020646", "00020646"});
        pairs.add(new String[]{"000208c6", "000208c6"});
        pairs.add(new String[]{"00020910", "00020910"});
        pairs.add(new String[]{"0002093c", "0002093c"});
        pairs.add(new String[]{"00020e00", "00020e00"});
        pairs.add(new String[]{"00020fb1", "00020fb1"});
        pairs.add(new String[]{"000211a1", "000211cb"});
        pairs.add(new String[]{"0002144b", "00021475"});
        pairs.add(new String[]{"000215c8", "000215ec"});
        pairs.add(new String[]{"000217b6", "000217da"});
        pairs.add(new String[]{"0002180b", "0002183d"});
        pairs.add(new String[]{"00021928", "0002195a"});
        pairs.add(new String[]{"00021962", "00021994"});
        pairs.add(new String[]{"00021af3", "00021b25"});
        pairs.add(new String[]{"00021b8f", "00021bc1"});
        pairs.add(new String[]{"00021f00", "00021f80"});
        pairs.add(new String[]{"00021f21", "00021fa1"});
        pairs.add(new String[]{"00021f70", "00021ff4"});
        pairs.add(new String[]{"00021f92", "00022016"});
        pairs.add(new String[]{"00021fba", "0002203e"});
        pairs.add(new String[]{"00022013", "00022097"});
        pairs.add(new String[]{"0002204a", "000220ce"});
        pairs.add(new String[]{"0002207f", "00022103"});
        pairs.add(new String[]{"000220e9", "0002216d"});
        pairs.add(new String[]{"0002214a", "000221ce"});
        pairs.add(new String[]{"00022193", "00022217"});
        pairs.add(new String[]{"00022197", "0002221b"});
        pairs.add(new String[]{"000221f0", "00022274"});
        pairs.add(new String[]{"00022225", "000222a9"});
        pairs.add(new String[]{"00022256", "000222da"});
        pairs.add(new String[]{"00022377", "000223fb"});
        pairs.add(new String[]{"000223a0", "00022424"});
        pairs.add(new String[]{"00022442", "0002249b"});
        pairs.add(new String[]{"000224ba", "00022513"});
        pairs.add(new String[]{"000224e9", "00022542"});
        pairs.add(new String[]{"000226de", "00022737"});
        pairs.add(new String[]{"0002279a", "00022801"});
        pairs.add(new String[]{"000228ed", "00022954"});
        pairs.add(new String[]{"000229c0", "00022a2d"});
        pairs.add(new String[]{"00022c11", "00022c7e"});
        pairs.add(new String[]{"00022c74", "00022ce1"});
        pairs.add(new String[]{"00022d12", "00022d7f"});
        pairs.add(new String[]{"00022f16", "00022f5f"});
        pairs.add(new String[]{"00022f82", "00022fcb"});
        pairs.add(new String[]{"00023006", "00023086"});
        pairs.add(new String[]{"00023027", "000230a7"});
        pairs.add(new String[]{"00023145", "000231c5"});
        pairs.add(new String[]{"00023193", "00023213"});
        pairs.add(new String[]{"00023346", "000233c6"});
        pairs.add(new String[]{"00023371", "000233f1"});
        pairs.add(new String[]{"00023424", "000234a4"});
        pairs.add(new String[]{"00023606", "00023686"});
        pairs.add(new String[]{"00023660", "000236e0"});
        pairs.add(new String[]{"0002369a", "0002371a"});
        pairs.add(new String[]{"00023bf9", "00023c91"});
        pairs.add(new String[]{"000242c1", "0002433c"});
        pairs.add(new String[]{"00024596", "00024611"});
        pairs.add(new String[]{"00024680", "000246fb"});
        pairs.add(new String[]{"000246ae", "00024729"});
        pairs.add(new String[]{"00024f76", "00024ff6"});
        pairs.add(new String[]{"00024fb4", "00025034"});
        pairs.add(new String[]{"00025057", "000250d7"});
        pairs.add(new String[]{"0002510f", "0002518f"});
        pairs.add(new String[]{"0002513a", "000251ba"});
        pairs.add(new String[]{"0002519e", "0002521e"});
        pairs.add(new String[]{"00025277", "000252f7"});
        pairs.add(new String[]{"00025378", "000253f8"});
        pairs.add(new String[]{"000254d7", "00025557"});
        pairs.add(new String[]{"0002555e", "000255de"});
        pairs.add(new String[]{"000255d4", "00025654"});
        pairs.add(new String[]{"000258c4", "0002592c"});
        pairs.add(new String[]{"00025cff", "00025d59"});
        pairs.add(new String[]{"00026271", "0002628f"});
        pairs.add(new String[]{"000268b3", "000268f1"});
        pairs.add(new String[]{"000268c4", "00026902"});
        pairs.add(new String[]{"0002697c", "000269ba"});
        pairs.add(new String[]{"00026a63", "00026aa7"});
        pairs.add(new String[]{"00026aad", "00026af1"});
        pairs.add(new String[]{"00026b8d", "00026c03"});
        pairs.add(new String[]{"00026c2e", "00026ca4"});
        pairs.add(new String[]{"00026ca4", "00026d1a"});
        pairs.add(new String[]{"00026d86", "00026dfc"});
        pairs.add(new String[]{"00026dfa", "00026e70"});
        pairs.add(new String[]{"00026e0e", "00026e84"});
        pairs.add(new String[]{"0002710d", "00027183"});
        pairs.add(new String[]{"000271f8", "0002726e"});
        pairs.add(new String[]{"0002721a", "00027290"});
        pairs.add(new String[]{"0002739e", "00027414"});
        pairs.add(new String[]{"00027428", "0002749e"});
        pairs.add(new String[]{"00027792", "000277de"});
        pairs.add(new String[]{"000277b8", "00027804"});
        pairs.add(new String[]{"000277de", "0002782a"});
        pairs.add(new String[]{"0002785a", "000278a6"});
        pairs.add(new String[]{"000278a4", "000278f0"});
        pairs.add(new String[]{"000278ca", "00027916"});
        pairs.add(new String[]{"00027c6d", "00027cbf"});
        pairs.add(new String[]{"00027c86", "00027d06"});
        pairs.add(new String[]{"00027d33", "00027db3"});
        pairs.add(new String[]{"00028b1b", "00028b0d"});
        pairs.add(new String[]{"00028f82", "00028d82"});
        pairs.add(new String[]{"00028fbc", "00028dbc"});
        pairs.add(new String[]{"0002a796", "0002a54e"});
        pairs.add(new String[]{"0002a980", "0002a700"});

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

// ApplyRvrSignaturesToVr4.java
// Run this with the VR4 ECU ROM as the ACTIVE program.
// Reads C:\Users\j.brophy.CORKILLSYSTEMS\github\Ghidra-H8-Processor\rvr_function_signatures.csv
// (produced by ExportFunctionSignatures.java against RVR) plus the fixed
// RVR-source-address -> VR4-target-address match table below (228 unique
// 1:1 matches from bulk_fuzzy_match at score 1.0, merged from matches_p1.json
// + matches_p2.json; collisions where multiple RVR names mapped to one VR4
// address were excluded, same rule used for the EVO5 pass).
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

public class ApplyRvrSignaturesToVr4 extends GhidraScript {

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

        // src RVR address -> dst VR4 address (228 unique 1:1 matches, score 1.0)
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
        pairs.add(new String[]{"000143e8", "000143e8"});
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
        pairs.add(new String[]{"00014502", "00014502"});
        pairs.add(new String[]{"0001450b", "0001450b"});
        pairs.add(new String[]{"0001451a", "0001451a"});
        pairs.add(new String[]{"0001452d", "0001452d"});
        pairs.add(new String[]{"0001454c", "0001454c"});
        pairs.add(new String[]{"0001456d", "0001456d"});
        pairs.add(new String[]{"00014599", "00014599"});
        pairs.add(new String[]{"000145c5", "000145c5"});
        pairs.add(new String[]{"0001460d", "0001460d"});
        pairs.add(new String[]{"00014656", "00014656"});
        pairs.add(new String[]{"00014735", "00014735"});
        pairs.add(new String[]{"000147c6", "000147c6"});
        pairs.add(new String[]{"0001481c", "0001481c"});
        pairs.add(new String[]{"00014838", "00014838"});
        pairs.add(new String[]{"000148a2", "000148a2"});
        pairs.add(new String[]{"00015319", "0001537b"});
        pairs.add(new String[]{"00015341", "000153a3"});
        pairs.add(new String[]{"0001553d", "0001559f"});
        pairs.add(new String[]{"00015555", "000155b7"});
        pairs.add(new String[]{"00015561", "000155c3"});
        pairs.add(new String[]{"000156c2", "00015715"});
        pairs.add(new String[]{"000156c8", "0001571b"});
        pairs.add(new String[]{"00015857", "000158fb"});
        pairs.add(new String[]{"00015877", "0001591b"});
        pairs.add(new String[]{"00015897", "0001593b"});
        pairs.add(new String[]{"0001589d", "00015941"});
        pairs.add(new String[]{"00015a3a", "00015a7e"});
        pairs.add(new String[]{"00015a6c", "00015ab0"});
        pairs.add(new String[]{"00015a76", "00015aba"});
        pairs.add(new String[]{"00015aff", "00015b43"});
        pairs.add(new String[]{"00015b0d", "00015b51"});
        pairs.add(new String[]{"00015b90", "00015bd4"});
        pairs.add(new String[]{"00015c6c", "00015cb0"});
        pairs.add(new String[]{"00015cbd", "00015d01"});
        pairs.add(new String[]{"00015d36", "00015d7a"});
        pairs.add(new String[]{"00015e17", "00015e5b"});
        pairs.add(new String[]{"00015edb", "00015f1f"});
        pairs.add(new String[]{"00015f1a", "00015f5e"});
        pairs.add(new String[]{"00015f71", "00015fb5"});
        pairs.add(new String[]{"00016029", "0001606d"});
        pairs.add(new String[]{"00016059", "0001609d"});
        pairs.add(new String[]{"0001609d", "000160e1"});
        pairs.add(new String[]{"000160d9", "0001611d"});
        pairs.add(new String[]{"00016110", "00016154"});
        pairs.add(new String[]{"00016140", "00016184"});
        pairs.add(new String[]{"0001615c", "000161a0"});
        pairs.add(new String[]{"000161e6", "0001622a"});
        pairs.add(new String[]{"00016222", "00016266"});
        pairs.add(new String[]{"0001626c", "000162b0"});
        pairs.add(new String[]{"00016297", "000162db"});
        pairs.add(new String[]{"000163eb", "0001642f"});
        pairs.add(new String[]{"0001645e", "0001649e"});
        pairs.add(new String[]{"0001646c", "000164ac"});
        pairs.add(new String[]{"00016488", "000164c8"});
        pairs.add(new String[]{"000164b7", "000164f7"});
        pairs.add(new String[]{"000164da", "0001651a"});
        pairs.add(new String[]{"00016600", "00016640"});
        pairs.add(new String[]{"00016680", "000166c0"});
        pairs.add(new String[]{"00016716", "00016756"});
        pairs.add(new String[]{"00016865", "000167f5"});
        pairs.add(new String[]{"000168d9", "00016869"});
        pairs.add(new String[]{"00016aa3", "00016a32"});
        pairs.add(new String[]{"00016b83", "00016b12"});
        pairs.add(new String[]{"00016c09", "00016b98"});
        pairs.add(new String[]{"00016c13", "00016ba2"});
        pairs.add(new String[]{"00016c33", "00016bc2"});
        pairs.add(new String[]{"00016c4f", "00016bde"});
        pairs.add(new String[]{"00016d40", "00016ccf"});
        pairs.add(new String[]{"00016e1c", "00016dab"});
        pairs.add(new String[]{"00016f23", "00016eb2"});
        pairs.add(new String[]{"00016f3b", "00016eca"});
        pairs.add(new String[]{"00016f53", "00016ee2"});
        pairs.add(new String[]{"00017206", "00017175"});
        pairs.add(new String[]{"0001722b", "0001719a"});
        pairs.add(new String[]{"00017256", "000171c5"});
        pairs.add(new String[]{"000174a1", "0001731a"});
        pairs.add(new String[]{"0001799f", "00017768"});
        pairs.add(new String[]{"00018628", "00017928"});
        pairs.add(new String[]{"000189c0", "00017e03"});
        pairs.add(new String[]{"00018ae5", "00017f1a"});
        pairs.add(new String[]{"00018b17", "00017f4c"});
        pairs.add(new String[]{"00018b63", "00017f98"});
        pairs.add(new String[]{"00018dca", "0001821a"});
        pairs.add(new String[]{"000191f0", "00018700"});
        pairs.add(new String[]{"0001952c", "00018f5c"});
        pairs.add(new String[]{"00019b2a", "0001943e"});
        pairs.add(new String[]{"00019d0e", "0001962c"});
        pairs.add(new String[]{"00019f35", "00028ea3"});
        pairs.add(new String[]{"00019f49", "00028eb7"});
        pairs.add(new String[]{"00019f77", "00028ee5"});
        pairs.add(new String[]{"00019ff2", "00028f77"});
        pairs.add(new String[]{"0001a042", "00028fc7"});
        pairs.add(new String[]{"0001a0a7", "0002902c"});
        pairs.add(new String[]{"0001a0c4", "00029049"});
        pairs.add(new String[]{"0001a0fa", "0002907f"});
        pairs.add(new String[]{"0001a130", "000290b5"});
        pairs.add(new String[]{"0001a146", "000290cb"});
        pairs.add(new String[]{"0001a182", "00029100"});
        pairs.add(new String[]{"0001a1b9", "00029137"});
        pairs.add(new String[]{"0001a1cc", "0002914a"});
        pairs.add(new String[]{"0001a1e5", "00029163"});
        pairs.add(new String[]{"0001a205", "00029183"});
        pairs.add(new String[]{"0001a23e", "000291bc"});
        pairs.add(new String[]{"0001a254", "000291d2"});
        pairs.add(new String[]{"0001a261", "000291df"});
        pairs.add(new String[]{"0001a2ba", "00029222"});
        pairs.add(new String[]{"0001a2e6", "0002924e"});
        pairs.add(new String[]{"00020000", "00020000"});
        pairs.add(new String[]{"00020fb1", "00021174"});
        pairs.add(new String[]{"000211a1", "00021326"});
        pairs.add(new String[]{"0002144b", "00021582"});
        pairs.add(new String[]{"000214ea", "000215dd"});
        pairs.add(new String[]{"000215c8", "000216bb"});
        pairs.add(new String[]{"0002180b", "000217ca"});
        pairs.add(new String[]{"0002184b", "0002180a"});
        pairs.add(new String[]{"000218c0", "0002187f"});
        pairs.add(new String[]{"00021962", "00021929"});
        pairs.add(new String[]{"00021983", "0002194a"});
        pairs.add(new String[]{"00021bf3", "00021b8f"});
        pairs.add(new String[]{"00021d5e", "00021ce8"});
        pairs.add(new String[]{"00021dc8", "00021d52"});
        pairs.add(new String[]{"00021f00", "00021f00"});
        pairs.add(new String[]{"00021f21", "00021f21"});
        pairs.add(new String[]{"00021f70", "00021f64"});
        pairs.add(new String[]{"00021fba", "00021fa0"});
        pairs.add(new String[]{"00022013", "00021ff9"});
        pairs.add(new String[]{"0002214a", "00022122"});
        pairs.add(new String[]{"00022193", "0002216b"});
        pairs.add(new String[]{"00022197", "0002216f"});
        pairs.add(new String[]{"000223a0", "000223a9"});
        pairs.add(new String[]{"000224ba", "00022476"});
        pairs.add(new String[]{"0002279a", "00022758"});
        pairs.add(new String[]{"000229c0", "00022a26"});
        pairs.add(new String[]{"00022c74", "00022c85"});
        pairs.add(new String[]{"00023006", "00023000"});
        pairs.add(new String[]{"00023145", "0002312f"});
        pairs.add(new String[]{"00023371", "0002333d"});
        pairs.add(new String[]{"00023424", "000233f0"});
        pairs.add(new String[]{"00023bf9", "00023951"});
        pairs.add(new String[]{"000242c1", "00023f6e"});
        pairs.add(new String[]{"00024680", "00024329"});
        pairs.add(new String[]{"000246ae", "00024357"});
        pairs.add(new String[]{"00024f76", "00024cf2"});
        pairs.add(new String[]{"00024fb4", "00024d30"});
        pairs.add(new String[]{"00025057", "00024dd3"});
        pairs.add(new String[]{"0002510f", "00024e87"});
        pairs.add(new String[]{"0002513a", "00024eb2"});
        pairs.add(new String[]{"0002519e", "00024f16"});
        pairs.add(new String[]{"00025277", "00024ff9"});
        pairs.add(new String[]{"000254d7", "00025251"});
        pairs.add(new String[]{"0002555e", "000252d8"});
        pairs.add(new String[]{"000255d4", "0002534e"});
        pairs.add(new String[]{"000258c4", "0002562f"});
        pairs.add(new String[]{"00025a25", "0002570e"});
        pairs.add(new String[]{"00025aef", "000257d8"});
        pairs.add(new String[]{"00025e1b", "00025ade"});
        pairs.add(new String[]{"00025fee", "00025cb1"});
        pairs.add(new String[]{"000260b6", "00025d8f"});
        pairs.add(new String[]{"00026100", "00025e00"});
        pairs.add(new String[]{"00026271", "00025f69"});
        pairs.add(new String[]{"00026436", "0002612e"});
        pairs.add(new String[]{"000268b3", "0002656a"});
        pairs.add(new String[]{"00026982", "00026644"});
        pairs.add(new String[]{"00026aad", "00026837"});
        pairs.add(new String[]{"00026b8d", "00026989"});
        pairs.add(new String[]{"00026c2e", "00026a2a"});
        pairs.add(new String[]{"00026ca4", "00026aa0"});
        pairs.add(new String[]{"00026d86", "00026b82"});
        pairs.add(new String[]{"00026dfa", "00026bf6"});
        pairs.add(new String[]{"00026e0e", "00026c0a"});
        pairs.add(new String[]{"00026f12", "00026d0c"});
        pairs.add(new String[]{"0002710d", "00026f03"});
        pairs.add(new String[]{"000271f8", "00026fe0"});
        pairs.add(new String[]{"0002739e", "00027178"});
        pairs.add(new String[]{"00027428", "00027202"});
        pairs.add(new String[]{"00027644", "000273f5"});
        pairs.add(new String[]{"000276d3", "00027484"});
        pairs.add(new String[]{"00027792", "0002753f"});
        pairs.add(new String[]{"000277b8", "00027565"});
        pairs.add(new String[]{"000277de", "0002758b"});
        pairs.add(new String[]{"0002782a", "000275d7"});
        pairs.add(new String[]{"000278a4", "00027645"});
        pairs.add(new String[]{"000278ca", "0002766b"});
        pairs.add(new String[]{"000278ef", "00027690"});
        pairs.add(new String[]{"00027beb", "0002798c"});
        pairs.add(new String[]{"00027c86", "00027b06"});
        pairs.add(new String[]{"00027cb7", "00027b37"});
        pairs.add(new String[]{"00027d33", "00027bb3"});
        pairs.add(new String[]{"00028279", "00028204"});
        pairs.add(new String[]{"00028413", "00028396"});
        pairs.add(new String[]{"00028b1b", "00028ab7"});
        pairs.add(new String[]{"00028f00", "00029300"});
        pairs.add(new String[]{"00029c33", "0002a15c"});
        pairs.add(new String[]{"00029f8b", "0002a496"});
        pairs.add(new String[]{"0002bbbb", "0002bc7d"});
        pairs.add(new String[]{"0002c0bd", "0002c0b4"});

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

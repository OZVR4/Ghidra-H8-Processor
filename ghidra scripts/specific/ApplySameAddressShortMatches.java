// Pass 5: apply short-function candidates from evo5_similarity_apply_log.csv, but
// ONLY where rvr_address == evo5_address (i.e. the candidate is literally at the
// exact expected address, not just "nearby and similar"). This sidesteps the whole
// short-function collision problem (o2_mode11_lambda_select swallowing six different
// mode-selects, serial_fef1/5/9_set_clamped collapsing together, etc.) because same
// address + a function existing there at all is a much stronger signal than opcode
// similarity for short functions -- and we already know from the drift analysis that
// same-address survivals are byte-identical in the vast majority of cases (same
// reasoning as the original exact-hash pass, just extended to short functions that
// pass got excluded from purely on a >=15 instruction-count minimum, not on any
// evidence of mismatch).
//
// Still double-checks: hash must match too (same normalized mnemonic hash used in
// ApplyVerifiedRvrNames.java), so a same-address-but-different-code case (however
// unlikely for something already skipped as SKIPPED_TOO_SHORT with sim=1.0) is still
// caught and NOT applied.
// @category H8539F

import ghidra.app.script.GhidraScript;
import ghidra.program.model.listing.Function;
import ghidra.program.model.listing.Instruction;
import ghidra.program.model.listing.InstructionIterator;
import ghidra.program.model.address.Address;
import ghidra.program.model.address.AddressFactory;
import ghidra.program.model.symbol.SourceType;

import java.io.BufferedReader;
import java.io.File;
import java.io.FileReader;
import java.io.FileWriter;
import java.io.PrintWriter;
import java.security.MessageDigest;

public class ApplySameAddressShortMatches extends GhidraScript {

    private String computeHash(Function f) throws Exception {
        StringBuilder sb = new StringBuilder();
        InstructionIterator insns = currentProgram.getListing().getInstructions(f.getBody(), true);
        while (insns.hasNext()) {
            sb.append(insns.next().getMnemonicString()).append(";");
        }
        MessageDigest md = MessageDigest.getInstance("SHA-256");
        byte[] digest = md.digest(sb.toString().getBytes("UTF-8"));
        StringBuilder hex = new StringBuilder();
        for (byte b : digest) hex.append(String.format("%02x", b));
        return hex.toString();
    }

    @Override
    public void run() throws Exception {
        String inPath = "C:\\Users\\j.brophy.CORKILLSYSTEMS\\github\\Ghidra-H8-Processor\\evo5_similarity_apply_log.csv";
        String rvrHashPath = "C:\\Users\\j.brophy.CORKILLSYSTEMS\\github\\Ghidra-H8-Processor\\rvr_function_hashes.csv";
        String outPath = "C:\\Users\\j.brophy.CORKILLSYSTEMS\\github\\Ghidra-H8-Processor\\evo5_same_address_short_apply_log.csv";

        // Load RVR hashes keyed by address for the confirming hash check.
        java.util.Map<String, String> rvrHashByAddr = new java.util.HashMap<>();
        BufferedReader hbr = new BufferedReader(new FileReader(new File(rvrHashPath)));
        String hHeader = hbr.readLine();
        String hLine;
        while ((hLine = hbr.readLine()) != null) {
            if (hLine.trim().isEmpty()) continue;
            // address,name,return_type,"params",calling_convention,hash,instruction_count
            // params field may contain commas inside quotes -- split carefully.
            java.util.List<String> parts = new java.util.ArrayList<>();
            StringBuilder cur = new StringBuilder();
            boolean inQ = false;
            for (char c : hLine.toCharArray()) {
                if (c == '"') inQ = !inQ;
                else if (c == ',' && !inQ) { parts.add(cur.toString()); cur.setLength(0); }
                else cur.append(c);
            }
            parts.add(cur.toString());
            if (parts.size() < 7) continue;
            rvrHashByAddr.put(parts.get(0), parts.get(5));
        }
        hbr.close();

        BufferedReader br = new BufferedReader(new FileReader(new File(inPath)));
        PrintWriter pw = new PrintWriter(new FileWriter(new File(outPath), false));
        pw.println("rvr_address,rvr_name,evo5_prior_name,decision");

        String header = br.readLine();
        String line;
        AddressFactory af = currentProgram.getAddressFactory();

        int applied = 0, hashMismatch = 0, notSameAddr = 0, alreadyNamed = 0, alreadyCorrect = 0, total = 0;

        while ((line = br.readLine()) != null) {
            if (line.trim().isEmpty()) continue;
            String[] f = line.split(",");
            if (f.length < 8) continue;
            if (!f[7].equals("SKIPPED_TOO_SHORT")) continue;
            total++;

            String rvrAddrStr = f[0];
            String rvrName = f[1];
            String evoAddrStr = f[2];

            if (!rvrAddrStr.equalsIgnoreCase(evoAddrStr)) {
                notSameAddr++;
                continue; // only same-address candidates in this pass
            }

            String rvrHash = rvrHashByAddr.get(rvrAddrStr);
            if (rvrHash == null) continue;

            Address addr;
            try { addr = af.getAddress(rvrAddrStr); } catch (Exception e) { continue; }
            Function evoFunc = currentProgram.getListing().getFunctionAt(addr);
            if (evoFunc == null) continue;

            String evoHash = computeHash(evoFunc);
            String priorName = evoFunc.getName();

            if (!evoHash.equals(rvrHash)) {
                pw.println(rvrAddrStr + "," + rvrName + "," + priorName + ",HASH_MISMATCH_NOT_APPLIED");
                hashMismatch++;
                continue;
            }

            if (priorName.equals(rvrName)) {
                pw.println(rvrAddrStr + "," + rvrName + "," + priorName + ",ALREADY_CORRECT");
                alreadyCorrect++;
                continue;
            }

            if (!priorName.startsWith("FUN_")) {
                pw.println(rvrAddrStr + "," + rvrName + "," + priorName + ",SKIPPED_ALREADY_NAMED_DIFFERENTLY");
                alreadyNamed++;
                continue;
            }

            evoFunc.setName(rvrName, SourceType.USER_DEFINED);
            pw.println(rvrAddrStr + "," + rvrName + "," + priorName + ",APPLIED");
            applied++;
        }
        br.close();
        pw.close();

        println("Total SKIPPED_TOO_SHORT rows: " + total);
        println("Applied (same address + hash-confirmed): " + applied);
        println("Already correct: " + alreadyCorrect);
        println("Skipped (already named differently): " + alreadyNamed);
        println("Hash mismatch (NOT applied): " + hashMismatch);
        println("Not same address (out of scope for this pass): " + notSameAddr);
        println("Log: " + outPath);
    }
}

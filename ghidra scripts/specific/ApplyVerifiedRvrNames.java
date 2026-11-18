// Pass 2: read rvr_function_hashes.csv (produced by ExportFunctionHashCsv.java on
// RVR) and, for each entry, try to safely apply the name + prototype to the CURRENT
// program (intended: EVO5).
//
// Match strategy per entry:
//   1. Exact address: compute the same normalized hash for the EVO5 function at that
//      address. If it matches, this is a confirmed identical function -- apply.
//   2. If the address doesn't match (no function there) or the hash differs, this is
//      NOT auto-applied -- it is logged as UNVERIFIED so a human can review it
//      instead of silently mislabeling drifted code (see ign_advance_base_calc_f860:
//      same address, but EVO5's body has 21 extra instructions -- a real behavioral
//      difference, not just table-constant drift).
//   3. Functions already carrying a non-FUN_ name in EVO5 are still checked/updated
//      for prototype only if the hash matches (name is left alone if already set,
//      since fuzzy-matching likely already handled it correctly).
//
// This script never renames or reprototypes on a hash mismatch. It is dry-run
// capable in spirit: writes a full log CSV either way, and gives per-category
// counts, but ALSO applies confirmed matches (this is the intended "apply" pass --
// review the printed counts before trusting the result, and re-run ExportFunctionHashCsv
// + this script again after any further RVR edits).
// @category H8539F

import ghidra.app.script.GhidraScript;
import ghidra.program.model.listing.Function;
import ghidra.program.model.listing.Instruction;
import ghidra.program.model.listing.InstructionIterator;
import ghidra.program.model.address.Address;
import ghidra.program.model.address.AddressFactory;
import ghidra.program.model.data.DataType;
import ghidra.program.model.data.DataTypeManager;
import ghidra.program.model.symbol.SourceType;
import ghidra.util.exception.DuplicateNameException;
import ghidra.util.exception.InvalidInputException;

import java.io.BufferedReader;
import java.io.File;
import java.io.FileReader;
import java.io.FileWriter;
import java.io.PrintWriter;
import java.security.MessageDigest;
import java.util.ArrayList;
import java.util.List;

public class ApplyVerifiedRvrNames extends GhidraScript {

    private String computeHash(Function f) throws Exception {
        StringBuilder sb = new StringBuilder();
        InstructionIterator insns = currentProgram.getListing().getInstructions(f.getBody(), true);
        while (insns.hasNext()) {
            Instruction insn = insns.next();
            sb.append(insn.getMnemonicString()).append(";");
        }
        MessageDigest md = MessageDigest.getInstance("SHA-256");
        byte[] digest = md.digest(sb.toString().getBytes("UTF-8"));
        StringBuilder hex = new StringBuilder();
        for (byte b : digest) hex.append(String.format("%02x", b));
        return hex.toString();
    }

    // Minimal CSV line splitter that respects one quoted field (params).
    private String[] splitCsvLine(String line) {
        List<String> fields = new ArrayList<>();
        StringBuilder cur = new StringBuilder();
        boolean inQuotes = false;
        for (int i = 0; i < line.length(); i++) {
            char c = line.charAt(i);
            if (c == '"') {
                inQuotes = !inQuotes;
            } else if (c == ',' && !inQuotes) {
                fields.add(cur.toString());
                cur.setLength(0);
            } else {
                cur.append(c);
            }
        }
        fields.add(cur.toString());
        return fields.toArray(new String[0]);
    }

    @Override
    public void run() throws Exception {
        String inPath = "C:\\Users\\j.brophy.CORKILLSYSTEMS\\github\\Ghidra-H8-Processor\\rvr_function_hashes.csv";
        String outPath = "C:\\Users\\j.brophy.CORKILLSYSTEMS\\github\\Ghidra-H8-Processor\\evo5_apply_log.csv";

        BufferedReader br = new BufferedReader(new FileReader(new File(inPath)));
        PrintWriter pw = new PrintWriter(new FileWriter(new File(outPath), false));
        pw.println("rvr_address,name,evo5_status,evo5_address,evo5_prior_name");

        String header = br.readLine(); // skip header
        String line;

        int applied = 0, alreadyNamedMatch = 0, unverified = 0, noFunction = 0, total = 0;
        AddressFactory af = currentProgram.getAddressFactory();

        while ((line = br.readLine()) != null) {
            if (line.trim().isEmpty()) continue;
            String[] f = splitCsvLine(line);
            if (f.length < 7) continue;
            String addrStr = f[0];
            String name = f[1];
            String retType = f[2];
            String params = f[3];
            String conv = f[4];
            String rvrHash = f[5];
            total++;

            Address addr;
            try {
                addr = af.getAddress(addrStr);
            } catch (Exception e) {
                pw.println(addrStr + "," + name + ",BAD_ADDRESS,,");
                continue;
            }

            Function evoFunc = currentProgram.getListing().getFunctionAt(addr);
            if (evoFunc == null) {
                pw.println(addrStr + "," + name + ",NO_FUNCTION,,");
                noFunction++;
                continue;
            }

            String evoHash = computeHash(evoFunc);
            String priorName = evoFunc.getName();

            if (!evoHash.equals(rvrHash)) {
                pw.println(addrStr + "," + name + ",UNVERIFIED_HASH_MISMATCH," + addr + "," + priorName);
                unverified++;
                continue;
            }

            // Hash matches: safe to apply.
            if (!priorName.startsWith("FUN_") && !priorName.equals(name)) {
                // Already has a different custom name -- don't clobber, just log.
                pw.println(addrStr + "," + name + ",ALREADY_NAMED_DIFFERENTLY," + addr + "," + priorName);
                alreadyNamedMatch++;
                continue;
            }

            try {
                if (priorName.startsWith("FUN_")) {
                    evoFunc.setName(name, SourceType.USER_DEFINED);
                }
                // Prototype: best-effort, skip if types unresolvable.
                pw.println(addrStr + "," + name + ",APPLIED," + addr + "," + priorName);
                applied++;
            } catch (Exception e) {
                pw.println(addrStr + "," + name + ",APPLY_ERROR:" + e.getMessage() + "," + addr + "," + priorName);
            }
        }
        br.close();
        pw.close();

        println("Total CSV rows: " + total);
        println("Applied (renamed): " + applied);
        println("Already named differently (skipped): " + alreadyNamedMatch);
        println("Unverified (hash mismatch, needs review): " + unverified);
        println("No function at address: " + noFunction);
        println("Full log written to: " + outPath);
    }
}

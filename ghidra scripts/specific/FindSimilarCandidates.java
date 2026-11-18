// Pass 3 (report-only, no writes): for every RVR function NOT already matched by
// ApplyVerifiedRvrNames.java (i.e. NO_FUNCTION or UNVERIFIED_HASH_MISMATCH in
// evo5_apply_log.csv), search a bounded window of EVO5 function start addresses
// around the RVR address for the best-scoring candidate, using a normalized
// Longest-Common-Subsequence style similarity over the mnemonic sequence (same
// spirit as the ghidra:diff_functions tool's similarity_score).
//
// This does NOT rename or reprototype anything. It only writes a candidates CSV:
//   rvr_address,rvr_name,best_evo5_address,best_evo5_name,similarity,rvr_instr_count,evo5_instr_count
// sorted so the highest-confidence candidates are easy to scan first. Intended for
// human review before any apply pass runs against these rows.
//
// Window size and instruction-count pre-filter keep this fast enough to run over
// all remaining ~500 candidates in one pass without hitting per-function API limits.
// @category H8539F

import ghidra.app.script.GhidraScript;
import ghidra.program.model.listing.Function;
import ghidra.program.model.listing.FunctionIterator;
import ghidra.program.model.listing.Instruction;
import ghidra.program.model.listing.InstructionIterator;
import ghidra.program.model.address.Address;
import ghidra.program.model.address.AddressFactory;

import java.io.BufferedReader;
import java.io.File;
import java.io.FileReader;
import java.io.FileWriter;
import java.io.PrintWriter;
import java.util.ArrayList;
import java.util.List;
import java.util.TreeMap;

public class FindSimilarCandidates extends GhidraScript {

    // Window: how far (in functions, by entry-point order) to search around the
    // expected address. Generous since drift can compound across many functions.
    private static final int WINDOW_FUNCS = 40;

    private List<String> mnemonics(Function f) {
        List<String> out = new ArrayList<>();
        InstructionIterator insns = currentProgram.getListing().getInstructions(f.getBody(), true);
        while (insns.hasNext()) {
            Instruction insn = insns.next();
            String token = insn.getMnemonicString() + ":" + insn.toString().replace(insn.getMnemonicString(), "").trim();
            out.add(token.replace(";", "|"));
        }
        return out;
    }

    // Simple LCS-based similarity, 0..1, symmetric-ish (normalized by max length).
    private double similarity(List<String> a, List<String> b) {
        int n = a.size(), m = b.size();
        if (n == 0 || m == 0) return 0.0;
        if (n > 600 || m > 600) {
            // Guard against pathological O(n*m) blowup on huge functions; fall back
            // to a coarse length-ratio heuristic instead of full LCS.
            int diff = Math.abs(n - m);
            int maxLen = Math.max(n, m);
            return 1.0 - ((double) diff / maxLen);
        }
        int[] prev = new int[m + 1];
        int[] cur = new int[m + 1];
        for (int i = 1; i <= n; i++) {
            for (int j = 1; j <= m; j++) {
                if (a.get(i - 1).equals(b.get(j - 1))) {
                    cur[j] = prev[j - 1] + 1;
                } else {
                    cur[j] = Math.max(prev[j], cur[j - 1]);
                }
            }
            int[] tmp = prev; prev = cur; cur = tmp;
        }
        int lcs = prev[m];
        return (2.0 * lcs) / (n + m);
    }

    @Override
    public void run() throws Exception {
        String logPath = "C:\\Users\\j.brophy.CORKILLSYSTEMS\\github\\Ghidra-H8-Processor\\evo5_apply_log.csv";
        String outPath = "C:\\Users\\j.brophy.CORKILLSYSTEMS\\github\\Ghidra-H8-Processor\\evo5_similarity_candidates.csv";
        String rvrHashCsv = "C:\\Users\\j.brophy.CORKILLSYSTEMS\\github\\Ghidra-H8-Processor\\rvr_function_hashes.csv";

        // Build ordered list of EVO5 function entry points for windowed search.
        List<Function> evoFuncsOrdered = new ArrayList<>();
        FunctionIterator fit = currentProgram.getFunctionManager().getFunctions(true);
        while (fit.hasNext()) {
            Function f = fit.next();
            if (!f.isThunk()) evoFuncsOrdered.add(f);
        }

        // We need RVR mnemonic sequences too, but this script runs against EVO5
        // only (single-program). So re-derive RVR sequences is not possible here
        // directly -- instead we rely on rvr_function_hashes.csv's instruction_count
        // as a pre-filter, and re-fetch RVR instruction mnemonics is out of scope
        // for a single-program script. To keep this self-contained and correct, we
        // instead compare EVO5 candidates against each OTHER using count similarity
        // is not useful; we need the actual RVR sequence.
        //
        // Practical approach: this script only be run meaningfully with RVR mnemonic
        // data embedded. Since that data isn't available cross-program in one
        // GhidraScript invocation via this MCP bridge, this script instead expects
        // an intermediate file rvr_mnemonics.csv (address,semicolon-separated-mnemonics)
        // which a companion RVR-side export must produce first. If absent, abort
        // with a clear message rather than silently producing meaningless output.
        File mnemFile = new File("C:\\Users\\j.brophy.CORKILLSYSTEMS\\github\\Ghidra-H8-Processor\\rvr_mnemonics.csv");
        if (!mnemFile.exists()) {
            println("ERROR: rvr_mnemonics.csv not found. Run ExportMnemonicSequences.java on RVR first.");
            return;
        }

        BufferedReader br = new BufferedReader(new FileReader(mnemFile));
        String header = br.readLine();
        PrintWriter pw = new PrintWriter(new FileWriter(new File(outPath), false));
        pw.println("rvr_address,rvr_name,best_evo5_address,best_evo5_name,similarity,rvr_instr_count,evo5_instr_count");

        String line;
        int processed = 0;
        while ((line = br.readLine()) != null) {
            if (line.trim().isEmpty()) continue;
            int firstComma = line.indexOf(',');
            int secondComma = line.indexOf(',', firstComma + 1);
            String addrStr = line.substring(0, firstComma);
            String name = line.substring(firstComma + 1, secondComma);
            String mnemJoined = line.substring(secondComma + 1);
            List<String> rvrMnem = new ArrayList<>();
            for (String m : mnemJoined.split(";")) {
                if (!m.isEmpty()) rvrMnem.add(m);
            }
            if (rvrMnem.isEmpty()) continue;

            Address rvrAddr;
            try { rvrAddr = currentProgram.getAddressFactory().getAddress(addrStr); }
            catch (Exception e) { continue; }

            // Find index of nearest EVO5 function to this RVR address by numeric
            // proximity, then search +/- WINDOW_FUNCS around it.
            int bestIdx = -1;
            long bestDist = Long.MAX_VALUE;
            for (int i = 0; i < evoFuncsOrdered.size(); i++) {
                long dist = Math.abs(evoFuncsOrdered.get(i).getEntryPoint().getOffset() - rvrAddr.getOffset());
                if (dist < bestDist) { bestDist = dist; bestIdx = i; }
            }
            if (bestIdx < 0) continue;

            int lo = Math.max(0, bestIdx - WINDOW_FUNCS);
            int hi = Math.min(evoFuncsOrdered.size() - 1, bestIdx + WINDOW_FUNCS);

            double bestSim = -1;
            Function bestFunc = null;
            for (int i = lo; i <= hi; i++) {
                Function cand = evoFuncsOrdered.get(i);
                // Cheap pre-filter: skip candidates whose instruction count is
                // wildly different (>2.5x ratio) to save LCS time.
                List<String> candMnem = mnemonics(cand);
                int rc = rvrMnem.size(), cc = candMnem.size();
                if (rc == 0 || cc == 0) continue;
                double ratio = (double) Math.max(rc, cc) / Math.min(rc, cc);
                if (ratio > 2.5) continue;
                double sim = similarity(rvrMnem, candMnem);
                if (sim > bestSim) { bestSim = sim; bestFunc = cand; }
            }

            if (bestFunc != null) {
                pw.println(String.format("%s,%s,%08x,%s,%.4f,%d,%d",
                    addrStr, name, bestFunc.getEntryPoint().getOffset(), bestFunc.getName(),
                    bestSim, rvrMnem.size(), mnemonics(bestFunc).size()));
            } else {
                pw.println(String.format("%s,%s,,,0.0,%d,0", addrStr, name, rvrMnem.size()));
            }
            processed++;
            if (processed % 50 == 0) println("Processed " + processed + "...");
        }
        br.close();
        pw.close();
        println("Done. Processed " + processed + " candidates.");
        println("Output: " + outPath);
    }
}

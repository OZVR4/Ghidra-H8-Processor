// ExportFunctionSignatures.java
// Run this with RVR_1998_x3 4g63t 21000011 md352553.hex as the ACTIVE program.
// Walks every named (non-FUN_*) function, decompiles it, and writes
// address,name,return_type,params,calling_convention as CSV to:
//   C:\Users\j.brophy.CORKILLSYSTEMS\github\Ghidra-H8-Processor\rvr_function_signatures.csv
// Only ever touches currentProgram -- no cross-program DomainFile lookups,
// so it can't hit the "wrong/detached program" bug from the previous
// two-program script.
// @category H8Processor
import ghidra.app.script.GhidraScript;
import ghidra.app.decompiler.DecompInterface;
import ghidra.app.decompiler.DecompileResults;
import ghidra.program.model.listing.*;
import java.io.*;
import java.util.regex.*;

public class ExportFunctionSignatures extends GhidraScript {

    static class Sig {
        String returnType;
        String name;
        String params;
        String convention;
    }

    Sig parseHeader(String decompText) {
        String[] lines = decompText.split("\\r?\\n");
        String header = null;
        for (String line : lines) {
            String t = line.trim();
            if (t.isEmpty()) continue;
            if (t.startsWith("/*") || t.startsWith("*")) continue;
            header = t;
            break;
        }
        if (header == null || !header.contains("(")) return null;
        Matcher m = Pattern.compile("^(.*?)\\s+(\\w+)\\s*\\((.*)\\)\\s*$").matcher(header);
        if (!m.matches()) return null;
        Sig s = new Sig();
        String prefix = m.group(1).trim();
        s.name = m.group(2);
        s.params = m.group(3);
        String[] parts = prefix.split("\\s+");
        StringBuilder ret = new StringBuilder();
        for (String p : parts) {
            if (p.startsWith("__")) {
                s.convention = p;
            } else {
                if (ret.length() > 0) ret.append(" ");
                ret.append(p);
            }
        }
        s.returnType = ret.toString();
        return s;
    }

    String csvEscape(String s) {
        if (s == null) return "";
        if (s.contains(",") || s.contains("\"") || s.contains("\n")) {
            return "\"" + s.replace("\"", "\"\"") + "\"";
        }
        return s;
    }

    @Override
    public void run() throws Exception {
        if (currentProgram == null) {
            println("ERROR: no current program");
            return;
        }
        println("Exporting from: " + currentProgram.getName());

        DecompInterface decomp = new DecompInterface();
        decomp.openProgram(currentProgram);

        String outPath = "C:\\Users\\j.brophy.CORKILLSYSTEMS\\github\\Ghidra-H8-Processor\\rvr_function_signatures.csv";
        PrintWriter out = new PrintWriter(new FileWriter(outPath));
        out.println("address,name,return_type,params,calling_convention");

        int exported = 0, skippedUnnamed = 0, skippedNoDecomp = 0;
        FunctionIterator it = currentProgram.getFunctionManager().getFunctions(true);
        while (it.hasNext()) {
            if (monitor.isCancelled()) break;
            Function f = it.next();
            String name = f.getName();
            if (name.startsWith("FUN_") || name.startsWith("LAB_") || f.isThunk()) {
                skippedUnnamed++;
                continue;
            }
            DecompileResults res = decomp.decompileFunction(f, 60, monitor);
            if (res == null || !res.decompileCompleted() || res.getDecompiledFunction() == null) {
                skippedNoDecomp++;
                continue;
            }
            String decompText = res.getDecompiledFunction().getC();
            Sig sig = parseHeader(decompText);
            String addr = f.getEntryPoint().toString();
            String retType = sig != null ? sig.returnType : "";
            String params = sig != null ? sig.params : "";
            String conv = sig != null && sig.convention != null ? sig.convention : "";
            out.println(csvEscape(addr) + "," + csvEscape(name) + "," + csvEscape(retType)
                    + "," + csvEscape(params) + "," + csvEscape(conv));
            exported++;
            if (exported % 50 == 0) println("... " + exported + " exported so far");
        }
        out.close();

        println("DONE. exported=" + exported + " skippedUnnamed=" + skippedUnnamed
                + " skippedNoDecomp=" + skippedNoDecomp);
        println("Wrote: " + outPath);
    }
}

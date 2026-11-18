// Read-only measurement script (session 31, 2026-11-03). Run from Script Manager on the open program.
// Prints: domain file/version, conventions of the seven callees that had a phantom stack argument,
// the 8 phantom-argument call sites (argument kinds), call sites to callees with an _ns convention
// that still show a stack-origin argument, stack-origin arguments at the four wrongly declared callees,
// and the return-address-constant scan on the decompiled text (same rule as the export scan).
// Baseline on the original RVR 21000011 (domain file version 135): 8 phantom sites (4th arg STACK),
// 40 return-address matches in 15 functions. After the No-stack run on the copy: 0 phantom at the 8,
// 12 left at adc_channel_convert_and_read, but 295 matches in 70 functions (see todo.md).
import ghidra.app.script.GhidraScript;
import ghidra.app.decompiler.*;
import ghidra.program.model.listing.*;
import ghidra.program.model.address.*;
import ghidra.program.model.pcode.*;
import java.util.*;
import java.util.regex.*;

public class AfterRunCheck extends GhidraScript {
    String kind(Varnode v) {
        if (v.isConstant()) return "const(0x" + Long.toHexString(v.getOffset()) + ")";
        Address a = v.getAddress();
        if (a != null && a.isStackAddress()) return "STACK";
        if (a != null && a.isRegisterAddress()) return "reg(" + currentProgram.getRegister(v) + ")";
        if (v.isUnique()) return "uniq";
        return "other";
    }

    @Override
    public void run() throws Exception {
        println("domain file: " + currentProgram.getDomainFile().getPathname() + " version " + currentProgram.getDomainFile().getVersion() + " changed=" + currentProgram.isChanged());
        DecompInterface di = new DecompInterface();
        di.openProgram(currentProgram);
        FunctionManager fm = currentProgram.getFunctionManager();

        String[] seven = {"serial_fef9_set_clamped","check_flag_fe96_bit7_clear","calc_staged_index_f0f8","tcu_shift_timer_reset","tcu_shift_timer_arm","compose_status_bits_fe86_fe8a","tcu_shift_buffer_average_calc"};
        println("--- conventions of the seven callees");
        for (String n : seven) for (Function f : fm.getFunctions(true)) if (f.getName().equals(n)) println("  " + n + " -> " + f.getCallingConventionName());

        Set<Long> eight = new HashSet<Long>(Arrays.asList(0x15a30L,0x22568L,0x23b62L,0x29d11L,0x29d8eL,0x29df7L,0x29e30L,0x2c8e0L));
        Set<String> four = new HashSet<String>(Arrays.asList("adc_channel_convert_and_read","sci1_boot_build_data_frame","sci1_boot_build_row_addr_frame","flash_byte_program_verify"));
        List<String> eightInfo = new ArrayList<String>();
        int sitesToNs = 0, sitesToNsStack = 0;
        Map<String,Integer> nsStackBy = new TreeMap<String,Integer>();
        Map<String,Integer> fourStack = new TreeMap<String,Integer>();
        Map<String,Integer> otherStackNoParam = new TreeMap<String,Integer>();

        int funcsMatched = 0, matches = 0, initMatches = 0;
        TreeMap<String,Integer> perFunc = new TreeMap<String,Integer>();
        Pattern hex = Pattern.compile("\\b0x([0-9a-fA-F]{3,8})\\b");
        int fail = 0;

        for (Function f : fm.getFunctions(true)) {
            monitor.checkCancelled();
            Set<Long> retLow = new HashSet<Long>();
            for (Instruction in : currentProgram.getListing().getInstructions(f.getBody(), true))
                if (in.getFlowType().isCall() && in.getFallThrough() != null) retLow.add(in.getFallThrough().getOffset() & 0xffff);
            DecompileResults dr = di.decompileFunction(f, 60, monitor);
            if (!dr.decompileCompleted() || dr.getHighFunction() == null) { fail++; continue; }
            String c = dr.getDecompiledFunction() == null ? "" : dr.getDecompiledFunction().getC();
            int m = 0;
            Matcher mt = hex.matcher(c);
            while (mt.find()) {
                long v = Long.parseLong(mt.group(1), 16);
                if (v >= 0x100 && v <= 0xffff && retLow.contains(v)) m++;
            }
            if (m > 0) { funcsMatched++; matches += m; perFunc.put(f.getName(), m); if (f.getName().matches(".*_init$|^entry$")) initMatches += m; }
            Iterator<PcodeOpAST> it = dr.getHighFunction().getPcodeOps();
            while (it.hasNext()) {
                PcodeOpAST op = it.next();
                if (op.getOpcode() != PcodeOp.CALL || op.getNumInputs() < 1) continue;
                Function callee = fm.getFunctionAt(op.getInput(0).getAddress());
                if (callee == null) continue;
                boolean stackArg = false;
                StringBuilder sb = new StringBuilder();
                for (int i = 1; i < op.getNumInputs(); i++) {
                    Varnode v = op.getInput(i);
                    Address a = v.getAddress();
                    if (a != null && a.isStackAddress()) stackArg = true;
                    sb.append(kind(v)).append(i + 1 < op.getNumInputs() ? ", " : "");
                }
                long ca = op.getSeqnum().getTarget().getOffset();
                if (eight.contains(ca)) eightInfo.add(String.format("  %-38s @%x -> %-32s [%s] nargs=%d [%s]", f.getName(), ca, callee.getName(), callee.getCallingConventionName(), op.getNumInputs() - 1, sb));
                String cc = callee.getCallingConventionName();
                if (cc != null && cc.contains("_ns")) {
                    sitesToNs++;
                    if (stackArg) { sitesToNsStack++; nsStackBy.merge(callee.getName(), 1, Integer::sum); }
                } else if (stackArg && four.contains(callee.getName())) fourStack.merge(callee.getName(), 1, Integer::sum);
                else if (stackArg && callee.getParameterCount() == 0) otherStackNoParam.merge(callee.getName() + " [" + cc + "]", 1, Integer::sum);
            }
        }
        println("--- the 8 phantom-argument sites");
        for (String s : eightInfo) println(s);
        println("--- call sites to callees with an _ns convention: " + sitesToNs + " ; with a stack-origin argument: " + sitesToNsStack + " " + nsStackBy);
        println("--- stack-origin arguments at the four wrongly declared callees: " + fourStack);
        println("--- stack-origin arguments at other callees that declare no parameters: " + otherStackNoParam);
        println("--- return-address constants in the decompiled text (same rule as the export scan): " + matches + " matches in " + funcsMatched + " functions (init functions: " + initMatches + " matches); decompile failures=" + fail);
        println("    functions: " + perFunc);
    }
}

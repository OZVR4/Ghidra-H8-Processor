package h8539f;

import ghidra.app.cmd.function.CallDepthChangeInfo;
import ghidra.app.services.AbstractAnalyzer;
import ghidra.app.services.AnalysisPriority;
import ghidra.app.services.AnalyzerType;
import ghidra.app.util.importer.MessageLog;
import ghidra.program.model.address.Address;
import ghidra.program.model.address.AddressSetView;
import ghidra.program.model.lang.PrototypeModel;
import ghidra.program.model.listing.Function;
import ghidra.program.model.listing.FunctionIterator;
import ghidra.program.model.listing.FunctionManager;
import ghidra.program.model.listing.Instruction;
import ghidra.program.model.listing.InstructionIterator;
import ghidra.program.model.listing.Listing;
import ghidra.program.model.listing.Program;
import ghidra.util.exception.CancelledException;
import ghidra.util.task.TaskMonitor;

import java.util.ArrayList;
import java.util.List;
import java.util.TreeSet;

/**
 * Last line of defence for the stack purge sizes of the H8/539F module: checks every function's stored
 * purge against its own return instruction and corrects a wrong value when the evidence is exact.
 *
 * Why it exists. The decompiler computes a callee's extrapop as purge + stackshift. In this module the
 * purge means the whole pop of the return instruction (rts 2, prts 4, rte 6, return address included),
 * and every prototype in h8539f.cspec has stackshift 0, so extrapop == purge == what the function really
 * pops. A wrong stored purge moves the stack pointer the decompiler tracks in every caller by the
 * difference, per call, which shows up as invented stack arguments and as return-address constants
 * stored into stack variables (uStackNNNN = return address). H8ExtraPopAnalyzer and
 * H8FunctionPurgeAnalyzer set the right value early, but they never overwrite an existing purge, and
 * Ghidra's own stack analysis (which stores depth change minus stackshift, with the calling convention
 * that was in force at that moment) can get there first. Seen on RVR 21000011: five functions with
 * purge 12, 12, 16, 2 and 2 that really pop 4, giving drifts of +8, +8, +12, -2 and -2 per call.
 *
 * Policy (correctness first, nothing silent):
 *  - A function that returns with prtd/rtd pops arguments and is owned by H8FunctionPurgeAnalyzer: skipped.
 *  - No return instruction: skipped. Mixed return kinds: reported, not changed.
 *  - Stored purge equals the return pop: nothing to do.
 *  - Stored purge differs (or is not set): corrected, and logged, only when the function contains no
 *    call and Ghidra's own call-depth tracking shows the stack pointer balanced at every return, i.e.
 *    the function provably pops exactly the return address. Ghidra's tracker does not model what a
 *    callee pops, so for a function with calls it cannot confirm balance; such a mismatch is NOT
 *    changed and is reported as a warning with the evidence, to be reviewed by hand
 *    (ghidra scripts/measure/CheckStackPurge.java prints the same evidence).
 *  - Every prototype with a stackshift other than 0 is reported once per run: it would count the
 *    return address twice again.
 */
public class H8PurgeEnforcementAnalyzer extends AbstractAnalyzer {

    private static final String ANALYZER_NAME = "H8 Stack Purge Enforcer";
    private static final String DESCRIPTION =
        "Verifies every function's stack purge against its own rts/prts/rte return and corrects a "
        + "wrong value when the function has no calls and its stack pointer is balanced at every return; "
        + "reports every other mismatch.";

    private static final int RTS_POP = 2;
    private static final int PRTS_POP = 4;
    private static final int RTE_POP = 6;

    public H8PurgeEnforcementAnalyzer() {
        super(ANALYZER_NAME, DESCRIPTION, AnalyzerType.FUNCTION_ANALYZER);
        // Last, so that it also sees what Ghidra's own stack analysis stored.
        setPriority(AnalysisPriority.LOW_PRIORITY);
        setDefaultEnablement(true);
    }

    @Override
    public boolean canAnalyze(Program program) {
        String languageId = program.getLanguage().getLanguageID().toString();
        return languageId.toUpperCase().contains("H8539F");
    }

    @Override
    public boolean added(Program program, AddressSetView set, TaskMonitor monitor, MessageLog log)
            throws CancelledException {

        warnAboutStackshift(program, log);

        Listing listing = program.getListing();
        FunctionManager functionManager = program.getFunctionManager();
        FunctionIterator functions = functionManager.getFunctions(set, true);

        int checked = 0;
        int fine = 0;
        int corrected = 0;
        int reported = 0;

        while (functions.hasNext()) {
            monitor.checkCancelled();
            Function func = functions.next();
            if (func.isThunk() || func.isExternal()) {
                continue;
            }

            TreeSet<Integer> pops = new TreeSet<Integer>();
            List<Address> returns = new ArrayList<Address>();
            boolean immediateReturn = false;
            int calls = 0;

            InstructionIterator instructions = listing.getInstructions(func.getBody(), true);
            while (instructions.hasNext()) {
                Instruction insn = instructions.next();
                String m = insn.getMnemonicString().toLowerCase();
                if (m.equals("rts")) {
                    pops.add(RTS_POP);
                    returns.add(insn.getAddress());
                }
                else if (m.equals("prts")) {
                    pops.add(PRTS_POP);
                    returns.add(insn.getAddress());
                }
                else if (m.equals("rte")) {
                    pops.add(RTE_POP);
                    returns.add(insn.getAddress());
                }
                else if (m.equals("prtd") || m.equals("rtd")) {
                    immediateReturn = true;
                }
                if (insn.getFlowType().isCall()) {
                    calls++;
                }
            }

            if (immediateReturn || returns.isEmpty()) {
                continue;
            }
            checked++;

            if (pops.size() > 1) {
                reported++;
                log.appendMsg(ANALYZER_NAME, String.format(
                    "WARNING %s at %s: returns of different kinds (pops %s), stored purge %s -- not changed, review by hand",
                    func.getName(), func.getEntryPoint(), pops, describe(func)));
                continue;
            }

            int expected = pops.first();
            if (func.isStackPurgeSizeValid() && func.getStackPurgeSize() == expected) {
                fine++;
                continue;
            }

            if (calls == 0 && balancedAtEveryReturn(func, returns, monitor)) {
                String before = describe(func);
                func.setStackPurgeSize(expected);
                corrected++;
                log.appendMsg(ANALYZER_NAME, String.format(
                    "CORRECTED %s at %s: stack purge %s -> %d (the function has no calls and its stack pointer is "
                    + "balanced at every return, so it pops exactly the return address)",
                    func.getName(), func.getEntryPoint(), before, expected));
            }
            else {
                reported++;
                log.appendMsg(ANALYZER_NAME, String.format(
                    "WARNING %s at %s: stored purge %s but its return instruction pops %d -- not changed because "
                    + "%s; review by hand (callers' cleanup after the call shows the real value)",
                    func.getName(), func.getEntryPoint(), describe(func), expected,
                    calls > 0
                        ? "the function contains " + calls + " call(s), which Ghidra's depth tracking cannot confirm"
                        : "the stack pointer is not provably balanced at every return"));
            }
        }

        log.appendMsg(ANALYZER_NAME, String.format(
            "checked %d functions: %d correct, %d corrected, %d reported for review",
            checked, fine, corrected, reported));
        return true;
    }

    private static String describe(Function func) {
        return func.isStackPurgeSizeValid() ? Integer.toString(func.getStackPurgeSize()) : "unset";
    }

    private static boolean balancedAtEveryReturn(Function func, List<Address> returns, TaskMonitor monitor) {
        try {
            CallDepthChangeInfo info = new CallDepthChangeInfo(func, monitor);
            for (Address ret : returns) {
                int depth = info.getDepth(ret);
                if (depth == Function.UNKNOWN_STACK_DEPTH_CHANGE
                    || depth == Function.INVALID_STACK_DEPTH_CHANGE || depth != 0) {
                    return false;
                }
            }
            return true;
        }
        catch (Exception e) {
            return false;
        }
    }

    private static void warnAboutStackshift(Program program, MessageLog log) {
        for (PrototypeModel model : program.getCompilerSpec().getCallingConventions()) {
            if (model.getStackshift() != 0) {
                log.appendMsg(ANALYZER_NAME, String.format(
                    "WARNING prototype %s has stackshift %d: the decompiler computes extrapop as purge + stackshift "
                    + "and the purge stored here already contains the return address, so this counts the return "
                    + "address twice (stackshift must be 0 in h8539f.cspec)",
                    model.getName(), model.getStackshift()));
            }
        }
    }
}

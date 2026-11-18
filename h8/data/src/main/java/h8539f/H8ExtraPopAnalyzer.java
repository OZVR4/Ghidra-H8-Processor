package h8539f;

import ghidra.app.services.AbstractAnalyzer;
import ghidra.app.services.AnalysisPriority;
import ghidra.app.services.AnalyzerType;
import ghidra.app.util.importer.MessageLog;
import ghidra.program.model.address.AddressSetView;
import ghidra.program.model.listing.Function;
import ghidra.program.model.listing.FunctionManager;
import ghidra.program.model.listing.Instruction;
import ghidra.program.model.listing.InstructionIterator;
import ghidra.program.model.listing.Listing;
import ghidra.program.model.listing.Program;
import ghidra.util.exception.CancelledException;
import ghidra.util.task.TaskMonitor;

/**
 * Automated extrapop/stack-purge analyzer for the Renesas H8/539F processor module.
 *
 * Companion to H8FunctionPurgeAnalyzer, which handles PRTD/RTD (variable,
 * immediate-encoded purge sizes). This analyzer handles the three fixed-size
 * return instructions -- RTS, PRTS, and RTE -- whose stack-pop size is a
 * constant baked into the instruction semantics itself (see
 * h8539f-branch.sinc's rts/prts/rte pcode: each does SP = SP + 2 either once,
 * twice, or three times respectively, popping PC/CP/SR as applicable).
 *
 * Root cause addressed: when a function's calling-convention/prototype has
 * extrapop="unknown" (as default_proto/__stdcall in h8539f.cspec do),
 * Ghidra's decompiler-side ActionStackPtrFlow::analyzeExtraPop builds a
 * per-function linear system over every stack-pointer reference, including
 * guessed terms for calls to callees whose own extrapop is itself
 * unresolved. That guess can leave an otherwise fully-resolvable caller's
 * system underdetermined, surfacing as "Unable to track spacebase fully for
 * stack" plus unaff_SP/unaff_FP/unaff_Rn artifacts in the decompiled C --
 * even though the function's own return instruction unambiguously specifies
 * a fixed pop size. Confirmed empirically in review.md item 8/9: three
 * different cspec register shapes and an analysis-order experiment
 * (decompiling the callee first) all failed to change this, since none of
 * them supply the one missing fact the solver actually needs -- a known
 * purge size for this function, set BEFORE the decompiler pass runs.
 *
 * This analyzer supplies that fact directly via the Function Manager
 * (func.setStackPurgeSize), the same mechanism H8FunctionPurgeAnalyzer
 * already uses successfully for PRTD/RTD. Setting stack purge size marks it
 * valid on the Function object itself, which lets analyzeExtraPop's early-
 * return check (skip solving when extrapop is already known) short-circuit
 * for these functions, avoiding the guess-based linear system entirely
 * rather than trying to make the guess smarter.
 */
public class H8ExtraPopAnalyzer extends AbstractAnalyzer {

    private static final String ANALYZER_NAME = "H8 ExtraPop/Return Purge Analyzer";
    private static final String DESCRIPTION =
        "Automatically sets function stack purge sizes from fixed-pop RTS/PRTS/RTE return "
        + "instructions, so the decompiler's extrapop solver doesn't need to guess.";

    // Fixed, unconditional pop sizes per h8539f-branch.sinc pcode:
    //   rts:  one  SP = SP + 2   (pops PC)                     -> 2 bytes
    //   prts: two  SP = SP + 2   (pops CP, then PC)            -> 4 bytes
    //   rte:  three SP = SP + 2  (pops SR, then CP, then PC)   -> 6 bytes
    private static final int RTS_PURGE = 2;
    private static final int PRTS_PURGE = 4;
    private static final int RTE_PURGE = 6;

    public H8ExtraPopAnalyzer() {
        super(ANALYZER_NAME, DESCRIPTION, AnalyzerType.FUNCTION_ANALYZER);
        // Same phase gate as H8FunctionPurgeAnalyzer -- run immediately after
        // initial function boundaries are mapped, before the decompiler's own
        // extrapop-derivation pass gets a chance to guess.
        setPriority(AnalysisPriority.FUNCTION_ANALYSIS.after());
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

        Listing listing = program.getListing();
        FunctionManager functionManager = program.getFunctionManager();

        InstructionIterator instructions = listing.getInstructions(set, true);

        while (instructions.hasNext()) {
            monitor.checkCancelled();

            Instruction insn = instructions.next();
            String mnemonic = insn.getMnemonicString();

            int purgeVal;
            if ("rts".equalsIgnoreCase(mnemonic)) {
                purgeVal = RTS_PURGE;
            } else if ("prts".equalsIgnoreCase(mnemonic)) {
                purgeVal = PRTS_PURGE;
            } else if ("rte".equalsIgnoreCase(mnemonic)) {
                purgeVal = RTE_PURGE;
            } else {
                continue;
            }

            Function func = functionManager.getFunctionContaining(insn.getAddress());
            if (func == null) {
                continue;
            }

            boolean alreadySet = func.isStackPurgeSizeValid() && func.getStackPurgeSize() != 0;

            if (alreadySet) {
                if (func.getStackPurgeSize() != purgeVal) {
                    // A different purge value was already recorded from an earlier
                    // return site in this same function (e.g. a mix of rts/prts/rte
                    // paths, or a prior PRTD/RTD hit) -- don't silently overwrite,
                    // since one of the two return points disagreeing means this
                    // function's real stack behavior needs a human look, not another
                    // guess layered on top of the first one.
                    log.appendMsg(ANALYZER_NAME, String.format(
                        "Conflicting purge values for function %s: existing %d, new %d from %s at %s -- keeping existing value",
                        func.getName(), func.getStackPurgeSize(), purgeVal, mnemonic,
                        insn.getAddressString(false, false)
                    ));
                }
                continue;
            }

            func.setStackPurgeSize(purgeVal);
            log.appendMsg(ANALYZER_NAME, String.format(
                "Auto-aligned stack frame purge to %d bytes for function %s at %s (from %s)",
                purgeVal, func.getName(), insn.getAddressString(false, false), mnemonic
            ));
        }
        return true;
    }
}

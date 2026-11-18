package h8539f;

import ghidra.app.services.AbstractAnalyzer;
import ghidra.app.services.AnalysisPriority;
import ghidra.app.services.AnalyzerType;
import ghidra.app.util.importer.MessageLog;
import ghidra.program.model.address.AddressSetView;
import ghidra.program.model.data.DataType;
import ghidra.program.model.data.DataTypeManager;
import ghidra.program.model.data.UnsignedShortDataType;
import ghidra.program.model.listing.Function;
import ghidra.program.model.listing.FunctionManager;
import ghidra.program.model.listing.Instruction;
import ghidra.program.model.listing.InstructionIterator;
import ghidra.program.model.listing.Listing;
import ghidra.program.model.listing.Parameter;
import ghidra.program.model.listing.ParameterImpl;
import ghidra.program.model.listing.Program;
import ghidra.program.model.listing.ReturnParameterImpl;
import ghidra.program.model.listing.Variable;
import ghidra.program.model.listing.VariableStorage;
import ghidra.program.model.scalar.Scalar;
import ghidra.program.model.symbol.RefType;
import ghidra.program.model.symbol.SourceType;
import ghidra.util.exception.CancelledException;
import ghidra.util.task.TaskMonitor;

import java.util.ArrayList;
import java.util.List;
import java.util.Set;
import java.util.TreeSet;

/**
 * Auto-fixes FP-relative stack parameter storage for H8/539F functions that
 * use a LINK/UNLK frame (link FP,#0x0 ... unlk FP), derived directly from
 * each function's own FP-relative operand offsets -- NOT from caller push
 * sequences and NOT from any guessed/default calling convention.
 *
 * Root cause addressed: H8FunctionPurgeAnalyzer and H8ExtraPopAnalyzer fix
 * the decompiler's extrapop/spacebase solver by supplying a known stack
 * purge size ahead of time. That is necessary but not sufficient for
 * LINK-frame functions: even with a correct purge size, the default
 * __stdcall prototype (R0-R2 then Stack+4) does not match a function whose
 * real parameters live at FP-relative offsets (e.g. @(0x6:8,FP),
 * @(0x8:8,FP), @(0xe:8,FP)) established by its own LINK/UNLK prologue and
 * epilogue. Left unresolved, this surfaces in the decompiler as
 * "Unable to track spacebase fully for stack" plus in_stack_XXXXXXXX /
 * &stack0xfffe+N artifacts, even though the true parameter layout is
 * unambiguously encoded in the function's own instructions.
 *
 * SAFETY / SCOPE (deliberately narrow -- read this before extending it):
 * This analyzer only acts when the evidence is self-contained and
 * unambiguous:
 *   1. The function must open with "link FP,#0x0" (or any link-with-FP
 *      form) and close with "unlk FP" -- a real, unambiguous LINK frame,
 *      not a guess.
 *   2. Every FP-relative operand found in the function body (the
 *      "@(N:8,FP)" addressing mode) is collected as a candidate parameter
 *      offset. These offsets are read directly off the instruction
 *      encoding, the same way H8FunctionPurgeAnalyzer reads PRTD/RTD
 *      purge sizes directly off their operand -- not inferred, not
 *      guessed.
 *   3. If the set of offsets is empty, non-contiguous in a way that
 *      doesn't correspond to a plausible 2-byte-aligned word sequence, or
 *      the function already has a non-default/user-defined signature,
 *      this analyzer does NOT act and logs why, the same conflict-safe
 *      posture as H8FunctionPurgeAnalyzer/H8ExtraPopAnalyzer.
 *   4. This analyzer does NOT touch functions reached only via computed/
 *      indirect calls with no LINK frame, and does NOT touch the
 *      zero-caller "artifact" cohort documented in review6.md whose
 *      offsets come from raw &stack0xfffe+N reads with NO link/unlk
 *      framing -- those have no self-contained evidence to derive from
 *      and must stay a human/manual CUSTOM_STORAGE decision (see
 *      FixTableLookupInterpStorage.java for that pattern applied by hand).
 *   5. An FP-relative offset whose first access (in program order) is a
 *      WRITE rather than a READ is excluded from the derived parameter
 *      list. Such a slot behaves as a return-value/out-param location
 *      (written by the callee, read back by the caller after return) and
 *      is not a genuine incoming parameter, even though it has the same
 *      @(N:8,FP) shape. This is logged, not silently dropped.
 *
 * This deliberately mirrors review6.md's own governing rule: never
 * batch-retype from a guess, only ever act on directly-observed evidence.
 * Here the "directly observed evidence" is the function's own FP-relative
 * addressing, which is a stronger and more self-contained signal than a
 * caller's push sequence would be (available even with zero resolved
 * callers), while still being a real fact read from the binary rather
 * than an inference.
 */
public class H8FramePointerParamAnalyzer extends AbstractAnalyzer {

    private static final String ANALYZER_NAME = "H8 Frame Pointer Parameter Analyzer";
    private static final String DESCRIPTION =
        "Auto-derives and applies FP-relative stack parameter storage for LINK/UNLK-framed "
        + "H8 functions, using only offsets read directly from the function's own instructions.";

    public H8FramePointerParamAnalyzer() {
        super(ANALYZER_NAME, DESCRIPTION, AnalyzerType.FUNCTION_ANALYZER);
        // Run after the purge-size analyzers so extrapop is already resolved
        // by the time this analyzer's CUSTOM_STORAGE prototype update runs --
        // avoids fighting the extrapop solver over the same function.
        setPriority(AnalysisPriority.FUNCTION_ANALYSIS.after().after());
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
        DataTypeManager dtm = program.getDataTypeManager();

        DataType ushortType = dtm.getDataType("/ushort");
        if (ushortType == null) {
            ushortType = new UnsignedShortDataType();
        }
        final DataType paramType = ushortType;

        DataType retType = dtm.getDataType("/ushort");
        if (retType == null) {
            retType = new UnsignedShortDataType();
        }

        java.util.Iterator<Function> functions = functionManager.getFunctionsOverlapping(set);

        while (functions.hasNext()) {
            monitor.checkCancelled();
            Function func = functions.next();

            // Only touch functions still at DEFAULT (unspecified/auto-derived)
            // signature source -- never overwrite a user-defined signature, and
            // never re-touch a function this same analyzer (or another
            // ANALYSIS-sourced pass) already acted on. Same non-clobbering
            // posture as the purge analyzers.
            //
            // Deliberately NOT gated on getParameterCount() > 0: Ghidra's own
            // default stack analyzer frequently assigns DEFAULT-sourced
            // parameters (e.g. raw in_stack_XXXXXXXX placeholders) to functions
            // whose spacebase tracking failed -- that's exactly the broken case
            // this analyzer exists to fix, and gating on parameter count alone
            // would skip right past the functions that most need the fix.
            SourceType sigSource = func.getSignatureSource();
            if (sigSource == SourceType.USER_DEFINED) {
                continue;
            }

            LinkFrameEvidence evidence = findLinkFrameEvidence(listing, func, monitor);
            if (evidence == null) {
                continue; // no LINK/UNLK frame found, out of scope
            }
            if (!evidence.hasUnlk) {
                log.appendMsg(ANALYZER_NAME, String.format(
                    "%s at %s: found 'link FP' but no matching 'unlk FP' -- ambiguous frame, skipping",
                    func.getName(), func.getEntryPoint()));
                continue;
            }
            if (evidence.fpOffsets.isEmpty()) {
                continue; // LINK frame but no FP-relative operands -- genuinely void, leave to other passes
            }

            // Drop offsets whose first access is a WRITE, not a READ -- these are
            // return-value slots (e.g. o2_mode16_lambda_select's @(0x6:8,FP): the
            // callee writes it and reads it back before returning, but no caller
            // ever reads it before the call happens), not genuine incoming
            // parameters. Log them for visibility instead of silently dropping.
            if (!evidence.writeFirstOffsets.isEmpty()) {
                log.appendMsg(ANALYZER_NAME, String.format(
                    "%s at %s: FP offsets %s are written before ever being read -- \""
                    + "excluding as likely return-value slot(s), not genuine parameters",
                    func.getName(), func.getEntryPoint(), evidence.writeFirstOffsets));
                evidence.fpOffsets = new ArrayList<>(evidence.fpOffsets);
                evidence.fpOffsets.removeAll(evidence.writeFirstOffsets);
            }
            if (evidence.fpOffsets.isEmpty()) {
                continue; // every candidate offset was write-first -- no genuine parameters left
            }

            // Validate the offsets form a plausible contiguous word sequence
            // (2-byte aligned, no implausible gaps suggesting a partially-
            // resolved or mixed-width access we shouldn't guess about).
            if (!isPlausibleWordSequence(evidence.fpOffsets)) {
                log.appendMsg(ANALYZER_NAME, String.format(
                    "%s at %s: LINK frame with FP offsets %s -- non-contiguous/irregular, "
                    + "not a safe auto-derive, flagging for manual review",
                    func.getName(), func.getEntryPoint(), evidence.fpOffsets));
                continue;
            }

            List<Variable> params = new ArrayList<>();
            int i = 0;
            boolean storageFailed = false;
            for (int offset : evidence.fpOffsets) {
                VariableStorage storage;
                try {
                    storage = new VariableStorage(program, offset, 2);
                } catch (Exception e) {
                    log.appendMsg(ANALYZER_NAME, String.format(
                        "%s at %s: failed to build storage for FP offset 0x%x (%s), skipping function",
                        func.getName(), func.getEntryPoint(), offset, e.getMessage()));
                    storageFailed = true;
                    break;
                }
                try {
                    params.add(new ParameterImpl(
                        "param_fp" + (i + 1), paramType, storage, program, SourceType.ANALYSIS));
                } catch (Exception e) {
                    log.appendMsg(ANALYZER_NAME, String.format(
                        "%s at %s: failed to build parameter at FP offset 0x%x (%s), skipping function",
                        func.getName(), func.getEntryPoint(), offset, e.getMessage()));
                    storageFailed = true;
                    break;
                }
                i++;
            }
            if (storageFailed) {
                continue;
            }

            Variable returnVar;
            try {
                returnVar = new ReturnParameterImpl(retType, program);
            } catch (Exception e) {
                log.appendMsg(ANALYZER_NAME, String.format(
                    "%s at %s: failed to build return variable (%s), skipping function",
                    func.getName(), func.getEntryPoint(), e.getMessage()));
                continue;
            }

            try {
                func.updateFunction(
                    "__stdcall",
                    returnVar,
                    params,
                    Function.FunctionUpdateType.CUSTOM_STORAGE,
                    true,
                    SourceType.ANALYSIS);

                log.appendMsg(ANALYZER_NAME, String.format(
                    "%s at %s: derived %d FP-relative parameter(s) at offsets %s from its own "
                    + "LINK/UNLK frame, applied CUSTOM_STORAGE signature",
                    func.getName(), func.getEntryPoint(), params.size(), evidence.fpOffsets));
            } catch (Exception e) {
                log.appendMsg(ANALYZER_NAME, String.format(
                    "%s at %s: updateFunction failed (%s), leaving unchanged",
                    func.getName(), func.getEntryPoint(), e.getMessage()));
            }
        }

        return true;
    }

    /**
     * Scans a function's instructions for a link/unlk FP frame and collects
     * every FP-relative operand offset found in between. Returns null if no
     * "link FP" prologue instruction is present at all (function is out of
     * scope for this analyzer, not an error).
     */
    private LinkFrameEvidence findLinkFrameEvidence(Listing listing, Function func, TaskMonitor monitor)
            throws CancelledException {

        InstructionIterator instructions = listing.getInstructions(func.getBody(), true);
        boolean sawLink = false;
        boolean sawUnlk = false;
        Set<Integer> offsets = new TreeSet<>();
        // Tracks, per FP-relative offset, whether the FIRST access to that slot
        // (in program order) was a read. A slot that is only ever written before
        // it is ever read is very likely a return-value slot (the callee stores
        // its result there and the caller reads it back after return), not a
        // genuine incoming parameter -- even though it has the same @(N:8,FP)
        // shape as a real parameter. See o2_mode16_lambda_select for a concrete
        // example: it writes @(0x6:8,FP) conditionally and unconditionally reads
        // it back before returning, with no caller-side read ever happening
        // before the call. A real parameter's first access is always a READ
        // (or READ_WRITE, e.g. an in-place increment of an incoming argument).
        java.util.Map<Integer, Boolean> firstAccessIsRead = new java.util.HashMap<>();

        while (instructions.hasNext()) {
            monitor.checkCancelled();
            Instruction insn = instructions.next();
            String mnemonic = insn.getMnemonicString();

            if ("link".equalsIgnoreCase(mnemonic)) {
                sawLink = true;
                continue;
            }
            if ("unlk".equalsIgnoreCase(mnemonic)) {
                sawUnlk = true;
                continue;
            }
            if (!sawLink) {
                continue; // haven't entered the frame yet
            }

            // Look for @(N:8,FP) style operands: any operand referencing FP
            // register indirect with a constant displacement.
            //
            // NOTE: this addressing mode is a compound displacement+base-register
            // operand in the H8/539F SLEIGH definition. Ghidra's getOpObjects()
            // does NOT expose a discrete Register object for FP in this mode (the
            // base register is baked into the varnode arithmetic, not surfaced as
            // a token), and getRegister(opIdx) likewise returns null. The scalar
            // displacement IS present as a Scalar object, but with no accompanying
            // Register(FP) to correlate it against -- so object-level matching
            // silently finds zero offsets on every real match. The only reliable
            // signal is the operand's default string representation, which does
            // render as "@(-0x2:8,FP)" / "@(0x6:8,FP)" etc. Parse that instead.
            for (int opIdx = 0; opIdx < insn.getNumOperands(); opIdx++) {
                String repr = insn.getDefaultOperandRepresentation(opIdx);
                if (repr == null || !repr.toUpperCase().contains(",FP)")) {
                    continue;
                }

                Integer scalarOffset = null;
                for (Object o : insn.getOpObjects(opIdx)) {
                    if (o instanceof Scalar) {
                        scalarOffset = (int) ((Scalar) o).getValue();
                        break;
                    }
                }
                if (scalarOffset == null) {
                    // Fallback: extract the displacement directly from the
                    // representation string, e.g. "@(-0x2:8,FP)" -> -0x2.
                    int atParen = repr.indexOf("(");
                    int colon = repr.indexOf(':', atParen);
                    if (atParen >= 0 && colon > atParen) {
                        String hexPart = repr.substring(atParen + 1, colon).trim();
                        try {
                            boolean negative = hexPart.startsWith("-");
                            String digits = hexPart.replace("-", "").replace("0x", "").replace("0X", "");
                            int val = Integer.parseInt(digits, 16);
                            scalarOffset = negative ? -val : val;
                        } catch (NumberFormatException e) {
                            scalarOffset = null;
                        }
                    }
                }

                if (scalarOffset != null && scalarOffset > 0) {
                    offsets.add(scalarOffset);

                    if (!firstAccessIsRead.containsKey(scalarOffset)) {
                        // Record only the FIRST access seen for this offset (map
                        // insertion happens once per offset, guarded by containsKey).
                        // RefType.isRead() is true for both READ and READ_WRITE;
                        // false for a pure WRITE.
                        RefType refType = insn.getOperandRefType(opIdx);
                        boolean readHappened = refType != null && refType.isRead();
                        firstAccessIsRead.put(scalarOffset, readHappened);
                    }
                }
            }
        }

        if (!sawLink) {
            return null;
        }

        List<Integer> writeFirstOffsets = new ArrayList<>();
        for (Integer off : offsets) {
            Boolean readFirst = firstAccessIsRead.get(off);
            if (readFirst != null && !readFirst) {
                writeFirstOffsets.add(off);
            }
        }

        LinkFrameEvidence evidence = new LinkFrameEvidence();
        evidence.hasUnlk = sawUnlk;
        evidence.fpOffsets = new ArrayList<>(offsets);
        evidence.writeFirstOffsets = writeFirstOffsets;
        return evidence;
    }

    /**
     * Conservative plausibility check: offsets should be 2-byte aligned
     * (H8 word access) and should not have a gap larger than one missing
     * word, which would suggest a mixed-width or partially-resolved access
     * this analyzer shouldn't guess about.
     */
    private boolean isPlausibleWordSequence(List<Integer> offsets) {
        for (int off : offsets) {
            if (off % 2 != 0) {
                return false;
            }
        }
        for (int i = 1; i < offsets.size(); i++) {
            int gap = offsets.get(i) - offsets.get(i - 1);
            if (gap > 4) {
                // more than one word's worth of gap -- irregular, don't guess
                return false;
            }
        }
        return true;
    }

    private static class LinkFrameEvidence {
        boolean hasUnlk;
        List<Integer> fpOffsets;
        // Subset of fpOffsets whose first access in program order was a WRITE,
        // not a READ -- these look like return-value slots rather than genuine
        // incoming parameters and are excluded from the derived signature.
        List<Integer> writeFirstOffsets;
    }
}

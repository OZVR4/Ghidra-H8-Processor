package h8539f;

import ghidra.app.cmd.disassemble.DisassembleCommand;
import ghidra.app.cmd.function.CreateFunctionCmd;
import ghidra.app.services.AbstractAnalyzer;
import ghidra.app.services.AnalysisPriority;
import ghidra.app.services.AnalyzerType;
import ghidra.app.util.importer.MessageLog;
import ghidra.program.model.address.Address;
import ghidra.program.model.address.AddressSetView;
import ghidra.program.model.listing.Function;
import ghidra.program.model.listing.FunctionManager;
import ghidra.program.model.listing.Program;
import ghidra.program.model.mem.Memory;
import ghidra.program.model.mem.MemoryAccessException;
import ghidra.program.model.symbol.RefType;
import ghidra.program.model.symbol.ReferenceManager;
import ghidra.program.model.symbol.SourceType;
import ghidra.util.exception.CancelledException;
import ghidra.util.task.TaskMonitor;

/**
 * Resolves the f502_state_table_dispatch computed-call mechanism, documented
 * in review8.md: a `cmp:g.b @0xf502` / `mulxu.w #0x6,Rn` / `pjsr @Rn` idiom
 * at 0x27e9c-0x27ed4 that dispatches through two contiguous 6-byte-record
 * state tables (base 0x1369a, 12 records, states 0-11; base 0x136e2, 9
 * records, states 12-20). Each record is
 * [flags:16][pad:8][bank:8][offset:16]; the real handler address is
 * bank&lt;&lt;16 | offset.
 *
 * ROOT CAUSE ADDRESSED: this is a genuine computed indirect SUBROUTINE call
 * (pjsr, not a jmp/switch Ghidra's own jump-table recovery watches for), so
 * Ghidra's static reference analysis cannot see it. Of the 21 handler
 * targets, 4 already exist as Functions but sit at zero xrefs (misleading
 * review6.md's zero-caller cohort sweep into calling them dead code -- see
 * review8.md for the correction), and 16 more are not disassembled as
 * functions at all, sitting as raw bytes in a gap Ghidra's own auto-analysis
 * never reached because nothing pointed at them.
 *
 * SAFETY / SCOPE (v1, deliberately narrow -- read review8.md's PLAN section
 * before extending this):
 *   1. Only the two specific, independently-verified table addresses from
 *      review8.md are consulted (0x1369a/12 records, 0x136e2/9 records).
 *      This analyzer does NOT scan the ROM for other instances of the same
 *      instruction idiom -- that's an explicitly out-of-scope v2, so a
 *      lookalike pattern elsewhere is never silently swept up here.
 *   2. Every record's bank byte is verified to be the expected constant
 *      (0x02) and its pad byte to be 0x00 before the record is trusted. A
 *      record that doesn't match is logged and skipped, not guessed at --
 *      same conflict-safe posture as H8FramePointerParamAnalyzer.
 *   3. This analyzer only CREATES functions where none exist and only ADDS
 *      a COMPUTED_CALL reference from the known pjsr site to the resolved
 *      target. It never modifies an existing function's signature,
 *      prototype, or return type -- that remains H8FramePointerParamAnalyzer
 *      and review4.md/review6.md's job, intended to run AFTER this one.
 *   4. The flags field is left untouched; its meaning (a per-state bitmask
 *      condition, see review8.md) is out of scope for this analyzer.
 */
public class H8StateTableDispatchAnalyzer extends AbstractAnalyzer {

    private static final String ANALYZER_NAME = "H8 State Table Dispatch Analyzer";
    private static final String DESCRIPTION =
        "Resolves the f502_state_table_dispatch computed pjsr mechanism (review8.md): creates "
        + "functions at previously-undisassembled state-table handler targets and adds "
        + "COMPUTED_CALL references so the xref graph reflects real callers.";

    // Known-good, independently-verified dispatch site and table bases (review8.md).
    private static final String DISPATCH_FUNC_ADDR = "00027e9c";
    private static final String PJSR_SITE_ADDR = "00027ed4";
    private static final String TABLE1_BASE = "0001369a";
    private static final int TABLE1_RECORD_COUNT = 12;
    private static final String TABLE2_BASE = "000136e2";
    private static final int TABLE2_RECORD_COUNT = 9;

    private static final int RECORD_SIZE = 6;
    private static final byte EXPECTED_BANK = 0x02;
    private static final byte EXPECTED_PAD = 0x00;

    public H8StateTableDispatchAnalyzer() {
        super(ANALYZER_NAME, DESCRIPTION, AnalyzerType.FUNCTION_ANALYZER);
        // Run after the frame-pointer/purge analyzers have settled function
        // boundaries generally, but functions this analyzer creates are new
        // to the program, so a later full re-analysis pass (or a second run)
        // is expected to pick them up for FP-relative param derivation, etc.
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

        // This analyzer targets two specific, known addresses rather than
        // the changed address set -- it only needs to run meaningfully
        // once (or idempotently re-verify) regardless of what triggered
        // this analysis pass, since the table locations are fixed evidence
        // from review8.md, not derived from whatever region is being
        // (re)analyzed right now.
        Address dispatchFuncAddr = program.getAddressFactory().getAddress(DISPATCH_FUNC_ADDR);
        Address pjsrSite = program.getAddressFactory().getAddress(PJSR_SITE_ADDR);

        FunctionManager functionManager = program.getFunctionManager();
        Function dispatchFunc = functionManager.getFunctionAt(dispatchFuncAddr);
        if (dispatchFunc == null) {
            log.appendMsg(ANALYZER_NAME, String.format(
                "Expected dispatch function at %s (f502_state_table_dispatch) not found -- "
                + "skipping, table addresses may not apply to this program", DISPATCH_FUNC_ADDR));
            return true;
        }

        int created = 0;
        int existed = 0;
        int refsAdded = 0;
        int skipped = 0;

        String[] bases = { TABLE1_BASE, TABLE2_BASE };
        int[] counts = { TABLE1_RECORD_COUNT, TABLE2_RECORD_COUNT };

        for (int t = 0; t < bases.length; t++) {
            monitor.checkCancelled();
            Address tableBase = program.getAddressFactory().getAddress(bases[t]);
            int recordCount = counts[t];

            for (int i = 0; i < recordCount; i++) {
                monitor.checkCancelled();
                Address recordAddr = tableBase.add((long) i * RECORD_SIZE);

                int[] decoded = decodeRecord(program, recordAddr, log);
                if (decoded == null) {
                    skipped++;
                    continue; // bad bank/pad byte, already logged by decodeRecord
                }
                int flags = decoded[0];
                int targetOffset = decoded[1];
                Address target = program.getAddressFactory().getAddress(
                    String.format("%08x", targetOffset | (EXPECTED_BANK << 16)));

                Function existingFunc = functionManager.getFunctionAt(target);
                if (existingFunc == null) {
                    // No function at this address yet -- disassemble then create one.
                    // Both steps are mechanical given an address that is now real,
                    // independently-verified evidence (review8.md), not a guess.
                    DisassembleCommand disasmCmd = new DisassembleCommand(target, null, true);
                    boolean disasmOk = disasmCmd.applyTo(program, monitor);
                    if (!disasmOk) {
                        log.appendMsg(ANALYZER_NAME, String.format(
                            "Table base %s record %d (flags=0x%04x): disassemble failed at %s (%s), skipping",
                            bases[t], i, flags, target, disasmCmd.getStatusMsg()));
                        skipped++;
                        continue;
                    }

                    CreateFunctionCmd createCmd = new CreateFunctionCmd(target);
                    boolean createOk = createCmd.applyTo(program, monitor);
                    if (!createOk) {
                        log.appendMsg(ANALYZER_NAME, String.format(
                            "Table base %s record %d (flags=0x%04x): createFunction failed at %s (%s), skipping",
                            bases[t], i, flags, target, createCmd.getStatusMsg()));
                        skipped++;
                        continue;
                    }

                    existingFunc = functionManager.getFunctionAt(target);
                    if (existingFunc == null) {
                        log.appendMsg(ANALYZER_NAME, String.format(
                            "Table base %s record %d (flags=0x%04x): function creation reported success "
                            + "but no function found at %s afterward, skipping reference add",
                            bases[t], i, flags, target));
                        skipped++;
                        continue;
                    }

                    created++;
                    log.appendMsg(ANALYZER_NAME, String.format(
                        "Table base %s record %d (flags=0x%04x): created function at %s",
                        bases[t], i, flags, target));
                } else {
                    existed++;
                }

                // Add the COMPUTED_CALL reference regardless of whether the function
                // was just created or already existed -- either way it may be missing
                // (this is exactly the review6.md zero-xref symptom for the 4 that
                // already existed as functions).
                ReferenceManager refManager = program.getReferenceManager();
                boolean alreadyReferenced = false;
                for (ghidra.program.model.symbol.Reference existingRef
                        : refManager.getReferencesFrom(pjsrSite)) {
                    if (existingRef.getToAddress().equals(target)) {
                        alreadyReferenced = true;
                        break;
                    }
                }
                if (!alreadyReferenced) {
                    refManager.addMemoryReference(
                        pjsrSite, target, RefType.COMPUTED_CALL, SourceType.ANALYSIS, 0);
                    refsAdded++;
                    log.appendMsg(ANALYZER_NAME, String.format(
                        "Table base %s record %d (flags=0x%04x): added COMPUTED_CALL reference %s -> %s",
                        bases[t], i, flags, pjsrSite, target));
                }
            }
        }

        log.appendMsg(ANALYZER_NAME, String.format(
            "Done: %d function(s) created, %d already existed, %d reference(s) added, %d record(s) skipped",
            created, existed, refsAdded, skipped));

        return true;
    }

    /**
     * Reads and validates one 6-byte record at recordAddr:
     * [flags:16][pad:8][bank:8][offset:16].
     * Returns {flags, offset} on success, or null (after logging why) if the
     * bank/pad bytes don't match the expected constants -- same
     * conflict-safe posture as H8FramePointerParamAnalyzer: don't guess
     * about a record that doesn't match the confirmed shape.
     */
    private int[] decodeRecord(Program program, Address recordAddr, MessageLog log) {
        Memory mem = program.getMemory();
        try {
            byte[] raw = new byte[RECORD_SIZE];
            int read = mem.getBytes(recordAddr, raw);
            if (read != RECORD_SIZE) {
                log.appendMsg(ANALYZER_NAME, String.format(
                    "Record at %s: short read (%d of %d bytes), skipping", recordAddr, read, RECORD_SIZE));
                return null;
            }

            int flags = ((raw[0] & 0xFF) << 8) | (raw[1] & 0xFF);
            byte pad = raw[2];
            byte bank = raw[3];
            int offset = ((raw[4] & 0xFF) << 8) | (raw[5] & 0xFF);

            if (bank != EXPECTED_BANK) {
                log.appendMsg(ANALYZER_NAME, String.format(
                    "Record at %s: unexpected bank byte 0x%02x (expected 0x%02x), skipping -- "
                    + "not a safe auto-derive, flagging for manual review",
                    recordAddr, bank, EXPECTED_BANK));
                return null;
            }
            if (pad != EXPECTED_PAD) {
                log.appendMsg(ANALYZER_NAME, String.format(
                    "Record at %s: unexpected pad byte 0x%02x (expected 0x%02x), skipping -- "
                    + "not a safe auto-derive, flagging for manual review",
                    recordAddr, pad, EXPECTED_PAD));
                return null;
            }

            return new int[] { flags, offset };
        } catch (MemoryAccessException e) {
            log.appendMsg(ANALYZER_NAME, String.format(
                "Record at %s: memory access error (%s), skipping", recordAddr, e.getMessage()));
            return null;
        }
    }
}

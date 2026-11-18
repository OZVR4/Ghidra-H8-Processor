// For each Batch 2 "Unknown"-category table address with a known literal-push caller
// (from FindBatch2Callers.java), scans backward within the same function for the
// immediately-preceding push-pair+pjsr event. If that preceding event's pjsr target is
// axis_lookup_interp (0x14735), reports its pushed (bank,offset) as the candidate axis --
// the established pattern from review2.md items 11/15 (axis_lookup_interp call sits
// immediately before the table's own table_lookup_interp/table_lookup_indexed call,
// sharing one axis for whichever table(s) follow). This is a LEAD, not proof -- still
// needs the usual byte-level header/data sanity check before writing to XML, per Method
// step 4's caveat.
// @category H8539F

import ghidra.app.script.GhidraScript;
import ghidra.program.model.address.Address;
import ghidra.program.model.listing.Instruction;
import ghidra.program.model.listing.InstructionIterator;
import ghidra.program.model.listing.Function;
import ghidra.program.model.scalar.Scalar;

import java.util.ArrayList;
import java.util.List;
import java.util.HashMap;
import java.util.Map;

public class FindBatch2Axes extends GhidraScript {

    static final long AXIS_LOOKUP_INTERP = 0x14735L;
    static final long TABLE_LOOKUP_INTERP = 0x14656L;
    static final long TABLE_LOOKUP_INDEXED = 0x14854L;

    static class PushEvent {
        int offset;
        int bank;
        Address bankPushAddr;
        Function fn;
        long pjsrTarget = -1;
    }

    @Override
    public void run() throws Exception {
        int[] addrs = new int[] {
            0x10D73, 0x11834, 0x1186C, 0x11880, 0x11894, 0x118A0, 0x11D20, 0x11D4C,
            0x11DEC, 0x11E4A, 0x11E6A, 0x11E86, 0x11E94, 0x11EA0, 0x11EC8, 0x11ED6,
            0x11EE2, 0x11F16, 0x11F30, 0x11F3C, 0x11F4A, 0x11F5C, 0x11F64, 0x11F6C,
            0x127A8, 0x127D8, 0x127FC, 0x12824, 0x1284E, 0x12858, 0x12860, 0x1286C,
            0x128D2, 0x128DE, 0x128EA, 0x12926, 0x12950, 0x1297A, 0x12986, 0x12AE6,
            0x12B06, 0x12B94, 0x12CBC, 0x12CD2, 0x12CE8, 0x12CF4, 0x12D00, 0x12D70,
            0x12DD6, 0x12DE4, 0x12DF2, 0x12E4A, 0x12EA2, 0x12EAE, 0x12EBC, 0x12ECE,
            0x12EDC, 0x12EEA, 0x12EF8, 0x12F06, 0x12F14, 0x1313E, 0x1314C, 0x13154,
            0x1315C, 0x13168, 0x13174, 0x1317C, 0x13188, 0x13190, 0x13198, 0x131A0,
            0x131A8, 0x131B0, 0x131B8, 0x131C0, 0x131CC, 0x131D4, 0x131E0, 0x131EC,
            0x131F6, 0x13202, 0x1320A, 0x13212, 0x1321C, 0x13236, 0x13250, 0x1325E,
            0x1326C, 0x1327A, 0x2D3C0, 0x2D5EC, 0x2D668, 0x2D796
        };

        // Build per-function ordered list of push-pair+pjsr events across the whole program.
        Map<Function, List<PushEvent>> byFunction = new HashMap<>();

        InstructionIterator it = currentProgram.getListing().getInstructions(true);
        Integer pendingOffset = null;
        Address pendingOffsetAt = null;

        while (it.hasNext()) {
            Instruction insn = it.next();
            String mnem = insn.getMnemonicString();
            String rep = insn.toString();

            if (mnem.startsWith("mov") && rep.contains("@-SP")) {
                Scalar sc = null;
                try { sc = insn.getScalar(0); } catch (Exception e) {}
                if (sc != null) {
                    long val = sc.getUnsignedValue();
                    if (rep.contains(":16")) {
                        pendingOffset = (int) val;
                        pendingOffsetAt = insn.getAddress();
                    } else if (rep.contains(":8") && pendingOffset != null) {
                        int bank = (int) val;
                        Function f = getFunctionContaining(insn.getAddress());
                        if (f != null) {
                            PushEvent ev = new PushEvent();
                            ev.offset = pendingOffset;
                            ev.bank = bank;
                            ev.bankPushAddr = insn.getAddress();
                            ev.fn = f;
                            byFunction.computeIfAbsent(f, k -> new ArrayList<>()).add(ev);
                        }
                        pendingOffset = null;
                    }
                }
            } else if (mnem.equalsIgnoreCase("pjsr")) {
                // Attach pjsr target to the most recent push event in this function, if any
                Function f = getFunctionContaining(insn.getAddress());
                if (f != null) {
                    List<PushEvent> evs = byFunction.get(f);
                    if (evs != null && !evs.isEmpty()) {
                        PushEvent last = evs.get(evs.size() - 1);
                        if (last.pjsrTarget == -1) {
                            try {
                                Address[] flows = insn.getFlows();
                                if (flows != null && flows.length > 0) {
                                    last.pjsrTarget = flows[0].getOffset();
                                }
                            } catch (Exception e) {}
                        }
                    }
                }
            }
        }

        StringBuilder sb = new StringBuilder();
        for (int a : addrs) {
            int wantBank = (a >> 16) & 0xFF;
            int wantOff = a & 0xFFFF;
            String key = String.format("%06X", a);

            // find the matching event for this table address
            PushEvent match = null;
            List<PushEvent> matchList = null;
            int matchIdx = -1;
            for (Map.Entry<Function, List<PushEvent>> e : byFunction.entrySet()) {
                List<PushEvent> evs = e.getValue();
                for (int i = 0; i < evs.size(); i++) {
                    PushEvent ev = evs.get(i);
                    if (ev.bank == wantBank && ev.offset == wantOff) {
                        match = ev;
                        matchList = evs;
                        matchIdx = i;
                        break;
                    }
                }
                if (match != null) break;
            }

            if (match == null) {
                sb.append(key).append(": NO CALLER FOUND\n");
                continue;
            }

            if (matchIdx == 0) {
                sb.append(key).append(" @ ").append(match.bankPushAddr)
                  .append(" (in ").append(match.fn.getName())
                  .append("): NO PRECEDING PUSH-EVENT IN FUNCTION\n");
                continue;
            }

            PushEvent prev = matchList.get(matchIdx - 1);
            String axisAddr = String.format("%06X", (prev.bank << 16) | prev.offset);
            String verdict;
            if (prev.pjsrTarget == AXIS_LOOKUP_INTERP) {
                verdict = "AXIS_LOOKUP_INTERP MATCH -- candidate axis";
            } else if (prev.pjsrTarget == TABLE_LOOKUP_INTERP || prev.pjsrTarget == TABLE_LOOKUP_INDEXED) {
                verdict = "preceding event is ANOTHER TABLE call, not axis -- no axis found here";
            } else if (prev.pjsrTarget == -1) {
                verdict = "preceding event has NO pjsr immediately after it -- inconclusive";
            } else {
                verdict = String.format("preceding event pjsr target = %06X (unrecognized) -- inconclusive", prev.pjsrTarget);
            }

            sb.append(key).append(" @ ").append(match.bankPushAddr)
              .append(" (in ").append(match.fn.getName()).append(") <- prev push ")
              .append(axisAddr).append(" @ ").append(prev.bankPushAddr)
              .append(" [pjsr=").append(prev.pjsrTarget == -1 ? "none" : String.format("%06X", prev.pjsrTarget))
              .append("] :: ").append(verdict).append("\n");
        }

        println(sb.toString());
    }
}

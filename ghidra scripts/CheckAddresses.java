// General-purpose address checker: reports, for each hardcoded address in `addrs`,
// whether it's used as a literal-push argument anywhere in the program, its caller
// function, and (if a preceding push-pair+pjsr event exists in the same function)
// whether that preceding event is an axis_lookup_interp call. Reusable across
// verification passes -- edit the addrs array and rerun.
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

public class CheckAddresses extends GhidraScript {

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
        int[] addrs = new int[] { 0x11DB8, 0x2D2E6, 0x12F84 };

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

            sb.append(key).append(" @ ").append(match.bankPushAddr)
              .append(" (in ").append(match.fn.getName()).append(") own pjsr=")
              .append(match.pjsrTarget == -1 ? "none" : String.format("%06X", match.pjsrTarget))
              .append("\n");

            if (matchIdx == 0) {
                sb.append("  (no preceding push-event in function)\n");
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

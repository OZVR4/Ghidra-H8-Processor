// Improved version: reads the XML address list directly from the file at
// runtime (avoids manual transcription errors), and checks addr, addr+4,
// and addr-4 against the XML set to account for the header-vs-header+4
// convention ambiguity found repeatedly this session (e.g. 11CCC vs 11CD0).
// Also separates ARRAY-BASE addresses (used only as input to table_lookup_
// indexed/table_read_indexed) from genuine direct table_lookup_interp calls,
// since array bases are not table addresses themselves.
// @category H8539F

import ghidra.app.script.GhidraScript;
import ghidra.program.model.address.Address;
import ghidra.program.model.listing.Instruction;
import ghidra.program.model.listing.InstructionIterator;
import ghidra.program.model.listing.Function;
import ghidra.program.model.scalar.Scalar;

import java.io.BufferedReader;
import java.io.FileReader;
import java.util.ArrayList;
import java.util.HashSet;
import java.util.List;
import java.util.Set;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

public class FindAllTableCallsVsXmlV2 extends GhidraScript {

    static final long TABLE_LOOKUP_INTERP = 0x14656L;
    static final long TABLE_LOOKUP_INDEXED = 0x14854L;
    static final long TABLE_READ_INDEXED = 0x14880L;

    static class PushEvent {
        int offset;
        int bank;
        Address bankPushAddr;
        Function fn;
        long pjsrTarget = -1;
    }

    @Override
    public void run() throws Exception {
        String xmlPath = "C:\\Users\\j.brophy.CORKILLSYSTEMS\\github\\Ghidra-H8-Processor\\test\\rvr\\xml\\ecuflash\\new\\21000011_1997-2001_RVR_X3_Mt__4g63t_.xml";
        Set<Integer> xmlAddrs = new HashSet<>();
        Pattern p = Pattern.compile("address=\"([0-9A-Fa-f]+)\"");
        try (BufferedReader r = new BufferedReader(new FileReader(xmlPath))) {
            String line;
            while ((line = r.readLine()) != null) {
                Matcher m = p.matcher(line);
                while (m.find()) {
                    try {
                        xmlAddrs.add(Integer.parseInt(m.group(1), 16));
                    } catch (Exception e) {}
                }
            }
        }
        println("Parsed " + xmlAddrs.size() + " distinct addresses from XML file.");

        List<PushEvent> allEvents = new ArrayList<>();
        InstructionIterator it = currentProgram.getListing().getInstructions(true);
        Integer pendingOffset = null;

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
                    } else if (rep.contains(":8") && pendingOffset != null) {
                        int bank = (int) val;
                        Function f = getFunctionContaining(insn.getAddress());
                        if (f != null) {
                            PushEvent ev = new PushEvent();
                            ev.offset = pendingOffset;
                            ev.bank = bank;
                            ev.bankPushAddr = insn.getAddress();
                            ev.fn = f;
                            allEvents.add(ev);
                        }
                        pendingOffset = null;
                    }
                }
            } else if (mnem.equalsIgnoreCase("pjsr")) {
                if (!allEvents.isEmpty()) {
                    PushEvent last = allEvents.get(allEvents.size() - 1);
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

        StringBuilder directMissing = new StringBuilder();
        StringBuilder arrayBaseInfo = new StringBuilder();
        Set<Integer> seenDirect = new HashSet<>();
        Set<Integer> seenArray = new HashSet<>();
        int directCalls = 0;
        int indexedCalls = 0;

        for (PushEvent ev : allEvents) {
            int fullAddr = (ev.bank << 16) | ev.offset;
            if (ev.pjsrTarget == TABLE_LOOKUP_INTERP) {
                directCalls++;
                boolean present = xmlAddrs.contains(fullAddr)
                    || xmlAddrs.contains(fullAddr + 4)
                    || xmlAddrs.contains(fullAddr - 4);
                if (!present && !seenDirect.contains(fullAddr)) {
                    seenDirect.add(fullAddr);
                    directMissing.append(String.format("%06X @ %s (in %s)\n",
                        fullAddr, ev.bankPushAddr, ev.fn.getName()));
                }
            } else if (ev.pjsrTarget == TABLE_LOOKUP_INDEXED || ev.pjsrTarget == TABLE_READ_INDEXED) {
                indexedCalls++;
                if (!seenArray.contains(fullAddr)) {
                    seenArray.add(fullAddr);
                    arrayBaseInfo.append(String.format("%06X @ %s (in %s) [ARRAY BASE, not a table addr]\n",
                        fullAddr, ev.bankPushAddr, ev.fn.getName()));
                }
            }
        }

        println("Direct table_lookup_interp calls: " + directCalls + ", distinct genuinely-missing: " + seenDirect.size());
        println("Indexed/read_indexed calls (array bases, informational only): " + indexedCalls + ", distinct: " + seenArray.size());
        println("");
        println("=== GENUINELY MISSING (direct calls, addr/addr+4/addr-4 all absent from XML) ===");
        println(directMissing.toString());
        println("=== ARRAY BASES (for reference, not missing tables) ===");
        println(arrayBaseInfo.toString());
    }
}

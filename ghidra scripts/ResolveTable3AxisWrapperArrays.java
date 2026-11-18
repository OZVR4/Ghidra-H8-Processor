// Resolves array-base indirection for table_3axis_interp_triple_wrapper
// (0x1497b), same technique as review.md item 22 / review2.md item 13,
// retargeted. Confirmed via disassembly before writing this: identical
// array[global_index & 7] pattern (0x1f1fc index var, two shll.w = *4
// stride, same 4-byte {pad,bank,hi,lo} slot format) as table_lookup_indexed
// and table_read_indexed.
// @category H8539F

import java.util.ArrayList;
import java.util.List;

import ghidra.app.script.GhidraScript;
import ghidra.program.model.address.Address;
import ghidra.program.model.listing.Function;
import ghidra.program.model.listing.Instruction;
import ghidra.program.model.listing.Listing;
import ghidra.program.model.mem.Memory;
import ghidra.program.model.scalar.Scalar;
import ghidra.program.model.symbol.Reference;
import ghidra.program.model.symbol.ReferenceIterator;
import ghidra.program.model.symbol.ReferenceManager;
import ghidra.program.model.symbol.RefType;
import ghidra.program.model.symbol.SourceType;

public class ResolveTable3AxisWrapperArrays extends GhidraScript {

	@Override
	public void run() throws Exception {
		Address targetFunc = toAddr(0x1497b);
		Function f = getFunctionAt(targetFunc);
		if (f == null) {
			println("ERROR: function not found at 0x1497b");
			return;
		}

		ReferenceManager refMan = currentProgram.getReferenceManager();
		Listing listing = currentProgram.getListing();

		ReferenceIterator refsTo = refMan.getReferencesTo(targetFunc);
		List<Address> callSites = new ArrayList<>();
		while (refsTo.hasNext()) {
			Reference r = refsTo.next();
			if (r.getReferenceType().isCall()) {
				callSites.add(r.getFromAddress());
			}
		}
		println("Found " + callSites.size() + " call sites to table_3axis_interp_triple_wrapper");

		for (Address callSite : callSites) {
			Instruction callInsn = listing.getInstructionAt(callSite);
			if (callInsn == null) {
				println("  " + callSite + ": no instruction, skip");
				continue;
			}

			Instruction cur = callInsn.getPrevious();
			Integer bank = null;
			Integer offset = null;
			int steps = 0;
			while (cur != null && steps < 12) {
				String mnem = cur.getMnemonicString();
				String rep = cur.toString();
				if (mnem.startsWith("mov") && rep.contains("@-SP")) {
					Scalar sc = null;
					try {
						sc = cur.getScalar(0);
					}
					catch (Exception e) {
						// ignore, sc stays null
					}
					if (sc != null) {
						long val = sc.getUnsignedValue();
						if (offset == null && rep.contains(":16")) {
							offset = (int) val;
						}
						else if (bank == null && rep.contains(":8")) {
							bank = (int) val;
						}
					}
				}
				if (bank != null && offset != null) {
					break;
				}
				cur = cur.getPrevious();
				steps++;
			}

			if (bank == null || offset == null) {
				println("  " + callSite + ": push pair NOT found (bank=" + bank + " offset=" + offset + ")");
				continue;
			}

			long realAddrLong = (bank.longValue() * 0x10000L) + (offset.longValue() & 0xFFFFL);
			Address arrayBase = toAddr(realAddrLong);
			Function callerFunc = getFunctionContaining(callSite);
			String callerName = callerFunc != null ? callerFunc.getName() : "UNKNOWN";

			println("  " + callSite + " (" + callerName + ") -> array base " + arrayBase);

			refMan.addMemoryReference(callSite, arrayBase, RefType.DATA, SourceType.USER_DEFINED, 0);

			Memory mem = currentProgram.getMemory();
			for (int i = 0; i < 8; i++) {
				Address slotAddr = arrayBase.add(i * 4L);
				try {
					byte slotBank = mem.getByte(slotAddr.add(1));
					byte hi = mem.getByte(slotAddr.add(2));
					byte lo = mem.getByte(slotAddr.add(3));
					int slotOffset = ((hi & 0xFF) << 8) | (lo & 0xFF);
					long targetLong = ((slotBank & 0xFFL) * 0x10000L) + (slotOffset & 0xFFFFL);
					Address targetAddr = toAddr(targetLong);
					println("      slot " + i + ": bank=" + (slotBank & 0xFF) + " offset=" +
						Integer.toHexString(slotOffset) + " -> " + targetAddr);
					refMan.addMemoryReference(slotAddr, targetAddr, RefType.DATA, SourceType.USER_DEFINED, 0);
				}
				catch (Exception e) {
					println("      slot " + i + ": read error " + e.getMessage());
				}
			}
		}
		println("DONE");
	}
}

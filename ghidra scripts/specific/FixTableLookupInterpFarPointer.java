import ghidra.app.script.GhidraScript;
import ghidra.program.model.address.Address;
import ghidra.program.model.listing.*;
import ghidra.program.model.data.*;
import ghidra.program.model.symbol.SourceType;
import java.util.*;

// Additive companion to FixTableLookupInterpStorage.java -- does NOT edit or replace it.
//
// WHY THIS SCRIPT EXISTS (2026-08-18, farpointerjoin Piece D follow-up):
// FixTableLookupInterpStorage.java forced `table` to a 2-byte void* via
// CUSTOM_STORAGE (Stack[0x6]:2). That was correct at the time -- pre-farpointerjoin,
// the two args (bank byte, offset word) were never actually fused, so a 2-byte
// pointer was the honest representation.
//
// Now that the farpointerjoin C++ patch fuses ep_val(bank)+table(offset) into a real
// 3-byte-into-4-byte pointer value at the decompiler level, the OLD 2-byte pointer
// type is stale for this function. Confirmed live via trial.xml save-state: the
// program's <typegrp> only has ONE anonymous void* type, size=2 -- reused by
// whatever else in this program declares a plain pointer. If we widen THAT type in
// place, every other legitimate 2-byte pointer in the program silently becomes 4
// bytes too. So this script creates a SEPARATE, explicitly-named 4-byte pointer
// type instead, and only ever touches the three known far-pointer-argument
// functions' storage -- the old type and every other function stay untouched.
//
// Also: do NOT call set_parameter_type / setLocalVariableType on the existing
// 2-byte `table` param to "resize" it in place. That path goes through
// VariableUtilities.resizeStackVarnode, which throws ArithmeticException: / by zero
// for this exact shape (confirmed live, 2026-08-18, GhidraMCPPlugin log). The
// correct move is what this script does: fully replace the parameter via
// updateFunction(..., CUSTOM_STORAGE, ...), the same successful mechanism the
// original script already used -- never ask Ghidra to resize the existing varnode,
// declare a new one outright.
public class FixTableLookupInterpFarPointer extends GhidraScript {
    @Override
    public void run() throws Exception {
        DataTypeManager dtm = currentProgram.getDataTypeManager();

        DataType voidType = dtm.getDataType("/void");
        if (voidType == null) voidType = new VoidDataType();

        DataType ushortType = dtm.getDataType("/ushort");
        if (ushortType == null) ushortType = new UnsignedShortDataType();

        DataType retType = dtm.getDataType("/ushort");
        if (retType == null) retType = new UnsignedShortDataType();

        // Explicitly-named 4-byte pointer-to-void, kept separate from the
        // program's existing (2-byte) anonymous void* type. Resolved into the
        // program's data type manager so it persists as a real, named type --
        // not an anonymous transient one that could get merged/deduped against
        // the existing 2-byte pointer by category path collision.
        Pointer32DataType farPtr32 = new Pointer32DataType(voidType);
        farPtr32.setName("FarPointerVoid4");
        DataType farPtrType = dtm.resolve(farPtr32, DataTypeConflictHandler.DEFAULT_HANDLER);
        println("Resolved far-pointer type: " + farPtrType.getName() + " (" + farPtrType.getLength() + " bytes), id=" + dtm.getID(farPtrType));

        // Same three functions FixTableLookupInterpStorage.java covers.
        // ep_val stays ushort @ Stack[0x4]:2, unchanged -- only `table` widens.
        String[] addrs = { "00014656", "00014854", "000148a2" };
        String[] names = { "table_lookup_interp", "table_lookup_indexed", "table_3axis_interp_triple" };

        for (int i = 0; i < addrs.length; i++) {
            Address addr = currentProgram.getAddressFactory().getAddress(addrs[i]);
            Function f = getFunctionAt(addr);
            if (f == null) { println(names[i] + " @ " + addrs[i] + ": NOT FOUND"); continue; }

            println("=== " + names[i] + " @ " + addrs[i] + " ===");
            println("BEFORE: " + f.getSignature(true));
            for (Parameter p : f.getParameters()) {
                println("  param " + p.getName() + " storage=" + p.getVariableStorage()
                    + " type=" + p.getDataType().getName() + " (" + p.getDataType().getLength() + " bytes)");
            }

            VariableStorage epStorage = new VariableStorage(currentProgram, 0x4, 2);
            VariableStorage tableStorage = new VariableStorage(currentProgram, 0x6, 4); // widened 2 -> 4

            Parameter epParam = new ParameterImpl("ep_val", ushortType, epStorage, currentProgram, SourceType.USER_DEFINED);
            Parameter tableParam = new ParameterImpl("table", farPtrType, tableStorage, currentProgram, SourceType.USER_DEFINED);

            List<Variable> params = new ArrayList<>();
            params.add(epParam);
            params.add(tableParam);

            Variable returnVar = new ReturnParameterImpl(retType, currentProgram);

            f.updateFunction("__stdcall_far_2arg", returnVar, params,
                Function.FunctionUpdateType.CUSTOM_STORAGE, true, SourceType.USER_DEFINED);

            println("AFTER: " + f.getSignature(true));
            for (Parameter p : f.getParameters()) {
                println("  param " + p.getName() + " storage=" + p.getVariableStorage()
                    + " type=" + p.getDataType().getName() + " (" + p.getDataType().getLength() + " bytes)");
            }
            println("");
        }

        println("Done. Old 2-byte void* type untouched; FarPointerVoid4 (4 bytes) applied to `table` on all three functions.");
        println("Next: force a fresh decompile on each function and verify the pcode model sees a real `table` varnode");
        println("(not auStack_6-style re-inference) before attempting to bind a struct type on top of this.");
    }
}

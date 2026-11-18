import ghidra.app.script.GhidraScript;
import ghidra.program.model.address.Address;
import ghidra.program.model.listing.*;
import ghidra.program.model.data.*;
import ghidra.program.model.symbol.SourceType;
import java.util.*;

public class FixTableLookupInterpStorage extends GhidraScript {
    @Override
    public void run() throws Exception {
        DataTypeManager dtm = currentProgram.getDataTypeManager();
        DataType ushortType = dtm.getDataType("/ushort");
        if (ushortType == null) ushortType = new UnsignedShortDataType();
        DataType ptrType = new PointerDataType(dtm.getDataType("/void"), 2, dtm);
        DataType retType = dtm.getDataType("/ushort");
        if (retType == null) retType = new UnsignedShortDataType();

        // Confirmed-correct offsets from table_lookup_interp (item 24):
        // ep_val (bank) = Stack[0x4]:2, table (offset/pointer) = Stack[0x6]:2
        String[] addrs = { "00014656", "00014854", "000148a2" };
        String[] names = { "table_lookup_interp", "table_lookup_indexed", "table_3axis_interp_triple" };

        for (int i = 0; i < addrs.length; i++) {
            Address addr = currentProgram.getAddressFactory().getAddress(addrs[i]);
            Function f = getFunctionAt(addr);
            if (f == null) { println(names[i] + " @ " + addrs[i] + ": NOT FOUND"); continue; }

            println("=== " + names[i] + " @ " + addrs[i] + " ===");
            println("BEFORE: " + f.getSignature(true));
            for (Parameter p : f.getParameters()) {
                println("  param " + p.getName() + " storage=" + p.getVariableStorage());
            }

            VariableStorage epStorage = new VariableStorage(currentProgram, 0x4, 2);
            VariableStorage tableStorage = new VariableStorage(currentProgram, 0x6, 2);

            Parameter epParam = new ParameterImpl("ep_val", ushortType, epStorage, currentProgram, SourceType.USER_DEFINED);
            Parameter tableParam = new ParameterImpl("table", ptrType, tableStorage, currentProgram, SourceType.USER_DEFINED);

            List<Variable> params = new ArrayList<>();
            params.add(epParam);
            params.add(tableParam);

            Variable returnVar = new ReturnParameterImpl(retType, currentProgram);

            f.updateFunction("__stdcall_far_2arg", returnVar, params,
                Function.FunctionUpdateType.CUSTOM_STORAGE, true, SourceType.USER_DEFINED);

            println("AFTER: " + f.getSignature(true));
            for (Parameter p : f.getParameters()) {
                println("  param " + p.getName() + " storage=" + p.getVariableStorage());
            }
            println("");
        }
    }
}

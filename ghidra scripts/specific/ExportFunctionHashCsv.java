// Pass 1: dump address, name, prototype (return type / params / calling convention),
// and normalized function hash for every named (non-FUN_*) function in the CURRENT
// program to CSV. Intended to run on RVR to produce a fresh, guaranteed-accurate
// companion to rvr_function_signatures.csv that also carries the opcode hash needed
// for safe cross-binary matching (see review.md's ign_advance_base_calc_f860 case:
// same address in both ROMs, but EVO5's body is a different, longer function --
// hash comparison is required to catch this, address alone is not enough).
// @category H8539F

import ghidra.app.script.GhidraScript;
import ghidra.program.model.listing.Function;
import ghidra.program.model.listing.FunctionIterator;
import ghidra.program.model.listing.Parameter;
import ghidra.program.model.util.CodeUnitInsertionException;
import ghidra.program.model.pcode.PcodeOp;
import ghidra.program.model.listing.Instruction;
import ghidra.program.model.listing.InstructionIterator;
import ghidra.program.model.address.Address;

import java.io.File;
import java.io.FileWriter;
import java.io.PrintWriter;
import java.security.MessageDigest;

public class ExportFunctionHashCsv extends GhidraScript {
    @Override
    public void run() throws Exception {
        String outPath = "C:\\Users\\j.brophy.CORKILLSYSTEMS\\github\\Ghidra-H8-Processor\\rvr_function_hashes.csv";
        PrintWriter pw = new PrintWriter(new FileWriter(new File(outPath), false));
        pw.println("address,name,return_type,params,calling_convention,hash,instruction_count");

        FunctionIterator it = currentProgram.getFunctionManager().getFunctions(true);
        int count = 0;
        MessageDigest md = MessageDigest.getInstance("SHA-256");

        while (it.hasNext()) {
            Function f = it.next();
            if (f.isThunk()) continue;
            String name = f.getName();
            if (name.startsWith("FUN_")) continue;

            // Build normalized opcode string (mnemonics only, operands stripped of
            // literal addresses/constants) same spirit as get_function_hash tool.
            StringBuilder sb = new StringBuilder();
            InstructionIterator insns = currentProgram.getListing().getInstructions(f.getBody(), true);
            int insnCount = 0;
            while (insns.hasNext()) {
                Instruction insn = insns.next();
                sb.append(insn.getMnemonicString()).append(";");
                insnCount++;
            }
            md.reset();
            byte[] digest = md.digest(sb.toString().getBytes("UTF-8"));
            StringBuilder hex = new StringBuilder();
            for (byte b : digest) hex.append(String.format("%02x", b));

            String retType = f.getReturnType() != null ? f.getReturnType().getName() : "";
            StringBuilder params = new StringBuilder();
            Parameter[] parms = f.getParameters();
            for (int i = 0; i < parms.length; i++) {
                if (i > 0) params.append(" ");
                params.append(parms[i].getDataType().getName());
            }
            String conv = f.getCallingConventionName() != null ? f.getCallingConventionName() : "";

            pw.println(String.format("%08x,%s,%s,\"%s\",%s,%s,%d",
                f.getEntryPoint().getOffset(), name, retType, params.toString(), conv, hex.toString(), insnCount));
            count++;
        }
        pw.close();
        println("Wrote " + count + " named functions with hashes to:");
        println(outPath);
    }
}

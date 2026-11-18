// Companion export for FindSimilarCandidates.java: dumps, for every NAMED
// (non-FUN_*) function in the CURRENT program, its address, name, and full
// semicolon-separated OPERAND-AWARE token sequence (mnemonic:operand-text per
// instruction, not mnemonic alone), to CSV. Intended to run on RVR.
//
// Operand-aware (not mnemonic-only) so that families of near-identical short
// functions differing only in an embedded immediate/register (e.g.
// o2_mode11_lambda_select vs o2_mode16_lambda_select, which differ only in
// "IMM:11" vs "IMM:16") are NOT collapsed into false matches the way a
// mnemonic-only LCS does.
// @category H8539F

import ghidra.app.script.GhidraScript;
import ghidra.program.model.listing.Function;
import ghidra.program.model.listing.FunctionIterator;
import ghidra.program.model.listing.Instruction;
import ghidra.program.model.listing.InstructionIterator;

import java.io.File;
import java.io.FileWriter;
import java.io.PrintWriter;

public class ExportMnemonicSequences extends GhidraScript {
    @Override
    public void run() throws Exception {
        String outPath = "C:\\Users\\j.brophy.CORKILLSYSTEMS\\github\\Ghidra-H8-Processor\\rvr_mnemonics.csv";
        PrintWriter pw = new PrintWriter(new FileWriter(new File(outPath), false));

        FunctionIterator it = currentProgram.getFunctionManager().getFunctions(true);
        int count = 0;
        while (it.hasNext()) {
            Function f = it.next();
            if (f.isThunk()) continue;
            String name = f.getName();
            if (name.startsWith("FUN_")) continue;

            StringBuilder sb = new StringBuilder();
            InstructionIterator insns = currentProgram.getListing().getInstructions(f.getBody(), true);
            while (insns.hasNext()) {
                Instruction insn = insns.next();
                // Token = mnemonic + operand text (operand text strips absolute
                // addresses/labels naturally via toString()'s scalar/register form,
                // but keeps literal immediate VALUES, which is exactly what we need
                // to distinguish e.g. IMM:11 from IMM:16).
                String token = insn.getMnemonicString() + ":" + insn.toString().replace(insn.getMnemonicString(), "").trim();
                sb.append(token.replace(";", "|")).append(";");
            }
            pw.println(String.format("%08x,%s,%s", f.getEntryPoint().getOffset(), name, sb.toString()));
            count++;
        }
        pw.close();
        println("Wrote " + count + " operand-aware token sequences to: " + outPath);
    }
}

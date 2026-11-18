// Read-only audit of the stack purge sizes of an H8/539F program. Run it from Script Manager after the
// auto-analysis of every new ROM, before trusting the decompiler output.
//
// Why: the decompiler computes a callee's extrapop as purge + stackshift. In this module the purge is the
// whole pop of the return instruction (rts 2, prts 4, rte 6) and every prototype must have stackshift 0.
// A wrong purge shifts the stack pointer the decompiler tracks in every caller (invented stack arguments,
// uStackNNNN = return address stores). See review32.md.
//
// It prints:
//   1. every prototype whose stackshift is not 0,
//   2. every function whose stored purge differs from its own return instruction, with the evidence
//      (calls inside the function, Ghidra's stack depth at each return, what the callers do right after
//      the call) and whether the H8 Stack Purge Enforcer analyzer would correct it by itself,
//   3. functions with returns of different kinds,
//   4. counts and a one-line RESULT.
// Functions that return with prtd/rtd (callee cleanup, owned by H8FunctionPurgeAnalyzer) and functions
// without a return instruction are counted but not judged. Nothing in the program is changed.
//
// Note: Ghidra's depth tracking does not model what a callee pops, so depth is only trustworthy for
// functions without calls; for the others the callers' cleanup (adds.b N,SP after the call) is the
// evidence to read: the caller removes the arguments, the callee pops only the return address.
//@category H8
import ghidra.app.cmd.function.CallDepthChangeInfo;
import ghidra.app.script.GhidraScript;
import ghidra.program.model.address.Address;
import ghidra.program.model.lang.PrototypeModel;
import ghidra.program.model.listing.Function;
import ghidra.program.model.listing.Instruction;
import ghidra.program.model.symbol.Reference;
import java.util.ArrayList;
import java.util.List;
import java.util.Map;
import java.util.TreeMap;
import java.util.TreeSet;

public class CheckStackPurge extends GhidraScript {

  private static String purgeText(Function f) {
    return f.isStackPurgeSizeValid() ? Integer.toString(f.getStackPurgeSize()) : "unset";
  }

  @Override
  public void run() throws Exception {
    println("Program: " + currentProgram.getDomainFile().getPathname()
      + " (" + currentProgram.getLanguageID() + ")");

    int badShift = 0;
    for (PrototypeModel pm : currentProgram.getCompilerSpec().getCallingConventions()) {
      if (pm.getStackshift() != 0) {
        badShift++;
        println("PROBLEM prototype " + pm.getName() + " has stackshift " + pm.getStackshift()
          + " (must be 0 in h8539f.cspec; it counts the return address twice)");
      }
    }

    int ok = 0, bad = 0, mixed = 0, noReturn = 0, immediate = 0, wouldCorrect = 0, needReview = 0;

    for (Function f : currentProgram.getFunctionManager().getFunctions(true)) {
      monitor.checkCancelled();
      if (f.isThunk() || f.isExternal()) {
        continue;
      }
      TreeSet<Integer> pops = new TreeSet<Integer>();
      List<Address> returns = new ArrayList<Address>();
      boolean hasImmediateReturn = false;
      int calls = 0;
      for (Instruction ins : currentProgram.getListing().getInstructions(f.getBody(), true)) {
        String m = ins.getMnemonicString().toLowerCase();
        if (m.equals("rts")) {
          pops.add(2);
          returns.add(ins.getAddress());
        }
        else if (m.equals("prts")) {
          pops.add(4);
          returns.add(ins.getAddress());
        }
        else if (m.equals("rte")) {
          pops.add(6);
          returns.add(ins.getAddress());
        }
        else if (m.equals("prtd") || m.equals("rtd")) {
          hasImmediateReturn = true;
        }
        if (ins.getFlowType().isCall()) {
          calls++;
        }
      }
      if (hasImmediateReturn) {
        immediate++;
        continue;
      }
      if (returns.isEmpty()) {
        noReturn++;
        continue;
      }
      if (pops.size() > 1) {
        mixed++;
        println("MIXED " + f.getName() + " @" + f.getEntryPoint() + " returns pop " + pops
          + ", stored purge " + purgeText(f) + " -- review by hand");
        continue;
      }
      int expected = pops.first();
      if (f.isStackPurgeSizeValid() && f.getStackPurgeSize() == expected) {
        ok++;
        continue;
      }

      bad++;
      StringBuilder depths = new StringBuilder();
      boolean balanced = true;
      try {
        CallDepthChangeInfo info = new CallDepthChangeInfo(f, monitor);
        for (Address r : returns) {
          int d = info.getDepth(r);
          if (d == Function.UNKNOWN_STACK_DEPTH_CHANGE || d == Function.INVALID_STACK_DEPTH_CHANGE) {
            depths.append("unknown ");
            balanced = false;
          }
          else {
            depths.append(d).append(' ');
            if (d != 0) {
              balanced = false;
            }
          }
        }
      }
      catch (Exception e) {
        depths.append("error ");
        balanced = false;
      }

      TreeMap<String, Integer> after = new TreeMap<String, Integer>();
      int callSites = 0;
      for (Reference ref : currentProgram.getReferenceManager().getReferencesTo(f.getEntryPoint())) {
        if (!ref.getReferenceType().isCall()) {
          continue;
        }
        callSites++;
        Instruction ci = currentProgram.getListing().getInstructionAt(ref.getFromAddress());
        Instruction next = (ci == null) ? null : ci.getNext();
        String key = (next == null) ? "?" : next.toString();
        Integer n = after.get(key);
        after.put(key, n == null ? 1 : n + 1);
      }
      StringBuilder callers = new StringBuilder();
      int shown = 0;
      for (Map.Entry<String, Integer> e : after.entrySet()) {
        if (shown++ >= 3) {
          callers.append(" ...");
          break;
        }
        callers.append(" [").append(e.getValue()).append("x ").append(e.getKey()).append("]");
      }

      boolean enforcerCorrects = (calls == 0 && balanced);
      if (enforcerCorrects) {
        wouldCorrect++;
      }
      else {
        needReview++;
      }
      println("MISMATCH " + f.getName() + " @" + f.getEntryPoint() + " conv=" + f.getCallingConventionName()
        + " stored purge " + purgeText(f) + ", return pops " + expected
        + " | calls inside: " + calls + ", depth at returns: " + depths.toString().trim()
        + " | call sites: " + callSites + ", right after the call:" + callers);
      println("         -> " + (enforcerCorrects
        ? "the H8 Stack Purge Enforcer analyzer corrects this to " + expected + " by itself"
        : "NOT corrected automatically (function has calls or the depth is not provably balanced): "
          + "set the purge by hand if the callers' cleanup above confirms that it pops only the return address"));
    }

    println("--- functions: ok=" + ok + ", mismatched purge=" + bad + " (enforcer corrects " + wouldCorrect
      + ", needs review " + needReview + "), mixed returns=" + mixed + ", prtd/rtd (not judged)=" + immediate
      + ", no return (not judged)=" + noReturn + ", prototypes with stackshift != 0: " + badShift);
    println(bad == 0 && mixed == 0 && badShift == 0
      ? "RESULT: OK - every judged function's purge equals its return pop and every stackshift is 0"
      : "RESULT: PROBLEMS FOUND - see the PROBLEM / MISMATCH / MIXED lines above");
  }
}

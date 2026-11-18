package h8539f;

import ghidra.app.services.AbstractAnalyzer;
import ghidra.app.services.AnalysisPriority;
import ghidra.app.services.AnalyzerType;
import ghidra.app.util.importer.MessageLog;
import ghidra.framework.options.Options;
import ghidra.program.model.address.Address;
import ghidra.program.model.address.AddressSetView;
import ghidra.program.model.lang.Register;
import ghidra.program.model.listing.Bookmark;
import ghidra.program.model.listing.BookmarkManager;
import ghidra.program.model.listing.BookmarkType;
import ghidra.program.model.listing.Function;
import ghidra.program.model.listing.FunctionIterator;
import ghidra.program.model.listing.FunctionManager;
import ghidra.program.model.listing.Instruction;
import ghidra.program.model.listing.Listing;
import ghidra.program.model.listing.Program;
import ghidra.program.model.pcode.PcodeOp;
import ghidra.program.model.pcode.Varnode;
import ghidra.program.model.symbol.FlowType;
import ghidra.util.exception.CancelledException;
import ghidra.util.exception.InvalidInputException;
import ghidra.util.task.TaskMonitor;

import java.util.ArrayList;
import java.util.Arrays;
import java.util.HashSet;
import java.util.LinkedHashMap;
import java.util.LinkedHashSet;
import java.util.List;
import java.util.Map;
import java.util.Set;
import java.util.TreeMap;
import java.util.TreeSet;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

/**
 * Preserved-register analyzer for the Renesas H8/539F processor module.
 *
 * The compiler spec's calling conventions list R1 and R2 as killed by a call. Many ROM helpers (the
 * div_*, muldiv_*, mul_*, adc_* helpers and others) save those registers at entry and restore them
 * before every return, so after such a call the caller's R1/R2 are still valid. Treating them as
 * killed makes the decompiler print extraout_R2 / junk base addresses after the call.
 *
 * For each function this analyzer finds the registers that are
 *   (a) saved by mov:g.w Rn,@-SP or stm (...),@-SP among the first instructions, and
 *   (b) restored by mov:g.w @SP+,Rn or ldm.w @SP+,(...) right before EVERY return,
 * and, when R1 and/or R2 qualify, switches the function to the matching clone of its CURRENT convention,
 * named &lt;base&gt;_pR1, &lt;base&gt;_pR2 or &lt;base&gt;_pR1R2 (defined in h8539f.cspec; identical to the base
 * convention except that those registers move from killedbycall to unaffected).
 *
 * Deliberately conservative, because existing conventions in the database are partly inferred:
 *  - it never changes anything about a convention except those registers;
 *  - it skips any function whose base convention is not one of the known bases;
 *  - it skips (and logs) if the wanted clone does not exist in the compiler spec;
 *  - every change is recorded as a bookmark (category "H8 Preserved Registers") with the evidence, and is
 *    undone automatically on re-analysis if the evidence no longer holds;
 *  - "Dry run" option logs what it would change without changing anything.
 *
 * Call-graph mode (option, off by default): additionally treats R1/R2 as preserved when the function never
 * writes them. For every function it collects, from p-code, which of R1/R2 the function's own instructions
 * write; adds the writes of everything it calls or tail-jumps to (whole-program fixpoint, so recursion and
 * mutual recursion are handled); and removes the registers the function saves and restores. A function that
 * ends up writing neither R1 nor R2 gets the matching clone. Anything that cannot be resolved (register- or
 * memory-indirect calls, computed jumps without targets, calls to addresses that are not a function entry,
 * functions without instructions) counts as writing both, so unresolved code only ever withholds a conversion.
 * Absolute-address calls such as "pjsr @0x1444c:24" are resolved even though the language models them as
 * computed calls. The
 * result never loses a conversion that save/restore evidence alone would give.
 *
 * No-stack mode (option, off by default): functions of the __stdcall family that provably read no stack
 * parameter (H8StackReadTracker: p-code, relative to the stack pointer at entry, conservative) get the matching
 * __stdcall_ns[_pR1|_pR2|_pR1R2] convention, which has no stack input entry. Without it the decompiler assumes up
 * to 512 bytes of stack arguments at offset 4 for every call and, when the caller pushed none, reads a stale
 * value from that slot (often an earlier call's pushed return address) and prints it as a call argument.
 * Disabled by default.
 */
public class H8PreservedRegsAnalyzer extends AbstractAnalyzer {

    private static final String ANALYZER_NAME = "H8 Preserved Registers Analyzer";
    private static final String DESCRIPTION =
        "Moves R1/R2 out of the killed-by-call set for functions that save and restore them (stm/ldm or push/pop).";
    private static final String OPTION_DRY_RUN = "Dry run (log only, change nothing)";
    private static final String OPTION_CALL_GRAPH =
        "Call-graph mode (also preserve R1/R2 in functions that never write them)";
    private static final String OPTION_NO_STACK =
        "No-stack mode (use _ns conventions for functions that read no stack parameter)";
    /** Base name of the no-stack variants of __stdcall (defined in h8539f.cspec). */
    private static final String NS_BASE = "__stdcall_ns";
    private static final String BOOKMARK_CATEGORY = "H8 Preserved Registers";

    private static final Set<String> BASES = new HashSet<String>(Arrays.asList(
        "__stdcall", "__stdcall_far_allstack", "__stdcall_far", "__stdcall_far_allstack_32ret", "__stdcall_far_2arg"));
    private static final String[] SUFFIXES = { "_pR1R2", "_pR1", "_pR2" };

    private static final Pattern PUSH = Pattern.compile("^mov:g\\.w\\s+(R[0-5]),@-SP$");
    private static final Pattern POP = Pattern.compile("^mov:g\\.w\\s+@SP\\+,(R[0-5])$");
    private static final Pattern STM = Pattern.compile("^stm\\s+\\((.*)\\),@-SP$");
    private static final Pattern LDM = Pattern.compile("^ldm\\.w\\s+@SP\\+,\\((.*)\\)$");
    private static final Pattern REG = Pattern.compile("R[0-5]|FP");
    /**
     * A call or jump whose only operand is a bare absolute address, e.g. "pjsr @0x1444c:24". The language builds
     * these as computed calls (CALLIND), so Ghidra's flow type says "computed", but the target is encoded in the
     * instruction and Ghidra's reference to it is exact. Register- and memory-indirect forms do not match.
     */
    private static final Pattern ABS_TARGET = Pattern.compile("^\\S+\\s+@0x[0-9a-fA-F]+(:\\d+)?$");

    private boolean dryRun = false;
    private boolean callGraph = false;
    private boolean noStack = false;

    public H8PreservedRegsAnalyzer() {
        super(ANALYZER_NAME, DESCRIPTION, AnalyzerType.FUNCTION_ANALYZER);
        // After the stack-purge analyzers (both run at FUNCTION_ANALYSIS.after()).
        setPriority(AnalysisPriority.FUNCTION_ANALYSIS.after().after());
        setDefaultEnablement(false);
    }

    @Override
    public boolean canAnalyze(Program program) {
        return program.getLanguage().getLanguageID().toString().toUpperCase().contains("H8539F");
    }

    @Override
    public void registerOptions(Options options, Program program) {
        options.registerOption(OPTION_DRY_RUN, Boolean.valueOf(dryRun), null,
            "Log which functions would be switched, and why, without changing any calling convention.");
        options.registerOption(OPTION_CALL_GRAPH, Boolean.valueOf(callGraph), null,
            "Also treat R1/R2 as preserved when the function, and everything it calls or tail-jumps to, never writes "
                + "them (p-code based, whole-program fixpoint). Calls or jumps that cannot be resolved count as writing both.");
        options.registerOption(OPTION_NO_STACK, Boolean.valueOf(noStack), null,
            "For functions that provably read no stack parameter, use the matching _ns convention (no stack input "
                + "entry), so the decompiler does not invent a stack argument at their call sites. p-code based; anything "
                + "that cannot be followed keeps the base convention. Applies to the __stdcall family only.");
    }

    @Override
    public void optionsChanged(Options options, Program program) {
        dryRun = options.getBoolean(OPTION_DRY_RUN, dryRun);
        callGraph = options.getBoolean(OPTION_CALL_GRAPH, callGraph);
        noStack = options.getBoolean(OPTION_NO_STACK, noStack);
    }

    private static final class Evidence {
        final Set<String> pushed = new TreeSet<String>();
        Set<String> preserved = null;
        int returns = 0;
        String pushText = null;
        String popText = null;
    }

    private static void collectRegs(Pattern p, String text, Set<String> out) {
        Matcher m = p.matcher(text);
        if (m.matches()) {
            Matcher r = REG.matcher(m.group(1));
            while (r.find()) {
                out.add(r.group());
            }
        }
    }

    private static boolean isReturn(String mnemonic) {
        return mnemonic.equals("prts") || mnemonic.equals("rts") || mnemonic.startsWith("prtd")
            || mnemonic.startsWith("rtd") || mnemonic.equals("rte");
    }

    private Evidence analyze(Listing listing, Function f) {
        List<Instruction> body = new ArrayList<Instruction>();
        for (Instruction i : listing.getInstructions(f.getBody(), true)) {
            body.add(i);
        }
        Evidence ev = new Evidence();
        if (body.isEmpty()) {
            return ev;
        }
        for (int k = 0; k < Math.min(12, body.size()); k++) {
            Instruction in = body.get(k);
            String text = in.toString();
            int before = ev.pushed.size();
            collectRegs(STM, text, ev.pushed);
            Matcher m = PUSH.matcher(text);
            if (m.matches()) {
                ev.pushed.add(m.group(1));
            }
            else if (in.getFlowType().isJump() || in.getFlowType().isCall()) {
                break;
            }
            if (ev.pushed.size() > before && ev.pushText == null) {
                ev.pushText = in.getAddress() + " " + text;
            }
        }
        for (int k = 0; k < body.size(); k++) {
            if (!isReturn(body.get(k).getMnemonicString())) {
                continue;
            }
            ev.returns++;
            Set<String> popped = new TreeSet<String>();
            for (int b = k - 1; b >= 0 && b >= k - 12; b--) {
                Instruction p = body.get(b);
                if (p.getFlowType().isJump() || p.getFlowType().isCall() || p.getFlowType().isTerminal()) {
                    break;
                }
                String text = p.toString();
                int before = popped.size();
                collectRegs(LDM, text, popped);
                Matcher m = POP.matcher(text);
                if (m.matches()) {
                    popped.add(m.group(1));
                }
                if (popped.size() > before && ev.popText == null) {
                    ev.popText = p.getAddress() + " " + text;
                }
            }
            if (ev.preserved == null) {
                ev.preserved = new TreeSet<String>(popped);
            }
            else {
                ev.preserved.retainAll(popped);
            }
        }
        if (ev.preserved == null) {
            ev.preserved = new TreeSet<String>();
        }
        ev.preserved.retainAll(ev.pushed);
        return ev;
    }

    // ---- call-graph mode ----------------------------------------------------------------------------

    private static final int BIT_R1 = 1;
    private static final int BIT_R2 = 2;
    private static final int BIT_BOTH = BIT_R1 | BIT_R2;

    /** Index into Node.clobber: unresolved calls/jumps count as writing R1 and R2 (the result that is applied). */
    private static final int PESSIMISTIC = 0;
    /** Index into Node.clobber: unresolved calls/jumps ignored. Only an upper bound for the log, never applied. */
    private static final int OPTIMISTIC = 1;

    /** Per-function facts for call-graph mode. Register sets are bit masks of BIT_R1 / BIT_R2. */
    private static final class Node {
        final Evidence ev;
        /** R1/R2 written by this function's own p-code. */
        int direct = 0;
        /** R1/R2 saved at entry and restored before every return (the original evidence). */
        int saved = 0;
        /** Jumps that leave the function body (tail calls). */
        int tailCalls = 0;
        /** Fixpoint result per mode: R1/R2 that a call to this function may change. */
        final int[] clobber = new int[2];
        final Set<Address> callees = new LinkedHashSet<Address>();
        /** Why the function cannot be fully resolved, as "kind: detail". Empty when it can. */
        final List<String> unresolved = new ArrayList<String>();

        Node(Evidence ev) {
            this.ev = ev;
        }

        /** True if the function has at least one return or tail jump, so "preserves" means something. */
        boolean hasExit() {
            return ev.returns + tailCalls > 0;
        }

        int preserved(int mode) {
            return BIT_BOTH & ~clobber[mode];
        }
    }

    /** Byte range of a register in register space. */
    private static final class RegRange {
        final long start;
        final long end;

        RegRange(Register r) {
            start = r.getAddress().getOffset();
            end = start + r.getMinimumByteSize();
        }

        boolean hits(long s, long e) {
            return s < end && e > start;
        }
    }

    /** R1/R2 bits that the instruction's p-code writes (any overlap counts, so byte and pair writes are seen). */
    private static int writtenBits(Instruction in, RegRange r1, RegRange r2) {
        int bits = 0;
        for (PcodeOp op : in.getPcode()) {
            Varnode out = op.getOutput();
            if (out == null || !out.isRegister()) {
                continue;
            }
            long s = out.getOffset();
            long e = s + out.getSize();
            if (r1.hits(s, e)) {
                bits |= BIT_R1;
            }
            if (r2.hits(s, e)) {
                bits |= BIT_R2;
            }
        }
        return bits;
    }

    private static void addCallee(Node n, FunctionManager fm, Address target, Instruction from) {
        Function callee = fm.getFunctionAt(target);
        if (callee == null) {
            n.unresolved.add("no function at target: " + target + " (from " + from.getAddress() + ")");
        }
        else {
            n.callees.add(callee.getEntryPoint());
        }
    }

    /**
     * Builds the whole-program graph and solves it. Always covers every function (not just the ones in the
     * analyzed set), because a function's result depends on its callees wherever they are.
     * Returns null if the language has no R1/R2 registers.
     */
    private Map<Address, Node> buildGraph(Program program, TaskMonitor monitor) throws CancelledException {
        Register reg1 = program.getLanguage().getRegister("R1");
        Register reg2 = program.getLanguage().getRegister("R2");
        if (reg1 == null || reg2 == null) {
            return null;
        }
        RegRange r1 = new RegRange(reg1);
        RegRange r2 = new RegRange(reg2);
        Listing listing = program.getListing();
        FunctionManager fm = program.getFunctionManager();
        Map<Address, Node> nodes = new LinkedHashMap<Address, Node>();

        for (Function f : fm.getFunctions(true)) {
            monitor.checkCancelled();
            Node n = new Node(analyze(listing, f));
            // analyze() leaves preserved == null for a function without instructions
            for (String r : (n.ev.preserved == null) ? new TreeSet<String>() : n.ev.preserved) {
                if (r.equals("R1")) {
                    n.saved |= BIT_R1;
                }
                else if (r.equals("R2")) {
                    n.saved |= BIT_R2;
                }
            }
            AddressSetView body = f.getBody();
            boolean any = false;
            for (Instruction in : listing.getInstructions(body, true)) {
                any = true;
                n.direct |= writtenBits(in, r1, r2);
                FlowType ft = in.getFlowType();
                if (ft.isCall()) {
                    Address[] flows = in.getFlows();
                    boolean exact = !ft.isComputed() || ABS_TARGET.matcher(in.toString()).matches();
                    if (!exact || flows.length == 0) {
                        n.unresolved.add("indirect call: " + in.getAddress());
                    }
                    else {
                        for (Address a : flows) {
                            addCallee(n, fm, a, in);
                        }
                    }
                }
                else if (ft.isJump()) {
                    Address[] flows = in.getFlows();
                    if (flows.length == 0) {
                        n.unresolved.add("computed jump without targets: " + in.getAddress());
                    }
                    for (Address a : flows) {
                        if (!body.contains(a)) {
                            n.tailCalls++;
                            addCallee(n, fm, a, in);
                        }
                    }
                }
            }
            if (!any) {
                n.unresolved.add("no instructions: " + f.getEntryPoint());
            }
            nodes.put(f.getEntryPoint(), n);
        }

        // Least fixpoint: every set starts empty and only grows, so recursion that writes nothing stays clean.
        for (int mode = PESSIMISTIC; mode <= OPTIMISTIC; mode++) {
            boolean changed = true;
            while (changed) {
                monitor.checkCancelled();
                changed = false;
                for (Node n : nodes.values()) {
                    int c = n.direct;
                    if (mode == PESSIMISTIC && !n.unresolved.isEmpty()) {
                        c |= BIT_BOTH;
                    }
                    for (Address a : n.callees) {
                        Node m = nodes.get(a);
                        c |= (m == null) ? BIT_BOTH : m.clobber[mode];
                    }
                    c &= ~n.saved;
                    if (c != n.clobber[mode]) {
                        n.clobber[mode] = c;
                        changed = true;
                    }
                }
            }
        }
        return nodes;
    }

    private static String suffixOfBits(int bits) {
        if (bits == BIT_BOTH) {
            return "_pR1R2";
        }
        if (bits == BIT_R1) {
            return "_pR1";
        }
        if (bits == BIT_R2) {
            return "_pR2";
        }
        return "";
    }

    private static String describeGraph(Node n, int keep) {
        StringBuilder sb = new StringBuilder("call graph:");
        if ((keep & BIT_R1) != 0) {
            sb.append(" R1 ").append((n.saved & BIT_R1) != 0 ? "saved/restored" : "never written");
        }
        if ((keep & BIT_R2) != 0) {
            sb.append(" R2 ").append((n.saved & BIT_R2) != 0 ? "saved/restored" : "never written");
        }
        sb.append(" (incl. ").append(n.callees.size()).append(" callees, ").append(n.tailCalls).append(" tail jumps)");
        return sb.toString();
    }

    private static String summarizeGraph(Map<Address, Node> graph) {
        int[] r1 = new int[2];
        int[] r2 = new int[2];
        int blocked = 0;
        Map<String, Integer> kinds = new TreeMap<String, Integer>();
        for (Node n : graph.values()) {
            for (int mode = PESSIMISTIC; mode <= OPTIMISTIC; mode++) {
                if (n.hasExit()) {
                    if ((n.preserved(mode) & BIT_R1) != 0) {
                        r1[mode]++;
                    }
                    if ((n.preserved(mode) & BIT_R2) != 0) {
                        r2[mode]++;
                    }
                }
            }
            if (!n.unresolved.isEmpty()) {
                blocked++;
                Set<String> seen = new HashSet<String>();
                for (String why : n.unresolved) {
                    seen.add(why.substring(0, why.indexOf(':')));
                }
                for (String kind : seen) {
                    kinds.merge(kind, Integer.valueOf(1), Integer::sum);
                }
            }
        }
        return "Call graph: " + graph.size() + " functions; preserve R1 in " + r1[PESSIMISTIC] + ", R2 in "
            + r2[PESSIMISTIC] + "; " + blocked + " have unresolved calls/jumps " + kinds
            + "; upper bound if those were harmless: R1 in " + r1[OPTIMISTIC] + ", R2 in " + r2[OPTIMISTIC];
    }

    private static String suffixOf(Set<String> preserved) {
        boolean r1 = preserved.contains("R1");
        boolean r2 = preserved.contains("R2");
        if (r1 && r2) {
            return "_pR1R2";
        }
        if (r1) {
            return "_pR1";
        }
        if (r2) {
            return "_pR2";
        }
        return "";
    }

    private static String stripSuffix(String convention) {
        for (String s : SUFFIXES) {
            if (convention.endsWith(s)) {
                return convention.substring(0, convention.length() - s.length());
            }
        }
        return convention;
    }

    @Override
    public boolean added(Program program, AddressSetView set, TaskMonitor monitor, MessageLog log)
            throws CancelledException {

        Listing listing = program.getListing();
        FunctionManager functionManager = program.getFunctionManager();
        BookmarkManager bookmarks = program.getBookmarkManager();
        FunctionIterator functions = (set == null) ? functionManager.getFunctions(true)
                                                   : functionManager.getFunctions(set, true);
        int changed = 0, reverted = 0, missingClone = 0, wouldChange = 0, viaGraph = 0, nsSet = 0;
        boolean missingLogged = false;

        Map<Address, Node> graph = null;
        if (callGraph) {
            graph = buildGraph(program, monitor);
            if (graph == null) {
                log.appendMsg(ANALYZER_NAME, "Call-graph mode: R1/R2 not found in the language; "
                    + "using save/restore evidence only");
            }
            else {
                log.appendMsg(ANALYZER_NAME, summarizeGraph(graph));
                if (dryRun) {
                    int shown = 0;
                    for (Map.Entry<Address, Node> e : graph.entrySet()) {
                        for (String why : e.getValue().unresolved) {
                            if (shown++ < 40) {
                                log.appendMsg(ANALYZER_NAME, "[unresolved] function at " + e.getKey() + ": " + why);
                            }
                        }
                    }
                    if (shown > 40) {
                        log.appendMsg(ANALYZER_NAME, "[unresolved] ... and " + (shown - 40) + " more");
                    }
                }
            }
        }

        Map<Address, H8StackReadTracker.Result> stackMap = null;
        if (noStack) {
            stackMap = H8StackReadTracker.solve(program, monitor);
            if (stackMap == null) {
                log.appendMsg(ANALYZER_NAME, "No-stack mode: SP/FP not found in the language; skipping");
            }
            else {
                int noRead = 0;
                for (H8StackReadTracker.Result sr : stackMap.values()) {
                    if (!sr.keepsStack()) {
                        noRead++;
                    }
                }
                log.appendMsg(ANALYZER_NAME, "No-stack mode: " + noRead + " of " + stackMap.size()
                    + " functions read no stack parameter");
            }
        }

        while (functions.hasNext()) {
            monitor.checkCancelled();
            Function f = functions.next();

            String current = f.getCallingConventionName();
            if (current == null) {
                current = "unknown";
            }
            String base = stripSuffix(current);
            boolean currentNs = base.equals(NS_BASE);
            boolean currentIsClone = !base.equals(current) || currentNs;
            if (currentNs || base.equals("unknown") || base.equals("default")) {
                base = "__stdcall";
            }
            if (!BASES.contains(base)) {
                continue;
            }

            Node node = (graph == null) ? null : graph.get(f.getEntryPoint());
            Evidence ev = (node != null) ? node.ev : analyze(listing, f);
            String suffix = ev.returns == 0 ? "" : suffixOf(ev.preserved);
            String how = null;
            boolean graphOnly = false;
            if (node != null) {
                int keep = node.preserved(PESSIMISTIC);
                String viaSuffix = node.hasExit() ? suffixOfBits(keep) : "";
                graphOnly = !viaSuffix.isEmpty() && !viaSuffix.equals(suffix);
                suffix = viaSuffix;
                how = describeGraph(node, keep);
            }
            boolean wantNs = false;
            String nsHow = null;
            if (stackMap != null && base.equals("__stdcall")) {
                H8StackReadTracker.Result sr = stackMap.get(f.getEntryPoint());
                if (sr != null && !sr.keepsStack()) {
                    wantNs = true;
                    nsHow = sr.describe();
                }
            }
            boolean managed = !suffix.isEmpty() || wantNs;
            if (!managed && !currentIsClone) {
                continue; // never touch a function we have no evidence for
            }
            String wanted = base + (wantNs ? "_ns" : "") + suffix;
            if (wanted.equals(current)) {
                continue;
            }
            if (managed && program.getCompilerSpec().getCallingConvention(wanted) == null) {
                missingClone++;
                if (!missingLogged) {
                    log.appendMsg(ANALYZER_NAME, "Calling convention " + wanted
                        + " is not defined in the compiler spec (update h8539f.cspec); skipping such functions");
                    missingLogged = true;
                }
                continue;
            }

            String preservedPart = suffix.isEmpty() ? "" : (how != null
                    ? "R1/R2 preserved (" + how + ", returns=" + ev.returns + ")"
                    : "R1/R2 preserved " + ev.preserved + " (saved " + ev.pushText + ", restored " + ev.popText
                        + ", returns=" + ev.returns + ")");
            String nsPart = wantNs ? "no stack parameter read (" + nsHow + ")" : "";
            String what = !managed
                ? "evidence no longer holds; reverting " + current + " -> " + wanted
                : (preservedPart + (preservedPart.isEmpty() || nsPart.isEmpty() ? "" : "; ") + nsPart
                    + "; " + current + " -> " + wanted);
            if (dryRun) {
                log.appendMsg(ANALYZER_NAME, "[dry run] " + f.getName() + " at " + f.getEntryPoint() + ": " + what);
                wouldChange++;
                if (wantNs) {
                    nsSet++;
                }
                if (graphOnly) {
                    viaGraph++;
                }
                continue;
            }
            try {
                f.setCallingConvention(wanted);
            }
            catch (InvalidInputException e) {
                log.appendMsg(ANALYZER_NAME, "Could not set " + wanted + " on " + f.getName() + " at "
                    + f.getEntryPoint() + ": " + e.getMessage());
                continue;
            }
            Bookmark old = bookmarks.getBookmark(f.getEntryPoint(), BookmarkType.ANALYSIS, BOOKMARK_CATEGORY);
            if (!managed) {
                if (old != null) {
                    bookmarks.removeBookmark(old);
                }
                reverted++;
            }
            else {
                bookmarks.setBookmark(f.getEntryPoint(), BookmarkType.ANALYSIS, BOOKMARK_CATEGORY, what);
                changed++;
                if (wantNs) {
                    nsSet++;
                }
                if (graphOnly) {
                    viaGraph++;
                }
            }
        }
        if (changed > 0 || reverted > 0 || missingClone > 0 || dryRun) {
            log.appendMsg(ANALYZER_NAME, "Preserved-register conventions: " + changed + " set, " + reverted
                + " reverted, " + missingClone + " skipped (clone missing)"
                + (graph != null ? ", " + viaGraph + " of the changes come from the call graph alone" : "")
                + (stackMap != null ? ", " + nsSet + " use a no-stack convention (_ns)" : "")
                + (dryRun ? " [dry run: " + wouldChange + " would change]" : ""));
        }
        return true;
    }
}

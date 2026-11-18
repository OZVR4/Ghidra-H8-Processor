package h8539f;

import ghidra.program.model.address.Address;
import ghidra.program.model.address.AddressSetView;
import ghidra.program.model.lang.Language;
import ghidra.program.model.lang.Register;
import ghidra.program.model.listing.Function;
import ghidra.program.model.listing.FunctionManager;
import ghidra.program.model.listing.Instruction;
import ghidra.program.model.listing.Program;
import ghidra.program.model.pcode.PcodeOp;
import ghidra.program.model.pcode.Varnode;
import ghidra.program.model.symbol.FlowType;
import ghidra.util.exception.CancelledException;
import ghidra.util.task.TaskMonitor;

import java.util.ArrayDeque;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.Iterator;
import java.util.LinkedHashMap;
import java.util.LinkedHashSet;
import java.util.List;
import java.util.Map;
import java.util.Set;
import java.util.TreeSet;
import java.util.regex.Pattern;

/**
 * Decides, per function, whether it can read its caller's stack arguments. Used by H8PreservedRegsAnalyzer
 * ("No-stack mode") to give functions that provably read none a calling convention without the stack input
 * entry, so the decompiler does not invent a stack argument at their call sites (it picks up whatever stale
 * value, often an earlier call's pushed return address, sits in that slot).
 *
 * Works on p-code, not on instruction text and not on the decompiler, and uses no ROM-specific addresses or
 * names, so it applies to any program in the H8539F language. Every SP/FP-relative access in the language goes
 * through the same user-defined op (spSegment, user-op 0) before the LOAD or STORE; the pointer value is
 * followed symbolically, relative to the stack pointer at function entry:
 *   - ENTRY(k): the value entry-SP + k; CONST(c): a known constant; anything else: unknown.
 *   - SP is tracked through push/pop/stm/ldm/link/unlk/adds/add and call instructions (a call restores SP by
 *     the callee's stack purge size, so a callee that pops its own arguments is handled).
 *   - A LOAD or STORE through ENTRY(k) with k at or above the return-address size is a stack parameter
 *     access. Offsets below zero are the function's own locals.
 *
 * Deliberately conservative: anything it cannot follow makes the function "keep the stack input", so doubt only
 * withholds a conversion: stack pointer lost, a stack address stored to memory, passed to a user-op or left in
 * a register across a call, unresolved or indirect calls, computed jumps without targets, tail jumps with the
 * frame not unwound, a tail target that reads the stack or cannot be resolved, interrupt returns (rte),
 * returns that pop arguments (prtd/rtd), accesses to the return-address area, and a callee whose purge size is
 * unknown.
 */
public final class H8StackReadTracker {

    private static final int UNK = 0, CONST = 1, ENTRY = 2;
    private static final Pattern ABS_TARGET = Pattern.compile("^\\S+\\s+@0x[0-9a-fA-F]+(:\\d+)?$");
    private static final int MAX_VISITS = 40;
    private static final int MAX_PURGE = 0x100;

    /** Result for one function. */
    public static final class Result {
        /** The function accesses stack memory at or above the return address (a stack parameter). */
        public boolean reads = false;
        /** Something could not be followed; the function must keep the stack input entry. */
        public boolean unresolved = false;
        /** Return-address size used to classify accesses: 4 for prts, 2 for rts. */
        public int retSize = 2;
        public final TreeSet<Long> offsets = new TreeSet<Long>();
        public final List<String> why = new ArrayList<String>();
        final Set<Address> tails = new LinkedHashSet<Address>();

        void unres(String s) {
            unresolved = true;
            if (why.size() < 4) {
                why.add(s);
            }
        }

        /** True if the function must keep the base convention (stack input entry present). */
        public boolean keepsStack() {
            return reads || unresolved;
        }

        public String describe() {
            if (!keepsStack()) {
                return "no stack parameter access, return address " + retSize + " bytes";
            }
            return why.isEmpty() ? "reads the stack" : why.get(0);
        }
    }

    private static final class Val {
        final int k;
        final long v;
        final int sz;

        Val(int k, long v, int sz) {
            this.k = k;
            this.v = v;
            this.sz = sz;
        }

        boolean same(Val o) {
            return o != null && k == o.k && sz == o.sz && (k == UNK || v == o.v);
        }
    }

    private static final Val UNKV = new Val(UNK, 0, 0);

    private final Program program;
    private final long spKey;
    private final long fpKey;
    private final int spSize;

    private H8StackReadTracker(Program program, Register sp, Register fp) {
        this.program = program;
        this.spKey = sp.getAddress().getOffset();
        this.fpKey = fp.getAddress().getOffset();
        this.spSize = sp.getMinimumByteSize();
    }

    /**
     * Analyzes every function in the program (a function's answer depends on its tail-jump targets wherever they
     * are). Returns null if the language has no SP/FP registers.
     */
    public static Map<Address, Result> solve(Program program, TaskMonitor monitor) throws CancelledException {
        Language lang = program.getLanguage();
        Register sp = lang.getRegister("SP");
        Register fp = lang.getRegister("FP");
        if (sp == null || fp == null) {
            return null;
        }
        H8StackReadTracker t = new H8StackReadTracker(program, sp, fp);
        Map<Address, Result> res = new LinkedHashMap<Address, Result>();
        for (Function f : program.getFunctionManager().getFunctions(true)) {
            monitor.checkCancelled();
            res.put(f.getEntryPoint(), t.analyze(f));
        }
        boolean changed = true;
        while (changed) {
            monitor.checkCancelled();
            changed = false;
            for (Result r : res.values()) {
                for (Address target : r.tails) {
                    Result tr = res.get(target);
                    if (tr == null) {
                        if (!r.unresolved) {
                            r.unres("tail jump to a non-function");
                            changed = true;
                        }
                    }
                    else if (tr.keepsStack() && !r.unresolved) {
                        r.unres("tail target reads the stack or is unresolved");
                        changed = true;
                    }
                }
            }
        }
        return res;
    }

    private static boolean isReturn(String m) {
        return m.equals("prts") || m.equals("rts") || m.startsWith("prtd") || m.startsWith("rtd") || m.equals("rte");
    }

    private static long sext(long v, int size) {
        if (size >= 8) {
            return v;
        }
        int sh = 64 - 8 * size;
        return (v << sh) >> sh;
    }

    private static long mask(long v, int size) {
        if (size >= 8) {
            return v;
        }
        return v & ((1L << (8 * size)) - 1);
    }

    private Val get(Map<Long, Val> regs, Map<Long, Val> tmp, Varnode vn) {
        if (vn.isConstant()) {
            return new Val(CONST, mask(vn.getOffset(), vn.getSize()), vn.getSize());
        }
        if (vn.isRegister()) {
            Val x = regs.get(vn.getOffset());
            return x == null ? UNKV : x;
        }
        if (vn.isUnique()) {
            Val x = tmp.get(vn.getOffset());
            return x == null ? UNKV : x;
        }
        return UNKV;
    }

    private void setOut(Map<Long, Val> regs, Map<Long, Val> tmp, Varnode out, Val val) {
        if (out == null) {
            return;
        }
        if (out.isUnique()) {
            if (val.k == UNK) {
                tmp.remove(out.getOffset());
            }
            else {
                tmp.put(out.getOffset(), val);
            }
            return;
        }
        if (!out.isRegister()) {
            return;
        }
        long s = out.getOffset();
        long e = s + out.getSize();
        Iterator<Map.Entry<Long, Val>> it = regs.entrySet().iterator();
        while (it.hasNext()) {
            Map.Entry<Long, Val> en = it.next();
            long ks = en.getKey();
            long ke = ks + Math.max(1, en.getValue().sz);
            if (ks < e && s < ke) {
                it.remove();
            }
        }
        if (val.k != UNK) {
            regs.put(s, new Val(val.k, val.v, out.getSize()));
        }
    }

    private void access(long off, Result r, Instruction in, boolean isRet) {
        if (off < 0) {
            return;
        }
        if (off < r.retSize) {
            if (!isRet) {
                r.unres("access to return-address area at " + in.getAddress());
            }
            return;
        }
        r.reads = true;
        r.offsets.add(Long.valueOf(off));
        if (r.why.size() < 4) {
            r.why.add("stack parameter access +" + off + " at " + in.getAddress());
        }
    }

    private void exec(PcodeOp op, Map<Long, Val> regs, Map<Long, Val> tmp, Result r, Instruction in,
            boolean isRet) {
        int oc = op.getOpcode();
        Varnode out = op.getOutput();
        int n = op.getNumInputs();
        switch (oc) {
            case PcodeOp.COPY: {
                Val a = get(regs, tmp, op.getInput(0));
                setOut(regs, tmp, out, (a.k == UNK) ? UNKV : new Val(a.k, a.v, out.getSize()));
                return;
            }
            case PcodeOp.INT_ADD:
            case PcodeOp.INT_SUB: {
                Val a = get(regs, tmp, op.getInput(0));
                Val b = get(regs, tmp, op.getInput(1));
                boolean sub = (oc == PcodeOp.INT_SUB);
                Val res = UNKV;
                if (a.k == ENTRY && b.k == CONST) {
                    res = new Val(ENTRY, a.v + (sub ? -sext(b.v, b.sz) : sext(b.v, b.sz)), out.getSize());
                }
                else if (!sub && a.k == CONST && b.k == ENTRY) {
                    res = new Val(ENTRY, b.v + sext(a.v, a.sz), out.getSize());
                }
                else if (a.k == CONST && b.k == CONST) {
                    res = new Val(CONST, mask(sub ? a.v - b.v : a.v + b.v, out.getSize()), out.getSize());
                }
                else if ((a.k == ENTRY || b.k == ENTRY) && out.getSize() != 1) {
                    r.unres("stack pointer arithmetic not modelled at " + in.getAddress());
                }
                setOut(regs, tmp, out, res);
                return;
            }
            case PcodeOp.INT_SEXT: {
                Val a = get(regs, tmp, op.getInput(0));
                Val res = UNKV;
                if (a.k == CONST) {
                    res = new Val(CONST, mask(sext(a.v, a.sz), out.getSize()), out.getSize());
                }
                else if (a.k == ENTRY) {
                    res = new Val(ENTRY, a.v, out.getSize());
                }
                setOut(regs, tmp, out, res);
                return;
            }
            case PcodeOp.INT_ZEXT: {
                Val a = get(regs, tmp, op.getInput(0));
                Val res = UNKV;
                if (a.k == CONST) {
                    res = new Val(CONST, a.v, out.getSize());
                }
                else if (a.k == ENTRY) {
                    res = new Val(ENTRY, a.v, out.getSize());
                }
                setOut(regs, tmp, out, res);
                return;
            }
            case PcodeOp.LOAD: {
                Val p = get(regs, tmp, op.getInput(1));
                if (p.k == ENTRY) {
                    access(p.v, r, in, isRet);
                }
                setOut(regs, tmp, out, UNKV);
                return;
            }
            case PcodeOp.STORE: {
                Val p = get(regs, tmp, op.getInput(1));
                Val d = get(regs, tmp, op.getInput(2));
                if (p.k == ENTRY) {
                    access(p.v, r, in, isRet);
                }
                if (d.k == ENTRY) {
                    r.unres("stack address stored to memory at " + in.getAddress());
                }
                return;
            }
            case PcodeOp.CALLOTHER: {
                // spSegment(TP, offset): builds the memory address of a stack or data access; the offset value
                // is what matters here.
                if (n == 3 && op.getInput(0).isConstant() && op.getInput(0).getOffset() == 0 && out != null) {
                    Val a = get(regs, tmp, op.getInput(2));
                    setOut(regs, tmp, out, (a.k == UNK) ? UNKV : new Val(a.k, a.v, out.getSize()));
                    return;
                }
                for (int i = 1; i < n; i++) {
                    if (get(regs, tmp, op.getInput(i)).k == ENTRY) {
                        r.unres("stack address passed to a userop at " + in.getAddress());
                    }
                }
                setOut(regs, tmp, out, UNKV);
                return;
            }
            case PcodeOp.CALL:
            case PcodeOp.CALLIND:
            case PcodeOp.BRANCH:
            case PcodeOp.CBRANCH:
            case PcodeOp.BRANCHIND:
            case PcodeOp.RETURN:
                return;
            default: {
                if (out != null) {
                    boolean ent = false;
                    for (int i = 0; i < n; i++) {
                        if (get(regs, tmp, op.getInput(i)).k == ENTRY) {
                            ent = true;
                        }
                    }
                    if (ent && out.getSize() != 1) {
                        r.unres("stack address used by " + op.getMnemonic() + " at " + in.getAddress());
                    }
                    setOut(regs, tmp, out, UNKV);
                }
            }
        }
    }

    private Map<Long, Val> step(Instruction in, Map<Long, Val> inState, Result r) {
        Map<Long, Val> regs = new HashMap<Long, Val>(inState);
        Map<Long, Val> tmp = new HashMap<Long, Val>();
        boolean isRet = isReturn(in.getMnemonicString());
        Val spStart = regs.get(spKey);
        for (PcodeOp op : in.getPcode()) {
            exec(op, regs, tmp, r, in, isRet);
        }
        FlowType ft = in.getFlowType();
        if (ft.isCall()) {
            Val spAfter = regs.get(spKey);
            long pushed = (spStart != null && spStart.k == ENTRY && spAfter != null && spAfter.k == ENTRY)
                    ? spStart.v - spAfter.v : -1;
            for (Map.Entry<Long, Val> en : regs.entrySet()) {
                if (en.getKey() != spKey && en.getKey() != fpKey && en.getValue().k == ENTRY) {
                    r.unres("stack address live across a call at " + in.getAddress());
                }
            }
            long purge = pushed;
            Address[] flows = in.getFlows();
            boolean exact = !ft.isComputed() || ABS_TARGET.matcher(in.toString()).matches();
            Function callee = (exact && flows.length == 1)
                    ? program.getFunctionManager().getFunctionAt(flows[0]) : null;
            if (callee == null) {
                r.unres("call to unknown or indirect target at " + in.getAddress());
            }
            else {
                int pc = callee.getStackPurgeSize();
                if (pc >= 0 && pc <= MAX_PURGE) {
                    purge = pc;
                }
                else {
                    r.unres("callee purge size unknown at " + in.getAddress());
                }
            }
            if (spAfter != null && spAfter.k == ENTRY && purge >= 0) {
                regs.put(spKey, new Val(ENTRY, spAfter.v + purge, spAfter.sz));
            }
            Iterator<Map.Entry<Long, Val>> it = regs.entrySet().iterator();
            while (it.hasNext()) {
                long k = it.next().getKey();
                if (k != spKey && k != fpKey) {
                    it.remove();
                }
            }
        }
        Val spNow = regs.get(spKey);
        if (!isRet && (spNow == null || spNow.k != ENTRY)) {
            r.unres("stack pointer not trackable after " + in.getAddress());
        }
        return regs;
    }

    private Result analyze(Function f) {
        Result r = new Result();
        AddressSetView body = f.getBody();
        Map<Address, Instruction> insns = new LinkedHashMap<Address, Instruction>();
        for (Instruction i : program.getListing().getInstructions(body, true)) {
            insns.put(i.getAddress(), i);
        }
        if (insns.isEmpty()) {
            r.unres("no instructions");
            return r;
        }
        boolean anyP = false;
        boolean anyN = false;
        for (Instruction i : insns.values()) {
            String m = i.getMnemonicString();
            if (m.equals("prts")) {
                anyP = true;
            }
            else if (m.equals("rts")) {
                anyN = true;
            }
            else if (m.startsWith("prtd") || m.startsWith("rtd")) {
                r.reads = true;
                if (r.why.size() < 4) {
                    r.why.add("return that pops arguments at " + i.getAddress());
                }
            }
            else if (m.equals("rte")) {
                r.unres("rte (interrupt return)");
            }
        }
        r.retSize = anyN ? 2 : (anyP ? 4 : 2);

        Map<Long, Val> init = new HashMap<Long, Val>();
        init.put(Long.valueOf(spKey), new Val(ENTRY, 0, spSize));
        Map<Address, Map<Long, Val>> in = new HashMap<Address, Map<Long, Val>>();
        Map<Address, Integer> visits = new HashMap<Address, Integer>();
        ArrayDeque<Address> work = new ArrayDeque<Address>();
        Address entry = f.getEntryPoint();
        in.put(entry, init);
        work.add(entry);
        while (!work.isEmpty()) {
            Address a = work.poll();
            Instruction ins = insns.get(a);
            if (ins == null) {
                r.unres("flow into a non-instruction at " + a);
                continue;
            }
            int vis = visits.merge(a, Integer.valueOf(1), Integer::sum).intValue();
            if (vis > MAX_VISITS) {
                r.unres("no fixpoint at " + a);
                break;
            }
            Map<Long, Val> out = step(ins, in.get(a), r);
            if (isReturn(ins.getMnemonicString())) {
                continue;
            }
            List<Address> succ = new ArrayList<Address>();
            if (ins.getFlowType().isJump()) {
                Address[] fl = ins.getFlows();
                if (fl.length == 0) {
                    r.unres("computed jump without targets at " + a);
                }
                for (Address t : fl) {
                    if (body.contains(t)) {
                        succ.add(t);
                    }
                    else {
                        Val sp = out.get(spKey);
                        if (sp == null || sp.k != ENTRY || sp.v != 0) {
                            r.unres("tail jump with the frame not unwound at " + a);
                        }
                        r.tails.add(t);
                    }
                }
            }
            if (ins.getFallThrough() != null) {
                succ.add(ins.getFallThrough());
            }
            for (Address s : succ) {
                Map<Long, Val> cur = in.get(s);
                if (cur == null) {
                    in.put(s, new HashMap<Long, Val>(out));
                    work.add(s);
                }
                else {
                    boolean ch = false;
                    Iterator<Map.Entry<Long, Val>> it = cur.entrySet().iterator();
                    while (it.hasNext()) {
                        Map.Entry<Long, Val> en = it.next();
                        Val o = out.get(en.getKey());
                        if (o == null || !o.same(en.getValue())) {
                            it.remove();
                            ch = true;
                        }
                    }
                    if (ch) {
                        work.add(s);
                    }
                }
            }
        }
        return r;
    }
}

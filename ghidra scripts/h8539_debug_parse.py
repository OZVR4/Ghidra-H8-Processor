# Debug: test read_preceding_push_pair logic directly against the known
# 0x21802 call site to find why h8539_find_axis_consumer.py reports
# NO-CALLER despite the ref being present in getReferencesTo results.
# @category MitsubishiECU

space = currentProgram.getAddressFactory().getDefaultAddressSpace()
listing = currentProgram.getListing()
refMgr = currentProgram.getReferenceManager()

target = space.getAddress(0x00014735)

def read_preceding_push_pair(instr):
    prev1 = instr.getPrevious()
    prev2 = prev1.getPrevious() if prev1 is not None else None
    if prev1 is None or prev2 is None:
        return None
    try:
        bank_str = prev1.toString()
        imm_str  = prev2.toString()
        print("    bank_str=%r imm_str=%r" % (bank_str, imm_str))
        if "#0x" not in bank_str or "#0x" not in imm_str:
            print("    -> rejected: missing #0x")
            return None
        bank = int(bank_str.split("#0x")[1].split(":")[0], 16)
        imm  = int(imm_str.split("#0x")[1].split(":")[0], 16)
        return (bank, imm)
    except Exception as e:
        print("    -> exception: %s" % e)
        return None

count = 0
matched = 0
for ref in refMgr.getReferencesTo(target):
    if not ref.getReferenceType().isCall():
        continue
    from_addr = ref.getFromAddress()
    if str(from_addr) != "00021802":
        continue
    count += 1
    call_instr = listing.getInstructionAt(from_addr)
    print("Found matching ref, from=%s call_instr=%s" % (from_addr, call_instr))
    pushed = read_preceding_push_pair(call_instr)
    print("pushed = %s" % (pushed,))
    if pushed is not None:
        bank, imm = pushed
        print("bank=0x%x imm=0x%x  (expect bank=2 imm=0xd080)" % (bank, imm))
        if bank == 2 and imm == 0x2d080:
            matched += 1
        print("imm == 0x2d080 ? %s" % (imm == 0x2d080))
        print("imm == 0xd080 ? %s" % (imm == 0xd080))

print("count(refs matching addr)=%d matched(bank+imm)=%d" % (count, matched))

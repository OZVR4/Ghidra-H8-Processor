# Debug helper: dump the exact toString() format of instructions
# preceding a known axis_lookup_interp call site, to diagnose why
# h8539_find_axis_consumer.py's read_preceding_push_pair() string
# parsing isn't matching. Temporary, not part of the review workflow.
# @category MitsubishiECU

listing = currentProgram.getListing()
space = currentProgram.getAddressFactory().getDefaultAddressSpace()

call_site = space.getAddress(0x00021802)  # known pjsr @0x14735 for 0xd080
instr = listing.getInstructionAt(call_site)
print("call instr: %s" % instr.toString())
p1 = instr.getPrevious()
p2 = p1.getPrevious() if p1 is not None else None
print("prev1 (%s): %s" % (p1.getAddress() if p1 else None, p1.toString() if p1 else None))
print("prev2 (%s): %s" % (p2.getAddress() if p2 else None, p2.toString() if p2 else None))

# also check refs from the call instr
refs = instr.getReferencesFrom()
for r in refs:
    print("ref: type=%s to=%s isCall=%s" % (r.getReferenceType(), r.getToAddress(), r.getReferenceType().isCall()))

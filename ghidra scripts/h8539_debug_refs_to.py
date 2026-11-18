# Debug helper: check what getReferencesTo(axis_lookup_interp) actually
# returns, to diagnose why h8539_find_axis_consumer.py's ref-based scan
# missed a known-good call site at 0x21802.
# @category MitsubishiECU

space = currentProgram.getAddressFactory().getDefaultAddressSpace()
refMgr = currentProgram.getReferenceManager()

target = space.getAddress(0x00014735)  # axis_lookup_interp
refs = list(refMgr.getReferencesTo(target))
print("Total refs to 0x14735: %d" % len(refs))
count = 0
for r in refs:
    count += 1
    if count <= 40:
        print("  from=%s type=%s isCall=%s" % (r.getFromAddress(), r.getReferenceType(), r.getReferenceType().isCall()))

# specifically check if 0x217fb's call site (0x21802) is among them
found = False
for r in refs:
    if str(r.getFromAddress()) == "00021802":
        found = True
print("0x21802 present in refs list: %s" % found)

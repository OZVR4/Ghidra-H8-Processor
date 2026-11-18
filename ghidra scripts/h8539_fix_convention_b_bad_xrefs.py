# H8/539F ECU - Convention B Bad-Xref Cleanup
# @category MitsubishiECU
#
# ONE-TIME FIX: h8539_find_convention_b_writers.py's first version had a
# ROM_BASE double-counting bug in create_value_ptr_xref() (same bug class
# review10.md already documented and fixed once in
# h8539_audit_axis_data_length.py's read_axis_count() -- addr() adds
# ROM_BASE, which is correct for CPU-page/table-header offsets but WRONG
# for value_ptr, which is a bare on-chip RAM address on its own). That bug
# created xrefs like 00011e96 -> 0001f0c8 instead of the correct
# 00011e96 -> 0000f0c8.
#
# This script removes any header+2 -> value_ptr xref pointing at
# ROM_BASE+value_ptr (the WRONG address) for the same TABLE_HEADER_ADDRESSES
# list used by the writer-finder script, so re-running the fixed version
# doesn't leave stale bad data sitting alongside the corrected xref.
#
# Run this ONCE after pulling the ROM_BASE fix, before re-running
# h8539_find_convention_b_writers.py.

from ghidra.program.model.symbol import RefType

currentProgramRef = currentProgram
space   = currentProgram.getAddressFactory().getDefaultAddressSpace()
refMgr  = currentProgram.getReferenceManager()
mem     = currentProgram.getMemory()

ROM_BASE = 0x00010000

# Same table list as h8539_find_convention_b_writers.py -- keep in sync.
TABLE_HEADER_ADDRESSES = [
    0x00011e94,
    0x000127d8,
]


def addr(offset):
    return space.getAddress((ROM_BASE + offset) & 0xFFFFFFFF)


def main():
    print("=" * 70)
    print("H8/539F Convention B Bad-Xref Cleanup")
    print("=" * 70)
    removed = 0
    for header_offset in TABLE_HEADER_ADDRESSES:
        field_addr = addr(header_offset + 2)
        try:
            value_ptr = mem.getShort(addr(header_offset + 2)) & 0xFFFF
        except Exception as e:
            print("  WARNING: could not re-read header @ 0x%05X: %s" % (ROM_BASE + header_offset, e))
            continue
        bad_addr = addr(value_ptr)  # the WRONG, ROM_BASE-doubled address
        for r in list(refMgr.getReferencesFrom(field_addr)):
            if r.getToAddress().equals(bad_addr):
                refMgr.delete(r)
                print("  removed bad xref: %s -> %s" % (field_addr, bad_addr))
                removed += 1
    print("")
    print("Total bad xrefs removed: %d" % removed)
    print("=" * 70)


main()

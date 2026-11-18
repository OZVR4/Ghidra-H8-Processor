# H8/539F ECU - Convention B value_ptr Writer Finder
# @category MitsubishiECU
#
# PROBLEM THIS SOLVES: review10.md documents "Convention B" table_lookup_interp
# call sites (single literal pushed -- table only, no axis literal) where the
# axis pointer is NOT caller-supplied but instead read out of the table's own
# ROM header at table+2 ("value_ptr", a RAM cell address, e.g. 0xF0C8). Finding
# what WRITES that RAM cell is blocked for the general case (EP-banked
# SEGMENTOP, review.md item 8 step 3b, not yet compiled/tested) -- see
# review10.md "WHY IT'S STILL BLOCKED" section.
#
# All three static methods already tried and exhausted (per review10.md):
#   1. get_xrefs_to(value_ptr)                -- zero results
#   2. search_byte_patterns for the raw bytes -- only lands inside table
#      headers (READER's own stored value_ptr field), never as a write
#   3. find_code_gaps() near a plausible handler -- no hidden function
#
# UNTRIED ANGLE this script adds: all three above searched for the RAM cell
# as an IMMEDIATE VALUE being pushed/referenced generically. None of them
# specifically searched the disassembly text for the RAM cell appearing as
# a DESTINATION memory operand of a direct-mode (DP-relative, not EP-banked)
# write instruction -- e.g. "mov:g.w R0,@0xf0c8:16" or similar. A write that
# uses DP (already working, per review10.md's step 1/2 confirmation) rather
# than EP would be completely invisible to xref-based search (since nothing
# in Ghidra's segmentop resolution failed for DP), invisible to the raw
# byte-pattern search (which looked for the VALUE 0xF0C8, not literal
# hex-in-a-destination-operand text), and irrelevant to the code-gap search.
# This script is a plain instruction-text scan across the whole ROM's
# defined instructions, looking for the RAM offset appearing specifically
# in a write/destination position.
#
# WHAT THIS SCRIPT DOES, for a given list of table header addresses:
#   1. Reads each table's header bytes directly from ROM (mode/correction/
#      value_ptr, per review10.md's documented header layout).
#   2. Creates a real Ghidra xref from the table header's value_ptr FIELD
#      (header+2) to the RAM cell it points at. This part is NOT blocked --
#      value_ptr is a plain 16-bit value sitting in ROM, no segmentation
#      involved in reading it.
#   3. Scans every defined instruction in the program for the value_ptr RAM
#      offset appearing as a destination (write) operand specifically,
#      using operand-position + mnemonic heuristics (mov-family with the
#      RAM offset in the final operand for register/immediate->memory
#      forms).
#   4. Reports each candidate hit with enough context (function, address,
#      full instruction text) for a human to manually verify -- this script
#      does NOT auto-create write-site xrefs, since a text-match hit is a
#      candidate, not a confirmed write, and could still be a coincidental
#      operand match (same false-positive class already documented
#      project-wide, e.g. the "EE 90" mid-instruction byte coincidence
#      noted in review10.md).
#   5. Bookmarks each table header with its resolved value_ptr and current
#      status (WRITER-CANDIDATE-FOUND vs WRITER-NOT-FOUND-STILL-BLOCKED) so
#      progress is visible/filterable in the Bookmarks window and this
#      script is safely re-runnable as more tables are added.
#   6. SELF-HEALING: an earlier version of this script had a ROM_BASE
#      double-counting bug when computing the value_ptr xref target
#      (same bug class already documented/fixed once in
#      h8539_audit_axis_data_length.py's read_axis_count(), per
#      review10.md). Every run now checks for and removes any such stale
#      bad xref before creating the correct one -- safe to re-run on a
#      program that was previously touched by the old buggy version.
#      (h8539_fix_convention_b_bad_xrefs.py, the old standalone cleanup
#      script, is superseded by this and no longer needs to be run.)
#
# This is READ-ONLY except for the header->value_ptr xref (step 2, which is
# unblocked and safe) and bookmarks (step 5). It does NOT create write-site
# xrefs automatically -- those need human confirmation first, same discipline
# as h8539_find_table_axis.py and h8539_find_axis_consumer.py.
#
# IMPORT SETTINGS REQUIRED (same ROM setup as the other scripts):
#   Format:       Raw Binary
#   Language:     H8:BE:32:H8539F
#   Base Address: 0x00010000

from ghidra.program.flatapi import FlatProgramAPI
from ghidra.program.model.listing import BookmarkType
from ghidra.program.model.symbol import RefType, SourceType

flat_api    = FlatProgramAPI(currentProgram)
mem         = currentProgram.getMemory()
space       = currentProgram.getAddressFactory().getDefaultAddressSpace()
listing     = currentProgram.getListing()
refMgr      = currentProgram.getReferenceManager()
fnMgr       = currentProgram.getFunctionManager()
bookmarkMgr = currentProgram.getBookmarkManager()

ROM_BASE = 0x00010000
ROM_END  = 0x0002FFFF

# ---------------------------------------------------------------------
# TABLE_HEADER_ADDRESSES is now populated automatically from the
# H8539F-TABLE-NOT-PAIRED and H8539F-TABLE-PAIRING-UNVERIFIED bookmark
# categories left by h8539_import_tables_xml2.py -- those are exactly the
# Convention B candidate buckets per review10.md (no caller-supplied axis
# literal found, so the axis pointer must come from the table's own
# header, if it's read as a lookup table at all). See
# collect_candidate_headers_from_bookmarks() below.
#
# Set MANUAL_TABLE_HEADER_ADDRESSES below to add specific addresses on
# top of the auto-collected ones (e.g. for tables not yet bookmarked, or
# re-testing a specific known example). Leave empty to rely purely on
# the bookmark categories.
# ---------------------------------------------------------------------
MANUAL_TABLE_HEADER_ADDRESSES = [
    # 0x00011e94,  # example: add specific addresses here if needed
]

CANDIDATE_BOOKMARK_CATEGORIES = [
    "H8539F-TABLE-NOT-PAIRED",
    "H8539F-TABLE-PAIRING-UNVERIFIED",
]

MAX_INSTR_SCAN_REPORT = 30   # cap printed candidate hits per RAM cell


def collect_candidate_headers_from_bookmarks():
    """Pull every bookmarked address from the NOT-PAIRED/UNVERIFIED
    categories -- these are exactly the Convention B candidate buckets.
    Returns a de-duplicated list of bare ROM offsets."""
    from ghidra.program.model.listing import BookmarkType
    seen = set()
    result = []
    for category in CANDIDATE_BOOKMARK_CATEGORIES:
        it = bookmarkMgr.getBookmarksIterator(BookmarkType.NOTE)
        while it.hasNext():
            bm = it.next()
            if bm.getCategory() != category:
                continue
            off = norm_offset(bm.getAddress().getOffset())
            if off not in seen:
                seen.add(off)
                result.append(off)
    return result


def norm_offset(a):
    if a >= ROM_BASE:
        return a - ROM_BASE
    return a


def addr(offset):
    return space.getAddress((ROM_BASE + offset) & 0xFFFFFFFF)


def read_table_header(header_offset):
    """Read mode/correction/value_ptr (and second pointer + stride for the
    2D/mode-0x03 path) directly from ROM bytes. Layout per review10.md:
      table[0] = mode byte (0x02 = 1D, 0x03 = 2D/two-axis)
      table[1] = correction/offset byte
      table+2  = ushort value_ptr -- dereferenced to get the index/lookup
                 value (e.g. current RPM/load), NOT an axis breakpoint ptr
      table+4  = second pointer field (2D/mode-0x03 path only)
      table+6  = stride byte (2D path only)
    Returns a dict or None if memory can't be read."""
    a = addr(header_offset)
    try:
        mode        = mem.getByte(a) & 0xFF
        correction  = mem.getByte(a.add(1)) & 0xFF
        value_ptr   = mem.getShort(a.add(2)) & 0xFFFF
        result = {
            "mode": mode,
            "correction": correction,
            "value_ptr": value_ptr,
        }
        if mode == 0x03:
            result["second_ptr"] = mem.getShort(a.add(4)) & 0xFFFF
            result["stride"]     = mem.getByte(a.add(6)) & 0xFF
        return result
    except Exception as e:
        print("  WARNING: could not read header @ 0x%05X: %s" % (ROM_BASE + header_offset, e))
        return None


def remove_bad_rom_base_doubled_xref(header_offset, value_ptr):
    """One-time self-healing cleanup: an earlier version of this script had
    a ROM_BASE double-counting bug (addr(value_ptr) instead of the bare RAM
    address) that created a WRONG xref pointing at ROM_BASE+value_ptr. Find
    and remove any such stale reference before creating the correct one, so
    re-running this script also repairs programs that hit the old bug."""
    field_addr = addr(header_offset + 2)
    bad_addr = addr(value_ptr)  # the WRONG, ROM_BASE-doubled address
    removed = 0
    for r in list(refMgr.getReferencesFrom(field_addr)):
        if r.getToAddress().equals(bad_addr):
            refMgr.delete(r)
            print("  removed stale bad xref (ROM_BASE double-count bug): %s -> %s" %
                  (field_addr, bad_addr))
            removed += 1
    return removed


def create_value_ptr_xref(header_offset, value_ptr):
    """Create a real Ghidra xref from the table header's value_ptr FIELD
    (header+2) to the RAM cell it points at. This is unblocked -- reading
    value_ptr out of the ROM header requires no segmentation/EP resolution
    at all, only the RAM cell's WRITER is blocked."""
    field_addr = addr(header_offset + 2)
    try:
        # value_ptr is a bare on-chip RAM address (per review10.md, on-chip
        # RAM spans 0xEE80-0xFE7F on this chip in mode 7/single-chip) --
        # NOT a ROM_BASE-relative CPU-page offset. addr() adds ROM_BASE,
        # which is correct for table headers/code but WRONG here -- this
        # is the same double-counting bug review10.md already found and
        # fixed in h8539_audit_axis_data_length.py's read_axis_count().
        # Use the RAM offset directly, no ROM_BASE addition.
        ram_addr = space.getAddress(value_ptr & 0xFFFFFFFF)
    except Exception as e:
        print("  WARNING: value_ptr 0x%04X does not resolve to a valid address: %s" % (value_ptr, e))
        return False
    if not mem.contains(ram_addr):
        print("  WARNING: value_ptr 0x%04X falls outside any defined memory block -- skipping xref" % value_ptr)
        return False
    already = False
    for r in refMgr.getReferencesFrom(field_addr):
        if r.getToAddress().equals(ram_addr):
            already = True
            break
    if not already:
        refMgr.addMemoryReference(field_addr, ram_addr, RefType.DATA, SourceType.USER_DEFINED, 0)
        print("  xref added: header+2 @ %s -> value_ptr cell %s" % (field_addr, ram_addr))
    else:
        print("  xref already present: header+2 @ %s -> value_ptr cell %s" % (field_addr, ram_addr))
    return True


def instr_writes_ram_offset(instr, ram_offset):
    """Heuristic: does this instruction WRITE to ram_offset, as opposed to
    merely mentioning it? Checks the FINAL operand specifically (H8 mov-
    family destination position in Ghidra's operand representation), to
    distinguish 'write to this cell' from 'read this cell' or an unrelated
    coincidental mention elsewhere in the instruction. This is a text-match
    heuristic, not p-code analysis -- every hit is a CANDIDATE requiring
    manual confirmation, same discipline as the rest of this project (see
    docstring re: 'EE 90' false-positive class)."""
    if instr is None:
        return False
    mnem = instr.getMnemonicString().lower()
    if not mnem.startswith("mov"):
        return False
    num_ops = instr.getNumOperands()
    if num_ops == 0:
        return False
    dest_repr = instr.getDefaultOperandRepresentation(num_ops - 1)
    if dest_repr is None:
        return False
    dest_repr_lower = dest_repr.lower()
    needle = "0x%x" % ram_offset
    if needle not in dest_repr_lower:
        return False
    # Require it look like a direct memory destination, not a register or
    # immediate coincidentally containing the same digits as a register
    # name/number -- Ghidra's operand representation for direct-page memory
    # operands on this architecture uses an '@' prefix.
    if "@" not in dest_repr_lower:
        return False
    return True


def scan_whole_rom_for_writers(ram_offset):
    """Whole-ROM scan for instructions whose destination operand is
    ram_offset. Reports every function/address/instruction text hit for
    manual review -- does not create any xref automatically."""
    hits = []
    fn_iter = fnMgr.getFunctions(True)
    for fn in fn_iter:
        body = fn.getBody()
        instrs = listing.getInstructions(body, True)
        for instr in instrs:
            if instr_writes_ram_offset(instr, ram_offset):
                hits.append((fn.getName(), instr.getAddress().toString(), instr.toString()))
    return hits


def safe_bookmark(a, category, text):
    try:
        bookmarkMgr.setBookmark(a, BookmarkType.NOTE, category, text)
    except Exception as e:
        print("  WARNING bookmark @ %s: %s" % (a, e))


def process_table(header_offset_raw):
    header_offset = norm_offset(header_offset_raw)
    header_addr = addr(header_offset)
    print("")
    print("=== Table header 0x%05X ===" % (ROM_BASE + header_offset))

    hdr = read_table_header(header_offset)
    if hdr is None:
        safe_bookmark(header_addr, "H8539F-CONVENTION-B-READ-FAILED",
                      "Could not read table header bytes")
        return

    print("  mode=0x%02X  correction=0x%02X  value_ptr=0x%04X" %
          (hdr["mode"], hdr["correction"], hdr["value_ptr"]))
    if "second_ptr" in hdr:
        print("  second_ptr=0x%04X  stride=0x%02X" % (hdr["second_ptr"], hdr["stride"]))

    value_ptr = hdr["value_ptr"]

    # Self-healing: remove any stale bad xref from the old ROM_BASE-doubling
    # bug before creating the correct one.
    remove_bad_rom_base_doubled_xref(header_offset, value_ptr)

    # Step 2: create the unblocked header -> value_ptr xref.
    create_value_ptr_xref(header_offset, value_ptr)

    # Step 3/4: search for a direct-mode writer of this RAM cell.
    print("  Scanning whole ROM for direct-mode writers of 0x%04X ..." % value_ptr)
    hits = scan_whole_rom_for_writers(value_ptr)

    if hits:
        print("  FOUND %d writer candidate(s):" % len(hits))
        for fn_name, a_str, instr_str in hits[:MAX_INSTR_SCAN_REPORT]:
            print("    %s @ %s :: %s" % (fn_name, a_str, instr_str))
        if len(hits) > MAX_INSTR_SCAN_REPORT:
            print("    ... and %d more" % (len(hits) - MAX_INSTR_SCAN_REPORT))
        safe_bookmark(header_addr, "H8539F-CONVENTION-B-WRITER-CANDIDATE-FOUND",
                      "value_ptr=0x%04X -- %d direct-mode writer candidate(s) found, "
                      "needs manual confirmation before treating as PAIRED" %
                      (value_ptr, len(hits)))
    else:
        print("  No direct-mode writer candidates found. Still blocked pending")
        print("  review.md item 8 step 3b (EP-banked SEGMENTOP), per review10.md.")
        safe_bookmark(header_addr, "H8539F-CONVENTION-B-WRITER-NOT-FOUND",
                      "value_ptr=0x%04X -- no direct-mode writer found; likely EP-banked, "
                      "blocked on review.md item 8 step 3b (see review10.md)" % value_ptr)

    return {
        "header_offset": header_offset,
        "value_ptr": value_ptr,
        "writer_hits": hits,
    }


def main():
    print("=" * 70)
    print("H8/539F Convention B value_ptr Writer Finder")
    print("=" * 70)

    auto_addrs = collect_candidate_headers_from_bookmarks()
    all_addrs = []
    seen = set()
    for a in auto_addrs + [norm_offset(x) for x in MANUAL_TABLE_HEADER_ADDRESSES]:
        if a not in seen:
            seen.add(a)
            all_addrs.append(a)

    print("")
    print("Candidates from bookmarks (%s): %d" %
          (", ".join(CANDIDATE_BOOKMARK_CATEGORIES), len(auto_addrs)))
    print("Manual additions: %d" % len(MANUAL_TABLE_HEADER_ADDRESSES))
    print("Total unique table headers to process: %d" % len(all_addrs))

    results = []
    for t in all_addrs:
        r = process_table(t)
        if r is not None:
            results.append(r)
    print("")
    print("=" * 70)
    n_found = sum(1 for r in results if r["writer_hits"])
    n_blocked = len(results) - n_found
    print("Summary: %d table(s) with writer candidate(s) found, %d still blocked (of %d)" %
          (n_found, n_blocked, len(results)))
    if n_found:
        print("")
        print("Tables with writer candidate(s) -- worth manual follow-up:")
        for r in results:
            if r["writer_hits"]:
                print("  0x%05X (value_ptr=0x%04X, %d candidate(s))" %
                      (ROM_BASE + r["header_offset"], r["value_ptr"], len(r["writer_hits"])))
    print("=" * 70)


main()

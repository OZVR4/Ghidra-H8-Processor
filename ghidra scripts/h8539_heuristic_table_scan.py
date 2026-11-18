# H8/539F ECU - Standalone ROM Heuristic Table Scanner
# @category MitsubishiECU
#
# Standalone extraction of the ROM scraper table-scanning logic from
# h8539_import_tables_xml2.py's run_rom_scraper() (Step 8 in that script).
#
# Detects, by raw byte-pattern scan (no XML required):
#   Pass 1 - MUT table (fixed CPU offset 0x1FAD0): 256 x 2-byte RAM pointers.
#   Pass 2 - Scaling tables (6-byte header):
#            [F0-F7] xx [E0-FE] xx 00 [02-90]
#   Pass 3 - 3D value tables (7-byte header): 0x03 00 Xptr Yptr nrows
#   Pass 4 - 2D value tables (4-byte header): 0x02 00 axis_ptr
#   Pass 4b - Indirect lookup records: same 4-byte shape as a 2D header,
#             but consumed whole by table_lookup_interp() with NO trailing
#             data array -- confirmed live via decompile at 0x11F5C. These
#             chain back-to-back with zero gap; each record's "next header"
#             check IS the boundary of the current one (see
#             find_next_header_boundary / CHAIN NEXT line in its comment).
#
# COMMENT/BOOKMARK FORMAT: compact, bordered block -- see build_block().
# Category names have the "H8539F-" prefix dropped (redundant -- every
# bookmark this script writes is already scoped to this ROM) and several
# long names shortened; see CATEGORY MAP note above safe_bookmark().
#
# This script WRITES to the program (labels, plate comments, bookmarks,
# byte-array typing) -- it is not read-only.
#
# Run standalone, any time, independent of the XML-import script. If that
# script's apply_xml() was run first in this same program, this scanner
# skips re-labeling any CPU offset already recorded as XML-touched (via
# get_last_xml_touched_addrs()). Additionally, if an EXISTING label at a
# candidate address is itself a stale TABLE_2D_*/TABLE_3D_* name whose
# embedded hex address doesn't match where the label actually sits (a
# known artifact of an older header-offset-convention guess), this
# scanner treats that as safely overwritable rather than "already
# correctly labeled" -- see is_stale_mislabel().
#
# IMPORT SETTINGS REQUIRED (same ROM setup as the master script):
#   Format:       Raw Binary
#   Language:     H8:BE:32:H8539F
#   Base Address: 0x00010000

import re
from ghidra.program.flatapi import FlatProgramAPI

# ──────────────────────────────────────────────────────────────────
# Setup
# ──────────────────────────────────────────────────────────────────

flat_api = FlatProgramAPI(currentProgram)
mem      = currentProgram.getMemory()
space    = currentProgram.getAddressFactory().getDefaultAddressSpace()
listing  = currentProgram.getListing()
symTable = currentProgram.getSymbolTable()
bookmarkMgr = currentProgram.getBookmarkManager()
refMgr   = currentProgram.getReferenceManager()

ROM_BASE  = 0x00010000
ROM_END   = 0x0002FFFF
ROM_SIZE  = 0x20000      # 128 KB (two 64 KB pages)

MUT_OFFSET  = 0x1FAD0
MUT_ADDR    = ROM_BASE + MUT_OFFSET
MUT_ENTRIES = 256

MEM_ADDR_MIN = 0xEE00
MEM_ADDR_MAX = 0xFEFF

PROGRAM_INFO_CATEGORY = "H8539F Setup Script"
XML_TOUCHED_ADDRS_KEY = "Last XML Touched Addresses"

# ──────────────────────────────────────────────────────────────────
# Shared helpers
# ──────────────────────────────────────────────────────────────────

def addr(offset):
    return space.getAddress(offset & 0xFFFFFFFF)

def in_ram(val):
    return MEM_ADDR_MIN <= val <= MEM_ADDR_MAX

def rom_byte(cpu_offset):
    try:
        return flat_api.getByte(addr(ROM_BASE + cpu_offset)) & 0xFF
    except Exception:
        return 0

def rom_word(cpu_offset):
    return (rom_byte(cpu_offset) << 8) | rom_byte(cpu_offset + 1)

def rom_bytes_at(ghidra_addr, length):
    result = []
    for i in range(length):
        try:
            result.append(flat_api.getByte(addr(ghidra_addr + i)) & 0xFF)
        except Exception:
            result.append(0)
    return result

def find_sentinel_data_len(start, max_len=0x1FF):
    for j in range(max_len):
        if rom_byte(start + j) == 0xFF:
            return j + 1
    return None

def header_shape_ok(header_offset, ttype):
    """POSITIVE-EVIDENCE only; combine with a data-length check."""
    if ttype == "2D":
        b = rom_bytes_at(ROM_BASE + header_offset, 4)
        if len(b) < 4:
            return False
        mode_ok = (b[0] == 0x02 and b[1] == 0x00)
        ptr = (b[2] << 8) | b[3]
        return mode_ok and in_ram(ptr)
    elif ttype == "3D":
        b = rom_bytes_at(ROM_BASE + header_offset, 7)
        if len(b) < 7:
            return False
        mode_ok = (b[0] == 0x03 and b[1] == 0x00)
        x_ptr = (b[2] << 8) | b[3]
        y_ptr = (b[4] << 8) | b[5]
        return mode_ok and in_ram(x_ptr) and in_ram(y_ptr)
    return False

def is_likely_code(header_offset):
    """True if this address is genuinely code (instruction AT/CONTAINING
    it, or a jump/call xref into it) -- catches byte-shape coincidences
    landing inside real instructions."""
    a = addr(ROM_BASE + header_offset)
    try:
        cu = listing.getCodeUnitAt(a)
        if cu is None:
            cu = listing.getCodeUnitContaining(a)
        if cu is not None and cu.getMnemonicString() and not cu.getMnemonicString().startswith("??"):
            from ghidra.program.model.listing import Instruction
            if isinstance(cu, Instruction):
                return True
    except Exception:
        pass
    try:
        refs = refMgr.getReferencesTo(a)
        for ref in refs:
            rt = ref.getReferenceType()
            if rt.isJump() or rt.isCall():
                return True
    except Exception:
        pass
    return False

def has_data_xref(header_offset):
    """True if a real DATA-type (non jump/call) xref lands on this address
    -- e.g. a literal pointer passed to table_lookup_interp(). Confirmed
    live on this ROM at 0x11F5C via decompile of
    f2ea_f2ee_composite_correction_calc."""
    a = addr(ROM_BASE + header_offset)
    try:
        refs = refMgr.getReferencesTo(a)
        for ref in refs:
            rt = ref.getReferenceType()
            if not (rt.isJump() or rt.isCall()) and rt.isData():
                return True
    except Exception:
        pass
    return False

def indirect_lookup_record_shape_ok(header_offset):
    """Same byte shape as a 2D/3D header (mode 0x02/0x03, RAM ptr) but
    consumed whole, with no trailing data array. Distinguish from a real
    2D/3D header only via has_data_xref + next-header chain, never by
    bytes alone (they are identical)."""
    b = rom_bytes_at(ROM_BASE + header_offset, 4)
    if len(b) < 4:
        return False
    mode_ok = b[0] in (0x02, 0x03)
    ptr = (b[2] << 8) | b[3]
    return mode_ok and in_ram(ptr)

def find_next_header_boundary(start, max_len=0x1FF):
    """Scan forward for the next byte offset that itself looks like a
    genuine 2D/3D header; that's the boundary of the CURRENT record.
    Starts at j=1 so a record's own first bytes can't self-match."""
    for j in range(1, max_len):
        off = start + j
        if header_shape_ok(off, "2D") or header_shape_ok(off, "3D"):
            return j
    return None

_TABLE_LABEL_RE = re.compile(r'^TABLE_(2D|3D)_([0-9A-Fa-f]{8})')

def is_stale_mislabel(a, cpu_offset):
    """
    True if the label AT Ghidra address 'a' is a TABLE_2D_*/TABLE_3D_*
    name whose embedded hex address does NOT match cpu_offset's own
    Ghidra address -- i.e. a leftover label from an older header-offset
    convention guess that put the name 4 (or 7) bytes away from where it
    actually sits.

    Confirmed real example on this ROM: 'TABLE_2D_00011F5C_CompositeCorr'
    was found sitting at Ghidra address 0x11F60, not 0x11F5C as its own
    name claims -- a leftover from an older script guessing the header
    sat 4 bytes earlier than it really did. Without this check, that
    label blocks this scanner from ever re-labeling 0x11F60 correctly,
    since 'symbol already exists' normally means 'already correct'.
    """
    sym = symTable.getPrimarySymbol(a)
    if sym is None:
        return False
    m = _TABLE_LABEL_RE.match(sym.getName())
    if not m:
        return False
    embedded_addr = int(m.group(2), 16)
    return embedded_addr != (ROM_BASE + cpu_offset)

def safe_label(a, name, overwrite=False):
    try:
        primary = symTable.getPrimarySymbol(a)
        if primary is not None and not overwrite:
            return False
        flat_api.createLabel(a, name, True)
        return True
    except Exception as e:
        print("  WARNING label '%s' @ %s: %s" % (name, a, e))
        return False

def safe_label_offset(cpu_offset, name, overwrite=False):
    return safe_label(addr(ROM_BASE + cpu_offset), name, overwrite)

def safe_label_ram(ram_val, name):
    if not in_ram(ram_val):
        return
    a = addr(ram_val)
    blk = mem.getBlock(a)
    if blk is None:
        return
    safe_label(a, name)

_warned_overlap_regions = set()

def safe_plate(a, text, written_set=None):
    try:
        from ghidra.program.model.listing import CodeUnit
        cu = listing.getCodeUnitAt(a)
        overlap = False
        if cu is None:
            cu = listing.getCodeUnitContaining(a)
            if cu is not None:
                overlap = True
        if cu is None:
            return
        if overlap:
            region_key = str(cu.getMinAddress())
            if region_key not in _warned_overlap_regions:
                _warned_overlap_regions.add(region_key)
                print("  WARNING: overlap - region @ %s (%d bytes) has entries "
                      "from more than one candidate (first seen via %s); "
                      "verify addresses by hand." %
                      (cu.getMinAddress(), cu.getLength(), a))
            text = "[OVERLAP] " + text
        key = str(cu.getMinAddress())
        if written_set is not None and key not in written_set:
            written_set.add(key)
            cu.setComment(CodeUnit.PLATE_COMMENT, text)
            return
        existing = cu.getComment(CodeUnit.PLATE_COMMENT)
        if existing and text in existing:
            return
        new_text = (existing + "\n" + text) if existing else text
        cu.setComment(CodeUnit.PLATE_COMMENT, new_text)
        if written_set is not None:
            written_set.add(key)
    except Exception as e:
        print("  WARNING plate comment @ %s: %s" % (a, e))

def safe_plate_offset(cpu_offset, text, written_set=None):
    safe_plate(addr(ROM_BASE + cpu_offset), text, written_set)

def safe_bookmark(a, category, text):
    try:
        from ghidra.program.model.listing import BookmarkType
        bookmarkMgr.setBookmark(a, BookmarkType.NOTE, category, text)
    except Exception as e:
        print("  WARNING bookmark @ %s: %s" % (a, e))

def type_header_as_data(header_addr, size):
    try:
        from ghidra.program.model.data import ByteDataType, ArrayDataType
        end_addr = header_addr.add(size - 1)
        if listing.getInstructionAt(header_addr) is None and \
           listing.getDefinedDataAt(header_addr) is not None and \
           listing.getDefinedDataAt(header_addr).getLength() == size:
            return False
        listing.clearCodeUnits(header_addr, end_addr, False)
        listing.createData(header_addr, ArrayDataType(ByteDataType(), size, 1))
        return True
    except Exception as e:
        print("  WARNING: could not type 0x%08X (%d bytes) as data: %s" %
              (header_addr.getOffset(), size, e))
        return False

def get_last_xml_touched_addrs():
    opts = currentProgram.getOptions(PROGRAM_INFO_CATEGORY)
    raw = opts.getString(XML_TOUCHED_ADDRS_KEY, "")
    if not raw:
        return []
    out = []
    for tok in raw.split(","):
        tok = tok.strip()
        if not tok:
            continue
        try:
            out.append(int(tok, 16))
        except ValueError:
            pass
    return out

# ──────────────────────────────────────────────────────────────────
# Compact comment/bookmark format
#
# CATEGORY MAP (H8539F- prefix dropped; some names shortened):
#   H8539F-TABLE                -> TABLE
#   H8539F-SCRAPED-TABLE        -> TABLE-SCRAPED
#   H8539F-TABLE-DEGENERATE     -> TABLE-EMPTY
#   H8539F-INDIRECT-LOOKUP-RECORD       -> LOOKUP-REC
#   H8539F-INDIRECT-LOOKUP-RECORD-UNREF -> LOOKUP-REC-UNREF
# ──────────────────────────────────────────────────────────────────

_BAR = "*" * 40

def build_block(title, lines):
    """
    Compact bordered comment block:
      **** <title> ****
      Field  : value
      ...
      ****************************************
    Border kept per user request; field labels padded to a fixed width
    for scan-ability; long prose paragraphs are NOT used -- every line is
    one field, one value.
    """
    out = ["**** %s ****" % title]
    out.extend(lines)
    out.append(_BAR)
    return "\n".join(out)

def field(label, value):
    return "%-8s: %s" % (label, value)

# ──────────────────────────────────────────────────────────────────
# ROM scraper
# ──────────────────────────────────────────────────────────────────

def run_rom_scraper():
    def in_range(val, lo, hi):
        return lo <= val <= hi

    xml_touched = set(get_last_xml_touched_addrs())
    xml_skip_scaling = 0
    xml_skip_3d = 0
    xml_skip_2d = 0
    written_addrs = set()

    claimed_ranges = []

    def in_claimed_range(offset):
        for (lo, hi) in claimed_ranges:
            if lo <= offset < hi:
                return True
        return False

    def label_ok(ghidra_a, cpu_offset):
        """True if this address is safe to (re)label: either nothing is
        there yet, or what's there is a known stale mislabel."""
        sym = symTable.getPrimarySymbol(ghidra_a)
        return sym is None or is_stale_mislabel(ghidra_a, cpu_offset)

    # ── Pass 1: MUT table ─────────────────────────────────────────
    print("[1a] MUT table at ROM offset 0x%05X (Ghidra 0x%08X)..." %
          (MUT_OFFSET, MUT_ADDR))
    mut_count = 0
    tx = currentProgram.startTransaction("MUT table labels")
    try:
        for i in range(MUT_ENTRIES):
            off = MUT_OFFSET + i * 2
            val = rom_word(off)
            entry_lbl = "MUT_%02X_entry" % i
            safe_label_offset(off, entry_lbl)
            block = build_block(entry_lbl, [
                field("Index", "0x%02X (%d)" % (i, i)),
                field("Target", "RAM:0x%04X" % val),
            ])
            safe_plate_offset(off, block, written_addrs)
            safe_label_ram(val, "MUT_%02X" % i)
            mut_count += 1
    finally:
        currentProgram.endTransaction(tx, True)
    print("[1a] OK: %d MUT entries labeled" % mut_count)

    # ── Pass 2: Scaling tables ────────────────────────────────────
    print("[1b] Scanning for scaling tables (0x%08X - 0x%08X)..." %
          (ROM_BASE, MUT_ADDR))
    scaling_count = 0
    tx = currentProgram.startTransaction("Scaling table labels")
    try:
        i = 0
        while i < MUT_OFFSET - 5:
            b = [rom_byte(i + j) for j in range(6)]
            if (in_range(b[0], 0xF0, 0xF7) and
                    in_range(b[2], 0xE0, 0xFE) and
                    b[4] == 0x00 and
                    in_range(b[5], 0x02, 0x90)):
                bogus = False
                max_word_delta = 0
                prev = rom_word(i + 6)
                for j in range(0, 6, 2):
                    nv = rom_word(i + 6 + j)
                    delta = abs(nv - prev)
                    if delta > max_word_delta:
                        max_word_delta = delta
                    if delta > 0x1000:
                        bogus = True
                        break
                    prev = nv
                if not bogus:
                    out_ptr = (b[0] << 8) | b[1]
                    in_ptr  = (b[2] << 8) | b[3]
                    count   = b[5]
                    lbl     = "SCALING_TABLE_%08X" % (ROM_BASE + i)
                    claimed_ranges.append((i, i + 6 + count * 2))
                    if (ROM_BASE + i) in xml_touched:
                        xml_skip_scaling += 1
                    else:
                        type_header_as_data(addr(ROM_BASE + i), 6 + count * 2)
                        safe_label_offset(i, lbl)
                        block = build_block(lbl, [
                            field("Shape", "out 0x%02X in[F0-F7] | in 0x%02X in[E0-FE] | pad ok | count 0x%02X in[02-90]" % (b[0], b[2], b[5])),
                            field("Data", "ok (delta max 0x%04X <= 0x1000)" % max_word_delta),
                            field("Out", "RAM:0x%04X" % out_ptr),
                            field("In", "RAM:0x%04X" % in_ptr),
                            field("Entries", "%d (%d bytes)" % (count, count * 2)),
                            field("Verify", "VERIFIED"),
                            field("Addr", "hdr 0x%08X" % (ROM_BASE + i)),
                            field("Bytes", " ".join("%02X" % x for x in b)),
                        ])
                        safe_plate_offset(i, block, written_addrs)
                        safe_bookmark(addr(ROM_BASE + i), "TABLE-SCRAPED",
                                      "%s (Scaling)" % lbl)
                    safe_label_ram(out_ptr, "SCALING_OUT_%04X" % out_ptr)
                    safe_label_ram(in_ptr,  "SCALING_IN_%04X"  % in_ptr)
                    scaling_count += 1
            i += 1
    finally:
        currentProgram.endTransaction(tx, True)
    print("[1b] OK: %d scaling tables found (%d already covered by prior XML import, skipped)" %
          (scaling_count, xml_skip_scaling))

    # ── Pass 3, 4 & 4b: 3D / 2D value tables + indirect lookup records ──
    print("[1c] Scanning for 2D/3D value tables and indirect lookup records...")
    tbl3d = 0
    tbl2d = 0
    ilr_count = 0
    tx = currentProgram.startTransaction("Value table labels")
    try:
        i = 0
        while i < MUT_OFFSET - 6:
            b = [rom_byte(i + j) for j in range(7)]

            if (header_shape_ok(i, "3D") and
                    not is_likely_code(i) and
                    not in_claimed_range(i)):
                data_len = find_next_header_boundary(i + 7)
                len_method_3d = "next-hdr" if data_len is not None else "sentinel"
                if data_len is None:
                    data_len = find_sentinel_data_len(i + 7)
                if data_len is not None:
                    table_height = b[6] - 1
                    x_ptr_raw = (b[2] << 8) | b[3]
                    y_ptr_raw = (b[4] << 8) | b[5]
                    is_same_axis = x_ptr_raw == y_ptr_raw
                    if is_same_axis:
                        lbl      = "TABLE_3D_%08X_SAMEAXIS" % (ROM_BASE + i + 7)
                        ghidra_a = addr(ROM_BASE + i + 7)
                        block = build_block(lbl, [
                            field("Shape", "mode ok | pad ok | X ptr ok | Y ptr ok"),
                            field("Fault", "X ptr == Y ptr (0x%04X) -- needs 2 distinct axis ptrs" % x_ptr_raw),
                            field("X axis", "RAM:0x%04X" % x_ptr_raw),
                            field("Y axis", "RAM:0x%04X (same as X)" % y_ptr_raw),
                            field("Verify", "SUSPECT (same-axis, not a real grid)"),
                            field("Addr", "hdr 0x%08X" % (ROM_BASE + i)),
                            field("Bytes", " ".join("%02X" % x for x in b)),
                        ])
                        if (ROM_BASE + i) not in xml_touched:
                            if label_ok(ghidra_a, i):
                                safe_label_offset(i, lbl, overwrite=True)
                            safe_plate_offset(i, block, written_addrs)
                            safe_bookmark(addr(ROM_BASE + i), "TABLE-EMPTY",
                                          "%s (3D, same-axis X=Y=0x%04X)" % (lbl, x_ptr_raw))
                        i += 7 + data_len
                        continue
                    is_degenerate = data_len <= 1 or data_len < table_height
                    if is_degenerate:
                        x_ptr = (b[2] << 8) | b[3]
                        y_ptr = (b[4] << 8) | b[5]
                        lbl   = "TABLE_3D_%08X_EMPTY" % (ROM_BASE + i + 7)
                        ghidra_a = addr(ROM_BASE + i + 7)
                        block = build_block(lbl, [
                            field("Shape", "mode ok | X ptr ok | Y ptr ok | X!=Y"),
                            field("Data", "fail (%s, len=%d < height %d rows)" % (len_method_3d, data_len, table_height)),
                            field("X axis", "RAM:0x%04X" % x_ptr),
                            field("Y axis", "RAM:0x%04X" % y_ptr),
                            field("Verify", "SUSPECT (degenerate/empty -- no data rows)"),
                            field("Addr", "hdr 0x%08X" % (ROM_BASE + i)),
                            field("Bytes", " ".join("%02X" % x for x in b)),
                        ])
                        if (ROM_BASE + i) not in xml_touched:
                            if label_ok(ghidra_a, i):
                                safe_label_offset(i, lbl, overwrite=True)
                            safe_plate_offset(i, block, written_addrs)
                            safe_bookmark(addr(ROM_BASE + i), "TABLE-EMPTY",
                                          "%s (3D, empty)" % lbl)
                        safe_label_ram(x_ptr, "AXIS_X_%04X" % x_ptr)
                        safe_label_ram(y_ptr, "AXIS_Y_%04X" % y_ptr)
                        i += 7 + data_len
                        continue
                    if table_height >= 1:
                        x_ptr = (b[2] << 8) | b[3]
                        y_ptr = (b[4] << 8) | b[5]
                        lbl   = "TABLE_3D_%08X" % (ROM_BASE + i + 7)
                        ghidra_a = addr(ROM_BASE + i + 7)
                        block = build_block(lbl, [
                            field("Shape", "mode ok | pad ok | X ptr ok | Y ptr ok | X!=Y"),
                            field("Data", "ok (%s, %d bytes, height %d rows)" % (len_method_3d, data_len, table_height)),
                            field("X axis", "RAM:0x%04X" % x_ptr),
                            field("Y axis", "RAM:0x%04X" % y_ptr),
                            field("Code", "no (not flagged as instruction/jump target)"),
                            field("Verify", "VERIFIED"),
                            field("Addr", "hdr 0x%08X  data 0x%08X" % (ROM_BASE + i, ROM_BASE + i + 7)),
                            field("Bytes", " ".join("%02X" % x for x in b)),
                        ])
                        if (ROM_BASE + i) in xml_touched:
                            xml_skip_3d += 1
                        else:
                            if label_ok(ghidra_a, i):
                                safe_label_offset(i, lbl, overwrite=True)
                            safe_plate_offset(i, block, written_addrs)
                            safe_bookmark(ghidra_a, "TABLE-SCRAPED", "%s (3D)" % lbl)
                        safe_label_ram(x_ptr, "AXIS_X_%04X" % x_ptr)
                        safe_label_ram(y_ptr, "AXIS_Y_%04X" % y_ptr)
                        tbl3d += 1
                        i += 7 + data_len
                        continue

            elif (header_shape_ok(i, "2D") and
                  not is_likely_code(i) and
                  not in_claimed_range(i)):
                # Indirect lookup record check FIRST (see module docstring):
                # same 4-byte shape as an ordinary 2D header, but this ROM
                # packs these back-to-back with zero gap and no trailing
                # data -- confirmed via decompile at 0x11F5C. Detect via
                # has_data_xref (real DATA reference, not jump/call), and
                # report the CHAIN NEXT boundary explicitly so the
                # relationship between adjacent records is visible in the
                # comment, matching the "shape check sees the next table"
                # idea directly.
                if indirect_lookup_record_shape_ok(i):
                    ctx_byte = rom_byte(i + 1)
                    ilr_ptr = (rom_byte(i + 2) << 8) | rom_byte(i + 3)
                    ghidra_a = addr(ROM_BASE + i)
                    has_live_ref = has_data_xref(i)
                    chain_next_ok = header_shape_ok(i + 4, "2D") or header_shape_ok(i + 4, "3D")
                    if has_live_ref:
                        verify = "VERIFIED"
                        ilr_lbl = "TABLE_2D_%08X_INDIRECT_LOOKUP" % (ROM_BASE + i)
                        cat = "LOOKUP-REC"
                    else:
                        verify = "SUSPECT (no xref -- shape-only match, verify manually)"
                        ilr_lbl = "TABLE_2D_%08X_INDIRECT_LOOKUP_UNREF" % (ROM_BASE + i)
                        cat = "LOOKUP-REC-UNREF"
                    block = build_block(ilr_lbl, [
                        field("Shape", "mode 0x%02X in{02,03} | ctx 0x%02X | ptr ok" % (b[0], ctx_byte)),
                        field("Xref", "yes (real DATA ref)" if has_live_ref else "no"),
                        field("Chain", "next hdr @ +4: %s" % ("ok" if chain_next_ok else "none found")),
                        field("Ptr", "RAM:0x%04X (indirect, offset by table_lookup_interp)" % ilr_ptr),
                        field("Verify", verify),
                        field("Addr", "hdr 0x%08X (fixed 4 bytes, no trailing data)" % (ROM_BASE + i)),
                        field("Bytes", " ".join("%02X" % x for x in rom_bytes_at(ROM_BASE + i, 4))),
                    ])
                    if (ROM_BASE + i) not in xml_touched:
                        if label_ok(ghidra_a, i):
                            safe_label_offset(i, ilr_lbl, overwrite=True)
                        safe_plate_offset(i, block, written_addrs)
                        safe_bookmark(ghidra_a, cat,
                                      "%s%s" % (ilr_lbl, "" if has_live_ref else " (NO XREF)"))
                    safe_label_ram(ilr_ptr, "INDIRECT_LOOKUP_%04X" % ilr_ptr)
                    ilr_count += 1
                    i += 4
                    continue
                data_len = find_next_header_boundary(i + 4)
                len_method_2d = "next-hdr" if data_len is not None else "sentinel"
                if data_len is None:
                    data_len = find_sentinel_data_len(i + 4)
                if data_len is not None:
                    is_degenerate = data_len <= 1
                    if is_degenerate:
                        axis_ptr = (b[2] << 8) | b[3]
                        lbl      = "TABLE_2D_%08X_EMPTY" % (ROM_BASE + i + 4)
                        ghidra_a = addr(ROM_BASE + i + 4)
                        block = build_block(lbl, [
                            field("Shape", "mode ok | axis ptr ok"),
                            field("Data", "fail (%s, len=%d <= 1)" % (len_method_2d, data_len)),
                            field("Axis", "RAM:0x%04X" % axis_ptr),
                            field("Verify", "SUSPECT (degenerate/empty -- no data)"),
                            field("Addr", "hdr 0x%08X" % (ROM_BASE + i)),
                            field("Bytes", " ".join("%02X" % x for x in b[:4])),
                        ])
                        if (ROM_BASE + i) not in xml_touched:
                            if label_ok(ghidra_a, i):
                                safe_label_offset(i, lbl, overwrite=True)
                            safe_plate_offset(i, block, written_addrs)
                            safe_bookmark(addr(ROM_BASE + i), "TABLE-EMPTY",
                                          "%s (2D, empty)" % lbl)
                        safe_label_ram(axis_ptr, "AXIS_%04X" % axis_ptr)
                        i += 4 + data_len
                        continue
                    axis_ptr = (b[2] << 8) | b[3]
                    lbl      = "TABLE_2D_%08X" % (ROM_BASE + i + 4)
                    ghidra_a = addr(ROM_BASE + i + 4)
                    block = build_block(lbl, [
                        field("Shape", "mode ok | pad ok | axis ptr ok"),
                        field("Data", "ok (%s, %d bytes)" % (len_method_2d, data_len)),
                        field("Axis", "RAM:0x%04X" % axis_ptr),
                        field("Code", "no (not flagged as instruction/jump target)"),
                        field("Verify", "VERIFIED"),
                        field("Addr", "hdr 0x%08X  data 0x%08X" % (ROM_BASE + i, ROM_BASE + i + 4)),
                        field("Bytes", " ".join("%02X" % x for x in b[:4])),
                    ])
                    if (ROM_BASE + i) in xml_touched:
                        xml_skip_2d += 1
                    else:
                        if label_ok(ghidra_a, i):
                            safe_label_offset(i, lbl, overwrite=True)
                        safe_plate_offset(i, block, written_addrs)
                        safe_bookmark(ghidra_a, "TABLE-SCRAPED", "%s (2D)" % lbl)
                    safe_label_ram(axis_ptr, "AXIS_%04X" % axis_ptr)
                    tbl2d += 1
                    i += 4 + data_len
                    continue

            i += 1
    finally:
        currentProgram.endTransaction(tx, True)
    print("[1c] OK: %d 3D tables, %d 2D tables, %d indirect lookup records (%d total)" %
          (tbl3d, tbl2d, ilr_count, tbl3d + tbl2d + ilr_count))
    print("[1c] %d 3D + %d 2D already covered by a prior XML import, skipped re-labeling" %
          (xml_skip_3d, xml_skip_2d))
    return mut_count, scaling_count, tbl3d, tbl2d, ilr_count


# ──────────────────────────────────────────────────────────────────
print("=" * 60)
print("H8/539F Standalone ROM Heuristic Table Scanner")
print("=" * 60)
mut_count, scaling_count, tbl3d, tbl2d, ilr_count = run_rom_scraper()
print("")
print("=" * 60)
print("Scan complete: %d MUT entries, %d scaling tables, %d 3D tables, %d 2D tables, %d indirect lookup records" %
      (mut_count, scaling_count, tbl3d, tbl2d, ilr_count))
print("=" * 60)

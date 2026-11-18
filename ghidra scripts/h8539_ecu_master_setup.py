# H8/539F ECU - Master Setup Script
# @category MitsubishiECU
#
# ROM SETUP + AUTO-ANALYSIS ONLY. Table annotation (XML import + ROM
# scraper) has been split into a separate script -- see PIPELINE below.
# Run IMMEDIATELY after import, BEFORE clicking "Analyze".
#
# PIPELINE (run in this order):
#   1. h8539_ecu_master_setup_new3.py   <- this script (setup + analysis)
#   2. h8539_import_tables_xml.py       <- XML import + ROM scraper,
#                                           runs AFTER auto-analysis
#   3. h8539_export_tables_xml.py       <- writes labeled tables back
#                                           out to EcuFlash-style XML
#
# WHY XML/SCRAPER MOVED OUT AND MOVED LATER (2026-07-29):
#   XML import and scraping used to run in this script, BEFORE Step 6
#   auto-analysis, on the theory that typing table headers as data early
#   would "protect" them from the disassembler walking through them. Two
#   things changed that:
#     (1) a live gap-scan on a fresh, un-annotated ROM showed that on this
#         ROM/analyzer combination, auto-analysis does NOT wander into the
#         known table regions on its own in the first place -- so that
#         protection wasn't actually load-bearing for most of the ROM.
#     (2) the real risk on this pipeline was never auto-analysis eating
#         tables -- it's community XML addresses being wrong (review.md
#         item 17) or the byte-pattern scraper false-positiving on code
#         that merely looks table-shaped (review.md item 3). Both of
#         those get BETTER, not worse, detection when checked after
#         auto-analysis has run: is_likely_code() (the check that catches
#         "this address is actually real code, not a table") has more
#         xrefs and instructions to work with post-analysis, so a
#         REJECTED/VERIFIED verdict is checked against final, settled ROM
#         state on the first pass, instead of needing a later rescue pass
#         to fix a verdict that was only ever a stale pre-analysis
#         snapshot (the old Step 9's entire original reason for existing).
#   Step 5b below stays in THIS script as the one exception: it only
#   types the fixed-offset MUT table (a known ROM-structure constant, not
#   XML/scraper-derived), which is cheap, always-safe insurance against a
#   previously-observed runaway-function merge bug (see that step's own
#   comment for the concrete incident this fixes).
#
# STEPS (each individually toggleable via prompts):
#
#   Step 1 - Validate base address + ROM ID check  (always runs)
#   Step 2 - Create on-chip RAM block       (0xEE80-0xFFFF)
#   Step 3 - Set CP/DP/TP context registers over ROM pages
#   Step 4  - Read reset vector, create entry function
#   Step 4b - Interrupt vector table: mark as data, create ISR functions,
#             flag duplicate/out-of-range targets, flag ISRs with a
#             non-standard terminating instruction
#   Step 4c - prts/rts function-start scan (report only, fallback/backup
#             for h8539pattern.xml)
#   Step 5  - Decompiler health check       (always runs, gates Step 6)
#   Step 5b - Pre-analysis data table protection (STRONGLY RECOMMENDED) -
#             types the fixed-offset MUT table region as data BEFORE
#             auto-analysis runs, so it can't decode through it as code.
#             (Scaling/2D/3D heuristic typing was deliberately removed
#             from this step already -- see its own comment below.)
#   Step 6  - Auto-analysis
#   Step 6b - Post-analysis undefined-byte gap report (report only)
#   Step 6c - Disassembly error-bookmark triage: classifies "Bad Instruction"
#             bookmarks left after analysis as padding, out-of-bounds flow,
#             data mistakenly swept as code, or a genuine grammar gap, using
#             only generic xref/flow signals (not tied to any known table
#             byte-signature -- covers page 2 too)
#
# Once this script completes, run h8539_import_tables_xml.py to apply
# EcuFlash XML labels and the ROM scraper against the now-analyzed program.
#
# IMPORT SETTINGS REQUIRED:
#   Format:       Raw Binary
#   Language:     H8:BE:32:H8539F
#   Base Address: 0x00010000
#
# ADDRESS CONVENTION:
#   EcuFlash XML "address" = first DATA byte of the table.
#   ROM header precedes data:
#     2D table header = 4 bytes  -> header at xml_addr - 4
#     3D table header = 7 bytes  -> header at xml_addr - 7
#     1D scalar       = no header -> labeled at xml_addr directly
#   Code xrefs point at the header address - that is where we label.

from java.math import BigInteger
from ghidra.program.flatapi import FlatProgramAPI
from ghidra.app.decompiler import DecompInterface, DecompileOptions
import os, xml.etree.ElementTree as ET

flat_api = FlatProgramAPI(currentProgram)
mem      = currentProgram.getMemory()
space    = currentProgram.getAddressFactory().getDefaultAddressSpace()
listing  = currentProgram.getListing()
symTable = currentProgram.getSymbolTable()
bookmarkMgr = currentProgram.getBookmarkManager()

ROM_BASE  = 0x00010000
ROM_END   = 0x0002FFFF
ROM_SIZE  = 0x20000      # 128 KB (two 64 KB pages)

# MUT table is at a fixed CPU offset from ROM base
MUT_OFFSET  = 0x1FAD0
MUT_ADDR    = ROM_BASE + MUT_OFFSET
MUT_ENTRIES = 256

# RAM range for pointer validation
MEM_ADDR_MIN = 0xEE00
MEM_ADDR_MAX = 0xFEFF

# Header sizes by table type (bytes before data start)
HEADER_SIZE = {"2D": 4, "3D": 7}

# Tracks XML table-overlap regions already warned about, keyed by the
# containing code unit's start address (as a string), so safe_plate only
# reports each distinct overlap region ONCE - not once per table/entry
# that lands inside it. See safe_plate() below.
_warned_overlap_regions = set()

# ──────────────────────────────────────────────────────────────────
# Shared helpers
# ──────────────────────────────────────────────────────────────────

def addr(offset):
    return space.getAddress(offset & 0xFFFFFFFF)

def in_rom(offset):
    return ROM_BASE <= offset <= ROM_END

def in_ram(val):
    return MEM_ADDR_MIN <= val <= MEM_ADDR_MAX

def abort(msg):
    print("ABORT: " + msg)
    raise Exception(msg)

def rom_byte(cpu_offset):
    """Read one byte from ROM at a CPU-page-relative offset."""
    try:
        return flat_api.getByte(addr(ROM_BASE + cpu_offset)) & 0xFF
    except Exception:
        return 0

def rom_word(cpu_offset):
    """Read a big-endian word from ROM at a CPU-page-relative offset."""
    return (rom_byte(cpu_offset) << 8) | rom_byte(cpu_offset + 1)

def rom_bytes_at(ghidra_addr, length):
    """Read 'length' bytes starting at a Ghidra address."""
    result = []
    for i in range(length):
        try:
            result.append(flat_api.getByte(addr(ghidra_addr + i)) & 0xFF)
        except Exception:
            result.append(0)
    return result

def safe_label(a, name, overwrite=False):
    """
    Create label at Ghidra address 'a'.
    Uses getPrimarySymbol() - returns None when no symbol exists.
    Skips silently if label already exists (unless overwrite=True).
    """
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
    """safe_label variant that takes a CPU-page-relative ROM offset."""
    return safe_label(addr(ROM_BASE + cpu_offset), name, overwrite)

def safe_label_ram(ram_val, name):
    """Label a RAM address if it falls in the valid RAM range."""
    if not in_ram(ram_val):
        return
    a = addr(ram_val)
    blk = mem.getBlock(a)
    if blk is None:
        return
    safe_label(a, name)

def safe_plate(a, text, written_set=None):
    """
    Plate comment at Ghidra address 'a'.

    written_set (optional): a set shared across one whole XML-apply run
    (a top-level apply_xml() call plus everything it recurses into via
    <include>). On this address's FIRST touch within that run, any stale
    plate comment left over from a PREVIOUS run is fully overwritten -
    this is what stops re-imports from stacking old and new table text
    forever. On a SECOND+ touch within the SAME run (e.g. two XML tables
    genuinely overlapping the same code unit), behaviour falls back to
    the old append-and-dedupe logic so both entries stay visible for that
    one import.

    If written_set is None (legacy callers, e.g. step 8's scanners),
    behaves exactly as before: append-only, never clobbers.
    """
    try:
        from ghidra.program.model.listing import CodeUnit
        cu = listing.getCodeUnitAt(a)
        overlap = False
        if cu is None:
            cu = listing.getCodeUnitContaining(a)
            if cu is not None:
                # 'a' has no code unit of its own - it fell inside a code
                # unit that starts at a DIFFERENT address, meaning this
                # table's declared address overlaps another table's already-
                # typed data (e.g. a 1D scalar address landing inside a 2D
                # table's header bytes). Flag it rather than silently
                # merging two tables' comments at the wrong address - we
                # can't tell which XML entry is stale/correct from here.
                overlap = True
        if cu is None:
            return
        if overlap:
            region_key = str(cu.getMinAddress())
            if region_key not in _warned_overlap_regions:
                _warned_overlap_regions.add(region_key)
                print("  WARNING: XML table overlap - region starting at %s "
                      "(%d bytes) has entries from more than one XML table "
                      "landing inside it (first seen via address %s). "
                      "Comments still attached, but verify the table "
                      "addresses against the XML/ROM - one is likely stale. "
                      "(further entries in this same region are not "
                      "re-reported individually)" %
                      (cu.getMinAddress(), cu.getLength(), a))
            text = "[OVERLAP - VERIFY ADDRESS] " + text

        key = str(cu.getMinAddress())
        if written_set is not None and key not in written_set:
            # First time this code unit has been written to in this run -
            # replace whatever plate comment is already there (likely
            # stale text from a previous XML import) instead of appending.
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
    """safe_plate variant taking a CPU-page-relative ROM offset."""
    safe_plate(addr(ROM_BASE + cpu_offset), text, written_set)

def safe_bookmark(a, category, text):
    """
    Set/replace a NOTE bookmark at address 'a'. Ghidra's BookmarkManager
    already de-dupes by (address, type, category) internally - calling
    this again on the same address+category just updates the comment
    text in place rather than creating a duplicate, so it's safe to call
    on every re-run without accumulating bookmarks.
    """
    try:
        from ghidra.program.model.listing import BookmarkType
        bookmarkMgr.setBookmark(a, BookmarkType.NOTE, category, text)
    except Exception as e:
        print("  WARNING bookmark @ %s: %s" % (a, e))

def safe_eol(cpu_offset, text):
    """Set EOL comment at a CPU-page-relative ROM offset."""
    try:
        flat_api.setEOLComment(addr(ROM_BASE + cpu_offset), text)
    except Exception:
        pass

def sanitise_name(name):
    """Make a valid Ghidra label from a human-readable table name."""
    result = ""
    for ch in name:
        if ch.isalnum() or ch == '_':
            result += ch
        elif ch in (' ', '-', '/'):
            result += '_'
    while '__' in result:
        result = result.replace('__', '_')
    return result.strip('_')

def type_header_as_data(header_addr, size):
    """
    Type 'size' bytes at header_addr as a byte array, clearing whatever was
    there first. Used to protect XML-declared table headers from
    auto-analysis BEFORE it runs, the same way Step 5b protects
    scraper-detected tables. Safe to call redundantly (idempotent).
    """
    try:
        from ghidra.program.model.data import ByteDataType, ArrayDataType
        end_addr = header_addr.add(size - 1)
        if listing.getInstructionAt(header_addr) is None and \
           listing.getDefinedDataAt(header_addr) is not None and \
           listing.getDefinedDataAt(header_addr).getLength() == size:
            return False   # already correctly typed, nothing to do
        listing.clearCodeUnits(header_addr, end_addr, False)
        listing.createData(header_addr, ArrayDataType(ByteDataType(), size, 1))
        return True
    except Exception as e:
        print("  WARNING: could not type 0x%08X (%d bytes) as data: %s" %
              (header_addr.getOffset(), size, e))
        return False

# ──────────────────────────────────────────────────────────────────
print("=" * 60)
print("H8/539F ECU Master Setup")
print("=" * 60)

# ──────────────────────────────────────────────────────────────────
# STEP 1 — Validate base address (mandatory)
# ──────────────────────────────────────────────────────────────────
if mem.getBlock(addr(ROM_BASE)) is None:
    abort("No memory block at 0x%08X.\n"
          "Re-import ROM as Raw Binary with base address 0x%08X." %
          (ROM_BASE, ROM_BASE))
print("[1] OK: ROM block found at 0x%08X" % ROM_BASE)

# ROM signature banner - helps distinguish ROMs across multi-session notes.
# This is NOT the verified EcuFlash ROM ID (that needs an XML - see Step 7);
# it's just a quick raw-byte fingerprint so you can tell ROMs apart at a glance.
_sig_bytes = rom_bytes_at(ROM_BASE, 16)
_sig_hex   = ' '.join('%02X' % b for b in _sig_bytes)
print("[1] ROM signature (first 16 bytes @ 0x%08X): %s" % (ROM_BASE, _sig_hex))

# ──────────────────────────────────────────────────────────────────
# STEP 2 — On-chip RAM block
# ──────────────────────────────────────────────────────────────────
do_ram = askYesNo("Step 2 - RAM Block",
    "Create on-chip RAM block (0xEE80-0xFFFF)?\n"
    "(Skip if already exists or re-running)")
if do_ram:
    tx = currentProgram.startTransaction("Create RAM block")
    try:
        ram_start = addr(0x0000EE80)
        if mem.getBlock(ram_start) is not None:
            print("[2] OK: RAM block already exists - skipped")
        else:
            blk = mem.createUninitializedBlock("RAM", ram_start, 0x1180, False)
            blk.setRead(True); blk.setWrite(True)
            blk.setExecute(False); blk.setVolatile(True)
            print("[2] OK: Created RAM block 0x0000EE80-0x0000FFFF")
    finally:
        currentProgram.endTransaction(tx, True)
else:
    print("[2] Skipped RAM block creation")

# ──────────────────────────────────────────────────────────────────
# STEP 3 — Context registers
# ──────────────────────────────────────────────────────────────────
do_ctx = askYesNo("Step 3 - Context Registers",
    "Set CP/DP/TP context registers over ROM pages?\n"
    "(Required for correct H8/539F disassembly)")
if do_ctx:
    tx = currentProgram.startTransaction("Set context registers")
    try:
        ctx    = currentProgram.getProgramContext()
        cp_reg = ctx.getRegister("CP")
        dp_reg = ctx.getRegister("DP")
        tp_reg = ctx.getRegister("TP")
        if cp_reg:
            ctx.setValue(cp_reg, addr(0x00010000), addr(0x0001FFFF), BigInteger.valueOf(1))
            ctx.setValue(cp_reg, addr(0x00020000), addr(0x0002FFFF), BigInteger.valueOf(2))
            print("[3] OK: CP=1 over page 1, CP=2 over page 2")
        if dp_reg:
            ctx.setValue(dp_reg, addr(0x00010000), addr(0x0002FFFF), BigInteger.valueOf(1))
            print("[3] OK: DP=1 over full ROM")
        if tp_reg:
            ctx.setValue(tp_reg, addr(0x00010000), addr(0x0002FFFF), BigInteger.valueOf(0))
            print("[3] OK: TP=0 over full ROM")
    finally:
        currentProgram.endTransaction(tx, True)
else:
    print("[3] Skipped context registers")

# ──────────────────────────────────────────────────────────────────
# STEP 4 — Reset vector -> entry function
# ──────────────────────────────────────────────────────────────────
do_entry = askYesNo("Step 4 - Entry Function",
    "Read reset vector and create entry function?\n"
    "(Skip if already done)")
if do_entry:
    tx = currentProgram.startTransaction("Create entry function")
    try:
        cp_word    = flat_api.getShort(addr(ROM_BASE)) & 0xFFFF
        pc_word    = flat_api.getShort(addr(ROM_BASE + 2)) & 0xFFFF
        entry_cp   = cp_word & 0xFF
        entry_flat = (entry_cp << 16) | pc_word
        print("[4] Reset vector: CP=0x%02x PC=0x%04x -> 0x%08x" %
              (entry_cp, pc_word, entry_flat))
        if entry_cp == 0:
            abort("Reset vector CP=0 - ROM is at wrong base address.")
        ep     = addr(entry_flat)
        cp_reg = currentProgram.getProgramContext().getRegister("CP")
        if cp_reg:
            currentProgram.getProgramContext().setValue(
                cp_reg, ep, ep, BigInteger.valueOf(entry_cp))
        flat_api.disassemble(ep)
        flat_api.createFunction(ep, "entry")
        print("[4] OK: entry function created at 0x%08x" % entry_flat)
    finally:
        currentProgram.endTransaction(tx, True)
else:
    print("[4] Skipped entry function")

# ──────────────────────────────────────────────────────────────────
# STEP 4b — Interrupt vector table: mark as data + create ISR functions
#
# The vector table's LOCATION (0x10000-0x1013F) and FORMAT (80 entries,
# each a 4-byte CP:PC pointer, same layout as the reset vector read in
# Step 4) are fixed by the H8/539F hardware itself, so this is safe to
# run on any H8/539F ROM. Only the actual handler ADDRESSES stored in
# the table are specific to this ROM's firmware - those are read live,
# not hardcoded.
#
# This fixes two things identified while analysing the RVR 21000011 ROM:
#   1. The vector table was sitting as undefined bytes instead of data
#      (README known limitation).
#   2. Several interrupt handlers (e.g. the ones ending in `rte`) had no
#      byte-pattern signature reliable enough to find via pattern search
#      (unlike `prts`/`prtd`, which got added to h8539pattern.xml) -
#      their entry addresses are already sitting right there in the
#      vector table, so no searching is needed at all.
# ──────────────────────────────────────────────────────────────────
# ──────────────────────────────────────────────────────────────────
# Step 4a - Register-indirect jump/call site report (jmp @Rn / jsr @Rn)
#
# Report only. These are the classic "computed switch" idiom on this
# compiler: index -> shll.w (word-size table entries) -> mov.w
# @(disp16,Rn),Rn (table load) -> optional sub.w #imm,Rn (bias) ->
# jmp/jsr @Rn. The table base is a REGISTER, not a fixed address, so
# it can't be resolved the way the IVT can - some tables live in RAM
# (populated at runtime, nothing to recover statically) and some may
# be ROM constants. This step just lists every site and prints the
# preceding instructions so each can be reviewed by hand.
# ──────────────────────────────────────────────────────────────────
do_jmp_report = askYesNo("Step 4a - Register-indirect jump/call sites",
    "Scan for jmp @Rn / jsr @Rn (computed switch) sites and print\n"
    "their addresses + preceding instructions to the console?\n"
    "(Report only - does not modify the program)")
if do_jmp_report:
    JMP_JSR_PATTERNS = [
        (chr(0x11) + chr(0xD0 + r), "jmp", r) for r in range(8)
    ] + [
        (chr(0x11) + chr(0xF0 + r), "jsr", r) for r in range(8)
    ]
    listing = currentProgram.getListing()
    hits = []
    for pat, mnem, reg in JMP_JSR_PATTERNS:
        start = currentProgram.getMinAddress()
        while True:
            hit_arr = flat_api.findBytes(start, pat, 1)
            if hit_arr is None or len(hit_arr) == 0:
                break
            hit = hit_arr[0]
            hits.append((hit, mnem, reg))
            try:
                start = hit.add(1)
            except Exception:
                break
    hits.sort(key=lambda h: h[0].getOffset())
    print("=" * 70)
    print("Register-indirect jump/call sites (jmp @Rn / jsr @Rn): %d found" % len(hits))
    print("=" * 70)
    for hit_addr, mnem, reg in hits:
        fn = flat_api.getFunctionContaining(hit_addr)
        fn_name = fn.getName() if fn is not None else "(no function)"
        print("  0x%08X  %s @R%d   in %s" % (hit_addr.getOffset(), mnem, reg, fn_name))
        try:
            ctx_addr = hit_addr
            preceding = []
            for _ in range(6):
                prev = listing.getInstructionBefore(ctx_addr)
                if prev is None:
                    break
                preceding.append(prev)
                ctx_addr = prev.getAddress()
            for instr in reversed(preceding):
                print("      %08X: %s" % (instr.getAddress().getOffset(), instr.toString()))
            target_instr = listing.getInstructionAt(hit_addr)
            if target_instr is not None:
                print("      %08X: %s   <-- indirect jump/call" %
                      (hit_addr.getOffset(), target_instr.toString()))
        except Exception as e:
            print("      (could not print context: %s)" % e)
        print("")
    print("=" * 70)
    print("Review each site by hand. If the table base register is a ROM")
    print("constant (mov #addr,Rn shortly before), the table may be worth")
    print("marking as data manually. If it resolves into RAM, the table is")
    print("populated at runtime and there is nothing static to recover.")
    print("=" * 70)

VECTOR_TABLE_BASE    = 0x00010000
VECTOR_TABLE_ENTRIES = 80          # 0x140 bytes / 4 bytes per entry
VECTOR_ENTRY_SIZE    = 4

do_ivt = askYesNo("Step 4b - Interrupt Vector Table",
    "Mark the interrupt vector table (0x10000-0x1013F) as data and\n"
    "create functions at every unique handler address it points to?\n"
    "(Skip if already done)")
if do_ivt:
    tx = currentProgram.startTransaction("Interrupt vector table")
    try:
        from ghidra.program.model.data import DWordDataType
        seen        = {}
        created     = 0
        target_idxs = {}   # target -> list of vector indices pointing at it
        for idx in range(VECTOR_TABLE_ENTRIES):
            off     = idx * VECTOR_ENTRY_SIZE
            cp_word = flat_api.getShort(addr(VECTOR_TABLE_BASE + off)) & 0xFFFF
            pc_word = flat_api.getShort(addr(VECTOR_TABLE_BASE + off + 2)) & 0xFFFF
            v_cp    = cp_word & 0xFF
            target  = (v_cp << 16) | pc_word
            print("  IVT_%02d @ 0x%08X -> target 0x%08X" %
                  (idx, VECTOR_TABLE_BASE + off, target))

            # Mark this 4-byte slot as data so it stops showing as undefined
            try:
                v_addr = addr(VECTOR_TABLE_BASE + off)
                if listing.getInstructionAt(v_addr) is not None or \
                   listing.getDefinedDataAt(v_addr) is None:
                    listing.clearCodeUnits(v_addr, addr(VECTOR_TABLE_BASE + off + 3), False)
                    listing.createData(v_addr, DWordDataType())
            except Exception as e:
                print("  WARNING: could not mark vector slot %d as data: %s" % (idx, e))
            safe_label_offset(off, "IVT_%02d" % idx)
            safe_plate_offset(off,
                "Interrupt Vector Table Entry\n"
                "Index  : %d\n"
                "Target : 0x%08X" % (idx, target))

            if target == 0:
                continue    # unpopulated entry
            if not in_rom(target):
                print("  WARNING: IVT_%02d target 0x%08X is outside ROM "
                      "(0x%08X-0x%08X) - skipped" % (idx, target, ROM_BASE, ROM_END))
                continue
            target_idxs.setdefault(target, []).append(idx)
            if target in seen:
                continue    # already created a function for this handler
            seen[target] = idx

            t_addr = addr(target)
            if flat_api.getFunctionAt(t_addr) is not None:
                continue    # already a function (e.g. reset vector / entry)
            containing_fn = flat_api.getFunctionContaining(t_addr)
            if containing_fn is not None:
                # Target is INSIDE an existing function's body (e.g. a shared
                # fallthrough/branch target reused as a vector target) - do
                # NOT create a new function here, that would split/fragment
                # the real one and can produce duplicate-named functions.
                print("  NOTE: IVT_%02d target 0x%08X is inside existing "
                      "function %s (0x%08X) - not creating a separate "
                      "function, this is a shared branch target" %
                      (idx, target, containing_fn.getName(),
                       containing_fn.getEntryPoint().getOffset()))
                continue

            try:
                cp_reg = currentProgram.getProgramContext().getRegister("CP")
                if cp_reg:
                    currentProgram.getProgramContext().setValue(
                        cp_reg, t_addr, t_addr, BigInteger.valueOf(v_cp))
                flat_api.disassemble(t_addr)
                fn = flat_api.createFunction(t_addr, None)
                if fn is not None:
                    body_len = fn.getBody().getNumAddresses()
                    if body_len < 2:
                        # Degenerate function - target wasn't a real
                        # instruction boundary (e.g. off-by-one into
                        # another function/table). Undo it rather than
                        # leave a garbage 1-byte function behind.
                        print("  WARNING: IVT_%02d target 0x%08X produced a "
                              "degenerate %d-byte function - removing "
                              "(likely not a real handler entry)" %
                              (idx, target, body_len))
                        flat_api.removeFunctionAt(t_addr)
                    else:
                        created += 1
                        print("    -> created function at 0x%08X (%s)" %
                              (target, fn.getName()))
            except Exception as e:
                print("  WARNING: could not create function at 0x%08X (vector %d): %s" %
                      (target, idx, e))
        # ── Conflict check: which target is the "shared default stub"? ──
        # Heuristic: whichever target has the most vector indices pointing
        # at it is almost certainly the shared default/unhandled-interrupt
        # stub. Any OTHER target hit by 2+ distinct indices is unexpected
        # and worth a human look.
        if target_idxs:
            default_target = max(target_idxs, key=lambda t: len(target_idxs[t]))
        else:
            default_target = None
        for target, idxs in target_idxs.items():
            if target != default_target and len(idxs) > 1:
                print("  WARNING: IVT entries %s all point to 0x%08X - "
                      "duplicate target that is NOT the shared default stub "
                      "(0x%08X, hit by %d entries)" %
                      (idxs, target, default_target, len(target_idxs[default_target])))

        # ── ISR termination check ──
        # Every unique handler this table points to should end in a return
        # or halt instruction. Flag anything that doesn't - it usually means
        # the function boundary was mis-detected (fell through into the next
        # handler) or the ROM does something unusual worth a manual look.
        TERMINATORS = ("rte", "sleep", "rts", "prts", "prtd")
        bad_term = 0
        for target, idx in seen.items():
            t_addr = addr(target)
            fn = flat_api.getFunctionAt(t_addr)
            if fn is None:
                continue    # createFunction failed earlier - already warned
            last_instr = None
            for ins in listing.getInstructions(fn.getBody(), True):
                last_instr = ins
            if last_instr is None:
                print("  WARNING: ISR at 0x%08X (vector %d) has no instructions "
                      "in its body - check disassembly" % (target, idx))
                bad_term += 1
                continue
            mnem = last_instr.getMnemonicString().lower()
            if mnem not in TERMINATORS:
                print("  WARNING: ISR at 0x%08X (vector %d) ends in '%s', not a "
                      "recognised terminator %s" % (target, idx, mnem, TERMINATORS))
                bad_term += 1
        if bad_term:
            print("[4b] %d handler(s) flagged for non-standard termination - see warnings above" %
                  bad_term)

        print("[4b] OK: %d vector slots labeled, %d unique handler functions created" %
              (VECTOR_TABLE_ENTRIES, created))
    finally:
        currentProgram.endTransaction(tx, True)
else:
    print("[4b] Skipped interrupt vector table")

# ──────────────────────────────────────────────────────────────────
# STEP 4c — prts/rts function-start scan (report only)
#
# Automated version of the README's manual "Search Memory for 11 19 / 19"
# technique. This is a SAFE fallback/backup for h8539pattern.xml (which
# should catch most of these during auto-analysis) - it only PRINTS
# candidate addresses rather than creating functions, since a byte scan
# can't tell a real function boundary from a coincidental match inside
# data (e.g. the vector table/padding region already flagged in
# h8539pattern.xml's own comments).
# ──────────────────────────────────────────────────────────────────
do_prts_scan = askYesNo("Step 4c - prts/rts Function-Start Scan",
    "Scan the ROM for 'prts' (0x11 0x19) and 'rts' (0x19) bytes not\n"
    "already followed by a defined function, and print a report of\n"
    "candidate function-start addresses?\n\n"
    "Report only - does not create functions or modify the program.\n"
    "(Run AFTER Step 6 auto-analysis for the most useful results.)")
if do_prts_scan:
    print("[4c] Scanning ROM for prts/rts boundaries...")
    candidates = []
    i = 0
    while i < ROM_SIZE - 1:
        b0 = rom_byte(i)
        b1 = rom_byte(i + 1)
        if b0 == 0x11 and b1 == 0x19:
            next_off = i + 2      # prts, 2 bytes
        elif b0 == 0x19:
            next_off = i + 1      # rts, 1 byte
        else:
            next_off = None
        if next_off is not None and next_off < ROM_SIZE:
            next_addr = addr(ROM_BASE + next_off)
            if (flat_api.getFunctionAt(next_addr) is None and
                    listing.getInstructionAt(next_addr) is None):
                candidates.append(ROM_BASE + next_off)
        i += 1
    if candidates:
        print("[4c] %d candidate function-start address(es) not currently a "
              "function or disassembled instruction:" % len(candidates))
        for c in candidates[:200]:
            print("       0x%08X" % c)
        if len(candidates) > 200:
            print("       ... and %d more (truncated)" % (len(candidates) - 200))
        print("[4c] Review these manually before creating functions - some may be "
              "data (e.g. inside the vector table/padding region) rather than code.")
    else:
        print("[4c] OK: no un-analysed prts/rts boundaries found")
else:
    print("[4c] Skipped prts/rts scan")

# ──────────────────────────────────────────────────────────────────
# STEP 5 — Decompiler health check (mandatory, gates Step 6)
# ──────────────────────────────────────────────────────────────────
print("")
print("[5] Checking decompiler...")
ifc  = DecompInterface()
opts = DecompileOptions()
opts.grabFromProgram(currentProgram)
ifc.setOptions(opts)
ok = ifc.openProgram(currentProgram)
ifc.dispose()
if not ok:
    abort("Decompiler openProgram() FAILED.\n"
          "Check h8.cspec, do a full Ghidra restart, delete and re-import the ROM.")
print("[5] OK: Decompiler is healthy")

# ──────────────────────────────────────────────────────────────────
# STEP 5b — Pre-analysis MUT table protection
#
# ROOT CAUSE this originally fixed: on an earlier run, un-typed table
# bytes let Ghidra's auto-analysis decode straight through them as
# instructions; once it decoded garbage inside a table and kept falling
# through, flow-following glued huge, unrelated stretches of the ROM
# into one runaway function (observed: a single function body spanning
# 0x20640-0x2FFFF after auto-analysis).
#
# Only the fixed-offset MUT table is typed here now (a known
# ROM-structure constant, not XML/scraper-derived) -- see the comment
# further down in this function for why the general scaling/2D/3D
# heuristic pass was deliberately removed from this step. XML- and
# scraper-detected tables are handled entirely by the separate
# h8539_import_tables_xml.py script, which runs AFTER this script's
# Step 6 auto-analysis (see this file's header comment for the full
# pipeline rationale) -- a live gap-scan showed auto-analysis doesn't
# need pre-typing to stay out of most table regions on this ROM, so
# this step is cheap, always-safe insurance for the one region
# (MUT) where a concrete incident already showed it was needed, not a
# general-purpose protection pass anymore.
# ──────────────────────────────────────────────────────────────────

def protect_data_regions_early(xml_touched=None):
    """
    xml_touched: unused now that XML import runs in a separate,
    post-analysis script -- kept as an accepted (but ignored-in-practice)
    parameter so the Step 5b call site below doesn't need updating.
    Historical docstring: set/list of CPU offsets already typed+verified
    by the old in-script XML import step
    (apply_xml's touched_offsets, i.e. only VERIFIED or NOTE-outside-ROM
    entries -- REJECTED tables are never added there, so this pass can
    still independently discover a real table sitting under a REJECTED
    XML entry's neighborhood). Skipping these avoids re-scanning/racing
    against regions the XML step already typed.
    """
    xml_touched = set(xml_touched) if xml_touched else set()
    from ghidra.program.model.data import ByteDataType, WordDataType, ArrayDataType

    def mark_range_as_data(cpu_offset, length, elem_type, elem_size):
        try:
            a     = addr(ROM_BASE + cpu_offset)
            end_a = addr(ROM_BASE + cpu_offset + length - 1)
            listing.clearCodeUnits(a, end_a, False)
            count = length // elem_size
            if count <= 0:
                return False
            arr = ArrayDataType(elem_type, count, elem_size)
            listing.createData(a, arr)
            print("  DATA 0x%08X - 0x%08X  (%d bytes, %s x%d)" %
                  (ROM_BASE + cpu_offset, ROM_BASE + cpu_offset + length - 1,
                   length, elem_type.getName(), count))
            return True
        except Exception as e:
            print("  WARNING: could not mark 0x%08X-0x%08X as data: %s" %
                  (ROM_BASE + cpu_offset, ROM_BASE + cpu_offset + length - 1, e))
            return False

    def in_range(val, lo, hi):
        return lo <= val <= hi

    def find_sentinel_data_len(start, max_len=0x1FF):
        """
        Sentinel-based length scan (same logic used by the scraper in
        h8539_import_tables_xml.py). Not actually called by this function
        for anything anymore now that only the fixed-offset MUT table is
        marked here (see the comment above 'marked = 0' below) -- kept in
        case a future targeted fix needs it, so this scope doesn't have to
        reintroduce it from scratch.
        """
        for j in range(max_len):
            if rom_byte(start + j) == 0xFF:
                return j + 1
        return None

    marked = 0

    # ── MUT table: fixed offset, always safe to mark upfront ──
    if mark_range_as_data(MUT_OFFSET, MUT_ENTRIES * 2, WordDataType(), 2):
        marked += 1

    # Scaling/2D/3D tables are deliberately NOT typed as data here. This
    # used to run the same byte-pattern heuristic the scraper uses and
    # commit to a guessed length/boundary before anything corroborated
    # it -- but that guess could differ from what the scraper's own
    # is_likely_code() computes later (it sees more once auto-analysis
    # has actually run), so the two passes could re-type the same bytes
    # with different boundaries and produce duplicate/conflicting plate
    # comments (e.g. 0x12844). All XML- and scraper-driven table typing
    # now happens entirely in h8539_import_tables_xml.py, which runs
    # AFTER this script's Step 6 auto-analysis -- see this file's header
    # comment for the full rationale. Only the fixed-offset MUT table is
    # typed here, pre-analysis.
    #
    # Tradeoff: table regions no longer get PRE-analysis protection from
    # this step specifically (the runaway-function merge bug this step
    # was originally added to prevent). A live gap-scan on a fresh,
    # un-annotated ROM showed auto-analysis doesn't actually wander into
    # most table regions on its own here, so this tradeoff has held up in
    # practice so far -- but if the runaway-merge symptom shows up again,
    # re-check the affected function boundaries after Step 6 rather than
    # re-adding a heuristic-driven typing pass to this step.

    return marked


do_protect = askYesNo("Step 5b - Pre-Analysis MUT Table Protection",
    "Type the fixed-offset MUT table region as DATA before auto-analysis runs?\n\n"
    "STRONGLY RECOMMENDED. Without this, auto-analysis has no way to know\n"
    "this table exists and can decode straight through it as code,\n"
    "sometimes merging huge unrelated stretches of the ROM into a single\n"
    "runaway function (a concrete incident of this has been observed on\n"
    "this ROM before).\n\n"
    "This only types bytes as data - labels/plate comments for the MUT\n"
    "table, and all XML/scraper table detection, happen properly later\n"
    "in h8539_import_tables_xml.py.")
if do_protect:
    tx = currentProgram.startTransaction("Pre-analysis MUT table protection")
    try:
        n_marked = protect_data_regions_early()
        print("[5b] OK: %d table region(s) typed as data before analysis" % n_marked)
    finally:
        currentProgram.endTransaction(tx, True)
else:
    print("[5b] Skipped pre-analysis data protection - auto-analysis may decode into tables")

# ──────────────────────────────────────────────────────────────────
# STEP 6 — Auto-analysis
# ──────────────────────────────────────────────────────────────────
do_analysis = askYesNo("Step 6 - Auto-Analysis",
    "Run auto-analysis now?\n"
    "(Can take 1-2 minutes. Skip if already analysed.)")
if do_analysis:
    print("[6] Running auto-analysis...")
    fc_before = currentProgram.getFunctionManager().getFunctionCount()

    # analyzeAll() must not be invoked from the Swing/EDT thread
    # (AutoAnalysisManager.scheduleWorker throws UnsupportedOperationException
    # if it is), so push the actual call onto a plain background thread.
    import threading
    _analysis_error = [None]
    def _run_analysis():
        try:
            flat_api.analyzeAll(currentProgram)
        except Exception as e:
            _analysis_error[0] = e
    _t = threading.Thread(target=_run_analysis)
    _t.start()
    _t.join()
    if _analysis_error[0] is not None:
        abort("Auto-analysis failed: %s" % _analysis_error[0])

    fc_after = currentProgram.getFunctionManager().getFunctionCount()
    print("[6] OK: Auto-analysis complete - %d functions found (%+d new since before this step)" %
          (fc_after, fc_after - fc_before))
else:
    print("[6] Skipped auto-analysis")

# ──────────────────────────────────────────────────────────────────
# STEP 6b — Post-analysis gap report
#
# Scans the whole ROM for contiguous runs of undefined bytes and prints
# them, so gaps left by auto-analysis (and not covered by Step 4c's
# candidates) are visible immediately instead of requiring a manual hunt.
# Report only - byte-by-byte over 128KB, so it can take a little while.
# ──────────────────────────────────────────────────────────────────
GAP_MIN_LENGTH = 16   # ignore runs shorter than this (alignment padding etc.)

do_gap_report = askYesNo("Step 6b - Post-Analysis Gap Report",
    "Scan the ROM for contiguous undefined-byte regions (>= %d bytes)\n"
    "left after auto-analysis and print them?\n\n"
    "Report only - does not modify the program. Scans the full 128KB\n"
    "ROM byte-by-byte, so it can take a little while." % GAP_MIN_LENGTH)
if do_gap_report:
    print("[6b] Scanning for undefined-byte gaps...")
    gaps = []
    run_start = None
    for off in range(ROM_SIZE):
        a = addr(ROM_BASE + off)
        is_undef = listing.getUndefinedDataAt(a) is not None
        if is_undef:
            if run_start is None:
                run_start = off
        else:
            if run_start is not None:
                length = off - run_start
                if length >= GAP_MIN_LENGTH:
                    gaps.append((run_start, length))
                run_start = None
    if run_start is not None:
        length = ROM_SIZE - run_start
        if length >= GAP_MIN_LENGTH:
            gaps.append((run_start, length))

    if gaps:
        print("[6b] %d undefined-byte gap(s) >= %d bytes found:" %
              (len(gaps), GAP_MIN_LENGTH))
        for start_off, length in gaps[:200]:
            print("       0x%08X - 0x%08X  (%d bytes)" %
                  (ROM_BASE + start_off, ROM_BASE + start_off + length - 1, length))
        if len(gaps) > 200:
            print("       ... and %d more (truncated)" % (len(gaps) - 200))
    else:
        print("[6b] OK: no undefined-byte gaps >= %d bytes found" % GAP_MIN_LENGTH)
else:
    print("[6b] Skipped gap report")

# ──────────────────────────────────────────────────────────────────
# STEP 6c — Post-analysis error-bookmark triage (universal, any ROM)
#
# Complements Step 5b: 5b prevents auto-analysis from decoding into the
# THREE KNOWN table byte-signatures (scaling/2D/3D), and only scans page 1
# (0x0-MUT_OFFSET). This step runs AFTER analysis and classifies whatever
# "Bad Instruction" bookmarks are left -- including page 2, and including
# table formats that don't match any of the three known signatures --
# using only generic Ghidra signals (xref types, function ownership,
# repeated-byte runs), so nothing here is specific to this ROM or to the
# three known EcuFlash table shapes.
#
# For each error bookmark:
#   1. Repeated-byte run (unprogrammed flash padding)    -> mark as data
#   2. Flow target outside defined memory                 -> flag, don't chase
#   3. No real flow (call/jump/branch) xref, and either
#      a data (read/write) xref or no owning function     -> mark as data
#   4. Otherwise (real flow context, still won't decode)  -> flag for manual/
#      Claude review; NOTHING is auto-written for this case, since it may
#      be a genuine SLEIGH grammar gap (verify against ana.cpp before
#      writing a constructor).
# ──────────────────────────────────────────────────────────────────
REPEAT_RUN_THRESHOLD = 8   # bytes

def is_repeated_byte_run(start_addr, min_run):
    try:
        first = flat_api.getByte(start_addr) & 0xFF
        count = 1
        a = start_addr.add(1)
        while count < min_run + 8 and mem.contains(a) and (flat_api.getByte(a) & 0xFF) == first:
            count += 1
            a = a.add(1)
        return count >= min_run
    except Exception:
        return False

def has_flow_ref_at(a):
    refMgr = currentProgram.getReferenceManager()
    for r in refMgr.getReferencesTo(a):
        if r.getReferenceType().isFlow():
            return True
    return False

def has_data_ref_at(a):
    from ghidra.program.model.symbol import RefType
    refMgr = currentProgram.getReferenceManager()
    for r in refMgr.getReferencesTo(a):
        rt = r.getReferenceType()
        if rt.isData() or rt == RefType.READ or rt == RefType.WRITE or rt == RefType.READ_WRITE:
            return True
    return False

def triage_disassembly_errors():
    from ghidra.program.model.listing import BookmarkType
    bm = currentProgram.getBookmarkManager()
    error_addrs = [b.getAddress() for b in bm.getBookmarksIterator(BookmarkType.ERROR)]
    print("[6c] Found %d disassembly-error bookmark(s)." % len(error_addrs))

    padding_count = 0
    oob_count     = 0
    data_count    = 0
    gap_count     = 0

    for a in error_addrs:
        if is_repeated_byte_run(a, REPEAT_RUN_THRESHOLD):
            try:
                listing.clearCodeUnits(a, a, False)
            except Exception:
                pass
            bm.setBookmark(a, BookmarkType.NOTE, "H8539F-AUTO-DATA",
                "AUTO: repeated-byte padding run")
            padding_count += 1
            print("  0x%08X -> padding (repeated-byte run)" % a.getOffset())
            continue

        if not mem.contains(a):
            bm.setBookmark(a, BookmarkType.NOTE, "H8539F-OOB",
                "AUTO: flow target outside defined memory - not chased")
            oob_count += 1
            print("  0x%08X -> out-of-bounds flow target" % a.getOffset())
            continue

        has_flow = has_flow_ref_at(a)
        if not has_flow:
            prev_instr = listing.getInstructionBefore(a)
            if prev_instr is not None and has_flow_ref_at(prev_instr.getMinAddress()):
                has_flow = True

        has_data = has_data_ref_at(a)
        owner    = listing.getFunctionContaining(a)

        if not has_flow and (has_data or owner is None):
            try:
                listing.clearCodeUnits(a, a, False)
            except Exception:
                pass
            bm.setBookmark(a, BookmarkType.NOTE, "H8539F-AUTO-DATA",
                "AUTO: no flow xrefs, data-pattern (dataRef=%s, owner=%s)" %
                (has_data, owner is not None))
            data_count += 1
            print("  0x%08X -> data pattern (dataRef=%s, owner=%s)" %
                  (a.getOffset(), has_data, owner is not None))
            continue

        bm.setBookmark(a, BookmarkType.NOTE, "H8539F-GAP",
            "REVIEW: real flow context but constructor failed to resolve - "
            "check against ana.cpp before writing a fix")
        gap_count += 1
        print("  0x%08X -> GENUINE GAP - needs manual/Claude review" % a.getOffset())

    print("[6c] Padding runs reclassified as data: %d" % padding_count)
    print("[6c] Out-of-bounds flow targets flagged: %d" % oob_count)
    print("[6c] Data-pattern regions reclassified:  %d" % data_count)
    print("[6c] Genuine grammar-gap candidates:     %d" % gap_count)
    print("[6c] Gap candidates bookmarked 'H8539F-GAP' for manual review - "
          "nothing auto-written for these.")
    return padding_count, oob_count, data_count, gap_count


do_triage = askYesNo("Step 6c - Disassembly Error Triage",
    "Classify every disassembly-error bookmark left after auto-analysis?\n\n"
    "Universal, xref-based triage (NOT limited to the three known EcuFlash\n"
    "table byte-signatures Step 5b/8 look for, and covers page 2 too):\n"
    "  - repeated-byte padding runs          -> marked as data\n"
    "  - flow that walked off defined memory -> flagged, not chased\n"
    "  - regions with only read/write xrefs\n"
    "    and no owning function              -> marked as data\n"
    "  - everything else (real flow context,\n"
    "    still won't decode)                 -> bookmarked 'H8539F-GAP'\n"
    "    for manual/Claude review - nothing is auto-written for these,\n"
    "    since they may be genuine grammar gaps.\n\n"
    "Recommended to re-run auto-analysis afterward if any regions were\n"
    "reclassified as data.")
if do_triage:
    tx = currentProgram.startTransaction("Disassembly error triage")
    try:
        triage_disassembly_errors()
    finally:
        currentProgram.endTransaction(tx, True)
else:
    print("[6c] Skipped disassembly error triage")

print("")
print("=" * 60)
print("H8/539F Master Setup (ROM + auto-analysis) complete!")
print("Next: run h8539_import_tables_xml.py to apply EcuFlash XML labels")
print("and the ROM scraper against this now-analyzed program.")
print("=" * 60)

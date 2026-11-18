# H8/539F ECU - Table Export (EcuFlash-style XML)
# @category MitsubishiECU
#
# Exports every table currently labeled in THIS Ghidra project - whether it
# came from an XML import (Step 5c, bookmarked "H8539F-TABLE") or the ROM
# scraper (Step 8, symbols named TABLE_2D_*/TABLE_3D_*/SCALING_TABLE_*) -
# into a single EcuFlash-style <rom> XML definition file.
#
# SOURCE OF TRUTH: whatever plate comment/bookmark is attached in Ghidra
# RIGHT NOW - not a fresh XML re-parse and not a fresh scraper pass. That
# means this reflects any manual corrections you've made since the last
# import/scrape (renamed tables, fixed/repaired addresses, deleted bad
# entries, etc). If you want it to reflect a NEW xml/scrape, re-run Step 5c
# / Step 8 first, then run this export.
#
# XML-imported tables are read back from apply_xml's own plate-comment
# format (Step 5c in h8539_ecu_master_setup_new.py):
#   EcuFlash Table : <name>
#   Category       : <category>
#   Type           : <2D/3D/1D>
#   Scaling        : <scaling>
#   Swap XY        : <...>
#   Flip Y         : <...>
#   Level          : <...>
#   Header address : 0x..... (2D/3D only)
#   Data address   : 0x..... (2D/3D only)
#   Address        : 0x..... (1D scalar / unrecognised type only)
#
# Scraper-found tables (no EcuFlash category/scaling metadata - the scraper
# only knows the raw header shape) are read back from run_rom_scraper's
# plate-comment format:
#   ROM Scraper: 2D/3D Value Table
#   Header  : N bytes
#   Data    : NNN bytes
#   Axis    : RAM:0xXXXX          (2D)
#   X axis  : RAM:0xXXXX          (3D)
#   Y axis  : RAM:0xXXXX          (3D)
#   Data @  : 0xXXXXXXXX
# or (scaling tables):
#   Scaling Table
#   Out ptr : RAM:0xXXXX
#   In ptr  : RAM:0xXXXX
#   Entries : N (NN bytes)
#
# KNOWN LIMITATION: scraper-derived entries only carry the axis RAM
# *pointer*, not its element count/scaling name (EcuFlash needs both to
# render an axis properly) - those aren't present in Ghidra unless a
# matching axis was itself imported from an XML. Scraper-only tables are
# exported with a best-effort axis stub and an inline comment flagging
# this, rather than guessed/fabricated values.

import re
from ghidra.program.model.listing import CodeUnit, BookmarkType

listing  = currentProgram.getListing()
symTable = currentProgram.getSymbolTable()
bm       = currentProgram.getBookmarkManager()


def plate_at(a):
    cu = listing.getCodeUnitAt(a)
    if cu is None:
        return None
    return cu.getComment(CodeUnit.PLATE_COMMENT)


def parse_kv_block(text):
    """
    Parses lines shaped 'Key            : Value' (as written by both
    apply_xml's meta[] and run_rom_scraper's plate text) into a dict.
    Keys are lower-cased and whitespace-collapsed for lookup convenience.
    """
    out = {}
    if not text:
        return out
    for line in text.split("\n"):
        m = re.match(r"\s*([A-Za-z0-9 /]+?)\s*:\s*(.*)", line)
        if m:
            key = re.sub(r"\s+", " ", m.group(1)).strip().lower()
            out[key] = m.group(2).strip()
    return out


def hexint(s):
    """Pulls the first 0x.... hex token (optionally after 'RAM:') out of a string."""
    if not s:
        return None
    m = re.search(r"0x([0-9A-Fa-f]+)", s)
    if m:
        return int(m.group(1), 16)
    return None


def esc(s):
    if s is None:
        return ""
    return (str(s).replace("&", "&amp;").replace("<", "&lt;")
                  .replace(">", "&gt;").replace('"', "&quot;"))


# ────────────────────────────────────────────────────────────────
# 1) Tables that came from an XML import - bookmarked H8539F-TABLE
# ────────────────────────────────────────────────────────────────
xml_tables = []
seen_addrs = set()

it = bm.getBookmarksIterator(BookmarkType.NOTE)
while it.hasNext():
    b = it.next()
    if b.getCategory() != "H8539F-TABLE":
        continue
    a = b.getAddress()
    kv = parse_kv_block(plate_at(a))
    if "ecuflash table" not in kv:
        # Bookmark exists but the plate comment it should be paired with is
        # missing/unparsed - skip rather than emit a table with no metadata.
        continue

    data_addr   = hexint(kv.get("data address"))
    header_addr = hexint(kv.get("header address"))
    scalar_addr = hexint(kv.get("address"))
    final_addr  = data_addr if data_addr is not None else \
                  (scalar_addr if scalar_addr is not None else header_addr)

    xml_tables.append({
        "source":         "xml",
        "name":           kv.get("ecuflash table", ""),
        "category":       kv.get("category", ""),
        "type":           kv.get("type", ""),
        "scaling":        kv.get("scaling", ""),
        "swapxy":         kv.get("swap xy", ""),
        "flipy":          kv.get("flip y", ""),
        "level":          kv.get("level", ""),
        "address":        final_addr,
        "header_address": header_addr,
    })
    for x in (data_addr, header_addr, scalar_addr):
        if x is not None:
            seen_addrs.add(x)

# ────────────────────────────────────────────────────────────────
# 2) Tables the scraper found - bookmarked H8539F-SCRAPED-TABLE
# ────────────────────────────────────────────────────────────────
scraped_tables = []

it2 = bm.getBookmarksIterator(BookmarkType.NOTE)
while it2.hasNext():
    b = it2.next()
    if b.getCategory() != "H8539F-SCRAPED-TABLE":
        continue
    a = b.getAddress()
    header_addr = a.getOffset()
    if header_addr in seen_addrs:
        continue   # already exported as an XML table at this address

    bm_text = b.getComment() or ""
    kind = "2D"
    if "(3D)" in bm_text:
        kind = "3D"
    elif "(Scaling)" in bm_text:
        kind = "Scaling"
    nm = bm_text.split(" (")[0] if " (" in bm_text else bm_text

    kv = parse_kv_block(plate_at(a))
    entry = {
        "source":         "scraper",
        "name":           nm,
        "category":       "",
        "type":           kind,
        "scaling":        "",
        "swapxy":         "",
        "flipy":          "",
        "level":          "",
        "address":        hexint(kv.get("data @")) if "data @" in kv else header_addr,
        "header_address": header_addr,
    }
    if kind == "2D":
        entry["axis"] = kv.get("axis", "")
    elif kind == "3D":
        entry["x_axis"] = kv.get("x axis", "")
        entry["y_axis"] = kv.get("y axis", "")
    elif kind == "Scaling":
        entry["out_ptr"] = kv.get("out ptr", "")
        entry["in_ptr"]  = kv.get("in ptr", "")
    scraped_tables.append(entry)

# ────────────────────────────────────────────────────────────────
# 3) Write EcuFlash-style XML
# ────────────────────────────────────────────────────────────────
lines = []
lines.append('<?xml version="1.0" encoding="UTF-8"?>')
lines.append('<rom>')
lines.append('  <!-- Exported from Ghidra project: %s -->' % esc(currentProgram.getName()))
lines.append('  <!-- Source: currently-labeled tables (XML-imported + scraper-found) -->')

all_tables = xml_tables + scraped_tables
all_tables = [t for t in all_tables if t["address"] is not None]
all_tables.sort(key=lambda t: t["address"])

for t in all_tables:
    attrs = ['name="%s"' % esc(t["name"]), 'address="%X"' % t["address"]]
    if t["type"]:     attrs.append('type="%s"' % esc(t["type"]))
    if t["category"]: attrs.append('category="%s"' % esc(t["category"]))
    if t["scaling"]:  attrs.append('scaling="%s"' % esc(t["scaling"]))
    if t["swapxy"]:   attrs.append('swapxy="%s"' % esc(t["swapxy"]))
    if t["flipy"]:    attrs.append('flipy="%s"' % esc(t["flipy"]))
    if t["level"]:    attrs.append('level="%s"' % esc(t["level"]))

    if t["source"] == "scraper":
        lines.append('  <table %s>' % " ".join(attrs))
        lines.append('    <!-- scraper-derived: no EcuFlash category/scaling metadata available -->')
        if t["type"] == "2D" and t.get("axis"):
            lines.append('    <table type="X Axis" address="%s" />' % esc(t["axis"]))
        elif t["type"] == "3D":
            if t.get("x_axis"):
                lines.append('    <table type="X Axis" address="%s" />' % esc(t["x_axis"]))
            if t.get("y_axis"):
                lines.append('    <table type="Y Axis" address="%s" />' % esc(t["y_axis"]))
        elif t["type"] == "Scaling":
            if t.get("out_ptr"):
                lines.append('    <!-- Out ptr: %s -->' % esc(t["out_ptr"]))
            if t.get("in_ptr"):
                lines.append('    <!-- In ptr: %s -->' % esc(t["in_ptr"]))
        lines.append('  </table>')
    else:
        lines.append('  <table %s />' % " ".join(attrs))

lines.append('</rom>')

out_file = askFile("Save exported table XML", "Save")
if out_file is not None:
    path = out_file.getAbsolutePath()
    if not path.lower().endswith(".xml"):
        path += ".xml"
    f = open(path, "w")
    try:
        f.write("\n".join(lines))
    finally:
        f.close()
    print("[export] Wrote %d tables (%d from XML, %d from scraper) to %s" %
          (len(all_tables), len(xml_tables), len(scraped_tables), path))
else:
    print("[export] Cancelled - no file selected")

# review11.md — h8539_import_tables_xml2.py: current behavior + implementation plan

Date: 2026-08-12
Script: `h8539_import_tables_xml2.py` (2825 lines)
Trigger for this review: "High Octane Ignition Map 1" (RVR 1998 x3 4g63t, header @0x11de8 /
axis-ptr block @0x11deb) — plate comment lands on the axis-pointer block instead of the true
3-byte type-marker start, because `header_shape_ok()` doesn't recognize this ROM's actual
3D header shape. Confirmed against an independent Evo5 ROM scraper dump showing the same
7-byte layout (`03 14 F0` + `C4 F0 C6 13`).

---

## 1. What the script currently does (as-is, before any fix)

### 1.1 Purpose and place in the pipeline
- Standalone Ghidra script for the Mitsubishi H8/539F ECU family.
- Split out of `h8539_ecu_master_setup_new3.py` (2026-07-29) so table annotation is its own pass.
- **Run order:** `h8539_ecu_master_setup_new3.py` (base setup + auto-analysis) → this script →
  optionally `h8539_export_tables_xml.py` (write labels back out to XML).
- Deliberately runs **after** auto-analysis, not before: `is_likely_code()` needs real,
  settled xrefs/instructions to reliably distinguish "this address is code" from "this address
  is a table header," and that evidence is only complete post-analysis.
- Required ROM import settings: Raw Binary, `H8:BE:32:H8539F`, base address `0x00010000`.

### 1.2 Documented addressing convention (as currently coded)
```
EcuFlash XML "address" = first DATA byte of the table.
  2D table header = 4 bytes  -> header at xml_addr - 4
  3D table header = 7 bytes  -> header at xml_addr - 7
  1D scalar       = no header -> labeled at xml_addr directly
Code xrefs point at the header address - that is where we label.
```
This is stated as a fixed, single convention in the file's top comment block, but the actual
code (`resolve_header_address`, §1.6) already knows this isn't universally true and tries two
conventions per table.

### 1.3 Step 5c — XML table labelling (main pass)
Driven by `apply_xml(file_path, ...)`:
- Prompts for an XML file via file browser.
- **ROM ID verification first** (`verify_rom_id`): reads `<internalidaddress>` +
  `<internalidhex>` from the XML and compares against live ROM bytes. On mismatch, prints a
  warning and skips that file entirely — does not abort the whole script. Checked once per
  top-level file via a shared `id_verified` flag across the `<include>` chain.
- **Address resolution** (`resolve_header_address`, §1.6) picks between "address = header"
  and "address = data, header is `hdr_size` bytes earlier" per table, using
  `header_shape_ok()` as the tie-breaker.
- **Per-table verification** (`verify_xml_table`, §1.7) returns one of
  `VERIFIED / CHECK / SUSPECT / REJECTED / NO_TABLE`.
  - Only `VERIFIED` tables are labeled **and** typed as data
    (`type_header_as_data`, §1.9).
  - `CHECK` and `SUSPECT` are report-only (bookmarked, never silently typed or discarded).
  - `REJECTED` / `NO_TABLE` block typing and are flagged loudly
    (`H8539F-TABLE-CODE-OVERLAP` / `H8539F-TABLE-NO-TABLE` bookmarks).
- **1D scalars**: no header exists to verify; always labeled/typed at the raw XML address,
  always reported as `SUSPECT` tier by design (nothing to corroborate).
- **Axis verification** (`verify_xml_axis` / `axis_shape_ok` / `find_header_behind` /
  `find_header_ahead`, §1.8) does the equivalent check for the X/Y axis records a 3D table
  points at, independent of the table's own header check.
- **Pairing check** (`verify_table_pairing`, §1.10): tries to confirm a table is actually
  called from real code with a plausible calling convention, not just sitting at a
  header-shaped address by coincidence. Result is one of `PAIRED / PAIRED-AMBIGUOUS /
  NOT-PAIRED / PAIRING-UNVERIFIED` (all bookmarked, informational).
- **Overlap detection** (`check_range_overlap`): tracks every table's header+data byte span
  touched so far in the run (`touched_ranges`) and flags genuine collisions between two
  distinct XML `<table>` entries — not just against whatever code unit sat at a single
  address, which `type_header_as_data` alone can't see across entries.
- **Packed micro-record detection** (`is_packed_micro_record`): recognizes fixed-width
  packed records (e.g. "Case Number," "Software Version") that are tagged 2D/3D in the XML
  but have no real multi-element axis, so they're kept off the generic header-backup-by-N-bytes
  path that would otherwise walk into the previous record's tail.
- **`<include>` handling**: followed recursively, cycle-guarded via `visited`.
- **Deduplication**: existing labels survive re-runs; only plate comments are refreshed.
  Already-correctly-typed regions are left alone.
- **Console/log output**: every `[XML-VERIFY]` line is written to a full log file
  (`h8539_import_tables_xml2.log`, never truncated). Only actionable tiers
  (`CHECK, SUSPECT, OVERLAP, REJECTED, WARNING, ERROR`) always echo to console; routine
  pairing/span/offset lines only echo when `VERBOSE_CONSOLE = True` (default `False`), because
  large table sets (~250+) can produce thousands of lines that exceed Ghidra's console
  scrollback.

### 1.4 Step 8 — ROM header scraper (`run_rom_scraper`)
Runs independently of Step 5c (complements XML labels or stands alone if no XML applied):
- **Pass 1 — MUT table**: fixed offset `0x1FAD0`, 256 entries × 2 bytes, each a RAM pointer
  to a diagnostic variable.
- **Pass 2 — Scaling tables**: 6-byte header shape
  `[F0-F7] xx [E0-FE] xx 00 [count]` (output RAM ptr | input RAM ptr | 0x00 | entry count),
  followed by `count*2` bytes of 16-bit data. Sanity check: first 3 data words must not
  differ by more than `0x1000`.
- **Pass 3 — 3D value tables**: 7-byte header
  `0x03 | padding | X-ptr(word) | Y-ptr(word) | nrows`. Sanity check: first 3 data words
  must not differ by more than `0x4000`.
- **Pass 4 — 2D value tables**: 4-byte header `0x02 | padding | axis-ptr(word)`. Same
  `0x4000` sanity check.
- Skips ranges already claimed by XML (`xml_touched`) or by an earlier, independently
  verified scraper pass — guards against known false positives (e.g. a preceding axis
  record's trailing data word coincidentally reading as a valid 2D header byte pattern;
  four confirmed phantom "tables" from an earlier session are named in the source comments).
- All existence checks use `getPrimarySymbol()` (avoids a known `hasNext` iteration bug).

### 1.5 Step 8d — Page 2 coverage marker
Report-only: counts labeled symbols in page 2 (`0x00020000` onward) and prints a reminder
that the scraper only scans page 1 — a low count likely means page-2 tables are still
unlabeled. This is a known, documented blind spot, not a bug.

### 1.6 `resolve_header_address(xml_addr_int, ttype, hdr_size)`
Handles the fact that this ROM's XML mixes two addressing conventions with nothing in the
XML itself indicating which one a given `<table>` entry uses:
- **Convention A** ("address = header"): XML address IS the header byte directly. Confirmed
  live on ordinary tables (e.g. "Knock Control Above Load" @ `0x12832` = `02 00 F0 C4`).
- **Convention B** ("address = data, header is `hdr_size` bytes earlier"): the convention the
  script originally assumed unconditionally; still correct for tables using a
  header-record-style layout.
- Tries both candidate addresses through `header_shape_ok()` (real positive-evidence check
  against live ROM bytes) and picks whichever one actually looks like a header on **this**
  ROM, rather than assuming one convention file-wide.
- Falls back to the old Convention-B guess (`"unresolved"`) if neither candidate passes.

### 1.7 `header_shape_ok(header_offset, ttype)` — **the function at the root of the bug**
Current definition:
- **2D**: reads 4 bytes. Valid iff `b[0] == 0x02 and b[1] == 0x00`, and the next 2 bytes form
  an in-RAM pointer (`0xF0xx`–`0xF7xx`).
- **3D**: reads 7 bytes. Valid iff `b[0] == 0x03 and b[1] == 0x00`, and the next 4 bytes form
  two in-RAM pointers (X, then Y).
- Explicitly documented as **positive-evidence only** — a `False` result is not proof the
  address is wrong, just that *this* signature isn't present; callers are told not to reject
  on this check alone.
- Does **not** know about the header-record indirection convention (review.md item 17).

**This is why the plate comment for "High Octane Ignition Map 1" lands on the axis-pointer
block instead of the type-marker start.** The real bytes at this table are:
```
0x11de8: 03 14 F0            <- 3-byte type marker
0x11deb: C4 F0 C6 13         <- X ptr (F0C4), Y ptr (F0C6), + trailing byte
0x11def: 05 05 05 05 ...     <- data
```
`header_shape_ok("3D")` requires byte[1] to be exactly `0x00`. Byte 1 here is `0x14`, so the
check fails at `0x11de8` regardless of anchor choice. It also fails to recognize `0x11deb` as
a *table* header in the documented 7-byte sense (it's 4 bytes: mode-less X/Y ptr pair), so
strictly by the current code neither candidate should pass — the SUSPECT/UNVERIFIED tiering
seen in the original dumps is the script correctly reporting "I don't recognize this shape,"
not a location bug. The mislabeling is a **missing header shape**, not a wrong anchor.

### 1.8 `verify_xml_axis` / `axis_shape_ok` / `find_header_behind` / `find_header_ahead`
Same positive-evidence philosophy as `header_shape_ok`, applied to axis records a 3D table
points at:
- **scaling-6b**: two in-RAM pointers + 2-byte big-endian element count (1–32).
- **3d-value-7b**: `0x03 00` + X-ptr + Y-ptr + nrows, X≠Y, 1–32 rows.
- **2d-value-4b**: `0x02 00` + one in-RAM pointer (weaker positive — confirms a header exists
  but can't confirm claimed element count).
- Some trusted axes (e.g. Engine Temp @`0x2d418`) have **no** header prefix at all — the XML
  address points straight at the first breakpoint value. A `False` here is not proof of a bad
  address.
- `find_header_behind` / `find_header_ahead` check both directions from the claimed axis
  offset, since a wrong-by-N-bytes XML address can go either way.

### 1.9 `type_header_as_data(header_addr, size)`
Only called for `VERIFIED` tables. Clears any existing code units in the header's byte range
and creates a plain byte-array data type of the given size. Skips silently if the exact same
array already exists there (idempotent across re-runs).

### 1.10 `verify_table_pairing(header_offset, ttype)`
Looks for a real caller: finds code xrefs to the header, checks whether the calling
instruction sequence matches a known table-lookup calling convention
(`find_table_call_sites`, `find_preceding_axis_call`, `read_preceding_push_pair`). Reports
`PAIRED / PAIRED-AMBIGUOUS / NOT-PAIRED / PAIRING-UNVERIFIED` — informational, does not gate
typing.

### 1.11 Cleanup / maintenance entry points
- `clear_degenerate_bookmarks()`: removes stale `ROM SCRAPER` plate comments/bookmarks tagged
  `DEGENERATE`, `FLAT-FILL`, or `SAME AXIS` from earlier runs.
- `clear_xml_labels(offsets, clear_scraped=False)`: full teardown of labels/plates/bookmarks
  for a given offset set, used when re-importing a corrected XML.
- `get_last_xml_applied` / `set_last_xml_applied` / `get_last_xml_touched_addrs` /
  `set_last_xml_touched_addrs`: small persistence helpers (Ghidra program properties) so a
  later run/step can know what the last XML import actually touched.

### 1.12 Step 9 — Re-verify REJECTED tables
Runs after Step 5c/8. Re-checks any table previously marked `REJECTED` against current ROM
state. Since Step 5c already runs post-analysis, a `REJECTED` verdict is a genuine collision
at run time, not a stale pre-analysis snapshot — this step exists for the case of re-running
the script later after manual fixes or a different XML.

---

## 2. Root cause summary

The script's design (positive-evidence-only checks, VERIFIED/CHECK/SUSPECT/REJECTED tiering,
try-both-conventions address resolution) is sound and well-documented. The specific bug is
narrow: **`header_shape_ok()`'s 3D shape only recognizes `03 00 <Xptr> <Yptr>` (7 bytes,
byte[1] forced to `0x00`)**. This ROM (and the Evo5 ROM scraper dump confirms it's not
RVR-specific) actually uses a **3-byte type marker** (`03 14 F0`) *preceding* a separate
4-byte axis-pointer block (`<Xptr> <Yptr> <trailing byte>`), with no `0x00` in that position
at all. Neither the old 7-bytes-together assumption nor the current byte[1]==0x00 gate
matches this layout, so every table using it is permanently stuck at SUSPECT/UNVERIFIED
tier and never gets typed, and any plate/label ends up anchored wherever a *different*
positive check (or a code xref) happens to land — which is how the axis-pointer block ended
up looking like "the header" in the original EcuFlash-style dump.

---

## 3. Implementation plan

### Phase 1 — Confirm the shape, don't assume it
1. Pull 5–10 more confirmed 3D tables from this ROM and the Evo5 dump (not just this one) and
   diff their raw bytes at the type-marker position. Confirm whether `03 14 F0` is a fixed
   3-byte constant across all of them, or whether byte 2/3 vary (i.e. is `14 F0` itself
   meaningful, or filler).
2. Confirm the axis-pointer block's trailing byte (`13` in this table) — is it a row count
   (matches "Height: 18 rows" from the ROM Scraper output plus some encoding), a flag, or
   something else? Cross-check against 2–3 more tables with different heights.
3. Do the same confirmation pass for whether any 2D tables on this ROM use an equivalent
   split-header layout, or whether the bug is 3D-only.

### Phase 2 — Extend `header_shape_ok()` (and only this function first)
4. Add a new recognized 3D shape, e.g. `3d-split-marker`:
   `type_marker(3 bytes: 0x03 ?? ??) + axis_block(4 bytes: Xptr, Yptr, trailing)`,
   distinct from the existing `mode,0x00,Xptr,Yptr` 7-byte shape — do **not** overwrite the
   existing shape, since Convention A tables elsewhere on this ROM are confirmed correct
   under the current definition.
5. Keep `header_shape_ok` positive-evidence-only, per its existing documented contract: a new
   shape is an *additional* accepted pattern, not a replacement, and a non-match still isn't
   proof of a bad address.
6. Update the function's return value (or add a sibling function) to report **which** shape
   matched, so downstream code (plate text, bookmarks) can say "3D split-marker header" vs
   "3D mode-prefixed header" instead of a single undifferentiated "VERIFIED."

### Phase 3 — Fix the anchor, not just the shape check
7. Update `resolve_header_address()` so that when the new split-marker shape matches, the
   returned `header_int` is the **start of the 3-byte type marker**, not the axis-pointer
   block — i.e. the full logical header (marker + axis block), matching your stated
   requirement that the plate comment describe the whole structure from its true start.
8. Update `type_header_as_data()` call sites for this shape to type the **full 7-byte span**
   (3-byte marker + 4-byte axis block) as one unit, not 4 bytes starting mid-structure.
9. Update the plate-comment text builder so a split-marker table's comment documents both
   pieces explicitly (marker bytes, axis pointers, trailing byte, data start) — matching the
   style already used elsewhere in the script's `build_block`/`field` helpers.

### Phase 4 — Propagate to dependent checks
10. `is_packed_micro_record`, `axis_shape_ok`, `find_header_behind`/`find_header_ahead`,
    `check_range_overlap`, and `verify_table_pairing` all consume header offsets/sizes
    derived from `header_shape_ok`/`resolve_header_address`. Audit each for any
    hardcoded 4-byte/7-byte assumption that would now be wrong for split-marker tables
    (e.g. `check_range_overlap`'s span math, `type_header_as_data`'s size parameter).
11. Re-run `verify_xml_table` tiering logic mentally (or via dry-run) against a table that
    now VERIFIES under the new shape, to confirm it doesn't accidentally trip `CHECK` or
    `OVERLAP` against a previously-recorded (wrongly-anchored) range from an old run.

### Phase 5 — Regression safety
12. Add the confirmed split-marker tables from Phase 1 as a small fixture list (addresses +
    expected shape) and a lightweight self-check the script can run in dry-run mode before
    touching the live ROM, so this fix doesn't silently regress if the shape logic changes
    again later.
13. Re-run Step 9 (re-verify REJECTED) after the fix lands, since tables previously REJECTED
    or stuck at SUSPECT under the old shape check are exactly the ones this fix targets —
    confirm the count of SUSPECT/REJECTED split-marker-shaped tables drops as expected and
    nothing that was genuinely REJECTED (real code collision) gets wrongly reclassified.

### Phase 6 — Documentation
14. Update the file's top-of-script convention comment (currently states a single flat
    "2D header = 4 bytes / 3D header = 7 bytes" rule) to describe both the mode-prefixed and
    split-marker shapes, so the next person reading the header comment doesn't reintroduce
    the same one-shape assumption.
15. Add a dated note to `header_shape_ok()`'s docstring (matching the file's existing style of
    dated, evidence-cited comments) recording this finding: split-marker 3D shape, confirmed
    against RVR + Evo5, address/table name of the confirming example.

---

## 4. Open questions before starting Phase 2
- Is `14` in `03 14 F0` a fixed constant, or does it vary per table (e.g. encoding something
  the mode-prefixed shape puts elsewhere)? Blocks how strict the new shape's byte[1]/byte[2]
  check should be.
- Does the trailing byte in the axis block (`13` here) actually match "18 rows" from the ROM
  Scraper output under some known encoding, or is that a coincidence for this one table?
- Are there tables where the split-marker shape and the existing mode-prefixed shape could
  both spuriously match the same bytes (ambiguous shape), and if so, which should win?

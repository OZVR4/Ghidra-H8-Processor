# review12.md — RVR scaling/axis table verification: header-offset bug + correction batches

Date: 2026-08-13
Scope: `RVRbase.xml` at `test/rvr/xml/ecuflash/new/RVRbase.xml`, cross-checked against
live ROM `RVR_1998_x3 4g63t 21000011 md352553.hex` in Ghidra.

---

## 1. Method

For each scraped scaling/axis table region:
1. Read raw bytes at the scraped start address.
2. Check for the `[4 bytes unknown][00 NN]` 6-byte header shape, where `NN` = element count.
3. Compare `NN` against the XML's declared `elements` count for that axis.
4. Read the `NN * 2` bytes immediately after the header as the real data.
5. Apply candidate scalings (RPM, Load, Load16, Temp, uint16 raw, etc.) and judge which
   produces a physically sane curve.
6. Cross-check against any known live xrefs/callers where available.

Verification is done directly against live ROM memory in this session (not just inherited
from prior write-ups), via `ghidra:read_memory` on the open RVR program.

---

## 2. Confirmed systematic finding: 6-byte header offset bug

Multiple XML `<table>` entries for X/Y axis scaling tables currently have `address` pointing
at the **header start** instead of the **real data start**, which is header + 6 bytes.

Header shape confirmed repeatedly: `XX XX XX XX 00 NN` where the last 2 bytes are a
big-endian element count that matches the XML's own declared `elements` value. Real
breakpoint data begins immediately after, at `header_addr + 6`.

### Confirmed instances (verified live, this session)

| XML name | XML `elements` | Header addr | Header bytes | Data addr (header+6) | Data values | Scaling check |
|---|---|---|---|---|---|---|
| Error | 9 | `2d146` | `f0 c0 f0 ba 00 09` | `2d14c` | 0,8,16,24,32,40,48,56,64 | RPM→0-250 RPM (odd, likely delta not absolute RPM — see §3 caution) |
| Engine Load | 7 | `2d3fe` | `f0 c2 f0 ba 00 07` | `2d404` | 33,65,97,129,161,193,225 | Load(x*10/32)→10.3-70.3%, clean |
| Engine Temp | 8 | `2d490` | `f0 c0 f1 0e 00 08` | `2d496` | 8,32,47,60,74,90,102,117 | Temp(x-40)→-32 to 77C, clean/plausible |
| Baro Press. | 5 | `2d28c` | `f0 c0 f1 30 00 05` | `2d292` | 93,125,157,173,189 | Load16(x*5/8)→58-118, plausible |
| Load (MAF Hz raw) | 21 | `2d218` | (header, unread in this session; prior pass confirmed) | `2d21e` | raw uint16 run | uint16 raw — already correct in XML |

### XML edit status

| Address in XML | Status |
|---|---|
| `2d3fe` -> `2d404` | **Applied** (4 entries: lines 323/327/331/335), dated comment added |
| `2d490` -> `2d496` | **Applied** (line 403), dated comment added |
| `2d28c` -> `2d292` | **Applied** (line 395), dated comment added |
| `2d21e` (MAF Hz raw) | Already correct, no change needed |
| `2d146` (Error) | Left as-is deliberately - existing dated comment in RVRbase.xml notes this address matches a real code caller (`@0x272c6`) that pushes pointer `0xD146` directly, so the header address is meaningful to the code even though it's not the axis data start. Flagged as an open question, not silently changed. |

No address collisions found for `2d404`, `2d496`, or `2d292` against the current XML (checked
directly, zero conflicting entries).

---

## 3. Open questions / cautions

- Is the first 4 bytes of the 6-byte header (`f0 c0`, `f0 c2`, `f1 0e`, `f1 30`, etc.) itself
  meaningful (e.g. an in-RAM pointer, per the project's existing header-record precedent -
  see main review.md item 17), or just padding/marker bytes? Until this is understood,
  do not bulk-edit beyond the individually-verified entries above.
- "Error" @2d146 axis values (0-64, RPM-scaled) are anomalously small for an RPM axis - likely
  a delta/error quantity using RPM units/scaling rather than true engine RPM. Name/scaling
  left as-is pending further trace, consistent with the existing file comment.
- An external index previously flagged possible collisions around `2d404`/`2d496` - re-checked
  directly against the actual `RVRbase.xml` in this project and no collision exists here;
  safe to proceed.

---

## 4. Full scraped scaling-table index (ground truth for table boundaries)

Raw scrape of every auto-detected scaling/axis table region in the RVR ROM, `0x2d080`-`0x2d8d6`,
with each region's total byte span (header + data). Used to sanity-check XML `elements` counts
and to spot further header-offset candidates. `db[N]` = span size in bytes; expected span for a
correctly-bounded table is `6 (header) + elements*2`.

```
2d080  db[26]   2d09a  db[38]   2d0c0  db[18]   2d0d2  db[30]
2d0f0  db[38]   2d116  db[26]   2d130  db[22]   2d146  db[24]
2d15e  db[34]   2d180  db[18]   2d192  db[34]   2d1b4  db[70]
2d1fa  db[16]   2d20a  db[14]   2d218  db[48]   2d248  db[24]
2d260  db[20]   2d274  db[24]   2d28c  db[16]   2d29c  db[14]
2d2aa  db[34]   2d2cc  db[20]   2d2e0  db[34]   2d302  db[42]
2d32c  db[42]   2d356  db[38]   2d37c  db[44]   2d3a8  db[26]
2d3c2  db[24]   2d3da  db[20]   2d3ee  db[16]   2d3fe  db[20]
2d412  db[22]   2d428  db[24]   2d440  db[28]   2d45c  db[22]
2d472  db[30]   2d490  db[22]   2d4a6  db[20]   2d4ba  db[22]
2d4d0  db[60]   2d50c  db[36]   2d530  db[24]   2d548  db[40]
2d570  db[38]   2d596  db[40]   2d5be  db[24]   2d5d6  db[24]
2d5ee  db[24]   2d606  db[26]   2d620  db[22]   2d636  db[26]
2d650  db[26]   2d66a  db[26]   2d684  db[26]   2d69e  db[22]
2d6b4  db[38]   2d6da  db[40]   2d702  db[24]   2d71a  db[22]
2d730  db[28]   2d74c  db[14]   2d75a  db[20]   2d76e  db[22]
2d784  db[20]   2d798  db[24]   2d7b0  db[26]   2d7da  db[44]
2d806  db[28]   2d822  db[36]   2d846  db[16]   2d856  db[26]
2d870  db[28]   2d88c  db[18]   2d89e  db[18]   2d8b0  db[12]
2d8bc  db[26]   2d8d6  db[22]
```

78 entries total, `0x2d080` through `0x2d8d6`. Not yet cross-checked one-by-one against XML
entries beyond the batches in section 2 - remaining queue for next pass.

---

## 5. Next steps

1. ~~Apply the three pending edits in section 2 to `RVRbase.xml` (batch 1).~~ **Done 2026-08-13.**
2. Continue verifying remaining entries in the section 4 index against XML declared
   addresses/counts, in small batches, applying confirmed corrections to `RVRbase.xml` as
   each batch is verified live against ROM bytes - not in bulk, and not from unverified
   prior write-ups alone.
3. Resolve the header first-4-bytes question (section 3) before deciding whether it changes
   how corrected addresses should be documented in plate comments elsewhere in the Ghidra
   project.

## 6. Batch log

- **Batch 1 (2026-08-13):** `2d3fe->2d404` (Engine Load, x4), `2d490->2d496` (Engine Temp),
  `2d28c->2d292` (Baro Press.). All applied to `RVRbase.xml` with dated inline comments.
  No collisions. `2d146` deliberately left untouched (see section 2 table).

- **Batches 2-5 (2026-08-13):** Full sweep of every remaining unique X/Y Axis address
  referenced anywhere in `RVRbase.xml` (24 addresses beyond the batch-1 set), each read live
  against ROM and checked for the 6-byte header-offset pattern:

  `2d4ac, 2d4ba/2d4c0, 2d210, 2d24e, 2d308, 2d2b0, 2d0a0, 2d198, 2d42e, 2d446, 2d418, 2d11c,
  2d2e6, 2d382, 2d332, 2d086, 2d63c, 2d656, 2d4d6, 2d100, 2d2d2, 2d59c, 2d3c8, 2d576, 2d5dc`

  **Result: all 24 are already correct** — either no header prefix exists at that address (data
  starts directly at the XML-declared address), or a prior fix already applied the +6 offset
  (e.g. `2d59c` Boost Error, `2d4c0` Coolant/Air Temp, `2d21e` MAF Hz raw). No further edits
  needed on any of these.

  Note: the header-offset bug is **not universal** — it appears to affect only a specific
  subset of tables (Engine Load, Engine Temp @2d490, Baro Press., Error @2d146, MAF Hz raw,
  Boost Error, Coolant/Air Temp @2d4c0), while the majority of RPM/Load/Temp axes in this XML
  have no header prefix at all and were already pointing at the right place. This matches the
  file's own documented philosophy (positive-evidence-only checks, not a blanket rule).

  One naming-only oddity spotted in passing (not an address bug, not fixed): the axis at
  `2d42e` has two XML entries, one named "Time" and one named "Engine Temp", both with
  `scaling="Temp"` — the "Time" label looks like a copy/paste or mislabeling artifact. Flagged
  for a future naming pass, out of scope for this address-correction batch.

## 7. Cross-ROM verification against EVO5

The EVO5 ROM (`22580006_EVO5_Stock.hex`, also open in this Ghidra project) has its own scraped
scaling-table index (`0x2c500`-`0x2cdc6`, 81 entries). A naive fixed-offset alignment against
RVR's index (`0x2d080`-`0x2d8d6`) holds for the first 5 entries (constant `+0xB80` offset,
identical sizes) but drifts after that once individual table sizes diverge between the two
ROMs/calibrations — so table-by-table index alignment isn't reliable past the first few entries.
Direct byte-pattern search (`ghidra:search_byte_patterns`) on EVO5 for the exact confirmed RVR
data sequences is more reliable and was used instead.

### Result: all four batch-1 fixes independently corroborated

| Table | RVR (data addr) | EVO5 match (header addr -> data addr) | EVO5 header bytes | Elements | Verdict |
|---|---|---|---|---|---|
| Engine Load | `2d404` | `2c884` -> `2c88a` | `f0 c4 f1 44 00 07` | 7 (same) | Identical data (33,65,...,225). Confirms header-offset fix. |
| Engine Temp | `2d496` | `2c924` -> `2c92a` | `f0 c4 f1 14 00 08` | 8 (same) | Identical data (8,32,...,117). Confirms header-offset fix. |
| Baro Press. | `2d292` | `2c758` -> `2c75e` | `f0 c4 f1 36 00 05` | 5 (same) | Identical data (93,125,...,189). Confirms header-offset fix. |
| Error | `2d14c` (not in XML, see section 2) | `2c9f4` -> `2c9fa` | `f0 da f0 c2 00 11` | **17** (RVR: 9) | Same header structure and same linear step-of-8 pattern (0,8,16...), but EVO5's axis runs to 17 elements (0-128) vs RVR's 9 (0-64). Not a discrepancy in the fix — RVR's own header independently declares count=9 and its own data stops exactly there. Just a coarser/shorter calibration on RVR, or a genuinely different (shorter) axis. |

### New finding: header's first 4 bytes are meaningful, not padding

This resolves the open question in section 3. In every EVO5 instance found, the first 4 header
bytes are themselves two `0xF0xx`/`0xF1xx`-style in-RAM pointers (e.g. `f0 c4 f1 44`) - the same
pointer-range convention already used elsewhere in this project's positive-evidence checks
(`header_shape_ok`, `axis_shape_ok`). So the full 6-byte header shape is:
`[RAM ptr 1 (2 bytes)][RAM ptr 2 (2 bytes)][element count (2 bytes, big-endian)]`, then data.
This is a stronger, more specific finding than "4 unknown bytes + count" and should feed back
into the main `h8539_import_tables_xml2.py` `header_shape_ok()` work described in review11.md's
Phase 2, since it gives a positive-evidence check for these bytes rather than treating them as
opaque.

**Conclusion:** cross-ROM verification confirms all four RVR fixes were correct, and adds a
concrete structural finding (header = 2 RAM pointers + count) worth carrying back into the main
script's shape-detection logic.

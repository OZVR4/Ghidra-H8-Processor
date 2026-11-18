Review 2: RVR XML axis/table address verification
====================================================================

Scope: verify and correct table/axis ADDRESSES ONLY in the RVR XML
(21000011_1997-2001_RVR_X3_Mt__4g63t_.xml) against the live
RVR_1998_x3 4g63t 21000011 md352553.hex ROM in Ghidra. Naming is out
of scope -- existing table/axis names are left as-is even when a fix
moves the address to a different real-world quantity than the name
implies. Purely a data-integrity pass.

Source list for this batch: the H8539F-TABLE-*-OVERLAP /
H8539F-AXIS-DATA-OVERLAP bookmark set (Ghidra bookmark export,
2026-08-01), covering table/data address collisions and axis/axis
collisions flagged during ROM scraping. After this batch: the ~94
"Unknown"-category scraper-derived TABLE_2D_*/TABLE_3D_* tables with
placeholder axis info (deliberately wrong axis, set only so data is
visible in EcuFlash -- see review.md item 20) get the same treatment.

Method (per entry):
1. Read raw ROM bytes at the flagged/candidate address(es).
2. Classify header shape against the confirmed scraper signatures
   (from h8539_ecu_master_setup.py Step 8b/8c, CHANGELOG.md):
   - 3D value table: 7-byte header `0x03 | pad | X-ptr(word) | Y-ptr(word) | nrows`
   - 2D value table: 4-byte header `0x02 | pad | axis-ptr(word)`
   - Scaling table: 6-byte header `[F0-F7] xx [E0-FE] xx 00 [02-90]`
3. Confirm data curve is sane (monotonic-ish, terminates at/near a
   0xFF sentinel before the next real table's header starts).
4. Where possible, find the real caller (table_lookup_interp @0x14656 /
   axis_lookup_interp @0x14735 / table_lookup_indexed @0x14854 /
   table_read_indexed @0x14880) via literal-push byte search and
   disassembly, and confirm the literal pointer + bank-adjustment.
   CONFIRMED FORMULA (2026-08-01, verified against 3 independent call
   sites): callers push a 16-bit offset then an 8-bit bank immediately
   before the pjsr:
     mov:g.w #0xXXXX:16,@-SP   ; 16-bit offset
     mov:g.w #0xN:8,@-SP       ; bank N
     pjsr @0x14735/0x14656/0x14854/0x14880
     real_address = N * 0x10000 + 0xXXXX
   Byte encoding for the search: `BF 07 <hi> <lo>` = mov:g.w #imm:16,@-SP;
   `BF 06 <imm>` = mov:g.w #imm:8,@-SP (bank). Use search_byte_patterns
   on `BF 07 <hi> <lo>` (the offset half of the candidate real address)
   to find literal-push callers directly.
   IMPORTANT CAVEAT learned this pass: a 2D/3D value table's own header
   axis-ptr field is a RAM address (not a ROM pointer) -- it's the RAM
   cell an axis_lookup_interp call writes its interpolated
   {index,fraction} result into (the record's own first word, "value_ptr").
   Two different tables can legitimately share one axis_lookup_interp
   record via this RAM cell (e.g. Load/RPM globals reused by many 3D
   maps), OR the same RAM tag can turn up on multiple *unrelated*,
   independently-real ROM records (seen at least twice this pass) that
   just happen to reuse the same scratch RAM pair. Matching a table's
   axis-ptr RAM value against a scaling-table record's value_ptr is a
   *lead*, not proof -- always confirm with a literal-push caller trace
   (or get_xrefs_to on the table's own address, the 8/8 DATA-xref
   pattern from items 3/4/6) before writing an XML change.
5. Record verdict: CONFIRMED CORRECT / CONFIRMED WRONG (+ real
   address if found) / UNRESOLVED, with the evidence.
6. Only write XML changes for CONFIRMED items. Never guess.

Status key: OPEN / CONFIRMED / WRONG-FIXED / WRONG-UNRESOLVED

--------------------------------------------------------------------
Batch 1 -- H8539F-TABLE-*-OVERLAP / H8539F-AXIS-DATA-OVERLAP bookmarks
--------------------------------------------------------------------

1. Air Temperature Compensation (2D) -- STATUS: CONFIRMED WRONG, FIXED (per
   review.md item 19, 2026-07-30). Old XML addr 0002af74 landed on real code,
   not a table. Real table at 0x11d7a (confirmed live caller iat_axis_update),
   axis at 2d4ac (7 elements). XML already updated: table address="11d7a",
   axis address="2d4ac". No further action needed.

2. Accel Enrichment (2D) -- flagged suspect solely for overlapping
   'Low Octane Fuel Map 1' (00011cb1 vs 00011bc0).
   STATUS: CONFIRMED WRONG -- byte-verified live 2026-08-01.
   Read 11bc0 onward directly: real header 03 00 F0C0 F0C2 0E
   (mode=3, stride=14). Full data body traced byte-by-byte to its
   real 0xFF sentinel at 0x11ccb (261 bytes of real curve data,
   11bc8-11cca). 0x11cb1 (the flagged address) is offset 0x11 into
   this table's own live data (byte value 0x8d) -- NOT a second
   table, not a real overlap between two tables at all. The
   "OVERLAP" framing on this bookmark is itself the bug: there's one
   real table (11bc0) and one bad address (11cb1) landing inside it.
   Immediately following the real sentinel at 11ccb is a SEPARATE,
   genuinely real header at 0x11ccc: 02 00 F0C8 (mode=2, single-axis
   2D), monotonic curve visible (60,56,50,46,3c,33,23,20,17,0e,0a...).
   Confirmed live consumer of 0x11ccc: decel_fuel_cut_eval (0x222a6)
     222ce: mov:g.w #0x1ccc:16,@-SP   (table pointer)
     222d2: mov:g.w #0x1:8,@-SP       (EP bank = 1)
     222d5: pjsr @0x14656:24          (table_lookup_interp)
   Call chain: decel_fuel_cut_eval <- f25e_bit6_update_from_decel_fuel_cut
   <- main_control_loop_update (real, reachable, periodic).
   Separately confirmed: fueling_accel_enrichment_calc (0x21aa5, the
   function the "Accel Enrichment" name implies) contains ZERO calls
   to table_lookup_interp/axis_lookup_interp/table_lookup_indexed --
   pure fixed-point arithmetic via div_s32_s16_rounded/
   div_u32_u16_rounded/identity_passthrough_arg1. This table is
   definitely not consumed by anything resembling accel-enrichment
   logic.
   VERIFIED COMMENT for XML (address only, name left as-is per scope):
     "Accel Enrichment" address corrected 11cb1 -> 11ccc. Real header
     at 11ccc (mode=2), real single-axis 2D table, confirmed live
     caller decel_fuel_cut_eval via main_control_loop_update. NOTE:
     despite the address fix, this table's actual function is decel
     fuel cut related, not accel enrichment -- naming correction is
     out of scope for this pass but flagged for whoever next touches
     names.
   Axis: mode=2 header means single embedded axis, no separate
   axis_lookup_interp call seen at this call site -- TO CONFIRM: check
   whether table_lookup_interp internally reads an axis pointer from
   within the 0x11ccc record itself (would need decompiling
   table_lookup_interp @0x14656, not yet done this pass).

3. Boost Enhancement Enrich/Enlean (Anti-lag) Map (3D) -- flagged
   suspect solely for overlapping 'Warmup Retard' (000124ec vs
   000124f9).
   STATUS: CONFIRMED CORRECT (per review.md item 19, 2026-07-30).
   124ec = confirmed real record pointer, 8/8 xrefs. Real data 342
   bytes, 0xFF sentinel at 12649. XML address 124f3 = 124ec+7,
   matches the mode-3 (3D) convention. Warmup Retard's old address
   (124f9) was the bug, not this table -- see item 4 below.
   No XML change needed for this entry.

4. (from same overlap pair) Warmup Retard -- old XML address 124f9
   overlapped Boost Enhancement's own confirmed data.
   STATUS: WRONG-FIXED (per review.md item 19, 2026-07-30, already
   applied). 124f9 had ZERO xrefs, sat only 13 bytes into Boost
   Enhancement's real data -- not a separate table. False lead
   (11ccc) tried and retracted (wrong mode byte: Warmup Retard needs
   mode=3/Load+RPM per XML's own axis element counts, 11ccc's
   neighborhood didn't fit). Real table found at 0x1264a -- second
   mode-3 header immediately after Boost Enhancement's real end,
   header 03 14 F0C0 F0C2 12 (stride 0x12=18), distinct 342-byte
   curve (confirmed different values from Boost Enhancement's own,
   ruling out copy-paste duplication), own sentinel at 127a7.
   CONFIRMED via get_xrefs_to: exactly 8 DATA xrefs at 1264a
   (0x2da0c-0x2da28), same index-block confirmation signature used
   throughout this file. XML already corrected: address="12651"
   (1264a+7, mode-3 convention). Axis element counts (Load 19, RPM
   17) left unchanged -- note real data is 342 bytes/stride 18, not
   a clean 19x17=323, worth a follow-up check if EcuFlash renders a
   trailing gap. No further action needed for the table address
   itself.

5. Injector Battery Voltage Latency Compensation -- flagged suspect
   for overlapping 'Desired ISCV intial step position with AC
   off-Neutral' (000128a3 vs 000128a2).
   STATUS: CONFIRMED WRONG, XML entry removed (per review.md item 19,
   2026-07-30, RESOLUTION). Ghidra bookmark
   H8539F-TABLE-DATA-OVERLAP-CORRECTED at 0x128a2 carries a
   ROM-scraper plate comment confirming the real table at that
   address IS "Desired ISCV intial step position with AC
   off-Neutral" (4-byte header, data at 128a6, ending ~128d6). The
   claimed Injector Battery Voltage Latency Compensation range
   (128a3-128ad, XML addr 128a7) falls entirely inside that real
   table's own header+data -- there was never a second, independent
   table there. No code-verified table_lookup_interp/
   table_lookup_indexed/table_read_indexed call site was ever found
   for this table across 9 checked candidate callers (see review.md
   item 19 continuation). XML entry commented out permanently (not
   pending replacement -- none exists or should be sought).
   No further action needed.

6. Desired ISCV intial step position with AC off-Neutral (the other
   side of item 5's overlap) -- 000128a2.
   STATUS: CONFIRMED CORRECT (per review.md item 19). 8/8 xrefs
   confirm 128a2 is real. No XML change needed.

7. TABLE_2D_00011EE2 (2D) -- flagged suspect for overlapping 'Decel
   Fuel Cut Resume Enrich' (00011ee2 vs 00011f04).
   STATUS: CONFIRMED, applied per review.md item 20 (2026-07-31).
   This was one of the "3 remaining H8539F-TABLE-DATA-OVERLAP-
   CORRECTED bookmarks never written to XML" -- corrected to
   header+4-bytes per ROM-scraper plate comment, matching the
   established 2D convention (11EE2 -> 11EE6). Placeholder RPM axis
   left untouched deliberately (scaffolding, not a real axis claim --
   applies to all ~94 "Unknown" category tables, see Batch 2 below).
   XML already updated. No further action needed for THIS entry's
   own address; "Decel Fuel Cut Resume Enrich" (11f04) not
   independently re-checked this pass -- TO VERIFY.

8. TABLE_2D_0002D3C0 (2D) -- flagged suspect for overlapping 'TPS
   (axis of Max Total Upward WGDC Correction vs TPS)' at 0x2D3C8.
   STATUS: CONFIRMED, applied per review.md item 20 (2026-07-31).
   Corrected to header+4-bytes (2D3C0 -> 2D3C4) per ROM-scraper
   plate comment, same convention as item 7. XML already updated.
   'TPS (axis of Max Total Upward WGDC Correction vs TPS)' AXIS_TPS
   at 2d3c8 -- CHECKED 2026-08-01, see item 10 below: confirmed not a
   real axis (lands in the table's own data body), real candidate
   axis is 0x2d3fe but TABLE_2D_0002D3C0's own caller still unfound,
   so this entry's axis address is WRONG-UNRESOLVED, not yet fixable.

9. TABLE_2D_0002D668 (2D) -- flagged suspect for overlapping 'RPM
   (axis of Accel Enrichment)' at 0x2D68A.
   STATUS: CONFIRMED, applied per review.md item 20 (2026-07-31).
   Corrected to header+4-bytes (2D668 -> 2D66C) per ROM-scraper
   plate comment. XML already updated. 'RPM (axis of Accel
   Enrichment)' AXIS_RPM at 2d68a NOT independently re-checked this
   pass -- TO VERIFY. NOTE: "Accel Enrichment" name reappears here
   too (different address, 2d68a is an axis not the 11ccc table from
   item 2) -- another naming-vs-address mismatch candidate, out of
   scope for this pass per naming being excluded, flagged only.

10. AXIS_TPS mislabel note (from review.md item 20, found in passing).
    STATUS: WRONG-UNRESOLVED (byte-verified 2026-08-01, table's own
    caller not found -- see item 8 update below for the shared
    context). AXIS_TPS @ 0x2d3c2 is not a real axis record at all: it
    lands squarely inside TABLE_2D_0002D3C0's OWN header (bytes 2-3 of
    "02 00 F0C2", i.e. the table's axis-ptr field itself, not a
    separate structure). The item 8 overlap address (0x2d3c8) also
    isn't real -- it lands 4 bytes into the table's own data body.
    Real axis record with matching leading RAM tag (F0C2) confirmed to
    exist at 0x2d3fe (F0C2 F0BA 0007, 7 elements: 33,65,97,129,161,
    193,225) -- this is the SAME record independently confirmed live
    via literal-push caller in item 11 below (used by
    f0ba_f2d0_o2_mode7_8_correction_calc for O2 mode 7/8 correction,
    not TPS/AXIS_TPS's old tcu_shift_torque_and_knock_mgmt lead, and
    not "Max Total Upward WGDC Correction vs TPS" either). NOT FIXED:
    could not find any literal-push caller for TABLE_2D_0002D3C0
    itself (checked both header 0x2d3c0 and data-start 0x2d3c4 --
    zero xrefs, zero literal pushes). The table/axis RAM-tag match
    (F0C2) is suggestive but per the Method step 4 caveat above is not
    proof by itself, especially since a second, unrelated record also
    sharing a different RAM tag pair (F0C2/F0BA) was found nearby at
    0x2d356 (16 elements) -- this ROM reuses RAM scratch pairs across
    multiple independent records, so tag-matching alone can't
    disambiguate here. Likely reached via computed-pointer-array
    indirection (same category as Batch 2's expected difficulty).
    NEXT STEP: find TABLE_2D_0002D3C0's real caller via
    table_lookup_indexed/table_read_indexed indirection before
    touching this XML entry.
    2026-08-01 FOLLOW-UP: decompiled table_lookup_indexed (0x14854) --
    it takes an ARRAY BASE POINTER as a stack parameter (pushed by its
    caller, same literal-push convention as axis/table_lookup_interp)
    plus a global index variable (0x1f1fc, masked &7) to pick 1 of up
    to 8 pointer-array entries. Searched for a literal push of
    TABLE_2D_0002D3C0's address as an ARRAY BASE (not as a direct
    table pointer) -- same zero-result outcome (`BF 07 D3 C0` / `D3 C4`
    still not found anywhere, and this table has no 8-DATA-xref array
    pointing to it the way items 3/4/6/12's ignition maps do). Also
    tried get_xrefs_to on the array-base candidates for already-
    CONFIRMED tables (e.g. 0x2da0c, the Warmup Retard/Boost Enhancement
    array) and got zero hits even there -- meaning array-base pointers
    themselves are apparently never referenced via a discoverable
    static xref/literal-push anywhere in this ROM, even for tables we
    are certain are real and correctly wired. The array base must be
    computed/loaded through an addressing mode Ghidra's static xref
    engine doesn't track (e.g. PC-relative table-of-tables one level
    up, or register-indirect through a value set by yet another
    function). This is a structural limitation, not a search-pattern
    problem -- REMAINS BLOCKED. Confirmed one more useful negative:
    TABLE_2D_0002D3C0 is definitively NOT part of the
    knock_octane_dual_axis_table_blend_f2a8 call chain (see item 12
    follow-up below) -- that function's arrays (0x2d9ac/0x2d9cc/
    0x2d96c/0x2d98c) resolve to 0x11abc/0x11f74-adjacent ignition
    tables, nothing at 0x2d3c0/2d3c4. No further lead to chase without
    a fundamentally different technique (e.g. symbolic execution or
    manual trace of the ISR/dispatch table that ultimately reaches
    fuel-table consumers). Recommend deferring to a dedicated session
    focused on cracking the array-base addressing mode generally, since
    this will also block most of Batch 2's ~94 tables.

11. RPM (axis of 'Limp Home - TPS Table') -- flagged overlapping TPS
    axis of the same table at 0x2D3F8 (overlap addr 0x2D3EE).
    STATUS: WRONG-FIXED (2026-08-01, XML updated).
    Byte reads confirmed: RPM axis header IS correctly at 0x2d3ee
    (F0C0 F170 0005, 5 elements: 128,256,512,768,1024 raw) -- no
    change needed. TPS axis was wrong on both address and element
    count: XML claimed 2d3f8/5-elem, but 0x2d3f8 lands mid-data inside
    the RPM record's own body (2d3ee's data spans 2d3f4-2d3fd). Real
    TPS axis header is 6 bytes later at 0x2d3fe (F0C2 F0BA 0007), 7
    elements: 33,65,97,129,161,193,225.
    LIVE CALLER CONFIRMED via disassembly (search_byte_patterns on
    `BF 07 D3 EE` and `BF 07 D3 FE`, bank-adjustment formula from
    Method step 4): f0ba_f2d0_o2_mode7_8_correction_calc (0x242f5,
    called from engine_periodic_correction_master_dispatch, a real
    reachable periodic function) pushes literal 0xd3ee then 0xd3fe
    (bank=2 both times) into axis_lookup_interp (0x14735)
    back-to-back, immediately before table_lookup_interp(0x11dc2)
    (bank=1, offset 0x1dc2 -> 0x11dc2, matching the Limp Home table's
    own address neighborhood, 11dc9 = 11dc2+7 per 3D convention).
    NOTE: the live caller is O2 sensor mode 7/8 correction, not
    limp-home logic as named -- naming correction out of scope for
    this pass, flagged only.
    XML APPLIED (test/rvr/xml/ecuflash/new/
    21000011_1997-2001_RVR_X3_Mt__4g63t_bac.xml): TPS axis
    address="2d3f8" elements="5" -> address="2d3fe" elements="7".
    RPM axis address="2d334" -> address="2d3ee" (was already wrong in
    the working XML, not just stale from review.md's notes -- see
    audit comment in the XML for full detail). elements="5" unchanged.

12. RPM (axis of 'Boost Desired Engine Load # 1') -- flagged
    overlapping RPM axis of 'High Octane Ignition Map 1' at 0x2D332
    (overlap addr 0x2D334).
    STATUS: PARTIALLY RESOLVED -- real record found and live-confirmed,
    but NOT applied to XML pending a Load/RPM identity conflict.
    Neither 0x2d332 nor 0x2d334 is a real header -- both fall inside
    one real axis record's own data (element 0 and element 1
    respectively). Real header: 0x2d32c (F0C0 F170 0012, 18 elements,
    full data read and confirmed monotonic 0->2816 raw, sentinel/next
    header at 0x2d356).
    LIVE CALLERS CONFIRMED via disassembly (search_byte_patterns on
    `BF 07 D3 2C`): TWO callers found, both Idle Speed Control
    functions -- isc_f34e_f34a_correction_calc (0x25742) and
    isc_f374_correction_calc (0x25e8b) -- NOT "Boost Desired Engine
    Load # 1" or "High Octane Ignition Map 1" as the XML names claim.
    Naming mismatch flagged, out of scope per this file's stated
    scope.
    BLOCKER before writing XML: read High Octane Ignition Map 1's
    real header directly (0x11f74, confirmed via 8/8 DATA xrefs at
    0x2db6c-0x2db88, same signature as items 3/4/6 -- table address
    11f7b = 11f74+7 is ALREADY correct, no change needed there). That
    header is X-ptr=F0C0, Y-ptr=F0C2, and this file's own convention
    is X=Load/Y=RPM for 3D maps -- meaning F0C0 (0x2d32c's RAM output)
    may actually feed the LOAD axis here, not RPM as item 12's name
    assumes. Additionally "Boost Desired Engine Load # 1"'s XML
    element count (9) doesn't match 0x2d32c's 18 at all, suggesting it
    may need a different, not-yet-found record entirely rather than
    2d32c. Writing an RPM-axis fix here risks a wrong Load/RPM swap --
    NOT applied, deliberately held back per Method step 6 (never
    guess). NEXT STEP: trace which specific RAM cell (F0C0 or F0C2)
    the ISC functions above actually populate as *output* semantically
    (Load vs RPM), and separately locate "Boost Desired Engine Load #
    1"'s real 9-element axis record (currently unknown).
    2026-08-01 FOLLOW-UP: found the real caller chain for the ignition
    maps -- knock_octane_dual_axis_table_blend_f2a8 (0x233a0), reached
    via table_lookup_indexed's callers list. Its disassembly is clean
    (no decompiler ambiguity this time):
      axis_lookup_interp(0x2d2e0, bank=2)   -- RPM, 14 elements
      axis_lookup_interp(0x2d302, bank=2)   -- Load, 18 elements
      then btst 0x5,@0xf0f8 selects array base 0x2d9ac (set) or
      0x2d9cc (clear) -> table_lookup_indexed (Map "1" pair)
      then btst 0x7,@0xf1f2 (nested w/ 0xf0f8 bit5) selects
      0x2d96c or 0x2d98c -> table_lookup_indexed (Map "2" pair)
      result blended via table_read/interp (0x14599), written to F2A8.
    This RESOLVES the Load/RPM identity question from the note above:
    swapxy="true" on these table XML entries means the header's X-ptr
    (F0C0) maps to the XML's "RPM/Y Axis" entry and Y-ptr (F0C2) maps
    to "Load/X Axis" -- so F0C0=RPM is correct as item 12 originally
    assumed, my earlier worry about a Load/RPM swap was unfounded.
    HOWEVER this does NOT confirm 0x2d32c (item 12's original
    candidate) as the real RPM axis for these ignition maps -- the
    live-traced RPM axis actually used here is a DIFFERENT record,
    0x2d2e0 (14 elements: matches this file's own ROM notes at the
    top, "RPM axis @2d2b0: 500-7000 RPM (14 elem)"), and the Load axis
    is 0x2d302 (18 elements, data at 2d308, matches "Load axis @2d308:
    10-260% (18 elem)"). 0x2d32c (18 elements, confirmed via items 12's
    original work to be called only by isc_f34e_f34a_correction_calc
    and isc_f374_correction_calc) is NOT part of this call chain at
    all -- it's a genuinely separate, ISC-only axis unrelated to any
    currently-named ignition/boost XML table.
    NEW COMPLICATION: the pointer array pushed here (0x2d9ac, selected
    when octane-flag bit0xf0f8:5 is set) does NOT point to 0x11f74/
    11f7b (the address the XML currently uses for "High Octane
    Ignition Map 1", independently confirmed via its OWN 8/8 DATA-xref
    array at 0x2db6c-0x2db88). It points to 0x11abc instead -- read
    directly, all 8 entries identical (00 01 1A BC = bank 1, offset
    0x1abc). This means there are apparently TWO independently-real
    ROM tables that could each plausibly be "the" high-octane ignition
    map depending on which subsystem is traced: the one at 0x11f74
    currently in the XML, and this one at 0x11abc feeding the knock/
    octane blend. Untangling which is the EcuFlash-tunable table (or
    whether both legitimately coexist for different purposes -- e.g.
    one being an internal knock-correction sub-map, the other the
    primary displayed map) needs dedicated investigation, not
    inference. NOT resolved, NOT applied to XML. "Boost Desired Engine
    Load # 1"'s real 9-element axis is still completely unlocated --
    not found in this follow-up either. REMAINS PARTIALLY RESOLVED /
    BLOCKED. Genuinely useful confirmed sub-finding worth keeping
    regardless of the above: 0x2d2e0 (RPM, 14 elem) and 0x2d302 (Load,
    18 elem) are a real, live-confirmed axis pair for at least the
    octane-blend logic and match this file's own top-of-file ROM
    notes -- good candidates to check against Low Octane Ignition Map
    1/2 and the Boost Enhancement/Warmup Retard family in a future
    pass, since several of those currently reuse addresses (2d332/

    RESOLVED AND APPLIED (2026-08-01, later same session): found the DIRECT
    literal-push caller for "Boost Desired Engine Load # 1" itself, no tag-
    matching or indirection needed. Function at 0x191a0 pushes
    axis_lookup_interp(0xd5d6, bank=2) then IMMEDIATELY table_lookup_interp
    (0x2af8, bank=1) = 0x12af8 -- this exact table, direct adjacency, the
    same strength of evidence as item 11's fix. Real axis: 0x2d5d6 (F0C0
    F190 0009, 9 elements: 0,64,128,192,256,320,384,448,512 raw) -- element
    count already matched XML (9), only the address (2d334) was wrong.
    XML APPLIED: RPM axis address="2d334" -> address="2d5d6", elements
    unchanged.

    Separately, ALSO re-derived "High Octane Ignition Map 1"'s real axis
    pair with much stronger evidence than the earlier "matching tag" lead:
    isc_f34e_f34a_correction_calc (0x25742) pushes axis_lookup_interp
    (0xd32c)+axis_lookup_interp(0xd37c) [bank=2] back-to-back, immediately
    followed by table_lookup_indexed(0xdb6c) [bank=2] -> array base
    0x2db6c, which Ghidra's OWN xref engine independently confirms (8/8
    DATA xrefs) resolves to 0x11f74 (11f7b = 11f74+7, matches the existing,
    correct XML table address). This closes the loop cleanly: RPM axis =
    0x2d32c (18 elem, unchanged), Load axis = 0x2d37c (19 elem, unchanged,
    old address 2d382 was its data-start not header, same recurring bug).
    XML APPLIED to "High Octane Ignition Map 1" only.
    IMPORTANT, checked before extending this fix further: read the raw
    bytes of isc_f34e_f34a_correction_calc's OTHER array selection at this
    branch point (0x2db8c, the function's alternate path when the F0F8
    bit5 flag is clear) -- it resolves to 0x120d2 (-> 0x120d9, "High Octane
    Ignition Map 2"), NOT "Low Octane Ignition Map 1" (12237) as the
    High/Low-octane naming would suggest. So this specific F0F8 bit5 check
    is a Map1/Map2 selector, not a High/Low-octane selector -- "Low Octane
    Ignition Map 1"'s real axis is NOT confirmed by this chain and was
    deliberately left untouched (still on the old 2d382/2d332 placeholder).
    Also NOT touched, still needing individual verification before any fix:
    Boost Enhancement Enrich/Enlean (Anti-lag) Map (124f3, shares the old
    2d382/2d332 placeholder but its own caller wasn't traced this session),
    Warmup Retard (12651, RPM elements="17" in XML -- doesn't match 2d32c's
    18, so it definitely needs a DIFFERENT axis, not a copy of this fix),
    Max Wastegate Duty #1-4 (12a96/12aaa/12abe/12ad2, elements="16", also
    doesn't match, needs its own trace). Do not blanket-apply 2d32c/2d37c
    to any of these without the same per-table caller verification used
    above -- this file's own top-of-file ROM notes already list several
    same-tagged-family axes with different element counts (2d2b0/14,
    2d332-area/18, plus whatever the 16/17-element ones turn out to be),
    confirming this RAM tag family is reused by many distinct, differently-
    sized real records, not one universal axis.
    ALSO NOTE: the ~90 "Unknown"-category TABLE_2D_*/TABLE_3D_* scraper-
    derived entries elsewhere in this XML (Batch 2's placeholder tables)
    also default to the same 2d332/2d382 placeholder pair, each with an
    explicit inline comment marking it as deliberate and unresolved. None
    of those were touched either -- resolving them individually is Batch
    2's job, not a blanket find-and-replace, for the same reason as above.

    ARRAY-INDIRECTION SCRIPT COVERAGE CHECK (2026-08-02): after the
    review.md byte-signature sweep found 9 total functions sharing the
    bank/record calling convention (see review.md's 2026-08-02 update),
    checked which of the newly-found ones are actually candidates for the
    ResolveTableReadIndexedArrays.java-style array-indirection script (i.e.
    which ones read an 8-slot array of {pad,bank,hi,lo} POINTERS to resolve
    further, vs which just read raw calibration values directly with no
    further indirection to discover). Determined by shift-count in each
    function's own disassembly (shll.w before the array index add): *4
    stride (two shll.w) = 4-byte pointer array, candidate; *2 or *1 stride
    (one or zero shll.w) = raw value array, not a candidate at all --
    running the pointer-resolution script against those would misinterpret
    calibration data as bogus bank/offset pairs.
      table_3axis_interp_triple_wrapper (0x1497b): *4 stride, candidate.
      Wrote ResolveTable3AxisWrapperArrays.java (same technique, retargeted
      at 0x1497b), ran it: 0 call sites found in this ROM. Same negative-
      result situation as gear_indexed_table_lookup_f1fc (already recorded
      above) -- nothing to resolve, not a bug, just unused in this build.
      table_read_word_indexed_f1fc (0x14838): *2 stride, NOT a candidate --
      its array is 8 raw uint16 calibration values, no addresses to find.
      read_indexed_byte_f1fc (0x1481c): *1 stride (no shift), NOT a
      candidate -- 8 raw bytes, same reasoning.
    Net result: every function in the 9-member family that COULD benefit
    from this script has now either had it run (table_lookup_indexed,
    table_read_indexed: real results; table_3axis_interp_triple_wrapper,
    gear_indexed_table_lookup_f1fc: confirmed zero callers) or been
    confirmed structurally inapplicable (table_read_word_indexed_f1fc,
    read_indexed_byte_f1fc). table_lookup_interp/axis_lookup_interp/
    table_3axis_interp_triple are the remaining 3 -- these are the DIRECT
    (non-array) record consumers, a different case entirely (single
    literal-push resolution via search_byte_patterns, not array-walking);
    that's the technique used throughout Batch 1/2 above, not this script.
    2d382) that this session has now cast doubt on.

--------------------------------------------------------------------
Batch 2 -- ~94 "Unknown" category scraper-derived TABLE_2D_*/
TABLE_3D_* placeholder-axis tables (deliberately wrong/scaffolding
axis info, set only so EcuFlash renders the data -- see review.md
item 20). NOT YET STARTED. Same verification method applies, but
expect most axis addresses to require the "find the real reader"
approach (computed-pointer-array indirection, per review.md item 20's
unresolved High Octane Ignition Map 1 precedent) rather than a
literal call-site push, since item 20 already tried and failed the
literal-push approach across all 94 targets and all 22 known lookup
functions.

--------------------------------------------------------------------
SESSION HANDOFF (2026-08-01, continuation session) -- start here if
picking this up fresh. Everything below is ALSO logged in more detail
in review.md items 21-22; this is the condensed pointer + what to do next.
--------------------------------------------------------------------

WHAT HAPPENED THIS SESSION (separate track from Batch 1/2 above, but
directly useful to them):

1. RETURN-STORAGE AUDIT (review.md item 21). Found and fixed a real bug
   class: some dispatcher functions (table_lookup_interp @0x14656,
   table_3axis_interp_triple @0x148a2, table_lookup_indexed @0x14854) were
   declared `void` in Ghidra despite real callers capturing their return
   value -- the computed result was being silently discarded in every
   decompile. Fixed via:
     ghidra:set_function_prototype(function_address="<addr>",
       calling_convention="__stdcall_far_2arg",
       prototype="ushort <name>(byte ep_val, void *table)")
   CAVEAT: this call also reliably drops the `table` parameter's storage
   back to <UNASSIGNED> every time (pre-existing quirk, already documented
   in table_lookup_interp's own plate comment before this session touched
   anything -- NOT something this session broke). set_variable_storage
   cannot fix it either (tool says GUI or custom Pcode/HighVariable script
   required). STILL UNRESOLVED, real follow-up needed if param binding
   matters for further work.
   ~30 other functions checked, left alone (already correct -- see item 21
   for the full list so they're not re-checked from scratch).
   Two harder candidates flagged, NOT fixed: octane_trim_f262_f264_table_
   select's table_read_indexed usage (looks like a genuine multi-register
   return, extraout_R1+R2, needs its own investigation, do not naively
   ushort-ify table_read_indexed), and read_indexed_byte_f1fc (0x1481c, byte
   vs word return-width mismatch, noticed in passing).
   Follow-up grep sweep (bug signature: bare call immediately before
   `return;`) found 5 more superficial candidates, ALL verified as false
   alarms via real disassembly (not just decompile) -- egr_sequence_control,
   ign_advance_top_update/wgdc_correction_integrator_update,
   isc_openloop_stepper_scale_calc. No new bugs. Good confirmation the 3
   fixes were the real bugs, not the tip of an iceberg.

2. ARRAY-BASE INDIRECTION SCRIPT (review.md item 22) -- THE BIG ONE,
   directly unblocks this file's own Method step 4 caveat and item 10's
   "structural limitation, remains blocked" conclusion above. Diagnosis
   (credited to an external analysis document): Ghidra's static xref engine
   cannot and will never automatically discover table_lookup_indexed's
   array-base -> runtime-indexed-target relationship, because the real
   target address only exists after a runtime read of a global index
   variable (`array[*(0x1f1fc) & 7]`) -- there's no static address-of
   relationship to record. Fix: don't fight the limitation, encode the
   ROM's own convention directly via a Ghidra script that:
     - finds all real callers of table_lookup_indexed via
       get_function_callers (Ghidra DOES track the call itself fine --
       just not what happens inside)
     - backward-scans each call site for the pushed (offset, bank) pair
       (same method as the original table_lookup_interp extractor)
     - creates an explicit refMan.addMemoryReference(callSite,
       arrayBaseAddr, ...) -- the missing link
     - reads each array's 8 slots (4 bytes each: byte0=pad, byte1=bank,
       bytes2-3=offset) and adds a reference from each slot to its real
       resolved target
   Ran against all 14 real call sites (9 calling functions). 100% resolved
   cleanly. 5 cross-validated against already-known-correct tables
   (Warmup Retard 0x1264a, High Octane Ignition Map 2 0x120d2, Low/High
   Octane Fuel Map 1 0x11bc0/0x119b8, Desired ISCV step position 0x128a2).
   6 NEW table addresses surfaced: 0x128ae, 0x11880, 0x1288a, 0x12896,
   0x11cea, 0x1238e.
   XML CROSS-CHECK of those 6 (this file's actual job):
     - 0x128ae, 0x12896: ALREADY CORRECT in XML ("Desired ISCV intial step
       position with AC off-Drive", "Desired Idle RPM-Drive"). No action.
     - 0x11880: found as TABLE_2D_00011880, an "Unknown"-category Batch 2
       placeholder table. NEW LEAD for Batch 2 -- real caller now known
       (fueling_base_injection_calc via array 0x2da8c), gives a real
       subsystem identity even though axis still unresolved. PRIORITIZE
       this one in Batch 2 given the head start.
     - 0x1288a, 0x11cea, 0x1238e: NOT IN THE XML AT ALL (zero matches).
       Genuinely undocumented tables, not mis-addressed existing entries.
       Adding them is a bigger task than this file's address-only scope
       (needs type/axis/scaling from scratch). NOT attempted. Separate
       follow-up project, not folded into Batch 1/2.

IMMEDIATE NEXT STEPS (pick any):
  (a) Run the SAME array-base-indirection script technique against the
      OTHER indexed lookup functions in this ROM (table_read_indexed
      @0x14880, gear_indexed_table_lookup_f1fc @0x149a7, and any others
      sharing the same array-selector pattern) -- very likely to surface
      more real table addresses the same clean way, per review.md item 22's
      own method note. Not yet done.
  (b) Prioritize TABLE_2D_00011880 in the Batch 2 placeholder-axis project
      now that its real caller is known.
  (c) Continue Batch 2 generally (still NOT YET STARTED per this file's
      original bottom section) -- now with one more tool in the kit
      (the array-reference script) than when Batch 2 was originally scoped.
  (d) Investigate the 3 genuinely-undocumented tables (0x1288a, 0x11cea,
      0x1238e) as their own new-table-addition project.
  (e) Separately: the return-storage param-binding quirk (table_lookup_
      interp/table_3axis_interp_triple/table_lookup_indexed all losing
      their non-first parameter's storage on every set_function_prototype
      call) still needs a real fix -- would require either manual GUI work
      or a custom Pcode/HighVariable-level Ghidra script, per
      set_variable_storage's own tool response. Not attempted.

GOTCHAS TO REMEMBER (cross-session):
  - run_script_inline auto-wraps submitted code in its own class scaffold.
    Submit ONLY bare statements/method-body code, never a full "public class
    X extends GhidraScript" declaration, or the two wrappers nest and javac
    fails with "illegal start of expression" (see review.md item 19). Two
    dead orphan script files (TableExtract.java, TableExtractV2.java) exist
    from this mistake early on -- harmless, ignore the recurring stale
    compile-error preamble Ghidra prints because of them on every script
    run.
  - Always dry_run:true a new script before applying for real.
  - Bank-adjustment formula (confirmed across many independent call sites):
    real_address = bank*0x10000 + offset, from a push pair
    `mov:g.w #offset:16,@-SP` then `mov:g.w #bank:8,@-SP` immediately before
    the pjsr.
  - Array-slot format (confirmed across all 14 sites this session): 4 bytes
    per slot, byte0=pad/unused(0x00), byte1=bank, bytes2-3=offset (big-endian
    word).
  - Verification always needs TWO things before trusting a resolved
    address: byte-level plausibility (real header signature + sane data
    curve) AND either a get_xrefs_to 8/8 hit or a genuine live-traced caller
    -- neither alone is proof, per this file's own Method section above.

--------------------------------------------------------------------
13. ARRAY-BASE INDIRECTION SCRIPT extended to table_read_indexed (0x14880)
    (2026-08-01, follow-up session). Action item (a) from the handoff above.
    STATUS: DONE for table_read_indexed. gear_indexed_table_lookup_f1fc
    (0x149a7) checked too -- ZERO callers found in this ROM, nothing to run,
    recorded as a negative result rather than skipped.

    Script saved as a real file (not run_script_inline, to sidestep that
    tool's OSGi class-loading failures encountered this session -- see
    GOTCHA below) at:
      ghidra scripts/ResolveTableReadIndexedArrays.java (project copy)
      C:\Users\j.brophy.CORKILLSYSTEMS\ghidra_scripts\ (Ghidra's script path,
        copied there since run_ghidra_script does not search the project
        folder)
    Same technique as item 22 exactly, retargeted at table_read_indexed:
    get_function_callers -> backward-scan each call site for the
    (offset,bank) push pair -> addMemoryReference(callSite, arrayBase) ->
    read each array's 8 slots -> addMemoryReference(slot, resolvedTarget).
    Confirmed via decompile before running that table_read_indexed uses the
    identical array[global_index & 7] pattern as table_lookup_indexed
    (same 0x1f1fc index variable, same &7 mask).

    Dry-run first (clean), then applied for real. Both runs identical,
    10/10 call sites resolved, every array's 8 slots internally consistent
    (matches the pattern established on known-real tables). Full mapping:
      f286_knock_octane_secondary_trim_compute_via_table (0x240c4)
        -> array 0x2db0c -> table 0x11d5a
      f286_knock_octane_secondary_trim_compute_via_table (0x240f6)
        -> array 0x2daec -> table 0x11d58
      f286_knock_octane_secondary_trim_compute_via_table (0x24111)
        -> array 0x2daec -> table 0x11d58 (same array as the 0x240f6 site)
      table_select_scale_fanout_f1be_f1c2 (0x20ea1)
        -> array 0x2da8c -> table 0x11880 (matches item 22's
        fueling_base_injection_calc lead -- SECOND independent caller now
        resolving to the same array/table, strong cross-validation)
      octane_trim_f262_f264_table_select (0x23b6f, 0x23bb5)
        -> array 0x2daac -> table 0x11d62
      octane_trim_f262_f264_table_select (0x23b8b, 0x23bcf)
        -> array 0x2dacc -> table 0x11d6e
      isc_f420_correction_calc (0x27764)
        -> array 0x2d92c -> table 0x128a2 (matches already-confirmed-real
        Desired ISCV step position table -- cross-validation)
      isc_f41a_correction_calc (0x263db)
        -> array 0x2d8ec -> table 0x1288a (matches item 22's undocumented
        table lead -- SECOND independent cross-validation)

    Spot-verified live post-apply: get_xrefs_to(0x2da8c) now shows both
    table_select_scale_fanout_f1be_f1c2 (0x20ea1) and
    fueling_base_injection_calc (0x21a36, item 22's original find) pointing
    at the same array -- confirms the reference was actually written, not
    just printed.

    3 GENUINELY NEW table addresses surfaced (not seen in item 22's pass):
    0x11d5a, 0x11d58, 0x11d62, 0x11d6e (four addresses, two pairs sharing
    arrays). XML CROSS-CHECK NOT YET DONE this session -- next step, same
    as item 22's own follow-up pattern (check whether these already exist
    in the XML under existing names, or are undocumented like 0x1288a/
    0x11cea/0x1238e turned out to be).

    GOTCHA (new this session): run_script_inline failed twice with an OSGi
    "class could not be found" error (McpInline_<hash> not found by
    <bundle>) on both attempts, regardless of code content -- NOT the same
    failure mode as item 19's "illegal start of expression" (which was a
    genuine double-class-wrapper mistake). This looks like a Ghidra-side
    script-bundle/classloader issue rather than a code problem. WORKAROUND
    used: write the script as a real .java file via Pulsar's create-file,
    matching the public-class-per-filename convention already used by
    ReloadSleighLanguage.java in this project, then run it with
    run_ghidra_script (which requires the file to sit in Ghidra's own
    ghidra_scripts search path, not just the project's "ghidra scripts"
    folder -- copy it there first). This worked cleanly on the first try.
    Recommend this as the default approach going forward instead of
    run_script_inline, given two reproducible failures against zero
    successes this session.

    NEXT STEPS: (1) XML cross-check the 4 new addresses. (2) items (b)-(e)
    from the previous handoff are still open and unaffected by this entry.

    XML CROSS-CHECK FOLLOW-UP (2026-08-01, same session): the "4 genuinely
    new addresses" claim above does NOT hold up. Byte-verified: 0x11d5a,
    0x11d58, 0x11d62, 0x11d6e all land inside the unbroken data body of one
    already-known table, TABLE_2D_00011D4C (real header 02 00 F0C8 at
    0x11d4c, data runs uninterrupted through 0x11d79, next real header
    02 00 F0D0 at 0x11d7a is the already-confirmed Air Temperature
    Compensation, item 1). Same "OVERLAP framing is itself the bug" pattern
    as item 2. Did NOT add 4 new XML table entries -- would have repeated
    session 10's mistake. Instead annotated TABLE_2D_00011D4C's existing
    entry (in bac2.xml) with the caller-identity finding, and separately
    annotated TABLE_2D_00011880's entry with its now-doubly-confirmed
    fueling-subsystem caller identity. See bac2.xml inline comments for
    full detail. Lesson: 8/8 xref confirmation on an array proves the
    ARRAY is real, not that its resolved target is a novel table distinct
    from ones already in the XML -- always byte-check a "new" address
    against neighboring known headers before treating it as undocumented.

--------------------------------------------------------------------
14. HANDOFF ACTION ITEM (a) CLOSED -- no more array-base-indirection
    functions exist in this ROM (2026-08-01, follow-up session).
--------------------------------------------------------------------

STATUS: NEGATIVE RESULT, item (a) exhausted. Searched for additional
table_lookup_indexed/table_read_indexed-style array-base-indirection
targets to extend the resolver script to, using three independent
methods, all converging on the same answer:

1. NAME-BASED: searched all functions matching "indexed" (7 hits) and
   a user-supplied xref-count candidate list (~35 functions spanning
   axis_lookup_interp/table_lookup_interp callers, status-word table
   dispatchers, and ISC/O2/knock table selectors). Decompiled every
   candidate not already covered. All either (a) already-known single-
   table literal-push callers (axis_lookup_interp/table_lookup_interp
   with a baked-in address, no runtime array selection), (b) a
   different table type entirely (status-word RAM-pointer tables, not
   ROM value/axis tables), or (c) not table-related at all (scalar
   selects, status composites). table_select_scale_fanout_f1be_f1c2
   confirmed as an already-known table_read_indexed CALLER, not a new
   indirection function itself.

2. VARIABLE-BASED: grepped the exported .c for the f1fc global index
   variable directly (29 hits) to catch inlined/anonymous consumers a
   name search would miss. Found 3 new call sites (lines 10395, 23119,
   23172/23175) beyond the known functions -- all three read small
   fixed-size RAM structs (hardware register emulation / gear-config
   snapshot / EEPROM backup dispatch), not ROM table-pointer arrays.
   Confirmed false positives by reading surrounding context (timer
   register dispatch, EEPROM backup write, port state cluster).

3. STRUCTURAL (most reliable): grepped the full 28,064-line decompiled
   source for the actual computational fingerprint of the pattern --
   `& 7) * 4` / `& 7) * 2` (masked-index-into-4-or-2-byte-stride-array)
   and a bare `* 4)` array-slot multiply. RESULT: exactly 3 matches,
   all already known -- table_read_indexed (0x14880, 14 real callers),
   table_read_word_indexed_f1fc (0x14838, 0 callers), and
   gear_indexed_table_lookup_f1fc (0x149a7, 0 callers). Also checked
   the exact (byte,void*) two-arg signature shared by table_lookup_
   indexed/table_read_indexed/axis_lookup_interp -- only those three
   functions in the whole ROM share that shape.

CONCLUSION: the 4-byte bank+offset table-pointer array-selector idiom
this ROM uses (confirmed items 13/22) has exactly two live consumers,
table_lookup_indexed and table_read_indexed, both already fully
resolved. The two structurally-identical siblings
(gear_indexed_table_lookup_f1fc, table_read_word_indexed_f1fc) are
confirmed dead code paths (zero callers each). HANDOFF ACTION ITEM (a)
IS CLOSED -- no further indirection-script extension work exists to
do in this ROM. Confidence level here is higher than the earlier name-
based checks alone, since this was corroborated by a search over the
literal decompiled source rather than function-name pattern matching.

NEXT: proceeding to handoff action item (c), general Batch 2 pass over
the ~94 "Unknown"-category placeholder tables using the established
single-table literal-push method (Method step 4) now that indirection
is confirmed exhausted as an avenue for this specific ROM.

--------------------------------------------------------------------
Batch 2 -- individual items (started 2026-08-01, follow-up session)
--------------------------------------------------------------------

15. TABLE_2D_00011880 -- first Batch 2 table resolved, using the head
    start from item 22 (fueling_base_injection_calc lead, doubly
    cross-validated in item 13 via table_select_scale_fanout_f1be_f1c2).
    STATUS: CONFIRMED, XML APPLIED (test/rvr/xml/ecuflash/new/
    21000011_1997-2001_RVR_X3_Mt__4g63t_bac2.xml -- CORRECTED 2026-08-01:
    initially misapplied to the base _.xml file by mistake, caught and
    reverted, re-applied to the correct working copy _bac2.xml, which is
    the file items 13/22's caller-identity annotations already live in).
    Header confirmed live: 02 00 F0C0 at 0x11880 (mode=2), data
    50 50 57 FF at 0x11884 (header+4 convention, same recurring bug
    as Batch 1's items 1/2/7/8/9). NOTE (2026-08-02, item 16): the name-only
    suffix pass below applied "_Fueling" directly to TABLE_2D_00011880's own
    name in the live working XML (not bac2.xml, which does not exist in this
    checkout -- see item 16), separate from this address/axis fix. Full disassembly of
    fueling_base_injection_calc (0x21a0e) read directly (not just
    decompiled) to nail down which axis feeds this specific table,
    since the function contains three table lookups sharing one
    axis_lookup_interp call: axis_lookup_interp(0xd0f0, bank=2) at
    entry, then a btst-gated array select (0xda6c/0xda8c) into
    table_lookup_indexed (-> this table, 0x11880/11884), then two
    more literal table_lookup_interp calls (0x118a0, 0x11894 --
    Batch 2 neighbors, not yet independently verified). Per Method
    step 4, all three share the single axis_lookup_interp result.
    Axis header confirmed at 0x2d0f0: F0C0 F170 0010 (16 elements),
    X-ptr F0C0 matches the table's own axis-ptr field exactly. Data
    is a clean 16-element linear ramp (0x80..0x480 in steps of 0x80).
    XML APPLIED: address 11880 -> 11884, axis RPM address 2d332(18)
    -> 2d0f0(16). Old 2d332 placeholder confirmed unrelated (Batch 1
    items 9/10/12 already established 2d332 as a different ISC-only
    axis family).
    FOLLOW-UP LEAD: the same function also literal-pushes 0x118a0 and
    0x11894 as direct table_lookup_interp targets, sharing this same
    axis (0x2d0f0). Both are also Batch 2 "Unknown" placeholders
    (TABLE_2D_0001186C's neighbor and TABLE_2D_00011894 itself) --
    strong candidates for the next items in this pass, already having
    a confirmed caller and axis without further search needed.

--------------------------------------------------------------------
16. NAME-SUFFIX PASS over remaining Batch 2 placeholders using
    FindBatch2Callers.java / DumpBatch2Headers.java (2026-08-02).
--------------------------------------------------------------------

    STATUS: DONE for this pass. Note upfront: the working XML in this
    checkout is test/rvr/xml/ecuflash/new/21000011_1997-2001_RVR_X3_Mt__4g63t_.xml
    directly -- "bac2.xml" as referenced by earlier entries above does not
    exist in this checkout, so all name-suffix edits below were applied to
    that file. Most of the ~94 Batch 2 entries already carry a caller-derived
    "_Subsystem" name suffix from earlier sessions (e.g. _ISC, _Fueling,
    _Knock, _CompositeCorr, _TCU); this pass targeted only the ones that
    were still missing a suffix despite a confirmed caller existing.

    Ran both scripts (dry_run first, both clean, no writes needed since both
    are read-only dumps/scans):
      DumpBatch2Headers.java -- confirmed all Batch 2 headers still read as
        expected (mode=2/3 table headers for ROM addresses; the three 2D
        axis-record addresses in the list, 2D3C4/2D5F0/2D66C, correctly read
        as garbage mode bytes when interpreted as table headers, consistent
        with items 8/10/11's established finding that those are axis
        records, not table records).
      FindBatch2Callers.java -- literal-push scan across the whole program
        for each Batch 2 address's (bank,offset) pair.

    Cross-referenced FindBatch2Callers' output against the current XML name
    attributes. Found 17 entries with a confirmed literal-push caller but no
    suffix yet applied, plus 1 more (TABLE_2D_00011880) whose caller was
    already known from item 15/22 via array-indirection (not a literal push,
    so FindBatch2Callers correctly reports "NO CALLER FOUND" for it -- that
    script only catches the direct-call convention, not the array-indexed
    one). Applied name-only suffixes for all 18, address/axis untouched:

      11880 -> _Fueling   (fueling_base_injection_calc via array 0x2da8c,
                            cross-validated twice, items 13/15/22 -- indirect
                            caller, not caught by this pass's literal-push
                            scan, added from prior findings instead)
      12DE4 -> _ISC       (isc_condition_eval)
      12EA2 -> _EGR       (egr_duty_target_calc)
      12EAE -> _EGR       (egr_duty_target_calc)
      12EBC -> _ISC       (isc_f402_composite_correction_calc)
      12ECE -> _ThresholdSelect  (tiered_threshold_table_select)
      12EDC -> _ThresholdSelect  (tiered_threshold_table_select)
      12EEA -> _ThresholdSelect  (tiered_threshold_table_select)
      12EF8 -> _ThresholdSelect  (tiered_threshold_table_select)
      12F06 -> _TCU       (tcu_dtc_status_check_dispatch)
      12F14 -> _TCU       (tcu_dtc_status_check_dispatch)
      1313E -> _TCU       (tcu_shift_solenoid_table_init)
      131A0 -> _ISC       (isc_openloop_target_calc | isc_openloop_stepper_scale_calc)
      131A8 -> _ISC       (isc_openloop_target_calc | isc_integrator_update)
      131B0 -> _ISC       (isc_integrator_update)
      131D4 -> _DualChannel  (f276_f27a_dual_channel_compute_via_table)
      131E0 -> _DualChannel  (f276_f27a_dual_channel_compute_via_table)
      13212 -> _TCU       (tcu_torque_converter_slip_calc)

    Everything else FindBatch2Callers reported "NO CALLER FOUND" for
    (1186C, 11D20, 11EA0, 11F6C, 12AE6, 12D00, 12DF2, 12E4A, 1314C, 13154,
    1315C, 13168, 13174, 1317C, 13188, 13190, 13198, 131B8, 131C0, 131CC,
    131EC, 131F6, 13202, 1320A, 1321C, 13236, 13250, 1325E, 1326C, 1327A,
    2D3C0, 2D5EC, 2D668, 2D796, 10D73) was left with no suffix, per Method
    step 6 -- never guess a subsystem without a confirmed caller. These
    remain candidates for the array-indirection technique or a fresh
    literal-push search if new call sites are discovered later, same as
    the still-open items above (10D73/1186C/etc. genuinely have no direct
    or indirect caller found by any method run so far).

    NEXT STEP: none of the remaining "NO CALLER FOUND" addresses are
    resolvable by literal-push or the existing array-indirection scripts
    (per item 14's exhaustive negative-result conclusion -- there is no
    third indirection function left to extend to). Any further progress on
    those would need a different technique entirely (e.g. manual trace of
    dispatch tables, per item 10's closing note), not a rerun of these two
    scripts.

--------------------------------------------------------------------
17. AXIS PASS over the newly-named Batch 2 tables using FindBatch2Axes.java
    (new script, 2026-08-02).
--------------------------------------------------------------------

    STATUS: DONE for this pass, 23 of 24 candidate leads applied (one 3D
    entry -- 12DF2/12E4A were never candidates; see below).

    METHOD: wrote FindBatch2Axes.java, which reuses FindBatch2Callers'
    push-pair/pjsr event tracking but, for each Batch 2 table address with a
    known caller, looks at the immediately-PRECEDING push-pair+pjsr event in
    the same function. If that preceding event's pjsr target is
    axis_lookup_interp (0x14735), its pushed (bank,offset) is the candidate
    axis -- this is exactly the manual pattern established in items 11/15
    (axis_lookup_interp call sits right before the table's own
    table_lookup_interp/table_lookup_indexed call). Ran dry_run (script is
    read-only, no writes), copied to both the project's ghidra scripts/
    folder and Ghidra's own ghidra_scripts search path per the item 13
    gotcha.

    Produced a verdict per address: AXIS_LOOKUP_INTERP MATCH (usable lead),
    "preceding event is ANOTHER TABLE call" (this table is chained after
    another table's axis, not paired with its own -- e.g. many of the
    _CompositeCorr/_CompositeCalc/_ISC entries turned out to share axes with
    a sibling table rather than having their own dedicated axis_lookup_interp
    call), "NO PRECEDING PUSH-EVENT" (table call is the first in its
    function), or "NO CALLER FOUND" (inherited from the caller pass).

    24 addresses came back AXIS_LOOKUP_INTERP MATCH. Per Method step 4, a
    caller/axis match is a LEAD, not proof -- read all 20 unique candidate
    axis addresses' first 6 header bytes via ghidra:read_memory before
    touching the XML. All 20 read back a plausible axis-record shape (two
    2-byte RAM tags + a 2-byte element count in the 4-17 range, no garbage),
    consistent with the established axis-header convention. One, 0x2d356,
    matches the 16-element record already flagged as real-but-unassigned
    back in item 10 -- good independent cross-validation.

    Applied 23 of the 24 (11880 was already resolved via item 15/22's
    array-indirection route, so it's not re-logged here) as address+element
    corrections to the live XML, replacing the RPM/2d332/18 placeholder.
    18 are 2D tables with a single axis, fully resolved:

      11DEC -> 2d260 (7 elem)     11E4A -> 2d130 (8 elem)
      11E6A -> 2d440 (11 elem)    11F64 -> 2d2aa (14 elem)
      127FC -> 2d490 (8 elem)     12824 -> 2d274 (9 elem)
      1284E -> 2d180 (6 elem)     12858 -> 2d29c (4 elem)
      12950 -> 2d1fa (5 elem)     12CD2 -> 2d548 (17 elem)
      12D70 -> 2d09a (16 elem)    12DD6 -> 2d116 (10 elem)
      12DE4 -> 2d5ee (9 elem)     12EA2 -> 2d620 (8 elem)
      12EF8 -> 2d636 (10 elem)    12F14 -> 2d636 (10 elem)
      1313E -> 2d66a (10 elem)    13212 -> 2d636 (10 elem)

    6 are 3D tables where only the Y-axis was found this pass (X-axis still
    needs a second, further-back push-event search the script doesn't do
    yet -- flagged inline as unresolved, X-axis left on the 2d332
    placeholder rather than guessed):

      127A8(Y) -> 2d75a (7 elem)   12B06(Y) -> 2d50c (15 elem)
      12B94(Y) -> 2d356 (16 elem)  131A0(Y) -> 2d7b0 (10 elem)
      131A8(Y) -> 2d7b0 (10 elem)  131B0(Y) -> 2d7b0 (10 elem)

    Left untouched (no axis_lookup_interp match, per Method step 6 -- never
    guess): every "NO CALLER FOUND" address from item 16, plus all the
    "ANOTHER TABLE call" verdicts (11834, 11E86, 11E94, 11EC8, 11ED6, 11EE2,
    11F16/30/3C/4A/5C, 127D8, 12860, 12CBC, 12EAE, 131D4/131E0) and all the
    "NO PRECEDING PUSH-EVENT"/"inconclusive" verdicts (11D4C, 128D2, 128DE,
    128EA, 12926, 1297A, 12986, 12CE8, 12CF4, 12EBC, 12ECE/EDC/EEA, 12F06).
    These remain candidates for a follow-up pass that walks further back
    than one event, or that handles chained-table axis sharing explicitly.

    NEXT STEP: extend FindBatch2Axes.java to look two events back (not just
    one) for the 6 still-open 3D X-axes, and to follow the "ANOTHER TABLE
    call" chain (i.e. when the preceding event is itself a table call, walk
    back one more step from there) for the composite/ISC entries that share
    an axis with a sibling table rather than owning one directly.

    UPDATE (2026-08-03) -- ALL 6 X-AXES RESOLVED, ITEM CLOSED. Manually applied
    the "two events back" extension described above by disassembling each
    caller directly (isc_f34e_f34a_correction_calc, canister_purge_duty_calc_f4ac,
    isc_f374_correction_calc, isc_openloop_stepper_scale_calc,
    isc_integrator_update x2) instead of extending FindBatch2Axes.java.
    Confirmed the same pattern holds in every case: TWO axis_lookup_interp
    calls occur back-to-back immediately before the table's own
    table_lookup_interp call -- the first (further back) is the X-axis, the
    second (immediately preceding) is the already-known Y-axis. All 6 X-axis
    headers read back a plausible, well-formed axis-record shape (F0C0/F0C2
    RAM tags, sane element counts), same verification standard as the
    original Y-axis pass:

      127A8 -> X=2d74c (4 elem)     [isc_f34e_f34a_correction_calc]
      12B06 -> X=2d530 (9 elem)     [canister_purge_duty_calc_f4ac]
      12B94 -> X=2d32c (18 elem)    [isc_f374_correction_calc]
      131A0 -> X=2d76e (8 elem)     [isc_openloop_stepper_scale_calc]
      131A8 -> X=2d784 (7 elem)     [isc_integrator_update]
      131B0 -> X=2d856 (10 elem)    [isc_integrator_update]

    NOTABLE CROSS-VALIDATION: 12B94's X-axis (0x2d32c, 18 elements, header
    F0C0 F170 0012) is the exact same confirmed-real RPM axis already used
    for "High Octane Ignition Map 1" elsewhere in this file (item 12) -- an
    independent third confirmation of that axis record's address, and a
    useful reminder that the ubiquitous 2d332 placeholder used everywhere
    else in this file is that same axis's DATA START (2d32c+6), not its
    header, the same recurring header-vs-data-start bug documented
    elsewhere in this file.

    XML APPLIED (2026-08-03): all 6 X Axis entries replaced (previously the
    2d332/18 placeholder) with the addresses/element-counts above. Verified
    post-edit via grep that only these 6 tables' X Axis lines changed and
    every other still-open table in the file correctly retained the 2d332
    placeholder. Item 17 is now fully closed -- no remaining action needed;
    FindBatch2Axes.java itself was NOT extended (this was done via manual
    per-function disassembly instead), so a future Batch-2-style sweep would
    still benefit from the script extension if more tables surface later.

--------------------------------------------------------------------
18. DIRECT-XREF CHECK on everything still "NO CALLER FOUND" (2026-08-02).
--------------------------------------------------------------------

    STATUS: DONE. 1 of 35 resolved (1186C); the other 34 confirmed to have
    zero code references by any method, not just a gap in the push-pair
    scanner.

    METHOD: ran ghidra:get_bulk_xrefs directly against all 34 remaining
    "NO CALLER FOUND" addresses (from item 16) plus 10D73, in one batch call
    -- this checks Ghidra's own reference database rather than re-scanning
    instructions, so it can catch patterns the literal-push scanner's
    specific mov/pjsr pattern-match doesn't cover (e.g. data-only refs,
    refs planted by a prior script run).

    33 addresses came back with zero xrefs of any kind -- confirms these
    are genuinely unreferenced by anything Ghidra's analysis has found, not
    just missed by the literal-push convention specifically. Consistent
    with the item 14 "exhaustive negative result" conclusion.

    1186C came back with 8 DATA xrefs from 0x2da6c-0x2da88 (4 bytes apart --
    a pointer array, same signature ResolveTableReadIndexedArrays.java
    plants). Read the array: all 8 slots hold the identical value 0x1186c
    (deliberate uniform array, not an index dispatch). Checked this array
    against ResolveTableReadIndexedArrays.java's known 10 call sites (fresh
    dry_run) -- not among them, so this is a genuinely new, 11th indirection
    array, previously undiscovered by any prior pass.

    Wrote a one-off script (FindArrayCaller.java, same push-pair/pjsr event
    tracking as FindBatch2Callers/Axes, pointed at just 0x2da6c) to find
    what pushes this array's address. Found it at 0x21a2a, inside
    fueling_base_injection_calc -- the SAME function that already owns
    11880/11894/118A0 (item 15/22). The push sequence there is:
    axis_lookup_interp(0x2d0f0) -> push 0x2da6c -> push 0x2da8c -> single
    pjsr to table_lookup_indexed. No pjsr occurs between the two array
    pushes, meaning that ONE call takes both array-base arguments (a
    dual-table blend/interpolation), and 0x2da8c is the already-known array
    resolving to 11880. So 1186C shares the exact same axis (0x2d0f0) as
    its three siblings.

    Verified axis 0x2d0f0's header byte-for-byte: F0C0 F170 0010 (16
    elements), a plausible axis-record shape. Applied to XML: renamed
    TABLE_2D_0001186C -> TABLE_2D_0001186C_Fueling, axis address/elements
    added (2d0f0 / 16, replacing the RPM/2d332/18 placeholder), full
    derivation logged inline.

    This raises an open question worth flagging for the next session: since
    table_lookup_indexed apparently takes two array arguments per call, the
    other ~9 already-resolved indirection arrays from
    ResolveTableReadIndexedArrays.java's output (item 13/14/22) may each
    have an unnoticed SIBLING array immediately preceding them in the same
    push sequence, the same way 0x2da6c precedes 0x2da8c. None of those
    call sites were re-examined for a second array this session -- only
    fueling_base_injection_calc's two known call sites (0x21a33/0x20ea1
    per item 13/15) were involved here, and only because 1186C's xref check
    happened to surface the sibling push at 0x21a2a. Worth a systematic
    "check one push-event earlier, is it also an array-base with no pjsr
    between them" pass across all 10 known table_read_indexed/
    table_lookup_indexed call sites -- may surface more previously-invisible
    tables the same way.

--------------------------------------------------------------------
19. FixBankOffsetReferences_v2.java -- ROM-agnostic rewrite of the manual
    bank+offset reference-creation workaround (2026-08-02), validated and run
    for real against the RVR ROM.
--------------------------------------------------------------------

    CONTEXT: item 22's original array-reference script (and item 27's
    AnnotateBankOffsetCallSites.java, both this-ROM sessions) used a
    backward-immediate-scan to resolve bank*0x10000+offset for every real
    caller of table_lookup_interp/table_lookup_indexed/table_3axis_interp_triple,
    working around the Ghidra #817-class decompiler bug (confirmed applies here,
    item 26) that never fuses the two visible call arguments into a real far
    pointer. PJ wrote FixBankOffsetReferences_v2.java to generalize this
    technique so it isn't tied to this specific ROM's addresses/bank layout.

    GENERALIZATION CHANGES from the original this-ROM-specific script:
      - Target functions resolved by NAME via the symbol table at runtime,
        not hardcoded addresses -- works on any binary where the callees are
        labeled, regardless of where they land.
      - Bank plausibility filter changed from a hardcoded {1,2} allowlist to
        pure memory-containment (mem.contains) -- a ROM with a different bank
        range no longer gets legitimate call sites silently rejected.
      - Added a FUNCTION-BOUNDARY check: the backward scan can never cross
        into the caller's own caller. Alone, this reproduces item 27's known
        table_3axis_interp_triple_wrapper false-positive rejection without
        needing to know that function's identity in advance.
      - Added an ADJACENCY check, refined twice during validation (see below).

    VALIDATION METHOD: ran dry_run against the live RVR program (146 total
    call sites across the 3 targets, matching item 27's count exactly) and
    diffed against item 22's independently-derived ground-truth array list
    and item 27's known-false-positive list.

    ROUND 1 (naive adjacency -- any non-qualifying-mov instruction breaks the
    pattern): 137/146 added, 9 skipped. Cross-checked against item 22's 4
    ground-truth isc_f34e_f34a_correction_calc arrays (0x2dbac/0x2da0c/
    0x2db8c/0x2dbcc) -- all 4 matched exactly, confirming the core logic
    sound. But of the 9 skips, only 3 were item 27's known bad rows
    (isc_f438_correction_calc, egr_duty_target_calc's bank=252 sites,
    table_3axis_interp_triple_wrapper). The other 6 needed individual
    disassembly review.

    DISASSEMBLY REVIEW OF THE UNEXPECTED SKIPS:
      - table_lookup_indexed's own internal call to table_lookup_interp
        @0x14874: NOT a real miss. Disassembly shows this call is fed by
        "mov R0,@-SP" / "mov R1,@-SP" -- REGISTER pushes (the runtime
        array-slot values from table_lookup_indexed's own indexing logic,
        per item 22), not immediate literals. Item 27's script had actually
        flagged this as a "GOOD CONFIRMATION" (resolved to a plausible-looking
        address) but that confidence was misplaced -- it was coincidentally
        grabbing unrelated nearby immediates (the "&7" mask constant etc.),
        not this call's real (runtime-computed) arguments. v2 correctly
        rejects it; the OLD script's "success" here was itself a latent bug,
        now caught.
      - The remaining 3 real misses (f2ea_f2ee_composite_correction_calc
        @0x24907, fueling_base_injection_calc @0x21a5a,
        f2e0_f2ec_composite_correction_calc @0x24792) were confirmed via
        direct disassembly to be GENUINE valid call sites with correct
        immediate pushes, broken only by a harmless interleaved
        "mov:f.w Rn,@(-N:8,FP)" frame-spill instruction between the second
        push and the pjsr -- a real compiler pattern, not a decode error.
        The naive adjacency rule was too strict.

    ROUND 2 (fixed adjacency): distinguished "any instruction" from
    "an instruction that pushes onto @-SP". Frame spills, register moves,
    and other non-stack-affecting instructions are now treated as
    transparent filler; only a genuine rival push to @-SP (immediate or
    register) breaks adjacency, since that's the only thing that could
    actually displace what "top of stack" means for the call. Re-ran
    dry_run: all 3 previously-missed genuine sites now resolve correctly
    (0x24907->0x11f30, 0x21a5a->0x11894, 0x24792->0x11e86), zero regressions
    on the previously-working 137, and the 3 true false positives plus the
    0x14874 register-push case remain correctly rejected.

    FINAL RESULT: 142/146 call sites resolved and applied for real (not just
    dry-run) -- MORE accurate than item 27's original 146-applied/0-flagged
    result, since item 27 never caught the 0x14874 latent bug and this
    version does. The 4 correctly-unresolved sites: table_lookup_indexed's
    own internal call (runtime register args, no static immediate exists to
    record), the 2 known bank=252 mis-grab sites (isc_f438_correction_calc,
    egr_duty_target_calc's first call), and table_3axis_interp_triple_wrapper
    (genuinely arg-less passthrough). None of these 4 have a real static
    bank+offset immediate pair to record, so leaving them unresolved is
    correct, not a gap.

    Script saved at "ghidra scripts/FixBankOffsetReferences_v2.java" (project
    copy) and C:\Users\j.brophy.CORKILLSYSTEMS\ghidra_scripts\ (Ghidra's
    search path, per the usual copy-before-running gotcha).

    STATUS: reference-database fix APPLIED (real run, not dry_run) to the
    live RVR program. This is complementary to, not a replacement for,
    item 24/25's decompiler param-storage fix -- per item 26, the decompiler-
    display layer and the reference-database layer are independent; this
    script only fixes the latter (get_xrefs_to / Listing XREF / navigation),
    not the #817-class decompiler pointer-fusion bug itself (which remains
    unfixed and un-fixable without a segmentop/constresolve-style mechanism).

--------------------------------------------------------------------
20. XML sweep using item 19's new/fixed references (2026-08-02).
--------------------------------------------------------------------

    STATUS: 2 XML entries corrected, 1 family of negative results confirmed
    and logged (not fixed -- no axis exists to find here).

    FIX 1 -- TABLE_2D_00011880_Fueling axis element was STALE despite its
    own inline comment claiming item 15 already resolved it: comment said
    "Address/axis already resolved per review.md item 22 / review2.md item
    15" but the actual <table name="RPM"> line still read address="2d332"
    elements="18" (the old, unrelated ISC-only placeholder, per items
    9/10/12). Re-verified via disassembly: axis_lookup_interp(0xd0f0,
    bank=2) @0x21a19 in fueling_base_injection_calc, immediately preceding
    this table's own call. Corrected to address="2d0f0" elements="16" to
    match what was already documented as true but never actually applied to
    the XML. Lesson: an inline comment claiming a fix was applied is not
    proof it was -- always re-check the actual attribute value, not just the
    comment text next to it.

    FIX 2 -- TABLE_2D_00011894_Fueling had NO axis annotation at all, still
    on the stale 2d332/18 placeholder. This table's call site
    (fueling_base_injection_calc @0x21a5a) was one of the 3 real call sites
    FixBankOffsetReferences_v2.java's adjacency-fix round resolved today
    (item 19). Direct disassembly confirms it shares the exact same
    axis_lookup_interp(0xd0f0, bank=2) @0x21a19 call as its sibling 0x11880
    above -- consistent with Method step 4's axis-sharing rule and
    review2.md item 15's own follow-up note anticipating this. Corrected to
    address="2d0f0" elements="16".

    NEGATIVE RESULT (logged, not a fix) -- the _CompositeCorr family in
    f2ea_f2ee_composite_correction_calc (0x11f16, 0x11f30, 0x11f3c, 0x11f5c)
    and f2e0_f2ec_composite_correction_calc (0x11e86, 0x11e7a, 0x11e94,
    0x11f22, plus 0x11f16 again) all still sit on the stale 2d332/18
    placeholder. Checked both functions' FULL disassembly directly (not just
    decompile) looking for a preceding axis_lookup_interp call, per the same
    method that resolved 0x11880/0x11894 above -- NEITHER function contains
    any axis_lookup_interp (0x14735) call anywhere in its body. Each
    table_lookup_interp call here is fed by a single scalar read from RAM
    (@0xf28e, @0xf20e-derived flags, or plain register arithmetic), not an
    axis-record lookup. Per Method step 6 (never guess), the 2d332
    placeholder stays as-is for all of these -- there is no discoverable
    real axis via the literal-push method for this table family. Would need
    a fundamentally different technique (manual trace of the RAM value's own
    producer elsewhere in the ROM, or accepting these tables may not use a
    real breakpoint-axis at all) to resolve further. Not attempted this
    session.

    METHOD NOTE: this sweep was possible specifically because item 19's
    FixBankOffsetReferences_v2.java resolved 3 previously-unresolved call
    sites (round 2's adjacency fix) that happened to land on/near existing
    Batch 2 XML entries -- a useful side effect of generalizing the script
    beyond its original scope.

--------------------------------------------------------------------
18. SCRAPER BUG FOUND AND FIXED: phantom 2D tables from unclaimed
    scaling-table byte ranges (2026-08-02).
--------------------------------------------------------------------

Four Batch 2 "Unknown" entries (TABLE_2D_0002D3C0, _0002D5EC, _0002D668,
_0002D796) were all false positives from the same root cause, found while
chasing an axis conflict for TABLE_2D_00012DE4 (see item 17's D5EE entry).

ROOT CAUSE: h8539_import_tables_xml.py's Step 8 runs multiple independent
byte-scanning passes over the same ROM region -- Pass 2 finds scaling/axis
tables (6-byte header: out_ptr, in_ptr, 0x00, count, then count*2 bytes of
data), Pass 3 finds 2D/3D value tables (4- or 7-byte header starting with
a mode byte 0x02/0x03). Neither pass tracked which byte ranges the other
had already claimed. In all four cases, a real axis record's OWN trailing
data word happened to equal 0x0200 -- which Pass 3's 2D-table gate reads
as mode=0x02, pad=0x00, then the following 2 bytes as an axis pointer
that (by further coincidence) fell in-range. Confirmed for all four by
reading the preceding axis record in full and showing its data run ends
exactly at the "table header" address Pass 3 had flagged.

This is a physical byte-ownership bug, not a "logical reference" bug --
it has nothing to do with multiple value tables legitimately sharing one
real axis (that's normal and unaffected; axis_ptr is a RAM address read
from a value table's own header, not a ROM byte-range claim).

FIX APPLIED to h8539_import_tables_xml.py: added a `claimed_ranges` list
+ `in_claimed_range()` helper. Pass 2 now records (start, end_exclusive)
for every scaling table it accepts; Pass 3's 2D and 3D header-shape gates
now both additionally require `not in_claimed_range(i)`. Since Pass 2
runs before Pass 3 in the same script execution, this prevents Pass 3
from ever re-claiming a byte range Pass 2 already consumed. Verified the
scaling-table gate's byte pattern (F0-F7, E0-FE, 0x00, 2-0x90) does match
the real axis record shape confirmed live in Ghidra, so Pass 2 should
already have been claiming these ranges -- this fix closes the gap.

All four phantom XML entries deleted with corrected reasoning (see their
individual delete-comments in the XML). Next full re-scrape should not
regenerate them.

--------------------------------------------------------------------
19. Two more overlap flags resolved from fresh scraper run (2026-08-02,
    post claimed_ranges fix): pre-existing (non-Batch-2) tables.
--------------------------------------------------------------------

Fresh scraper output confirmed the claimed_ranges fix worked (D3C0/D5EC/
D668/D796 no longer appear as phantom H8539F-TABLE entries). It also
surfaced two NEW H8539F-AXIS-DATA-OVERLAP flags, both on established
(non-Batch-2) tables, not Batch 2 placeholders:

- Boost Limit: axis was 2d100 (mid-data of 2d0f0's own record, same
  false-positive shape as the deleted phantoms). Table's own header
  (11cea) expects axis-ptr F0C4; only real F0C4-tagged 10-element axis
  in range is 2d080. Fixed 2d100 -> 2d080, element count unchanged (10).

- Post Start Enrich Trim vs Load: axis was 2d35e (mid-data of 2d356's
  own record). Table's own header (11d86) expects axis-ptr F0C0; two
  F0C0-tagged 14-element candidates exist (2d2aa, already claimed by
  item 17's TABLE_2D_00011F64_HysteresisTimer; 2d2e0, unclaimed). Used
  2d2e0. Fixed 2d35e -> 2d2e0, element count unchanged (14).

Both fixes are TAG-MATCHED, not caller-confirmed (no literal-push caller
found for either table) -- weaker evidence than item 17's caller-traced
fixes, flagged as such in the XML comments. Worth revisiting if a caller
ever surfaces.

--------------------------------------------------------------------
20. FindBatch2Callers.java UNDERCOUNTING BUG FOUND (2026-08-02):
    functions calling MULTIPLE Batch 2 tables only had their LAST call
    site's table matched. Two genuinely missing tables found and added.
--------------------------------------------------------------------

While spot-checking a "NO CALLER FOUND" address (0001313E's neighbors)
by reading tcu_shift_solenoid_table_init's full disassembly by hand
(not just script output), found the function calls table_lookup_interp
13 times total, but FindBatch2Callers.java's original run only reported
ONE match for this function (0001313E). The other 12 calls include:
- 5 already-known tables (Knock Sensor Filter Maps 1-5) -- good sanity
  check, confirms the technique works.
- 2 addresses (131C4, 131FA) that had NO XML entry at all -- true gaps,
  not axis-resolution issues. Both confirmed real via live get_xrefs_to
  (6 and 5 DATA xrefs respectively, all from this one function) and live
  header bytes: 03 00 F0C0 F0C2 0B/0A FF -- real 3D headers, height=11/10,
  but the byte immediately after height is the 0xFF sentinel, meaning
  ZERO data rows in this ROM. Real but empty/degenerate tables, not
  detection errors.

ADDED to XML: TABLE_3D_000131C4_TCU, TABLE_3D_000131FA_TCU, both with
full caller/xref evidence, both flagged as legitimately empty (height
field is real, data is zero rows).

IMPLICATION: any OTHER function that calls table_lookup_interp/indexed
multiple times for DIFFERENT Batch 2 addresses may have the same
undercounting problem in the original FindBatch2Callers.java output.
The remaining ~28 "NO CALLER FOUND" Batch 2 addresses have NOT been
re-checked against this specific failure mode yet -- worth a targeted
follow-up pass (e.g. rerun the caller search collecting ALL matches per
function, not just the last one, before concluding an address has no
caller). Not fixed this session; flagged for next pass.

--------------------------------------------------------------------
21. Whole-program caller scan vs XML (FindAllTableCallsVsXmlV2.java,
    2026-08-02): 3 more genuinely missing tables found and added.
--------------------------------------------------------------------

Built a proper version of item 20's ad-hoc discovery: a script that scans
the ENTIRE program (not a pre-built candidate list) for every literal-push
call to table_lookup_interp/indexed/read_indexed, then diffs against every
address in the live XML file (read directly, not hand-transcribed, to
avoid copy errors). Checked addr, addr+4, addr-4 against the XML to allow
for the header-vs-header+4 addressing convention seen throughout this
project. 151 total calls found; 6 flagged as genuinely missing after
filtering array-base addresses (17 of those, informational only, not
table candidates -- input to table_lookup_indexed/read_indexed, already
handled by items 13/22).

Of the 6: 3 were false positives from the diff script's own limitation
(only checked +/-4, but this ROM's 3D tables use a 7-byte header, so
Barometric Pressure Compensation, Limp Home - TPS Table, and the entry
at 12992 were already present at +7 offsets I hadn't accounted for).

GENUINELY MISSING, ADDED:
- Open Loop Load # 2 (11d38): sibling to existing "Open Loop Load # 1"
  (11d24), confirmed via f21c_bit8_hysteresis_from_table_variant which
  branches between the two on f0f8 bit5, both sharing axis 2d09a/2d0a0.
  Standard header convention.
- Idle Stepper Lookup Table (12992, data at 12999): confirmed via
  idle_stepper_table_lookup_wrapper, a clean dual-axis 3D lookup
  (2d4ba: 8-elem Engine Temp, 2d4d0: 27-elem RPM). Standard convention,
  matches bookmark name exactly.
- TABLE_2D_000132F8 (132f8): confirmed via calc_f110_f10e_via_table,
  called twice. UNUSUAL header shape -- "02 28 F0CC..." not the normal
  "02 00 ptr ptr" convention (second byte 0x28=40 isn't the usual zero
  pad). Axis candidate (0x1339e, bank=1) is also unusual -- a ROM CODE
  address, not the normal 0x2Dxxx RAM axis region, and its own "header"
  bytes don't fit the standard axis shape either (implausible count
  0xA1=161). Added the table entry with caller/address confirmed solid,
  but NO axis assigned and the format flagged as not-fully-understood --
  a genuinely open question for a future pass, not guessed at.

Also noted 0x1334e6 shares the same unusual "02 28" header shape as
132F8 (found during the same scan, in calc_f118_via_table) -- consistent
with this being a real, distinct header format used by a small family of
tables in the 0x132xx-0x134xx region, not a one-off anomaly. Not yet
added to the XML -- same open-question status as 132F8, flagged here for
whoever picks this up next.

--------------------------------------------------------------------
22. 1D scalar table search attempted, inconclusive -- stopped
    (2026-08-02).
--------------------------------------------------------------------

Tried to find missing 1D scalar tables (no ROM header exists for these,
per h8539_import_tables_xml.py's own design -- Step 8 only scans for
2D/3D headers, 1D entries are only ever labeled from an existing XML,
never discovered fresh from bytes). Two script attempts:

1. Find1DScalarCandidates.java: any address in 0x10800-0x10FFF with a
   real xref, not within 4 bytes of a known XML address. Result: 613
   candidates -- useless, this range is dense with internal RAM-style
   control-flow reads/writes on ROM-resident small values, not a sparse
   set of real calibration constants the way 2D/3D table headers are.

2. Find1DScalarCandidatesV2.java: tightened to single-xref addresses
   with a direct ROM-to-register load instruction pattern. Result: 0
   candidates -- either the regex pattern doesn't match this ISA's
   actual mnemonic/operand syntax closely enough, or there genuinely
   isn't a clean middle ground between "613 hits" and "0 hits" for this
   kind of scan.

CONCLUSION: 1D scalars have no reliable structural signature the way
2D/3D headers or axis records do, and this ROM's control-flow code
reads/writes far too many small values in this address range for a
generic reference-count heuristic to separate real calibration
constants from internal state. Stopped here -- not a productive avenue
without a much more targeted heuristic (e.g. cross-referencing against
EcuFlash's own known parameter list for this ROM family, if one exists,
rather than blind ROM-region scanning).

FOLLOW-UP (2026-08-03): tried a V3 script filtering specifically for the
confirmed real mechanism (bare "mov ADDR,Rn" direct load, no wrapper
function -- verified by reading Rev Limit's actual disassembly:
"mov:g.w @0x886:16,R0" at 0x22624, no helper call). V3 also required
exactly one xref per candidate, assuming real constants are read once.
That assumption was WRONG -- re-checked Rev Limit itself and it has
TWO xrefs (closedloop_eligibility_check @ 0x22624 AND fuel_pw_and_
airvol_compute @ 0x29fc8), so the single-xref filter would have
excluded even a known-good example. V3 returned 0 candidates, consistent
with being too strict for the wrong reason. Also hit a real program-
context issue mid-session: the Ghidra GUI's active tab had switched to
a different ROM (22580006_EVO5_Stock.hex) without MCP-side control over
which program is "current" -- confirmed switch_program's success report
doesn't reflect reality; the active program actually follows GUI focus.
Re-ran after switching tabs manually and got the same 0-candidate
result, so this wasn't a program-mismatch artifact.
FINAL STATUS: 1D scalar discovery via Ghidra static analysis is not
practical with the tools/techniques available this session. Genuinely
stopping here; would need a fundamentally different source of ground
truth (e.g. a reference EcuFlash definition file) to make further
progress.

--------------------------------------------------------------------
APPENDIX -- source detail moved from review.md (2026-08-03 consolidation)
--------------------------------------------------------------------

The following is review.md's original items 19-22 (2026-07-30/2026-08-01
XML table-address verification + return-storage/array-indirection work),
moved here verbatim because their subject is this file's scope, not
review.md's (the H8/539F Sleigh-implementation review). review.md items
1-18, 23-27 remain in review.md; only the two files' shared XML-table
content was consolidated. review.md now points here instead of repeating
this material. Numbering below (19-22) is preserved from the original for
cross-reference continuity with citations elsewhere in this file
("per review.md item 19/20/21/22") -- these numbers are now only found
here.

19. XML table address verification pass + table_lookup_interp call-site extraction (2026-07-30)
    -- IN PROGRESS, no XML file changes made yet (see status below).

    Built and validated a working Ghidra script-based pipeline for extracting real
    (table_addr, ep_val) pairs from every table_lookup_interp call site, using
    run_script_inline. NOTE: the inline-script tool auto-wraps submitted code in
    its own class scaffold -- submit only bare statements/method-body code, NOT a
    full "public class X extends GhidraScript" declaration, or the two wrappers
    nest and javac fails with "illegal start of expression" (burned two dead
    script files, TableExtract.java/TableExtractV2.java, learning this; they are
    harmless orphans, ignore the recurring compile-error preamble Ghidra now
    prints on every script run because of them).

    Ran the extractor against table_lookup_interp (0x14656): confirmed real,
    complete 130/130 call sites, (caller function, table_addr, ep_val) for each.
    Two rows (isc_f438_correction_calc, egr_duty_target_calc) returned
    imms=[254,252] -- implausibly small to be real addresses, almost certainly a
    parser mis-grab of the wrong preceding immediates; not yet fixed in the
    extraction script, don't trust those two rows as-is.

    Grouped the 69 distinct callers by subsystem from function names alone (no
    script needed): ISC/idle (~25 functions, by far the largest cluster -- idle
    control leans on this dispatcher more than anything else), ignition, fueling,
    O2/closed-loop, knock/octane, EGR/purge, WGDC/boost, TCU/transmission, plus a
    residual set of still-unlabeled fXXXX-register functions.

    VERIFICATION METHOD ESTABLISHED (two independent checks, both real, both
    useful, neither replaces the other):
      (a) Byte-level plausibility: read raw bytes at a candidate address and
          check for the record-header signature (mode byte 2 or 3, mode-selector
          byte, then for mode 3 two RAM pointers + stride, then a smooth/sane
          data curve) vs garbage. Confirmed on Idle Stepper Table (0x12992,
          matches hand-trace from item 18) and on all four EcuFlash-named fuel
          maps (High/Low Octane Fuel Map, High/Low Octane Fuel Map 1) -- headers
          identical (same axis pointers/stride, as expected for same table
          family), bodies diverge exactly where real tuning data should differ.
      (b) get_xrefs_to / get_bulk_xrefs on the candidate address: a subset of
          tables (Idle Stepper, Boost Enhancement, Low Octane Fuel Map 1, Desired
          ISCV step position, at minimum) are independently pointed to by an
          8-entry data-driven pointer/index block at ~0x2d9xx-0x2daxx (looks like
          a per-condition/per-gear table-select array). A confirmed-real address
          in that family shows exactly 8 DATA xrefs; a wrong/mis-scanned address
          a few bytes off shows ZERO. This is a strong, cheap true/false check
          BUT only applies to tables reached via that index block -- tables
          reached only via a hardcoded literal push at a table_lookup_interp call
          site (the majority, e.g. Idle Stepper, IAT candidate, Warmup Retard
          candidate) will show zero xrefs either way; absence of xrefs is NOT
          evidence of a wrong address for that access style, only presence of 8
          xrefs at a plausible-but-close address is strong evidence of realness,
          and zero xrefs at an address that SHOULD be in the index block (i.e.
          its sibling overlap address has 8) is strong evidence of wrongness.

    XML VERIFICATION RESULTS against
    test/rvr/xml/ecuflash/new/21000011_1997-2001_RVR_X3_Mt__4g63t_.xml (an
    already-real, previously-hand-maintained file -- NOT generated by us; it has
    its own extensive prior-session removal history for a different bad batch of
    guessed addresses, see the "Correction A1-A18..." comment block near EOF):

      CONFIRMED CORRECT, no XML change needed:
      - Low Octane Fuel Map 1 (11bc7) = confirmed real record pointer (11bc0) + 7
        byte header. Convention confirmed: XML data-address = raw record pointer
        + 7 for mode-3 (3D) tables.
      - Boost Enhancement Enrich/Enlean (Anti-lag) Map (124f3) = confirmed real
        record pointer (124ec) + 7. 8/8 xrefs confirm 124ec.
      - Accel Enrichment (11cb5) -- false alarm, NOT an overlap. Low Octane Fuel
        Map 1's real table simply ends before 11cb5 starts (real gap, ~238-241
        bytes, plausible size for a 3D table); Accel Enrichment's own bytes are a
        visibly different pattern, not header/continuation data. Zero xrefs
        either way (expected -- not in the index-block family), but byte-level
        check alone is sufficient here since there's no header collision.
      - Desired ISCV intial step position with AC off-Neutral (128a2) --
        confirmed real, 8/8 xrefs.

      CONFIRMED WRONG, replacement NOT yet verified -- DO NOT EDIT XML YET:
      - Warmup Retard, XML address 12500 (= record pointer 124f9 + 7). 124f9 has
        ZERO xrefs and sits only 13 bytes into Boost Enhancement's own confirmed
        header/data (124ec) -- not a separate table, definitely wrong.
        FALSE LEAD, RETRACTED: warmup_ign_advance_correction_calc's own
        table_lookup_interp call (table_addr decimal 7372 -> 11ccc) was initially
        proposed as the fix. WRONG -- its mode byte (2, single-axis) matches the
        already-correctly-placed "Warmup Ignition Control vs Eng Temp" (127cc,
        2D) far better than "Warmup Retard" (3D, Load+RPM axes, mode byte should
        be 3). Do not reuse 11ccc/11cd3 as the Warmup Retard address.
        NEXT LEAD, not yet checked: Warmup Retard shares its exact axis pair
        (Load 2d382, RPM 2d332) with Boost Enhancement Enrich/Enlean -- same
        category (Timing), same level (1) -- so its real record likely sits
        physically close to 124ec in ROM (adjacent table cluster), not near
        11ccc. Search that immediate neighborhood (both before 124ec and after
        its real end, once known) for a second mode-byte-3 header with the same
        F0-xx RAM-pointer-pair convention, sized to fit a 19x17 grid (per XML's
        own axis element counts for this table).
      - Injector Battery Voltage Latency Compensation, XML address 128a7. Only 5
        bytes past confirmed-real 128a2 (Desired ISCV step position) -- still
        inside that table's real curve data, not a separate table. Zero xrefs.
        DEAD END CHECKED: fueling_inj_pw_calc (real name, found via disassembling
        fueling_lambda_update's call list) looked like the obvious place for
        this -- it is NOT. Contains zero table_lookup_interp/axis_lookup_interp
        calls, pure fixed-point arithmetic on f186/f188/f18a/f18c/f190/f192/f194/
        f196 from constants at 0x810/0xf298/0xf29a/0xf29c. Rule this function out
        entirely for this table.
        NEXT LEAD, not yet checked: table is tiny (XML says byte[4][1]), same
        scale as the already-confirmed-real "Ignition Coil Charge Time -
        Normal/Alt" tables (12808/12816), which use a Battery axis (2d27a) and
        are presumably NOT accessed via the big axis_lookup_interp+
        table_lookup_interp machinery given their size. Check callers of
        table_lookup_indexed (0x14854) and table_read_indexed (0x14880) instead
        of table_lookup_interp for this one -- small tables likely use the
        lighter-weight direct-index path, not the axis-record dispatcher.
      - Air Temperature Compensation (2D): does NOT currently exist as a named
        entry in this XML file at all (original flagged address 0002af74 landed
        on a real beq opcode inside tcu_rx_main_scheduler -- confirmed via
        disassembly, definitely code not data). iat_axis_update (real function)
        calls table_lookup_interp with a real address resolving to 0x11d7a; byte
        check shows a sane monotonic curve (158,143,135,129,122,115,106...).
        Reasonably confident this is the real table, but it has zero xrefs
        (expected -- literal call-site access, not index-block) and there's no
        existing XML entry to compare/replace, so this would be an ADDITION not
        a fix. Not yet added.

    STATUS (updated 2026-07-30): XML file modified -- Warmup Retard and
    Injector Battery Voltage Latency Compensation entries commented out in
    place (not deleted) and documented in a new "CONFIRMED WRONG TABLES"
    block near EOF, following the existing session-10 removed-tables
    convention. No replacement addresses were guessed or written for either --
    both remain open pending a verified address per the leads below.
    One optional addition identified (Air Temperature Compensation @ 0x11d7a)
    but not added pending same level of confidence as the confirmed fixes.

    2026-07-30 AIR TEMPERATURE COMPENSATION ADDED: re-verified independently
    (iat_axis_update, 0x21396, disassembled directly -- confirmed pushes
    #0xd4a6 for axis_lookup_interp then #0x1d7a/mode=1 for table_lookup_interp
    0x14656). Byte-read the header/data/sentinel directly: 02 00 F0D0 header
    at 11d7a, curve 9E 8F 87 81 7A 73 6A (158,143,135,129,122,115,106), 0xFF
    sentinel at 11d85 -- sits exactly back-to-back with Post Start Enrich Trim
    vs Load (11d86), zero gap or overlap. Found the table's own dedicated axis
    at 2d4a6 (F0D0 F118 0007 header, 6 bytes -- same header-then-data shape as
    the Idle Stepper axis documented in item 17), real 7-element breakpoint
    array at 2d4ac (8,33,49,63,78,96,125 -- temperature-shaped values,
    matches the 7-byte data curve exactly, no overrun into the next record).
    Cross-checked XML's own address convention against two already-trusted
    entries before writing anything: 2d418 (Engine/Air Temp axis, already in
    XML) has values starting immediately at the declared address, no header;
    128a2 (Desired ISCV step position, already in XML, confirmed real) uses
    its record/header address directly as the XML address, not header+4 as
    its own plate comment's phrasing suggested -- confirmed by reading the
    live XML rather than trusting the comment. Followed that same header-
    address convention: XML added with address="11d7a" (header, not 11d7e)
    and axis address="2d4ac" (skipping the axis's own 6-byte header, matching
    item 17's precedent exactly, not 2d4a6). Zero xrefs on both (expected,
    literal call-site access). This closes the last open item from the
    H8539F-TABLE-CODE-OVERLAP bookmark list -- Accel Enrichment remains the
    only genuinely unresolved entry from that set.

    2026-07-30 continuation -- Injector Battery Voltage Latency Compensation:
    table_lookup_indexed/table_read_indexed caller lists checked (9 functions:
    closedloop_fuel_ready_check, f33e_bit6_update_from_table_da2c_threshold,
    fueling_base_injection_calc, idle_target_rpm_compute_f400,
    isc_f34e_f34a_correction_calc, isc_f35e_correction_calc,
    isc_f408_f406_stepper_target_calc, isc_f41c_table_lookup,
    knock_octane_dual_axis_table_blend_f2a8, f286_knock_octane_secondary_trim_
    compute_via_table, isc_f41a_correction_calc, isc_f420_correction_calc,
    octane_trim_f262_f264_table_select, table_select_scale_fanout_f1be_f1c2) --
    none are injector/battery related by content (ISC/idle, O2, knock/octane).
    fueling_base_injection_calc's table_lookup_indexed call writes to
    f1be/f1c2, same registers as table_select_scale_fanout_f1be_f1c2 (O2/octane
    trim area) -- ruled out for this table. Named-function search for
    inj/batt/volt found no unexamined candidates (fueling_inj_target_select:
    pure flag logic, no table lookup, ruled out; fueling_coolant_inj_offset_
    calc: saturated-subtract offset, no table lookup, ruled out;
    injpw_airvol_reset_on_fuelcut: fuel-cut zero-reset gate, no table lookup,
    ruled out).

    NEW LEAD: extended the item-18 extraction script to axis_lookup_interp
    (0x14735, distinct from table_lookup_interp 0x14656) -- 83 call sites
    total, extracted clean via the same run_script_inline pipeline (same
    orphan-file compile-error preamble noise, harmless, ignore per item 18's
    note). fueling_lambda_update (the injector pulse-width chain, calls
    fueling_base_injection_calc -> ... -> fueling_inj_pw_calc ->
    axis_lookup_interp as its last step) has exactly one axis_lookup_interp
    call site: 0x21a05, imms=[0x2, 0xd15e, 0x21c2f]. 0xd15e is almost
    certainly the AXIS pointer (not the data table itself -- axis_lookup_interp
    resolves X-axis breakpoints, consistent with the 0x2/mode-byte-style first
    immediate seen across all 83 sites); 0x21c2f is fueling_inj_pw_calc's own
    address, likely a return-context artifact of the backward-scan window, not
    a table value -- do not treat as a real immediate. NOT YET DONE: resolve
    what data table 0xd15e's axis actually indexes into (the corresponding
    table_lookup_interp/table_lookup_indexed call using this axis has not been
    identified yet -- axis and data-table calls are typically paired but the
    pairing call site for 0xd15e is still unknown). This is the most promising
    unexplored lead for this table; pick up here next session.

    2026-07-30 RESOLUTION (via Ghidra bookmark cross-check, prior-session ROM
    scraper pass not previously reflected in this file): Injector Battery
    Voltage Latency Compensation is NOT a real table. Bookmarks
    H8539F-TABLE-DATA-OVERLAP-CORRECTED at 0x128a2 carry a ROM-scraper plate
    comment confirming the real table at that address is "Desired ISCV intial
    step position with AC off-Neutral" -- 4-byte header, data at 0x128a6,
    ending ~0x128d6. The claimed Injector Battery Voltage Latency
    Compensation range (0x128a3-0x128ad, XML address 128a7) falls entirely
    inside that real table's own header+data -- there was never a second,
    independent table there. This supersedes the axis_lookup_interp/0xd15e
    lead above (still an open, real thread about fueling_lambda_update's axis
    call, but it does not lead to this XML table -- no further work needed on
    this specific XML entry). XML updated: entry commented out permanently
    (not "pending a replacement" -- none exists or should be sought).

    Cross-checked the same bookmark set for Warmup Retard: only
    H8539F-TABLE-DATA-OVERLAP-SUSPECT exists at 0x124ec/0x124f9, no -CORRECTED
    bookmark -- confirmed the address was still genuinely unresolved as of
    that check.

    2026-07-30 WARMUP RETARD RESOLVED: no code-side caller found across the
    full warmup_* function family (warmup_ign_advance_correction_calc,
    warmup_correction_f25a_f0f8_f212_calc, warmup_mode_state_machine,
    warmup_complete_gate_check, warmup_state_f594_f0e6_init,
    engine_warmup_dispatch_b, engine_warmup_periodic_dispatch,
    f216_bits_f188_f17a_update_and_warmup_gate all individually checked --
    only one table_lookup_interp call exists across the whole family, in
    warmup_ign_advance_correction_calc, and it resolves to 0x11ccc, the
    already-identified "Warmup Ignition Control vs Eng Temp" table, not
    Warmup Retard). Fell back to the byte-neighborhood lead instead: byte-
    scanned Boost Enhancement Enrich/Enlean's real data (124ec header, 342
    bytes, 0xFF sentinel at 12649) and found an identical-shape header
    immediately following at 1264a (03 14 F0C0 F0C2 12 -- same mode byte,
    stride 0x12=18, same axis pointers as Boost Enhancement), with its own
    distinct 342-byte data curve (confirmed different values from Boost
    Enhancement's own, ruling out a duplicate/copy-paste artifact) and its
    own 0xFF sentinel at 127a7. CONFIRMED via get_xrefs_to: exactly 8 DATA
    xrefs at 1264a (0x2da0c through 0x2da28), matching the same index-block
    confirmation signature established in item 19 for every other verified-
    real table in this file. No collision with any other XML-claimed
    address range. XML corrected: Warmup Retard address="12651" (record
    pointer 1264a + 7, per the mode-3 convention). Axis element counts
    (Load 19, RPM 17) left unchanged since only the address was verified
    wrong -- note the real data is 342 bytes / stride 18, not a clean
    19x17=323, worth a follow-up check if EcuFlash renders a trailing gap.

    2026-07-30 ACCEL ENRICHMENT STILL UNRESOLVED, XML deliberately not
    touched (see the contradiction noted above this section): candidate
    replacement 11ccc remains unverified because fueling_accel_enrichment_calc
    (0x21aa5) contains no table_lookup_interp/axis_lookup_interp/
    table_lookup_indexed call at all -- confirmed its three callees
    (0x144d1/0x1417d/0x14151) are pure fixed-point arithmetic helpers
    (identity_passthrough_arg1, div_u32_u16_rounded, div_s32_s16_rounded),
    not table primitives. This suggests Accel Enrichment may not correspond
    to a real 2D lookup table in this ROM's code at all, which is a
    different and deeper problem than a wrong address -- do not write 11ccc
    or any other guessed address without first finding a genuine
    table-lookup call site backing it.

    2026-07-30 CONTRADICTION FOUND, NOT YET RESOLVED, XML NOT TOUCHED: this
    item's earlier "Accel Enrichment (11cb5) -- false alarm, NOT an overlap"
    conclusion (line ~1596 above) does not hold up against a direct byte read.
    Low Octane Fuel Map 1's real data (11bc7 onward) was byte-scanned in this
    session: monotonic AFR-like surface continues uninterrupted through
    11cb1/11cb5 (bytes at 11cb1: 0B 0A 18 17 -- still curve data, not a
    header), with the actual 0xFF sentinel found at 11ccb (260 data bytes,
    not 261 -- prior estimate was off by one). So Accel Enrichment's claimed
    address (XML 11cb5, header 11cb1) is NOT in a real gap -- it's genuinely
    inside Low Octane Fuel Map 1's tail data, contradicting the earlier
    conclusion in this same file. The byte right after the real sentinel
    (11ccc) is 02 00 F0 C8 -- looks like a plausible next-table header (mode
    byte 2, RAM pointer F0C8) and is a candidate for Accel Enrichment's real
    address, but fueling_accel_enrichment_calc (0x21aa5, the actual named
    caller) was disassembled this session and contains NO table_lookup_interp/
    axis_lookup_interp/table_lookup_indexed call at all -- it's pure
    fixed-point arithmetic via helpers at 0x144d1/0x1417d/0x14151. This means
    Accel Enrichment does not have a code-verified table_lookup call site the
    way every other table in this file does, so 11ccc is an UNVERIFIED guess,
    not a confirmed replacement -- do not write it to the XML. Matching
    Ghidra bookmark (H8539F-TABLE-DATA-OVERLAP-SUSPECT at 0x11cb1) also never
    got a -CORRECTED follow-up, consistent with this being genuinely
    unresolved rather than settled either way. NEXT STEP: check what
    0x144d1/0x1417d/0x14151 actually do (fixed-point helpers, not obviously
    table primitives, but not yet individually inspected) and whether Accel
    Enrichment is a scalar/computed value in this ROM rather than a real 2D
    lookup table -- may mean the XML's "Accel Enrichment" table concept itself
    needs re-examination, not just its address.

Item 20: "Unknown" category scraper-derived TABLE_2D_* placeholder axes (2026-07-31)
------------------------------------------------------------------------------------
- Applied all 3 remaining H8539F-TABLE-DATA-OVERLAP-CORRECTED bookmarks that had
  never been written to the XML (item 19 only covered 2 of 5 total CORRECTED
  bookmarks): TABLE_2D_00011EE2 (11EE2->11EE6), TABLE_2D_0002D3C0 (2D3C0->2D3C4),
  TABLE_2D_0002D668 (2D668->2D66C). All three corrected to header+4-bytes per
  ROM-scraper plate comments, matching the established convention. Placeholder
  "RPM"/2d332/elements=18 axis stub left untouched on all three -- confirmed with
  the user this is deliberate scaffolding (set so data is viewable in EcuFlash,
  not a real axis claim), applies to all ~94 "Unknown" category scraper-derived
  tables in this file, not a bug.

- Found in passing: AXIS_TPS at 0x2d3c2 is likely MISLABELED. Traced its RAM
  input pointer (F4EA) to its actual writer, tcu_shift_torque_and_knock_mgmt
  (0x28fff) -- a TCU shift/knock-management function, not a throttle-position
  read path. F4EA is the high-byte output of axis_interp_lerp_u8frac_256 fed by
  F5DE (an IPU capture-period signal also feeding the RPM breakpoint chain),
  scaled by a 40000-based constant -- the function's own header comment
  independently flags this as "likely a smoothed/averaged metric, possibly
  duty-cycle or dwell-adjacent rather than RPM" and explicitly warns not to
  conflate it with the real RPM/axis-interp thread. NOT YET renamed in Ghidra or
  XML (user chose to scope corrections to addresses only, not names, this
  session) -- flagging here so it isn't lost. Needs its own resolution pass.
  NOTE: this specific "tcu_shift_torque_and_knock_mgmt" lead was later
  superseded (not confirmed) by item 10 above, which found the real axis
  record for TABLE_2D_0002D3C0's neighborhood at 0x2d3fe via a different,
  live-traced caller (f0ba_f2d0_o2_mode7_8_correction_calc) -- this F4EA trace
  is kept here for the record but should not be treated as the live answer.

- Attempted to script a bulk axis-resolution pass across all 94 "Unknown"
  placeholder tables, per user request ("look up method... in reverse").
  RESULT: could not resolve any of them. Full negative-result chain, in case
  future sessions want to pick this up rather than repeat it:

    1. table_lookup_interp (0x14656) xref scan, loose 12-instruction backward
       window: produced 55 "matches", ALL FALSE POSITIVES on manual check --
       the loose window grabbed unrelated immediates from surrounding
       instructions (e.g. adds.b stack-cleanup operands), same failure mode
       item 19 already flagged for this exact technique.
    2. Rewrote with a strict walk (only consume Scalars from contiguous
       mov...@-SP pushes, stop at first non-mov/non-push instruction).
       Verified correct against a known-good call site (0x25c35, ep=0x1
       table=0x27e4, matches disassembly exactly). Re-ran against all 94
       targets across ALL 22 known table/axis lookup+wrapper functions
       (table_lookup_interp, axis_lookup_interp, table_lookup_indexed,
       table_read_indexed, egr_table_row_lookup_f496, gear_indexed_table_
       lookup_f1fc, idle_stepper_table_lookup_wrapper, ign_advance_table_
       lookup, isc_f352/f354/f41c/f41e/f42a_table_lookup*, table_1e4a_lookup_
       with_f2d6_d8_reset, axis_interp_lerp_u8frac(_256), iat_axis_update,
       knock_octane_dual_axis_*, map_tps_axis_update, table_3axis_interp_
       triple(_wrapper)) checking ALL collected immediates per call site, not
       just positional args. ZERO matches. This is a real result, not a script
       bug -- the strict logic is proven correct on a known site.
    2b. get_xrefs_to on raw target addresses (e.g. 0x13198, 0x2d3c4): zero
       xrefs, code or data.
    3. search_byte_patterns for the target address encoded as a literal
       pointer (16-bit and 24-bit forms) anywhere in ROM: zero hits for every
       address tried (0x13198, 0x2d3c4).
    4. inspect_memory_content on target addresses confirms they ARE real,
       well-formed table data (correct 3-field header + FF-terminated
       breakpoint list shape, byte-identical structure to confirmed-real
       tables elsewhere) -- ruling out "scraper found padding/garbage" as the
       explanation. The tables are real; they're just unreachable by any
       static-reference method tried.
    5. Hypothesis "read via the MUT/SCI1 diagnostic path instead of normal ECU
       code" -- CLOSED, ruled out. Fully decompiled BOTH SCI1 candidates:
         - sci1_meta_cmd_dispatch_c0_ff (0x28869): actuator-test bit-flag
           writer (cmd bytes C0-FF -> f510/f512/f514/f516 RAM flags, per-gear
           byte/word lookups at ROM 0x232/0x252). No table-address references
           at all, only RAM flag words. Already exhaustively documented
           2026-07-14 through 2026-07-26 per its own plate comment; not
           relevant to our 94 tables.
         - adc_sensor_convert_single (0x171c3), the actual MUT live-data read
           path referenced by that first function's own comment: RequestID ->
           16-bit RAM address via indirection table at ROM 0x2fad0 (EP=2
           banked), returns a single BYTE from RAM. This reads live RAM
           values for scan-tool display (RPM/TPS/boost/etc as CURRENT
           readings) -- structurally unrelated to our ROM tuning tables
           (different indirection table, different data shape, RAM not ROM,
           single byte not a breakpoint array). Confirms these 94 tables are
           not part of the MUT read path in any form.
    6. Hypothesis "reached via an indexed/computed pointer array, like MUT's
       own mechanism" -- found a REAL example of this pattern elsewhere,
       but could not connect it to our 94 targets:
         - f27e_table_indexed_clamp_compute_via_table (0x2415b): genuine
           selector-indexed lookup, entry+0x34 struct field as base pointer,
           indexed by (f218 & 0xF)*2. Real mechanism, but traced back through
           its only caller chain (knock_octane_secondary_dispatch (0x23a9a) ->
           status_word_table_subdispatch_group_a (0x23e60) ->
           f27e_table_indexed_clamp_compute_via_table) and found NO explicit
           parameter passing anywhere in the chain -- "entry" is likely a
           fixed global/RAM struct unrelated to our 94 ROM addresses, not a
           generic per-table dispatch mechanism. Dead end for this purpose,
           though the mechanism itself is confirmed real and may be worth
           revisiting for other unrelated tables.

- CONFIRMED WORKING EXAMPLE of the right general mechanism, traced end-to-end
  for comparison (per user request to trace a known-good 3D table): High
  Octane Ignition Map 1 (XML address 11f7b). Real header found at 11f74 (03 14
  F0 C0 F0 C2 12 ... -- 3D shape, RPM+Load out-ptrs), NOT the XML address
  itself (same header-vs-data-address offset pattern as every other table in
  this file). get_xrefs_to(0x11f74) found genuine DATA xrefs from 0x2db6c
  onward -- a pointer array, 4 bytes/entry (e.g. "00 01 1F 74" repeated 8x,
  then "00 01 20 D2" repeated 8x), i.e. a real octane/RPM-zone-selector table
  of table-pointers, exactly the indirect mechanism hypothesized in point 6
  above. However, tracing the array's OWN reader was not completed this
  session: get_xrefs_to on the array base and nearby rounded addresses found
  nothing; search_byte_patterns for "02 DB" found 4 candidate sites but the
  most promising (isc_f038_constant_select @ 0x277e3) was a byte-pattern
  coincidence inside an unrelated instruction, not a real reference.
  ign_advance_table_lookup (0x173e0) computes a plausible octane-blend
  selector (0-3) from knock/RPM/load thresholds, reading a SEPARATE small
  4-entry byte table at 0x31b7 (blend weights, not the pointer array) -- likely
  upstream of whatever consumes the 0x2db6c array, but its own caller
  (ign_advance_f0e6_bit7_update, 0x173c0) is an output-relay-flag writer that
  discards the returned selector value entirely, confirming the real consumer
  of the 0x2db6c array is a different, not-yet-found function. NEXT STEP for
  whoever picks this up: find what reads the 0x2db6c-style array now that its
  existence and shape are confirmed -- likely requires searching for a
  computed-jump/PJSR-via-register pattern (base+selector*4 then indirect call
  or read) rather than any literal-immediate or byte-pattern search, since
  every literal-based method tried above came up empty for this array's own
  base address too.

- NET CONCLUSION for the 94-table placeholder-axis project: the axis data for
  these tables is NOT resolvable by any static xref/byte-pattern method tried
  so far. Either (a) they use a computed-pointer-array indirection like High
  Octane Ignition Map 1's confirmed mechanism, in which case the missing piece
  is finding the array's reader function (not yet found for even the ONE
  confirmed-good example), or (b) some other mechanism not yet hypothesized.
  Recommend either continuing the search for a computed-jump pattern, or
  moving to dynamic analysis (debugger watchpoint on a target table's ROM
  address while running/emulating) if available, since static analysis alone
  has now been tried from multiple independent angles without success.
  SUPERSEDED (2026-08-01): this net conclusion was resolved by item 22 below
  -- the missing piece was table_lookup_indexed's array-base indirection,
  fixed via a dedicated reference-creation script rather than further static
  xref/byte-pattern search.

21. Return-storage audit pass across table/axis dispatcher functions (2026-08-01)
    -- COMPLETE for the ~34-function list checked this session.

    Context: table_lookup_interp (item 18/19) was already known to compute a
    real interpolated result via its final axis_interp_lerp_u8frac call, but
    was declared `void` -- the decompiled callee silently discarded the value
    on every path, while caller-side decompiles (fueling_base_injection_calc,
    iat_axis_update, etc.) clearly captured it (`uVar1 = (*(code*)0x14656)();`).
    That mismatch is the actual bug class this pass checked for across a wider
    function list: "declared void/wrong return, but a real caller captures a
    value" vs "genuinely void, result only ever reaches memory, correctly
    modeled already."

    METHOD: for each function, decompile and check (a) does it end with a
    computed value being discarded on a path a caller might need, (b) do its
    real callers (via get_function_callers) actually capture a return. Only
    fix (a)+(b) together -- do not touch functions that are genuinely void.

    FIXED (real bug, confirmed via a real caller before touching):
    - table_lookup_interp (0x14656) -- ushort return added (item 18/19
      continuation, done first this session, everything below built on top of
      this). set_function_prototype call:
        ghidra:set_function_prototype(function_address="00014656",
          calling_convention="__stdcall_far_2arg",
          prototype="ushort table_lookup_interp(byte ep_val, void *table)")
      RESULT: return value correctly modeled now (`return uVar4;`).
      REGRESSION CAVEAT (see below): this call also silently drops the
      `table` parameter's Stack[0x7]:2 storage back to <UNASSIGNED> every time
      the prototype is re-applied -- this is NOT something this fix broke,
      it's the SAME pre-existing param-binding quirk the function's own plate
      comment already documented before any of this session's edits ("keeps
      re-inferring a 3rd stack param regardless of declared prototype").
      set_variable_storage cannot repair it either -- tool response explicitly
      says programmatic storage control is limited here and requires either
      the Ghidra GUI (right-click variable -> Retype/Edit) or a custom
      Pcode/HighVariable-level script (see FixEBPRegisterReuse.java mentioned
      in the tool's own error text for the pattern to follow). NOT attempted
      this session -- flagged as a real follow-up, not solved.
    - table_3axis_interp_triple (0x148a2) -- same fix, same
      __stdcall_far_2arg/ushort prototype. CONFIRMED via real caller
      f250_f252_channel_snapshot_update (0x249f2): `uStack_2 =
      table_3axis_interp_triple();`. Same param-storage regression as above
      (expected, same underlying quirk, not re-litigated per function).
    - table_lookup_indexed (0x14854) -- same fix. Was declared void while
      internally calling table_lookup_interp() without capturing IT either (a
      second copy of the same bug one level up); CONFIRMED via real caller
      knock_octane_dual_axis_table_blend_f2a8 (`uVar2 =
      table_lookup_indexed(...)`). After fix, decompiles cleanly as
      `uVar1 = table_lookup_interp(); return uVar1;`. Same param-storage
      quirk resurfaced, not fixed.

    CHECKED, ALREADY CORRECT (no change made) -- listed so nobody re-checks
    these from scratch: axis_interp_lerp_u8frac, axis_interp_lerp_u8frac_256,
    iat_axis_update, knock_octane_dual_axis_table_blend_f2a8, map_tps_axis_update,
    table_3axis_interp_triple_wrapper, o2_closed_loop_mode_selector_via_table,
    octane_level_init_or_force_from_gear_table, status_word_bit3_conditional_
    update_via_table, status_word_composite_multibit_update_via_table,
    status_word_table_subdispatch_group_a, table_1e4a_lookup_with_f2d6_d8_reset,
    table_read_indexed, table_read_word_indexed_f1fc,
    table_select_scale_fanout_f1be_f1c2, tcu_shift_solenoid_table_init (great
    confirmation case -- 17 internal table_lookup_interp calls all now
    decompile as `uVar1 = (*(code*)0x14656)(); *(...) = uVar1;` post-fix),
    throttle_param_table_select, tiered_threshold_table_select,
    egr_table_row_lookup_f496, f2b4_threshold_table_lookup (also confirms the
    fix propagates: `sVar3 = (*(code*)0x14656)(); return sVar3 + 0x80;`),
    gear_indexed_table_lookup_f1fc, idle_stepper_table_lookup_wrapper,
    ign_advance_table_lookup, isc_f352_table_lookup, isc_f354_table_lookup_
    conditional, isc_f41c_table_lookup (confirms table_lookup_indexed fix
    propagates), isc_f41e_table_lookup, isc_f42a_table_lookup.
    Most of these are genuinely void by design (result only ever reaches a
    memory-mapped global, e.g. f2a8/f496/f41c/etc, no caller ever captures a
    return) -- correctly modeled already, nothing to fix. A handful already
    had a correct real return type before this pass and independently confirm
    the two fixes above are propagating cleanly through their internal calls.

    NOT FIXED, KNOWN UNRELATED ARTIFACT (do not touch, separate issue):
    - knock_octane_dual_axis_blend_far_trampoline (0x23397) -- decompiles as
      an infinite self-call with no base case. Already flagged in its own
      plate comment as a suspected far-call target mis-resolution tied to the
      known CP-register SLEIGH-context limitation (see README). Unrelated to
      return-storage; do not attempt a prototype fix here, needs the CP-ctx
      fix first.

    NOT FIXED, HARDER PROBLEM CLASS -- flagged for a dedicated follow-up:
    - octane_trim_f262_f264_table_select (0x23abe) -- decompiles with heavy
      extraout_R1/extraout_R1_00/_01/_02 and extraout_R2/_00/_01 register
      artifacts around its table_read_indexed calls, plus unaff_R3/unaff_R4.
      This looks like table_read_indexed's REAL output spans two registers
      (a pointer in one + an index/value in another), not the single-ushort
      return this pass's fix pattern covers. Needs its own investigation into
      table_read_indexed's true multi-register return signature before
      anything here can be safely reprotyped -- do NOT apply the simple
      ushort fix to table_read_indexed blindly, it may already be under-
      modeled in a way a naive single-register fix would make worse, not
      better.
    - read_indexed_byte_f1fc (0x1481c) -- NOT in this session's original
      list, noticed in passing while checking
      octane_level_init_or_force_from_gear_table (which calls it and shows
      an `undefined1 extraout_var` artifact combined via CONCAT11 with the
      real return byte -- classic signature of a return-WIDTH mismatch, byte
      vs word, rather than a void-vs-typed mismatch). Worth checking
      alongside the item above, same general "return spans more than the
      modeled single value" theme.

    STATUS: 3 real fixes applied and confirmed stable/propagating
    (table_lookup_interp, table_3axis_interp_triple, table_lookup_indexed).
    ~28 functions checked and left alone (already correct). 1 known unrelated
    artifact left alone (CP-register trampoline). 2 flagged multi-register
    return candidates NOT touched, need dedicated follow-up
    (octane_trim_f262_f264_table_select's table_read_indexed usage,
    read_indexed_byte_f1fc). Outstanding cross-session issue: the param-
    storage-reverts-to-unassigned quirk on table_lookup_interp/
    table_3axis_interp_triple/table_lookup_indexed (pre-existing, documented,
    not caused by this pass) still needs either GUI manual fix or a custom
    Pcode-level script -- set_function_prototype and set_variable_storage
    both confirmed insufficient for it.

    CLOSING NOTE (2026-08-01, same session): grep-swept the full decompiled .c
    for the same bug signature (bare call immediately before `return;`, i.e. a
    computed value discarded) beyond the original 34-function list. Found 5
    further candidates; ALL 5 verified as false alarms against real
    disassembly, not just decompiler output:
    - egr_sequence_control's discarded call to 0x18b17
      (egr_f490_gear_transition_update) -- already-confirmed genuinely-void
      function, no issue.
    - ign_advance_top_update discarding wgdc_correction_integrator_update's
      return -- CONFIRMED via real disassembly this is intentional: the ROM
      saves ign_advance_base_calc_f860's return into a stack slot BEFORE
      calling wgdc_correction_integrator_update, then reloads that saved
      value (not wgdc's return) to feed wgdc_output_clamp_f44c. wgdc's return
      is genuinely unused at the hardware level, not a decompiler artifact.
    - isc_openloop_stepper_scale_calc discarding muldiv_s16_rounded's result
      -- CONFIRMED via real disassembly this is a decompiler call-site
      cosmetic gap, not a bug: table_lookup_interp's R0 result is actually
      pushed directly as muldiv_s16_rounded's argument (`mov:g.w R0,@-SP`
      immediately before the pjsr), the decompile just doesn't show the
      connection (same general family as the param-binding quirk, but at a
      call site rather than a function's own prototype). muldiv_s16_rounded's
      own final result is genuinely unused afterward (function returns
      immediately after).
    NET RESULT: no new return-storage bugs found beyond the 3 already fixed.
    Good confirmation the 3 fixes (table_lookup_interp, table_3axis_interp_
    triple, table_lookup_indexed) were the real bugs in this codebase, not
    the tip of a larger iceberg. Method note for future passes: grep for
    `^\s*[A-Za-z_]\w*\([^;]*\);\n\s*return;` catches candidates cheaply, but
    every hit needs a real disassembly check (not just decompile re-read)
    before concluding bug vs. false-alarm -- several of these looked
    suspicious in the decompiled C alone and only resolved cleanly once the
    actual instruction stream was checked.

22. Custom analyzer script: explicit references for table_lookup_indexed's
    array-base indirection (2026-08-01) -- RESOLVES the structural blocker
    documented in item 10 above ("array-base pointers themselves are
    apparently never referenced via a discoverable static xref, even for
    tables we're certain are real -- this is a structural limitation").

    DIAGNOSIS (external analysis, credited): Ghidra's static xref engine
    cannot follow this ROM's indirection pattern for table_lookup_indexed --
    callers push an ARRAY BASE pointer (not a direct table address), and the
    function does `table = array[global_index & 7]` at runtime. Since the
    real target is only known via a runtime-computed index, there is no
    static address-of relationship for Ghidra to record automatically. This
    is structurally different from table_lookup_interp's callers (which push
    a literal bank+offset directly, hence ARE statically discoverable).

    FIX: rather than asking Ghidra's general xref engine to infer something
    it structurally cannot, wrote a script (run_script_inline, bare
    method-body -- NOT a full class declaration, see item 19's gotcha) that
    encodes the ROM's own convention directly:
      1. get_function_callers on table_lookup_indexed (0x14854) -- Ghidra
         DOES track direct calls to the function itself fine, just not the
         indirection inside it. Found 14 real call sites (9 distinct calling
         functions, some with multiple array-selecting call sites, e.g.
         isc_f34e_f34a_correction_calc has 4).
      2. For each call site, backward-scan (same method as item 19's
         extractor) for the two pushed immediates (offset, bank) to resolve
         the real array base address (bank*0x10000+offset).
      3. refMan.addMemoryReference(callSite, arrayBaseAddr, RefType.DATA,
         SourceType.USER_DEFINED, 0) -- creates the previously-missing
         call-site -> array-base reference.
      4. Read each array's 8 pointer-array slots (4 bytes each: byte0=00
         (unused/pad), byte1=bank, bytes2-3=offset -- confirmed format from
         earlier manual reads in this file), resolve each to a real address,
         and add a further reference from the slot -> its real target table.
      5. Label each array base (ARRAY_<callerName>_<hex> convention).
    Dry-run first (confirmed clean output for all 14 sites), then applied for
    real. VERIFIED applied: get_xrefs_to(0x2da0c) now shows the new
    call-site reference from isc_f34e_f34a_correction_calc @0x257a0 -- this
    exact address previously returned ZERO xrefs (item 10's negative-result
    baseline).

    RESULT -- all 14 call sites resolved cleanly to sensible array bases,
    each array's 8 slots internally consistent (all 8 slots identical per
    array in every case checked, matching the pattern already established on
    known-real tables like the Idle Stepper table). Full mapping:
      isc_f35e_correction_calc (0x25880) -> array 0x2da2c -> table 0x127be
      f33e_bit6_update_from_table_da2c_threshold (0x24f87) -> array 0x2da2c
        -> table 0x127be (SAME array as isc_f35e_correction_calc -- two
        different callers legitimately sharing one array, consistent with
        the method note above about RAM-tag/array reuse)
      isc_f408_f406_stepper_target_calc (0x2716b) -> array 0x2d92c
        -> table 0x128a2 (matches already-confirmed-real Desired ISCV step
        position table -- cross-validation the script's resolution is sound)
      isc_f41c_table_lookup (0x275ad) -> array 0x2d94c -> table 0x128ae (NEW,
        not previously identified)
      fueling_base_injection_calc (0x21a36) -> array 0x2da8c -> table 0x11880
        (NEW)
      idle_target_rpm_compute_f400 (0x262aa) -> array 0x2d8ec -> table
        0x1288a (NEW)
      idle_target_rpm_compute_f400 (0x262cc) -> array 0x2d90c -> table
        0x12896 (NEW -- same caller, second independent array)
      closedloop_fuel_ready_check (0x2271e) -> array 0x2da4c -> table 0x11cea
        (NEW)
      isc_f34e_f34a_correction_calc (0x25777) -> array 0x2dbac -> table
        0x12230
      isc_f34e_f34a_correction_calc (0x257a0) -> array 0x2da0c -> table
        0x1264a (matches already-confirmed-real Warmup Retard table --
        second cross-validation)
      isc_f34e_f34a_correction_calc (0x257c9) -> array 0x2db8c -> table
        0x120d2 (matches already-confirmed-real High Octane Ignition Map 2
        per item 12 above -- third cross-validation)
      isc_f34e_f34a_correction_calc (0x25790) -> array 0x2dbcc -> table
        0x1238e (NEW)
      knock_octane_dual_axis_table_blend_f2a8 (0x233d9) -> array 0x2d9cc
        -> table 0x11bc0 (matches already-confirmed-real Low Octane Fuel Map
        1 -- fourth cross-validation)
      knock_octane_dual_axis_table_blend_f2a8 (0x23400) -> array 0x2d98c
        -> table 0x119b8 (matches already-confirmed-real High Octane Fuel
        Map 1 -- fifth cross-validation)

    5 genuinely NEW table addresses surfaced this way (0x128ae, 0x11880,
    0x1288a, 0x12896, 0x11cea, 0x1238e -- six, corrected count), all via real
    resolved data rather than guesses, directly extending this file's
    XML-verification project. NEXT STEP: cross-check these against the XML
    for further address fixes (in progress, see following entry).

    METHOD NOTE for future use: this script targeted table_lookup_indexed
    specifically because get_function_callers already gave a clean, small,
    complete call-site list (14) to work from. The SAME technique should
    generalize to any other indexed/array-base lookup function in this ROM
    (table_read_indexed, gear_indexed_table_lookup_f1fc, etc.) once their own
    caller lists are pulled the same way -- worth doing as a follow-up rather
    than assuming table_lookup_indexed was the only function with this
    indirection pattern.

    XML CROSS-CHECK of the 6 new addresses (2026-08-01, same session):
    - 0x128ae, 0x12896: CONFIRMED already correct in XML (exact address
      match: "Desired ISCV intial step position with AC off-Drive",
      "Desired Idle RPM-Drive"). Excellent independent validation of the
      whole array-reference script's resolution logic -- these came out
      correct without ever having been targeted or reverse-engineered from
      the XML side. No action needed.
    - 0x11880: found as TABLE_2D_00011880, one of the ~94 "Unknown"-category
      Batch 2 placeholder tables. NEW LEAD for Batch 2: real caller now known
      (fueling_base_injection_calc via array 0x2da8c) -- gives a real
      subsystem identity (fueling) even though the axis itself still needs
      separate resolution. Worth prioritizing in Batch 2 given this head
      start.
    - 0x1288a, 0x11cea, 0x1238e: NOT PRESENT in the XML at all (zero grep
      matches for any of the three). These aren't mis-addressed existing
      entries -- they're genuinely undocumented tables with no XML entry
      whatsoever. Adding them is a bigger task than this review's stated
      scope (address-only fixes to existing entries): would need a type,
      axis, and scaling worked out from scratch, not just an address
      correction. NOT attempted this session -- flagged as a separate,
      larger follow-up (new-table-addition project) rather than folded into
      the address-fix pass.

--------------------------------------------------------------------
23. Caller trace for 20 unattributed H8539F-SCRAPED-TABLE placeholders
    (2026-08-03) -- prompted by comparing the scraper's raw output against
    the promoted H8539F-AXIS bookmarks.
--------------------------------------------------------------------

    CONTEXT: the ROM scraper produces a generic SCALING_TABLE_* bookmark for
    every byte-pattern match it finds (best-guess name like AXIS_RPM inferred
    purely from RAM tag bytes, db[N] = raw header+data byte count). Only
    entries with a confirmed caller get promoted to a named H8539F-AXIS
    bookmark with a real element count. Diffed the two bookmark categories
    and found 20 addresses still stuck as generic scraper output with no
    promoted caller: 2D0C0, 2D0D2, 2D146, 2D15E, 2D192, 2D1B4, 2D20A, 2D218,
    2D248, 2D2CC, 2D302, 2D37C, 2D3A8, 2D3C2, 2D3DA, 2D412, 2D428, 2D45C,
    2D472, 2D4A6.

    ROOT-CAUSE CHECK FIRST: confirmed via get_bulk_xrefs that all 20 return
    zero references, AND (critically) that get_xrefs_to on 0x2d74c -- an
    address whose real caller (isc_f34e_f34a_correction_calc) was JUST
    confirmed by hand in item 17's follow-up -- ALSO returns zero. This
    proves the "NO CALLER FOUND" signal is not about function typing,
    calling-convention, or multi-value returns; it's that every axis in this
    ROM is reached via a raw 16-bit immediate (`mov:g.w #0xNNNN,@-SP`) rather
    than an address-mode operand, which Ghidra's static xref engine never
    tracks as a data reference regardless of how well-typed the calling
    function is. get_xrefs_to/get_bulk_xrefs are structurally blind to this
    pattern for ANY axis in this ROM, confirmed or not -- only a whole-program
    literal-immediate scan (the same technique FindBatch2Callers.java already
    uses for table addresses) can find these callers.

    METHOD: ran FindBatch2Callers.java's exact scan logic (unmodified
    algorithm, just re-targeted at these 20 addresses instead of the Batch 2
    table list) via run_script_inline. Confirmed the tool actually executes
    against the live Ghidra instance (not a simulated dry run) -- output
    matched real, verifiable function names and addresses.

    RESULT: 18 of 20 traced immediately, only 2D0D2 and 2D1B4 came back "NO
    CALLER FOUND":

      2D0C0 -> closedloop_eligibility_check (0x22674)
      2D146 -> isc_f408_correction_calc (0x27336)
      2D15E -> fueling_lambda_update (0x21a02)
      2D192 -> throttle_target_ramp_update (0x21775) AND
               f2ca_table_override_from_f114_gate (0x2377e) -- two independent
               callers of the same axis address
      2D20A -> f28e_f292_knock_lambda_composite_calc (0x234bd)
      2D218 -> f28e_f292_knock_lambda_composite_calc (0x2344c)
      2D248 -> f28e_f292_knock_lambda_composite_calc (0x234af)
      2D2CC -> f0ba_f2b2_threshold_blend_calc (0x238f4)
      2D302 -> knock_octane_dual_axis_table_blend_f2a8 (0x233b9)
      2D37C -> isc_f34e_f34a_correction_calc (0x25760)
      2D3A8 -> f4de_f4e2_octane_correction_calc (0x265e6)
      2D3C2 -> f4de_f4e2_octane_correction_calc (0x265f4)
      2D3DA -> isc_f438_correction_calc (0x26b55)
      2D412 -> map_tps_axis_update (0x21243)
      2D428 -> map_tps_axis_update (0x21251)
      2D45C -> map_tps_axis_update (0x2125f)
      2D472 -> egr_table_row_lookup_f496 (0x18cda)
      2D4A6 -> iat_axis_update (0x213bc)

    Only 2 of the original 20 leads are still genuinely dead ends after this
    pass: 2D0D2 and 2D1B4.

    PER-LEAD FOLLOW-UP (checked table targets, NOT all applied to XML --
    see individual verdicts):

    - 2D37C (isc_f34e_f34a_correction_calc): this is the SAME axis already
      resolved as the Load axis of "High Octane Ignition Map 1" back in
      item 12 (0x2d37c, 19 elem). Pure cross-validation via an independent
      route -- no action needed, already correct in the XML.

    - 2D3A8/2D3C2 (f4de_f4e2_octane_correction_calc): disassembled the full
      function. Both axis_lookup_interp calls (back-to-back, same pattern as
      item 17) precede a table_lookup_interp call to 0x12d74 (bank=1,
      offset=0x2d74) -- NOT item 10's blocked 0x2D3C0 target as hoped, but
      notably 0x12d74 is exactly 4 bytes into the DATA BODY of the
      already-named TABLE_2D_00012D70_O2Load (header 0x12d70, confirmed via
      read_memory: 02 00 F0C0 at header, 03 00 F0C0 F0C2 0A00 immediately
      after -- an unusual shape, possibly a second nested mini-header/axis
      pair rather than plain data, not fully understood). TABLE_2D_00012D70
      already has a DIFFERENT confirmed axis (2d09a, via a different caller
      o2_downstream_enable_check) already in the XML. Whether this is the
      same physical table read two different ways by two different
      subsystems (same class of ambiguity as item 12's two-ignition-maps
      case), or a second, distinct, currently-uncatalogued table living
      inside what looks like 12D70's data body, is NOT resolved. NOT applied
      to XML -- flagged for a dedicated follow-up, do not guess.

    - 2D0C0 (closedloop_eligibility_check): disassembled the function --
      single axis_lookup_interp(0xd0c0) immediately precedes a
      table_lookup_interp call to 0x11f70 (bank=1). Checked the XML:
      0x11f70 has ZERO grep matches anywhere in the file -- this is a
      genuinely uncatalogued table, not a misfiled axis on an existing
      entry. Same category as the 0x11cea/0x1238e/0x1288a findings logged
      above in this file -- adding it means working out a type/name/element
      count from scratch, out of scope for a straight address-fix pass. NOT
      attempted this session.

    - Remaining ~13 (2D146, 2D15E, 2D192, 2D20A, 2D218, 2D248, 2D2CC, 2D302,
      2D3DA, 2D412, 2D428, 2D45C, 2D472, 2D4A6): callers identified above,
      but their table targets have NOT yet been individually disassembled/
      checked against the XML this session -- unlike the 5 above, these are
      still open leads, not verdicts. NEXT STEP: for each, disassemble the
      caller, find the table_lookup_interp/indexed call immediately
      following the axis push(es), cross-check the target address against
      the XML the same way as done for 2D0C0/2D37C/2D3A8/2D3C2 above, and
      only apply address+element corrections for clean, unambiguous
      placeholder fixes -- same discipline as item 17.

    NEXT STEP (script): FindBatch2Callers.java's core scan logic works
    unmodified for axis addresses, not just table addresses -- worth adding
    a permanent axis-address list to the script (or a parallel script) so
    future passes don't need an ad-hoc inline re-run.

--------------------------------------------------------------------
24. Bulk rerun of FindBatch2Axes.java against all remaining 2d332-placeholder
    tables (2026-08-03)
--------------------------------------------------------------------

    CONTEXT: after item 17's manual X-axis fixes, 78 <table address="2d332">
    lines still remained across ~57 distinct Unknown-category tables.
    FindBatch2Axes.java already had nearly this exact address list hardcoded
    (built for the original item 17 pass), so reran it as-is rather than
    writing anything new, to see how many now resolve given the additional
    axis/caller ground truth established since.

    RESULT: only ONE new clean resolution out of ~57 candidates:

      13212 (TABLE_2D_00013212_TCU) -> axis 2d636 (Y, 10 elem), via
      tcu_torque_converter_slip_calc. Same shared axis as 12EF8/12F14
      (already-established TCU/threshold-dispatch family). APPLIED TO XML.

    Everything else falls into one of four buckets, none of which are safe
    to guess at:

    a) NO CALLER FOUND (no literal push anywhere in the program) --
       11D20, 11EA0, 11F6C, 12AE6, 12D00, 12DF2, 12E4A, 1314C, 13154, 1315C,
       13168, 13174, 1317C, 13188, 13190, 13198, 131B8, 131C0, 131CC, 131EC,
       131F6, 13202, 1320A, 1321C, 13236, 13250, 1325E, 1326C, 1327A --
       genuinely no lead via this method. Candidates for the array-base
       indirection route (item 10/22's mechanism) or genuinely dead ends.

    b) "preceding event is ANOTHER TABLE call, not axis" -- the push
       immediately before this table's own call is a DIFFERENT table call,
       not an axis. This means either (i) this table shares an axis with
       its neighbor further back (would need a 2-events-back check per
       table, same as item 17, but here the 2nd-back slot is occupied by
       yet another table, not an axis -- may need 3+ events back), or (ii)
       this table genuinely has no local axis push and inherits one set up
       earlier in the caller. Affects: 11834, 118A0, 11E86, 11E94, 11EC8,
       0127D8, 12860, 012EAE, 0131D4 (each has a named caller function
       identified, useful even without the axis).

    c) "NO PRECEDING PUSH-EVENT IN FUNCTION" -- this table's push is the
       FIRST push event in its function; nothing local precedes it. Affects
       11D4C, 11ED6, 01286C, 0128EA, 012926, 01297A, 012986, 012CE8, 012CF4,
       012EBC, 0131E0. Same interpretation caveat as (b)(ii).

    d) "preceding event has NO pjsr immediately after it" -- inconclusive,
       the previous push exists but isn't followed by a call at all (may be
       an unrelated push for a different purpose entirely). Affects 128D2,
       128DE, 12ECE, 12EDC, 12EEA, 12F06.

    All four buckets have a named caller function already identified for
    most entries (useful subsystem-identity leads even without axis
    resolution) -- see the raw script output preserved in this session for
    the full per-table caller names if picking this back up later.

    NOT attempted this session: extending the scan to 3 events back for
    bucket (b), or investigating whether bucket (a)'s dead ends are reached
    via the array-base indirection mechanism instead (same as item 10/22).
    Both are plausible next steps but weren't run this pass -- flagging
    rather than guessing.

    UPDATE (2026-08-03, same session): ran the 3-events-back extension on
    bucket (b)'s 9 candidates (walking backward past consecutive TABLE
    calls until hitting an axis_lookup_interp, rather than stopping at
    exactly 1 or 2 back). 3 of 9 resolved cleanly, APPLIED TO XML:

      11834 -> axis 2d218 (21 elem), THREE events back in
      f28e_f292_knock_lambda_composite_calc, past two sibling table calls
      (0x1181a, 0x11800). New axis, not previously used elsewhere.

      12860 -> axis 2d29c (4 elem), TWO events back in
      f37a_correction_calc_from_f33e_bit2_or_f378, past sibling table call
      0x12858 -- shares the exact same axis as TABLE_2D_00012858_ISC.

      12EAE -> axis 2d620 (8 elem), TWO events back in egr_duty_target_calc,
      past sibling table call 0x12ea2 -- shares the exact same axis as
      TABLE_2D_00012EA2_EGR.

    Remaining 6 of the 9 (118A0, 11E86, 11E94, 11EC8, 127D8, 131D4) walked
    back to either the start of their function or an unrecognized
    non-axis/non-table pjsr with no axis found -- genuinely stuck via this
    method, not a guessing target.

    Running placeholder-table tally after this update: 3 more resolved
    (11834, 12860, 12EAE), 6 confirmed dead-ends-for-now in bucket (b),
    bucket (a)'s ~29 no-caller-found tables and bucket (c)/(d)'s ~17 tables
    still untouched this session.

--------------------------------------------------------------------
25. Array-base/data-start xref angle on bucket (a)'s 29 no-caller-found
    tables (2026-08-03, same session)
--------------------------------------------------------------------

    METHOD: bucket (a) tables have zero literal-push callers (confirmed via
    FindBatch2Axes.java rerun in item 24). Tried a different angle: instead
    of scanning for literal pushes of the HEADER address, ran
    get_bulk_xrefs directly against each table's DATA-START address
    (header+4), on the theory that some callers reference data-start
    directly as a data xref rather than pushing the header as a literal
    (the same pattern already seen for TABLE_2D_00011880 and the two
    degenerate TCU tables 131C4/131FA).

    RESULT: header addresses themselves: zero xrefs across all 29, as
    expected. Data-start (+4) addresses: 5 of 29 got hits, but 2 were
    coincidental overlaps with already-fully-documented tables (131C0+4=
    131C4, and 131F6+4=131FA both land exactly on the two known-degenerate
    TCU tables' own headers -- not real leads for 131C0/131F6, just address
    proximity). The other 3 were genuine, and all 3 got a full
    disassembly-confirmed resolution:

    - 11F6C -> RESOLVED, axis 2d0c0 (6 elem). Data-start 0x11f70 is called
      from closedloop_eligibility_check (0x225be) -- the SAME function/
      target already identified in item 23's separate unattributed-axis
      trace (2D0C0 -> 0x11f70), which at the time was flagged as a
      "genuinely uncatalogued table, zero XML matches". That flag is now
      SUPERSEDED: 0x11f70 isn't a new table at all, it's simply
      TABLE_2D_00011F6C's own data body (header at 11f6c, data at
      11f6c+4=11f70, standard convention) -- the table already existed in
      the XML, just still on the 2d332 placeholder. Item 23's "new table"
      lead is now closed as a non-issue. APPLIED TO XML.

    - 13198 -> RESOLVED, axis 2d7ca (5 elem), via isc_openloop_target_calc
      (0x17c75) -- single axis_lookup_interp immediately precedes the
      table call, clean 1-event-back case that the original scan had
      simply never reached (this function wasn't in the literal-push
      target list before). APPLIED TO XML.

    - 12AE6 -> caller confirmed (ign_advance_rpm_zone_enable_check,
      0x19364) but NOT resolved -- full disassembly shows this is the
      FIRST and ONLY pjsr in the entire function body; there is no
      axis_lookup_interp call anywhere in it. Either this table doesn't
      use a real breakpoint axis (possible scalar/threshold read dressed
      up as a 2D table), or its axis is established by a caller of THIS
      function rather than locally. Real subsystem identity is now known
      (ignition-advance RPM-zone gating) even though the axis isn't --
      worth a name-suffix update (_IgnAdvance or similar) even without an
      axis fix. NOT applied to XML this session (name change only, low
      priority vs the open axis question).

    Bucket (a) remaining: 26 of 29 still have zero leads via either the
    literal-push or data-start-xref method. Not investigated further this
    session -- next angle would be checking whether any of these are
    referenced through one of the KNOWN indirection arrays (0x2d8ec,
    0x2d90c, 0x2d92c, 0x2d94c, 0x2da0c, 0x2da2c, 0x2da4c, 0x2da6c, 0x2da8c,
    0x2daac/0x2dacc/0x2daec/0x2db0c, 0x2db2c, 0x2db4c, 0x2db6c, 0x2db8c,
    0x2dbac, 0x2dbcc from the earlier array-dump review) rather than a
    fresh, not-yet-discovered array.

    Updated running tally: 58 tables started on 2d332 this session -> 8
    resolved (13212, 11834, 12860, 12EAE, 11F6C, 13198, plus the earlier
    item-17 six were separate) -- 50 remain unresolved, with 12AE6 now
    carrying a confirmed caller/subsystem identity despite no axis fix.

--------------------------------------------------------------------
26. Data-start xref angle applied to buckets (c) and (d) (2026-08-03,
    same session)
--------------------------------------------------------------------

    Ran the same data-start (+4) get_bulk_xrefs check from item 25 against
    bucket (c)'s 11 candidates (11D4C, 11ED6, 1286C, 128EA, 12926, 1297A,
    12986, 12CE8, 12CF4, 12EBC, 131E0) and bucket (d)'s 6 (128D2, 128DE,
    12ECE, 12EDC, 12EEA, 12F06). Only 2 of 17 got any hit:

    - 12926 -> RESOLVED, axis 2d116 (10 elem), via isc_f426_f03a_decay_calc
      (0x2655d). Clean single axis_lookup_interp immediately before the
      table call. Same axis as TABLE_2D_00012DD6_O2Heater -- good
      cross-validation. Header byte-verified (F0C0 F170 000A). APPLIED TO
      XML.

    - 131E0 -> hit was a false lead. The data-xref from
      f2f0_f2f8_ring_and_f27x_f2cc_periodic_init (0x23027) turned out on
      disassembly to be a single DP-relative scalar byte read
      (mov:g.b @0x31e4,R0, no pjsr at all, no table_lookup_interp) -- an
      unrelated init routine reading one calibration constant at that
      address, not a real table lookup. Does not help resolve 131E0's axis;
      its only genuine table_lookup_interp caller remains
      f276_f27a_dual_channel_compute_via_table (already known from bucket
      (b), still stuck at "first push in function, no local axis").

    Remaining 15 of these 17: zero xref hits either at header or
    data-start. Not investigated further this session.

    Spot-checked byte-level header plausibility on a few of bucket (a)'s
    genuinely-uncalled addresses (11D20, 1314C) to rule out the "phantom
    scraper artifact" possibility (the same false-header issue already
    found and fixed for D3C0/D5EC/D668/D796 elsewhere in this file, where
    "02 00" was actually leftover data from a preceding record, not a real
    header). Both checked out as plausible real headers (clean zero-padding
    or a real preceding record boundary immediately before each, header
    bytes shaped correctly) -- so at least these two are likely genuine
    tables that are simply never read by any statically-discoverable
    mechanism, not mis-scraped phantoms. Did not have time to check all
    remaining candidates this way; worth doing systematically in a future
    pass before writing any of them off as fully resolved-not-applicable.

    Updated running tally: 58 started -> 9 resolved this session total
    (13212, 11834, 12860, 12EAE, 11F6C, 13198, 12926, plus item-17's six
    were counted separately before this file existed) -- 49 remain, with
    12AE6 carrying a confirmed caller/subsystem identity despite no axis.

--------------------------------------------------------------------
27. #817-class decompiler bug: root-cause investigation in the real
    Ghidra C++ source (2026-08-03) -- confirms the reference-database
    workaround (item 19) is the correct fix at this layer; a "proper"
    core patch is not what it first looks like.
--------------------------------------------------------------------

CONTEXT: PJ asked whether the bank+offset call-argument #817 pattern
(item 19/26's "two visible call arguments never fused into a real far
pointer") could be patched properly in Ghidra's own C++ source, since
different ROMs may need different fixes. Read the actual mechanism
(userop.cc's SegmentOp::unify, coreaction.cc's ActionSegmentize::apply,
fspec.hh's ParamEntry/JoinRecord) rather than reasoning from the GitHub
issue thread alone.

FINDING 1 -- our SP/FP/Rn spSegment() CALLOTHER calls are NOT affected
by #817 at all. ActionSegmentize::apply() (coreaction.cc ~line 628)
already runs SegmentOp::unify() against every CALLOTHER matching a
declared <segmentop> userop, and correctly promotes qualifying ones to
a real CPUI_SEGMENTOP. unify() requires exactly a 3-input CALLOTHER
(useropid, base, inner) -- our spSegment(TP,SP)-style calls are exactly
that shape, emitted as one atomic SLEIGH semantic action inside a
single instruction's pcode, so they already convert correctly. Nothing
to fix here.

FINDING 2 -- the actual #817 pattern we hit (call-site bank+offset
pushed as two separate immediates: `mov #offset:16,@-SP` then
`mov #bank:8,@-SP` then pjsr) is structurally invisible to
SegmentOp::unify() and can never be made visible to it. unify() only
ever inspects the inputs of a single CALLOTHER pcode op. The two pushes
are separate instructions, lifted independently, with no pcode-level
connection to the pjsr that follows -- by the time pjsr's own pcode
exists, there is no operand linking it back to those two prior STOREs.
SLEIGH constructors are single-instruction (or single fixed encoding)
scoped and cannot reach backward into independently-matched prior
instructions to fuse their semantics. This is not a missing feature in
unify() or a bug -- the relationship we need (two stack args belonging
to one call) is an ABI/calling-convention-level fact, not a
single-instruction fact, and ActionSegmentize runs at the raw pcode
level with zero visibility into call/parameter semantics.

CONCLUSION on "patch it properly": there is no C++ change to
SegmentOp::unify or ActionSegmentize that could ever recognize this
pattern, because the information needed doesn't exist at the stage
where that mechanism runs. A real fix for this SPECIFIC bug shape would
require an entirely new Action running much later, after
parameter/prototype binding (where multi-slot arguments ARE already a
first-class concept -- see item below), not a modification to the
segmentop mechanism.

FINDING 3 -- the actually-promising unexplored angle: ParamEntry
already has native support for a logical parameter assembled from
multiple separate storage pieces, via JoinRecord / resolveJoin()
(fspec.hh ~line 116, same "join" space mechanism used for e.g. a 64-bit
value split across two 32-bit registers). This is exactly the shape of
our bank+offset call argument: two adjacent, fixed-size, fixed-order
stack slots (bank byte, offset word) that are really one logical
far-pointer argument. Unlike our earlier R0:R1 join attempt (cspec
lines 198-232, reverted -- see that history), which broke because it
was an OUTPUT/return-value join and polluted Ghidra's global
return-type inference model, an INPUT-side join for call arguments does
not touch that pollution path at all -- different mechanism, different
blast radius.

Sketch (not yet tried live):
  <pentry minsize="3" maxsize="3">
    <addr space="join" piece1="stack:1,offset=6" piece2="stack:2,offset=4"/>
  </pentry>
declared per-prototype (like our existing __stdcall_far_2arg etc.), so
different ROMs with different push order/sizes just get their own
prototype -- no C++ needed, purely a cspec-level per-function
declaration.

What this WOULD get us: Ghidra's parameter binding treats the two
pushes as one real 3-byte argument instead of two independent ones,
which could be typed directly as our far-pointer type in the
prototype.
What this would NOT get us: it still would not decompile as a clean
"bank*0x10000+offset" C expression -- the joined varnode is just a
3-byte blob unless we ALSO give it a genuine custom Datatype with a
real display/typeop hook, which pushes back into real C++ territory.
It also does not create the xref -- FixBankOffsetReferences_v2.java
(item 19) is still needed for that regardless.

STATUS: investigation only, nothing applied. NEXT STEP (see new TODO):
read ParamEntry::decode / resolveJoin in fspec.cc to confirm exact
<pentry><addr space="join" .../></pentry> XML syntax, whether piece1/
piece2 must be same-space (ours would be stack+stack, simpler than the
cross-register case Ghidra's join support was originally built for),
and any alignment/ordering constraints, BEFORE trying this live against
table_lookup_interp or a similar 2-arg far-pointer target.

VERDICT ON PJ's ORIGINAL QUESTION: no, there is no proper unify()/
ActionSegmentize-level C++ patch possible for the call-argument case --
that mechanism is structurally scoped to single instructions and this
is a cross-instruction ABI fact. The join-pentry angle (Finding 3) is
the closest thing to a "proper", per-ROM-configurable fix that stays in
cspec/XML rather than C++, but even if it works it only fixes parameter
BINDING, not the decompiled display -- FixBankOffsetReferences_v2.java
remains the correct/sufficient fix for the reference-database problem
regardless of whether the join-pentry experiment pans out.

--------------------------------------------------------------------
TODO (new, 2026-08-03): try the join-pentry approach from item 27
--------------------------------------------------------------------
1. Read ParamEntry::decode and resolveJoin in fspec.cc (not yet read)
   to confirm exact <pentry><addr space="join" piece1=... piece2=...
   /></pentry> syntax and any same-space/alignment/ordering
   constraints, before touching the live cspec.
2. Try it on ONE function first (table_lookup_interp or
   table_lookup_indexed -- both already have a real 2-arg prototype,
   __stdcall_far_2arg, to modify/clone), not a blanket change.
3. Verify via get_function_variables that the two stack slots bind as
   ONE joined parameter, not two, and that this does NOT reproduce the
   R0:R1 "pollutes global return-value model" regression from the
   cspec's own history (should be safe -- this is an INPUT join, not
   OUTPUT -- but confirm live, don't assume).
4. If binding works: try typing the joined param with a custom
   far-pointer Datatype and see whether decompiled display improves at
   all, or whether it's still just a 3-byte blob needing a real
   display/typeop hook (expected outcome per Finding 3 above).
5. Either way, FixBankOffsetReferences_v2.java's xref-creation stays in
   place -- this experiment is about parameter binding/display only,
   not a replacement for the reference-database fix.

--------------------------------------------------------------------
TODO CLOSED (2026-08-03), same session -- step 1 disproved the whole
approach before step 2 was needed. Negative result, recorded so this
doesn't get re-opened later.
--------------------------------------------------------------------

Read ParamEntry::decode (fspec.cc line 501) and ParamEntry::resolveJoin
(fspec.cc line 94) directly. Two independent, both-fatal problems with
the item-27 sketch:

1. ParamEntry::decode has NO piece1/piece2 attributes at all. A
   <pentry> element decodes exactly one <addr> child (line 550,
   `addr = Address::decode(decoder)`). The piece1="..." piece2="..."
   syntax in the item-27 sketch doesn't exist in the parser -- it was
   an invented syntax, not a real one. There is no per-pentry way to
   declare "this parameter equals stack-offset-6 plus stack-offset-4."

2. Even granting a way to construct a join-space Address some other
   way (e.g. a pre-built <join> space address, which is what
   ParamEntry::decode actually expects to receive), resolveJoin()
   (line 94-116) would still reject our case outright. Line 103-113:
   it iterates the join record's pieces and requires EACH piece to
   already match an independently-existing earlier ParamEntry in the
   same curList (findEntryByStorage) -- and throws
   "<pentry> join must overlap at least one previous entry" if any
   piece has no match. Our bank byte (stack+6) and offset word
   (stack+4) are never independently meaningful parameters anywhere
   else in h8539f.cspec's calling convention -- they only ever exist
   together as this one far-pointer argument. So even with correct
   syntax, cspec load would throw at this exact check.

CONCLUSION: the join-pentry mechanism is real but built for a
genuinely different case than ours -- e.g. two ALREADY-independently-
real registers/stack slots that also happen to jointly carry a larger
value elsewhere in the same convention (the classic case: two 32-bit
registers that separately hold 32-bit args in other prototypes, and
jointly hold a 64-bit arg in this one). Our bank+offset pair is never
independently addressable -- it's a joint-only value from the start --
which is exactly the shape resolveJoin's overlap requirement rejects.

This is NOT "hard, worth attempting anyway" -- it would fail to even
load at cspec-decode time. Confirmed by reading the actual decode/
resolveJoin source, not inferred from the header comments alone.

STATUS: join-pentry angle fully closed, no further action possible
here. No remaining "proper" cspec/C++ per-ROM fix identified for the
call-argument #817 pattern -- FixBankOffsetReferences_v2.java (item 19)
remains the correct and, as far as this investigation found, the ONLY
viable fix at any layer for this specific bug shape. Future sessions:
don't re-open the join-pentry idea without a genuinely different
mechanism in mind -- this exact angle was checked against the real
Ghidra source and dead-ends at ParamEntry::resolveJoin's overlap
requirement.

--------------------------------------------------------------------
27. CRITICAL: axis addresses were all off by 6 bytes -- pointing at the
    axis_lookup_interp record HEADER, not the real breakpoint data
    (2026-08-04)
--------------------------------------------------------------------

    DISCOVERY: user reported that several tables "validated" by items 17/
    24-26 rendered garbage in EcuFlash (huge nonsensical row/column
    headers, e.g. 240750 and 241438 for TABLE_3D_00012B06_CanisterPurge,
    plus far more rows than the specified element count). Investigated by
    decompiling axis_lookup_interp (0x14735) itself instead of continuing
    to infer its record layout from byte patterns alone. Confirmed record
    layout:
      offset 0: value_ptr (RAM cell holding the runtime X value, ALSO
                the destination the final result is written back to)
      offset 2: axis_ptr  (RAM cell holding the actual comparison value)
      offset 4: axis_count
      offset 6: axis_table -- the REAL breakpoint array starts HERE, not
                at the record's own address.

    Every axis address written into the XML by this project's
    axis_lookup_interp-based discovery work (items 17, 18, 24, 25, 26 --
    NOT just this session's additions, but earlier "already confirmed"
    work too, since it used the identical discovery method) pointed at
    the 6-byte record header instead of header+6. EcuFlash was rendering
    the header's own value_ptr/axis_ptr words (e.g. 0xF0C0=61632,
    0xF170=61808) as if they were the first two breakpoints, run through
    the RPM scaling's toexpr (x*1000/256): 61632*1000/256=240750.0 and
    61808*1000/256=241437.5~241438 -- an EXACT match to the garbage the
    user saw, conclusively confirming the bug (not just a pattern-match
    guess this time -- verified against the real decompiled function and
    the real scaling formula in RVR_base.xml).

    MAJOR SIDE-FINDING: 2d332 (2d32c+6), the ubiquitous "placeholder"
    address used across ~50 still-unresolved Unknown-category tables, was
    NEVER actually a broken placeholder -- it's the correctly-addressed,
    byte-confirmed real 18-element RPM axis all along. It just isn't
    necessarily the semantically-correct axis for every table still
    defaulting to it (that's a separate, still-open question), but its
    ADDRESS is valid and won't render garbage. This reframes a lot of the
    "58 tables need fixing" framing from items 24-26 -- most of those
    tables are not broken, just possibly using a generic (but real)
    fallback axis instead of a more specific one.

    FIX APPLIED: added +6 to every axis address in the Unknown-category
    section of the RVR-specific XML that was NOT already 2d332 (since
    2d332 already correctly happened to be a data-start address). Full
    list of addresses fixed (old -> new, element count unchanged in every
    case since the header never counted as a breakpoint to begin with):

      2d74c->2d752 (4, 127A8 X)         2d75a->2d760 (7, 127A8 Y)
      2d530->2d536 (9, 12B06 X)         2d50c->2d512 (15, 12B06 Y)
      2d32c->2d332 (18, 12B94 X)        2d356->2d35c (16, 12B94 Y)
      2d76e->2d774 (8, 131A0 X)         2d7b0->2d7b6 (10, 131A0/A8/B0 Y x3)
      2d784->2d78a (7, 131A8 X)         2d856->2d85c (10, 131B0 X)
      2d0f0->2d0f6 (16, x3 occurrences) 2d260->2d266 (7)
      2d130->2d136 (8)                  2d440->2d446 (11)
      2d2aa->2d2b0 (14)                 2d0c0->2d0c6 (6, 11F6C)
      2d490->2d496 (8)                  2d274->2d27a (9)
      2d180->2d186 (6)                  2d29c->2d2a2 (4, x2)
      2d1fa->2d200 (5)                  2d548->2d54e (17)
      2d09a->2d0a0 (16)                 2d116->2d11c (10, x2)
      2d5ee->2d5f4 (9)                  2d218->2d21e (21, 11834)
      2d620->2d626 (8, x2)              2d636->2d63c (10, x3)
      2d7ca->2d7d0 (5, 13198)           2d66a->2d670 (10)

    Every corrected address was byte-verified via read_memory to contain
    plausible real ramp/breakpoint data (not another header shape) before
    being written.

    NOTABLE CROSS-VALIDATIONS: several of these +6-corrected addresses
    land EXACTLY on already-established real axes elsewhere in the file --
    2d530+6=2d536 matches the pre-existing "Error" RPM axis (Idle Error
    Correction tables); 2d2aa+6=2d2b0 matches the Fuel Maps' RPM axis;
    2d09a+6=2d0a0 matches Open Loop Throttle's RPM axis; 2d116+6=2d11c
    matches Decel Fuel Cut Ignition Retard's "Engine Speed" axis; 2d218+6
    =2d21e matches MAF Scaling's axis exactly (21 elements, byte-for-byte).
    These are NOT coincidences -- they're strong independent confirmation
    that the +6 correction is right, not just a plausible-looking guess.

    OUT OF SCOPE / FLAGGED, NOT FIXED: two axis addresses OUTSIDE the
    Unknown-category section (2d490 used by "Warmup Ignition Control vs
    Eng Temp", and 2d260 used by "Injector Battery Voltage Latency
    Compensation" -- both pre-existing, real-named "Timing"/other category
    tables, not part of the ROM-scraper's Unknown section) tested as
    header-shaped bytes too when spot-checked, meaning they may have the
    SAME bug despite predating this project's scraper work entirely.
    NOT fixed this session -- flagged for the user's decision since it's
    outside the stated "Unknown tables" scope and touches tables with real,
    named EcuFlash provenance.

    NEXT STEP: re-render the corrected tables in EcuFlash to confirm clean
    axis display (no more garbage leading rows/columns). Also worth
    auditing base.xml's OWN axis addresses the same way (item 26's earlier
    base.xml spot-checks used a DIFFERENT, unrelated bug pattern --
    mid-data collisions from wrong table-of-origin, not this header-offset
    bug -- so base.xml's core tables are a separate concern, but the two
    flagged above straddle both worlds and deserve a dedicated look).

28. TABLE_3D_00012DF2 / TABLE_3D_00012E4A axis fix -- CONDITIONAL-SELECT
    table pair missed by all prior discovery methods (2026-08).

    User reported EcuFlash rendering these two 3D tables with alternating
    zero-striped columns (screenshots: TABLE_3D_00012DF2, TABLE_3D_00012B94,
    TABLE_2D_00011834_Knock all showed the same symptom). 12B94 and 11834
    turned out to be instances of item 27's already-known +6 header-offset
    bug / missing-axis bug. 12DF2 was different: table body data at 12DF2
    verified genuine (byte-for-byte matches EcuFlash's displayed row 0:
    03 00 F0 C0 F0 C2 0A 00... = 3,0,240,192,240,194,10,0...), and its axes
    (both X and Y set to the generic 2d332/18-element placeholder) had NO
    entry in any of the 19 known array-indirection tables (0x2d8ec through
    0x2dbcc) and no literal push anywhere in the ROM -- exhaustively checked
    via byte-pattern search, all misses.

    Traced instead by searching for the raw 2-byte pointer value alone
    (0x2df2) rather than any known call convention: one hit, at 0x1871e,
    inside a real, previously-unnamed-in-XML function egr_duty_target_calc
    (0x186fa-0x18791, plate comment already existed from a prior session but
    was never connected to this table). Read the function's RAW disassembly
    directly (not the decompiled C, which had abstracted the stack-pushed
    axis pointers away into opaque stack slots):

      000186fa: link FP,#-0x4:8
      00018708: mov:g.w #0x1:8,@(-0x4:8,FP)
      0001870c: mov:g.w #0x2e4a:16,@(-0x2:8,FP)   ; default table = 0x2e4a
      00018711: btst.w 0x5,@0xf0f8:16
      00018715: beq #0x00018720:8
      00018717: mov:g.w #0x1:8,@(-0x4:8,FP)
      0001871b: mov:g.w #0x2df2:16,@(-0x2:8,FP)   ; F0F8 bit5 set -> 0x2df2 instead
      00018720: mov:g.w #0xd606:16,@-SP
      00018724: mov:g.w #0x2:8,@-SP
      00018727: pjsr @0x14735:24                  ; axis_lookup_interp(bank=2, 0xd606)
      0001872e: mov:g.w #0xd620:16,@-SP
      00018732: mov:g.w #0x2:8,@-SP
      00018735: pjsr @0x14735:24                  ; axis_lookup_interp(bank=2, 0xd620)
      0001873c: mov:f.w @(-0x2:8,FP),R0            ; selected table ptr
      00018744: pjsr @0x14656:24                   ; table_lookup_interp
      ...
      0001874d: mov:g.w #0x2ea2:16,@-SP
      00018754: pjsr @0x14656:24                   ; third table (0x2ea2, already
                                                     ; resolved elsewhere as
                                                     ; TABLE_2D_00012EA2_EGR)
      00018761: pjsr @0x142f3:24                    ; blend function combining both

    Root cause: this function does TWO independent axis_lookup_interp calls
    (0xd606 and 0xd620) feeding a genuine 2-axis blend at 0x14735, then
    conditionally selects between value tables 0x2df2/0x2e4a based on F0F8
    bit5 -- a dispatch shape none of the existing discovery scripts looked
    for (they all assumed either a literal push immediately before
    table_lookup_interp, or an array-of-pointers indirection; this is
    neither -- it's a literal table-select branch combined with axes that
    are looked up separately and NOT positionally adjacent to the value-
    table push). TABLE_3D_00012E4A is the sibling selected on the same
    branch (F0F8 bit5 clear).

    Both axes header byte-verified as real (not header-shaped mid-data):
      2d606: F0C0 F170 000A (10 elements), real breakpoints at 2d606+6=2d60c
      2d620: F0C2 F190 0008 (8 elements), real breakpoints at 2d620+6=2d626
        -- 2d620/2d626 is the SAME axis already confirmed correct for
        TABLE_2D_00012EA2_EGR and TABLE_2D_00012EAE_EGR (the third table,
        0x2ea2, called from the same function) -- strong independent
        cross-validation this axis identification is right.

    XML FIXED: TABLE_3D_00012DF2 and TABLE_3D_00012E4A both changed from
    X Axis=2d332(18)/Y Axis=2d332(18) [wrong, generic placeholder] to
    X Axis=2d60c(10 elem)/Y Axis=2d626(8 elem) [real, byte-verified axes].
    Inline XML comments document the full disassembly trace so this isn't
    rediscovered blind.

    SEPARATELY: while investigating this function, found its Ghidra
    signature was `undefined egr_duty_target_calc(void)` -- untyped 1-byte
    return despite genuinely returning a 16-bit value (confirmed via the
    mul_u16_x2_sat result feeding the return). Fixed via set_function_
    prototype to `ushort egr_duty_target_calc(void)` (note: calling
    convention must be given as "__stdcall", NOT "default" -- "default" is
    rejected as an unknown convention name on this program). Re-decompiled
    clean after the change, no regression. Function name itself was already
    correct, no rename needed. Sibling function canister_purge_duty_calc_f4ac
    (0x19560) has the identical untyped-return issue, NOT yet fixed --
    flagged for a future pass.

    NEXT STEP: re-render TABLE_3D_00012DF2/TABLE_3D_00012E4A in EcuFlash to
    confirm clean 10x8 grids instead of striped garbage. Also worth checking
    whether any other still-on-2d332-placeholder "Unknown" tables have this
    same conditional-select dispatch shape rather than item 27's simpler
    +6-header-offset bug -- the discovery method here (raw 16-bit pointer
    byte search, not call-convention-assuming script) may need to be run
    across the remaining placeholder tables.

29. Follow-up scan for more conditional-select dual-axis table dispatches
    (same shape as item 28), plus a return-type audit of the table-lookup/
    actuator function family (2026-08).

    METHOD: pulled all callers of axis_lookup_interp (0x14735) via
    get_xrefs_to -- 87 call sites across ~60 distinct functions. Functions
    calling it exactly twice (or in an obvious paired/conditional block) are
    candidates for item 28's dispatch shape. Checked each candidate's raw
    disassembly, not decompiled C (decompiler strips the stack-pushed axis
    pointers into opaque slots, same issue as item 28).

    SECOND CONFIRMED INSTANCE FOUND: knock_octane_dual_axis_table_blend_f2a8
    (0x2339d-0x23423). Its own pre-existing plate comment already flagged
    "2x axis_lookup_interp + 2x table_lookup_indexed (table selected by f0f8
    bit5)" at low-medium confidence with unconfirmed table identities --
    this session confirms and completes that comment. Raw disassembly:

      0002339f: pjsr @0x23424:24                 ; f110_f17a_threshold_gate_check
      000233a3: mov:g.w R0,@0xf0ba:16
      000233a7: mov:g.w #0xd2e0:16,@-SP
      000233ab: mov:g.w #0x2:8,@-SP
      000233ae: pjsr @0x14735:24                 ; axis_lookup_interp(bank2,0xd2e0)
      000233b5: mov:g.w #0xd302:16,@-SP
      000233b9: mov:g.w #0x2:8,@-SP
      000233bc: pjsr @0x14735:24                 ; axis_lookup_interp(bank2,0xd302)
      000233c3: btst.w 0x5,@0xf0f8:16
      000233c7: beq #0x000233d2:8
      000233c9: mov:g.w #0xd9ac:16,@-SP          ; F0F8 bit5 SET
      000233d2: mov:g.w #0xd9cc:16,@-SP          ; F0F8 bit5 CLEAR
      000233d9: pjsr @0x14854:24                 ; table_lookup_indexed(2, array)
      000233e4: btst.w 0x7,@0xf1f2:16             ; second, nested condition
      000233e8: beq #0x0002341c:8
      000233ea: btst.w 0x5,@0xf0f8:16
      000233f0: mov:g.w #0xd96c:16,@-SP          ; F1F2 bit7 SET, F0F8 bit5 SET
      000233f9: mov:g.w #0xd98c:16,@-SP          ; F1F2 bit7 SET, F0F8 bit5 CLEAR
      00023400: pjsr @0x14854:24                 ; second table_lookup_indexed
      00023413: pjsr @0x14599:24                 ; weighted_blend_u8frac(a,b,frac)
      0002341c: mov:g.w R2,@0xf2a8:16             ; result stored to F2A8

    Both axes header byte-verified real: 2d2e0 = F0C0 F170 000E (14 elements,
    data at 2d2e6), 2d302 = F0C2 F0BA 0012 (18 elements, data at 2d308).

    NOTE: this call uses table_lookup_indexed (0x14854), not
    table_lookup_interp (0x14656) like item 28 -- its args are
    (bank, array_base), consistent with review.md item 22's already-
    documented dual-table-blend array-indirection convention. Read all four
    candidate addresses as arrays (not table headers) accordingly, and all
    four resolved cleanly to 8-identical-slot indirection arrays (matching
    the established signature):
      2d9cc -> table 0x11bc0   (F0F8 bit5 clear)
      2d9ac -> table 0x11abc   (F0F8 bit5 set)
      2d98c -> table 0x119b8   (F1F2 bit7 set AND F0F8 bit5 clear)
      2d96c -> table 0x118b4   (F1F2 bit7 set AND F0F8 bit5 set)
    Spot-verified 0x11bc0's own data: 03 00 F0 C0 F0 C2 0E 8D 80 80...
    matches the established table-header shape exactly (mode byte 03,
    axis pointers F0C0/F0C2, count 0x0E=14). None of these four tables or
    their axes have any entry in the XML yet -- net NEW tables, not a fix
    to existing wrong ones. NOT YET ADDED TO XML (four tables x one shared
    axis pair -- needs proper category/naming, left for a dedicated pass
    since "Timing/Knock" category and real names aren't confirmed, same
    caveat as the function's own plate comment).

    STILL UNCHECKED from the 87-site caller list (same axis_lookup_interp-
    twice heuristic, not yet individually disassembled): isc_f42a_table_
    lookup, idle_stepper_table_lookup_wrapper, isc_integrator_update,
    f4de_f4e2_octane_correction_calc, tcu_shift_solenoid_table_init,
    map_tps_axis_update, isc_f374_correction_calc, f0ba_f2d0_o2_mode7_8_
    correction_calc, calc_f110_f10e_via_table, isc_f34e_f34a_correction_
    calc, isc_openloop_stepper_scale_calc. Some of these may be ordinary
    2-different-purpose axis lookups rather than a conditional-select pair
    -- needs the same raw-disassembly check per function before assuming
    the item 28/29 shape applies, NOT a blind pattern match on call count
    alone (isc_openloop_target_calc calls it 5x, isc_integrator_update 4x,
    tcu_shift_solenoid_table_init 5x -- these look like multiple genuinely
    distinct table lookups in one function, not the paired-select shape).

    RETURN-TYPE AUDIT (separate finding, same session): spot-checked
    signatures of every function found via the axis_lookup_interp caller
    list above. ALL are typed plain `undefined` (untyped, 1-byte) return --
    isc_f354_table_lookup_conditional, tcu_dtc_status_check_dispatch,
    isc_f380_f37e_correction_calc, f28e_f292_knock_lambda_composite_calc,
    isc_openloop_target_calc, o2_heater_duty_downstream_correction,
    ign_advance_idle_correction_f44e, knock_octane_dual_axis_table_blend_
    f2a8, egr_duty_target_calc (fixed in item 28) all confirmed `undefined`
    at time of check. For contrast, spot-checked the low-level fixed-point
    math library (sat_add_u16 @ 0x14000) -- that one IS properly typed
    (`ushort __stdcall_far_allstack sat_add_u16(ushort a, ushort b)`), so
    this is NOT a universal project default -- it's specific to (at least)
    the table-lookup/actuator dispatcher family, ~60 functions by the
    axis_lookup_interp caller count alone, likely more considering
    table_lookup_interp/table_lookup_indexed/table_read_indexed have their
    own separate, overlapping caller lists not yet cross-checked.

    NOT YET FIXED (scope decision needed): retyping every one of these is
    a real, mechanical, per-function job (confirm real return width from
    decompile -> set_function_prototype with __stdcall convention, per
    item 28's already-proven method and the "default is not a valid
    convention name" gotcha) but doing ~60+ of them here would be a large,
    unreviewed batch change. Only egr_duty_target_calc has been fixed so
    far (item 28). Recommend a dedicated batch pass, confirming each
    function's real return usage from decompile before retyping (some may
    be genuinely void/side-effect-only, e.g. ones that only write to a
    global and never use R0) rather than blindly typing all of them ushort.

    NEXT STEP: (1) raw-disassembly-check the 11 still-unchecked callers
    above for the item 28/29 dual-axis-select shape; (2) decide whether/how
    to add the 4 new EGR-adjacent tables (11bc0/11abc/119b8/118b4) to the
    XML with real names; (3) scope and execute the return-type batch pass
    if desired, confirming real usage per-function rather than blind-typing.

30. Follow-up on item 29's 11 unchecked callers -- item 28/29 dispatch-shape
    check completed (2026-08).

    Raw-disassembly-checked all 11 remaining axis_lookup_interp callers
    flagged in item 29 as unverified. Result:

    - isc_f34e_f34a_correction_calc: ALREADY FULLY RESOLVED in this file
      (see item 22's xref list, arrays 0x2dbac/0x2da0c/0x2db8c/0x2dbcc all
      previously found and documented). Independently re-derived the exact
      same 4 arrays this session before checking -- good cross-check that
      the item 28/29 raw-disassembly method produces correct results, no
      new action needed.
    - isc_f42a_table_lookup, idle_stepper_table_lookup_wrapper,
      isc_openloop_stepper_scale_calc, f4de_f4e2_octane_correction_calc,
      isc_f374_correction_calc, calc_f110_f10e_via_table,
      f0ba_f2d0_o2_mode7_8_correction_calc: all confirmed NOT the item
      28/29 conditional-select shape. Each does either (a) two sequential
      axis_lookup_interp calls with NO branch between them feeding one
      fixed table (genuine 2-axis interpolation, different bug class), or
      (b) an outer condition that picks between two independent branches
      each with its own single axis lookup (not a shared dual-axis blend).
      idle_stepper_table_lookup_wrapper's axes (2d4ba/2d4d0) and
      isc_f374_correction_calc's table (0x12b94) were spot-checked against
      the XML and are already correctly resolved -- no fixes needed.
    - isc_integrator_update: PARTIAL MATCH, not fully resolved. This is a
      large multi-purpose function (0x17dd3-0x1821b) with two separate
      axis-pair blocks embedded in unrelated state-machine logic: block 1
      (0xd856/0xd7b0 -> table 0x131b0, called twice for two inputs) and
      block 2 (0xd784/0xd7b0 -> table 0x131a8, same shape), selected by an
      outer btst on F45A bit3, not F0F8 bit5 like items 28/29. Looks
      structurally similar (branch-selected table+axis pair) but embedded
      in a much larger function and not cleanly isolated -- flagged for a
      dedicated follow-up session rather than resolved here.
    - isc_openloop_target_calc (5 calls) and tcu_shift_solenoid_table_init
      (5 calls): NOT checked this session -- high call count is likely
      multiple genuinely distinct lookups (as suspected in item 29) rather
      than the paired-select shape, based on the pattern seen in the other
      9, but not individually verified. Left open.

    CONCLUSION: item 28/29's conditional-select dual-axis-table dispatch
    shape appears to be rare, not systemic -- only 2 confirmed clean
    instances in the whole ROM (egr_duty_target_calc / item 28,
    knock_octane_dual_axis_table_blend_f2a8 / item 29), plus one already-
    known instance (isc_f34e_f34a_correction_calc) and one partial/embedded
    candidate (isc_integrator_update) needing further isolation. The 19
    already-catalogued array-indirection tables plus these 2-3 dispatchers
    likely account for most/all of the ROM's non-literal-push table calls.

31. Return-type audit, scope decision and start of batch fixes (2026-08).

    Per item 29's finding that the axis_lookup_interp/table_lookup_interp/
    table_lookup_indexed caller family is consistently typed plain
    `undefined` (untyped, 1-byte) despite most genuinely returning/storing
    a 16-bit value, while the math library (sat_add_u16 etc.) is properly
    typed -- decision: fix per-function, confirming real return usage from
    the decompile first (does it return via R0 to a caller that uses the
    result, or only write a global and return void), rather than blindly
    retyping the whole family to ushort. Method proven in item 28:
    set_function_prototype with the `__stdcall` calling convention (NOT
    "default" -- rejected as an unknown convention name on this program),
    then force_decompile to confirm a clean re-decompile with no regression.

    Fixed so far: egr_duty_target_calc (item 28, ushort).

    STILL TO DO, confirmed real 16-bit-return candidates from item 29's
    spot-check list (all currently `undefined`, all confirmed via their own
    decompile to return a used value in R0, not just write a global):
      knock_octane_dual_axis_table_blend_f2a8 -- returns via R2/uVar2,
        which the caller stores to F2A8; candidate for ushort
      isc_f354_table_lookup_conditional -- not yet decompiled to confirm
        return usage this session
      tcu_dtc_status_check_dispatch -- not yet decompiled to confirm
      isc_f380_f37e_correction_calc -- not yet decompiled to confirm
      f28e_f292_knock_lambda_composite_calc -- not yet decompiled to
        confirm
      isc_openloop_target_calc -- not yet decompiled to confirm
      o2_heater_duty_downstream_correction -- not yet decompiled to confirm
      ign_advance_idle_correction_f44e -- not yet decompiled to confirm

    NOT YET DONE: confirming each of the above via decompile before
    retyping, then applying set_function_prototype to each. Given the
    caller list for axis_lookup_interp alone is ~60 functions (and
    table_lookup_interp/table_lookup_indexed/table_read_indexed have their
    own separate, larger, only-partially-overlapping caller lists), this is
    a genuinely large batch job. Recommend continuing it function-by-
    function in a dedicated pass (confirm-then-retype, not blind batch),
    logging each fix here as done.

32. Item 31's 8 candidates completed -- all confirmed via decompile before
    retyping, applied via set_function_prototype (__stdcall convention),
    no regressions (2026-08).

    RETURN-VALUE VERDICT vs FIX APPLIED (all changed FROM plain `undefined`):
      isc_f354_table_lookup_conditional      -> ushort  (returns AND stores
        uVar1 to F354 -- real used return, matches item 28's egr shape)
      tcu_dtc_status_check_dispatch          -> void    (long dispatcher,
        table result used only in internal comparisons, never returned)
      isc_f380_f37e_correction_calc          -> void    (writes F37E/F380/
        F85E globals, no caller-usable return)
      f28e_f292_knock_lambda_composite_calc  -> void    (writes 5 separate
        globals: F292/F296/F0BA/F294/F290/F28E, no return)
      isc_openloop_target_calc               -> ushort  (genuinely returns
        uVar1 -- either a real muldiv_s16_rounded result or literal 0 --
        both real values used by its caller)
      o2_heater_duty_downstream_correction   -> void    (writes F4DC/F4F4
        globals, no return)
      ign_advance_idle_correction_f44e       -> void    (writes F44E global
        only, no return)
      knock_octane_dual_axis_table_blend_f2a8 -> void   (CORRECTION from
        item 29's tentative "candidate for ushort" note -- re-decompiled
        and confirmed this one only writes F2A8, does NOT return via R0 to
        any caller; item 29's guess was wrong, void is correct)

    Net: 2 of 8 were genuinely ushort (real, used 16-bit returns), 6 of 8
    were genuinely void (globals-only side effects) despite ALL 8 sharing
    the same untyped `undefined` starting signature. This confirms item
    31's decision to confirm-before-retype rather than blind-batch was
    right -- a blind ushort sweep would have mistyped 6 of these 8 (75%).

    BONUS FINDING while decompiling isc_openloop_target_calc: it also
    contains item 28/29's conditional-select dual-axis shape internally
    (F45A bit8 selects between axes 0xd784/0xd7b0), directly resolving
    item 30's open "not yet checked" flag on this function -- it IS a
    (partial) match, same as isc_integrator_update. Its own parameter list
    (param_1/2/3, currently uncaptured in the Ghidra signature -- only the
    return type was fixed here) is a separate open item, not resolved this
    session; real parameter types/widths not yet confirmed, left as `void`
    args deliberately rather than guessed.

    STILL OPEN: tcu_shift_solenoid_table_init (item 30, not yet checked for
    the dispatch shape); isc_integrator_update's embedded dispatch blocks
    not yet cleanly isolated (item 30); isc_openloop_target_calc's own
    parameter types not yet resolved (this item); the remaining ~50+
    functions in the broader axis_lookup_interp/table_lookup_interp/
    table_lookup_indexed/table_read_indexed caller lists not yet audited
    for return type at all (item 31's list was only the 8 spot-checked in
    item 29 -- this is a small fraction of the full untyped-return
    population in this function family).





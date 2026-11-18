Review 5: axis identity/scaling verification against live ROM callers
====================================================================

GOAL: RVR_base.xml table addresses were largely verified for correctness in
review2.md (Batch 1, H8539F-TABLE/AXIS-OVERLAP bookmarks). That pass was
explicitly scoped to ADDRESSES ONLY -- names and scaling were left as-is
even when known wrong, because the point was "is there a real table here",
not "is this table what the name claims". This file picks up where that
one stopped: verify that each axis's NAME, UNITS, and SCALING formula
actually match what the ROM code does with it, independent of what the
community EcuFlash XML says. The community XML has already been shown
wrong in specific, confirmed cases (review.md item 19, review2.md item 2,
etc.) so inherited names/scalings are leads, not evidence.

This is NOT a small pass. RVR_base.xml currently has 45+ named X-Axis
entries and a similar count of Y-Axis entries, several of which already
show internal self-contradictions on inspection alone (see SEEDS section
below) before any ROM tracing has even happened.

Prerequisite now satisfied: review4.md's return-type audit of
table_lookup_interp/axis_lookup_interp/table_lookup_indexed/
table_read_indexed callers is COMPLETE (2026-08-05, 15 genuine fixes
landed). Real callers of these four primitives should now decompile with
correct return types, so tracing "what does this axis actually feed"
should surface real comparisons/RAM writes instead of being obscured by
bare-void noise. Read review4.md first if a caller here still looks wrong
-- check whether it was one of the 15 fixed, or still open.

--------------------------------------------------------------------
METHOD (per axis)
--------------------------------------------------------------------
1. Identify the axis's real address and element count from RVR_base.xml
   (may differ from what the community XML originally had -- confirm live
   against ROM bytes first, same as review2.md Method step 1).
2. Read the raw bytes. Confirm shape: is this axis data alone, or does it
   sit right after a 6-byte scaling-table header (`[F0-F7] xx [E0-FE] xx
   00 [elem_count]`, confirmed format, see logging notes 2026-08-05)? If a
   header exists, note it -- it independently confirms element count.
3. Find real callers via axis_lookup_interp / table_lookup_interp xrefs
   (get_xrefs_to on the axis address, or search literal-push bytes as in
   review2.md Method step 4 if xrefs are thin).
4. Decompile at least one real caller. Determine what RAM cell or
   physical-looking quantity the axis is interpolated against (e.g. is it
   compared against a known ADC channel RAM address, a crank-timer-derived
   value, a fixed sensor read -- see the ADC/RAM pointer map already
   established in RVR_base.xml's header notes: F0C0=RPM, F0C2=Load/MAP,
   F0C4=CoolantTempA(ADC), F0C6=AirTemp(ADC), F0C8=CoolantTempB(ADC),
   F0CA=TimeAfterStart, F0CE=ISCVDemand, F0D0=IAT_raw).
5. Cross-check the raw value RANGE against the claimed scaling formula.
   e.g. RPM axis raw values run 500-7000 -> plausible with existing "RPM"
   scaling; a signed range centered near zero (-64..+64) is NOT a sensor
   reading, it's a trim/correction/error term, and any "sensor" scaling
   assigned to it is wrong regardless of what any XML says.
6. Record verdict: CONFIRMED CORRECT (name+scaling both match caller
   evidence) / CONFIRMED WRONG (state the real name/scaling, with
   evidence) / UNRESOLVED (caller trace inconclusive, needs a second
   pass). Only correct entries with CONFIRMED WRONG verdicts. Never guess.
7. When fixing RVR_base.xml, keep the same discipline as review2.md: only
   change what's proven, leave everything else exactly as found, note the
   change inline with a dated comment.
8. STANDING RULE (added 2026-08-09, PJ's instruction): axis/table data can
   never legitimately overlap another table -- two structures cannot
   physically occupy the same ROM bytes. Whenever an axis's real element
   count is UNRESOLVED/UNCONFIRMED (caller trace inconclusive, no
   scaling-header cross-check available), its element count in
   RVR_base.xml MUST be capped so the data span (address + elements*2 for
   uint16, or *1 for uint8) does NOT reach the start address of the next
   known table/axis in ROM order. This is a placeholder safety bound, not
   a claim about the true axis length -- tag it with a dated comment
   stating it's a physical-gap cap pending a real caller trace (e.g. "el
   count capped to N pending caller trace, real length unconfirmed").
   Never leave an unconfirmed axis's element count large enough to
   overlap a neighbor even temporarily. When the real length is later
   confirmed (header cross-check or caller trace), replace the cap with
   the real value and remove the placeholder tag. Precedent: this was
   already done ad hoc for 0x11f4a/0x127d8/0x1297a before being written
   up as a rule here (see the Group A trim-curve entries elsewhere in
   this file) -- including a caught mislabeling bug from doing this too
   hastily (axis identity must still be checked, not just the count).

Status key: OPEN / CONFIRMED CORRECT / CONFIRMED WRONG-FIXED /
CONFIRMED WRONG-UNRESOLVED

--------------------------------------------------------------------
SEEDS -- self-contradictions found by inspection alone (2026-08-05),
before any ROM tracing. Start here; these are the highest-confidence
leads that something is actually wrong, not just unverified.
--------------------------------------------------------------------

1. Address 0x2d418 used as BOTH "Engine Temp" (multiple tables: Cranking
   Primer neighbor lines 256/260/264/348) AND "Air Temp" (lines 352, 485,
   489) across different tables in the same file. Only one physical
   sensor can live at one RAM/ROM address -- either every "Air Temp" use
   is a mislabel of a real Engine Temp axis, every "Engine Temp" use is a
   mislabel of a real Air Temp axis, or (least likely, but must be
   checked) the ROM genuinely reuses one axis definition for two
   different table families that happen to want the same breakpoints
   (possible if e.g. Air Temp and Coolant Temp sensors share an
   identical ADC scaling curve -- would need the caller trace to
   distinguish which RAM cell each specific table's real caller reads).

   PARTIAL PROGRESS 2026-08-05 [Claude, import-script header-behind
   check]: 0x2d418's address itself is now confirmed correct -- a header
   sits 6 bytes behind it at 0x2d412 (F0C8 F10E 0008, count=8 matches),
   data starts exactly at 0x2d418. This corrects a stale assumption in
   the import script's own docstring, which had called 0x2d418
   "headerless"; nobody had checked "-6" before. Doesn't resolve the
   Engine-Temp-vs-Air-Temp naming question, but rules out an address bug
   as the cause.
   Also found a related case while checking a different seed: 0x2d4c0
   (8 elements, claimed as "Coolant Temp"/"Air temp"/"Air Temp" across 3
   tables, lines 478/485/489) has a header 6 bytes behind it at 0x2d4ba
   (F0C0 F10E 0008, count=8 matches) -- structurally fine, but header
   ptr1=F0C0=RPM, not any temp sensor. None of the 3 claimed names match
   the pointer. Folds into this same open question, needs the same
   caller trace.

   RESOLVED 2026-08-05 [Claude, RVR_base.xml + duplicate variant fixed]:
   independent cross-check (user-supplied Ghidra bookmark export) showed
   9 of 10 uses of 0x2d418 in RVR_base.xml already read "Engine Temp",
   including on "Desired Idle RPM-Drive" -- an idle/RPM-adjacent table
   that fits an engine-temp compensation axis far better than air temp.
   Only one outlier disagreed: the X-Axis on "Ignition Trim vs Air Temp"
   (was named "Air Temp"). Fixed that single entry to "Engine Temp" in
   both RVR_base.xml and the 21000011_...Mt_4g63t_.xml duplicate, with
   inline dated comments. The related 0x2d4c0 three-way naming conflict
   (Coolant Temp/Air Temp/Air Temp, header points to RPM) is NOT resolved
   by this fix and remains open -- still needs its own caller trace.
   STATUS: CONFIRMED WRONG-FIXED (0x2d418 only). 0x2d4c0 STATUS: OPEN
   (unchanged, naming only; shape confirmed correct).

2. Address 0x2d2e6 used as "RPM" (X Axis, 14 elements, line 313, on Lean
   Spool Trailing Time) AND as "Engine Load" scaled "RPM" (X Axis, 10
   elements, lines 403/407, on Knock/Octane Control Above Load). Two
   different element counts at the supposedly same address is already a
   contradiction before even getting to the name -- one of these element
   counts (or the address itself) has to be wrong. Also "Engine Load"
   scaled as "RPM" units is internally inconsistent regardless of address.

   RESOLVED 2026-08-05 [Claude, import-script header-behind check]: a
   header sits 6 bytes behind 0x2d2e6, at 0x2d2e0 (F0C0 F170 000E,
   count=14). Data at 0x2d2e6 is 14 elements:
   128,256,384,512,640,768,896,1024,1152,1280,1408,1536,1664,1792 (raw,
   even +128 steps) -- a clean RPM timer-count curve, header ptr1=F0C0=RPM
   confirms. The 14-element "RPM" reading (line 313) is CORRECT. The
   10-element "Engine Load" reading (lines 403/407) is WRONG -- same
   address, but the real record has 14 elements, not 10, and is RPM.
   STATUS: CONFIRMED WRONG-FIXED 2026-08-05 (14-elem "RPM" reading
   confirmed correct; the two 10-elem "Engine Load" entries -- on Knock
   Control Above Load and Octane Control Above Load -- fixed to 14-elem
   RPM in both RVR_base.xml and the 21000011_...Mt_4g63t_.xml duplicate,
   with inline dated comments. Independently corroborated by a
   user-supplied Ghidra bookmark export showing the same ROM-vs-XML
   element-count disagreement (ROM 14 vs XML 10) from a second sweep.)

3. Address 0x2d536 named "Error" but scaled "RPM" (X Axis, 9 elements,
   lines 493/497, on Idle Error Correction tables). Either this is
   genuinely an RPM-error delta (plausible for idle control -- error
   between target and actual RPM) and the NAME should be something like
   "RPM Error", or the scaling is wrong and it's a differently-scaled
   error term. Needs caller trace to Idle Error Correction logic to
   confirm which.

   PARTIALLY RESOLVED 2026-08-05 [Claude, import-script header-behind
   check]: header sits 6 bytes behind 0x2d536, at 0x2d530 (F0C2 F0BA
   0009, count=9, matches). Header ptr1=F0C2=Load/MAP -- the SAME
   pointer as the confirmed-good Load axis @2d308. Raw data at 0x2d536:
   0,8,16,24,32,40,48,56,64 -- a clean small monotonic curve, not a
   signed/centered error term (which is what a real RPM-error delta
   should look like). This looks like a mislabeled Load axis, not an
   RPM-error delta. Address itself needs no change; naming/scaling still
   needs the caller trace before editing the XML.
   ADDITIONAL CONTEXT 2026-08-05 [user-supplied Ghidra bookmark export]:
   0x2d536 is also independently confirmed shared with
   TABLE_3D_00012B06_CanisterPurge, where RVR_base.xml already correctly
   names it "RPM" (with an inline 2026-08-04 comment cross-validating it
   against this same "Error" axis on Idle Error Correction). So the
   ADDRESS/sharing is not in question -- both table families genuinely
   use the same 9-element record. The open problem is narrower than
   originally framed: it is specifically the "Error" name on the two
   Idle Error Correction tables that's wrong, not a mismatch between
   Canister Purge and Idle Error Correction. Still not fixed -- the
   header pointer plus clean monotonic data argue for a Load-family
   name, but no caller trace has confirmed the exact replacement name
   yet, so per this file's own discipline (never guess) this stays open.
   STATUS: CONFIRMED WRONG-UNRESOLVED (address correct and shared
   correctly with Canister Purge; "Error" name on Idle Error Correction
   tables still wrong, exact replacement name not yet confirmed by
   caller trace).

   SUPERSEDED 2026-08-06 [Claude, real caller traced directly]: the
   above reasoning was analyzing the WRONG axis. Idle Error Correction's
   actual real caller (single shared function @0x272c6, dispatches to
   either 0x1295e "Neutral" or 0x1296c "Drive AC On" via a bit test on
   0xf0f8) was disassembled directly and pushes axis 0xD146 (real
   0x2d146) into axis_lookup_interp -- NOT 0xD536/0x2d536 at all. The
   0x2d536 vs Idle-Error-Correction "cross-validation" in the
   Canister Purge XML comment (added 2026-08-04) was therefore a
   coincidence, not a real shared record -- corrected in the XML now.
   Header at 0x2d146 = F0C0 F0BA 0009 (ptr1=F0C0=RPM, count=9, matches
   XML element count). Data: 0,8,16,24,32,40,48,56,64 -- the exact same
   breakpoint VALUES as 0x2d536, just at a different address with a
   different header pointer (F0C0/RPM here vs F0C2/Load-MAP at 2d536).
   XML address fixed 2d536->2d146 on both Idle Error Correction tables
   in new/RVR_base.xml and the 21000011_...Mt_4g63t_.xml duplicate
   (old/ untouched, out of scope as always).
   NAME LEFT AS-IS, NEW OPEN QUESTION: ptr1=F0C0 matches every other
   confirmed-RPM axis's header convention in this file, but the raw
   breakpoint range (0-64) is far smaller than every other confirmed RPM
   axis (which run in the hundreds-to-low-thousands, e.g. 2d2e6's
   128-1792, 2d332's 0-2816). Either (a) this genuinely is RPM but
   scaled/represented differently here (e.g. a coarse RPM-band index
   rather than a raw timer count), or (b) the "ptr1 = physical sensor
   type" convention that has held for every other axis in this file
   breaks down for this specific record and it's actually some other
   quantity that happens to read F0C0. Needs one more trace: decompile
   axis_lookup_interp's actual comparison against @0xf0c0 at this call
   site (not just the header field) to see what raw value range is
   really being compared, before deciding whether "Error"/"RPM" as
   currently written is right, needs a scaling change, or needs a full
   rename. STATUS: CONFIRMED WRONG-FIXED (address only, 2d536->2d146);
   name/scaling STILL UNRESOLVED, narrower and better-evidenced than
   before but not closed.

   FOLLOW-UP 2026-08-07 [Claude]: re-verified the literal push at raw
   disassembly level (not just decompile) for isc_f408_correction_calc
   @0x272c6, the sole real caller reaching 0x2d146 via axis_lookup_interp.
   Confirmed exact instruction: `mov:g.w #0xd146:16,@-SP` at 0x27332,
   immediately followed by `pjsr @0x14735` (axis_lookup_interp) at
   0x27339 -- matches the prior finding exactly, no discrepancy.
   Attempted the next step (trace what writes @0xf0c0 before this call,
   to settle whether the raw 0-64 range really is RPM-shaped at this
   call site or something else) via get_xrefs_to(0xf0c0) and a
   6DF0C0-pattern byte search -- both returned zero results. This RAM
   cell is very likely accessed through the H8 bank-relative addressing
   this project's tooling doesn't yet resolve to a flat xref (same class
   of gap noted elsewhere re: bank/segment addressing not mapping onto
   Ghidra's flat pointer model). Do NOT read the zero-xref result as
   evidence F0C0 is unwritten -- it's a tooling gap, not a finding.
   NEXT STEP for whoever picks this up: find the F0C0 writer some other
   way (grep decompiled output across all functions for \"0xf0c0\" as a
   destination operand, or check isc_stepper_trim_dispatch/
   isc_stepper_correction_dispatch_b, this function's two real callers,
   for anything that primes 0xf0c0 upstream in the same dispatch chain).
   STATUS UNCHANGED: CONFIRMED WRONG-FIXED (address only); name/scaling
   STILL UNRESOLVED. Raw disassembly re-check adds confidence to the

   FOLLOW-UP 2026-08-07 [Claude, second real F0C0 consumer + exhaustive
   writer search]: found a SECOND genuine reader of 0xf0c0, independent of
   isc_f408_correction_calc -- egr_table_row_lookup_f496 (@0x18ce5, sole
   caller egr_position_target_f494_calc @0x18bc2) does `tst.w @0xf0c0:16`
   then `mov:g.w @0xf0c0:16,R2` / `extu.b R2` (masks to the LOW BYTE only)
   and compares that against a small threshold read from @0xf496,
   incrementing a row-select index with hysteresis -- an EGR row-selector,
   not a table axis, and again only ever uses F0C0's low byte, never the
   full 16-bit value. Traced egr_position_target_f494_calc (the only
   caller) in full -- it does NOT write 0xf0c0 either; it passes an
   unrelated cell (DAT_0001f1e8, EGR position feedback) as param_1, and
   0xf0c0 is read independently inside the callee. Still zero writers found.
   Went on to exhaustively rule out a missed WRITE instruction rather than
   just relying on xrefs: compared the exact byte encoding of the known
   0xf0c0 read (`1D F0 C0 82` = mov:g.w @0xf0c0:16,R2) against the known
   0xf496 WRITE two instructions later in the same function (`1D F4 96 92`
   = mov:g.w R2,@0xf496:16) -- confirms reads and writes to absolute
   16-bit RAM addresses on this H8 share the same 0x1D opcode prefix, with
   direction/register encoded only in the trailing mode byte (0x82 vs 0x92
   here). So a flat "F0 C0" byte search (ghidra:search_byte_patterns, 150+
   hits) DOES catch both directions -- the earlier "tooling gap" was about
   Ghidra's reference manager specifically, not the byte pattern itself.
   Manually reviewed every in-code (non-table-data) hit from the full
   150+ list: only two real memory-access instructions to 0xf0c0 exist in
   the entire ROM -- both at 0x18ce4/0x18cea (the egr_table_row_lookup_f496
   read/test above), both reads. The only other in-code-range hit
   (0x14a32) is a false positive -- confirmed via raw bytes (06 EE F0 C0
   30 03) to be `bset.w @(-0x10:8,FP),0x0`, unrelated FP-relative bit-set
   where F0/C0 are coincidental displacement/immediate bytes. NO WRITE to
   0xf0c0 exists anywhere in this ROM via any direct/absolute addressing
   instruction -- exhaustive for that addressing mode, not just an xref gap.
   Also cross-checked a user-supplied Ghidra bookmark/symbol export
   covering the full F0C0 hit list: confirms the same ~150 addresses, all
   either axis-header ptr1=F0C0 fields ("X axis : RAM:0xF0C0 (redirection
   ptr, real)" -- e.g. 0x127a8, 0x131b0, 0x131cc, 0x13202, 0x1320a,
   consistent with the existing pointer map) or table/scaling data bytes --
   no new candidate writer surfaced. One entry (tcu_rx_main_scheduler
   @0x2aa36) carries a stale plate comment from an unrelated EARLIER
   (2026-07-14) session's own dead-end search for F0C0 ("No function or
   label references 0xF0C0 in the live program... Status stays...
   unsupported, inconclusive") -- decompiled the function to confirm it
   does NOT itself touch 0xf0c0; the comment is leftover parked context
   from that prior investigation (RPM-location hunting via EFEA), not a
   live reference. Independent confirmation this exact question was
   already dead-ended once before by a different method/session.
   REVISED WORKING THEORY (still NOT confirmed, per "never guess"): given
   (a) no software write exists anywhere in this ROM via any direct
   addressing mode, and (b) BOTH known real consumers (isc_f408 axis
   breakpoints 0-64, egr_table_row_lookup_f496's masked &0xff compare)
   only ever use F0C0's LOW BYTE and treat it as a small band/row index,
   not a raw magnitude -- F0C0 may be a hardware-latched peripheral
   register (e.g. an input-capture/counter register fed directly by a
   crank-pulse timer peripheral) rather than a software RAM variable at
   all -- which would explain both the total absence of a software writer
   AND the naturally small/wrapping low-byte range seen in every
   consumer. NOT YET CONFIRMED -- would need the H8539F peripheral
   register map cross-referenced against 0xf0c0's offset to verify it
   aligns with a real timer/ADC capture register before treating this as
   more than a plausible theory. NEXT STEP: check H8539F datasheet/
   peripheral map for what's mapped at the 0xf0c0 offset, or look for any
   existing Ghidra memory-map/register definitions covering that range.
   STATUS: CONFIRMED WRONG-FIXED (address only, 2d536->2d146 unchanged);
   name/scaling STILL UNRESOLVED -- narrowed further with a plausible
   hardware-register theory, but not confirmed. Do not rename/rescale
   until the peripheral-map check is done.

   FOLLOW-UP 2026-08-07 [Claude, hardware-register theory CHECKED AND
   RULED OUT]: read datasheets/grep able datasheets/H8_539F_hardware.md
   directly -- "Appendix C: On-Chip Registers" (the chip's own complete
   peripheral register address table, starting line 17728) lists every
   on-chip peripheral register address for the H8/539F. The ENTIRE table
   spans only H'FE80 through H'FEEF (~112 bytes: ports 1-9, timers, SCI1/
   2/3, INTC, BSC, A/D converter, flash controller). Confirmed by direct
   read of the full table -- 0xf0c0 does not appear anywhere in it, and
   H'F0xx is not a peripheral-register range on this chip at all; it's
   plain internal RAM. This RULES OUT the hardware-latched-register theory
   from the prior follow-up entirely -- 0xf0c0 is ordinary software RAM,
   the same as every other Fxxx cell referenced throughout this ROM
   (matches the existing F0C0=RPM/F0C2=Load/etc. pointer map convention
   already used everywhere else in this file, which are all plain RAM
   cells too).
   REVISED STATUS: back to the original open question with the hardware
   theory closed off as a dead end. Given (a) confirmed plain RAM, (b) no
   software write found via any direct-addressing instruction anywhere in
   this ROM (prior follow-up, exhaustive), and (c) two independent real
   consumers both treating only the low byte as a small band/index value
   -- the write must happen via an addressing mode this project's tooling
   doesn't yet resolve (register-indirect, computed/indexed write, or an
   interrupt/DMA-style hardware transfer controller (DTC) path -- this
   chip has an on-chip DTC per the manual's section 7, which can write RAM
   from a peripheral event without ever executing a literal `mov ...,
   @0xf0c0` instruction in the disassembled code stream at all). NEXT
   STEP: check whether any DTC register/vector table entry in this ROM
   targets 0xf0c0 as a destination address (DTC transfers bypass normal
   instruction-level writes entirely, which would fully explain the
   exhaustive-search null result without contradicting it). STATUS:
   CONFIRMED WRONG-FIXED (address only, 2d536->2d146); name/scaling STILL
   UNRESOLVED. Hardware-register theory closed (ruled out); DTC-write
   theory now the leading candidate, untested.
   address fix but does not close the open question.

4. Address 0x2d3ee already flagged by whoever wrote the XML as "Axis1
   (possible: RPM)" with scaling="raw" (unscaled placeholder) -- self-
   admitted unresolved. 5 elements. Sibling axis "Axis2 (possible: Load)"
   at 0x2d3fe, scaling="raw", 7 elements, on the same parent table
   (TABLE_3D_00011DC9_O2Mode7_8Correction). Good first target since it's
   already flagged rather than confidently (and possibly wrongly) named.

   RESOLVED 2026-08-05 [Claude]: header found AT both claimed addresses
   (2d3ee: F0C0 F170 0005, ptr1=F0C0=RPM, count=5 matches; 2d3fe: F0C2
   F0BA 0007, ptr1=F0C2=Load/MAP, count=7 matches). Real axis data is 6
   bytes later in each case (2d3f4 for Axis1, 2d404 for Axis2). Both
   names ("possible: RPM"/"possible: Load") are correct; the "raw"
   scaling placeholder should become a proper RPM/Load scaling formula
   (needs one more caller trace for the exact formula, since Axis1's raw
   values 128,256,512,768,1024 look like timer counts, not direct RPM).
   STATUS: CONFIRMED CORRECT (address/name), OPEN (scaling formula).

   FOLLOW-UP 2026-08-07 [Claude]: re-traced the real caller
   (f0ba_f2d0_o2_mode7_8_correction_calc @0x24319) at both disassembly
   and decompile level to look for a scaling multiply applied to Axis1/
   Axis2 specifically. Confirmed the literal pushes (0xd3ee then 0xd3fe,
   both into axis_lookup_interp @0x14735) match the existing finding
   exactly. However axis_lookup_interp only writes {index,frac} back
   into each axis's own result cell here -- the function's subsequent
   multiply/divide chain (muldiv_u16_scaled, muldiv_u16_shr8_sat, etc.)
   operates on the O2-correction table's VALUE output, not on the axes'
   raw breakpoints themselves. No scaling constant for Axis1/Axis2 is
   derivable from this call site -- the RPM/Load scaling (if any) is
   applied either inside axis_lookup_interp itself (shared across all
   axes, not caller-specific) or purely at the EcuFlash/XML display
   layer, not in ROM code this caller touches. This caller cannot answer
   the open question by itself. STATUS UNCHANGED: OPEN (scaling
   formula) -- narrowed rather than resolved: ruled out "this specific
   caller applies a distinguishing scale factor" as a way to confirm it.
   NEXT STEP: compare Axis1's raw breakpoints (128,256,512,768,1024)
   directly against a confirmed-RPM axis with a KNOWN scaling formula
   already in RVR_base.xml (e.g. 2d2e6, also 128-step raw values) --
   if the existing RPM scaling formula for 2d2e6 produces sane RPM
   numbers when applied to 2d3ee's raw values too, that's real evidence
   for reusing the same formula, since axis_lookup_interp's scaling
   behavior should be uniform across all axes it processes (per the
   struct definition section's confirmed field layout, there's no
   per-axis scaling field in axis_lookup_record itself -- scaling is an
   XML/display-layer convention keyed by name, not stored in the ROM
   header).

   RESOLVED 2026-08-07 [Claude]: ran the comparison. RVR_base.xml's "RPM"
   scaling formula is toexpr="x*1000/256". Applied to Axis1's raw values
   (128,256,512,768,1024): 500/1000/2000/3000/4000 RPM -- clean idle-to-
   low-RPM curve, same formula family as the confirmed-good 2d2e6 RPM
   axis (128-step raw values there too). Separately read Axis2's raw
   data directly (0x2d3fe header confirms F0C2/Load-MAP, count=7; data at
   0x2d404 = 0,33,65,97,129,161,193). RVR_base.xml's "Load" scaling is
   toexpr="x*10/32"; applied: 0/10.3/20.3/30.3/40.3/50.3/60.3% -- clean
   ~10%-step Load curve. Both formulas produce sane physical ranges with
   no red flags (no wraparound, no absurd magnitude, no non-monotonic
   jumps). XML updated: Axis1 scaling="raw"->"RPM", Axis2 scaling="raw"
   ->"Load", both with inline dated comments, in new/RVR_base.xml only
   (old/ untouched per this file's standing scope rule).
   STATUS: CONFIRMED CORRECT AND FIXED (scaling formula, both axes).
   SEED #4 fully closed.

5. Address 0x2d59c "Boost Error" (Y Axis, on Turbo Boost Error Correction)
   -- element count disagreement flagged by a user-supplied Ghidra
   bookmark export (import-script header-behind check): ROM header says
   17, XML claimed 16.

   RESOLVED 2026-08-06 [Claude]: read raw bytes directly (not assumed
   from the log). Header at 0x2d596 = F0C0 F0BA 0011 (count=17). Data at
   0x2d59c is 17 clean 16-bit BE values (0,16,32,...,240,255) followed
   immediately by a clean next-table header (F0C0 F13E 0009) with no
   overlap/garbage -- the 17th value (255) is genuine axis data the XML
   was truncating. Separately confirmed via disassembly: real caller
   wgdc_correction_integrator_update (0x19059) pushes literal record
   pointer 0xD596 into table_lookup_interp(0x12a80), matching this same
   header address, so the caller trace and the byte trace agree.
   STATUS: CONFIRMED WRONG-FIXED (elements 16->17 in new/RVR_base.xml).
   NOTE: same bug also present in old/RVR_base.xml (line 546) -- left
   unfixed intentionally, old/ is a frozen backup, out of scope for this
   review (confirmed 2026-08-06).

6. Address 0x2d332 "RPM" (Y Axis, on Warmup Retard) -- one stale entry
   (line 344) still read elements="17" while the other 5 uses of this
   same axis address in the file already correctly said 18.

   RESOLVED 2026-08-06 [Claude]: header at 0x2d32c = F0C0 F170 0012
   (count=18). Data at 0x2d332 is 18 clean 16-bit BE values (0, 128,
   192, 256, 320, 384, 512, 640, 768, 896, 1024, 1152, 1280, 1408, 1536,
   1664, 1792, 2816), immediately followed by a clean next-table header
   (F0C2 F0BA 0010). Confirms the 5 already-correct entries and fixes
   the 1 outlier. STATUS: CONFIRMED WRONG-FIXED (elements 17->18, single
   entry on the Warmup Retard table in new/RVR_base.xml). Same
   old/RVR_base.xml caveat as #5 above -- intentionally left unfixed,
   old/ is out of scope (frozen backup).

7. AXIS-DATA-OVERLAP cluster (5 flagged pairs from user-supplied Ghidra
   bookmark export, 2026-08-06). Per-address disassembly + xref trace
   (get_xrefs_to on each candidate address, then decompile/disassemble
   the real caller to find the literal record pointer pushed before
   table_lookup_interp/axis_lookup_interp) -- NOT assumed from the log:

   a. 0x2d3c8 (TPS, real) / 0x2d3ca (claimed "Degrees"/Air Temp axis on
      "Air Temperature Compensation", 0x11d7a) -- NOT a genuine overlap,
      but the initial 2026-08-06 analysis of this one was WRONG and got
      corrected the same day: first pass only read the SECOND of two
      pushes before the table_lookup_interp call and concluded the table
      was "self-contained" with an inline axis -- missed the axis-record
      push entirely. Full disassembly of iat_axis_update (0x213c6, sole
      caller via get_xrefs_to on 0x11d7a) actually shows TWO pushes:
      #0xD4A6 (bank=2) into axis_lookup_interp via pjsr @0x14735 BEFORE
      the #0x1D7A (bank=1) table push into table_lookup_interp -- 0xD4A6
      is a real, standard-format separate axis record; 0x1D7A is just
      the value-table address. Header at 0x2d4a6 = F0D0 F118 0007
      (ptr1=F0D0=IAT_raw per review5.md's own pointer map -- clean match
      for "Air Temp"), count=7. Breakpoints start 2d4a6+6=2d4ac
      (8,33,49,63,78,96,125 -- clean monotonic curve). This matches what
      a user-supplied Ghidra bookmark export already had right ("Air
      Temp (axis of 'Air Temperature Compensation', 7 elements)" @
      0x2d4ac) -- should have cross-checked that export before concluding
      "self-contained". STATUS: CONFIRMED WRONG-FIXED (address
      2d3ca->2d4ac, name Degrees->Air Temp, elements 8->7, in
      new/RVR_base.xml). 0x2d3ca itself remains a genuine phantom (2
      bytes into the unrelated TPS array at 2d3c8, zero xrefs, no real
      caller) -- same noise category as 7c below, just not the cause of
      this table's issue.

   b. 0x2d332 / 0x2d334 ("Boost Desired Engine Load # 1") -- RESOLVED,
      same root cause as an already-fixed sibling issue. 0x2d334 has
      ZERO xrefs (get_xrefs_to confirmed) -- same "unverified
      placeholder" failure mode already documented and fixed for the
      Max Wastegate Duty #1-4 tables earlier the same session (see
      inline XML comments at those entries). Traced the real caller
      (ign_advance_load_correction_f450, sole xref to 0x12af8): pushes
      literal record pointer 0xD5D6 into table_lookup_interp(0x12af8).
      Header at 0x2d5d6 = F0C0 F190 0009 (ptr1=F0C0=RPM, count=9),
      breakpoints start 2d5d6+6=2d5dc (0,64,128,192,256,320,384,448,512
      -- clean RPM curve). Name "RPM" was already correct by
      coincidence; address and (as a side effect) the underlying record
      were wrong. STATUS: CONFIRMED WRONG-FIXED (address 2d334->2d5dc,
      "Boost Desired Engine Load # 1" table only, in new/RVR_base.xml).

   c. 0x2d59c / 0x2d5a2 (both claimed "Boost Error") -- NOT a genuine
      overlap. 0x2d5a2 has zero xrefs and falls inside the already-fixed
      17-element 0x2d59c array (element index 3) -- a phantom duplicate
      bookmark from the same scanning heuristic that produced 2d3ca
      above. No XML entry uses 0x2d5a2, so no fix was needed. Disregard
      as scanner noise.

   d. 0x2d0f6 / 0x2d100 ("TABLE_2D_00011894_Fueling sibling" vs "Boost
      Limit") -- UNRESOLVED, genuinely inconclusive, now with a new
      wrinkle found 2026-08-06. 0x2d0f6 does not appear anywhere in
      RVR_base.xml. get_xrefs_to(0x11cee) [Boost Limit's own table
      address] and get_xrefs_to(0x2d100) both return zero results, and a
      literal-constant byte search (per review5.md Method step 3) for
      the standard "mov:g.w #imm,@-SP" push pattern (bytes BF 07 <low
      word>) found ZERO occurrences of either 0x11cee's low word (0cee)
      or 0x2d100's low word (d100) anywhere in the ROM -- ruling out the
      normal axis_lookup_interp/table_lookup_interp calling convention
      entirely, not just Ghidra's xref database missing it. A broader
      raw 2-byte search for 0cee turned up one genuine but DIFFERENT use
      at 0x21fd4, inside f216_bit6_update_from_hysteresis_gate: `cmp:g.w
      @0xcee:16,R0` -- 0x11cee compared directly as a plain scalar
      threshold against a live value at 0xf1e6, used only to set/clear a
      status bit. This is NOT a 2D-table-with-axis read at all, and not
      obviously related to "Boost Limit vs RPM". Also checked
      table_lookup_indexed/table_read_indexed callers (the two other
      lookup primitives from review4.md) -- none of their 24 combined
      callers have suggestive names or were checked byte-for-byte yet
      (stopped here rather than brute-force all 24 without better reason
      to suspect one). Two live possibilities, genuinely undecided: (1)
      "Boost Limit" is dead/unused table data with no real 2D-lookup
      caller in this ROM, and the f216 scalar-compare use of 0x11cee is
      an unrelated coincidence of address reuse; or (2) "Boost Limit" is
      NOT actually a 2D table at all in this ROM revision -- it may be a
      simple scalar threshold (like the f216 use) and the XML's
      "type=2D + RPM Y-Axis" structure is wrong at a more basic level
      than address, similar in spirit to the Air Temp misdiagnosis in
      7a above (though this time genuinely unresolved rather than a
      mistake in the trace). Per this file's "never guess" discipline,
      left OPEN, NOT FIXED. STATUS: CONFIRMED WRONG-UNRESOLVED (no
      table_lookup_interp-style caller found for either candidate
      address by direct xref OR literal-constant search; one unrelated
      scalar use of the table's own address found, raising the
      possibility this isn't a 2D table at all).

   e. 0x2d35c / 0x2d35e ("TABLE_3D_00012B94_ISC" vs "Load" axis on "Post
      Start Enrich Trim vs Load") -- RESOLVED, wrong on two counts.
      Traced BOTH real callers of the parent table 0x11d86
      (throttle_target_ramp_update @0x21771 and
      f2ca_table_override_from_f114_gate @0x2377a, both found via
      get_xrefs_to on 0x11d86) -- both independently push literal record
      pointer 0xD192 before table_lookup_interp(0x11d86), agreeing with
      each other. Header at 0x2d192 = F0C0 F190 000E (ptr1=F0C0=RPM, NOT
      Load; count=14), breakpoints start 2d192+6=2d198
      (0,32,64,96,128,160,192... clean RPM curve, not Load-shaped).
      STATUS: CONFIRMED WRONG-FIXED (both address 2d35e->2d198 AND name
      Load->RPM, in new/RVR_base.xml).

   f. 0x2d080 / 0x2d086 ("Boost Limit" Y-Axis per a since-superseded
      Ghidra bookmark export, VS "Engine Speed" 10-elem X-Axis already
      in RVR_base.xml on "Ignition Trim vs Air Temp Active Above Load")
      -- genuinely traced 2026-08-06, partially resolved, one open
      question remains. A bookmark export (from an apparently stale/
      pre-fix scraper run -- the on-disk XML never actually held this
      address for Boost Limit, which still correctly shows 0x2d100 per
      SEED #7d) claimed axis 0x2d080 belonged to "Boost Limit". Traced
      directly instead: search_byte_patterns for "bf07d080" (the
      standard literal-push convention) found exactly one real caller,
      throttle_body_update (@0x217e3, called from
      main_loop_engine_control_update) -- NOT anything boost-related.
      throttle_body_update calls throttle_param_table_select() (which
      only flips an unrelated scalar based on a status flag) then
      axis_lookup_interp() on this record, and critically NEVER calls
      table_lookup_interp itself -- it only writes the axis's
      interpolated {index,frac} result back into the record's own
      value_ptr cell (0x2d080) for some other, not-yet-identified
      function to read later. This RULES OUT the bookmark's "Boost
      Limit" attribution (consistent with #7d's separate finding that
      Boost Limit/0x11cee has zero real callers by any method) AND
      rules out "Ignition Trim vs Air Temp Active Above Load" (0x127ee)
      as the consumer -- disassembly of that table's own header shows
      it reads F0C4 directly as a raw register (mode=02, index_src=F0C4,
      the table_2d_record "reads raw sensor register directly" case
      flagged as an open question under table_2d_record above), bypassing
      this axis's result cell entirely, so it cannot be what "Engine
      Speed" at 2d086 is really for either.
      Header at 0x2d080 itself: F0C4 F170 000A (ptr1=F0C4=CoolantTempA
      per review5.md's own pointer map, NOT F0C0/RPM as the "Engine
      Speed"/RPM XML entry implies; count=10, matches). Data at 0x2d086:
      128,256,384,512,640,768,896,1024,1152,1280 -- the same 128-step
      monotonic shape seen on confirmed RPM axes elsewhere (e.g. 2d2e6),
      which is likely why this got named "Engine Speed"/RPM by whoever
      wrote the original XML -- but the header pointer disagrees, and
      the real caller is throttle-body logic, not an RPM-keyed table.
      Best-supported working theory (NOT confirmed): this may be a
      temperature-compensated throttle-body position axis rather than
      an RPM axis, given the caller's name and the CoolantTempA pointer
      -- but no real consumer of the written-back result has been found
      yet, so the exact name/table this axis actually feeds remains
      open. Per this file's "never guess" discipline, "Engine Speed"/RPM
      at 2d086 is NOT renamed yet.
      STATUS: CONFIRMED WRONG-UNRESOLVED ("Boost Limit" attribution from
      the stale bookmark export is definitively ruled out; "Engine
      Speed"/RPM naming on the existing 2d086 XML entry is suspect given
      the F0C4/CoolantTempA header pointer, but the real consumer/table
      has not been traced yet, so no XML rename made). NEXT STEP: trace
      forward from throttle_body_update's call site inside
      main_loop_engine_control_update to find what runs afterward and
      reads @0x2d080, or search for a table_2d_record/table_3axis_record
      header whose index_src/x_ptr/y_ptr field equals 0xD080 via a wider
      net than the exact 4-byte anchor already tried (0200d080/03d080/
      0300d080 all came back empty -- consumer may not use either
      standard record shape, or may reach it through
      table_lookup_indexed/table_read_indexed's DAT_0001f1fc-keyed
      dispatch instead, per the note under STRUCT DEFINITIONS above).

      FOLLOW-UP 2026-08-07 [Claude]: checked the indexed-dispatch
      possibility directly. get_function_callers on table_lookup_indexed
      and table_read_indexed returns the same 14 functions already
      enumerated for SEED #8 (closedloop_fuel_ready_check,
      f33e_bit6_update_from_table_da2c_threshold,
      fueling_base_injection_calc, idle_target_rpm_compute_f400,
      isc_f34e_f34a_correction_calc, isc_f35e_correction_calc,
      isc_f408_f406_stepper_target_calc, isc_f41c_table_lookup,
      knock_octane_dual_axis_table_blend_f2a8,
      f286_knock_octane_secondary_trim_compute_via_table,
      isc_f41a_correction_calc, isc_f420_correction_calc,
      octane_trim_f262_f264_table_select,
      table_select_scale_fanout_f1be_f1c2). Spot-decompiled
      closedloop_fuel_ready_check @0x226fd to re-verify the selector
      range directly rather than trusting the SEED #8 note secondhand:
      confirmed selector constant 0xda4c, inside the same 0xd8ec-0xdbcc
      RAM-bank cluster already documented under SEED #8. This is
      nowhere near 0x2d080/0xD080, and by the same address-space
      argument SEED #8 already made (ROM-page 0x12xxx/0x2xxxx vs
      RAM-bank 0xdxxx), a partial or near match isn't structurally
      possible here either. Rules out the indexed-dispatch path for
      2d086/0x2d080 the same way SEED #8 ruled it out for the Decel
      Fuel Cut cluster.

      Also attempted the get_xrefs_to and raw byte-pattern approaches
      fresh (not reusing SEED #7f's prior attempt): get_xrefs_to(0x2d080)
      shows only the known write from throttle_body_update, no read side
      indexed. A raw "D080" byte search returns 22 hits but all are false
      positives (byte pairs matching unrelated immediate operands like
      0x6672 or 0x80 next to 0xd0-prefixed opcodes at instruction
      boundaries, not genuine @0xd080:16 addressing-mode encodings) --
      confirmed by disassembling several candidates (e.g. 0x16613,
      0x26100) directly and finding no reference to 0x2d080 in either.

      All three access methods now checked and exhausted for this
      specific address (direct literal push, table_lookup_indexed/
      table_read_indexed indirection, raw byte pattern), same as SEED
      #8's cluster. STATUS UNCHANGED: CONFIRMED WRONG-UNRESOLVED
      ("Boost Limit" and "Ignition Trim vs Air Temp..." both still ruled
      out as consumers; "Engine Speed"/RPM name still not renamed,
      still no real consumer found by any known method). Flagging
      alongside SEED #8 for the same "second pair of eyes / broader
      search" treatment -- the xref-based methodology this project has
      used throughout is now exhausted for both open items, and a
      genuinely different approach (manual disassembly walk of the
      ~5 functions calling immediately after throttle_body_update in
      main_loop_engine_control_update, address-by-address, not relying
      on Ghidra's xref database or literal-push search at all) is the
      most promising untried option for either.

      ATTEMPTED 2026-08-07 [Claude]: manually disassembled the 5
      top-level functions main_loop_engine_control_update calls
      immediately after throttle_body_update (0x21942, 0x219e2,
      0x21d5e, 0x21dc8, 0x21df5) directly at the shallow level -- none
      of the 5 reference 0xd080/0x2d080 themselves. This does NOT rule
      out the deeper call tree (0x219e2 alone fans out into at least 7
      further pjsr sub-calls, e.g. 0x21a0e/0x21aa5/0x21af3/0x21b2d/
      0x21b8f/0x21bf3/0x21c2f, none of which were disassembled here) --
      a full manual walk would need to recurse into every sub-call of
      every one of these 5, which is a large search space disassembled
      by hand and wasn't completed this session. Treat this as a
      partial, INCONCLUSIVE result at the shallow level only, not a
      4th exhausted method alongside the other three -- do not read
      "no hit in the top-level 5" as ruling out the immediate-successor
      hypothesis; it only rules out the shallowest layer of it.
      STATUS UNCHANGED: CONFIRMED WRONG-UNRESOLVED. NEXT STEP if
      continuing this specific thread: recurse one level into 0x219e2's
      7 sub-calls first (named fueling_base_injection_calc-adjacent
      dispatcher, most likely candidate given throttle position feeds
      fueling logic downstream) before trying the other 4 top-level
      functions' sub-calls.

      ATTEMPTED 2026-08-07 [Claude, one level deeper]: disassembled
      0x21a0e (first of 0x219e2's 7 sub-calls) directly -- turns out to
      BE fueling_base_injection_calc, already known from SEED #8's
      table_lookup_indexed caller list. It pushes 0xd0f0 (its own axis)
      into axis_lookup_interp, not 0xd080 -- confirms it's unrelated,
      not a partial match. Stopping the manual recursive walk here: the
      remaining search space (6 more direct sub-calls of 0x219e2, plus
      all sub-calls of the other 4 top-level functions, plus their own
      sub-calls recursively) is large enough that continuing to expand
      it by hand one function at a time has poor odds-per-effort
      compared to other open items in this file. FLAGGING FOR BROADER
      TOOLING rather than continuing by hand: this is exactly the kind
      of exhaustive-but-shallow search h8539_find_table_axis.py (added
      this project, see SEED #8 TOOLING NOTE) was built to automate for
      the literal-push case -- worth checking whether it (or a variant)
      can be pointed at 0x2d080 and run against EVERY function in the
      ROM rather than a hand-picked candidate list, instead of manually
      recursing further. STATUS UNCHANGED: CONFIRMED WRONG-UNRESOLVED.
      Same recommendation as SEED #8: second pair of eyes / broader
      automated search, not more manual xref-chasing.

      RESOLVED (exhaustively) 2026-08-07 [Claude, new tooling]: built
      h8539_find_axis_consumer.py, the reverse-direction companion to
      h8539_find_table_axis.py -- given an AXIS address, finds every
      real axis_lookup_interp call site pushing it, then searches BOTH
      the same function (forward instruction walk) AND every other
      function in the entire ROM's instruction stream for any reference
      to that axis's RAM cell. This is a genuine whole-ROM exhaustive
      scan, not a hand-picked candidate list like the manual attempts
      above. Hit one real bug during development worth recording: the
      literal actually pushed before axis_lookup_interp is the BARE
      16-bit page offset only (0xd080), not the full bank-prefixed
      address (0x2d080) -- confirmed directly from disassembly
      ('mov:g.w #0xd080:16,@-SP' with bank pushed separately as a
      distinct byte immediate). First script run silently produced a
      false NO-CALLER because it compared against the wrong address
      form; fixed via normalize_axis_offset() before trusting any
      result. After the fix, ran against 0x2d080: found exactly ONE
      reference anywhere in the whole ROM's instruction stream --
      the known write itself (throttle_body_update @0x217fb). No
      same-function consumer, no cross-function consumer, nothing.
      This confirms -- exhaustively now, not just by the 3 methods
      already tried by hand -- that the {index,frac} result
      axis_lookup_interp writes into 0x2d080 is never read anywhere
      in this ROM revision via any addressing mode this scan's
      string-match approach can see (which covers every direct/
      immediate-operand reference; it would still miss a genuinely
      computed/indirect access, e.g. an address built at runtime in a
      register rather than encoded as a literal in any instruction --
      but that's a materially different and much rarer pattern than
      anything else documented in this ROM so far).
      STATUS: CONFIRMED WRONG-UNRESOLVED, but now with EXHAUSTIVE
      evidence rather than partial. Working theory (still NOT
      confirmed, per "never guess"): this axis_lookup_interp call may
      be genuinely vestigial/diagnostic -- write-only, result never
      consumed in this ROM revision -- OR "Engine Speed"/RPM at 2d086
      is simply the wrong table-XML entry's address entirely (i.e. the
      real "Engine Speed" this name refers to lives elsewhere, and
      2d086/2d080 is an unrelated, currently-unused axis that
      coincidentally has RPM-shaped raw data because it's a leftover/
      shared axis definition from another calibration variant). XML
      NOT changed -- neither theory is confirmed, and "never guess"
      applies to unnaming as much as renaming. TOOLING NOTE:
      h8539_find_axis_consumer.py added to ghidra scripts/ this
      session, copied to <user_home>/ghidra_scripts/ per the same
      two-copy requirement as h8539_find_table_axis.py. Available for
      the same treatment on any other axis with an open consumer
      question (e.g. could be pointed at 0x2d536/0x2d146 for SEED #3's
      still-open F0C0-writer question, which hit a similar tooling gap
      via get_xrefs_to earlier this session -- worth trying next).

      TRIED 2026-08-07 [Claude]: ran h8539_find_axis_consumer.py against
      0x2d146 too. Result: PAIRED, same-fn table call found at 0x27356
      (table_lookup_interp, matching the known branch into 0x1296c/
      0x295e already seen in isc_f408_correction_calc's disassembly).
      This confirms 2d146's axis result IS consumed downstream -- but
      does NOT answer SEED #3's actual open question, which is the
      UPSTREAM one (what writes @0xf0c0 BEFORE the axis push, to settle
      whether the raw 0-64 range is genuinely RPM-shaped here). This
      script only searches forward/downstream from the axis_lookup_interp
      call; it has no upstream-writer-finding mode. Scope mismatch, not
      a negative result -- SEED #3's specific question remains open and
      needs a different tool (an axis_lookup_interp-style script but
      walking BACKWARD from the call site for whatever RAM cell writes
      into F0C0, or a whole-ROM scan for instructions with F0C0 as a
      destination operand specifically, which is a mirror-image script
      not yet written). Correcting my own suggestion above: this
      specific tool wasn't the right fit for SEED #3 after all.

8. "Decel Fuel Cut Delay" cluster (4 tables: Neutral A/C Off @0x128fa, In
   Gear A/C Off, Neutral A/C On, In Gear A/C On -- addresses 12902/1290e/
   1291a in the pre-fix XML). User-reported (2026-08-06): opened all 4 in
   EcuFlash, noticed a +4 byte offset artifact -- first row of 3 of the 4
   tables showed a stray "2 0 240 192" (the table_2d_record header's own
   mode/mode_hi/index_src bytes) plus a spurious 255 tail value.

   RESOLVED (address bug) 2026-08-06 [Claude]: direct byte read confirmed
   a table_2d_record header (02 00 F0 C0) at 0x128f6/12902/1290e/1291a,
   each followed by 7 bytes of real data + 1 sentinel (0xFF), 12 bytes
   apart. 3 of the 4 XML addresses pointed AT the header instead of
   header+4 (data start); "Neutral A/C Off" was already correct.
   Addresses fixed in both new/RVR_base.xml and the MT-specific
   21000011_...Mt_4g63t_.xml duplicate (which had ALL 4 wrong, including
   a base-address quirk on the first entry). Data now reads clean 7-byte
   rows matching the shared "Engine Load" X-Axis (0x2d3fe, elements=7,
   already CONFIRMED CORRECT, see FULL NAMED-AXIS CHECKLIST above) with
   no header/sentinel bytes leaking into the visible cells.

   STATUS ON NAMES: CONFIRMED WRONG-UNRESOLVED (names never traced,
   caller does not exist). The 4 names ("Neutral/In Gear A/C Off/On")
   were inherited unchanged from old/RVR_base.xml (the original community
   XML) and have NEVER been confirmed by any caller trace in this
   project -- they were only carried forward address-by-address. Once the
   +4 byte bug was fixed, a caller-existence check was run two
   independent ways:
     1. Literal-byte search (per Method step 3) for the low words of all
        4 data addresses (28fa/2906/2912/291e) AND all 4 header addresses
        (28f6/2902/290e/291a) anywhere in the ROM: ZERO hits for all 8.
     2. h8539_find_table_axis.py (new script, added this session -- see
        note below) run against all 4 data addresses: ZERO real callers
        found via table_lookup_interp/table_3axis_interp_triple's own
        Ghidra reference manager (NO-CALLER for all 4). The same run
        correctly reproduced PAIRED with the right axis for 3 known-good
        sanity-check tables (0x11800, 0x11840, 0x11d86) and NO-CALLER for
        the already-documented SEED #7d dead end (0x11cee/"Boost Limit"),
        confirming the script itself is trustworthy, not just quiet.
   Two independent methods agree: no code anywhere in this ROM revision
   references these 4 table addresses via the standard literal-push
   convention. This does NOT necessarily mean the table_2d_record-shaped
   data at these addresses is fake -- the header shape itself is genuine
   (02 00 F0 C0 matches 69 other confirmed instances elsewhere in ROM,
   see STRUCT DEFINITIONS below) -- but it means the 4 EcuFlash table
   NAMES currently attached to it are unverified inherited guesses, not
   traced facts, and the possibility remains open that this data is
   reached via table_lookup_indexed/table_read_indexed's DAT_0001f1fc-
   keyed indirect dispatch (untraced) rather than a direct literal push,
   or that it's genuinely dead/unused ROM data. Per this file's "never
   guess" discipline, NAMES LEFT AS-IS pending further trace -- do not
   rename or delete these 4 XML entries without first tracing the
   indirect-dispatch path. NEXT STEP: trace table_lookup_indexed/
   table_read_indexed's ~24 combined callers (per SEED #7d's own
   unfinished thread) specifically checking whether any reaches 0x128fa/
   12906/12912/1291e via the DAT_0001f1fc-keyed indirection instead of a
   literal push.

   INDIRECT-DISPATCH TRACE COMPLETE 2026-08-06 [Claude]: enumerated all 14
   real callers of table_lookup_indexed (9, via get_function_callers) and
   table_read_indexed (5) in this ROM, decompiled every one, and recorded
   every table-selector constant each pushes. All 14 callers are ISC/
   idle/octane/knock-trim related (closedloop_fuel_ready_check,
   idle_target_rpm_compute_f400, isc_f34e_f34a/f35e/f408_f406/f41a/f41c/
   f420_correction_calc, knock_octane_dual_axis_table_blend_f2a8,
   f286_knock_octane_secondary_trim_compute_via_table,
   octane_trim_f262_f264_table_select,
   table_select_scale_fanout_f1be_f1c2, fueling_base_injection_calc,
   f33e_bit6_update_from_table_da2c_threshold) -- several use the same
   "table pointer picked by g_status_flags_f0f8 bit5 (A/C flag)" pattern
   this cluster's own naming (Neutral/In Gear, A/C On/Off) would predict,
   which is why this trace felt worth doing in full rather than stopping
   early. Full set of table-selector constants found across all 14
   callers: 0xd8ec, 0xd90c, 0xd92c, 0xd94c, 0xd96c, 0xd98c, 0xd9ac, 0xd9cc,
   0xd9ec, 0xda0c, 0xda2c, 0xda4c, 0xda6c, 0xda8c, 0xdaac, 0xdacc, 0xdaec,
   0xdb0c, 0xdb6c, 0xdb8c, 0xdbac, 0xdbcc -- a tight, contiguous RAM-bank
   cluster (0xd8ec-0xdbcc). None is anywhere near 0x128fa/12906/12912/
   1291e (which are ROM-page addresses in the 0x12xxx range, not RAM-bank
   0xdxxx addresses at all -- a different address space entirely, so no
   partial/near match is even structurally possible here). This rules out
   the indirect-dispatch possibility completely: NO caller of
   table_lookup_indexed/table_read_indexed reaches this cluster, closing
   off the third open possibility from the note above.
   STATUS: CONFIRMED WRONG-UNRESOLVED, but now exhaustively so -- all
   three access methods this ROM is known to use for 2D-lookup tables
   (direct literal push to table_lookup_interp/table_3axis_interp_triple,
   AND the indexed-indirection wrappers table_lookup_indexed/
   table_read_indexed) have been checked and return zero real callers for
   all 4 addresses. Remaining open possibilities, in order of likelihood:
   (1) genuinely dead/unused ROM data with a coincidentally valid-looking
   table_2d_record header shape (least surprising, given 69 other
   genuine instances of this exact header pattern exist elsewhere in ROM
   -- a false positive on shape alone was always possible); (2) reached
   via a calling convention not yet catalogued in this project (e.g. a
   computed/indirect jsr rather than any literal push, which neither
   xref search nor h8539_find_table_axis.py's method would catch -- this
   would need a broader disassembly-level search for accesses via a
   pointer table or state-machine dispatch); (3) reached only by MUT
   (Mitsubishi diagnostic protocol) or a non-lookup access path entirely
   outside this project's traced primitive set. NAMES STILL LEFT AS-IS --
   no evidence found to support OR contradict them; this trace only
   proved the data is unreached by every KNOWN access method, which is
   different from proving the names wrong. Flagging for a second pair of
   eyes / broader search rather than continuing to narrow via the same
   xref-based methodology, which has now been exhausted for this cluster.

   TOOLING NOTE: h8539_find_table_axis.py added to ghidra scripts/ this
   session -- automates exactly the "disassemble real caller, find
   preceding axis_lookup_interp push, read its literal" trace this file
   has been doing by hand since SEED #1. Edit TABLE_ADDRESSES at the top
   and run; reports PAIRED/NO-AXIS-CALL/NO-CALLER per table. Does not
   modify XML/Ghidra state, read-only. NOTE: Ghidra's script directory
   (<user_home>/ghidra_scripts/) is a SEPARATE COPY from this repo's
   ghidra scripts/ folder -- edits here must be copied over (e.g. via
   Pulsar's run-command / PowerShell Copy-Item) before re-running in
   Ghidra, editing the repo copy alone has no effect on script execution.

REAL CALLER FOUND 2026-08-07 [Claude, manual disassembly trace]: isc_f438_correction_calc (0x26b67) is the real caller of the 4-table Decel Fuel Cut Delay cluster. The 4 literals (0x28f6/0x2902/0x290e/0x291a) are pushed via an FP-relative spill-then-reload (mov #imm,@(-0x2:8,FP) → mov @(-0x2:8,FP),R0 → mov R0,@-SP) rather than a direct @-SP immediate push — a fourth calling-convention shape this project hadn't catalogued, invisible to both the literal-byte search and FixBankOffsetReferences_v2.java's adjacency scan. Axis is 0x2d3da (RPM-shaped, corroborated independently by review2.md line 2145). Naming reopened, not resolved: the branch is selected by g_status_flags_f0f8 bits 4/5, but tracing those bits back through compose_status_word_f0f8 → g_status_flags_f20e bits 0/4 shows the real gate is an open-loop/operating-mode flag pair (bit4 set inside a function already named ecu_openloop_state_reset), not an A/C-switch or gear-position signal. The inherited "Neutral/In Gear A/C Off/On" names are now suspect but NOT renamed — bit0 of f20e still needs its own trace. STATUS: caller CONFIRMED (was NO-CALLER). Naming: OPEN.

9. DEGENERATE/EMPTY 3D-table cluster: 0x131A0_ISC, 0x131A8_ISC, 0x131B0_ISC
   (user screenshot, EcuFlash rendering wildly jumpy 8x10/7x10/10x10 grids
   instead of a smooth surface). RESOLVED 2026-08-06 [Claude, live Ghidra
   read_memory + xref trace]:

   Raw bytes at 0x131a0 (48 bytes dumped directly):
     131a0: 03 00 F0 C0 F0 C2 08 FF   <- 000131A0_ISC header
     131a8: 03 00 F0 C0 F0 C2 07 FF   <- 000131A8_ISC header
     131b0: 03 00 F0 C0 F0 C2 0A FF   <- 000131B0_ISC header
     131b8: 02 00 F0 C8               <- TABLE_2D_000131B8
     131bc: 02 00 F0 D0               <- TABLE_2D_000131C0
     131c0: 02 00 F0 C0               <- (part of same run)
     131c4: 03 00 F0 C0 F0 C2 0B FF   <- TABLE_3D_000131C4_TCU (already
                                         known-degenerate per struct notes)
     131cc: 03 00 F0 C0 ...           <- TABLE_3D_000131CC (real, populated)

   All three flagged tables are valid table_3axis_record headers (mode=03,
   x_ptr=F0C0/RPM, y_ptr=F0C2/Load-MAP, both genuine RAM redirection
   pointers -- confirmed real and in active use: get_xrefs_to on 0x131a0/
   0x131a8 shows real DATA references from isc_openloop_stepper_scale_calc,
   isc_openloop_target_calc, and isc_integrator_update) -- but in every
   case the very next byte after the header's nrows field is the 0xFF
   sentinel, i.e. ZERO actual data rows exist behind any of the three
   headers, exactly the same pattern already documented for
   TABLE_3D_000131C4_TCU under table_3axis_record above.

   EcuFlash was never wrong about the axis identity or the redirection
   pointers -- it was told nrows=8/7/10 by the (real) header and dutifully
   read that many rows starting immediately after, walking straight
   through the sentinel into the NEXT record's header bytes (02 00 F0 C8,
   02 00 F0 D0, 02 00 F0 C0, then another 03 00 F0 C0 F0 C2 0B FF...) and
   rendering those as bogus "cell values" -- which is exactly the
   240/192/194/255/2/0 noise pattern the user was seeing on screen.

   FIX APPLIED 2026-08-06: h8539_import_tables_xml1.py Pass 3 (3D value
   tables, ~line 1801) patched to detect this case directly -- if
   data_len <= 1 (sentinel immediately follows the header) or
   data_len < table_height, the record is now bookmarked
   "H8539F-TABLE-DEGENERATE" instead of "H8539F-SCRAPED-TABLE", with a
   plate comment explaining the header/axis pointers are real but the
   value grid is absent, and it is excluded from the populated-table
   count/labeling path entirely (no TABLE_3D_* symbol claiming a false
   row count is created). RVR_base.xml itself was not touched by this fix
   -- these three ISC tables were never present in the maintained XML to
   begin with (screenshot showed live EcuFlash-vs-ROM scraper output, not
   an XML entry), so there was nothing to correct there.
   STATUS: CONFIRMED WRONG-FIXED (scraper detection only; no XML entries
   existed to correct). Same treatment should eventually be extended to
   Pass 4 (2D value tables) if an equivalent degenerate-header pattern
   ever turns up there -- table_2d_record's 4-byte header has no row
   count field of its own to sanity-check against, so that would need a
   different detection approach (e.g. minimum data_len only).

10. FALSE-RPM-AXIS CLUSTER (2026-08-10, Claude): 8 Ghidra-scraper-only
    table_2d_record bookmarks -- TABLE_2D_00011EE2_CompositeCalc,
    TABLE_2D_00011F4A_CompositeCorr, TABLE_2D_000127D8_ISC,
    TABLE_2D_0001297A_Trim, TABLE_2D_0001315C, TABLE_2D_00013168,
    TABLE_2D_0001317C, TABLE_2D_000131EC -- each carry an
    H8539F-AXIS-DATA-OFFSET note claiming their axis is "6 bytes behind"
    at 0x2d32c (the confirmed-good 18-element RPM axis @0x2d332, same one
    SEED #6 fixed). NONE of these 8 are in new/RVR_base.xml as curated
    entries yet -- scraper-only, never promoted. Investigated because a
    user screenshot showed EcuFlash rendering several of these as jumpy/
    garbage grids under the inherited 18-elem-RPM assumption.

    FINDING: the "6 bytes behind = shared RPM axis" claim is a scraper
    false trail for this whole cluster, same failure mode as SEED #7a's
    Air Temp misdiagnosis -- it's just grabbing the nearest preceding
    axis-shaped header, not what's actually consumed. Each of these 8
    table_2d_records has its OWN 4-byte header (mode=02, mode_hi=00,
    index_src) read directly, and index_src is NOT F0C0/RPM for 6 of the
    8. None of them go through axis_lookup_interp or touch 0x2d332 at
    all -- confirmed both by direct header byte reads and by the fact
    that real callers (where found) call table_lookup_interp with no
    preceding axis_lookup_interp call.

    Also discovered while tracing these: table_lookup_interp itself
    (0x14656) has NO count/bounds field or check anywhere in its own
    logic -- it just uses the dereferenced index_src cell's low byte as
    a raw offset into the table's inline data, unlike axis_lookup_interp
    which does clamp against a real stored count. So "element count" for
    a table_2d_record isn't a fixed ROM-stored value the way it is for
    axis records; the real constraint is whatever range the driving
    RAM cell can take, which is a separate open question per table (see
    Group A below).

    Split into two groups based on how far the trace got:

    GROUP A -- TRACED (real caller found, real index_src register
    confirmed by direct header read; STATUS: CONFIRMED WRONG-UNRESOLVED,
    narrowed -- identity known, exact element count/boundary still open):
      - 0x11ee2 CompositeCalc: index_src=F0C4 (CoolantTempA, ADC).
        Real caller: f310_f316_composite_calc (@0x2495c), literal push
        confirmed via disassembly (BF 07 1E E2 / BF 06 01 / pjsr
        @0x14656). Real data span independently confirmed elsewhere in
        this file's own XML comments as 11ee2-11eef (10 data bytes) --
        the following bytes are a SEPARATE real table @0x11ef0 (a third,
        not-yet-fully-documented record shape consumed by
        table_3axis_interp_triple), not a continuation or a header
        false-positive. Element count for CompositeCalc itself: 10.
      - 0x11f4a CompositeCorr: index_src=F0C6 (AirTemp, ADC). Real
        caller: f2ea_f2ee_composite_correction_calc (@0x248ec), literal
        0x1f4a pushed into table_lookup_interp. Boundary not yet
        confirmed the way 11ee2's was.
      - 0x127d8 ISC: index_src=F0D0 (IAT_raw). Real caller:
        isc_f356_correction_calc (@0x25c61), literal 0x27d8 pushed into
        table_lookup_interp. Boundary not yet confirmed.
      - 0x1297a Trim: index_src=F0C8 (CoolantTempB, ADC). Real caller:
        f414_trim_calc_gated_by_f0f8_f3f0 (@0x2728d), literal 0x297a
        pushed into table_lookup_interp. Boundary not yet confirmed.
      UPDATE 2026-08-10 (same session): found these 3 ARE already in
      21000011_1997-2001_RVR_X3_Mt__4g63t_.xml (a different file than
      new/RVR_base.xml, which is where I'd checked before -- they were
      never missing from XML, just not where I looked). An earlier pass
      TODAY had already capped 11f4a/127d8/1297a's element counts to the
      physical byte-gap before the next table (14/32/46 respectively),
      but still mislabeled the axis as "RPM"/2d332 in all three. Fixed
      now: axis identity corrected in-place to match the real index_src
      register found above (F0C6/F0D0/F0C8), with UNCONFIRMED suffix
      since scaling/engineering-units are still unresolved and the
      capped element counts remain physical-gap placeholders, not
      confirmed true lookup ranges (no bounds check exists in
      table_lookup_interp itself, per the earlier finding).
      CAUTION FLAGGED: 1297a's existing "gap before next table" comment
      cites "12926 to 12954" as the boundary basis, but 0x12926 is
      BEFORE 0x1297a, not after it -- that citation is almost certainly
      a copy/paste artifact from the blanket trim pass and should not be
      trusted. The real next-table gap for 1297a has not been
      re-verified this session.
      NEXT STEP for Group A: boundary-walk confirmed complete for all 4
      as of this session (see UPDATE 2 below).

      UPDATE 2 2026-08-10 (same session): direct byte-read verification
      of all 3 remaining tables' boundaries, not trusting the earlier
      blanket-trim gap numbers:
        - 0x11f4a: CONFIRMED CORRECT as-is. 14 data bytes, all flat 0x80
          (128) -- plausible neutral/unity-correction table. Genuine
          next header starts cleanly at 0x11f5c, matching XML. No fix
          needed.
        - 0x127d8: WAS WRONG (32), same failure as 1297a below --
          blanket trim pass missed a real, previously-unlisted
          table_2d_record starting at 0x127e4 (index_src=F0C0/RPM) that
          sits well inside the assumed 127dc-127fc span. Real data:
          7 bytes (126,127,128,128,127,126,125) + an 0xFF end-marker
          sentinel (matches this file's already-documented sentinel
          pattern from SEED #5/#6/#9), then 127e4's genuine header
          starts cleanly. XML fixed: 32 -> 7.
        - 0x1297a: WAS WRONG (46), confirming the caution flag above --
          real data is 8 bytes (38,34,30,25,21,17,7,5, a clean declining
          curve), then a genuine NEW table_2d_record header starts
          cleanly at 0x12986 (also index_src=F0C8, interestingly).
          XML fixed: 46 -> 8.
      TAKEAWAY: the earlier blanket trim pass's "cap to nearest known
      table in its address list" method is NOT reliable -- it silently
      missed real intervening tables in 2 of 3 cases here. Any other
      table this project trimmed the same way should be treated as
      suspect until independently byte-verified the way these were,
      not just trusted because a plausible-looking comment exists.
      BONUS LEAD for Group B: the newly-found table at 0x127e4 uses
      index_src=F0C0 (RPM raw register) -- same register as Group B's
      0x1315c/0x13168. Real caller found: isc_f354_table_lookup_
      conditional (@0x25c15, xref from isc_f342_composite_correction_
      calc). MAJOR FINDING from checking it (see entry immediately
      below): F0C0 is NOT a dedicated RPM register after all.

  ** FOUNDATIONAL CORRECTION 2026-08-10 (same session): "F0C0 = RPM" IS
     NOT A FIXED IDENTITY **
    isc_f354_table_lookup_conditional (@0x25c15) calls axis_lookup_interp
    FIRST (literal push: bank=2, offset=0xd28c -> real address 0x2d28c),
    THEN table_lookup_interp (bank=1, offset=0x27e4 -> 0x127e4). This is
    the pattern our Group A tables notably do NOT show (no preceding
    axis_lookup_interp call in any of their 4 callers) -- so this is a
    genuinely different consumption pattern for the same F0C0 cell.
    Read the axis header at 0x2d28c directly:
      0x2d28c: F0 C0 F1 30 00 05  (output_dest=F0C0, count=5)
      0x2d292: 00 5d 00 7d 00 9d 00 ad 00 bd  (breakpoints: 93,125,157,
                173,189)
    Those are NOT RPM-shaped values (confirmed RPM axes in this ROM run
    500-11000+, e.g. the 2d332 18-elem axis). This proves F0C0 is a
    SHARED SCRATCH CELL that axis_lookup_interp writes its {index,frac}
    result into -- whichever axis was most recently interpolated by
    whatever caller ran last is what's actually sitting in F0C0 when a
    given table_lookup_interp reads it. It is NOT a dedicated, always-
    RPM register the way this file's existing pointer map implies.
    IMPLICATION: every table_2d_record with index_src=F0C0 (or likely
    F0C4/F0C6/F0C8/F0D0 too, by the same logic) needs its OWN caller
    checked individually for a preceding axis_lookup_interp call before
    trusting ANY fixed identity label for that register -- "RPM" (or
    "CoolantTempA" etc.) may be correct for some call sites and wrong
    for others simultaneously. This likely also bears on SEED #3's F0C0
    writer search: the reason no direct-addressing WRITE to F0C0 was
    ever found may simply be that axis_lookup_interp itself is the
    writer (a shared subroutine, not a per-register dedicated store),
    which direct byte-pattern search for a specific address wouldn't
    catch since the destination is a runtime argument, not baked into
    the write instruction's literal bytes.
    NOT YET DONE: checking whether Group B's 1315c/13168 -- if/when a
    caller is found -- follow this axis_lookup_interp-fed pattern or the
    Group-A-style raw/scratch pattern. Also not yet done: auditing
    OTHER existing F0C0="RPM" labeled tables elsewhere in this project
    (outside today's cluster) for the same risk.

    GROUP B -- UNTRACED, NO CALLER FOUND (header identity known by
    direct byte read only; zero real callers by any method tried;
    STATUS: CONFIRMED WRONG-UNRESOLVED, same "no caller" category as
    SEED #7d/#8 before their tooling breakthroughs -- do not assume
    dead, per this file's "never guess" discipline):
      - 0x1315c: index_src=F0C0 (RPM, read as raw register directly,
        NOT via axis_lookup_interp/0x2d332). Zero hits: get_xrefs_to,
        literal-push search (bf07315c), raw 2-byte "315c" scan.
      - 0x13168: index_src=F0C0 (RPM, raw register). Same zero-hit
        result across all three methods.
      - 0x1317c: index_src=F0C8 (CoolantTempB). Same zero-hit result.
      - 0x131ec: index_src=F0C8 (CoolantTempB). Same zero-hit result.
      NEXT STEP for Group B: same as SEED #7d/#8 -- try
      h8539_find_axis_consumer.py / h8539_find_table_axis.py-style
      whole-ROM scans rather than more manual xref-chasing, since the
      manual methods are now exhausted for these 4.

      UPDATE 2026-08-10 (same session): while chasing the 0x127e4 lead,
      found a THIRD distinct table-access convention in this codebase,
      separate from both table_lookup_interp(literal push) and the
      axis_lookup_interp-fed pattern: table_read_indexed(bank, offset),
      seen in f286_knock_octane_secondary_trim_compute_via_table (sibling
      of the 127e4 caller's dispatch family, status_word_table_
      subdispatch_group_a). Critically, its bank and offset arguments are
      loaded via SEPARATE, non-adjacent mov:i instructions (e.g.
      "bank=0xdb; uVar4=0xc;" with other code between the two loads) --
      NOT a single contiguous literal the way table_lookup_interp's
      "BF 07 <addr:16>" push is.
      IMPLICATION: this fully explains why the exhaustive contiguous-
      byte scans for 1315c/13168/1317c/131ec (bf07-prefixed push, and
      plain 2-byte "315c" etc. scans) all came back empty -- if any of
      these 4 are reached via table_read_indexed with a split bank/
      offset load (or a page value set once and reused across a whole
      block of calls), the address would never exist as a contiguous
      byte pattern anywhere in ROM, regardless of whether the table is
      genuinely consumed. This is a real methodological gap, not
      further evidence the tables are dead -- do not downgrade these 4
      to "likely dead" on the strength of the contiguous-scan result
      alone.
      REVISED NEXT STEP for Group B: search for standalone single-byte
      loads of the relevant low bytes (0x5c/0x68/0x7c/0xec) occurring
      near table_read_indexed call sites in the surrounding 0x13xxx
      cluster's likely callers (ISC/knock/trim dispatch family), rather
      than continuing to assume a single contiguous literal is required.

      UPDATE 2026-08-10 (same session, continued): checked this theory
      directly -- pulled ALL 5 callers of table_read_indexed in the
      entire ROM (f286_knock_octane_secondary_trim_compute_via_table,
      isc_f41a_correction_calc, isc_f420_correction_calc,
      octane_trim_f262_f264_table_select,
      table_select_scale_fanout_f1be_f1c2). Every single one uses ONLY
      RAM banks 0xd8-0xdb (small adaptive-trim/correction arrays).
      NONE ever uses bank 0x01 (ROM page 1, where 1315c/13168/1317c/
      131ec live). table_read_indexed is structurally scoped to a
      different address space and RULED OUT (not just unconfirmed) as a
      path to these 4 -- do not re-check this avenue in a future
      session.
      Group B therefore remains genuinely unresolved after this
      session's attempt: no caller found by direct xref, contiguous
      literal-push scan, raw 2-byte scan, or table_read_indexed. Next
      genuinely untried avenue: automated whole-ROM scan tooling
      (h8539_find_axis_consumer.py-style) per the original SEED #7d/#8
      precedent, since manual methods are now exhausted for these 4.

      FINAL UPDATE 2026-08-10 (same session): went further than planned
      and manually checked EVERY SINGLE caller of table_lookup_interp in
      the entire ROM (all ~65, enumerated via get_function_callers) plus
      all 5 callers of table_read_indexed, decompiling each and
      inspecting for any literal reference to 0x315c/0x3168/0x317c/
      0x31ec. ZERO hits across the entire set. This is now a genuinely
      exhaustive manual result, not a partial one -- every known code
      path that could plausibly reach a table_2d_record has been
      checked directly. Side-note found along the way: tcu_rx_main_
      scheduler's own plate comment (an earlier, independent session)
      already documented that F0C0 has no discoverable software writer
      by any method either ("no function or label references 0xF0C0...
      status stays... unsupported, inconclusive") -- consistent with,
      not additional to, this session's findings.
      CONCLUSION: manual methods are fully exhausted for Group B in
      this session. A future session should not re-attempt xref/literal/
      byte-pattern searches for these 4 -- go straight to automated
      whole-ROM scan tooling (h8539_find_axis_consumer.py-style, per
      SEED #7d/#8 precedent) as the only remaining untried avenue.

    SIDE FINDING (relevant to SEED #3): searched exhaustively for any
    direct-addressing write OR read of 0xF0C4 (CoolantTempA) anywhere in
    this ROM, same method already used for 0xF0C0. Result: zero genuine
    code references -- the one apparent hit (0x271ce) is a confirmed
    false positive (a `bset.w` instruction in isc_stepper_target_dispatch
    whose F0/C4 bytes are coincidental displacement/immediate bytes, same
    false-positive class already documented for F0C0). This is now a
    SECOND independently-discovered RAM cell with no software writer by
    any direct-addressing method, which strengthens (but does not yet
    confirm) SEED #3's DTC-write theory rather than being a one-off. Not
    yet checked: whether a DTC vector table entry targets F0C4, same open
    next-step already flagged under SEED #3.

--------------------------------------------------------------------
--------------------------------------------------------------------
STRUCT DEFINITIONS -- axis_lookup_record / table_2d_record /
table_3axis_record (2026-08-06, Claude)
--------------------------------------------------------------------
Motivation: every axis trace in this file has been manually re-deriving
the same "6-byte header, ptr1/ptr2/count" layout by hand, byte-by-byte --
slow and error-prone (the 7a Air Temp misdiagnosis happened partly
because of this). A pre-existing axis_lookup_record struct was found in
the project (created some earlier session) but only had 2 of its 4
fields named, with offsets 0-3 left undefined -- exactly the gap every
trace kept re-deriving by hand.

Rather than guess the fix, went to ground truth: decompiled the actual
primitive functions this data is fed to (axis_lookup_interp @0x14735,
table_lookup_interp @0x14656, table_3axis_interp_triple @0x148a2), and
cross-checked field offsets/types against h8539_import_tables_xml1.py's
own independently-written axis_shape_ok() (ghidra scripts/, ~line 470),
which already encodes three header shapes from its own bookmark-sweep
history (scaling-6b, 3d-value-7b, 2d-value-4b) without having been told
about the decompiler evidence. Decompiler and script agree exactly on
byte layout for all three shapes -- strong independent confirmation.

CONFIRMED: these are three genuinely different record shapes, not one
struct with optional fields, and they belong to two different roles:

  1. axis_lookup_record (8 bytes in Ghidra's byte-aligned layout, 6 real
     bytes of ROM header + 2 alignment; matches script's "scaling-6b"):
       offset 0: value_ptr  (ushort) -- RAM cell holding the X value to
                 look up; axis_lookup_interp reads it once at entry AND
                 writes the interpolated {index,frac} result back into
                 the SAME cell at exit (confirmed directly from
                 axis_lookup_interp's decompile: "*(ushort*)*_bank = uVar2"
                 at the end, where *_bank is this field).
       offset 2: axis_ptr    (ushort) -- a SECOND RAM cell, dereferenced
                 by axis_lookup_interp to get the actual value used for
                 the table search/comparison (lookup_value = *(ushort*)
                 _bank[1]). This is the field previously called "ptr2"
                 and wrongly suspected to be a fixed per-sensor-type
                 companion address -- it is NOT. Confirmed by direct
                 measurement: ptr1=F0C0(RPM) axes were found with THREE
                 different ptr2 values (F190 @2d192/2d5d6, F170 @2d2e0,
                 F1E0 @2d248) at different element counts, ruling out
                 any fixed RPM->ptr2 mapping. ptr2 is per-table-instance:
                 it's whichever specific derived/processed RAM cell that
                 particular caller's chain already deposited the live
                 sensor reading into before calling axis_lookup_interp,
                 not a property of the sensor type itself.
       offset 4: count       (ushort, clamped to 0xff/255 by the function
                 -- NOT byte+pad as the pre-existing struct's naming
                 implied; the high byte is always 0x00 in every example
                 because no axis has come close to 256 elements, which is
                 exactly why it LOOKED like byte+pad rather than a full
                 ushort. Confirmed against axis_lookup_interp's own
                 decompile: count_minus_1 = _bank[2] reads offset 4 as a
                 full ushort element).
       offset 6: breakpoints (ushort, first of an inline array, length =
                 count * 2 bytes, immediately following the header).

  2. table_2d_record (5 bytes: 4-byte ROM header + 1 start of data;
     matches script's "2d-value-4b", consumed by table_lookup_interp):
       offset 0: mode      (byte) -- 0x02 for this 2D byte-table shape
                 (script's own gate: b4[0]==0x02). NOT the same "mode"
                 concept as axis records; this is table_lookup_interp's
                 own dispatch byte (mode==3 triggers a different, 2D-
                 bilinear code path inside table_lookup_interp itself --
                 see table_3axis_record note below for why that's
                 confusingly named the same as the 3-axis mode value).
       offset 1: mode_hi   (byte) -- always 0x00 in every example.
       offset 2: index_src (ushort) -- RAM pointer to the SAME cell an
                 axis_lookup_record's value_ptr field points at (i.e.
                 the {index,frac} pair axis_lookup_interp already wrote
                 there). Confirmed directly from table_lookup_interp's
                 decompile: uVar3 = **(ushort**)(table+2) & 0xff (index),
                 frac = **(ushort**)(table+2) >> 8 (fraction) -- this
                 field is DEREFERENCED, not read as a raw value, proving
                 it's a pointer to the axis's result cell, not a second
                 copy of axis data.
       offset 4: data      (byte, first of inline value array).
     REAL CALLER VERIFIED (not just decompile-inferred): disassembled
     f28e_f292_knock_lambda_composite_calc @0x23456 directly. It pushes
     0xD218 (bank=2) into axis_lookup_interp FIRST, then pushes 0x1800
     (bank=1, = the "MAF Scaling (2D)" table @0x11800) into
     table_lookup_interp SECOND -- two separate pjsr calls, two separate
     records, exactly matching the two-stage {axis writes result cell},
     {table reads result cell} model above. 0x11800's own bytes read
     "02 00 F0 C0" -- mode=0x02, index_src=0xF0C0, matching a
     table_2d_record exactly (though note: in this specific call, the
     axis record pushed was 0xD218, NOT 0xF0C0 itself -- 0xF0C0 here is
     table_2d_record's OWN stored index_src field, which in this
     particular table points at the raw RPM register rather than at
     0xD218's result cell; the two 4-byte-header tables checked so far
     don't all point at the SAME cell axis_lookup_interp just wrote,
     which is a detail worth keeping in mind if this shape gets
     traced further -- it may mean some table_2d_record instances read
     a raw sensor register directly instead of an axis-interpolated
     index, bypassing axis_lookup_interp's index/frac computation
     entirely for that particular table. Not yet fully understood,
     flagged for follow-up, not blocking the struct's correctness for
     the fields it does have confirmed).

  3. table_3axis_record (9 bytes: 7-byte ROM header + 2 start of data;
     matches script's "3d-value-7b", consumed by
     table_3axis_interp_triple):
       offset 0: mode   (byte) -- 0x03 gates the full 2D-bilinear-style
                 interpolation path inside table_3axis_interp_triple
                 (matches script's own gate: b7[0]==0x03, b7[1]==0x00).
       offset 1: mode_hi (byte) -- always 0x00.
       offset 2: x_ptr  (ushort) -- RAM pointer, X-axis result cell.
       offset 4: y_ptr  (ushort) -- RAM pointer, Y-axis result cell.
       offset 6: nrows  (byte) -- row count/stride for the 2D grid.
       offset 7: data   (ushort, first of inline value array).
     VERIFIED against 0x11840 "Barometric Pressure Compensation (3D)":
     raw bytes 03 00 F0C0 F0C2 09 -- mode=3, x_ptr=F0C0(RPM),
     y_ptr=F0C2(Load/MAP), nrows=9. Both x_ptr and y_ptr are known-good
     sensor pointers from review5.md's own pointer map, giving high
     confidence in this shape without yet tracing a real caller
     end-to-end (unlike table_2d_record above, no disassembly-verified
     caller found yet for this specific address -- shape confidence
     comes from the header gate + decompiled consumer function
     agreeing, not from a traced call site. Should get the same
     caller-trace treatment as table_2d_record before being fully
     trusted the way axis_lookup_record now is).

     REAL CALLER NOW TRACED 2026-08-06 [Claude]: disassembled
     f28e_f292_knock_lambda_composite_calc (@0x23456) directly. Two
     corrections to the paragraph above:
     1. ENTRY POINT CORRECTED: the call site pushes 0x1840 (bank=1) and
        calls table_lookup_interp @0x14656 -- the SAME primitive used for
        2D tables -- NOT a separate table_3axis_interp_triple function.
        Disassembly of table_lookup_interp confirms it checks the
        record's mode byte itself (cmp.b #0x3,@R4) and branches
        internally to a distinct code path when mode==3. There is no
        separate 3-axis entry point at this call site; "consumed by
        table_3axis_interp_triple" in the paragraph above was never
        actually traced and should be read as superseded by this note.
     2. FIELD SEMANTICS REFINED: offset+2 (x_ptr/index_src) is
        dereferenced and split into index/frac (low byte / high byte),
        identical to table_2d_record's index_src -- confirmed consistent
        across both mode paths. But offset+4 (y_ptr) in the mode==3
        branch is dereferenced ONCE as a plain scalar and multiplied
        directly against nrows (offset+6) -- it is NOT split into an
        index/frac pair the way x_ptr is. The two are asymmetric despite
        both being called "*_ptr" in the struct doc; y_ptr looks like a
        raw row-index/selector, not an interpolated axis result.
     Separately, the same call site independently confirms axis
     0x2d20a (Load/MAP, 4 elem, header F0C2 F0BA 0004, data starting at
     0x2d210) as the real y-side input feeding this table -- matches
     and confirms the existing "Atmosphere Pressure" XML entry exactly,
     see FULL NAMED-AXIS CHECKLIST update above.
     STATUS: table_3axis_record record shape CONFIRMED CORRECT for
     offsets 0-2 and 6-7; offset+4 (y_ptr) semantics OPEN -- needs
     tracing table_lookup_interp's mode==3 branch a level further (what
     RAM cell does 0x2d20a's caller write into F0C2 before this call,
     and is it always a raw row-selector or sometimes something else)
     before fully trusting the "y_ptr = RAM pointer, Y-axis result cell"
     description as originally written.

table_lookup_indexed/table_read_indexed (the other two review4.md
primitives) do NOT introduce a new record shape -- both are pure
index-resolving wrappers around table_lookup_interp/a raw array read,
keyed off DAT_0001f1fc & 7, and hand off to the same table_2d_record-style
argument. No separate struct needed for them.

GHIDRA STATE (2026-08-06): all three structs created in Ghidra's data
type manager (deleted and recreated axis_lookup_record cleanly rather
than patching the old one in place, after an in-place field-add attempt
produced a wrong 4-byte pointer-typed field and had to be backed out --
all three fields are plain `ushort` raw RAM-offset values, NOT Ghidra
pointer types, since the H8's bank/segment addressing doesn't map onto
flat 32-bit pointers cleanly). axis_lookup_record applied so far at 12
confirmed axis header addresses: 0x2d192, 0x2d5d6, 0x2d2e0, 0x2d248,
0x2d4a6, 0x2d302, 0x2d530, 0x2d412, 0x2d3ee, 0x2d3fe, 0x2d596, 0x2d32c.
table_2d_record applied at 0x11800. table_3axis_record applied at
0x11840. Remaining ~35 X-axis addresses and the full Y-axis list (not
yet pulled per line 427 below) still need axis_lookup_record applied --
mechanical now that the struct exists, no further caller-tracing
required for THAT part, though the axis's NAME/scaling verdict work
(this file's actual goal) is separate from just applying the type.
STATUS: axis_lookup_record CONFIRMED CORRECT AND VERIFIED (multiple
independent real callers traced). table_2d_record CONFIRMED CORRECT for
its documented fields but with an open follow-up question (does
index_src always point at an axis_lookup_interp result cell, or
sometimes at a raw sensor register directly -- see note above).
table_3axis_record CONFIRMED-BY-SHAPE, still needs one real
disassembled caller for full confidence parity with the other two.

--------------------------------------------------------------------
ALSO IN SCOPE (from earlier Ghidra ROM-scraper "Scaling" bookmark sweep,
ALSO IN SCOPE (from earlier Ghidra ROM-scraper "Scaling" bookmark sweep,
2026-08-05 session) -- addresses not yet matched to any named axis
--------------------------------------------------------------------
Cross-referencing the ~87 H8539F-SCRAPED-TABLE (Scaling) bookmarks
against RVR_base.xml's current named axes showed that the great majority
are just the 6-byte headers sitting immediately before axis data already
correctly captured under other names elsewhere in the file (e.g. scraped
0x2d302 is the header for the already-named "Load" axis at 0x2d308;
0x2d2aa is the header for "RPM" at 0x2d2b0; 0x2d09a is the header for
"RPM" at 0x2d0a0; 0x2d218 is the header for "Load (MAF Hz raw)" at
0x2d21e; 0x2d248 is the header for "RPM" at 0x2d24e). Those do not need
new XML entries -- they're redundant with what's already there and
should NOT be added as standalone "Scaling Tables" (an earlier attempt
this session to do exactly that was reverted -- EcuFlash requires a real
or synthetic axis to read more than 1 byte, so a bare address dump
doesn't render as a table anyway).

Genuinely unclaimed (no matching header-adjacent axis found in current
XML), raw bytes already pulled 2026-08-05:

  0x12065 (18 db) -- small signed-byte curve, runs ~-10..+23, monotonic,
    crosses zero. Correction/trim shape, NOT a physical sensor axis.
    Caller not yet traced.
  0x1208b (12 db) -- same shape as above (~-10..+15).
  0x1209e (12 db) -- same shape (~-10..+13).
  0x121c3 (12 db) -- same shape (~-10..+16).
  0x121d6 (10 db) -- same shape (~-10..+14).
  0x12222 (10 db) -- same shape (~-10..+5, smaller range).
  0x1246f (10 db) -- same shape (~-10..+9).
    All 7 above share an identical structural fingerprint (3-4 negative
    bytes, a zero, then a monotonic increasing run to low double
    digits) and sit in a tight address cluster (0x12065-0x1246f, all
    within the Ignition Trim / Warmup Retard / Boost Enhancement address
    neighborhood in the existing XML -- see Timing category, ~0x127xx-
    0x128xx). Strong candidate: these are the correction-value AXES
    (not lookup axes -- the axis dimension read by an ignition or
    fueling trim table) for a family of trim tables in that
    neighborhood, i.e. they may be the actual RIGHT axis data for
    something currently using an unrelated (wrong) axis, OR they may be
    entirely new not-yet-tabled corrections. Needs caller trace.
    STATUS: OPEN, not yet traced.

  0x13576 (284 db) -- 16-bit big-endian monotonic ramp, 14 up to 229,
    non-linear (slow-fast-slow / roughly logarithmic). ~140 elements.
    Far bigger than any axis currently in the XML. Shape resembles a
    frequency-to-load or airflow conversion table (compare to MAF
    Scaling / MAF Smoothing Table category, though those are much
    smaller). STATUS: OPEN, not yet traced. Given the size, likely a
    genuine standalone value table, not an axis -- may belong in
    review2.md's scope (table address verification) rather than here.

  0x23cea (174 db) -- no clean 8-bit or 16-bit monotonic structure found
    on inspection. Does not fit the scaling-table shape at all.
    SUSPECT MISCLASSIFICATION -- likely code or a packed/mixed record
    the ROM scraper mistakenly bookmarked as "Scaling". Needs a
    disassembly check at this address before doing anything else with
    it (see review2.md Method step 2's header-shape classification).
    STATUS: OPEN, needs classification, not axis analysis.

--------------------------------------------------------------------
FULL NAMED-AXIS CHECKLIST (from RVR_base.xml, 2026-08-05 pull)
--------------------------------------------------------------------
Legend: [ ] not yet checked   [x] checked, see note

X Axes (45 total tag instances, deduplicated by address below):
[x] 2d210 "Atmosphere Pressure" (4 elem, scaling=Load16) -- CONFIRMED CORRECT
    2026-08-06 [Claude]: real caller found. f28e_f292_knock_lambda_composite_calc
    (@0x23456) pushes axis record 0xD248 (RPM, already-applied) then 0xD20A
    (real addr 0x2d20a) into axis_lookup_interp before calling
    table_lookup_interp(0x1840) ("Barometric Pressure Compensation (3D)").
    Header at 0x2d20a = F0C2 F0BA 0004 (ptr1=F0C2=Load/MAP, count=4), data
    starts exactly at 0x2d210 -- matches XML address/element-count exactly.
    Name and scaling both confirmed correct as-is, no XML change needed.
[ ] 2d308 "Load" (18 elem, scaling=Load) -- used on 4 fuel map tables
[ ] 2d35e "Load" (14 elem, scaling=Load)
[ ] 2d42e "Time"/"Engine Temp" (9 elem, scaling=Temp) -- NOTE: same
    address used with two different names ("Time" line 244, "Engine
    Temp" lines 248/264) -- add to SEEDS, needs its own contradiction
    check same as 2d418.
[ ] 2d446 "Temp" (11 elem, scaling=Temp)
[x] 2d418 "Engine Temp"/"Air Temp" -- SEE SEED #1. FIXED 2026-08-05: sole
    "Air Temp" outlier (on Ignition Trim vs Air Temp) corrected to
    "Engine Temp". 0x2d4c0 three-way conflict still OPEN, separate issue.
[ ] 2d3fe "Engine Load" (7 elem, scaling=Load) -- also appears as
    "Axis2 (possible: Load)" scaling=raw on a different parent table,
    see SEED #4. Two different names/scalings at the same address here
    too.
[ ] 2d11c "Engine Speed" (10 elem, scaling=RPM)
[ ] 2d3ee "Axis1 (possible: RPM)" -- SEE SEED #4
[x] 2d2e6 "RPM"/"Engine Load" -- SEE SEED #2. FIXED 2026-08-05: both
    "Engine Load" (10-elem) entries corrected to "RPM" (14-elem).
[ ] 2d382 "Load" (19 elem, scaling=Load) -- used on 5 ignition/boost
    tables
[ ] 2d28c "Baro Press." (5 elem, scaling=Load16)
[ ] 2d086 "Engine Speed" (10 elem, scaling=RPM) -- SEE SEED #7f (2026-08-06):
    real caller traced (throttle_body_update), header ptr1=F0C4=CoolantTempA
    not F0C0=RPM. Naming suspect but NOT changed -- real consumer/table not
    yet found. Do not rename without finishing that trace.
[ ] 2d490 "Engine Temp" (8 elem, scaling=Temp)
[ ] 2d27a "Battery" (9 elem, scaling=BatteryVoltage)
[ ] TAR (no address -- static/synthetic?) (10 elem, scaling=Load16) --
    used on Knock Sensor Filter Maps 1-5. Verify this is genuinely
    static and not missing a real address.
[ ] 2d4c0 "Coolant Temp"/"Air temp"/"Air Temp" -- THIRD instance of the
    same-address-different-name pattern. Add as SEED #5.
[x] 2d536 "Error" -- SEE SEED #3. Address confirmed correct and correctly
    shared with Canister Purge (already named "RPM" there). "Error" name
    on Idle Error Correction tables still wrong -- STILL OPEN pending
    caller trace for exact replacement name.
[ ] 2d3c8 "TPS" (9 elem, scaling=TPS Percent)

Y Axes (2026-08-09 pull, Claude): grep type="Y Axis" against RVR_base.xml,
dedup by address, cross-referenced against already-resolved SEEDS. 13
distinct addresses total; 5 already covered by prior seed work (2d4ac
Air Temp/SEED#7a, 2d3fe Axis2/SEED#4, 2d332 RPM, 2d59c Boost Error/SEED#5,
2d5dc RPM/SEED#7b -- all CONFIRMED CORRECT or CONFIRMED WRONG-FIXED
already). Remaining unchecked: 2d21e (Load MAF Hz raw, 21 elem), 2d24e
(RPM, 9 elem), 2d2b0 (RPM, 14 elem), 2d0a0 (RPM, 16 elem), 2d260
(Battery Voltage, 7 elem), 2d656 (Load, 10 elem), 2d418 (Engine Temp,
8 elem -- same address as the X-axis SEED#1 contradiction, now also
used as a Y-axis, worth an independent check), 2d4d6 (ISCV Demand,
27 elem), 2d100 (RPM, 10 elem -- see below), 2d2d2 (RPM, 7 elem,
scaling=RPMStatLimit), 2d576 (RPM, 16 elem). (Addressless "PN1"
CaseAlphaNumeric entries excluded, not real axes.)

2d100 Y-axis check (2026-08-09, Claude): this is the SAME "Boost Limit"
table (0x11cee) already dead-ended in SEED #7d as an X-axis -- 2d100 is
its Y-axis, not a new lead. Re-ran all three exhausted methods now that
disassembly is more mature: get_xrefs_to(0x11cee) and get_xrefs_to(0x2d100)
both still zero; literal-push byte search (bf070cee / bf07d100) still
zero; broadened 2-byte "0cee" scan turned up several NEW candidate hits
beyond the one already-documented f216_bit6_update_from_hysteresis_gate
scalar-compare (0x21fd4) -- checked the first two by full disassembly
(0x14084 inside mul_u16_x2_or_sum_check_overflow, 0x14ee0 inside
mut_configurable_reqid_periodic_snapshot_8word): both confirmed false
positives, coincidental FP-displacement/immediate byte pairs unrelated
to 0x0cee, same noise category already documented for other addresses
in this file. STATUS: SEED #7d dead-end RECONFIRMED with the more mature
disassembly, no new evidence found either way. Not spending further
time here per the file's own guidance (flagged for broader/automated
search, not more manual xref-chasing) -- moving to other unchecked
Y-axis addresses instead.

--------------------------------------------------------------------
NEXT SESSION START HERE
--------------------------------------------------------------------
1. Pull the Y-Axis address list (grep type="Y Axis", dedup by address)
   and add it to the checklist above before starting any tracing --
   need the full picture of contradictions before prioritizing.
2. Work remaining SEEDS first (already-known contradictions, highest
   information value per trace). SEEDS #1, #2, #5, #6 fixed (2026-08-05/
   06); SEED #7 (the AXIS-DATA-OVERLAP cluster) mostly resolved 2026-08-06
   -- #7b, #7e, and #7a all fixed 2026-08-06, #7c was noise (no fix
   needed), #7d still open (see below). Still open from earlier seeds:
   #3 (0x2d536 "Error" name on Idle Error Correction -- needs caller
   trace, likely a Load-family rename), #4 (0x2d3ee/0x2d3fe scaling
   formula), and the 0x2d4c0 three-way naming conflict spun out of #1.
3. SEED #7d (0x2d0f6/0x2d100, "Boost Limit" RPM axis) is a genuine dead
   end via direct xref -- get_xrefs_to found zero callers for the parent
   table (0x11cee) AND for 0x2d100 itself. Needs the literal-constant
   search fallback from review2.md Method step 4/review5.md Method step
   3 (search for 0x2100 or 0xd100-style push-immediate bytes) rather than
   get_xrefs_to, since the caller may reach it some other way (jump
   table, computed call, etc).
5. RESOLVED 2026-08-06: old/RVR_base.xml is a frozen backup, confirmed
   out of scope for this review -- leave it as-is, do not mirror fixes
   there. All fixes in this pass apply only to
   test/rvr/xml/ecuflash/new/RVR_base.xml.
6. Then work the 7-address 0x12xxx trim-curve cluster (small, tight
   address range, likely fast to resolve as a group once one caller is
   found).
7. 0x13576 and 0x23cea need their own investigation track (table
   classification, not axis identity) -- probably belongs alongside
   review2.md's Batch 1 method rather than here; flag if picking this up
   and unsure which file to log into.
8. STRUCT DEFINITIONS section (added 2026-08-06) has axis_lookup_record/
   table_2d_record/table_3axis_record now created and verified in
   Ghidra. axis_lookup_record still needs applying (apply_data_type) at
   the ~35 remaining X-axis addresses and the full Y-axis list once
   pulled (item 1 above) -- mechanical, no caller-tracing needed for
   that part. table_2d_record has one open follow-up (does index_src
   always point at an axis result cell, or sometimes a raw register --
   see struct section) and table_3axis_record still needs a real
   disassembled caller traced (currently confirmed by shape/decompile
   agreement only, not a live call site) before it has the same
   confidence level as the other two.

Review: TableHeader struct rollout + XML header-claim verification
====================================================================

Scope: RVR_1998_x3 4g63t 21000011 md352553.hex, RVR table region
0x2d080-0x2d8d6 (78 header-shaped entries per review12.md's section 4
index). Picks up directly from review12.md's raw index and RVRbase.xml's
per-entry comments.

Context
-------
Created struct TableHeader (ushort ram_ptr1; ushort ram_ptr2; ushort
element_count; -- 6 bytes) matching the shape review12.md confirmed live
against ROM and cross-verified against EVO5. Applied it at 78 addresses
across 0x2d080-0x2d8d6, sourced from RVRbase.xml's own per-entry
verification comments (which turned out to already have a far more
complete sweep of this region than review12.md alone reflected).

IMPORTANT CAVEAT, raised mid-session and not yet fully resolved: the XML
comments are themselves a claim, not ground truth. A live check was
started to corroborate each header address against real code (does
some function actually push this address into the shared table-lookup
helper at 0x14735?), the same pattern independently confirmed for
0x2d3fe and 0x2d3ee via direct disassembly. That check was NOT run to
completion across all 78 addresses before the session moved on -- the
following 7 items are what's left to verify/resolve.

Method used successfully so far (repeat this for remaining addresses):
for a given header address, get_field_access_context to find real xrefs;
if a hit exists, confirm via disassemble_function/disassemble_bytes on
the caller that the address is pushed immediately before a call to the
shared lookup helper (`mov:g.w #0xADDR:16,@-SP` / `mov:g.w #0x2:8,@-SP`
/ `pjsr @0x14735:24`) -- that's the corroborating signature, not just a
byte-pattern match. 33 addresses (0x2d080 through 0x2d66a) were
confirmed this way with a live caller before the sweep was interrupted.

Open items
----------

1. Two isolated "no live xref" misses inside the otherwise-confirmed
   run (0x2d080-0x2d66a): 0x2d0d2 and 0x2d1b4 both came back with zero
   entries from get_field_access_context, unlike every neighboring
   address in the same sweep. TableHeader is still applied at both.
   TODO: re-check under both address-bank-prefix forms per the
   methodology lesson in review.md's 2026-08-07 session entry (checking
   only one prefix is a known blind spot), and grep the full .c dump
   (test/rvr/RVR_1998_x3 4g63t 21000011 md352553.hex.c) for the literal
   address text as a second, structurally-different check before
   concluding either way.

2. A confirmed run of consecutive zero-xref misses starting at 0x2d684
   and continuing through at least 0x2d730 (0x2d684, 0x2d69e, 0x2d6b4,
   0x2d6da, 0x2d702, 0x2d71a, 0x2d730 -- 7 addresses checked, all zero).
   This is a large enough contiguous block that it's more likely a real
   pattern than isolated noise -- candidate explanations: (a) genuinely
   dead/unused tables on this calibration, (b) reached only via an
   indirect/computed call the xref scanner doesn't resolve (same known
   blind spot documented elsewhere in this project for bank-prefixed/
   indirect RAM writes), or (c) the XML's shape-matching here was
   byte-pattern-only and wrong. TODO: grep the .c dump for each address;
   if still no hit, treat as SUSPECT per review.md item 3's planned
   VERIFIED/SUSPECT/REJECTED classification (not REJECTED outright --
   absence of a resolvable xref isn't proof of a wrong struct, per the
   project's own repeated lesson about premature "definitely nothing
   here" conclusions).

3. Sweep was not finished: 0x2d74c through 0x2d8d6 (the tail of the
   78-address list, ~15 addresses) were never individually xref-checked
   in this session. TODO: run the same get_field_access_context sweep
   used for the rest of the region; log any further zero-xref misses
   alongside items 1/2 rather than assuming they're clean.

4. For every zero-xref address found (items 1/2/3 combined), the
   decompiler output on the region was separately observed to be
   degraded (heritage/deadcode-elimination warnings, unresolved
   function-pointer calls with hidden arguments -- seen concretely on
   f0ba_f2d0_o2_mode7_8_correction_calc at 0x242f5, where the live
   disassembly proved a real 0xd3fe reference existed that the
   decompiled C output didn't textually show at all). TODO: for any
   address still unresolved after items 1-3's .c-dump-grep pass,
   fall back to raw disassemble_function/disassemble_bytes on
   candidate caller regions rather than trusting decompiled C or the
   xref index alone -- both were independently shown unreliable on this
   region this session.

5. None of the 33 xref-confirmed headers have been checked for the
   SECOND half of the corroboration -- confirming the header's
   element_count field actually matches the XML's declared table length
   at the point of use (review.md item 3's planned Step 5d does this
   generally; it hasn't been run against this specific 78-address set).
   TODO: for each confirmed header, read the live element_count byte and
   diff it against the XML's stated element count; log any mismatch as
   its own follow-up rather than assuming shape-match implies
   count-match.

6. Naming was intentionally limited this session to addresses with a
   named, non-generic caller function (8 done: hdr_maf_hz_raw,
   hdr_boost_error, hdr_post_start_enrich_rpm, hdr_idle_stepper_lookup,
   hdr_idle_stepper_lookup_2, hdr_map_tps_axis,
   hdr_octane_correction_axis, hdr_closedloop_eligibility). Two
   early tentative names (for 0x2d45c and 0x2d3c2) were caught as WRONG
   once actually checked against their real caller and corrected --
   worth remembering as a concrete example of why the XML's semantic
   "Likely RPM/Load" guesses should not be used to name anything
   directly. TODO: continue the same caller-xref-first naming approach
   for the remaining ~66 struct'd-but-unnamed addresses, once items
   1-3 have resolved which of them are even genuinely live.

7. No revert decision has been made for the confirmed-zero-xref
   addresses (items 1-2, ~9 so far). TableHeader is currently applied
   at all of them without a live-code caller backing the claim. TODO:
   decide policy -- most likely keep the struct applied (worst case it's
   a relabeled 6 bytes with no functional risk) but add a plate comment
   flagging "struct applied per XML claim, no live xref found as of
   2026-08-15" at each, so a future session doesn't mistake it for a
   fully-verified entry the way the other 33 now can be.

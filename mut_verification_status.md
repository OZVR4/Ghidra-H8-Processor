METHODOLOGY / RECIPE (added 2026-07-22, distilled from what's actually
worked across sessions -- read this first, especially when starting the
same kind of investigation on a different ECU/ROM)
============================================================
Goal of this whole document: fully name and document every MUT RequestID,
dispatcher command byte, RAM flag, and physical output pin on this ECU so
the result can become a reusable, shareable signature applicable to other
Mitsubishi H8/500-family ECUs, not just this one ROM.

WHAT HAS RELIABLY WORKED:

1. Prefer tracing FORWARD FROM REAL HARDWARE over backward from assumed
   command semantics. Command-byte/RAM-flag chains (e.g.
   sci1_meta_cmd_dispatch_c0_ff's f510/f512/f516 bits) very often terminate
   in further internal RAM state machines with no confirmed physical
   meaning. Starting instead from confirmed hardware registers -- H8 port
   data registers P1DR-PCDR (0xfe80-0xfe97), SCI1/2/3 UART registers
   (0xfec0-0xfed5), timer registers (0xff2x+) -- and tracing BACKWARD to
   find their writers is far more likely to land on something with a real,
   nameable physical identity (a relay, a solenoid, a lamp). See the
   "PHYSICAL OUTPUT PIN TRACE" section below for the method that produced
   the first confirmed command-byte-to-physical-pin chain in this project.

2. ALWAYS confirm you have a function's true ENTRY POINT before commenting
   or labeling. search_byte_patterns matches raw bytes anywhere in the ROM,
   including mid-function -- it does NOT return function boundaries. Call
   get_function_by_address on every hit before treating it as "the
   function". Plate comments silently fail (or attach nowhere useful) if
   set on a non-entry address -- verify with a decompile/re-read after
   every batch_set_comments call, don't trust the tool's own success flag
   alone.

3. For any register/table you plan to describe as a bitmask, byte layout,
   or repeating structure: verify FINAL entries and conclusions via
   individual read_memory calls anchored on absolute addresses, not by
   eyeballing one long combined hex dump and counting bytes by hand. This
   project has already produced two separate miscounting errors doing it
   the fast way (a table entry-count error, and a specific bit-mask
   misread) -- both caught only because they were double-checked
   independently before being written down as fact. The slow way is worth
   it whenever a finding is about to be saved as an established fact
   rather than a rough lead.

4. Before accepting a plausible-looking hypothesis (e.g. "this graduated
   mode-select value looks like a PWM fan-speed driver"), check the ACTUAL
   register labels via list_globals for anything you're about to name as a
   guess. This project had a specific case where a well-reasoned-sounding
   fan-PWM hypothesis was completely wrong once the real register labels
   (SCI1/2/3 UART SMR/BRR/SCR) were checked -- the corrected finding is
   preserved alongside the original wrong one in the relevant plate comment
   so future readers don't rediscover the same dead end.

5. When re-deriving a decompiler's rendering of an addressing mode (banked/
   segmented EA forms in particular), trace the actual Sleigh grammar
   constructor (the .slaspec/.sinc pcode body) rather than reverse-
   engineering the intent from the decompiler's C-like output alone -- the
   decompiler's CONCAT/tracked_set folding is a useful cross-check but the
   grammar itself is the ground truth for what an instruction actually
   computes. See the DP=2 pointer table derivation below for a worked
   example (eaw_disp16 -> Rn_banked chain).

6. Label AND comment as you go, not as a final pass. Every RAM cell, ROM
   table, and function that gets a confirmed (or well-evidenced
   provisional) meaning should get: a descriptive name via rename_or_label/
   rename_function, and a plate comment stating what's confirmed, what's
   inferred, what's still open, and pointing back to the relevant section
   of this file. This is what actually accumulates into a reusable
   signature -- comments only in this markdown file, without corresponding
   Ghidra labels, don't transfer to a fresh project the same way.

7. When a hypothesis turns out wrong, CORRECT THE EXISTING PLATE COMMENT IN
   PLACE (don't just note the correction elsewhere) -- the whole point is
   that the next person (or the next session) reads the function and gets
   the corrected understanding immediately, not a stale wrong one plus a
   pointer to a correction buried in this file's chronological log.

8. Keep confidence levels explicit and honest in every write-up:
   CONFIRMED (independently re-derived from live disassembly/memory, ideally
   twice), LIKELY/well-evidenced (a real mechanism traced, but physical
   real-world identity inferred rather than proven), or OPEN/no writer
   found. Do not let a "LIKELY" finding get cited elsewhere as if it were
   CONFIRMED -- several sections below deliberately flag this distinction
   (e.g. the P1DR.5/ignition-output lead) precisely so it doesn't harden
   into an unearned fact over time.

WHAT HAS NOT WORKED / TO AVOID:
- Relying on external tooling's actuator/RequestID lists (EvoScan's UI,
  MUT-III's mutdata.mdb, MUT-II hand-programmer documentation) as a source
  of ground truth for THIS ROM. These reflect a generic/shared convention
  across many Mitsubishi ECUs and have already been shown to NOT match
  what this specific ROM's dispatcher actually implements (the DP=2 table
  finding). Use them only as naming inspiration once a real ROM mechanism
  is independently confirmed, never as a substitute for tracing the ROM
  itself.
- Chasing a RAM flag that turns out to be shared across 100+ xref sites
  (e.g. f20e, f502) to a single conclusive meaning in one sitting -- these
  are typically genuine shared engine-state words, not actuator-specific.
  Recognize the scale early (a bulk search_byte_patterns hit count is a
  fast signal) and either scope the trace narrowly (one specific
  consumer/writer pair) or note it as a low-priority/impractical thread
  rather than getting stuck.


0x2fad0 (indexed via adc_sensor_convert_single @0x171c3) and cross-
referencing every entry against Mitsubishi_MUTII_EFI.xml (the standard
Evo 4/5/6+ EFI EvoScan profile the RVR's own MUT profile was adapted
from, per logging.txt's PURPOSE section). Also cross-checked (2026-07-13)
against Mitsubishi_MUTII_EFI_GalantLegnum.xml (same-generation Galant/
Legnum EC5x EvoScan profile, ~40 items) -- smaller profile, but it named
two RequestIDs this table had BLANK, and raised a conflict worth flagging
on RequestID 0x00 (see rows below and the note after the table).

IMPORTANT CAVEAT: "Name (per EFI profile)" below is an INHERITED CLAIM,
not a verified fact. Several of the already-investigated entries in this
table turned out to be WRONG for this specific ROM (knock_flag, and the
Engine RPM claim at RequestID 0x21/F17B, both refuted -- see below and
review.md item 7). Every "NAMED (untraced)" row is a lead worth checking,
not a conclusion. Status column meanings:
  CONFIRMED        - address AND semantics independently verified live
  REFUTED          - address correct, semantic label proven wrong
  OPEN             - address known, writer/relationship not fully traced
  NAMED (untraced) - EFI profile claims a name, nobody has checked it yet
  BLANK (untraced) - no name anywhere (not even the stock EFI profile),
                     completely uninvestigated -- may be RVR/Tephra-mod-
                     specific, unused, or reserved

NOTE ON THE 2026-07-14 SWEEP: every BLANK/NAMED(untraced) row EXCEPT the
following was checked via get_bulk_xrefs and came back with ZERO hits (see
"PRE-EMPTIVE SWEEP" section near the end of this file for the full caveat --
zero hits means "not found by this cheap method," NOT "confirmed unused").
Rows with an actual finding from that sweep are individually annotated
in-line below with "POINTER (2026-07-14 sweep)" or "LIKELY SCRAPER ARTIFACT":
0x0F (F26E), 0x2A/0x2B (F971/F972), 0x4F (F270, see sweep section), 0x80-0x82
(F5C0/F5C1/F5C3). Every other BLANK/NAMED(untraced) row below was included
in the sweep's zero-hit set and remains exactly as uninvestigated as before
2026-07-14 -- the sweep did not narrow them down, it just confirmed the cheap
method doesn't see them.

| ReqID(hex) | Addr  | Name (per EFI profile)                          | Status |
|---|-------|----------------------| 00/01 | F0F7/F0F6 | LoadMUT2Byte (deprecated 2-byte load)         | NAMED (untraced) -- THIRD CONFLICTING CLAIM (2026-07-15): the same uploaded Mitsubishi_MUTII_EFI.xml that names this LoadMUT2Byte ALSO has a separate entry for ReqID 0x00 called "gmas" / "Airflow Rate gm/s" (Eval="x", no formula detail) -- an internal collision within the same XML file, not just a cross-file conflict. Combined with the GalantLegnum packed-switch-flags claim already noted, ReqID 0x00 now has THREE mutually incompatible claims across sources (LoadMUT2Byte, switch-flags byte, airflow-rate). Strongly suggests this ID has been repurposed differently across ROM revisions/mods rather than having one stable meaning -- do not trust any single claim without live tracing. |gs byte) before trusting either label. |
| 02/03 | F0FD/F0FC | RPMMUT2Byte (deprecated 2-byte RPM)           | NAMED (untraced) -- RPM-adjacent, set aside per user |
| 04/05 | F343/F363 | AirFlowMUT2Byte                               | OPEN -- item 6, non-contiguous hi/lo pair flagged |
| 06 | F361 | Timing Advance (TimingAdv)                          | NAMED (untraced) |
| 07 | F109 | Coolant Temp                                        | CONFIRMED (addr only, no writer) |
| 08 | EECA | ISC Correction (mode-shadow copy)                   | CONFIRMED (2026-07-15): written by isc_eeca_eecc_eece_correction_calc (0x27a81), which computes one ISC correction term and mode-selects which of EECA/EECC/EECE gets it, based on F0F8 bit4/bit0 + byte@0x2c7. EECA = default-mode target. See EECC row and plate comment on thi| 09 | F3FB | ISC Correction low byte (mode-shadow, default mode) | CONFIRMED (2026-07-15): F3FB is the LOW byte of the 16-bit word written at F3FA by isc_f3fa_f3fc_f3fe_correction_calc (0x27990) -- confirmed real, not a scraper artifact (see adc_sensor_convert_single @0x171c3: ReqID<=0xBF reads a SINGLE byte at the table-decoded address, so the 0x2fad0 table's entry for 0x09 must decode directly to 0xF3FB). Same mode-shadow idiom as EECA/EECC (0x08/0x0A) above -- F3FA/F3FB pair = default-mode ISC correction (F0F8 bit4==0). |                          | BLANK (untraced) |
| 0A | EECC | ISC Correction (mode-shadow copy, alt mode)         | CONFIRMED (2026-07-15): same writer/value as EECA (0x08) above -- isc_eeca_eecc_eece_correction_calc (0x27a81) selects EECC when F0F8 bit4=1, byte@0x2c7!=1. A third shadow (EECE, not a MUT ID in this table) covers the remaining mode. All three are the SAME computed correction value; MUT reading 0x08 vs 0x0A just pi| 0B | F3FD | ISC Correction low byte (mode-shadow, alt mode)     | CONFIRMED (2026-07-15): F3FD is the LOW byte of the 16-bit word written at F3FC by isc_f3fa_f3fc_f3fe_correction_calc (0x27990) -- same reasoning as F3FB/0x09 above. F3FC/F3FD pair = alt-mode ISC correction (F0F8 bit4==1, byte@0x2c7!=1). A third pair (F3FE/F3FF, not a MUT ID in this table) covers the remaining mode, mirroring EECE. |                           | BLANK (untraced) |
| 0C | EE99 | Fuel Trim Low (LTFT)                                | NAMED (untraced) |
| 0D | EE9B | Fuel Trim Mid (LTFT)                                | NAMED (untraced) |
| 0E | EE9D | Fuel Trim High (LTFT)                               | NAMED (untraced) |
| 0F | F26E | Oxygen Feedback Trim (STFT)                         | NAMED (untraced) -- POINTER (2026-07-14 sweep): written inside tcu_shift_torque_and_knock_mgmt (0x28fff, ~0x96b0-0x96d1), a clamped accumulator fed by F264/F262 (<<2, add/sub via sat_add_u16/sat_sub_u16 depending on F21C Also touched by a SEPARATE function (0x20782, renamed 2026-07-14 to
f26e_cluster_accumulator_unrelated_to_ihex, formerly misleadingly named
sci1_boot_ihex_data_byte_store), misleadingly named sci1_boot_ihex_data_byte_store) which reads/writes F26E directly at 0x207a0-0x207a4 as a simple 16-bit accumulator (`add:g.w R0,R1` then store back). Two distinct writers to the same address is unusual -- worth reconciling before trusting either as "the" Oxygen Feedback Trim writer. Does NOT look like classic STFT (short-term fuel trim) math (no O2-sensor-error-driven PI/PID pattern visible in either writer) -- treat the inherited label with suspicion. |
| 10 | F10F | Coolant Temp Scaled                                 | CONFIRMED (addr only, no writer) |
| 11 | F119 | MAF Air Temp Scaled| 12 | F111 | EGR Temperature                                     | NAMED (untraced) -- also claimed as "LC1 Wideband o2 e8 mod" (LC1WidebandO2) in the same source XML (2026-07-15), but that entry's own Notes say it requires wiring an external 0-5V sensor into the ECU -- a user hardware mod repurposing this cell on modified ROMs, not a competing claim about the stock ROM's meaning. EGRTemp remains the primary lead. |                                   | NAMED (untraced) |
| 13 | F11F | Oxygen Sensor                                       | CONFIRMED |
| 14 | F13B | Battery                                             | CONFIRMED |
| 15 | F12F | Barometer                                           | NAMED (untraced) |
| 16 | EED5 | ISC Steps                                           | CONFIRMED |
| 17 | F13D | Throttle Position (TPS)                             | CONFIRMED |
| 18 | F21D | --                                                  | BLANK (untraced) |
| 19 | F217 | --                                                  | BLANK (untraced) |
| 1A | F1DF | Air Flow Hz                                         | NAMED (untraced) |
| 1B | EE83 | ECU Load | 1C | F187 | ECULoad                                              | NAMED (untraced) -- source: uploaded Mitsubishi_MUTII_EFI.xml (2026-07-15), missed in the original 2026-07-13 build. Eval="5*x/8", clips at 160. |                                             | BLANK (untraced) |
| 1D | F18D | Airflow/Rev (AccelEnrich)                           | NAMED (untraced) |
| 1E | F20F | --                                                  | BLANK (untraced) |
| 1F | F189 | Load11Bit4                                          | NAMED (untraced) |
| 20 | F179 | --                                                  | BLANK (untraced) -- adjacent to 0x21, unlabeled |
| 21 | F17B | Engine RPM (per EFI profile)                        | REFUTED for this ROM -- confirmed to be engine-torque-derived scale value (engine_torque_pct_scale_calc), NOT RPM. See review.md item 7. |
| 22 | EED1 | --                                                  | BLANK (untraced) |
| 23 | F3F1 | --                                                  | BLANK (untraced) |
| 24 | F401 | Target Idle RPM                                     | CONFIRMED |
| 25 | F40B | --                                                  | BLANK (untraced) |
| 26 | F3A3 | Knock Sum                                           | CONFIRMED |
| 27 | EEC9 | Octane Level (OctaneFlag)                           | CONFIRMED |
| 28 | F291 | --                                                  | BLANK (untraced) |
| 29 | F970 | Injector Pulse Width                                | CONFIRMED |
| 2A | F971 | Injector Pulse Width low byte (word w/ F970)        | CONFIRMED (2026-07-15): resolved by reading fuel_pw_and_airvol_compute (0x29fba) directly instead of relying on xrefs. The function writes InjPulseWidth as a single 16-bit store at 0xf970 (@0x2a6c1) -- there is no separate byte-level write anywhere, confirming F971 is simply the low byte of the F970/F971 big-endian word, same idiom as F3FA/F3FB (0x08/0x09). injpw_airvol_reset_on_fuelcut (0x24680) corroborates: its fuel-cut reset zeroes F970 as one ushort store, never touching F971 independently. No separate signal here -- do not treat as distinct from F970. |
| 2B | F972 | Air Volume high byte (word w/ F973)                 | CONFIRMED (2026-07-15): resolved by reading fuel_pw_and_airvol_compute (0x29fba) directly. AirVol is written as a single 16-bit store at 0xf972 (@0x2a6d7) -- no separate byte-level write exists, so F972 (this ReqID) and F973 (ReqID 0x2C, already CONFIRMED as "Air Volume") are the high/low bytes of the SAME word, same idiom as F3FC/F3FD (0x0A/0x0B). injpw_airvol_reset_on_fuelcut (0x24680) corroborates: zeroes F972 as one ushort store. Resolves the prior open question about whether this is part of AirVol's word or a distinct 32-bit value -- it is the former; not a separate signal. |
| 2C | F973 | Air Volume                                          | CONFIRMED |
| 2D | F85F | --                                                  | BLANK (untraced) |
| 2E | F1E7 | --                                                  | BLANK (untraced) |
| 2F | F1E9 | Speed                                               | NAMED (untraced) |
| 30 | F15D | Knock Voltage                                       | CONFIRMED addr, writer NOT found (open, see below) |
| 31 | F297 | --                                                  | BLANK (untraced) |
| 32 | F2A5 | AFR Map (target AFR used by ECU)                    | NAMED (untraced) |
| 33 | F345 | --                                                  | BLANK (untraced) |
| 34 | F1FD | --                                                  | BLANK (untraced) |
| 35 | F2DB | --                                                  | BLANK (untraced) |
| 36 | EEEF | --                                                  | BLANK (untraced) |
| 37 | EEF3 | --                                                  | BLANK (untraced) |
| 38 | F15F | Boost (MAP)                                         | CONFIRMED |
| 39 | F161 | ADC channel #8 (physical sensor unidentified)       | CONFIRMED live (2026-07-15): adc_read_sequence_main (0x1556d) writes F160/F161 as a single 16-bit word via ADC channel #8 (`trap_hang(8)` call @0x5628, store @0x562c) -- distinct from the neighboring F15E/F15F word (channel #0xB, MAP/Boost, ReqID 0x38). Genuine, independent ADC-fed signal, not an artifact or shared cell. Physical sensor identity not yet determined -- channel #8 not cross-check| 3B | F15B | -- (structurally dead, always reads 0)              | CORRECTED 2026-08-08 (was "CONFIRMED DEAD" 2026-07-15, that label was WRONG): F15A/F15B (16-bit word) has exactly TWO touches in the entire ROM, both inside gear_state_config_loader_f1fc (0x20f28): it calls zero_var_f15a() (0x156c2, a one-line `DAT_0001f15a = 0`) and then immediately reads the same word back to derive a 3-bit TCU gear-config index into F1FA. No other function anywhere writes F15A between the zero and the read (confirmed via search_byte_patterns "F1 5A" -- only 2 hits total, both these). CORRECTION: decompiling EvoScan.exe (real Windows diagnostic tool for this ECU family, de4dot + ILSpy, see review.md) shows RequestID 0x3B is the FIRST of three MUT-II EFI DTC status bytes (0x3B/0x3C/0x3D) that EvoScan explicitly reads and decodes into the 14 classic MUT-II fault codes (Oxygen Sensor 11, Injector circuit 41, etc, see frmMain.cs method_201/method_60 call chain). "Always reads 0" in this ROM almost certainly means NO DTCs ARE CURRENTLY STORED (a healthy/no-fault state), NOT that the cell is dead or vestigial -- it's a real, meaningful, actively-read diagnostic status word, just correctly reporting "no faults" for whatever ROM/vehicle-state snapshot this is. Do NOT flag as unimplemented/vestigial; re-classify as a real DTC status cell, currently observed at its "no faults" value. Note EvoScan's own UI string ("valid for 1996 or Earlier Mitsubishi Vehicles ONLY") flags this specific 3-byte read as intended for pre-1997 vehicles -- this ROM is a 1998 RVR, so applicability to this exact ECU is not fully certain and worth keeping in mind, though the underlying MUT RequestIDs (0x3B/0x3C/0x3D) may still be shared across model years even if EvoScan's own UI only surfaces them for older vehicles. | 0 -- structurally dead for MUT-reading purposes, same category as the confirmed-dead MUT_83/0xF0BB cell. Do not log this RequestID; flag as unimplemented/vestigial rather than a real sensor. |
| 3C | F123 | Oxygen Sensor #2                                    | CONFIRMED |
| 3D | F125 | --                                                  | BLANK (untraced) |
| 3E | F121 | Knock Sum 3E (legacy pre-1998 Knock Sum)            | NAMED (untraced) |
| 3F | F5BD | --                                                  | BLANK (untraced) |
| 40 | EED7 | Alt Maps (dev-mod placeholder, undocumented)        | NAMED (untraced) -- first entry of the EED7-EEE9 cluster, adjacent to Load1B |
| 41 | EED9 | Load 1Byte (Load1B)                                 | OPEN -- producer not found, see below |
| 42 | EEDB | --                                                  | BLANK (untraced) |
| 43 | EEDD | --                                                  | BLANK (untraced) |
| 44 | EEDF | MAT Scaled (Manifold Air Temp Scaled)               | NAMED (untraced) |
| 45 | EEE1 | MAP Scaled                                          | NAMED (untraced) |
| 46 | EEE3 | --                                                  | BLANK (untraced) |
| 47 | EEE5 | --                                                  | BLANK (untraced) |
| 48 | EEE7 | --                                                  | BLANK (untraced) |
| 49 | EEE9 | AC Relay Switch (bit flag)                          | NAMED (untraced) |
| 4A | F45D | Crank/IdlePos/PwrSteering/ACSwitch/Inhibitor (combined switch-flags byte) | NAMED (untraced) |
| 4B | F2A7 | --                                                  | BLANK (untraced) |
| 4C | EEAF | --                                                  | BLANK (untraced) |
| 4D | EEB1 | --                                                  | BLANK (untraced) |
| 4E | EEB3 | --                                                  | BLANK (untraced) |
| 4F | F270 | --                                                  | BLANK (untraced) |
| 50 | F27F | Fuel Trim InUse (LTFT)                              | NAMED (untraced) |
| 51 | F283 | --                                                  | BLANK (untraced) |
| 52 | F287 | --                                                  | BLANK (untraced) |
| 53 | F28B | --                                                  | BLANK (untraced) |
| 54 | F2DD | --                                                  | BLANK (untraced) |
| 55 | F2DF | --                                                  | BLANK (untraced) |
| 56 | F1C9 | --                                                  | BLANK (untraced) |
| 57 | F1CD | --                                                  | BLANK (untraced) |
| 58 | F5BF | --                                                  | BLANK (untraced) |
| 59 | F21F | --                                                  | BLANK (untraced) |
| 5A | F22B | --                                                  | BLANK (untraced) |
| 5B | F22D | --                                                  | BLANK (untraced) |
| 5C | F127 | --                                                  | BLANK (untraced) |
| 5D | F129 | --                                                  | BLANK (untraced) |
| 5E | F237 | --                                                  | BLANK (untraced) |
| 5F | F239 | --                                                  | BLANK (untraced) |
| 60 | EE8C | --                                                  | BLANK (untraced) |
| 61 | EE8E | --                                                  | BLANK (untraced) |
| 62 | EE90 | --                                                  | BLANK (untraced) |
| 63 | EE92 | --                                                  | BLANK (untraced) |
| 64 | EE94 | --                                                  | BLANK (untraced) |
| 65 | EE96 | --                                                  | BLANK (untraced) |
| 66 | F233 | --                                                  | BLANK (untraced) |
| 67 | F235 | --                                                  | BLANK (untraced) |
| 68 | EEF1 | --                                                  | BLANK (untraced) |
| 69 | EEF5 | --                                                  | BLANK (untraced) |
| 6A | F3A9 | knock_adc_processed                                 | CONFIRMED |
| 6B | F967 | knock_base                                          | CONFIRMED |
| 6C | F963 | knock_var                                           | CONFIRMED |
| 6D | F969 | knock_change                                        | CONFIRMED |
| 6E | F3B7 | knock_dynamics                                      | CONFIRMED |
| 6F | EFC3 | knock_flag                                          | REFUTED -- confirmed throttle-transient detector, nothing to do with knock |
| 70 | F4BD | --                                                  | BLANK (untraced) |
| 71 | F1FF | --                                                  | BLANK (untraced) |
| 72 | F201 | Knock_72 (bit flags for knock)                      | NAMED (untraced) |
| 73 | F14D | --                                                  | BLANK (untraced) |
| 74 | F151 | --                                                  | BLANK (untraced) |
| 75 | F405 | --                                                  | BLANK (untraced) |
| 76 | F40D | --                                                  | BLANK (untraced) |
| 77 | F49D | --                                                  | BLANK (untraced) |
| 78 | F49B | --                                                  | BLANK (untraced) |
| 79 | F2CF | Injector Latency                                    | NAMED (untraced) |
| 7A | EEA7 | --                                                  | BLANK (untraced) |
| 7B | EEBD | --                                                  | BLANK (untraced) |
| 7C | EEA9 | --                                                  | BLANK (untraced) |
| 7D | EEBF | --                                                  | BLANK (untraced) |
| 7E | EEAB | --                                                  | BLANK (untraced) |
| 7F | EEC1 | --                                                  | BLANK (untraced) |
| 80 | F5C0 | --                                                  | BLANK (untraced) -- POINTER (2026-07-14 sweep): written inside channel_dispatch_and_snapshot_update (0x19d80). CONFIRMED as a 16-bit word, gear-indexed snapshot (read via current_gear_state_index @0xF1FC into a ROM table at 0x20c-offset), part of a run of five 16-bit snapshot words: F5C0 (this one, gear-table value), F5C2 (from 0x21C), F5C4 (from 0x21E), F5C6 (from 0x206), and a status/flags word F5C8 (bits set/cleared based on F0E6/F0EA). The real structure here is 16-bit-word-spaced (F5C0/C2/C4/C6/C8), NOT byte-spaced -- this table's own 0x81/0x82 rows (F5C1/F5C3, odd byte offsets) don't align with any field boundary found and are likely scraper artifacts, not real distinct RequestID targets. |
| 81 | F5C1 | --                                                  | BLANK (untraced) -- LIKELY SCRAPER ARTIFACT (2026-07-14 sweep): see 0x80/F5C0 finding -- the real structure at this ROM location is 16-bit-word-spaced (F5C0/C2/C4/C6/C8), so F5C1 (odd byte offset) does not align with any real field boundary found. |
| 82 | F5C3 | --                                                  | BLANK (untraced) -- LIKELY SCRAPER ARTIFACT (2026-07-14 sweep): same as 0x81/F5C1 -- odd byte offset does not align with the confirmed 16-bit-word structure (F5C0/C2/C4/C6/C8). |
| 83 | F0BB | --                                                  | BLANK (untraced) |
| 84 | F495 | RadFans (Cooling Fan Duty Cycle)                    | REFUTED (2026-07-26): F495 is the LOW byte of the 16-bit word F494 -- exhaustively traced this session as the EGR position/duty target written EXCLUSIVELY by egr_position_target_f494_calc (0x18bc2, the Timer-6-PWM EGR hardware variant, config 0x102e4==2, CONFIRMED ACTIVE on this ROM) and consumed EXCLUSIVELY by serial_fef9_duty_scale_and_set -> ff88_set_scaled_clamped -> T6GR1 (real H8 Timer 6 PWM compare register). No fan-related code touches F494/F495 anywhere. The GalantLegnum profile's "RadFans" label for RequestID 0x84 does not match this ROM -- likely a different platform/generation's RequestID map, or the profile is simply wrong for this cell. This IS a genuine, confirmed, live actuator value (EGR duty), just mislabeled. |
| 85 | F449 | EGR Duty Cycle 2 (secondary EGR output)             | CONFIRMED (2026-07-26): F449 is the LOW byte of F448, written by egr_target_f448_update (0x18650) and consumed by software_pwm_output_scheduler (0x158d7) -> P1DR bit3 (real port pin), gated ROM config 0x102de!=0, CONFIRMED ACTIVE on this ROM (0x102de=1). A SECOND, independent EGR duty output, coexisting with F494/ReqID-0x84's Timer-6-hardware-PWM EGR path. No prior name in any profile -- newly identified this session. |
| 86 | F44D | Wastegate Duty Cycle (WGDC)                         | CONFIRMED (name matches; PHYSICAL PIN ADDED 2026-07-26): writer chain wgdc_correction_integrator_update (0x18fe0) -> wgdc_output_clamp_f44c (0x19110) already confirmed. NEW this session: F44C is also consumed by software_pwm_output_scheduler (0x158d7) -> PADR bit1 (real port pin, 0xfe93.1) -- the first time in this whole investigation an already MUT-confirmed value has been traced all the way to a physical output pin. |
| 87 | F163 | --                                                  | BLANK (untraced) |
| 88 | F167 | --                                                  | BLANK (untraced) |
| 89 | F169 | --                                                  | BLANK (untraced) |
| 8A | F13F | Load Error                                          | NAMED (untraced) |
| 8B | F4AD | WGDC Correction (WGDCCorr)                          | REFUTED as WGDC-related, RENAMED (2026-07-26): F4AD is the LOW byte of F4AC, and F4AC is written EXCLUSIVELY by canister_purge_duty_calc_f4ac (0x19560) -- a CANISTER PURGE duty-cycle target, structurally and functionally unrelated to WGDC/F44C (different writer function, different subsystem, no shared inputs found). This resolves the long-open "relationship to WGDC not determined" question definitively: there is no relationship. The profile's "WGDCCorr" label is wrong for this cell -- should be read as Canister Purge Duty Cycle 2 (a second, duty-modulated purge output, alongside the already-traced simple on/off purge at F0E6 bit2 / SCI1 cmd 0xD1). NOTE: F4AC/F4AD's own physical hardware sink is STILL untraced -- checked this session whether software_pwm_output_scheduler reads it (it does not); the real output path remains unknown. |
| 8C | F84F | --                                                  | BLANK (untraced) |
| 8D | EEFB | --                                                  | BLANK (untraced) |
| 8E | F4DF | --                                                  | BLANK (untraced) |
| 8F | F4DD | --                                                  | BLANK (untraced) |
| 90 | F4E5 | --                                                  | BLANK (untraced) |
| 91 | F4E7 | --                                                  | BLANK (untraced) |
| 92 | F4EB | --                                                  | BLANK (untraced) |
| 93 | F4DB | --                                                  | BLANK (untraced) |
| 94 | F0BB | --                                                  | BLANK (untraced) -- duplicate address of ReqID 0x83, worth checking if intentional |
| 95 | F0BB | --                                                  | BLANK (untraced) -- duplicate address of ReqID 0x83/0x94 |
| 9A | ??? | FuelPumpSpeed (Fuel Pump Hi/Low, dual-speed bit)     | NAMED (untraced) -- source: GalantLegnum profile. NOT in the base 150-entry 0x2fad0 table scraped for this file (table stops at 0x95) -- address unknown, needs its own lookup/derivation before this row can be trusted as belonging to the same RequestID scheme. |

Tally: 56 of 150 entries have SOME name attached (from CONFIRMED/REFUTED/
OPEN work or an EFI profile -- base Evo profile or the GalantLegnum
cross-check); of those, only 24 have actually been independently verified
(CONFIRMED/REFUTED/OPEN-with-partial-trace). That leaves 94 entries with
literally no name anywhere, and ~32 more that have an inherited EFI-profile
name but have never been checked against this ROM's live disassembly --
remember 0x21 (Engine RPM) and 0x6F (knock_flag) both looked equally
credible before they were actually traced and turned out wrong. Treat
every NAMED (untraced) row the same way: a lead, not a fact. This also
applies to the two new GalantLegnum-sourced rows (0x84 RadFans, 0x9A
FuelPumpSpeed) and the 0x00 conflict flag -- a second EFI profile agreeing
or disagreeing is a useful signal, not proof either way. Plus a 151st row
(0x9A) added below the table for a RequestID outside the original 0x00-0x95
scrape range -- its address is not yet known and needs separate lookup.

Priority follow-ups suggested by this table (clusters worth checking first,
since contiguous addresses tend to be a related family, as the knock
cluster and AirVol/InjPulseWidth pairs already showed):
- EED7-EEE9 (ReqID 0x40-0x49): Alt Maps, Load1B, 3 blank, MAT Scaled, MAP
  Scaled, 2 blank, AC Relay Switch -- mixed known/unknown cluster right
  next to the open Load1B question item 1 below.
- EE8C-EE96 (ReqID 0x60-0x65): six consecutive blanks, tight spacing --
  likely a per-cylinder or per-gear array, completely uninvestigated.
- EEA7-EEC1 (ReqID 0x7A-0x7F): six consecutive blanks, adjacent to Octane
  Level (EEC9) -- possibly part of the octane/knock retard pipeline.
- F4Dx-F4Ex-F84F (ReqID 0x8C-0x93): eight blanks near the WGDC/WGDCCorr
  family (F44D/F4AD) -- possibly a third WGDC-pipeline stage.
- ReqID 0x83/0x94/0x95 all resolve to the SAME address F0BB -- worth
  checking whether that's intentional (a status/flags byte read three
  ways) or a scraper/table artifact.

MUT Verification Status
========================
Extracted from test/rvr/logging.txt (session 10, 2026-07-03, written under
the OLD/buggy pre-MAP3-6 Sleigh grammar) and re-verified against the
CURRENT live disassembly/decompile during the 2026-07-12/13 sessions.

Source file test/rvr/logging.txt is safe to delete once this file and the
plate comments described in review.md item 6/7 have captured everything
needed from it.

RPM identity is tracked separately (see review.md item 7 / the isr_sci3_eri
lead) and is intentionally NOT included here -- this file covers everything
else that was re-verifiable independent of that open question.

CONFIRMED (address + semantics both verified against live disassembly)
------------------------------------------------------------------------
- TPS (0x17 -> RAM 0xF13D): writer adc_read_sequence_b (0x15689), channel 7,
  16-bit word at 0xF13C/F13D. ~20 consumers gated on throttle position
  confirm semantics, not just address.
- Battery (0x14 -> RAM 0xF13B): same function/pattern as TPS, channel 5,
  word at 0xF13A/F13B.
- AirTemp (0x3A -> RAM 0xF117), MAP/Boost (0x38 -> RAM 0xF15F): writer
  adc_read_sequence_main (0x1556d), even-word/odd-byte pattern.
- O2 Sensor (0x13 -> RAM 0xF11F): writer adc_read_sequence_main channel 3;
  consumer o2_closed_loop_fuel_trim_compute (0x237a0) reads it directly as
  narrowband O2 input to closed-loop fuel trim.
- O2 Sensor 2 (0x3C -> RAM 0xF123): writer adc_read_sequence_main channel 4;
  consumer o2_sensor2_threshold_flag_update_f226_bit13 (0x22f16) sets/clears
  bit13 of the F226 "lambda mode" register.
- Octane Level (0x27 -> RAM 0xEEC8/EEC9): this IS the "knock retard"
  mechanism on this ECU. Chain: knock sum (F3A2/F3A3) ->
  knock_event_counter_update_eec6 (0x251c4) -> octane_level_compute_from_
  knock_counter (0x252d3, writes EEC8/9). Separate init/override path:
  octane_level_init_or_force_from_gear_table (0x24e80), caller not yet
  found.
- TargetIdleRPM (0x24 -> RAM 0xF400/F401): writer idle_target_rpm_compute_
  f400 (0x2629b). Coolant-temp-indexed table, A/C flag (F0F8 bit0x20)
  selects table pair, floor clamp + A/C trim offset.
- AirVol (0x2C -> RAM 0xF972/F973) and InjPulseWidth (0x29 -> RAM
  0xF970/F971): both written by fuel_pw_and_airvol_compute (0x29fba).
  Secondary reset function injpw_airvol_reset_on_fuelcut (0x24680) zeroes
  both under a fuel-cut condition (F25A bit0x28).
- ISCSteps (0x16 -> RAM 0xEED4/EED5): writer isc_stepper_output_state_
  machine_eed4 (0x26f82), closed-loop idle stepper state machine.
- WGDC (0x86 -> RAM 0xF44C/F44D): writer chain wgdc_correction_integrator_
  update (0x18fe0) -> wgdc_output_clamp_f44c (0x19110). RESOLVED 2026-07-26:
  physical output pin found (software_pwm_output_scheduler -> PADR bit1),
  and the "relationship to WGDC" question for 0x8B/F4AD is now CLOSED --
  there isn't one. F4AD/F4AC is canister purge duty (canister_purge_duty_
  calc_f4ac), a completely separate subsystem. See the ReqID 0x86/0x8B
  table rows above for full detail.
- TCU Shift Torque Cmd Lo/Hi per gear (ROM 0x1328e/0x13296): gear-indexed
  ((gear&3)<<1), read into RAM 0xF39E/F3A0, used as a raw SCI3 protocol
  command byte to the TCU -- never used arithmetically. Small values (1-2)
  are expected, not a data error.
- Knock Sum (0x26 -> RAM 0xF3A3), Knock Voltage (0x30 -> RAM 0xF15D),
  knock_adc (0x6A -> F3A9), knock_var (0x6C -> F963, holds a DERIVED
  decision magnitude not raw variance), knock_base (0x6B -> F967, holds a
  snapshot of the PREVIOUS cycle's retard output F3AA), knock_change
  (0x6D -> F969, delta current/previous), knock_dynamics (0x6E -> F3B7,
  persistence counter, sets F200 bit0 under sustained knock).
  NOTE: 2026-07-12 re-verification of Knock Voltage's *write* side found
  the read location confirmed (inside tcu_shift_torque_and_knock_mgmt) but
  NO writer under bank prefixes 0x0-0x3 -- flagged in Ghidra, needs further
  investigation (possible bank prefix >3, computed/indirect write, or not
  runtime-written at all).
- Coolant Temp (0x07 -> RAM 0xF109) / CoolantTempScaled (0x10 -> RAM
  0xF10F): address arithmetic only, NOT independently traced. 2026-07-12
  re-check: no writer found under bank prefixes 0x0-0x1 either -- see the
  four-way conflict in Open Items below.

REFUTED / MISLABELED
------------------------------------------------------------------------
- knock_flag (0x6F -> RAM 0xEFC2/EFC3): NOT knock-related. CONFIRMED
  mislabel, re-verified 2026-07-12. Both writers (efc2_threshold_update
  @0x252f9, and a second writer inside tcu_rx_main_scheduler @0x2b676) are
  throttle-position-delta detectors (TPS-now vs TPS-previous, @0xF13C/
  F142) -- genuinely nothing to do with knock. Inherited from the stock
  Evo profile.

MAJOR STRUCTURAL FIX: knock + TCU shift torque is ONE function
------------------------------------------------------------------------
What was called "knock_process_main" (0x2933b) and "tcu_shift_torque_mgmt"
(0x28fff) were never two coordinating functions -- Ghidra had mis-split ONE
real function into four pieces across a 2-byte inline data literal
(0x186a @ ROM 0x29339-0x2933a) left unresolved since session 3. Fixed by
marking that literal as data and merging all four pieces into one function,
now named tcu_shift_torque_and_knock_mgmt (ROM 0x28fff-0x29c32). Fixing the
boundary surfaced its real caller: isr_sci3_eri (COMPUTED_CALL @ 0x169b1).

f3a8/f3aa/f3ac/f3ae/f3b0/f3b2 form a shared "Knock-to-TCU Torque
Coordination" register cluster, gated by RAM 0xf1f2 bit7
(gear_config_word_bit7_tcu_knock_coord), loaded per-gear via
current_gear_state_index (RAM 0xf1fc) from gear_config_table_f1f2 (ROM
0x272). Practical meaning: knock-informed torque-cut coordination with the
TCU is only enabled in certain gears. The specific ROM calibration table
addresses previously guessed for this cluster were REMOVED from the
MT-specific XML (never independently traced, several overlapped Warmup
Retard's real byte range) -- the RAM register finding survives, those
specific addresses do not.

Action needed (2026-07-13): the tcu_shift_torque_and_knock_mgmt boundary
fix (0x28fff-0x29c32) and its isr_sci3_eri caller xref should be
spot-checked against the current live listing before further work builds
on this -- see review.md item 6.

TUNING XML CLEANUP: speculative table block removed
------------------------------------------------------------------------
The MT-specific XML's "UNKNOWN / PARTIALLY IDENTIFIED TABLES" block (~60
tables) was built from data-shape pattern guessing without cross-checking
against tables already defined elsewhere in the file. Verification: (1)
byte-range overlap check found 3408 collisions concentrated in this block
(e.g. High Octane Ignition Map 1 fully contained four of the "unknown"
tables); (2) a 28-address Ghidra xref sample found zero addresses used as
an independently-addressed lookup table anywhere in the code. REMOVED the
entire block from 21000011_1997-2001_RVR_X3_Mt__4g63t_.xml; full text
preserved separately in logging_session10_removed_tables.xml for reference
only (not for re-inclusion without independent verification). Overlap
count after fix: 3408 -> 913 (remainder expected/benign: MT file
legitimately overriding base-file tables, shared axis arrays).

RETRACTED DEAD ENDS (do not re-investigate without new evidence)
------------------------------------------------------------------------
- isr_ram_tpu_crank @ RAM 0xF9BA: not real code, bytes undefined; suspected
  boot_ram_block_copy source (0x1F290) is just uninitialized RAM.
- isr_adi_adc @ ROM 0x163cc: resolved dead vector, calls a one-line no-op
  stub. Not the knock sampling path.
- 0x3898[chan] dispatch table: decoded as a generic ADC/DMA transfer
  descriptor state machine (init/start/poll/ready/done) underlying the
  already-known adc_read_sequence_main/b channel reads.
  NOTE (2026-07-12 re-check): no function and no xrefs currently exist at
  0x13898 (review.md item 3's flagged unresolved address table) -- NOT
  confirmed this is the same address as this "0x3898" shorthand; treat as
  independent until the dispatch-table structure is re-derived against the
  current live listing.
- isc_integrator_update (0x17dd3): the one speculative table address with
  any xref at all ("Lambda Fuel Trim vs O2 A", 0x11412) turned out to be
  read as a single scalar threshold byte, not a 42-element table lookup --
  does not rescue that table definition.

Open items (non-RPM)
------------------------------------------------------------------------
1. Load1B (0x41 -> RAM 0xEED8/EED9): producer not found. Two writers
   located are both RESET/init paths (startup_phase_reset_eed6_cluster,
   startup_phase_reset_eed6_eefa_bulk), not the per-cycle compute. Since
   Load1B is described as MAF/RPM-derived like AirVol, check whether
   fuel_pw_and_airvol_compute (0x29fba) produces it further down, or
   whether a sibling load-calc function exists.
   [UPDATE 2026-07-15] Still no producer found, but the trigger chain for
   startup_phase_reset_eed6_eefa_bulk is now fully traced and it is NOT
   startup-only: sci1_meta_cmd_dispatch_c0_ff command 0xFC (SCI1) sets
   @0xf516 bit7 -> eee0_eeee_diag_flags_reset_dispatch (0x282c1) reads that
   bit and calls startup_phase_reset_eed6_eefa_bulk on demand. So Load1B
   (and the whole EED6-EEFA cluster) can be force-reset live over SCI1, not
   just at boot. Does not resolve the producer question, but changes the
   read-safety picture slightly: if this dispatcher is ever reachable from
   the MUT/SCI3 path, a stray 0xFC-equivalent command would zero this
   whole cluster rather than just fail to answer a read. No evidence yet
   that it IS reachable from SCI3 (see dispatcher section above).
2. WGDC (0xF44C/F44D) vs WGDCCorr (0xF4AD) relationship -- CLOSED
   (2026-07-26): there is no relationship. F4AD's profile label
   "WGDCCorr" is wrong -- it's canister_purge_duty_calc_f4ac's output
   (canister purge duty cycle), a different subsystem entirely from WGDC.
   See ReqID 0x8B row above.
3. Coolant Temp four-way address conflict: RVR_base.xml (F0C4/F0C8), MUT
   profile (F109/F10F), and a fourth independently-traced ADC path
   (F84E->F12E->F130->F29A, via coolant_temp_validity_and_scale, 0x2142c)
   all claim to be coolant temp; none overlap, none reconciled. Do not
   treat any as authoritative.
4. gear_config_table_f1f0 (ROM 0x262) and gear_config_table_f1f4 (ROM
   0x282) -- two of the three per-gear config words loaded alongside f1f2 --
   not yet decoded. Also unknown: which specific gear index/indices have
   f1f2 bit7 set in ROM table 0x272.
5. o2_downstream_clamp_by_type's exact transform not re-derived. f1fe
   bit3, f200 bit0, f0fe bit0 (force-zero/force-max conditions in the
   octane level chain) not decoded.
6. AirFlowMUT2Byte (RequestID 0x04/0x05 -> RAM 0xf343/0xf363): NOT
   contiguous, unlike other 2-byte pairs in the profile -- hi/lo byte
   stitching assumption unconfirmed, verify before trusting logged values.
7. General backlog: ~140 "Bad Instruction" bookmarks remain, clustered
   around 0x10000-0x10090, 0x14000-0x14e10, and scattered singles. The
   tcu_shift_torque_and_knock_mgmt fix came from investigating exactly one
   of these and found a real mis-split function with a hidden caller --
   treat "Bad Instruction adjacent to a zero-xref function" as a priority
   pattern when working this backlog.

SLEIGH DECODE BUG -- RESOLVED, historical note only
------------------------------------------------------------------------
logging.txt (written under the OLD pre-project-fix Sleigh grammar) recorded
six "Unable to resolve constructor" Bad Instruction sites (0x142a6, 0x142dc,
0x14349, 0x14379, 0x143a7, 0x1440a), all byte-for-byte identical at the
failing instruction (opcode bytes `A0 16`, Ghidra's partial-decode guess
read it as `tst.b R0`).

STATUS (2026-07-13, per user): this was a bug in the old grammar only. The
user has since updated the Sleigh implementation and confirms all such
decode bugs are gone in the current live Ghidra project. No action needed
here -- this is retained purely as a historical note explaining an old
logging.txt entry, not as an open item. If any "Unable to resolve
constructor" bookmarks are seen again in the live project, they should be
treated as new findings, not a recurrence of this one, and verified fresh
against the current .sinc files rather than assumed to be the same bug.

PRE-EMPTIVE SWEEP OF BLANK/NAMED-UNTRACED ADDRESSES (2026-07-14)
------------------------------------------------------------------------
Ran ghidra:get_bulk_xrefs across ~110 addresses covering every BLANK and
several NAMED(untraced) rows in the master table above, to surface any
address that's actually touched by live code -- a by one function, renamed 2026-07-14 to f26e_cluster_accumulator_unrelated_to_ihex
(formerly misleadingly named sci1_boot_ihex_data_byte_store)e relevant rows above
(F26E, F971/F972, F5C0/F5C1/F5C3). Also found in passing: RAM addresses
F270, F272, F274, F275, F276, F278, F27A, F27C, F27E, F281 are all touched
by one function currently misleadingly named sci1_boot_ihex_data_byte_store
(0x20782) -- despite the name, this function's body has nothing to do with
SCI1 or Intel-hex boot loading; it looks like an accumulator/threshold
tracker unrelated to its name. F270 is a BLANK MUT row (ReqID unlisted in
this sweep's scope but present in the table above) written here. Rename
candidate, not yet renamed -- caller (0x20747) is itself NOT inside any
currently-defined function, which is its own small gap worth a bookmark.

CAVEAT ON THIS SWEEP'S COVERAGE: the overwhelming majority (~95+ of ~110)
of addresses checked showed ZERO xrefs via get_bulk_xrefs. Per the
decompiler-reliability note earlier in this file and review.md's repeated
findings (Knock Voltage, Coolant Temp, several others), static xref
tracking on this ROM has a KNOWN BLIND SPOT for bank-prefixed/indirect RAM
access -- a zero-hit result here is NOT proof an address is unused, only
that it isn't referenced via a directly-encoded operand Ghidra's xref
engine caught. Do not mark any zero-hit BLANK row as "confirmed dead"
based on this sweep alone (contrast with the F0BB case in the "NEW
FINDING" section above, which WAS confirmed dead, but only after a raw
byte-pattern search across the whole ROM, filtered for false positives --
a much more expensive check than bulk xrefs, not repeated here for all
~95 zero-hit addresses due to volume). Treat every zero-hit row as
"not yet found by this cheap method," not "unused." The five findings
above (F26E, F971/F972 adjacency, F5C0 cluster, F270 cluster) are the
sweep's actual yield; everything else in the BLANK/NAMED-untraced rows
remains exactly as uninvestigated as before this pass.

## Note:

so some of the request id's could be actuator tests

Look at what's already in the file: the Mode5 (Actuator Tests) block uses RequestID values like 01, 02, 03, 04, 05, 06, 17, 1A — and those exact same byte values already exist in the Mode2 (Data List Items) block as sensor reads (e.g. 01 is part of LoadMUT2Byte's pair in Mode2, but is "Fuel Pump Relay" in Mode5; 17 is Throttle Position in Mode2, but "Ignition Timing Fix" in Mode5).

That's not a conflict — it's how the real MUT protocol works. EvoScan sends a Mode byte first (2 = data list read, 5 = actuator test, etc.), and the RequestID byte only means something within that mode. So the same byte 0x17 means "read throttle position" under Mode 2 and "force timing to 5° BTDC" under Mode 5 — two completely separate command families, almost certainly dispatched by different code paths in the ECU (Mode 2 → the adc_sensor_convert_single / MUT-table read path we've fully traced; Mode 5 → whatever handles the actual relay/solenoid/output commands).

A few implications:

The ~87 new BLANK entries added are all Mode 2 requests (data reads), so they go through the confirmed read-only dispatch chain — logging them shouldn't trigger anything physical. That's a different risk category from Mode 5.
The existing Mode5 rows' Notes are probably wrong/misleading. They currently carry the same "[RAM CONFIRMED via adc_sensor_convert_single + MUT Table @ 0x2fad0]" annotation as the Mode2 rows — but that's the Mode 2 read table, and there's no evidence anyone actually traced Mode 5's dispatch function to confirm it uses the same table or the same addresses. That looks like it was pasted across uncritically.

REAL COMMAND DISPATCHER FOUND (2026-07-14) -- separate from the MUT table entirely
------------------------------------------------------------------------
This resolves the open question directly above: the dispatcher IS separate,
and part of it IS live actuator control, not just a Mode 5 theory.

Traced upward from the SCI1 receive path (isr_sci1_eri -> ... ->
sci1_protocol_state_machine @0x287af -> sci1_dispatch_and_latch_response
@0x2882b -> sci1_meta_cmd_dispatch_c0_ff @0x28869). This last function is a
genuine COMMAND-BYTE dispatcher, and it is COMPLETELY SEPARATE from
adc_sensor_convert_single (0x171c3) and the 150-entry MUT table at ROM
0x2fad0 that this whole file has been built around. Two different
mechanisms, two different tables, confirmed by direct disassembly:

- Command byte < 0xC0 (0xbf boundary check at 0x28870): reads a WORD from a
  ROM pointer table at ROM 0x2530 -- `mov:g.w @(-0x530:16,R0),R1` with DP=2,
  R0 = command_byte<<1 -- then dereferences that pointer and returns a byte.
  This is a genuinely DIFFERENT table from 0x2fad0. Not yet mapped at all.
  This may be the real Mode-2-equivalent data-read path, or something else
  entirely -- UNKNOWN, not yet traced further per user instruction to stop
  here.

- Command byte 0xC0-0xFF: explicit CASE-BY-CASE handling, each case doing
  direct bit manipulation on what look like hardware/actuator control
  registers -- NOT reads from any lookup table. Concrete examples seen in
  the disassembly:
    0xC3 -> bset.w @0xf512:16, bit 6
    0xC4 -> sets bits in @0xf516:16 (or.w 0x140)
    0xD9 -> bset.w @0xf510:16, bit 0
    0xFA -> bset.w @0xf516:16, bit 2 (conditionally, then bit 5 or error)
    0xFB -> bset.w @0xf516:16, bit 6
    0xFC -> bset.w @0xf516:16, bit 7
    0xCA/0xCB/0xF3/0xF8/0xF9 and the 0xEC-0xEF, 0xFD-0xFF range: various
    reads/writes of @0xf50e, @0xf594, @0xf596, @0xf1fc (gear index), and a
    small 4-byte struct at ROM 0x232 indexed by (0xec-0xef minus 0xec).
  F516, F512, F510 are set/cleared bit-by-bit per command byte in a pattern
  that looks like direct control-flag manipulation (each command sets
  exactly one bit, no arithmetic/scaling) -- structurally consistent with
  actuator/relay/solenoid TRIGGER flags, not sensor data. NOT yet confirmed
  which physical actuator (if any) each bit maps to -- that would need
  tracing what reads F516/F512/F510 elsewhere and what hardware those
  reads control (timer/PWM channel enable, port bit, etc.) -- NOT DONE,
  stopping here per user instruction.

WHAT THIS MEANS FOR THIS FILE'S SCOPE: everything in this file (the
150-entry table at 0x2fad0, all CONFIRMED/OPEN/BLANK rows above) is
downstream of adc_sensor_convert_single, which this dispatcher never calls
for command bytes 0xC0-0xFF. In other words: the MUT table this whole file
tracks is READ-ONLY telemetry, structurally isolated from the actuator-like
command range found here. Reading/logging any RequestID in the table above
does NOT appear to risk triggering the 0xC0-0xFF actuator-style commands --
those are a different byte, dispatched by a different function, gated by a
different comparison. This is a reassuring structural finding for the
"is it safe to log all 150 entries" question, but it is NOT a substitute
for actually confirming what EvoScan sends on the wire for this specific
profile -- if the RVR MUT profile's Mode5/actuator-test block ever gets
used, it almost certainly routes through THIS dispatcher (0x28869), not
through adc_sensor_convert_single, and those rows' "[RAM CONFIRMED via
adc_sensor_convert_single...]" annotations (wherever they exist in the
tuning XML) are WRONG and should not be trusted -- flagged, not yet fixed.

NOT YET DONE (stopping here per user instruction, 2026-07-14):
- The <0xC0 ROM-0x2530 pointer table itself: unmapped, no entries decoded.
- Which physical hardware F516/F512/F510 bits 0-7 each control: unknown.
- Whether the RVR MUT profile's actual Mode5 RequestID values (01-06, 17,
  1A, etc. per the note above) correspond 1:1 with this dispatcher's
  0xC0-0xFF range, or with the <0xC0 pointer-table range, or with neither
  (e.g. mode byte routing could happen even earlier, upstream of this
  function) -- NOT confirmed. Do not assume a mapping between the "Mode 5"
  RequestID bytes documented elsewhere and the C0-FF command bytes seen
  here without further tracing.
- Whether sci1_meta_cmd_dispatch_c0_ff is reachable from SCI3 (the TCU/MUT
  logging port used elsewhere in this file) at all, or only from SCI1 --
  the call chain traced above is entirely SCI1. If MUT logging in practice
  goes over SCI3, this dispatcher may not even be reachable that way -- not
  confirmed either way.

UPDATE 2026-07-15 -- dispatcher table fully decoded, two consumers found
------------------------------------------------------------------------
NOTE FIRST: none of this resolves a MUT table entry above. This dispatcher
remains structurally separate from the 0x2fad0 MUT table (see previous
section) -- Mode5 RequestID bytes are all <0x20, this command range is
0xC0-0xFF, no overlap found. Logged here as dispatcher/actuator-path
progress only.

- The 0xC0-0xD8 table (ROM 0x13740, labeled cmd_c0_d8_actuator_bit_table)
  is now fully read and decoded (20 entries). It's a clean one-bit-per-
  command sweep: idx byte selects @0xf512 (idx=1) or @0xf510 (idx=0) via a
  computed address, mask word sets one bit. Covers f512 bits 0-4,6 and
  f510 bits 0-13. C0-C2/C5/CC are stubbed (mask=0, unimplemented/reserved).
  CA/CB have real table entries but are unreachable (intercepted earlier,
  always error).
- Two real downstream consumers found via xrefs to f516 (f510/f512 have
  NO reader anywhere yet found):
  - f516_request_latch_f09a_calib_select (0x27e1a, renamed from
    eeea_bit7_f09a_update): consumes f516 bits 2/5/6/8 (set by dispatcher
    commands C3/C4/FA/FB), confirms they're one-shot request flags -
    latches via @0xeeea bit7 and selects output register f09a between two
    calibration values (f050 vs f052).
  - f516_hibits_f520_f0f2_mode_select (0x28700, renamed from
    f520_f0f2_f0f4_mode_select): consumes f516 bits 11/12/13/15 into a
    mode value written to f520/f0f2/f0f4 -- but NO WRITER for these bits
    has been found anywhere, including in the dispatcher's own table.
    Likely another command range/dispatcher uses the same computed-
    address trick and lands on these bits -- NOT YET LOCATED.
- Per-gear tables read by the dispatcher's 0xEC-0xEF/0xFE-0xFF range are
  now dumped and labeled: ROM 0x232 (gear_indexed_4byte_struct_ec_ef,
  4 bytes/gear, byte0 constant across gears, byte3 gear-unique) and ROM
  0x252 (gear_indexed_word_table_fe_ff, 2 bytes/gear, mostly a repeated
  value with two gear indices differing). Both look like calibration
  constants, not flags -- physical meaning still undetermined.
- Plate comments updated on all three functions (0x28869, 0x27e1a,
  0x28700) with the above.

- FC's real target found: @0xf516 bit7 is consumed by
  eee0_eeee_diag_flags_reset_dispatch (0x282c1), which on-demand calls
  startup_phase_reset_eed6_eefa_bulk (0x28413) - the same bulk-reset
  function already known as one of Load1B's two writers (see item 1
  above). This means command 0xFC is a LIVE trigger for that reset, not
  just a startup path -- promoted to a concrete finding, see item 1's
  2026-07-15 update. Same function also clears f516 bit1, setter unknown.

STILL NOT DONE: writer for f516 bits 1/12/13/15; any reader for f510/
f512; physical hardware meaning of any bit; the <0xC0 pointer table;
SCI1-vs-SCI3 reachability (now higher priority given the 0xFC live-reset
finding).

RESOLVED 2026-07-15: f516 bit11's writer found -
f588_duty_gate_f516_bit11_set (0x28b62, renamed from FUN_00028b62),
confirmed clean in disassembly (`bset.w @0xf516:16, 0xb`). It's a
computed-jump branch target living inside sci1_periodic_phase_dispatch_f526
(0x28b2f, renamed from FUN_00028b2f, called from sci1_protocol_state_machine
0x287af) - gated on a duty/period counter (f588 vs f58c) and sets bit11
when it elapses, also advancing a phase counter f526 conditionally on
@0xf102 bit5. A sibling branch, f528_f52e_zero_f526_advance_alt_branch
(0x28b89), handles the opposite f102-bit5 case and zeroes f528/f52a/f52c/
f52e instead. So bit11 is NOT set by the SCI1 command dispatcher at all -
it's set by an independent periodic phase-sequencer. This is consistent
with f516_hibits_f520_f0f2_mode_select treating bit11 as a "disable/off"
mode signal.
CAVEAT: sci1_periodic_phase_dispatch_f526 (0x28b2f) itself has real
decompiler trouble (Bad Instruction warnings, likely an unresolved jump
table mis-splitting the function) - flagged as a Bad-Instruction-bookmark
candidate, not yet properly re-split. The two bit11-related branch targets
decompile cleanly and are trustworthy; the parent's case 0/2/6 paths are
not, and its case 7 (which calls into fueling/lambda functions) is
unrelated background-task content, not confirmed reliable either.
Bits 1/12/13/15 remain open - not resolved by this finding.

UPDATE 2026-07-23: bits 13/15 writer found separately - see
phase_cases_1to6_f516_hibit_f54a_writer (0x28b89) case 2, which sets bit9
alongside either bit15 (f528==0x0200) or bit13+advance (f528==0x0220). Not
related to the SCI1 command dispatcher, purely an internal duty/phase state
machine (same family as bit11's writer above).

Bits 1/12: EXHAUSTIVELY CHECKED 2026-07-23, no writer exists anywhere in
this ROM. Pulled the complete xref list to RAM 0xf516 via Ghidra
(get_xrefs_to) - all 10 references, covering every known reader/writer
(sci1_meta_cmd_dispatch_c0_ff, phase_cases_1to6_f516_hibit_f54a_writer,
f588_duty_gate_f516_bit11_set, peripheral_fec0_fed0_state_update). None
sets bit1 (0x2) or bit12 (0x1000) anywhere. peripheral_fec0_fed0_state_update
only reads f516 (gates on bits 12-15==0) and clears bit9, contrary to an
earlier assumption it might be a bit1/12 writer candidate - it isn't.
The one remaining loose thread, case 7 at 0x28cc8 (previously flagged in
sci1_periodic_phase_dispatch_f526's plate comment as "a valid jump target
via xref, not yet disassembled"), was re-checked and is currently undefined
data with ZERO xrefs - that plate-comment claim appears to be stale/
unconfirmed, not a live branch, so it isn't hiding a writer either.
CONCLUSION: bit1 (cleared by eee0_eeee_diag_flags_reset_dispatch, see FC
note above) and bit12 (read by f516_hibits_f520_f0f2_mode_select) are
write-only-from-nowhere on this ROM - most likely dead flags from a removed
feature or a different ROM revision, not a gap in analysis coverage. Not
worth further search time on this ROM; only reopen if a fresh
disassembly/analysis pass changes the xref set at 0xf516, or if the w4a51
ROM (item 12 in review.md) shows a different picture.

MUT BLANK/UNTRACED SWEEP, ROUND 2 (2026-07-15 session, in progress)
------------------------------------------------------------------
Goal: work through the ~150 BLANK/NAMED-untraced rows, and separately
identify which confirmed cells are true MUT "requests" (sensor/computed
values a scan tool reads) vs "actuators" (values that drive/target real
hardware output - stepper, duty cycle, solenoid).

APPROACH: individual per-address get_bulk_xrefs on BLANK cells mostly
returns zero hits (same indirect/bank-prefixed blind spot documented
throughout this file) - re-confirmed on EEEF/EEF3/F161/F15B this session,
no new information there. Switched to searching from the PRODUCER side
instead: enumerated all *_calc/*_duty_*/*_target_* functions
(ghidra:search_functions name_pattern "calc") and cross-checked their
write addresses against the master table, since a MUT-readable cell is
written by some real function even when static xrefs to the address
itself come back empty.

CONFIRMED THIS ROUND:
- ReqID 0x08 (EECA) and 0x0A (EECC): see updated table rows above.
  Both are mode-shadow copies of the same ISC correction value from
  isc_eeca_eecc_eece_correction_calc (0x27a81). Plate comment added.

PATTERN IDENTIFIED (useful for future BLANK entries): a recurring
"mode-select shadow copy" idiom - a single computed correction/target
value gets written to one of 2-3 candidate RAM cells depending on
F0F8 bit4, F0F8 bit0, and byte@0x2c7. Seen in both
isc_eeca_eecc_eece_correction_calc (EECA/EECC/EECE) and a second,
not-yet-fully-logged instance in the F3FA/F3FC/F3FE ISC pipeline
(isc_f3fa_f3fc_f3fe_correction_calc, 0x27990, same F0F8/0x2c7 gating).
If a BLANK row's address sits 2-4 bytes from a CONFIRMED cell AND that
CONFIRMED cell's writer contains F0F8-bit/0x2c7-style mode branching,
check the sibling addresses in that same writer function before
concluding "no writer found."

ACTUATOR-SIDE CONTEXT GATHERED (not MUT table entries, but useful
reference for the requests-vs-actuators question):
- F4AC (canister purge duty cycle): confirmed via canister_purge_duty_calc_f4ac
  (0x19560) - genuine PWM-style duty output, lookup-table + threshold-select
  driven. Not itself a MUT ID.
- F448 (EGR duty target): confirmed via egr_duty_target_calc (0x186fa) ->
  egr_target_f448_update (0x18650) caller. Not itself a MUT ID.
- F402 (ISC stepper target, feeds the EECA/EECC/F3FA-family mode-shadow
  copies above): confirmed via isc_f402_stepper_target_calc (0x2725d) -
  lookup-table base + F42A + F43A correction terms, saturated. Not itself
  a MUT ID but sits directly upstream of ones that are.
These three establish the general actuator-driver shape (lookup table ->
correction terms -> saturate -> store) to help classify further BLANK
entries as request-vs-actuator once traced.

RESOLVED (2026-07-15, same session): F3FB (0x09) and F3FD (0x0B) checked
against the F3FA/F3FC/F3FE mode-shadow triplet flagged above. Confirmed
these are REAL low-byte reads, not scraper artifacts -- different outcome
than the F5C0 cluster's 0x81/0x82 rows. Reasoning: adc_sensor_convert_single
(0x171c3), the actual MUT byte-fetch routine, does `mov:g.b @R1+,R0` +
`extu.b` for ReqID<=0xBF -- i.e. reads exactly ONE byte at whatever address
the 0x2fad0 table decodes to for that ID, no width ambiguity. Since
isc_f3fa_f3fc_f3fe_correction_calc only ever writes 16-bit words (mov:g.w)
at F3FA/F3FC/F3FE, and this chip is big-endian, F3FB/F3FD landing exactly
on the LOW byte of the F3FA/F3FC words (respectively) means the 0x2fad0
table's own entries for ReqID 0x09/0x0B genuinely point there -- not a
coincidence, not a mis-scrape. Table updated to CONFIRMED for both rows.
Lesson for future adjacency checks: word-store + odd-offset-ReqID is NOT
automatically an artifact -- rows have so far always come back zero-hit on static xref.

RESOLVED (2026-07-15, same session): F971 (0x2A) and F972 (0x2B), flagged as
POINTER/candidate leads in the 2026-07-14 sweep, are now fully CONFIRMED --
see updated table rows above. Read fuel_pw_and_airvol_compute (0x29fba) and
injpw_airvol_reset_on_fuelcut (0x24680) directly rather than relying on bulk
xrefs. Both confirm InjPulseWidth (F970/F971) and AirVol (F972/F973) are each
a single 16-bit big-endian word with no independent byte-level writer --
F971 is just F970's low byte, F972 is just F973's high byte. Same
mode-shadow-adjacent-word idiom as F3FA/F3FB and F3FC/F3FD, now confirmed a
third time on an unrelated writer function. This closes out the F970-F973
cluster entirely (all four ReqIDs now CONFIRMED) and reinforces the standing
approach note: for BLANK rows adjacent to a CONFIRMED word-value cell,
decompiling the writer function directly resolves the row far faster than
repeated bulk-xref attempts, which are a known dead end for this ROM's
indirect/bank-prefixed addressing.

EEEF/EEF3 CHECKED, STILL BLANK (2026-07-15, same session): both are the odd
low-bytes of the EEEE/EEF2 words zeroed in bulk by
startup_phase_reset_eed6_eefa_bulk (0x28413) -- but that's a reset path only
(itself flagged old-XML/not-yet-re-verified), not a steady-state producer.
Raw byte-pattern search for "EE EE" turned up two candidate hits inside
tcu_gear_ratio_calc (0x2cac4) and eeprom_backup_table_write_dispatch
(0x149f5); both fully decompiled and neither actually references 0xEEEE
anywhere in the body -- false positives from coincidental instruction
encoding, same failure mode as the earlier F0BB/F0BA false hits. No
steady-state writer found for EEEF/EEF3 this round. Remain BLANK.

F161/F15B RESOLVED (2026-07-15, same session): both moved out of BLANK via
adc_read_sequence_main (0x1556d) and gear_state_config_loader_f1fc
(0x20f28) -- see updated table rows above.
  - F161 (0x39): genuine live ADC channel #8 signal (F160/F161 word),
    independent of the neighboring MAP/Boost channel. Physical sensor
    identity still open.
  - F15B (0x3B): CONFIRMED DEAD. F15A/F15B is zeroed then immediately
    read back in the same function with no other writer anywhere in the
    ROM, so it always reads 0. Same class of finding as the MUT_83/0xF0BB
    dead cell from the 2026-07-13 session -- do not log this RequestID.
This is a good illustration of the approach note above generalizing
beyond *_calc/*_duty/*_target functions: the adc_read_sequence_*
functions are the actual ADC-channel producer layer underneath most of
the F1xx sensor block and are worth checking directly for any remaining
BLANK row in that address range before assuming "no producer exists."
[NOTE: lines 601/675-687 in this file had corrupted/duplicated text from
an earlier same-session edit collision (stray "olved jump..." fragment
and a truncated "deBits..." line) - found and repaired 2026-07-15 while
processing the uploaded XML files below. If anything in the
sci1_periodic_phase_dispatch_f526 section above looks inconsistent with
review.md's copy of the same finding, review.md is the fallback source
of truth - it was not affected by this corruption.]

EFI/TCU MUT PROFILE CROSS-CHECK, ROUND 2 (2026-07-15, user-uploaded XML)
--------------------------------------------------------------------------
User uploaded Mitsubishi_MUTII_EFI.xml (107 entries) and
Mitsubishi_MUTII_TCU.xml (13 entries) directly. Diffed both against the
current master table above.

EFI FILE FINDINGS:
- ReqID 0x1C (F187) was BLANK in the table but this XML clearly names it
  "ECULoad" (Eval="5*x/8", clips at 160) - missed in the original
  2026-07-13 build. Table updated to NAMED (untraced).
- ReqID 0x00 now has a THIRD conflicting claim: this same XML has an
  internal collision between "LoadMUT2Byte" and "gmas"/"Airflow Rate
  gm/s" at the same RequestID - not just a cross-file conflict with the
  GalantLegnum switch-flags claim already on record. Table row updated
  with all three claims listed; do not trust any one without live
  tracing.
- ReqID 0x12 (F111, EGRTemp) has a secondary claim "LC1WidebandO2" in
  the same XML, but its own Notes describe it as requiring an external
  0-5V sensor wired in by the user - a hardware-mod repurposing, not a
  genuine claim about the stock ROM. EGRTemp remains the primary lead,
  noted but not treated as a real conflict.
- FC = "Clear EFI Diagnostic Codes" in this file. CORROBORATES the
  2026-07-15 SCI1 command-dispatcher finding (review.md item 7,
  "UPDATE 2026-07-15" paragraph) that command 0xFC triggers
  startup_phase_reset_eed6_eefa_bulk via eee0_eeee_diag_flags_reset_dispatch
  - the disassembly-only trace inferred "bulk reset trigger" structurally;
  this XML confirms the semantic meaning independently (diagnostic code
  clear). Worth citing in that review.md item if revisited.
- Everything else in the file (0x21/0x29/0x38/0x3C/0x4A multi-name
  entries) is expected EvoScan-side formula/bit-label reuse of a single
  byte for several derived display values, or external accessory pseudo-
  IDs (ZTX/LC1/WDB/DLPIO8 - Zeitronix, Innovate LM-1/LC-1, wideband, and
  digital-logger-pod hardware, NOT real ECU MUT RequestIDs at all) - not
  genuine conflicts, no table changes needed for those.
- No other currently-BLANK row in the table matched anything in this
  file - the 0x1C miss above appears to be the only gap from the
  original build.

TCU FILE FINDINGS (NEW SCOPE - not previously covered anywhere in this
file or review.md):
- This ROM/vehicle has a SEPARATE MUT RequestID scheme for the TCU
  (transmission control) side, entirely distinct from the EFI table
  above and not yet cross-checked against any live TCU-side disassembly
  in this project. 13 entries: AT_ID1/2/3 (00/01/03, identification
  bytes), AT_Temp (86, transmission temp), AT_Park/AT_Reverse (B4,
  packed bit flags), AT_Neutral/AT_Drive (B3, packed bits),
  AT_Low/AT_2nd/AT_3rd/AT_OutputShaftSpeedPulse/AT_InputShaftSpeedPulse
  (A8, packed bits), AT_Brake (AA, packed bit), AT_ClearDTC (FC).
- CORRECTION (2026-07-15, per user): on this vehicle the TCU is a
  SEPARATE PROCESSOR living inside the same PCM housing as the EFI ECU,
  with its OWN MUT table - not a second logical channel on the same H8
  reachable from this program's SCI1/SCI3 dispatcher. There is likely
  NOTHING in common between the EFI-side RequestID scheme documented
  throughout this file and the TCU-side scheme in the uploaded XML.
  Retracts the "shared physical serial link" / "reachable from the same
  SCI1/SCI3 family" speculation below - that assumption was wrong.
- AT_ClearDTC also happening to use command FC is very likely COINCIDENCE
  (or a shared EvoScan/diagnostic convention across unrelated Mitsubishi
  ECUs) rather than evidence of a shared command dispatcher - do NOT cite
  this as corroboration for the EFI-side eee0_eeee_diag_flags_reset_dispatch
  finding. That finding stands on its own EFI-side evidence only.
- This table has NOT been cross-referenced against anything - there is no
  live TCU-processor ROM open in this project at all (this program is the
  EFI/PCM-side H8/539F ROM only). The tcu_* functions already traced in
  this project (tcu_shift_torque_and_knock_mgmt, tcu_periodic_dispatch,
  tcu_shift_ratio_buffer_update, tcu_torque_converter_slip_calc, etc.) are
  logic running on the EFI-side H8 that COMMUNICATES WITH the separate TCU
  processor (e.g. via the SCI1/SCI3 links documented in review.md item 7)
  - they are NOT the TCU processor's own code, and have no relationship to
  this RequestID scheme.
- NEXT STEP if pursued: this project would need the TCU processor's own
  ROM dump/disassembly (a different physical chip, not covered by the
  currently-open RVR_1998_x3 program) before any of these 13 IDs could be
  traced to real addresses. Not started - out of scope for this project's
  current Ghidra program unless/until a TCU ROM is obtained.

NOT YET DONE: the bulk of the ~150 BLANK/NAMED-untraced rows remain
untraced. Next step agreed with user: continue working the *_calc/
*_duty_*/*_target_*/*_stepper_* function list against the table, and
prioritize clusters adjacent to CONFIRMED cells (per the F5C0 and
F970-F973 precedents) over isolated BLANK rows, since isolated BLANK
rows have so far always come back zero-hit on static xref.

PRODUCER-SIDE SWEEP, ROUND 3 (2026-07-22 session)
------------------------------------------------------------------
Continued the *_calc function-list approach from Round 2.

EE8C-EE96 cluster (ReqID 0x60-0x65): addresses independently RE-CONFIRMED --
these already carry live Ghidra data labels MUT_60_entry..MUT_65_entry at
ROM 0x2fb90 (6-entry, 2-byte-stride table), so the master table's scrape was
correct. ONE writer found: inj_channel_state_init (0x21f00, renamed from
FUN_00021f1a in an earlier pass -- same function, Ghidra just resolved a
mid-body address to it) unconditionally sets all six to neutral value 0x8080,
no gating conditions. No runtime/per-cycle updater found despite checking
every *inj*- and *cyl*-named function (fueling_inj_pw_calc, which DOES show
a genuine 4-way per-cylinder repeating write pattern at F186/188/18A/18C +
F190/192/194/196 -- confirming this ROM does use per-cylinder arrays
elsewhere, just not reachable from this specific search for THIS cluster).
Plate comment added to inj_channel_state_init documenting all of this.
Remains OPEN: if these 6 values move during live logging, a real runtime
writer exists via indirect addressing not yet located; if they never move,
likely an unused/init-only feature.

F84F/EEFB/F4DF/F4DD/F4E5/F4E7/F4EB/F4DB cluster (ReqID 0x8C-0x93): addresses
RE-CONFIRMED via live Ghidra data labels MUT_8C_entry..MUT_93_entry at ROM
0x2fbe8 (8-entry, 2-byte-stride table, immediately following WGDCCorr/0x8B
and preceding the F0BB dead-cell run at 0x94/0x95). Disassembly comment
added at 0x2fbe8 documenting this. Checked three nearby *_calc functions as
candidate producers -- all ruled out, none write any of the 8 target
addresses:
  - o2_heater_duty_f4da_calc (0x19bbc): confirmed real O2 heater PWM duty
    writer (F4DA only), plate comment added.
  - isc_f4d6_gated_offset_calc (0x259df): computed offset, no direct RAM
    store in this function body, plate comment added.
  - f4de_f4e2_octane_correction_calc (0x265c9): confirmed writer of F4DE/
    F4E0/F4E2 (a related-but-distinct triplet, F4E0 not previously
    documented as part of this write group), plate comment added.
No producer found for any of the 8 cluster addresses. All three ROM-level
findings above should be considered solid/current; the producer question
for this cluster remains genuinely open.

DP=2 POINTER TABLE (<0xC0 dispatcher branch) -- RESOLVED (2026-07-22 session)
------------------------------------------------------------------
Followed up on review.md item "c" (resolve the <0xC0 pointer table in
sci1_meta_cmd_dispatch_c0_ff @0x28869, gating whether Mode5 RequestIDs
01-06/17/1A map onto it).

MECHANISM CONFIRMED (grammar-level, not guesswork this time): the relevant
instruction is `mov:g.w @(-0x530:16,R0),R1` at 0x2887e, preceded by
`ldc.b 0x2,DP` at 0x2887b. This EA form compiles via h8539f.slaspec's
eaw_disp16 constructor, which uses Rn_banked (NOT raw Rn) as its base:
  Rn_banked "R0" (Rn=0): local addr:4 = (zext(DP) << 16) | zext(R0); export addr;
  eaw_disp16: tmp = (Rn_banked + disp16); export *:2 tmp;
So the effective address is ((DP<<16)|R0) + disp16, evaluated as a full
24-bit quantity (banking happens BEFORE the disp16 add, confirmed by
decompile_function's own constant-folded output under tracked_set:
`((param_4 << 1) | 0x20000) - 0x530`, where 0x20000 = DP(2)<<16). This
matches the space="ram" default_memory_blocks layout, which per
list_segments has a SEPARATE lowercase "ram" block spanning 0x10000-0x2ffff
(the ROM image itself, loaded at base 0x10000) alongside the real chip
"RAM" block at 0xee80-0xffff. DP=2 banked addresses land in the ROM/"ram"
block, not chip RAM -- consistent with this being a ROM-resident pointer
table, not a runtime-computed one.

For cmd_byte=0x01 (Fuel Pump Relay, one of the 8 disputed Mode5 rows):
  addr = ((2<<16) | (0x01<<1)) - 0x530 = 0x020002 - 0x530 = 0x1FAD2

LIVE ROM CHECK: read_memory/inspect_memory_content at 0x1fad2, and swept the
entire plausible table span for cmd_byte 0x00-0xBF (0x1FAD0-0x1FC4E, 384
bytes) plus generous padding on both sides (0x1F800-0x1FC90, ~1200 bytes
total) -- ALL 0xFF. This is unprogrammed/blank flash, not a decode error
(0x1fad2 itself IS a valid, in-range address inside the ram:0x10000-0x2ffff
block -- reads succeed and return real backing bytes, they're just all
0xFF, unlike a truly out-of-range address which fails outright as
confirmed by contrast-testing 0x2530 directly, which errors with "Unable
to read bytes").

CONCLUSION: on THIS ROM image (RVR_1998_x3 4g63t 21000011 md352553.hex),
the DP=2 pointer table that would service SCI1 command bytes <0xC0 is
entirely blank. Every command byte in that range that reaches the table
read will dereference 0xFFFF -> attempt a byte read at ROM 0xFFFFFF-ish
(FF as page, i.e., effectively an invalid/wrapped access) -- this branch
is a dead/unimplemented code path on this specific ROM revision, not a
live actuator-command mechanism. Confirms (rather than merely leaves open)
that RequestIDs 01-06/17/1A do NOT map onto anything live via this route
either. Combined with the earlier finding that they also don't fall in the
0xC0-0xFF table-driven range, there is now no remaining live-code candidate
mechanism for those 8 RequestIDs on this ROM -- they should be treated as
inherited-but-unimplemented on this specific calibration, not merely
"unverified." mut_verification_status.md's Mode5 XML fix (below) can be
updated accordingly if desired, though "(UNVERIFIED)" vs "(UNIMPLEMENTED ON
THIS ROM)" is a wording choice, not a new investigative step -- the
open item is effectively closed absent a different ROM revision to compare
against (a different .hex file/calibration might have this table
programmed; not checked, out of scope for this ROM).

Reachability check: sci1_meta_cmd_dispatch_c0_ff has exactly one caller,
sci1_dispatch_and_latch_response (0x2882b), consistent with the existing
plate comment -- this is a static-reachability check, not proof the <0xC0
branch never executes in the field, but combined with the blank table it
would return garbage/0xFF if it ever did fire.

REACHABILITY CONFIRMATION + MODE5 ACTUATOR ID SUMMARY (2026-07-22, same
session, follow-up to the DP=2 table finding above)
------------------------------------------------------------------
Re-verified the caller chain end-to-end rather than trusting the prior
write-up on faith:

- sci1_dispatch_and_latch_response (0x2882b) calls sci1_meta_cmd_dispatch_c0_ff
  UNCONDITIONALLY on command-byte value -- gated only by flags f522 bit6=1/
  bit5=0, never by the byte's range. Confirmed via decompile_function, not
  assumption.
- The command byte itself is read from RAM 0xf534. get_bulk_xrefs on 0xf534
  returns zero (known banked-access blind spot), but search_byte_patterns
  found it referenced at 0x16759 (sci1_rx_frame_accumulator). That function
  computes `((DAT_0001f584 << 1) | 0x10000) - 0xacc`, which equals exactly
  0xf534 when the running index f584 is 0. f584 is an incrementing 0-10
  index into an 11-byte accumulator filled from DAT_0001fecd (the live
  SCI1 UART receive-data register) as bytes arrive on the wire.
  CONCLUSION: 0xf534 is byte 0 of the real SCI1 receive-frame buffer --
  i.e. it's genuinely the first byte of whatever a connected tool sends
  over the wire, completely unfiltered before reaching the dispatcher.
  This means the blank DP=2 table found above is a real, reachable dead
  end for any tool that ever sends a command byte <0xC0 -- not a
  hypothetical/unreachable code path.
- Spot-checked the 0xC0-0xD8 actuator bit table at ROM 0x13740 directly
  against raw bytes (read_memory, 80 bytes from 0x13740): confirmed byte0=1/
  byte2-3=0x0000 (stub/mask=0) for entries C0-C4, then a clean descending
  one-bit sweep (0x10,0x08,0x04,0x02,0x01) starting at entry C5, exactly
  matching the existing plate comment with no discrepancy found.

FINAL ANSWER, Mode5 (Actuator Test) RequestIDs, this ROM
(RVR_1998_x3 4g63t 21000011 md352553.hex):

| ReqID | Name (per XML profile, UNCONFIRMED on this ROM) |
|-------|--------------------------------------------------|
| 0x01  | Fuel Pump Relay                                   |
| 0x02  | EGR Solenoid                                      |
| 0x03  | Purge Control Solenoid                            |
| 0x04  | A/C Relay                                         |
| 0x05  | Condenser Fan Hi/Lo (exact split unclear)         |
| 0x06  | Condenser Fan Hi/Lo (exact split unclear) or 6th distinct actuator |
| 0x17  | Ignition Timing Fix                               |
| 0x1A  | ISC Step Fix                                      |

STATUS: none of these 8 IDs correspond to anything live on THIS ROM.
Exhaustive check of both candidate mechanisms came back negative:
  (1) 0xC0-0xFF table-driven dispatcher: fully decoded, real bit-toggle
      logic exists here, but for different command byte VALUES (0xC0+),
      not 0x01-0x1A. No overlap.
  (2) <0xC0 DP=2 pointer table: mechanism now fully resolved (see above),
      and the entire table span is unprogrammed 0xFF on this ROM image.
Names above are inherited from a generic Mitsubishi MUT-II EFI profile
template, not confirmed by anything this ROM's own code does. Do not
transmit any of these 8 commands to a real vehicle expecting a specific
effect -- best current evidence is they do nothing (or return garbage) on
this specific calibration.

OPEN, NOT YET DONE: whether a different ROM revision (e.g.
"RVR_1998_x3 4g63t 21000012 md352554.hex" or the w4a51 AT calibration,
both present in test/rvr/roms/) has this same DP=2 table actually
programmed. Not checked -- out of scope unless revisited.

PHYSICAL OUTPUT PIN TRACE (2026-07-22, "work backward from real hardware
outputs" session)
------------------------------------------------------------------
Different approach from the sci1_meta_cmd_dispatch_c0_ff RAM-flag chain:
instead of starting from command bytes, started from the H8's real port
data registers (P1DR-PCDR, ROM 0xfe80-0xfe97, confirmed via list_globals)
and traced backward to find genuine physical-output-driving code, since the
f510/f512/f516 flag chains kept terminating in further RAM state machines
with no confirmed hardware meaning (and one earlier hypothesis this
session -- that f516's high bits drove a PWM dual-speed fan -- was
CHECKED AND REFUTED: fec0-fed5 are SCI1/2/3 UART registers per
list_globals, not a timer/PWM peripheral; see the corrected plate comment
at 0x28700).

FOUND: mirror_status_f0e6_to_ports (ROM 0x156ce, now labeled) is a genuine
RAM-flag-to-port-pin mirror -- 8 bits of RAM 0xf0e6 map directly onto real
P1DR/P2DR/P4DR/P6DR/PCDR pins, several gated by per-variant ROM config
bytes at 0x102e4-0x102ee. RAM 0xf0e6 itself has been labeled
output_relay_flags_f0e6. Full bit map:

  bit0 (0x01) -> P1DR.4, direct
  bit1 (0x02) -> P2DR.7, gated by ROM byte 0x102ee
  bit2 (0x04) -> P4DR.7, gated by ROM byte 0x102ed (polarity flips on its bit3)
  bit3 (0x08) -> PCDR.3, direct, INVERTED (bit clear = pin set)
  bit4 (0x10) -> P4DR.4, gated by ROM byte 0x102e4==1
  bit5 (0x20) -> PCDR.6, same gate as bit4
  bit6 (0x40) -> PCDR.7, direct
  bit7 (0x80) -> P1DR.5, gated by ROM byte 0x102e6==0
  bit13(0x2000)-> P6DR.2, INVERTED, gated by f1f6 bit3

ONE bit's writer fully traced: bit3, via knock_octane_f0e6_bit3_gate_dispatch
(ROM 0x284a3). Sets/clears based on an RPM threshold, the EED6/EED8/EEDA
per-cylinder cluster (same cluster used by the warmup/calibration-select
system under f516_request_latch_f09a_calib_select), and a call into
knock_octane_pattern_match_state_machine (ROM 0x28557) -- a bit-pattern
state machine over the EED0-EEE8 array family driving internal timers
f51a/f51c/f51e/f04a, structurally consistent with misfire/sustained-knock
pattern detection. HYPOTHESIS (not confirmed against real hardware): PCDR
bit3 is a check-engine/MIL lamp driver or a knock-condition dash warning
output. This is circumstantial -- flag as unconfirmed if cited elsewhere.

The other 8 bits (0/1/2/4/5/6/7/13) are NOT yet traced beyond a one-shot
unconditional set of bit1 at cold-init (engine_state_f498_init, 0x17302 --
not diagnostically useful). ~20 more xref sites to f0e6 exist in the ROM,
untraced: 0x17341/173d5/173db/174c9/174cf/1861d/18623/187ba/187c2/18bb1/
18bbb/18dbf/18dc5/18ddd/19359/1935f/19377/197ce/197d8/19806/19891/19904/
1998f/19e4c/27c94 -- these are the natural next targets, since they're the
best current lead for identifying genuine actuator names (fuel pump relay,
A/C clutch relay, radiator fan, etc.) against real physical pins rather
than unconfirmed RAM-flag semantics. Also worth doing the same "start from
port registers, trace backward" sweep for P3DR/P5DR/P7DR/P8DR/P9DR/PADR/
PBDR, which haven't been checked yet at all this session.

NOTE ON SCOPE: this thread grew out of trying to identify more MUT
actuator-test names beyond the 8 (Fuel Pump Relay/EGR/Purge/AC Relay/
Condenser Fan Hi-Lo/Ignition Timing Fix/ISC Step Fix) already found
unmapped-on-this-ROM in the DP=2 pointer table finding above. It has NOT
yet produced a confirmed mapping back to any MUT/EvoScan-style RequestID
or actuator name -- it's a parallel, independent hardware-level
investigation, not (yet) a resolution of the original Mode5 RequestID
question. The two threads may or may not converge; worth keeping in mind
if resuming either one.

SESSION CORRECTIONS + FIRST CONFIRMED SCI1-DISPATCHER-TO-PHYSICAL-PIN CHAIN
(2026-07-22, continuation of "PHYSICAL OUTPUT PIN TRACE" above)
------------------------------------------------------------------
Two corrections to prior work in this same session, both caught and fixed
before propagating further -- noting the process here since it's a useful
lesson for future signature-naming work: eyeballing a long combined
read_memory hex dump for a repeating-structure table is error-prone.
Individual per-entry read_memory calls anchored on absolute address are
slower but far more reliable; use that method when precision matters (e.g.
finalizing a table's entry count or a specific bit assignment).

CORRECTION 1: cmd_c0_d8_actuator_bit_table (ROM 0x13740) is 25 entries
(cmd 0xC0-0xD8 inclusive), not 20 as the original plate comment said.
Confirmed the table's true end: bytes immediately after the 0xD8 entry
(0x137a0+4) break the 4-byte-entry pattern entirely.

CORRECTION 2: f516_hibits_f520_f0f2_mode_select (0x28700) does NOT drive a
PWM dual-speed radiator/condenser fan, despite its graduated f0f2 mode
values initially looking exactly like a plausible fan-speed selector. The
consuming peripheral registers (0xfec0-0xfed5) are confirmed via
list_globals to be SCI1/2/3 UART SMR/BRR/SCR/TDR/SSR/RDR registers, not a
timer/PWM channel -- this is UART baud-rate/serial-config switching, most
likely a slow-init-handshake vs fast-K-line-comms speed change typical of
MUT/ISO-style diagnostic protocols. Plate comment at 0x28700 corrected in
place so this dead end isn't picked up as a lead again.

NEW FINDING: first confirmed end-to-end chain from a sci1_meta_cmd_dispatch_c0_ff
command byte all the way to a real physical output pin:
  cmd 0xD8 -> sets f510 bit1 (cmd_c0_d8_actuator_bit_table entry 24,
    00 ff 00 02, triple-confirmed via independent read_memory calls)
  -> read by engine_mode_f20e_f510_check (0x174a1): true unless
    (f20e bit4 SET AND f510 bit1 CLEAR)
  -> consumed by ign_advance_f0e6_bit7_update (0x173c0), gated on ROM
    config byte 0x102e6==0
  -> sets/clears output_relay_flags_f0e6 (0xf0e6) bit7
  -> mirrored by mirror_status_f0e6_to_ports (0x156ce) onto P1DR bit5
    (physical pin 0xfe82), same 0x102e6==0 gate
This is the FIRST bit in the whole sci1_meta_cmd_dispatch_c0_ff table with
a fully traced path to a real port pin. Confidence on the CHAIN itself is
high (every link independently verified against live disassembly/memory).
Confidence on WHAT PHYSICAL COMPONENT P1DR.5 actually drives is LOW -
f20e is an extremely widely-shared engine-mode flag word (170+ xref sites,
not practically fully traceable), so the physical identity is only a weak
inference (ignition-confirm/coil-adjacent, given the function's inherited
name "ign_advance_f0e6_bit7_update" and f20e's general "engine
running/ignition active" flavor) - NOT a confirmed component name. Do
not treat "P1DR.5 = ignition output" as established; it's a lead only.

Also worth noting: cmd 0xD8's table entry (0x0002) breaks what was
otherwise a perfectly clean descending-by-half bit sweep across D4-D8
(0x0040, 0x0020, 0x0010, 0x0008, then D8 should be 0x0004 by the pattern
but is actually 0x0002) - unexplained irregularity, possibly a deliberately
skipped/reserved bit2, not yet investigated further.

REMAINING WORK for full f510/f512/f516 physical-pin identification (the
"completely name everything" goal): systematically trace every other bit's
reader the same way bit1 was just traced here - i.e. for each bit,
find its RAM consumer function, then follow that consumer's output until
it either reaches a real port register (P1DR-PCDR, confirmed labeled) or a
clearly-general/shared system (like f20e or f502) where further tracing
isn't practical. The ~20 untraced output_relay_flags_f0e6 (0xf0e6) writer
xref sites listed in the "PHYSICAL OUTPUT PIN TRACE" section above remain
the best next targets, plus doing the same "start from port register,
trace backward" sweep for P3DR/P5DR/P7DR/P8DR/P9DR/PADR/PBDR which haven't
been touched at all yet.

EGR + PURGE SOLENOID FINDINGS (2026-07-22, continuation of port-backward
trace)
------------------------------------------------------------------
Continued the f0e6 writer sweep. Three more bits identified, two with
high confidence and coherent, self-consistent gating logic:

- f0e6 bit1 -> P2DR.7 (0xfe83) = EGR solenoid control. Writer:
  egr_f0e6_bit1_update (0x18600), gate: egr_activity_condition_check
  (0x18628), RPM-windowed (disabled at engine-off and at high RPM with a
  decaying counter active). NOT reachable from sci1_meta_cmd_dispatch_c0_ff
  -- purely internal engine-state driven. Same ROM gate byte (0x102ee)
  used by both writer and port-mirror side.

- f0e6 bits4+5 (mask 0x30) -> P4DR.4 (0xfe87) + PCDR.6 (0xfe97) = EGR
  valve position/sequence drive (two-bit, likely open/close or step
  direction). Writer: egr_f0e6_valve_bits_update (0x18b63), fed by RAM
  0xf490 (an EGR valve-state word maintained by a larger EGR state machine
  -- egr_sequence_control @ 0x187cf, egr_mode_dispatch @ 0x187a0 which
  selects between 3 hardware EGR variants on ROM byte 0x102e4: 0=none,
  1=this sequence drive, 2=egr_position_target_f494_calc, a different
  position-target strategy not yet traced). NOT reachable from the SCI1
  dispatcher either -- also purely internal.

- f0e6 bit2 -> P4DR.7 (0xfe87) = canister PURGE solenoid control. Writer:
  purge_f0e6_bit2_update (0x18db0), gate: purge_enable_check (0x18dca),
  RPM/coolant-temp hysteresis-band gated (classic purge-control pattern).
  ***THIS ONE IS REACHABLE FROM THE SCI1 DISPATCHER*** -- purge_enable_check
  has a force-override path: when f20e bit4 is SET and f510 bit8 is SET,
  purge is forced active regardless of the normal hysteresis. f510 bit8
  is set by sci1_meta_cmd_dispatch_c0_ff command byte 0xD1 (table entry
  17, 00 ff 01 00 at ROM 0x13784, confirmed via read_memory). Full chain:
    cmd 0xD1 -> f510 bit8 -> purge_enable_check override
    -> output_relay_flags_f0e6 bit2 -> P4DR.7 (physical pin)
  This is the STRONGEST candidate found in the whole investigation for a
  genuine, externally-triggerable MUT/EvoScan-style actuator command on
  this ROM -- recommend prioritizing this for real-hardware verification
  if that becomes possible. Exact real-world f20e-bit4 trigger condition
  not fully pinned down (f20e is the same widely-shared 170+-xref flag
  word noted elsewhere as impractical to fully trace), but the mechanism
  itself, cmd byte through to the physical pin, is fully confirmed live.

RUNNING TALLY of output_relay_flags_f0e6 bits identified so far:
  bit0: ISC/knock-condition-gated, NOT SCI1-reachable (P1DR.4)
  bit1: EGR solenoid, NOT SCI1-reachable (P2DR.7)
  bit2: PURGE solenoid, SCI1-REACHABLE via cmd 0xD1 (P4DR.7)  <-- best lead
  bit3: TWO writers (see below), NOT SCI1-reachable (PCDR.3)
  bit4+5: EGR valve position/sequence (two-bit), NOT SCI1-reachable
        (P4DR.4 + PCDR.6)
  bit6: writer not yet traced (PCDR.7)
  bit7: SCI1-REACHABLE via cmd 0xD8 (f510 bit1), physical meaning LOW
        CONFIDENCE (P1DR.5, weakly inferred ignition-adjacent)
  bit13: writer not yet traced (P6DR.2) -- but see f510 bit13/0x2000
        read by o2_upstream_enable_check below, a possible lead
Remaining genuinely untraced: bit6 (PCDR.7), bit13 (P6DR.2).

XREF SWEEP, ROUND 4 (2026-07-22 session, continuation): worked through the
remaining ~12 untraced f0e6 xref sites listed above. Most resolved to
functions already covered by name (get_function_by_address confirmed each
address before treating it as new, per this file's own methodology note):
0x17341 -> inside engine_state_f498_init (already known, cold-init bit1
set). 0x1861d -> inside egr_f0e6_bit1_update (already known). 0x19e4c ->
inside channel_dispatch_and_snapshot_update (already known F5C0 snapshot
function; decompiled directly this round to confirm it only READS f0e6
bit0 into F5C8, adds no new writer).

bit0 CONFIRMED: writer is isc_f0e6_bit0_update (0x174c0, already named from
an earlier pass but not yet decompiled/closed out). Full body is a clean
two-way branch on knock_condition_eval() -- sets bit0 if knock condition
true, clears it otherwise. No SCI1/dispatcher involvement. NOT
SCI1-reachable.

bit3 has a SECOND writer, not previously documented: warmup_state_f594_f0e6_init
(0x27c86) unconditionally ORs in bit3 (`DAT_0001f0e6 = DAT_0001f0e6 | 8`) as
part of a 3-line warmup-state init (also sets F09A from a ROM constant and
F594=0x41). This is separate from and additional to the already-known
conditional writer knock_octane_f0e6_bit3_gate_dispatch (0x284a3, RPM/knock-
pattern gated). Two writers to the same bit -- one unconditional warmup-set,
one conditional runtime gate -- is a real, not-yet-reconciled finding; the
bit3->MIL/warning-lamp hypothesis should be treated as even less certain
until the warmup-init interaction is understood (does warmup unconditionally
turn the lamp/output ON, later governed by the runtime gate?). Neither
writer is SCI1-reachable.

NEW MULTI-BIT FIELD FOUND (separate from the tracked bit0-7/13 set):
o2_sensor_control_dispatch (0x19720) writes f0e6 bits 8-11 (mask 0xf0ff,
i.e. `uVar2 | (f0e6 & 0xf0ff)`) based on O2 sensor upstream/downstream
enable-check results (o2_upstream_enable_check @0x197f4,
o2_downstream_enable_check @0x198f2). This nibble is NOT part of the
port-pin bit0-7/13 set documented above (no entry in
mirror_status_f0e6_to_ports' bit map for bits 8-11) -- likely an internal
O2-control-mode field, not a physical-pin flag. Not yet further traced;
noting for completeness since it came up in the same xref sweep.

Also of note: o2_upstream_enable_check (0x197f4) reads f510 bit13 (0x2000)
as part of its override-gating logic (`(f20e & 0x10)==0 || (f510 &
0x2000)==0`), alongside its read of f0e6 bit8/9. This is the same f510
word the SCI1 dispatcher writes bits into for the purge (bit8) and P1DR.5
(bit1) chains above -- worth checking whether anything in
sci1_meta_cmd_dispatch_c0_ff's table sets bit13, which would make O2
sensor override SCI1-reachable too. NOT YET CHECKED.

STILL OPEN, next targets: bit6 (PCDR.7) and bit13 (P6DR.2) writers
remain untraced -- no candidate xref site from the original ~20-site list
turned out to touch them; a fresh xref/byte-pattern sweep specifically
for these two bits (rather than relying on the original generic f0e6
xref list, which turned out to be dominated by already-covered
functions) is the concrete next step.

XREF SWEEP, ROUND 5 (2026-07-22 session, continuation): re-ran the F0E6
byte-pattern search fresh (broader than the original ~20-site list -- 28
hits total this time, including 3 new addresses: 0x156d1 (inside
mirror_status_f0e6_to_ports itself, expected), 0x28538/0x2853e (inside
knock_octane_f0e6_bit3_gate_dispatch, already known). Checked every
remaining unresolved address via get_function_by_address before treating
as new, per this file's own methodology note.

BIT6 NOW FULLY RESOLVED. Writer is f0e6_bit6_update (0x19350, already
labeled from an earlier pass but not yet decompiled/closed out) -- clean
two-way branch on ign_advance_rpm_zone_enable_check() (0x19364), same
shape as bit0/bit7's writers. That check function is an RPM-zone
hysteresis gate (ROM lookup tables 0xd6e/0xd70/0xd72 selected by f43e
state, load-based override via f186) with a CONFIRMED SCI1 override path:
it also reads f510 bit7 (0x80) alongside f20e bit4, bypassing the normal
RPM/load check when both are set. f510 bit7 is set by
sci1_meta_cmd_dispatch_c0_ff command byte 0xD2 (table entry 18, ROM
0x13788, confirmed via direct read_memory: bytes `00 ff 00 82` -- byte0=0
selects f510, mask 0x0082 sets bits 1 AND 7 simultaneously). This also
resolves the "D8 sweep irregularity" flagged earlier in this file and in
review.md: cmd 0xD2's two-bit mask (0x0082) is not an anomaly in the D-row
sweep pattern -- it deliberately triggers two independent override checks
at once (bit7 for this ign_advance_rpm_zone_enable_check chain, bit1 for a
still-unfound separate consumer, NOT to be confused with the different
"bit1" already traced via cmd 0xD8/engine_mode_f20e_f510_check, which is a
different f510 bit1 reference entirely despite the coincidental naming --
re-verify carefully if revisited).

This is a THIRD confirmed SCI1-reachable actuator chain on this ROM (after
cmd 0xD1's purge-solenoid override and cmd 0xD8's P1DR.5/f0e6-bit7 chain):
  cmd 0xD2 -> f510 bit7 -> ign_advance_rpm_zone_enable_check override
  -> output_relay_flags_f0e6 bit6 -> PCDR.7 (physical pin)
Physical real-world component identity is still NOT confirmed (same
low-confidence "ignition-adjacent" inference as the bit7/P1DR.5 chain,
based on naming continuity with ign_advance_f0e6_bit7_update rather than
independent hardware verification) -- treat as a lead, not a fact.

BIT13 REMAINS GENUINELY OPEN (as an f0e6 output-pin writer). Exhaustive:
all 28 hits from the fresh F0E6 byte-pattern sweep are now accounted for
(either the port-mirror function itself, or one of the already-documented
bit0/1/2/3/4+5/6/7 writers/gates). None write f0e6 bit13 (mask 0x2000).
This is consistent with this ROM's known static-analysis blind spot for
bank-prefixed/indirect RAM writes (same class of gap documented elsewhere
in this file for Knock Voltage, Coolant Temp, and others) -- f0e6 bit13's
writer, if a dedicated one exists, is not reachable via a directly-encoded
literal-address instruction. f1f6 bit3 (which gates whether f0e6 bit13 is
mirrored to P6DR.2 at all, per mirror_status_f0e6_to_ports) was checked as
an adjacent lead (search_byte_patterns "F1 F6", 9 hits) but NOT further
traced this round -- a plausible next step.

CORROBORATED 2026-07-23: independent xref-based check (get_xrefs_to on RAM
0xf0e6, all 26 references) reaches the same conclusion via a different
method than the 2026-07-22 byte-pattern sweep - every xref is accounted for
(mirror_status_f0e6_to_ports, or one of the already-documented bit0/1/2/3/
4+5/6/7 writers), none touch bit13 (mask 0x2000). Two independent
methods (byte-pattern sweep and xref trace) agreeing strengthens the case
that any writer, if one exists, uses bank-prefixed/indirect addressing that
neither approach can see - not worth further search time without a new
lead (e.g. tracing f1f6 bit3's own writer, still unexamined).

CORRECTION to the previous round's note (2026-07-22, same session,
verified via direct read_memory rather than left as a flagged assumption):
f510 bit13 (the DIFFERENT bit13, on a different RAM word, read by
o2_upstream_enable_check as part of its O2-sensor-override gate) IS
SCI1-reachable after all. sci1_meta_cmd_dispatch_c0_ff's 0xC0-0xD8 table
entry for cmd 0xCD, at ROM 0x13770 (confirmed via read_memory: bytes
`00 ff 20 00`), targets f510 (byte0=0) with mask 0x2000 = bit13. This
directly matches the bit o2_upstream_enable_check (0x197f4) reads as part
of its `(f20e & 0x10)==0 || (f510 & 0x2000)==0` override-gating condition.
So the O2 sensor upstream-enable override chain (cmd 0xCD -> f510 bit13 ->
o2_upstream_enable_check override -> o2_sensor_control_dispatch's f0e6
bits8-11 field) is a FOURTH confirmed SCI1-reachable chain on this ROM,
alongside cmd 0xD1 (purge), 0xD8 (P1DR.5/f0e6 bit7), and 0xD2 (PCDR.7/f0e6
bit6). Unlike the other three, this one does not terminate in a physical
port pin (f0e6 bits8-11 are an internal O2-control-mode field per the
earlier finding, not part of mirror_status_f0e6_to_ports' bit map) -- so
its real-world effect, if any, is forcing the ECU's O2 sensor read/control
logic into an override mode rather than toggling a discrete relay/solenoid.
Physical significance not yet assessed; flagging as a genuine new lead,
not yet fully characterized.

RUNNING TALLY, UPDATED (2026-07-22, end of session): f0e6 bits 0/1/2/3
(x2 writers)/4+5/6/7 all have confirmed writers; only bit13 remains
untraced. Three of those chains (bit2/purge, bit7/P1DR.5, bit6/PCDR.7)
are confirmed SCI1-reachable via cmd 0xD1/0xD8/0xD2 respectively. A
fourth SCI1-reachable chain (cmd 0xCD -> f510 bit13 -> O2 sensor override)
exists but does not terminate in f0e6/a physical pin -- tracked
separately, not folded into the f0e6 tally.


master list for actually logging the car, built from this file's earlier
rounds. It already correctly reflects most of the CONFIRMED/OPEN/BLANK
rows above, including the full 2026-07-14 Unk_0xNN sweep.

BUG FOUND AND FIXED (2026-07-22): its Mode5 (Actuator Tests) block had 8
rows (RequestID 01/02/03/04/05/06/17/1A -- Fuel Pump Relay, EGR Solenoid,
Purge Control Solenoid, A/C Relay, Condenser Fan Hi/Lo, Ignition Timing Fix,
ISC Step Fix) each annotated "[RAM CONFIRMED ...]" pointing at the MODE 2
address for that same RequestID number -- e.g. RequestID 0x17 here claimed
RAM 0xf13d, which is actually TPS's real, independently-confirmed Mode2
address, not anything actuator-related. This was a paste-across error: the
Mode2 annotation template got copied onto Mode5 rows without updating the
underlying mechanism.

What's actually confirmed on the actuator side (see review.md item 7 /
"REAL COMMAND DISPATCHER FOUND" section above for the original trace,
extended this session by a full decompile of sci1_meta_cmd_dispatch_c0_ff,
0x28869): a genuine, live actuator/relay dispatcher exists, handling SCI1
command bytes 0xC0-0xFF with direct bit manipulation on RAM F510/F512/F516,
gear-indexed reads via F1FC into ROM tables at 0x232/0x252, and real H8
hardware timer register names (T2CRH, TMDRA) appearing in the disassembly.
Actuator-test functionality 100% EXISTS in this ROM -- not in question.

What remains UNRESOLVED: whether RequestIDs 01-06/17/1A (the 8 disputed
rows) map onto this 0xC0-0xFF dispatcher at all. They structurally do NOT
(all 8 values are <0x20, nowhere near the 0xC0-0xFF range), so either (a) a
different, not-yet-found mechanism handles them, or (b) these specific
RequestIDs don't correspond to anything real on this ECU and were inherited
from generic EvoScan/Evo tooling convention without verification. A second
candidate mechanism was explored: the SAME dispatcher function also handles
command bytes <0xC0 via a DP=2-banked pointer table (raw instruction:
`ldc.b 0x2,DP` + `mov:g.w @(-0x530:16,R0)`). This table was NOT resolved --
this project has no verified DP-bank-to-physical-address mapping for the
H8/539F SLEIGH implementation (confirmed via h8539f-sleigh-grammar notes),
and a manual resolution attempt this session (assuming DP=2 maps linearly
to a fixed offset) was unverified guesswork, not a confirmed finding -- it
should NOT be trusted or built upon without independently deriving the real
DP-bank semantics first, ideally with input from someone who knows this
specific H8 variant's banking hardware.

FIX APPLIED: the 8 Mode5 rows had their false "[RAM CONFIRMED]" claims
removed, Display names marked "(UNVERIFIED)", and Notes rewritten to
explain exactly which real Mode2 address was mistakenly borrowed for each,
plus a block comment above <Mode5> explaining the full situation (real
dispatcher exists and is solid; these specific RequestIDs don't provably
connect to it; do not transmit any of these 8 commands to a real vehicle
without independent verification, since an unverified ACTUATOR command --
unlike a read-only Mode2 item -- could trigger unintended hardware action).

NEXT STEP if pursued: either (a) get a real DP-bank mapping (from hardware
docs or someone familiar with this H8 variant) and properly resolve the
<0xC0 pointer table, or (b) determine some other way whether these 8
RequestIDs correspond to anything real on this ECU at all.


f516 BIT1/BIT12 WRITER SEARCH -- CLOSED (2026-07-26)
=====================================================
Picked up sci1_meta_cmd_dispatch_c0_ff's own "NOT YET DONE: writer for f516
bits 1/12" item. get_xrefs_to(0x1f516) only returns 10 hits (the source-level
touches Ghidra's xref DB tracked), all inside already-documented functions,
none touching bit1 (0x0002) or bit12 (0x1000).

Went further than a plain xref check: ran search_byte_patterns for the raw
address bytes "F5 16" across the WHOLE ROM. Returned 27 hits, not 10 -- the
extra 17 are individual instructions inside multi-branch functions that
Ghidra's xref generation apparently doesn't enumerate exhaustively (each
mov/bset/bclr touching the literal address is a separate hit even when
several live in the same source-level statement or the same function).

Manually inspected every one of the 27:
  - 1 false positive: 0x232d0, inside status_word_bit3_conditional_update_via_table.
    The "F5 16" bytes there are coincidental (table-offset arithmetic bytes,
    not an address reference) -- confirmed by reading
    TABLE_STATUS_WORD_PTRS_137A8 (0x137a8) directly, byte-for-byte: 16 real
    entries (f20e, f212, f21c, f220, f25e, f236, f11e, f122, f22a, f126,
    f12a, f22e, f232, ee8c, ee8e, plus a leading 0x0000 pad), none of them
    f516. A textbook case of the "raw byte-pattern search needs manual
    inspection before acting" lesson from review.md items 3/11.
  - 26 genuine hits, ALL falling inside functions already catalogued in the
    sci1_meta_cmd_dispatch_c0_ff plate comment: the dispatcher itself,
    f516_request_latch_f09a_calib_select (0x27e1a),
    eee0_eeee_diag_flags_reset_dispatch (0x282c1),
    f516_hibits_f520_f0f2_mode_select (0x28700),
    peripheral_fec0_fed0_state_update (0x165ec),
    f588_duty_gate_f516_bit11_set (0x28b62), and
    phase_cases_1to6_f516_hibit_f54a_writer (0x28c25 region). This last one's
    case 7 (@0x28cc8) had never actually been disassembled/defined as code
    before (a genuine gap, same class as item 11's function-boundary sweep) --
    disassembled it live this session: it's real code (sets f526/f54a/f58c on
    various gates) but does NOT touch f516 at all, closing that last unknown
    jump-table branch.

RESULT: none of the 26 genuine touches set bit1 or bit12. Every literal-
address instruction anywhere in this ROM that touches RAM word 0xf516 is now
accounted for. This is a stronger result than a plain xref check (it also
found the case-7 gap and the false-positive table-lookalike), but the
conclusion is the same as item 1's f0e6 bit13 finding: CLOSED as "no writer
reachable via literal addressing," NOT "confirmed no writer exists." A DP-
banked/indirect write is still theoretically possible (this ROM has at least
one confirmed real DP-banked mechanism -- the sub-0xC0 pointer table earlier
in this dispatcher), but this project has no verified DP-bank-to-physical-
address mapping to chase that with, same caveat as the <0xC0 table above.

Plate comment at sci1_meta_cmd_dispatch_c0_ff (0x28869) updated in place to
reflect this (methodology item 7 -- corrected/closed inline, not just noted
here). Do not re-run the same literal-address byte sweep without a new lead
(e.g. an actual confirmed DP-bank mapping, or a live hardware/log capture
showing these bits toggling).


~20 UNTRACED output_relay_flags_f0e6 WRITER SITES -- CLOSED (2026-07-26)
==========================================================================
Worked through the full untraced-xref list from the "PHYSICAL OUTPUT PIN
TRACE" section above: 0x17341/173d5/173db/174c9/174cf/1861d/18623/187ba/
187c2/18bb1/18bbb/18dbf/18dc5/18ddd/19359/1935f/19377/197ce/197d8/19806/
19891/19904/1998f/19e4c/27c94.

RESULT: this list was mostly STALE, not actually untraced. Decompiling each
address showed almost all of them land inside functions that the SAME
2026-07-22 session had already fully documented under other addresses
(egr_f0e6_bit1_update/egr_activity_condition_check @0x18600-18628,
egr_f0e6_valve_bits_update @0x18b90ish, purge_f0e6_bit2_update/
purge_enable_check @0x18db0ish, f0e6_bit6_update/ign_advance_rpm_zone_
enable_check @0x19340ish, o2_sensor_control_dispatch/o2_upstream_enable_
check/o2_downstream_enable_check @0x197ce-199xx). The address list was
apparently captured by an xref sweep BEFORE the individual per-bit write-ups
existed, then never refreshed.

Genuinely NEW findings from this pass:
  1. isc_f0e6_bit0_update (0x174c9): confirmed bit0's writer -- gated purely
     on knock_condition_eval(). Matches what review.md item 1 already said in
     brief, now independently re-derived and folded into this file's own bit
     map.
  2. warmup_state_f594_f0e6_init (0x27c86): NEW writer for bit3 (mask 0x08),
     unconditional set as part of warmup-state init (also sets f09a, f594).
     Second writer for bit3 alongside knock_octane_f0e6_bit3_gate_dispatch
     (0x284a3) -- same init-then-steady-state pattern as bit1's two writers.
  3. egr_mode_dispatch (0x187a0) and egr_sequence_control (0x187cf) traced
     as the orchestration layer above egr_f0e6_valve_bits_update: mode
     dispatch picks between 3 hardware variants (config byte 0x102e4: 0=no
     EGR/clear bits4-5, 1=sequence-control via egr_sequence_control, 2=
     egr_position_target_f494_calc, a THIRD variant not yet individually
     decompiled -- low priority, doesn't change any f0e6 bit finding since
     egr_sequence_control terminates in the already-known
     egr_f0e6_valve_bits_update either way).
  4. channel_dispatch_and_snapshot_update (0x19e4c) reads f0e6 bit0 (not a
     writer) to update a SEPARATE status word f5c8 -- a false "writer" hit
     from whatever originally built the untraced-address list (READ, not
     WRITE, of f0e6).

MOST IMPORTANT OUTCOME: re-deriving the full bit map this way surfaced a
REAL BIT-NUMBERING ERROR in review.md item 1's summary, present since
2026-07-23 and never caught until this cross-check. review.md had said
"bit3 = EGR solenoid (P2DR.7)" and "bit7 = canister purge (P4DR.7)" -- both
PIN-correct but BIT-NUMBER-wrong. The actual mapping (re-derived directly
from mirror_status_f0e6_to_ports @0x156ce, see bit table earlier in this
file):
  - EGR solenoid / P2DR.7 is bit1 (0x02), not bit3.
  - Canister purge / P4DR.7 is bit2 (0x04), not bit7.
  - The REAL bit3 (0x08, PCDR.3, inverted) is unrelated to EGR -- it's the
    knock/MIL-adjacent output covered by knock_octane_f0e6_bit3_gate_dispatch
    + the newly-found warmup_state_f594_f0e6_init.
  - The REAL bit7 (0x80, P1DR.5) is unrelated to purge -- it's
    ign_advance_f0e6_bit7_update, forceable via SCI1 cmd 0xD8, and was not
    mentioned at all in review.md's old summary.
Anyone testing "cmd 0xD1 = force purge" or targeting "bit7" for purge on
real hardware based on the old review.md text would have been chasing the
wrong bit (though cmd 0xD1's mechanism itself -- forcing f510 bit8, read by
purge_enable_check -- was always correctly traced; only the review.md
SUMMARY's bit-number label was wrong, not the underlying dispatcher finding
at 0x18dbf). review.md item 1 corrected in place 2026-07-26.

Full corrected f0e6 bit map (all 8 low bits + bit13 now have a known writer
or a closed "no literal writer" verdict; bits 8-11 are the separate O2
control-mode field, not part of the port-mirror table):
  bit0 (0x01) P1DR.4  -- isc_f0e6_bit0_update, knock-gated
  bit1 (0x02) P2DR.7  -- egr_f0e6_bit1_update (+ unconditional init set),
                          EGR solenoid, well-evidenced, NOT SCI1-reachable
  bit2 (0x04) P4DR.7  -- purge_f0e6_bit2_update, canister purge,
                          SCI1-reachable via cmd 0xD1 (strongest actuator
                          candidate on this ROM)
  bit3 (0x08) PCDR.3  -- knock_octane_f0e6_bit3_gate_dispatch (+ unconditional
                          warmup-init set), INVERTED, MIL/knock-lamp
                          candidate, unconfirmed identity
  bit4 (0x10) P4DR.4  -- egr_f0e6_valve_bits_update, EGR valve phase A
  bit5 (0x20) PCDR.6  -- egr_f0e6_valve_bits_update, EGR valve phase B
  bit6 (0x40) PCDR.7  -- f0e6_bit6_update, SCI1-reachable via cmd 0xD2,
                          ignition-zone-adjacent, unconfirmed identity
  bit7 (0x80) P1DR.5  -- ign_advance_f0e6_bit7_update, SCI1-reachable via
                          cmd 0xD8, ignition-adjacent, unconfirmed identity
  bit13(0x2000) P6DR.2 -- CLOSED, no literal-address writer found anywhere
                          (blind-spot caveat applies, see item 1 above)

REMAINING OPEN: exact physical identity of bit3/bit6/bit7 outputs (all
"plausible X" inferences, none independently confirmed against real
hardware); egr_position_target_f494_calc (the third EGR hardware variant,
config==2) not individually decompiled; P3DR/P5DR/P7DR/P8DR/P9DR/PADR/PBDR
still completely unswept from the port-register-backward direction. The
f0e6 output-pin trace itself is now essentially complete for bits 0-7/13.


cmd_c0_d8_actuator_bit_table CC-D8 REGION -- OFF-BY-ONE FOUND AND FIXED (2026-07-26)
=====================================================================================
While chasing the EGR "third hardware variant" (egr_position_target_f494_calc,
config==2) and its gate function check_mode_gate_f510 (which checks f510
bits 0x1800 = bits 11/12), re-derived the CC-D2 region of
cmd_c0_d8_actuator_bit_table directly via read_memory instead of trusting
the existing plate comment's transcription. Found it was off by one entry
across the whole CC-D8 span:

  WRONG (all prior notes):  CD-D2: mask 0x2000,0x1000,0x0800,0x0400,0x0200,
                             0x0100 ; D3: mask 0x0082 (dual-bit anomaly) ;
                             D4-D8: mask 0x0040,0x0020,0x0010,0x0008,0x0002
                             (D8 said to break pattern, expected 0x0004)

  CORRECT (verified via read_memory at 0x13768-0x137a3, every entry
  individually): CA=0(skip) CB=0(skip) CC=0x2000(bit13) CD=0x1000(bit12)
  CE=0x0800(bit11) CF=0x0400(bit10) D0=0x0200(bit9) D1=0x0100(bit8)
  D2=0x0082(bit7+bit1) D3=0x0040(bit6) D4=0x0020(bit5) D5=0x0010(bit4)
  D6=0x0008(bit3) D7=0x0004(bit2) D8=0x0002(bit1)

This single correction retroactively DISSOLVES two previously-flagged
"anomalies" that were actually the same off-by-one artifact seen from two
different wrong vantage points:
  - The "D3 two-bit entry (0x0082)" -- the dual-bit value is real, but it's
    D2's entry, not D3's.
  - The "D8 pattern break (expected 0x0004, got 0x0002)" -- there is no
    break. D8 correctly continues an unbroken 13-entry descending bit sweep
    running CC(bit13) -> CD(12) -> CE(11) -> CF(10) -> D0(9) -> D1(8) ->
    D2(7, +bonus bit1) -> D3(6) -> D4(5) -> D5(4) -> D6(3) -> D7(2) -> D8(1).
    D2's extra bit1 is the ONLY genuine deviation in the whole table: cmd
    0xD2 deliberately triggers two independent override checks at once
    (its own bit7 slot, plus a duplicate of D8's bit1), exactly matching
    what was already hypothesized about cmd 0xD2 in the f0e6_bit6_update
    plate comment -- just now correctly attributed to the D2 table entry
    instead of split across two wrongly-labeled "anomalies".

ALSO FOUND while re-decompiling sci1_meta_cmd_dispatch_c0_ff's actual
branch logic (not just the table bytes) to double check this: the real
gate condition for the ENTIRE table-driven range (all of C0-D8 except the
unconditionally-skipped CA/CB) is `(f00e==0) AND (f25a bit4 set)` at the
moment the command byte is processed -- if either is false, the table
lookup is bypassed entirely and the function just echoes f516 back and
returns 0xFF, regardless of what the table says. This was NOT previously
documented at this level of precision (earlier notes only mentioned a
narrower f00e==0 gate on CA/CB specifically, which turned out to actually
be an unconditional hard skip, not a runtime gate at all -- see the CA/CB
correction above). Practical implication: forcing ANY table-driven actuator
bit via SCI1 (e.g. cmd 0xD1's purge override) requires f25a bit4 to be set
at command-processing time, on top of whatever downstream conditions
(f20e, f594, etc.) were already documented for each individual bit's
consumer. f25a's physical/logical meaning is not yet characterized -- next
lead if pursuing the purge/ignition-override actuator chains further as a
real hardware test target.

Plate comment at sci1_meta_cmd_dispatch_c0_ff (0x28869) and review.md item
6 (SCI1 command dispatcher paragraph, "cmd 0xCD sets f510 bit13") both
corrected in place to cite CC instead of CD and the corrected table.

STILL OPEN: f510 bits 9-13 (set by cmd D0-CC per the corrected table) have
not been traced FORWARD to their consumers the way bit1 (D8), bit7/bit8
(D2/D1, purge+ignition overrides), and bit13 (CC, read by
o2_upstream_enable_check) already have been -- bits 9/10/11/12 specifically
(CF/CE/CD/D0... wait: D0=bit9, CF=bit10, CE=bit11, CD=bit12) are still
unidentified consumers, a good next target if returning to this thread.
egr_position_target_f494_calc's own gate (check_mode_gate_f510, checking
f510 bits 11+12 together via mask 0x1800) is itself a currently-untraced
consumer of exactly two of these bits -- worth checking next, since it
would mean SCI1 commands CD and/or CE can influence the EGR "position
target" hardware variant (a duty-cycle/stepper-style EGR valve control,
distinct from the relay-style two-bit sequence already traced), which
would be a genuinely new SCI1-to-EGR-actuator chain if confirmed.


FIFTH SCI1 ACTUATOR-FORCE CHAIN FOUND -- EGR VALVE VIA TIMER 6 PWM (2026-07-26)
================================================================================
Followed up the "STILL OPEN" lead from the previous section (whether
check_mode_gate_f510, reading f510 bits 11/12, is itself an untraced
consumer of the newly-corrected cmd_c0_d8_actuator_bit_table region). It is
-- and it terminates in the best-evidenced physical actuator found in this
whole investigation so far.

CHAIN: SCI1 cmd 0xCD (sets f510 bit12) or cmd 0xCE (sets f510 bit11)
  -> check_mode_gate_f510 (0x18ae5): override active if f20e bit4 SET and
     either bit is present
  -> egr_position_target_f494_calc (0x18bc2, the THIRD EGR hardware
     variant, selected when ROM config byte 0x102e4==2): overridden path
     forces f494 to a fixed ROM calibration value at 0xe6e instead of the
     normal RPM-band-computed duty target
  -> serial_fef9_duty_scale_and_set (0x15a11, runs only when config==2)
  -> ff88_set_scaled_clamped (0x16c33): scales f494*16, clamps to [1, 2048]
  -> writes DAT_0001ff88, confirmed via list_globals to be T6GR1H/L -- a
     REAL Timer 6 PWM duty/compare register, not a RAM flag or an inferred
     port-pin label.

CONFIRMED THIS VARIANT IS ACTIVE ON THIS ROM: read ROM byte 0x102e4
directly -- value is 2 (RVR_1998_x3 4g63t 21000011 md352553.hex). So this
isn't a dead/alternate-hardware code path the way the <0xC0 DP=2 table was
-- it is the live EGR control mechanism on this exact ROM.

CORROBORATION Timer 6 is a real, actively-configured PWM channel (not just
a coincidentally-named RAM address): peripheral_block_ff70_ff88_cold_init
(0x16b52), found via a raw search_byte_patterns sweep for "FF 80" (the same
method used to close the f516 bit1/bit12 search earlier), initializes at
cold boot: T6CRH(0xff80)=1, T6GR2H(0xff8a)=0x7FF (2047 -- matches
ff88_set_scaled_clamped's clamp ceiling of 2048 almost exactly, consistent
with GR2=PWM period and GR1=PWM duty, a standard H8 compare-match PWM
pair), T6GR1H(0xff88)=1 (initial duty), T6OER(0xff84)=0 (output-enable
register, exact polarity not determined without an H8/500 datasheet). Same
init function also sets TMDRB (0xff35, Timer 1's UNRELATED mode register)
bit3 specifically when config==2 -- a secondary hardware reconfiguration
tied to the same variant selector, not yet explained, low priority
side-note.

WHY THIS IS THE STRONGEST CANDIDATE SO FAR: every other SCI1-actuator
chain found in this investigation (cmd 0xD1 purge override, cmd 0xD8/0xD2
ignition-adjacent overrides) terminates in an f0e6 bit mirrored to a port
pin whose PHYSICAL identity is only an inferred, unconfirmed guess (e.g.
"P1DR.5 = ignition-adjacent" based on naming and a widely-shared f20e flag,
not proof). This chain terminates in an unambiguous, confirmed H8 timer
peripheral register (T6GR1) that is demonstrably configured as a real PWM
channel at boot with a matching period/duty register pair. The remaining
gap isn't "is this a real register" (it is) but "what's physically wired
to Timer 6's PWM output pin on the vehicle" -- no H8/500 pinout
cross-reference has been done in this project to answer that.

f20e bit4's meaning is still the same unresolved 170+-xref-site blind spot
affecting several of these chains -- would resolve a lot of remaining
uncertainty across bit7/bit8/bit11/bit12 overrides simultaneously if ever
pinned down, but isn't practically traceable in one sitting.

NEXT STEPS if continuing: (1) find the H8/500 pin assigned to Timer 6's
PWM output (T6's output pin per the H8/500 pinout, cross-referenced
against this ECU's harness pinout if available) to get a real physical
identity instead of just "Timer 6 PWM"; (2) check T6OER's bit polarity
against an H8/500 timer datasheet to confirm the output is actually
enabled, not just configured; (3) decompile egr_table_row_lookup_f496 and
the ROM constant at 0xe6e to characterize what duty value cmd 0xCD/0xCE
actually forces (e.g. full-open, full-closed, or some fixed test position);
(4) the parallel, still-untraced config==1 "sequence control" f0e6 bits4/5
chain remains present in the ROM but is DEAD CODE on this specific ROM
build (0x102e4==2, not 1) -- worth flagging as such if it's ever cited
elsewhere, since only variant 2 (this PWM chain) is actually live here.


"DO ANY OTHER TIMERS DO THE SAME THING AS TIMER 6" -- ANSWERED (2026-07-26)
============================================================================
Checked whether any of the other 6 on-chip timers (T1,T2,T3,T4,T5,T7 --
each with GR1/GR2 compare registers per list_globals) are used as an
actuator PWM output the way Timer 6 (T6GR1, driving the EGR position-target
variant) is.

METHOD: get_xrefs_to on each timer's GR1 compare register, using the
correct banked address (0x1ffXX, not the bare 0xffXX list_globals shows --
list_globals appears to truncate/not display the bank digit, learned this
the hard way after an initial round of xref checks against the wrong
address returned false "no writer" results for T6 itself, which IS a known
writer).

RESULT: no other timer has a confirmed writer.
  - T2 (GR1H/CNTH) IS touched, but only as an INPUT -- read by
    isr_ipu_ch2ch4_input_capture (0x168fa), the cam-position/coarse-RPM
    capture ISR already documented elsewhere. Not an actuator output.
  - T1, T3, T4, T5, T7: zero xrefs found to their GR1 registers anywhere in
    the ROM. Either genuinely unused on this ROM, or (same caveat as
    always) a literal-addressing blind spot -- not exhaustively ruled out,
    a byte-pattern sweep for T3/T5/T7 was attempted but produced too many
    false positives (their address bytes, e.g. "FF 58" for T3, are common
    generic immediate-mask values elsewhere in the ROM, unlike f516's
    sweep which had a low, checkable hit count) to be useful without a
    much more targeted approach than time allowed for this pass.

SO: Timer 6 remains the ONLY hardware timer confirmed as an actuator PWM
output on this ROM.

BUT a bigger, separate finding fell out of searching for "duty"-named
functions to answer this: there's a SEPARATE, NON-timer PWM mechanism --
software_pwm_output_scheduler (0x158d7) -- a software bit-banged PWM
running on a 48-step counter (f0f0), almost certainly driven by a periodic
timer ISR (which one, not yet identified). It drives SEVEN independent
duty-cycle outputs onto real port pins, not RAM flags:

  F448 (EGR duty, egr_target_f448_update) -> P1DR bit3 (0xfe82.3), gated
    ROM config 0x102de!=0 -- CONFIRMED ACTIVE (0x102de=1 on this ROM). A
    SECOND, independent EGR duty mechanism, coexisting with Timer 6's
    hardware-PWM EGR path (F494, config 0x102e4==2, also active). Whether
    these drive the same physical valve, two different EGR components, or
    one is a dead/unused parallel path despite its gate passing is NOT
    determined without real vehicle wiring info.
  F452/F454/F456/F458 (all four from o2_sensor_control_dispatch, the O2
    upstream/downstream control-mode function traced earlier this session)
    -> P2DR bits 0/1/2/3 respectively, each independently gated on f1f2
    bits 14/1/2. Resolves what those four O2-dispatch outputs physically
    are: four independent PWM-driven O2 heater duty outputs.
  F44C (wgdc_output_clamp_f44c, ALREADY CONFIRMED as WGDC -- turbo
    Wastegate Duty Cycle, MUT RequestID 0x86 in an earlier, independent
    verification session) -> PADR bit1 (0xfe93.1). *** BEST FINDING OF
    THIS SESSION *** -- this is the first time in this whole investigation
    an already MUT-confirmed value has been traced all the way to a real
    physical output pin. WGDC = PADR.1, software-PWM driven.
  F45C (isc_target_calc_and_store, an ISC DUTY SOLENOID path -- distinct
    from the separately-documented ISC STEPPER MOTOR mechanism,
    isc_stepper_mode_state_reload_d/f406/f408 found earlier this session --
    this ROM appears to carry code for BOTH an idle duty solenoid and an
    idle stepper motor) -> PADR bit2 (0xfe93.2), gated f1f2 bit12.

This means PADR (Port A), previously flagged in the "PHYSICAL OUTPUT PIN
TRACE" section as completely unswept, now has two confirmed bits: bit1 =
WGDC, bit2 = ISC duty. P1DR and P2DR (already partially mapped via the
f0e6 port-mirror table) each gain an ADDITIONAL, independently-driven bit
beyond their f0e6-mirrored ones -- P1DR now has 3 known bits (0e6-mirror
bit4=ISC-knock-gated, bit5=ignition-adjacent, PLUS this new bit3=EGR duty),
P2DR now has 5 known bits (0e6-mirror bit7=EGR solenoid, PLUS these four
new bits0-3=O2 heater duties).

Plate comment at software_pwm_output_scheduler (0x158d7) written up in
full. canister_purge_duty_calc_f4ac (F4AC, a previously-found actuator
function from an earlier session) was checked too -- it is NOT consumed by
this scheduler or any other traced function (xrefs to F4AC are only
self-references and an init write); its physical output mechanism remains
untraced, a good next target if continuing this thread.

NEXT STEPS if continuing: (1) find what periodically calls
software_pwm_output_scheduler (likely a timer-driven ISR, would confirm
the soft-PWM's actual output frequency/resolution); (2) trace F4AC
(canister purge duty, separate from the f0e6-bit2 on/off purge already
found) to its real consumer -- still missing; (3) characterize the
remaining f1f2 gate bits (1/2/12/14) controlling which O2/ISC channels are
active -- likely bank1/bank2 or single/dual-heater hardware variants; (4)
determine via real hardware whether the two parallel EGR duty mechanisms
(Timer 6 vs this scheduler's P1DR.3) are redundant, separate components, or
one is inactive on the actual vehicle despite both config gates passing in
ROM.


TCU SERIAL RX FRAME FOUND -- F4CA AND THE TCU LINK (2026-07-26)
=================================================================
Picked up on a question about RAM cell F4CA. Traced it to a small
checksummed receive-frame validator inside tcu_rx_main_scheduler
(0x2aa36, ~0x2b160-0x2b1cc), gated on (f1f2 bit13 OR ram-bit 0x2c8) AND
tcu_rx_frame_state_counter_f4ce==5:

  checksum = (F4C4 + F4C6 + F4C8 + F4CA) & 0xFF
  if checksum == F4CC:
      F4BC,F4BE,F4C0,F4C2 = F4C4,F4C6,F4C8,F4CA   # "confirmed good" latch
      tcu_rx_frame_timeout_counter_f4d2 = 0
      if (F4C0 & 7) != 0: F4A4 |= 4

CONFIRMED GENUINELY SERIAL, not just an internal protocol: F4C2 (the
confirmed-copy cell for F4CA) is ALSO written directly by
serial_fec5_byte_to_fifo (0x166bf) -- "fec5" is inside the already-
confirmed fec0-fed5 SCI1/2/3 UART register range (see
f516_hibits_f520_f0f2_mode_select). This is a real UART RX path, most
likely a dedicated ECU<->TCU serial link, separate from the SCI1
diagnostic protocol traced everywhere else in this file.

DOWNSTREAM IMPACT -- connects directly to the canister-purge actuator work
from earlier this session:
  - F4BC and F4BE (confirmed copies of F4C4/F4C6) are read directly inside
    canister_purge_duty_calc_f4ac (0x19560) as an addend and a mode-gate
    respectively. So canister purge duty is genuinely influenced by data
    arriving over the TCU link, not just internal engine-side state --
    plausibly purge gets suspended/modulated during transmission shifts.
  - F4C2 (confirmed copy of F4CA) is read by
    tcu_link_confirmed_flag_f226_bit7_update (0x22225, renamed 2026-07-26
    from f226_bit7_update_from_f4c2_check), which sets status flag F226
    bit7 when (F4C2 & 0x60)==0x40 -- best working hypothesis is a "TCU
    link confirmed/handshake OK" status bit, gated on the same F4A4 bit7
    purge-enable flag and a separate config byte (0x102fd).

RENAMED IN GHIDRA (2026-07-26): F4C4/F4C6/F4C8/F4CA -> tcu_rx_frame_raw_
byte0-3_f4c4/f4c6/f4c8/f4ca; F4CC -> tcu_rx_frame_checksum_f4cc; F4BC/F4BE/
F4C0/F4C2 -> tcu_rx_confirmed_byte0-3_f4bc/f4be/f4c0/f4c2; F4CE -> tcu_rx_
frame_state_counter_f4ce; F4D2 -> tcu_rx_frame_timeout_counter_f4d2.
Function f226_bit7_update_from_f4c2_check renamed to
tcu_link_confirmed_flag_f226_bit7_update. Plate comments written on
tcu_rx_main_scheduler, tcu_link_confirmed_flag_f226_bit7_update,
serial_fec5_byte_to_fifo, and canister_purge_duty_calc_f4ac (updated in
place to cross-reference this).

STILL OPEN: what F4C4/F4C6 (the raw sources of F4BC/F4BE, i.e. the actual
TCU-reported values influencing purge) represent physically -- likely
gear/shift-state or torque-converter data given the parent function's
"tcu_" naming and neighboring tcu_shift_ratio_buffer_update/
tcu_torque_converter_slip_calc functions elsewhere in this ROM (not
individually decompiled this session); which SCI channel (2 vs 3) "fec5"
belongs to, and whether it's the same physical UART as the
tcu_periodic_dispatch (0x2c12b) chain; canister_purge_duty_calc_f4ac's own
still-unresolved F4AC output (checked this session whether
software_pwm_output_scheduler consumes it -- it does not; F4AC's real
hardware sink remains untraced, and it's unclear whether F4AC's duty path
and the already-traced F0E6 bit2 on/off purge path (cmd 0xD1) drive the
same physical solenoid or two different ones).


ACTUATORS x MUT LOGGING TABLE -- CROSS-CHECK (2026-07-26)
============================================================
Checked today's actuator finds (F494/EGR-Timer6, F448/EGR-softPWM,
F44C/WGDC, F45C/ISC-duty, F452-458/O2-heaters, F4AC/purge-duty) against
the MUT ReqID table at 0x2fad0 directly, by reading its 300 raw bytes and
matching against each target address. Recognized the pattern immediately:
every match was off by exactly one byte from what I expected (F495 not
F494, F449 not F448, etc) -- because adc_sensor_convert_single, for
ReqID<=0xBF, returns the single byte AT the table-decoded address, and
this table consistently points at the LOW byte of each target word (same
convention already established elsewhere in this file, e.g. F3FB/F3FD).

RESULT: three real actuator words are DIRECTLY LOGGABLE via MUT Mode2:
  ReqID 0x84 -> F495 (low byte of F494, EGR Timer-6-PWM duty target).
    Table already had a NAME here ("RadFans (Cooling Fan Duty Cycle)",
    from the GalantLegnum profile) -- REFUTED. F494 is exclusively written
    by the EGR position-target function and exclusively consumed by the
    Timer 6 PWM chain; nothing fan-related touches it. The profile's label
    for this cell does not apply to this ROM.
  ReqID 0x85 -> F449 (low byte of F448, the second/software-PWM EGR duty
    output). Was BLANK in the table; now CONFIRMED and named.
  ReqID 0x8B -> F4AD (low byte of F4AC, canister purge duty). Table
    already had a name ("WGDC Correction / WGDCCorr") with an explicitly
    flagged open question about its relationship to WGDC (0x86/F44D) --
    RESOLVED: there is no relationship. F4AC is written by
    canister_purge_duty_calc_f4ac, a completely different subsystem from
    WGDC (wgdc_output_clamp_f44c). The "WGDCCorr" label is wrong; this
    cell should be understood as a second canister-purge-duty value.

ReqID 0x86 -> F44D (WGDC) was already CONFIRMED by name in this table from
an earlier session -- today's contribution there was finding its physical
pin (PADR bit1, via software_pwm_output_scheduler), not the MUT mapping
itself, which is now a nice end-to-end confirmation: MUT ReqID 0x86 reads
the exact same RAM cell that a real port pin is driven from. This is the
strongest example in the whole project of "MUT-loggable value = confirmed
physical actuator", and a template for what full confidence looks like for
the others once their physical pins are found too.

F45C (ISC duty solenoid) and F452/F454/F456/F458 (O2 heater duties, all
four) do NOT appear anywhere in the 0x00-0x95 table -- not loggable via
this MUT mechanism as currently understood. Worth checking the 0x9A+
range (per the existing FuelPumpSpeed note, ReqIDs above 0x95 exist in
some profiles but fall outside this project's already-scraped table) if
pursuing further.

PRACTICAL UPSHOT for logging: 0x84 and 0x8B's profile labels should NOT be
trusted for this ROM if building a logging profile off the existing XML --
use "EGR Duty (Timer 6 PWM target)" and "Canister Purge Duty 2" instead.
0x85 can be safely added as a new loggable channel ("EGR Duty 2").
Table rows 0x84/0x85/0x86/0x8B all updated in place above with full
cross-references.


RESPONSE PACK/SEND CHAIN TRACED -- diagnostic_snapshot_f54a_f566_build FOUND
AND RENAMED (2026-08-07)
============================================================
Continuing from the dispatch chain (sci1_dispatch_and_latch_response,
sci1_latch_and_send_f54a_handshake_byte, sci1_tx_response_feeder -- all
already documented above): traced UP one more level to find what actually
PACKS bytes before they're sent. Found via f526_state_reset_dispatch
(0x28d23, itself called every tick from sci1_protocol_state_machine) --
every 200 ticks (F598 countdown), if phase counter F526==0x80: sets
F58C=15 (frame length) and calls a function at 0x28d7d, previously
unnamed/undecompiled (DAT_.. globals only, no plate comment).

RENAMED 0x28d7d: diagnostic_snapshot_f54a_f566_build ->
  sci1_periodic_status_frame_build_f54a_f566
Plate comment added there (full field-by-field breakdown) and updated on
f526_state_reset_dispatch (0x28d23) to cross-reference it. See those plate
comments in Ghidra for full detail; summary:

Builds a 14-byte frame at F54A-F564 + 1 checksum byte at F566
(sum of all 14 words & 0xff):
  F54A: bool(RAM 0x102c1 != 0)
  F54C: packed bitfield pulling from F4B6/F4B8/F4C0/F4B4 -- CONFIRMED this
    directly reuses the TCU RX frame fields from the "TCU SERIAL RX FRAME
    FOUND" section above (F4C0 = tcu_rx_confirmed_byte2_f4c0). Real link
    between TCU status and this SCI1 broadcast.
  F54E: raw copy of F18C (untraced)
  F550: raw copy of F17A (untraced)
  F552/F554: muldiv_s16_rounded_3op() result (hi/lo split), saturated via
    passthrough_or_sat() -- operands not yet identified
  F556: raw copy of F13A (untraced)
  F558: raw copy of F130 (untraced)
  F55A: clamp_u8() result -- operand not yet identified
  F55C: raw copy of F10E (untraced)
  F55E/F560/F562/F564: always zero (reserved/padding)
  F566: checksum

IMPORTANT FINDING: this frame does NOT go through the 0x2fad0 MUT table /
adc_sensor_convert_single at all, and does not appear to carry a MUT
RequestID. It looks like a fixed-format periodic status/handshake
broadcast (plausibly the "C0 55 EF 85"-style post-handshake status
sequence referenced in sci1_latch_and_send_f54a_handshake_byte's plate
comment), NOT a per-RequestID Mode-2 data response. Whether an actual MUT
RequestID read (Mode 2) reuses this same F54A staging buffer + feeder
mechanism with different fill logic, or goes out a completely separate
path, is still OPEN -- do not assume this is "the" MUT send path without
further tracing.

NOT YET DONE (stopping here, 2026-08-07):
- Name/trace the five raw-copy source cells: F18C, F17A, F13A, F130, F10E.
- Identify operands passed into muldiv_s16_rounded_3op and clamp_u8 in
  this function.
- Confirm/deny whether sci1_tx_response_feeder's -0xab6 ROM/RAM table
  (see its plate comment) is the same buffer as this F54A frame, or a
  separate one -- address arithmetic doesn't obviously line up (F54A is a
  fixed RAM address; -0xab6 looks like a ROM-relative pointer-table
  offset indexed by F588<<1), so probably NOT the same buffer, but not
  confirmed.
- Trace how an actual Mode-2 MUT RequestID response (via
  adc_sensor_convert_single) gets from its table-read byte onto the wire
  -- this session did NOT find that chain, only the periodic status-frame
  chain. That's the next real target if "pack and send the MUT IDs" means
  the Mode-2 data-list response specifically, not the handshake broadcast.


FIVE FRAME-SOURCE CELLS TRACED (2026-08-07, same session)
============================================================
Traced the writers of the five "raw copy" fields in
sci1_periodic_status_frame_build_f54a_f566 (see above). Results, from
strongest to weakest confidence:

- F556 (frame) <- DAT_0001f13a: CONFIRMED = MUT Battery voltage,
  RequestID 0x14. Already independently verified in an earlier session
  (adc_read_sequence_b @0x15689, cross-checked against the 0x2fad0 MUT
  table). RENAMED DAT_0001f13a -> mut_battery_voltage_f13a.
  ** First hard confirmation that the periodic status frame carries a
  real, already-known MUT value -- revises the earlier "probably not
  MUT-related" read on this frame. **

- F558 (frame) <- DAT_0001f130: CONFIRMED = validated/scaled coolant
  temperature, the F84E->F12E->F130->F29A chain documented in
  coolant_temp_validity_and_scale's plate comment (0x21414, earlier
  session). RENAMED DAT_0001f130 -> coolant_temp_scaled_f130. Reminder:
  this path still does NOT reconcile with the MUT profile's claimed
  coolant-temp address (F109/F10F) -- separate open question, unaffected
  by this rename.

- F54E (frame) <- DAT_0001f18c: TRACED, not renamed. Written by
  fueling_inj_pw_calc (0x21c2f, called from fueling_lambda_update). F18C
  is one of four sibling cells (F186/F188/F18A/F18C), each computed as
  clamp_u8(inc_if_signed_flag(mul_u16_sat_s16(...))) -- reads like a
  clamped per-cylinder or per-bank injector pulse-width term. Not renamed
  -- which of the 4 siblings maps to which bank/cylinder isn't confirmed.

- F55C (frame) <- DAT_0001f10e: TRACED, not renamed. Written by
  calc_f110_f10e_via_table (0x21314, called from map_tps_axis_update and
  post_init_status_compose_and_ef98_signal) -- table/axis-interpolation
  routine using F10A/F10C as axis inputs, paired output with F110/F112.
  Consistent with a MAP- or TPS-derived interpolated load value. Not
  renamed -- axis identity not confirmed.

- F550 (frame) <- DAT_0001f17a: TRACED (partially) -- NO WRITE XREF FOUND
  anywhere in the program via static analysis (checked the full ~86-entry
  xref list; every single one is a READ). Must be written via a
  register-indirect store Ghidra's static xref pass can't see. Extremely
  heavily read -- 80+ call sites across nearly every major gating/
  threshold function in the ROM: closedloop_eligibility_check,
  purge_enable_check, isc_condition_eval, warmup_complete_gate_check,
  knock_condition_eval, cranking_detect_check, o2_upstream_enable_check,
  egr_f490_condition_flags_update, and others, plus three functions
  already named directly around it (f110_f17a_threshold_gate_check,
  f17a_load_zone_and_ef96_f1d8_check, f17a_f13c_load_zone_and_f514_gate_
  check). Strong circumstantial signature of a core DERIVED ENGINE LOAD
  value used as the primary zone/threshold input across the whole ROM --
  not confirmed, not renamed. Finding the indirect writer is the natural
  next step given how central this cell appears to be.

Ghidra plate comment on sci1_periodic_status_frame_build_f54a_f566
updated in place with all of the above, field-by-field.

REVISED CONCLUSION: the periodic status frame is NOT purely a generic
handshake/status broadcast as first suspected -- better described as a
compact PERIODIC SNAPSHOT of a small fixed set of key channels (link
status bit, TCU status bits, injector PW, the still-mysterious high-use
F17A load-like cell, battery voltage, coolant temp, a computed value, an
interpolated table value), sent automatically every 200 ticks once phase
counter F526 reaches 0x80. Remains structurally separate from the
request-driven Mode-2 MUT table read path (adc_sensor_convert_single /
0x2fad0 table) -- this frame's field list is fixed, NOT
RequestID-selectable -- but it demonstrably reuses real MUT-relevant RAM
cells (confirmed for Battery, strongly likely for others) as sources. The
original "pack and send the MUT IDs" question is still open for the
Mode-2 per-RequestID path specifically; this session resolved the
periodic/handshake path instead, a related but distinct mechanism worth
keeping documented since it shares real sensor cells with the MUT table.


F17A RESOLVED -- ENGINE TORQUE %, NOT RPM (2026-08-07, same session)
============================================================
Chased F17A's "no static write xref" mystery. Root cause: the writer,
engine_torque_pct_scale_calc (0x2184b), already exists in this ROM and
was already fully investigated in an EARLIER session (see logging.txt
OPEN ITEM #1) -- it just wasn't cross-referenced to this frame-builder
trace before now, and its write at 0x218b8 apparently isn't caught by a
routine xref sweep centered on the bare address (bank-switch/stc.w
sequence immediately before the store may be why -- worth remembering
for any other "no writer found" cell in this project).

RESULT (already proven earlier, reconfirmed and now cross-linked):
F17A = clamp_u8(div_s32_s16_rounded(F5CA/F5CC-derived engine torque
values)). NO RPM input anywhere in the computation. An old theory that
F17A was RPM was explicitly REFUTED in the earlier session; this session
did not need to re-litigate that, just connect it to today's frame-pack
investigation and to the ~86 read call sites (closedloop_eligibility_
check, purge_enable_check, isc_condition_eval, warmup_complete_gate_
check, knock_condition_eval, cranking_detect_check, o2_upstream_enable_
check, egr_f490_condition_flags_update, and more) that all gate on it as
their primary load/threshold input, plus its role as field F550 in
sci1_periodic_status_frame_build_f54a_f566.

RENAMED: DAT_0001f17a -> engine_torque_pct_f17a
Plate comments updated on engine_torque_pct_scale_calc (0x2184b, added
cross-ref to this frame + the load-gate consumers) and on
sci1_periodic_status_frame_build_f54a_f566 (0x28d7d, field F550 entry
updated from "TRACED, not renamed" to "CONFIRMED/RESOLVED").

RPM's real storage location is STILL UNKNOWN -- logging.txt OPEN ITEM #1
remains open on that specific point; today's work only closes the F17A
identity question, it does not find RPM.

Frame-builder cell status after this session: F550(torque%)=CONFIRMED,
F556(battery)=CONFIRMED, F558(coolant)=CONFIRMED, F54E(injector PW
sibling)=traced/not renamed, F55C(table/axis output)=traced/not renamed.
Only F552/F554 (computed value) and F55A (clamp_u8 result) remain
completely untraced -- their callee operands were never identified this
session; that's the natural next target in this function if continuing.


F552/F554/F55A OPERAND CHAIN TRACED (2026-08-07, same session)
============================================================
Decompiler was hiding the pushed arguments to muldiv_s16_rounded_3op and
clamp_u8 inside sci1_periodic_status_frame_build_f54a_f566 (calling-
convention resolution collapsed them). Read the raw disassembly directly
(0x28dd6-0x28dfb) instead. Findings:

- muldiv_s16_rounded_3op is called with FOUR pushed values, not three:
  DAT_0001f33c, DAT_0001f33a (both RAM), a ROM word at 0x808, and the
  literal constant 0x246. (The function is named "3op" from an earlier
  session's read of its internal math, but the call site pushes a 4th
  fixed scale/rounding constant -- naming may be slightly imprecise, not
  changed this session.)
- The raw 16-bit result is saturated via passthrough_or_sat(), then split
  hi/lo into F552 and F554.
- clamp_u8() is called on the SAME result (same R1:R0 pair, not a
  separate computation) to produce F55A. So F55A is NOT an independent
  fourth field -- it's a clamped view of the identical F552/F554 value.
  Frame-builder plate comment updated to reflect this (F552/F554/F55A
  documented together as one entry now, not three separate ones).

STILL OPEN: DAT_0001f33a and DAT_0001f33c both have ZERO xrefs anywhere
in the program except this single read site in the frame builder -- no
writer found by static xref search, and no other reader either. Same
"invisible write" signature as F17A had before its writer
(engine_torque_pct_scale_calc) was tracked down manually -- worth
applying the same manual-search approach here if this value matters
enough to chase further. Until a writer turns up, F33A/F33C's physical
meaning -- and therefore what F552/F554/F55A actually represent in the
sent frame -- remains UNKNOWN. The ROM word at 0x808 also couldn't be
read directly from this context (address not in the mapped segment as
given -- likely needs an EP/bank-relative reference to resolve).

Disassembly EOL comments added at 0x28dee (arg push sequence) and
0x28dfb (clamp_u8 call) in Ghidra flagging this for anyone reading the
listing directly, not just the plate comment.

SESSION WRAP-UP: all 14 fields of the periodic status frame
(sci1_periodic_status_frame_build_f54a_f566, F54A-F564 + F566 checksum)
are now at least traced to their immediate source, with confidence
levels ranging from CONFIRMED (F550 torque%, F556 battery, F558 coolant)
through TRACED-but-unnamed (F54E injector PW, F55C table/axis output) to
STILL UNKNOWN (F552/F554/F55A -- operand chain traced, but the two RAM
inputs feeding it have no other references anywhere and no known writer).
The original "pack and send the MUT IDs" question remains open for the
separate Mode-2 per-RequestID adc_sensor_convert_single path -- this
whole investigation thread has been about the periodic/handshake frame,
which is a related but structurally distinct mechanism.


F33A/F33C RESOLVED -- BANK-PREFIX BUG IN EARLIER XREF SEARCH (2026-08-07,
same session, user-supplied correction)
============================================================
The "zero other xrefs anywhere" conclusion for F33A/F33C in the previous
write-up was WRONG -- caused by searching under address prefix 0001f33a/
0001f33c when the real xrefs are indexed under 0000f33a/0000f33c (same
physical RAM cell, different H8/500 bank/EP tag -- this ROM uses both
prefixes for the same locations depending on which bank was active at
the access site, same idiom as the "bVar << 0x10 | 0xfxxx" pattern seen
throughout this project). User caught this by spotting both prefixed
forms side by side in a Ghidra symbol table listing. FLAGGING FOR FUTURE
SESSIONS: when a static xref search on a 0001-prefixed (or 0000-prefixed)
address comes back empty, ALWAYS re-check the other prefix before
concluding no writer/reader exists -- this is now the second time in one
session (after F17A) that a "no xref" dead end turned out to be a bank-
prefix miss, not a real absence of code.

RESULT: accum_latch_100tick_f33a / accum_latch_100tick_f33c (renamed from
DAT_0001f33a / DAT_0001f33c) are written inside tcu_rx_main_scheduler
(0x2aa36, write site ~0x2af3e), gated by a 100-tick reload timer (EF8E):

    DAT_0000f33a = DAT_0000f336;
    DAT_0000f33c = DAT_0000f338;
    DAT_0000f336 = 0;
    DAT_0000f338 = 0;

i.e. a straight SNAPSHOT-AND-RESET: F33A/F33C hold whatever accumulator
F336/F338 built up over the previous 100-tick window, and F336/F338 are
zeroed immediately after for the next window. F336/F338 are themselves
written inside fuel_pw_and_airvol_compute (0x29fba, write site ~0x2a77f)
following a mul_u16_hi/sat_add_s16-based accumulation near the end of
that function. Exact physical quantity being accumulated NOT fully
pinned down this session (fuel_pw_and_airvol_compute is large and
multi-purpose; only the write-site tail was inspected) -- but the parent
function's scope (injector pulse width / air volume computation, already
confirmed source of MUT ReqID 0x29/0x2C -- F970/F972) makes an integrated
fuel-delivery or airflow quantity the leading candidate. Whatever it is,
F552/F554/F55A of the periodic status frame carry a scaled/rounded
100-tick-windowed average or rate of it (via
muldiv_s16_rounded_3op(F33C, F33A, ROM@0x808, const 0x246)).

RENAMED: DAT_0001f33a -> accum_latch_100tick_f33a
         DAT_0001f33c -> accum_latch_100tick_f33c
Ghidra plate comment on sci1_periodic_status_frame_build_f54a_f566
(0x28d7d) rewritten with the corrected, complete chain. Disassembly EOL
comments added at 0x28dee (arg push site) and 0x2af3e (the latch/reset
site in tcu_rx_main_scheduler).

SESSION STATUS: all 14 fields of the periodic status frame are now
traced to a concrete source function. Remaining open items are about
PHYSICAL MEANING, not missing code paths:
  - F336/F338's exact quantity inside fuel_pw_and_airvol_compute (not
    pinned down, only the accumulation tail was read this session)
  - F18C's exact injector-PW sibling identity (bank/cylinder mapping)
  - F10E's exact table/axis identity
  - ROM@0x808's value and the role of constant 0x246 (scale/divisor pair)
The original "pack and send the MUT IDs" question (Mode-2 per-RequestID
path via adc_sensor_convert_single) remains a SEPARATE, still-open
thread from this periodic/handshake-frame investigation.


MAJOR FINDING: NO LIVE WIRE-DRIVEN MUT REQUESTID PATH EXISTS ON THIS ROM
(2026-08-07, same session -- corrected methodology per user feedback:
used xref/call-graph/byte-pattern search tools directly instead of
manual disassembly reasoning)
============================================================
Went back to the original "pack and send the MUT IDs" question with a
proper tool-driven trace instead of guessing from names. Method: pulled
every caller of adc_sensor_convert_single (0x171c3, the confirmed MUT
table @0x2fad0 reader) via get_function_callers, decompiled all of them,
and separately pulled every reader of the live incoming command byte
F534 via get_xrefs_to to see if the two ever connect.

adc_sensor_convert_single has exactly 13 call sites, ALL accounted for,
split across exactly two callers:
  - adc_5channel_backup_snapshot_write (0x1504b) -- 5 calls
  - channel_periodic_update_ringbuf_snapshot (0x14edd) -- 8 calls
Both are structurally identical: indexed by an internal "channel" number
(param_4, NOT the live RequestID byte), each builds a fixed-size record
(10 bytes / 5 words) by calling adc_sensor_convert_single with FIXED,
HARDCODED RequestID literals baked into the call sites (not values read
from the wire), computes a checksum via byte_sum_banked, writes the
record into a ring buffer at 0xf7e0+channel*0x34, and invokes a function
pointer from a table at 0x3898+channel*0x14. Both are called from
channel_event_or_periodic_dispatch (0x19e6d) with only TWO possible
channel values (gated by config flags at F84A/F84C), itself called only
from tcu_rx_main_scheduler's periodic tick path -- i.e. this is an
internal periodic black-box/backup snapshot mechanism, not a live
request/response mechanism.

Separately, F534 (sci1_rx_frame_buf_0, the live incoming SCI1 command
byte) has exactly 3 readers, ALL already fully documented elsewhere in
this file:
  - sci1_dispatch_and_latch_response (0x2882b)
  - f526_state_reset_dispatch (0x28d33)
  - sci1_send_final_handshake_byte_85_and_park_phase_machine (0x28d0d)
None of these three call adc_sensor_convert_single, reference the
0x2fad0 table, or reference either "channel" snapshot function above.
sci1_dispatch_and_latch_response's own command handler,
sci1_meta_cmd_dispatch_c0_ff, was independently already fully reverse
engineered in an earlier session: commands <0xC0 hit a pointer table at
ROM 0x1fad0 (NOTE: different address from the MUT table at 0x2fad0 --
easy to confuse, confirmed different) which is entirely UNPROGRAMMED
(all 0xFF) on this ROM -- dead code. Commands 0xC0-0xFF are all actuator
bit-flag writes (already documented), none of them read sensor data or
touch adc_sensor_convert_single.

Also ran a byte-pattern search for a literal embedded pointer to the MUT
table address (both 0x2fad0 and its possible bank-relative forms) --
no hits, consistent with it only ever being accessed via the fixed
compile-time offset already documented in adc_sensor_convert_single's
own code (EP=2 banked, -0x530 displacement), not via any other computed
reference elsewhere in the program.

CONCLUSION: on this ROM (RVR_1998_x3 4g63t 21000011 md352553.hex), there
is NO live path from an incoming SCI1 command byte to a MUT-table
(0x2fad0) read. The MUT table is real and its 8-bit RequestID indexing
scheme is fully understood (see adc_sensor_convert_single's plate
comment), but it is only ever consulted by two internal, periodic,
fixed-RequestID snapshot/backup-logging routines -- never by a
handler that takes an arbitrary RequestID off the wire and answers it.
This is a genuinely different conclusion from earlier working
assumptions in this file (which treated "how does the ECU answer a MUT
RequestID query" as an open dispatch-tracing problem) -- the honest
answer, at least for THIS ROM/this static command-byte dispatcher, is
that no such generic query handler exists. Diagnostic tools reading this
ECU's "MUT" data over SCI1 in practice must be doing so through the
0xC0-0xFF actuator-test / periodic-status-frame mechanisms already
documented (sci1_periodic_status_frame_build_f54a_f566 etc.), OR through
a mechanism this session did not find, OR this ROM's dead 0x1fad0 table
was intended to hold exactly this kind of per-RequestID handler and
simply was never programmed (matches the "unprogrammed/blank" finding
already on record for that table from an earlier session).

NOT YET DONE / OPEN QUESTIONS THIS RAISES:
- Check the OTHER ROM files in test/rvr/roms/ to see if THEIR 0x1fad0-
  equivalent table is programmed -- if so, that ROM would show what a
  live RequestID handler actually looks like, filling in what's dead
  code here.
- Confirm there's no SCI2/SCI3 (as opposed to SCI1) path that also
  reaches adc_sensor_convert_single or the 0x2fad0 table -- this
  session's xref search was address-based (catches all callers
  regardless of which UART triggers them, since it's software not
  wired to a specific peripheral) so this is likely already covered,
  but worth stating explicitly: the "only 2 callers, only fixed
  RequestIDs" finding is UART-agnostic and should hold regardless of
  which serial channel a real MUT tool would use.
- Reconsider whether "packs and sends the MUT IDs" was actually asking
  about the periodic status frame investigated earlier in this session
  (which DOES carry real sensor values including one confirmed MUT-table
  value, Battery/ReqID 0x14) rather than a live per-request handler that
  turns out not to exist on this ROM.


GHIDRA RENAMES/LABELS APPLIED FOR THE ABOVE FINDING (2026-08-07, same
session)
============================================================
Made the "no live MUT RequestID path" finding visible directly in the
tool, not just in this file:

RENAMED:
  adc_5channel_backup_snapshot_write (0x1504b)
    -> mut_fixed_reqid_backup_snapshot_5word
  channel_periodic_update_ringbuf_snapshot (0x14edd)
    -> mut_fixed_reqid_periodic_snapshot_8word

PLATE COMMENTS ADDED/UPDATED:
  - adc_sensor_convert_single (0x171c3): appended the caller-audit
    conclusion to the existing plate comment.
  - mut_fixed_reqid_backup_snapshot_5word (0x1504b): full rewrite
    explaining it's a fixed-RequestID internal logger, cross-referenced
    to its twin.
  - mut_fixed_reqid_periodic_snapshot_8word (0x14edd): same, mirrored.
  - channel_event_or_periodic_dispatch (0x19e6d): new comment explaining
    its role as the shared dispatcher for both snapshot loggers and the
    still-unidentified third handler at 0x14dd8.

LABELS CREATED:
  - mut_snapshot_ringbuf_base @ 0000f7e0 (the shared 0x34-byte-stride
    ring buffer both loggers write into)
  - mut_snapshot_channel_callback_table @ 00003898 (the shared 0x14-byte-
    stride per-channel callback/config table)
  Inline disassembly comments added at both loggers' address-computation
  sites (0x14ee8, 0x14f00) referencing these labels.

STILL OPEN (unchanged from before, now easier to pick up thanks to the
renames above):
  - Identify the 5 and 8 hardcoded RequestID literals used by the two
    loggers respectively.
  - Identify the purpose of the 0x3898-table callback function pointers.
  - Identify the third handler at 0x14dd8 (the branch taken when F84A/
    F84C bits 2 or 3 are set, as an alternative to the two renamed
    loggers).
  - Check the other ROM files in test/rvr/roms/ for a programmed
    equivalent of the dead 0x1fad0 table, which might reveal what a live
    per-RequestID handler looks like on a different build.


CORRECTION TO "MAJOR FINDING" ABOVE -- REQUESTIDS ARE CONFIGURABLE AT
RUNTIME, NOT HARDCODED LITERALS (2026-08-07, same session)
============================================================
The renames mut_fixed_reqid_backup_snapshot_5word /
mut_fixed_reqid_periodic_snapshot_8word applied earlier this session
were based on a decompiled read that hid the real addressing mode, and
the "fixed, hardcoded RequestID literal" claim in the finding above is
WRONG. Caught by going back and running disassemble_function directly on
both functions instead of trusting the decompiler's collapsed argument
view (same category of self-correction as the F17A/F33A bank-prefix
misses earlier in this session -- decompiler output and naive xref
sweeps both have blind spots on this H8/500 target, raw disassembly
keeps being the tiebreaker).

REAL MECHANISM: both functions read each RequestID from a RAM record at
0xf814 + channel*0x34 (5 words for the "backup" function at offsets
0x0/0x2/0x4/0x6/0x8, 8 words for the "periodic" function at
0x0/0x2/0x4/0x6/0x8/0xa/0xc/0xe), not from literals in the call sites.
That RAM record is populated and validated at runtime by
eeprom_backup_table_write_dispatch (0x149f5): it checks a terminator
byte (0x0D at record offset 0x33) and a checksum (byte_sum_banked'd,
stored at offset 0x32) against a staging blob at 0xf862+, and on a
match copies/derives fields into the record (including the RequestID
list read by the two snapshot functions). This is the classic shape of
an EEPROM-backed, checksummed configuration record -- strongly
suggesting the RequestID list logged by each of the 2 "channels" is
UPLOADABLE/CONFIGURABLE at runtime (plausibly via the same SCI1 serial
link documented throughout this file), not something fixed at compile
time.

RENAMED (correcting the earlier renames):
  mut_fixed_reqid_backup_snapshot_5word (0x1504b)
    -> mut_configurable_reqid_backup_snapshot_5word
  mut_fixed_reqid_periodic_snapshot_8word (0x14edd)
    -> mut_configurable_reqid_periodic_snapshot_8word
Plate comments on both, plus on adc_sensor_convert_single (0x171c3),
rewritten to reflect this correction rather than silently overwritten --
each retains the "CORRECTED" note explaining what was wrong and why.

WHAT STAYS TRUE from the original finding: the live incoming SCI1
command byte (F534) still has only 3 readers, none of which reach
adc_sensor_convert_single or these two logger functions -- so the
byte-by-byte live command stream and this EEPROM-configured logging
mechanism are still two SEPARATE systems. The corrected picture is: MUT
RequestID data IS being actively read and logged at runtime (contrary to
the original finding's implication that it might not be), just via a
configuration record uploaded/validated ahead of time rather than a
RequestID parsed fresh out of each incoming command byte.

CONSEQUENCE FOR "do we know all the MUT IDs": NOT YET -- this doesn't
directly hand us more RequestID identities (the actual configured values
live in RAM, invisible in a static ROM dump), but it DOES mean the path
to get them is now well-defined and tractable:
  1. Find what populates the 0xf862+ staging blob (most likely candidate
     for "next step" -- probably another SCI1/EEPROM-write handler).
  2. If that's ROM-resident default/factory config data (rather than
     purely runtime-uploaded), it may be readable directly from the ROM
     image the same way the MUT table itself was -- worth checking
     before assuming this requires a live capture.
  3. Failing that, a live capture (real hardware + logic analyzer, or a
     runtime debugger session, per Ghidra's live-debug tools already
     available in this environment) of RAM 0xf814+/0xf862+ while a real
     diagnostic tool talks to the ECU would reveal the actual configured
     RequestID list directly.

NOT YET DONE:
- Trace what writes/uploads 0xf862+ (the staging buffer for the
  EEPROM-backed config record) -- concrete next step.
- Determine whether 0xf862+'s content originates from ROM-resident
  default data or is purely runtime/live-uploaded.
- Re-examine whether this configurable-logging mechanism is what real
  MUT diagnostic tools actually use against this ECU, vs. some other
  path not yet found.


0xF862+ STAGING BUFFER TRACED -- DEFAULT CONFIG IS UNPROGRAMMED (2026-08-07,
same session, continuing directly from the correction above)
============================================================
Traced the source of the 0xf862+ staging blob per the "NOT YET DONE" item
from the previous entry. Found init_copy_const_block_via_memcpy_banked
(0x149c8), called unconditionally at boot from
subsystem_unconditional_init_chain (0x20d3c):

    memcpy_banked(2, 0xcf80, 0, 0xf862, 0x100);

i.e. copies 256 bytes from ROM bank 2:0xcf80 into RAM 0xf862 at init
time. This IS ROM-resident default data (not a live upload), so it's
directly readable from the static ROM image -- read it via
ghidra:read_memory at ROM address 0x2cf80, length 256.

RESULT: all 256 bytes are 0xFF. The default EEPROM-backed MUT logging
config block is COMPLETELY UNPROGRAMMED/BLANK on this ROM
(RVR_1998_x3 4g63t 21000011 md352553.hex) -- same "dead/blank ROM
region" pattern already seen twice elsewhere in this project (the
sci1_meta_cmd_dispatch_c0_ff sub-0xC0 pointer table @0x1fad0, and
several MUT table cells like 0x83/0x94/0x95 -> F0BB). Consequence: on
THIS ROM, eeprom_backup_table_write_dispatch's checksum/terminator check
(expects byte 0x0D at record offset 0x33) will fail against an all-0xFF
source, so the configurable RequestID list in
mut_configurable_reqid_backup_snapshot_5word /
mut_configurable_reqid_periodic_snapshot_8word never gets populated from
this default block -- meaning THIS specific code path is also
functionally dead on this ROM at boot, UNLESS something else
(a real EEPROM chip read at runtime, or a live SCI1 upload) overwrites
0xf862+ later -- which a static ROM dump cannot show or rule out.

REVISED "do we know all the MUT IDs" ANSWER: still no, and this
particular avenue (the two configurable-logging functions) is now a dead
end for extracting MORE RequestID identities from static analysis alone
-- their default config is blank, and any real config would only exist
in live RAM on actual running hardware (or a genuine external EEPROM
chip's contents, if this ECU uses one, which hasn't been confirmed
either way in this project). Getting further RequestID coverage from
here on will most likely require either (a) directly working through the
still-BLANK/untraced rows already logged in the main MUT table earlier
in this file via the SAME kind of manual writer-tracing used for
Battery/Coolant/TorquePct/etc, rather than (b) chasing this particular
runtime-config mechanism further, since (b) has now hit its natural
static-analysis limit.

Ghidra: no renames needed this round (init_copy_const_block_via_memcpy_
banked's existing name is already accurate). No plate comment existed
there before -- added one below documenting the 0xFF finding directly at
the function, so this dead end is visible in-tool, not just here.


CORRECTION: "DEAD END" CALL WAS PREMATURE -- MECHANISM IS LIVE, NOT
BOOT-ONLY (2026-08-07, same session, user pushback prompted re-check)
============================================================
The previous entry's framing ("this whole logging path is functionally
dead... unless something else overwrites RAM later, not visible in a
static dump") undersold how likely that "something else" actually is.
Checked properly instead of leaving it as a caveat:

- subsystem_unconditional_init_chain (0x20ccd) runs zero_ram_range
  (0xef64-0xf974) BEFORE init_copy_const_block_via_memcpy_banked copies
  the blank ROM default into 0xf862 -- so the all-0xFF state is a
  deliberate, confirmed COLD-BOOT default, not a static-analysis
  artifact. This part of the earlier finding stands.

- BUT eeprom_backup_table_write_dispatch (0x149f5) -- the function that
  validates the checksum/terminator and populates the RequestID record
  -- is called from channel_dispatch_and_snapshot_update (0x19d80),
  which is called from main_loop (0x20a80) itself. It is NOT a one-shot
  boot-time check. It runs continuously as part of normal ECU operation,
  every main loop pass (subject to its own internal gating/timing
  conditions, not fully re-examined this session).

CORRECTED CONCLUSION: this mechanism is very plausibly LIVE and
functional on real hardware. If anything writes valid data (terminator
byte 0x0D, matching checksum) into RAM 0xf862+ at any point during
normal operation -- a real serial EEPROM chip being read over I2C/SPI
(no such peripheral driver identified yet in this project, worth a
dedicated search), a live SCI1 upload, or some other mechanism -- this
dispatcher will detect it on its next main_loop pass and populate the
RequestID lists used by mut_configurable_reqid_backup_snapshot_5word and
mut_configurable_reqid_periodic_snapshot_8word. The all-0xFF ROM default
only tells us what a factory-fresh/never-configured unit looks like at
cold boot, not that the mechanism is unused in practice -- withdrawing
the earlier "dead end" characterization.

NOT YET DONE (revised, higher priority than previously stated):
- Identify what ELSE, if anything, can write to RAM 0xf862+ during
  normal operation (search for a hardware EEPROM driver / I2C-SPI
  peripheral access in this ROM -- not yet searched for by name).
- Check eeprom_backup_table_write_dispatch's own gating conditions more
  closely (the DAT_...f84a/f84c-adjacent flags already noted) to
  understand WHEN in practice it would find valid data to load, if ever,
  versus continuing to see the blank default.
- If a real donor ECU or EEPROM chip dump ever becomes available for
  this platform, reading its 0xf862-equivalent region directly would
  settle this immediately.


EEPROM/EXTERNAL-WRITER SEARCH RESULT: NONE FOUND IN THIS ROM (2026-08-07,
same session)
============================================================
Searched for what else could write RAM 0xf862+ at runtime, per the
previous entry's "NOT YET DONE" item.

- No SPI-, I2C-, or EEPROM-named functions exist anywhere in this ROM
  (search_functions for "spi"/"i2c" returned zero hits; "eeprom" returns
  only the already-known eeprom_backup_table_write_dispatch).
- This H8/500 does have an on-chip FLASH subsystem
  (flash_write_marked_blocks_loop @0x20888, flash_write_or_verify
  @0x20844, flash_byte_program_verify, sci1_boot_flash_write_block_loop/
  sci1_boot_flash_write_param_rx/sci1_boot_rxbuf_write) -- a real,
  substantial reflash-over-serial mechanism, consistent with how H8/500
  MCUs are normally field-reprogrammed (boot-mode SCI protocol writing
  on-chip flash block-by-block via a bitmask at 0xfee2). CHECKED: this
  subsystem does NOT touch RAM 0xf862 anywhere -- confirmed via both
  get_xrefs_to and a raw search_byte_patterns sweep for the literal byte
  sequence "f8 62" across the entire ROM image (4 hits total, all inside
  the two already-known functions: init_copy_const_block_via_memcpy_
  banked and eeprom_backup_table_write_dispatch). So the flash-reflash
  subsystem is a SEPARATE, unrelated mechanism (almost certainly for
  reflashing the ECU's tuning/calibration ROM itself, not this small
  config record) -- ruled out as the answer to "what writes 0xf862+".

CONCLUSION: within this ROM's static code, NOTHING besides the two
already-documented functions ever references RAM 0xf862 or its
surrounding record. Combined with the earlier finding that
eeprom_backup_table_write_dispatch runs every main_loop pass (so it IS
actively watching this location, not just checking once): the most
likely remaining explanations, in rough order of plausibility, are:
  1. A genuine external EEPROM/serial chip exists on this ECU's PCB,
     wired through a peripheral (I2C/SPI/generic port-bit-banged
     protocol) that simply hasn't been identified/named in this Ghidra
     project yet -- possible the bit-banged access looks like ordinary
     port I/O (e.g. via the fe80-fe97 port registers already documented
     elsewhere) rather than a dedicated peripheral register block, which
     would explain why nothing shows up under "spi"/"i2c" naming.
  2. Real MUT/diagnostic tools upload this record over SCI1 using a
     command byte this project hasn't decoded yet -- but this
     contradicts the earlier finding that ALL 3 readers of the live
     command byte F534 were fully traced and none reach this code, so
     this would require an entirely separate, still-undiscovered SCI1
     (or SCI2/SCI3) receive path.
  3. This mechanism is simply VESTIGIAL/unused on real RVR hardware --
     present in the code (inherited from a shared platform codebase
     across multiple Mitsubishi models) but never actually populated on
     this particular ECU/model, i.e. genuinely always blank on real
     hardware too, not just in this static analysis.
Session stopped here rather than speculate further -- distinguishing
between these three would need either a live hardware capture, a real
EEPROM/PCB inspection, or finding an as-yet-unidentified port-bit-banged
access pattern touching 0xf862, none of which are answerable from static
ROM analysis alone with the leads currently in hand.

No Ghidra renames from this entry (no new functions were found to name;
the flash subsystem's existing names are already reasonably accurate and
out of scope for this particular thread).


BIT-BANGED EEPROM DRIVER SEARCH: NO CANDIDATE FOUND (2026-08-07, same
session, continuing option 2 from the previous entry)
============================================================
Searched for a software-clocked serial EEPROM driver (I2C/SPI/one-wire
style) that might write RAM 0xf862+ via ordinary port I/O rather than a
named peripheral, since a name-based search for "spi"/"i2c"/"eeprom"
found nothing new.

Checked every function with a port-related name:
  - update_port_fe82_indexed (0x158a7): single-shot 2-bit output from a
    4-entry lookup table, gear-state related, called once from
    tcu_rx_main_scheduler. Not a protocol.
  - mirror_status_f0ea_to_ports (0x1578f): a long but straight-line
    (no loops) sequence mapping ~13 individual F0EA status bits directly
    to output port pins (relay/solenoid-style actuator outputs, one flag
    = one physical pin), called once per main loop pass from
    tcu_rx_main_scheduler. This is a real output-mirroring function but
    structurally nothing like a clocked serial protocol -- no
    read-modify-write timing loop, no dedicated clock+data pin pair.
  - mirror_status_f0e6_to_ports, toggle_flag_fe8b_bit4,
    f31c_bit_toggle_pulse, struct_bit11_toggle_from_gate_check: all
    single-flag toggles, not protocol drivers (not decompiled
    individually this pass, ruled out by name/role already established
    elsewhere in this project).

Also searched function names for "clock" and "toggle" directly -- no
further candidates beyond the above.

CONCLUSION: no bit-banged EEPROM/serial driver was found by this search.
Given this ROM's function set is fully enumerated (722 functions total,
a large fraction already named/documented across this project's
sessions) and nothing resembling a clocked read/write loop touching a
consistent pin pair turned up, the most likely remaining explanations
are narrowing toward:
  - option 3 from the previous entry (vestigial/unused mechanism on this
    platform) becoming relatively more likely, since a real hardware
    EEPROM would need SOME driver code somewhere in this ROM, and
    nothing matching that shape has surfaced after two separate search
    passes (naming-based, then structural/behavioral), or
  - a driver that exists but wasn't caught because it doesn't touch port
    registers directly (e.g. if it goes through the H8/500's built-in
    serial peripherals -- SCI2/SCI3 -- rather than bit-banged GPIO,
    which would mean re-examining the SCI2/SCI3 register writers already
    documented elsewhere in this project for one that ALSO touches
    0xf862, rather than searching port functions).

NOT YET DONE: cross-check the already-documented SCI2/SCI3 UART register
writers (FEC0-FED5 range, TCU link functions, etc, already traced
extensively elsewhere in this file) against 0xf862 specifically -- this
was not done as part of this port-focused search and is the more
promising remaining lead if this thread is picked up again. This session
is stopping the EEPROM-writer search here rather than open a third
search angle without a stronger reason to expect it will succeed.


WORKING CONCLUSION: NO EEPROM ON THIS ECU -- MECHANISM IS VESTIGIAL
(2026-08-07, same session, user input)
============================================================
User's assessment: this ECU likely has no onboard EEPROM chip at all.
This fits the evidence better than continuing to search for a hidden
driver:
  - Three independent search angles (xref search, whole-ROM byte-pattern
    sweep, port-function structural review) all found nothing that
    writes RAM 0xf862+ besides the two already-documented functions.
  - The H8/500 in this ECU already has on-chip FLASH (confirmed, real,
    substantial subsystem -- flash_write_marked_blocks_loop,
    sci1_boot_flash_write_* etc) which is the normal/expected
    non-volatile storage for a ROM-based ECU of this era -- a separate
    EEPROM chip would be somewhat redundant unless used for something
    flash isn't suited to (e.g. frequent small writes flash can't do
    efficiently), and no such usage pattern or driver surfaced anywhere.
  - RVR/4G63T-family ECUs of this generation are not otherwise known to
    use a separate serial EEPROM for this kind of data (no external
    corroboration found or expected to be needed here, given the direct
    code-level evidence already gathered points the same way).

TREATING AS RESOLVED (pending any future contrary evidence): the
eeprom_backup_table_write_dispatch (0x149f5) mechanism and its config
record at RAM 0xf814/0xf862 are VESTIGIAL on this ECU -- inherited from
a shared platform codebase (consistent with this project's other
confirmed-dead findings: the sub-0xC0 command table @0x1fad0, the
default config block itself being all-0xFF) but never actually populated
on real RVR hardware. The two "configurable RequestID" snapshot loggers
(mut_configurable_reqid_backup_snapshot_5word,
mut_configurable_reqid_periodic_snapshot_8word) will, in practice, never
find valid data and never actually log anything via this path on a real
vehicle.

Ghidra plate comments on init_copy_const_block_via_memcpy_banked
(0x149c8) and eeprom_backup_table_write_dispatch (0x149f5) updated to
state this conclusion plainly rather than leave it as an open question.

CLOSING THIS THREAD. Returning to the main MUT RequestID table's
remaining BLANK/untraced rows (0x40-0x49, 0x60-0x65, 0x7A-0x7F,
0x8C-0x93 clusters, per earlier notes in this file) as the next
productive target, using the same direct writer-tracing method already
proven on Battery/Coolant/TorquePct/etc earlier in this session.


METHODOLOGY CORRECTION: RAW BYTE-PATTERN SEARCH GAVE A FALSE POSITIVE
(2026-08-07, same session, self-caught)
============================================================
Attempting to trace writers for several BLANK MUT table cells (0x18/
F21D, 0x19/F217, 0x20/F179, 0x2E/F1E7, 0x44/EEDF, 0x45/EEE1) using
search_byte_patterns on the raw 2-byte address (e.g. "f2 1d") instead of
symbol-based xref search (which returned nothing for any of these,
correctly, since get_bulk_xrefs found zero references under either the
0000 or 0001 bank prefix).

EEDF and EEE1 checks were valid: only 1 byte-pattern hit each, and both
land squarely inside the MUT table's own data region (~0x2fb00-0x2fb60,
i.e. the table storing THESE addresses as targets for MUT lookups),
confirming these two are genuinely dead/never-written cells (MAT Scaled
and MAP Scaled respectively) -- consistent with their "BLANK" status.

F21D's check was NOT valid: the byte pattern "f2 1d" matched at 0x2be19,
which on inspection turned out to be inside a `mov:g.w @0xf71c:16,...`
instruction referencing a COMPLETELY DIFFERENT address (F71C, part of
tcu_torque_converter_slip_calc's slip-sample array F714-F71E) -- the
raw bytes just happened to overlap by coincidence with a different
instruction's operand encoding at a shifted byte offset. This is a
false positive, not a real reference to F21D. CORRECTED: F21D has no
confirmed writer this session; do not treat 0x2be19/
tcu_torque_converter_slip_calc as related to MUT RequestID 0x18.

LESSON FOR FUTURE SESSIONS: search_byte_patterns on a raw 2-byte address
literal is UNRELIABLE for single-address searches -- H8/500 instruction
encodings are variable-length and a 2-byte substring can coincidentally
appear inside a different, unrelated instruction/operand. Every hit
MUST be manually verified by decompiling/disassembling the containing
function and confirming the actual referenced address matches, not just
trusted because the byte pattern matched. (The EEDF/EEE1 checks above
happened to be valid because both hits landed inside a recognizable,
already-understood ROM data table region, not because the method itself
is reliable in general.) Prefer get_xrefs_to / get_bulk_xrefs (checking
BOTH 0000 and 0001 bank prefixes) as the primary method; use
search_byte_patterns only as a secondary cross-check with mandatory
manual verification of every hit before drawing conclusions from it.

CONFIRMED THIS SESSION (byte-pattern hits verified valid):
  - 0x44/EEDF (MAT Scaled) -- dead, no writer, only self-referenced from
    inside the MUT table itself.
  - 0x45/EEE1 (MAP Scaled) -- dead, no writer, same as above.

STILL OPEN / NOT VALIDLY CHECKED YET: 0x18/F21D, 0x19/F217, 0x20/F179,
0x2E/F1E7 -- xref search found nothing (genuine, both bank prefixes
checked), byte-pattern search was attempted but is not a reliable method
for confirming true absence of a writer given the false-positive above.
These remain genuinely unresolved rather than confirmed-dead; would need
either a cleaner search method or manual disassembly review of likely
candidate functions to properly rule a writer in or out.


F21D DOUBLE-CHECKED PROPERLY (2026-08-07, same session, user pushback
prompted re-verification)
============================================================
User correctly challenged the "false positive" dismissal of the 0x2be19
byte-pattern hit and pointed out the MUT table itself DOES have a real,
correctly-labeled entry (MUT_18_entry @ 0x2fb00, target RAM:0xF21D) --
worth re-checking properly rather than accepting the earlier dismissal
at face value.

Re-verified with get_assembly_context on 0x2be19 directly: confirmed it
is NOT an instruction boundary at all (mid-instruction byte offset,
falls inside the encoding of the mov:f.w @0xf71c:16,R0 instruction that
actually starts at 0x2be14) -- so the original "false positive" call was
correct, just for a slightly different precise reason than first stated
(byte-offset overlap mid-instruction, not a coincidental stack-relative
operand). This is now independently confirmed via the assembly-boundary
check, not just re-asserted.

Separately confirmed the MUT table's own entry is legitimate:
MUT_18_entry @ ROM 0x2fb00 correctly targets RAM 0xF21D (per existing
Ghidra label, user-supplied) -- this is expected and consistent with
every other MUT table entry; get_xrefs_to on the table entry address
itself (0x2fb00) returns no references, which is also expected since
table entries are accessed via computed indexing (RequestID*2 offset
from the table base), not individual direct references -- same as every
other entry in this table.

Ran a clean get_xrefs_to on F21D itself, BOTH bank prefixes (0000f21d
and 0001f21d) as the reliable/primary method (not byte-pattern search):
ZERO references either way. CONFIRMED (properly this time, not from a
byte-pattern artifact): F21D has no writer anywhere in the program --
genuinely a dead/unimplemented MUT cell, same category as EEDF/EEE1
found earlier. RequestID 0x18 should be treated as confirmed-dead, not
merely "unresolved due to unreliable search method" as the previous
entry left it.

CONFIRMED DEAD THIS SESSION (all via clean xref search, both prefixes,
zero hits): 0x18/F21D, 0x44/EEDF, 0x45/EEE1.
STILL GENUINELY OPEN (not yet re-checked with the same rigor):
0x19/F217, 0x20/F179, 0x2E/F1E7.


F217/F179/F1E7 CONFIRMED DEAD -- IMPROVED METHOD (2026-08-07, same
session)
============================================================
Per user's suggestion, checked the remaining 3 open cells (0x19/F217,
0x20/F179, 0x2E/F1E7) using BOTH methods together and cross-verifying
every byte-pattern hit against instruction boundaries before drawing any
conclusion -- addressing the earlier F21D mistake properly rather than
just avoiding byte-pattern search altogether:

  1. get_xrefs_to on the target address, BOTH bank prefixes (0000/0001)
  2. search_byte_patterns on the raw address bytes as a cross-check
  3. For every byte-pattern hit, get_assembly_context to confirm whether
     it lands on a real instruction boundary (a genuine code reference)
     or falls inside/adjacent to unrelated data (a coincidental match)

RESULTS -- all three fully consistent across both methods:
  - 0x19/F217: get_xrefs_to = 0 hits (both prefixes). search_byte_patterns
    = exactly 1 hit @ 0x2fb02, which get_assembly_context confirms is
    NOT an instruction boundary -- it's the MUT table's own entry for
    this RequestID (MUT_19_entry, immediately following MUT_18_entry at
    0x2fb00, consistent 2-byte stride). No real code reference exists.
  - 0x20/F179: get_xrefs_to = 0 hits (both prefixes). search_byte_patterns
    = exactly 1 hit @ 0x2fb10 (MUT_20_entry), confirmed non-instruction
    (table data) the same way.
  - 0x2E/F1E7: get_xrefs_to = 0 hits (both prefixes). search_byte_patterns
    = exactly 1 hit @ 0x2fb2c (MUT_2E_entry), confirmed non-instruction
    (table data) the same way.

CONFIRMED DEAD (RequestID -> RAM, no writer found by either method,
every byte-pattern hit verified as table data not code): 0x19/F217,
0x20/F179, 0x2E/F1E7. Added to the growing confirmed-dead list alongside
0x18/F21D, 0x44/EEDF, 0x45/EEE1 found earlier this session.

METHOD NOTE (supersedes the "prefer xref, byte-pattern unreliable"
framing from the earlier F21D correction): byte-pattern search IS a
useful, fast cross-check -- the earlier mistake wasn't in using it, it
was in not verifying the hit against an instruction boundary before
concluding. Used together (xref for the primary null-result check,
byte-pattern as a corroborating second signal, assembly-context to
verify every hit's true nature) the two methods agree cleanly and give
higher confidence than either alone. Using both together going forward
for remaining BLANK table rows.

RUNNING TALLY OF CONFIRMED-DEAD MUT CELLS (this session):
  0x18 F21D, 0x19 F217, 0x20 F179, 0x2E F1E7, 0x44 EEDF, 0x45 EEE1


STRONGER CONFIRMATION VIA FULL DECOMPILED SOURCE DUMP (2026-08-07, same
session, user pointed at a better tool)
============================================================
User pointed out a full decompiled-C dump of the whole ROM exists:
test/rvr/RVR_1998_x3 4g63t 21000011 md352553.hex.c (28,456 lines) --
a complete plain-text decompilation of every function, generated from
this same Ghidra project (confirmed current: it already reflects
today's renames, e.g. engine_torque_pct_f17a appears throughout).

This is a materially BETTER verification method than either of the two
used so far this session:
  - Unlike get_xrefs_to, it isn't split by bank-prefix (0000 vs 0001) --
    it's flat decompiled text, so the F17A-style "miss one prefix, hit
    the other" blind spot cannot happen here.
  - Unlike search_byte_patterns, it matches on the actual decompiled
    symbol name/address text, not raw instruction-encoding bytes -- so
    the F21D-style false-positive-from-coincidental-byte-overlap problem
    cannot happen here either.

Sanity-checked the method first: grepped "f17a" (127 matches) and
confirmed the real, already-known writer instruction is directly visible
in plain text (line 14170: `*(undefined2 *)((uint)bVar2 << 0x10 | 0xf17a)
= uVar1;`, inside engine_torque_pct_scale_calc) -- confirms this file is
a reliable, complete, current source to grep against.

Re-ran all 6 cells confirmed-dead earlier this session through this
file as a stronger cross-check:
  0x18 F21D -> 0 matches
  0x19 F217 -> 0 matches
  0x20 F179 -> 0 matches
  0x2E F1E7 -> 0 matches
  0x44 EEDF -> 0 matches
  0x45 EEE1 -> 0 matches

All 6 confirmed dead a THIRD way, independent of both earlier methods'
known blind spots. Treating these as SOLIDLY confirmed dead now, not
just "dead so far as static analysis could tell."

METHOD UPGRADE FOR ALL FUTURE WRITER-TRACING IN THIS PROJECT: grep the
full decompiled .c dump FIRST for any target address/symbol, as the
primary method going forward -- faster, immune to both known blind
spots, and works on the same underlying decompilation Ghidra already
has. Reserve get_xrefs_to for jumping from a grep hit's line number to
the live Ghidra function name/address the same way session by session,
and reserve search_byte_patterns only for cases where the decompiled
dump might not exist or need regenerating. This changes the standing
guidance from earlier in this file (which only compared xref-search vs
byte-pattern) -- the .c dump was not in use before this point in the
session and is a stronger option than either.


REAL ECU LOGS FOUND -- CONTRADICTS "NO LIVE MUT HANDLER" CONCLUSION,
NEEDS RECONCILING (2026-08-07, same session, user pointed at real data)
============================================================
User pointed at test/rvr/ecu logs/EvoScanDataLog_*.csv -- three real
captured logs from EvoScan (a real diagnostic tool) actually talking to
this ECU. Read EvoScanDataLog_2026.07.04_14.23.02.csv (205 data rows).

Header row lists real logged channels including: CoolantTempScaled,
MAPScaled, MATScaled, Battery, AirVol, LoadMUT2Byte, RPMMUT2Byte,
AirFlowMUT2Byte, Load11bit4, ECULoad, InjectorLatency, KnockSum, TPS,
RPM, TimingAdv, and more -- all with real, changing, plausible values
across 205 rows (e.g. RPM climbing 1156->4312, CoolantTempScaled rising
51.8->98.6 consistent with engine warmup, Battery ~14.2-14.4V, MAP/MAT
scaled values changing with load).

THIS DIRECTLY CONTRADICTS TWO CONCLUSIONS FROM EARLIER THIS SESSION,
NEED TO FLAG RATHER THAN IGNORE:

1. "0x44/EEDF (MAT Scaled) and 0x45/EEE1 (MAP Scaled) are confirmed dead
   -- no writer anywhere in the program" -- but MATScaled and MAPScaled
   are REAL, LIVE, CHANGING channels in this log, successfully read by
   EvoScan from actual running hardware. Either:
   (a) the static "no writer" finding is simply wrong / missed something
       (most likely, given this session already found TWO real
       bank-prefix blind spots for F17A and F33A/F33C -- a third miss
       here would fit the pattern), or
   (b) EvoScan computes/derives these two specific values on the PC side
       from OTHER raw channels it does read from the ECU (e.g. computing
       a scaled MAP/MAT from raw ADC counts using an EvoScan-side
       formula), rather than reading a pre-scaled value directly from
       RAM 0xEEDF/0xEEE1 -- possible given "Scaled" suffix implies
       post-processing, and other columns exist that might be the raw
       inputs (need to check EvoScan's channel definitions/formulas,
       not available in this project yet as far as this session has
       found).
   NOT YET DETERMINED which explanation is correct -- do not treat
   either the earlier "dead" finding OR this log as automatically
   correct without reconciling them.

2. The existence of LoadMUT2Byte / RPMMUT2Byte / AirFlowMUT2Byte columns
   -- explicitly named as MUT reads -- strongly suggests EvoScan DOES
   perform live, wire-driven MUT queries against this ECU, contradicting
   the earlier "MAJOR FINDING: NO LIVE WIRE-DRIVEN MUT REQUESTID PATH
   EXISTS ON THIS ROM" conclusion. Possible reconciliations, not yet
   determined:
   (a) the earlier static-analysis conclusion missed a real live-query
       handler somewhere in the ROM (most concerning possibility, would
       mean re-opening that whole investigation thread), or
   (b) EvoScan is reading these 3 specific "MUT2Byte" channels via a
       genuinely different mechanism than the RequestID table this
       session traced (e.g. a raw memory-peek command rather than a
       MUT-table-indexed RequestID, which this ROM's live command-byte
       handler WAS confirmed to support for other purposes -- worth
       checking sci1_meta_cmd_dispatch_c0_ff's actuator commands again
       for anything resembling a raw address peek), or
   (c) these 3 columns are present in EvoScan's log FORMAT/template but
       not actually successfully populated for this particular
       ECU/session (all 3 need to be checked for whether they show real
       varying data or a constant placeholder in this log -- NOT YET
       CHECKED).

NOT YET DONE (high priority, directly reopens earlier "closed" threads):
- Check whether LoadMUT2Byte/RPMMUT2Byte/AirFlowMUT2Byte columns in this
  log show real varying values or constant placeholders across all 205
  rows -- distinguishes possibility (c) above from (a)/(b).
- Check the other two log files (EvoScanDataLog_2026.07.04_14.26.28.csv,
  EvoScanDataLog_2026.07.11_13.55.16.csv) for consistency.
- If real MUT2Byte data is present and varying, re-open the "no live MUT
  handler" investigation -- specifically re-check sci1_meta_cmd_dispatch_
  c0_ff and any command byte range not yet fully mapped, since finding a
  real live handler would mean this session's conclusion there was
  incomplete, not final.
- Re-check whether EEDF/EEE1 truly have no writer by grepping the .c
  dump (proven better method) rather than trusting the earlier
  get_xrefs_to-based "confirmed dead" call, given this log's direct
  contradiction.
- Look for any EvoScan channel-definition/config file in this project
  (an .xml or .ini describing what address/formula each column reads)
  that would settle whether MATScaled/MAPScaled are direct RAM reads or
  PC-side computed values, and what protocol LoadMUT2Byte etc use.


CONFIRMED: get_xrefs_to HAS A REAL, REPRODUCIBLE BLIND SPOT -- NOT JUST
"CHECK BOTH PREFIXES", THE BUG IS DEEPER (2026-08-07, same session, user
asked to test directly)
============================================================
Tested directly rather than continuing to assume "check both prefixes"
was a sufficient fix. Used F130 as a known-good control case (17 real
xrefs already independently confirmed via the .c dump and multiple
functions' plate comments):

  get_xrefs_to(0001f130) -> 17 real references (WRITE in
    coolant_temp_validity_and_scale, plus 16 real READs across many
    functions) -- CORRECT.
  get_xrefs_to(0000f130) -> ZERO references -- WRONG. Same physical RAM
    cell, same ROM, same session, immediately after the correct query.

CONFIRMED: this is a real, reproducible tool limitation, not just an
occasional prefix mismatch that "checking both" reliably catches.
get_xrefs_to indexes references PER ADDRESS-SPACE-PREFIX STRING as typed
into the tool call, and does NOT unify 0000fXXX and 0001fXXX as the same
physical location even when Ghidra's underlying memory model treats them
as the same RAM byte (confirmed physically identical by the fact that
coolant_temp_validity_and_scale's WRITE at 0x21420, decompiled multiple
times this session, always resolves to the same physical cell regardless
of which prefix a caller happens to use in source).

CONSEQUENCE: "checked both 0000 and 0001 prefixes, zero hits either way"
is WEAKER evidence than this session treated it as being, for the batch
of cells declared confirmed-dead earlier today (0x18 F21D, 0x19 F217,
0x20 F179, 0x2E F1E7, 0x44 EEDF, 0x45 EEE1). Those checks are still
correctly reporting what get_xrefs_to returns for those two specific
literal prefix strings -- but if any real writer for these cells uses a
THIRD addressing form not captured by either literal prefix (e.g. a
runtime-computed bank-relative address, the `(uint)bVar<<0x10 | 0xfxxx`
idiom already seen elsewhere in this project for F17A/F33A/F33C's real
writers), get_xrefs_to would silently miss it under BOTH prefixes
tested, exactly as it already did once before for F17A/F33A/F33C.

REVISED STANDING GUIDANCE (supersedes "check both prefixes" from
earlier): get_xrefs_to / get_bulk_xrefs should be treated as a WEAK,
provisional signal only, even when checked under both known prefixes --
NOT a reliable "no writer exists" proof by itself. The .c dump grep
(proven immune to this specific failure mode, since it matches on
decompiled symbolic address text rather than a live per-prefix index)
should be the PRIMARY method for any "does X have a writer" question
from now on in this project, with get_xrefs_to used only as a secondary,
weaker corroborating signal -- reversing the priority order used earlier
today before the .c dump was located.

IMMEDIATE ACTION: re-verify all 6 "confirmed dead" cells from earlier
this session using the .c dump grep (already done for these exact 6
cells in an earlier entry today -- all 6 came back 0 matches there too).
Given the .c dump is generated by decompiling the SAME Ghidra project's
functions, it should reflect whatever address form the decompiler
resolves each write to, REGARDLESS of the bank-prefix string a human
happened to type into get_xrefs_to -- so the .c dump's 0-match result
for these 6 cells is NOT subject to this same failure mode, and stands
as better evidence than initially credited. This explains the apparent
contradiction with the real ECU log data (EvoScan showing live
MATScaled/MAPScaled values): most likely explanation is now (b) from the
previous entry -- EvoScan computes MATScaled/MAPScaled on the PC side
from other raw channels it reads, rather than a direct RAM 0xEEDF/0xEEE1
read -- since the .c dump grep (immune to the prefix bug) also found
zero writers for those two addresses. NOT fully confirmed -- would need
EvoScan's channel/formula definitions to be certain -- but the "Ghidra
tool bug" explanation for the EEDF/EEE1 discrepancy is now better
supported than the "static analysis missed a real writer" explanation.

STILL OPEN: the LoadMUT2Byte/RPMMUT2Byte/AirFlowMUT2Byte log columns
question is NOT resolved by this -- that's about whether a live
RequestID-driven MUT handler exists at all (a different question from
"does cell X have a writer"), and needs its own re-check (see previous
entry's NOT YET DONE list) rather than being explained by this
particular tool limitation.


SCRIPT RUN: FindRealWritersAcrossBankForms CONFIRMS ALL 6 CELLS TRULY
HAVE NO WRITER (2026-08-07, same session)
============================================================
Ran the new script (ghidra scripts/FindRealWritersAcrossBankForms.java,
via run_script_inline) against the live Ghidra project. Full whole-
program scan: 25,774 instructions (every instruction in the program,
confirmed by explicit counter), two passes each:
  - Pass 1 (MEM-REF): genuine resolved memory references, normalized to
    bare 16-bit RAM offset (immune to the 0000/0001 prefix-index bug
    confirmed earlier this session).
  - Pass 2 (IMM-ONLY): the target's bare 16-bit value appearing as ANY
    scalar immediate operand anywhere in the instruction stream,
    regardless of whether Ghidra resolved it as a reference -- the pass
    specifically designed to catch the computed-bank-store idiom
    ((uint)bVar<<0x10 | 0xfXXX) that was the real, previously-hidden
    writer for F17A and F33A/F33C.

RESULT: 0x18 F21D, 0x19 F217, 0x20 F179, 0x2E F1E7, 0x44 EEDF, 0x45 EEE1
all came back 0 hits in BOTH passes. This is now the strongest evidence
gathered this session for these 6 cells -- immune to both known blind
spots simultaneously (prefix-index bug AND computed-bank-store literal
scan), covering literally every instruction in the ROM once. Combined
with the earlier .c dump grep (also 0 matches for all 6, via a third,
structurally independent method), these 6 MUT RequestIDs should now be
treated as SOLIDLY confirmed dead/unimplemented on this ROM -- three
independent methods, each immune to a different failure mode, all agree.

RECONCILING WITH THE EVOSCAN LOG CONTRADICTION: this strengthens the
"EvoScan computes MATScaled/MAPScaled on the PC side from other raw
channels" explanation (option (b) from the earlier log-discrepancy
entry) over "static analysis missed a real writer" (option (a)) -- a
genuine ROM-side writer for EEDF/EEE1 would need to either use a memory
reference (caught by Pass 1), or embed the literal 16-bit address
somewhere in the instruction stream forming it (caught by Pass 2), and
neither turned up anything across the entire program. Still not
absolutely proof against a fully runtime-computed address with no
compile-time-constant low word anywhere (a theoretical remaining gap
noted in the script's own documentation), but that's a narrow enough
edge case that the PC-side-computation explanation is now clearly the
better-supported one for these two specific channels.

STILL OPEN (unchanged from before, NOT resolved by this script run):
the LoadMUT2Byte/RPMMUT2Byte/AirFlowMUT2Byte question is a different
claim (whether a live RequestID-driven query handler exists at all, not
whether one specific RAM cell has a writer) and needs its own separate
check, not addressed by this script or this entry.

Script confirmed working and reusable for future BLANK-row verification
in this project -- recommend running it (with an updated TARGET_OFFSETS
list) as the primary/first check for any future "does this MUT cell have
a writer" question, ahead of even the .c dump grep, since it covers both
failure modes in a single automated pass rather than requiring a human
to remember to cross-check two separate methods.


EVOSCAN.EXE INSPECTED -- ONE STRONG CONFIRMATION, CHANNEL FORMULAS NOT
RECOVERED (2026-08-07, same session, user supplied the EvoScan binary)
============================================================
User uploaded EvoScan.exe (a real .NET/Mono GUI executable, 4.8MB, "PE32
executable (GUI) Intel 80386 Mono/.Net assembly"). Inspected via string
extraction and raw byte scanning (no .NET decompiler was available in
this environment -- no monodis/ildasm/ilspycmd installed, and the
sandbox's network allowlist doesn't include a source to install one from
this session's tools).

CONFIRMED, STRONG POSITIVE RESULT: found an embedded XML block
(<ecumemmodels>) defining memory layouts for several Mitsubishi ECU
chip families EvoScan supports, including an entry for EXACTLY this
chip:

    <ecumemmodel version="1.0" model="H8539F">
        ... flash block list ...
        <memsegment name="RAM"  type="RAM"   start="0000EE80" length="1000" />
    </ecumemmodel>

RAM start 0xEE80, length 0x1000 (4096 bytes) -> RAM range 0xEE80-0xFE80.
This INDEPENDENTLY CONFIRMS the RAM floor-clamp address (0xEE80) already
found in adc_sensor_convert_single's plate comment during static
analysis -- EvoScan's own vendor-supplied memory model agrees exactly
with what Ghidra's disassembly showed. Real, useful corroboration that
this whole project's memory-map understanding is correct, from an
independent source.

NOT RECOVERED: the actual per-channel formula/PID definitions (what
raw byte/address MATScaled, MAPScaled, LoadMUT2Byte etc are computed
from) were NOT found as plain embedded text. Searched for common marker
strings (ecupidmodels, <pid, PIDModel, DataList, channel names
themselves) -- none present as literal text. Several apparent gzip/zip
magic-byte matches turned out to be coincidental byte sequences in
compiled .NET IL/metadata, not real compressed payloads (attempted
decompression on all of them, all failed cleanly with format errors,
consistent with false positives rather than corrupted real data).

LIKELY EXPLANATION: EvoScan's channel/formula definitions are probably
either (a) in a separate external file (a .xml/.ini/.mdb the .exe reads
at runtime, not bundled inside this binary at all -- worth asking if
such a file exists alongside the EXE on the original system), or (b)
compiled into .NET IL as actual code (e.g. C# methods computing each
channel value) rather than as data, which would require a real IL
decompiler (ILSpy, dnSpy, dotPeek) to read -- not available in this
environment's toolset.

NOT YET DONE:
- If any config/definition file (xml/ini/mdb/csv) ships alongside
  EvoScan.exe on the original machine, that would very likely contain
  the actual channel formulas -- worth checking for and uploading if it
  exists, rather than the .exe alone.
- A proper .NET decompiler (ILSpy/dnSpy) run over this .exe outside this
  environment would likely recover the channel-computation methods
  directly as readable C# -- suggested as a next step if the person has
  access to run one locally, since this environment cannot install one.
- The H8539F memory-model confirmation, while not resolving the channel-
  formula question, is real corroborating evidence worth keeping -- adds
  independent confidence to this project's memory-map work generally.


MAJOR RECONCILIATION: EVOSCAN SOURCE + ROM ANALYSIS CONFIRM A REAL DTC
SUBSYSTEM (2026-08-08, continuing from a separate session's de4dot/ILSpy
work, verified independently in this session)
============================================================
A prior/parallel session deobfuscated EvoScan.exe (Eazfuscator.NET 3.1,
cleaned via de4dot) and decompiled it with ilspycmd to
tools_ilspy\decompiled_evoscan\. That work's claims were INDEPENDENTLY
VERIFIED in this session by reading the actual decompiled frmMain.cs
directly (not just trusting the prior session's summary):

CONFIRMED REAL (read directly from frmMain.cs this session):
  - Line 14303: UI string "The Following EFI MUTII Diagnostic Trouble
    Codes are valid for 1996 or Earlier Mitsubishi Vehicles ONLY!" --
    IMPORTANT CAVEAT: this ROM is a 1998 RVR. EvoScan's own UI flags this
    specific 3-byte DTC read as intended for pre-1997 vehicles. The
    underlying MUT RequestIDs may still apply (protocols are often
    shared/extended across model years), but this is not certain and
    should be kept in mind, not assumed away.
  - Lines 14307-14321: the real MUT-II EFI DTC read sequence is exactly
    three sequential MUT command sends: method_60("3B",12) -> num3,
    method_60("3C",12) -> num4, method_60("3D",12) -> int_.
  - Line 14331: method_201(num3, num4, int_) decodes those three bytes'
    bits into 14 named classic MUT-II fault strings (Oxygen Sensor(11),
    Intake air flow sensor(12), ..., EGR sensor(43)), confirmed via
    direct grep of method_201's body (line 15529 onward).

CORRECTS AN EARLIER SESSION'S CONCLUSION: MUT RequestID 0x3B (RAM
F15A/F15B) was previously logged as "CONFIRMED DEAD (2026-07-15)" --
zeroed then immediately read back inside gear_state_config_loader_f1fc,
with no other writer anywhere, so it always reads 0. That FACTUAL finding
(always reads 0, only one zero-then-read site) still stands and is not
disputed. What was WRONG was the INTERPRETATION: "always 0" was labeled
"structurally dead / vestigial / do not log". Given EvoScan explicitly
reads this exact RequestID (0x3B) as the first of three DTC status
bytes, "always 0" almost certainly means "no diagnostic trouble codes
currently stored" (a healthy no-fault state), not "this cell doesn't do
anything". CORRECTED the table entry in place (see the 0x3B row above,
timestamped 2026-08-08) to reflect this -- re-classified as a real,
meaningful DTC status cell rather than vestigial/dead.

NEW ROM-SIDE EVIDENCE FOUND THIS SESSION (using
FindRealWritersAcrossBankForms, the robust dual-prefix+immediate-literal
script): checked the other 2 DTC-adjacent cells named in the earlier
table (0x3C/F123 already CONFIRMED as "Oxygen Sensor #2" -- consistent!
-- and 0x3D/F125, previously BLANK). Also checked F157/F158 as
plausible related cells given proximity to F158's real writer/reader.
Results:
  - F125 (0x3D): 0 hits both passes -- genuinely still no writer found,
    remains open (same status as before, not resolved by this pass).
  - F157: 0 hits both passes -- no writer found.
  - F158: REAL HITS FOUND. Written by tps_delta_calc (0x2152f). Read by
    tcu_dtc_status_check_dispatch (0x2bc0f) -- a function ALREADY NAMED
    "dtc_status_check" in this project from earlier work, now directly
    relevant. Decompiled it: it's a genuine multi-condition sensor-
    plausibility/rationality checker (the classic automotive DTC-setting
    pattern) cross-checking TPS-derived values, engine_torque_pct_f17a,
    coolant_temp_scaled_f130, and TCU shift state against expected
    ranges, calling a fault-handling routine at ROM 0x2bdb3 whenever a
    check fails. This is REAL, meaningful, actively-used code -- not
    dead -- and it ties together three separately-confirmed real cells
    from earlier this session (engine_torque_pct_f17a, coolant_temp_
    scaled_f130) plus this newly-relevant F158/tps_delta_calc pair, all
    under one coherent DTC/plausibility-check subsystem. Strong positive
    signal that the EvoScan reconciliation is pointing at something
    real in this ROM, not a coincidence.

TAKEAWAY: the EvoScan decompilation is a genuinely valuable, verifiable
cross-reference source for this project -- it already corrected one
earlier misclassification (0x3B) and pointed at real, previously-
unexplored ROM code (tcu_dtc_status_check_dispatch) that ties multiple
already-confirmed cells together into a coherent subsystem. This is a
much stronger source of ground truth than static ROM analysis alone,
since it reflects a REAL working diagnostic tool's understanding of this
exact ECU family.

NOT YET DONE:
- Trace ROM address 0x2bdb3 (the fault/DTC-set routine called
  repeatedly from tcu_dtc_status_check_dispatch) to find where it
  actually sets a stored DTC bit -- would likely lead directly to F15A/
  F157/F123/F125's real DTC bit-storage location(s).
- Check EvoScan's decompiled source for the 0x3C (F123, already
  "Oxygen Sensor #2") and 0x3D (F125, still blank) RAM mappings more
  directly -- may need to trace EvoScan's own memory-model XML
  (EvoScan.memmodels.xml, already extracted to tools_ilspy) to see if
  it states which RAM address each MUT RequestID maps to for the
  H8539F specifically, which would let us cross-check against our own
  0x2fad0 table entries directly rather than inferring from behavior.
- Given the "1996 or earlier" caveat, verify whether this 1998 RVR ROM's
  0x2fad0 table even has meaningfully different entries at 0x3B/0x3C/0x3D
  compared to what an older ROM would have -- worth comparing against
  other ROM files in test/rvr/roms/ if any are from pre-1997 vehicles.
- Continue exploring tools_ilspy\decompiled_evoscan\frmMain.cs for the
  EFI (non-DTC) live channel read logic (MATScaled/MAPScaled/etc) that
  motivated pulling in EvoScan in the first place -- not yet located,
  the DTC logic was found first via a parallel session's work and this
  session verified and extended it, but the original open question about
  live MUT2Byte channels remains unresolved.


CORRECTION/CLARIFICATION: EXACT "MUTII EFI" APPLICABILITY IS AMBIGUOUS
IN EVOSCAN'S OWN UI (2026-08-08, same session, user asked directly)
============================================================
User asked directly: "is this MUT-II EFI?" -- checked properly rather
than assuming. Confirmed: YES, the 0x3B/0x3C/0x3D DTC read sequence
(method_193 -> case "MUTII EFI":, line 15437) is specifically gated on
the user selecting "MUTII EFI" from EvoScan's protocol dropdown
(cboDTC), not a generic/default path.

BUT EvoScan's own UI is internally inconsistent about the exact model-
year range for that option:
  - Dropdown item text (line 4743): "MUTII EFI (Mitsu 1990-1999 i.e.
    Evo4/5/6)"
  - Tooltip on the same dropdown (line 4757): "choose MUT EFI for
    Mitsubishi 1992-1996, choose OBDII EFI for Mitsubishi 1996-2007"
  These two ranges disagree with each other (1990-1999 vs 1992-1996),
  and neither matches the "1996 or Earlier ONLY" warning string shown
  once DTCs are actually read (line 14303, found earlier this session).
  Three different year cutoffs appear in the same tool's UI.

THIS ROM IS A 1998 RVR -- sits in the disputed gap between all three
stated ranges. Genuinely uncertain from EvoScan's UI text alone whether
a real 1998 RVR should use "MUTII EFI" or "OBDII EFI" in EvoScan. This
is not resolvable by reading more of EvoScan's UI code -- it would need
either: (a) knowing what protocol a real 1998 RVR ECU actually speaks on
the wire (which is exactly what this Ghidra project is independently
trying to determine), or (b) real-world usage knowledge of which option
RVR owners actually select in EvoScan successfully.

DOES NOT INVALIDATE the method_60("3B"/"3C"/"3D",12)/method_201 findings
-- those are real code confirmed to exist and run under the "MUTII EFI"
selection. It DOES mean: do not assume this exact 3-byte DTC sequence is
necessarily what a 1998 RVR responds to correctly on real hardware
without independent confirmation (e.g. actually trying it against real
hardware, or finding corroborating evidence in the ROM itself that
RequestIDs 0x3B/0x3C/0x3D are meaningfully used for DTC-style bit
patterns rather than something else). The F158/tps_delta_calc/
tcu_sensor_range_check_and_f04e_max_update connection found earlier this
session remains a plausible but UNCONFIRMED link, not proof this ROM
implements the exact same MUTII-EFI DTC scheme EvoScan expects for
Evo4/5/6-era vehicles specifically.


ACTUATOR TEST TABLE RECOVERED FROM EVOSCAN -- REAL OVERLAP WITH ROM-SIDE
FINDINGS, ONE NAMING DISCREPANCY FLAGGED (2026-08-08, same session)
============================================================
Found EvoScan's real actuator-test command table in frmMain.cs,
method_309 (~line 19826-19885), gated on protocol-string prefix (EFI/
AYC/ACD/ABS/EVOX). For "EFI" (the relevant one for this ECU), confirmed
13 actuators with explicit RequestID bytes (decimal in source, converted
to hex below):

  Injector No.1        218 (0xDA)
  Injector No.2         219 (0xDB)
  Injector No.3         220 (0xDC)
  Injector No.4         221 (0xDD)
  Injector No.5         222 (0xDE)
  Injector No.6         223 (0xDF)
  Fuel Pump              216 (0xD8)
  Fuel Pres. Solenoid    214 (0xD6)
  Wastegate Solenoid     211 (0xD3)
  IG 5deg BTDC           217 (0xD9)
  Radiator Fan High      206 (0xCE)
  Radiator Fan Low       205 (0xCD)
  Speed Adj. Screw mode  195 (0xC3)

ALL 13 fall inside the 0xC0-0xFF actuator command range already fully
mapped in this project via sci1_meta_cmd_dispatch_c0_ff and the
cmd_c0_d8_actuator_bit_table (ROM 0x13740) -- direct, checkable overlap
with real ROM-side static analysis, not a new/separate mechanism.

CROSS-CHECKED AGAINST EXISTING ROM FINDINGS:
  - 0xC3: ROM-side already confirmed as `bset.w @0xf512:16, bit 6` --
    a simple bit-set command. EvoScan calls this "Speed Adj. Screw mode"
    -- plausible, not yet independently verified beyond the bit-set
    itself (what F512 bit6 physically does downstream not confirmed).
  - 0xCD: ROM-side already confirmed (2026-07-26 entry, "FIFTH SCI1
    ACTUATOR-FORCE CHAIN") as setting F510 bit12, part of a real,
    live-on-this-ROM chain terminating in Timer 6 PWM duty register
    T6GR1H (0xff88) -- documented at the time as an "EGR valve" control
    chain (egr_position_target_f494_calc, 3rd EGR hardware variant,
    confirmed active since ROM byte 0x102e4==2 on this exact build).
    EvoScan calls 0xCD "Radiator Fan Low".
  - 0xCE: same chain, sets F510 bit11 instead of bit12, same downstream
    T6GR1H PWM path. EvoScan calls 0xCE "Radiator Fan High".

DISCREPANCY WORTH FLAGGING, NOT GLOSSING OVER: the earlier ROM-side
session labeled the 0xCD/0xCE chain "EGR valve" (Timer 6 PWM), but
EvoScan's UI calls the same two RequestIDs "Radiator Fan Low/High".
These don't obviously match (EGR valve vs radiator fan are physically
different systems). Possible explanations, none confirmed:
  (a) the earlier ROM-side "EGR" label was itself inferred from function/
      variable naming conventions (egr_position_target_f494_calc, etc)
      that may have been a mis-assumption carried forward, not a hard
      confirmation -- worth re-examining with fresh eyes now that a
      real diagnostic tool suggests a different physical system;
  (b) EvoScan's generic "EFI" actuator list is shared/templated across
      multiple Mitsubishi ECU variants and the specific RequestID-to-
      physical-actuator mapping may differ on this particular RVR build
      vs whatever EvoScan's list was originally written against -- i.e.
      EvoScan's labels could be wrong/approximate for this exact ECU;
  (c) both are correct and the PWM output genuinely drives a radiator
      fan on this ECU, with "EGR" being purely a ROM-internal
      function-naming artifact (Mitsubishi's own internal variable/
      function names in the original firmware build could easily have
      been mis-transcribed as "EGR" during this project's naming passes
      if the evidence was circumstantial rather than a confirmed EGR
      valve datasheet/pinout match).
This is NOT resolved -- flagging clearly rather than picking one
explanation. The underlying ROM-side finding (F510 bit11/bit12 -> Timer
6 PWM duty, confirmed live on this ROM) stands regardless of which
system it physically controls; only the PHYSICAL IDENTITY label ("EGR"
vs "radiator fan") is now in question.

STRONG POSITIVE TAKEAWAY: the fact that EvoScan and independent ROM-side
static analysis agree on the EXACT SAME TWO REQUESTID BYTES (0xCD, 0xCE)
being part of one coherent actuator-override mechanism, found via two
completely different methods (a real vendor tool's UI vs static
disassembly), is strong corroboration this project's 0xC0-0xFF actuator
command mapping is fundamentally correct, even where a physical-system
LABEL might need revisiting.

NOT YET DONE:
- Check the ROM's cmd_c0_d8_actuator_bit_table entries for the other 11
  EvoScan-listed RequestIDs (0xC3 already checked above; 0xD3, 0xD6,
  0xD8, 0xD9, 0xDA-0xDF not yet cross-referenced against existing ROM-
  side documentation -- some may already be covered under different
  framing, e.g. 0xD8 is already extensively documented elsewhere in this
  file under "engine_mode_f20e_f510_check"/ignition-adjacent findings).
- Revisit whether "EGR valve" was ever a confirmed physical identity for
  the 0xCD/0xCE chain or just an inferred label from ROM function/
  variable names, per the discrepancy above -- check the original
  2026-07-26 entry's actual evidence trail rather than assuming either
  label is correct.
- No TCU-specific actuator or MUT-II TCU protocol option exists anywhere
  in EvoScan (checked this session, separate from this entry) -- if a
  TCU-side actuator test list is needed, it will not come from this
  source.


DISCREPANCY CHECKED -- "EGR" LABEL FOR 0xCD/0xCE WAS NEVER INDEPENDENTLY
CONFIRMED, EVOSCAN'S "RADIATOR FAN" LABEL MAY WELL BE RIGHT (2026-08-08,
same session)
============================================================
Traced the actual evidence trail behind egr_position_target_f494_calc's
name (the function driving Timer 6 PWM via cmd 0xCD/0xCE), rather than
assuming either label was correct.

FOUND: this ROM has no debug symbols -- every function name in this
project, including "egr_position_target_f494_calc", was ASSIGNED during
analysis, not recovered from firmware metadata. Its name's origin traces
back to a 2026-07-26 session note: it was called "a THIRD variant" of
EGR hardware control, selected by the SAME ROM config byte (0x102e4)
that ALSO selects two genuinely, independently-confirmed EGR mechanisms
-- egr_f0e6_bit1_update (EGR solenoid, P2DR.7) and egr_f0e6_valve_bits_
update (EGR valve position bits, P4DR.4 + PCDR.6). The name was applied
by association/grouping ("it's config==2 of the same EGR-variant
selector"), NOT because its actual physical output was independently
verified to be an EGR valve. The same 2026-07-26 notes explicitly flag
it as "not yet individually decompiled" and "not yet traced" at the time
the "EGR" framing was first applied -- i.e. the name predates any real
investigation of what it actually drives.

Structurally, this third variant is NOT similar to the other two
confirmed-EGR mechanisms at all: the other two write RAM port-mirror bits
(f0e6 bits 1/4/5) that get physically mirrored to P2DR.7/P4DR.4/PCDR.6 --
real, confirmed EGR-adjacent GPIO pins. This third variant instead writes
F494 -> scales/clamps it -> T6GR1H, a genuine Timer 6 PWM COMPARE
register, an entirely different hardware mechanism (PWM duty cycle, not
a digital GPIO bit). There is no code-level overlap between this PWM
path and the two confirmed-EGR GPIO paths other than sharing the same
upstream config-byte selector -- which only tells us "this ECU variant
uses ONE of these 3 mechanisms for whatever this selector controls," not
that all 3 necessarily control the same physical system.

CONCLUSION: the "EGR" label for cmd 0xCD/0xCE (egr_position_target_f494_
calc / T6GR1H PWM chain) was NEVER independently confirmed against a
real pinout or hardware reference -- it was inherited purely from being
grouped under the same config selector as two unrelated, separately-
confirmed EGR mechanisms. EvoScan's "Radiator Fan High/Low" label is a
real, independent source (a working diagnostic tool used against real
Mitsubishi ECUs of this family) and is NOT contradicted by any actual
evidence in this project -- only by an unverified assumption. Given
radiator fans are commonly PWM-duty-controlled (fan speed control) while
EGR valves on this era of Mitsubishi ECU are more commonly GPIO/relay or
stepper-driven (matching the OTHER two confirmed EGR mechanisms' GPIO
style), "Radiator Fan" is arguably the MORE PLAUSIBLE physical identity
for a PWM-duty output specifically, not less.

RECOMMENDATION: treat "EGR" as the likely-incorrect label going forward.
Ghidra function/variable names using "egr_...f494..." for this specific
third-variant chain should be understood as probably mislabeled --
worth renaming to something neutral (e.g. "timer6_pwm_actuator_target")
or to "radiator_fan" once/if further corroborated, rather than continuing
to propagate "EGR" as fact. NOT renaming yet in this entry -- flagging
the finding first; will apply renames in a follow-up action if desired.

This is the SAME pattern already seen once before in this project (MUT
RequestID 0x84/F495, previously labeled "RadFans" by a DIFFERENT vendor
profile (GalantLegnum XML) and found to actually be this same EGR-labeled
F494/F495 word -- interesting that the EARLIER mislabel was ALSO
"RadFans", on the exact same underlying RAM cell, from a different
source than EvoScan. Two independent vendor-side sources (GalantLegnum
profile AND EvoScan) both call this mechanism "radiator fan"-related,
against one internally-inherited, never-independently-confirmed "EGR"
label from this project. This strengthens the case that "radiator fan"
is correct and "EGR" was the error, not weakens it -- two independent
external sources agreeing against one unconfirmed internal assumption.


CONTRADICTION FOUND: EARLIER SESSION ALREADY CONCLUDED EVOSCAN'S
ACTUATOR REQUESTIDS DON'T MAP TO THIS DISPATCHER -- NEEDS RECONCILING
(2026-08-08, same session, found while re-reading
sci1_meta_cmd_dispatch_c0_ff's full plate comment)
============================================================
Re-reading sci1_meta_cmd_dispatch_c0_ff's existing, extensive plate
comment (dated 2026-07-22 for this specific point) surfaced a DIRECT
CONTRADICTION with this session's own actuator-table work:

  "MODE5 ACTUATOR REQUESTID CONCLUSION (2026-07-22): the RVR EvoScan XML
  profile's 8 Mode5 (Actuator Test) RequestIDs do NOT map onto anything
  live on this ROM. This EvoScan RequestID numbering is UNRELATED to
  this dispatcher's own native command byte values (0xC0-0xFF)."

This directly conflicts with THIS session's finding (see "ACTUATOR TEST
TABLE RECOVERED FROM EVOSCAN..." and "DISCREPANCY CHECKED..." entries
above) that EvoScan.exe's method_309 actuator list -- 13 RequestIDs
0xC3, 0xCD, 0xCE, 0xD3, 0xD6, 0xD8, 0xD9, 0xDA-0xDF -- overlap this
dispatcher's OWN native 0xC0-0xFF command byte range, and that 0xC3/
0xCD/0xCE specifically were cross-checked against already-confirmed ROM
behavior (F512 bit6 / F510 bit12 / F510 bit11 respectively).

POSSIBLE EXPLANATIONS, NOT YET RESOLVED:
  (a) The earlier (2026-07-22) session checked a DIFFERENT EvoScan
      RequestID source -- specifically "the RVR EvoScan XML profile's
      Mode5 RequestIDs" -- which may be a SEPARATE mechanism from
      method_309's hardcoded actuator list found via decompiling
      EvoScan.exe THIS session. EvoScan may have multiple, independent
      code paths for actuator testing (one XML-profile-driven "Mode5"
      system, one hardcoded-per-protocol-string method_309 system) that
      don't necessarily use the same RequestID numbering as each other,
      let alone the same numbering as this ROM's native dispatcher.
  (b) One of the two sessions' conclusions is simply wrong -- either the
      2026-07-22 "do NOT map" conclusion was based on incomplete
      evidence (an XML profile that doesn't reflect what method_309
      actually sends over the wire), or THIS session's "0xC3/0xCD/0xCE
      match" cross-check was a coincidence / wrong inference.
  (c) Both are correct in their own narrow scope: the XML PROFILE's
      Mode5 numbering (whatever it is) genuinely doesn't match, while
      method_309's ACTUAL wire-level bytes (confirmed via method_60
      calls, which do send literal hex strings like "3B"/"3C"/"3D" onto
      the serial link) DO match, because method_309's numbers ARE the
      real bytes EvoScan puts on the wire, whereas an XML profile might
      contain a different internal ID scheme translated by other code
      not yet examined.

THIS MATTERS A LOT for the original "how does live MUT dispatch work on
this ROM" question -- if explanation (c) is right, method_309's numbers
are the real ground truth (actual wire bytes) and this session's cross-
check against 0xC3/0xCD/0xCE holds up as genuine evidence of a working
live dispatch path via sci1_meta_cmd_dispatch_c0_ff -- meaning the
"MUT dispatch mechanism" isn't actually a mystery at all: it's THIS
function, already fully reverse-engineered in prior sessions, and this
session's EvoScan work is corroborating it, not finding something new.

NOT YET DONE: locate and re-read whatever this project's records say
about "the RVR EvoScan XML profile's 8 Mode5 RequestIDs" specifically
(search this file and any linked profile/XML files for "Mode5") to
understand exactly what was checked in the 2026-07-22 finding, and
determine whether it's the same or a different mechanism from
method_309's hardcoded actuator list found this session.


CONTRADICTION RESOLVED: TWO DIFFERENT EVOSCAN SOURCES, NOT A REAL
CONFLICT (2026-08-08, same session)
============================================================
Re-read the earlier (2026-07-22) "FINAL ANSWER, Mode5 (Actuator Test)
RequestIDs" section in full to resolve the apparent contradiction flagged
in the previous entry. RESOLVED -- explanation (a) from that entry was
correct, no real conflict:

The 2026-07-22 session checked EvoScan's XML PROFILE data (a generic
Mitsubishi MUT-II EFI template, RequestIDs 0x01-0x1A: Fuel Pump Relay,
EGR Solenoid, Purge Control Solenoid, A/C Relay, Condenser Fan Hi/Lo,
Ignition Timing Fix, ISC Step Fix). Correctly found NONE of those 8
low-numbered IDs correspond to anything live on this ROM -- neither the
<0xC0 pointer table (unprogrammed) nor the 0xC0-0xFF dispatcher (wrong
value range entirely, table starts at 0xC0 not 0x01) has anything there.

THIS session (2026-08-08) checked a COMPLETELY DIFFERENT EvoScan source:
the actual C# code in method_309 (frmMain.cs), decompiled via de4dot+
ILSpy, which sends real hardcoded actuator command BYTES over the wire
via method_60(hexString, 12) -- 0xC3, 0xCD, 0xCE, 0xD3, 0xD6, 0xD8, 0xD9,
0xDA-0xDF. These land squarely in the SAME 0xC0-0xFF range this project
already fully reverse-engineered from the ROM side, and 3 of them
(0xC3/0xCD/0xCE) were independently cross-checked against already-
confirmed ROM behavior in this session and matched.

CONCLUSION: no contradiction. Two genuinely separate EvoScan data
sources were checked, at different times, covering different (non-
overlapping) RequestID ranges:
  - XML profile "Mode5" IDs (0x01-0x1A) = confirmed DEAD on this ROM
    (2026-07-22 finding stands, unchanged).
  - method_309's real wire-level actuator bytes (0xC0-0xFF range) =
    genuinely LIVE and MATCH this ROM's already-documented
    sci1_meta_cmd_dispatch_c0_ff behavior (this session's finding
    stands, unchanged).
The XML profile and the actual C# actuator-send code are apparently two
independent systems within EvoScan itself (unsurprising -- XML profiles
are often generic/shared templates across many ECU variants, while
hardcoded per-protocol-string C# lists like method_309 are more likely
tuned per-platform). This project's "MUT dispatch mechanism" is NOT
actually unresolved for the 0xC0-0xFF actuator range -- it was already
fully reverse-engineered in prior sessions (sci1_meta_cmd_dispatch_c0_ff,
exhaustively documented, every command byte's effect known). This
session's EvoScan work independently corroborates that existing finding
for the actuator-test side rather than discovering something new.

WHAT REMAINS GENUINELY UNRESOLVED (restating clearly, now that the
false contradiction is cleared up): the Mode-2 DATA-READ side (arbitrary
live RequestID -> sensor value response, as opposed to the fixed-set
actuator/DTC commands covered above) still has no confirmed live
dispatch path on this ROM -- adc_sensor_convert_single's only 2 callers
remain the internal fixed-request loggers (mut_configurable_reqid_
backup_snapshot_5word / _periodic_snapshot_8word), and the <0xC0 pointer
table that would normally hold a live Mode-2 handler is unprogrammed.
THIS is the real open question, not the actuator dispatch mechanism
(which is solved) or the DTC read mechanism (which is now reasonably
well understood per this session's EvoScan work, modulo the F125/0x3D
writer still being unfound and the model-year applicability caveat).


DIRECT ANSWER FOUND: *MUT2BYTE / MATSCALED / MAPSCALED ARE NOT REAL LIVE
DATA IN THE ACTUAL LOG (2026-08-08, same session)
============================================================
Went back to the original open question (do the EvoScan log's MATScaled/
MAPScaled/LoadMUT2Byte/RPMMUT2Byte/AirFlowMUT2Byte columns hold real
live data?) and checked the actual CSV values directly rather than just
inferring from column names, using
"test\rvr\ecu logs\EvoScanDataLog_2026.07.04_14.23.02.csv" (204 rows,
confirmed a real drive/idle session via the genuine RPM column: 76
distinct values, plausible progression).

RESULTS -- checked distinct-value counts and actual numbers across all
204 rows:
  - MATScaled: CONSTANT at -40 for all 204 rows. -40 is a classic
    "sensor fault/no-reading" default value, not real intake air temp.
  - MAPScaled: CONSTANT at 0 for all 204 rows. 0 kPa is implausible for
    a running engine -- not real manifold pressure data.
  - RPMMUT2Byte: only 2 distinct values across all 204 rows
    (229007.8125 and 197007.8125) -- physically implausible as RPM
    (a real engine doesn't run at 229,000 RPM); compare to the genuine
    RPM column's 76 distinct, plausible values in the SAME log. Not real
    data.
  - LoadMUT2Byte: only 4 distinct values, two near-identical pairs
    (3364.6875/3364.0625 and 2724.6875/2724.0625) -- also not
    consistent with genuine continuously-varying live sensor data.
  - AirFlowMUT2Byte: 45 distinct values -- more variation than the
    others, not yet individually assessed for plausibility, lower
    priority given the pattern from the other 4 channels.

CONCLUSION (settles the question definitively, no longer needs the
EvoScan-source-code angle): in this REAL, ACTUAL log capture, from
whatever real hardware attempt produced it, MATScaled and MAPScaled
truly are NOT successfully read live data -- they're constant fault/
default values, DIRECTLY CONFIRMING the earlier Ghidra-side finding
(EEDF/EEE1 have no writer anywhere in this ROM, verified 3 independent
ways). RPMMUT2Byte and LoadMUT2Byte show the same signature (implausible,
barely-varying values) strongly suggesting the same story: these MUT-
table-indexed "2Byte" channels are not successfully read live either on
whatever hardware/ROM combination produced this log.

This resolves the "MAJOR RECONCILIATION...contradicts static analysis"
tension raised earlier in this file when the log was first found:
EXPLANATION (b)/(c) from that entry is now CONFIRMED correct --
EvoScan's own log shows it did NOT get real data for these channels,
meaning there's no actual contradiction with the ROM-side "no writer
found" conclusion. The log's column NAMES suggested live MUT reads were
happening; the log's ACTUAL VALUES show they were not.

CAVEAT: this is ONE log file, from an unknown vehicle/ROM-version
combination (not confirmed to be THIS exact ROM file --
RVR_1998_x3 4g63t 21000011 md352553.hex -- vs a similar but different
calibration). The other two EvoScanDataLog CSVs in the same folder were
not yet checked this pass -- worth a quick cross-check for consistency,
but given how clean and consistent this result is (constant fault values,
matching the independently-derived ROM analysis exactly), not expected
to change the conclusion.

NOT YET DONE: check the other 2 log files for consistency; assess
AirFlowMUT2Byte's 45 distinct values properly (currently unclear if
real or also junk); if these logs' provenance (which vehicle/ROM) can be
established, confirm whether it matches this exact ROM file.

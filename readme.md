# Hitachi H8/500 Processor Module for Ghidra

A Ghidra processor module for the Hitachi H8/500 family, with full support for the **H8/539F**.

These processors are screenprinted as **MH7202** and **MH7203** in Mitsubishi ECUs (RVR X3, Evo5/6, Galant/Legnum, etc.).

Forked and substantially extended from [c4ashley/ghidra-h8-500](https://github.com/c4ashley/ghidra-h8-500)
(which itself extended [Hurricos](https://git.laboratoryb.org/hurricos/ghidra-h8-500)).

H8/539F-specific work by [professor-jonny](https://github.com/professor-jonny).

## Features

- Full H8/539F instruction set (cross-checked against IDA SDK `src/module/h8500/`)
- Working decompiler -- produces C output via Ghidra's decompiler window
- 32-bit flat address space (`H8:BE:32:H8539F`) matching the chip's maximum mode
- Correct ROM memory map: 128KB mapped at `0x10000-0x2FFFF` (per hardware manual)
- On-chip RAM at `0xEE80-0xFFFF` created automatically by the setup script
- Full H8/539F peripheral register map in `h8539f.pspec` (SCI1/2/3, timers, ADC, ports)
- Also supports H8/520 and H8/538F (separate language IDs)
- MAP3/MAP4/MAP5 dispatch fully implemented (single-operand, bit-op, and immediate-EA
  instruction families), split across `h8539f-arith.sinc`, `h8539f-logic.sinc`,
  `h8539f-mem.sinc`, `h8539f-bit.sinc`, `h8539f-branch.sinc`
- Function-start pattern matching (`h8539pattern.xml`, wired into `patternconstraints.xml`
  for `H8:BE:32:H8539F`) -- auto-detects function starts from compiler prologue idioms
  (`link FP,#imm`, `stm (reglist),@-SP`) and far-return boundaries (`prts`, `prtd`),
  the automated equivalent of the manual "search for 11 19" technique
- Setup script (`h8539_ecu_master_setup_new.py`) marks the interrupt vector table
  (`0x10000-0x1013F`) as data and auto-creates functions at every unique handler address
  it points to, using the vector table itself rather than pattern/byte searching

## Install

1. Make sure Ghidra is installed. Note the path to its install directory.
2. Edit `h8\data\languages\Makefile` -- set `GHIDRADIR` to your Ghidra install path.
3. Run `make && make install` from the `h8\data\languages\` directory.

Alternatively, copy the contents of `h8\data\languages\` into:
```
<GhidraInstall>\Ghidra\Processors\h8\data\languages\
```
and run `sleigh.bat` (Windows) or `sleigh` (Linux/Mac) from `<GhidraInstall>\support\`
to compile the `.sla`:
```powershell
.\sleigh.bat "<GhidraInstall>\Ghidra\Processors\h8\data\languages\h8539f.slaspec" `
             "<GhidraInstall>\Ghidra\Processors\h8\data\languages\h8539f.sla"
```

## Optional: R0-R5 displacement variant (placeholder)

`optional\r0r5-add-before-segment\` holds replacement copies of `h8539f.slaspec` and
`h8539f-arith.sinc` that fix the bank of R0-R5 displacement addressing (the displacement is
added before the segment, as the FP/SP forms already do). It is **not** the default: it has
known decompiler regressions (one function stops decompiling, and more `undefined*` types
and casts appear), so it is kept as a placeholder until the problem is corrected properly.
To try it, copy the two files over the ones in `h8\data\languages\`, delete `h8539f.sla`,
and restart Ghidra. See that folder's `README.md` for the details and how to go back.

## Opening a ROM in Ghidra

### 1. Import

- **File > Import File**, select your `.hex` or `.bin` ROM file.
- Format: **Raw Binary**
- Language: **H8:BE:32:H8539F**
- Under **Options**, set **Base Address** to `0x00010000`

  > This is critical. The H8/539F's ROM is physically mapped starting at H'10000, not H'00000.
  > Getting this wrong will cause all far-call targets to be off by 0x10000.

### 2. Do NOT click Analyze yet

Click OK to finish the import, then double-click the file to open it in the Code Browser.
**Do not click "Analyze" when prompted** -- use the setup script instead.

### 3. Run the setup script

In Ghidra's Script Manager (`Window > Script Manager`), find and run:

```
h8539_ecu_master_setup_new.py   (category: MitsubishiECU)
```

The script presents a GUI dialog for each step so you can toggle them individually.
It will:

1. Validate the import base address (aborts with a clear message if wrong), and print a
   ROM signature banner (first 16 bytes) to help tell ROMs apart across sessions
2. Create the on-chip RAM memory block (`0xEE80-0xFFFF`)
3. Set CP/DP/TP context registers over both ROM pages
4. Read the reset vector and create the `entry` function
   - **4b.** Mark the interrupt vector table (`0x10000-0x1013F`) as data, auto-create
     functions at every unique handler address it points to, flag duplicate targets
     that aren't the shared default stub, flag targets outside ROM, and flag any ISR
     whose last instruction isn't `rte`/`sleep`/`rts`/`prts`/`prtd`
   - **4c.** (report only) Scan the ROM for `prts`/`rts` boundaries not already at a
     defined function and print candidate function-start addresses to review manually
5. Test the decompiler -- aborts if it fails so you don't waste time on a broken analysis
6. Run auto-analysis (finds ~600+ functions on a typical ECU ROM)
   - **6b.** (report only) Scan for contiguous undefined-byte gaps (>=16 bytes) left
     after auto-analysis and print them
7. Apply EcuFlash XML table labels (optional -- file browser prompt, ROM ID verified before labelling)
8. Run the ROM header scraper (optional -- MUT table, scaling tables, 2D/3D value tables by byte pattern)
   - **8d.** (report only) Print a count of labeled symbols in page 2 (`0x20000+`) as a
     reminder that the scraper only scans page 1

### 4. EcuFlash XML labels (optional)

If you have an EcuFlash ROM definition XML for your specific ECU, select it via the
file browser when prompted at Step 7. The script verifies the ROM ID against the XML
before applying any labels -- a mismatch skips labelling with a clear warning.

Tables are labeled at their **header address** (the address code xrefs point to), not
the raw data address in the XML:

- 2D tables: labeled at `xml_addr - 4`
- 3D tables: labeled at `xml_addr - 7`
- 1D scalars: labeled at `xml_addr` directly (no header)

`<include>` references in the XML are followed automatically.

If no XML is selected, the script offers to run the ROM scraper as a fallback.

### 5. Finding additional functions

Auto-analysis now uses `h8539pattern.xml` to detect function starts automatically from
compiler prologue idioms and far-return boundaries (`prts`/`prtd`), so most of what used
to require a manual search is found during auto-analysis itself. The setup script's
**Step 4c** also runs an automated version of the manual search below (report only --
it prints candidates rather than creating functions, since a byte scan can false-positive
inside data). If code is still missed after both of those:

- Search Memory (`Search > Memory`) for the byte pattern `11 19` (mnemonic `prts` --
  the far-call return instruction used by most functions). Look for hits not already
  inside a defined function.
- Similarly search for `19` alone (`rts`, near-call return).
- Or check **Step 6b**'s undefined-byte gap report, which lists leftover gaps after
  auto-analysis regardless of what caused them.

## File layout

```
h8\data\languages\
    h8539f.slaspec      -- H8/539F instruction set (SLEIGH source, top-level)
    h8539f-arith.sinc   -- arithmetic instruction family (add/sub/cmp/mulxu/divxu/etc, MAP5)
    h8539f-logic.sinc   -- logic instruction family (and/or/xor/andc/orc/xorc, MAP5)
    h8539f-mem.sinc     -- move/load instruction family (mov/ldc, MAP5)
    h8539f-bit.sinc     -- bit-test/set/clear instruction family
    h8539f-branch.sinc  -- branch/jump/return instruction family (bra, jmp, rtd, prtd, etc)
    h8539f.sla          -- compiled SLEIGH binary (generated by sleigh)
    h8539f.pspec        -- processor spec: peripheral registers, default symbols
    h8539f.cspec        -- compiler spec: calling conventions, stack, global ranges
    h8539pattern.xml    -- function-start byte patterns (prologues, prts/prtd), wired
                            into patternconstraints.xml for H8:BE:32:H8539F
    patternconstraints.xml -- maps each language ID to its pattern file
    h8.cspec            -- shared compiler spec for H8/520 and H8/538F
    h8.ldefs            -- language definitions (all variants)
    h8520.slaspec       -- H8/520 instruction set
    h8538f.slaspec      -- H8/538F instruction set
    h8520.pspec         -- H8/520 processor spec
    h8538f.pspec        -- H8/538F processor spec

test\
    h8539_ecu_master_setup_new.py  -- Ghidra script: full ROM setup with GUI (run this first)
    rvr\                -- example ROM files and EcuFlash XMLs

optional\
    r0r5-add-before-segment\  -- OPTIONAL placeholder: replacement h8539f.slaspec and
                                 h8539f-arith.sinc (see its README.md); not the default

datasheets\h8539f\      -- Hitachi hardware/programming manuals

source\ida-sdk-main\src\module\h8500\ana.cpp  -- ground-truth opcode reference (IDA SDK)
```

## Xref-repair utility scripts

This ROM addresses tables/axes/functions via literal 16-bit immediates pushed to the
stack immediately before a far call (`mov #offset,@-SP` / `mov #bank,@-SP` / `pjsr`),
never through an address-mode operand. Ghidra's static xref engine is structurally
blind to that pattern -- `getReferencesTo()` returns zero hits for these call sites no
matter how well-typed the calling function is. The following scripts (in
`ghidra scripts\`) repair this by scanning raw instructions for the push pattern and
writing real `DATA` references into Ghidra's reference manager, so the xref/call-tree
views reflect the ROM's actual callers. All three use
`ReferenceManager.addMemoryReference(...)` with `SourceType.USER_DEFINED`:

- **`FixBankOffsetReferences_v2.java`** -- general-purpose. Targets are supplied by
  function name (resolved at runtime via the symbol table, so it's ROM-agnostic) rather
  than hardcoded addresses. For each named target, scans for the literal push pattern
  before its call sites, computes the real bank-adjusted address, and adds a `DATA`
  xref at the push site if one isn't already there.
- **`ResolveTableReadIndexedArrays.java`** -- specific to `table_read_indexed`'s
  array-base indirection. Adds an xref at the call site (to the array base) plus one at
  each resolved array slot (to the actual target address), same
  `array[index & 7]` / 4-byte `{pad,bank,hi,lo}` slot pattern as
  `table_lookup_indexed`.
- **`ResolveTable3AxisWrapperArrays.java`** -- same technique, retargeted at
  `table_3axis_interp_triple_wrapper`.

Run these from Ghidra's Script Manager after any pass that adds/renames functions
whose callers you want reflected in the xref database. They only add references (never
remove/overwrite existing ones with different source types), so re-running is safe.

Related **read-only** investigation/reporting scripts (no xref writes, console output
only): `FindAxisCallers.java`, `FindBatch2Callers.java`, `FindBatch2Axes.java`,
`FindAllTableCallsVsXml.java` / `FindAllTableCallsVsXmlV2.java`, `DumpBatch2Headers.java`,
`Find1DScalarCandidates.java` / `Find1DScalarCandidatesV2.java`. `AnnotateBankOffsetCallSites.java`
writes human-readable EOL comments at call sites (bank-adjusted address) but does not
touch the reference database.

## References

- Hitachi H8/538-539 Hardware Manual (OMC942723072) -- `datasheets\h8539f\H8 538-539.pdf`
- H8/500 Series Programming Manual -- `datasheets\h8539f\H8 500 programming.pdf`
- EcuFlash ROM definitions: https://github.com/EcuFlash/OpenECU (community ROM XMLs)


# Review 13 — H8/500 Paged/Banked Addressing Issues

## Context

While auditing `h8/data/languages/h8539f.pspec` against the H8/538-539 and
H8/539F Hardware Manuals, and reverse-engineering the RVR transmission ROM
boot loader, we hit two related but distinct problems caused by the same
root cause: Ghidra's flat address model does not understand H8/500
page-register-relative (`CP`/`DP`/`EP`) addressing.

---

## Issue A — Symbol collision across page contexts (fixable in this repo)

### Symptom

`DAT_0001f394` is read by `boot_sci1_rom_id_verify` (assumes `DP = 0x1`) as
a static ROM-ID-to-baudrate lookup table, but the *same* symbol is also
written by three unrelated calls in `tcu_shift_torque_and_knock_mgmt` and
read by `tcu_shift_timing_state_machine` elsewhere in the main vehicle
firmware — code that almost certainly runs under a different active page
register state.

Xrefs to `ram:0xf394`:
```
00029d1c  tcu_shift_timing_state_machine        [READ]
00029503  tcu_shift_torque_and_knock_mgmt        [WRITE]
00029509  tcu_shift_torque_and_knock_mgmt        [WRITE]
00029510  tcu_shift_torque_and_knock_mgmt        [WRITE]
00029517  tcu_shift_torque_and_knock_mgmt        [WRITE]
0002010b  boot_sci1_rom_id_verify                [DATA]
```

Nothing in the boot path ever writes `0xF394` before
`boot_sci1_rom_id_verify` reads it, which only makes sense if the boot-time
reference and the TCU-time references are actually two *different*
physical locations that happen to share the same flat 16-bit literal.
Ghidra has no way to tell them apart because it treats `0xF394` as one
address regardless of which page register (`CP`/`DP`/`EP`) was active when
each reference was made.

### Root cause

Confirmed via `get_current_program_info`: only 2 memory blocks are defined
(`RAM: 0000ee80-0000ffff`, `ram: 00010000-0002ffff`), and the disassembly
consistently shows `assume CP = 0x2 / DP = 0x1 / TP = 0x0` headers per
function — meaning the disassembler is *tracking* the page-register
assumption per function already, but the pspec/slaspec never turns that
into distinct address resolution. Every reference collapses onto one flat
space.

**UPDATE — verified 2026-08-15:** `h8539f.slaspec` line 514 already defines
`addr16_dp` with the correct DP-fold:

```
addr16_dp: addr16 is addr16 { local addr:4 = (zext(DP) << 16) | (addr16); export addr; }
```

Cross-checked against the H8/538-539 Hardware Manual, Table 3-8 "Effective
Address Calculation" (grepable datasheet, lines 1964-1989):

| Mode | Page register | Confirms |
|---|---|---|
| `@Rn`, `@(d:8/16,Rn)`, `@–Rn`, `@Rn+` | `DP` (R0-R3) / `EP` (R4,R5) / `TP` (R6,R7) | matches existing `Rn_banked` in `h8539f-branch.sinc` |
| `@aa:8` (short absolute) | `BR` | matches `ana.cpp`'s `aa8()` (`aux_page` + `insn.get_next_byte()`) |
| **`@aa:16` (absolute)** | **`DP`** — manual shows `23‑15‑0 / DP / EA extension data` | **matches our `addr16_dp` formula exactly** |
| PC-relative `d:8`/`d:16` | `CP` | matches existing `jmp`/branch pcode |

**So `addr16_dp`'s formula is architecturally correct, confirmed by the
hardware manual itself.** The `DAT_0001f394` collision is *not* a wrong
formula — it's that `DP` is a genuine runtime CPU register (not a Sleigh
`context` field), so Ghidra's static xref/symbol database can't reliably
resolve `zext(DP)<<16 | addr16` to one concrete `Address` unless the
decompiler's constant-propagation happens to pin down `DP`'s value within
that specific function (which is what produces the `assume DP = 0x1`
annotation on `boot_sci1_rom_id_verify` — but this is local, per-function,
best-effort analysis, not guaranteed, and definitely not shared/reconciled
against other functions like the unrelated `tcu_shift_*` code that also
happens to reference literal `0xF394`).

**Also worth noting:** `ana.cpp`'s `aa16()` (IDA reference module,
`h8500/ana.cpp` line ~211) sets only `aux_disp16`, never `aux_page` — i.e.
it silently drops the `DP` page fold that the hardware manual confirms
`@aa:16` actually has. Since `ana.cpp` is a work-in-progress reference
implementation, not authoritative, this looks like a bug/gap in the IDA
module rather than evidence against our Sleigh. Worth fixing there too if
`ana.cpp` is meant to stay a faithful reference (flag `aux_page` for
`aa16`, sourced from `DP` rather than `BR`).

### Proposed fix (in-repo, Sleigh-level)

This is standard, well-precedented work for banked-memory processors in
Ghidra (comparable to Z80 bank switching or x86 real-mode segment handling
in other processor modules):

1. Declare `CP`, `DP`, `EP` as SLEIGH `context` register fields
   (`define context contextreg ... CP=(...) DP=(...) EP=(...)`) if not
   already modeled that way in `h8.slaspec` / `h8500.sinc` — check current
   state first.
2. Have the relevant addressing-mode constructors propagate the active
   page context so that a `DP`-relative (or `CP`-relative) reference
   resolves through a context-qualified calculation rather than a bare
   16-bit literal — i.e. the same textual operand can produce different
   `Address` objects in different page contexts.
3. Alternatively/additionally: define distinct logical address spaces per
   page and have the loader/analyzer route `DP=1`-context reads to one
   space and default-context reads to another, so Ghidra's symbol table
   naturally keeps `DAT_..._page1` and `DAT_..._pageDefault` separate
   instead of colliding.
4. Re-run analysis and confirm `boot_sci1_rom_id_verify`'s read and the
   `tcu_shift_*` read/writes resolve to distinct symbols.

This is real Sleigh/pspec/cspec work, self-contained to this repo, no
Ghidra core changes required.

### Also check while doing this

- Whether other `DAT_0000fxxx`/`DAT_0001fxxx` symbols across the ROM have
  the same collision (cheap to audit via `get_xrefs_to` + checking the
  `assume CP/DP/EP` header of each referencing function once the above
  is in place).
- Fix `ana.cpp`'s `aa16()` (h8500 module, ~line 211) to set `aux_page`
  sourced from `DP`, matching the hardware manual's Table 3-8 and our
  `addr16_dp` construct — currently it only sets `aux_disp16` and silently
  drops the page fold, which is a real gap if `ana.cpp` is meant to stay a
  faithful IDA-side reference alongside the Ghidra module.
- Hardware citation for the record: H8/538-539 Hardware Manual (OMC942723072),
  Table 3-8 "Effective Address Calculation", grepable datasheet
  `H8_538-539_hardware.md` lines 1964-1989.
- The `SCI1_BAUDRATE_CLOCK_LUT` at `ram:0x20134` (word[9], values
  `0202,0303,0404,0507,0A0F,1819,1F2F,251F,2833,3865,48CC,0000`) still has
  **zero xrefs** anywhere in the program (byte-pattern search for an
  immediate load of its low 16 bits, `0134`, found no real instruction
  matches). Once page-context resolution is fixed, re-check whether this
  table becomes reachable from somewhere we currently can't see (e.g. a
  reference that's currently mis-resolving to a different page).

---

## Issue B — Split far-pointer call arguments (NOT fixable via Sleigh)

### Symptom

Calling conventions on this processor sometimes pass a far pointer as two
separate immediate pushes to the stack (page byte, then offset word)
ahead of a call, rather than as one fused value. The decompiler never
recombines these into one logical pointer parameter — it shows two
independent scalar arguments at the call site with no resolved
cross-reference to the actual combined target address.

### Upstream tracking

Already filed against this exact repo:
**https://github.com/NationalSecurityAgency/ghidra/issues/9450**
("Decompiler cannot recover far pointers split across multiple call-site
stack arguments (paged/banked architectures)")

The issue includes a full root-cause writeup in the decompiler's C++
source (`fspec.cc`, `coreaction.cc`, `userop.cc`) and a proposed
(unimplemented) fix: a new `<farpointerjoin>` cspec element plus a new
`Action` that unconditionally joins declared adjacent parameter slots at
call sites, modeled on the existing `SplitVarnode::createJoinedWhole()`
precedent used for double-precision-in-two-registers cases.

### Why Sleigh can't fix this

Sleigh only defines **per-instruction** p-code semantics. This bug is a
**cross-instruction, call-site-level** fusion problem — recognizing that
two separate push instructions, several instructions apart, followed by a
call, together form one logical far pointer. That analysis lives in the
decompiler's C++ engine (`ActionParamDouble`, `checkInputJoin()`,
`doInputJoin()`, `Architecture::constructJoinAddress()`), which no
`.slaspec`/`.pspec`/`.cspec` file can influence — only a compiled Ghidra
core change can.

### Options

1. **Track upstream** — wait to see if #9450 gets picked up/implemented
   by the Ghidra team.
2. **Manual annotation** — survey the ROM for the split-push-then-call
   pattern and hand-comment/label affected call sites so a human reader
   isn't misled by the decompiler's flattened two-scalar-argument view.
   (Not yet done — would need a byte-pattern/pattern-matching survey to
   scope how widespread this is in the RVR ROM.)
3. **Implement it ourselves** — actually build and PR the proposed
   `<farpointerjoin>` mechanism upstream. This is a real C++ contribution
   to Ghidra core (new cspec element decode in `FuncProto::decode()`, new
   `Action` registered in `ActionDatabase::universalAction()`), separate
   in scope and effort from anything in this repo's Sleigh module. Doable,
   but a distinct, larger undertaking from Issue A above.

---

## Suggested priority

- **Do Issue A now.** It's self-contained, directly fixes a real bug we
  hit today (`DAT_0001f394` collision), and is standard Sleigh/pspec
  practice with precedent elsewhere in Ghidra.
- **Scope Issue B before committing to it.** First survey how often the
  split-far-pointer-call pattern actually appears in this ROM to judge
  whether (2) manual annotation is sufficient for now, or whether the
  pattern is common enough to justify (3) the upstream Ghidra
  contribution.

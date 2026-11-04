# 0001. Reverse workflow and reference binaries

**Status:** Accepted (2026-10-21)

## Context

Two engine builds are available locally and neither is tracked by git:

* `Tayutama2_trial_TG.exe` — the binary `src/raw/text_*.c` was exported from
  (via `reverse/BGI.sqlite`, git-ignored). Older build.
* `千の刃濤、桃花染の皇姫 体験版/senmomo_trial/BGI.exe` — AUGUST 2016,
  BGI 1.622 / Compat 1.72, the same version string FriendToLoverHD reports.
  It is a complete, runnable game, so it is the reference for behaviour and
  for anything the older build lacks (main opcodes 0x56..0x5F, the 0xD0
  table contents).

Earlier sessions kept their IDA output in the per-session scratchpad, which is
wiped; one interrupted session lost a day of 0xD0 reading that way.

## Decision

1. Every subsystem goes **raw export → cleaned C → Rust** (`src/README.md`).
2. Raw exports from the senmomo binary live in `src/raw/bgi1622/`, produced by
   `src/tools/ida_export_closure.py` (closure of one dispatch table: direct
   calls, functions taken by address, vtables). Both pseudocode (`.c`) and
   disassembly (`.asm`) are checked in, because Hex-Rays loses `__usercall`
   register arguments and x87 operand order; the asm settles those.
3. Cleaned C goes to `src/engine/<subsystem>.c` with a header comment naming
   the raw addresses it covers. The Rust port cites the same addresses.
4. Every multi-session effort gets an ADR with a **Progress / next step**
   section that is updated in the same commit as the code.

## Reference facts (BGI.exe 1.622, image base 0x400000)

* Native tables, 0x400 bytes each: 0x81 @0x503F00, 0x80 @0x504300,
  0xA0 @0x504700, 0x92 @0x504B00, 0x91 @0x504F00, 0x90 @0x505300,
  0xB0 @0x505700, 0xD0 @0x505B00, 0xC0 @0x505F00; main opcode table
  @0x506300 (loop `sub_48D860`).
* Operand helpers: `sub_445480` pops an int, `sub_48EA40` pops a pointer,
  `sub_4454A0` pushes the result, `sub_464C50` is the fatal script error.
  Handlers pop the last-pushed argument first.
* `80:ED/EE/EF` (`SystemExtensionQuery/Reset/Set`) are null in the 0x80 table.
  `sysprg.arc:scrsys._bp` only wraps them in library functions nobody calls;
  calling one is a fatal "unknown selector" on the target, so they stay
  unregistered.
* IDA database: build with
  `idat -A -B BGI.exe`, then export with the tool above
  (`SEL_TABLE=<table> SEL_GROUP=<hex> SEL_OUT=... SEL_ASM=...`).

## Running senmomo

Clone the game directory with `cp -c` into the scratchpad, skip `*.exe`, and
delete `SenNoHatouTsukisomeNoKoukiTV` (ipl.\_bp terminates when it exists).
Title at about frame 1200; "開始" at (90,455). Always `ETHORNELL_HEADLESS_MUTE=1`.

## Consequences

* A new session reads the ADR index and the relevant `src/engine/*.c`, not old
  transcripts.
* `src/raw/bgi1622/` grows by one table at a time (`d0.*` first).

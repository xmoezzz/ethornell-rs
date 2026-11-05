# 0006. Houkago Cinderella FD runtime limits

**Status:** Startup limits verified (2026-11-05); other version differences open.

## Evidence

The supplied Japanese executable is identified by SHA256 in
`raw/houkago_cinderella_fd/sound_channels.asm`. The immutable disassembly
fragments cover the sound validator, A0:09, its gain-bank setter, the BGM
validator, and main-thread creation. Cleaned references are
`engine/sound_channels.c` and `engine/main_thread_regions.c`.

* A0:09 at `0x494900` checks volume before calling `0x4A6C30` to validate
  the channel. That validator compares the channel unsigned with 128.
  `0x4A11F0` independently checks the same limit before storing the primary
  gain bank at `0x58C370 + 4*channel`. Thus 78 and 80 are valid channels,
  including when no sound is loaded. BGM remains limited to 16.
* Main-thread creation at `0x49A5AE` passes 4096 operand slots, 8 MiB code
  and 4 MiB data to `0x49AC60`, which forwards them to `0x44C830`. The old
  512 KiB code limit rejects scrgrp7._bp during startup after 78 is fixed.

## Decision

Select these verified limits only when both decoded code resources match:

| Resource | Bytes | FNV64 |
|---|---:|---|
| system.arc:ipl._bp | 12608 | 46f725982b5b8cd6 |
| sysprg.arc:title._bp | 20608 | 603c84fe0ba8bc49 |

This extends the existing session version detector. Game directory names,
game IDs and text encodings do not select limits. Unknown versions retain
64 SE channels and the old main-thread limits. Existing Amachoco dispatch
profiles retain their own ABI. No new selector contracts are enabled here.

All SE banks, playback clocks, active flags and fade envelopes have the
selected size. Load, memory registration, playback, query and release share
that range. The BGM bank and Sys80:44's explicit child-thread sizes are
unchanged. Both desktop and restricted host constructors apply the profile.

## Validation and remaining work

The default SE tests still reject 64 and negative channels. The extended
bank test registers and plays 64, 78, 80 and 127, checks backend channel
mapping, clocks/fades and release, and rejects 128 and negative channels.
A module-loading regression confirms a 600 KiB module fails in the old
main thread and loads under the extended limit, while oversized modules
still fail. App and VM unit suites pass.

The authorized game copy, without the 16-byte launcher marker, reaches its
title menu in 1800 offscreen frames. The CLI builds and its initial frame
also runs successfully. Audio is muted in the game reproduction; the
regression checks emitted playback commands rather than device output.

Still open:

* 91:09 and 91:0A remain unimplemented warnings. This executable's table
  is at 0x501818; the handlers are 0x48E480 (one pop, one push) and
  0x48E4A0 (two pops, one push). Audit their helper chains before adding a
  dispatch profile rather than assuming the existing Amachoco semantics.
* The restricted host reaches a `BGI.edb` open-error prompt at frame 779.
  This is separate from the desktop startup errors fixed here.
* This change configures native logical region limits; the VM's existing
  fixed local backing windows remain unchanged. Accesses beyond their
  supported range need a separate memory-layout change. The title startup
  reproduction does not establish full-game compatibility.

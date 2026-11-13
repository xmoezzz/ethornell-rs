# 0006. Houkago Cinderella FD runtime limits

**Status:** Startup limits verified (2026-11-05); other version differences open.

## Follow-up: BackML source-position curve

The first heroine's route subsequently fails at Graph90:38, property 0x8E
in scrgrp3._bp. The original BackML vtable at 0x511A84 points to the setter
0x423C60, whose switch sends property 0x8E to 0x423A80. It stores one
curve selector at object+0x184+88*selected_layer and ignores the extra
operand. Existing rotation/scale fields and their curves are preserved.

The draw path at 0x423EB1 calls 0x420010 with that selector before
multiplying both source-coordinate deltas. The rotation and scale curves
follow separately. This is a source-motion curve, not a rotation override
or an unsupported property to ignore. Raw instructions and the switch
tables are in `raw/houkago_cinderella_fd/backml_source_curve.asm`; cleaned
reference is appended to `engine/backml.c`.

The Rust layer records an optional source curve. An absent selector keeps
the older engine's direct unsigned 8.24 multiplication. Once 0x8E is set,
the source offsets use its 16.16 curve weight; zero/unknown selectors use
the native signed linear fallback. A pixel regression samples a gradient
at half progress: the quadratic curve moves both source coordinates half
as far as the linear curve. The same test checks selected-layer isolation,
preservation of rotation/scale settings, and the ignored extra operand.

The first heroine reproduced the original fatal error before this change.
Afterward, the five heroine openings completed offscreen runs of 10000,
6500, 4600, 4600 and 4600 frames. A second first-heroine run used dense
ordinary Enter presses for 9000 frames and rendered 82 distinct nonempty
message strings. The first/second/fifth runs exercised property 0x8E.
No VM-fatal stop occurred in these runs. All 520 app unit tests pass, and
the CLI was rebuilt. Snapshots and input scripts are retained in the
2026-11-05 `houkago-cinderella-fd/play` visualization artifacts.

Deeper desktop runs also log 92:95 stubs and BGI.edb open/close prompts;
the stream failure is addressed below; 92:95 remains open. Passing these bounded replays
does not establish full-route compatibility or absence of modal prompts.

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
* This change configures native logical region limits; the VM's existing
  fixed local backing windows remain unchanged. Accesses beyond their
  supported range need a separate memory-layout change. The title startup
  reproduction does not establish full-game compatibility.

## Follow-up: Config sprite getter and diagnostic stream

Config calls Graph91:38 property 0x10 in syswndfnc._bp to obtain a sprite's
primary bitmap before compositing the menu. The supplied executable's
42E330 -> 42D8E0 returns the DWORD at object+0x160, without mode gating or
bitmap validation. This is a getter missing from the port, rather than a
property to ignore. The Rust getter returns the retained bitmap number
(or -1 before configuration), including after bitmap replacement.

BGI.edb is written through Sys81:28..2B. The old implementation tried to
read the file for every open mode and could not create it. file._bp then
retries 40 times at 100 ms intervals, explaining pauses before the open
error. Its close completion also incorrectly returned 0, whereas the
script waits for a positive completion.

The already recovered worker in raw/text_400000.c (4014D0) selects reads
for mode 0 and writes for modes 1/2; close and successful seek complete
with 1, transfer completes with a byte count, and failure/zero bytes with
-1. UINT_MAX seeks to EOF, and an offset beyond EOF fails. stream_io.rs
implements those results synchronously through the existing user-file
API, including create/truncate and open-or-create. It does not synthesize
a successful file open or suppress the script's diagnostic prompts.
Asynchronous worker timing is still outside this implementation.

Validation: 520 app and 294 VM unit tests pass. A 4100-frame native-clock
replay entered Config from the title, visited display/text/sound/system
pages and returned without a fatal error or diagnostic prompt. A separate
5000-frame restricted-host first-heroine replay completed without a host
fatal/prompt; its filesystem log confirms BGI.edb creation and append
writes in private storage. Both replays captured logs only. This is
bounded verification, not a full-route or graphics correctness claim.

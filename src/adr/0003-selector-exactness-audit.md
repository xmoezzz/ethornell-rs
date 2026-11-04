# 0003. Every native selector matches BGI 1.622 exactly

**Status:** In progress (started 2026-10-22)

## Context

The implementation table in `native_call.rs` marked 374 of the 741 selectors
of the senmomo `BGI.exe` (BGI 1.622) as `Partial` and 319 as
`PortableEquivalent`. A spot check on 2026-10-21/22 showed both labels are
unreliable:

* most `Partial` notes describe the target without naming a gap — nobody had
  compared them line by line;
* several `PortableEquivalent` selectors were wrong: 81:0F (IsIconic) always
  returned 0, 81:65 minimized the window instead of locking its position,
  A0:86 never wrote its output, 90:78 and C0:18 panicked;
* many notes cite addresses of the older Tayutama2 build, whose globals and
  helpers differ from 1.622.

The user's requirement: no `Partial`; every selector behaves like the target.

## Decision

1. Audit all 741 selectors (not only the `Partial` ones) against the 1.622
   binary, one at a time, in table order. Evidence: `src/raw/bgi1622/
   native_all.c/.asm` (the closure of all nine tables) through
   `src/tools/raw_fn.py`, and the IDA database for anything outside the
   closure (render loop, WndProc, startup). The Rust side is located with
   `src/tools/rust_arm.py`.
2. A selector is done when its script-visible behaviour is the target's:
   argument pop order and validation, fatal errors, status codes, every
   memory write, state changes and their timing, integer/x87 arithmetic.
   Where the target calls an OS service (files, dialogs, registry, shell,
   CPUID, timers, MCI, DirectShow, Flash), the same algorithm runs over the
   host's equivalent service; when the host has no equivalent the selector
   behaves as the target does when that service fails. Such cases are named
   in the selector's notes.
   File-system and path services are the exception: on non-Windows hosts
   they run over the emulated Windows namespace of
   [ADR 0004](0004-windows-filesystem-emulation.md), not the failure path.
3. Fix, then `src/tools/promote.py` with notes citing 1.622 addresses, then
   mark the row `verified` in `0003-selector-audit.csv`.
4. Static `.data`/`.bss` values are read from the PE file, not IDA
   (`get_wide_dword` shows 0xFFFFFFFF for `.bss`).

## Consequences

* `PortableEquivalent` means verified against 1.622 by this audit.
* Tests run with dialogs suppressed (`host_dialog::host_dialogs_suppressed`).

## Progress / next step

Checklist: [`0003-selector-audit.csv`](0003-selector-audit.csv)
(`selector,handler,symbol,status`).

Done so far (2026-10-22): 80:00–1F (minus 0x03), 80:58/59/5A/65, 81:0F/65/68,
90:78, 90:F3, A0:86, C0:18. Cross-cutting fixes made on the way:

* **Clock** (`time.rs`, `native_thread.rs`): sub_499290 is a wrapping 32-bit
  clock starting at the host uptime (0 for replays); thread deadlines use
  wrapping add and the signed distance of sub_445630; 80:59 extends the
  deadline instead of resetting it; 80:05 reads the host monotonic clock.
* **Window lifecycle** (`sync_host_window_state`): WM_ACTIVATE/WM_SIZE order;
  Sys81:68 freezes the clock on deactivation; minimizing mutes the primary
  BGM/SE banks and the global DirectShow movie and restores them afterwards;
  minimize latch (80:0E/80:65); IsIconic (81:0F); centre lock (81:65).
* **Input pump** (`apply_runtime_input_event`): Knob-captured left/right
  presses only move the accumulator; wheel counts before a Knob takes it;
  horizontal wheel (142/143); WM_SYSKEYDOWN/UP (Alt+Enter, Alt+F4, F10, Alt
  recorded on release); WM_CLOSE counts; 4x4 double-click box; aggregates
  193/194 follow the class 0x100/0x200 tables; lazy release in sub_46E0A0.
* **Frame-time accumulator** (80:06/07), **processor record** (80:0A/0B,
  `cpu_record.rs`), full display-mode size table.

Open items found on the way (to fix with their selector groups):

* Knob: right press sets Knob+0x0C (sub_463CE0 hit = pointer-chain scope
  test, not geometry); the Rust Knob hit test and +0x0C flag need the Knob
  audit (91:D*).
* DCIPIcon presses are handled in the Rust input event; the target polls the
  input records in sub_448690 (90:B*).
* Modal dialogs freeze the clock (sub_4992E0(1)); the portable dialogs block
  the frame loop instead, so only gaps under 500 ms still count.
* **VM value/memory model (blocking 80:20/21 and every pointer-producing
  selector).** BGI 1.622 values are raw 32-bit cells; addresses are
  segment<<26 | offset (sub_48E8C0): 0 global, 1 code (opcode 0x05 pushes
  `(pc_base+off) | 0x04000000`), 2 thread data (opcode 0x04 pushes
  `(base-off) | 0x08000000`), 3 object, 0x10–0x3F heap. The heap
  (sub_48E760/sub_48E800) has five classes (≤4 KiB/64 KiB/1 MiB/16 MiB/64 MiB;
  65536/4096/256/16/32 slots; segment bases 0x10/0x14/0x18/0x1C/0x20), takes
  the lowest free slot of the first fitting class and returns
  ((slot & mask) << shift) | ((base + (slot >> (26-shift))) << 26); size 0 or
  over 64 MiB is fatal, a failed allocation is fatal. The Rust VM instead
  uses typed values (Str/Func/Ptr), tag 0x12 for frame memory, tag 0x10 for
  string literals and a bump/free-list heap with its own tags. Script-visible
  pointer values therefore differ. Waiting on the user's decision whether to
  rewrite the VM core to the target model.
* The VM answers many 0x81 selectors itself; several app `call_sys` arms for
  0x81 are dead (81:65/68/69 seen) — sort these out in the 0x81 pass.

Resource I/O: 80:30..39 and 80:3B are done (`resource_io.rs`, C in
`src/engine/resource_io.c`); 80:3A is deferred on ADR 0004 (host side). Fixed
on the way (2026-10-22): 80:32 resolves root + name unless absolute, no
longer creates parent directories, and pushes 1 for length 0; 80:33 builds
"dir\\file" / primary root + file without an absolute check, read-only files
are refused; 80:38 null name -> 0; 80:3A mode 4 is CSIDL_PROGRAM_FILES and
modes 0/4 report 1 even without a path; 80:3B pop order was wrong (target:
mode, initial dir, title, extension, description, buffer) and the 780-byte
buffer is now cleared first. senmomo replay still creates `UserData\` itself
(Sys80:28) before writing.

Continued 2026-10-22 (second pass): 80:3C..3F, 45..4C, 50, 52..54, 5C..5F
verified; 80:40/41/44 deferred (sub_4450B0 returns the code-segment offset
of the loaded module — blocked on the VM value model). Fixes:

* 80:3C: the VM rewrites the file operand to primary root + file before the
  host (sync dialog or async request) sees it; OK|CANCEL with the script's
  caption, Cancel -> quit question ("中止しますか？" on Japanese systems,
  sub_46FC50), YES -> 0. Shared host helper `retry_or_quit_prompt`.
* 80:3D kind 1 returns 0 without a media root; 80:3E stores "%s\\" as is.
* 80:3F ported into the VM (`sys80_3f_find_media`): drives taken once, 20
  tries per round (Sleep(50) only without drives), media root = drive +
  sub_465280(marker) (trailing "\\" only for one-level markers); new host
  hooks removable_drive_roots/path_exists/retry_or_quit_prompt/sleep_ms; the
  old heuristic search (game dir, cwd, caption as marker) is gone.
* 80:4A/4B: 1.622 fatal texts (無効なスレッドハンドル/メッセージ数); 4B no
  longer clamps to 0..256 (count < 1 fatal, no upper bound).
* 80:50 ends the thread's slice (handler status 1).
* 80:54: pushes wParam then lParam (was reversed), -1/-1 when cancelled,
  newest message wins and is not consumed (every waiter is signalled).
  Which Win32 messages the host synthesizes is tracked in ADR 0004.
* 80:52 only gates the main loop's idle/raster wait (sub_48D480): pacing,
  not script-visible.
* `cthread.c` cites Tayutama2 globals/strings in places (566894, 4EB9BC);
  1.622 uses 5668A8 (switch target), 5668AC/5668B0 (exclusive thread).

IDA: copy an existing `bgi.i64` from an older scratchpad
(`find /private/tmp/claude-501 -name bgi.i64`); scripts for xrefs /
decompile / asm ranges are easy to recreate (idat -A -S<script> bgi.i64).

80:61 (dword_565EA8 = fullscreen flag of sub_45F8A0) and 80:62 (1.622 fatal
text) verified.

Third pass (2026-10-22): 80:63/64/66..69, 6A..6F, 74, 78..7B verified;
80:60, 70, 71 deferred. Fixes:

* 80:60: size index / pixel mode stored (dword_506B88 = 2, dword_518614 = 0
  initially); windowed client size follows the mode while dword_507218 is
  set (initially 1), else the Sys81:64 size; Sys81:64 with a zero dimension
  follows the mode again and resizes a windowed window at once. Deferred
  for the graph reset on a real size change (sub_443100: filters +537,
  effectors +547 rebuilt when their screen geometry differs; windows +573
  re-centred at (-w/2, -h/2), children shifted) — do it with the 90/91
  window/filter/effector audit — and for pixel mode 0 (senmomo uses 1).
* Alt+Enter/hotkey toggle: sub_4619A0 only requests; the frame step
  (sub_4619B0 -> `apply_display_toggle_request`) runs it while Sys80:62 is
  enabled and no DirectShow movie plays, and posts event {1, fullscreen, 0}.
* 80:64 clears the input records (sub_46DF80) on show and hide.
* 80:6B: full engine restart (status 5): the VM resolves (root, archive)
  like sub_465650 (`bootstrap_root_and_archive`), halts, and
  `RuntimeEngine::take_restart` builds a new engine in `drive_frame`.
* 80:6D: WM_DROPFILES posts event {16, 0, 0}; an empty drop keeps the path.
* 80:6F: desktop = monitor video mode (GetAdapterDisplayMode), halved for
  spanned monitors, compared in double precision with the mode size.
* 80:70: index > 12 is fatal (無効なサイズ番号); always pushes 1. 80:70/71
  deferred: the global segment is still a `global_config` mirror beside
  `memory` (VM model question).
* 80:78..7B: path = validated root + BGI%.4d.cad; GetLocalTime (local, not
  UTC) seeds the integrity stream with wMilliseconds; 80:78 ignores a failed
  write; label error text from 1.622.
* Error texts from 1.622 for 80:60 (ピクセルモード/スクリーンサイズ) and
  80:67 (マウスカーソル形状番号).

80:80..85 verified (2026-10-22):

* BGI.gdb lives at validated root + "BGI.gdb" (load and save).
* Load accepts the config/user-data sizes the file declares (user data is
  zero-filled to 1 MiB) instead of rejecting non-1024/1 MiB layouts, and
  reads the pre-3.00 fixed layout (`install_legacy_global_data`; variant
  chosen by the flag record at 394248; status 2 when sizes do not add up,
  data installed anyway).
* The stored window position goes through `restore_window_position`
  (sub_46FE90: >= 1/8 of the window on some monitor, else sub_461B40 centre;
  (0, 0) in full screen). The app samples monitors, the current monitor,
  decorations and the outer position every frame.
* Save writes GetWindowRect left/top into header +20/+24 (was always 0).
* 80:82/83 fatal texts (無効なオフセット / 無効なデータサイズ, unsigned).

80:88..8B verified: 88 accepts an empty table name; 8A checks start >=
length (2) before the count checks (3), all unsigned; 8B pops index, name,
then the output DWORD (the old tests and a "swap if the name exists"
heuristic had name/output reversed) and leaves the output untouched on
failure.

80:90..CF verified (2026-10-22):

* 80:94/96/95/97: 1.622 fatal texts (the two handlers use different ones).
* 80:98..9E: capacity and removal count are unsigned, a negative index or
  start walks off the list (2); the game-specific 0x4CAD8 buffer hack is
  gone (a load is a plain memcpy).
* 80:A1 only queues {0, code, parameter}; the "0x7FFF quits" guess is gone.
* 80:B0..B6 match, including that a cancelled CProcExclusion leaves its
  waiter queued.
* SDC: `compress_sdc` ports sub_493E90 (LZ, per-byte chains, 0x801 window,
  2..17-byte matches); byte-exact against the target code run in Unicorn
  (`src/tools/emu_sdc.py`, ignored test `sdc_compressor_matches_the_target_
  vectors`). Seed = GetSystemTime wMilliseconds. Zero-length C0 and
  zero-size/count C4 produce a 32-byte empty stream. C5 decodes into a host
  buffer (no VM heap allocation). CF's SDC test includes the NUL.

80:D0..FE done (2026-10-22); group 0x80 has no todo left. Fixes:

* 80:D2 records live in host memory with their shadow values (no VM heap
  allocations, which shifted script heap addresses).
* Namespace 0x80000000 of the string tables is the resource-name list of
  80:84/85 and BGI.gdb (`resource_names()`), so D9..DE see it and D8 keeps it.
* 80:E0/E2 port sub_472A10's control flow into the VM
  (`launch_native_process`): "dir\\cmd" or primary root + cmd; no message ->
  0, E2 one OK box, E0 OK|CANCEL + quit question. senmomo's boot call
  (ISESUforSHTK.exe, null message) just returns 0 on macOS.
* 80:E1's command runs at shutdown (WinMain), once, without waiting.
* 80:E9 hashes primary root + name unless absolute.
* 80:F0 argument roles fixed (subfolder string, initial vs output folder).
* 80:FA copies the file even when it does not end in "\\" NUL.

Deferred in 0x80: 20/21, 40/41/44, 70/71 (VM value/memory model), 3A and
F0..FC (ADR 0004 emulation), 60 (graph reset on a size change, with 90/91).

Group 0x81 done (2026-10-22) except the deferred ones. Fixes:

* 81:04's threshold (dword_503DE8, initially 500 not 1000) now drives the
  pause detection of the engine clock (`normalize_engine_elapsed_ms`).
* 81:07 press slots dword_503E40 per button (left/right/middle/X1/X2,
  (-1, -1) before the first press), recorded where the WndProc does.
* 81:08/09 buffer limits; 81:0A/0B space folding sub_496650; 81:0B is the
  D3D adapter description + driver version (renderer adapter name), not
  CPUID; 81:0E reads the desktop mode, not the game size.
* Touch (81:16..19) and DirectInput (81:1D) have no host source: empty
  history, 0 results, no fake mouse data.
* dword_507690 starts at 0x2000 and 81:1F reaches the app (the app arm was
  dead).
* 81:61..63 are the full-screen scaling mode (0 fit, 1 stretch, 2 1:1) and
  the monitor choice: renderer `PresentationScale`, primary vs current
  monitor.
* 81:6D..6F: ps_3_0 reported, no D3DX, so the bicubic effect fails as on a
  PC without d3dx9_43.dll.
* 81:B0 returns the LCS length (was len+len-LCS); 81:B7 packs Shift-JIS
  codes, no UTF-16 conversion.
* Blob tables 81:D0..D5 rewritten from sub_4478A0..sub_447AA0 (handles,
  capacity doubling, status codes, pop orders).
* 81:E0 ports sub_472A10's wait path; 81:EC/ED are host-wide (file lock).
* The dead app-side 0x81 arms (`native_system_ext.rs` and the lib.rs
  fallbacks) are gone: the VM answers every 0x81 selector.

Deferred in 0x81: 28..3E and F2/F7 (ADR 0004 file layer: the async CFileDX
worker sub_4014D0 for 28..2B, file times, drives, dialogs, installer);
44 (child threads), 6A/6B (the script-error path sub_464C50 -> sub_464A80:
message box unless captured, capture when dword_566014, throw) — VM model.

Group 0x90 in progress (2026-10-22): 00..5D done except 0C and 10.
Shared tools: `crates/ethornell-app/src/target_checks.rs` holds the 1.622
validators sub_4984C0..sub_4989C0 with their texts; use them instead of
English placeholder errors.

Renderer items found by the 0x90 audit (selector state is right, the
drawing is not yet the target's; do these as one renderer pass):

* Presentation: sub_462100 is ported as a gate (Graph90:01 enable,
  Graph90:02 interval, `take_present_due`); redraw requests are treated as
  always pending and partial (dirty-rectangle) redraws as full ones.
* Back buffer: 90:04/05 copy the current tree, the target copies the last
  presented frame (only differs between a change and the next present).
* 90:0C (dword_565B54/58): CDspObjWindow::Draw (sub_42B520) blits the
  window's own buffer only while this is set, with the combined
  transparency; the app ignores it. 90:10's async CProcLoadBitmap
  completes at once (timing is VM procedure scheduling).
* Sprite mode 2 (90:59) renders as a float quad; the target's affine
  raster (sub_429A60/sub_4295C0/sub_4281E0) and its <2 extent check are
  only approximated.
* 90:CD tone-curve blit: argument order, checks and texts are the
  target's, but the pixel kernel (SSE sub_419550/sub_4198E0: luminance,
  mono blend, LUT, overlay-style colour film, modes 8/38) is a guess.
* 90:CE: checks and texts done; the two encoders (sub_46A320 format 0,
  sub_4A1930 format 1, quality <= 100) are not ported (the app writes a
  raw BG payload). Port and verify under Unicorn like the SDC compressor.
* Text glyphs come from host fonts; the anti-aliasing layouts of 90:0D
  are stored but GDI rasterization is not reproduced.
* 91:9B with the glyph-box flag (p6) and 91:9C/9D layouts use GDI glyph
  metrics (sub_4352B0 / sub_434F40); the app uses cell arithmetic. 91:93's
  p3 drops the default text effects (sub_4351E0(0)) in the target only.
* 91:90/92: the message object's p2 (vtable+0x20) and the sub_435150
  options are not applied; the ruby margin is added at layout start for
  every draw, while sub_435640 adds it only with its ruby argument set.
* 91:55 links: a sprite masked by another sprite (Sprite+0x160) is stored
  but not drawn as a mask.
* Rain (C0:40..4F): the API is the target's, the drawing is not. C0:43
  is a screen-sized grayscale mask that the app still draws as the drop
  image, and the drop field from 46..4E is a guess; CDspObjRain's draw
  needs reversing. Particle layers of C0:06 are stored and used for the
  raster split only.
* 91:18..1E: checks, texts and argument order are the target's; the
  kernels are not. 91:18/19 (sub_416B60 / sub_4179B0) are rotate-zoom
  blits: p1/p2 destination and p4/p5 source positions in 16.16, p6 the
  angle in 16.16 degrees (multiples of 360 with unit scale and whole
  positions take the plain blit sub_416890 -> sub_40B560), p7/p8 the
  16.16 scales, p9 the alpha, p10 the source-alpha switch; the app treats
  them as an integer rectangle copy. 91:1A sub_418B50, 91:1B
  sub_418F30, 91:1C sub_402C60 (destination recreated at
  (w*p2>>16, h*p3>>16) in the source format), 91:1D sub_419CF0 types
  0..5, 91:1E sub_4163C0 (32-bit source) / sub_415E30 (8-bit source)
  over the overlap where target (x, y) reads source (x+p2, y+p3).

Deferred to ADR 0004 in 0x90: C4/C5 (external image import/export files).

Next: 90:D0 onwards in table order.

Group 0x92 (2026-10-22): done except 14/15 (DCProcPreloadBmp, a
DCProcImageSynth that parses "name x y mode alpha" lines and goes through
the DC cache; only p0 is used — VM procedure phase), 1F (loose BMP via
sub_465F20: -1 when unreadable, 0x80000001..7 from the BMP header checks of
sub_402110 — ADR 0004) and F1 (BF_Movie load procedure — VM phase).
Compatibility kept on purpose: 92:89/8D/90/91 with a handle that is not a
CDspObjWindow keep the older engines' bitmap behaviour (Wonderful
Everyday's scrmsg._bp); 1.622 itself raises "無効なウィンドウハンドル".

Status 2026-10-22 (after 0x91/92/A0/B0/C0/D0): every selector is verified
except the renderer kernels (90:0C, 90:CD, 90:CE, 91:18..1E — their checks,
texts and argument order are done) and the deferred ones: the ADR 0004
file layer (80:3A, 80:F0..FC, 81:28..3E, 81:F2/F7, 90:C4/C5, 92:1F) and the
VM model (80:20/21/40/41/44/60/70/71, 81:44/6A/6B, 92:14/15/F1).
Next: the kernel pass (port each kernel, compare against the target under
Unicorn), then ADR 0004, then the VM.

Kernel pass (2026-10-22): 90:0C, 90:CD, 91:18, 91:19, 91:1A..1E verified;
90:CE format 0 done. Every kernel is a literal port in
`crates/ethornell-app/src/bitmap_kernels.rs` (CBG v1 in
`crates/ethornell-image/src/cbg_encode.rs`), checked byte for byte by the
ignored test `bitmap_kernels::tests::kernels_match_the_target_vectors`
against vectors from `src/tools/emu_kernels.py` (the 1.622 code under
Unicorn; ~600 vectors, kinds 1..9 documented in the script). Notes:

* The emulation sets the x87 control word to the Windows default 0x27F
  (53-bit precision) before the rotate-zoom walk (sub_4169D0); sin/cos then
  agree with Rust's f64. Tables the CRT start-up fills (qword_50B0F0,
  qword_50B900, dword_50A8D0 from sub_407760) are written by the script;
  operator new/delete and GetTickCount are hooked.
* The target splits large destinations over its worker threads
  (sub_41A120: bands when w*h >= 0xA00 and the pool has >= 2 threads; the
  band count depends on the host CPU). The ports run the single-band path.
* `rcpps` (sub_40B330 / sub_40BD00) is modelled as the exact reciprocal, as
  Unicorn computes it; real CPUs approximate it to 12 bits, per vendor.
* sub_469B70 (CBG v1 zero runs) reads one byte past its input; taken as 0.
* Found on the way: 1.622's format-2 alpha blits (mode 0 2->1 sub_40B1B0,
  2->2 sub_40B330, mode 1 2->2 sub_40BD00) work on pixel pairs: a pair is
  skipped when both alphas are 0 and copied raw when both are 255 (2->2) or
  >= 254 (2->1); otherwise both pixels take the SSE path, the odd last
  pixel the scalar one. `bitmap_blend.rs` now follows this (it had the
  older engine's per-pixel kernels). 90:CD's tone-curve tables are indexed
  by BGRA lane (the old `RuntimeColorLut::apply` used RGBA and is gone).
* 90:0C: windows (0xB000000x) draw their own buffer only after 90:0C(1, t)
  (dword_565B54 starts at 0); senmomo calls 90:0C(1, 0) at boot.

90:CE format 1 done the same day: `crates/ethornell-image/src/cbg2_encode.rs`
ports sub_49C200 and its workers literally (x87 at 53-bit precision: every
stack variable an f32, temporaries f64), kind 10 vectors. The emulation runs
sub_49BBB0 / sub_49BE00 / sub_4A1E70 / sub_49C200 itself (sub_4A1930's SEH
frame is skipped; FS points at a fake TEB through a GDT for the inner SEH
prologues), with a one-thread worker pool so sub_446910 runs the strips
inline, and C++ throws (RaiseException) ending the call. Target behaviour
kept on purpose:

* The encoder decodes its own output and fails (status 3 -> -2,
  "エンコードに失敗しました") on a mismatch. sub_4A1290's alpha search reaches
  x = width - column on the row above, which for padded widths <= 31 is the
  pixel itself: such copies decode to nothing, so narrow 32-bit images with
  uneven alpha are rejected. The check decodes into a fresh buffer; the
  port zero-fills it (the target's heap block is uninitialised).
* For 24-bit images sub_49C200 writes one strip offset per strip, while the
  engine's decoder (sub_49D7B0) always reads strips + 1, counted from the
  weight tables. The CBG v2 decoder in `cbg.rs` now addresses strips the
  same way (it used to count from the end of the table and dropped strip 0
  of such images).

Kernel pass finished: no selector is `todo` any more (688 verified, 53
deferred). The `PARTIAL_IMPLEMENTATION_OPCODES` labels in `native_call.rs`
lag behind this checklist (many verified selectors are still labelled
Partial); the CSV is authoritative.

Next: ADR 0004 (file layer: 80:3A, 80:F0..FC, 81:28..3E, 81:F2/F7, 90:C4/C5,
92:1F), then the VM value model (80:20/21/40/41/44/60/70/71, 81:44/6A/6B,
92:14/15/F1), which still needs the user's decision on rewriting the VM core
to raw 32-bit cells and segment<<26 addresses.

Reopened 2026-10-22 (found while documenting every selector from the 1.622
handlers; status set back to todo):

* 80:1B: two Rust paths (input.rs dispatch_input_opcode and lib.rs) with different error handling, neither with the 1.622 texts.
* 90:0E: invalid-font-width message prints the italic argument; the target prints the width (47A281 push ebx).
* 90:11: creation failure raises an English placeholder instead of 「無効なピクセルモード [ %d ] が指定されています」; negative sizes rejected by the port only.
* 90:5B: the target applies the transparency check to argument 8; the port stores argument 7 as the alpha.
* 90:9D: density compared signed (target unsigned: negatives fatal); offsets above 100 stored (target ignores them).
* 90:9E: app pops (bitmap, count); the target takes (count, bitmap).
* 90:A0: native_selection checks the window before count/initial/columns; 0 columns rejected (target divides by zero).
* 90:A1: same check-order difference as 90:A0.
* 90:A2: same check-order difference as 90:A0.
* 90:A3: same check-order difference as 90:A0.
* 90:B0: window checked before the focus mode (target after).
* 90:B1: window checked before the focus mode (target after).
* 90:C7: bitmap-number check runs after the cache lookup, so a fatal call with consume set removes the entry.
* 91:46: -1 accepted (the target checks the bitmap number on every call: 「無効なビットマップ番号 [ -1 ]」).
* 91:9C: returns a line count; the target returns the updated X position.
* 91:9D: returns a line count; the target returns the updated X position.
* B0:C3: argument order reversed (target: archive first, font name last).
* C0:C0: spline handles start at 0 (target 0x80000000).
* C0:F0: VM reads (archive, table_out, count_out, resource, base); target order is table_out, count_out, archive, resource, base.

All nineteen reopened selectors fixed and verified (2026-10-22).

VM value model done (ADR 0005, 2026-10-22): 80:20/21/40/41/44/70/71,
81:44/6A/6B and 92:14/15/F1 verified. 92:14 had its arguments swapped
(script order is archive, spec), so preloads never loaded; it now fills the
preload store dword_566AB8 that 90:C6 registers into, 90:10 / 90:C7 consume
and 92:15 empties. Status: 701 verified, 40 deferred: the ADR 0004 file
layer (80:3A, 80:F0..FC, 81:28..3E, 81:F2/F7, 90:C4/C5, 92:1F) and 80:60
(graph reset on a screen-size change, sub_443100: filters +537, effectors
+547, windows +573 re-centred).

2026-10-24, issue #87: corrected B0:C4 after checking sub_42DEB0 and
sub_42F030 in both exports. The argument is an optional output buffer.
Null returns the total Shift-JIS byte count, including each NUL terminator.
A buffer receives consecutive names and returns the family count.
The earlier availability query returned zero and caused amachoco's
`system.arc:ipl._bp` to call Alloc(0) at 0x182. The VM now owns the writes.
The portable app supplies its two stock faces through its CJK font substitute.
It does not enumerate installed host fonts. The cleaned reference is
`src/engine/font_enumeration.c`. Regression tests cover the allocation
sequence, encoded lengths, output limits, empty lists, and invalid pointers.

2026-10-25, issue #89: Japanese Amachoco and portable save paths.

The Japanese retail resources select the Amachoco ABI profile with two fingerprints.
The bootstrap has 7536 bytes and FNV64 `aa1f17da017b7c27`.
The title has 24368 bytes and FNV64 `e221d8b4be4f097a`.
An unknown resource pair retains the default profile.

The supplied `amachoco.exe` confirms these additional contracts:

* `81:00`, `sub_494EF0 -> sub_49EA60`: one argument and one result.
  Mode 0 selects CP932. Mode 1 selects UTF-8. Invalid modes return 0.
  Threads share the selected mode.
* `91:09`, `sub_48AB80 -> sub_498ED0 -> sub_464380`: one argument and one result.
  The handler stores graphics resource mode 0 or 1. Invalid modes return 0.
* `92:99`, `sub_490540 -> sub_4041C0`: seven arguments and one result.
  Script order is ABC output, count output, text, font, size, width percentage, and style.
  Each output pointer can be null. Each UTF-16 unit receives three signed widths.
  The portable implementation uses the existing `91:9B` cell approximation instead of GDI glyph metrics.
  `91:9B` also counts UTF-8 characters correctly in this profile.

`userdata._bp` builds an absolute save path from `80:3D`.
The absolute-path check previously accepted only Windows paths.
On Linux, `80:32` added the game root twice and failed to write the save.
The shared check now accepts POSIX absolute paths for writes, reads, size checks, and existence checks.
Empty `80:C4` streams remain valid zero-record inputs for `80:C5` and no longer produce a warning.

Regression tests check path handling, call arguments, mode changes, character widths, and empty record arrays.
The game reproduction uses a temporary copy with Japanese directory names.
It reaches dialogue and writes quick-save slots 102 and 103.
Quick-load in a new session reads slot 102 and restores the saved scenario.
The app, script, and VM suites pass 832 tests. The CLI build also passes.

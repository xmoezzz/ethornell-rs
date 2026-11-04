# PlayStation Vita

Native GXM frontend on vita2d's context. BGI scene textures, affine quads,
clipping, nearest/linear sampling and alpha/add/subtract blending run on the
GPU. Glyphs are rasterized by the shared font code and uploaded as textures;
ordinary scene drawing uses the GPU. Destination-reading filters, feedback
effects and selected bitmap raster operations still use CPU pixel buffers.

Requires VitaSDK with `libvita2d`, cargo-vita 0.2.2, and
nightly-2026-09-18 with rust-src:

```sh
./platform/vita/build.sh
```

The supplied `docker/Dockerfile` provides the same build environment.
Output: `player/target/armv7-sony-vita-newlibeabihf/release/ethornell_vita.vpk`.
Install using VitaShell. Copy the original game into
`ux0:data/ethornell/game/` and a Japanese/CJK TrueType font to
`ux0:data/ethornell/font.ttf`. Saves are separate at `ux0:data/ethornell/saves/`;
diagnostics are in `player.log`.

Left stick moves the pointer: a light tilt for buttons, full tilt crosses
the screen in under a second. Cross clicks, Circle sends Escape, Square
Space, Triangle Enter, L Control, and the D-pad arrow keys. Front touch
holds/releases the same left button as Cross. Start+Select requests the
game's normal close path. Confirmations appear over the game and accept
Cross/Circle. Structured text-entry dialogs, Flash and desktop integration
report unsupported status. Game memory demands must fit Vita's extended
memory allocation; the player requests a 320 MiB heap. Idle VM threads retain
compact data/code snapshots with zero tails restored on activation. New archive
images trigger an end-of-frame sweep of unreferenced images, so startup atlases
do not overlap later loads for another 60 frames. `player.log` records heap
usage, allocation failures, frame-rate samples and the shutdown stage.
MPEG program streams use a 12 MiB CDRAM source buffer. Duration probing and
PES indexing borrow this source; video is decoded incrementally in chunks of
at most 64 KiB. If the primary heap is fragmented, a separate CDRAM allocator
provides up to eight reusable pools of at most 16 MiB each, subject to the
memory GXM has left available. Movie soundtracks remain enabled. MPEG video
uses software decoding; these memory changes do not guarantee real-time
720p playback on Vita.
Thread handover stages only nonzero prefixes and clears only the outgoing
used tails. Indexed VM memory writes track conservative prefix bounds, so a
handover scans only possibly used bytes; unrestricted mutable Vec access falls
back to full bounds. Child threads exchange compact snapshots with their owner
without expanding both windows. Vita scans with word operations instead of libc
memcmp. Mode-5 sprite moves that only change GPU geometry skip cropped source
copies while retaining the native eager mip snapshots. CPU composition has a
contiguous-row path for integer, unscaled default copies; effector state borrows
its feedback pixels instead of cloning that full bitmap at every draw.
Ordinary screen and sprite filters reuse pixels when their input geometry,
resource revisions and filter parameters are unchanged; feedback effects remain
frame-driven. `engine_frames` counts actual engine updates. The performance
line's `loop_hz` counts host polling, including calls skipped by the frame limiter,
and must not be interpreted as the game's frame rate.
The VM dispatches canonical instructions by numeric opcode and skips disabled
instruction tracing in the interpreter loop. Private asynchronous contexts keep
their operand-ring mirror synchronized across scheduler slices; public execution
entry points still reconcile direct changes to the exposed operand stack.

Console frame deltas are capped at the script's pause-gap threshold, so a slow
CPU effect still advances its completion timer rather than freezing a transition.

The player links VitaSDK's published libraries. GXM programs are embedded,
so no runtime shader compiler or extra firmware module is needed. Shader
binaries and their matching Cg sources come from
[xmoezzz/siglus_rs](https://github.com/xmoezzz/siglus_rs/tree/5f358ef1360b0439e92aed2b749e8f25d8721954/crates/siglus_scene_vm/src/render/vita/shaders),
under MPL-2.0. Triple vertex rings and deferred texture retirement follow
that port's GPU lifetime handling. SLP vectorization is disabled to avoid
Vita3K's paired 64-bit saturation issue; loop vectorization stays enabled.

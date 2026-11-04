# Ethornell Android host ABI v1

`ethornell_android.h` is the only ABI definition. Use `sizeof` for every
`struct_size`, zero reserved fields, and reject incompatible ABI versions.
Handles are thread-local generation tokens, never pointers. The host must run
create, surface, input, stepping, response and destruction serially on one engine
thread. Wrong-thread calls are rejected as invalid handles. No Rust object is
marked unsafe Send to satisfy the host.

The JNI shim owns Application GlobalRef (process lifetime, because v1 has no
context-shutdown API) and ANativeWindow references. It must
wait for detach acknowledgement **before** returning surfaceDestroyed or releasing
the window. If an engine call hangs, an independent process watchdog must stop
the isolated engine process; do not return and continue using a dead Surface.
Attach failure does not retain the new window. On successful replacement, the
previous borrow has ended before return. Size zero must be detached, not attached.

No Surface, pause mask or pending confirmation means no narrative advancement.
Call step with CLOCK_MONOTONIC nanoseconds; ticks are gated to ~16ms without
replaying missed VM frames. Pause/detach clears pressed input and resets clocks.
Move is coalescible; down/up are FIFO. Physical-pixel input uses the same
aspect-fit viewport as rendering. Primary touch maps to left mouse. The host
must supply accessible right-click/backlog controls and map Android keycodes
to the documented Windows descriptors.

poll_event returns 1 when empty. Confirmation requests suspend the actual VM
native continuation; complete_request settles it exactly once. Bind host UI
responses to both handle and request_id. No default answer is speculatively
returned. Destroy cancels pending work. Error status must be followed by
last_error on the same thread. Panic poisons the session; only diagnostics,
detach and destroy are valid afterward. Abort/OOM/segfault cannot be caught.
Rejected stale Surface generations, unknown/stale request IDs and regressing
clock timestamps do not poison an otherwise healthy session. Genuine runtime,
GPU, audio and policy failures do. A black-bar pointer release cancels its
gesture (including queued edges) rather than generating an UP over old content.

Input dialogs (UserB0 text, two-field, segmented, list and date prompts)
suspend the VM in the same way and arrive as event kind 5, whose text names
the dialog type (`input`, `segmented`, `select`, `date`). Fetch the full
request with `request_payload` and answer with `complete_request`, putting a
JSON object in the response text (the integer result is ignored):

| type | request fields | answer |
| --- | --- | --- |
| `input` | `title`, `fields[]` of `{label, initial, max_bytes, numeric}` (1 or 2) | `texts[]`, one per field |
| `segmented` | `title`, `prompt`, `segments`, `max_bytes`, `numeric` | `texts[]`, one per segment |
| `select` | `title`, `prompt`, `options[]` | `index` (zero-based) |
| `date` | `fields[4]`, `month`, `day` (zero-based) | `texts[4]`, `month`, `day` |

Every answer carries `"accepted": true`; `{"accepted": false}` cancels. The
engine applies the desktop filters itself (`numeric` keeps digits and a
leading minus, segments keep ASCII letters and digits, `max_bytes` counts
Shift-JIS bytes), so the host may show plain text fields. A malformed answer
is an error, not a cancel.

Resources are read-only and user_root is a separate explicitly writable root.
Do not expose whole-game save export until game-specific user-file behavior is
verified. Resource and guest system-string encodings are not graph encoding.
The independent user-root tree (excluding reserved `.ethornell-*` names) is the
only writable/mirrorable set. No original resource `UserData` is automatically
imported. Writes are synchronous temp-file/sync/rename/directory-sync operations;
await a serial engine-thread barrier before export. Pause without its command
ACK is not a flush barrier. A post-rename sync error means committed but
durability uncertain. See `STORAGE.md` for the exact storage contract.
No shell execution, DLL loading or unrestricted filesystem/URL access is
authorized by this interface. Arbitrary file pickers, movie audio,
BMV/WMV/MP4 and complete SWF playback are not claimed v1 capabilities. Existing
MPEG video-only / Ruffle limitations remain visible diagnostics, not promises.

Build validation is not device or game compatibility validation. See build
provenance next to the generated library for source/toolchain/hash evidence.

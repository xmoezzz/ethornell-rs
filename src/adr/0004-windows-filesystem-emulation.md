# 0004. Non-Windows hosts emulate the Windows filesystem namespace

**Status:** Proposed (2026-10-22)

## Context

BGI 1.622 talks to the file system through Win32: `CreateFileA`,
`GetFileAttributesA`, `DeleteFileA`, `FindFirstFileA`, `CreateDirectoryA`,
`GetWindowsDirectoryA`, `SHGetSpecialFolderLocation`, `SHGetFolderPathA`, the
registry value `ProgramFilesDir`, drive probing (`GetDriveTypeA`,
`DeviceIoControl` CHECK_VERIFY, `GetVolumeInformationA`), common file
dialogs and `.lnk` shortcuts. Scripts see the resulting paths as strings,
store them, concatenate them and hand them back.

ADR 0003 rule 2 said: where the host has no equivalent service, behave as
the target does when that service fails. For directories and paths that is
the wrong model (user decision, 2026-10-22: directory behaviour should be emulated on
other platforms). Concretely:

* Sys80:3A mode 0 (`GetWindowsDirectoryA`) and 4 (`CSIDL_PROGRAM_FILES`)
  always report success; "failure" leaves the script with an untouched
  buffer it believes is a path. Mode 5 (registry `ProgramFilesDir`) fails
  outright on macOS.
* Host-native roots leak to scripts. The primary root on macOS is
  `/Users/.../game/`; Sys80:3D returns it, but the target's absolute-path test
  (`sub_464EE0`: first byte `\` or second byte `:`) calls it relative, so a
  script that passes it back to Sys80:30..35 gets it prefixed a second time.
  Sys80:2B (SplitPath, `_splitpath`) sees no drive either.
* Win32 semantics a script can observe: case-insensitive names, `\` and `/`
  as separators, `CreateFileA` not creating directories, `DeleteFileA`
  refusing read-only files, the attribute bits of Sys80:2C/2D, drive types
  and free space (Sys81:36/37), the media check `sub_4650E0`.

## Decision

On non-Windows hosts the app presents scripts with an emulated Windows
filesystem; on Windows the real Win32 services are used unchanged.

1. **Script-visible paths are Windows paths.** Every path the engine hands to
   a script (Sys80:3A, 3B, 3D, 3F, 6D, F8, 81:38/3A/3B and the enumerations)
   is a drive path with backslashes. Every path a script hands in is parsed
   with the target's rules (`sub_464EE0`, `_splitpath`) and translated by
   one module before it reaches the host file system.
2. **Wine-style prefix with mounts.** One emulated drive `C:` backed by a
   per-user prefix directory (`<data dir>/ethornell/drive_c/`), plus mounts
   that take precedence over it:

   | Windows path | Backing |
   |---|---|
   | `C:\Game\` (primary root, `::Buffer`) | the game directory |
   | `C:\Windows\` (Sys80:3A mode 0) | prefix (created on demand) |
   | `C:\Program Files\` (mode 4, mode 5 / `ProgramFilesDir`) | prefix |
   | `C:\Users\<user>\Desktop\` (mode 1, CSIDL 16) | host desktop |
   | `C:\Users\<user>\Documents\` (mode 3, CSIDL 5) | host documents |
   | `...\AppData\Roaming\Microsoft\Windows\Start Menu\Programs\` (mode 2) | prefix |

   Unmounted paths on `C:` live in the prefix; other drive letters do not
   exist (`GetFileAttributesA` fails, `GetDriveTypeA` = NO_ROOT_DIR). The
   game-directory mount name `C:\Game\` is fixed so saves that store paths
   stay valid when the game folder moves.
3. **Win32 semantics in the translation layer**, not in each selector:
   case-insensitive lookup with case-preserving creation; trailing dots and
   spaces stripped; `<>"|?*` rejected; MAX_PATH (260) enforced where the
   target's buffers are 260/780 bytes; READONLY from the owner write bit,
   HIDDEN/SYSTEM/ARCHIVE kept in an extended attribute (falling back to a
   sidecar table in the prefix); `DeleteFileA`/`MoveFileA` refuse READONLY;
   `CreateFileA` never creates parent directories; `C:` is a fixed drive with
   media (`sub_4650E0` true), free space from `statvfs`.
4. **Registry and shell objects only as far as paths need them:**
   `ProgramFilesDir` answers `C:\Program Files`; shortcuts (Sys80:F7, 81:F7)
   are written as `.lnk` files into the emulated folder so later existence
   checks see them. A full registry emulation is out of scope here.
5. **The restricted file policy becomes a mount backend.** The
   `bgi-resource\`/`bgi-user\` aliases in `file_policy.rs` map onto the same
   table (`C:\Game\` read-only, `C:\Game\UserData\` → user dir) instead of a
   second naming scheme.
6. ADR 0003 rule 2 is narrowed accordingly: for file-system and path
   services the "host equivalent" is this emulation; "behave as on failure"
   remains for services with no path meaning (CPUID, MCI, DirectShow, Flash).

## Consequences

* One pure, unit-testable module (`crates/ethornell-app/src/winfs.rs`):
  `parse` (target rules), `to_host` (mount resolution + case folding),
  `to_script` (host path → Windows path), attribute get/set.
* Script-visible roots change from host paths to `C:\Game\`; replay hashes
  are unaffected unless a script draws a path.
* Saves written by older builds that stored host paths keep loading because
  absolute host paths still resolve through the game mount when they lie
  under the game directory.
* Selectors whose host side depends on this stay `deferred` in
  `0003-selector-audit.csv` until it lands; their VM side (pop order,
  buffers, status codes) is still audited under 0003.

## Progress / next step

Partial implementation (2026-10-27): the non-Windows standalone runtime
exposes the primary root as `C:\Game\`. The shared path helpers map that
root to the native game directory. This prevents Unicode host names from
changing when the VM writes a Shift-JIS path buffer. Absolute host paths
remain accepted. Restricted hosts still use the `bgi-resource\` alias.
The prefix, special folders, and other Windows path rules remain pending.

Selectors waiting on it: 80:3A (host side:
modes 0/4/5 currently return "no path"; under this ADR they return the
emulated paths and mode 0/4 keep reporting 1).

Next:
1. `winfs.rs` with parse/to_host/to_script and tests (no I/O).
2. Primary root exposed as `C:\Game\`; route Sys80:30..3F through `to_host`.
3. Special folders (80:3A) and `ProgramFilesDir`.
4. Attributes (80:2C/2D), enumeration (80:24..26), directory ops (80:27..2A,
   2F), drives (81:36/37), shortcuts, dialogs returning Windows paths.
5. Fold `file_policy` aliases into the mount table.

Installer block (Sys80:F0..FC, deferred in 0003 until this lands; their pop
orders are checked): F0 is dialog 108/122 (folder field, Browse appending the
default subfolder, two check boxes, folder validation sub_46FFB0/sub_4731A0)
and F1 the installer message dialog — both need a faithful host dialog, not
the current generic prompts; F2/F3 install files, the uninstall list,
shortcuts and the registry entry; F4/F5 maintain the uninstall list; F6/F7
shortcuts (.lnk in the emulated Start Menu/Desktop); F8/F9 the registry key
of the installed folder (a small emulated registry in the prefix); FA/FB the
Windows directory (C:\Windows mount); FC the file association.

Related Win32 emulation found by the 0003 audit (same principle, not
filesystem): Sys80:54 waits for a Windows message number and receives
(wParam, lParam) from the WndProc (sub_499930 -> sub_49AAE0 signals every
message). The app synthesizes only mouse (0x200..0x20E) and key (0x100/0x101)
messages; WM_ACTIVATE, WM_SIZE, WM_CLOSE, WM_SYSKEYDOWN, WM_POWERBROADCAST etc.
should be pushed from `sync_host_window_state` with the target's wParam/lParam
encoding (key lParam = repeat 1 | scan code << 16).

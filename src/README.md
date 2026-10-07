# src/ — the original engine, restored as C, ahead of the Rust port

Workflow for every subsystem: **raw Hex-Rays output → hand-named C with fixed
structs → Rust port that follows the C function by function.** The C does not
need to be pretty or to link; its job is to freeze the structures and the
control flow so the port cannot drift.

| Path | What it is |
|---|---|
| `raw/text_XXXXXX.c` | Lossless export of all 5679 functions of `Tayutama2_trial_TG.exe`, grouped in 0x4000-byte windows. Never edit. |
| `raw/INDEX.csv`, `raw/FAILED.txt` | Address → name → decompilation status; the 12 functions Hex-Rays could not decompile. |
| `include/ida_prelude.h` | Types/macros the pseudocode needs. |
| `include/globals.h` | 2232 data symbols (auto-names found in the pseudocode plus `names`/`strings`). |
| `include/engine_types.h` | **Hand-recovered structs.** Each field names its byte offset and the functions proving it. |
| `engine/*.c` | Hand-cleaned functions using those structs (currently `cthread.c`). |
| `handlers/closure.csv` | For each of the 695 native selectors: handler and every function reachable by direct call. |
| `handlers/shared_core.txt` | Functions used by ≥ 8 selectors — port these first. |
| `tools/export_decompiled.py`, `tools/handler_closure.py` | Regenerate the above from `reverse/BGI.sqlite`. |

`reverse/` is not tracked; `raw/` and `handlers/` are the checked-in copy of its
decompilations. 2946 functions are engine code (the rest is CRT/MSVC); 1977 of
them are used by exactly one selector. Virtual calls (`(**v)(v, 1)`) are not
visible to the closure walk and must be followed through the vtables by hand.

## Status

| Subsystem | C | Rust |
|---|---|---|
| `CThread` (0x444000–0x445500): operand ring, code/data regions, modules, reservations, callbacks | `engine/cthread.c` | `ethornell-vm/src/target_thread.rs` (tested, **not yet wired in**) |
| Message markup / ruby registry / reveal timeline | — (ported straight from raw) | `ethornell-app/src/{ruby_registry,text_anim,text}.rs` |

## Integration note

The running VM (`ethornell-vm/src/lib.rs`) stores operands as `Vec<Value>` and
BP memory as a sparse map, with `native_thread::CThreadLayout32` holding audit
handles. `target_thread.rs` owns real byte regions and the wrapping DWORD ring.
Switching the VM to it means moving BP pointers to region offsets, so it is a
deliberate, separate step.

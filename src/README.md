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
| `CThread` (0x444000–0x445500): operand ring, code/data regions, modules, reservations, callbacks | `engine/cthread.c` | `ethornell-vm/src/target_thread.rs` is the tested reference; its limits are enforced on the live thread (see below) |
| BP interpreter: scheduler loop, 89-entry opcode table, base opcodes, call/ret/jmp/jc | `engine/bp_interp.c` | `ethornell-vm/src/lib.rs` (verified opcode by opcode; see `ethornell-script/src/vm_opcode.rs` for evidence addresses) |
| Message markup / ruby registry / reveal timeline | ported straight from `raw/` | `ethornell-app/src/{ruby_registry,text_anim,text}.rs` |

## How the live VM follows the target thread

The interpreter keeps `Value`-typed operands and a sparse BP memory map, so it
does not store its state inside `TargetThread`. The target's observable rules
are enforced on `native_thread::CThread` instead:

* region sizes: main thread 4096 operand slots / 0x80000 code / 0x40000 data
  (`sub_48C990`); children take them from `0x80:0x44` (pop order: data bytes,
  code bytes, slots, file, archive);
* `0x80:0x40`: the module must fit the code region (`sub_444CE0`); the used
  size is the sum of the LIFO module chain using the header's code length
  (`BpProgram::module_size`), not the instruction extent;
* `store_base` / `call`: the frame pointer must stay below the data region;
* `call` / `jmp` to 0 are fatal (`sub_473910`); `ret` with an empty data stack
  ends the thread (status 4);
* `i32::MIN / -1` wraps instead of trapping.

## Open fidelity items

* `store_base` zero-fills new frames; the target does not (`sub_4738A0`).
  Removing the fill makes the save-slot excerpt line (usdtwnd.\_bp, the
  shrink-to-two-lines loop at 0x1CA6) show stale bytes, so some earlier step
  that fills that buffer in the original is not reproduced yet.
* Blit modes 4 (ARGB source), 5-9, 0xC0/0xC1, 0xFF keep a float approximation.

# 0002. 0xD0 is the logic extension (grid field, space, search worker)

**Status:** Done (2026-10-21) — all 46 selectors ported

## Context

The 0xD0 native table was carried as "legacy 3D" (`execute_legacy_3d` in
`crates/ethornell-vm/src/extended_opcodes.rs`): it pops the right number of
arguments, hands out handles for create calls and returns 0 for the rest.
RTTI in BGI 1.622 shows it is a strategy-game (SLG, grid tactics) toolkit instead:

| Selectors | Class | Object | Handle list |
|---|---|---|---|
| D0:00–2D | `DCTELgclGrdFldMngr` : `DCTELgclFldMngr` — tile map, terrain cost tables, units, reachability/path fields | 0x498 bytes, ctor `sub_454940` | `dword_5669EC`, ids from `dword_5669E4++` |
| (per unit) | `DCTELgclGrdFld` : `DCTELogicalField` — one unit's movement field | created through `GrdFldMngr` vtbl +0x24 | owned by the unit record (+0x28) |
| D0:40–7A | `DCTELgclSpcMngr` — 3D space / body simulation | 0x170 bytes, ctor `sub_4559B0` | `dword_5669F8`, ids from `++dword_5669F0` |
| D0:80–8E | `DCTESLGEvaluator` — SLG AI, run on a worker thread | 0x160 bytes, ctor `sub_4583E0`; job record 0x78 | `dword_566A00`, ids from `++dword_5669FC` |

Every wrapper maps the internal status through `sub_497120`
(0 → 0, 0x80000000 → 1, 0x80000001 → 2, … 0xA0000006 → 22, −1/−2 kept, other → −1);
that small integer is what the script sees.

None of the local games (senmomo, Tayutama2) call 0xD0, so there is no runtime
oracle; behaviour is checked by unit tests written against the decompilation.

Evidence: `src/raw/bgi1622/d0.c` (279 functions), `d0.asm` (same functions plus
the vtables), handlers 0x477910–0x478A80, wrappers 0x497120–0x4983B0.
Float arguments arrive as 16.16 fixed and are multiplied by
`dbl_4EC968 = 1/65536` in the handler.

## Decision

1. Port the three classes for real, one at a time, in this order:
   grid field manager (+ per-unit field) → evaluator → space manager.
   The evaluator depends on the grid field; the space manager is independent.
2. Cleaned C: `src/engine/logic_grid_field.c`, `logic_slg_evaluator.c`,
   `logic_space.c`. Rust: a new module per class under `crates/ethornell-vm`.
3. **The evaluator's worker thread is not reproduced.** The target queues a
   job (`D0:88/8A/8D` with the async flag), returns 0, and `D0:87` reports
   14 ("busy") until the thread posts the result. The port runs the job to
   completion at submission and stores the result, so the first `D0:87`
   already sees it. A script that polls until "not busy" behaves the same,
   and replays stay deterministic. A script that relied on observing 14 at
   least once would differ; none is known.
4. Status codes are kept as the target's internal 0x8000xxxx/0xA000xxxx values
   inside the port and mapped through the `sub_497120` table at the boundary,
   so the C and Rust can be compared line by line.

## Consequences

* `execute_legacy_3d` is replaced selector by selector; the ABI table there
  (`legacy_3d_abi`) stays the source of argument counts until each group is
  ported.
* Without a game, ABI mistakes (argument order) are the main risk; each
  handler's pop order is taken from `d0.asm`, not from Hex-Rays, and recorded
  in the C file.

## Progress / next step

* [x] Raw export of the whole closure checked in (`src/raw/bgi1622/d0.*`).
* [x] `src/engine/logic_grid_field.c`: all 19 grid selectors (D0:00–2D), their
      script argument order, the manager/unit/field layouts, the movement
      search, line of fire, directions. Handler argument order was derived
      mechanically from `d0.asm` (a small symbolic run over each handler's
      pops and pushes); the table is at the end of the C file.
* [x] Rust: `crates/ethornell-vm/src/logic/{mod,grid,spline}.rs`, wired in
      front of `execute_legacy_3d`. `CSpline` is ported exactly (coefficient
      form, 100-point cap) rather than reusing the app's animation spline,
      whose arithmetic differs.
* [x] `legacy_3d_abi` argument counts fixed for D0:67/6A/70/72/74/75 (they
      did not match the handlers and would have misaligned the stack).
* [x] Evaluator (D0:80–8E): `src/engine/logic_slg_evaluator.c`,
      `crates/ethornell-vm/src/logic/{slg,msvc_qsort}.rs`, plus the grid
      calls it needs (threat-map cache `sub_453C20`/`sub_4541A0`, snapshot
      `sub_453260`, neighbours, path points, cell cost/facing, sorted
      reachable list). The CRT `qsort` (0x4AC3F0) is ported step for step
      because ties in scores and costs are common and it is not stable.
      Tests cover the damage formula, facing multiplier, attack choice,
      approaching an out-of-reach target and counterattacks by facing.
      Ported behaviour that looks odd but is the target's: a unit's own tile
      is "occupied" in its own search, so the walk-attack estimate never
      strikes from where it stands; enemy skills of effect 4 overwrite the
      acting unit's buffs during the simulation; the acting unit can be
      queued twice when an enemy walks and acts again.
* [x] Space manager (D0:40–7A): `src/engine/logic_space.c`,
      `crates/ethornell-vm/src/logic/space.rs`. Round bodies (96 bytes) with
      two link kinds, overlap/neighbour queries, the sweep test and the
      planar FIFO path search on half-step cells with 16 moves. The thread
      pool there only pre-computes sweep tests, so it is not reproduced.
      `execute_legacy_3d` no longer has placeholder branches.

Space deviations: `rsqrtss`/`rcpss`/`rcpps` are hardware estimates whose
bits differ between CPU vendors; the port starts the same Newton step from
the exact value (results can differ in the last bit before the 16.16
snap). The sweep test's kept edge distances start at +inf (stack garbage in
the target). A path target cell with negative coordinates fails with
0xA0000006 (the target indexes outside its grid). The backtrack stops after
one pass over the grid (the target loops forever when a walk returns to the
start cell, whose cost 0.0 doubles as "unset"). A resolution that makes the
direction stride 0 ends the expansion (the target spins).

### Evaluator notes (read 2026-10-21, not yet in C)

Handlers (script args in call order; "job" = queued on the worker thread
when the async flag is set, else run inline):

| Sel | Args | Wrapper → work | Result |
|---|---|---|---|
| D0:80 | (out*) | 497DC0: job record 0x78 + thread, evaluator 0x160 | **no push**; 0 / 13 |
| D0:81 | (h) | 497EE0: stop thread, delete | raw 0 / 1 |
| D0:84 | (h, n) | 4980B0 → 458530: eval[8] (+0x20) = n, n < 1 → 0x90000002 | busy 14 |
| D0:87 | (out*, h) | 498050: *out = job result, then result = 0xFFFF0000 | raw 0 / busy 14 |
| D0:88 | (h, grid, n, units*, async) | 498110 → 458550: load n (1..64) unit records of 2100 bytes, snapshot grid | job type 0 |
| D0:8A | (out_a*, out_b*, h, p3, p4) | 4983B0 → 458A70 (3.3 KB): decide; always async, writes both outputs from the thread | job type 2 |
| D0:8C | (h, grid) | 498210 → 4586B0: re-snapshot the grid (clone via vtbl+20, sub_4532A0) | busy 14 |
| D0:8D | (h, index, unit*, flag, async) | 498280 → 4586E0: replace one unit record, rebuild its threat maps | job type 1 |
| D0:8E | (out*, h, index) | 498350 → 458A40: copy a unit record (0x834) out | busy 14 |

Evaluator (0x160 dwords): [2] thread count (= `[dword_56670C+4]`, an engine
global), [3] `DCDstrbtdPrcssng` pool running `sub_45BB20 → sub_45A530`,
[4] grid snapshot, [5] per-thread grid clones, [8] set by D0:84, [9] unit
count, [10] unit records (0x8DC each; the first 0x834 are the script's),
[11] acting unit, [12..] per-unit ints, [82]/[83]/[84] candidate count /
next / array (76 bytes each), [85] = 50, [86] = 67, +0x130 = {0x18000,
0x10000, 0xAB85} (1.5, 1.0, ~0.67).

Unit record: +0 grid unit id (0 = empty slot), +2100/+2104 position,
[527] = dir − 2, [141]/[142] go to `sub_453C20`, [533]/[534] reach maps,
[535..566] 16 pairs of threat maps keyed by `sub_459880` (20-byte weapon
profile: kind, range, bonus, arc, …). `sub_453C20` caches per profile, on
the grid unit's +0x2C list, a 7-int record per tile: [0] covered, [1..6]
hit counts by attack direction (vtbl+C). `sub_4541A0` fetches by key.

Threads: workers pop candidates from the shared queue under the pool's
lock, evaluate on a private copy of the unit records and a private grid
clone, and write the score into the candidate; the caller then sorts with
`qsort` (`sub_45BB40`, by +24 descending). Nothing depends on which thread
took which candidate, so a single-threaded port in queue order gives the
same result (decision 3 holds).

Evaluator deviations (target crashes or reads garbage): a missing grid
copy fails D0:8A with -1; targets without a record are skipped; with 0
sides the side tests are false (the target divides by zero); an ally's
target facing outside 0..3 skips the ally's hit; scoring runs on one grid
copy in candidate order — the target's per-thread copies differ only in the
acting unit's facing left over from the previous candidate, which matters
only for self-targeted damage skills.

Deviations from the target, all where it would corrupt memory or crash:
units never placed (x = -1) are skipped when marking occupancy and zones
(the target writes at index -(w+1)); a target height outside 0..31 in D0:28
is out of range (the target reads past a 32-entry stack array); D0:04 rejects
maps over 4M tiles or with a negative area; null output pointers are not
written.

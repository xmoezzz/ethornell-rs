//! The 0xD0 native table: the strategy-game ("logic") extension of BGI 1.622.
//! See `src/adr/0002-d0-logic-extension.md` for the map of the three classes
//! and `src/engine/logic_grid_field.c` for the cleaned target code.

mod grid;
mod msvc_qsort;
mod slg;
mod space;
mod spline;

use super::{Value, Vm, VmResult};
use grid::{Cell, GridFieldManager, Tile};
use slg::{Evaluator, SCRIPT_WORDS};
use space::SpaceManager;

/// The target's internal statuses; `status_to_script` (sub_497120) turns them
/// into what the script sees.
pub(crate) mod status {
    pub const NO_HANDLE: u32 = 0x8000_0000;
    pub const BAD_ARGUMENT: u32 = 0x8000_0001;
    pub const NO_MAP: u32 = 0x8000_0002;
    pub const NO_UNIT: u32 = 0x8000_0003;
    pub const OUT_OF_MAP: u32 = 0x8000_0004;
    pub const NO_TABLE: u32 = 0x8000_0005;
    pub const NO_SEARCH: u32 = 0x8000_0006;
    pub const UNREACHABLE: u32 = 0x8000_0007;
    pub const BAD_ORIGIN: u32 = 0x8000_0008;
    pub const BAD_HEIGHT: u32 = 0x8000_0009;
    pub const BAD_RADIUS: u32 = 0x8000_000A;
    pub const BAD_DIRECTION: u32 = 0x8000_000B;
    pub const FAILED: u32 = 0xFFFF_FFFF;
    pub const BROKEN_PATH: u32 = 0xFFFF_FFFE;
    pub const SEARCH_FAILED: u32 = FAILED;
    // DCTELogicalField's own codes, remapped by the manager.
    pub const NO_CELLS: u32 = 0x8000_0003;
    pub const NOT_REACHED: u32 = 0x8000_0004;
    pub const OUT_OF_FIELD: u32 = 0x8000_0005;
    pub const PRUNED: u32 = 0x8000_0006;
    pub const NO_THREAT_KEY: u32 = 0x8000_000D;
    pub const NO_THREAT_MAP: u32 = 0x8000_000E;
    // DCTESLGEvaluator.
    pub const BAD_COUNT: u32 = 0x9000_0002;
    pub const BAD_INDEX: u32 = 0x9000_0003;
}

/// sub_497120.
pub(crate) fn status_to_script(status: u32) -> i32 {
    match status {
        0 => 0,
        0x8000_0000..=0x8000_000B => (status - 0x8000_0000 + 1) as i32,
        0x9000_0002 => 16,
        0x9000_0003 => 17,
        0xA000_0000 => 1,
        0xA000_0001..=0xA000_0003 => (status - 0xA000_0001 + 18) as i32,
        0xA000_0004 => 8,
        0xA000_0005 => 21,
        0xA000_0006 => 22,
        0xFFFF_FFFE => -2,
        _ => -1,
    }
}

/// Largest map the port accepts (16 bytes a tile). The target only rejects 0
/// and dies in `operator new` on absurd sizes.
const MAX_TILES: i64 = 1 << 22;

#[derive(Debug, Default)]
pub(crate) struct LogicState {
    /// dword_5669EC, newest first when searched; ids from dword_5669E4++ (0..).
    grids: Vec<(i32, GridFieldManager)>,
    next_grid: i32,
    /// dword_566A00, ids from ++dword_5669FC (1..).
    jobs: Vec<EvaluatorJob>,
    next_job: i32,
    /// dword_5669F8, ids from ++dword_5669F0 (1..).
    spaces: Vec<(i32, SpaceManager)>,
    next_space: i32,
}

/// The 0x78-byte job record of D0:80: the evaluator plus the last result a
/// worker posted (read by D0:87).
#[derive(Debug)]
struct EvaluatorJob {
    id: i32,
    result: i32,
    evaluator: Evaluator,
}

impl LogicState {
    fn grid(&mut self, handle: i32) -> Result<&mut GridFieldManager, u32> {
        self.grids
            .iter_mut()
            .rev()
            .find(|(id, _)| *id == handle)
            .map(|(_, grid)| grid)
            .ok_or(status::NO_HANDLE)
    }

    fn space(&mut self, handle: i32) -> Result<&mut SpaceManager, u32> {
        self.spaces
            .iter_mut()
            .rev()
            .find(|(id, _)| *id == handle)
            .map(|(_, space)| space)
            .ok_or(0xA000_0000)
    }

    fn job(&mut self, handle: i32) -> Option<&mut EvaluatorJob> {
        self.jobs.iter_mut().rev().find(|job| job.id == handle)
    }
}

/// Fold a manager result into the status the wrapper returns.
fn done(result: Result<u32, u32>) -> u32 {
    result.unwrap_or_else(|status| status)
}

impl Vm {
    /// Runs a ported 0xD0 selector with its arguments in call order. `None`
    /// means the selector is not ported yet.
    pub(super) fn execute_logic(&mut self, id: u8, args: &[Value]) -> VmResult<Option<i32>> {
        let int = |index: usize| args.get(index).map(Value::as_i32).unwrap_or_default();
        let ptr = |index: usize| int(index) as u32;
        let result = match id {
            // (out*, kind, scale): only kind 0 exists; raw 1 / 0.
            0x00 => {
                if int(1) != 0 {
                    return Ok(Some(0));
                }
                let handle = self.logic.next_grid;
                self.logic.next_grid = handle.wrapping_add(1);
                self.logic
                    .grids
                    .push((handle, GridFieldManager::new(int(2))));
                self.logic_write_i32(ptr(0), handle)?;
                return Ok(Some(1));
            }
            // (h): raw 1 found / 0.
            0x01 => {
                let handle = int(0);
                let found = self.logic.grids.iter().rposition(|(id, _)| *id == handle);
                if let Some(index) = found {
                    self.logic.grids.remove(index);
                }
                return Ok(Some(i32::from(found.is_some())));
            }
            0x04 => {
                let (w, h) = (int(1), int(2));
                let cells = i64::from(w) * i64::from(h);
                if let Err(status) = self.logic.grid(int(0)) {
                    status
                } else if w != 0 && h != 0 && !(1..=MAX_TILES).contains(&cells) {
                    status::BAD_ARGUMENT
                } else {
                    let bytes = self.logic_read(ptr(3), cells.max(0) as usize * Tile::SIZE)?;
                    let tiles = bytes
                        .chunks_exact(Tile::SIZE)
                        .map(Tile::from_bytes)
                        .collect();
                    done(
                        self.logic
                            .grid(int(0))
                            .map(|grid| grid.set_map(w, h, tiles)),
                    )
                }
            }
            0x05 => {
                let count = match self.logic.grid(int(1)) {
                    Ok(grid) => grid.map_cells(),
                    Err(status) => return Ok(Some(status_to_script(status))),
                };
                let bytes = self.logic_read(ptr(2), count * 4)?;
                let cost = bytes
                    .chunks_exact(4)
                    .map(|word| i32::from_le_bytes(word.try_into().unwrap()))
                    .collect();
                match self
                    .logic
                    .grid(int(1))
                    .and_then(|grid| grid.add_cost_table(cost))
                {
                    Ok(table) => {
                        self.logic_write_i32(ptr(0), table)?;
                        0
                    }
                    Err(status) => status,
                }
            }
            0x10 => match self.logic.grid(int(1)).and_then(GridFieldManager::add_unit) {
                Ok(unit) => {
                    self.logic_write_i32(ptr(0), unit)?;
                    0
                }
                Err(status) => status,
            },
            0x11 => done(self.logic.grid(int(0)).map(|grid| grid.remove_unit(int(1)))),
            0x12 => done(self.logic.grid(int(0)).map(|grid| grid.drop_search(int(1)))),
            0x14 => done(
                self.logic
                    .grid(int(0))
                    .map(|grid| grid.set_position(int(1), int(2), int(3))),
            ),
            0x15 => done(
                self.logic
                    .grid(int(0))
                    .map(|grid| grid.set_size(int(1), int(2), int(3))),
            ),
            0x16 => {
                match self
                    .logic
                    .grid(int(0))
                    .and_then(|grid| grid.unit_mut(int(1)).map(|_| ()))
                {
                    Ok(()) => {}
                    Err(status) => return Ok(Some(status_to_script(status))),
                }
                let pair = self.logic_read(ptr(2), 8)?;
                let flags = u32::from_le_bytes(pair[0..4].try_into().unwrap());
                let team = u32::from_le_bytes(pair[4..8].try_into().unwrap());
                done(
                    self.logic
                        .grid(int(0))
                        .map(|grid| grid.set_flags(int(1), flags, team)),
                )
            }
            0x17 => done(
                self.logic
                    .grid(int(0))
                    .map(|grid| grid.set_table(int(1), int(2))),
            ),
            0x18 => done(
                self.logic
                    .grid(int(0))
                    .map(|grid| grid.set_dir(int(1), int(2))),
            ),
            0x20 => done(
                self.logic
                    .grid(int(0))
                    .map(|grid| grid.search(int(1), int(2), int(3), (int(4), int(5)))),
            ),
            // (dirs*, count*, h, unit, x, y)
            0x21 => {
                let want_dirs = ptr(0) != 0;
                match self
                    .logic
                    .grid(int(2))
                    .and_then(|grid| grid.path(int(3), int(4), int(5), want_dirs))
                {
                    Ok((steps, dirs)) => {
                        self.logic_write_i32(ptr(1), steps)?;
                        self.logic_write_i32s(ptr(0), &dirs)?;
                        0
                    }
                    Err(status) => status,
                }
            }
            // (cells*, h, unit)
            0x22 => {
                let bytes = match self.logic.grid(int(1)).and_then(|grid| grid.cells(int(2))) {
                    Ok(cells) => cells
                        .iter()
                        .flat_map(|cell| Cell::to_bytes(*cell))
                        .collect::<Vec<_>>(),
                    Err(status) => return Ok(Some(status_to_script(status))),
                };
                self.logic_write(ptr(0), &bytes)?;
                0
            }
            // (xy*, count*, h, unit)
            0x23 => match self
                .logic
                .grid(int(2))
                .and_then(|grid| grid.reachable(int(3)))
            {
                Ok(tiles) => {
                    let flat: Vec<i32> = tiles.iter().flat_map(|&(x, y)| [x, y]).collect();
                    self.logic_write_i32s(ptr(0), &flat)?;
                    self.logic_write_i32(ptr(1), tiles.len() as i32)?;
                    0
                }
                Err(status) => status,
            },
            // (xy*, dist*, count*, h, x, y, range, bonus, arc, units_only, ignore_units)
            0x28 => match self.logic.grid(int(3)).and_then(|grid| {
                grid.targets(
                    int(4),
                    int(5),
                    int(6),
                    int(7),
                    int(8),
                    int(9) != 0,
                    int(10) != 0,
                    true,
                )
            }) {
                Ok(found) => {
                    let flat: Vec<i32> = found.iter().flat_map(|&(x, y, _)| [x, y]).collect();
                    let distances: Vec<i32> = found.iter().map(|&(_, _, d)| d).collect();
                    self.logic_write_i32s(ptr(0), &flat)?;
                    self.logic_write_i32s(ptr(1), &distances)?;
                    self.logic_write_i32(ptr(2), found.len() as i32)?;
                    0
                }
                Err(status) => status,
            },
            // (out*, tx, ty, h, unit)
            0x2C => match self
                .logic
                .grid(int(3))
                .and_then(|grid| grid.unit_facing_delta(int(4), int(1), int(2)))
            {
                Ok(delta) => {
                    self.logic_write_i32(ptr(0), delta)?;
                    0
                }
                Err(status) => status,
            },
            // (out*, h, tx, ty, fx, fy)
            0x2D => match self.logic.grid(int(1)) {
                Ok(_) => {
                    let dir = grid::direction_to(int(2), int(3), int(4), int(5));
                    self.logic_write_i32(ptr(0), dir)?;
                    0
                }
                Err(status) => status,
            },
            0x40..=0x7A => return self.execute_space(id, args),
            0x80..=0x8E => return self.execute_evaluator(id, args),
            _ => return Ok(None),
        };
        Ok(Some(status_to_script(result)))
    }

    /// D0:80..8E. Jobs the target hands to its worker thread run here at
    /// once; their status goes to the job result that D0:87 reads (ADR 0002).
    fn execute_evaluator(&mut self, id: u8, args: &[Value]) -> VmResult<Option<i32>> {
        let int = |index: usize| args.get(index).map(Value::as_i32).unwrap_or_default();
        let ptr = |index: usize| int(index) as u32;
        // Unknown evaluator (or grid) handles return a raw 1 everywhere.
        const UNKNOWN: i32 = 1;
        Ok(Some(match id {
            // (out*): no value is pushed.
            0x80 => {
                self.logic.next_job = self.logic.next_job.wrapping_add(1);
                let handle = self.logic.next_job;
                self.logic.jobs.push(EvaluatorJob {
                    id: handle,
                    result: 0,
                    evaluator: Evaluator::new(),
                });
                self.logic_write_i32(ptr(0), handle)?;
                0
            }
            0x81 => match self.logic.jobs.iter().rposition(|job| job.id == int(0)) {
                Some(index) => {
                    self.logic.jobs.remove(index);
                    0
                }
                None => UNKNOWN,
            },
            0x84 => match self.logic.job(int(0)) {
                Some(job) => status_to_script(job.evaluator.set_sides(int(1))),
                None => UNKNOWN,
            },
            // (out*, h)
            0x87 => match self.logic.job(int(1)) {
                Some(job) => {
                    let result = std::mem::replace(&mut job.result, 0xFFFF_0000u32 as i32);
                    self.logic_write_i32(ptr(0), result)?;
                    0
                }
                None => UNKNOWN,
            },
            // (h, grid, n, units*, async)
            0x88 => {
                if self.logic.job(int(0)).is_none() || self.logic.grid(int(1)).is_err() {
                    return Ok(Some(UNKNOWN));
                }
                let n = int(2) as u32;
                let status = if n.wrapping_sub(1) >= 64 {
                    status::BAD_COUNT
                } else {
                    let words = self.logic_read_i32s(ptr(3), n as usize * SCRIPT_WORDS)?;
                    let logic = &mut self.logic;
                    let grid = logic
                        .grids
                        .iter()
                        .rev()
                        .find(|(handle, _)| *handle == int(1))
                        .map(|(_, grid)| grid)
                        .unwrap();
                    let job = logic
                        .jobs
                        .iter_mut()
                        .rev()
                        .find(|job| job.id == int(0))
                        .unwrap();
                    job.evaluator.load(grid, n, &words)
                };
                self.finish_job(int(0), status, int(4) != 0)
            }
            // (out*, count*, h, acting, max): always a job.
            0x8A => {
                let Some(job) = self.logic.job(int(2)) else {
                    return Ok(Some(UNKNOWN));
                };
                match job.evaluator.decide(int(3) as u32, int(4) as u32) {
                    Ok(results) => {
                        job.result = 0;
                        let flat: Vec<i32> = results.iter().flatten().copied().collect();
                        self.logic_write_i32s(ptr(0), &flat)?;
                        self.logic_write_i32(ptr(1), results.len() as i32)?;
                    }
                    Err(status) => job.result = status_to_script(status),
                }
                0
            }
            // (h, grid)
            0x8C => {
                if self.logic.job(int(0)).is_none() || self.logic.grid(int(1)).is_err() {
                    return Ok(Some(UNKNOWN));
                }
                let logic = &mut self.logic;
                let grid = logic
                    .grids
                    .iter()
                    .rev()
                    .find(|(handle, _)| *handle == int(1))
                    .map(|(_, grid)| grid)
                    .unwrap();
                let job = logic
                    .jobs
                    .iter_mut()
                    .rev()
                    .find(|job| job.id == int(0))
                    .unwrap();
                status_to_script(job.evaluator.resnapshot(grid))
            }
            // (h, index, unit*, with_maps, async)
            0x8D => {
                if self.logic.job(int(0)).is_none() {
                    return Ok(Some(UNKNOWN));
                }
                let words = self.logic_read_i32s(ptr(2), SCRIPT_WORDS)?;
                let job = self.logic.job(int(0)).unwrap();
                let status = job.evaluator.load_one(int(1) as u32, &words, int(3) != 0);
                self.finish_job(int(0), status, int(4) != 0)
            }
            // (out*, h, index)
            0x8E => {
                let Some(job) = self.logic.job(int(1)) else {
                    return Ok(Some(UNKNOWN));
                };
                match job.evaluator.record(int(2) as u32) {
                    Ok(record) => {
                        let words = record.script_words().to_vec();
                        self.logic_write_i32s(ptr(0), &words)?;
                        0
                    }
                    Err(status) => status_to_script(status),
                }
            }
            _ => return Ok(None),
        }))
    }

    /// D0:40..7A: the space manager. 16.16 arguments become floats as the
    /// handlers do (int * 1/65536 in x87, stored as float).
    fn execute_space(&mut self, id: u8, args: &[Value]) -> VmResult<Option<i32>> {
        let int = |index: usize| args.get(index).map(Value::as_i32).unwrap_or_default();
        let ptr = |index: usize| int(index) as u32;
        let float = |index: usize| (f64::from(int(index)) * (1.0 / 65_536.0)) as f32;
        let xyz = |first: usize| [float(first), float(first + 1), float(first + 2)];
        let status = match id {
            // (out*): raw 0.
            0x40 => {
                self.logic.next_space = self.logic.next_space.wrapping_add(1);
                let handle = self.logic.next_space;
                self.logic.spaces.push((handle, SpaceManager::new()));
                self.logic_write_i32(ptr(0), handle)?;
                return Ok(Some(0));
            }
            // (h): raw 0 / 1.
            0x41 => {
                let found = self.logic.spaces.iter().rposition(|(h, _)| *h == int(0));
                if let Some(index) = found {
                    let (_, mut space) = self.logic.spaces.remove(index);
                    space.clear();
                }
                return Ok(Some(i32::from(found.is_none())));
            }
            0x60 => done(self.logic.space(int(0)).map(|space| {
                space.add(
                    int(1) as u32,
                    xyz(2),
                    xyz(5),
                    float(8),
                    float(9),
                    float(10),
                    int(11),
                    int(12),
                )
            })),
            0x61 => done(
                self.logic
                    .space(int(0))
                    .map(|space| space.remove(int(1) as u32)),
            ),
            0x64 => done(
                self.logic
                    .space(int(0))
                    .map(|space| space.set_pos(int(1) as u32, xyz(2))),
            ),
            0x66 => done(
                self.logic
                    .space(int(0))
                    .map(|space| space.set_dir(int(1) as u32, xyz(2))),
            ),
            // (out*, h, i)
            0x65 | 0x67 => match self.logic.space(int(1)).and_then(|space| {
                if id == 0x65 {
                    space.pos(int(2) as u32)
                } else {
                    space.dir(int(2) as u32)
                }
            }) {
                Ok(v) => {
                    self.logic_write_i32s(ptr(0), &v[..3])?;
                    0
                }
                Err(status) => status,
            },
            0x68 => done(
                self.logic
                    .space(int(0))
                    .map(|space| space.link(int(1) as u32, int(2) as u32, int(3))),
            ),
            // (out*, h, i, kind)
            0x69 => match self
                .logic
                .space(int(1))
                .and_then(|space| space.parent(int(2) as u32, int(3) as u32))
            {
                Ok(parent) => {
                    self.logic_write_i32(ptr(0), parent)?;
                    0
                }
                Err(status) => status,
            },
            // (list*, count*, h, i, kind)
            0x6A => match self
                .logic
                .space(int(2))
                .and_then(|space| space.children(int(3) as u32, int(4) as u32))
            {
                Ok(list) => self.write_space_list(ptr(0), ptr(1), &list)?,
                Err(status) => status,
            },
            // (h, i, x, y, z, mask, sweep)
            0x70 => done(
                self.logic
                    .space(int(0))
                    .map(|space| space.sweep_body(int(1) as u32, xyz(2), int(5), int(6))),
            ),
            // (out*, count*, h, from, to, range, step, mask, sweep, resolution, threaded)
            0x72 => match self.logic.space(int(2)).and_then(|space| {
                space.path(
                    int(3) as u32,
                    int(4) as u32,
                    float(5),
                    float(6),
                    int(7),
                    int(8),
                    int(9),
                )
            }) {
                Ok(path) => {
                    let flat: Vec<i32> = path.iter().flatten().copied().collect();
                    self.logic_write_i32s(ptr(0), &flat)?;
                    self.logic_write_i32(ptr(1), path.len() as i32)?;
                    0
                }
                Err(status) => status,
            },
            // (list*, count*, h, i, mask)
            0x74 => match self
                .logic
                .space(int(2))
                .and_then(|space| space.ahead(int(3) as u32, int(4)))
            {
                Ok(list) => self.write_space_list(ptr(0), ptr(1), &list)?,
                Err(status) => status,
            },
            // (list*, count*, h, x, y, z, radius, exclude, mask)
            0x75 => match self.logic.space(int(2)) {
                Ok(space) => {
                    let list = space.around(xyz(3), float(6), int(7), int(8));
                    self.write_space_list(ptr(0), ptr(1), &list)?
                }
                Err(status) => status,
            },
            // (out*, count*, h, i, mask): 32-byte records.
            0x78 => match self
                .logic
                .space(int(2))
                .and_then(|space| space.neighbours(int(3) as u32, int(4)))
            {
                Ok(list) => {
                    for (n, relation) in list.iter().enumerate() {
                        self.write_space_relation(ptr(0).wrapping_add(32 * n as u32), relation)?;
                    }
                    self.logic_write_i32(ptr(1), list.len() as i32)?;
                    0
                }
                Err(status) => status,
            },
            // (out*, h, a, b)
            0x79 => match self
                .logic
                .space(int(1))
                .and_then(|space| space.relation(int(2) as u32, int(3) as u32))
            {
                Ok(relation) => {
                    self.write_space_relation(ptr(0), &relation)?;
                    0
                }
                Err(status) => status,
            },
            // (out*, h, i, x, y, z)
            0x7A => match self
                .logic
                .space(int(1))
                .and_then(|space| space.relation_to_point(int(2) as u32, xyz(3)))
            {
                Ok(relation) => {
                    self.write_space_relation(ptr(0), &relation)?;
                    0
                }
                Err(status) => status,
            },
            _ => return Ok(None),
        };
        Ok(Some(status_to_script(status)))
    }

    fn write_space_list(&mut self, list: u32, count: u32, values: &[i32]) -> VmResult<u32> {
        self.logic_write_i32s(list, values)?;
        self.logic_write_i32(count, values.len() as i32)?;
        Ok(0)
    }

    /// {index, (bytes 4..16 untouched), x, y, z, length}.
    fn write_space_relation(&mut self, out: u32, (index, v): &(i32, [i32; 4])) -> VmResult<()> {
        if out == 0 {
            return Ok(());
        }
        self.logic_write_i32(out, *index)?;
        self.logic_write_i32s(out.wrapping_add(16), v)
    }

    /// An async job posts its status for D0:87 and returns 0; a synchronous
    /// call returns the status itself.
    fn finish_job(&mut self, handle: i32, status: u32, asynchronous: bool) -> i32 {
        if !asynchronous {
            return status_to_script(status);
        }
        if let Some(job) = self.logic.job(handle) {
            job.result = status_to_script(status);
        }
        0
    }

    fn logic_read_i32s(&self, ptr: u32, count: usize) -> VmResult<Vec<i32>> {
        Ok(self
            .logic_read(ptr, count * 4)?
            .chunks_exact(4)
            .map(|word| i32::from_le_bytes(word.try_into().unwrap()))
            .collect())
    }

    fn logic_read(&self, ptr: u32, len: usize) -> VmResult<Vec<u8>> {
        if len == 0 {
            return Ok(Vec::new());
        }
        let range = self.resolve_range(ptr, len)?;
        Ok(self.memory[range].to_vec())
    }

    /// Writes through a script pointer; a null pointer is skipped (the target
    /// would fault).
    fn logic_write(&mut self, ptr: u32, bytes: &[u8]) -> VmResult<()> {
        if ptr == 0 || bytes.is_empty() {
            return Ok(());
        }
        let ptr = ptr;
        let range = self.resolve_write_range(ptr, bytes.len())?;
        self.memory[range].copy_from_slice(bytes);
        self.clear_shadow_values(ptr, bytes.len());
        Ok(())
    }

    fn logic_write_i32(&mut self, ptr: u32, value: i32) -> VmResult<()> {
        self.logic_write(ptr, &value.to_le_bytes())
    }

    fn logic_write_i32s(&mut self, ptr: u32, values: &[i32]) -> VmResult<()> {
        let bytes: Vec<u8> = values
            .iter()
            .flat_map(|value| value.to_le_bytes())
            .collect();
        self.logic_write(ptr, &bytes)
    }
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn script_statuses_follow_sub_497120() {
        assert_eq!(status_to_script(0), 0);
        assert_eq!(status_to_script(status::NO_HANDLE), 1);
        assert_eq!(status_to_script(status::BAD_DIRECTION), 12);
        assert_eq!(status_to_script(0x8000_000C), -1);
        assert_eq!(status_to_script(0x9000_0003), 17);
        assert_eq!(status_to_script(0xA000_0000), 1);
        assert_eq!(status_to_script(0xA000_0003), 20);
        assert_eq!(status_to_script(0xA000_0004), 8);
        assert_eq!(status_to_script(0xA000_0006), 22);
        assert_eq!(status_to_script(status::BROKEN_PATH), -2);
        assert_eq!(status_to_script(status::FAILED), -1);
    }
}

//! `DCTELgclGrdFldMngr` / `DCTELgclGrdFld` (0xD0:00..2D): a strategy-game
//! tile map with units, movement search and line-of-fire tests. Port of
//! `src/engine/logic_grid_field.c`; function names cite the BGI 1.622 address.
//! Statuses are the target's internal codes (see `super::status`).

use super::msvc_qsort::qsort;
use super::spline::CSpline;
use super::status::*;

/// One map tile as the script lays it out (16 bytes).
#[derive(Debug, Clone, Copy, Default, PartialEq, Eq)]
pub(crate) struct Tile {
    pub height: i32,
    /// Extra move cost; a unit's cost table is added in per search.
    pub cost: i32,
    /// 1 occupied (per search), 2 wall, 4 not targetable, 8 no entry.
    pub flags: u32,
    /// Bits 28..31: height above `height`; low byte: zone-of-control bits.
    pub extra: u32,
}

impl Tile {
    pub const SIZE: usize = 16;

    pub fn from_bytes(bytes: &[u8]) -> Self {
        let word = |i: usize| u32::from_le_bytes(bytes[i * 4..i * 4 + 4].try_into().unwrap());
        Self {
            height: word(0) as i32,
            cost: word(1) as i32,
            flags: word(2),
            extra: word(3),
        }
    }

    fn top(&self) -> i32 {
        self.height.wrapping_add((self.extra >> 28) as i32)
    }
}

/// One tile of a movement search (24 bytes, copied out by D0:22).
#[derive(Debug, Clone, Copy, Default, PartialEq, Eq)]
pub(crate) struct Cell {
    pub visited: i32,
    /// Direction back to the parent; 1 marks the start.
    pub back_dir: i32,
    pub steps: i32,
    pub cost: i32,
    pub climb: i32,
    pub turns: i32,
}

impl Cell {
    pub const SIZE: usize = 24;

    pub fn to_bytes(self) -> [u8; Self::SIZE] {
        let mut out = [0; Self::SIZE];
        for (i, value) in [
            self.visited,
            self.back_dir,
            self.steps,
            self.cost,
            self.climb,
            self.turns,
        ]
        .into_iter()
        .enumerate()
        {
            out[i * 4..i * 4 + 4].copy_from_slice(&value.to_le_bytes());
        }
        out
    }

    /// (cost, climb, turns) ordered lexicographically against this cell.
    fn improved_by(&self, cost: i32, climb: i32, turns: i32) -> bool {
        (cost, climb, turns) < (self.cost, self.climb, self.turns)
    }
}

#[derive(Debug, Clone)]
struct CostTable {
    id: i32,
    cost: Vec<i32>,
}

#[derive(Debug, Clone)]
pub(crate) struct Unit {
    pub id: i32,
    pub x: i32,
    pub y: i32,
    pub dir: i32,
    /// Body height for line of fire.
    pub height: i32,
    /// Hit radius against the sub-tile centre distance table.
    pub radius: i32,
    /// bit0 flies, bit1 ignores zone of control, bit2 flat cost.
    pub move_flags: u32,
    /// Low byte: zone-of-control bits this unit projects.
    pub team: u32,
    pub table_id: i32,
    pub zoc_climb: i32,
    field: Option<GridField>,
    /// +0x2C: threat maps cached by sub_453C20 for the evaluator.
    cache: Vec<ThreatMaps>,
}

/// One weapon profile's threat maps: 7 ints per tile, [0] covered and
/// [1..=4] hits from directions 2..5.
#[derive(Debug, Clone)]
pub(crate) struct ThreatMaps {
    key: [i32; 5],
    map: Vec<i32>,
    map2: Option<Vec<i32>>,
}

/// `DCTELgclGrdFld`: one unit's last movement search.
#[derive(Debug, Clone)]
pub(crate) struct GridField {
    w: i32,
    h: i32,
    tiles: Vec<Tile>,
    cells: Vec<Cell>,
}

#[derive(Debug, Clone, Copy)]
struct Node {
    x: i32,
    y: i32,
    back_dir: i32,
    height: i32,
    steps: i32,
    cost: i32,
    climb: i32,
    turns: i32,
    remain: i32,
}

/// Sub-tile corner squares in 16-bit tile fractions, {x0, y0, x1, y1}.
const CORNER_RECT: [[i32; 4]; 4] = [
    [0, 0, 0x2000, 0x2000],
    [0xE000, 0, 0x10000, 0x2000],
    [0, 0xE000, 0x2000, 0x10000],
    [0xE000, 0xE000, 0x10000, 0x10000],
];

#[derive(Debug, Clone)]
pub(crate) struct GridFieldManager {
    /// Height units per tile; distances divide height differences by it.
    scale: i32,
    w: i32,
    h: i32,
    tiles: Vec<Tile>,
    next_table_id: i32,
    /// Oldest first; lookups walk newest first like the target's list.
    tables: Vec<CostTable>,
    next_unit_id: i32,
    units: Vec<Unit>,
    /// Per tile, the height of each inside corner (sub_455220).
    corners: Vec<[i32; 4]>,
    /// trunc(sqrt(dx²+dy²))·32 from each 1/16 sub-tile to the tile centre.
    center_dist: [[i32; 16]; 16],
}

/// sub_401000 (and its 21x21 cache read by sub_454B90): the angle of (x, y),
/// y up, in 16.16 degrees.
pub(crate) fn angle_of(x: i32, y: i32) -> i32 {
    if x == 0 {
        return match y {
            0 => 0,
            1.. => 0x5A_0000,
            _ => 0x10E_0000,
        };
    }
    let ratio = f64::from(y) / f64::from(x);
    let pi = std::f64::consts::PI;
    if x > 0 {
        if y >= 0 {
            (ratio.atan() * 11_796_480.0 / pi) as i32
        } else {
            0x168_0000 - (ratio.atan() * -11_796_480.0 / pi) as i32
        }
    } else {
        0xB4_0000 - (ratio.atan() * -11_796_480.0 / pi) as i32
    }
}

/// sub_454DE0: the direction from (fx, fy) toward (tx, ty) by 90° sectors.
pub(crate) fn direction_to(tx: i32, ty: i32, fx: i32, fy: i32) -> i32 {
    let degrees = (angle_of(tx.wrapping_sub(fx), fy.wrapping_sub(ty)) >> 16) as u32;
    if degrees.wrapping_sub(45) <= 89 {
        2
    } else if degrees.wrapping_sub(225) <= 89 {
        3
    } else if degrees.wrapping_sub(135) > 89 {
        5
    } else {
        4
    }
}

/// sub_4557B0: how far (16.16 degrees, 0..180) the target lies from the way
/// `dir` faces; `None` with 0x8000000B for an invalid direction.
pub(crate) fn facing_delta(tx: i32, ty: i32, ux: i32, uy: i32, dir: i32) -> Result<i32, u32> {
    let base = match dir {
        0 => return Ok(0),
        2 => 0x5A_0000,
        3 => 0x10E_0000,
        4 => 0xB4_0000,
        5 => 0,
        _ => return Err(BAD_DIRECTION),
    };
    let delta = angle_of(tx.wrapping_sub(ux), uy.wrapping_sub(ty))
        .wrapping_sub(base)
        .wrapping_abs();
    Ok(if delta > 0xB4_0000 {
        0x168_0000 - delta
    } else {
        delta
    })
}

/// sub_4548E0 with rel = 0: 2<->3, 4<->5.
fn reverse_dir(dir: i32) -> i32 {
    match dir {
        2 => 3,
        3 => 2,
        4 => 5,
        5 => 4,
        other => other,
    }
}

/// sub_4548A0.
fn step_dir(x: &mut i32, y: &mut i32, dir: i32) -> bool {
    match dir {
        2 => *y -= 1,
        3 => *y += 1,
        4 => *x -= 1,
        5 => *x += 1,
        _ => return false,
    }
    true
}

impl GridFieldManager {
    /// sub_454940.
    pub(crate) fn new(scale: i32) -> Self {
        let mut center_dist = [[0; 16]; 16];
        for (row, line) in center_dist.iter_mut().enumerate() {
            let dy = if row < 8 { 8 - row } else { row - 7 } as i32;
            for (column, entry) in line.iter_mut().enumerate() {
                let dx = if column < 8 { 8 - column } else { column - 7 } as i32;
                *entry = (f64::from(dx * dx + dy * dy).sqrt() as i32) << 8 >> 3;
            }
        }
        Self {
            scale,
            w: 0,
            h: 0,
            tiles: Vec::new(),
            next_table_id: 0x1000_0000,
            tables: Vec::new(),
            next_unit_id: 0x2000_0000,
            units: Vec::new(),
            corners: Vec::new(),
            center_dist,
        }
    }

    pub(crate) fn map_cells(&self) -> usize {
        (self.w * self.h) as usize
    }

    fn has_map(&self) -> bool {
        !self.tiles.is_empty()
    }

    fn index(&self, x: i32, y: i32) -> usize {
        (y * self.w + x) as usize
    }

    fn in_map(&self, x: i32, y: i32) -> bool {
        (0..self.w).contains(&x) && (0..self.h).contains(&y)
    }

    /// D0:04 sub_453480. `tiles` holds w*h entries.
    pub(crate) fn set_map(&mut self, w: i32, h: i32, tiles: Vec<Tile>) -> u32 {
        if w == 0 || h == 0 {
            return BAD_ARGUMENT;
        }
        self.units.clear();
        self.tables.clear();
        self.corners.clear();
        self.w = w;
        self.h = h;
        self.tiles = tiles;
        self.build_corners();
        0
    }

    /// sub_455220: a vertex where tile (x,y) is lower than both (x,y-1) and
    /// (x+1,y) gives the four tiles around it min(top above, top right).
    fn build_corners(&mut self) {
        let mut corners = vec![[0; 4]; self.map_cells()];
        for y in 1..self.h {
            for x in 0..self.w - 1 {
                let above = self.tiles[self.index(x, y - 1)].top();
                let right = self.tiles[self.index(x + 1, y)].top();
                let here = self.tiles[self.index(x, y)].height;
                if here < above && here < right {
                    let corner = above.min(right);
                    corners[self.index(x, y - 1)][3] = corner;
                    corners[self.index(x + 1, y - 1)][2] = corner;
                    corners[self.index(x, y)][1] = corner;
                    corners[self.index(x + 1, y)][0] = corner;
                }
            }
        }
        self.corners = corners;
    }

    /// D0:05 sub_453530. `cost` holds w*h entries.
    pub(crate) fn add_cost_table(&mut self, cost: Vec<i32>) -> Result<i32, u32> {
        if !self.has_map() {
            return Err(NO_MAP);
        }
        let id = self.next_table_id;
        self.next_table_id = id.wrapping_add(1);
        self.tables.push(CostTable { id, cost });
        Ok(id)
    }

    /// D0:10 sub_4535B0.
    pub(crate) fn add_unit(&mut self) -> Result<i32, u32> {
        if !self.has_map() {
            return Err(NO_MAP);
        }
        let id = self.next_unit_id;
        self.next_unit_id = id.wrapping_add(1);
        self.units.push(Unit {
            id,
            x: -1,
            y: -1,
            dir: 0,
            height: 3,
            radius: 128,
            move_flags: 0,
            team: 0,
            table_id: 0,
            zoc_climb: 1,
            field: None,
            cache: Vec::new(),
        });
        Ok(id)
    }

    /// sub_4542A0 (newest first).
    pub(crate) fn unit_mut(&mut self, id: i32) -> Result<&mut Unit, u32> {
        self.units
            .iter_mut()
            .rev()
            .find(|unit| unit.id == id)
            .ok_or(NO_UNIT)
    }

    /// sub_454270: the newest unit standing on (x, y).
    fn unit_at(&self, x: i32, y: i32) -> Option<&Unit> {
        self.units
            .iter()
            .rev()
            .find(|unit| unit.x == x && unit.y == y)
    }

    /// sub_454370.
    fn table(&self, id: i32) -> Option<&[i32]> {
        self.tables
            .iter()
            .rev()
            .find(|table| table.id == id)
            .map(|table| table.cost.as_slice())
    }

    /// D0:11 sub_453630.
    pub(crate) fn remove_unit(&mut self, id: i32) -> u32 {
        match self.units.iter().rposition(|unit| unit.id == id) {
            Some(index) => {
                self.units.remove(index);
                0
            }
            None => NO_UNIT,
        }
    }

    /// D0:12 sub_453680.
    pub(crate) fn drop_search(&mut self, id: i32) -> u32 {
        match self.unit_mut(id) {
            Ok(unit) => {
                unit.field = None;
                0
            }
            Err(status) => status,
        }
    }

    /// D0:14 sub_4536B0.
    pub(crate) fn set_position(&mut self, id: i32, x: i32, y: i32) -> u32 {
        let in_map = self.in_map(x, y);
        match self.unit_mut(id) {
            Ok(_) if !in_map => OUT_OF_MAP,
            Ok(unit) => {
                unit.x = x;
                unit.y = y;
                0
            }
            Err(status) => status,
        }
    }

    /// D0:15 sub_453790.
    pub(crate) fn set_size(&mut self, id: i32, height: i32, radius: i32) -> u32 {
        match self.unit_mut(id) {
            Ok(_) if height < 0 => BAD_HEIGHT,
            Ok(_) if radius < 0 => BAD_RADIUS,
            Ok(unit) => {
                unit.height = height;
                unit.radius = radius;
                0
            }
            Err(status) => status,
        }
    }

    /// D0:16 sub_4537D0.
    pub(crate) fn set_flags(&mut self, id: i32, move_flags: u32, team: u32) -> u32 {
        match self.unit_mut(id) {
            Ok(unit) => {
                unit.move_flags = move_flags;
                unit.team = team;
                0
            }
            Err(status) => status,
        }
    }

    /// D0:17 sub_453800.
    pub(crate) fn set_table(&mut self, id: i32, table_id: i32) -> u32 {
        let exists = self.table(table_id).is_some();
        match self.unit_mut(id) {
            Ok(_) if !exists => NO_TABLE,
            Ok(unit) => {
                unit.table_id = table_id;
                0
            }
            Err(status) => status,
        }
    }

    /// D0:18 sub_453720 (vtbl+18 sub_454F50 accepts 0 and 2..5).
    pub(crate) fn set_dir(&mut self, id: i32, dir: i32) -> u32 {
        match self.unit_mut(id) {
            Ok(_) if !(dir == 0 || (2..=5).contains(&dir)) => BAD_DIRECTION,
            Ok(unit) => {
                unit.dir = dir;
                0
            }
            Err(status) => status,
        }
    }

    /// sub_454F80: the unit's tile and its four neighbours within
    /// `zoc_climb` height get the unit's team byte.
    fn project_zone(&self, tiles: &mut [Tile], unit: &Unit) {
        let origin = tiles[self.index(unit.x, unit.y)].height;
        for (dx, dy) in [(0, 0), (0, -1), (0, 1), (-1, 0), (1, 0)] {
            let (x, y) = (unit.x + dx, unit.y + dy);
            if !self.in_map(x, y) {
                continue;
            }
            let tile = &mut tiles[self.index(x, y)];
            if origin.wrapping_sub(tile.height).wrapping_abs() <= unit.zoc_climb {
                tile.extra |= unit.team & 0xFF;
            }
        }
    }

    /// sub_454390: the field's private map. The target writes the occupied
    /// bit for a unit that was never placed at index -(w+1); the port skips
    /// such units (no in-map tile changes either way).
    fn search_map(&self, table_id: i32, mark_units: bool) -> Vec<Tile> {
        let mut tiles = self.tiles.clone();
        if let Some(cost) = self.table(table_id) {
            for (tile, extra) in tiles.iter_mut().zip(cost) {
                tile.cost = tile.cost.wrapping_add(*extra);
            }
        }
        for unit in self.units.iter().rev() {
            if !mark_units || !self.in_map(unit.x, unit.y) {
                continue;
            }
            tiles[self.index(unit.x, unit.y)].flags |= 1;
            self.project_zone(&mut tiles, unit);
        }
        tiles
    }

    /// D0:20 sub_453830: search from the unit's tile with `jump` (largest
    /// climb) and `move` (budget, < 1 unlimited), stopping early once
    /// (dest_x, dest_y) is settled.
    pub(crate) fn search(&mut self, id: i32, jump: i32, budget: i32, dest: (i32, i32)) -> u32 {
        self.search_with(id, jump, budget, dest, true)
    }

    /// sub_453830 with its last argument: whether units occupy tiles and
    /// project zones of control.
    pub(crate) fn search_with(
        &mut self,
        id: i32,
        jump: i32,
        budget: i32,
        dest: (i32, i32),
        mark_units: bool,
    ) -> u32 {
        let has_map = self.has_map();
        let (w, h) = (self.w, self.h);
        let Some(index) = self.units.iter().rposition(|unit| unit.id == id) else {
            return NO_UNIT;
        };
        if !has_map {
            // sub_457B40 finds no map in the field: 0x80000002, mapped to -1.
            self.units[index].field = Some(GridField::empty());
            return SEARCH_FAILED;
        }
        let tiles = self.search_map(self.units[index].table_id, mark_units);
        let unit = &self.units[index];
        let mut field = GridField {
            w,
            h,
            tiles,
            cells: Vec::new(),
        };
        field.search(unit, jump, budget, dest);
        self.units[index].field = Some(field);
        0
    }

    fn field(&self, id: i32) -> Result<&GridField, u32> {
        let unit = self
            .units
            .iter()
            .rev()
            .find(|unit| unit.id == id)
            .ok_or(NO_UNIT)?;
        unit.field.as_ref().ok_or(NO_SEARCH)
    }

    /// D0:21 sub_453930: steps to (x, y) and, with `want_dirs`, the forward
    /// directions from the unit.
    pub(crate) fn path(
        &self,
        id: i32,
        x: i32,
        y: i32,
        want_dirs: bool,
    ) -> Result<(i32, Vec<i32>), u32> {
        let field = self.field(id)?;
        field.path(x, y, want_dirs).map_err(|status| match status {
            NO_CELLS => NO_SEARCH,
            NOT_REACHED | OUT_OF_FIELD => UNREACHABLE,
            BROKEN_PATH => BROKEN_PATH,
            _ => FAILED,
        })
    }

    /// D0:22 sub_453B30.
    pub(crate) fn cells(&self, id: i32) -> Result<&[Cell], u32> {
        let field = self.field(id)?;
        if field.cells.is_empty() {
            return Err(NO_SEARCH);
        }
        Ok(&field.cells)
    }

    /// D0:23 sub_453B80 (unsorted).
    pub(crate) fn reachable(&self, id: i32) -> Result<Vec<(i32, i32)>, u32> {
        let field = self.field(id)?;
        if field.cells.is_empty() {
            return Err(FAILED);
        }
        Ok(field.reachable())
    }

    /// vtbl+28 sub_4551F0 -> sub_454580.
    fn distance(&self, x0: i32, y0: i32, h0: i32, x1: i32, y1: i32, h1: i32) -> f64 {
        let dx = x0.wrapping_sub(x1);
        let dy = y0.wrapping_sub(y1);
        let flat = dx.wrapping_mul(dx).wrapping_add(dy.wrapping_mul(dy));
        let lift = f64::from(h0.wrapping_sub(h1)) / f64::from(self.scale);
        (f64::from(flat) + lift * lift).sqrt()
    }

    /// D0:28 vtbl+4 sub_454BC0: tiles within `range` of (x, y) that a shot
    /// reaches, row-major, with their 16.16 distances. Lower targets are in
    /// reach `bonus`/256 tile further per height unit. A target height outside
    /// 0..31 reads past the target's 32-entry table; the port skips it.
    #[allow(clippy::too_many_arguments)]
    pub(crate) fn targets(
        &self,
        x: i32,
        y: i32,
        range: i32,
        bonus: i32,
        arc: i32,
        units_only: bool,
        ignore_units: bool,
        exclude_self: bool,
    ) -> Result<Vec<(i32, i32, i32)>, u32> {
        if !self.has_map() {
            return Err(NO_MAP);
        }
        // The target reads the shooter's tile unchecked; off the map every
        // line of fire fails (0x80000008), so nothing is found.
        if !self.in_map(x, y) {
            return Ok(Vec::new());
        }
        let origin = self.tiles[self.index(x, y)].height;
        let step = range.wrapping_mul(bonus) << 8;
        let mut reach = (range << 16).wrapping_add(step.wrapping_mul(origin));
        let mut threshold = [0; 32];
        for entry in &mut threshold {
            *entry = if reach < 0 { 0 } else { reach + 0x4000 };
            reach = reach.wrapping_sub(step);
        }
        let mut found = Vec::new();
        for ty in 0..self.h {
            for tx in 0..self.w {
                let height = self.tiles[self.index(tx, ty)].height;
                let distance = (self.distance(tx, ty, height, x, y, origin) * 65_536.0) as i32;
                let Some(&limit) = usize::try_from(height).ok().and_then(|h| threshold.get(h))
                else {
                    continue;
                };
                if distance >= limit {
                    continue;
                }
                if units_only && self.unit_at(tx, ty).is_none() {
                    continue;
                }
                let hit = self.line_of_fire(tx, ty, x, y, arc, ignore_units) == Ok(true);
                if hit || (!exclude_self && tx == x && ty == y) {
                    found.push((tx, ty, distance));
                }
            }
        }
        Ok(found)
    }

    /// vtbl+34 sub_455360: does a shot from (x1, y1) reach (x0, y0)?
    pub(crate) fn line_of_fire(
        &self,
        x0: i32,
        y0: i32,
        x1: i32,
        y1: i32,
        arc: i32,
        ignore_units: bool,
    ) -> Result<bool, u32> {
        if !self.has_map() {
            return Err(NO_MAP);
        }
        if !self.in_map(x0, y0) || (x1 == x0 && y1 == y0) {
            return Err(OUT_OF_MAP);
        }
        if !self.in_map(x1, y1) {
            return Err(BAD_ORIGIN);
        }
        let target = self.tiles[self.index(x0, y0)];
        let shooter = self.tiles[self.index(x1, y1)];
        let distance = self.distance(x0, y0, target.height, x1, y1, shooter.height);
        let steps = (distance * 8.0) as i64 as i32;
        let lift = |x, y| self.unit_at(x, y).map_or(3, |unit| unit.height) << 15;
        let (lift1, lift0) = (lift(x1, y1), lift(x0, y0));
        let mut spline = CSpline::default();
        spline.add(
            (x1 << 16) + 0x8000,
            (y1 << 16) + 0x8000,
            (shooter.height << 16).wrapping_add(lift1),
        );
        if arc >= 1 {
            let raise = (f64::from(arc) * distance * -0.003_906_25 * 65_536.0) as i32;
            spline.add(
                (x0 + x1 + 1) << 15,
                (y0 + y1 + 1) << 15,
                (shooter.height.wrapping_add(target.height) << 15)
                    .wrapping_sub(raise)
                    .wrapping_add(lift0.wrapping_add(lift1) >> 1),
            );
        }
        spline.add(
            (x0 << 16) + 0x8000,
            (y0 << 16) + 0x8000,
            (target.height << 16).wrapping_add(lift0),
        );
        spline.set_duration(steps as u32);
        let mut tick = 0u32;
        while tick <= steps as u32 {
            if let Some([px, py, pz]) = spline.sample(tick) {
                let (tx, ty, z) = (px >> 16, py >> 16, pz >> 16);
                // Samples stay between the two tile centres; guard anyway.
                if !self.in_map(tx, ty) {
                    return Ok(false);
                }
                let tile = self.tiles[self.index(tx, ty)];
                if tile.flags & 2 != 0 || tile.top() > z {
                    return Ok(false);
                }
                let (fx, fy) = (px & 0xFFFF, py & 0xFFFF);
                let corners = self.corners[self.index(tx, ty)];
                let blocked = corners.iter().zip(CORNER_RECT).any(|(&corner, rect)| {
                    corner > z && rect[0] <= fx && fx < rect[2] && rect[1] <= fy && fy < rect[3]
                });
                if blocked {
                    return Ok(false);
                }
                if !ignore_units && (tx != x1 || ty != y1) {
                    if let Some(unit) = self.unit_at(tx, ty) {
                        if unit.height.wrapping_add(tile.height) > z
                            && unit.radius
                                >= self.center_dist[(fy >> 12) as usize][(fx >> 12) as usize]
                        {
                            return Ok(tx == x0 && ty == y0);
                        }
                    }
                }
            }
            tick = tick.wrapping_add(1);
            if tick == 0 {
                break;
            }
        }
        Ok(target.flags & 4 == 0)
    }

    /// D0:2C vtbl+38 via sub_453BE0: facing delta from a unit to (tx, ty).
    pub(crate) fn unit_facing_delta(&self, id: i32, tx: i32, ty: i32) -> Result<i32, u32> {
        let unit = self
            .units
            .iter()
            .rev()
            .find(|unit| unit.id == id)
            .ok_or(NO_UNIT)?;
        facing_delta(tx, ty, unit.x, unit.y, unit.dir)
    }
}

/// The calls the evaluator makes on its own copy of the grid.
impl GridFieldManager {
    /// vtbl+20 + sub_4532A0 (via sub_453260): a copy with the map, cost
    /// tables, units and their threat caches, but no searches. None without
    /// a map.
    pub(crate) fn snapshot(&self) -> Option<Self> {
        if !self.has_map() {
            return None;
        }
        let mut copy = Self::new(self.scale);
        copy.set_map(self.w, self.h, self.tiles.clone());
        copy.tables = self.tables.clone();
        let highest = self.tables.iter().map(|t| t.id as u32).max().unwrap_or(0);
        copy.next_table_id = highest.wrapping_add(1) as i32;
        copy.units = self
            .units
            .iter()
            .map(|unit| Unit {
                field: None,
                ..unit.clone()
            })
            .collect();
        Some(copy)
    }

    pub(crate) fn size(&self) -> Result<(i32, i32), u32> {
        if self.has_map() {
            Ok((self.w, self.h))
        } else {
            Err(NO_MAP)
        }
    }

    fn unit(&self, id: i32) -> Result<&Unit, u32> {
        self.units
            .iter()
            .rev()
            .find(|unit| unit.id == id)
            .ok_or(NO_UNIT)
    }

    /// sub_4536F0.
    pub(crate) fn unit_pos(&self, id: i32) -> Result<(i32, i32), u32> {
        self.unit(id).map(|unit| (unit.x, unit.y))
    }

    /// sub_453760.
    pub(crate) fn unit_dir(&self, id: i32) -> Result<i32, u32> {
        self.unit(id).map(|unit| unit.dir)
    }

    /// vtbl+14 sub_454F00: the tile next to the unit in direction `dir`
    /// (0..3 when `relative`, else 2..5). Not bounds-checked.
    pub(crate) fn step_from_unit(
        &self,
        id: i32,
        dir: i32,
        relative: bool,
    ) -> Result<(i32, i32), u32> {
        let unit = self.unit(id)?;
        let (mut x, mut y) = (unit.x, unit.y);
        let dir = if relative { dir.wrapping_add(2) } else { dir };
        if step_dir(&mut x, &mut y, dir) {
            Ok((x, y))
        } else {
            Err(BAD_DIRECTION)
        }
    }

    /// sub_453AC0: the search cost of (x, y).
    pub(crate) fn cell_cost(&self, id: i32, x: i32, y: i32) -> Result<i32, u32> {
        let field = self.field(id)?;
        field.cell(x, y).map(|cell| cell.cost).map_err(field_status)
    }

    /// sub_453A50: the direction a unit faces after walking to (x, y)
    /// (1 for the start).
    pub(crate) fn cell_facing(&self, id: i32, x: i32, y: i32) -> Result<i32, u32> {
        let field = self.field(id)?;
        field
            .cell(x, y)
            .map(|cell| reverse_dir(cell.back_dir))
            .map_err(field_status)
    }

    /// sub_4538C0 -> DCTELgclGrdFld vtbl+4 sub_454600 with a height limit of
    /// 1: for the four sides of (x, y) (y-1, y+1, x-1, x+1) that the search
    /// reached, that are open and within one height step, the step count and
    /// the tile; -1 elsewhere (six slots, the last two always -1).
    pub(crate) fn neighbors(
        &self,
        id: i32,
        x: i32,
        y: i32,
    ) -> Result<([i32; 6], [(i32, i32); 6]), u32> {
        let field = self.field(id)?;
        if field.cells.is_empty() {
            return Err(NO_SEARCH);
        }
        Ok(field.neighbors(x, y, 1))
    }

    /// sub_4539B0: the tiles walked from the unit's current tile to (x, y).
    pub(crate) fn path_points(&self, id: i32, x: i32, y: i32) -> Result<Vec<(i32, i32)>, u32> {
        let (_, dirs) = self.path(id, x, y, true)?;
        let unit = self.unit(id)?;
        let (mut cx, mut cy) = (unit.x, unit.y);
        let mut points = Vec::with_capacity(dirs.len());
        for dir in dirs {
            if !step_dir(&mut cx, &mut cy, dir) {
                return Err(UNREACHABLE);
            }
            points.push((cx, cy));
        }
        Ok(points)
    }

    /// sub_453B80 with `sorted`: reachable tiles ordered by cost with the
    /// CRT qsort (sub_4583C0 compares cell costs).
    pub(crate) fn reachable_sorted(&self, id: i32) -> Result<Vec<(i32, i32)>, u32> {
        let field = self.field(id)?;
        if field.cells.is_empty() {
            return Err(FAILED);
        }
        let mut tiles: Vec<(i32, i32, i32)> = field
            .reachable()
            .into_iter()
            .map(|(x, y)| (x, y, field.cells[field.index(x, y)].cost))
            .collect();
        qsort(&mut tiles, |a, b| a.2.wrapping_sub(b.2));
        Ok(tiles.into_iter().map(|(x, y, _)| (x, y)).collect())
    }

    /// sub_453C20: rebuild the unit's threat cache for `keys` (weapon
    /// profiles {kind, range, bonus, arc, radius}); see the C file.
    pub(crate) fn build_threat_maps(
        &mut self,
        id: i32,
        jump: i32,
        budget: i32,
        keys: &[[i32; 5]],
        mark_units: bool,
        double_move: bool,
    ) -> u32 {
        let Some(index) = self.units.iter().rposition(|unit| unit.id == id) else {
            return NO_UNIT;
        };
        self.units[index].cache.clear();
        let reach = if double_move {
            budget.wrapping_mul(2)
        } else {
            budget
        };
        self.search_with(id, jump, reach, (-1, -1), mark_units);
        let n = self.map_cells();
        let origin = (self.units[index].x, self.units[index].y);
        let mut tiles = vec![origin];
        if let Ok(reachable) = self.reachable(id) {
            tiles.extend(reachable);
        }
        for key in keys {
            if self.units[index]
                .cache
                .iter()
                .any(|entry| entry.key == *key)
            {
                continue;
            }
            let mut map = vec![0; 7 * n];
            let mut map2 = vec![0; 7 * n];
            for &(x, y) in &tiles {
                // The target keeps a stale cost when the lookup fails; it
                // cannot fail for these tiles, so they are skipped instead.
                let Ok(cost) = self.cell_cost(id, x, y) else {
                    continue;
                };
                let use_second = if cost <= budget {
                    false
                } else if double_move && key[0] == 0 && cost <= budget.wrapping_mul(2) {
                    true
                } else {
                    continue;
                };
                let hits = self
                    .targets(x, y, key[1], key[2], key[3], false, !mark_units, true)
                    .unwrap_or_default();
                let target = if use_second { &mut map2 } else { &mut map };
                for (hx, hy, _) in hits {
                    if key[4] >= 1 {
                        let area = self
                            .targets(hx, hy, key[4], 0, 0, false, false, true)
                            .unwrap_or_default();
                        for (ax, ay, _) in area {
                            let side = if (ax, ay) == (hx, hy) {
                                direction_to(x, y, hx, hy)
                            } else {
                                direction_to(hx, hy, ax, ay)
                            };
                            let slot = 7 * self.index(ax, ay);
                            target[slot + (side - 1) as usize] += 1;
                            target[slot] = 1;
                        }
                    } else if key[4] == 0 {
                        let side = direction_to(x, y, hx, hy);
                        let slot = 7 * self.index(hx, hy);
                        target[slot + (side - 1) as usize] += 1;
                        target[slot] = 1;
                    }
                }
            }
            let map2 = (double_move && key[0] == 0).then_some(map2);
            self.units[index].cache.push(ThreatMaps {
                key: *key,
                map,
                map2,
            });
        }
        0
    }

    /// sub_4541A0: a cached threat map (`second`: the double-move one).
    pub(crate) fn threat_map(
        &self,
        id: i32,
        key: &[i32; 5],
        second: bool,
    ) -> Result<Vec<i32>, u32> {
        let unit = self.unit(id)?;
        let entry = unit
            .cache
            .iter()
            .find(|entry| entry.key == *key)
            .ok_or(NO_THREAT_KEY)?;
        let map = if second {
            entry.map2.as_ref()
        } else {
            Some(&entry.map)
        };
        map.cloned().ok_or(NO_THREAT_MAP)
    }
}

/// The manager's mapping of DCTELogicalField statuses (sub_453A50/sub_453AC0).
fn field_status(status: u32) -> u32 {
    match status {
        NO_CELLS => NO_SEARCH,
        NOT_REACHED | OUT_OF_FIELD => UNREACHABLE,
        _ => FAILED,
    }
}

impl GridField {
    fn empty() -> Self {
        Self {
            w: 0,
            h: 0,
            tiles: Vec::new(),
            cells: Vec::new(),
        }
    }

    fn index(&self, x: i32, y: i32) -> usize {
        (y * self.w + x) as usize
    }

    /// sub_458120.
    fn push(&self, queue: &mut Vec<Node>, node: Node) -> u32 {
        if !(0..self.w).contains(&node.x) || !(0..self.h).contains(&node.y) {
            return OUT_OF_FIELD;
        }
        let cell = &self.cells[self.index(node.x, node.y)];
        // The next step costs at least 1.
        if cell.visited != 0 && !((node.cost + 1, node.climb) < (cell.cost, cell.climb)) {
            return PRUNED;
        }
        queue.push(node);
        0
    }

    /// sub_457B40.
    fn search(&mut self, unit: &Unit, jump: i32, budget: i32, dest: (i32, i32)) {
        self.cells = vec![Cell::default(); (self.w * self.h) as usize];
        let flies = unit.move_flags & 1 != 0;
        let flat = unit.move_flags & 4 != 0;
        let zone_mask = if unit.move_flags & 2 != 0 {
            0
        } else {
            !unit.team & 0xFF
        };
        let mut queue = Vec::new();
        self.push(
            &mut queue,
            Node {
                x: unit.x,
                y: unit.y,
                back_dir: 1,
                height: 0,
                steps: 0,
                cost: 0,
                climb: 0,
                turns: 0,
                remain: if budget < 1 { i32::MAX } else { budget },
            },
        );
        let mut next = 0;
        while next < queue.len() {
            let node = queue[next];
            next += 1;
            if self.relax(&mut queue, &node, jump, flies, flat, zone_mask)
                && (node.x, node.y) == dest
            {
                break;
            }
        }
    }

    /// sub_458210: true when the cell took the node's values.
    fn relax(
        &mut self,
        queue: &mut Vec<Node>,
        node: &Node,
        jump: i32,
        flies: bool,
        flat: bool,
        zone_mask: u32,
    ) -> bool {
        let index = self.index(node.x, node.y);
        if self.cells[index].visited != 0
            && !self.cells[index].improved_by(node.cost + 1, node.climb, node.turns)
        {
            return false;
        }
        let tile = self.tiles[index];
        let start = node.back_dir == 1;
        if tile.flags & 2 != 0
            || (!flies && (tile.flags & 0xC != 0 || (!start && tile.flags & 1 != 0)))
        {
            self.cells[index].visited = 1;
            return false;
        }
        let top = tile.top();
        let from = if start { top } else { node.height };
        let stand = if flies && from >= top { from } else { top };
        let diff = stand.wrapping_sub(from).wrapping_abs();
        if jump < diff && !start {
            return false;
        }
        let mut step = 0;
        if !start {
            step = tile.cost.wrapping_add(1);
            if !flat {
                if flies {
                    if diff >= 4 {
                        step += diff - 3;
                    }
                } else if diff >= 2 {
                    step += diff - 1;
                }
            }
        }
        let cost = node.cost.wrapping_add(step);
        let climb = node.climb.wrapping_add(diff);
        if self.cells[index].visited != 0 && !self.cells[index].improved_by(cost, climb, node.turns)
        {
            return false;
        }
        let remain = node.remain.wrapping_sub(step);
        if remain < 0 {
            return false;
        }
        self.cells[index] = Cell {
            visited: 1,
            back_dir: node.back_dir,
            steps: node.steps,
            cost,
            climb,
            turns: node.turns,
        };
        if (remain >= 1 && zone_mask & tile.extra == 0) || start {
            self.expand(queue, node, stand, cost, climb, remain);
        }
        true
    }

    /// DCTELgclGrdFld vtbl+8 sub_454770: the four neighbours except the way
    /// back, in the order y-1, y+1, x-1, x+1.
    fn expand(
        &self,
        queue: &mut Vec<Node>,
        node: &Node,
        height: i32,
        cost: i32,
        climb: i32,
        remain: i32,
    ) {
        let back = self.cells[self.index(node.x, node.y)].back_dir;
        for (dx, dy, child_back) in [(0, -1, 3), (0, 1, 2), (-1, 0, 5), (1, 0, 4)] {
            if back == reverse_dir(child_back) {
                continue;
            }
            self.push(
                queue,
                Node {
                    x: node.x + dx,
                    y: node.y + dy,
                    back_dir: child_back,
                    height,
                    steps: node.steps + 1,
                    cost,
                    climb,
                    turns: node.turns + i32::from(back != child_back),
                    remain,
                },
            );
        }
    }

    /// sub_457DE0 / sub_457D70 lookup.
    fn cell(&self, x: i32, y: i32) -> Result<Cell, u32> {
        if self.cells.is_empty() {
            return Err(NO_CELLS);
        }
        if !(0..self.w).contains(&x) || !(0..self.h).contains(&y) {
            return Err(OUT_OF_FIELD);
        }
        let cell = self.cells[self.index(x, y)];
        if cell.back_dir == 0 {
            return Err(NOT_REACHED);
        }
        Ok(cell)
    }

    /// vtbl+4 sub_454600. The target reads (x, y)'s height unchecked; off
    /// the field the port reports no neighbours.
    fn neighbors(&self, x: i32, y: i32, max_climb: i32) -> ([i32; 6], [(i32, i32); 6]) {
        let mut steps = [-1; 6];
        let mut tiles = [(0, 0); 6];
        if !(0..self.w).contains(&x) || !(0..self.h).contains(&y) {
            return (steps, tiles);
        }
        let here = self.tiles[self.index(x, y)].height;
        for (slot, (dx, dy)) in [(0, -1), (0, 1), (-1, 0), (1, 0)].into_iter().enumerate() {
            let (nx, ny) = (x + dx, y + dy);
            if !(0..self.w).contains(&nx) || !(0..self.h).contains(&ny) {
                continue;
            }
            let index = self.index(nx, ny);
            if self.cells[index].back_dir == 0 || self.tiles[index].flags & 0xD != 0 {
                continue;
            }
            if self.tiles[index].height.wrapping_sub(here).wrapping_abs() > max_climb {
                continue;
            }
            steps[slot] = self.cells[index].steps;
            tiles[slot] = (nx, ny);
        }
        (steps, tiles)
    }

    /// sub_457C10.
    fn path(&self, x: i32, y: i32, want_dirs: bool) -> Result<(i32, Vec<i32>), u32> {
        if self.cells.is_empty() {
            return Err(NO_CELLS);
        }
        if !(0..self.w).contains(&x) || !(0..self.h).contains(&y) {
            return Err(OUT_OF_FIELD);
        }
        let steps = self.cells[self.index(x, y)].steps;
        if self.cells[self.index(x, y)].back_dir == 0 {
            return Err(NOT_REACHED);
        }
        let mut dirs = Vec::new();
        if steps >= 1 && want_dirs {
            dirs = vec![0; steps as usize];
            let (mut cx, mut cy) = (x, y);
            for slot in (0..steps as usize).rev() {
                let back = self.cells[self.index(cx, cy)].back_dir;
                dirs[slot] = reverse_dir(back);
                if !step_dir(&mut cx, &mut cy, back) {
                    return Err(BROKEN_PATH);
                }
            }
        }
        Ok((steps, dirs))
    }

    /// sub_457E80 unsorted: settled tiles (cost > 0) that are not occupied,
    /// untargetable or closed, row-major.
    fn reachable(&self) -> Vec<(i32, i32)> {
        let mut out = Vec::new();
        for y in 0..self.h {
            for x in 0..self.w {
                let index = self.index(x, y);
                if self.cells[index].cost > 0 && self.tiles[index].flags & 0xD == 0 {
                    out.push((x, y));
                }
            }
        }
        out
    }
}

#[cfg(test)]
mod tests {
    use super::*;

    fn flat_map(w: i32, h: i32) -> Vec<Tile> {
        vec![Tile::default(); (w * h) as usize]
    }

    fn placed(manager: &mut GridFieldManager, x: i32, y: i32) -> i32 {
        let id = manager.add_unit().unwrap();
        assert_eq!(manager.set_position(id, x, y), 0);
        id
    }

    #[test]
    fn angles_follow_the_target_quadrants() {
        assert_eq!(angle_of(1, 0), 0);
        assert_eq!(angle_of(0, 1), 0x5A_0000);
        assert_eq!(angle_of(-1, 0), 0xB4_0000);
        assert_eq!(angle_of(0, -1), 0x10E_0000);
        assert_eq!(angle_of(1, 1) >> 16, 45);
        assert_eq!(angle_of(-1, 1) >> 16, 135);
        assert_eq!(angle_of(-1, -1) >> 16, 225);
        assert_eq!(angle_of(1, -1) >> 16, 315);
        assert_eq!(direction_to(5, 0, 5, 5), 2);
        assert_eq!(direction_to(5, 9, 5, 5), 3);
        assert_eq!(direction_to(0, 5, 5, 5), 4);
        assert_eq!(direction_to(9, 5, 5, 5), 5);
        assert_eq!(facing_delta(5, 0, 5, 5, 3), Ok(0xB4_0000));
        assert_eq!(facing_delta(9, 5, 5, 5, 2), Ok(0x5A_0000));
        assert_eq!(facing_delta(9, 5, 5, 5, 7), Err(BAD_DIRECTION));
    }

    #[test]
    fn center_distance_table_matches_the_constructor() {
        let manager = GridFieldManager::new(1);
        // dx = dy = 8 at the corners, 1 at the four centre sub-tiles.
        assert_eq!(manager.center_dist[0][0], 11 * 32);
        assert_eq!(manager.center_dist[7][7], 32);
        assert_eq!(manager.center_dist[8][8], 32);
        assert_eq!(manager.center_dist[7][15], 8 * 32);
    }

    #[test]
    fn units_need_a_map_and_valid_values() {
        let mut manager = GridFieldManager::new(1);
        assert_eq!(manager.add_unit(), Err(NO_MAP));
        assert_eq!(manager.set_map(0, 3, Vec::new()), BAD_ARGUMENT);
        assert_eq!(manager.set_map(3, 3, flat_map(3, 3)), 0);
        let id = manager.add_unit().unwrap();
        assert_eq!(id, 0x2000_0000);
        assert_eq!(manager.set_position(id, 3, 0), OUT_OF_MAP);
        assert_eq!(manager.set_size(id, -1, 0), BAD_HEIGHT);
        assert_eq!(manager.set_size(id, 0, -1), BAD_RADIUS);
        assert_eq!(manager.set_dir(id, 1), BAD_DIRECTION);
        assert_eq!(manager.set_table(id, 0x1000_0000), NO_TABLE);
        let table = manager.add_cost_table(vec![0; 9]).unwrap();
        assert_eq!(table, 0x1000_0000);
        assert_eq!(manager.set_table(id, table), 0);
        assert_eq!(manager.remove_unit(id), 0);
        assert_eq!(manager.remove_unit(id), NO_UNIT);
    }

    #[test]
    fn search_spends_terrain_cost_and_stops_at_walls() {
        let mut manager = GridFieldManager::new(1);
        let mut tiles = flat_map(5, 1);
        tiles[2].cost = 2; // entering x=2 costs 3
        tiles[4].flags = 2; // wall
        manager.set_map(5, 1, tiles);
        let unit = placed(&mut manager, 0, 0);
        assert_eq!(manager.search(unit, 1, 0, (-1, -1)), 0);
        let cells = manager.cells(unit).unwrap();
        let costs: Vec<i32> = cells.iter().map(|cell| cell.cost).collect();
        assert_eq!(costs, [0, 1, 4, 5, 0]);
        assert_eq!(cells[4].visited, 1);
        assert_eq!(cells[4].back_dir, 0);
        assert_eq!(manager.reachable(unit).unwrap(), [(1, 0), (2, 0), (3, 0)]);
        assert_eq!(manager.path(unit, 3, 0, true), Ok((3, vec![5, 5, 5])));
        assert_eq!(manager.path(unit, 4, 0, true), Err(UNREACHABLE));
    }

    #[test]
    fn budget_and_climb_limit_the_search() {
        let mut manager = GridFieldManager::new(1);
        let mut tiles = flat_map(4, 1);
        tiles[2].height = 3; // a step of 3 from x=1
        manager.set_map(4, 1, tiles);
        let unit = placed(&mut manager, 0, 0);
        manager.search(unit, 2, 0, (-1, -1));
        assert_eq!(manager.cells(unit).unwrap()[2].visited, 0);
        manager.search(unit, 3, 0, (-1, -1));
        // 1 + (3 - 1) extra for the climb.
        assert_eq!(manager.cells(unit).unwrap()[2].cost, 1 + 3);
        assert_eq!(manager.cells(unit).unwrap()[2].climb, 3);
        manager.search(unit, 3, 3, (-1, -1));
        assert_eq!(manager.cells(unit).unwrap()[2].visited, 0);
    }

    #[test]
    fn zone_of_control_stops_movement_next_to_enemies() {
        let mut manager = GridFieldManager::new(1);
        manager.set_map(5, 1, flat_map(5, 1));
        let mover = placed(&mut manager, 0, 0);
        manager.set_flags(mover, 0, 1);
        let enemy = placed(&mut manager, 3, 0);
        manager.set_flags(enemy, 0, 2);
        manager.search(mover, 1, 0, (-1, -1));
        let cells = manager.cells(mover).unwrap();
        // x=2 is in the enemy's zone: entered but not left.
        assert_eq!(cells[2].cost, 2);
        assert_eq!(cells[3].visited, 0);
        // Ignoring zones (bit 1) reaches the enemy's tile, which is occupied.
        manager.set_flags(mover, 2, 1);
        manager.search(mover, 1, 0, (-1, -1));
        let cells = manager.cells(mover).unwrap();
        assert_eq!(
            (cells[3].visited, cells[3].cost, cells[4].visited),
            (1, 0, 0)
        );
        // Flying (bit 0) still stops in the zone; flying past it passes over.
        manager.set_flags(mover, 1, 1);
        manager.search(mover, 1, 0, (-1, -1));
        assert_eq!(manager.cells(mover).unwrap()[3].visited, 0);
        manager.set_flags(mover, 3, 1);
        manager.search(mover, 1, 0, (-1, -1));
        assert_eq!(manager.cells(mover).unwrap()[4].cost, 4);
    }

    #[test]
    fn ties_prefer_fewer_turns() {
        let mut manager = GridFieldManager::new(1);
        manager.set_map(3, 3, flat_map(3, 3));
        let unit = placed(&mut manager, 0, 0);
        manager.search(unit, 1, 0, (-1, -1));
        let (steps, dirs) = manager.path(unit, 2, 2, true).unwrap();
        assert_eq!(steps, 4);
        // Both L-shapes cost 4 with one turn after the first move; the first
        // queued neighbour (y+1) settles the corner first.
        assert_eq!(dirs, [3, 3, 5, 5]);
    }

    #[test]
    fn line_of_fire_is_blocked_by_walls_and_other_units() {
        let mut manager = GridFieldManager::new(1);
        let mut tiles = flat_map(5, 1);
        manager.set_map(5, 1, tiles.clone());
        placed(&mut manager, 0, 0);
        placed(&mut manager, 4, 0);
        assert_eq!(manager.line_of_fire(4, 0, 0, 0, 0, false), Ok(true));
        placed(&mut manager, 2, 0);
        assert_eq!(manager.line_of_fire(4, 0, 0, 0, 0, false), Ok(false));
        assert_eq!(manager.line_of_fire(4, 0, 0, 0, 0, true), Ok(true));
        // The arc's apex rises arc·distance/256 height units: 256 lifts the
        // shot from 1.5 to 5.5 over the blocker (body top 3).
        assert_eq!(manager.line_of_fire(4, 0, 0, 0, 256, false), Ok(true));
        tiles[2].flags = 2;
        manager.set_map(5, 1, tiles);
        assert_eq!(manager.line_of_fire(4, 0, 0, 0, 0, true), Ok(false));
        assert_eq!(manager.line_of_fire(0, 0, 0, 0, 0, true), Err(OUT_OF_MAP));
        assert_eq!(manager.line_of_fire(0, 0, 9, 0, 0, true), Err(BAD_ORIGIN));
    }

    #[test]
    fn targets_respect_range_and_line_of_fire() {
        let mut manager = GridFieldManager::new(1);
        manager.set_map(6, 1, flat_map(6, 1));
        placed(&mut manager, 0, 0);
        let found = manager.targets(0, 0, 3, 0, 0, false, true, true).unwrap();
        let tiles: Vec<_> = found.iter().map(|&(x, y, _)| (x, y)).collect();
        assert_eq!(tiles, [(1, 0), (2, 0), (3, 0)]);
        assert_eq!(found[2].2, 3 << 16);
        placed(&mut manager, 2, 0);
        let units = manager.targets(0, 0, 5, 0, 0, true, false, true).unwrap();
        // Without exclude_self the shooter's own tile is listed too.
        let with_self = manager.targets(0, 0, 5, 0, 0, true, false, false).unwrap();
        assert_eq!(
            with_self.iter().map(|&(x, _, _)| x).collect::<Vec<_>>(),
            [0, 2]
        );
        assert_eq!(units.iter().map(|&(x, _, _)| x).collect::<Vec<_>>(), [2]);
    }
}

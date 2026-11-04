//! `DCTESLGEvaluator` (0xD0:80..8E): the strategy-game AI. Port of
//! `src/engine/logic_slg_evaluator.c`; addresses are BGI 1.622. All grid
//! calls go to the evaluator's own copy of the grid. Arithmetic is 32-bit
//! wrapping like the target's `imul`/`add`.

use super::grid::{GridFieldManager, direction_to};
use super::msvc_qsort::qsort;
use super::status::*;
use std::rc::Rc;

/// Dwords in a unit record (0x8DC); the script owns the first 525 (0x834 bytes).
pub(crate) const RECORD_WORDS: usize = 567;
pub(crate) const SCRIPT_WORDS: usize = 525;
const MAX_UNITS: u32 = 64;
const MIN: i32 = i32::MIN;
const NDIRS: usize = 4; // GrdFldMngr vtbl+8 (`mov eax, 4`)

// Record fields (see the C file for the full layout).
const ID: usize = 0;
const TEAM: usize = 1;
const SIDE: usize = 2;
const TARGET: usize = 3;
const HP: usize = 7;
const HP_MAX: usize = 8;
const STATS: usize = 11;
const WAIT: usize = 75;
const BUFF_TIME: usize = 77;
const BUFF_VALUE: usize = 109;
const WAIT_BUFF: usize = 108;
const JUMP: usize = 141;
const MOVE: usize = 142;
const ATK_RANGE: usize = 149;
const ATK_KIND: usize = 152;
const ATK_POWER: usize = 153;
const SKILL: usize = 173;
const POS_X: usize = 525;
const POS_Y: usize = 526;
const DIR: usize = 527;
const GOAL_X: usize = 528;
const GOAL_Y: usize = 529;
const STEP_X: usize = 530;
const STEP_Y: usize = 531;
const ADVANCED: usize = 532;
const MAP: usize = 533;

// Candidate fields (76 bytes).
const C_X: usize = 0;
const C_Y: usize = 1;
const C_TYPE: usize = 2;
const C_SKILL: usize = 3;
const C_TX: usize = 4;
const C_TY: usize = 5;
const C_FACE: usize = 7;
const C_ARRIVE: usize = 8;
const C_COST: usize = 9;
const C_PARTNER: usize = 10;
const C_COST_W: usize = 11;
const C_ON_PATH: usize = 12;
const C_SCORE: usize = 13;
type Candidate = [i32; 19];

/// One unit: the record words plus the threat maps hung on [533..=566].
#[derive(Debug, Clone)]
pub(crate) struct Record {
    v: Box<[i32; RECORD_WORDS]>,
    maps: [Option<Rc<Vec<i32>>>; 34],
}

impl Record {
    fn empty() -> Self {
        Self {
            v: Box::new([0; RECORD_WORDS]),
            maps: std::array::from_fn(|_| None),
        }
    }

    fn map(&self, word: usize) -> Option<&[i32]> {
        self.maps[word - MAP].as_deref().map(Vec::as_slice)
    }

    fn covered(&self, word: usize, tile: usize) -> bool {
        self.map(word).is_some_and(|map| map[7 * tile] != 0)
    }

    pub(crate) fn script_words(&self) -> &[i32] {
        &self.v[..SCRIPT_WORDS]
    }
}

#[derive(Debug)]
pub(crate) struct Evaluator {
    /// [8] D0:84; 0 until set.
    sides: i32,
    records: Vec<Record>,
    acting: usize,
    /// [12..]: who acts next, -1 terminated.
    order: Vec<i32>,
    snapshot: Option<GridFieldManager>,
    w: i32,
    h: i32,
    candidates: Vec<Candidate>,
}

/// sub_459880: weapon key `s` (0 basic attack, 1..=16 skill s-1).
fn weapon_key(r: &[i32], s: usize) -> Option<[i32; 5]> {
    if s == 0 {
        return Some([0, r[149], r[150], r[151], 0]);
    }
    if s > 16 {
        return None;
    }
    let k = &r[22 * s + 151..];
    (k[0] != 0).then(|| [k[1], k[10], k[11], k[12], k[13]])
}

/// sub_4010B0.
fn snap(x: f64, eps: f64) -> f64 {
    let floor = x.floor();
    if 1.0 - eps <= x - floor {
        x.ceil()
    } else if eps < x - floor {
        x
    } else {
        floor
    }
}

/// sub_459830 with the evaluator's 2.0: 1.0 hit from the front, 2.0 from behind.
fn facing_mul(delta: i32) -> i32 {
    let v = snap(
        (f64::from(delta) / 11_796_480.0 * std::f64::consts::FRAC_PI_2).cos(),
        0.000_976_562_5,
    );
    ((v + (1.0 - v) * 2.0) * 65_536.0) as i32
}

/// sub_459BA0. Kinds 2 and 256 leave out[1] as it was.
fn buffs(kind: i32, atk: &[i32], out: &mut [i32; 2], def: &[i32]) {
    let on = |r: &[i32], i: usize| {
        if r[BUFF_TIME + i] > 0 {
            r[BUFF_VALUE + i]
        } else {
            0
        }
    };
    match kind {
        0 => {
            *out = [
                on(atk, 0).wrapping_sub(on(atk, 1)),
                on(def, 2).wrapping_sub(on(def, 3)),
            ]
        }
        1 => {
            *out = [
                on(atk, 4).wrapping_sub(on(atk, 5)),
                on(def, 6).wrapping_sub(on(def, 7)),
            ]
        }
        2 => out[0] = on(atk, 4).wrapping_sub(on(atk, 5)),
        256 => out[0] = on(atk, 8).wrapping_sub(on(atk, 9)),
        _ => *out = [0, 0],
    }
}

/// sub_4598F0: damage (or healing for kind 2), 0 for an unknown kind.
#[allow(clippy::too_many_arguments)]
fn damage(
    rel: i32,
    a: &[i32],
    ab: i32,
    kind: i32,
    p: &[i32],
    d: &[i32],
    db: i32,
    facing: i32,
) -> i32 {
    let mut sum: i64 = 0;
    match kind {
        0 | 1 => {
            let o = if kind == 1 { 16 } else { 0 };
            for e in 0..4 {
                let base = o + 4 * e;
                let div = d[base + 2].max(1);
                let term = a[base + 1]
                    .wrapping_add(p[e])
                    .wrapping_add(ab)
                    .wrapping_mul(a[base])
                    .wrapping_div(div)
                    .wrapping_sub(d[base + 3])
                    .wrapping_sub(db);
                sum += i64::from(term.max(0));
            }
        }
        2 => {
            for e in 0..4 {
                sum += i64::from(a[16 + 4 * e + 1].wrapping_add(p[e]).wrapping_add(ab));
            }
        }
        _ => return 0,
    }
    let product = sum
        .wrapping_mul(i64::from(facing))
        .wrapping_mul(i64::from(rel));
    (product >> 32) as i32
}

/// sub_45BB60 for one tile (7 ints): per facing, the first side in the
/// order back, sides, front (vtbl+10 with rel=1) that hits it.
fn threat_sides(out: &mut [i32; NDIRS], tile: &[i32], face: i32) -> bool {
    const ORDER: [[i32; 4]; 4] = [[1, 2, 3, 0], [0, 3, 2, 1], [3, 1, 0, 2], [2, 0, 1, 3]];
    if tile[0] == 0 {
        return false;
    }
    for (i, slot) in out.iter_mut().enumerate() {
        if face >= 0 && face != i as i32 {
            continue;
        }
        if let Some(&side) = ORDER[i]
            .iter()
            .find(|&&side| tile[side as usize + 1] as u32 >= 1)
        {
            *slot = side;
        }
    }
    true
}

/// sub_45A420: the first record standing on (x, y).
fn record_at(records: &[Record], x: i32, y: i32) -> Option<usize> {
    records
        .iter()
        .position(|r| r.v[POS_X] == x && r.v[POS_Y] == y)
}

/// sub_45A4E0: the first record with the same unit id.
fn record_of(records: &[Record], id: i32) -> i32 {
    records
        .iter()
        .position(|r| r.v[ID] != 0 && r.v[ID] == id)
        .map_or(-1, |index| index as i32)
}

impl Evaluator {
    /// sub_4583E0.
    pub(crate) fn new() -> Self {
        Self {
            sides: 0,
            records: Vec::new(),
            acting: 0,
            order: Vec::new(),
            snapshot: None,
            w: 0,
            h: 0,
            candidates: Vec::new(),
        }
    }

    /// D0:84 sub_458530.
    pub(crate) fn set_sides(&mut self, sides: i32) -> u32 {
        if sides < 1 {
            return BAD_COUNT;
        }
        self.sides = sides;
        0
    }

    /// sub_4597C0: multiplier for `atk_side` hitting `tgt_side`.
    fn relation(&self, tgt_side: i32, atk_side: i32) -> i32 {
        let n = self.sides;
        if !(0..n).contains(&tgt_side) || !(0..n).contains(&atk_side) {
            return 0;
        }
        if tgt_side == (atk_side + 1) % n {
            0x18000
        } else if tgt_side == (n + atk_side - 1) % n {
            0xAB85
        } else {
            0x10000
        }
    }

    /// D0:88 sub_458550 (n records of 525 words).
    pub(crate) fn load(&mut self, grid: &GridFieldManager, n: u32, source: &[i32]) -> u32 {
        if n.wrapping_sub(1) >= MAX_UNITS {
            return BAD_COUNT;
        }
        self.records.clear();
        self.snapshot = grid.snapshot();
        self.records = vec![Record::empty(); n as usize];
        for i in 0..n as usize {
            self.load_one(
                i as u32,
                &source[i * SCRIPT_WORDS..(i + 1) * SCRIPT_WORDS],
                true,
            );
        }
        0
    }

    /// D0:8C sub_4586B0.
    pub(crate) fn resnapshot(&mut self, grid: &GridFieldManager) -> u32 {
        self.snapshot = grid.snapshot();
        0
    }

    /// D0:8E sub_458A40.
    pub(crate) fn record(&self, index: u32) -> Result<&Record, u32> {
        self.records.get(index as usize).ok_or(BAD_INDEX)
    }

    /// D0:8D sub_4586E0.
    pub(crate) fn load_one(&mut self, index: u32, source: &[i32], with_maps: bool) -> u32 {
        let Some(record) = self.records.get_mut(index as usize) else {
            return BAD_INDEX;
        };
        record.v[..SCRIPT_WORDS].copy_from_slice(source);
        if record.v[ID] == 0 {
            record.v[POS_X] = -1;
            record.v[POS_Y] = -1;
            return 0;
        }
        let Some(snap) = self.snapshot.as_mut() else {
            return 0;
        };
        let id = record.v[ID];
        if let Ok((x, y)) = snap.unit_pos(id) {
            record.v[POS_X] = x;
            record.v[POS_Y] = y;
        }
        if let Ok(dir) = snap.unit_dir(id) {
            record.v[DIR] = dir.wrapping_sub(2);
        }
        if !with_maps {
            return 0;
        }
        // Keys for the basic attack and skills 1..15 (slot 16 is not built).
        let mut keys = vec![weapon_key(&record.v[..], 0).unwrap()];
        keys.extend((1..16).filter_map(|s| weapon_key(&record.v[..], s)));
        snap.build_threat_maps(id, record.v[JUMP], record.v[MOVE], &keys, false, true);
        record.maps = std::array::from_fn(|_| None);
        let first = snap.threat_map(id, &keys[0], false).ok();
        if first.is_some() {
            record.maps[1] = snap.threat_map(id, &keys[0], true).ok().map(Rc::new);
        }
        record.maps[0] = first.map(Rc::new);
        for s in 0..16 {
            let Some(key) = weapon_key(&record.v[..], s + 1) else {
                continue;
            };
            if let Ok(map) = snap.threat_map(id, &key, false) {
                record.maps[2 + 2 * s] = Some(Rc::new(map));
                record.maps[3 + 2 * s] = snap.threat_map(id, &key, true).ok().map(Rc::new);
            }
        }
        0
    }

    // -----------------------------------------------------------------------
    // sub_459CF0: pick each unit's target and where it heads.

    fn choose_target(&mut self, i: usize) {
        let n = self.records.len();
        let sides = self.sides;
        let Some(snap) = self.snapshot.as_mut() else {
            return;
        };
        let r = self.records[i].v.clone();
        if r[ID] == 0 {
            return;
        }
        snap.search_with(r[ID], r[JUMP], 0, (-1, -1), false);
        let mut dist = vec![0i32; n];
        let mut goal = vec![(0, 0); n];
        if r[ATK_RANGE] == 1 {
            for j in 0..n {
                let o = &self.records[j].v;
                if o[TEAM] & r[TEAM] != 0 || o[ID] == 0 {
                    dist[j] = 0;
                    continue;
                }
                let mut best = i32::MAX;
                let mut at = snap.unit_pos(o[ID]).unwrap_or((0, 0));
                if let Ok((steps, tiles)) = snap.neighbors(r[ID], at.0, at.1) {
                    for k in 0..6 {
                        if steps[k] < 0 {
                            continue;
                        }
                        if let Ok(cost) = snap.cell_cost(r[ID], tiles[k].0, tiles[k].1) {
                            if cost < best {
                                best = cost;
                                at = tiles[k];
                            }
                        }
                    }
                }
                if best == i32::MAX {
                    dist[j] = -1;
                } else {
                    dist[j] = best;
                    goal[j] = at;
                }
            }
        } else {
            let mut enemy = vec![(0, 0); n];
            for j in 0..n {
                let o = &self.records[j].v;
                if o[TEAM] & r[TEAM] != 0 || o[ID] == 0 {
                    dist[j] = 0;
                } else {
                    dist[j] = -1;
                    if let Ok(pos) = snap.unit_pos(o[ID]) {
                        enemy[j] = pos;
                    }
                }
            }
            if let Ok(tiles) = snap.reachable_sorted(r[ID]) {
                for (x, y) in tiles {
                    let cost = snap.cell_cost(r[ID], x, y).unwrap_or(0);
                    let hits = snap
                        .targets(x, y, r[149], r[150], r[151], true, true, true)
                        .unwrap_or_default();
                    for (hx, hy, _) in hits {
                        if let Some(j) = (0..n).find(|&j| dist[j] < 0 && enemy[j] == (hx, hy)) {
                            dist[j] = cost;
                            goal[j] = (x, y);
                        }
                    }
                    if !dist.iter().any(|&d| d < 0) {
                        break;
                    }
                }
            }
        }
        // Keep or switch targets, preferring the side this unit beats.
        let wrap = |v: i32| (sides != 0).then(|| v.wrapping_rem(sides));
        let me_side = r[SIDE];
        let next = wrap(me_side + 1);
        let prev = wrap(sides + me_side - 1);
        let mut best = 0;
        let mut cur_side = -1;
        let mut target = r[TARGET];
        let mut goal_xy = (r[GOAL_X], r[GOAL_Y]);
        if target >= 0 && dist.get(target as usize).is_some_and(|&d| d >= 1) {
            best = dist[target as usize];
            cur_side = self.records[target as usize].v[SIDE];
        }
        for j in 0..n {
            let d = dist[j];
            if d < 1 {
                continue;
            }
            let s = self.records[j].v[SIDE];
            let is_next = next == Some(s);
            let neutral = !is_next && prev != Some(s);
            let cur_next = next == Some(cur_side);
            let cur_prev = prev == Some(cur_side);
            let cur_neutral = !cur_next && !cur_prev;
            let take = if d < best {
                let le = 2 * d <= best;
                (is_next && (!cur_next || le))
                    || (neutral && (cur_prev || (cur_neutral && le)))
                    || best == 0
            } else if d < best.wrapping_add(r[MOVE]) {
                (is_next && !cur_next) || (neutral && cur_prev) || best == 0
            } else {
                best == 0
            };
            if take {
                best = d;
                cur_side = s;
                target = j as i32;
                goal_xy = goal[j];
            }
        }
        let record = &mut self.records[i].v;
        record[TARGET] = target;
        record[GOAL_X] = goal_xy.0;
        record[GOAL_Y] = goal_xy.1;
        if target >= 0 {
            if let Ok(points) = snap.path_points(r[ID], goal_xy.0, goal_xy.1) {
                for (x, y) in points {
                    if snap
                        .cell_cost(r[ID], x, y)
                        .is_ok_and(|cost| cost <= r[MOVE])
                    {
                        record[STEP_X] = x;
                        record[STEP_Y] = y;
                    }
                }
            }
        }
    }

    // -----------------------------------------------------------------------
    // D0:8A sub_458A70: the ranked choices for `acting`, as 7-int results
    // {x, y, type or facing, skill, tx, ty, score}.

    pub(crate) fn decide(&mut self, acting: u32, max: u32) -> Result<Vec<[i32; 7]>, u32> {
        if acting as usize >= self.records.len() {
            return Err(BAD_INDEX);
        }
        let acting = acting as usize;
        self.acting = acting;
        for i in 0..self.records.len() {
            self.choose_target(i);
        }
        // Without a grid copy every grid call in the target dereferences null.
        let Some(snap) = self.snapshot.as_mut() else {
            return Err(FAILED);
        };
        let (w, h) = snap.size().unwrap_or((0, 0));
        self.w = w;
        self.h = h;
        let mut records = self.records.clone();
        let me = records[acting].v.clone();
        let mut path = Vec::new();
        let mut partner = None;
        if me[TARGET] >= 0 {
            snap.search_with(me[ID], me[JUMP], 0, (-1, -1), false);
            path = snap
                .path_points(me[ID], me[GOAL_X], me[GOAL_Y])
                .unwrap_or_default();
            let p = &records[me[TARGET] as usize].v;
            partner = Some(p[ID]);
            snap.search_with(p[ID], p[JUMP], 0, (-1, -1), false);
        }
        snap.search_with(me[ID], me[JUMP], me[MOVE], (-1, -1), true);
        let mut tiles = snap.reachable_sorted(me[ID]).unwrap_or_default();
        tiles.push(snap.unit_pos(me[ID]).unwrap_or((0, 0)));

        let mut template: Candidate = [0; 19];
        template[C_SKILL] = -1;
        template[C_TX] = -1;
        template[C_TY] = -1;
        template[6] = MIN;
        template[C_FACE] = -1;
        for slot in &mut template[C_SCORE..] {
            *slot = MIN;
        }
        let mut candidates: Vec<Candidate> = Vec::new();
        let mut arrive = 6;
        for (x, y) in tiles {
            snap.set_position(me[ID], x, y);
            records[acting].v[POS_X] = x;
            records[acting].v[POS_Y] = y;
            let mut c = template;
            c[C_X] = x;
            c[C_Y] = y;
            c[C_TYPE] = 0;
            if let Ok(facing) = snap.cell_facing(me[ID], x, y) {
                arrive = facing;
            }
            c[C_ARRIVE] = arrive - 2;
            if let Ok(cost) = snap.cell_cost(me[ID], x, y) {
                c[C_COST] = cost;
            }
            if let Some(partner) = partner {
                c[C_PARTNER] = snap.cell_cost(partner, x, y).unwrap_or(-1);
            }
            c[C_COST_W] = if c[C_COST] != 0 {
                me[143].wrapping_add(c[C_COST].wrapping_mul(me[144]))
            } else {
                0
            };
            c[C_ON_PATH] = i32::from(path.contains(&(x, y)));
            candidates.push(c);

            let mut attack = c;
            attack[C_TYPE] = 6;
            for (hx, hy, _) in snap
                .targets(x, y, me[149], me[150], me[151], true, false, true)
                .unwrap_or_default()
            {
                if let Some(o) = record_at(&records, hx, hy) {
                    if records[o].v[TEAM] & me[TEAM] == 0 {
                        let mut a = attack;
                        a[C_TX] = hx;
                        a[C_TY] = hy;
                        candidates.push(a);
                    }
                }
            }
            attack[C_TYPE] = 7;
            for s in 0..16 {
                let k = &me[SKILL + 22 * s..SKILL + 22 * s + 22];
                if k[0] == 0 {
                    continue;
                }
                attack[C_SKILL] = s as i32;
                if k[13] < 0 {
                    candidates.push(attack);
                    continue;
                }
                let hits = snap
                    .targets(x, y, k[10], k[11], k[12], k[13] == 0, false, false)
                    .unwrap_or_default();
                for (hx, hy, _) in hits {
                    let ok = if k[13] == 0 {
                        // A unit without a record faults in the target.
                        record_at(&records, hx, hy).is_some_and(|o| {
                            let ally = records[o].v[TEAM] & me[TEAM] != 0;
                            match k[1] {
                                0 | 4 => !ally,
                                1..=3 => ally,
                                _ => true,
                            }
                        })
                    } else {
                        let area = snap
                            .targets(hx, hy, k[13], 0, 0, true, false, false)
                            .unwrap_or_default();
                        if area.is_empty() {
                            false
                        } else {
                            let (mut allies, mut enemies) = (0, 0);
                            for (ax, ay, _) in area {
                                if let Some(o) = record_at(&records, ax, ay) {
                                    if records[o].v[TEAM] & me[TEAM] != 0 {
                                        allies += 1;
                                    } else {
                                        enemies += 1;
                                    }
                                }
                            }
                            match k[1] {
                                0 | 4 => enemies > 0,
                                1..=3 => allies > 0,
                                _ => true,
                            }
                        }
                    };
                    if ok {
                        let mut a = attack;
                        a[C_TX] = hx;
                        a[C_TY] = hy;
                        candidates.push(a);
                    }
                }
            }
        }

        // Turn order after `acting`: lowest wait + wait buff first.
        let n = self.records.len();
        let mut done = vec![false; n];
        done[acting] = true;
        self.order.clear();
        for _ in 0..n {
            let mut pick = -1;
            let mut low = i32::MAX;
            for j in 0..n {
                if done[j] {
                    continue;
                }
                if records[j].v[ID] == 0 {
                    done[j] = true;
                    continue;
                }
                let key = records[j].v[WAIT].wrapping_add(records[j].v[WAIT_BUFF]);
                if key < low {
                    low = key;
                    pick = j as i32;
                }
            }
            self.order.push(pick);
            if pick < 0 {
                break;
            }
            done[pick as usize] = true;
        }

        // Score every candidate on one grid copy (the target spreads them
        // over threads, each with its own copy; nothing depends on which).
        let mut grid = snap.snapshot();
        self.candidates = candidates;
        if let Some(grid) = grid.as_mut() {
            for index in 0..self.candidates.len() {
                self.score(index, grid);
            }
        }

        let mut results: Vec<[i32; 7]> = Vec::new();
        for c in &self.candidates {
            for f in 0..6 {
                if c[C_SCORE + f] == MIN {
                    continue;
                }
                let mut r = [c[0], c[1], c[2], c[3], c[4], c[5], 0];
                let mut bonus = 0;
                if r[C_TYPE] == 0 {
                    r[C_TYPE] = f as i32;
                    if c[C_FACE] == f as i32 {
                        bonus = 2;
                    }
                    if c[C_ARRIVE] == f as i32 {
                        bonus = 1;
                    }
                }
                r[6] = c[C_SCORE + f].wrapping_add(bonus);
                results.push(r);
            }
        }
        qsort(&mut results, |a, b| b[6].wrapping_sub(a[6]));
        results.truncate(max as usize);
        Ok(results)
    }

    // -----------------------------------------------------------------------
    // sub_45A530: score one candidate.

    fn score(&mut self, index: usize, g: &mut GridFieldManager) {
        let mut c = self.candidates[index];
        let mut r = self.records.clone();
        let me = self.acting;
        let tgt = (r[me].v[TARGET] >= 0).then(|| r[me].v[TARGET] as usize);
        r[me].v[POS_X] = c[C_X];
        r[me].v[POS_Y] = c[C_Y];
        for record in &r {
            if record.v[ID] != 0 {
                g.set_position(record.v[ID], record.v[POS_X], record.v[POS_Y]);
            }
        }
        // Copy of the turn order with room to grow; -1 beyond the end.
        let mut order = self.order.clone();
        order.resize(order.len() + 2 * r.len() + 4, -1);
        // Locals the target keeps between calls when a callee fails.
        let mut buf = [0i32; 2];
        let mut delta = 0;
        let mut from = (0, 0);
        let mut side = [0i32; NDIRS];
        let mut skill_side = [[0i32; NDIRS]; 16];

        let mut score: i32 = 0;
        let mut wait = c[C_COST_W];
        let mut face = -1;
        match c[C_TYPE] {
            0 => {
                wait = wait.wrapping_add(r[me].v[147]);
                score = r[me].v[148];
            }
            6 => {
                if let Some(t) = record_at(&r, c[C_TX], c[C_TY]) {
                    let m = r[me].v.clone();
                    buffs(m[ATK_KIND], &m[..], &mut buf, &r[t].v[..]);
                    if let Ok(d) = g.unit_facing_delta(r[t].v[ID], c[C_X], c[C_Y]) {
                        delta = d;
                    }
                    let rel = self.relation(r[t].v[SIDE], m[SIDE]);
                    let mut dmg = damage(
                        rel,
                        &m[STATS..STATS + 32],
                        buf[0],
                        m[ATK_KIND],
                        &m[ATK_POWER..ATK_POWER + 4],
                        &r[t].v[STATS..STATS + 32],
                        buf[1],
                        facing_mul(delta),
                    );
                    if dmg > r[t].v[HP] {
                        dmg = r[t].v[HP];
                    }
                    r[t].v[HP] = r[t].v[HP].wrapping_sub(dmg);
                    let kill = if r[t].v[HP] <= 0 { m[161] } else { 0 };
                    score = m[159]
                        .wrapping_add(m[160].wrapping_mul(dmg))
                        .wrapping_add(kill);
                    let moved = if c[C_COST] != 0 { m[158] } else { 0 };
                    wait = wait.wrapping_add(m[157].wrapping_add(moved));
                }
                face = direction_to(c[C_TX], c[C_TY], c[C_X], c[C_Y]) - 2;
            }
            7 => {
                let k: [i32; 22] = r[me].v[SKILL + 22 * c[C_SKILL] as usize..][..22]
                    .try_into()
                    .unwrap();
                if k[13] < 0 {
                    score = r[me].v[159];
                } else {
                    let hits: Vec<(i32, i32)> = if k[13] == 0 {
                        vec![(c[C_TX], c[C_TY])]
                    } else {
                        let exclude = (c[C_TX], c[C_TY]) != (c[C_X], c[C_Y]);
                        g.targets(c[C_TX], c[C_TY], k[13], 0, 0, true, false, exclude)
                            .unwrap_or_default()
                            .into_iter()
                            .map(|(x, y, _)| (x, y))
                            .collect()
                    };
                    for (hx, hy) in hits {
                        let Some(t) = record_at(&r, hx, hy) else {
                            continue;
                        };
                        buffs(k[3], &r[me].v[..], &mut buf, &r[t].v[..]);
                        let mut fm = 0x10000;
                        if k[1] == 0 {
                            let source = if (hx, hy) == (c[C_TX], c[C_TY]) {
                                (c[C_X], c[C_Y])
                            } else {
                                (c[C_TX], c[C_TY])
                            };
                            if let Ok(d) = g.unit_facing_delta(r[t].v[ID], source.0, source.1) {
                                delta = d;
                            }
                            fm = facing_mul(delta);
                        }
                        let rel = self.relation(r[t].v[SIDE], r[me].v[SIDE]);
                        let mut dmg = damage(
                            rel,
                            &r[me].v[STATS..STATS + 32],
                            buf[0],
                            k[3],
                            &k[4..8],
                            &r[t].v[STATS..STATS + 32],
                            buf[1],
                            fm,
                        );
                        let weights = if r[t].v[TEAM] & r[me].v[TEAM] != 0 {
                            [k[16], k[17], k[18]]
                        } else {
                            [k[19], k[20], k[21]]
                        };
                        let effect = k[2];
                        let mut requeue = false;
                        let tv = &mut r[t].v;
                        match k[1] {
                            0 => {
                                if dmg > tv[HP] {
                                    dmg = tv[HP];
                                }
                                tv[HP] = tv[HP].wrapping_sub(dmg);
                                let kill = if tv[HP] <= 0 { weights[2] } else { 0 };
                                score = score.wrapping_add(
                                    weights[0]
                                        .wrapping_add(weights[1].wrapping_mul(dmg))
                                        .wrapping_add(kill),
                                );
                            }
                            1 => {
                                let room = tv[HP_MAX].wrapping_sub(tv[HP]);
                                if dmg > room {
                                    dmg = room;
                                }
                                tv[HP] = tv[HP].wrapping_add(dmg);
                                score = score.wrapping_add(
                                    weights[0].wrapping_add(weights[1].wrapping_mul(dmg)),
                                );
                            }
                            2 => {
                                let slot = (BUFF_TIME as i32 + effect) as usize;
                                if dmg > tv[slot] {
                                    dmg = tv[slot];
                                }
                                tv[slot] = tv[slot].wrapping_sub(dmg);
                                score = score.wrapping_add(
                                    weights[0].wrapping_add(weights[1].wrapping_mul(dmg)),
                                );
                                requeue = matches!(effect, 8 | 9 | 31);
                            }
                            3 | 4 => {
                                tv[(BUFF_TIME as i32 + effect) as usize] = k[8];
                                tv[(BUFF_VALUE as i32 + effect) as usize] = dmg;
                                score = score.wrapping_add(
                                    weights[0].wrapping_add(weights[1].wrapping_mul(dmg)),
                                );
                                requeue = matches!(effect, 8 | 9 | 31);
                            }
                            _ => {}
                        }
                        if requeue && r[t].v[ID] != r[me].v[ID] {
                            self.requeue(&r, t, &mut order, &mut buf);
                        }
                    }
                }
                let moved = if c[C_COST] != 0 { k[15] } else { 0 };
                wait = wait.wrapping_add(k[14].wrapping_add(moved));
                face = direction_to(c[C_TX], c[C_TY], c[C_X], c[C_Y]) - 2;
            }
            _ => {}
        }
        let m = r[me].v.clone();
        score = score.wrapping_add(m[146].wrapping_mul(c[C_COST]));
        if c[C_PARTNER] >= 1 {
            score = score.wrapping_add(c[C_PARTNER].wrapping_mul(m[172]));
        } else if c[C_PARTNER] < 0 {
            score = score.wrapping_add(self.h.wrapping_add(self.w).wrapping_mul(m[172]));
        }
        let mut walk_attack = false;
        if c[C_ON_PATH] != 0 {
            score = score
                .wrapping_add(m[5].wrapping_mul(c[C_COST]).wrapping_mul(c[C_COST]))
                .wrapping_add(m[4]);
            walk_attack = c[C_TYPE] == 0 && m[ATK_RANGE] == 1;
        }
        r[me].v[WAIT] = wait;
        buffs(256, &r[me].v[..], &mut buf, &[]);
        wait = wait.wrapping_add(r[me].v[WAIT_BUFF].wrapping_sub(buf[0]));

        let tile = c[C_X].wrapping_add(self.w.wrapping_mul(c[C_Y])) as usize;
        let mut sum = [0i32; 6];
        let mut hp = [r[me].v[HP]; NDIRS];
        let mut skill_hit = [0i32; 16];
        let mut i = 0;
        while order[i] >= 0 {
            let ui = order[i] as usize;
            if r[ui].v[HP] <= 0 {
                i += 1;
                continue;
            }
            let u_wait = r[ui].v[WAIT];
            if u_wait > wait || (u_wait == wait && (self.acting as i32) < order[i]) {
                break;
            }
            if r[ui].v[TEAM] & r[me].v[TEAM] != 0 {
                if let Some(t) = tgt {
                    let tt =
                        r[t].v[POS_X].wrapping_add(self.w.wrapping_mul(r[t].v[POS_Y])) as usize;
                    if r[ui].covered(MAP, tt) {
                        score = score.wrapping_add(self.ally_hit(
                            &mut r, ui, t, tt, g, &mut buf, &mut side, &mut from, &mut delta,
                        ));
                    }
                }
                i += 1;
                continue;
            }
            let sel = usize::from(r[ui].v[ADVANCED] != 0);
            let uid = r[ui].v[ID];
            let mut threat = false;
            if r[ui].covered(MAP + sel, tile) {
                let key = weapon_key(&r[ui].v[..], 0).unwrap();
                g.build_threat_maps(uid, r[ui].v[JUMP], r[ui].v[MOVE], &[key], true, false);
                if let Ok(map) = g.threat_map(uid, &key, false) {
                    threat = threat_sides(&mut side, &map[7 * tile..], face);
                }
            }
            let mut any = threat;
            for s in 0..16 {
                skill_hit[s] = 0;
                if r[ui].v[SKILL + 22 * s] == 0 || !r[ui].covered(MAP + 2 + sel + 2 * s, tile) {
                    continue;
                }
                let key = weapon_key(&r[ui].v[..], s + 1).unwrap_or_default();
                g.build_threat_maps(uid, r[ui].v[JUMP], r[ui].v[MOVE], &[key], true, false);
                let Ok(map) = g.threat_map(uid, &key, false) else {
                    continue;
                };
                match r[ui].v[SKILL + 22 * s + 1] {
                    0 => {
                        skill_hit[s] =
                            i32::from(threat_sides(&mut skill_side[s], &map[7 * tile..], face))
                    }
                    4 => skill_hit[s] = map[7 * tile],
                    _ => {}
                }
                any = any || skill_hit[s] != 0;
            }
            let weight = if r[ui].v[ADVANCED] != 0 {
                if r[me].v[171] != 0 { r[me].v[171] } else { 50 }
            } else {
                100
            };
            for f in 0..NDIRS {
                if face >= 0 && face != f as i32 {
                    continue;
                }
                g.set_dir(r[me].v[ID], f as i32 + 2);
                if threat {
                    let alive = hp[f] >= 1;
                    let dmg = self.counter_hit(
                        &r,
                        ui,
                        me,
                        r[ui].v[ATK_KIND],
                        &r[ui].v[ATK_POWER..ATK_POWER + 4].to_vec(),
                        side[f],
                        g,
                        &mut buf,
                        &mut from,
                        &mut delta,
                        true,
                    );
                    hp[f] = hp[f].wrapping_sub(dmg);
                    let die = if hp[f] <= 0 && alive { r[me].v[164] } else { 0 };
                    let v = r[me].v[163]
                        .wrapping_mul(dmg)
                        .wrapping_add(r[me].v[162])
                        .wrapping_add(die);
                    sum[f] = sum[f].wrapping_add(v.wrapping_mul(weight) / 100);
                }
                for s in 0..16 {
                    if skill_hit[s] == 0 {
                        continue;
                    }
                    let k: [i32; 22] = r[ui].v[SKILL + 22 * s..][..22].try_into().unwrap();
                    match k[1] {
                        0 => {
                            let alive = hp[f] >= 1;
                            let dmg = self.counter_hit(
                                &r,
                                ui,
                                me,
                                k[3],
                                &k[4..8].to_vec(),
                                skill_side[s][f],
                                g,
                                &mut buf,
                                &mut from,
                                &mut delta,
                                true,
                            );
                            hp[f] = hp[f].wrapping_sub(dmg);
                            let die = if hp[f] <= 0 && alive { r[me].v[167] } else { 0 };
                            let v = r[me].v[166]
                                .wrapping_mul(dmg)
                                .wrapping_add(r[me].v[165])
                                .wrapping_add(die);
                            sum[f] = sum[f].wrapping_add(v.wrapping_mul(weight) / 100);
                        }
                        4 => {
                            let dmg = self.counter_hit(
                                &r,
                                ui,
                                me,
                                k[3],
                                &k[4..8].to_vec(),
                                0,
                                g,
                                &mut buf,
                                &mut from,
                                &mut delta,
                                false,
                            );
                            let effect = k[2];
                            r[me].v[(BUFF_TIME as i32 + effect) as usize] = k[8];
                            r[me].v[(BUFF_VALUE as i32 + effect) as usize] = dmg;
                            let v = r[me].v[169].wrapping_mul(dmg).wrapping_add(r[me].v[168]);
                            sum[f] = sum[f].wrapping_add(v.wrapping_mul(weight) / 100);
                        }
                        _ => {}
                    }
                }
            }
            if !any && r[ui].v[ADVANCED] == 0 {
                // Out of reach: it walks toward its target and acts again.
                let u = &mut r[ui].v;
                let walk = u[143].wrapping_add(u[144].wrapping_mul(u[MOVE].wrapping_mul(3) >> 2));
                u[WAIT] = u[WAIT].wrapping_add(walk);
                let new_wait = u[WAIT];
                let mut j = i + 1;
                let mut insert = 0;
                while order[j] >= 0 {
                    if insert == 0 && new_wait < r[order[j] as usize].v[WAIT] {
                        insert = j;
                    }
                    j += 1;
                }
                if insert == 0 {
                    insert = j;
                }
                let mut k = j;
                while k >= insert {
                    order[k + 1] = order[k];
                    k -= 1;
                }
                order[insert] = order[i];
                let u = &mut r[ui].v;
                u[ADVANCED] = 1;
                if u[TARGET] >= 0 {
                    u[POS_X] = u[STEP_X];
                    u[POS_Y] = u[STEP_Y];
                }
            }
            if c[C_TYPE] == 0 && any && c[C_FACE] < 0 {
                c[C_FACE] = self.pick_face(&r[ui], sel, tile, &skill_hit, &c, g);
            }
            i += 1;
        }

        if walk_attack {
            if let Some(t) = tgt {
                score =
                    score.wrapping_add(self.walk_attack(&mut r, me, t, g, &mut buf, &mut delta));
            }
        }
        for f in 0..NDIRS {
            if face < 0 || face == f as i32 {
                c[C_SCORE + f] = score.wrapping_add(sum[f]);
            }
        }
        self.candidates[index] = c;
    }

    /// One counter hit on `me` by unit `ui` with formula `kind`/`power`,
    /// standing on `me`'s side `side` (facing-weighted when `facing`).
    #[allow(clippy::too_many_arguments)]
    fn counter_hit(
        &self,
        r: &[Record],
        ui: usize,
        me: usize,
        kind: i32,
        power: &[i32],
        side: i32,
        g: &GridFieldManager,
        buf: &mut [i32; 2],
        from: &mut (i32, i32),
        delta: &mut i32,
        facing: bool,
    ) -> i32 {
        buffs(kind, &r[ui].v[..], buf, &r[me].v[..]);
        let mut fm = 0x10000;
        if facing {
            if let Ok(tile) = g.step_from_unit(r[me].v[ID], side, true) {
                *from = tile;
            }
            if let Ok(d) = g.unit_facing_delta(r[me].v[ID], from.0, from.1) {
                *delta = d;
            }
            fm = facing_mul(*delta);
        }
        let rel = self.relation(r[me].v[SIDE], r[ui].v[SIDE]);
        damage(
            rel,
            &r[ui].v[STATS..STATS + 32],
            buf[0],
            kind,
            power,
            &r[me].v[STATS..STATS + 32],
            buf[1],
            fm,
        )
    }

    /// An ally's basic attack on our target before our next turn (B6FF).
    #[allow(clippy::too_many_arguments)]
    fn ally_hit(
        &self,
        r: &mut [Record],
        ui: usize,
        t: usize,
        tile: usize,
        g: &mut GridFieldManager,
        buf: &mut [i32; 2],
        side: &mut [i32; NDIRS],
        from: &mut (i32, i32),
        delta: &mut i32,
    ) -> i32 {
        let uid = r[ui].v[ID];
        let key = weapon_key(&r[ui].v[..], 0).unwrap();
        g.build_threat_maps(uid, r[ui].v[JUMP], r[ui].v[MOVE], &[key], true, false);
        let Ok(map) = g.threat_map(uid, &key, false) else {
            return 0;
        };
        let t_face = r[t].v[DIR];
        if !threat_sides(side, &map[7 * tile..], t_face) {
            return 0;
        }
        // The target's facing indexes a 4-entry local; outside it the
        // target reads stack garbage, the port skips the hit.
        let Some(&attack_side) = usize::try_from(t_face).ok().and_then(|f| side.get(f)) else {
            return 0;
        };
        let alive = r[t].v[HP] >= 1;
        buffs(r[ui].v[ATK_KIND], &r[ui].v[..], buf, &r[t].v[..]);
        if let Ok(tile) = g.step_from_unit(r[t].v[ID], attack_side, true) {
            *from = tile;
        }
        if let Ok(d) = g.unit_facing_delta(r[t].v[ID], from.0, from.1) {
            *delta = d;
        }
        let rel = self.relation(r[t].v[SIDE], r[ui].v[SIDE]);
        let mut dmg = damage(
            rel,
            &r[ui].v[STATS..STATS + 32],
            buf[0],
            r[ui].v[ATK_KIND],
            &r[ui].v[ATK_POWER..ATK_POWER + 4],
            &r[t].v[STATS..STATS + 32],
            buf[1],
            facing_mul(*delta),
        );
        if dmg > r[t].v[HP] {
            dmg = if alive { r[t].v[HP] } else { 0 };
        }
        r[t].v[HP] = r[t].v[HP].wrapping_sub(dmg);
        let me = self.acting;
        let kill = if r[t].v[HP] <= 0 && alive {
            r[me].v[161]
        } else {
            0
        };
        r[me].v[160]
            .wrapping_mul(dmg)
            .wrapping_add(r[me].v[159])
            .wrapping_add(kill)
    }

    /// A melee unit walking its path: the best hit from a tile next to the
    /// target, weighted 67/100 (B8FD). The target's HP drops across tries.
    fn walk_attack(
        &self,
        r: &mut [Record],
        me: usize,
        t: usize,
        g: &mut GridFieldManager,
        buf: &mut [i32; 2],
        delta: &mut i32,
    ) -> i32 {
        let m = r[me].v.clone();
        g.search_with(m[ID], m[JUMP], m[MOVE], (-1, -1), true);
        let Ok((steps, tiles)) = g.neighbors(m[ID], r[t].v[POS_X], r[t].v[POS_Y]) else {
            return 0;
        };
        let mut best = MIN;
        for k in 0..NDIRS {
            if steps[k] < 0 {
                continue;
            }
            let alive = r[t].v[HP] >= 1;
            buffs(m[ATK_KIND], &m[..], buf, &r[t].v[..]);
            if let Ok(d) = g.unit_facing_delta(r[t].v[ID], tiles[k].0, tiles[k].1) {
                *delta = d;
            }
            let rel = self.relation(r[t].v[SIDE], m[SIDE]);
            let mut dmg = damage(
                rel,
                &m[STATS..STATS + 32],
                buf[0],
                m[ATK_KIND],
                &m[ATK_POWER..ATK_POWER + 4],
                &r[t].v[STATS..STATS + 32],
                buf[1],
                facing_mul(*delta),
            );
            if dmg > r[t].v[HP] {
                dmg = if alive { r[t].v[HP] } else { 0 };
            }
            r[t].v[HP] = r[t].v[HP].wrapping_sub(dmg);
            let kill = if r[t].v[HP] <= 0 && alive { m[161] } else { 0 };
            let v = m[160]
                .wrapping_mul(dmg)
                .wrapping_add(m[159])
                .wrapping_add(kill);
            if best < v {
                best = v;
            }
        }
        if best == MIN {
            0
        } else {
            best.wrapping_mul(67) / 100
        }
    }

    /// A plain move's facing: the side least exposed (B4CB). Per facing,
    /// the hits from that side on this tile in the weapon map and in every
    /// damage skill that reaches it; a unique maximum wins, else the side
    /// toward the enemy if it is one of the maxima, else the last maximum.
    fn pick_face(
        &self,
        u: &Record,
        sel: usize,
        tile: usize,
        skill_hit: &[i32; 16],
        c: &Candidate,
        _g: &GridFieldManager,
    ) -> i32 {
        let mut hits = [0u32; 6];
        for (f, total) in hits.iter_mut().enumerate().take(NDIRS) {
            if let Some(map) = u.map(MAP + sel) {
                *total = total.wrapping_add(map[7 * tile + 1 + f] as u32);
            }
            for s in 0..16 {
                if skill_hit[s] != 0 && u.v[SKILL + 22 * s + 1] == 0 {
                    if let Some(map) = u.map(MAP + 2 + sel + 2 * s) {
                        *total = total.wrapping_add(map[7 * tile + 1 + f] as u32);
                    }
                }
            }
        }
        let most = hits[..NDIRS].iter().copied().max().unwrap_or(0);
        let maxima: Vec<usize> = (0..NDIRS).filter(|&f| hits[f] == most).collect();
        let last = *maxima.last().unwrap();
        if maxima.len() == 1 {
            return last as i32;
        }
        let toward = direction_to(u.v[POS_X], u.v[POS_Y], c[C_X], c[C_Y]) - 2;
        if hits[toward as usize] == most {
            toward
        } else {
            last as i32
        }
    }

    /// A speed change on record `t` moves it in `order` (sub_45A530 AAAA):
    /// remove it and reinsert before the first entry that now waits longer.
    fn requeue(&self, r: &[Record], t: usize, order: &mut [i32], buf: &mut [i32; 2]) {
        buffs(256, &r[t].v[..], buf, &[]);
        let raw = r[t].v[WAIT_BUFF]
            .wrapping_sub(buf[0])
            .wrapping_add(r[t].v[WAIT]);
        let size = if raw < 0 { 0 } else { raw };
        if size == r[t].v[WAIT] {
            return;
        }
        let ti = record_of(r, r[t].v[ID]);
        if order[0] < 0 {
            return;
        }
        let mut first: isize = -1;
        let mut last: isize = -1;
        let mut state: isize = 0;
        let mut i = 0usize;
        while order[i] >= 0 {
            let entry = order[i];
            if entry == ti {
                if first >= 0 {
                    last = i as isize - 1;
                    break;
                }
                state = -1;
                first = i as isize + 1;
            } else if state <= 0 && size < r[entry as usize].v[WAIT] {
                if first >= 0 {
                    last = i as isize - 1;
                    break;
                }
                state = 1;
                first = i as isize;
            }
            i += 1;
        }
        if first == last {
            return;
        }
        if last < 0 {
            last = i as isize - 1;
        }
        let mut tmp: Vec<i32> = order[..=i].to_vec();
        let pos = if state == 1 { first } else { last };
        tmp[pos as usize] = ti;
        let count = (last - first + 1).max(0) as usize;
        let dst = (first + state) as usize;
        tmp[dst..dst + count].copy_from_slice(&order[first as usize..first as usize + count]);
        order[..=i].copy_from_slice(&tmp);
    }
}

#[cfg(test)]
mod tests {
    use super::*;
    use crate::logic::grid::Tile;

    #[test]
    fn damage_follows_the_formula() {
        let mut a = [0i32; 32];
        let mut d = [0i32; 32];
        // element 0: (add 10 + power 5 + buff 0) * mul 4 / def 3 - 2 = 18
        a[0] = 4;
        a[1] = 10;
        d[2] = 3;
        d[3] = 2;
        assert_eq!(damage(0x10000, &a, 0, 0, &[5, 0, 0, 0], &d, 0, 0x10000), 18);
        // Facing and relation multiply in 16.16.
        assert_eq!(damage(0x18000, &a, 0, 0, &[5, 0, 0, 0], &d, 0, 0x20000), 54);
        // Negative terms count as zero; unknown kinds deal nothing.
        d[3] = 100;
        assert_eq!(damage(0x10000, &a, 0, 0, &[5, 0, 0, 0], &d, 0, 0x10000), 0);
        assert_eq!(damage(0x10000, &a, 0, 7, &[5, 0, 0, 0], &d, 0, 0x10000), 0);
        // Healing ignores defence: sum of (add + power + buff) over kind-1 slots.
        a[17] = 20;
        assert_eq!(
            damage(0x10000, &a, 1, 2, &[5, 0, 0, 0], &d, 0, 0x10000),
            26 + 3
        );
    }

    #[test]
    fn facing_runs_from_one_to_two() {
        assert_eq!(facing_mul(0), 0x10000);
        assert_eq!(facing_mul(0xB4_0000), 0x20000);
        let side = facing_mul(0x5A_0000);
        assert!(side > 0x10000 && side < 0x20000);
    }

    #[test]
    fn threat_sides_prefer_the_back() {
        let mut out = [9; NDIRS];
        // Hits from directions 2 (slot 1) and 5 (slot 4).
        let tile = [1, 1, 0, 0, 1, 0, 0];
        assert!(threat_sides(&mut out, &tile, -1));
        // Facing up (0): back is down (1) - none, then left (2) - none, then right (3).
        assert_eq!(out, [3, 0, 3, 0]);
        assert!(!threat_sides(&mut out, &[0; 7], -1));
    }

    fn unit_record(id: i32, team: i32, side: i32) -> Vec<i32> {
        let mut r = vec![0; SCRIPT_WORDS];
        r[ID] = id;
        r[TEAM] = team;
        r[SIDE] = side;
        r[TARGET] = -1;
        r[HP] = 100;
        r[HP_MAX] = 100;
        r[JUMP] = 1;
        r[MOVE] = 3;
        r[ATK_RANGE] = 1;
        // Weapon: element 0, mul 1, add 10; defence divisor 1.
        r[STATS] = 1;
        r[STATS + 1] = 10;
        r[STATS + 2] = 1;
        r[159] = 5; // per hit
        r[160] = 1; // per damage
        r[161] = 1000; // kill
        r
    }

    #[test]
    fn a_unit_next_to_a_weak_enemy_attacks_it() {
        let mut grid = GridFieldManager::new(1);
        grid.set_map(5, 1, vec![Tile::default(); 5]);
        let me = grid.add_unit().unwrap();
        let foe = grid.add_unit().unwrap();
        grid.set_position(me, 0, 0);
        grid.set_position(foe, 3, 0);
        grid.set_dir(me, 5);
        grid.set_dir(foe, 4);
        let mut source = unit_record(me, 1, 0);
        let mut enemy = unit_record(foe, 2, 1);
        enemy[HP] = 5;
        source.extend(enemy);
        let mut evaluator = Evaluator::new();
        assert_eq!(evaluator.set_sides(2), 0);
        assert_eq!(evaluator.load(&grid, 2, &source), 0);
        // The enemy became the target, approached from (2,0).
        let results = evaluator.decide(0, 8).unwrap();
        assert_eq!(evaluator.records[0].v[TARGET], 1);
        assert_eq!(
            (
                evaluator.records[0].v[GOAL_X],
                evaluator.records[0].v[GOAL_Y]
            ),
            (2, 0)
        );
        let best = results[0];
        assert_eq!(
            (best[0], best[1], best[2], best[4], best[5]),
            (2, 0, 6, 3, 0)
        );
        // 10 damage from behind? No: the enemy faces us (dir 4), so 1.0x,
        // capped at its 5 HP: 5 + 1*5 + 1000 kill.
        assert_eq!(best[6], 5 + 5 + 1000);
        assert!(results.windows(2).all(|w| w[0][6] >= w[1][6]));
    }

    #[test]
    fn an_enemy_out_of_reach_is_approached_along_the_path() {
        let mut grid = GridFieldManager::new(1);
        grid.set_map(8, 1, vec![Tile::default(); 8]);
        let me = grid.add_unit().unwrap();
        let foe = grid.add_unit().unwrap();
        grid.set_position(me, 0, 0);
        grid.set_position(foe, 7, 0);
        let mut source = unit_record(me, 1, 0);
        source[4] = 100; // on the path toward the target
        source[5] = 1; // ... plus cost squared
        source.extend(unit_record(foe, 2, 1));
        let mut evaluator = Evaluator::new();
        evaluator.set_sides(2);
        evaluator.load(&grid, 2, &source);
        let results = evaluator.decide(0, 4).unwrap();
        let r = &evaluator.records[0].v;
        assert_eq!((r[TARGET], r[GOAL_X], r[STEP_X]), (1, 6, 3));
        // Walk the full 3 tiles (100 + 3²), facing the way it walked (+1),
        // plus the melee estimate for the next turn: from (6,0) a 10-point
        // hit at 1.5x (side 1 follows side 0) is 5 + 15, weighted 67%.
        assert_eq!(results[0], [3, 0, 3, -1, -1, -1, 110 + 13]);
        assert_eq!(results.len(), 4);
    }

    #[test]
    fn counterattacks_from_behind_hurt_twice_as_much() {
        let mut grid = GridFieldManager::new(1);
        grid.set_map(3, 1, vec![Tile::default(); 3]);
        let me = grid.add_unit().unwrap();
        let foe = grid.add_unit().unwrap();
        grid.set_position(me, 0, 0);
        grid.set_position(foe, 2, 0);
        let mut source = unit_record(me, 1, 0);
        source[MOVE] = 1;
        source[147] = 10; // waiting after a move
        source[157] = 10; // waiting after an attack
        source[163] = -1; // damage taken counts against
        let mut enemy = unit_record(foe, 2, 1);
        enemy[MOVE] = 1;
        source.extend(enemy);
        let mut evaluator = Evaluator::new();
        evaluator.set_sides(2);
        evaluator.load(&grid, 2, &source);
        let results = evaluator.decide(0, 64).unwrap();
        // Attack from (1,0): 5 + 15 (1.5x, side 1 follows side 0), then the
        // enemy strikes back from the front: 10 at 1.5x.
        assert_eq!(results[0], [1, 0, 6, -1, 2, 0, 20 - 15]);
        let moved = |f: i32| results.iter().find(|r| r[0] == 1 && r[2] == f).unwrap()[6];
        // Moving to (1,0), minus the counter: 15 facing the enemy (+1 for
        // facing the way it walked), 30 with the back turned. The next-turn
        // melee estimate finds nothing: the unit's own tile counts as
        // occupied in its own search, so it is not a tile to strike from.
        assert_eq!(moved(3), -15 + 1);
        assert_eq!(moved(2), -30);
    }
}

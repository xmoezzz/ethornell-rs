//! `DCTELgclSpcMngr` (0xD0:40..7A): round bodies in 3D, two kinds of
//! parent/child links, overlap and sweep tests, and a planar path search
//! between two bodies. Port of `src/engine/logic_space.c`. Maths is single
//! precision in the target's operation order; the target's `rsqrtss` /
//! `rcpss` / `rcpps` estimates (CPU-dependent) are replaced by exact values
//! before the same Newton step.

use std::collections::VecDeque;

const MAX_BODIES: u32 = 0x1000;
const FIRST_CAPACITY: u32 = 64;

pub(crate) const COLLISION: u32 = 0xA000_0004;
pub(crate) const BAD_BODY: u32 = 0xA000_0001;
pub(crate) const BAD_OTHER: u32 = 0xA000_0002;
pub(crate) const BAD_MASK: u32 = 0xA000_0003;
pub(crate) const BAD_KIND: u32 = 0xA000_0005;
pub(crate) const NO_PATH: u32 = 0xA000_0006;

type Vec4 = [f32; 4];

#[derive(Debug, Clone, Default)]
struct Body {
    used: bool,
    radius: f32,
    speed: f32,
    probe: f32,
    mask: i32,
    priority: i32,
    parent: [i32; 2],
    /// Slot lists; -1 is a free slot.
    children: [Vec<i32>; 2],
    pos: Vec4,
    dir: Vec4,
    vel: Vec4,
}

/// One result of D0:78/79/7A: {index, -, -, -, x, y, z, length} in 16.16.
pub(crate) type Relation = (i32, [i32; 4]);

fn sub(a: Vec4, b: Vec4) -> Vec4 {
    [a[0] - b[0], a[1] - b[1], a[2] - b[2], a[3] - b[3]]
}

fn add(a: Vec4, b: Vec4) -> Vec4 {
    [a[0] + b[0], a[1] + b[1], a[2] + b[2], a[3] + b[3]]
}

fn scale(a: Vec4, s: f32) -> Vec4 {
    [a[0] * s, a[1] * s, a[2] * s, a[3] * s]
}

/// x² + (y² + z²).
fn len2(v: Vec4) -> f32 {
    v[0] * v[0] + (v[1] * v[1] + v[2] * v[2])
}

/// rsqrtss + one Newton step.
fn rsqrt_newton(s: f32) -> f32 {
    let r = (1.0 / f64::from(s).sqrt()) as f32;
    r * 1.5 + s * r * r * r * -0.5
}

/// rcpss / rcpps + one Newton step.
fn rcp_newton(s: f32) -> f32 {
    let r = 1.0 / s;
    (r + r) - s * (r * r)
}

/// sub_455860: |v| as s * rsqrt(s).
fn length(v: Vec4) -> f32 {
    let s = len2(v);
    s * rsqrt_newton(s)
}

/// sub_4558C0: (unit vector, length), zero for a zero vector.
fn norm_len(v: Vec4) -> Vec4 {
    let s = len2(v);
    if !(s > 0.0) {
        return [0.0; 4];
    }
    let r = rsqrt_newton(s);
    [v[0] * r, v[1] * r, v[2] * r, s * r]
}

/// sub_4010B0(x, 1/1024) on a float, kept as float.
fn snap(x: f32) -> f32 {
    let x = f64::from(x);
    let eps = 0.000_976_562_5;
    let floor = x.floor();
    (if 1.0 - eps <= x - floor {
        x.ceil()
    } else if eps < x - floor {
        x
    } else {
        floor
    }) as f32
}

/// sub_455940 per lane.
fn to_fixed(v: Vec4) -> [i32; 4] {
    v.map(super::super::extended_opcodes::float_to_fixed)
}

fn mask_or_all(mask: i32) -> u8 {
    match mask as u8 {
        0 => 0xFF,
        m => m,
    }
}

#[derive(Debug, Clone)]
pub(crate) struct SpaceManager {
    bodies: Vec<Body>,
}

/// Grid cell of the path search (32 bytes).
#[derive(Debug, Clone, Copy, Default)]
struct Cell {
    dir: i32,
    cost: f32,
    end: Vec4,
}

#[derive(Debug, Clone, Copy)]
struct Node {
    dir: i32,
    cost: f32,
    start: Vec4,
    end: Vec4,
    x: i32,
    y: i32,
}

/// The 16 moves: the perimeter of a 5x5 square from (2,0) anticlockwise.
const MOVES: [(i32, i32); 16] = [
    (2, 0),
    (2, 1),
    (2, 2),
    (1, 2),
    (0, 2),
    (-1, 2),
    (-2, 2),
    (-2, 1),
    (-2, 0),
    (-2, -1),
    (-2, -2),
    (-1, -2),
    (0, -2),
    (1, -2),
    (2, -2),
    (2, -1),
];

struct PathSearch<'a> {
    space: &'a SpaceManager,
    n: i32,
    cells: Vec<Cell>,
    goals: [(i32, i32); 16],
    moves: [Vec4; 16],
    from: usize,
    from_pos: Vec4,
    range: f32,
    step: f32,
    mask: i32,
    sweep: i32,
    resolution: i32,
    queue: VecDeque<Node>,
}

impl SpaceManager {
    /// sub_4559B0.
    pub(crate) fn new() -> Self {
        Self {
            bodies: vec![Body::default(); FIRST_CAPACITY as usize],
        }
    }

    fn body(&self, index: u32) -> Result<&Body, u32> {
        self.bodies
            .get(index as usize)
            .filter(|body| body.used)
            .ok_or(BAD_BODY)
    }

    fn body_mut(&mut self, index: u32) -> Result<&mut Body, u32> {
        self.bodies
            .get_mut(index as usize)
            .filter(|body| body.used)
            .ok_or(BAD_BODY)
    }

    /// D0:60 sub_455A40.
    #[allow(clippy::too_many_arguments)]
    pub(crate) fn add(
        &mut self,
        index: u32,
        pos: [f32; 3],
        dir: [f32; 3],
        radius: f32,
        speed: f32,
        probe: f32,
        mask: i32,
        priority: i32,
    ) -> u32 {
        if index >= MAX_BODIES {
            return BAD_BODY;
        }
        let mut capacity = self.bodies.len() as u32;
        while index >= capacity {
            capacity *= 2;
        }
        self.bodies.resize(capacity as usize, Body::default());
        if radius < 1.0 {
            return BAD_OTHER;
        }
        if mask as u8 == 0 {
            return BAD_MASK;
        }
        self.bodies[index as usize] = Body {
            used: true,
            radius,
            speed,
            probe,
            mask,
            priority,
            parent: [-1, -1],
            children: [Vec::new(), Vec::new()],
            pos: [snap(pos[0]), snap(pos[1]), snap(pos[2]), 0.0],
            dir: [0.0; 4],
            vel: [0.0; 4],
        };
        self.set_dir(index, dir);
        0
    }

    /// D0:61 sub_455C30.
    pub(crate) fn remove(&mut self, index: u32) -> u32 {
        if self.body(index).is_err() {
            return BAD_BODY;
        }
        for kind in 0..2 {
            self.link(index, kind, -1);
            let children = self.bodies[index as usize].children[kind as usize].clone();
            for child in children {
                if child != -1 {
                    self.link(child as u32, kind, -1);
                }
            }
        }
        self.bodies[index as usize] = Body::default();
        0
    }

    /// D0:41 sub_4573E0.
    pub(crate) fn clear(&mut self) {
        for index in 0..self.bodies.len() as u32 {
            self.remove(index);
        }
    }

    /// D0:64 sub_455CD0.
    pub(crate) fn set_pos(&mut self, index: u32, pos: [f32; 3]) -> u32 {
        match self.body_mut(index) {
            Ok(body) => {
                body.pos = [snap(pos[0]), snap(pos[1]), snap(pos[2]), 0.0];
                0
            }
            Err(status) => status,
        }
    }

    /// D0:65 sub_455DC0.
    pub(crate) fn pos(&self, index: u32) -> Result<[i32; 4], u32> {
        self.body(index).map(|body| to_fixed(body.pos))
    }

    /// D0:66 sub_455DF0.
    pub(crate) fn set_dir(&mut self, index: u32, dir: [f32; 3]) -> u32 {
        let Ok(body) = self.body_mut(index) else {
            return BAD_BODY;
        };
        let v = [snap(dir[0]), snap(dir[1]), snap(dir[2]), 0.0];
        let s = len2(v);
        if !(s > 0.0) {
            return BAD_OTHER;
        }
        let unit = scale(v, rsqrt_newton(s));
        body.vel = scale(unit, body.speed);
        body.dir = unit;
        0
    }

    /// D0:67 sub_455F60.
    pub(crate) fn dir(&self, index: u32) -> Result<[i32; 4], u32> {
        self.body(index).map(|body| to_fixed(body.dir))
    }

    /// D0:68 sub_455F90: link `child` under `parent` for `kind`, or detach
    /// with parent -1.
    pub(crate) fn link(&mut self, child: u32, kind: u32, parent: i32) -> u32 {
        let capacity = self.bodies.len() as u32;
        if self.body(child).is_err() {
            return BAD_BODY;
        }
        if kind >= 2 {
            return BAD_KIND;
        }
        let k = kind as usize;
        if (parent as u32) < capacity && parent as u32 != child {
            if !self.bodies[parent as usize].used {
                return BAD_OTHER;
            }
            self.link(child, kind, -1);
            let list = &mut self.bodies[parent as usize].children[k];
            let slot = match list.iter().position(|&entry| entry == -1) {
                Some(slot) => slot,
                None => {
                    let old = list.len();
                    let grown = if old == 0 { 64 } else { 2 * old };
                    list.resize(grown, -1);
                    old
                }
            };
            list[slot] = child as i32;
            self.bodies[child as usize].parent[k] = parent;
            return 0;
        }
        if parent != -1 {
            return BAD_OTHER;
        }
        let old = self.bodies[child as usize].parent[k];
        if (old as u32) < capacity {
            let list = &mut self.bodies[old as usize].children[k];
            if let Some(slot) = list.iter().position(|&entry| entry == child as i32) {
                list[slot] = -1;
            }
            self.bodies[child as usize].parent[k] = -1;
        }
        0
    }

    /// D0:69 sub_456160.
    pub(crate) fn parent(&self, index: u32, kind: u32) -> Result<i32, u32> {
        let body = self.body(index)?;
        if kind >= 2 {
            return Err(BAD_KIND);
        }
        Ok(body.parent[kind as usize])
    }

    /// D0:6A sub_4561A0.
    pub(crate) fn children(&self, index: u32, kind: u32) -> Result<Vec<i32>, u32> {
        let body = self.body(index)?;
        if kind >= 2 {
            return Err(BAD_KIND);
        }
        Ok(body.children[kind as usize]
            .iter()
            .copied()
            .filter(|&entry| entry != -1)
            .collect())
    }

    /// sub_457340.
    fn overlapping(&self, point: Vec4, radius: f32, exclude: i32, mask: u8) -> Vec<i32> {
        let mut found = Vec::new();
        for (index, body) in self.bodies.iter().enumerate() {
            if !body.used || index as i32 == exclude || body.mask as u8 & mask == 0 {
                continue;
            }
            let reach = radius + body.radius;
            if len2(sub(point, body.pos)) < reach * reach {
                found.push(index as i32);
            }
        }
        found
    }

    /// D0:74 sub_456960: bodies the next step of `index` would touch.
    pub(crate) fn ahead(&self, index: u32, mask: i32) -> Result<Vec<i32>, u32> {
        let body = self.body(index)?;
        let point = add(body.vel, body.pos);
        Ok(self.overlapping(point, body.probe, index as i32, mask_or_all(mask)))
    }

    /// D0:75 sub_4569C0.
    pub(crate) fn around(&self, point: [f32; 3], radius: f32, exclude: i32, mask: i32) -> Vec<i32> {
        self.overlapping(
            [point[0], point[1], point[2], 0.0],
            radius,
            exclude,
            mask_or_all(mask),
        )
    }

    /// D0:78 sub_456A10: the others nearest first (stable), each with the
    /// unit vector toward it and its distance.
    pub(crate) fn neighbours(&self, index: u32, mask: i32) -> Result<Vec<Relation>, u32> {
        let me = self.body(index)?;
        let mask = mask_or_all(mask);
        let mut sorted: Vec<(i32, Vec4)> = Vec::new();
        for (other, body) in self.bodies.iter().enumerate() {
            if !body.used || other as u32 == index || body.mask as u8 & mask == 0 {
                continue;
            }
            let rel = norm_len(sub(body.pos, me.pos));
            // Insert after every entry whose length is <= this one.
            let at = sorted
                .iter()
                .position(|(_, v)| !(rel[3] >= v[3]))
                .unwrap_or(sorted.len());
            sorted.insert(at, (other as i32, rel));
        }
        Ok(sorted.into_iter().map(|(i, v)| (i, to_fixed(v))).collect())
    }

    /// D0:79 sub_456C00.
    pub(crate) fn relation(&self, a: u32, b: u32) -> Result<Relation, u32> {
        let from = self.body(a)?;
        let to = self
            .bodies
            .get(b as usize)
            .filter(|body| body.used)
            .ok_or(BAD_OTHER)?;
        Ok((b as i32, to_fixed(norm_len(sub(to.pos, from.pos)))))
    }

    /// D0:7A sub_456CC0.
    pub(crate) fn relation_to_point(&self, a: u32, point: [f32; 3]) -> Result<Relation, u32> {
        let from = self.body(a)?;
        let p = [point[0], point[1], point[2], 0.0];
        Ok((-1, to_fixed(norm_len(sub(p, from.pos)))))
    }

    /// D0:70 sub_456210: sweep body `index` to `point`.
    pub(crate) fn sweep_body(&self, index: u32, point: [f32; 3], mask: i32, sweep: i32) -> u32 {
        let Ok(body) = self.body(index) else {
            return BAD_BODY;
        };
        let end = [point[0], point[1], point[2], 0.0];
        self.sweep(
            body.pos,
            end,
            body.radius,
            body.priority,
            index as i32,
            mask,
            sweep,
        )
    }

    /// sub_456D70: COLLISION when a body of `radius` moving from `a` to
    /// `b` hits another body.
    #[allow(clippy::too_many_arguments)]
    fn sweep(
        &self,
        a: Vec4,
        b: Vec4,
        radius: f32,
        priority: i32,
        exclude: i32,
        mask: i32,
        sweep: i32,
    ) -> u32 {
        if a == b {
            return 0;
        }
        let mask = mask_or_all(mask);
        let dx = b[0] - a[0];
        let dy = b[1] - a[1];
        let dz = b[2] - a[2];
        let half_pi = std::f64::consts::FRAC_PI_2;
        let e1 = f64::from(f64::from(dy).atan2(f64::from(dx)) as f32) + half_pi;
        let e2 = f64::from(f64::from(dz).atan2(f64::from(dx)) as f32) + half_pi;
        // Edge k of each plane: start, direction, |dir|² and its inverse.
        let mut edges = [[([0f32; 4], [0f32; 4], 0f32, 0f32); 2]; 2];
        for (k, offset) in [0.0, std::f64::consts::PI].into_iter().enumerate() {
            let a1 = e1 + offset;
            let o1 = scale([a1.cos() as f32, a1.sin() as f32, 0.0, 0.0], radius);
            let a2 = e2 + offset;
            let o2 = scale([a2.cos() as f32, 0.0, a2.sin() as f32, 0.0], radius);
            for (plane, o, lane) in [(0, o1, 1), (1, o2, 2)] {
                let start = add(o, a);
                let d = sub(add(o, b), start);
                let l = d[0] * d[0] + d[lane] * d[lane];
                edges[plane][k] = (start, d, l, rcp_newton(l));
            }
        }
        // Kept edge distances; the target leaves them in a stack array.
        let mut kept = [[f32::INFINITY; 2]; 2];
        for (index, body) in self.bodies.iter().enumerate() {
            if !body.used || index as i32 == exclude || body.mask as u8 & mask == 0 {
                continue;
            }
            let c = body.pos;
            let reach = radius + body.radius;
            let rr = reach * reach;
            let da = len2(sub(a, c));
            let db = len2(sub(b, c));
            if db < rr {
                if da >= rr || priority <= body.priority {
                    return COLLISION;
                }
                continue;
            }
            if da < rr || sweep == 0 {
                continue;
            }
            let r2 = body.radius * body.radius;
            let mut hit = [false; 2];
            for k in 0..2 {
                for (plane, lane) in [(0usize, 1usize), (1, 2)] {
                    if hit[plane] {
                        continue;
                    }
                    let (start, d, l, il) = edges[plane][k];
                    let v = sub(c, start);
                    let t = d[0] * v[0] + d[lane] * v[lane];
                    if t > 0.0 && l > t {
                        let m = (v[0] * v[0] + v[lane] * v[lane]) - t * il * t;
                        if m < r2 {
                            hit[plane] = true;
                        } else {
                            kept[plane][k] = m;
                        }
                    }
                }
                if hit[0] && hit[1] {
                    return COLLISION;
                }
            }
            let wide = (radius + radius) * (radius + radius);
            for plane in 0..2 {
                if !hit[plane] {
                    hit[plane] = kept[plane][0] < wide && kept[plane][1] < wide;
                }
            }
            if hit[0] && hit[1] {
                return COLLISION;
            }
        }
        0
    }

    /// D0:72 sub_456280: path from `from` to touching `to`, one 16.16
    /// {x, y, z, length} per move.
    #[allow(clippy::too_many_arguments)]
    pub(crate) fn path(
        &self,
        from: u32,
        to: u32,
        range: f32,
        step: f32,
        mask: i32,
        sweep: i32,
        resolution: i32,
    ) -> Result<Vec<[i32; 4]>, u32> {
        let source = self.body(from)?;
        let target = self
            .bodies
            .get(to as usize)
            .filter(|b| b.used)
            .ok_or(BAD_OTHER)?;
        let step = if step == 0.0 { 1.0 } else { step };
        let inv = rcp_newton(step);
        let reach = range * inv;
        let steps = (f64::from(reach).ceil() as f32 as i32)
            .wrapping_abs()
            .max(1);
        let n = 2 * steps;
        let size = 2 * n + 1;
        let delta = sub(target.pos, source.pos);
        let cell_of = |v: f32| {
            let scaled = (v * 2.0 * inv).round_ties_even();
            if scaled >= -2_147_483_648.0 && scaled < 2_147_483_648.0 {
                (scaled as i32).wrapping_add(n)
            } else {
                i32::MIN.wrapping_add(n)
            }
        };
        let (tx, ty) = (cell_of(delta[0]), cell_of(delta[1]));
        // The target indexes its grid with negative cells too; refuse them.
        if tx >= size || ty >= size || tx < 0 || ty < 0 {
            return Err(NO_PATH);
        }
        let contact = (source.radius + target.radius) * 2.0 * inv;
        let g = (f64::from(contact).ceil() as f32 as i32).wrapping_abs();
        let h = if g < 4 {
            g
        } else {
            ((f64::from(g) + 0.7) * 0.707_106_7) as i32
        };
        let mut goals = [(0, 0); 16];
        for it in 0..2 {
            let base = 8 * it as usize;
            goals[base] = (tx + g + it, ty);
            goals[base + 1] = (tx + h + it, ty + h + it);
            goals[base + 2] = (tx, ty + g + it);
            goals[base + 3] = (tx - h - it, ty + h + it);
            goals[base + 4] = (tx - g - it, ty);
            goals[base + 5] = (tx - h - it, ty - h - it);
            goals[base + 6] = (tx, ty - g - it);
            goals[base + 7] = (tx + h + it, ty - h - it);
        }
        let half = f64::from(step) * 0.5;
        let moves = MOVES.map(|(mx, my)| {
            [
                (f64::from(mx) * half) as f32,
                (f64::from(my) * half) as f32,
                0.0,
                0.0,
            ]
        });
        let resolution = if 4i32.wrapping_shl(resolution as u32) > 16 {
            2
        } else {
            resolution
        };
        let mut search = PathSearch {
            space: self,
            n,
            cells: vec![Cell::default(); (size * size) as usize],
            goals,
            moves,
            from: from as usize,
            from_pos: source.pos,
            range,
            step,
            mask,
            sweep,
            resolution,
            queue: VecDeque::new(),
        };
        search.enqueue(source.pos, source.pos, (n, n), -1, 0.0, 0.0);
        let mut goal = None;
        while let Some(node) = search.queue.pop_front() {
            if let Some(found) = search.process(node) {
                goal = Some(found);
                break;
            }
        }
        let goal = goal.ok_or(NO_PATH)?;
        let (mut x, mut y) = search.goals[goal];
        let mut path = Vec::new();
        // The start cell's cost is 0, which also means "unset": a walk that
        // returns to it overwrites its -1. The target then loops forever;
        // the port stops after one pass over the grid.
        for _ in 0..search.cells.len() {
            let cell = search.cells[(y * size + x) as usize];
            if cell.dir == -1 {
                path.reverse();
                return Ok(path);
            }
            path.push(to_fixed(cell.end));
            let (bx, by) = MOVES[(cell.dir.wrapping_sub(8) & 15) as usize];
            x += bx;
            y += by;
        }
        Err(NO_PATH)
    }
}

impl PathSearch<'_> {
    fn size(&self) -> i32 {
        2 * self.n + 1
    }

    /// sub_457400.
    fn enqueue(
        &mut self,
        start: Vec4,
        end: Vec4,
        (x, y): (i32, i32),
        dir: i32,
        cost: f32,
        len: f32,
    ) {
        let size = self.size();
        if !(0..size).contains(&x) || !(0..size).contains(&y) {
            return;
        }
        if length(sub(end, self.from_pos)) > self.range {
            return;
        }
        let cell = self.cells[(y * size + x) as usize];
        if cell.cost != 0.0
            && f64::from(cell.cost) < f64::from(cost) + f64::from(len) + f64::from(self.step)
        {
            return;
        }
        self.queue.push_back(Node {
            dir,
            cost: (f64::from(cost) + f64::from(len)) as f32,
            start,
            end: [end[0], end[1], end[2], len],
            x,
            y,
        });
    }

    /// sub_457620 for one popped node; the goal index when reached.
    fn process(&mut self, node: Node) -> Option<usize> {
        let source = &self.space.bodies[self.from];
        let blocked = self.space.sweep(
            node.start,
            node.end,
            source.radius,
            source.priority,
            self.from as i32,
            self.mask,
            self.sweep,
        ) != 0;
        if blocked {
            return None;
        }
        let size = self.size();
        let index = (node.y * size + node.x) as usize;
        let cell = self.cells[index];
        if cell.cost != 0.0 && f64::from(cell.cost) < f64::from(self.step) + f64::from(node.cost) {
            return None;
        }
        self.cells[index] = Cell {
            dir: node.dir,
            cost: node.cost,
            end: node.end,
        };
        if let Some(goal) = self.goals.iter().position(|&g| g == (node.x, node.y)) {
            return Some(goal);
        }
        let (count, first) = if node.dir == -1 {
            (16, 0)
        } else {
            (9, node.dir.wrapping_sub(4) & 15)
        };
        let stride = 16i32.wrapping_shr(self.resolution.wrapping_add(2) as u32);
        if stride <= 0 {
            // The target spins here forever.
            return None;
        }
        let mut i = 0;
        while i < count {
            let k = ((first + i) & 15) as usize;
            let movement = self.moves[k];
            let len = length(movement);
            let next = add(node.end, movement);
            let (mx, my) = MOVES[k];
            self.enqueue(
                node.end,
                next,
                (node.x + mx, node.y + my),
                k as i32,
                node.cost,
                len,
            );
            i += stride;
        }
        None
    }
}

#[cfg(test)]
mod tests {
    use super::*;

    fn space_with(bodies: &[([f32; 3], f32)]) -> SpaceManager {
        let mut space = SpaceManager::new();
        for (i, &(pos, radius)) in bodies.iter().enumerate() {
            assert_eq!(
                space.add(i as u32, pos, [1.0, 0.0, 0.0], radius, 2.0, radius, 1, 0),
                0
            );
        }
        space
    }

    #[test]
    fn bodies_store_snapped_positions_and_unit_directions() {
        let mut space = SpaceManager::new();
        assert_eq!(
            space.add(0, [1.0004, 2.5, 0.0], [3.0, 4.0, 0.0], 1.0, 10.0, 1.0, 1, 0),
            0
        );
        assert_eq!(space.pos(0), Ok([1 << 16, 0x28000, 0, 0]));
        let dir = space.dir(0).unwrap();
        assert!((dir[0] - 0x9999).abs() <= 1 && (dir[1] - 0xCCCC).abs() <= 1);
        assert_eq!(
            space.add(1, [0.0; 3], [1.0, 0.0, 0.0], 0.5, 1.0, 1.0, 1, 0),
            BAD_OTHER
        );
        assert_eq!(
            space.add(1, [0.0; 3], [1.0, 0.0, 0.0], 1.0, 1.0, 1.0, 0x100, 0),
            BAD_MASK
        );
        assert_eq!(space.set_dir(0, [0.0; 3]), BAD_OTHER);
        assert_eq!(
            space.add(100, [0.0; 3], [1.0, 0.0, 0.0], 1.0, 1.0, 1.0, 1, 0),
            0
        );
        assert_eq!(space.bodies.len(), 128);
        assert_eq!(
            space.add(0x1000, [0.0; 3], [1.0, 0.0, 0.0], 1.0, 1.0, 1.0, 1, 0),
            BAD_BODY
        );
    }

    #[test]
    fn links_fill_free_slots_and_detach() {
        let mut space = space_with(&[
            ([0.0; 3], 1.0),
            ([5.0, 0.0, 0.0], 1.0),
            ([9.0, 0.0, 0.0], 1.0),
        ]);
        assert_eq!(space.link(1, 0, 0), 0);
        assert_eq!(space.link(2, 0, 0), 0);
        assert_eq!(space.children(0, 0), Ok(vec![1, 2]));
        assert_eq!(space.parent(2, 0), Ok(0));
        assert_eq!(space.link(1, 0, -1), 0);
        assert_eq!(space.children(0, 0), Ok(vec![2]));
        assert_eq!(space.link(1, 0, 0), 0);
        assert_eq!(space.children(0, 0), Ok(vec![1, 2]));
        assert_eq!(space.link(1, 2, 0), BAD_KIND);
        assert_eq!(space.link(1, 0, 1), BAD_OTHER);
        assert_eq!(space.remove(0), 0);
        assert_eq!(space.parent(1, 0), Ok(-1));
        assert_eq!(space.parent(0, 0), Err(BAD_BODY));
    }

    #[test]
    fn overlap_and_neighbour_queries() {
        let space = space_with(&[
            ([0.0; 3], 1.0),
            ([3.0, 0.0, 0.0], 1.0),
            ([0.0, 2.0, 0.0], 1.0),
        ]);
        // Body 0 moves +x by 2: its probe circle reaches body 1.
        assert_eq!(space.ahead(0, 0), Ok(vec![1]));
        assert_eq!(space.around([0.0, 0.0, 0.0], 1.5, -1, 0), vec![0, 2]);
        let near = space.neighbours(0, 0xFF).unwrap();
        assert_eq!(near.iter().map(|r| r.0).collect::<Vec<_>>(), [2, 1]);
        assert_eq!(near[0].1, [0, 1 << 16, 0, 2 << 16]);
        assert_eq!(space.relation(0, 1), Ok((1, [1 << 16, 0, 0, 3 << 16])));
        assert_eq!(
            space.relation_to_point(0, [0.0, -4.0, 0.0]),
            Ok((-1, [0, -(1 << 16), 0, 4 << 16]))
        );
        assert_eq!(space.relation(0, 9), Err(BAD_OTHER));
    }

    #[test]
    fn sweeps_hit_bodies_in_the_way() {
        let space = space_with(&[
            ([0.0; 3], 1.0),
            ([5.0, 0.0, 0.0], 1.0),
            ([5.0, 5.0, 0.0], 1.0),
        ]);
        // Ending inside body 1.
        assert_eq!(space.sweep_body(0, [4.5, 0.0, 0.0], 0, 0), COLLISION);
        // Passing through body 1 is only seen with the sweep flag.
        assert_eq!(space.sweep_body(0, [10.0, 0.0, 0.0], 0, 0), 0);
        assert_eq!(space.sweep_body(0, [10.0, 0.0, 0.0], 0, 1), COLLISION);
        // Going up, nothing in the way until body 2's column.
        assert_eq!(space.sweep_body(0, [0.0, 10.0, 0.0], 0, 1), 0);
    }

    fn check_path(path: &[[i32; 4]], target: (f64, f64), blocker: Option<(f64, f64, f64)>) {
        let unit = 65_536.0;
        let mut at = (0.0, 0.0);
        for p in path {
            let (x, y) = (f64::from(p[0]) / unit, f64::from(p[1]) / unit);
            let (dx, dy) = (x - at.0, y - at.1);
            // Every move is one of the 16 (in half-step cells of 1.0).
            assert!(
                MOVES
                    .iter()
                    .any(|&(mx, my)| (f64::from(mx) - dx).abs() < 1e-3
                        && (f64::from(my) - dy).abs() < 1e-3)
            );
            let len = f64::from(p[3]) / unit;
            assert!((len - (dx * dx + dy * dy).sqrt()).abs() < 1e-3);
            if let Some((bx, by, reach)) = blocker {
                assert!(((x - bx).powi(2) + (y - by).powi(2)).sqrt() >= reach);
            }
            at = (x, y);
        }
        // It stops on a contact tile: 2..4 units from the target's centre.
        let gap = ((at.0 - target.0).powi(2) + (at.1 - target.1).powi(2)).sqrt();
        assert!((2.0..4.3).contains(&gap), "{gap}");
    }

    #[test]
    fn path_reaches_contact_and_walks_around_a_blocker() {
        let mut space = space_with(&[([0.0; 3], 1.0), ([12.0, 0.0, 0.0], 1.0)]);
        let path = space.path(0, 1, 20.0, 2.0, 0, 1, 2).unwrap();
        check_path(&path, (12.0, 0.0), None);
        // FIFO order and the d-4 first expansion settle the lower diagonal
        // contact tile (9,-3) before the straight one (10,0).
        let last = path.last().unwrap();
        assert_eq!((last[0], last[1]), (9 << 16, -3 << 16));
        space.add(2, [6.0, 0.0, 0.0], [1.0, 0.0, 0.0], 2.0, 0.0, 2.0, 1, 0);
        let around = space.path(0, 1, 20.0, 2.0, 0, 1, 2).unwrap();
        check_path(&around, (12.0, 0.0), Some((6.0, 0.0, 3.0)));
        assert_eq!(space.path(0, 1, 4.0, 2.0, 0, 1, 2), Err(NO_PATH));
        assert_eq!(space.path(0, 7, 20.0, 2.0, 0, 1, 2), Err(BAD_OTHER));
    }
}

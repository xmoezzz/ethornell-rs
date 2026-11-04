//! BGI 1.622 DCParticleControl, DCPSnow, DCPFirefly and DCParticleEnvAir.
//! Raw evidence: 44DCE0,44E080..44F190,44F660..450740.
use super::*;

#[derive(Debug, Clone, Default)]
pub(super) struct ParticleBank {
    pub frames: Vec<DecodedImage>,
    pub format: i32,
    pub speed: [i32; 2],
    mips: Vec<Vec<Option<DecodedImage>>>,
}
impl ParticleBank {
    pub fn new(frames: Vec<DecodedImage>, format: i32, speed: [i32; 2]) -> Self {
        let mips = (0..32)
            .map(|shrink| {
                frames
                    .iter()
                    .map(|image| scale_particle_frame(image, format, shrink))
                    .collect()
            })
            .collect();
        Self {
            frames,
            format,
            speed,
            mips,
        }
    }
}
#[derive(Debug, Default)]
pub(super) struct ParticleBanks {
    pub primary: BTreeMap<(i32, i32), ParticleBank>,
    pub secondary: BTreeMap<i32, ParticleBank>,
    pub mix: BTreeMap<i32, i32>,
    pub snow: BTreeMap<i32, [i32; 11]>,
    pub firefly: BTreeMap<i32, [i32; 18]>,
}
#[derive(Debug, Clone)]
struct Particle {
    kind: i32,
    bank: i32,
    f: [i32; 30],
}
#[derive(Debug, Clone)]
pub(super) struct ParticleWorld {
    pub camera: [i32; 3],
    pub rotation: [f64; 3],
    pub focal: i32,
    pub center: [i32; 2],
    pub interval: u32,
    pub clock: u32,
    // Requested counts, current counts, emission intervals, next emission.
    pub count: [[i32; 64]; 2],
    pub active: [[i32; 64]; 2],
    pub spacing: [[u32; 64]; 2],
    pub next: [[u32; 64]; 2],
    pub emitter: [[i32; 14]; 64],
    env: [i32; 29],
    particles: Vec<Option<Particle>>,
}
impl Default for ParticleWorld {
    fn default() -> Self {
        Self {
            camera: [0; 3],
            rotation: [0.0; 3],
            focal: 0,
            center: [0; 2],
            interval: 10,
            clock: 0,
            count: [[0; 64]; 2],
            active: [[0; 64]; 2],
            spacing: [[0; 64]; 2],
            next: [[0; 64]; 2],
            emitter: {
                let mut e = [[0; 14]; 64];
                for row in &mut e {
                    row[10] = 1;
                }
                e
            },
            env: [0; 29],
            particles: Vec::new(),
        }
    }
}
fn add(a: i32, b: i32) -> i32 {
    a.wrapping_add(b)
}
fn sub(a: i32, b: i32) -> i32 {
    a.wrapping_sub(b)
}
fn mul(a: i32, b: i32) -> i32 {
    a.wrapping_mul(b)
}
fn div(a: i32, b: i32) -> i32 {
    if b == 0 { 0 } else { a.wrapping_div(b) }
}
fn random_range(base: i32, range: i32, rng: &mut impl FnMut(i32) -> i32) -> i32 {
    add(sub(base, range), rng(mul(2, range)))
}
fn env_target(env: &mut [i32; 29], at: usize, rng: &mut impl FnMut(i32) -> i32) {
    for i in 0..3 {
        env[at + i] = random_range(env[6 + i], env[9 + i], rng);
    }
}
fn env_segment(env: &mut [i32; 29], duration: i32, mode: i32) {
    env[20] = mode;
    env[21] = if duration == 0 { 1 } else { duration };
    env[22] = 0;
}
fn env_change(env: &mut [i32; 29], initial: bool, rng: &mut impl FnMut(i32) -> i32) {
    let duration = add(env[16], rng(env[17]));
    env_segment(env, duration, 0);
    if initial {
        env_target(env, 23, rng);
    } else {
        env[23] = env[26];
        env[24] = env[27];
        env[25] = env[28];
    }
    env_target(env, 26, rng);
}
impl ParticleWorld {
    pub fn reset(&mut self) {
        self.particles.clear();
        self.active = [[0; 64]; 2];
        self.next = [[0; 64]; 2];
    }
    pub fn configure_count(&mut self, kind: usize, bank: usize, count: i32, spacing: u32) -> bool {
        if kind >= 2 || bank >= 64 || count < 0 {
            return false;
        }
        let total: i64 = self.count.iter().flatten().map(|n| i64::from(*n)).sum();
        if total - i64::from(self.count[kind][bank]) + i64::from(count) > 32768 {
            return false;
        }
        self.count[kind][bank] = count;
        self.spacing[kind][bank] = spacing;
        true
    }
    pub fn configure_environment(&mut self, a: &[i32], rng: &mut impl FnMut(i32) -> i32) {
        for i in 0..3 {
            self.env[6 + i] = a[2 + i * 2] >> 8;
            self.env[9 + i] = a[3 + i * 2] >> 8;
        }
        self.env[12..16].copy_from_slice(&a[8..12]);
        self.retime_environment();
        self.env[2] = i32::from(a[1] != 0 && self.interval != 0);
        env_change(&mut self.env, true, rng);
    }
    pub fn retime_environment(&mut self) {
        self.env[1] = self.interval as i32;
        if self.interval != 0 {
            for i in 0..4 {
                self.env[16 + i] = ((self.env[12 + i] as u32) / self.interval) as i32;
            }
        }
    }
    fn step_env(&mut self, rng: &mut impl FnMut(i32) -> i32) -> [i32; 3] {
        let e = &mut self.env;
        if e[2] == 0 {
            e[3..6].fill(0);
            return [0; 3];
        }
        for i in 0..3 {
            e[3 + i] = if e[20] == 0 {
                e[23 + i]
            } else {
                div(
                    add(mul(sub(e[21], e[22]), e[23 + i]), mul(e[22], e[26 + i])),
                    e[21],
                )
            };
        }
        e[22] = e[22].wrapping_add(1);
        if e[22] as u32 >= e[21] as u32 {
            if e[20] == 0 {
                let duration = add(e[18], rng(e[19]));
                env_segment(e, duration, 1);
            } else {
                env_change(e, false, rng);
            }
        }
        [e[3], e[4], e[5]]
    }
    pub fn poll(&mut self, now: u32, banks: &ParticleBanks, rng: &mut impl FnMut(i32) -> i32) {
        if self.interval == 0 {
            return;
        }
        let caught_up = self.clock.wrapping_add(500) >= now;
        if !caught_up {
            self.clock = now;
        }
        while self.clock <= now {
            let wind = self.step_env(rng);
            for slot in &mut self.particles {
                let Some(p) = slot else {
                    continue;
                };
                for i in 0..3 {
                    p.f[4 + i] = add(p.f[4 + i], mul(p.f[7], wind[i]) >> 8);
                }
                if !p.step(banks, &self.emitter[p.bank as usize], rng) {
                    self.active[p.kind as usize][p.bank as usize] -= 1;
                    *slot = None;
                }
            }
            let advanced = self.clock.wrapping_add(self.interval);
            self.clock = advanced;
            if advanced < self.interval {
                break;
            }
        }
        for kind in 0..2 {
            for bank in 0..64 {
                if !caught_up {
                    self.next[kind][bank] = now;
                }
                if self.next[kind][bank] > now {
                    continue;
                }
                while self.active[kind][bank] < self.count[kind][bank] {
                    let particle =
                        Particle::new(kind as i32, bank as i32, banks, &self.emitter[bank], rng);
                    if let Some(slot) = self.particles.iter_mut().find(|p| p.is_none()) {
                        *slot = Some(particle);
                    } else if self.particles.len() < 32768 {
                        self.particles.push(Some(particle));
                    } else {
                        break;
                    }
                    self.active[kind][bank] += 1;
                    if self.next[kind][bank] == 0 {
                        self.next[kind][bank] = now;
                    }
                    self.next[kind][bank] =
                        self.next[kind][bank].wrapping_add(self.spacing[kind][bank]);
                    if self.next[kind][bank] > now {
                        break;
                    }
                }
                if self.active[kind][bank] >= self.count[kind][bank] {
                    self.next[kind][bank] = 0;
                }
            }
        }
    }
    pub fn warm_up(
        &mut self,
        count: u32,
        now: u32,
        banks: &ParticleBanks,
        rng: &mut impl FnMut(i32) -> i32,
    ) {
        let (clock, next) = (self.clock, self.next);
        for offset in 1..=count {
            self.poll(now.wrapping_add(offset), banks, rng);
        }
        self.clock = clock;
        self.next = next;
    }
    pub fn rebind_secondary(
        &mut self,
        bank: i32,
        banks: &ParticleBanks,
        rng: &mut impl FnMut(i32) -> i32,
    ) {
        let Some(animation) = banks.secondary.get(&bank) else {
            return;
        };
        for p in self
            .particles
            .iter_mut()
            .flatten()
            .filter(|p| p.kind == 1 && p.bank == bank)
        {
            p.f[11] = add(animation.speed[0], rng(animation.speed[1]));
            p.f[12] = animation.frames.len() as i32;
        }
    }
    pub fn raster(
        &mut self,
        width: u32,
        height: u32,
        first: &[i32],
        banks: &ParticleBanks,
    ) -> Vec<DecodedImage> {
        let mut planes = first
            .iter()
            .map(|_| DecodedImage {
                width,
                height,
                rgba: vec![0; width as usize * height as usize * 4],
            })
            .collect::<Vec<_>>();
        if self.focal <= 0 || first.is_empty() {
            return planes;
        }
        let mut visible = Vec::new();
        for (index, p) in self
            .particles
            .iter()
            .enumerate()
            .filter_map(|(i, p)| p.as_ref().map(|p| (i, p)))
        {
            let mut v = [
                sub(p.f[4], self.camera[0]),
                sub(p.f[5], self.camera[1]),
                sub(p.f[6], self.camera[2]),
            ];
            let (s, c) = self.rotation[0].sin_cos();
            let (y, z) = (v[1] as f64, v[2] as f64);
            v[1] = (c * y - s * z) as i32;
            v[2] = (c * z + s * y) as i32;
            let (s, c) = self.rotation[1].sin_cos();
            let (x, z) = (v[0] as f64, v[2] as f64);
            v[2] = (c * z - s * x) as i32;
            v[0] = (s * z + c * x) as i32;
            let (s, c) = self.rotation[2].sin_cos();
            let (x, y) = (v[0] as f64, v[1] as f64);
            v[0] = (c * x - s * y) as i32;
            v[1] = (s * x + c * y) as i32;
            if v[2] >= 0 {
                visible.push((v, index));
            }
        }
        visible.sort_by(|a, b| b.0[2].cmp(&a.0[2]));
        let mut limit = 0i32;
        let thresholds = first
            .iter()
            .map(|n| {
                limit = add(limit, n.wrapping_shl(8));
                limit
            })
            .collect::<Vec<_>>();
        for (v, index) in visible {
            let Some(plane) = thresholds.iter().position(|limit| v[2] <= *limit) else {
                continue;
            };
            let denom = add(v[2], self.focal.wrapping_shl(8));
            if denom == 0 {
                continue;
            }
            let shrink = (32 - div(mul(32, self.focal), add(self.focal, v[2] >> 8))).clamp(0, 32);
            if shrink >= 32 {
                continue;
            }
            let p = self.particles[index].as_mut().unwrap();
            let Some(image) = p.image(shrink, banks) else {
                continue;
            };
            let x = add(self.center[0], div(mul(self.focal, v[0]), denom)) - image.width as i32 / 2;
            let y =
                sub(self.center[1], div(mul(self.focal, v[1]), denom)) - image.height as i32 / 2;
            let transparency = if p.kind == 1 {
                let age = p.f[18] as u32;
                let remaining = (p.f[17] as u32).wrapping_sub(age);
                if remaining < p.f[28] as u32 {
                    256 - ((remaining << 8) / (p.f[28] as u32).max(1)) as i32
                } else if age < p.f[27] as u32 {
                    256 - ((age << 8) / (p.f[27] as u32).max(1)) as i32
                } else {
                    0
                }
            } else {
                0
            };
            let format = banks.primary.get(&(p.kind, p.bank)).map_or(2, |b| b.format);
            crate::bitmap_blend::blit_mode(
                &mut planes[plane],
                2,
                &image,
                format,
                x,
                y,
                p.f[14],
                transparency,
            );
        }
        planes
    }
}
impl Particle {
    fn new(
        kind: i32,
        bank: i32,
        banks: &ParticleBanks,
        emitter: &[i32; 14],
        rng: &mut impl FnMut(i32) -> i32,
    ) -> Self {
        let mut p = Self {
            kind,
            bank,
            f: [0; 30],
        };
        if let Some(animation) = banks.primary.get(&(kind, bank)) {
            p.f[10] = add(animation.speed[0], rng(animation.speed[1]));
            p.f[11] = p.f[10];
            p.f[12] = animation.frames.len() as i32;
        }
        if kind == 0 {
            if let Some(style) = banks.snow.get(&bank) {
                p.f[4] = sub(rng(mul(2, style[1])), style[1]);
                p.f[5] = style[2];
                p.f[6] = sub(rng(mul(2, style[3])), style[3]);
                for i in 0..3 {
                    p.f[16 + i] = random_range(style[4 + 2 * i], style[5 + 2 * i], rng);
                }
                p.f[7] = style[10];
                p.f[14] = 32;
                p.f[3] = 1;
            }
        } else if let Some(style) = banks.firefly.get(&bank) {
            p.f[17] = add(style[1], rng(style[2]));
            p.f[4] = random_range(emitter[0], style[3], rng);
            p.f[5] = add(sub(style[4], emitter[1]), rng(mul(2, emitter[1])));
            p.f[6] = random_range(emitter[2], style[5], rng);
            for i in 0..3 {
                p.f[20 + 2 * i] = random_range(style[6 + 2 * i], style[7 + 2 * i], rng);
            }
            p.normalize_velocity(emitter, true, rng);
            p.change_velocity(style, emitter, rng);
            p.f[7] = style[14];
            p.f[27] = style[15];
            p.f[28] = style[16];
            p.f[14] = style[17];
            p.f[3] = 1;
        }
        if kind == 1 {
            if let Some(animation) = banks.secondary.get(&bank) {
                p.f[11] = add(animation.speed[0], rng(animation.speed[1]));
                p.f[12] = animation.frames.len() as i32;
            }
        }
        p
    }
    fn normalize_velocity(
        &mut self,
        e: &[i32; 14],
        initial: bool,
        rng: &mut impl FnMut(i32) -> i32,
    ) {
        if e[3] == 0 {
            return;
        }
        let v = [self.f[20] as f64, self.f[22] as f64, self.f[24] as f64];
        let magnitude = (v.iter().map(|v| v * v).sum::<f64>()).sqrt().max(1.0);
        let mut factor = 1.0;
        for i in 0..3 {
            factor *= 1.0 - v[i].abs() / magnitude * (65536.0 - e[6 + i] as f64) / 65536.0;
        }
        let speed = add(e[4], rng(e[5])) as f64;
        for i in 0..3 {
            self.f[20 + 2 * i] = (v[i] * factor * speed / magnitude) as i32;
        }
        if initial && e[9] != 0 && e[4] > 0 && speed != 0.0 {
            self.f[17] = (self.f[17] as u32 as f64 * e[4] as f64 / speed) as i64 as i32;
        }
    }
    fn change_velocity(&mut self, s: &[i32; 18], e: &[i32; 14], rng: &mut impl FnMut(i32) -> i32) {
        let mut v = [self.f[20] as f64, self.f[22] as f64, self.f[24] as f64];
        for i in 0..3 {
            self.f[19 + 2 * i] = v[i] as i32;
        }
        if e[11] != 0 {
            let angle = random_range(e[12], e[13], rng) as f64 * std::f64::consts::PI / 11796480.0;
            let (sin, cos) = angle.sin_cos();
            let (y, z) = (v[1], v[2]);
            v[1] = cos * y - sin * z;
            v[2] = sin * y + cos * z;
            let (x, z) = (v[0], v[2]);
            v[2] = cos * z - sin * x;
            v[0] = sin * z + cos * x;
            let (x, y) = (v[0], v[1]);
            v[0] = cos * x - sin * y;
            v[1] = sin * x + cos * y;
            for i in 0..3 {
                self.f[20 + 2 * i] = v[i].round() as i32;
            }
        } else {
            for i in 0..3 {
                self.f[20 + 2 * i] = random_range(s[6 + 2 * i], s[7 + 2 * i], rng);
            }
            self.normalize_velocity(e, false, rng);
        }
        let segment = add(s[12], rng(s[13]));
        self.f[25] = if segment == 0 { 1 } else { segment };
        self.f[26] = 0;
    }
    fn step(
        &mut self,
        banks: &ParticleBanks,
        emitter: &[i32; 14],
        rng: &mut impl FnMut(i32) -> i32,
    ) -> bool {
        self.f[8] = add(self.f[8], self.f[10]);
        self.f[9] = add(self.f[9], self.f[11]);
        if let Some(bank) = banks.primary.get(&(self.kind, self.bank)) {
            let span = (bank.frames.len() as u32).wrapping_shl(16);
            if span != 0 {
                self.f[8] = ((self.f[8] as u32) % span) as i32;
            }
        }
        let span = (self.f[12] as u32).wrapping_shl(16);
        if span != 0 {
            self.f[9] = ((self.f[9] as u32) % span) as i32;
        }
        if self.f[3] == 0 {
            return false;
        }
        if self.kind == 0 {
            for i in 0..3 {
                self.f[4 + i] = add(self.f[4 + i], self.f[16 + i]);
            }
            return true;
        }
        let age = self.f[18] as u32;
        self.f[18] = add(self.f[18], 1);
        if age >= self.f[17] as u32 {
            return false;
        }
        for i in 0..3 {
            let velocity = add(
                self.f[19 + 2 * i],
                div(
                    mul(self.f[26], sub(self.f[20 + 2 * i], self.f[19 + 2 * i])),
                    self.f[25],
                ),
            );
            self.f[4 + i] = add(self.f[4 + i], velocity);
        }
        if self.f[26] as u32 >= self.f[25] as u32 {
            if let Some(style) = banks.firefly.get(&self.bank) {
                self.change_velocity(style, emitter, rng);
            }
        } else if emitter[10] != 0 {
            self.f[26] = add(self.f[26], 1);
        }
        true
    }
    fn image(&self, shrink: i32, banks: &ParticleBanks) -> Option<DecodedImage> {
        fn scaled(bank: &ParticleBank, frame: i32, shrink: i32) -> Option<DecodedImage> {
            bank.mips
                .get(shrink as usize)?
                .get((frame as u32 >> 16) as usize)?
                .clone()
        }
        let primary = banks.primary.get(&(self.kind, self.bank))?;
        let image = scaled(primary, self.f[8], shrink)?;
        if self.kind != 1 || banks.mix.get(&self.bank).copied().unwrap_or(0) == 0 {
            return Some(image);
        }
        let secondary = scaled(banks.secondary.get(&self.bank)?, self.f[9], shrink)?;
        let (w, h) = (
            image.width.max(secondary.width),
            image.height.max(secondary.height),
        );
        let mut out = DecodedImage {
            width: w,
            height: h,
            rgba: vec![0; w as usize * h as usize * 4],
        };
        graph::blit_decoded_image_raw_copy(
            &mut out,
            &image,
            (w - image.width) as i32 / 2,
            (h - image.height) as i32 / 2,
        );
        let mut other = DecodedImage {
            width: w,
            height: h,
            rgba: vec![0; out.rgba.len()],
        };
        graph::blit_decoded_image_raw_copy(
            &mut other,
            &secondary,
            (w - secondary.width) as i32 / 2,
            (h - secondary.height) as i32 / 2,
        );
        crate::bitmap_blend::crossfade_cache(&out, &other, primary.format, banks.mix[&self.bank])
    }
}

// 44DCE0 allocates floor(scale*size) but 495880 samples a rounded scaled
// footprint centered inside it. This is not equivalent to nearest resize.
fn scale_particle_frame(source: &DecodedImage, format: i32, shrink: i32) -> Option<DecodedImage> {
    let scale = (32 - shrink) as u32 * 2048;
    let width = source.width.wrapping_mul(scale) >> 16;
    let height = source.height.wrapping_mul(scale) >> 16;
    if width == 0 || height == 0 {
        return None;
    }
    let w = source.width.wrapping_mul(scale).wrapping_add(32768) >> 16;
    let h = source.height.wrapping_mul(scale).wrapping_add(32768) >> 16;
    let mut out = DecodedImage {
        width,
        height,
        rgba: vec![0; width as usize * height as usize * 4],
    };
    let origin_x = width as i32 / 2 - w as i32 / 2;
    let origin_y = height as i32 / 2 - h as i32 / 2;
    let mut footprint = 65536 / (scale >> 8);
    let kernel = if scale > 32768 && scale < 65536 {
        1
    } else if scale < 32768 {
        2
    } else {
        0
    };
    let (mut start, shift) = (footprint << 7, 8);
    let excluded = if kernel == 0 {
        footprint = 512;
        start = 0;
        footprint >> 9
    } else {
        footprint >> shift
    };
    let span_x = source.width.saturating_sub(excluded) << 16;
    let span_y = source.height.saturating_sub(excluded) << 16;
    let step_x = span_x / w;
    let step_y = span_y / h;
    let pixel = |x: u32, y: u32| -> [u8; 4] {
        if x >= source.width || y >= source.height {
            [0; 4]
        } else {
            source.rgba[((y * source.width + x) * 4) as usize..][..4]
                .try_into()
                .unwrap()
        }
    };
    for y in 0..height {
        for x in 0..width {
            let sx = start.wrapping_add(step_x.wrapping_mul((x as i32 - origin_x) as u32));
            let sy = start.wrapping_add(step_y.wrapping_mul((y as i32 - origin_y) as u32));
            let mut color = [0u8; 4];
            match kernel {
                0 => {
                    let (ix, iy) = (sx >> 16, sy >> 16);
                    let (fx, fy) = ((sx >> 8) & 255, (sy >> 8) & 255);
                    let p = [
                        pixel(ix, iy),
                        pixel(ix + 1, iy),
                        pixel(ix, iy + 1),
                        pixel(ix + 1, iy + 1),
                    ];
                    for c in 0..4 {
                        color[c] = (((256 - fy)
                            * ((256 - fx) * p[0][c] as u32 + fx * p[1][c] as u32)
                            + fy * ((256 - fx) * p[2][c] as u32 + fx * p[3][c] as u32))
                            >> 16) as u8;
                    }
                }
                1 => {
                    let (left, top) = (sx >> 8, sy >> 8);
                    let right = (left + footprint).min(source.width << 8);
                    let bottom = (top + footprint).min(source.height << 8);
                    let area = right
                        .saturating_sub(left)
                        .wrapping_mul(bottom.saturating_sub(top))
                        >> 8;
                    let mut sum = [0u32; 4];
                    let mut py = top;
                    while py < bottom {
                        let ny = ((py + 256) & !255).min(bottom);
                        let mut px = left;
                        while px < right {
                            let nx = ((px + 256) & !255).min(right);
                            let weight = if area == 0 {
                                0
                            } else {
                                ((ny - py) * (nx - px) << 8) / area
                            } >> 8;
                            let p = pixel(px >> 8, py >> 8);
                            for c in 0..4 {
                                sum[c] = sum[c]
                                    .saturating_add((p[c] as u32 * weight) & 65535)
                                    .min(65535);
                            }
                            px = nx;
                        }
                        py = ny;
                    }
                    for c in 0..4 {
                        color[c] = (sum[c] >> 8) as u8;
                    }
                }
                _ => {
                    let (left, top) = (sx >> 16, sy >> 16);
                    let right = (left + ((footprint + 255) >> 8)).min(source.width);
                    let bottom = (top + ((footprint + 255) >> 8)).min(source.height);
                    let mut count = 0;
                    let mut total = 0;
                    let mut sum = [0u32; 4];
                    for py in top..bottom {
                        for px in left..right {
                            let p = pixel(px, py);
                            total += 1;
                            if p[3] != 0 || format == 1 {
                                count += 1;
                                for c in 0..4 {
                                    sum[c] = sum[c].wrapping_add(p[c] as u32);
                                }
                            }
                        }
                    }
                    for c in 0..3 {
                        color[c] = (sum[c] / count.max(1)) as u8;
                    }
                    color[3] = (sum[3] / total.max(1)) as u8;
                }
            }
            if format == 1 {
                color[3] = 255;
            }
            out.rgba[((y * width + x) * 4) as usize..][..4].copy_from_slice(&color);
        }
    }
    Some(out)
}

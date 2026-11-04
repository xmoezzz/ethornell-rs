use super::*;

const PARTICLE_HANDLE_BASE: i32 = 0xC000_0000u32 as i32;
const MAX_PARTICLE_SCREENS: usize = 8;
const MAX_SPLINE_POINTS: usize = 100;

#[derive(Debug, Default)]
pub(super) struct NativeEffectState {
    particle_screens: BTreeMap<i32, NativeParticleScreen>,
    particle_banks: native_particle::ParticleBanks,
    particle_interpolation_mode: i32,
    splines: BTreeMap<i32, NativeSpline>,
    next_spline_handle: i32,
}

#[derive(Debug, Clone)]
struct NativeParticleScreen {
    handle: i32,
    width: i32,
    height: i32,
    enabled: bool,
    display: [i32; 5],
    frame_count: usize,
    frame_tables: [Vec<i32>; 2],
    native_field_340: i32,
    auto_update_interval_ms: i32,
    auto_update_deadline: u32,
    commit_count: u64,
    emitter_layout: Vec<i32>,
    emission_percent: i32,
    advance_count: u64,
    emitter_configuration: Vec<i32>,
    world: native_particle::ParticleWorld,
    layers: Vec<i32>,
}

const PARTICLE_SCREEN_HANDLE: &str = "無効なパーティクルスクリーンハンドルが指定されました";

impl NativeParticleScreen {
    fn new(handle: i32, width: i32, height: i32) -> Self {
        Self {
            handle,
            width,
            height,
            enabled: true,
            display: [0, 0, 0, 128, 0],
            frame_count: 1,
            frame_tables: [vec![0x7fff], vec![0]],
            native_field_340: 4096,
            auto_update_interval_ms: 0,
            auto_update_deadline: 0,
            commit_count: 0,
            emitter_layout: Vec::new(),
            emission_percent: 0,
            advance_count: 0,
            emitter_configuration: Vec::new(),
            world: {
                let mut world = native_particle::ParticleWorld::default();
                world.center = [width / 2, height / 2];
                world
            },
            layers: Vec::new(),
        }
    }

    fn commit(&mut self) {
        self.commit_count = self.commit_count.saturating_add(1);
    }
}

#[derive(Debug, Clone)]
pub(super) struct NativeSpline {
    duration: i32,
    points: Vec<[i32; 3]>,
}

impl NativeEffectState {
    fn allocate_particle_screen(&mut self, width: i32, height: i32) -> Option<i32> {
        let slot = (0..MAX_PARTICLE_SCREENS).find(|slot| {
            !self
                .particle_screens
                .contains_key(&(PARTICLE_HANDLE_BASE | *slot as i32))
        })?;
        let handle = PARTICLE_HANDLE_BASE | slot as i32;
        self.particle_screens
            .insert(handle, NativeParticleScreen::new(handle, width, height));
        Some(handle)
    }

    fn release_particle_screen(&mut self, handle: i32) -> bool {
        self.particle_screens.remove(&handle).is_some()
    }

    fn particle_screen_mut(&mut self, handle: i32) -> Option<&mut NativeParticleScreen> {
        self.particle_screens.get_mut(&handle)
    }

    /// sub_4417A0 -> sub_424A50 / sub_425010.
    pub(super) fn configure_particle_frame_tables(
        &mut self,
        handle: i32,
        count: i32,
        first: &[i32],
        second: &[i32],
    ) -> i32 {
        let Some(screen) = self.particle_screens.get_mut(&handle) else {
            return 255;
        };
        if (count as u32).wrapping_sub(1) > 0xF || first.len() != count as usize {
            return 7;
        }
        if second
            .iter()
            .any(|offset| (offset.wrapping_add(screen.display[2]) as u32) > 0xFFFF)
        {
            return 8;
        }
        screen.frame_count = first.len();
        screen.frame_tables = [
            first.to_vec(),
            if first.len() == 1 {
                vec![0]
            } else {
                second.to_vec()
            },
        ];
        0
    }

    pub(super) fn create_spline(&mut self) -> i32 {
        // sub_495420: ids come from dword_503E18, which starts at
        // 0x80000000 (the field counts from 0 here).
        let handle = self.next_spline_handle.wrapping_add(i32::MIN);
        self.next_spline_handle = self.next_spline_handle.wrapping_add(1);
        self.splines.insert(
            handle,
            NativeSpline {
                duration: 0,
                points: Vec::new(),
            },
        );
        handle
    }

    pub(super) fn release_spline(&mut self, handle: i32) -> i32 {
        if self.splines.remove(&handle).is_some() {
            0
        } else {
            1
        }
    }

    pub(super) fn spline_exists(&self, handle: i32) -> bool {
        self.splines.contains_key(&handle)
    }

    pub(super) fn configure_spline(
        &mut self,
        handle: i32,
        duration: i32,
        points: &[[i32; 3]],
    ) -> i32 {
        if points.len() < 2 {
            return 2;
        }
        if duration < 2 {
            return 3;
        }
        let Some(spline) = self.splines.get_mut(&handle) else {
            return 1;
        };
        spline.duration = duration;
        spline.points.clear();
        spline
            .points
            .extend(points.iter().take(MAX_SPLINE_POINTS).copied());
        0
    }

    pub(super) fn sample_spline(
        &self,
        handle: i32,
        time: i32,
    ) -> std::result::Result<[i32; 3], i32> {
        let Some(spline) = self.splines.get(&handle) else {
            return Err(1);
        };
        if time < 0 || time >= spline.duration {
            return Err(4);
        }
        if spline.duration <= 0 || spline.points.is_empty() {
            return Err(-1);
        }
        let t = f64::from(time) / f64::from(spline.duration);
        let mut output = [0; 3];
        for axis in 0..3 {
            let values = spline
                .points
                .iter()
                .map(|point| f64::from(point[axis]))
                .collect::<Vec<_>>();
            output[axis] = sample_natural_cubic(&values, t) as i32;
        }
        Ok(output)
    }
}

fn sample_natural_cubic(values: &[f64], normalized: f64) -> f64 {
    match values {
        [] => 0.0,
        [value] => *value,
        [first, second] => first + (second - first) * normalized.clamp(0.0, 1.0),
        _ => {
            let segments = values.len() - 1;
            let scaled = normalized.clamp(0.0, 1.0) * segments as f64;
            let segment = (scaled.floor() as usize).min(segments - 1);
            let local = scaled - segment as f64;

            // The target CSpline builds a natural cubic spline independently
            // for x/y/z. Equal knot spacing lets us solve the tridiagonal
            // second-derivative system with this compact Thomas pass.
            let n = values.len();
            let mut lower = vec![0.0; n];
            let mut diagonal = vec![1.0; n];
            let mut upper = vec![0.0; n];
            let mut rhs = vec![0.0; n];
            for index in 1..n - 1 {
                lower[index] = 1.0;
                diagonal[index] = 4.0;
                upper[index] = 1.0;
                rhs[index] = 6.0 * (values[index + 1] - 2.0 * values[index] + values[index - 1]);
            }
            for index in 1..n {
                let factor = lower[index] / diagonal[index - 1];
                diagonal[index] -= factor * upper[index - 1];
                rhs[index] -= factor * rhs[index - 1];
            }
            let mut second = vec![0.0; n];
            for index in (0..n).rev() {
                second[index] = if index + 1 < n {
                    (rhs[index] - upper[index] * second[index + 1]) / diagonal[index]
                } else {
                    rhs[index] / diagonal[index]
                };
            }
            let a = 1.0 - local;
            let b = local;
            a * values[segment]
                + b * values[segment + 1]
                + ((a * a * a - a) * second[segment] + (b * b * b - b) * second[segment + 1]) / 6.0
        }
    }
}

impl RuntimeTraceApi {
    pub(super) fn tick_native_effect_state(&mut self, elapsed_ms: u64) {
        let mut state = std::mem::take(&mut self.native_effect);
        let now = self.engine_time_ms as u32;
        let mut due = Vec::new();
        for (&handle, screen) in &mut state.particle_screens {
            screen.world.poll(now, &state.particle_banks, &mut |n| {
                self.native_particle_random(n)
            });
            let interval = screen.auto_update_interval_ms as u32;
            if interval != 0 && screen.auto_update_deadline <= now {
                if self.graph_object_is_drawable(handle) {
                    screen.commit();
                    due.push(handle);
                }
                if screen.auto_update_deadline == 0 {
                    screen.auto_update_deadline = now;
                }
                screen.auto_update_deadline = screen
                    .auto_update_deadline
                    .wrapping_add(interval.wrapping_mul(
                        now.wrapping_sub(screen.auto_update_deadline) / interval + 1,
                    ));
            }
        }
        for handle in due {
            self.render_native_particle_screen(&mut state, handle, now, false);
        }
        self.native_effect = state;
        self.effects.tick(elapsed_ms);
    }

    fn dispatch_particle_native(
        &mut self,
        id: u16,
        stack: &mut Vec<ethornell_vm::Value>,
    ) -> ethornell_vm::VmResult<ethornell_vm::Value> {
        use ethornell_vm::{Value, VmError};
        let count = match id {
            0 => 2,
            1 | 8 | 15 | 31 => 1,
            4 | 9 | 10 | 12 | 13 | 36 | 44 => 2,
            5 => 6,
            11 => 10,
            16 => 12,
            24 => 7,
            26 => 8,
            27 => 3,
            32 | 40 => 4,
            37 => 11,
            41 => 16,
            45 => 18,
            _ => {
                return Err(VmError::Runtime(format!(
                    "unregistered particle syscall C0:{id:02X}"
                )));
            }
        };
        let mut a = pop_args(stack, count)
            .iter()
            .map(value_to_i32)
            .collect::<Vec<_>>();
        a.reverse();
        let mut state = std::mem::take(&mut self.native_effect);
        // Target texts of sub_475F20..sub_476ED0.
        let fail = |text: String| VmError::Runtime(text);
        let handle_error = || VmError::Runtime(PARTICLE_SCREEN_HANDLE.into());
        let now = self.engine_time_ms as u32;
        let result = (|| -> ethornell_vm::VmResult<Value> {
            let mut render = None;
            match id {
                // sub_4414F0: the eight slots before the size.
                0 => {
                    if state.particle_screens.len() >= 8 {
                        return Err(fail(
                            "これ以上、パーティクルスクリーンオブジェクトを生成することはできません".into(),
                        ));
                    }
                    if a[0] <= 0
                        || a[1] <= 0
                        || (a[0] as usize)
                            .checked_mul(a[1] as usize)
                            .is_none_or(|n| n > 16 * 1024 * 1024)
                    {
                        return Err(fail(format!(
                            "無効なスクリーンサイズ [ {} , {} ] が指定されました",
                            a[0], a[1]
                        )));
                    }
                    let handle = state.allocate_particle_screen(a[0], a[1]).ok_or_else(|| {
                        fail("これ以上、パーティクルスクリーンオブジェクトを生成することはできません".into())
                    })?;
                    state.particle_screens.get_mut(&handle).unwrap().world.clock = now;
                    self.display_tree.register_inferred(handle);
                    self.graph_object_enabled.insert(handle, true);
                    let p = self.graph_object_properties.entry(handle).or_default();
                    p.native.sort_class = 4;
                    p.native.draw_enabled = 1;
                    self.graph90_refresh_native_sort_key(handle);
                    self.render_native_particle_screen(&mut state, handle, now, false);
                    return Ok(Value::Int(handle));
                }
                1 => {
                    if !state.release_particle_screen(a[0]) {
                        return Err(handle_error());
                    }
                    self.remove_graph_object(a[0]);
                }
                4 => {
                    let s = state
                        .particle_screens
                        .get_mut(&a[0])
                        .ok_or_else(handle_error)?;
                    s.enabled = a[1] != 0;
                    self.set_graph_object_enabled(a[0], s.enabled);
                }
                5 => {
                    target_checks::priority(a[5])?;
                    target_checks::effect_level(a[4])?;
                    target_checks::effect_mode(a[3])?;
                    let s = state
                        .particle_screens
                        .get_mut(&a[0])
                        .ok_or_else(handle_error)?;
                    s.display = [a[1], a[2], a[5], a[3], a[4]];
                    let p = self.graph_object_properties.entry(a[0]).or_default();
                    p.blend_mode = a[3];
                    p.native.blend_mode = a[3];
                    p.set_alpha_parameter(a[4]);
                    p.native.priority = a[5] as u32;
                    self.graph90_set_position_recursive(a[0], a[1], a[2]);
                    self.display_tree.set_chain_depth(a[0], a[5]);
                    self.graph90_refresh_native_sort_key(a[0]);
                    render = Some((a[0], false));
                }
                8 => {
                    let s = state
                        .particle_screens
                        .get_mut(&a[0])
                        .ok_or_else(handle_error)?;
                    s.commit();
                    render = Some((a[0], false));
                }
                9 => {
                    let s = state
                        .particle_screens
                        .get_mut(&a[0])
                        .ok_or_else(handle_error)?;
                    if s.auto_update_interval_ms == 0 && a[1] != 0 {
                        s.auto_update_deadline = now;
                    }
                    s.auto_update_interval_ms = a[1];
                }
                10 => {
                    let s = state
                        .particle_screens
                        .get_mut(&a[0])
                        .ok_or_else(handle_error)?;
                    s.native_field_340 = a[1];
                }
                // sub_4418A0: handle, then sub_424D90 refuses the scale.
                11 => {
                    let s = state
                        .particle_screens
                        .get_mut(&a[0])
                        .ok_or_else(handle_error)?;
                    if a[7] <= 0 {
                        return Err(fail(format!("無効な縮尺 [ {} ] が指定されました", a[7])));
                    }
                    s.emitter_layout = a[1..].to_vec();
                    s.world.camera = [a[1] >> 8, a[2] >> 8, a[3] >> 8];
                    s.world.rotation =
                        [a[4], a[5], a[6]].map(|n| n as f64 * std::f64::consts::PI / 11796480.0);
                    s.world.focal = a[7];
                    s.world.center = [a[8], a[9]];
                }
                // sub_441900: handle, then the interval.
                12 => {
                    let s = state
                        .particle_screens
                        .get_mut(&a[0])
                        .ok_or_else(handle_error)?;
                    if a[1] as u32 > 100 {
                        return Err(fail(format!("無効な処理間隔 [ {} ] が指定されました", a[1])));
                    }
                    s.emission_percent = a[1];
                    s.world.interval = a[1] as u32;
                    s.world.retime_environment();
                }
                13 => {
                    let s = state
                        .particle_screens
                        .get_mut(&a[0])
                        .ok_or_else(handle_error)?;
                    let mut rng = |n| self.native_particle_random(n);
                    s.world
                        .warm_up(a[1] as u32, now, &state.particle_banks, &mut rng);
                    s.advance_count = s.advance_count.wrapping_add(a[1] as u32 as u64);
                }
                15 => {
                    let s = state
                        .particle_screens
                        .get_mut(&a[0])
                        .ok_or_else(handle_error)?;
                    s.world.reset();
                    s.commit_count = 0;
                    s.advance_count = 0;
                }
                16 => {
                    let s = state
                        .particle_screens
                        .get_mut(&a[0])
                        .ok_or_else(handle_error)?;
                    s.emitter_configuration = a[1..].to_vec();
                    s.world
                        .configure_environment(&a, &mut |n| self.native_particle_random(n));
                }
                24 | 26 | 36 | 44 => {
                    let (kind, bank, frames, first, speed, mix) = if id == 36 || id == 44 {
                        (i32::from(id == 44), a[0], 1, a[1], [0, 0], None)
                    } else {
                        let numerator = (a[6] as u32).wrapping_shl(16);
                        let base = if a[4] != 0 {
                            numerator / (a[4] as u32)
                        } else {
                            0
                        };
                        let spread = match state.particle_interpolation_mode {
                            0 => {
                                if a[5] != 0 {
                                    numerator / (a[5] as u32)
                                } else {
                                    0
                                }
                            }
                            1 => {
                                if a[4] != 0 && a[5] != 0 {
                                    let d = a[4].wrapping_add(a[5]) as u32;
                                    if d != 0 {
                                        (numerator / d).wrapping_sub(base)
                                    } else {
                                        0
                                    }
                                } else {
                                    0
                                }
                            }
                            _ => {
                                if a[4] != 0 {
                                    (u64::from(base) * u64::from(a[5] as u32)
                                        / (a[4] as u32 as u64))
                                        as u32
                                } else {
                                    0
                                }
                            }
                        };
                        (
                            a[0],
                            a[1],
                            a[2],
                            a[3],
                            [base as i32, spread as i32],
                            (id == 26).then(|| a[7]),
                        )
                    };
                    // sub_491900 / sub_491A60: every bitmap exists (4), then
                    // the type (1), the pattern (2), at most 32 frames (6),
                    // one RGB/ARGB format and size (5).
                    if id == 26 {
                        target_checks::mixing_ratio(a[7])?;
                    }
                    let single = id == 36 || id == 44;
                    let range = |first: i32, frames: i32| {
                        format!("{first} ～{}", first.wrapping_add(frames).wrapping_sub(1))
                    };
                    let mut images = Vec::new();
                    let mut format = 0;
                    let mut shapes = Vec::new();
                    if frames != 0 && first != -1 {
                        for offset in 0..frames.clamp(0, 64) {
                            let resource = first.wrapping_add(offset);
                            let Some(image) = self.graph_bitmap_image(resource) else {
                                return Err(fail(format!(
                                    "指定されたビットマップ [ {first} ] は存在しません"
                                )));
                            };
                            let f = self.bitmap_formats.get(&resource).copied().unwrap_or(2);
                            shapes.push((image.width, image.height, f));
                            images.push(image);
                        }
                    }
                    if kind as u32 >= 2 || id == 26 && kind != 1 {
                        return Err(fail(format!("無効なパーティクルタイプ [ {kind} ] が指定されました")));
                    }
                    if bank as u32 >= 64 {
                        return Err(fail(if single {
                            format!("無効なパターン番号 [ {bank} ] が指定されました")
                        } else {
                            format!("無効なオブジェクトパターン [ {bank} ] が指定されました")
                        }));
                    }
                    if frames as u32 > 32 {
                        return Err(fail(format!("無効なビットマップ数 [ {frames} ] が指定されました")));
                    }
                    if let Some(&shape) = shapes.first() {
                        if !matches!(shape.2, 1 | 2) || shapes.iter().any(|other| *other != shape) {
                            return Err(fail(if single {
                                format!("指定されたビットマップ [ {first} ] はパーティクルには使用できません")
                            } else {
                                format!(
                                    "指定されたビットマップ [ {} ] の中に適合しないものが含まれています",
                                    range(first, frames)
                                )
                            }));
                        }
                        format = shape.2;
                    }
                    let animation = native_particle::ParticleBank::new(images, format, speed);
                    if let Some(mix) = mix {
                        state.particle_banks.secondary.insert(bank, animation);
                        state.particle_banks.mix.insert(bank, mix);
                        for screen in state.particle_screens.values_mut() {
                            screen
                                .world
                                .rebind_secondary(bank, &state.particle_banks, &mut |n| {
                                    self.native_particle_random(n)
                                });
                        }
                    } else {
                        state.particle_banks.primary.insert((kind, bank), animation);
                    }
                }
                27 => {
                    // sub_4767B0: mixing ratio, then sub_491BC0.
                    target_checks::mixing_ratio(a[2])?;
                    if a[0] != 1 {
                        return Err(fail(format!("無効なパーティクルタイプ [ {} ] が指定されました", a[0])));
                    }
                    if a[1] as u32 >= 64 {
                        return Err(fail(format!("無効なオブジェクトパターン [ {} ] が指定されました", a[1])));
                    }
                    if !state
                        .particle_banks
                        .secondary
                        .get(&a[1])
                        .is_some_and(|b| !b.frames.is_empty())
                    {
                        return Err(fail(format!(
                            "パーティクルタイプ [ {} ] のオブジェクトパターン [ {} ] には別イメージが設定されていません",
                            a[0], a[1]
                        )));
                    }
                    state.particle_banks.mix.insert(a[1], a[2]);
                }
                31 => {
                    if a[0] as u32 > 2 {
                        return Err(fail(format!("無効な制御モード [ {} ] が指定されました", a[0])));
                    }
                    state.particle_interpolation_mode = a[0];
                }
                32 | 40 => {
                    let s = state
                        .particle_screens
                        .get_mut(&a[0])
                        .ok_or_else(handle_error)?;
                    // sub_424E30 / sub_424E50: pattern (10), then count (6).
                    if a[1] as u32 >= 64 {
                        return Err(fail(format!("無効なパターン番号 [ {} ] が指定されました", a[1])));
                    }
                    if !s.world.configure_count(
                        usize::from(id == 40),
                        a[1] as usize,
                        a[2],
                        a[3] as u32,
                    ) {
                        return Err(fail(format!("無効な最大オブジェクト数 [ {} ] が指定されました", a[2])));
                    }
                }
                37 => {
                    if a[0] as u32 >= 64 {
                        return Err(fail(format!("無効なパターン番号 [ {} ] が指定されました", a[0])));
                    }
                    let mut style = [0i32; 11];
                    style[0] = 1;
                    for (i, value) in a[1..10].iter().enumerate() {
                        style[i + 1] = if matches!(i, 0 | 2 | 4 | 6 | 8) {
                            value.wrapping_abs() >> 8
                        } else {
                            *value >> 8
                        };
                    }
                    style[10] = a[10] >> 8;
                    state.particle_banks.snow.insert(a[0], style);
                }
                41 => {
                    let s = state
                        .particle_screens
                        .get_mut(&a[0])
                        .ok_or_else(handle_error)?;
                    if a[1] as u32 >= 64 {
                        return Err(fail(format!("無効なパターン番号 [ {} ] が指定されました", a[1])));
                    }
                    let e = &mut s.world.emitter[a[1] as usize];
                    e.copy_from_slice(&a[2..16]);
                    for i in [0usize, 2, 12] {
                        e[i] >>= 8;
                    }
                    for i in [1usize, 4, 5, 13] {
                        e[i] = e[i].wrapping_abs() >> 8;
                    }
                    for i in [6usize, 7, 8] {
                        e[i] = e[i].wrapping_abs();
                    }
                }
                45 => {
                    if a[0] as u32 >= 64 {
                        return Err(fail(format!("無効なパターン番号 [ {} ] が指定されました", a[0])));
                    }
                    let mut s = [0i32; 18];
                    s[0] = 1;
                    s[1] = a[1];
                    s[2] = a[2];
                    for i in 3..12 {
                        s[i] = if matches!(i, 3 | 5 | 7 | 9 | 11) {
                            a[i].wrapping_abs() >> 8
                        } else {
                            a[i] >> 8
                        };
                    }
                    s[12] = a[12];
                    s[13] = a[13];
                    s[14] = a[14] >> 8;
                    s[15] = if a[15] == 0 { 1 } else { a[15] };
                    s[16] = if a[16] == 0 { 1 } else { a[16] };
                    s[17] = a[17];
                    state.particle_banks.firefly.insert(a[0], s);
                }
                _ => unreachable!(),
            }
            if let Some((handle, poll)) = render {
                self.render_native_particle_screen(&mut state, handle, now, poll);
            }
            Ok(Value::None)
        })();
        self.native_effect = state;
        result
    }

    fn native_particle_random(&mut self, n: i32) -> i32 {
        let value =
            ((self.next_native_crt_rand() as u32) << 15) | self.next_native_crt_rand() as u32;
        let divisor = n.wrapping_add(1).unsigned_abs();
        let r = if divisor == 0 {
            0
        } else {
            (value % divisor) as i32
        };
        if n < 0 { r.wrapping_neg() } else { r }
    }

    pub(super) fn render_native_particle_screen(
        &mut self,
        state: &mut NativeEffectState,
        handle: i32,
        now: u32,
        poll: bool,
    ) {
        let Some(screen) = state.particle_screens.get_mut(&handle) else {
            return;
        };
        if poll {
            screen.world.poll(now, &state.particle_banks, &mut |n| {
                self.native_particle_random(n)
            });
        }
        if let Some((x, y)) = self.graph_native_base_position(handle) {
            screen.display[0] = x;
            screen.display[1] = y;
        }
        if let Some(p) = self.graph_object_properties.get(&handle) {
            screen.display[2] = p.native.priority as i32;
        }
        let images = screen.world.raster(
            screen.width as u32,
            screen.height as u32,
            &screen.frame_tables[0],
            &state.particle_banks,
        );
        while screen.layers.len() > images.len() {
            let layer = screen.layers.pop().unwrap();
            self.graph_layers.remove(&layer);
            if let Some(layers) = self.graph_object_layers.get_mut(&handle) {
                layers.remove(&layer);
            }
        }
        for (index, image) in images.into_iter().enumerate() {
            let layer = if let Some(layer) = screen.layers.get(index) {
                *layer
            } else {
                let id = self.alloc_node();
                screen.layers.push(id);
                id
            };
            let key = format!("runtime:particle:{handle}:{index}");
            let (width, height) = (image.width as f32, image.height as f32);
            self.store_graph_image(key.clone(), image);
            let priority = screen.display[2]
                .wrapping_add(screen.frame_tables[1].get(index).copied().unwrap_or(0));
            self.graph_layers.insert(
                layer,
                RuntimeGraphLayer {
                    hit_id: handle,
                    owner_object: Some(handle),
                    key,
                    target_surface: None,
                    x: screen.display[0] as f32,
                    y: screen.display[1] as f32,
                    width,
                    height,
                    src_x: 0.0,
                    src_y: 0.0,
                    opacity: 1.0,
                    z: priority,
                    enabled: true,
                    transform_x: 0.0,
                    transform_y: 0.0,
                    transform_z: 0,
                    scale_x: 1.0,
                    scale_y: 1.0,
                    rotation_degrees: 0.0,
                    clip: None,
                },
            );
            self.graph_object_layers
                .entry(handle)
                .or_default()
                .insert(layer);
        }
        self.graph_redraw_requested = Some(false);
    }

    pub(super) fn dispatch_native_effect(
        &mut self,
        call: &mut ethornell_vm::NativeCallFrame,
    ) -> Option<ethornell_vm::VmResult<ethornell_vm::Value>> {
        let (group, id) = (call.group(), call.id());
        let stack = call.args_mut();
        if group != 0xC0 {
            return None;
        }

        let value = match id {
            0x00..=0x2D => return Some(self.dispatch_particle_native(id, stack)),
            0x40..=0x4F => {
                let count = match id {
                    0x40 => 2,
                    0x41 => 1,
                    0x42..=0x44 | 0x47..=0x4B | 0x4E | 0x4F => 2,
                    0x45 => 6,
                    0x46 => 7,
                    0x4C | 0x4D => 4,
                    _ => unreachable!(),
                };
                let args = pop_args(stack, count);
                if let Err(error) = self.check_rain_call(id, &args) {
                    return Some(Err(error));
                }
                let result = match self.effects.call(group, id, &args) {
                    crate::effects::EffectCall::Handled(result) => result,
                    crate::effects::EffectCall::Unhandled => None,
                };
                self.sync_rain_screen_object(id, &args, result);
                result
                    .map(ethornell_vm::Value::Int)
                    .unwrap_or(ethornell_vm::Value::None)
            }
            0xC0 => ethornell_vm::Value::Int(self.native_effect.create_spline()),
            0xC1 => {
                let handle = pop_int_value(stack).unwrap_or_default();
                ethornell_vm::Value::Int(self.native_effect.release_spline(handle))
            }
            // C2/C3 are VM-owned because their target contracts dereference
            // caller memory. Reaching the host path indicates malformed routing.
            0xC2 => {
                let _ = pop_args(stack, 4);
                ethornell_vm::Value::Int(1)
            }
            0xC3 => {
                let _ = pop_args(stack, 3);
                ethornell_vm::Value::Int(4)
            }
            // F0 is VM-owned so it can write the BWEF table and count directly.
            0xF0 => {
                let _ = pop_args(stack, 5);
                ethornell_vm::Value::Int(i32::MIN + 1)
            }
            _ => return None,
        };
        Some(Ok(value))
    }

    /// sub_477090..sub_4776F0 and their cores, in target order. `popped`
    /// is in pop order (last source argument first).
    fn check_rain_call(&mut self, id: u16, popped: &[ethornell_vm::Value]) -> ethornell_vm::VmResult<()> {
        use ethornell_vm::VmError;
        let mut a = popped.iter().map(value_to_i32).collect::<Vec<_>>();
        a.reverse();
        let fail = |text: String| Err(VmError::Runtime(text));
        let handle_error = || Err(VmError::Runtime("無効なレインスクリーンハンドルが指定されました".into()));
        match id {
            // sub_441C70: eight screens, then the size (sub_4253D0).
            0x40 => {
                if self.effects.rain_count() >= 8 {
                    return fail("これ以上、レインスクリーンオブジェクトを生成することはできません".into());
                }
                if a[0] <= 0
                    || a[1] <= 0
                    || (a[0] as usize).checked_mul(a[1] as usize).is_none_or(|n| n > 16 * 1024 * 1024)
                {
                    return fail(format!("無効なスクリーンサイズ [ {} , {} ] が指定されました", a[0], a[1]));
                }
            }
            // sub_4259A0: -1 clears; else a screen-sized format-3 bitmap.
            0x43 => {
                if !self.effects.rain_exists(a[0]) {
                    return handle_error();
                }
                if a[1] != -1 {
                    let Some(info) = ethornell_vm::GraphApi::query_bitmap_info(self, a[1]) else {
                        return fail(format!(
                            "指定されたマスキング用ビットマップ [ {} ] は存在しません",
                            a[1]
                        ));
                    };
                    let (width, height) = snapshot::runtime_frame_size(self);
                    if info.format != 3 || info.width != width || info.height != height {
                        return fail(format!(
                            "指定されたマスキング用ビットマップ [ {} ] はグレイスケールではない、或いはレインスクリーンとサイズが一致しません",
                            a[1]
                        ));
                    }
                }
            }
            0x45 => {
                crate::target_checks::priority(a[5])?;
                crate::target_checks::effect_level(a[4])?;
                crate::target_checks::effect_mode(a[3])?;
                if !self.effects.rain_exists(a[0]) {
                    return handle_error();
                }
            }
            // sub_425550 / sub_4255A0 / sub_425690 / sub_425780: zero refused.
            0x47 | 0x48 | 0x4B | 0x4E => {
                if !self.effects.rain_exists(a[0]) {
                    return handle_error();
                }
                if a[1] == 0 {
                    let what = match id {
                        0x47 => "ステージ当たりの落下距離",
                        0x48 => "雨の長さ",
                        0x4B => "ステージ当たりの時間",
                        _ => "プロジェクション値",
                    };
                    return fail(format!("無効な{what} [ {} ] が指定されました", a[1]));
                }
            }
            // sub_492D80: 1..=1000.
            0x4F => {
                if (a[1] as u32).wrapping_sub(1) > 999 {
                    return fail(format!("無効なフレームレート [ {} ] が指定されました", a[1]));
                }
            }
            0x41 | 0x42 | 0x44 | 0x46 | 0x49 | 0x4A | 0x4C | 0x4D => {
                if !self.effects.rain_exists(a[0]) {
                    return handle_error();
                }
            }
            _ => {}
        }
        Ok(())
    }

    /// A rain screen (UserC0:40, tag 0xC1) is a display object like a
    /// particle screen (UserC0:00): scripts fade it with Graph90:22 and
    /// reorder it with Graph90:3A, so it is registered with the display tree
    /// and its enabled state, depth, blend mode and alpha live in the object
    /// state those calls act on. `popped` is in reverse-pop order, so the
    /// handle (the first script argument) is last.
    fn sync_rain_screen_object(
        &mut self,
        id: u16,
        popped: &[ethornell_vm::Value],
        created: Option<i32>,
    ) {
        let handle = popped.last().map(value_to_i32).unwrap_or_default();
        match id {
            0x40 => {
                if let Some(handle) = created.filter(|handle| *handle != 0) {
                    self.display_tree.register_inferred(handle);
                    self.graph_object_enabled.insert(handle, true);
                    self.graph_object_properties.entry(handle).or_default();
                }
            }
            _ if NativeDisplayKind::from_handle(handle) != NativeDisplayKind::RainScreen
                || !self.display_tree.contains(handle) => {}
            0x41 => {
                self.remove_graph_object(handle);
            }
            0x44 => {
                let enabled = popped.first().map(value_to_i32).unwrap_or_default() != 0;
                self.set_graph_object_enabled(handle, enabled);
            }
            0x45 => {
                let values = popped.iter().rev().map(value_to_i32).collect::<Vec<_>>();
                if let [_, _, _, blend, alpha, z] = values.as_slice() {
                    let properties = self.graph_object_properties.entry(handle).or_default();
                    properties.blend_mode = *blend;
                    properties.set_alpha_parameter(*alpha);
                    self.display_tree.set_chain_depth(handle, *z);
                }
            }
            _ => {}
        }
    }
}

pub(super) fn is_native_blend_mode(value: i32) -> bool {
    matches!(
        value,
        0x00..=0x09 | 0x20..=0x27 | 0x40 | 0x41 | 0x80 | 0xC0 | 0xC1 | 0xF0 | 0xFF
    )
}

#[cfg(test)]
mod tests {
    use super::{NativeEffectState, PARTICLE_HANDLE_BASE, sample_natural_cubic};

    #[test]
    fn snow_syscalls_copy_frames_advance_and_render_depth_planes() {
        use crate::*;
        use ethornell_vm::SoundApi;
        let manager = ResourceManager::open_game(env!("CARGO_MANIFEST_DIR")).unwrap();
        let mut api = RuntimeTraceApi::new(manager);
        let call = |api: &mut RuntimeTraceApi, id, args: &[i32]| {
            api.dispatch_particle_native(
                id,
                &mut args.iter().copied().map(ethornell_vm::Value::Int).collect(),
            )
            .unwrap()
        };
        api.store_graph_image(
            "test:snow".into(),
            DecodedImage {
                width: 8,
                height: 8,
                rgba: vec![255; 256],
            },
        );
        api.graph_resources
            .insert(1, RuntimeGraphResource::whole("test:snow".into()));
        api.bitmap_formats.insert(1, 2);
        let handle = call(&mut api, 0, &[64, 64]).as_i32();
        assert_eq!(
            SoundApi::configure_particle_frame_tables(&mut api, handle, 2, &[1, 32767], &[0, 1]),
            0
        );
        call(&mut api, 0x0B, &[handle, 0, 0, 0, 0, 0, 0, 256, 32, 32]);
        call(&mut api, 0x24, &[0, 1]);
        call(&mut api, 0x25, &[0, 0, 0, 0, 65536, 0, 0, 0, 0, 0, 0]);
        call(&mut api, 0x20, &[handle, 0, 1, 0]);
        // Bank owns the copied native image, including after source release.
        api.graph_resources.remove(&1);
        api.tick_native_effect_state(0);
        call(&mut api, 8, &[handle]);
        let key = format!("runtime:particle:{handle}:0");
        assert_eq!(api.graph_images[&key].rgba[(28 * 64 + 28) * 4 + 3], 255);
        api.engine_time_ms = 20;
        api.tick_native_effect_state(20);
        call(&mut api, 8, &[handle]);
        assert_eq!(api.graph_images[&key].rgba[(28 * 64 + 28) * 4 + 3], 0);
        assert_eq!(api.graph_images[&key].rgba[(28 * 64 + 30) * 4 + 3], 255);
        assert_eq!(api.graph_object_layers[&handle].len(), 2);
        call(&mut api, 0x0F, &[handle]);
        api.tick_native_effect_state(0);
        call(&mut api, 8, &[handle]);
        // Reset clears live particles; requested emitter counts persist and
        // cause the next commit to emit a fresh particle at its initial point.
        assert_eq!(api.graph_images[&key].rgba[(28 * 64 + 28) * 4 + 3], 255);
    }

    #[test]
    fn particle_screen_uses_target_tag_and_eight_slots() {
        let mut state = NativeEffectState::default();
        for slot in 0..8 {
            assert_eq!(
                state.allocate_particle_screen(1280, 720),
                Some(PARTICLE_HANDLE_BASE | slot)
            );
        }
        assert_eq!(state.allocate_particle_screen(1, 1), None);
        assert!(state.release_particle_screen(PARTICLE_HANDLE_BASE | 3));
        assert_eq!(
            state.allocate_particle_screen(640, 480),
            Some(PARTICLE_HANDLE_BASE | 3)
        );
    }

    #[test]
    fn spline_matches_target_status_and_natural_cubic_endpoints() {
        let mut state = NativeEffectState::default();
        let handle = state.create_spline();
        assert_eq!(
            state.configure_spline(handle, 1, &[[0, 0, 0], [1, 1, 1]]),
            3
        );
        assert_eq!(state.configure_spline(99, 10, &[[0, 0, 0], [1, 1, 1]]), 1);
        assert_eq!(
            state.configure_spline(handle, 10, &[[0, 0, 0], [10, 20, 30], [20, 40, 60]]),
            0
        );
        assert_eq!(state.sample_spline(handle, 0), Ok([0, 0, 0]));
        assert_eq!(state.sample_spline(handle, 10), Err(4));
        assert!((sample_natural_cubic(&[0.0, 10.0, 20.0], 0.5) - 10.0).abs() < 0.001);
        assert_eq!(state.release_spline(handle), 0);
        assert_eq!(state.release_spline(handle), 1);
    }
}

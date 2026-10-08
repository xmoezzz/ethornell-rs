use crate::graph::{RuntimeGraphLayer, RuntimeGraphObjectProperties, RuntimeSurface};
use crate::timing::duration_ms_to_ticks;
use ethornell_vm::Value;
use std::collections::{BTreeMap, BTreeSet};
use std::sync::{Mutex, OnceLock};

#[derive(Debug, Clone, Copy, PartialEq, Eq)]
pub(crate) struct ScheduledObjectControl {
    pub(crate) target_object: i32,
    pub(crate) target_x: i32,
    pub(crate) target_y: i32,
    pub(crate) position_curve: i32,
    pub(crate) target_alpha: i32,
    pub(crate) alpha_curve: i32,
    pub(crate) fixed_parameter_target: i32,
    pub(crate) update_denominator: i32,
    pub(crate) update_numerator: i32,
    pub(crate) input_descriptor: i32,
    pub(crate) input_enabled: bool,
    pub(crate) duration_ms: i32,
}

impl ScheduledObjectControl {
    // Native 0x90:28 (sub_47AC80) pops these fields in this order before
    // forwarding them to sub_491D60/sub_431D90/sub_431E80.
    pub(crate) fn from_popped_args(args: &[Value]) -> Option<Self> {
        let source = args.iter().rev().collect::<Vec<_>>();
        Some(Self {
            target_object: value_to_i32(source.first()?)?,
            target_x: value_to_i32(source.get(1)?)?,
            target_y: value_to_i32(source.get(2)?)?,
            position_curve: value_to_i32(source.get(3)?)?,
            target_alpha: value_to_i32(source.get(4)?)?,
            alpha_curve: value_to_i32(source.get(5)?)?,
            fixed_parameter_target: value_to_i32(source.get(6)?)?,
            duration_ms: value_to_i32(source.get(7)?)?.max(0),
            update_denominator: value_to_i32(source.get(8)?)?,
            update_numerator: value_to_i32(source.get(9)?)?,
            input_enabled: value_to_i32(source.get(10)?)? != 0,
            input_descriptor: value_to_i32(source.get(11)?)?,
        })
    }
}

#[derive(Debug, Clone, Copy, PartialEq, Eq)]
pub(crate) struct ScheduledSplineControl {
    pub(crate) target_object: i32,
    pub(crate) position_curve: i32,
    pub(crate) target_alpha: i32,
    pub(crate) alpha_curve: i32,
    pub(crate) spline_time_scale: u16,
    pub(crate) fixed_parameter_target: i32,
    pub(crate) duration_ms: i32,
    pub(crate) update_denominator: i32,
    pub(crate) update_numerator: i32,
    pub(crate) input_enabled: bool,
    pub(crate) input_descriptor: i32,
}

impl ScheduledSplineControl {
    // Native 0x90:29 (sub_47ADF0 -> sub_491E60 -> sub_432490) keeps
    // source-order arguments. Argument 5 packs the alpha curve in its low
    // 16 bits and the spline alpha-time scale in its high 16 bits; argument
    // 6 is the independent fixed-parameter target.
    pub(crate) fn from_source_args(args: &[Value]) -> Option<Self> {
        let packed_curve = value_to_i32(args.get(5)?)? as u32;
        Some(Self {
            target_object: value_to_i32(args.first()?)?,
            position_curve: value_to_i32(args.get(3)?)?,
            target_alpha: value_to_i32(args.get(4)?)?,
            alpha_curve: (packed_curve & 0xffff) as i32,
            spline_time_scale: (packed_curve >> 16) as u16,
            fixed_parameter_target: value_to_i32(args.get(6)?)?,
            duration_ms: value_to_i32(args.get(7)?)?.max(0),
            update_denominator: value_to_i32(args.get(8)?)?,
            update_numerator: value_to_i32(args.get(9)?)?,
            input_enabled: value_to_i32(args.get(10)?)? != 0,
            input_descriptor: value_to_i32(args.get(11)?)?,
        })
    }
}

#[derive(Debug, Default)]
pub(crate) struct GraphAnimationRegistry {
    records: BTreeMap<i32, GraphAnimationRecord>,
}

impl GraphAnimationRegistry {
    pub(crate) fn start(&mut self, handle: i32, duration: i32, args: &[Value]) {
        if handle <= 0 {
            return;
        }
        let duration_ticks = duration_ms_to_ticks(duration);
        self.records.insert(
            handle,
            GraphAnimationRecord {
                handle,
                duration_ticks,
                remaining_ticks: duration_ticks,
                active: duration_ticks > 0,
                released: false,
                args: args.iter().filter_map(value_to_i32).collect(),
            },
        );
    }

    pub(crate) fn cancel(&mut self, handle: i32) {
        if let Some(record) = self.records.get_mut(&handle) {
            record.remaining_ticks = 0;
            record.active = false;
        }
    }

    pub(crate) fn release(&mut self, handle: i32) {
        if let Some(record) = self.records.get_mut(&handle) {
            record.remaining_ticks = 0;
            record.active = false;
            record.released = true;
        }
    }

    pub(crate) fn evaluate(&mut self, args: &[Value]) -> Option<GraphAnimationSnapshot> {
        let values = args.iter().filter_map(value_to_i32).collect::<Vec<_>>();
        let handle = values
            .iter()
            .copied()
            .find(|value| self.records.contains_key(value))
            .or_else(|| values.iter().copied().find(|value| *value > 0))?;
        let record = self
            .records
            .entry(handle)
            .or_insert_with(|| GraphAnimationRecord::new(handle));
        // Evaluation is a status query. Native time advances from the engine
        // tick, not from how often a BP script polls the handle.
        Some(record.snapshot())
    }

    pub(crate) fn active_count(&self) -> usize {
        self.records.values().filter(|record| record.active).count()
    }

    pub(crate) fn tick(&mut self) -> Vec<i32> {
        let mut finished = Vec::new();
        for record in self.records.values_mut() {
            let was_active = record.active;
            if record.active && record.remaining_ticks > 0 {
                record.remaining_ticks -= 1;
            }
            if record.remaining_ticks == 0 {
                record.active = false;
            }
            if was_active && !record.active {
                finished.push(record.handle);
            }
        }
        finished
    }
}

#[derive(Debug, Clone)]
struct GraphAnimationRecord {
    handle: i32,
    duration_ticks: u32,
    remaining_ticks: u32,
    active: bool,
    released: bool,
    args: Vec<i32>,
}

impl GraphAnimationRecord {
    fn new(handle: i32) -> Self {
        Self {
            handle,
            duration_ticks: 1,
            remaining_ticks: 0,
            active: false,
            released: false,
            args: Vec::new(),
        }
    }

    fn snapshot(&self) -> GraphAnimationSnapshot {
        GraphAnimationSnapshot {
            handle: self.handle,
            duration_ticks: self.duration_ticks,
            remaining_ticks: self.remaining_ticks,
            active: self.active,
            released: self.released,
            args: self.args.clone(),
        }
    }
}

#[derive(Debug, Clone)]
pub(crate) struct GraphAnimationSnapshot {
    pub(crate) handle: i32,
    pub(crate) duration_ticks: u32,
    pub(crate) remaining_ticks: u32,
    pub(crate) active: bool,
    pub(crate) released: bool,
    pub(crate) args: Vec<i32>,
}

#[derive(Debug, Clone, Copy, PartialEq, Eq)]
pub(crate) struct NativeControlCompletion {
    /// First deferred value written by CProcCtrlDspObj::Tick (0x431F00).
    /// The target stores it as procedure_update_count * 1000 / duration_or_steps.
    pub(crate) progress_per_mille: i32,
    /// Second deferred value: 0 natural, 1 input/procedure-local forced end,
    /// -1 base CProcedure cancellation/abnormal termination.
    pub(crate) status: i32,
}

impl NativeControlCompletion {
    const NATURAL: Self = Self {
        progress_per_mille: 1000,
        status: 0,
    };
}

#[derive(Debug, Default)]
pub(crate) struct LayerAnimationSystem {
    tracks: Vec<LayerAnimation>,
    object_alpha_tracks: Vec<ObjectAlphaAnimation>,
    native_object_controls: Vec<NativeObjectControlAnimation>,
    native_special_path_controls: Vec<NativeSpecialPathControlAnimation>,
    native_spline_controls: Vec<NativeSplineObjectControlAnimation>,
    native_shake_object_controls: Vec<NativeShakeObjectControlAnimation>,
    native_control_completions: BTreeMap<u64, NativeControlCompletion>,
    next_native_control_id: u64,
    spline_tracks: Vec<SplineVectorAnimation>,
}

impl LayerAnimationSystem {
    pub(crate) fn fade_to(&mut self, layer_id: i32, from: f32, to: f32, duration_ticks: u32) {
        self.fade_to_eased(layer_id, from, to, duration_ticks, 0);
    }

    pub(crate) fn fade_to_eased(
        &mut self,
        layer_id: i32,
        from: f32,
        to: f32,
        duration_ticks: u32,
        curve: i32,
    ) {
        self.animate(
            layer_id,
            LayerAnimationProperty::Opacity,
            from,
            to,
            duration_ticks,
            curve,
        );
    }

    pub(crate) fn move_x_to(&mut self, layer_id: i32, from: f32, to: f32, duration_ticks: u32) {
        self.animate(
            layer_id,
            LayerAnimationProperty::TransformX,
            from,
            to,
            duration_ticks,
            0,
        );
    }

    pub(crate) fn move_y_to(&mut self, layer_id: i32, from: f32, to: f32, duration_ticks: u32) {
        self.animate(
            layer_id,
            LayerAnimationProperty::TransformY,
            from,
            to,
            duration_ticks,
            0,
        );
    }

    pub(crate) fn scale_x_to(&mut self, layer_id: i32, from: f32, to: f32, duration_ticks: u32) {
        self.animate(
            layer_id,
            LayerAnimationProperty::ScaleX,
            from,
            to,
            duration_ticks,
            0,
        );
    }

    pub(crate) fn scale_y_to(&mut self, layer_id: i32, from: f32, to: f32, duration_ticks: u32) {
        self.animate(
            layer_id,
            LayerAnimationProperty::ScaleY,
            from,
            to,
            duration_ticks,
            0,
        );
    }

    pub(crate) fn base_vector_to(
        &mut self,
        layer_id: i32,
        from: (f32, f32, i32),
        to: (f32, f32, i32),
        duration_ticks: u32,
        position_curve: i32,
        z_curve: i32,
    ) {
        self.animate(
            layer_id,
            LayerAnimationProperty::BaseX,
            from.0,
            to.0,
            duration_ticks,
            position_curve,
        );
        self.animate(
            layer_id,
            LayerAnimationProperty::BaseY,
            from.1,
            to.1,
            duration_ticks,
            position_curve,
        );
        self.animate(
            layer_id,
            LayerAnimationProperty::BaseZ,
            from.2 as f32,
            to.2 as f32,
            duration_ticks,
            z_curve,
        );
    }

    pub(crate) fn spline_vector_to(
        &mut self,
        layer_id: i32,
        from: (f32, f32, f32),
        points: &[[i32; 4]],
        duration_ticks: u32,
        curve: i32,
        time_scale: u16,
    ) {
        if duration_ticks == 0 || points.is_empty() {
            return;
        }
        if animation_fidelity() == AnimationFidelity::TargetStrict {
            report_unrecovered_curve_once(curve, "spline");
            if time_scale != 0 && std::env::var_os("TRACE_REVERSE_GAPS").is_some() {
                tracing::debug!(
                    layer_id,
                    time_scale,
                    point_count = points.len(),
                    "target CSpline time-scale field retained without invented coefficient formula"
                );
            }
        }
        self.spline_tracks
            .retain(|track| track.layer_id != layer_id);
        let mut x = Vec::with_capacity(points.len() + 1);
        let mut y = Vec::with_capacity(points.len() + 1);
        let mut z = Vec::with_capacity(points.len() + 1);
        x.push(from.0);
        y.push(from.1);
        z.push(from.2);
        for point in points {
            x.push(motion_coord(point[0]));
            y.push(motion_coord(point[1]));
            z.push(motion_coord(point[2]));
        }
        self.spline_tracks.push(SplineVectorAnimation {
            layer_id,
            x: SplineSampler::from_control_points(x),
            y: SplineSampler::from_control_points(y),
            z: SplineSampler::from_control_points(z),
            duration_ticks,
            elapsed_ticks: 0,
            curve,
            time_scale,
        });
    }

    fn animate(
        &mut self,
        layer_id: i32,
        property: LayerAnimationProperty,
        from: f32,
        to: f32,
        duration_ticks: u32,
        curve: i32,
    ) {
        if duration_ticks == 0 {
            return;
        }
        self.tracks
            .retain(|track| !(track.layer_id == layer_id && track.property == property));
        self.tracks.push(LayerAnimation {
            layer_id,
            property,
            from,
            to,
            duration_ticks,
            elapsed_ticks: 0,
            curve,
        });
    }

    pub(crate) fn animate_object_alpha(
        &mut self,
        object_id: i32,
        from: i32,
        to: i32,
        duration_ticks: u32,
        curve: i32,
    ) {
        self.object_alpha_tracks
            .retain(|track| track.object_id != object_id);
        if duration_ticks == 0 {
            return;
        }
        self.object_alpha_tracks.push(ObjectAlphaAnimation {
            object_id,
            from: from.clamp(0, 256) as f32,
            to: to.clamp(0, 256) as f32,
            duration_ticks,
            elapsed_ticks: 0,
            curve,
        });
    }

    /// Schedule the target CProcCtrlDspObj core used by Graph90:20-23.
    ///
    /// Target sub_431D90 captures the object's current position/alpha and
    /// sub_432160 samples `sub_498720() - start_time` in milliseconds.  Keep
    /// this separate from compatibility layer animations whose callers still
    /// express duration in legacy frame counts.
    #[allow(clippy::too_many_arguments)]
    pub(crate) fn schedule_native_object_control(
        &mut self,
        object_id: i32,
        from_position: (i32, i32),
        to_position: (i32, i32),
        position_curve: i32,
        from_alpha: i32,
        to_alpha: i32,
        alpha_curve: i32,
        from_fixed_parameter_16_16: i32,
        to_fixed_parameter_16_16: i32,
        duration_ms: i32,
        update_denominator: i32,
        update_numerator: i32,
        input_enabled: bool,
        input_descriptor: i32,
    ) -> u64 {
        self.object_alpha_tracks
            .retain(|track| track.object_id != object_id);
        let control_id = self.allocate_native_control_id();
        let duration_ms = if duration_ms == 0 { 1 } else { duration_ms };
        if duration_ms < 0 {
            return control_id;
        }
        let update_interval_ms = if update_numerator != 0 && update_denominator != 0 {
            1000i64
                .saturating_mul(i64::from(update_numerator))
                .checked_div(i64::from(update_denominator))
                .unwrap_or(0)
                .clamp(0, i64::from(u32::MAX)) as u64
        } else {
            0
        };
        self.native_object_controls
            .push(NativeObjectControlAnimation {
                control_id,
                object_id,
                from_x: from_position.0,
                from_y: from_position.1,
                to_x: to_position.0,
                to_y: to_position.1,
                position_curve,
                from_alpha: from_alpha.clamp(0, 256),
                to_alpha: to_alpha.clamp(0, 256),
                alpha_curve,
                from_fixed_parameter_16_16,
                to_fixed_parameter_16_16,
                duration_ms: duration_ms as u64,
                wall_elapsed_ms: 0,
                update_numerator,
                update_interval_ms,
                next_update_ms: update_interval_ms,
                input_enabled,
                input_descriptor,
                force_complete: false,
                procedure_update_count: 0,
                last_x: from_position.0,
                last_y: from_position.1,
                last_alpha: from_alpha.clamp(0, 256),
                last_fixed_parameter_16_16: from_fixed_parameter_16_16,
            });
        control_id
    }

    #[allow(clippy::too_many_arguments)]
    pub(crate) fn schedule_native_spline_control(
        &mut self,
        object_id: i32,
        from_vector: (i32, i32, i32),
        points: &[[i32; 4]],
        position_curve: i32,
        from_alpha: i32,
        to_alpha: i32,
        alpha_curve: i32,
        from_fixed_parameter_16_16: i32,
        fixed_parameter_target: i32,
        duration_ms: i32,
        update_denominator: i32,
        update_numerator: i32,
        spline_time_scale: u16,
        input_enabled: bool,
        input_descriptor: i32,
    ) -> u64 {
        let control_id = self.allocate_native_control_id();
        let duration_ms = if duration_ms == 0 { 1 } else { duration_ms };
        if points.is_empty() || duration_ms < 0 {
            return control_id;
        }
        let mut x = Vec::with_capacity(points.len() + 1);
        let mut y = Vec::with_capacity(points.len() + 1);
        let mut z = Vec::with_capacity(points.len() + 1);
        x.push(from_vector.0);
        y.push(from_vector.1);
        z.push(from_vector.2);
        for point in points {
            x.push(point[0]);
            y.push(point[1]);
            z.push(point[2]);
        }
        let update_interval_ms = if update_numerator != 0 && update_denominator != 0 {
            1000i64
                .saturating_mul(i64::from(update_numerator))
                .checked_div(i64::from(update_denominator))
                .unwrap_or(0)
                .clamp(0, i64::from(u32::MAX)) as u64
        } else {
            0
        };
        let to_fixed_parameter_16_16 = if fixed_parameter_target < 0 {
            from_fixed_parameter_16_16
        } else {
            fixed_parameter_target.saturating_mul(0x1_0000)
        };
        self.native_spline_controls
            .push(NativeSplineObjectControlAnimation {
                control_id,
                object_id,
                x: NativeNaturalCubicSpline::new(x),
                y: NativeNaturalCubicSpline::new(y),
                z: NativeNaturalCubicSpline::new(z),
                final_vector: (
                    points.last().unwrap()[0],
                    points.last().unwrap()[1],
                    points.last().unwrap()[2],
                ),
                position_curve,
                from_alpha: from_alpha.clamp(0, 256),
                to_alpha: to_alpha.clamp(0, 256),
                alpha_curve,
                from_fixed_parameter_16_16,
                to_fixed_parameter_16_16,
                duration_ms: duration_ms.max(1) as u64,
                wall_elapsed_ms: 0,
                update_numerator,
                update_interval_ms,
                next_update_ms: update_interval_ms,
                spline_time_scale: if spline_time_scale == 0 {
                    0x1_0000u32
                } else {
                    u32::from(spline_time_scale)
                },
                input_enabled,
                input_descriptor,
                force_complete: false,
                procedure_update_count: 0,
                last_vector: (i32::MIN, i32::MIN, i32::MIN),
                last_alpha: i32::MIN,
                last_fixed_parameter_16_16: i32::MIN,
            });
        control_id
    }

    #[allow(clippy::too_many_arguments)]
    pub(crate) fn schedule_native_special_path_control(
        &mut self,
        object_id: i32,
        from_position: (i32, i32),
        middle_position: (i32, i32),
        to_position: (i32, i32),
        position_curve: i32,
        from_alpha: i32,
        to_alpha: i32,
        duration_ms: i32,
        sample_rate_hz: i32,
        max_catchup_steps: i32,
        input_enabled: bool,
        input_descriptor: i32,
    ) -> Option<u64> {
        if sample_rate_hz <= 0 || duration_ms < 0 {
            return None;
        }
        let sample_count = ((i64::from(duration_ms) * i64::from(sample_rate_hz)) / 1000)
            .max(1)
            .min(i64::from(u32::MAX)) as u32;
        let path = build_native_three_point_path(
            from_position,
            middle_position,
            to_position,
            sample_count.saturating_add(1),
        )?;
        let interval_ms = if sample_count <= 1 {
            duration_ms.max(0) as u64
        } else {
            (1000 / sample_rate_hz).max(0) as u64
        };
        let control_id = self.allocate_native_control_id();
        self.native_special_path_controls
            .push(NativeSpecialPathControlAnimation {
                control_id,
                object_id,
                path,
                position_curve,
                from_alpha: from_alpha.clamp(0, 256),
                to_alpha: to_alpha.clamp(0, 256),
                total_steps: sample_count,
                current_step: 0,
                interval_ms,
                accumulated_ms: 0,
                max_catchup_steps: max_catchup_steps.max(0) as u32,
                input_enabled,
                input_descriptor,
                force_complete: false,
                procedure_update_count: 0,
                last_x: i32::MIN,
                last_y: i32::MIN,
                last_alpha: i32::MIN,
            });
        Some(control_id)
    }

    #[allow(clippy::too_many_arguments)]
    pub(crate) fn schedule_native_shake_object_control(
        &mut self,
        object_id: i32,
        mode: i32,
        from_position: (i32, i32),
        amplitude: i32,
        frequency_hz: i32,
        cycles: i32,
        decay_percent: i32,
        sample_rate_hz: i32,
        input_enabled: bool,
        input_descriptor: i32,
    ) -> Option<u64> {
        if !(0..6).contains(&mode)
            || frequency_hz <= 0
            || cycles <= 0
            || sample_rate_hz <= 0
            || sample_rate_hz < frequency_hz
        {
            return None;
        }
        let interval_ms = 1000 / sample_rate_hz;
        if interval_ms <= 0 {
            // The target's following division would fault for a zero interval;
            // reject it here rather than silently inventing a waveform.
            return None;
        }
        let waveform_len = 1000 / (frequency_hz.saturating_mul(interval_ms));
        if waveform_len <= 0 {
            return None;
        }

        // sub_43C8A0 builds one amplitude value per cycle in 16.16, applying
        // (100-decay)/100 after each cycle with integer truncation.
        let mut envelope = Vec::with_capacity(cycles as usize);
        let mut amplitude_fixed = i64::from(amplitude) << 16;
        for _ in 0..cycles {
            envelope.push((amplitude_fixed >> 16) as i32);
            amplitude_fixed = i64::from(100 - decay_percent).saturating_mul(amplitude_fixed) / 100;
        }

        // The second table is the target's deterministic triangular phase
        // wave. Modes 4/5 start half a phase later; 0/2 invert its sign.
        let mut waveform = Vec::with_capacity(waveform_len as usize);
        let center = if mode < 4 { 0_i64 } else { 0x8000_i64 };
        let mut phase = center;
        let mut delta = i64::from(0x20000 / waveform_len);
        for _ in 0..waveform_len {
            phase += delta;
            if phase >= 0x10000 {
                delta = -delta;
                phase = 0x20000 - phase;
            }
            if phase <= 0 {
                delta = -delta;
                phase = -phase;
            }
            let value = if mode == 0 || mode == 2 {
                center - phase
            } else {
                phase - center
            };
            waveform.push(value as i32);
        }

        let control_id = self.allocate_native_control_id();
        self.native_shake_object_controls
            .push(NativeShakeObjectControlAnimation {
                control_id,
                object_id,
                mode,
                base_x: from_position.0,
                base_y: from_position.1,
                envelope,
                waveform,
                total_steps: (cycles as u32).saturating_mul(waveform_len as u32),
                current_step: 0,
                interval_ms: interval_ms as u64,
                accumulated_ms: 0,
                input_enabled,
                input_descriptor,
                force_complete: false,
                procedure_update_count: 0,
                last_x: i32::MIN,
                last_y: i32::MIN,
            });
        Some(control_id)
    }

    fn allocate_native_control_id(&mut self) -> u64 {
        self.next_native_control_id = self.next_native_control_id.wrapping_add(1).max(1);
        self.next_native_control_id
    }

    pub(crate) fn has_native_control(&self, control_id: u64) -> bool {
        self.native_object_controls
            .iter()
            .any(|track| track.control_id == control_id)
            || self
                .native_special_path_controls
                .iter()
                .any(|track| track.control_id == control_id)
            || self
                .native_spline_controls
                .iter()
                .any(|track| track.control_id == control_id)
            || self
                .native_shake_object_controls
                .iter()
                .any(|track| track.control_id == control_id)
    }

    pub(crate) fn native_input_controls(&self) -> Vec<(u64, i32)> {
        let mut controls = self
            .native_object_controls
            .iter()
            .filter(|track| track.input_enabled)
            .map(|track| (track.control_id, track.input_descriptor))
            .collect::<Vec<_>>();
        controls.extend(
            self.native_special_path_controls
                .iter()
                .filter(|track| track.input_enabled)
                .map(|track| (track.control_id, track.input_descriptor)),
        );
        controls.extend(
            self.native_spline_controls
                .iter()
                .filter(|track| track.input_enabled)
                .map(|track| (track.control_id, track.input_descriptor)),
        );
        controls.extend(
            self.native_shake_object_controls
                .iter()
                .filter(|track| track.input_enabled)
                .map(|track| (track.control_id, track.input_descriptor)),
        );
        controls
    }

    /// Model the target's input-interrupt path. CProcCtrlDspObj samples input
    /// before its subclass updater; the updater then commits the exact terminal
    /// state and 0x431F00 reports terminal status 1. The first deferred value is
    /// the pre-terminal procedure progress, not an unconditional 1000.
    pub(crate) fn request_native_control_completion(&mut self, control_id: u64) -> bool {
        // CProcCtrlDspObj::Tick (0x431F00) does not publish the deferred
        // outputs until the subclass updater has actually run.  Input/forced
        // completion therefore only latches the request here; the next native
        // procedure poll performs one terminal updater invocation, increments
        // CProcCtrlDspObj+0x40, commits the exact endpoint, and records status 1.
        if let Some(track) = self
            .native_object_controls
            .iter_mut()
            .find(|track| track.control_id == control_id)
        {
            track.force_complete = true;
            return true;
        }
        if let Some(track) = self
            .native_special_path_controls
            .iter_mut()
            .find(|track| track.control_id == control_id)
        {
            track.force_complete = true;
            return true;
        }
        if let Some(track) = self
            .native_spline_controls
            .iter_mut()
            .find(|track| track.control_id == control_id)
        {
            track.force_complete = true;
            return true;
        }
        if let Some(track) = self
            .native_shake_object_controls
            .iter_mut()
            .find(|track| track.control_id == control_id)
        {
            track.force_complete = true;
            return true;
        }
        false
    }

    pub(crate) fn abort_native_control(&mut self, control_id: u64) -> bool {
        // Base CProcedure cancellation (0x431AF0 -> 0x431F00) exits before the
        // subclass updater.  It therefore neither increments the procedure
        // update counter nor snaps the controlled object to its endpoint.
        if let Some(index) = self
            .native_object_controls
            .iter()
            .position(|track| track.control_id == control_id)
        {
            let track = self.native_object_controls.swap_remove(index);
            self.native_control_completions.insert(
                control_id,
                NativeControlCompletion {
                    progress_per_mille: progress_per_mille_from_updates(
                        track.procedure_update_count,
                        track.duration_ms,
                    ),
                    status: -1,
                },
            );
            return true;
        }
        if let Some(index) = self
            .native_special_path_controls
            .iter()
            .position(|track| track.control_id == control_id)
        {
            let track = self.native_special_path_controls.swap_remove(index);
            self.native_control_completions.insert(
                control_id,
                NativeControlCompletion {
                    progress_per_mille: progress_per_mille_from_updates(
                        track.procedure_update_count,
                        u64::from(track.total_steps),
                    ),
                    status: -1,
                },
            );
            return true;
        }
        if let Some(index) = self
            .native_spline_controls
            .iter()
            .position(|track| track.control_id == control_id)
        {
            let track = self.native_spline_controls.swap_remove(index);
            self.native_control_completions.insert(
                control_id,
                NativeControlCompletion {
                    progress_per_mille: progress_per_mille_from_updates(
                        track.procedure_update_count,
                        track.duration_ms,
                    ),
                    status: -1,
                },
            );
            return true;
        }
        if let Some(index) = self
            .native_shake_object_controls
            .iter()
            .position(|track| track.control_id == control_id)
        {
            let track = self.native_shake_object_controls.swap_remove(index);
            self.native_control_completions.insert(
                control_id,
                NativeControlCompletion {
                    progress_per_mille: progress_per_mille_from_updates(
                        track.procedure_update_count,
                        u64::from(track.total_steps),
                    ),
                    status: -1,
                },
            );
            return true;
        }
        false
    }

    pub(crate) fn take_native_control_completion(
        &mut self,
        control_id: u64,
    ) -> NativeControlCompletion {
        self.native_control_completions
            .remove(&control_id)
            .unwrap_or(NativeControlCompletion::NATURAL)
    }

    pub(crate) fn has_native_object_control(&self, object_id: i32) -> bool {
        self.native_object_controls
            .iter()
            .any(|track| track.object_id == object_id)
            || self
                .native_special_path_controls
                .iter()
                .any(|track| track.object_id == object_id)
            || self
                .native_spline_controls
                .iter()
                .any(|track| track.object_id == object_id)
            || self
                .native_shake_object_controls
                .iter()
                .any(|track| track.object_id == object_id)
    }

    pub(crate) fn tick_native_object_controls(
        &mut self,
        elapsed_ms: u64,
    ) -> Vec<LayerAnimationEvent> {
        self.tick_native_object_controls_filtered(elapsed_ms, None)
    }

    fn tick_native_object_controls_filtered(
        &mut self,
        elapsed_ms: u64,
        only_control: Option<u64>,
    ) -> Vec<LayerAnimationEvent> {
        let mut events = Vec::new();
        let mut index = 0;
        while index < self.native_object_controls.len() {
            if only_control.is_some_and(|control_id| {
                self.native_object_controls[index].control_id != control_id
            }) {
                index += 1;
                continue;
            }
            let mut completion = None;
            {
                let track = &mut self.native_object_controls[index];
                track.wall_elapsed_ms = track.wall_elapsed_ms.saturating_add(elapsed_ms);

                // The base CProcedure is first scheduled for +1 ms.  A zero
                // elapsed host pass is not a procedure updater invocation,
                // except when input/local forced completion makes 0x431F00
                // invoke the updater immediately.
                if !track.force_complete && elapsed_ms == 0 {
                    index += 1;
                    continue;
                }

                let sampled_elapsed = if track.force_complete {
                    track.duration_ms
                } else if track.update_numerator != 0 {
                    // Exact 0x432160 catch-up limiter: one subclass updater
                    // invocation samples no later than its current deadline,
                    // then advances that deadline by 1000*numerator/denominator.
                    // Do not collapse multiple native procedure polls into one
                    // rendered host frame; scheduler cadence is a separate layer.
                    let sampled = track.wall_elapsed_ms.min(track.next_update_ms);
                    track.next_update_ms = sampled.saturating_add(track.update_interval_ms);
                    sampled
                } else {
                    track.wall_elapsed_ms
                };
                let elapsed = sampled_elapsed.min(track.duration_ms);
                track.procedure_update_count = track.procedure_update_count.wrapping_add(1);

                let progress_8_24 = (((u128::from(elapsed)) << 24)
                    / u128::from(track.duration_ms.max(1)))
                .min(0x0100_0000) as i32;
                let position_weight = target_curve_fixed_16(track.position_curve, progress_8_24);
                let alpha_weight = target_curve_fixed_16(track.alpha_curve, progress_8_24);
                let x = interpolate_target_fixed(track.from_x, track.to_x, position_weight);
                let y = interpolate_target_fixed(track.from_y, track.to_y, position_weight);
                let alpha_parameter =
                    interpolate_target_fixed(track.from_alpha, track.to_alpha, alpha_weight)
                        .clamp(0, 256);
                let fixed_parameter_16_16 = if elapsed >= track.duration_ms {
                    track.to_fixed_parameter_16_16
                } else {
                    let delta = i64::from(track.to_fixed_parameter_16_16)
                        - i64::from(track.from_fixed_parameter_16_16);
                    let value = i64::from(track.from_fixed_parameter_16_16)
                        + (delta * elapsed as i64) / track.duration_ms.max(1) as i64;
                    value.clamp(i64::from(i32::MIN), i64::from(i32::MAX)) as i32
                };
                let changed = x != track.last_x
                    || y != track.last_y
                    || alpha_parameter != track.last_alpha
                    || fixed_parameter_16_16 != track.last_fixed_parameter_16_16;
                if changed {
                    track.last_x = x;
                    track.last_y = y;
                    track.last_alpha = alpha_parameter;
                    track.last_fixed_parameter_16_16 = fixed_parameter_16_16;
                    events.push(LayerAnimationEvent::NativeObjectControlUpdated {
                        object_id: track.object_id,
                        x,
                        y,
                        alpha_parameter,
                        fixed_parameter_16_16,
                    });
                }
                if elapsed >= track.duration_ms {
                    if track.last_x != track.to_x
                        || track.last_y != track.to_y
                        || track.last_alpha != track.to_alpha
                        || track.last_fixed_parameter_16_16 != track.to_fixed_parameter_16_16
                    {
                        events.push(LayerAnimationEvent::NativeObjectControlUpdated {
                            object_id: track.object_id,
                            x: track.to_x,
                            y: track.to_y,
                            alpha_parameter: track.to_alpha,
                            fixed_parameter_16_16: track.to_fixed_parameter_16_16,
                        });
                    }
                    events.push(LayerAnimationEvent::NativeObjectControlFinished {
                        object_id: track.object_id,
                    });
                    completion = Some((
                        track.control_id,
                        NativeControlCompletion {
                            progress_per_mille: progress_per_mille_from_updates(
                                track.procedure_update_count,
                                track.duration_ms,
                            ),
                            status: if track.force_complete { 1 } else { 0 },
                        },
                    ));
                }
            }
            if let Some((control_id, result)) = completion {
                self.native_control_completions.insert(control_id, result);
                self.native_object_controls.swap_remove(index);
            } else {
                index += 1;
            }
        }
        events
    }

    pub(crate) fn tick_native_special_path_controls(
        &mut self,
        elapsed_ms: u64,
    ) -> Vec<LayerAnimationEvent> {
        self.tick_native_special_path_controls_filtered(elapsed_ms, None)
    }

    fn tick_native_special_path_controls_filtered(
        &mut self,
        elapsed_ms: u64,
        only_control: Option<u64>,
    ) -> Vec<LayerAnimationEvent> {
        let mut events = Vec::new();
        let mut index = 0;
        while index < self.native_special_path_controls.len() {
            if only_control.is_some_and(|control_id| {
                self.native_special_path_controls[index].control_id != control_id
            }) {
                index += 1;
                continue;
            }
            let mut completion = None;
            {
                let track = &mut self.native_special_path_controls[index];
                let updater_invoked = if track.force_complete {
                    track.current_step = track.total_steps;
                    true
                } else if track.current_step < track.total_steps {
                    track.accumulated_ms = track.accumulated_ms.saturating_add(elapsed_ms);
                    let remaining = track.total_steps - track.current_step;
                    let mut due = if track.interval_ms == 0 {
                        remaining
                    } else {
                        (track.accumulated_ms / track.interval_ms).min(u64::from(remaining)) as u32
                    };
                    if due == 0 {
                        false
                    } else {
                        if track.max_catchup_steps != 0 && due > track.max_catchup_steps {
                            due = track.max_catchup_steps;
                            // 0x4320E0 resets the deadline to now+interval once
                            // its per-updater catch-up cap is reached.
                            track.accumulated_ms = 0;
                        } else if track.interval_ms != 0 {
                            track.accumulated_ms %= track.interval_ms;
                        } else {
                            track.accumulated_ms = 0;
                        }
                        track.current_step = track
                            .current_step
                            .saturating_add(due)
                            .min(track.total_steps);
                        true
                    }
                } else {
                    false
                };
                if !updater_invoked {
                    index += 1;
                    continue;
                }
                // 0x431F00 increments this once per virtual updater call, not
                // once per path sample advanced inside 0x4320E0.
                track.procedure_update_count = track.procedure_update_count.wrapping_add(1);

                let done = track.current_step >= track.total_steps;
                let (x, y, alpha_parameter) = if done {
                    let &(x, y) = track.path.last().unwrap_or(&(0, 0));
                    (x, y, track.to_alpha)
                } else {
                    let progress_8_24 = (((u64::from(track.current_step)) << 24)
                        / u64::from(track.total_steps.max(1)))
                    .min(0x0100_0000) as i32;
                    let weight = target_curve_fixed_16(track.position_curve, progress_8_24);
                    let point_count = track.total_steps.saturating_add(1);
                    let path_index = ((u64::from(point_count) * weight.max(0) as u64) >> 16)
                        .min(u64::from(track.total_steps))
                        as usize;
                    let (x, y) = track.path[path_index];
                    let delta = i64::from(track.to_alpha - track.from_alpha);
                    let alpha = (i64::from(track.from_alpha)
                        + (delta * i64::from(track.current_step))
                            / i64::from(track.total_steps.max(1)))
                    .clamp(0, 256) as i32;
                    (x, y, alpha)
                };
                if x != track.last_x || y != track.last_y || alpha_parameter != track.last_alpha {
                    track.last_x = x;
                    track.last_y = y;
                    track.last_alpha = alpha_parameter;
                    events.push(LayerAnimationEvent::NativeSpecialPathControlUpdated {
                        object_id: track.object_id,
                        x,
                        y,
                        alpha_parameter,
                    });
                }
                if done {
                    events.push(LayerAnimationEvent::NativeObjectControlFinished {
                        object_id: track.object_id,
                    });
                    completion = Some((
                        track.control_id,
                        NativeControlCompletion {
                            progress_per_mille: progress_per_mille_from_updates(
                                track.procedure_update_count,
                                u64::from(track.total_steps),
                            ),
                            status: if track.force_complete { 1 } else { 0 },
                        },
                    ));
                }
            }
            if let Some((control_id, result)) = completion {
                self.native_control_completions.insert(control_id, result);
                self.native_special_path_controls.swap_remove(index);
            } else {
                index += 1;
            }
        }
        events
    }

    pub(crate) fn tick_native_shake_object_controls(
        &mut self,
        elapsed_ms: u64,
    ) -> Vec<LayerAnimationEvent> {
        self.tick_native_shake_object_controls_filtered(elapsed_ms, None)
    }

    fn tick_native_shake_object_controls_filtered(
        &mut self,
        elapsed_ms: u64,
        only_control: Option<u64>,
    ) -> Vec<LayerAnimationEvent> {
        let mut events = Vec::new();
        let mut index = 0;
        while index < self.native_shake_object_controls.len() {
            if only_control.is_some_and(|control_id| {
                self.native_shake_object_controls[index].control_id != control_id
            }) {
                index += 1;
                continue;
            }
            let mut completion = None;
            {
                let track = &mut self.native_shake_object_controls[index];
                let updater_invoked = if track.force_complete {
                    track.current_step = track.total_steps;
                    true
                } else if track.current_step < track.total_steps {
                    track.accumulated_ms = track.accumulated_ms.saturating_add(elapsed_ms);
                    let remaining = track.total_steps - track.current_step;
                    let due = if track.interval_ms == 0 {
                        remaining
                    } else {
                        (track.accumulated_ms / track.interval_ms).min(u64::from(remaining)) as u32
                    };
                    if due == 0 {
                        false
                    } else {
                        if track.interval_ms != 0 {
                            track.accumulated_ms %= track.interval_ms;
                        } else {
                            track.accumulated_ms = 0;
                        }
                        // CProcShakeDspObj leaves its catch-up cap at zero, so
                        // 0x4320E0 may advance multiple samples in this one
                        // virtual updater invocation.
                        track.current_step = track
                            .current_step
                            .saturating_add(due)
                            .min(track.total_steps);
                        true
                    }
                } else {
                    false
                };
                if !updater_invoked {
                    index += 1;
                    continue;
                }
                track.procedure_update_count = track.procedure_update_count.wrapping_add(1);

                let done = track.current_step >= track.total_steps;
                let (x, y) = if done || track.waveform.is_empty() || track.envelope.is_empty() {
                    (track.base_x, track.base_y)
                } else {
                    let wave_len = track.waveform.len() as u32;
                    let step_index = track.current_step;
                    let envelope_index = (step_index / wave_len)
                        .min(track.envelope.len().saturating_sub(1) as u32)
                        as usize;
                    let wave_index = (step_index % wave_len) as usize;
                    let offset = ((i64::from(track.envelope[envelope_index])
                        * i64::from(track.waveform[wave_index]))
                        >> 16)
                        .clamp(i64::from(i32::MIN), i64::from(i32::MAX))
                        as i32;
                    match track.mode {
                        0 | 1 | 4 => (track.base_x, track.base_y.wrapping_add(offset)),
                        2 | 3 | 5 => (track.base_x.wrapping_add(offset), track.base_y),
                        _ => (track.base_x, track.base_y),
                    }
                };
                if x != track.last_x || y != track.last_y {
                    track.last_x = x;
                    track.last_y = y;
                    events.push(LayerAnimationEvent::NativeShakeObjectControlUpdated {
                        object_id: track.object_id,
                        x,
                        y,
                    });
                }
                if done {
                    events.push(LayerAnimationEvent::NativeObjectControlFinished {
                        object_id: track.object_id,
                    });
                    completion = Some((
                        track.control_id,
                        NativeControlCompletion {
                            progress_per_mille: progress_per_mille_from_updates(
                                track.procedure_update_count,
                                u64::from(track.total_steps),
                            ),
                            status: if track.force_complete { 1 } else { 0 },
                        },
                    ));
                }
            }
            if let Some((control_id, result)) = completion {
                self.native_control_completions.insert(control_id, result);
                self.native_shake_object_controls.swap_remove(index);
            } else {
                index += 1;
            }
        }
        events
    }

    pub(crate) fn tick_native_spline_controls(
        &mut self,
        elapsed_ms: u64,
    ) -> Vec<LayerAnimationEvent> {
        self.tick_native_spline_controls_filtered(elapsed_ms, None)
    }

    fn tick_native_spline_controls_filtered(
        &mut self,
        elapsed_ms: u64,
        only_control: Option<u64>,
    ) -> Vec<LayerAnimationEvent> {
        let mut events = Vec::new();
        let mut index = 0;
        while index < self.native_spline_controls.len() {
            if only_control.is_some_and(|control_id| {
                self.native_spline_controls[index].control_id != control_id
            }) {
                index += 1;
                continue;
            }
            let mut completion = None;
            {
                let track = &mut self.native_spline_controls[index];
                track.wall_elapsed_ms = track.wall_elapsed_ms.saturating_add(elapsed_ms);
                if !track.force_complete && elapsed_ms == 0 {
                    index += 1;
                    continue;
                }
                let sampled_elapsed = if track.force_complete {
                    track.duration_ms
                } else if track.update_numerator != 0 {
                    let sampled = track.wall_elapsed_ms.min(track.next_update_ms);
                    track.next_update_ms = sampled.saturating_add(track.update_interval_ms);
                    sampled
                } else {
                    track.wall_elapsed_ms
                };
                let elapsed = sampled_elapsed.min(track.duration_ms);
                track.procedure_update_count = track.procedure_update_count.wrapping_add(1);
                let done = elapsed >= track.duration_ms;
                let progress_8_24 = (((u128::from(elapsed)) << 24)
                    / u128::from(track.duration_ms.max(1)))
                .min(0x0100_0000) as i32;
                let vector = if done {
                    track.final_vector
                } else {
                    let weight = target_curve_fixed_16(track.position_curve, progress_8_24);
                    let t = f64::from(weight) / 65_536.0;
                    (track.x.sample(t), track.y.sample(t), track.z.sample(t))
                };
                let alpha_parameter = if done {
                    track.to_alpha
                } else {
                    let scaled_progress = ((i128::from(progress_8_24) * 65_536)
                        / i128::from(track.spline_time_scale.max(1)))
                    .clamp(0, 0x0100_0000) as i32;
                    let weight = target_curve_fixed_16(track.alpha_curve, scaled_progress);
                    interpolate_target_fixed(track.from_alpha, track.to_alpha, weight).clamp(0, 256)
                };
                let fixed_parameter_16_16 = if done {
                    track.to_fixed_parameter_16_16
                } else {
                    let delta = i64::from(track.to_fixed_parameter_16_16)
                        - i64::from(track.from_fixed_parameter_16_16);
                    (i64::from(track.from_fixed_parameter_16_16)
                        + delta * elapsed as i64 / track.duration_ms.max(1) as i64)
                        .clamp(i64::from(i32::MIN), i64::from(i32::MAX)) as i32
                };
                if vector != track.last_vector
                    || alpha_parameter != track.last_alpha
                    || fixed_parameter_16_16 != track.last_fixed_parameter_16_16
                {
                    track.last_vector = vector;
                    track.last_alpha = alpha_parameter;
                    track.last_fixed_parameter_16_16 = fixed_parameter_16_16;
                    events.push(LayerAnimationEvent::NativeSplineControlUpdated {
                        object_id: track.object_id,
                        fixed_x_16_16: vector.0,
                        fixed_y_16_16: vector.1,
                        fixed_z_16_16: vector.2,
                        alpha_parameter,
                        fixed_parameter_16_16,
                    });
                }
                if done {
                    events.push(LayerAnimationEvent::NativeObjectControlFinished {
                        object_id: track.object_id,
                    });
                    completion = Some((
                        track.control_id,
                        NativeControlCompletion {
                            progress_per_mille: progress_per_mille_from_updates(
                                track.procedure_update_count,
                                track.duration_ms,
                            ),
                            status: if track.force_complete { 1 } else { 0 },
                        },
                    ));
                }
            }
            if let Some((control_id, result)) = completion {
                self.native_control_completions.insert(control_id, result);
                self.native_spline_controls.swap_remove(index);
            } else {
                index += 1;
            }
        }
        events
    }

    /// Advance only the CProcCtrlDspObj-family instance currently being
    /// polled by the VM cooperative scheduler.  The target does not advance
    /// all graph controls from a renderer-wide frame callback.
    pub(crate) fn tick_native_control(
        &mut self,
        control_id: u64,
        elapsed_ms: u64,
    ) -> Vec<LayerAnimationEvent> {
        let mut events = self.tick_native_object_controls_filtered(elapsed_ms, Some(control_id));
        events
            .extend(self.tick_native_special_path_controls_filtered(elapsed_ms, Some(control_id)));
        events.extend(self.tick_native_spline_controls_filtered(elapsed_ms, Some(control_id)));
        events
            .extend(self.tick_native_shake_object_controls_filtered(elapsed_ms, Some(control_id)));
        events
    }

    pub(crate) fn clear_object(&mut self, object_id: i32) {
        self.object_alpha_tracks
            .retain(|track| track.object_id != object_id);
        self.native_object_controls
            .retain(|track| track.object_id != object_id);
        self.native_special_path_controls
            .retain(|track| track.object_id != object_id);
        self.native_spline_controls
            .retain(|track| track.object_id != object_id);
        self.native_shake_object_controls
            .retain(|track| track.object_id != object_id);
    }

    pub(crate) fn clear_layer(&mut self, layer_id: i32) {
        self.tracks.retain(|track| track.layer_id != layer_id);
        self.spline_tracks
            .retain(|track| track.layer_id != layer_id);
    }

    pub(crate) fn clear_layers<'a>(&mut self, layer_ids: impl IntoIterator<Item = &'a i32>) {
        let ids = layer_ids.into_iter().copied().collect::<Vec<_>>();
        self.tracks.retain(|track| !ids.contains(&track.layer_id));
        self.spline_tracks
            .retain(|track| !ids.contains(&track.layer_id));
    }

    pub(crate) fn active_count(&self) -> usize {
        self.tracks.len()
            + self.object_alpha_tracks.len()
            + self.native_object_controls.len()
            + self.native_special_path_controls.len()
            + self.native_spline_controls.len()
            + self.native_shake_object_controls.len()
            + self.spline_tracks.len()
    }

    pub(crate) fn tick(
        &mut self,
        layers: &mut BTreeMap<i32, RuntimeGraphLayer>,
        surfaces: &mut BTreeMap<i32, RuntimeSurface>,
        objects: &mut BTreeMap<i32, RuntimeGraphObjectProperties>,
    ) -> Vec<LayerAnimationEvent> {
        let mut events = Vec::new();
        let mut index = 0;
        while index < self.tracks.len() {
            let track = &mut self.tracks[index];
            track.elapsed_ticks = track.elapsed_ticks.saturating_add(1);
            let t = (track.elapsed_ticks as f32 / track.duration_ticks as f32).clamp(0.0, 1.0);
            let eased = sample_curve(track.curve, t);
            let value = track.from + (track.to - track.from) * eased;
            if let Some(layer) = layers.get_mut(&track.layer_id) {
                match track.property {
                    LayerAnimationProperty::BaseX => layer.x = value,
                    LayerAnimationProperty::BaseY => layer.y = value,
                    LayerAnimationProperty::BaseZ => layer.z = value.round() as i32,
                    LayerAnimationProperty::Opacity => layer.opacity = value.clamp(0.0, 1.0),
                    LayerAnimationProperty::TransformX => layer.transform_x = value,
                    LayerAnimationProperty::TransformY => layer.transform_y = value,
                    LayerAnimationProperty::ScaleX => layer.scale_x = value.max(0.001),
                    LayerAnimationProperty::ScaleY => layer.scale_y = value.max(0.001),
                }
            } else if let Some(surface) = surfaces.get_mut(&track.layer_id) {
                match track.property {
                    LayerAnimationProperty::BaseX => surface.x = value,
                    LayerAnimationProperty::BaseY => surface.y = value,
                    LayerAnimationProperty::BaseZ => surface.z = value.round() as i32,
                    LayerAnimationProperty::Opacity => surface.opacity = value.clamp(0.0, 1.0),
                    LayerAnimationProperty::TransformX
                    | LayerAnimationProperty::TransformY
                    | LayerAnimationProperty::ScaleX
                    | LayerAnimationProperty::ScaleY => {}
                }
            }
            if track.elapsed_ticks >= track.duration_ticks {
                events.push(LayerAnimationEvent::Finished {
                    layer_id: track.layer_id,
                    property: track.property,
                });
                self.tracks.swap_remove(index);
            } else {
                index += 1;
            }
        }
        let mut index = 0;
        while index < self.object_alpha_tracks.len() {
            let track = &mut self.object_alpha_tracks[index];
            track.elapsed_ticks = track.elapsed_ticks.saturating_add(1);
            let t = (track.elapsed_ticks as f32 / track.duration_ticks as f32).clamp(0.0, 1.0);
            let eased = sample_curve(track.curve, t);
            let value = track.from + (track.to - track.from) * eased;
            let alpha_parameter = value.round().clamp(0.0, 256.0) as i32;
            objects
                .entry(track.object_id)
                .or_default()
                .set_alpha_parameter(alpha_parameter);
            events.push(LayerAnimationEvent::ObjectAlphaUpdated {
                object_id: track.object_id,
                alpha_parameter,
            });
            if track.elapsed_ticks >= track.duration_ticks {
                events.push(LayerAnimationEvent::ObjectAlphaFinished {
                    object_id: track.object_id,
                });
                self.object_alpha_tracks.swap_remove(index);
            } else {
                index += 1;
            }
        }
        let mut index = 0;
        while index < self.spline_tracks.len() {
            let track = &mut self.spline_tracks[index];
            track.elapsed_ticks = track.elapsed_ticks.saturating_add(1);
            let mut t = (track.elapsed_ticks as f32 / track.duration_ticks as f32).clamp(0.0, 1.0);
            t = sample_curve(track.curve, t);
            let divisor = if track.time_scale == 0 {
                65_536.0
            } else {
                track.time_scale as f32
            };
            t = (t * 65_536.0 / divisor).clamp(0.0, 1.0);
            if let Some(layer) = layers.get_mut(&track.layer_id) {
                layer.transform_x = track.x.sample(t);
                layer.transform_y = track.y.sample(t);
                layer.transform_z = track.z.sample(t).round() as i32;
            }
            if track.elapsed_ticks >= track.duration_ticks {
                events.push(LayerAnimationEvent::Finished {
                    layer_id: track.layer_id,
                    property: LayerAnimationProperty::TransformX,
                });
                self.spline_tracks.swap_remove(index);
            } else {
                index += 1;
            }
        }
        events
    }
}

#[derive(Debug, Clone, Copy, PartialEq, Eq)]
pub(crate) enum LayerAnimationProperty {
    BaseX,
    BaseY,
    BaseZ,
    Opacity,
    TransformX,
    TransformY,
    ScaleX,
    ScaleY,
}

#[derive(Debug, Clone, Copy)]
pub(crate) enum LayerAnimationEvent {
    Finished {
        layer_id: i32,
        property: LayerAnimationProperty,
    },
    ObjectAlphaUpdated {
        object_id: i32,
        alpha_parameter: i32,
    },
    ObjectAlphaFinished {
        object_id: i32,
    },
    NativeObjectControlUpdated {
        object_id: i32,
        x: i32,
        y: i32,
        alpha_parameter: i32,
        fixed_parameter_16_16: i32,
    },
    NativeObjectControlFinished {
        object_id: i32,
    },
    NativeSpecialPathControlUpdated {
        object_id: i32,
        x: i32,
        y: i32,
        alpha_parameter: i32,
    },
    NativeShakeObjectControlUpdated {
        object_id: i32,
        x: i32,
        y: i32,
    },
    NativeSplineControlUpdated {
        object_id: i32,
        fixed_x_16_16: i32,
        fixed_y_16_16: i32,
        fixed_z_16_16: i32,
        alpha_parameter: i32,
        fixed_parameter_16_16: i32,
    },
}

#[derive(Debug, Clone)]
struct LayerAnimation {
    layer_id: i32,
    property: LayerAnimationProperty,
    from: f32,
    to: f32,
    duration_ticks: u32,
    elapsed_ticks: u32,
    curve: i32,
}

#[derive(Debug, Clone)]
struct ObjectAlphaAnimation {
    object_id: i32,
    from: f32,
    to: f32,
    duration_ticks: u32,
    elapsed_ticks: u32,
    curve: i32,
}

#[derive(Debug, Clone)]
struct NativeObjectControlAnimation {
    control_id: u64,
    object_id: i32,
    from_x: i32,
    from_y: i32,
    to_x: i32,
    to_y: i32,
    position_curve: i32,
    from_alpha: i32,
    to_alpha: i32,
    alpha_curve: i32,
    from_fixed_parameter_16_16: i32,
    to_fixed_parameter_16_16: i32,
    duration_ms: u64,
    wall_elapsed_ms: u64,
    update_numerator: i32,
    update_interval_ms: u64,
    next_update_ms: u64,
    input_enabled: bool,
    input_descriptor: i32,
    force_complete: bool,
    procedure_update_count: u32,
    last_x: i32,
    last_y: i32,
    last_alpha: i32,
    last_fixed_parameter_16_16: i32,
}

#[derive(Debug, Clone)]
struct NativeSpecialPathControlAnimation {
    control_id: u64,
    object_id: i32,
    path: Vec<(i32, i32)>,
    position_curve: i32,
    from_alpha: i32,
    to_alpha: i32,
    total_steps: u32,
    current_step: u32,
    interval_ms: u64,
    accumulated_ms: u64,
    max_catchup_steps: u32,
    input_enabled: bool,
    input_descriptor: i32,
    force_complete: bool,
    procedure_update_count: u32,
    last_x: i32,
    last_y: i32,
    last_alpha: i32,
}

#[derive(Debug, Clone)]
struct NativeShakeObjectControlAnimation {
    control_id: u64,
    object_id: i32,
    mode: i32,
    base_x: i32,
    base_y: i32,
    envelope: Vec<i32>,
    waveform: Vec<i32>,
    total_steps: u32,
    current_step: u32,
    interval_ms: u64,
    accumulated_ms: u64,
    input_enabled: bool,
    input_descriptor: i32,
    force_complete: bool,
    procedure_update_count: u32,
    last_x: i32,
    last_y: i32,
}

#[derive(Debug, Clone)]
struct NativeSplineObjectControlAnimation {
    control_id: u64,
    object_id: i32,
    x: NativeNaturalCubicSpline,
    y: NativeNaturalCubicSpline,
    z: NativeNaturalCubicSpline,
    final_vector: (i32, i32, i32),
    position_curve: i32,
    from_alpha: i32,
    to_alpha: i32,
    alpha_curve: i32,
    from_fixed_parameter_16_16: i32,
    to_fixed_parameter_16_16: i32,
    duration_ms: u64,
    wall_elapsed_ms: u64,
    update_numerator: i32,
    update_interval_ms: u64,
    next_update_ms: u64,
    spline_time_scale: u32,
    input_enabled: bool,
    input_descriptor: i32,
    force_complete: bool,
    procedure_update_count: u32,
    last_vector: (i32, i32, i32),
    last_alpha: i32,
    last_fixed_parameter_16_16: i32,
}

#[derive(Debug, Clone)]
struct NativeNaturalCubicSpline {
    values: Vec<i32>,
    second_derivatives: Vec<f64>,
}

impl NativeNaturalCubicSpline {
    fn new(values: Vec<i32>) -> Self {
        let count = values.len();
        let mut second_derivatives = vec![0.0; count];
        if count > 2 {
            let mut upper = vec![0.0; count];
            for index in 1..count - 1 {
                let denominator = 4.0 - upper[index - 1];
                upper[index] = 1.0 / denominator;
                second_derivatives[index] = (3.0
                    * (f64::from(values[index - 1]) - 2.0 * f64::from(values[index])
                        + f64::from(values[index + 1]))
                    - second_derivatives[index - 1])
                    / denominator;
            }
            for index in (1..count - 1).rev() {
                second_derivatives[index] -= upper[index] * second_derivatives[index + 1];
            }
        }
        Self {
            values,
            second_derivatives,
        }
    }

    fn sample(&self, t: f64) -> i32 {
        if self.values.len() <= 1 {
            return self.values.first().copied().unwrap_or_default();
        }
        let scaled = t.clamp(0.0, 1.0) * (self.values.len() - 1) as f64;
        let index = (scaled.floor() as usize).min(self.values.len() - 2);
        let local = scaled - index as f64;
        let a = 1.0 - local;
        let b = local;
        let value = a * f64::from(self.values[index])
            + b * f64::from(self.values[index + 1])
            + ((a * a * a - a) * self.second_derivatives[index]
                + (b * b * b - b) * self.second_derivatives[index + 1])
                / 3.0;
        value as i32
    }
}

#[derive(Debug, Clone)]
struct SplineVectorAnimation {
    layer_id: i32,
    x: SplineSampler,
    y: SplineSampler,
    z: SplineSampler,
    duration_ticks: u32,
    elapsed_ticks: u32,
    curve: i32,
    time_scale: u16,
}

#[derive(Debug, Clone, Copy, PartialEq, Eq)]
enum AnimationFidelity {
    /// Default.  Unknown target curve identifiers and `CSpline` internals are
    /// not assigned invented formulas.
    TargetStrict,
    /// Opt-in compatibility with the old port's guessed easing and natural
    /// cubic spline.  This mode exists for visual comparison only.
    LegacyCompatibility,
}

fn animation_fidelity() -> AnimationFidelity {
    static FIDELITY: OnceLock<AnimationFidelity> = OnceLock::new();
    *FIDELITY.get_or_init(|| {
        if std::env::var_os("ETHORNELL_LEGACY_ANIMATION_CURVES").is_some() {
            AnimationFidelity::LegacyCompatibility
        } else {
            AnimationFidelity::TargetStrict
        }
    })
}

fn report_unrecovered_curve_once(curve: i32, kind: &'static str) {
    if curve == 0 || std::env::var_os("TRACE_REVERSE_GAPS").is_none() {
        return;
    }
    static REPORTED: OnceLock<Mutex<BTreeSet<(i32, &'static str)>>> = OnceLock::new();
    let reported = REPORTED.get_or_init(|| Mutex::new(BTreeSet::new()));
    let Ok(mut reported) = reported.lock() else {
        return;
    };
    if reported.insert((curve, kind)) {
        tracing::warn!(
            curve,
            kind,
            fallback = "linear",
            "target animation curve formula is unrecovered"
        );
    }
}

#[derive(Debug, Clone)]
enum SplineSampler {
    /// Conservative target-strict fallback.  Control points and timing remain
    /// exact, but interpolation does not invent overshoot while native
    /// `CSpline` coefficients remain unrecovered.
    PiecewiseLinear(Vec<f32>),
    /// Previous port behavior, available only through the explicit legacy
    /// compatibility switch.
    LegacyNaturalCubic(NaturalCubicSpline),
}

impl SplineSampler {
    fn from_control_points(values: Vec<f32>) -> Self {
        // sub_443E50/sub_444000 are recovered: the target builds a natural
        // cubic spline independently for X/Y/Z.  The coefficient image stores
        // M/2, therefore the cubic term divides by 3 (not 6).
        Self::LegacyNaturalCubic(NaturalCubicSpline::new(values))
    }

    fn sample(&self, t: f32) -> f32 {
        match self {
            Self::PiecewiseLinear(values) => sample_piecewise_linear(values, t),
            Self::LegacyNaturalCubic(spline) => spline.sample(t),
        }
    }
}

fn sample_piecewise_linear(values: &[f32], t: f32) -> f32 {
    if values.len() <= 1 {
        return values.first().copied().unwrap_or_default();
    }
    let scaled = t.clamp(0.0, 1.0) * (values.len() - 1) as f32;
    let index = (scaled.floor() as usize).min(values.len() - 2);
    let local = scaled - index as f32;
    values[index] + (values[index + 1] - values[index]) * local
}

#[derive(Debug, Clone)]
struct NaturalCubicSpline {
    values: Vec<f32>,
    second_derivatives: Vec<f32>,
}

impl NaturalCubicSpline {
    fn new(values: Vec<f32>) -> Self {
        let count = values.len();
        let mut second_derivatives = vec![0.0; count];
        if count > 2 {
            let mut upper = vec![0.0; count];
            for index in 1..count - 1 {
                let denominator = 4.0 - upper[index - 1];
                upper[index] = 1.0 / denominator;
                second_derivatives[index] = (3.0
                    * (values[index - 1] - 2.0 * values[index] + values[index + 1])
                    - second_derivatives[index - 1])
                    / denominator;
            }
            for index in (1..count - 1).rev() {
                second_derivatives[index] -= upper[index] * second_derivatives[index + 1];
            }
        }
        Self {
            values,
            second_derivatives,
        }
    }

    fn sample(&self, t: f32) -> f32 {
        if self.values.len() <= 1 {
            return self.values.first().copied().unwrap_or_default();
        }
        let scaled = t.clamp(0.0, 1.0) * (self.values.len() - 1) as f32;
        let index = (scaled.floor() as usize).min(self.values.len() - 2);
        let local = scaled - index as f32;
        let a = 1.0 - local;
        let b = local;
        a * self.values[index]
            + b * self.values[index + 1]
            + ((a * a * a - a) * self.second_derivatives[index]
                + (b * b * b - b) * self.second_derivatives[index + 1])
                / 3.0
    }
}

fn progress_per_mille_from_updates(update_count: u32, denominator: u64) -> i32 {
    if denominator == 0 {
        return 0;
    }
    let value = (u128::from(update_count) * 1000) / u128::from(denominator);
    value.min(i32::MAX as u128) as i32
}

fn round_half_away_from_zero(value: f64) -> i32 {
    if value < 0.0 {
        (value - 0.5) as i32
    } else {
        (value + 0.5) as i32
    }
}

/// Target sub_494730 samples the 2D path used by CProcCtrlDspObjSp. X is
/// treated as the monotonic independent axis and Y is a natural cubic through
/// current/middle/final points. The sampled table is what sub_432AC0 indexes.
fn build_native_three_point_path(
    start: (i32, i32),
    middle: (i32, i32),
    end: (i32, i32),
    sample_points: u32,
) -> Option<Vec<(i32, i32)>> {
    if sample_points < 2 {
        return None;
    }
    let ascending = start.0 < end.0;
    if ascending {
        if middle.0 <= start.0 || middle.0 >= end.0 {
            return None;
        }
    } else if start.0 > end.0 {
        if middle.0 >= start.0 || middle.0 <= end.0 {
            return None;
        }
    } else {
        return None;
    }

    let x1 = if ascending {
        f64::from(middle.0 - start.0)
    } else {
        f64::from(start.0 - middle.0)
    };
    let x2 = if ascending {
        f64::from(end.0 - start.0)
    } else {
        f64::from(start.0 - end.0)
    };
    let y0 = 0.0_f64;
    let y1 = f64::from(middle.1 - start.1);
    let y2 = f64::from(end.1 - start.1);
    let h0 = x1;
    let h1 = x2 - x1;
    if h0 <= 0.0 || h1 <= 0.0 {
        return None;
    }
    // sub_494560 stores c=M/2. Natural endpoints are zero and only the
    // middle coefficient is nonzero for this fixed three-point path.
    let c1 = 3.0 * ((y2 - y1) / h1 - (y1 - y0) / h0) / (2.0 * (h0 + h1));
    let step = x2 / f64::from(sample_points - 1);
    let mut path = Vec::with_capacity(sample_points as usize);
    for index in 0..sample_points {
        let x = step * f64::from(index);
        let (base_x, base_y, h, c0, c_next) = if x < x1 {
            (0.0, y0, h0, 0.0, c1)
        } else {
            (x1, y1, h1, c1, 0.0)
        };
        let dx = x - base_x;
        let slope = if base_x == 0.0 {
            (y1 - y0) / h0
        } else {
            (y2 - y1) / h1
        };
        let y = base_y
            + dx * (slope - (2.0 * c0 + c_next) * h / 3.0
                + ((c_next - c0) / (3.0 * h) * dx + c0) * dx);
        let x_delta = round_half_away_from_zero(x);
        let sampled_x = if ascending {
            start.0.wrapping_add(x_delta)
        } else {
            start.0.wrapping_sub(x_delta)
        };
        let sampled_y = start.1.wrapping_add(round_half_away_from_zero(y));
        path.push((sampled_x, sampled_y));
    }
    Some(path)
}

pub(crate) fn motion_coord(raw: i32) -> f32 {
    if raw.unsigned_abs() >= 32_768 {
        raw as f32 / 65_536.0
    } else {
        raw as f32
    }
}

/// Exact normalized form of target `sub_41A690`.  The target receives
/// progress in 8.24 fixed point and returns 16.16.
fn sample_curve(curve: i32, t: f32) -> f32 {
    let progress = (t.clamp(0.0, 1.0) as f64 * 16_777_216.0) as i32;
    target_curve_fixed_16(curve, progress) as f32 / 65_536.0
}

pub(crate) fn target_curve_fixed_16(curve: i32, progress_8_24: i32) -> i32 {
    let progress = progress_8_24.clamp(0, 0x0100_0000);
    if curve == 0 || !(1..=15).contains(&curve) {
        return progress / 256;
    }
    let value = match curve {
        // Preserve the target's integer truncation before trig conversion;
        // otherwise repeated animation samples can differ by one 16.16 unit.
        1 => {
            let angle_units = 11_796_480i64 - (180i64 * i64::from(progress) / 256);
            ((f64::from(angle_units as i32) * std::f64::consts::PI / 11_796_480.0).cos() + 1.0)
                * 32_768.0
        }
        2 => {
            let angle_units = 90i64 * i64::from(progress) / 256;
            (f64::from(angle_units as i32) * std::f64::consts::PI / 11_796_480.0).sin() * 65_536.0
        }
        3 => {
            let angle_units = 5_898_240i64 - (90i64 * i64::from(progress) / 256);
            (1.0 - (f64::from(angle_units as i32) * std::f64::consts::PI / 11_796_480.0).sin())
                * 65_536.0
        }
        4..=15 => {
            let t = f64::from(progress) / 16_777_216.0;
            match curve {
                4 => t.powi(2) * 65_536.0,
                5 => (1.0 - (1.0 - t).powi(2)) * 65_536.0,
                6 => t.powf(2.5) * 65_536.0,
                7 => (1.0 - (1.0 - t).powf(2.5)) * 65_536.0,
                8 => t.powi(3) * 65_536.0,
                9 => (1.0 - (1.0 - t).powi(3)) * 65_536.0,
                10 => t.powi(4) * 65_536.0,
                11 => (1.0 - (1.0 - t).powi(4)) * 65_536.0,
                12 => t.powi(5) * 65_536.0,
                13 => (1.0 - (1.0 - t).powi(5)) * 65_536.0,
                14 => t.powi(6) * 65_536.0,
                15 => (1.0 - (1.0 - t).powi(6)) * 65_536.0,
                _ => unreachable!(),
            }
        }
        _ => unreachable!(),
    };
    value.clamp(0.0, 65_536.0) as i32
}

fn interpolate_target_fixed(from: i32, to: i32, weight_16_16: i32) -> i32 {
    let delta = i64::from(to) - i64::from(from);
    let value = i64::from(from) + ((delta * i64::from(weight_16_16)) >> 16);
    value.clamp(i64::from(i32::MIN), i64::from(i32::MAX)) as i32
}

fn value_to_i32(value: &Value) -> Option<i32> {
    match value {
        Value::Int(value) => Some(*value),
        Value::Ptr(value) => Some(*value as i32),
        Value::Func { offset, .. } => Some(*offset as i32),
        Value::Str(_) | Value::Program(_) | Value::None => None,
    }
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn schedule_uses_native_duration_and_input_slots() {
        let args = [
            Value::Int(1808),
            Value::Int(1),
            Value::Int(0),
            Value::Int(0),
            Value::Int(250),
            Value::Int(0),
            Value::Int(0),
            Value::Int(0),
            Value::Int(0),
            Value::Int(0),
            Value::Int(0),
            Value::Int(0),
        ];
        let schedule = ScheduledObjectControl::from_popped_args(&args).unwrap();
        assert_eq!(schedule.target_object, 0);
        assert_eq!(schedule.target_x, 0);
        assert_eq!(schedule.target_y, 0);
        assert_eq!(schedule.position_curve, 0);
        assert_eq!(schedule.target_alpha, 0);
        assert_eq!(schedule.alpha_curve, 0);
        assert_eq!(schedule.fixed_parameter_target, 0);
        assert_eq!(schedule.input_descriptor, 1808);
        assert!(schedule.input_enabled);
        assert_eq!(schedule.duration_ms, 250);
        assert_eq!(schedule.update_denominator, 0);
        assert_eq!(schedule.update_numerator, 0);
    }

    #[test]
    fn target_curves_match_recovered_sub_41a690_shapes() {
        for curve in 0..=15 {
            assert!((sample_curve(curve, 0.0) - 0.0).abs() < 0.000_01);
            assert!((sample_curve(curve, 1.0) - 1.0).abs() < 0.000_01);
        }
        assert!((sample_curve(1, 0.5) - 0.5).abs() < 0.000_01);
        assert!((sample_curve(4, 0.5) - 0.25).abs() < 0.000_01);
        assert!((sample_curve(5, 0.5) - 0.75).abs() < 0.000_01);
        assert!((sample_curve(8, 0.5) - 0.125).abs() < 0.000_01);
        assert!((sample_curve(9, 0.5) - 0.875).abs() < 0.000_01);
        assert!((sample_curve(99, 0.5) - 0.5).abs() < 0.000_01);
    }

    #[test]
    fn spline_schedule_uses_reverse_engineered_argument_slots() {
        let args = [
            Value::Int(42),
            Value::Int(2),
            Value::Ptr(0x1000),
            Value::Int(5),
            Value::Int(100),
            Value::Int(((32_768u32 << 16) | 7) as i32),
            Value::Int(192),
            Value::Int(1_000),
            Value::Int(250),
            Value::Int(3),
            Value::Int(1),
            Value::Int(1808),
        ];
        let schedule = ScheduledSplineControl::from_source_args(&args).unwrap();
        assert_eq!(schedule.target_object, 42);
        assert_eq!(schedule.position_curve, 5);
        assert_eq!(schedule.target_alpha, 100);
        assert_eq!(schedule.alpha_curve, 7);
        assert_eq!(schedule.spline_time_scale, 32_768);
        assert_eq!(schedule.fixed_parameter_target, 192);
        assert_eq!(schedule.duration_ms, 1_000);
        assert_eq!(schedule.update_denominator, 250);
        assert_eq!(schedule.update_numerator, 3);
        assert!(schedule.input_enabled);
        assert_eq!(schedule.input_descriptor, 1808);
    }

    #[test]
    fn polling_animation_status_does_not_advance_time() {
        let mut registry = GraphAnimationRegistry::default();
        let args = [Value::Int(7), Value::Int(32)];
        registry.start(7, 32, &args);
        let first = registry.evaluate(&[Value::Int(7)]).unwrap();
        let second = registry.evaluate(&[Value::Int(7)]).unwrap();
        assert_eq!(first.remaining_ticks, 2);
        assert_eq!(second.remaining_ticks, 2);
        registry.tick();
        assert_eq!(
            registry.evaluate(&[Value::Int(7)]).unwrap().remaining_ticks,
            1
        );
    }

    #[test]
    fn short_durations_are_milliseconds_not_frame_counts() {
        assert_eq!(duration_ms_to_ticks(1), 1);
        assert_eq!(duration_ms_to_ticks(16), 1);
        assert_eq!(duration_ms_to_ticks(32), 2);
    }

    #[test]
    fn native_object_control_uses_elapsed_milliseconds_and_exact_endpoints() {
        let mut animations = LayerAnimationSystem::default();
        animations.schedule_native_object_control(
            0x8000_0000u32 as i32,
            (10, 20),
            (110, 220),
            4,
            0,
            256,
            0,
            7 << 16,
            7 << 16,
            100,
            1,
            0,
            false,
            0,
        );
        let first = animations.tick_native_object_controls(50);
        assert!(first.iter().any(|event| matches!(
            event,
            LayerAnimationEvent::NativeObjectControlUpdated {
                object_id,
                x: 35,
                y: 70,
                alpha_parameter: 128,
                ..
            } if *object_id == 0x8000_0000u32 as i32
        )));
        let second = animations.tick_native_object_controls(50);
        assert!(second.iter().any(|event| matches!(
            event,
            LayerAnimationEvent::NativeObjectControlUpdated {
                x: 110,
                y: 220,
                alpha_parameter: 256,
                ..
            }
        )));
        assert!(second.iter().any(|event| matches!(
            event,
            LayerAnimationEvent::NativeObjectControlFinished { .. }
        )));
    }

    #[test]
    fn cooperative_poll_advances_only_the_exact_native_control() {
        let mut animations = LayerAnimationSystem::default();
        let first = animations.schedule_native_object_control(
            7,
            (0, 0),
            (100, 0),
            0,
            0,
            0,
            0,
            0,
            0,
            100,
            0,
            0,
            false,
            0,
        );
        let second = animations.schedule_native_object_control(
            8,
            (0, 0),
            (200, 0),
            0,
            0,
            0,
            0,
            0,
            0,
            100,
            0,
            0,
            false,
            0,
        );

        let events = animations.tick_native_control(first, 50);
        assert!(events.iter().any(|event| matches!(
            event,
            LayerAnimationEvent::NativeObjectControlUpdated {
                object_id: 7,
                x: 50,
                ..
            }
        )));
        assert!(!events.iter().any(|event| matches!(
            event,
            LayerAnimationEvent::NativeObjectControlUpdated { object_id: 8, .. }
        )));
        assert!(animations.has_native_control(first));
        assert!(animations.has_native_control(second));

        let events = animations.tick_native_control(second, 50);
        assert!(events.iter().any(|event| matches!(
            event,
            LayerAnimationEvent::NativeObjectControlUpdated {
                object_id: 8,
                x: 100,
                ..
            }
        )));
    }

    #[test]
    fn native_wait_identity_is_per_procedure_not_per_object() {
        let mut animations = LayerAnimationSystem::default();
        let first = animations.schedule_native_object_control(
            7,
            (0, 0),
            (100, 0),
            0,
            0,
            0,
            0,
            0,
            0,
            100,
            0,
            0,
            false,
            0,
        );
        let second = animations.schedule_native_object_control(
            7,
            (0, 0),
            (200, 0),
            0,
            0,
            0,
            0,
            0,
            0,
            200,
            0,
            0,
            false,
            0,
        );
        assert_ne!(first, second);
        assert!(animations.has_native_control(first));
        assert!(animations.has_native_control(second));

        assert!(animations.request_native_control_completion(first));
        animations.tick_native_object_controls(0);
        assert!(!animations.has_native_control(first));
        assert!(animations.has_native_control(second));
        let completion = animations.take_native_control_completion(first);
        // 0x431F00 increments the updater-call counter for the terminal
        // forced invocation before publishing count*1000/duration.
        assert_eq!(completion.progress_per_mille, 10);
        assert_eq!(completion.status, 1);
    }

    #[test]
    fn native_input_skip_commits_endpoint_and_counts_terminal_updater() {
        let mut animations = LayerAnimationSystem::default();
        let control = animations.schedule_native_object_control(
            9,
            (0, 0),
            (100, 0),
            0,
            0,
            256,
            0,
            0,
            0,
            100,
            0,
            0,
            true,
            1808,
        );
        let first = animations.tick_native_object_controls(25);
        assert!(first.iter().any(|event| matches!(
            event,
            LayerAnimationEvent::NativeObjectControlUpdated { x: 25, .. }
        )));

        assert!(animations.request_native_control_completion(control));
        let terminal = animations.tick_native_object_controls(0);
        assert!(terminal.iter().any(|event| matches!(
            event,
            LayerAnimationEvent::NativeObjectControlUpdated {
                x: 100,
                alpha_parameter: 256,
                ..
            }
        )));
        assert!(!animations.has_native_control(control));
        let completion = animations.take_native_control_completion(control);
        assert_eq!(completion.progress_per_mille, 20);
        assert_eq!(completion.status, 1);
    }

    #[test]
    fn native_base_cancel_does_not_snap_to_endpoint() {
        let mut animations = LayerAnimationSystem::default();
        let control = animations.schedule_native_object_control(
            11,
            (0, 0),
            (100, 0),
            0,
            0,
            256,
            0,
            0,
            0,
            100,
            0,
            0,
            false,
            0,
        );
        let partial = animations.tick_native_object_controls(25);
        assert!(partial.iter().any(|event| matches!(
            event,
            LayerAnimationEvent::NativeObjectControlUpdated { x: 25, .. }
        )));
        assert!(animations.abort_native_control(control));
        assert!(!animations.has_native_control(control));
        assert!(animations.tick_native_object_controls(100).is_empty());
        let completion = animations.take_native_control_completion(control);
        assert_eq!(completion.progress_per_mille, 10);
        assert_eq!(completion.status, -1);
    }

    #[test]
    fn native_control_update_deadline_caps_one_procedure_poll() {
        let mut animations = LayerAnimationSystem::default();
        let control = animations.schedule_native_object_control(
            13,
            (0, 0),
            (1_000, 0),
            0,
            0,
            0,
            0,
            0,
            0,
            1_000,
            250,
            1,
            false,
            0,
        );
        // 0x432160 caps one updater invocation to its current 4 ms sampling
        // deadline even when the host arrives 500 ms late.  The scheduler may
        // poll the procedure again, but the subclass itself must not collapse
        // those missing polls into one call.
        // Progress (4 << 24) / 1000 through the linear curve (sub_41A690:
        // a1 / 256 = 262) gives 1000 * 262 >> 16 = 3.
        let first = animations.tick_native_object_controls(500);
        assert!(first.iter().any(|event| matches!(
            event,
            LayerAnimationEvent::NativeObjectControlUpdated { x: 3, .. }
        )));
        assert!(animations.has_native_control(control));
        let second = animations.tick_native_object_controls(500);
        assert!(second.iter().any(|event| matches!(
            event,
            LayerAnimationEvent::NativeObjectControlUpdated { x: 7, .. }
        )));
        assert!(animations.has_native_control(control));
    }

    #[test]
    fn native_natural_completion_returns_updater_count_ratio() {
        let mut animations = LayerAnimationSystem::default();
        let control = animations.schedule_native_object_control(
            14,
            (0, 0),
            (100, 0),
            0,
            0,
            0,
            0,
            0,
            0,
            100,
            0,
            0,
            false,
            0,
        );
        animations.tick_native_object_controls(50);
        animations.tick_native_object_controls(50);
        assert!(!animations.has_native_control(control));
        let completion = animations.take_native_control_completion(control);
        assert_eq!(completion.progress_per_mille, 20);
        assert_eq!(completion.status, 0);
    }

    #[test]
    fn zero_duration_native_control_still_installs_one_millisecond_procedure() {
        let mut animations = LayerAnimationSystem::default();
        let control = animations.schedule_native_object_control(
            15,
            (0, 0),
            (100, 0),
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            false,
            0,
        );
        assert!(animations.has_native_control(control));
        animations.tick_native_object_controls(0);
        assert!(animations.has_native_control(control));
        animations.tick_native_object_controls(1);
        assert!(!animations.has_native_control(control));
    }

    #[test]
    fn object_alpha_uses_native_transparency_units() {
        let mut animations = LayerAnimationSystem::default();
        let mut layers = BTreeMap::new();
        let mut surfaces = BTreeMap::new();
        let mut objects = BTreeMap::new();
        animations.animate_object_alpha(7, 0, 256, 2, 0);
        let events = animations.tick(&mut layers, &mut surfaces, &mut objects);
        assert!(events.iter().any(|event| matches!(
            event,
            LayerAnimationEvent::ObjectAlphaUpdated {
                object_id: 7,
                alpha_parameter: 128
            }
        )));
        assert_eq!(objects.get(&7).unwrap().alpha_parameter(), 128);
        assert!((objects.get(&7).unwrap().opacity() - 0.5).abs() < 0.01);
        let events = animations.tick(&mut layers, &mut surfaces, &mut objects);
        assert!(events.iter().any(|event| matches!(
            event,
            LayerAnimationEvent::ObjectAlphaUpdated {
                object_id: 7,
                alpha_parameter: 256
            }
        )));
        assert_eq!(objects.get(&7).unwrap().alpha_parameter(), 256);
        assert_eq!(objects.get(&7).unwrap().opacity(), 0.0);
    }

    #[test]
    fn legacy_natural_cubic_spline_is_not_default_target_semantics() {
        let spline = NaturalCubicSpline::new(vec![0.0, 10.0, 0.0]);
        assert!((spline.sample(0.25) - 6.875).abs() < 0.0001);
        assert_eq!(spline.sample(0.0), 0.0);
        assert_eq!(spline.sample(0.5), 10.0);
        assert_eq!(spline.sample(1.0), 0.0);
    }
}

use super::*;

const DEBUG_WINDOW_HANDLE_PREFIX: u32 = 0xFF00_0000;
const MAX_DEBUG_WINDOWS: usize = 8;
const CHILD_BITMAP_BASE:i32=0x5100;
const EDIT_BITMAP:i32=0x5200;
const SCROLL_TRACK:i32=0x5300;
const SCROLL_THUMB:i32=0x5301;
const MAX_EDIT_TEXT_BYTES: usize = 255;
const MAX_MODELESS_DIALOGS: usize = 16;
const MODELESS_DIALOG_X: f32 = 48.0;
const MODELESS_DIALOG_Y: f32 = 44.0;
const MODELESS_SLIDER_X: f32 = MODELESS_DIALOG_X + 150.0;
const MODELESS_SLIDER_WIDTH: f32 = 360.0;
const MODELESS_SLIDER_Y: f32 = MODELESS_DIALOG_Y + 54.0;
const MODELESS_SLIDER_ROW_HEIGHT: f32 = 34.0;
const MODELESS_TOGGLE_Y: f32 = MODELESS_DIALOG_Y + 238.0;
const MODELESS_TOGGLE_ROW_HEIGHT: f32 = 30.0;
const MODELESS_CLOSE_Y: f32 = MODELESS_DIALOG_Y + 370.0;

#[derive(Debug, Clone)]
struct NativeDebugBitmapDraw {
    x: i32,
    y: i32,
    bitmap: i32,
    blend_mode: i32,
    alpha: i32,
}

#[derive(Debug, Clone)]
struct NativeDebugTextDraw {
    x: i32,
    y: i32,
    text: String,
    font_id: i32,
    font_size: i32,
    style: i32,
    color: i32,
    blend_mode: i32,
    alpha: i32,
    extent: i32,
}

#[derive(Debug, Clone)]
struct NativeDebugWindow {
    title: String,
    x: i32,
    y: i32,
    width: i32,
    height: i32,
    visible: bool,
    clear_color: i32,
    scroll_flags:u32,
    scroll_range:[i32;2],
    scroll_position:[i32;2],
    // Win32 SCROLLINFO is independent of the cached WM_SCROLL positions
    // returned by B0:1F. SetScrollInfo does not update those cached fields.
    scroll_widget_position:[i32;2],
    scroll_page:[i32;2],
    scroll_enabled:[bool;2],
    redraw_policy:i32,
    bitmap_draws: Vec<NativeDebugBitmapDraw>,
    text_draws: Vec<NativeDebugTextDraw>,
    clipboard_text: String,
}

impl NativeDebugWindow {
    fn new(title: String, x: i32, y: i32, width: i32, height: i32) -> Self {
        Self {
            title,
            x,
            y,
            width,
            height,
            visible: true,
            clear_color: 0,
            scroll_flags:0,
            scroll_range:[0;2],
            scroll_position:[0;2],
            scroll_widget_position:[0;2],scroll_page:[0;2],scroll_enabled:[false;2],
            redraw_policy:1,
            bitmap_draws: Vec::new(),
            text_draws: Vec::new(),
            clipboard_text: String::new(),
        }
    }
}

#[derive(Debug, Clone)]
struct NativeEditControl {
    active: bool,
    visible: bool,
    x: i32,
    y: i32,
    width: i32,
    height: i32,
    font_id: i32,
    font_size: i32,
    max_chars: usize,
    focused: bool,
    font_scale: i32,
    color: i32,
    hide_on_enter: bool,
    printable_input_enabled: bool,
}

impl Default for NativeEditControl {
    fn default() -> Self {
        Self {
            active: false,
            visible: false,
            x: 0,
            y: 0,
            width: 0,
            height: 0,
            font_id: 0,
            font_size: 16,
            max_chars: 256,
            focused: false,
            font_scale: 100,
            color: 0,
            hide_on_enter: false,
            printable_input_enabled: true,
        }
    }
}

#[derive(Debug, Clone)]
struct NativeDialogRequest {
    selector: u16,
    arguments: Vec<String>,
}

#[derive(Debug, Clone)]
struct NativeModelessDialog {
    initial: [i32; 9],
    visible: bool,
    events: VecDeque<[i32; 2]>,
}

#[derive(Debug, Clone)]
struct NativeFontRecord {
    id: i32,
    option: i32,
    metadata_charset: Option<i32>,
}

#[derive(Debug, Clone)]
struct NativeWallpaperRequest {
    path: String,
    style: i32,
    tile: i32,
}

#[derive(Debug, Clone, Copy)]
struct NativeScreenShakeState {
    active: bool,
    direction_mode: i32,
    current_amplitude_fixed: i64,
    cycle_count: u32,
    damping_percent: i32,
    updates_per_second: u32,
    updates_per_cycle: u32,
    cycle_index: u32,
    update_index: u32,
    scalar_fixed: i64,
    step_fixed: i64,
    previous_random: (i64, i64),
    current_random: (i64, i64),
    random_delta: (i64, i64),
    elapsed_ms: u64,
    update_interval_ms: u64,
    immediate_update_pending: bool,
    lock_renderer: bool,
    offset: (f32, f32),
}

impl Default for NativeScreenShakeState {
    fn default() -> Self {
        Self {
            active: false,
            direction_mode: 0,
            current_amplitude_fixed: 0,
            cycle_count: 0,
            damping_percent: 0,
            updates_per_second: 60,
            updates_per_cycle: 1,
            cycle_index: 0,
            update_index: 0,
            scalar_fixed: 0,
            step_fixed: 0,
            previous_random: (0, 0),
            current_random: (0, 0),
            random_delta: (0, 0),
            elapsed_ms: 0,
            update_interval_ms: 16,
            immediate_update_pending: false,
            lock_renderer: false,
            offset: (0.0, 0.0),
        }
    }
}

fn msvc_rand(seed: &mut u32) -> i32 {
    *seed = seed.wrapping_mul(214_013).wrapping_add(2_531_011);
    ((*seed >> 16) & 0x7fff) as i32
}

fn target_shake_random_average(seed: &mut u32, maximum: i64) -> i64 {
    // sub_43CFF0 is used only when CProcShakeScreen's current amplitude is
    // positive. It averages eight independent 30-bit CRT-rand reductions.
    if maximum <= 0 {
        return 0;
    }
    let modulus = maximum.saturating_add(1).min(i64::from(i32::MAX));
    let mut total = 0_i64;
    for _ in 0..8 {
        let high = i64::from(msvc_rand(seed)) << 15;
        let low = i64::from(msvc_rand(seed));
        total = total.saturating_add((high | low) % modulus);
    }
    total >> 3
}

impl NativeScreenShakeState {
    fn configure(
        &mut self,
        direction_mode: i32,
        amplitude: i32,
        oscillation_ticks: i32,
        cycle_count: i32,
        damping_percent: i32,
        updates_per_second: i32,
        lock_renderer: bool,
    ) -> bool {
        // sub_43CCE0 validates exactly these four conditions. In particular,
        // damping is not clamped by the target and update rates above 1000
        // legitimately produce a zero-millisecond procedure deadline step.
        if !(0..=3).contains(&direction_mode)
            || oscillation_ticks < 1
            || cycle_count < 1
            || updates_per_second < 1
            || updates_per_second < oscillation_ticks
        {
            return false;
        }
        self.active = true;
        self.direction_mode = direction_mode;
        self.current_amplitude_fixed = i64::from(amplitude) << 11;
        self.cycle_count = cycle_count as u32;
        self.damping_percent = damping_percent;
        self.updates_per_second = updates_per_second as u32;
        self.updates_per_cycle = self.updates_per_second / oscillation_ticks as u32;
        self.cycle_index = 0;
        self.update_index = 0;
        self.scalar_fixed = 0;
        self.step_fixed = 0;
        self.previous_random = (0, 0);
        self.current_random = (0, 0);
        self.random_delta = (0, 0);
        self.elapsed_ms = 0;
        self.update_interval_ms = 1000 / u64::from(self.updates_per_second);
        // sub_43CCE0 schedules CProcShakeScreen at deadline +0, so the first
        // procedure poll is immediately eligible even if no clock time passed.
        self.immediate_update_pending = true;
        self.lock_renderer = lock_renderer;
        self.offset = (0.0, 0.0);
        true
    }

    fn begin_cycle(&mut self, rng_seed: &mut u32) {
        // 0x43CDD0 resets the triangular scalar at every cycle and derives its
        // step from the *current* damped amplitude.
        self.scalar_fixed = 0;
        self.step_fixed = self.current_amplitude_fixed.saturating_mul(4)
            / i64::from(self.updates_per_cycle.max(1));

        self.previous_random = self.current_random;
        if self.direction_mode == 3 && self.current_amplitude_fixed > 0 {
            let x = target_shake_random_average(rng_seed, self.current_amplitude_fixed);
            let y = target_shake_random_average(rng_seed, self.current_amplitude_fixed);
            self.current_random = match self.cycle_index & 3 {
                0 => (-x, -y),
                1 => (x, y),
                2 => (-x, y),
                _ => (x, -y),
            };
        } else if self.direction_mode == 3 {
            self.current_random = (0, 0);
        }
        self.random_delta = (
            self.current_random.0 - self.previous_random.0,
            self.current_random.1 - self.previous_random.1,
        );
    }

    fn advance_one_update(&mut self, rng_seed: &mut u32) {
        if !self.active {
            return;
        }
        if self.update_index == 0 {
            self.begin_cycle(rng_seed);
        }

        // Exact scalar portion of 0x43CDD0: add 4*A/N, then reflect around
        // +/-A without discarding overshoot.
        self.scalar_fixed = self.scalar_fixed.saturating_add(self.step_fixed);
        let amplitude = self.current_amplitude_fixed;
        if self.scalar_fixed > amplitude || self.scalar_fixed < -amplitude {
            self.step_fixed = self.step_fixed.saturating_neg();
        }
        if self.scalar_fixed > amplitude {
            self.scalar_fixed = amplitude
                .saturating_mul(2)
                .saturating_sub(self.scalar_fixed);
        }
        if self.scalar_fixed < -amplitude {
            self.scalar_fixed = self
                .scalar_fixed
                .saturating_neg()
                .saturating_sub(amplitude.saturating_mul(2));
        }

        // The local masks in 0x43CDD0 are:
        //   mode 0: (scalar, 0)
        //   mode 1: (0, scalar)
        //   mode 2: (scalar, scalar)
        //   mode 3: interpolate two CRT-random endpoints.
        let (x_fixed, y_fixed) = match self.direction_mode {
            0 => (self.scalar_fixed, 0),
            1 => (0, self.scalar_fixed),
            2 => (self.scalar_fixed, self.scalar_fixed),
            3 => {
                let n = i64::from(self.updates_per_cycle.max(1));
                let i = i64::from(self.update_index);
                (
                    self.previous_random.0 + self.random_delta.0.saturating_mul(i) / n,
                    self.previous_random.1 + self.random_delta.1.saturating_mul(i) / n,
                )
            }
            _ => (0, 0),
        };
        self.offset = ((x_fixed >> 12) as f32, (y_fixed >> 12) as f32);

        self.update_index = self.update_index.saturating_add(1);
        if self.update_index >= self.updates_per_cycle {
            self.cycle_index = self.cycle_index.saturating_add(1);
            self.current_amplitude_fixed = self
                .current_amplitude_fixed
                .saturating_mul(i64::from(100 - self.damping_percent))
                / 100;
            self.update_index = 0;
            if self.cycle_index >= self.cycle_count {
                self.cancel();
            }
        }
    }

    fn cancel(&mut self) {
        self.active = false;
        self.scalar_fixed = 0;
        self.step_fixed = 0;
        self.immediate_update_pending = false;
        self.offset = (0.0, 0.0);
    }

    fn tick(&mut self, elapsed_ms: u64, rng_seed: &mut u32) -> (f32, f32) {
        if !self.active {
            self.offset = (0.0, 0.0);
            return self.offset;
        }

        self.elapsed_ms = self.elapsed_ms.saturating_add(elapsed_ms);
        if self.immediate_update_pending {
            self.immediate_update_pending = false;
            self.advance_one_update(rng_seed);
        }

        if self.update_interval_ms == 0 {
            // A zero deadline increment remains continuously due in the target,
            // but one host pass must stay finite. One procedure update per pass
            // preserves forward progress without inventing a wall-clock delay.
            if self.active {
                self.advance_one_update(rng_seed);
            }
            return self.offset;
        }

        while self.active && self.elapsed_ms >= self.update_interval_ms {
            self.elapsed_ms -= self.update_interval_ms;
            self.advance_one_update(rng_seed);
        }
        self.offset
    }
}

#[derive(Debug)]
pub(super) struct NativeUserState {
    pub(super) inspectors: crate::property_inspector::InspectorWindows,
    pub(super) text: String,
    edit_alignment:u32,
    last_edit_image:Option<(String,[i32;6])>,
    /// dword_5666E4 (UserB0:83); `None` uses the built-in caption.
    message_box_title: Option<String>,
    pub(super) cursor_object: Option<(i32, i32, i32)>,
    debug_windows: [Option<NativeDebugWindow>; MAX_DEBUG_WINDOWS],
    edit: NativeEditControl,
    dialog_history: VecDeque<NativeDialogRequest>,
    blocking_message: Option<(String, String)>,
    next_dialog_result: Option<i32>,
    modeless_dialogs: BTreeMap<i32, NativeModelessDialog>,
    next_modeless_dialog: i32,
    next_font_id: i32,
    fonts: BTreeMap<String, NativeFontRecord>,
    font_resources: BTreeSet<String>,
    font_aliases: BTreeMap<String, String>,
    wallpaper: Option<NativeWallpaperRequest>,
    crt_rng_seed: u32,
    screen_shake: NativeScreenShakeState,
    last_window_bitmap_draw: Option<(i32, i32, i32)>,
}

impl Default for NativeUserState {
    fn default() -> Self {
        Self {
            edit_alignment:0,
            last_edit_image:None,
            inspectors: Default::default(),
            text: String::new(),
            message_box_title: None,
            cursor_object: None,
            debug_windows: std::array::from_fn(|_| None),
            edit: NativeEditControl::default(),
            dialog_history: VecDeque::new(),
            blocking_message: None,
            next_dialog_result: None,
            modeless_dialogs: BTreeMap::new(),
            next_modeless_dialog: 1,
            // sub_468B70 resets the registry at startup and interns the
            // two stock faces first: id 0 Gothic, id 1 Mincho (the
            // Japanese names on a Japanese system, sub_46F6F0).
            next_font_id: 2,
            fonts: BTreeMap::from([
                (
                    "ＭＳ ゴシック".to_string(),
                    NativeFontRecord {
                        id: 0,
                        option: -1,
                        metadata_charset: None,
                    },
                ),
                (
                    "ＭＳ 明朝".to_string(),
                    NativeFontRecord {
                        id: 1,
                        option: -1,
                        metadata_charset: None,
                    },
                ),
            ]),
            font_resources: BTreeSet::new(),
            font_aliases: BTreeMap::new(),
            wallpaper: None,
            crt_rng_seed: 1,
            screen_shake: NativeScreenShakeState::default(),
            last_window_bitmap_draw: None,
        }
    }
}

impl NativeUserState {
    /// sub_46BC80: the configured caption or aEthornellBurik_0.
    pub(super) fn message_box_caption(&self) -> String {
        self.message_box_title.clone().unwrap_or_else(|| {
            "Ethornell - BURIKO General Interpreter ( Version : 1.622 - Compatibility : 1.72 )"
                .to_string()
        })
    }

    fn begin_blocking_message(&mut self, title: String, message: String) {
        self.blocking_message = Some((title, message));
    }

    pub(super) fn has_blocking_message(&self) -> bool {
        self.blocking_message.is_some()
    }

    pub(super) fn acknowledge_blocking_message(&mut self) -> bool {
        self.blocking_message.take().is_some()
    }

    fn debug_slot(handle: i32) -> Option<usize> {
        let raw = handle as u32;
        if !matches!(raw&0xffff_ff00,DEBUG_WINDOW_HANDLE_PREFIX|0xf8000000) {
            return None;
        }
        let slot = (raw & 0xFF) as usize;
        (slot < MAX_DEBUG_WINDOWS).then_some(slot)
    }

    fn allocate_debug_window(
        &mut self,
        title: String,
        x: i32,
        y: i32,
        width: i32,
        height: i32,
    ) -> Option<i32> {
        let slot = self.debug_windows.iter().position(Option::is_none)?;
        self.debug_windows[slot] = Some(NativeDebugWindow::new(title, x, y, width, height));
        Some((DEBUG_WINDOW_HANDLE_PREFIX | slot as u32) as i32)
    }

    fn debug_window(&self, handle: i32) -> Option<&NativeDebugWindow> {
        self.debug_windows.get(Self::debug_slot(handle)?)?.as_ref()
    }

    fn debug_window_mut(&mut self, handle: i32) -> Option<&mut NativeDebugWindow> {
        self.debug_windows
            .get_mut(Self::debug_slot(handle)?)?
            .as_mut()
    }

    /// sub_468BB0: the face name interned under `id`.
    pub(crate) fn font_face(&self, id: i32) -> Option<&str> {
        self.fonts
            .iter()
            .find(|(_, record)| record.id == id)
            .map(|(name, _)| self.font_aliases.get(name).map(String::as_str).unwrap_or(name.as_str()))
    }

    /// sub_469110: whether a face id was interned.
    pub(super) fn font_name_known(&self, id: i32) -> bool {
        self.fonts.values().any(|record| record.id == id)
    }

    fn intern_font(&mut self, name: String, option: i32) -> i32 {
        // sub_468A70 registers metadata only for option 0 (charset 128)
        // or option 1 (charset/default value 0). Option -1 and all other
        // values still intern the stable font id without metadata.
        let metadata_charset = match option {
            0 => Some(128),
            1 => Some(0),
            _ => None,
        };
        if let Some(record) = self.fonts.get_mut(&name) {
            if option != -1 {
                record.option = option;
                record.metadata_charset = metadata_charset;
            }
            return record.id;
        }
        let id = self.next_font_id;
        self.next_font_id = self.next_font_id.saturating_add(1).max(1);
        self.fonts.insert(
            name,
            NativeFontRecord {
                id,
                option,
                metadata_charset,
            },
        );
        id
    }

    fn record_dialog(&mut self, selector: u16, args: &[ethornell_vm::Value]) -> i32 {
        let arguments = args
            .iter()
            .map(|value| match value {
                ethornell_vm::Value::Str(text) => text.clone(),
                ethornell_vm::Value::Int(value) => value.to_string(),
                ethornell_vm::Value::Ptr(value) => format!("0x{value:08X}"),
                ethornell_vm::Value::Func { offset, .. } => format!("func@0x{offset:08X}"),
                ethornell_vm::Value::Program(program) => program
                    .script_name
                    .as_deref()
                    .unwrap_or("<program>")
                    .to_string(),
                ethornell_vm::Value::None => String::new(),
            })
            .collect();
        self.dialog_history.push_back(NativeDialogRequest {
            selector,
            arguments,
        });
        while self.dialog_history.len() > 32 {
            self.dialog_history.pop_front();
        }
        self.next_dialog_result.take().unwrap_or(0)
    }

    fn create_modeless_dialog(&mut self, initial: [i32; 9]) -> Option<i32> {
        if self.modeless_dialogs.len() >= MAX_MODELESS_DIALOGS {
            return None;
        }
        let mut handle = self.next_modeless_dialog.max(1);
        while self.modeless_dialogs.contains_key(&handle) {
            handle = handle.saturating_add(1).max(1);
        }
        self.next_modeless_dialog = handle.saturating_add(1).max(1);
        let mut initial = initial;
        for value in &mut initial[..5] {
            *value = (*value).clamp(0, 128);
        }
        for value in &mut initial[5..] {
            *value = i32::from(*value != 0);
        }
        self.modeless_dialogs.insert(
            handle,
            NativeModelessDialog {
                initial,
                visible: false,
                events: VecDeque::new(),
            },
        );
        Some(handle)
    }

    fn has_visible_modeless_dialog(&self) -> bool {
        self.modeless_dialogs.values().any(|dialog| dialog.visible)
    }

    fn modeless_dialog_text_lines(&self) -> Vec<(String, f32, f32, f32)> {
        let Some((&handle, dialog)) = self
            .modeless_dialogs
            .iter()
            .rev()
            .find(|(_, dialog)| dialog.visible)
        else {
            return Vec::new();
        };
        let mut lines = vec![
            (
                format!("BGI modeless control #{handle}"),
                MODELESS_DIALOG_X,
                MODELESS_DIALOG_Y,
                24.0,
            ),
            (
                "Five native 0..128 controls".to_string(),
                MODELESS_DIALOG_X,
                MODELESS_DIALOG_Y + 27.0,
                16.0,
            ),
        ];
        for index in 0..5 {
            let value = dialog.initial[index].clamp(0, 128);
            let filled = usize::try_from(value).unwrap_or_default() * 24 / 128;
            let bar = format!("{}{}", "=".repeat(filled), "-".repeat(24 - filled));
            lines.push((
                format!("Value {index}: [{bar}] {value:3}"),
                MODELESS_DIALOG_X,
                MODELESS_SLIDER_Y + index as f32 * MODELESS_SLIDER_ROW_HEIGHT,
                20.0,
            ));
        }
        for index in 5..9 {
            let value = dialog.initial[index] != 0;
            let (left_value, right_value) = if index < 7 { (0, 1) } else { (1, 0) };
            let left = if i32::from(value) == left_value {
                "(*)"
            } else {
                "( )"
            };
            let right = if i32::from(value) == right_value {
                "(*)"
            } else {
                "( )"
            };
            lines.push((
                format!("Option {index}: {left} {left_value}     {right} {right_value}"),
                MODELESS_DIALOG_X,
                MODELESS_TOGGLE_Y + (index - 5) as f32 * MODELESS_TOGGLE_ROW_HEIGHT,
                20.0,
            ));
        }
        lines.push((
            "[ Close ]".to_string(),
            MODELESS_DIALOG_X,
            MODELESS_CLOSE_Y,
            22.0,
        ));
        lines
    }

    fn handle_modeless_pointer(&mut self, x: f32, y: f32, pressed: bool) -> bool {
        let Some((_, dialog)) = self
            .modeless_dialogs
            .iter_mut()
            .rev()
            .find(|(_, dialog)| dialog.visible)
        else {
            return false;
        };

        // The target creates this control surface with CreateDialogParamA as a
        // separate modeless HWND. The portable overlay must therefore never
        // capture unrelated coordinates in the main game window. The previous
        // implementation returned true for every point whenever the dialog was
        // visible, which allowed hover to work while swallowing all clicks.
        let dialog_left = MODELESS_DIALOG_X;
        let dialog_top = MODELESS_DIALOG_Y;
        let dialog_right = MODELESS_DIALOG_X + 520.0;
        let dialog_bottom = MODELESS_CLOSE_Y + 30.0;
        if x < dialog_left || x > dialog_right || y < dialog_top || y > dialog_bottom {
            return false;
        }
        if !pressed {
            return true;
        }
        for index in 0..5 {
            let row_y = MODELESS_SLIDER_Y + index as f32 * MODELESS_SLIDER_ROW_HEIGHT;
            if y >= row_y - 5.0
                && y < row_y + 27.0
                && (MODELESS_SLIDER_X..=MODELESS_SLIDER_X + MODELESS_SLIDER_WIDTH).contains(&x)
            {
                let value = (((x - MODELESS_SLIDER_X) / MODELESS_SLIDER_WIDTH) * 128.0)
                    .round()
                    .clamp(0.0, 128.0) as i32;
                dialog.initial[index] = value;
                dialog.events.push_back([index as i32, value]);
                return true;
            }
        }
        for index in 5..9 {
            let row_y = MODELESS_TOGGLE_Y + (index - 5) as f32 * MODELESS_TOGGLE_ROW_HEIGHT;
            if y >= row_y - 4.0
                && y < row_y + 25.0
                && (MODELESS_DIALOG_X..=MODELESS_DIALOG_X + 520.0).contains(&x)
            {
                let left = x < MODELESS_DIALOG_X + 285.0;
                let value = if index < 7 {
                    i32::from(!left)
                } else {
                    i32::from(left)
                };
                dialog.initial[index] = value;
                dialog.events.push_back([index as i32, value]);
                return true;
            }
        }
        if (MODELESS_CLOSE_Y - 5.0..MODELESS_CLOSE_Y + 30.0).contains(&y)
            && (MODELESS_DIALOG_X..MODELESS_DIALOG_X + 150.0).contains(&x)
        {
            dialog.visible = false;
            dialog.events.push_back([-1, 0]);
        }
        true
    }
}

const CHILD_WINDOW_HANDLE: &str = "無効なチャイルドウィンドウハンドルが指定されました";

impl RuntimeTraceApi {
    fn paint_native_child_bitmap(&mut self,handle:i32,bitmap:i32,x:i32,y:i32,rect:Option<[i32;4]>,mode:i32,level:i32)->ethornell_vm::VmResult<()> {
        let Some(mut source)=self.graph_bitmap_image(bitmap) else {return Err(ethornell_vm::VmError::Runtime("native child-window source bitmap missing".into()))};
        if let Some([sx,sy,width,height])=rect {
            if sx<0||sy<0||width<=0||height<=0||sx as i64+width as i64>source.width as i64||sy as i64+height as i64>source.height as i64 {
                return Err(ethornell_vm::VmError::Runtime("native child-window source rectangle outside bitmap".into()));
            }
            source=crop_decoded_image(&source,RuntimeClipRect {x:sx as f32,y:sy as f32,width:width as f32,height:height as f32});
        }
        let format=self.bitmap_formats.get(&bitmap).copied().unwrap_or(2);
        self.paint_native_child_image(handle,&source,format,x,y,mode,level)
    }
    fn paint_native_child_image(&mut self,handle:i32,source:&DecodedImage,format:i32,x:i32,y:i32,mode:i32,level:i32)->ethornell_vm::VmResult<()> {
        let Some(slot)=NativeUserState::debug_slot(handle).filter(|&slot|self.native_user.debug_windows[slot].is_some()) else {return Err(ethornell_vm::VmError::Runtime(CHILD_WINDOW_HANDLE.into()))};
        let destination=CHILD_BITMAP_BASE+slot as i32;
        let Some(mut image)=self.graph_bitmap_image(destination) else {return Err(ethornell_vm::VmError::Runtime("native child-window backing bitmap missing".into()))};
        if !bitmap_blend::blit_mode_with_options(&mut image,1,&source,format,x,y,mode,level,self.native_additive_coverage) {
            return Err(ethornell_vm::VmError::Runtime("unsupported native child-window bitmap blend".into()));
        }
        self.replace_graph_bitmap_pixels(destination,image);Ok(())
    }

    pub(super) fn native_child_property(&self,handle:i32,property:i32)->std::result::Result<i32,i32> {
        let Some(w)=self.native_user.debug_window(handle) else {return Err(-1)};
        match property {0|1=>Ok(w.scroll_range[property as usize]),2|3=>Ok(w.scroll_position[(property-2) as usize]),_=>Err(i32::MIN+12)}
    }

    pub(super) fn native_control_draw_items(&self)->Vec<RuntimeGraphDrawItem> {
        let mut items=Vec::new();
        let mut append=|bitmap:i32,x:i32,y:i32,width:i32,height:i32,z:i32| {
            let Some((key,region))=self.resource_image_region(bitmap) else {return};
            items.push(RuntimeGraphDrawItem {owner_object:None,key:key.into(),x:x as f32,y:y as f32,width:width as f32,height:height as f32,
                src_x:region.x,src_y:region.y,src_width:region.width,src_height:region.height,opacity:1.,ignore_source_alpha:true,
                rotation_degrees:0.,destination_quad:None,linear_sampling:false,clip:Some(RuntimeClipRect {x:x as f32,y:y as f32,width:width as f32,height:height as f32}),
                z,blend_mode:0,order_serial:0,hit_id:0});
        };
        for (slot,window) in self.native_user.debug_windows.iter().enumerate() {
            if let Some(w)=window.as_ref().filter(|w|w.visible) {
                let z=25000+slot as i32*3;
                append(CHILD_BITMAP_BASE+slot as i32,w.x,w.y,w.width,w.height,z);
                for axis in 0..2 {
                    if w.scroll_flags&(1<<axis)==0 {continue}
                    let length=if axis==0 {w.width} else {w.height};
                    let range=if w.scroll_enabled[axis] {w.scroll_range[axis]} else {0};
                    let page=w.scroll_page[axis].max(0);
                    let thumb=if range>0 {((length as i64*page as i64)/(range as i64+page as i64).max(1)) as i32} else {length};
                    let thumb=thumb.clamp(14,length);
                    let offset=if range>0 {((length-thumb) as i64*w.scroll_widget_position[axis] as i64/range as i64) as i32} else {0};
                    let (x,y,width,height)=if axis==0 {(w.x,w.y+w.height-14,length,14)} else {(w.x+w.width-14,w.y,14,length)};
                    append(SCROLL_TRACK,x,y,width,height,z+1);
                    let (x,y,width,height)=if axis==0 {(x+offset,y,thumb,14)} else {(x,y+offset,14,thumb)};
                    append(SCROLL_THUMB,x,y,width,height,z+2);
                }
            }
        }
        let edit=&self.native_user.edit;
        if edit.active&&edit.visible {append(EDIT_BITMAP,edit.x,edit.y,edit.width,edit.height,25030);}
        for [x,y,width,height] in self.native_user.inspectors.panels() {append(property_inspector::PANEL_BITMAP,x as i32,y as i32,width as i32,height as i32,29990);}
        items
    }

    fn refresh_native_edit_surface(&mut self) {
        let edit=&self.native_user.edit;
        if !edit.active||!edit.visible||edit.width<=0||edit.height<=0 {return}
        let signature=(self.native_user.text.clone(),[edit.width,edit.height,edit.font_id,edit.font_size,edit.font_scale,edit.color]);
        let signature_text=format!("{}|{}",self.native_user.edit_alignment,signature.0);
        let signature=(signature_text,signature.1);
        if self.native_user.last_edit_image.as_ref()==Some(&signature) {return}
        let (width,height,size,scale,color)=(edit.width,edit.height,edit.font_size,edit.font_scale,edit.color as u32);
        let advance=(snapshot::measure_text_advance(&self.native_user.text,size as f32,0.) as f32*scale as f32/100.) as i32;
        let x=match self.native_user.edit_alignment {1=>(width-advance)/2,2=>width-advance,_=>0}.max(0);
        let mut image=DecodedImage {width:width as u32,height:height as u32,rgba:vec![255;(width*height*4) as usize]};
        snapshot::rasterize_bitmap_text(&mut image,&self.native_user.text,x,0,size as f32,0.,scale as f32,
            [((color>>16)&255) as f32/255.,((color>>8)&255) as f32/255.,(color&255) as f32/255.,1.]);
        self.store_runtime_bitmap(EDIT_BITMAP,image,1);self.native_user.last_edit_image=Some(signature);
    }

    pub(super) fn has_active_native_screen_shake(&self) -> bool {
        self.native_user.screen_shake.active
    }

    pub(super) fn cancel_native_screen_shake(&mut self) {
        self.native_user.screen_shake.cancel();
        self.screen_shake_offset = (0.0, 0.0);
    }

    pub(super) fn seed_native_crt_rng(&mut self, seed: u32) {
        self.native_user.crt_rng_seed = seed;
    }

    pub(super) fn next_native_crt_rand(&mut self) -> i32 {
        msvc_rand(&mut self.native_user.crt_rng_seed)
    }

    pub(super) fn tick_native_user_state(&mut self, elapsed_ms: u64) {
        self.refresh_native_edit_surface();
        if self.native_user.debug_windows.iter().flatten().any(|w|w.scroll_flags!=0)&&!self.graph_resources.contains_key(&SCROLL_TRACK) {
            self.store_runtime_bitmap(SCROLL_TRACK,DecodedImage {width:1,height:1,rgba:vec![80,80,80,255]},1);
            self.store_runtime_bitmap(SCROLL_THUMB,DecodedImage {width:1,height:1,rgba:vec![190,190,190,255]},1);
        }
        let native_user = &mut self.native_user;
        self.screen_shake_offset = native_user
            .screen_shake
            .tick(elapsed_ms, &mut native_user.crt_rng_seed);
        if let Some((object, offset_x, offset_y)) = self.native_user.cursor_object
            && let Some((x, y)) = self.mouse_pos
        {
            let x = x.round() as i32 + offset_x;
            let y = y.round() as i32 + offset_y;
            let properties = self.graph_object_properties.entry(object).or_default();
            properties.native.position_x = x;
            properties.native.position_y = y;
            self.display_tree
                .set_local_position(object, x as f32, y as f32);
        }
    }

    pub(super) fn handle_native_edit_key(&mut self, code: KeyCode) -> bool {
        if self.native_user.inspectors.key(code) {return true;}
        let edit = &mut self.native_user.edit;
        if !edit.active || !edit.visible || !edit.focused {
            return false;
        }
        match code {
            KeyCode::Tab => {
                edit.visible = false;
                edit.focused = false;
                true
            }
            KeyCode::Enter | KeyCode::NumpadEnter => {
                edit.focused = false;
                if edit.hide_on_enter {
                    edit.visible = false;
                }
                true
            }
            KeyCode::Backspace => {
                self.native_user.text.pop();
                true
            }
            _ => false,
        }
    }

    pub(super) fn append_native_edit_text(&mut self, text: &str) -> bool {
        if self.native_user.inspectors.text(text) {return true;}
        let edit = &self.native_user.edit;
        if !edit.active || !edit.visible || !edit.focused || !edit.printable_input_enabled {
            return false;
        }

        let mut candidate = self.native_user.text.clone();
        for ch in text.chars().filter(|ch| !ch.is_control()) {
            if candidate.chars().count() >= edit.max_chars {
                break;
            }
            let next = format!("{candidate}{ch}");
            if encoding_rs::SHIFT_JIS.encode(&next).0.len() > MAX_EDIT_TEXT_BYTES {
                break;
            }
            candidate.push(ch);
        }
        self.native_user.text = candidate;
        true
    }

    pub(super) fn has_visible_user_modeless_dialog(&self) -> bool {
        self.native_user.has_visible_modeless_dialog() || self.native_user.inspectors.focused()
    }

    pub(super) fn user_modeless_dialog_text_lines(&self) -> Vec<(String, f32, f32, f32)> {
        let mut lines=self.native_user.modeless_dialog_text_lines();
        lines.extend(self.native_user.inspectors.text_lines());lines
    }

    /// 06CB40: wheel increments use the cached Y position, whereas a
    /// scrollbar gesture starts with SCROLLINFO. Both update the cached
    /// query result only after actual input. Rendering is never auto-scrolled:
    /// the native BP program reads the position and redraws its own contents.
    pub(super) fn handle_native_child_wheel(&mut self,delta:f32)->bool {
        let Some((x,y))=self.mouse_pos else {return false};
        let Some(w)=self.native_user.debug_windows.iter_mut().rev().flatten().find(|w|w.visible&&x>=w.x as f32&&y>=w.y as f32&&x<(w.x+w.width) as f32&&y<(w.y+w.height) as f32) else {return false};
        if w.scroll_flags&2!=0&&delta!=0. {
            // The original modulo is undefined for step=0. Keep the control
            // stationary for nonpositive steps instead of dividing by zero.
            let step=w.redraw_policy;
            if step>0 {
                let value=w.scroll_position[1].saturating_add(if delta>0. {-step} else {step});
                let value=value-value%step;
                let value=value.clamp(0,w.scroll_range[1]);
                w.scroll_position[1]=value;w.scroll_widget_position[1]=value;
            }
        }
        true
    }
    fn handle_native_child_pointer(&mut self,x:f32,y:f32,pressed:bool)->bool {
        let Some(w)=self.native_user.debug_windows.iter_mut().rev().flatten().find(|w|w.visible&&x>=w.x as f32&&y>=w.y as f32&&x<(w.x+w.width) as f32&&y<(w.y+w.height) as f32) else {return false};
        if pressed {
            let local=[x-w.x as f32,y-w.y as f32];
            for axis in 0..2 {
                if w.scroll_flags&(1<<axis)==0||!w.scroll_enabled[axis] {continue}
                let in_bar=if axis==0 {local[1]>=w.height as f32-14.} else {local[0]>=w.width as f32-14.};
                if in_bar {
                    let length=if axis==0 {w.width} else {w.height};
                    let range=w.scroll_range[axis];let page=w.scroll_page[axis].max(0);
                    let thumb=((length as i64*page as i64)/(range as i64+page as i64).max(1)) as i32;
                    let thumb=thumb.clamp(14,length);
                    let value=if length==thumb {0} else {((local[axis]-thumb as f32*0.5)/(length-thumb) as f32*range as f32).round() as i32};
                    let value=value.clamp(0,range);w.scroll_widget_position[axis]=value;w.scroll_position[axis]=value;
                }
            }
        }
        true
    }

    pub(super) fn handle_user_modeless_pointer(&mut self, x: f32, y: f32, pressed: bool) -> bool {
        let edit=&mut self.native_user.edit;
        if edit.active&&edit.visible {
            let inside=x>=edit.x as f32&&y>=edit.y as f32&&x<(edit.x+edit.width) as f32&&y<(edit.y+edit.height) as f32;
            if pressed {edit.focused=inside;}
            if inside {return true;}
        }

        self.native_user.inspectors.pointer(x,y,pressed) || self.handle_native_child_pointer(x,y,pressed) || self.native_user.handle_modeless_pointer(x, y, pressed)
    }

    pub(super) fn create_user_modeless_dialog(&mut self, initial: [i32; 9]) -> Option<i32> {
        self.native_user.create_modeless_dialog(initial)
    }

    pub(super) fn close_user_modeless_dialog(&mut self, handle: i32) -> bool {
        self.native_user.modeless_dialogs.remove(&handle).is_some()
    }

    pub(super) fn set_user_modeless_dialog_visible(&mut self, handle: i32, visible: bool) -> bool {
        let Some(dialog) = self.native_user.modeless_dialogs.get_mut(&handle) else {
            return false;
        };
        dialog.visible = visible;
        true
    }

    pub(super) fn poll_user_modeless_dialog(
        &mut self,
        handle: i32,
    ) -> std::result::Result<Option<[i32; 2]>, ()> {
        let Some(dialog) = self.native_user.modeless_dialogs.get_mut(&handle) else {
            return Err(());
        };
        Ok(dialog.events.pop_front())
    }

    pub(super) fn dispatch_native_user(
        &mut self,
        call: &mut ethornell_vm::NativeCallFrame,
    ) -> Option<ethornell_vm::VmResult<ethornell_vm::Value>> {
        let (group, id) = (call.group(), call.id());
        let stack = call.args_mut();
        if group != 0xB0 {
            return None;
        }

        let value = match id {
            // sub_478030 resolves one graph resource and draws it directly to
            // the parent window HDC at the supplied coordinates.
            0x00 => {
                let resource = pop_int_value(stack).unwrap_or_default();
                let y = pop_int_value(stack).unwrap_or_default();
                let x = pop_int_value(stack).unwrap_or_default();
                // sub_478B30: sub_4986D0, then sub_461F20's missing bitmap.
                if let Err(error) = target_checks::bitmap(resource) {
                    return Some(Err(error));
                }
                if !self.graph_resources.contains_key(&resource) {
                    return Some(Err(ethornell_vm::VmError::Runtime(format!(
                        "指定されたビットマップ [ {resource} ] は存在しません"
                    ))));
                }
                self.native_user.last_window_bitmap_draw = Some((x, y, resource));
                if self.graph_resources.contains_key(&resource)
                    || self.graph_surfaces.contains_key(&resource)
                {
                    let surface = self
                        .graph_surfaces
                        .entry(resource)
                        .or_insert_with(|| RuntimeSurface::bitmap(resource, 1.0, 1.0));
                    surface.x = x as f32;
                    surface.y = y as f32;
                    surface.enabled = true;
                }
                ethornell_vm::Value::None
            }
            // sub_461690 centers the target main window when not fullscreen.
            0x02 => {
                if self.window_mode != 0 {
                    ethornell_vm::Value::Int(0)
                } else {
                    let width = self.window_surface_width.max(1);
                    let height = self.window_surface_height.max(1);
                    let x = (self.screen_width - width).max(0) / 2;
                    let y = (self.screen_height - height).max(0) / 2;
                    self.pending_window_position = Some((x, y));
                    ethornell_vm::Value::Int(1)
                }
            }
            // sub_46F930 validates that the complete window remains inside
            // the selected monitor/work area before SetWindowPos.
            0x03 => {
                let y = pop_int_value(stack).unwrap_or_default();
                let x = pop_int_value(stack).unwrap_or_default();
                let valid = self.window_mode == 0
                    && x >= 0
                    && y >= 0
                    && x.saturating_add(self.window_surface_width) <= self.screen_width
                    && y.saturating_add(self.window_surface_height) <= self.screen_height;
                if valid {
                    self.pending_window_position = Some((x, y));
                }
                ethornell_vm::Value::Int(i32::from(valid))
            }
            // sub_48EBD0 binds one display object to the physical cursor and
            // stores two signed offsets. A zero object removes the binding.
            0x04 => {
                let y_offset = pop_int_value(stack).unwrap_or_default();
                let x_offset = pop_int_value(stack).unwrap_or_default();
                let object = pop_int_value(stack).unwrap_or_default();
                // sub_478C70 -> sub_48F6D0: an unknown object is fatal.
                if object != 0 && !self.graph_handle_exists(object) {
                    return Some(Err(ethornell_vm::VmError::Runtime(
                        "無効なスプライトハンドルが指定されました".into(),
                    )));
                }
                if object == 0 {
                    self.native_user.cursor_object = None;
                    let visible = if self.cursor_idle_timeout_ms != 0 {
                        self.cursor_idle_visible
                    } else {
                        true
                    };
                    self.set_cursor_visible(visible);
                } else {
                    self.native_user.cursor_object = Some((object, x_offset, y_offset));
                    self.set_cursor_visible(false);
                    if self.cursor_idle_timeout_ms != 0 {
                        self.set_graph_object_enabled(object, self.cursor_idle_visible);
                    }
                    self.tick_native_user_state(0);
                }
                ethornell_vm::Value::None
            }
            0x05 => {
                let timeout_ms = pop_int_value(stack).unwrap_or_default();
                self.set_cursor_idle_timeout(timeout_ms);
                ethornell_vm::Value::None
            }
            0x06 => {
                let visible = self.query_cursor_visibility_latch(platform::cursor_suppressed());
                ethornell_vm::Value::Int(i32::from(visible))
            }
            // CProcShakeScreen. The VM owns the procedure boundary; the host
            // owns the per-native-tick motion state and completion predicate.
            0x08 => {
                let lock_renderer = pop_int_value(stack).unwrap_or_default() != 0;
                let updates_per_second = pop_int_value(stack).unwrap_or_default();
                let damping_percent = pop_int_value(stack).unwrap_or_default();
                let cycle_count = pop_int_value(stack).unwrap_or_default();
                let oscillation_ticks = pop_int_value(stack).unwrap_or_default();
                let amplitude = pop_int_value(stack).unwrap_or_default();
                let direction_mode = pop_int_value(stack).unwrap_or_default();
                if !self.native_user.screen_shake.configure(
                    direction_mode,
                    amplitude,
                    oscillation_ticks,
                    cycle_count,
                    damping_percent,
                    updates_per_second,
                    lock_renderer,
                ) {
                    // sub_478D00 -> sub_43D0A0.
                    let text = if direction_mode as u32 >= 4 {
                        format!("無効な振動パターン [ {direction_mode} ] が指定されました")
                    } else if oscillation_ticks < 1 {
                        format!("無効な周波数 [ {oscillation_ticks} ] が指定されました")
                    } else if cycle_count < 1 {
                        format!("無効な繰り返し回数 [ {cycle_count} ] が指定されました")
                    } else if updates_per_second >= 1 {
                        format!(
                            "無効なフレームレート [ {updates_per_second} ] が指定されました\n\nフレームレートは周波数より高くなければなりません"
                        )
                    } else {
                        format!("無効なフレームレート [ {updates_per_second} ] が指定されました")
                    };
                    return Some(Err(ethornell_vm::VmError::Runtime(text)));
                }
                ethornell_vm::Value::None
            }
            // Eight target auxiliary windows, tagged 0xFF000000 | slot.
            0x10 | 0x12 => {
                let flags=if id==0x12 {pop_int_value(stack).unwrap_or_default() as u32} else {0};
                let height = pop_int_value(stack).unwrap_or_default();
                let width = pop_int_value(stack).unwrap_or_default();
                let y = pop_int_value(stack).unwrap_or_default();
                let x = pop_int_value(stack).unwrap_or_default();
                let title = pop_string_value(stack).unwrap_or_default();
                // sub_478ED0 -> sub_42FEC0: 32..=2048 each way, eight slots.
                if !(32..=2048).contains(&width) || !(32..=2048).contains(&height) {
                    return Some(Err(ethornell_vm::VmError::Runtime(format!(
                        "無効なウィンドウサイズ [ {width} , {height} ] が指定されました"
                    ))));
                }
                let Some(handle) = self.native_user.allocate_debug_window(title, x, y, width, height)
                else {
                    return Some(Err(ethornell_vm::VmError::Runtime(
                        "これ以上、チャイルドウィンドウを生成することはできません".into(),
                    )));
                };
                let slot=NativeUserState::debug_slot(handle).unwrap();
                self.native_user.debug_windows[slot].as_mut().unwrap().scroll_flags=flags;
                self.store_runtime_bitmap(CHILD_BITMAP_BASE+slot as i32,DecodedImage {width:width as u32,height:height as u32,rgba:vec![0;(width*height*4) as usize]},1);
                let handle=if self.bp_profile==ethornell_script::native_abi::NativeAbiProfile::ModernFused {(0xf8000000|slot as u32) as i32} else {handle};
                ethornell_vm::Value::Int(handle)
            }
            0x11 => {
                let handle = pop_int_value(stack).unwrap_or_default();
                let Some(slot) = NativeUserState::debug_slot(handle) else {
                    // sub_478FA0's text repeats "チャイルド".
                    return Some(Err(ethornell_vm::VmError::Runtime(
                        "無効なチャイルドチャイルドウィンドウハンドルが指定されました".into(),
                    )));
                };
                self.native_user.debug_windows[slot] = None;
                let bitmap=CHILD_BITMAP_BASE+slot as i32;
                if let Some(resource)=self.graph_resources.remove(&bitmap) {self.graph_images.remove(&resource.key);self.graph_image_revisions.remove(&resource.key);}
                self.bitmap_dimensions.remove(&bitmap);self.bitmap_formats.remove(&bitmap);self.graph_surfaces.remove(&bitmap);
                ethornell_vm::Value::None
            }
            0x14 => {
                let visible = pop_int_value(stack).unwrap_or_default() != 0;
                let handle = pop_int_value(stack).unwrap_or_default();
                let Some(window) = self.native_user.debug_window_mut(handle) else {
                    return Some(Err(ethornell_vm::VmError::Runtime(CHILD_WINDOW_HANDLE.into())));
                };
                window.visible = visible;
                ethornell_vm::Value::None
            }
            0x15 => {
                let title = pop_string_value(stack).unwrap_or_default();
                let handle = pop_int_value(stack).unwrap_or_default();
                let Some(window) = self.native_user.debug_window_mut(handle) else {
                    return Some(Err(ethornell_vm::VmError::Runtime(CHILD_WINDOW_HANDLE.into())));
                };
                window.title = title;
                ethornell_vm::Value::None
            }
            0x16 => {
                let y = pop_int_value(stack).unwrap_or_default();
                let x = pop_int_value(stack).unwrap_or_default();
                let handle = pop_int_value(stack).unwrap_or_default();
                let Some(window) = self.native_user.debug_window_mut(handle) else {
                    return Some(Err(ethornell_vm::VmError::Runtime(CHILD_WINDOW_HANDLE.into())));
                };
                window.x = x;
                window.y = y;
                ethornell_vm::Value::None
            }
            0x17 => {
                let handle = pop_int_value(stack).unwrap_or_default();
                let Some(window) = self.native_user.debug_window(handle) else {
                    return Some(Err(ethornell_vm::VmError::Runtime(CHILD_WINDOW_HANDLE.into())));
                };
                stack.push(ethornell_vm::Value::Int(window.x));
                ethornell_vm::Value::Int(window.y)
            }
            0x18 => {
                let color = pop_int_value(stack).unwrap_or_default();
                let handle = pop_int_value(stack).unwrap_or_default();
                let Some(window) = self.native_user.debug_window_mut(handle) else {
                    return Some(Err(ethornell_vm::VmError::Runtime(CHILD_WINDOW_HANDLE.into())));
                };
                window.clear_color = color;
                window.bitmap_draws.clear();
                window.text_draws.clear();
                let (width,height)=(window.width as u32,window.height as u32);
                let pixel=native_bitmap_clear_color(color);
                self.store_runtime_bitmap(CHILD_BITMAP_BASE+NativeUserState::debug_slot(handle).unwrap() as i32,
                    DecodedImage {width,height,rgba:pixel.repeat((width*height) as usize)},1);
                ethornell_vm::Value::None
            }
            0x19 => {
                let alpha = pop_int_value(stack).unwrap_or_default();
                let blend_mode = pop_int_value(stack).unwrap_or_default();
                let bitmap = pop_int_value(stack).unwrap_or_default();
                let y = pop_int_value(stack).unwrap_or_default();
                let x = pop_int_value(stack).unwrap_or_default();
                let handle = pop_int_value(stack).unwrap_or_default();
                // sub_479130: validators, then sub_4301A0: window, bitmap,
                // sub_40A520's pixel mode (1) and empty clip (4).
                if let Err(error) = target_checks::bitmap(bitmap)
                    .and_then(|()| target_checks::effect_mode(blend_mode))
                    .and_then(|()| target_checks::effect_level(alpha))
                {
                    return Some(Err(error));
                }
                let Some((client_width, client_height)) = self
                    .native_user
                    .debug_window(handle)
                    .map(|window| (window.width, window.height))
                else {
                    return Some(Err(ethornell_vm::VmError::Runtime(CHILD_WINDOW_HANDLE.into())));
                };
                let Some(info) = ethornell_vm::GraphApi::query_bitmap_info(self, bitmap) else {
                    return Some(Err(ethornell_vm::VmError::Runtime(format!(
                        "指定されたビットマップ [ {bitmap} ] は存在しません"
                    ))));
                };
                if !matches!(info.format, 1 | 2) {
                    return Some(Err(ethornell_vm::VmError::Runtime(format!(
                        "画面モードとビットマップ [ {bitmap} ] のピクセルモードには互換性がありません"
                    ))));
                }
                if x >= client_width
                    || y >= client_height
                    || x.saturating_add(info.width as i32) <= 0
                    || y.saturating_add(info.height as i32) <= 0
                {
                    return Some(Err(ethornell_vm::VmError::Runtime(format!(
                        "指定された出力座標 [ {x} , {y} ] ではチャイルドウィンドウのクライアント外になります"
                    ))));
                }
                let window = self.native_user.debug_window_mut(handle).unwrap();
                window.bitmap_draws.push(NativeDebugBitmapDraw {
                    x,
                    y,
                    bitmap,
                    blend_mode,
                    alpha,
                });
                if let Err(e)=self.paint_native_child_bitmap(handle,bitmap,x,y,None,blend_mode,alpha) {return Some(Err(e));}
                ethornell_vm::Value::None
            }
            0x1A => {
                let alpha = pop_int_value(stack).unwrap_or_default();
                let blend_mode = pop_int_value(stack).unwrap_or_default();
                let color = pop_int_value(stack).unwrap_or_default();
                let style = pop_int_value(stack).unwrap_or_default();
                let font_size = pop_int_value(stack).unwrap_or_default();
                let font_id = pop_int_value(stack).unwrap_or_default();
                let text = pop_string_value(stack).unwrap_or_default();
                let y = pop_int_value(stack).unwrap_or_default();
                let x = pop_int_value(stack).unwrap_or_default();
                let handle = pop_int_value(stack).unwrap_or_default();
                // sub_479280: sub_498660 on the font, then sub_4302A0:
                // window, height (p5) and width percent (p6, "style" here)
                // through sub_409280. No blend validators.
                if self.native_user.font_face(font_id).is_none() {
                    return Some(Err(ethornell_vm::VmError::Runtime(format!(
                        "無効なフォント番号 [ {font_id} ] が指定されました"
                    ))));
                }
                if self.native_user.debug_window(handle).is_none() {
                    return Some(Err(ethornell_vm::VmError::Runtime(CHILD_WINDOW_HANDLE.into())));
                }
                if !(4..=200).contains(&font_size) {
                    return Some(Err(ethornell_vm::VmError::Runtime(format!(
                        "指定されたフォントサイズ [ {font_size} ] は無効です"
                    ))));
                }
                if !(25..=200).contains(&style) {
                    return Some(Err(ethornell_vm::VmError::Runtime(format!(
                        "指定されたフォント幅 [ {style} ] は無効です"
                    ))));
                }
                let extent=(snapshot::measure_text_advance(&text,font_size as f32,0.) as f32*style as f32/100.).round() as i32;
                let Some(window) = self.native_user.debug_window_mut(handle) else {
                    return Some(Err(ethornell_vm::VmError::Runtime(CHILD_WINDOW_HANDLE.into())));
                };
                window.text_draws.push(NativeDebugTextDraw {
                    x,
                    y,
                    text:text.clone(),
                    font_id,
                    font_size,
                    style,
                    color,
                    blend_mode,
                    alpha,
                    extent,
                });
                let color=[((color as u32>>16)&255) as f32/255.,((color as u32>>8)&255) as f32/255.,(color as u32&255) as f32/255.,1.];
                let width=(extent.max(1) as u32).min(8192);let height=font_size as u32*2;
                let mut glyphs=DecodedImage {width,height,rgba:vec![0;width as usize*height as usize*4]};
                snapshot::rasterize_bitmap_text(&mut glyphs,&text,0,0,font_size as f32,0.,style as f32,color);
                if let Err(error)=self.paint_native_child_image(handle,&glyphs,2,x,y,blend_mode,alpha) {return Some(Err(error));}
                ethornell_vm::Value::Int(extent)
            }
            0x1B => {
                let height=pop_int_value(stack).unwrap_or_default();let width=pop_int_value(stack).unwrap_or_default();
                let sy=pop_int_value(stack).unwrap_or_default();let sx=pop_int_value(stack).unwrap_or_default();
                let bitmap=pop_int_value(stack).unwrap_or_default();let y=pop_int_value(stack).unwrap_or_default();
                let x=pop_int_value(stack).unwrap_or_default();let handle=pop_int_value(stack).unwrap_or_default();
                if let Err(e)=target_checks::bitmap(bitmap) {return Some(Err(e));}
                if sx<0||sy<0||width<=0||height<=0 {return Some(Err(ethornell_vm::VmError::Runtime("invalid native child-window bitmap rectangle".into())))}
                if let Err(e)=self.paint_native_child_bitmap(handle,bitmap,x,y,Some([sx,sy,width,height]),0x80,0) {return Some(Err(e));}
                ethornell_vm::Value::None
            }
            0x1C => {
                let close_message = pop_string_value(stack).unwrap_or_default();
                let handle = pop_int_value(stack).unwrap_or_default();
                let Some(window) = self.native_user.debug_window_mut(handle) else {
                    return Some(Err(ethornell_vm::VmError::Runtime(CHILD_WINDOW_HANDLE.into())));
                };
                window.clipboard_text = close_message;
                ethornell_vm::Value::None
            }
            0x1E => {
                let value=pop_int_value(stack).unwrap_or_default();let property=pop_int_value(stack).unwrap_or_default();let handle=pop_int_value(stack).unwrap_or_default();
                let Some(window)=self.native_user.debug_window_mut(handle) else {return Some(Err(ethornell_vm::VmError::Runtime(CHILD_WINDOW_HANDLE.into())))};
                match property {
                    0|1=>{
                        let axis=property as usize;
                        if window.scroll_flags&(1<<axis)==0 {return Some(Err(ethornell_vm::VmError::Runtime("native child window has no requested scrollbar".into())))}
                        let range=value as u32&65535;let page=value as u32>>16;
                        if value!=0&&range<=1 {return Some(Err(ethornell_vm::VmError::Runtime("invalid native child-window scroll range".into())))}
                        if value==0 {
                            window.scroll_enabled[axis]=false;window.scroll_page[axis]=0;window.scroll_widget_position[axis]=0;
                        } else {
                            window.scroll_range[axis]=range as i32;window.scroll_page[axis]=page as i32;window.scroll_enabled[axis]=true;
                            window.scroll_widget_position[axis]=window.scroll_widget_position[axis].clamp(0,range as i32);
                        }
                    }
                    2|3=>{
                        let axis=(property-2) as usize;
                        if window.scroll_flags&(1<<axis)==0||value>window.scroll_range[1] {return Some(Err(ethornell_vm::VmError::Runtime("invalid native child-window scroll position".into())))}
                        // 06CE10 validates both axes against the cached Y
                        // range, then only changes the OS widget's position.
                        window.scroll_widget_position[axis]=value.clamp(0,window.scroll_range[axis]);
                    }
                    5=>window.redraw_policy=value,
                    _=>return Some(Err(ethornell_vm::VmError::Runtime("invalid native child-window property".into()))),
                }
                ethornell_vm::Value::Int(0)
            }
            0x2A => {
                let alignment=pop_int_value(stack).unwrap_or_default() as u32;
                if alignment<3 {self.native_user.edit_alignment=alignment;}
                ethornell_vm::Value::Int(i32::from(alignment<3))
            }
            // Portable edit control mirrors the target Win32 EDIT lifecycle.
            0x20 => {
                let focus = pop_int_value(stack).unwrap_or_default() != 0;
                let max_chars = pop_int_value(stack).unwrap_or_default();
                let font_size = pop_int_value(stack).unwrap_or_default();
                let font_id = pop_int_value(stack).unwrap_or_default();
                let height = pop_int_value(stack).unwrap_or_default();
                let width = pop_int_value(stack).unwrap_or_default();
                let y = pop_int_value(stack).unwrap_or_default();
                let x = pop_int_value(stack).unwrap_or_default();
                // sub_479460 -> sub_464480: size within the screen (1), font
                // (2), height 8..=64 (3), 1..=256 characters (4); the
                // position is not checked.
                let fail = |text: String| Some(Err(ethornell_vm::VmError::Runtime(text)));
                if (width as u32) < 8
                    || width > self.screen_width
                    || (height as u32) < 8
                    || height > self.screen_height
                {
                    return fail(format!("無効なウィンドウサイズ [ {width}, {height} ] が指定されました"));
                }
                if self.native_user.font_face(font_id).is_none() {
                    return fail(format!("無効なフォント番号 [ {font_id} ] が指定されました"));
                }
                if (font_size as u32).wrapping_sub(8) > 0x38 {
                    return fail(format!("無効なフォントサイズ [ {font_size} ] が指定されました"));
                }
                if (max_chars as u32).wrapping_sub(1) > 0xFF {
                    return fail(format!("無効な有効文字数 [ {max_chars} ] が指定されました"));
                }
                self.native_user.edit.active = true;
                self.native_user.edit.visible = true;
                self.native_user.edit.x = x;
                self.native_user.edit.y = y;
                self.native_user.edit.width = width;
                self.native_user.edit.height = height;
                self.native_user.edit.font_id = font_id;
                self.native_user.edit.font_size = font_size;
                self.native_user.edit.max_chars = max_chars as usize;
                self.native_user.edit.focused = focus;
                ethornell_vm::Value::None
            }
            // sub_4795D0 -> sub_4646F0 always pushes 0.
            0x21 => {
                self.native_user.edit.active = false;
                self.native_user.edit.visible = false;
                self.native_user.edit.focused = false;
                ethornell_vm::Value::Int(0)
            }
            0x22 => {
                let scale = pop_int_value(stack).unwrap_or_default();
                let valid = (25..=200).contains(&scale);
                if valid {
                    self.native_user.edit.font_scale = scale;
                }
                ethornell_vm::Value::Int(i32::from(valid))
            }
            // sub_479620: dword_565FF8, the shown flag of sub_464770.
            0x23 => ethornell_vm::Value::Int(i32::from(
                self.native_user.edit.active && self.native_user.edit.visible,
            )),
            // sub_464770 only acts on an existing control.
            0x24 => {
                let visible = pop_int_value(stack).unwrap_or_default() != 0;
                if self.native_user.edit.active {
                    self.native_user.edit.visible = visible;
                }
                ethornell_vm::Value::None
            }
            0x25 => {
                let color = pop_int_value(stack).unwrap_or_default() as u32;
                self.native_user.edit.color = (((color & 0x0000_FF) << 16)
                    | (color & 0x00FF_00)
                    | ((color & 0xFF_0000) >> 16))
                    as i32;
                ethornell_vm::Value::None
            }
            0x26 => {
                // sub_464860 copies into the initial-text buffer that the
                // next B0:20 shows; EM_LIMITTEXT does not cut it.
                let text = pop_string_value(stack).unwrap_or_default();
                self.native_user.text = truncate_shift_jis(&text, MAX_EDIT_TEXT_BYTES);
                ethornell_vm::Value::None
            }
            // VM owns the destination pointer write.
            0x27 => {
                let _destination = stack.pop();
                let bytes = encoding_rs::SHIFT_JIS
                    .encode(&self.native_user.text)
                    .0
                    .len();
                ethornell_vm::Value::Int(bytes.min(MAX_EDIT_TEXT_BYTES) as i32)
            }
            0x28 => {
                self.native_user.edit.hide_on_enter = pop_int_value(stack).unwrap_or_default() != 0;
                ethornell_vm::Value::None
            }
            0x29 => {
                self.native_user.edit.printable_input_enabled =
                    pop_int_value(stack).unwrap_or_default() != 0;
                ethornell_vm::Value::None
            }
            0x80 => {
                let message = pop_string_value(stack).unwrap_or_default();
                let title = self.native_user.message_box_caption();
                tracing::info!(title = %title, message = %message, "UserMessage");
                self.native_user
                    .record_dialog(0x80, &[ethornell_vm::Value::Str(message.clone())]);
                if self.native_dialog_presentation {
                    let _ = host_dialog::show_message(
                        host_dialog::MessageDialogKind::Information,
                        &title,
                        &message,
                        true,
                    );
                    // sub_46BC80 clears the input records after MessageBoxA.
                    self.clear_native_input_records();
                } else {
                    tracing::warn!(
                        title = %title,
                        message = %message,
                        "native message awaiting confirmation; send Enter or a mouse release to continue"
                    );
                    self.native_user.begin_blocking_message(title, message);
                    self.frame_yield_requested = true;
                }
                ethornell_vm::Value::None
            }
            0x81 => {
                // sub_478C40: a zero first pop selects MB_DEFBUTTON2 (No).
                let default_yes = pop_int_value(stack).unwrap_or_default();
                let message = pop_string_value(stack).unwrap_or_default();
                let args = [
                    ethornell_vm::Value::Str(message.clone()),
                    ethornell_vm::Value::Int(default_yes),
                ];
                let scripted = self.native_user.record_dialog(0x81, &args);
                let selected = if scripted != 0 {
                    scripted == 6
                } else {
                    host_dialog::show_message(
                        host_dialog::MessageDialogKind::YesNo,
                        &self.native_user.message_box_caption(),
                        &message,
                        default_yes != 0,
                    )
                    .unwrap_or(false)
                };
                self.clear_native_input_records();
                ethornell_vm::Value::Int(i32::from(selected))
            }
            0x82 => {
                // sub_478CA0: default-button flag, mode (1 = OK/Cancel), text.
                let default_first = pop_int_value(stack).unwrap_or_default();
                let kind = pop_int_value(stack).unwrap_or_default();
                let message = pop_string_value(stack).unwrap_or_default();
                let args = [
                    ethornell_vm::Value::Str(message.clone()),
                    ethornell_vm::Value::Int(kind),
                    ethornell_vm::Value::Int(default_first),
                ];
                let scripted = self.native_user.record_dialog(0x82, &args);
                let selected = if scripted != 0 {
                    if kind == 1 {
                        scripted == 1
                    } else {
                        scripted == 6
                    }
                } else {
                    let dialog_kind = if kind == 1 {
                        host_dialog::MessageDialogKind::OkCancel
                    } else {
                        host_dialog::MessageDialogKind::YesNo
                    };
                    host_dialog::show_message(
                        dialog_kind,
                        &self.native_user.message_box_caption(),
                        &message,
                        default_first != 0,
                    )
                    .unwrap_or(false)
                };
                self.clear_native_input_records();
                ethornell_vm::Value::Int(i32::from(selected))
            }
            0x83 => {
                // sub_46BC30 copies the string, or clears the caption for a
                // null pointer so the built-in one applies again.
                self.native_user.message_box_title = match stack.pop() {
                    Some(ethornell_vm::Value::Int(0) | ethornell_vm::Value::Ptr(0)) | None => None,
                    Some(value) => Some(value_to_string(&value).unwrap_or_default()),
                };
                ethornell_vm::Value::None
            }
            0x84 | 0x85 | 0x86 | 0x87 | 0x8C | 0x8F => {
                let count = match id {
                    0x84 | 0x8C => 4,
                    0x86 => 5,
                    0x8F => 6,
                    0x85 => 9,
                    0x87 => 12,
                    _ => unreachable!(),
                };
                let args = pop_args(stack, count);
                ethornell_vm::Value::Int(self.native_user.record_dialog(id, &args))
            }
            // A0 and A3 are VM-owned because they write caller memory.
            0xA0 => {
                let _args = pop_args(stack, 3);
                ethornell_vm::Value::Int(0)
            }
            0xA1 => {
                let handle = pop_int_value(stack).unwrap_or_default();
                ethornell_vm::Value::Int(i32::from(self.close_user_modeless_dialog(handle)))
            }
            0xA2 => {
                let visible = pop_int_value(stack).unwrap_or_default() != 0;
                let handle = pop_int_value(stack).unwrap_or_default();
                ethornell_vm::Value::Int(i32::from(
                    self.set_user_modeless_dialog_visible(handle, visible),
                ))
            }
            0xA3 => {
                let _args = pop_args(stack, 2);
                ethornell_vm::Value::Int(-2)
            }
            // Target font-name/resource registry.
            0xC0 => {
                let name = pop_string_value(stack).unwrap_or_default();
                ethornell_vm::Value::Int(self.native_user.intern_font(name, -1))
            }
            0xC1 => {
                let option = pop_int_value(stack).unwrap_or_default();
                let name = pop_string_value(stack).unwrap_or_default();
                ethornell_vm::Value::Int(self.native_user.intern_font(name, option))
            }
            0xC2 => {
                let name = pop_string_value(stack).unwrap_or_default();
                let available = !name.is_empty()
                    && (self.manager.find(&name).is_some()
                        || if let Some(policy) = file_policy::active() { policy.metadata(&name).is_some_and(|m| m.is_file()) } else { std::path::Path::new(&name).is_file() });
                if available {
                    self.native_user.font_resources.insert(name);
                }
                ethornell_vm::Value::Int(i32::from(available))
            }
            0xC3 => {
                // sub_479D20 -> sub_469130: script order (archive, font
                // file). A font already registered under that name returns
                // 1; with an archive the file is read from it
                // (AddFontMemResourceEx), without one from the primary root
                // (AddFontResourceA).
                let name = pop_string_value(stack).unwrap_or_default();
                let archive = pop_string_value(stack).unwrap_or_default();
                let key = if archive.is_empty() { name.clone() } else { format!("{archive}:{name}") };
                let registered = self
                    .native_user
                    .font_resources
                    .iter()
                    .any(|entry| entry.rsplit(':').next() == Some(name.as_str()));
                let available = registered
                    || (!name.is_empty()
                        && if archive.is_empty() {
                            self.manager.find(&name).is_some()
                        } else {
                            self.manager.find(&key).is_some() || self.manager.find(&name).is_some()
                        });
                if available && !registered {
                    self.native_user.font_resources.insert(key);
                }
                ethornell_vm::Value::Int(i32::from(available))
            }
            0xC6 => {
                let second = pop_string_value(stack).unwrap_or_default();
                let first = pop_string_value(stack).unwrap_or_default();
                let available = !first.is_empty()
                    && !second.is_empty()
                    && (first == second
                        || self.native_user.fonts.contains_key(&first)
                        || self.native_user.fonts.contains_key(&second)
                        || self.native_user.font_aliases.get(&first) == Some(&second));
                ethornell_vm::Value::Int(i32::from(available))
            }
            0xC7 => {
                let face = pop_string_value(stack).unwrap_or_default();
                let alias = pop_string_value(stack).unwrap_or_default();
                if !alias.is_empty() {
                    self.native_user.font_aliases.insert(alias, face);
                }
                ethornell_vm::Value::None
            }
            0xC8 => {
                // 14006B2A0 updates the first alias entry or creates one.
                let face=pop_string_value(stack).unwrap_or_default();
                let alias=pop_string_value(stack).unwrap_or_default();
                self.native_user.font_aliases.insert(alias,face);
                ethornell_vm::Value::None
            }
            0xCF => {
                // 1400BE0E0(1): discard native glyph/font-resource caches,
                // then republish the two stock face IDs.
                self.native_user.font_resources.clear();
                let stock=NativeUserState::default();
                self.native_user.fonts=stock.fonts;
                self.native_user.next_font_id=stock.next_font_id;
                ethornell_vm::Value::None
            }
            0xF0 => {
                if file_policy::restricted() { return Some(Err(ethornell_vm::VmError::Runtime("unsupported desktop wallpaper capability".into()))); }
                let tile = pop_int_value(stack).unwrap_or_default();
                let style = pop_int_value(stack).unwrap_or_default();
                let path = pop_string_value(stack).unwrap_or_default();
                let resolved = runtime_file_path_from_root(&self.manager, &self.native_root, &path)
                    .unwrap_or_else(|| std::path::PathBuf::from(&path));
                let applied = host_dialog::set_wallpaper(&resolved.to_string_lossy(), style, tile);
                tracing::info!(path, style, tile, applied, "SetDesktopWallpaper");
                self.native_user.wallpaper = Some(NativeWallpaperRequest { path, style, tile });
                ethornell_vm::Value::None
            }
            _ => return None,
        };
        Some(Ok(value))
    }
}

fn truncate_shift_jis(text: &str, max_bytes: usize) -> String {
    if encoding_rs::SHIFT_JIS.encode(text).0.len() <= max_bytes {
        return text.to_string();
    }
    let mut output = String::new();
    for ch in text.chars() {
        let candidate = format!("{output}{ch}");
        if encoding_rs::SHIFT_JIS.encode(&candidate).0.len() > max_bytes {
            break;
        }
        output.push(ch);
    }
    output
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn version_child_scroll_input_updates_queries_after_widget_configuration() {
        let manager=crate::ResourceManager::open_game(env!("CARGO_MANIFEST_DIR")).unwrap();
        let mut api=crate::RuntimeTraceApi::new(manager);
        let h=api.native_user.allocate_debug_window("Debug".into(),0,0,200,100).unwrap();
        let w=api.native_user.debug_window_mut(h).unwrap();
        w.scroll_flags=3;w.scroll_enabled=[true,true];w.scroll_range=[100,200];w.scroll_page=[20,40];
        w.scroll_widget_position=[0,150];w.redraw_policy=7;
        assert_eq!(api.native_child_property(h,3),Ok(0));
        api.mouse_pos=Some((50.,50.));
        assert!(api.handle_native_child_wheel(-1.));assert_eq!(api.native_child_property(h,3),Ok(7));
        assert!(api.handle_native_child_pointer(100.,95.,true));
        assert!(api.native_child_property(h,2).unwrap()>0);
        assert_eq!(api.native_child_property(h,3),Ok(7));
        api.native_user.debug_window_mut(h).unwrap().redraw_policy=0;
        assert!(api.handle_native_child_wheel(-1.));assert_eq!(api.native_child_property(h,3),Ok(7));
    }
    #[test]
    fn debug_window_handles_use_target_tag_and_eight_slots() {
        let mut state = NativeUserState::default();
        for slot in 0..MAX_DEBUG_WINDOWS {
            let handle = state
                .allocate_debug_window(String::new(), 0, 0, 32, 32)
                .expect("slot should exist");
            assert_eq!(handle as u32, DEBUG_WINDOW_HANDLE_PREFIX | slot as u32);
        }
        assert!(
            state
                .allocate_debug_window(String::new(), 0, 0, 32, 32)
                .is_none()
        );
    }

    #[test]
    fn font_interning_preserves_target_metadata_modes() {
        let mut state = NativeUserState::default();
        let plain = state.intern_font("plain".to_string(), -1);
        assert_eq!(state.fonts["plain"].metadata_charset, None);
        let japanese = state.intern_font("jp".to_string(), 0);
        assert_eq!(state.fonts["jp"].metadata_charset, Some(128));
        let default_charset = state.intern_font("default".to_string(), 1);
        assert_eq!(state.fonts["default"].metadata_charset, Some(0));
        assert_ne!(plain, japanese);
        assert_ne!(japanese, default_charset);
        assert_eq!(plain, 2);
        assert_eq!(state.font_face(0), Some("ＭＳ ゴシック"));
        assert_eq!(state.font_face(1), Some("ＭＳ 明朝"));
        assert_eq!(state.intern_font("ＭＳ 明朝".to_string(), -1), 1);
    }

    #[test]
    fn blocking_message_requires_explicit_acknowledgement() {
        let mut state = NativeUserState::default();
        state.begin_blocking_message("BGI".into(), "message".into());

        assert!(state.has_blocking_message());
        assert!(state.acknowledge_blocking_message());
        assert!(!state.has_blocking_message());
        assert!(!state.acknowledge_blocking_message());
    }

    #[test]
    fn shake_finishes_and_restores_zero_offset() {
        let mut shake = NativeScreenShakeState::default();
        let mut rng_seed = 1_u32;
        assert!(shake.configure(2, 64, 1, 2, 50, 60, false));
        for _ in 0..300 {
            shake.tick(16, &mut rng_seed);
        }
        assert!(!shake.active);
        assert_eq!(shake.offset, (0.0, 0.0));
    }

    #[test]
    fn shake_uses_target_cycle_step_and_reflection() {
        let mut shake = NativeScreenShakeState::default();
        let mut rng_seed = 1_u32;
        assert!(shake.configure(0, 32, 2, 1, 0, 8, false));
        assert_eq!(shake.updates_per_cycle, 4);
        shake.advance_one_update(&mut rng_seed);
        assert_eq!(shake.step_fixed, shake.current_amplitude_fixed);
        assert_eq!(shake.scalar_fixed, shake.current_amplitude_fixed);
        assert_eq!(shake.offset, (16.0, 0.0));
        shake.advance_one_update(&mut rng_seed);
        assert_eq!(shake.scalar_fixed, 0);
        assert!(shake.step_fixed < 0);
    }

    #[test]
    fn screen_shake_uses_target_msvc_crt_rand_sequence() {
        let mut seed = 1_u32;
        assert_eq!(msvc_rand(&mut seed), 41);
        assert_eq!(msvc_rand(&mut seed), 18_467);
        assert_eq!(msvc_rand(&mut seed), 6_334);
        assert_eq!(msvc_rand(&mut seed), 26_500);
    }

    #[test]
    fn modeless_dialog_queues_slider_toggle_and_close_events() {
        let mut state = NativeUserState::default();
        let handle = state
            .create_modeless_dialog([64, 0, 128, 32, 96, 0, 1, 1, 0])
            .expect("modeless slot");
        state.modeless_dialogs.get_mut(&handle).unwrap().visible = true;

        assert!(state.handle_modeless_pointer(
            MODELESS_SLIDER_X + MODELESS_SLIDER_WIDTH,
            MODELESS_SLIDER_Y,
            true,
        ));
        assert_eq!(
            state.modeless_dialogs[&handle].events.front(),
            Some(&[0, 128])
        );
        state
            .modeless_dialogs
            .get_mut(&handle)
            .unwrap()
            .events
            .clear();

        assert!(state.handle_modeless_pointer(MODELESS_DIALOG_X + 420.0, MODELESS_TOGGLE_Y, true,));
        assert_eq!(
            state.modeless_dialogs[&handle].events.front(),
            Some(&[5, 1])
        );
        state
            .modeless_dialogs
            .get_mut(&handle)
            .unwrap()
            .events
            .clear();

        assert!(state.handle_modeless_pointer(MODELESS_DIALOG_X + 20.0, MODELESS_CLOSE_Y, true,));
        assert_eq!(
            state.modeless_dialogs[&handle].events.front(),
            Some(&[-1, 0])
        );
        assert!(!state.modeless_dialogs[&handle].visible);
    }

    #[test]
    fn edit_text_truncation_preserves_character_boundaries() {
        let text = "あ".repeat(200);
        let truncated = truncate_shift_jis(&text, MAX_EDIT_TEXT_BYTES);
        assert!(encoding_rs::SHIFT_JIS.encode(&truncated).0.len() <= MAX_EDIT_TEXT_BYTES);
    }
}

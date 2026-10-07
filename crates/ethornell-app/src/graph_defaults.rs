use crate::ruby_registry::RubyRegistry;

#[derive(Debug, Clone, Copy, Default, PartialEq, Eq)]
pub(crate) struct SurfaceTextState {
    pub(crate) text_layout_mode: i32,
    pub(crate) cursor_x: i32,
    pub(crate) cursor_y: i32,
    pub(crate) font: i32,
    pub(crate) font_size: i32,
    pub(crate) scale_percent: i32,
    pub(crate) font_style: i32,
    pub(crate) layout_option: i32,
    pub(crate) render_option: i32,
}

#[derive(Debug, Clone, PartialEq, Eq)]
pub(crate) struct TextAnimationDefaults {
    pub(crate) glyph_delay_ms: i32,
    pub(crate) reveal_steps: i32,
    pub(crate) reveal_step_delay_ms: i32,
    pub(crate) settle_steps: i32,
    pub(crate) settle_step_delay_ms: i32,
    pub(crate) auto_advance_enabled: bool,
    pub(crate) auto_advance_delay_ms: i32,
}

impl Default for TextAnimationDefaults {
    fn default() -> Self {
        Self {
            glyph_delay_ms: 16,
            reveal_steps: 8,
            reveal_step_delay_ms: 50,
            settle_steps: 1,
            settle_step_delay_ms: 0,
            auto_advance_enabled: false,
            auto_advance_delay_ms: 0,
        }
    }
}

impl TextAnimationDefaults {
    pub(crate) fn set_glyph_delay(&mut self, delay_ms: i32) {
        self.glyph_delay_ms = delay_ms.max(0);
    }

    pub(crate) fn set_reveal(&mut self, steps: i32, step_delay_ms: i32) {
        if steps > 0 {
            self.reveal_steps = steps;
            self.reveal_step_delay_ms = step_delay_ms;
        }
    }

    pub(crate) fn set_settle(&mut self, steps: i32, step_delay_ms: i32) {
        if steps > 0 {
            self.settle_steps = steps;
            self.settle_step_delay_ms = step_delay_ms;
        }
    }

    pub(crate) fn set_auto_advance(&mut self, enabled: i32, delay_ms: i32) {
        self.auto_advance_enabled = enabled != 0;
        self.auto_advance_delay_ms = delay_ms;
    }
}

#[derive(Debug, Clone, PartialEq, Eq)]
pub(crate) struct TextLayoutDefaults {
    /// Target dword_507638. Used as the native horizontal/layout advance.
    pub(crate) layout_advance: i32,
    /// Target dword_50763C. A zero script value is normalized to one.
    pub(crate) scale_denominator: i32,
    /// Target dword_565BB0, added by sub_436FF0.
    pub(crate) character_spacing: i32,
    /// Target dword_507640, validated to 25..=100.
    pub(crate) font_percent: i32,
    /// Target dword_565BDC, used by 91:8E's exact cursor boundary test.
    pub(crate) boundary_offset: i32,
    /// Target dword_565CF0.
    pub(crate) mode_flag: i32,
    /// Target dword_5076B0 configured by 91:99.
    pub(crate) scale_reciprocal_16_16: i32,
}

impl Default for TextLayoutDefaults {
    fn default() -> Self {
        Self {
            layout_advance: 0,
            scale_denominator: 1,
            character_spacing: 0,
            font_percent: 100,
            boundary_offset: 0,
            mode_flag: 0,
            scale_reciprocal_16_16: 0,
        }
    }
}

impl TextLayoutDefaults {
    pub(crate) fn configure(&mut self, args: [i32; 6]) -> Result<(), TextLayoutError> {
        let [
            layout_advance,
            scale_denominator,
            character_spacing,
            font_percent,
            boundary_offset,
            mode_flag,
        ] = args;
        if !(25..=100).contains(&font_percent) {
            return Err(TextLayoutError::FontPercent(font_percent));
        }
        if boundary_offset < 0 {
            return Err(TextLayoutError::BoundaryOffset(boundary_offset));
        }
        self.layout_advance = layout_advance;
        self.scale_denominator = scale_denominator.max(1);
        self.character_spacing = character_spacing;
        self.font_percent = font_percent;
        self.boundary_offset = boundary_offset;
        self.mode_flag = mode_flag;
        Ok(())
    }

    pub(crate) fn set_scale_divisor(&mut self, divisor: i32) -> bool {
        if divisor == 0 {
            return false;
        }
        self.scale_reciprocal_16_16 = divisor.wrapping_add(0xffff) / divisor;
        true
    }
}

#[derive(Debug, Clone, Copy, PartialEq, Eq)]
pub(crate) enum TextLayoutError {
    FontPercent(i32),
    BoundaryOffset(i32),
}

#[derive(Debug, Clone, Default, PartialEq, Eq)]
pub(crate) struct TextStyleDefaults {
    pub(crate) font_name: Option<String>,
    /// Target dword_565CE0. Nonpositive values select the 91:98 percentage.
    pub(crate) ruby_height: i32,
    /// Target dword_565CE4. Its remaining consumers are not yet named.
    pub(crate) ruby_font_field: i32,
    /// Target dword_565CE8.
    pub(crate) ruby_x_offset: i32,
    /// Target dword_565CEC.
    pub(crate) ruby_y_offset: i32,
    /// Target dword_507644.
    pub(crate) override_0: i32,
    /// Target dword_507648.
    pub(crate) override_1: i32,
}

impl TextStyleDefaults {
    pub(crate) fn set_fields(&mut self, values: [i32; 6]) {
        self.ruby_height = values[0];
        self.ruby_font_field = values[1];
        self.ruby_x_offset = values[2];
        self.ruby_y_offset = values[3];
        self.override_0 = values[4];
        self.override_1 = values[5];
    }
}

#[derive(Debug, Clone, Default, PartialEq, Eq)]
pub(crate) struct TextFontOverrideDefaults {
    /// Target pszFaceName configured by 92:9D.
    pub(crate) face_name: Option<String>,
    /// Target dword_565D1C.
    pub(crate) height: i32,
    /// Target dword_565D20.
    pub(crate) creation_field_0: i32,
    /// Target dword_565D24.
    pub(crate) creation_field_1: i32,
    /// Target bItalic.
    pub(crate) italic: bool,
}

#[derive(Debug, Clone, Default)]
pub(crate) struct GraphRuntimeDefaults {
    pub(crate) text_animation: TextAnimationDefaults,
    pub(crate) text_layout: TextLayoutDefaults,
    pub(crate) text_style: TextStyleDefaults,
    pub(crate) text_font_override: TextFontOverrideDefaults,
    /// Graph90:98 global caret frame table. The VM converts the BP pointer
    /// before passing the copied bitmap handles to the runtime; `None` is the
    /// target's -1 empty-frame record.
    pub(crate) caret_frame_count: i32,
    pub(crate) caret_frame_table_pointer: i32,
    pub(crate) caret_frames: Vec<Option<i32>>,
    pub(crate) caret_frame_delay_ms: i32,
    pub(crate) caret_mode: i32,
    pub(crate) caret_x: i32,
    pub(crate) caret_y: i32,
    pub(crate) message_delay_enabled: bool,
    pub(crate) message_delay_ms: i32,
    pub(crate) shadow_enabled: bool,
    pub(crate) shadow_x: i32,
    pub(crate) shadow_y: i32,
    pub(crate) shadow_alpha: i32,
    pub(crate) message_input_forces_completion: bool,
    /// Target globals `dword_565BA4`/`dword_565BA8` configured by 0x90:0x91.
    pub(crate) message_input_scope_mode: i32,
    pub(crate) message_input_scope_value: i32,
    /// Target global `dword_565BAC` configured by 0x90:0x92.
    pub(crate) message_input_filter_enabled: bool,
    /// Target ruby registry `unk_565BB4`, shared by `91:94/95/96`, the
    /// `<r>`/`<ruby>` message tags and the message layout.
    pub(crate) ruby_registry: RubyRegistry,
}

impl GraphRuntimeDefaults {
    pub(crate) fn configure_caret_position(&mut self, mode: i32, x: i32, y: i32) {
        self.caret_mode = mode;
        self.caret_x = x;
        self.caret_y = y;
    }

    pub(crate) fn configure_message_delay(&mut self, enabled: i32, delay_ms: i32) {
        self.message_delay_enabled = enabled != 0;
        self.message_delay_ms = delay_ms;
    }

    pub(crate) fn configure_message_input_scope(&mut self, mode: i32, value: i32) -> bool {
        match mode {
            0 | 2 => {
                self.message_input_scope_mode = mode;
                true
            }
            1 if (0..0x1_0000).contains(&value) => {
                self.message_input_scope_mode = 1;
                self.message_input_scope_value = value;
                true
            }
            _ => false,
        }
    }

    pub(crate) fn resolved_message_input_scope(&self, display_object: i32) -> i32 {
        match self.message_input_scope_mode {
            1 => self.message_input_scope_value.wrapping_shl(16) | 0xffff,
            // Target mode 2 packs CDspObj's virtual +88 value. Portable graph
            // handles are the stable equivalent used by the input registry.
            2 => display_object.wrapping_shl(16) | 0xffff,
            _ => 2,
        }
    }

    pub(crate) fn configure_shadow(&mut self, x: i32, y: i32, alpha: i32) {
        if (0..=100).contains(&x) && (0..=100).contains(&y) && (0..=256).contains(&alpha) {
            self.shadow_x = x;
            self.shadow_y = y;
            self.shadow_alpha = alpha;
        }
    }

    pub(crate) fn shadow_parameters(&self) -> Option<(i32, i32, i32)> {
        self.shadow_enabled.then_some((
            self.shadow_x,
            self.shadow_y,
            self.shadow_alpha.clamp(0, 256),
        ))
    }

    pub(crate) fn shadow_for_height(&self, font_height: f32) -> Option<(i32, i32, f32)> {
        let (x_percent, y_percent, concentration) = self.shadow_parameters()?;
        let height = font_height.max(0.0).round() as i32;
        Some((
            height.saturating_mul(x_percent) / 100,
            height.saturating_mul(y_percent) / 100,
            (256 - concentration) as f32 / 256.0,
        ))
    }

    pub(crate) fn configure_text_style(&mut self, font_name: Option<String>, values: [i32; 6]) {
        self.text_style.font_name = font_name.filter(|name| !name.is_empty());
        self.text_style.set_fields(values);
    }

    pub(crate) fn update_text_substitution(
        &mut self,
        source: Option<String>,
        replacement: Option<String>,
    ) {
        match (source.filter(|value| !value.is_empty()), replacement) {
            (None, _) => self.ruby_registry.clear(),
            (Some(source), Some(replacement)) if !replacement.is_empty() => {
                self.ruby_registry.register(&source, &replacement, false);
            }
            (Some(source), _) => {
                self.ruby_registry.remove(&source);
            }
        }
    }

    pub(crate) fn collect_ruby_records(&mut self, text: &str) -> (String, i32) {
        self.ruby_registry.collect_records(text)
    }

    pub(crate) fn register_ruby_records(&mut self, records: &str) -> bool {
        self.ruby_registry.register_records(records)
    }
}

#[cfg(test)]
mod tests {
    use super::{GraphRuntimeDefaults, TextAnimationDefaults, TextLayoutDefaults};

    #[test]
    fn invalid_layout_does_not_replace_native_defaults() {
        let mut defaults = TextLayoutDefaults::default();
        defaults.configure([200, 100, 0, 40, 16, 1]).unwrap();
        assert!(defaults.configure([1, 2, 3, 24, 4, 5]).is_err());
        assert_eq!(defaults.font_percent, 40);
        assert_eq!(defaults.boundary_offset, 16);
    }

    #[test]
    fn non_positive_animation_steps_leave_previous_values() {
        let mut defaults = TextAnimationDefaults::default();
        defaults.set_reveal(6, 25);
        defaults.set_reveal(0, 99);
        assert_eq!(
            (defaults.reveal_steps, defaults.reveal_step_delay_ms),
            (6, 25)
        );
    }

    #[test]
    fn shadow_uses_font_height_percentages_and_inverse_concentration() {
        let mut defaults = GraphRuntimeDefaults::default();
        defaults.shadow_enabled = true;
        defaults.configure_shadow(10, 20, 64);
        assert_eq!(defaults.shadow_for_height(40.0), Some((4, 8, 0.75)));
    }

    #[test]
    fn substitutions_support_add_remove_and_clear() {
        let mut defaults = GraphRuntimeDefaults::default();
        defaults.update_text_substitution(Some("A".into()), Some("B".into()));
        assert_eq!(
            defaults.collect_ruby_records("A+A"),
            ("A\\B\nA\\B\n".into(), 2)
        );
        defaults.update_text_substitution(Some("A".into()), None);
        assert_eq!(defaults.collect_ruby_records("A"), (String::new(), 0));
        defaults.update_text_substitution(Some("A".into()), Some("B".into()));
        defaults.update_text_substitution(None, None);
        assert_eq!(defaults.collect_ruby_records("A"), (String::new(), 0));
    }

    #[test]
    fn ruby_record_parser_matches_native_line_format() {
        let mut defaults = GraphRuntimeDefaults::default();
        assert!(defaults.register_ruby_records("矢古民\\やこたみ\n町\\ちょう\n"));
        assert_eq!(
            defaults.collect_ruby_records("矢古民町"),
            ("矢古民\\やこたみ\n町\\ちょう\n".into(), 2)
        );
        assert!(!defaults.register_ruby_records("missing separator"));
    }
}

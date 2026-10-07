use std::collections::BTreeMap;

#[derive(Clone, Debug, PartialEq, Eq)]
pub(crate) struct RuntimeFontRegistration {
    pub(crate) font_name_id: i32,
    pub(crate) height: i32,
    pub(crate) weight_percent: i32,
    pub(crate) italic: bool,
    pub(crate) font_id: i32,
}

#[derive(Debug)]
pub(crate) struct RuntimeGraphConfig {
    pub(crate) center: (i32, i32),
    /// Graph90:07 dword_565AE0: how long (ms) sub_402080 keeps loads
    /// synchronous after the first one.
    pub(crate) sync_load_hold_ms: i32,
    pub(crate) enabled: i32,
    pub(crate) display_mode: (i32, i32),
    pub(crate) renderer_options: (i32, i32),
    pub(crate) bitmap_priorities: BTreeMap<i32, i32>,
    pub(crate) fonts: Vec<RuntimeFontRegistration>,
    pub(crate) text_global_properties: BTreeMap<u32, i32>,
}

impl Default for RuntimeGraphConfig {
    fn default() -> Self {
        Self {
            // CObjectManager::CObjectManager initializes the optional graph
            // centre through sub_442E70(-1, -1). sub_442E90 only accepts an
            // override that lies inside the current display dimensions.
            center: (-1, -1),
            sync_load_hold_ms: 0,
            enabled: 0,
            display_mode: (0, 0),
            renderer_options: (0, 0),
            bitmap_priorities: BTreeMap::new(),
            fonts: Vec::new(),
            text_global_properties: BTreeMap::new(),
        }
    }
}

impl RuntimeGraphConfig {
    pub(crate) fn register_font(&mut self, registration: RuntimeFontRegistration) -> bool {
        if registration.font_name_id != 0
            && (!(4..=200).contains(&registration.height)
                || !(25..=200).contains(&registration.weight_percent)
                || registration.font_id < 2)
        {
            return false;
        }
        if let Some(existing) = self.fonts.iter_mut().find(|existing| {
            existing.font_name_id == registration.font_name_id
                && existing.height == registration.height
                && existing.weight_percent == registration.weight_percent
                && existing.italic == registration.italic
        }) {
            existing.font_id = registration.font_id;
        } else {
            self.fonts.push(registration);
        }
        true
    }
}

#[cfg(test)]
mod tests {
    use super::{RuntimeFontRegistration, RuntimeGraphConfig};

    #[test]
    fn native_font_registration_updates_matching_records() {
        let mut config = RuntimeGraphConfig::default();
        let mut registration = RuntimeFontRegistration {
            font_name_id: 1,
            height: 28,
            weight_percent: 100,
            italic: false,
            font_id: 1024,
        };
        assert!(config.register_font(registration.clone()));
        registration.font_id = 2048;
        assert!(config.register_font(registration));
        assert_eq!(config.fonts.len(), 1);
        assert_eq!(config.fonts[0].font_id, 2048);
    }

    #[test]
    fn native_named_font_validation_matches_recovered_ranges() {
        let mut config = RuntimeGraphConfig::default();
        let invalid = RuntimeFontRegistration {
            font_name_id: 1,
            height: 3,
            weight_percent: 100,
            italic: false,
            font_id: 1024,
        };
        assert!(!config.register_font(invalid));
    }
}

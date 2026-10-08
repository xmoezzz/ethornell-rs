use crate::display_tree::NativeDisplayKind;

pub(crate) const BACK_B_SECONDARY_LAYER_ID: i32 = -0x2200_0000;
pub(crate) const BACK_F_SECONDARY_LAYER_ID: i32 = -0x2400_0000;
/// 1x1 opaque black image drawn full-screen for an inactive CDspObjBack.
pub(crate) const BACK_INACTIVE_FILL_KEY: &str = "runtime:back-inactive-fill";
pub(crate) const BACK_S_ADDITIONAL_LAYER_IDS: [i32; 3] = [-0x2300_0001, -0x2300_0002, -0x2300_0003];

#[derive(Debug, Clone, Copy, PartialEq, Eq)]
pub(crate) enum NativeBackgroundSecondaryResource {
    Bitmap { handle: i32, generation: u64 },
    Sentinel(i32),
}

#[derive(Debug, Clone, Copy, PartialEq, Eq, Default)]
pub(crate) struct NativeBackFState {
    pub(crate) secondary_x: i32,
    pub(crate) secondary_y: i32,
    pub(crate) mask_resource_binding: Option<(i32, u64)>,
    pub(crate) mask_parameter: i32,
    // sub_41D210 initializes +0x168/+0x16c to zero. Their setter is
    // recovered separately from the 90:43 constructor ABI, so keep the
    // exact constructor state here rather than inventing mask semantics.
    pub(crate) mask_control_enabled: i32,
    pub(crate) mask_control_mode: i32,
}

/// `CDspObjBack` class selector returned by target `sub_4207A0` and consumed
/// by `sub_43E190`. Graph90:40-4A select classes 1-11 in this exact order;
/// System91:40 additionally selects class 12 (`CDspObjBackML`).
#[derive(Debug, Clone, Copy, PartialEq, Eq)]
pub(crate) enum NativeBackgroundClass {
    BackN = 1,
    BackB = 2,
    BackS = 3,
    BackF = 4,
    BackD = 5,
    BackDst = 6,
    BackGrd = 7,
    BackRpl = 8,
    BackStr = 9,
    BackRtt = 10,
    BackMsc = 11,
    BackMl = 12,
}

impl NativeBackgroundClass {
    pub(crate) fn for_graph90_selector(selector: u16) -> Option<Self> {
        Some(match selector {
            0x40 => Self::BackN,
            0x41 => Self::BackB,
            0x42 => Self::BackS,
            0x43 => Self::BackF,
            0x44 => Self::BackD,
            0x45 => Self::BackDst,
            0x46 => Self::BackGrd,
            0x47 => Self::BackRpl,
            0x48 => Self::BackStr,
            0x49 => Self::BackRtt,
            0x4a => Self::BackMsc,
            _ => return None,
        })
    }

    pub(crate) fn display_kind(self) -> NativeDisplayKind {
        match self {
            Self::BackN => NativeDisplayKind::BackN,
            Self::BackB => NativeDisplayKind::BackB,
            Self::BackS => NativeDisplayKind::BackS,
            Self::BackF => NativeDisplayKind::BackF,
            Self::BackD => NativeDisplayKind::BackD,
            Self::BackDst => NativeDisplayKind::BackDst,
            Self::BackGrd => NativeDisplayKind::BackGrd,
            Self::BackRpl => NativeDisplayKind::BackRpl,
            Self::BackStr => NativeDisplayKind::BackStr,
            Self::BackRtt => NativeDisplayKind::BackRtt,
            Self::BackMsc => NativeDisplayKind::BackMsc,
            Self::BackMl => NativeDisplayKind::BackMl,
        }
    }

    pub(crate) fn graph90_selector(self) -> Option<u16> {
        let selector = 0x3f_u16 + self as u16;
        (selector <= 0x4a).then_some(selector)
    }
}

/// Target-confirmed subclass fields reached by each class' vtable +44 setter.
/// Classes whose override is `nullsub_3` deliberately have no position state.
#[derive(Debug, Clone, Copy, PartialEq, Eq)]
pub(crate) enum NativeBackgroundPosition {
    None,
    BackSCell { x: i32, y: i32 },
    BackFSource { x: i32, y: i32 },
    BackRplSource { x: i32, y: i32 },
    BackStrSource { x: i32, y: i32 },
    BackMlFixed { x_16_16: i32, y_16_16: i32 },
}

#[derive(Debug, Clone, Copy, PartialEq, Eq)]
pub(crate) struct NativeBackgroundState {
    pub(crate) class: NativeBackgroundClass,
    pub(crate) position: NativeBackgroundPosition,
    pub(crate) resource_binding: Option<(i32, u64)>,
    pub(crate) secondary_resource_binding: Option<NativeBackgroundSecondaryResource>,
    pub(crate) quad_resource_bindings: Option<[(i32, u64); 4]>,
    pub(crate) backf: Option<NativeBackFState>,
    /// CDspObjBack+0x138 (vtable+120, Graph90:4C's second value). While 0
    /// the draw method (sub_41C340) clears the target to black instead of
    /// drawing the content.
    pub(crate) active: i32,
}

impl NativeBackgroundState {
    pub(crate) fn new(class: NativeBackgroundClass) -> Self {
        let position = match class {
            NativeBackgroundClass::BackS => NativeBackgroundPosition::BackSCell { x: 0, y: 0 },
            NativeBackgroundClass::BackF => NativeBackgroundPosition::BackFSource { x: 0, y: 0 },
            NativeBackgroundClass::BackRpl => {
                NativeBackgroundPosition::BackRplSource { x: 0, y: 0 }
            }
            NativeBackgroundClass::BackStr => {
                NativeBackgroundPosition::BackStrSource { x: 0, y: 0 }
            }
            NativeBackgroundClass::BackMl => NativeBackgroundPosition::BackMlFixed {
                x_16_16: 0,
                y_16_16: 0,
            },
            _ => NativeBackgroundPosition::None,
        };
        Self {
            class,
            position,
            resource_binding: None,
            secondary_resource_binding: None,
            quad_resource_bindings: None,
            backf: (class == NativeBackgroundClass::BackF).then(NativeBackFState::default),
            active: 0,
        }
    }

    pub(crate) fn resources_are_current(
        self,
        mut current_generation: impl FnMut(i32) -> Option<u64>,
    ) -> bool {
        let Some((handle, generation)) = self.resource_binding else {
            return false;
        };
        if current_generation(handle) != Some(generation) {
            return false;
        }
        match self.class {
            NativeBackgroundClass::BackN => true,
            NativeBackgroundClass::BackB => match self.secondary_resource_binding {
                Some(NativeBackgroundSecondaryResource::Bitmap { handle, generation }) => {
                    current_generation(handle) == Some(generation)
                }
                Some(NativeBackgroundSecondaryResource::Sentinel(0x7000 | 0x7001)) => true,
                _ => false,
            },
            NativeBackgroundClass::BackS => self.quad_resource_bindings.is_some_and(|bindings| {
                bindings
                    .into_iter()
                    .all(|(handle, generation)| current_generation(handle) == Some(generation))
            }),
            NativeBackgroundClass::BackF => {
                let secondary_current = match self.secondary_resource_binding {
                    Some(NativeBackgroundSecondaryResource::Bitmap { handle, generation }) => {
                        current_generation(handle) == Some(generation)
                    }
                    Some(NativeBackgroundSecondaryResource::Sentinel(
                        0x7000 | 0x7001 | 0x7fff | -1,
                    )) => true,
                    _ => false,
                };
                let mask_current =
                    self.backf
                        .is_some_and(|state| match state.mask_resource_binding {
                            Some((handle, generation)) => {
                                current_generation(handle) == Some(generation)
                            }
                            None => true,
                        });
                secondary_current && mask_current
            }
            _ => true,
        }
    }

    /// Applies the target vtable +44 behavior. A false result means the
    /// subclass override is a no-op or BackS rejected an out-of-range cell.
    pub(crate) fn set_position(
        &mut self,
        x: i32,
        y: i32,
        screen_width: i32,
        screen_height: i32,
    ) -> bool {
        self.position = match self.class {
            NativeBackgroundClass::BackS => {
                if x < 0 || y < 0 || x > screen_width || y > screen_height {
                    return false;
                }
                NativeBackgroundPosition::BackSCell { x, y }
            }
            NativeBackgroundClass::BackF => NativeBackgroundPosition::BackFSource { x, y },
            NativeBackgroundClass::BackRpl => NativeBackgroundPosition::BackRplSource { x, y },
            NativeBackgroundClass::BackStr => NativeBackgroundPosition::BackStrSource { x, y },
            NativeBackgroundClass::BackMl => NativeBackgroundPosition::BackMlFixed {
                x_16_16: x,
                y_16_16: y,
            },
            _ => return false,
        };
        true
    }

    pub(crate) fn integer_position(self) -> Option<(i32, i32)> {
        match self.position {
            NativeBackgroundPosition::BackSCell { x, y }
            | NativeBackgroundPosition::BackFSource { x, y }
            | NativeBackgroundPosition::BackRplSource { x, y }
            | NativeBackgroundPosition::BackStrSource { x, y } => Some((x, y)),
            NativeBackgroundPosition::BackMlFixed { x_16_16, y_16_16 } => {
                Some((x_16_16 >> 16, y_16_16 >> 16))
            }
            NativeBackgroundPosition::None => None,
        }
    }
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn graph90_background_selectors_match_target_class_ids() {
        for selector in 0x40..=0x4a {
            let class = NativeBackgroundClass::for_graph90_selector(selector).unwrap();
            assert_eq!(class as u16, selector - 0x3f);
            assert_eq!(class.graph90_selector(), Some(selector));
        }
        assert_eq!(NativeBackgroundClass::BackMl.graph90_selector(), None);
    }

    #[test]
    fn target_noop_and_stateful_position_overrides_stay_distinct() {
        let mut noop = NativeBackgroundState::new(NativeBackgroundClass::BackN);
        assert!(!noop.set_position(12, 34, 1280, 720));
        assert_eq!(noop.position, NativeBackgroundPosition::None);

        let mut backf = NativeBackgroundState::new(NativeBackgroundClass::BackF);
        assert!(backf.set_position(-12, 34, 1280, 720));
        assert_eq!(backf.integer_position(), Some((-12, 34)));

        let mut backs = NativeBackgroundState::new(NativeBackgroundClass::BackS);
        assert!(!backs.set_position(-1, 0, 1280, 720));
        assert_eq!(backs.integer_position(), Some((0, 0)));
        assert!(backs.set_position(1280, 720, 1280, 720));
        assert_eq!(backs.integer_position(), Some((1280, 720)));
    }
}

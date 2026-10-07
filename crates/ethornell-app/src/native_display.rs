/// Exact 32-bit target field image for the recovered `CDspObj` base class.
///
/// The target allocates `0x134` bytes for the base class. Only field offsets
/// directly touched by the target constructor or recovered helpers are split
/// out. A split slot is not assigned a semantic name until its reads and
/// writes have been closed in the target executable.
#[repr(C)]
#[derive(Debug, Clone, Copy, PartialEq, Eq)]
pub(crate) struct CDspObjLayout32 {
    pub(crate) vftable: u32, // +0x00
    /// Base object enable gate written by CDspObj::SetEnabled.
    pub(crate) enabled: u32, // +0x04
    /// Written by Graph90:38 property 0xC0 (sub_493130); no reader recovered.
    pub(crate) property_c0_value: u32, // +0x08
    /// Suppression gate tested inversely by CDspObj::IsDrawable.
    pub(crate) suppress_draw: u32, // +0x0c
    /// Written by Graph90:38 property 0xC1 (sub_41ADE0).
    pub(crate) property_c1_value: u32, // +0x10
    /// Independent draw gate written by the vtable+4 setter.
    pub(crate) draw_enabled: u32, // +0x14
    /// Object sort-class band returned by sub_41B0B0 and packed by
    /// CDspObj::vtable+0x1C (sub_41B0C0). Values >= 8 are clamped to 7.
    pub(crate) sort_class: u32, // +0x18
    /// 16-bit render priority written by the vtable+84 setter.
    pub(crate) priority: u32, // +0x1c
    /// Constructor-supplied sort index. When +0x7C is nonzero,
    /// sub_41B0C0 uses this value (masked to 13 bits) instead of fixed Z.
    pub(crate) sort_index: u32, // +0x20
    /// Signed additive bias used by sub_41B0C0; Graph property 0x8100
    /// writes this field through sub_41BF10.
    pub(crate) sort_bias: i32, // +0x24
    pub(crate) unknown_28: u32, // +0x28
    pub(crate) unknown_2c: u32, // +0x2c
    pub(crate) position_x: i32, // +0x30
    pub(crate) position_y: i32, // +0x34
    pub(crate) primary_offset_x: i32, // +0x38
    pub(crate) primary_offset_y: i32, // +0x3c
    pub(crate) secondary_offset_x: i32, // +0x40
    pub(crate) secondary_offset_y: i32, // +0x44
    /// Per-object global-display-offset gate. sub_41ADF0 writes this DWORD;
    /// sub_41C0E0 adds dword_565B34/565B38 only when it is nonzero.
    pub(crate) global_display_offset_enabled: u32,
    /// Three signed 16.16 coordinates written by `sub_41B370` (vtable +60).
    /// `sub_41B4C0` resolves this object by adding its three local 16.16
    /// vector banks (+0x4C, +0x5C and +0x6C families). Parent/member motion
    /// is propagated eagerly by the setters; `sub_41B4C0` does not walk parents.
    pub(crate) fixed_position_x_16_16: i32, // +0x4c
    pub(crate) fixed_position_y_16_16: i32, // +0x50
    pub(crate) fixed_position_z_16_16: i32, // +0x54
    pub(crate) unknown_58_to_77: [u8; 0x20],
    /// Mask-related target slot; exact member role is not yet closed.
    pub(crate) mask_slot_78: u32, // +0x78
    /// Independent CDspObj gate consulted by sub_41B370. When nonzero, a
    /// fixed-position update also mirrors rounded X/Y into the ordinary
    /// +0x30/+0x34 position through vtable+0x28. Other reads exist, so the
    /// broader meaning of this slot remains intentionally unnamed.
    pub(crate) fixed_position_updates_integer_position: u32, // +0x7c
    /// sub_41BEB0/sub_41BED0 fixed-position rounding enable. When nonzero,
    /// sub_41B370 rounds X/Y to whole 16.16 pixels when Z is zero or when
    /// `fixed_position_rounding_mode == 1`.
    pub(crate) fixed_position_rounding_enabled: u32, // +0x80
    /// Secondary rounding mode written by Graph property 0x8000 extra arg.
    pub(crate) fixed_position_rounding_mode: u32, // +0x84
    /// Written by Graph90:38 property 0x8001 (sub_41BEF0), read back by the
    /// unused getter sub_41BF00.
    pub(crate) property_8001_value: u32, // +0x88
    pub(crate) unknown_8c: u32, // +0x8c
    pub(crate) unknown_90: u32, // +0x90
    pub(crate) unknown_94: u32, // +0x94
    pub(crate) unknown_98: u32, // +0x98
    pub(crate) unknown_9c: u32, // +0x9c
    pub(crate) unknown_a0: u32, // +0xa0
    pub(crate) unknown_a4: u32, // +0xa4
    /// Blit mode used when the object draws (sub_41B600 / sub_41B610);
    /// Graph90:38 property 1 and Graph91:38 parameter 1.
    pub(crate) blend_mode: i32, // +0xa8
    /// Target `Graph90:0x32` reaches the `CDspObj` virtual setter at
    /// `sub_41B620`, which writes the native transparency parameter here.
    /// `0` is opaque and `256` is fully transparent.
    pub(crate) alpha_parameter: i32, // +0xac
    /// Additional mask transparency propagated through the child chain.
    pub(crate) mask_alpha: i32, // +0xb0
    /// 0..=256 alpha multiplier propagated through the child chain.
    pub(crate) alpha_multiplier: i32, // +0xb4
    /// Mode-zero object parameter stored as a signed 16.16 value.
    pub(crate) fixed_parameter_16_16: i32, // +0xb8
    /// Property 0x7FFF0000 (sub_41C1C0): when set, the object manager
    /// refreshes the object every frame through sub_41AEE0.
    pub(crate) auto_update: u32, // +0xbc
    /// Sixteen script-owned DWORDs: property / parameter 0x7FFFFFFF
    /// (sub_41C1E0 / sub_41C200).
    pub(crate) user_slots: [i32; 16], // +0xc0
    /// Gate read by CDspObjSprite::sub_429AF0 before consulting the optional
    /// CObjectManager graph centre. Normal script-visible display objects are
    /// constructed with this set; a few internal helper sprites explicitly
    /// clear it in the target.
    pub(crate) use_graph_center: u32, // +0x100
    pub(crate) unknown_104_to_107: [u8; 0x04],
    pub(crate) unknown_108: u32, // +0x108
    pub(crate) unknown_10c: u32, // +0x10c
    pub(crate) unknown_110: u32, // +0x110
    pub(crate) unknown_114: u32, // +0x114
    pub(crate) unknown_118: u32, // +0x118
    pub(crate) unknown_11c_to_11f: [u8; 0x04],
    pub(crate) unknown_120: u32, // +0x120
    pub(crate) unknown_124_to_12b: [u8; 0x08],
    pub(crate) unknown_12c: u32, // +0x12c
    pub(crate) unknown_130_to_133: [u8; 0x04],
}

impl Default for CDspObjLayout32 {
    fn default() -> Self {
        Self {
            vftable: 0,
            enabled: 1,
            property_c0_value: 0,
            suppress_draw: 0,
            property_c1_value: 0,
            draw_enabled: 0,
            sort_class: 0,
            priority: 0,
            sort_index: 0,
            sort_bias: 0,
            unknown_28: 0,
            unknown_2c: 0,
            position_x: 0,
            position_y: 0,
            primary_offset_x: 0,
            primary_offset_y: 0,
            secondary_offset_x: 0,
            secondary_offset_y: 0,
            // CDspObj::CDspObj (sub_41A400) calls sub_41ADF0(1).
            // Individual objects may later change this through property 196.
            global_display_offset_enabled: 1,
            fixed_position_x_16_16: 0,
            fixed_position_y_16_16: 0,
            fixed_position_z_16_16: 0,
            unknown_58_to_77: [0; 0x20],
            mask_slot_78: 0,
            // CDspObj::CDspObj calls sub_41BEA0(this, 1). Specific
            // transformed subclasses (notably Sprite mode 5/6 and BackML)
            // explicitly clear this later.
            fixed_position_updates_integer_position: 1,
            fixed_position_rounding_enabled: 0,
            fixed_position_rounding_mode: 0,
            property_8001_value: 0,
            unknown_8c: 0,
            unknown_90: 0,
            unknown_94: 0,
            unknown_98: 0,
            unknown_9c: 0,
            unknown_a0: 0,
            unknown_a4: 0,
            // CDspObj::CDspObj (sub_41A400) starts with blit mode 0x80.
            blend_mode: 128,
            alpha_parameter: 0,
            mask_alpha: 0,
            alpha_multiplier: 256,
            fixed_parameter_16_16: 0,
            auto_update: 0,
            user_slots: [0; 16],
            // Normal graph constructors pass 1 to sub_41C0B0. Special
            // internal Sprite constructors that pass 0 are not allocated by
            // Graph90:50.
            use_graph_center: 1,
            unknown_104_to_107: [0; 0x04],
            unknown_108: 0,
            unknown_10c: 0,
            unknown_110: 0,
            unknown_114: 0,
            unknown_118: 0,
            unknown_11c_to_11f: [0; 0x04],
            unknown_120: 0,
            unknown_124_to_12b: [0; 0x08],
            unknown_12c: 0,
            unknown_130_to_133: [0; 0x04],
        }
    }
}

/// Exact target class sizes recovered from RTTI, constructors and allocations.
/// Subclass bodies remain opaque until their member semantics are recovered.
pub(crate) const CDSP_OBJ_SIZE: usize = 0x134;
pub(crate) const CDSP_OBJ_BACK_SIZE: usize = 0x13c;
pub(crate) const CDSP_OBJ_BACK_N_SIZE: usize = 0x144;
pub(crate) const CDSP_OBJ_BACK_B_SIZE: usize = 0x14c;
pub(crate) const CDSP_OBJ_BACK_S_SIZE: usize = 0x1a4;
pub(crate) const CDSP_OBJ_BACK_F_SIZE: usize = 0x170;
pub(crate) const CDSP_OBJ_BACK_D_SIZE: usize = 0x2244;
pub(crate) const CDSP_OBJ_BACK_DST_SIZE: usize = 0x158;
pub(crate) const CDSP_OBJ_BACK_GRD_SIZE: usize = 0x148;
pub(crate) const CDSP_OBJ_BACK_RPL_SIZE: usize = 0x168;
pub(crate) const CDSP_OBJ_BACK_STR_SIZE: usize = 0x170;
pub(crate) const CDSP_OBJ_BACK_RTT_SIZE: usize = 0x154;
pub(crate) const CDSP_OBJ_BACK_MSC_SIZE: usize = 0x154;
pub(crate) const CDSP_OBJ_BACK_ML_SIZE: usize = 0x3e0;
pub(crate) const CDSP_OBJ_SPRITE_SIZE: usize = 0x418;
pub(crate) const CDSP_OBJ_WINDOW_SIZE: usize = 0x3cc;
pub(crate) const CDSP_OBJ_EFFECTOR_SIZE: usize = 0x1c8;
pub(crate) const CDSP_OBJ_FILTER_SIZE: usize = 0x14c;
pub(crate) const CDSP_OBJ_GROUP_SIZE: usize = 0x134;
pub(crate) const CDSP_OBJ_KNOB_SIZE: usize = 0x174;
pub(crate) const CDSP_OBJ_LANDSCAPE_SIZE: usize = 0x17c;
pub(crate) const CDSP_OBJ_MAP_SIZE: usize = 0x1bc;

/// Target bitmap metadata record copied by `BitmapRegistry_CopyInfoRecord`.
/// Width and height are target-confirmed at offsets `+0x08` and `+0x0c`;
/// the other four DWORD meanings remain unrecovered.
#[repr(C)]
#[derive(Debug, Clone, Copy, Default, PartialEq, Eq)]
pub(crate) struct BitmapInfoRecordLayout32 {
    pub(crate) unknown_00: u32, // +0x00
    pub(crate) unknown_04: u32, // +0x04
    pub(crate) width: u32,      // +0x08
    pub(crate) height: u32,     // +0x0c
    /// Written by Graph90:38 property 0xC1 (sub_41ADE0).
    pub(crate) property_c1_value: u32, // +0x10
    pub(crate) unknown_14: u32, // +0x14
}

/// Exact 0x48-byte target bitmap-registry entry. Only fields read directly by
/// target helpers are named. The trailing bytes stay opaque.
#[repr(C)]
#[derive(Debug, Clone, Copy, PartialEq, Eq)]
pub(crate) struct BitmapRegistryEntryLayout32 {
    pub(crate) bitmap_object: u32,            // +0x00
    pub(crate) info_00: u32,                  // +0x04
    pub(crate) info_04: u32,                  // +0x08
    pub(crate) width: u32,                    // +0x0c
    pub(crate) height: u32,                   // +0x10
    pub(crate) info_10: u32,                  // +0x14
    pub(crate) info_14: u32,                  // +0x18
    pub(crate) unknown_1c: u32,               // +0x1c
    pub(crate) transform_or_orientation: i32, // +0x20
    pub(crate) unknown_24: u32,               // +0x24
    /// CBG/script auxiliary reference point X; sub_401EF0/sub_402440.
    pub(crate) auxiliary_x: i32, // +0x28
    /// CBG/script auxiliary reference point Y; sub_401EF0/sub_402440.
    pub(crate) auxiliary_y: i32, // +0x2c
    pub(crate) unknown_30_to_47: [u8; 0x18],  // +0x30
}

impl Default for BitmapRegistryEntryLayout32 {
    fn default() -> Self {
        Self {
            bitmap_object: 0,
            info_00: 0,
            info_04: 0,
            width: 0,
            height: 0,
            info_10: 0,
            info_14: 0,
            unknown_1c: 0,
            transform_or_orientation: 0,
            unknown_24: 0,
            auxiliary_x: -1,
            auxiliary_y: -1,
            unknown_30_to_47: [0; 0x18],
        }
    }
}

/// Exact-size target display subclasses. Their opaque tails deliberately do
/// not acquire semantic fields until target member accesses close them.
#[repr(C)]
#[derive(Debug, Clone, Copy, PartialEq, Eq)]
pub(crate) struct CDspObjBackLayout32 {
    pub(crate) base: CDspObjLayout32,
    pub(crate) unknown_134_to_13b: [u8; 0x08],
}

macro_rules! opaque_back_layout {
    ($name:ident, $tail:expr) => {
        #[repr(C)]
        #[derive(Debug, Clone, Copy, PartialEq, Eq)]
        pub(crate) struct $name {
            pub(crate) base: CDspObjBackLayout32,
            pub(crate) opaque_tail: [u8; $tail],
        }
    };
}

opaque_back_layout!(CDspObjBackDLayout32, 0x2108);
opaque_back_layout!(CDspObjBackDstLayout32, 0x1c);
opaque_back_layout!(CDspObjBackGrdLayout32, 0x0c);
opaque_back_layout!(CDspObjBackRttLayout32, 0x18);
opaque_back_layout!(CDspObjBackMscLayout32, 0x18);
opaque_back_layout!(CDspObjBackMlLayout32, 0x2a4);

#[repr(C)]
#[derive(Debug, Clone, Copy, PartialEq, Eq)]
pub(crate) struct CDspObjBackNLayout32 {
    pub(crate) base: CDspObjBackLayout32,
    pub(crate) bitmap: i32,            // +0x13c
    pub(crate) bitmap_generation: u32, // +0x140
}

#[repr(C)]
#[derive(Debug, Clone, Copy, PartialEq, Eq)]
pub(crate) struct CDspObjBackBLayout32 {
    pub(crate) base: CDspObjBackLayout32,
    pub(crate) primary_bitmap: i32,              // +0x13c
    pub(crate) secondary_bitmap_or_mode: i32,    // +0x140
    pub(crate) primary_bitmap_generation: u32,   // +0x144
    pub(crate) secondary_bitmap_generation: u32, // +0x148
}

#[repr(C)]
#[derive(Debug, Clone, Copy, PartialEq, Eq)]
pub(crate) struct CDspObjBackSLayout32 {
    pub(crate) base: CDspObjBackLayout32,
    pub(crate) bitmaps: [i32; 4],            // +0x13c
    pub(crate) bitmap_generations: [u32; 4], // +0x14c
    pub(crate) cell_x: i32,                  // +0x15c
    pub(crate) cell_y: i32,                  // +0x160
    pub(crate) tile_rects: [i32; 16],        // +0x164
}

#[repr(C)]
#[derive(Debug, Clone, Copy, PartialEq, Eq)]
pub(crate) struct CDspObjBackFLayout32 {
    pub(crate) base: CDspObjBackLayout32,
    pub(crate) primary_x: i32,                    // +0x13c
    pub(crate) primary_y: i32,                    // +0x140
    pub(crate) primary_bitmap: i32,               // +0x144
    pub(crate) primary_bitmap_generation: u32,    // +0x148
    pub(crate) secondary_x: i32,                  // +0x14c
    pub(crate) secondary_y: i32,                  // +0x150
    pub(crate) secondary_bitmap_or_sentinel: i32, // +0x154
    pub(crate) secondary_bitmap_generation: u32,  // +0x158
    pub(crate) mask_bitmap: i32,                  // +0x15c
    pub(crate) mask_parameter: i32,               // +0x160
    pub(crate) mask_bitmap_generation: u32,       // +0x164
    pub(crate) mask_control_enabled: i32,         // +0x168
    pub(crate) mask_control_mode: i32,            // +0x16c
}

#[repr(C)]
#[derive(Debug, Clone, Copy, PartialEq, Eq)]
pub(crate) struct CDspObjBackRplLayout32 {
    pub(crate) base: CDspObjBackLayout32,
    pub(crate) unknown_13c_to_14b: [u8; 0x10],
    pub(crate) source_x: i32, // +0x14c
    pub(crate) source_y: i32, // +0x150
    pub(crate) unknown_154_to_167: [u8; 0x14],
}

#[repr(C)]
#[derive(Debug, Clone, Copy, PartialEq, Eq)]
pub(crate) struct CDspObjBackStrLayout32 {
    pub(crate) base: CDspObjBackLayout32,
    pub(crate) unknown_13c_to_147: [u8; 0x0c],
    pub(crate) source_x: i32, // +0x148
    pub(crate) source_y: i32, // +0x14c
    pub(crate) unknown_150_to_16f: [u8; 0x20],
}

#[repr(C)]
#[derive(Debug, Clone, Copy, PartialEq, Eq)]
pub(crate) struct CDspObjSpriteLayout32 {
    pub(crate) base: CDspObjLayout32,
    pub(crate) unknown_134_to_417: [u8; 0x2e4],
}

#[repr(C)]
#[derive(Debug, Clone, Copy, PartialEq, Eq)]
pub(crate) struct CDspObjWindowLayout32 {
    pub(crate) base: CDspObjLayout32,
    pub(crate) unknown_134_to_3cb: [u8; 0x298],
}

#[repr(C)]
#[derive(Debug, Clone, Copy, PartialEq, Eq)]
pub(crate) struct CDspObjEffectorLayout32 {
    pub(crate) base: CDspObjLayout32,
    pub(crate) unknown_134_to_1c7: [u8; 0x94],
}

#[repr(C)]
#[derive(Debug, Clone, Copy, PartialEq, Eq)]
pub(crate) struct CDspObjFilterLayout32 {
    pub(crate) base: CDspObjLayout32,
    pub(crate) unknown_134_to_14b: [u8; 0x18],
}

#[repr(C)]
#[derive(Debug, Clone, Copy, PartialEq, Eq)]
pub(crate) struct CDspObjKnobLayout32 {
    pub(crate) base: CDspObjLayout32,
    pub(crate) unknown_134_to_173: [u8; 0x40],
}

#[repr(C)]
#[derive(Debug, Clone, Copy, PartialEq, Eq)]
pub(crate) struct CDspObjLandscapeLayout32 {
    pub(crate) base: CDspObjLayout32,
    pub(crate) unknown_134_to_17b: [u8; 0x48],
}

#[repr(C)]
#[derive(Debug, Clone, Copy, PartialEq, Eq)]
pub(crate) struct CDspObjMapLayout32 {
    pub(crate) base: CDspObjLayout32,
    pub(crate) unknown_134_to_1bb: [u8; 0x88],
}

#[cfg(test)]
mod tests {
    use super::*;
    use std::mem::{offset_of, size_of};

    #[test]
    fn cdspobj_base_layout_matches_target() {
        assert_eq!(size_of::<CDspObjLayout32>(), CDSP_OBJ_SIZE);
        assert_eq!(offset_of!(CDspObjLayout32, enabled), 0x04);
        assert_eq!(offset_of!(CDspObjLayout32, draw_enabled), 0x14);
        assert_eq!(offset_of!(CDspObjLayout32, sort_class), 0x18);
        assert_eq!(offset_of!(CDspObjLayout32, priority), 0x1c);
        assert_eq!(offset_of!(CDspObjLayout32, sort_index), 0x20);
        assert_eq!(offset_of!(CDspObjLayout32, sort_bias), 0x24);
        assert_eq!(offset_of!(CDspObjLayout32, unknown_28), 0x28);
        assert_eq!(offset_of!(CDspObjLayout32, position_x), 0x30);
        assert_eq!(offset_of!(CDspObjLayout32, fixed_position_x_16_16), 0x4c);
        assert_eq!(offset_of!(CDspObjLayout32, fixed_position_y_16_16), 0x50);
        assert_eq!(offset_of!(CDspObjLayout32, fixed_position_z_16_16), 0x54);
        assert_eq!(offset_of!(CDspObjLayout32, primary_offset_x), 0x38);
        assert_eq!(offset_of!(CDspObjLayout32, secondary_offset_x), 0x40);
        assert_eq!(offset_of!(CDspObjLayout32, mask_slot_78), 0x78);
        assert_eq!(
            offset_of!(CDspObjLayout32, fixed_position_updates_integer_position),
            0x7c
        );
        assert_eq!(offset_of!(CDspObjLayout32, use_graph_center), 0x100);
        assert_eq!(
            offset_of!(CDspObjLayout32, fixed_position_rounding_enabled),
            0x80
        );
        assert_eq!(
            offset_of!(CDspObjLayout32, fixed_position_rounding_mode),
            0x84
        );
        assert_eq!(offset_of!(CDspObjLayout32, unknown_8c), 0x8c);
        assert_eq!(offset_of!(CDspObjLayout32, alpha_parameter), 0xac);
        assert_eq!(offset_of!(CDspObjLayout32, mask_alpha), 0xb0);
        assert_eq!(offset_of!(CDspObjLayout32, alpha_multiplier), 0xb4);
        assert_eq!(offset_of!(CDspObjLayout32, fixed_parameter_16_16), 0xb8);
        assert_eq!(offset_of!(CDspObjLayout32, unknown_108), 0x108);
        assert_eq!(offset_of!(CDspObjLayout32, unknown_120), 0x120);
        assert_eq!(offset_of!(CDspObjLayout32, unknown_12c), 0x12c);
    }

    #[test]
    fn cdspobj_constructor_fixed_position_gates_match_target() {
        let native = CDspObjLayout32::default();
        assert_eq!(native.enabled, 1);
        assert_eq!(native.draw_enabled, 0);
        // CDspObj::CDspObj -> sub_41BEA0(1), sub_41BEB0(0, 0).
        assert_eq!(native.fixed_position_updates_integer_position, 1);
        // CDspObj::CDspObj -> sub_41ADF0(1).
        assert_eq!(native.global_display_offset_enabled, 1);
        assert_eq!(native.use_graph_center, 1);
        assert_eq!(native.fixed_position_rounding_enabled, 0);
        assert_eq!(native.fixed_position_rounding_mode, 0);
    }

    #[test]
    fn bitmap_layouts_match_target_records() {
        assert_eq!(size_of::<BitmapInfoRecordLayout32>(), 0x18);
        assert_eq!(offset_of!(BitmapInfoRecordLayout32, width), 0x08);
        assert_eq!(offset_of!(BitmapInfoRecordLayout32, height), 0x0c);
        assert_eq!(size_of::<BitmapRegistryEntryLayout32>(), 0x48);
        assert_eq!(offset_of!(BitmapRegistryEntryLayout32, bitmap_object), 0x00);
        assert_eq!(offset_of!(BitmapRegistryEntryLayout32, width), 0x0c);
        assert_eq!(offset_of!(BitmapRegistryEntryLayout32, height), 0x10);
        assert_eq!(
            offset_of!(BitmapRegistryEntryLayout32, transform_or_orientation),
            0x20
        );
        assert_eq!(offset_of!(BitmapRegistryEntryLayout32, auxiliary_x), 0x28);
        assert_eq!(offset_of!(BitmapRegistryEntryLayout32, auxiliary_y), 0x2c);
        let bitmap = BitmapRegistryEntryLayout32::default();
        assert_eq!((bitmap.auxiliary_x, bitmap.auxiliary_y), (-1, -1));
    }

    #[test]
    fn display_subclass_layouts_match_target_allocations() {
        assert_eq!(size_of::<CDspObjBackLayout32>(), CDSP_OBJ_BACK_SIZE);
        assert_eq!(size_of::<CDspObjBackNLayout32>(), CDSP_OBJ_BACK_N_SIZE);
        assert_eq!(size_of::<CDspObjBackBLayout32>(), CDSP_OBJ_BACK_B_SIZE);
        assert_eq!(offset_of!(CDspObjBackNLayout32, bitmap), 0x13c);
        assert_eq!(offset_of!(CDspObjBackNLayout32, bitmap_generation), 0x140);
        assert_eq!(offset_of!(CDspObjBackBLayout32, primary_bitmap), 0x13c);
        assert_eq!(
            offset_of!(CDspObjBackBLayout32, secondary_bitmap_or_mode),
            0x140
        );
        assert_eq!(
            offset_of!(CDspObjBackBLayout32, primary_bitmap_generation),
            0x144
        );
        assert_eq!(
            offset_of!(CDspObjBackBLayout32, secondary_bitmap_generation),
            0x148
        );
        assert_eq!(size_of::<CDspObjBackSLayout32>(), CDSP_OBJ_BACK_S_SIZE);
        assert_eq!(size_of::<CDspObjBackFLayout32>(), CDSP_OBJ_BACK_F_SIZE);
        assert_eq!(size_of::<CDspObjBackDLayout32>(), CDSP_OBJ_BACK_D_SIZE);
        assert_eq!(size_of::<CDspObjBackDstLayout32>(), CDSP_OBJ_BACK_DST_SIZE);
        assert_eq!(size_of::<CDspObjBackGrdLayout32>(), CDSP_OBJ_BACK_GRD_SIZE);
        assert_eq!(size_of::<CDspObjBackRplLayout32>(), CDSP_OBJ_BACK_RPL_SIZE);
        assert_eq!(size_of::<CDspObjBackStrLayout32>(), CDSP_OBJ_BACK_STR_SIZE);
        assert_eq!(size_of::<CDspObjBackRttLayout32>(), CDSP_OBJ_BACK_RTT_SIZE);
        assert_eq!(size_of::<CDspObjBackMscLayout32>(), CDSP_OBJ_BACK_MSC_SIZE);
        assert_eq!(size_of::<CDspObjBackMlLayout32>(), CDSP_OBJ_BACK_ML_SIZE);
        assert_eq!(offset_of!(CDspObjBackSLayout32, cell_x), 0x15c);
        assert_eq!(offset_of!(CDspObjBackSLayout32, cell_y), 0x160);
        assert_eq!(offset_of!(CDspObjBackSLayout32, bitmaps), 0x13c);
        assert_eq!(offset_of!(CDspObjBackSLayout32, bitmap_generations), 0x14c);
        assert_eq!(offset_of!(CDspObjBackSLayout32, tile_rects), 0x164);
        assert_eq!(offset_of!(CDspObjBackFLayout32, primary_x), 0x13c);
        assert_eq!(offset_of!(CDspObjBackFLayout32, primary_y), 0x140);
        assert_eq!(offset_of!(CDspObjBackFLayout32, primary_bitmap), 0x144);
        assert_eq!(
            offset_of!(CDspObjBackFLayout32, primary_bitmap_generation),
            0x148
        );
        assert_eq!(offset_of!(CDspObjBackFLayout32, secondary_x), 0x14c);
        assert_eq!(offset_of!(CDspObjBackFLayout32, secondary_y), 0x150);
        assert_eq!(
            offset_of!(CDspObjBackFLayout32, secondary_bitmap_or_sentinel),
            0x154
        );
        assert_eq!(
            offset_of!(CDspObjBackFLayout32, secondary_bitmap_generation),
            0x158
        );
        assert_eq!(offset_of!(CDspObjBackFLayout32, mask_bitmap), 0x15c);
        assert_eq!(offset_of!(CDspObjBackFLayout32, mask_parameter), 0x160);
        assert_eq!(
            offset_of!(CDspObjBackFLayout32, mask_bitmap_generation),
            0x164
        );
        assert_eq!(
            offset_of!(CDspObjBackFLayout32, mask_control_enabled),
            0x168
        );
        assert_eq!(offset_of!(CDspObjBackFLayout32, mask_control_mode), 0x16c);
        assert_eq!(offset_of!(CDspObjBackRplLayout32, source_x), 0x14c);
        assert_eq!(offset_of!(CDspObjBackRplLayout32, source_y), 0x150);
        assert_eq!(offset_of!(CDspObjBackStrLayout32, source_x), 0x148);
        assert_eq!(offset_of!(CDspObjBackStrLayout32, source_y), 0x14c);
        assert_eq!(size_of::<CDspObjSpriteLayout32>(), CDSP_OBJ_SPRITE_SIZE);
        assert_eq!(size_of::<CDspObjWindowLayout32>(), CDSP_OBJ_WINDOW_SIZE);
        assert_eq!(size_of::<CDspObjEffectorLayout32>(), CDSP_OBJ_EFFECTOR_SIZE);
        assert_eq!(size_of::<CDspObjFilterLayout32>(), CDSP_OBJ_FILTER_SIZE);
        assert_eq!(size_of::<CDspObjKnobLayout32>(), CDSP_OBJ_KNOB_SIZE);
        assert_eq!(
            size_of::<CDspObjLandscapeLayout32>(),
            CDSP_OBJ_LANDSCAPE_SIZE
        );
        assert_eq!(size_of::<CDspObjMapLayout32>(), CDSP_OBJ_MAP_SIZE);
        assert_eq!(CDSP_OBJ_GROUP_SIZE, size_of::<CDspObjLayout32>());
    }
}

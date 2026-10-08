use super::*;
use ethornell_vm::GraphApi;

const GRAPH90_SPRITE_TAG: u32 = 0x8000_0000;
const GRAPH90_FILTER_TAG: u32 = 0x9000_0000;
const GRAPH90_MAP_TAG: u32 = 0xA000_0000;
const GRAPH90_CLASS_SPRITE: i32 = 1;
const GRAPH90_CLASS_FILTER: i32 = 2;
const GRAPH90_CLASS_MAP: i32 = 3;

impl RuntimeTraceApi {
    fn graph90_source_args(stack: &mut Vec<ethornell_vm::Value>, count: usize) -> Vec<i32> {
        let mut values = pop_args(stack, count)
            .iter()
            .map(value_to_i32)
            .collect::<Vec<_>>();
        values.reverse();
        values
    }

    fn graph90_tag_matches(handle: i32, tag: u32, slots: u32) -> bool {
        (handle as u32).wrapping_sub(tag) < slots
    }

    fn graph90_allocate_object(
        &mut self,
        tag: u32,
        slots: u32,
        class: i32,
        kind: NativeDisplayKind,
        initial_mode: i32,
    ) -> i32 {
        for slot in 0..slots {
            let handle = (tag | slot) as i32;
            if self.graph_handle_exists(handle) {
                continue;
            }
            self.display_tree.register(handle, kind);
            self.graph_object_enabled.insert(handle, true);
            self.graph_object_draw_enabled.insert(handle, false);
            self.graph_object_layers.entry(handle).or_default();
            {
                let properties = self.graph_object_properties.entry(handle).or_default();
                properties
                    .named_properties
                    .insert("target-class".to_string(), class);
                properties
                    .named_properties
                    .insert("target-object-mode".to_string(), initial_mode);
                properties.native.enabled = 1;
                properties.native.draw_enabled = 0;
            }
            // The concrete constructors pass a per-registry monotonically
            // increasing ECX value to CDspObj::CDspObj; it is not the reusable
            // public slot encoded in the handle.
            self.graph90_initialize_native_constructor_sort(handle, kind);
            return handle;
        }
        0
    }

    pub(super) fn graph90_is_sprite_handle(&self, handle: i32) -> bool {
        self.graph90_object_matches(handle, GRAPH90_SPRITE_TAG, 512, GRAPH90_CLASS_SPRITE)
    }

    fn graph90_object_matches(&self, handle: i32, tag: u32, slots: u32, class: i32) -> bool {
        if !Self::graph90_tag_matches(handle, tag, slots) || !self.graph_handle_exists(handle) {
            return false;
        }
        self.graph_object_properties
            .get(&handle)
            .and_then(|properties| properties.named_properties.get("target-class"))
            .is_none_or(|actual| *actual == class)
    }

    fn graph90_release_object(&mut self, handle: i32, tag: u32, slots: u32, class: i32) -> bool {
        if !self.graph90_object_matches(handle, tag, slots, class) {
            return false;
        }
        self.graph_transition_nodes.remove(&handle);
        self.remove_graph_object(handle)
    }

    fn graph90_current_object(&mut self) -> i32 {
        let object = self.current_graph_object.unwrap_or(0);
        self.display_tree.register_inferred(object);
        self.graph_object_enabled.entry(object).or_insert(true);
        self.graph_object_draw_enabled.entry(object).or_insert(true);
        self.graph_object_properties.entry(object).or_default();
        object
    }

    fn graph90_record_call(&mut self, object: i32, selector: u16, args: &[i32]) {
        let properties = self.graph_object_properties.entry(object).or_default();
        properties
            .named_properties
            .insert("target-last-selector".to_string(), selector as i32);
        for (index, value) in args.iter().copied().enumerate() {
            properties
                .named_properties
                .insert(format!("target-90-{selector:02X}-arg-{index}"), value);
        }
        self.trace_graph(format!(
            "target graph 90:{selector:02X} object=#{object} args={args:?}"
        ));
    }

    fn graph90_set_enabled(&mut self, object: i32, enabled: bool) {
        self.graph_object_enabled.insert(object, enabled);
        self.display_tree.set_enabled(object, enabled);
        let properties = self.graph_object_properties.entry(object).or_default();
        properties.native.enabled = u32::from(enabled);
    }

    fn graph90_set_mode(&mut self, object: i32, mode: i32) {
        if mode != 5 {
            self.graph_mode5_render_states.remove(&object);
        }
        let properties = self.graph_object_properties.entry(object).or_default();
        properties
            .named_properties
            .insert("target-object-mode".to_string(), mode);
        if matches!(mode, 5 | 6) {
            // sub_427170/sub_427220 explicitly call sub_41BEA0(this, 0)
            // before their vtable+60 fixed-position write. No recovered path
            // restores the base-constructor value when the sprite later
            // changes mode, so this is intentionally one-way.
            properties.native.fixed_position_updates_integer_position = 0;
        }
    }

    /// Begins one of the target CDspObjSprite mode configuration paths.
    ///
    /// The native mode constructors rebuild the Sprite's mode-specific state
    /// before applying the base CDspObj alpha.  In particular, mode 0
    /// `sub_427410`, mode 2 `sub_427790`, and mode 5 `sub_427170` write
    /// `Sprite+0x244 = -1` before the subsequent SetAlpha call.  Mode 1
    /// `sub_4274A0` installs its new selector before `sub_426FA0` applies the
    /// base alpha directly through `sub_41B620`.
    ///
    /// A stale portable `graph_transition_nodes` entry therefore must never
    /// participate in the alpha setter for a new configuration.  Leaving it
    /// alive makes an old selector-1 Sprite route `SetAlpha(256)` into the
    /// transition value instead of the base transparency, producing the
    /// visible one-frame `opaque -> transparent -> fade in` flash seen on
    /// title images.
    fn graph90_begin_sprite_configuration(&mut self, object: i32, mode: i32) {
        self.graph_transition_nodes.remove(&object);
        self.graph90_set_mode(object, mode);
    }

    fn graph90_require_transparency_parameter(
        selector: u16,
        parameter: i32,
    ) -> ethornell_vm::VmResult<()> {
        if (0..=256).contains(&parameter) {
            Ok(())
        } else {
            Err(ethornell_vm::VmError::Runtime(format!(
                "Graph90:{selector:02X} parameter {parameter} is outside 0..=256"
            )))
        }
    }

    pub(super) fn graph90_require_registered_object(
        &mut self,
        selector: u16,
        object: i32,
    ) -> ethornell_vm::VmResult<()> {
        if self.display_tree.contains(object) {
            return Ok(());
        }

        // The target resolves CDspObj handles through its class-specific
        // managers (sub_443270), not through our portable display-tree
        // bookkeeping. Tagged handles use the full unsigned bit pattern: the
        // first Sprite allocated by Graph90:50 is exactly 0x80000000, which is
        // i32::MIN in the BP VM. If portable render bookkeeping is temporarily
        // missing while native object state still exists, rebuild that mirror
        // instead of rejecting a live target object.
        let native_state_exists = self.graph_object_properties.contains_key(&object)
            || self.graph_object_enabled.contains_key(&object)
            || self.graph_object_draw_enabled.contains_key(&object)
            || self.graph_object_layers.contains_key(&object)
            || self.graph_layers.contains_key(&object)
            || self.graph_surfaces.contains_key(&object)
            || self.graph_knob_states.contains_key(&object)
            || self.graph_groups.contains_key(&object)
            || self.graph91_effectors.contains_key(&object)
            || self.graph91_landscapes.contains_key(&object);
        if native_state_exists {
            self.display_tree.register_inferred(object);
            return Ok(());
        }

        Err(ethornell_vm::VmError::Runtime(format!(
            "Graph90:{selector:02X} object #{object} is not registered"
        )))
    }

    fn graph90_require_priority(selector: u16, priority: i32) -> ethornell_vm::VmResult<()> {
        if (0..0x1_0000).contains(&priority) {
            Ok(())
        } else {
            Err(ethornell_vm::VmError::Runtime(format!(
                "Graph90:{selector:02X} priority {priority} is outside 0..=65535"
            )))
        }
    }

    fn graph90_set_mask_alpha_recursive(&mut self, object: i32, mask_alpha: i32) {
        let objects = self.graph_native_descendants_inclusive(object);
        for &object in &objects {
            let properties = self.graph_object_properties.entry(object).or_default();
            properties.mask_alpha = mask_alpha;
            properties.native.mask_alpha = mask_alpha;
        }
        for object in objects {
            let _ = self.graph90_refresh_backf_primary(object);
        }
    }

    fn graph90_set_fixed_parameter_recursive(&mut self, object: i32, value: i32) {
        let fixed = value << 16;
        self.graph90_set_fixed_parameter_raw_recursive(object, fixed);
    }

    pub(super) fn graph90_set_fixed_parameter_raw_recursive(&mut self, object: i32, fixed: i32) {
        let objects = self.graph_native_descendants_inclusive(object);
        let mut geometry_refresh = Vec::new();
        for &object in &objects {
            let properties = self.graph_object_properties.entry(object).or_default();
            properties.properties.insert(0x35, (fixed, 0));
            properties.native.fixed_parameter_16_16 = fixed;
            let mode = properties
                .named_properties
                .get("target-object-mode")
                .copied()
                .unwrap_or_default();
            let secondary_parameter = match mode {
                5 => properties
                    .named_properties
                    .get("mode5-secondary-parameter")
                    .copied(),
                6 => properties
                    .named_properties
                    .get("mode6-secondary-parameter")
                    .copied(),
                _ => None,
            };
            // CDspObjSprite::SetFixedParameter (sub_428470) mirrors
            // CDspObj+0xB8's unsigned high word into the mode-5/mode-6
            // transition control at +0x240 when +0x244 == 3, then rebuilds
            // the projected/cache state through sub_428E70.
            if matches!(mode, 5 | 6) && secondary_parameter == Some(3) {
                let transition = ((fixed as u32 >> 16) & 0xffff) as i32;
                let key = if mode == 5 {
                    "mode5-transition-current"
                } else {
                    "mode6-transition-current"
                };
                properties
                    .named_properties
                    .insert(key.to_string(), transition);
                tracing::info!(
                    sprite = object,
                    mode,
                    transition_value = transition,
                    fixed_parameter_16_16 = fixed,
                    "GraphUpdateSpriteTransitionFromFixedParameter"
                );
            }
            // CDspObjSprite::SetFixedParameter (sub_428470) always rebuilds
            // mode 5 through sub_4299A0/sub_428E70/sub_429000 and mode 6
            // through sub_428E70/sub_429000/sub_42A090. +0x244 == 3 only
            // controls transition mirroring; it is not a geometry gate.
            if matches!(mode, 5 | 6) {
                geometry_refresh.push(object);
            }
        }
        for object in objects {
            let _ = self.graph90_refresh_backf_primary(object);
        }
        for object in geometry_refresh {
            let _ = self.graph90_resync_fixed_sprite_geometry(object);
        }
    }

    fn graph90_set_alpha_multiplier_recursive(&mut self, object: i32, multiplier: i32) {
        let objects = self.graph_native_descendants_inclusive(object);
        for &object in &objects {
            let before = self
                .graph_object_properties
                .get(&object)
                .map(|properties| {
                    (
                        properties.alpha_multiplier,
                        properties.transparency_parameter(),
                        properties.opacity(),
                    )
                })
                .unwrap_or((256, 0, 1.0));
            {
                let properties = self.graph_object_properties.entry(object).or_default();
                properties.alpha_multiplier = multiplier;
                properties.native.alpha_multiplier = multiplier;
            }
            let after = self
                .graph_object_properties
                .get(&object)
                .map(|properties| {
                    (
                        properties.alpha_multiplier,
                        properties.transparency_parameter(),
                        properties.opacity(),
                    )
                })
                .unwrap_or((256, 0, 1.0));
            tracing::info!(
                object,
                requested_multiplier = multiplier,
                before = ?before,
                after = ?after,
                native_owner = ?self.graph_native_owners.get(&object),
                display_parent = ?self.display_tree.parent(object),
                "GraphObjectAlphaMultiplierChanged"
            );
        }
        for object in objects {
            let _ = self.graph90_refresh_backf_primary(object);
        }
    }

    fn graph90_set_child_link_offset(&mut self, parent: i32, child: i32, offset: (i32, i32)) {
        if let Some(member) = self
            .graph_groups
            .get_mut(&parent)
            .and_then(|group| group.members.get_mut(&child))
        {
            *member = offset;
            return;
        }
        if let Some(transform) = self.graph91_object_transforms.get_mut(&child) {
            transform.attachment_offset = [offset.0, offset.1];
        }
    }

    fn graph90_child_link_offset(&self, parent: i32, child: i32) -> (i32, i32) {
        if let Some(offset) = self
            .graph_groups
            .get(&parent)
            .and_then(|group| group.members.get(&child))
        {
            return *offset;
        }
        if self.graph_native_owners.get(&child) == Some(&parent)
            && let Some(transform) = self.graph91_object_transforms.get(&child)
        {
            return (
                transform.attachment_offset[0],
                transform.attachment_offset[1],
            );
        }
        if let Some(surface) = self
            .graph_surfaces
            .get(&child)
            .filter(|surface| surface.parent_surface == Some(parent))
        {
            return (
                surface.local_x.round() as i32,
                surface.local_y.round() as i32,
            );
        }
        self.display_tree
            .local_position(child)
            .map(|(x, y)| (x.round() as i32, y.round() as i32))
            .unwrap_or_default()
    }

    pub(super) fn graph90_set_position_recursive(&mut self, object: i32, x: i32, y: i32) {
        if self.graph90_set_specialized_position(object, x, y) {
            return;
        }
        // sub_41B1D0 with its update flag set: sub_41C130 rewrites the
        // parent's member record for this object (CDspObj+0x11C parent,
        // record list at +0x12C) to the new offset from the parent.
        let relink = self.graph_native_owners.get(&object).copied().and_then(|parent| {
            let (parent_x, parent_y) = self.graph_native_base_position(parent)?;
            Some((parent, (x.wrapping_sub(parent_x), y.wrapping_sub(parent_y))))
        });
        let mut pending = vec![(object, x, y, relink.map(|(_, offset)| offset))];
        if let Some((parent, offset)) = relink {
            self.graph90_set_child_link_offset(parent, object, offset);
        }
        let mut visited = BTreeSet::new();
        while let Some((current, absolute_x, absolute_y, local)) = pending.pop() {
            if !visited.insert(current) {
                continue;
            }
            // Target sub_41B1D0 walks only the CDspObj member records at
            // +0x12C. Portable surface ownership and display-tree relations
            // are not transform parents for this setter.
            let children = self
                .graph_native_member_children(current)
                .into_iter()
                .map(|child| (child, self.graph90_child_link_offset(current, child)))
                .collect::<Vec<_>>();
            self.set_graph_object_position(current, absolute_x as f32, absolute_y as f32);
            if let Some((local_x, local_y)) = local {
                self.display_tree
                    .set_local_position(current, local_x as f32, local_y as f32);
            }
            for (child, (local_x, local_y)) in children.into_iter().rev() {
                pending.push((
                    child,
                    absolute_x.wrapping_add(local_x),
                    absolute_y.wrapping_add(local_y),
                    Some((local_x, local_y)),
                ));
            }
        }
    }

    /// Returns true when the object's vtable +44 override consumed the call.
    /// This includes target no-op overrides, which must not fall through to
    /// the base CDspObj child-propagating implementation.
    fn graph90_set_specialized_position(&mut self, object: i32, x: i32, y: i32) -> bool {
        if let Some(class) = self
            .graph_object_properties
            .get(&object)
            .and_then(|properties| properties.background.map(|state| state.class))
        {
            let changed = self
                .graph_object_properties
                .get_mut(&object)
                .and_then(|properties| properties.background.as_mut())
                .is_some_and(|state| {
                    state.set_position(x, y, self.screen_width, self.screen_height)
                });
            if changed {
                match class {
                    NativeBackgroundClass::BackS => {
                        self.graph90_layout_backs_layers(object, x, y);
                    }
                    NativeBackgroundClass::BackF => {
                        if let Some(layer) = self.graph_layers.get_mut(&object) {
                            layer.x = -x as f32;
                            layer.y = -y as f32;
                        }
                        let _ = self.graph90_refresh_backf_primary(object);
                    }
                    NativeBackgroundClass::BackMl => {
                        let properties = self.graph_object_properties.entry(object).or_default();
                        properties.native.fixed_position_x_16_16 = x;
                        properties.native.fixed_position_y_16_16 = y;
                        self.graph91_multilayer_background.object_x_16_16 = x;
                        self.graph91_multilayer_background.object_y_16_16 = y;
                        self.graph91_sync_all_multilayer_render_layers();
                    }
                    _ => {}
                }
            }
            return true;
        }

        if self.display_tree.kind(object) == Some(NativeDisplayKind::Landscape) {
            if let Some(landscape) = self.graph91_landscapes.get_mut(&object) {
                landscape.x = x;
                landscape.y = y;
            }
            let properties = self.graph_object_properties.entry(object).or_default();
            properties.native.fixed_position_x_16_16 = x << 16;
            properties.native.fixed_position_y_16_16 = y << 16;
            self.set_graph_object_position(object, x as f32, y as f32);
            return true;
        }

        false
    }

    fn graph90_set_offset_recursive(&mut self, object: i32, selector: u16, x: i32, y: i32) {
        // sub_41B2D0/sub_41B320 recurse only through CDspObj member records at
        // +0x12C. Portable display-tree/surface ownership must not inherit
        // these native offset banks.
        for object in self.graph_native_descendants_inclusive(object) {
            let (offset_x, offset_y) = {
                let properties = self.graph_object_properties.entry(object).or_default();
                properties.properties.insert(u32::from(selector), (x, y));
                match selector {
                    0x36 => {
                        properties.native.secondary_offset_x = x;
                        properties.native.secondary_offset_y = y;
                    }
                    0x37 => {
                        properties.native.primary_offset_x = x;
                        properties.native.primary_offset_y = y;
                    }
                    _ => unreachable!("only target offset selectors use this helper"),
                }
                (
                    properties
                        .native
                        .primary_offset_x
                        .saturating_add(properties.native.secondary_offset_x),
                    properties
                        .native
                        .primary_offset_y
                        .saturating_add(properties.native.secondary_offset_y),
                )
            };
            for layer_id in self.graph_target_layers(object) {
                if let Some(layer) = self.graph_layers.get_mut(&layer_id) {
                    layer.transform_x = offset_x as f32;
                    layer.transform_y = offset_y as f32;
                }
            }
        }
    }

    pub(super) fn graph90_prepare_current_background(
        &mut self,
        class: NativeBackgroundClass,
    ) -> i32 {
        let object = self.graph90_current_object();
        let previous_class = self
            .graph_object_properties
            .get(&object)
            .and_then(|properties| properties.background.map(|state| state.class));
        if previous_class != Some(class) {
            // sub_43E190 removes and destroys the prior singleton before it
            // allocates the requested concrete class. Drop class-owned render
            // materialization and restore fresh CDspObj constructor state.
            if let Some(layers) = self.graph_object_layers.remove(&object) {
                for layer in layers {
                    self.graph_layers.remove(&layer);
                }
            }
            self.graph_layers.remove(&object);
            let mut properties = RuntimeGraphObjectProperties::default();
            let mut background = NativeBackgroundState::new(class);
            // sub_43E190 re-applies the manager's stored Graph90:4C pair.
            background.active = self.graph_config.renderer_options.1;
            properties.background = Some(background);
            if class == NativeBackgroundClass::BackMl {
                // CDspObjBackML::CDspObjBackML (sub_41DB20) clears +0x7C
                // after the base constructor, matching transformed sprites.
                properties.native.fixed_position_updates_integer_position = 0;
            }
            if class == NativeBackgroundClass::BackF {
                // CDspObjBackF::CDspObjBackF (`sub_41D210`) calls
                // sub_41B600(this, 1) after the base constructor. This is
                // observable through GetMaskAlpha as soon as mask alpha or
                // alpha multiplier differs from its constructor default.
                properties.blend_mode = 1;
            }
            properties.native.priority = self.graph_default_priority.max(0) as u32;
            properties
                .named_properties
                .insert("target-current-class".to_string(), class as i32);
            properties
                .named_properties
                .insert("target-object-mode".to_string(), class as i32);
            self.graph_object_properties.insert(object, properties);
        }
        if previous_class != Some(class) {
            let draw = self.graph_config.renderer_options.0 != 0;
            self.set_graph_object_draw_enabled(object, draw);
        }
        self.display_tree.replace_kind(object, class.display_kind());
        let priority = self.graph_object_properties[&object].native.priority as i32;
        self.display_tree.set_chain_depth(object, priority);
        self.graph90_refresh_native_sort_key(object);
        object
    }

    fn graph90_sync_primary_layer(&mut self, object: i32, resource: i32, x: i32, y: i32) -> bool {
        let Some((key, source)) = self
            .resource_image_region(resource)
            .map(|(key, source)| (key.to_string(), source))
        else {
            return false;
        };
        let properties = self.graph_object_properties.entry(object).or_default();
        if let Some(background) = properties.background.as_mut() {
            background.set_position(x, y, self.screen_width, self.screen_height);
        }
        // CDspObjBackF::Draw (`sub_41D590`) passes
        // `-(parent + this[79/80])` to the bitmap blitter. These fields are
        // source-origin offsets, so their renderer-facing translation has the
        // opposite sign.
        let render_x = -x;
        let render_y = -y;
        let z = properties.native.priority as i32;
        self.display_tree.set_chain_depth(object, z);
        self.graph_object_layers
            .entry(object)
            .or_default()
            .insert(object);
        let layer = self
            .graph_layers
            .entry(object)
            .or_insert_with(|| RuntimeGraphLayer {
                hit_id: object,
                owner_object: Some(object),
                key: key.clone(),
                target_surface: None,
                x: render_x as f32,
                y: render_y as f32,
                width: source.width,
                height: source.height,
                src_x: source.x,
                src_y: source.y,
                opacity: 1.0,
                z,
                enabled: true,
                transform_x: 0.0,
                transform_y: 0.0,
                transform_z: 0,
                scale_x: 1.0,
                scale_y: 1.0,
                rotation_degrees: 0.0,
                clip: None,
            });
        layer.key = key;
        layer.x = render_x as f32;
        layer.y = render_y as f32;
        layer.width = source.width;
        layer.height = source.height;
        layer.src_x = source.x;
        layer.src_y = source.y;
        layer.z = z;
        layer.enabled = true;
        true
    }

    fn graph90_sync_backf_layers(
        &mut self,
        object: i32,
        primary_x: i32,
        primary_y: i32,
        primary: i32,
        secondary_x: i32,
        secondary_y: i32,
        secondary: i32,
        mask: i32,
        mask_parameter: i32,
    ) -> std::result::Result<(), String> {
        let Some(primary_generation) = self
            .graph_resources
            .get(&primary)
            .map(|resource| resource.generation)
        else {
            return Err(format!("primary bitmap #{primary} is not registered"));
        };
        let primary_image_region = self
            .resource_image_region(primary)
            .map(|(key, source)| (key.to_string(), source));

        let (secondary_binding, mut secondary_image) = match secondary {
            0x7000 | 0x7001 | 0x7fff | -1 => {
                (NativeBackgroundSecondaryResource::Sentinel(secondary), None)
            }
            _ => {
                let Some(generation) = self
                    .graph_resources
                    .get(&secondary)
                    .map(|resource| resource.generation)
                else {
                    return Err(format!("secondary bitmap #{secondary} is not registered"));
                };
                let image_region = self
                    .resource_image_region(secondary)
                    .map(|(key, source)| (key.to_string(), source));
                (
                    NativeBackgroundSecondaryResource::Bitmap {
                        handle: secondary,
                        generation,
                    },
                    image_region,
                )
            }
        };

        // Target format 1 is the XRGB-style path. Its high byte is copied as
        // payload by the software routines but is not source-alpha coverage.
        // The GPU compatibility renderer does interpret byte 3 as alpha, so
        // only for a format-1 secondary carrying non-opaque legacy bytes,
        // materialize a BackF-local opaque view. This keeps selector 128 as a
        // true copy without mutating the source bitmap or changing generation.
        if secondary_image.is_some()
            && self.bitmap_formats.get(&secondary).copied() == Some(1)
            && let Some(mut image) = self.graph_bitmap_image(secondary)
        {
            let needs_opaque_view = image
                .rgba
                .as_chunks::<4>()
                .0
                .iter()
                .any(|pixel| pixel[3] != 0xff);
            if needs_opaque_view {
                for pixel in image.rgba.as_chunks_mut::<4>().0 {
                    pixel[3] = 0xff;
                }
                let width = image.width;
                let height = image.height;
                let key = format!("runtime:backf:{object}:secondary");
                self.store_graph_image(key.clone(), image);
                secondary_image = Some((
                    key,
                    crate::graph::RuntimeClipRect {
                        x: 0.0,
                        y: 0.0,
                        width: width as f32,
                        height: height as f32,
                    },
                ));
            }
        }

        let z = self.graph_object_properties[&object].native.priority as i32;
        let properties = self.graph_object_properties.entry(object).or_default();
        properties.format_resource = Some(primary);
        let Some(background) = properties.background.as_mut() else {
            return Err("BackF object has no native background state".to_string());
        };
        background.set_position(primary_x, primary_y, self.screen_width, self.screen_height);
        background.resource_binding = Some((primary, primary_generation));
        background.secondary_resource_binding = Some(secondary_binding);
        let Some(backf) = background.backf.as_mut() else {
            return Err("BackF object lost its subclass state".to_string());
        };
        backf.secondary_x = secondary_x;
        backf.secondary_y = secondary_y;

        self.display_tree.set_chain_depth(object, z);
        let mut owned_layers = BTreeSet::new();
        if let Some((primary_key, primary_source)) = primary_image_region {
            owned_layers.insert(object);
            self.graph_layers.insert(
                object,
                RuntimeGraphLayer {
                    hit_id: object,
                    owner_object: Some(object),
                    key: primary_key,
                    target_surface: None,
                    x: -primary_x as f32,
                    y: -primary_y as f32,
                    width: primary_source.width,
                    height: primary_source.height,
                    src_x: primary_source.x,
                    src_y: primary_source.y,
                    opacity: 1.0,
                    z,
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
        } else {
            // Target configuration stores the bitmap handle/generation even
            // when no renderer-facing image has materialized yet. Do not turn
            // that legal staging state into a VM error or keep drawing stale
            // pixels from the previous BackF binding.
            self.graph_layers.remove(&object);
        }
        if let Some((key, source)) = secondary_image {
            owned_layers.insert(BACK_F_SECONDARY_LAYER_ID);
            self.graph_layers.insert(
                BACK_F_SECONDARY_LAYER_ID,
                RuntimeGraphLayer {
                    hit_id: object,
                    owner_object: Some(object),
                    key,
                    target_surface: None,
                    x: -secondary_x as f32,
                    y: -secondary_y as f32,
                    width: source.width,
                    height: source.height,
                    src_x: source.x,
                    src_y: source.y,
                    opacity: 1.0,
                    z,
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
        } else if matches!(secondary, 0x7000 | 0x7001) {
            // sub_41D590 uses 0x7000/0x7001 as black/white sentinel
            // backgrounds. It conditionally clears only when the primary
            // fails to cover the current draw context; materializing the
            // solid layer unconditionally is equivalent because the primary
            // pass is opaque wherever it covers that layer.
            let width = self.screen_width.max(1) as u32;
            let height = self.screen_height.max(1) as u32;
            let channel: u8 = if secondary == 0x7001 { 0xff } else { 0x00 };
            let key = format!("runtime:backf:{object}:sentinel:{secondary}");
            self.store_graph_image(
                key.clone(),
                DecodedImage {
                    width,
                    height,
                    rgba: [channel, channel, channel, 0xff]
                        .repeat(width as usize * height as usize),
                },
            );
            owned_layers.insert(BACK_F_SECONDARY_LAYER_ID);
            self.graph_layers.insert(
                BACK_F_SECONDARY_LAYER_ID,
                RuntimeGraphLayer {
                    hit_id: object,
                    owner_object: Some(object),
                    key,
                    target_surface: None,
                    x: 0.0,
                    y: 0.0,
                    width: width as f32,
                    height: height as f32,
                    src_x: 0.0,
                    src_y: 0.0,
                    opacity: 1.0,
                    z,
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
        } else {
            self.graph_layers.remove(&BACK_F_SECONDARY_LAYER_ID);
        }
        self.graph_object_layers.insert(object, owned_layers);

        // sub_43D750 applies sub_41D350 (primary/secondary) before
        // sub_41D440 (mask). If mask validation fails, the target keeps the
        // newly committed primary/secondary fields and the previous mask
        // fields, and it does not apply the new alpha. Preserve that staged
        // mutation instead of making 90:43 an all-or-nothing transaction.
        let mask_binding = if mask == -1 {
            None
        } else {
            let Some(generation) = self
                .graph_resources
                .get(&mask)
                .map(|resource| resource.generation)
            else {
                let _ = self.graph90_refresh_backf_primary(object);
                return Err(format!("mask bitmap #{mask} is not registered"));
            };
            let format = self.bitmap_formats.get(&mask).copied().unwrap_or(2);
            if format != 3 {
                let _ = self.graph90_refresh_backf_primary(object);
                return Err(format!(
                    "mask bitmap #{mask} has target format {format}, expected format 3"
                ));
            }
            Some((mask, generation))
        };
        let properties = self.graph_object_properties.entry(object).or_default();
        let Some(backf) = properties
            .background
            .as_mut()
            .and_then(|background| background.backf.as_mut())
        else {
            return Err("BackF object lost its subclass state".to_string());
        };
        backf.mask_resource_binding = mask_binding;
        // sub_41D440's mask==-1 fast path changes only +0x15c. The old
        // parameter/generation fields remain physically present but are
        // ignored while the handle is -1.
        if mask_binding.is_some() {
            backf.mask_parameter = mask_parameter;
        }
        Ok(())
    }

    fn graph90_backf_mask_control_value(
        properties: &RuntimeGraphObjectProperties,
        backf: crate::native_background::NativeBackFState,
    ) -> i32 {
        if backf.mask_control_enabled == 0 {
            return 0;
        }
        let transparency = properties.transparency_parameter();
        let mode_zero_parameter =
            ((properties.native.fixed_parameter_16_16 as u32 >> 16) & 0xffff) as i32;
        match backf.mask_control_mode {
            1 => 256 - transparency,
            2 => mode_zero_parameter,
            3 => 256 - mode_zero_parameter,
            _ => transparency,
        }
    }

    /// Rebuilds the renderer-facing BackF primary when the target takes a
    /// pixel path that cannot be represented by a single ordinary layer.
    /// The source resource itself is never modified: target generation checks
    /// continue to refer to the original bitmap handle.
    pub(super) fn graph90_refresh_backf_primary(
        &mut self,
        object: i32,
    ) -> std::result::Result<(), String> {
        let Some(properties) = self.graph_object_properties.get(&object).cloned() else {
            return Ok(());
        };
        let Some(background) = properties.background else {
            return Ok(());
        };
        if background.class != NativeBackgroundClass::BackF {
            return Ok(());
        }
        let Some(backf) = background.backf else {
            return Err("BackF object lost its subclass state".to_string());
        };
        let Some((primary, _generation)) = background.resource_binding else {
            return Ok(());
        };
        let (primary_x, primary_y) = match background.position {
            crate::native_background::NativeBackgroundPosition::BackFSource { x, y } => (x, y),
            _ => (0, 0),
        };
        let Some(mut primary_image) = self.graph_bitmap_image(primary) else {
            // CDspObjBackF configuration is allowed to precede renderer-side
            // image materialization. Keep the native handle/generation state
            // and simply defer the draw-facing refresh.
            self.graph_layers.remove(&object);
            if let Some(layers) = self.graph_object_layers.get_mut(&object) {
                layers.remove(&object);
            }
            return Ok(());
        };
        let primary_format = self.bitmap_formats.get(&primary).copied().unwrap_or(2);
        let transparency = properties.transparency_parameter();

        let runtime_image = if let Some((mask, _generation)) = backf.mask_resource_binding {
            // sub_411B80/sub_411EA0 only execute the mask pixel loop for
            // target bitmap format 1. A format-2 primary reaches the native
            // routine but returns without modifying the destination. Preserve
            // that no-op instead of applying the format-1 mask equation to an
            // alpha-bearing source.
            if primary_format != 1 {
                for pixel in primary_image.rgba.as_chunks_mut::<4>().0 {
                    pixel[3] = 0;
                }
                Some(primary_image)
            } else {
                let Some(mask_image) = self.graph_bitmap_image(mask) else {
                    self.graph_layers.remove(&object);
                    if let Some(layers) = self.graph_object_layers.get_mut(&object) {
                        layers.remove(&object);
                    }
                    return Ok(());
                };
                let extra_control = Self::graph90_backf_mask_control_value(&properties, backf);
                for y in 0..primary_image.height {
                    for x in 0..primary_image.width {
                        let destination_x = x as i32 - primary_x;
                        let destination_y = y as i32 - primary_y;
                        let pixel_index = ((y * primary_image.width + x) * 4) as usize;
                        let weight = if destination_x >= 0
                            && destination_y >= 0
                            && destination_x < mask_image.width as i32
                            && destination_y < mask_image.height as i32
                        {
                            let mask_index = ((destination_y as u32 * mask_image.width
                                + destination_x as u32)
                                * 4) as usize;
                            backf_mask_weight(
                                mask_image.rgba[mask_index + 3],
                                backf.mask_parameter,
                                transparency,
                                extra_control,
                            )
                        } else {
                            0
                        };
                        // wgpu exposes UNORM alpha in 0..255 while the target
                        // compositor works in 0..256. Round to the closest UNORM
                        // coverage; RGB remains the target primary source.
                        primary_image.rgba[pixel_index + 3] =
                            ((u32::from(weight) * 255 + 128) >> 8).min(255) as u8;
                    }
                }
                Some(primary_image)
            }
        } else if let Some(NativeBackgroundSecondaryResource::Sentinel(sentinel)) =
            background.secondary_resource_binding
        {
            // The C0/C1 equations below are recovered for target format 1.
            // Format 2 dispatches to separate alpha-aware routines; keep the
            // original bitmap there and let graph_draw_items retain the raw
            // C0/C1 selector rather than applying the wrong format-1 formula.
            if primary_format != 1 {
                None
            } else {
                // Unmasked sentinel path: 0x7000 (and -1/0x7fff when alpha is
                // non-zero) uses C0, i.e. source -> black. 0x7001 uses C1,
                // source -> white. Format-1's fourth byte is not renderer
                // coverage, so force the generated compatibility texture
                // opaque after transforming RGB.
                let toward_white = sentinel == 0x7001;
                let needs_black_fade = matches!(sentinel, 0x7000) || transparency != 0;
                if toward_white || needs_black_fade {
                    let inverse = 256 - transparency;
                    for pixel in primary_image.rgba.as_chunks_mut::<4>().0 {
                        for channel in &mut pixel[..3] {
                            let source = i32::from(*channel);
                            let base = (source * inverse) >> 8;
                            let bias = if toward_white {
                                (255 * transparency) >> 8
                            } else {
                                0
                            };
                            *channel = (base + bias).clamp(0, 255) as u8;
                        }
                        pixel[3] = 0xff;
                    }
                    Some(primary_image)
                } else {
                    None
                }
            }
        } else {
            // Mixed format path: selector 1 with a format-1 primary still
            // treats the fourth byte as XRGB payload, while the renderer's
            // unrecovered-selector fallback uses source alpha. Give only that
            // compatibility fallback an opaque BackF-local source view.
            let secondary_format = match background.secondary_resource_binding {
                Some(NativeBackgroundSecondaryResource::Bitmap { handle, .. }) => {
                    self.bitmap_formats.get(&handle).copied().unwrap_or(2)
                }
                _ => 2,
            };
            if primary_format == 1 && secondary_format != 1 {
                let needs_opaque_view = primary_image
                    .rgba
                    .as_chunks::<4>()
                    .0
                    .iter()
                    .any(|pixel| pixel[3] != 0xff);
                if needs_opaque_view {
                    for pixel in primary_image.rgba.as_chunks_mut::<4>().0 {
                        pixel[3] = 0xff;
                    }
                    Some(primary_image)
                } else {
                    None
                }
            } else {
                None
            }
        };

        if let Some(image) = runtime_image {
            let width = image.width;
            let height = image.height;
            let key = format!("runtime:backf:{object}:primary");
            self.store_graph_image(key.clone(), image);
            let Some(layer) = self.graph_layers.get_mut(&object) else {
                return Ok(());
            };
            layer.key = key;
            layer.src_x = 0.0;
            layer.src_y = 0.0;
            layer.width = width as f32;
            layer.height = height as f32;
        } else {
            // Restore the original primary resource when no special pixel
            // path is active (normal secondary + no mask, or zero-alpha
            // -1/0x7fff sentinel).
            let Some((key, source)) = self
                .resource_image_region(primary)
                .map(|(key, source)| (key.to_string(), source))
            else {
                self.graph_layers.remove(&object);
                if let Some(layers) = self.graph_object_layers.get_mut(&object) {
                    layers.remove(&object);
                }
                return Ok(());
            };
            let Some(layer) = self.graph_layers.get_mut(&object) else {
                return Ok(());
            };
            layer.key = key;
            layer.src_x = source.x;
            layer.src_y = source.y;
            layer.width = source.width;
            layer.height = source.height;
        }
        Ok(())
    }

    fn graph90_sync_backn_layer(&mut self, object: i32, resource: i32) -> bool {
        let generation = self
            .graph_resources
            .get(&resource)
            .map(|resource| resource.generation);
        let Some((key, source)) = self
            .resource_image_region(resource)
            .map(|(key, source)| (key.to_string(), source))
        else {
            return false;
        };
        let properties = self.graph_object_properties.entry(object).or_default();
        properties.format_resource = Some(resource);
        if let Some(background) = properties.background.as_mut() {
            background.resource_binding = generation.map(|generation| (resource, generation));
        }
        let z = properties.native.priority as i32;
        self.graph_object_layers
            .entry(object)
            .or_default()
            .insert(object);
        self.display_tree.set_chain_depth(object, z);
        self.graph_layers.insert(
            object,
            RuntimeGraphLayer {
                hit_id: object,
                owner_object: Some(object),
                key,
                target_surface: None,
                x: 0.0,
                y: 0.0,
                width: source.width,
                height: source.height,
                src_x: source.x,
                src_y: source.y,
                opacity: 1.0,
                z,
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
        true
    }

    fn graph90_sync_backb_layers(&mut self, object: i32, primary: i32, secondary: i32) -> bool {
        let Some(primary_generation) = self
            .graph_resources
            .get(&primary)
            .map(|resource| resource.generation)
        else {
            return false;
        };
        let Some((primary_key, primary_source)) = self
            .resource_image_region(primary)
            .map(|(key, source)| (key.to_string(), source))
        else {
            return false;
        };
        let (secondary_binding, secondary_image) = match secondary {
            0x7000 | 0x7001 => (NativeBackgroundSecondaryResource::Sentinel(secondary), None),
            _ => {
                let Some(generation) = self
                    .graph_resources
                    .get(&secondary)
                    .map(|resource| resource.generation)
                else {
                    return false;
                };
                let Some((key, source)) = self
                    .resource_image_region(secondary)
                    .map(|(key, source)| (key.to_string(), source))
                else {
                    return false;
                };
                (
                    NativeBackgroundSecondaryResource::Bitmap {
                        handle: secondary,
                        generation,
                    },
                    Some((key, source)),
                )
            }
        };

        let properties = self.graph_object_properties.entry(object).or_default();
        properties.format_resource = Some(primary);
        if let Some(background) = properties.background.as_mut() {
            background.resource_binding = Some((primary, primary_generation));
            background.secondary_resource_binding = Some(secondary_binding);
        }
        let z = properties.native.priority as i32;
        self.display_tree.set_chain_depth(object, z);
        let mut owned_layers = BTreeSet::from([object]);
        self.graph_layers.insert(
            object,
            RuntimeGraphLayer {
                hit_id: object,
                owner_object: Some(object),
                key: primary_key,
                target_surface: None,
                x: 0.0,
                y: 0.0,
                width: primary_source.width,
                height: primary_source.height,
                src_x: primary_source.x,
                src_y: primary_source.y,
                opacity: 1.0,
                z,
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
        if let Some((key, source)) = secondary_image {
            owned_layers.insert(BACK_B_SECONDARY_LAYER_ID);
            self.graph_layers.insert(
                BACK_B_SECONDARY_LAYER_ID,
                RuntimeGraphLayer {
                    hit_id: object,
                    owner_object: Some(object),
                    key,
                    target_surface: None,
                    x: 0.0,
                    y: 0.0,
                    width: source.width,
                    height: source.height,
                    src_x: source.x,
                    src_y: source.y,
                    opacity: 1.0,
                    z,
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
        } else {
            self.graph_layers.remove(&BACK_B_SECONDARY_LAYER_ID);
        }
        self.graph_object_layers.insert(object, owned_layers);
        true
    }

    fn graph90_back_resource_matches_screen(&self, resource: i32) -> bool {
        if !self.graph_resources.contains_key(&resource) {
            return false;
        }
        let Some((_key, source)) = self.resource_image_region(resource) else {
            return false;
        };
        let width = self.screen_width.max(1) as f32;
        let height = self.screen_height.max(1) as f32;
        if source.width != width || source.height != height {
            return false;
        }
        match (
            self.bitmap_formats.get(&resource),
            self.bitmap_formats.get(&NATIVE_SCREEN_BITMAP),
        ) {
            (Some(resource), Some(screen)) => resource == screen,
            _ => true,
        }
    }

    fn graph90_sync_backs_layers(
        &mut self,
        object: i32,
        resources: [i32; 4],
        x: i32,
        y: i32,
    ) -> bool {
        if !resources
            .into_iter()
            .all(|resource| self.graph90_back_resource_matches_screen(resource))
        {
            return false;
        }
        let bindings =
            resources.map(|resource| (resource, self.graph_resources[&resource].generation));
        let properties = self.graph_object_properties.entry(object).or_default();
        properties.format_resource = Some(resources[0]);
        if let Some(background) = properties.background.as_mut() {
            background.resource_binding = Some(bindings[0]);
            background.quad_resource_bindings = Some(bindings);
        }
        self.graph90_layout_backs_layers(object, x, y);
        true
    }

    fn graph90_layout_backs_layers(&mut self, object: i32, x: i32, y: i32) {
        let Some(bindings) = self
            .graph_object_properties
            .get(&object)
            .and_then(|properties| properties.background)
            .and_then(|background| background.quad_resource_bindings)
        else {
            return;
        };
        let resources = bindings.map(|(resource, _generation)| resource);
        let Some(images) = resources
            .into_iter()
            .map(|resource| {
                self.resource_image_region(resource)
                    .map(|(key, source)| (key.to_string(), source))
            })
            .collect::<Option<Vec<_>>>()
        else {
            return;
        };
        let width = self.screen_width.max(1);
        let height = self.screen_height.max(1);
        let quadrants = [
            (0, 0, x, y, width - x, height - y),
            (width - x, 0, 0, y, x, height - y),
            (0, height - y, x, 0, width - x, y),
            (width - x, height - y, 0, 0, x, y),
        ];
        let z = self.graph_object_properties[&object].native.priority as i32;
        let layer_ids = [
            object,
            BACK_S_ADDITIONAL_LAYER_IDS[0],
            BACK_S_ADDITIONAL_LAYER_IDS[1],
            BACK_S_ADDITIONAL_LAYER_IDS[2],
        ];
        self.display_tree.set_chain_depth(object, z);
        self.graph_object_layers
            .insert(object, layer_ids.into_iter().collect());
        for (index, layer_id) in layer_ids.into_iter().enumerate() {
            let (dest_x, dest_y, source_x, source_y, draw_width, draw_height) = quadrants[index];
            let (key, source) = &images[index];
            self.graph_layers.insert(
                layer_id,
                RuntimeGraphLayer {
                    hit_id: object,
                    owner_object: Some(object),
                    key: key.clone(),
                    target_surface: None,
                    x: dest_x as f32,
                    y: dest_y as f32,
                    width: draw_width as f32,
                    height: draw_height as f32,
                    src_x: source.x + source_x as f32,
                    src_y: source.y + source_y as f32,
                    opacity: 1.0,
                    z,
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
        }
    }

    pub(super) fn graph90_refresh_background_screen_layout(&mut self) {
        let backs = self
            .graph_object_properties
            .iter()
            .filter_map(|(&object, properties)| {
                let background = properties.background?;
                (background.class == NativeBackgroundClass::BackS).then(|| {
                    background
                        .integer_position()
                        .map(|position| (object, position))
                })?
            })
            .collect::<Vec<_>>();
        for (object, (x, y)) in backs {
            self.graph90_layout_backs_layers(object, x, y);
        }
    }

    fn graph90_sync_sprite_primary_layer(
        &mut self,
        object: i32,
        resource: i32,
        configured_position: Option<(f32, f32)>,
    ) -> bool {
        let Some((key, source)) = self
            .resource_image_region(resource)
            .map(|(key, source)| (key.to_string(), source))
        else {
            return false;
        };
        let properties = self.graph_object_properties.entry(object).or_default();
        let (x, y) = configured_position
            .or_else(|| {
                self.graph_layers
                    .get(&object)
                    .map(|layer| (layer.x, layer.y))
            })
            .unwrap_or((
                properties.native.position_x as f32,
                properties.native.position_y as f32,
            ));
        let z = properties.native.priority as i32;
        // `RuntimeGraphLayer::enabled` is a host/materialization-local gate.
        // Native CDspObj visibility is evaluated dynamically by
        // `object_enabled_for_layer()` from the separate +0x04 enabled and
        // +0x14 draw-enabled fields. Caching either native gate here makes a
        // bitmap replacement performed while hidden permanently latch the
        // materialized layer off until some later rebuild.
        let layer_enabled = true;

        self.display_tree
            .register(object, NativeDisplayKind::Sprite);
        self.display_tree.set_local_position(object, x, y);
        self.display_tree.set_chain_depth(object, z);
        self.graph_object_layers
            .entry(object)
            .or_default()
            .insert(object);
        let layer = self
            .graph_layers
            .entry(object)
            .or_insert_with(|| RuntimeGraphLayer {
                hit_id: object,
                owner_object: Some(object),
                key: key.clone(),
                target_surface: None,
                x,
                y,
                width: source.width,
                height: source.height,
                src_x: source.x,
                src_y: source.y,
                opacity: 1.0,
                z,
                enabled: layer_enabled,
                transform_x: 0.0,
                transform_y: 0.0,
                transform_z: 0,
                scale_x: 1.0,
                scale_y: 1.0,
                rotation_degrees: 0.0,
                clip: None,
            });
        layer.hit_id = object;
        layer.owner_object = Some(object);
        layer.key = key;
        layer.x = x;
        layer.y = y;
        layer.width = source.width;
        layer.height = source.height;
        layer.src_x = source.x;
        layer.src_y = source.y;
        layer.z = z;
        layer.enabled = layer_enabled;
        true
    }

    fn graph90_sync_mode2_primary_layer(&mut self, args: &[i32]) -> bool {
        if args.len() != 13 {
            return false;
        }
        let object = args[0];
        let resource = args[3];
        let Some((_, resource_region)) = self.resource_image_region(resource) else {
            return false;
        };

        // Current target sub_4275B0 does not interpret args 4..9 as a source
        // rectangle and destination extent. It configures an affine transform
        // over the *entire* bitmap:
        //   args[4]/[5] -> pixel transform/pivot terms, converted to 16.16
        //   args[6]     -> signed 16.16 rotation in degrees
        //   args[7]/[8] -> signed 16.16 X/Y stretch ratios
        //   args[9]     -> raster/blit flag stored at Sprite+0x280
        // sub_4291E0/sub_429220 then compute a raster-origin offset and
        // sub_4281E0 draws at ordinary object position minus that origin.
        //
        // The portable quad renderer can represent the same ordinary affine
        // geometry for positive stretch ratios by converting the target pivot
        // transform into an equivalent center-rotated quad. This preserves the
        // critical distinction between CDspObj position (args 1/2) and raster
        // top-left; it deliberately does not invent source cropping.
        let scale_x = args[7] as f32 / 65_536.0;
        let scale_y = args[8] as f32 / 65_536.0;
        if !scale_x.is_finite() || !scale_y.is_finite() || scale_x <= 0.0 || scale_y <= 0.0 {
            return false;
        }
        let rotation_degrees = args[6] as f32 / 65_536.0;
        let pivot_x = args[4] as f32;
        let pivot_y = args[5] as f32;
        let source_width = resource_region.width.max(0.0);
        let source_height = resource_region.height.max(0.0);
        if source_width <= 0.0 || source_height <= 0.0 {
            return false;
        }

        if !self.graph90_sync_sprite_primary_layer(
            object,
            resource,
            Some((args[1] as f32, args[2] as f32)),
        ) {
            return false;
        }

        let source_center_x = source_width * 0.5;
        let source_center_y = source_height * 0.5;
        let local_center_x = (source_center_x - pivot_x) * scale_x;
        let local_center_y = (source_center_y - pivot_y) * scale_y;
        let radians = rotation_degrees.to_radians();
        let (sin, cos) = radians.sin_cos();
        let rotated_center_x = local_center_x * cos - local_center_y * sin;
        let rotated_center_y = local_center_x * sin + local_center_y * cos;
        let world_center_x = args[1] as f32 + pivot_x + rotated_center_x;
        let world_center_y = args[2] as f32 + pivot_y + rotated_center_y;
        let destination_width = source_width * scale_x;
        let destination_height = source_height * scale_y;

        if let Some(layer) = self.graph_layers.get_mut(&object) {
            layer.src_x = resource_region.x;
            layer.src_y = resource_region.y;
            layer.width = source_width;
            layer.height = source_height;
            layer.scale_x = scale_x;
            layer.scale_y = scale_y;
            layer.rotation_degrees = rotation_degrees;
            // Runtime renderer rotates around the destination quad center.
            // Choose the unrotated top-left so that this center equals the
            // target transform of the source bitmap center about args[4]/[5].
            layer.x = world_center_x - destination_width * 0.5;
            layer.y = world_center_y - destination_height * 0.5;
        }
        let raster_x = world_center_x - destination_width * 0.5;
        let raster_y = world_center_y - destination_height * 0.5;
        let properties = self.graph_object_properties.entry(object).or_default();
        properties.native.position_x = args[1];
        properties.native.position_y = args[2];
        // sub_4281E0 obtains the ordinary composite position and subtracts
        // Sprite+0x2DC/+0x2E0 for mode 2. Preserve that separation so a later
        // vtable+0x28 position animation moves the object anchor without
        // discarding its affine raster origin.
        properties.named_properties.insert(
            "mode2-origin-offset-x".to_string(),
            (args[1] as f32 - raster_x).round() as i32,
        );
        properties.named_properties.insert(
            "mode2-origin-offset-y".to_string(),
            (args[2] as f32 - raster_y).round() as i32,
        );
        true
    }

    fn graph90_sync_mode3_primary_layer(&mut self, args: &[i32]) -> bool {
        if args.len() != 10 {
            return false;
        }
        self.graph90_sync_sprite_primary_layer(
            args[0],
            args[3],
            Some((args[1] as f32, args[2] as f32)),
        )
    }

    fn graph90_sync_mode4_primary_layer(&mut self, args: &[i32]) -> bool {
        if args.len() != 10 {
            return false;
        }
        self.graph90_sync_sprite_primary_layer(
            args[0],
            args[3],
            Some((args[1] as f32, args[2] as f32)),
        )
    }

    /// sub_41B4C0 resolves the mode-5/mode-6 transform vector by adding
    /// CDspObj +0x4C/+0x50/+0x54, +0x5C/+0x60/+0x64 and
    /// +0x6C/+0x70/+0x74.  The latter two banks are Graph91:37 and 91:36;
    /// they participate before perspective projection and must not be applied
    /// later as an ordinary renderer translation.
    fn graph90_resolve_projected_sprite_vector(
        &self,
        mut args: NativeMode5NodeArgs,
    ) -> NativeMode5NodeArgs {
        if let Some(transform) = self.graph91_object_transforms.get(&args.node_id) {
            args.position_x = args
                .position_x
                .wrapping_add(transform.primary_vector[0])
                .wrapping_add(transform.secondary_vector[0]);
            args.position_y = args
                .position_y
                .wrapping_add(transform.primary_vector[1])
                .wrapping_add(transform.secondary_vector[1]);
            args.position_z = args
                .position_z
                .wrapping_add(transform.primary_vector[2])
                .wrapping_add(transform.secondary_vector[2]);
        }
        args
    }

    fn graph90_sync_mode6_primary_layer(&mut self, args: &[i32]) -> bool {
        if args.len() != 20 {
            return false;
        }
        // Mode 6 shares the leading projection state with mode 5.  Target
        // sub_427D90 stores the primary at +0x150 and fixed-point position /
        // projection fields at +0x248..+0x280 before sub_42A650/sub_42A100
        // build the projected geometry.  Preserve the additional mode-6
        // fields separately below while materialising the same leading
        // projection so the object remains drawable.
        let Some(projected) = NativeMode5NodeArgs::from_source_args(args) else {
            return false;
        };
        let projected = self.graph90_resolve_projected_sprite_vector(projected);
        let Some((_, source)) = self.resource_image_region(projected.resource_id) else {
            return false;
        };
        let (screen_width, screen_height) = if self.screen_width > 0 && self.screen_height > 0 {
            (self.screen_width, self.screen_height)
        } else {
            (1280, 720)
        };
        let use_graph_center = self
            .graph_object_properties
            .get(&projected.node_id)
            .is_none_or(|properties| properties.native.use_graph_center != 0);
        let geometry = projected.screen_geometry(
            source.width,
            source.height,
            screen_width as f32,
            screen_height as f32,
            self.graph_config.center,
            use_graph_center,
        );
        if !self.graph90_sync_sprite_primary_layer(
            projected.node_id,
            projected.resource_id,
            Some((geometry.x, geometry.y)),
        ) {
            return false;
        }
        if let Some(layer) = self.graph_layers.get_mut(&projected.node_id) {
            layer.scale_x = geometry.width / source.width.max(1.0);
            layer.scale_y = geometry.height / source.height.max(1.0);
            layer.rotation_degrees = geometry.rotation_degrees;
            // sub_41B4C0 already folded the Graph91 vector banks into the
            // projected coordinates above.
            layer.transform_x = 0.0;
            layer.transform_y = 0.0;
            layer.transform_z = 0;
        }
        true
    }

    fn graph90_mode5_dynamic_state(&self, object: i32) -> NativeMode5DynamicState {
        let Some(properties) = self.graph_object_properties.get(&object) else {
            return NativeMode5DynamicState::default();
        };
        let pair = |property: u32| {
            properties
                .properties
                .get(&property)
                .copied()
                .unwrap_or((0, 0))
        };
        let (offset_delta_x_16_16, offset_delta_y_16_16) = pair(0x80);
        let (rotation_delta_16_16, uniform_scale_delta_16_16) = pair(0x81);
        let (scale_delta_x_16_16, scale_delta_y_16_16) = pair(0x82);
        let curve = pair(0x8f).0;
        // Property 0x60 -> sub_428540 stores HIWORD(arg0) at Sprite+0x300
        // and arg1 at +0x308. sub_4299A0 passes +0x308 to sub_429220
        // only while +0x300 is non-zero.
        let (packed_width_adjust_gate, width_adjust_value) = pair(0x60);
        let width_adjust_16_16 = if ((packed_width_adjust_gate as u32) >> 16) != 0 {
            width_adjust_value
        } else {
            0
        };
        let (raw_scale_x, raw_scale_y) = properties
            .properties
            .get(&0x42)
            .copied()
            .unwrap_or((0x10000, 0));
        let base_scale_x_16_16 = if raw_scale_x == 0 { 1 } else { raw_scale_x };
        let (base_scale_y_16_16, coupled_scale) = if raw_scale_y != 0 {
            (raw_scale_y, true)
        } else {
            (base_scale_x_16_16, false)
        };
        NativeMode5DynamicState {
            progress_8_24: properties.native.fixed_parameter_16_16,
            offset_delta_x_16_16,
            offset_delta_y_16_16,
            rotation_delta_16_16,
            scale_delta_x_16_16,
            scale_delta_y_16_16,
            uniform_scale_delta_16_16,
            curve,
            base_scale_x_16_16,
            base_scale_y_16_16,
            coupled_scale,
            width_adjust_16_16,
        }
    }

    fn graph90_sync_mode5_primary_layer(&mut self, args: &[i32]) -> bool {
        let Some(mut raw_mode5) = NativeMode5NodeArgs::from_source_args(args) else {
            return false;
        };
        // A failed rebuild must not leave renderer geometry from an earlier
        // mode-5 configuration attached to the reusable sprite handle.
        self.graph_mode5_render_states.remove(&raw_mode5.node_id);
        // Sprite property 0x40 replaces the constructor's integer base offset
        // pair; 0x41 replaces the base rotation and immediately rebuilds mode
        // 5 in the target. Keep the recorded constructor arguments immutable
        // and fold these virtual-property overrides only while projecting.
        if let Some(properties) = self.graph_object_properties.get(&raw_mode5.node_id) {
            if let Some((x, y)) = properties.properties.get(&0x40).copied() {
                raw_mode5.anchor_x = x;
                raw_mode5.anchor_y = y;
            }
            if let Some((rotation, _)) = properties.properties.get(&0x41).copied() {
                raw_mode5.rotation = rotation;
            }
        }
        let mode5 = self.graph90_resolve_projected_sprite_vector(raw_mode5);
        let Some((_, source)) = self.resource_image_region(mode5.resource_id) else {
            return false;
        };
        let (screen_width, screen_height) = if self.screen_width > 0 && self.screen_height > 0 {
            (self.screen_width, self.screen_height)
        } else {
            (1280, 720)
        };
        let use_graph_center = self
            .graph_object_properties
            .get(&mode5.node_id)
            .is_none_or(|properties| properties.native.use_graph_center != 0);
        let dynamic = self.graph90_mode5_dynamic_state(mode5.node_id);
        let geometry = mode5.screen_geometry_mode5_exact(
            source.width,
            source.height,
            screen_width as f32,
            screen_height as f32,
            self.graph_config.center,
            use_graph_center,
            dynamic,
        );

        let secondary = args.get(5).copied().unwrap_or(-1);
        let transition_value = self
            .graph_object_properties
            .get(&mode5.node_id)
            .and_then(|properties| {
                properties
                    .named_properties
                    .get("mode5-transition-current")
                    .copied()
            })
            .unwrap_or_else(|| args.get(6).copied().unwrap_or_default());

        // CDspObjSprite mode 5 does not draw `primary_bitmap` directly when
        // an optional secondary bitmap exists. sub_428E70 resolves both
        // descriptors and sub_40C0F0 builds the cache at sprite+0x220; the
        // case-5 draw path consumes that cache. Materialize the same logical
        // image before binding the renderer layer.
        let mode5_cache = if secondary != -1 {
            let primary_format = self.bitmap_formats.get(&mode5.resource_id).copied();
            let secondary_format = self.bitmap_formats.get(&secondary).copied();
            if primary_format != secondary_format || !matches!(primary_format, Some(1 | 2)) {
                tracing::warn!(
                    sprite = mode5.node_id,
                    primary = mode5.resource_id,
                    secondary,
                    ?primary_format,
                    ?secondary_format,
                    "mode-5 dual bitmap formats do not match the native cache path"
                );
                return false;
            }
            let Some(primary_image) = self.graph_bitmap_image(mode5.resource_id) else {
                return false;
            };
            let Some(secondary_image) = self.graph_bitmap_image(secondary) else {
                return false;
            };
            if (primary_image.width, primary_image.height)
                != (secondary_image.width, secondary_image.height)
            {
                tracing::warn!(
                    sprite = mode5.node_id,
                    primary = mode5.resource_id,
                    secondary,
                    primary_size = ?(primary_image.width, primary_image.height),
                    secondary_size = ?(secondary_image.width, secondary_image.height),
                    "mode-5 dual bitmap dimensions do not match"
                );
                return false;
            }
            let Some(cache_image) = crate::bitmap_blend::crossfade_cache(
                &primary_image,
                &secondary_image,
                primary_format.unwrap_or_default(),
                transition_value,
            ) else {
                return false;
            };
            let nonzero_alpha = cache_image
                .rgba
                .as_chunks::<4>()
                .0
                .iter()
                .filter(|pixel| pixel[3] != 0)
                .count()
                .min(i32::MAX as usize) as i32;
            let max_alpha = cache_image
                .rgba
                .as_chunks::<4>()
                .0
                .iter()
                .map(|pixel| pixel[3])
                .max()
                .unwrap_or_default() as i32;
            let cache_key = format!("runtime:sprite-mode5:{}:cache", mode5.node_id);
            self.store_graph_image(cache_key.clone(), cache_image);
            let properties = self
                .graph_object_properties
                .entry(mode5.node_id)
                .or_default();
            properties
                .named_properties
                .insert("mode5-cache-nonzero-alpha".to_string(), nonzero_alpha);
            properties
                .named_properties
                .insert("mode5-cache-max-alpha".to_string(), max_alpha);
            Some(cache_key)
        } else {
            None
        };

        let synced = self.graph90_sync_sprite_primary_layer(
            mode5.node_id,
            mode5.resource_id,
            Some((geometry.x, geometry.y)),
        );
        if synced {
            self.graph_mode5_render_states.insert(
                mode5.node_id,
                RuntimeMode5RenderState {
                    local_affine_quad: geometry.local_affine_quad,
                    linear_sampling: mode5.interpolation,
                },
            );
            if let Some(layer) = self.graph_layers.get_mut(&mode5.node_id) {
                if let Some(cache_key) = mode5_cache {
                    layer.key = cache_key;
                    layer.src_x = 0.0;
                    layer.src_y = 0.0;
                }
                layer.scale_x = geometry.width / source.width.max(1.0);
                layer.scale_y = geometry.height / source.height.max(1.0);
                layer.rotation_degrees = geometry.rotation_degrees;
                // Graph91:37/36 (+0x5C..+0x64 / +0x6C..+0x74) were
                // consumed by sub_41B4C0 before projection. Graph90:37/36 are
                // different fields: integer CDspObj offset banks at
                // +0x38/+0x3C and +0x40/+0x44. sub_41B260 adds those *after*
                // the projected ordinary position, so preserve them as the
                // renderer translation on every mode-5 resync.
                let native = self
                    .graph_object_properties
                    .get(&mode5.node_id)
                    .map(|properties| properties.native)
                    .unwrap_or_default();
                layer.transform_x = native
                    .primary_offset_x
                    .saturating_add(native.secondary_offset_x)
                    as f32;
                layer.transform_y = native
                    .primary_offset_y
                    .saturating_add(native.secondary_offset_y)
                    as f32;
                layer.transform_z = 0;
            }
            let properties = self
                .graph_object_properties
                .entry(mode5.node_id)
                .or_default();
            // +0x4C/+0x50/+0x54 retain the raw vtable+60 vector.  The
            // resolved mode5 value above additionally includes the +0x5C and
            // +0x6C vector banks and must not be written back here, otherwise
            // a later resync would add those banks twice.
            properties.native.fixed_position_x_16_16 = raw_mode5.position_x;
            properties.native.fixed_position_y_16_16 = raw_mode5.position_y;
            properties.native.fixed_position_z_16_16 = raw_mode5.position_z;
            // sub_429AF0 writes the ordinary CDspObj position separately
            // from the +0x4C/+0x50/+0x54 fixed-point vector.  Keep that
            // distinction: renderer raster placement uses ordinary position
            // minus the mode-5 origin offset, while later vtable+60 updates
            // continue from the fixed-point vector.
            properties.native.position_x = geometry.object_x.floor() as i32;
            properties.native.position_y = geometry.object_y.floor() as i32;
            properties.named_properties.insert(
                "mode5-origin-offset-x".to_string(),
                geometry.origin_offset_x.round() as i32,
            );
            properties.named_properties.insert(
                "mode5-origin-offset-y".to_string(),
                geometry.origin_offset_y.round() as i32,
            );
            properties.named_properties.insert(
                "mode5-perspective-scale-16-16".to_string(),
                (geometry.scale * 65_536.0).round() as i32,
            );
            properties
                .named_properties
                .insert("mode5-transition-current".to_string(), transition_value);
            self.display_tree.set_local_position(
                mode5.node_id,
                geometry.object_x,
                geometry.object_y,
            );
        }
        synced
    }

    /// Common state for the mode-5/mode-6 constructors.  The target reaches
    /// CDspObj vtable +60 (sub_41B370) here, so X/Y/Z are signed 16.16.  They
    /// must not pass through the ordinary +44 integer-pixel position setter.
    fn graph90_set_fixed_common_state(
        &mut self,
        object: i32,
        x_16_16: i32,
        y_16_16: i32,
        z_16_16: i32,
        blend_mode: Option<i32>,
        alpha_parameter: Option<i32>,
        priority: Option<i32>,
    ) {
        self.graph90_set_common_state(object, None, blend_mode, alpha_parameter, priority);
        // sub_427170/sub_427220 call the object's vtable+60 setter rather
        // than writing +0x4C/+0x50/+0x54 directly. Route construction through
        // the same helper used by animation so +0x80/+0x84 rounding, +0x7C
        // mirroring, and native member propagation stay identical.
        self.graph91_set_object_fixed_position(object, x_16_16, y_16_16, z_16_16);
        self.graph90_refresh_native_sort_key(object);
    }

    /// The mode-5 projection inputs and result exactly as
    /// `graph90_sync_mode5_primary_layer` computes them (constructor args with
    /// the current fixed position and the 0x40/0x41 property overrides).
    fn graph90_mode5_projection(
        &self,
        object: i32,
    ) -> Option<(
        NativeMode5NodeArgs,
        NativeMode5DynamicState,
        crate::graph::NativeMode5Geometry,
    )> {
        let properties = self.graph_object_properties.get(&object)?;
        let mut args = self.graph90_recorded_source_args(object, 0x5C, 17)?;
        args[1] = properties.native.fixed_position_x_16_16;
        args[2] = properties.native.fixed_position_y_16_16;
        args[3] = properties.native.fixed_position_z_16_16;
        let mut raw = NativeMode5NodeArgs::from_source_args(&args)?;
        if let Some((x, y)) = properties.properties.get(&0x40).copied() {
            raw.anchor_x = x;
            raw.anchor_y = y;
        }
        if let Some((rotation, _)) = properties.properties.get(&0x41).copied() {
            raw.rotation = rotation;
        }
        let mode5 = self.graph90_resolve_projected_sprite_vector(raw);
        let (_, source) = self.resource_image_region(mode5.resource_id)?;
        let (screen_width, screen_height) = if self.screen_width > 0 && self.screen_height > 0 {
            (self.screen_width, self.screen_height)
        } else {
            (1280, 720)
        };
        let dynamic = self.graph90_mode5_dynamic_state(object);
        let geometry = mode5.screen_geometry_mode5_exact(
            source.width,
            source.height,
            screen_width as f32,
            screen_height as f32,
            self.graph_config.center,
            properties.native.use_graph_center != 0,
            dynamic,
        );
        Some((mode5, dynamic, geometry))
    }

    /// Sprite [148] / [151]: the base rotation that Graph91:38 parameter
    /// 0x41 reports when property 0x41 never replaced it.
    pub(crate) fn graph90_sprite_base_rotation(&self, object: i32) -> Option<i32> {
        let args = self.graph90_recorded_source_args(object, 0x5C, 17)?;
        NativeMode5NodeArgs::from_source_args(&args).map(|args| args.rotation)
    }

    /// Graph91:38 parameter 0x10000000 for a mode-5 sprite (sub_4296C0).
    pub(crate) fn graph90_sprite_transform_parameters(&self, object: i32) -> Option<Vec<i32>> {
        let (mode5, dynamic, _) = self.graph90_mode5_projection(object)?;
        Some(mode5.mode5_transform_parameters(dynamic).to_vec())
    }

    /// Sprite [181]/[182]: the projected raster size from sub_4291E0.
    pub(crate) fn graph90_sprite_projected_extent(&self, object: i32) -> Option<(i32, i32)> {
        let (_, _, geometry) = self.graph90_mode5_projection(object)?;
        Some((geometry.width as i32, geometry.height as i32))
    }

    fn graph90_recorded_source_args(
        &self,
        object: i32,
        selector: u16,
        argc: usize,
    ) -> Option<Vec<i32>> {
        let properties = self.graph_object_properties.get(&object)?;
        let mut args = Vec::with_capacity(argc);
        for index in 0..argc {
            let key = format!("target-90-{selector:02X}-arg-{index}");
            args.push(*properties.named_properties.get(&key)?);
        }
        Some(args)
    }

    /// Rebuilds mode-5/mode-6 renderer geometry after the native vtable +60
    /// X/Y/Z setter changes a transformed sprite.  CProcCtrlDspObjBC uses
    /// this path, and mode 5 must recompute sub_41AAA0/sub_429220 when Z
    /// changes rather than merely translating the already-projected layer.
    pub(super) fn graph90_resync_fixed_sprite_geometry(&mut self, object: i32) -> bool {
        let Some((mode, x, y, z)) =
            self.graph_object_properties
                .get(&object)
                .and_then(|properties| {
                    Some((
                        *properties.named_properties.get("target-object-mode")?,
                        properties.native.fixed_position_x_16_16,
                        properties.native.fixed_position_y_16_16,
                        properties.native.fixed_position_z_16_16,
                    ))
                })
        else {
            return false;
        };
        let (selector, argc) = match mode {
            5 => (0x5C_u16, 17_usize),
            6 => (0x5D_u16, 20_usize),
            _ => return false,
        };
        let Some(mut args) = self.graph90_recorded_source_args(object, selector, argc) else {
            return false;
        };
        args[1] = x;
        args[2] = y;
        args[3] = z;
        if mode == 5 {
            self.graph90_sync_mode5_primary_layer(&args)
        } else {
            self.graph90_sync_mode6_primary_layer(&args)
        }
    }

    /// The argument checks every Graph90 sprite-configure handler runs before
    /// its helper: priority < 0x10000 (sub_497BB0), alpha <= 0x100
    /// (sub_497DB0), a known blit mode (sub_497C40) and bitmap handles below
    /// 0x4000 (sub_497B60). Each failure is a script error.
    pub(super) fn graph90_validate_sprite_arguments(
        selector: u16,
        priority: i32,
        alpha: i32,
        blend_mode: i32,
        bitmaps: &[i32],
    ) -> std::result::Result<(), ethornell_vm::VmError> {
        let fail = |what: String| {
            Err(ethornell_vm::VmError::Runtime(format!(
                "Graph90:{selector:02X} {what}"
            )))
        };
        if priority as u32 >= 0x1_0000 {
            return fail(format!("priority {priority} is out of range"));
        }
        if alpha as u32 > 0x100 {
            return fail(format!("alpha {alpha} exceeds 256"));
        }
        if !matches!(blend_mode, 0..=9 | 0x20..=0x27 | 0x40 | 0x41 | 0x80 | 0xC0 | 0xC1 | 0xF0 | 0xFF)
        {
            return fail(format!("blend mode {blend_mode:#x} is invalid"));
        }
        for bitmap in bitmaps {
            if *bitmap as u32 >= 0x4000 {
                return fail(format!("bitmap handle {bitmap} is out of range"));
            }
        }
        Ok(())
    }

    pub(super) fn graph90_set_common_state(
        &mut self,
        object: i32,
        position: Option<(i32, i32)>,
        blend_mode: Option<i32>,
        alpha_parameter: Option<i32>,
        priority: Option<i32>,
    ) {
        // The configure helpers (e.g. sub_426F50) call vtable+44 SetPosition
        // (sub_41B1B0 -> sub_41B1D0, propagating to member children), then
        // sub_41B600 (blit mode +0xA8), SetAlpha and SetPriority.
        if let Some((x, y)) = position {
            self.graph90_set_position_recursive(object, x, y);
        }
        if let Some(blend_mode) = blend_mode {
            let properties = self.graph_object_properties.entry(object).or_default();
            properties.blend_mode = blend_mode;
            properties.native.blend_mode = blend_mode;
        }
        if let Some(alpha_parameter) = alpha_parameter {
            self.set_graph_object_alpha_recursive(object, alpha_parameter);
        }
        if let Some(priority) = priority.filter(|value| (0..0x1_0000).contains(value)) {
            self.graph_object_properties
                .entry(object)
                .or_default()
                .native
                .priority = priority as u32;
            self.display_tree.set_chain_depth(object, priority);
            self.graph90_refresh_native_sort_key(object);
        }
    }

    pub(super) fn dispatch_native_graph(
        &mut self,
        call: &mut ethornell_vm::NativeCallFrame,
    ) -> Option<ethornell_vm::VmResult<ethornell_vm::Value>> {
        let (group, id) = (call.group(), call.id());
        let stack = call.args_mut();
        let value = match (group, id) {
            (0x90, 0x0A) => {
                // Native pop order is redraw mode then enable gate; helper
                // sub_4319E0 stores enable in dword_50768C and redraw mode in
                // dword_565B78.
                let full_redraw = pop_int_value(stack).unwrap_or_default() != 0;
                let enabled = pop_int_value(stack).unwrap_or_default() != 0;
                self.graph_object_update_redraw_enabled = enabled;
                self.graph_object_update_redraw_full = full_redraw;
                self.trace_graph(format!(
                    "object-update redraw enabled={enabled} full={full_redraw} (90:0A)"
                ));
                ethornell_vm::Value::None
            }
            (0x90, 0x0B) => {
                // Target sub_401B70 -> sub_407BB0 writes the supplied bitmap
                // handle to the bitmap manager's current/primary slot.
                let bitmap = pop_int_value(stack).unwrap_or_default();
                self.primary_bitmap = Some(bitmap);
                self.trace_graph(format!("primary bitmap=#{bitmap} (90:0B)"));
                ethornell_vm::Value::None
            }
            (0x90, 0x0F) => {
                let color = pop_int_value(stack).unwrap_or_default();
                self.graph_bitmap_unblend_color = color;
                ethornell_vm::Value::None
            }
            (0x90, 0x1A) => {
                let args = pop_args(stack, 6);
                self.apply_native_bitmap_operation(&args, NativeBitmapOperation::Composite);
                ethornell_vm::Value::None
            }
            (0x90, 0x1B) => {
                let args = pop_args(stack, 4);
                self.apply_native_bitmap_operation(&args, NativeBitmapOperation::Copy);
                ethornell_vm::Value::None
            }
            (0x90, 0x1C) => {
                let args = pop_args(stack, 10);
                self.apply_native_bitmap_operation(&args, NativeBitmapOperation::Scale);
                ethornell_vm::Value::None
            }
            (0x90, 0x1D) => {
                let args = pop_args(stack, 4);
                self.apply_native_bitmap_operation(&args, NativeBitmapOperation::Copy);
                ethornell_vm::Value::None
            }
            (0x90, 0x21) => {
                // Target selector 0x21 dispatches to sub_47A890 ->
                // sub_491B40 -> sub_431D50.  The executable ABI descriptor
                // records nine source arguments: object, x, y, position
                // curve, alpha, duration, update_denominator, input_enabled,
                // input_descriptor. Native pop order is reversed.
                let args = pop_args(stack, 9);
                let ints = args.iter().map(value_to_i32).collect::<Vec<_>>();
                let [
                    input_descriptor,
                    input_enabled,
                    update_denominator,
                    duration_ms,
                    target_transparency,
                    position_curve,
                    target_y,
                    target_x,
                    object,
                ] = ints.as_slice()
                else {
                    return Some(Err(ethornell_vm::VmError::Runtime(
                        "Graph90:21 expected nine arguments".to_string(),
                    )));
                };
                let control_id = match self.schedule_native_cdspobj_control(
                    *object,
                    Some((*target_x, *target_y)),
                    *position_curve,
                    *target_transparency,
                    0,
                    None,
                    *duration_ms,
                    *update_denominator,
                    0,
                    *input_enabled != 0,
                    *input_descriptor,
                    "Graph90:21",
                ) {
                    Ok(control_id) => control_id,
                    Err(error) => return Some(Err(error)),
                };
                call.start_graph_control_procedure(*object, control_id);
                ethornell_vm::Value::None
            }
            (0x90, 0x24) => {
                // Target sub_47AAF0 pops source arguments in reverse order.
                // sub_491C40/sub_432970 then build a sampled three-point
                // path from the object's current position, the script middle
                // point and the script final point.
                let args = pop_args(stack, 12);
                let ints = args.iter().map(value_to_i32).collect::<Vec<_>>();
                let [
                    input_descriptor,
                    input_enabled,
                    max_catchup_steps,
                    sample_rate_hz,
                    duration_ms,
                    target_transparency,
                    position_curve,
                    target_y,
                    target_x,
                    middle_y,
                    middle_x,
                    object,
                ] = ints.as_slice()
                else {
                    return Some(Err(ethornell_vm::VmError::Runtime(
                        "Graph90:24 expected twelve arguments".to_string(),
                    )));
                };
                let control_id = match self.schedule_native_cdspobj_special_path(
                    *object,
                    (*middle_x, *middle_y),
                    (*target_x, *target_y),
                    *position_curve,
                    *target_transparency,
                    *duration_ms,
                    *sample_rate_hz,
                    *max_catchup_steps,
                    *input_enabled != 0,
                    *input_descriptor,
                    "Graph90:24",
                ) {
                    Ok(control_id) => control_id,
                    Err(error) => return Some(Err(error)),
                };
                call.start_graph_control_procedure(*object, control_id);
                ethornell_vm::Value::None
            }
            (0x90, 0x2C) => {
                // Target path: sub_47AF80 -> sub_491F90 -> CProcShakeDspObj.
                // This selector is an object shake control, not the spline
                // procedure previously assigned to it in the portable map.
                let args = pop_args(stack, 9);
                let ints = args.iter().map(value_to_i32).collect::<Vec<_>>();
                let [
                    input_descriptor,
                    input_enabled,
                    sample_rate_hz,
                    decay_percent,
                    cycles,
                    frequency_hz,
                    amplitude,
                    mode,
                    object,
                ] = ints.as_slice()
                else {
                    return Some(Err(ethornell_vm::VmError::Runtime(
                        "Graph90:2C expected nine arguments".to_string(),
                    )));
                };
                let control_id = match self.schedule_native_cdspobj_shake(
                    *object,
                    *mode,
                    *amplitude,
                    *frequency_hz,
                    *cycles,
                    *decay_percent,
                    *sample_rate_hz,
                    *input_enabled != 0,
                    *input_descriptor,
                    "Graph90:2C",
                ) {
                    Ok(control_id) => control_id,
                    Err(error) => return Some(Err(error)),
                };
                call.start_graph_control_procedure(*object, control_id);
                ethornell_vm::Value::None
            }
            (0x90, 0x33) => {
                let y = pop_int_value(stack).unwrap_or_default();
                let x = pop_int_value(stack).unwrap_or_default();
                let object = pop_int_value(stack).unwrap_or_default();
                if let Err(error) = self.graph90_require_registered_object(id, object) {
                    return Some(Err(error));
                }
                let kind = self.display_tree.kind(object);
                let parent = self.display_tree.parent(object);
                self.trace_graph(format!(
                    "target graph 90:33 object=#{object} kind={kind:?} parent={parent:?} position=({x},{y})"
                ));
                self.graph90_set_position_recursive(object, x, y);
                tracing::debug!(object, x, y, "GraphSetObjectPosition");
                ethornell_vm::Value::None
            }
            (0x90, 0x34) => {
                let mask_alpha = pop_int_value(stack).unwrap_or_default();
                let object = pop_int_value(stack).unwrap_or_default();
                if let Err(error) = Self::graph90_require_transparency_parameter(id, mask_alpha)
                    .and_then(|()| self.graph90_require_registered_object(id, object))
                {
                    return Some(Err(error));
                }
                self.graph90_set_mask_alpha_recursive(object, mask_alpha);
                tracing::debug!(object, mask_alpha, "GraphSetObjectMaskAlpha");
                ethornell_vm::Value::None
            }
            (0x90, 0x35) => {
                let value = pop_int_value(stack).unwrap_or_default();
                let object = pop_int_value(stack).unwrap_or_default();
                if let Err(error) = Self::graph90_require_transparency_parameter(id, value)
                    .and_then(|()| self.graph90_require_registered_object(id, object))
                {
                    return Some(Err(error));
                }
                self.graph90_set_fixed_parameter_recursive(object, value);
                tracing::debug!(
                    object,
                    value,
                    fixed = value << 16,
                    "GraphSetObjectFixedParameter"
                );
                ethornell_vm::Value::None
            }
            (0x90, 0x36 | 0x37) => {
                let y = pop_int_value(stack).unwrap_or_default();
                let x = pop_int_value(stack).unwrap_or_default();
                let object = pop_int_value(stack).unwrap_or_default();
                if let Err(error) = self.graph90_require_registered_object(id, object) {
                    return Some(Err(error));
                }
                self.graph90_set_offset_recursive(object, id, x, y);
                let native = self
                    .graph_object_properties
                    .get(&object)
                    .map(|properties| properties.native)
                    .unwrap_or_default();
                let world_state = self.graph_layers.get(&object).map(|layer| {
                    let (world_x, world_y, world_z) = self.layer_world_transform(object, layer);
                    (world_x, world_y, world_z)
                });
                tracing::info!(
                    object,
                    x,
                    y,
                    selector = id,
                    primary_offset = ?(native.primary_offset_x, native.primary_offset_y),
                    secondary_offset = ?(native.secondary_offset_x, native.secondary_offset_y),
                    ?world_state,
                    "GraphSetObjectOffset"
                );
                ethornell_vm::Value::None
            }
            (0x90, 0x39) => {
                let multiplier = pop_int_value(stack).unwrap_or_default();
                let object = pop_int_value(stack).unwrap_or_default();
                if let Err(error) = Self::graph90_require_transparency_parameter(id, multiplier)
                    .and_then(|()| self.graph90_require_registered_object(id, object))
                {
                    return Some(Err(error));
                }
                self.graph90_set_alpha_multiplier_recursive(object, multiplier);
                tracing::debug!(object, multiplier, "GraphSetObjectAlphaMultiplier");
                ethornell_vm::Value::None
            }
            (0x90, 0x3A) => {
                let priority = pop_int_value(stack).unwrap_or_default();
                let object = pop_int_value(stack).unwrap_or_default();
                if let Err(error) = Self::graph90_require_priority(id, priority)
                    .and_then(|()| self.graph90_require_registered_object(id, object))
                {
                    return Some(Err(error));
                }
                self.graph_object_properties
                    .entry(object)
                    .or_default()
                    .native
                    .priority = priority as u32;
                self.display_tree.set_chain_depth(object, priority);
                self.graph90_refresh_native_sort_key(object);
                for layer_id in self.graph_target_layers(object) {
                    if let Some(layer) = self.graph_layers.get_mut(&layer_id) {
                        layer.z = priority;
                    }
                }
                tracing::debug!(object, priority, "GraphSetObjectPriority");
                ethornell_vm::Value::None
            }
            (0x90, 0x3F) => {
                let object = pop_int_value(stack).unwrap_or_default();
                // Every CDspObj-derived vtable in this target points slot +108
                // at sub_41BE90, which returns 0x80000001. The public handler
                // maps that to its target script error; a missing object uses
                // the separate 255 path. Preserve the fact that this selector
                // is not a handle-existence query or a successful no-op.
                let reason = if self.graph_handle_exists(object) {
                    "target object extension operation is unsupported"
                } else {
                    "target graph object does not exist"
                };
                return Some(Err(ethornell_vm::VmError::Runtime(format!(
                    "Graph90:3F object #{object}: {reason}"
                ))));
            }
            (0x90, 0x40) => {
                let bitmap = pop_int_value(stack).unwrap_or_default();
                let object = self.graph90_prepare_current_background(NativeBackgroundClass::BackN);
                let properties = self.graph_object_properties.entry(object).or_default();
                properties.format_resource = Some(bitmap);
                properties
                    .named_properties
                    .insert("target-primary-bitmap".to_string(), bitmap);
                if !self.graph90_sync_backn_layer(object, bitmap) {
                    return Some(Err(ethornell_vm::VmError::Runtime(format!(
                        "Graph90:40 bitmap #{bitmap} is not a valid registered bitmap"
                    ))));
                }
                self.graph90_record_call(object, id, &[bitmap]);
                ethornell_vm::Value::None
            }
            (0x90, 0x41) => {
                let args = Self::graph90_source_args(stack, 3);
                let object = self.graph90_prepare_current_background(NativeBackgroundClass::BackB);
                if let [primary, secondary, alpha] = args.as_slice() {
                    self.graph90_set_common_state(object, None, None, Some(*alpha), None);
                    if !self.graph90_sync_backb_layers(object, *primary, *secondary) {
                        return Some(Err(ethornell_vm::VmError::Runtime(format!(
                            "Graph90:41 resources primary=#{primary} secondary=#{secondary} are not a valid BackB pair"
                        ))));
                    }
                }
                self.graph90_record_call(object, id, &args);
                ethornell_vm::Value::None
            }
            (0x90, 0x42) => {
                let args = Self::graph90_source_args(stack, 6);
                let object = self.graph90_prepare_current_background(NativeBackgroundClass::BackS);
                if let [bitmap_0, bitmap_1, bitmap_2, bitmap_3, x, y] = args.as_slice() {
                    let position_valid = self
                        .graph_object_properties
                        .get_mut(&object)
                        .and_then(|properties| properties.background.as_mut())
                        .is_some_and(|background| {
                            background.set_position(*x, *y, self.screen_width, self.screen_height)
                        });
                    if !position_valid {
                        return Some(Err(ethornell_vm::VmError::Runtime(format!(
                            "Graph90:42 position ({x}, {y}) is outside the target screen"
                        ))));
                    }
                    if !self.graph90_sync_backs_layers(
                        object,
                        [*bitmap_0, *bitmap_1, *bitmap_2, *bitmap_3],
                        *x,
                        *y,
                    ) {
                        // sub_43D6F0 writes the seam through sub_41F540
                        // before sub_41F4A0 validates the four resources.
                        // A failed rebind therefore keeps the old handles but
                        // draws them with the newly accepted seam.
                        self.graph90_layout_backs_layers(object, *x, *y);
                        return Some(Err(ethornell_vm::VmError::Runtime(format!(
                            "Graph90:42 resources [{bitmap_0}, {bitmap_1}, {bitmap_2}, {bitmap_3}] do not match the target screen bitmap"
                        ))));
                    }
                }
                self.graph90_record_call(object, id, &args);
                ethornell_vm::Value::None
            }
            (0x90, 0x43) => {
                let args = Self::graph90_source_args(stack, 9);
                // sub_47B7C0 checks the alpha (sub_497DB0) before anything else.
                if let Some(&alpha) = args.get(8)
                    && alpha as u32 > 0x100
                {
                    return Some(Err(ethornell_vm::VmError::Runtime(format!(
                        "Graph90:43 alpha {alpha} exceeds 256"
                    ))));
                }
                let object = self.graph90_prepare_current_background(NativeBackgroundClass::BackF);
                if let [
                    primary_x,
                    primary_y,
                    primary,
                    secondary_x,
                    secondary_y,
                    secondary,
                    mask,
                    mask_parameter,
                    alpha_parameter,
                ] = args.as_slice()
                {
                    if let Err(reason) = self.graph90_sync_backf_layers(
                        object,
                        *primary_x,
                        *primary_y,
                        *primary,
                        *secondary_x,
                        *secondary_y,
                        *secondary,
                        *mask,
                        *mask_parameter,
                    ) {
                        return Some(Err(ethornell_vm::VmError::Runtime(format!(
                            "Graph90:43 CDspObjBackF configuration rejected: {reason}"
                        ))));
                    }
                    // sub_43D750 applies vtable+0x48 only after both resource
                    // configuration stages have succeeded (the range was
                    // checked by sub_497DB0 above).
                    self.set_graph_object_alpha_recursive_raw(object, *alpha_parameter);
                    if let Err(reason) = self.graph90_refresh_backf_primary(object) {
                        return Some(Err(ethornell_vm::VmError::Runtime(format!(
                            "Graph90:43 CDspObjBackF draw state rejected: {reason}"
                        ))));
                    }
                    let properties = self.graph_object_properties.entry(object).or_default();
                    properties
                        .named_properties
                        .insert("target-secondary-resource".to_string(), *secondary);
                    properties
                        .named_properties
                        .insert("target-mask-resource".to_string(), *mask);
                    properties
                        .named_properties
                        .insert("target-mask-parameter".to_string(), *mask_parameter);
                    self.trace_graph(format!(
                        "target graph 90:43 CDspObjBackF object=#{object} primary=#{} secondary=#{} mask=#{} mask_parameter={} alpha={}",
                        primary, secondary, mask, mask_parameter, alpha_parameter
                    ));
                }
                self.graph90_record_call(object, id, &args);
                ethornell_vm::Value::None
            }
            (0x90, 0x44) => {
                let args = Self::graph90_source_args(stack, 3);
                let object = self.graph90_prepare_current_background(NativeBackgroundClass::BackD);
                if args.len() == 3 {
                    self.graph90_set_common_state(object, None, None, Some(args[2]), None);
                    let properties = self.graph_object_properties.entry(object).or_default();
                    properties
                        .named_properties
                        .insert("target-frame-count".to_string(), args[0]);
                    properties
                        .named_properties
                        .insert("target-frame-table-pointer".to_string(), args[1]);
                }
                self.graph90_record_call(object, id, &args);
                ethornell_vm::Value::None
            }
            (0x90, 0x45) => {
                let args = Self::graph90_source_args(stack, 5);
                let object =
                    self.graph90_prepare_current_background(NativeBackgroundClass::BackDst);
                if args.len() == 5 {
                    self.graph90_set_common_state(object, None, None, Some(args[3]), None);
                    let properties = self.graph_object_properties.entry(object).or_default();
                    properties.format_resource = Some(args[0]);
                    properties
                        .named_properties
                        .insert("target-resource-1".to_string(), args[1]);
                    properties
                        .named_properties
                        .insert("target-resource-2".to_string(), args[2]);
                    properties
                        .named_properties
                        .insert("target-extension-0x154".to_string(), args[4]);
                }
                self.graph90_record_call(object, id, &args);
                ethornell_vm::Value::None
            }
            (0x90, 0x46) => {
                let args = Self::graph90_source_args(stack, 3);
                let object =
                    self.graph90_prepare_current_background(NativeBackgroundClass::BackGrd);
                if args.len() == 3 {
                    self.graph90_set_common_state(object, None, None, Some(args[2]), None);
                    self.graph90_set_mode(object, args[1]);
                    self.graph_object_properties
                        .entry(object)
                        .or_default()
                        .format_resource = Some(args[0]);
                }
                self.graph90_record_call(object, id, &args);
                ethornell_vm::Value::None
            }
            (0x90, 0x47) => {
                let args = Self::graph90_source_args(stack, 5);
                let object =
                    self.graph90_prepare_current_background(NativeBackgroundClass::BackRpl);
                if args.len() == 5 {
                    self.graph90_set_common_state(object, None, None, Some(args[4]), None);
                    let properties = self.graph_object_properties.entry(object).or_default();
                    properties.format_resource = Some(args[0]);
                    properties
                        .named_properties
                        .insert("target-auxiliary-resource".to_string(), args[1]);
                    properties
                        .named_properties
                        .insert("target-record-count".to_string(), args[2]);
                    properties
                        .named_properties
                        .insert("target-record-pointer".to_string(), args[3]);
                }
                self.graph90_record_call(object, id, &args);
                ethornell_vm::Value::None
            }
            (0x90, 0x48) => {
                let args = Self::graph90_source_args(stack, 5);
                let object =
                    self.graph90_prepare_current_background(NativeBackgroundClass::BackStr);
                if args.len() == 5 {
                    self.graph90_set_common_state(
                        object,
                        Some((args[1], args[2])),
                        None,
                        None,
                        None,
                    );
                    let properties = self.graph_object_properties.entry(object).or_default();
                    properties.format_resource = Some(args[0]);
                    properties
                        .named_properties
                        .insert("target-width".to_string(), args[3]);
                    properties
                        .named_properties
                        .insert("target-height".to_string(), args[4]);
                }
                self.graph90_record_call(object, id, &args);
                ethornell_vm::Value::None
            }
            (0x90, 0x49) => {
                let args = Self::graph90_source_args(stack, 3);
                let object =
                    self.graph90_prepare_current_background(NativeBackgroundClass::BackRtt);
                if args.len() == 3 {
                    let properties = self.graph_object_properties.entry(object).or_default();
                    properties.format_resource = Some(args[0]);
                    properties
                        .named_properties
                        .insert("target-width".to_string(), args[1]);
                    properties
                        .named_properties
                        .insert("target-height".to_string(), args[2]);
                }
                self.graph90_record_call(object, id, &args);
                ethornell_vm::Value::None
            }
            (0x90, 0x4A) => {
                let args = Self::graph90_source_args(stack, 5);
                let object =
                    self.graph90_prepare_current_background(NativeBackgroundClass::BackMsc);
                if args.len() == 5 {
                    self.graph90_set_common_state(object, None, None, Some(args[3]), None);
                    let properties = self.graph_object_properties.entry(object).or_default();
                    properties.format_resource = Some(args[0]);
                    properties
                        .named_properties
                        .insert("target-secondary-resource".to_string(), args[1]);
                    properties
                        .named_properties
                        .insert("target-source-mode".to_string(), args[2]);
                    properties
                        .named_properties
                        .insert("target-source-flag".to_string(), args[4]);
                }
                self.graph90_record_call(object, id, &args);
                ethornell_vm::Value::None
            }
            (0x90, 0x4C) => {
                let args = Self::graph90_source_args(stack, 2);
                let object = self.graph90_current_object();
                if args.len() == 2 {
                    // sub_43E490: the manager keeps (draw, active) for later
                    // background classes; the current one gets vtable+4
                    // (draw gate with member propagation) and vtable+120
                    // (CDspObjBack+0x138).
                    self.graph_config.renderer_options = (args[0], args[1]);
                    self.set_graph_object_draw_enabled(object, args[0] != 0);
                    if let Some(background) = self
                        .graph_object_properties
                        .entry(object)
                        .or_default()
                        .background
                        .as_mut()
                    {
                        background.active = args[1];
                    }
                    self.graph_redraw_requested = Some(true);
                }
                self.graph90_record_call(object, id, &args);
                ethornell_vm::Value::None
            }
            (0x90, 0x4D) => {
                let object = self.graph90_current_object();
                let mode = self
                    .graph_object_properties
                    .get(&object)
                    .and_then(|properties| properties.named_properties.get("target-object-mode"))
                    .copied()
                    .unwrap_or_default();
                ethornell_vm::Value::Int(mode)
            }
            (0x90, 0x50) => {
                let handle = self.graph90_allocate_object(
                    GRAPH90_SPRITE_TAG,
                    512,
                    GRAPH90_CLASS_SPRITE,
                    NativeDisplayKind::Sprite,
                    // CDspObjSprite::CDspObjSprite (sub_4256C0) initializes
                    // Sprite+0x134 / a2[77] to mode 0. A fresh Sprite must
                    // therefore accept Graph90:57 primary replacement through
                    // the mode-0 path even before a full 90:56 configure.
                    0,
                );
                tracing::info!(sprite = handle, "GraphCreateSpriteObject");
                ethornell_vm::Value::Int(handle)
            }
            (0x90, 0x51) => {
                // sub_47C110: drop the object's input registration
                // (sub_496300), then refuse a sprite still locked by an input
                // processor (+0x130, sub_41AD50) or attached as a child
                // (+0x11C parent, sub_41ACE0), and finally release the slot
                // (sub_43E610); every refusal is a script error.
                let handle = pop_int_value(stack).unwrap_or_default();
                if self.graph_sprite_targets.unregister_sprite(handle) {
                    self.unregister_pointer_object_node(handle);
                }
                if self
                    .graph_input_objects
                    .values()
                    .any(|processor| processor.is_live() && processor.layer == handle)
                {
                    return Some(Err(ethornell_vm::VmError::Runtime(format!(
                        "Graph90:51 sprite #{handle} is still used by an input processor"
                    ))));
                }
                if self.graph_native_owners.contains_key(&handle) {
                    return Some(Err(ethornell_vm::VmError::Runtime(format!(
                        "Graph90:51 sprite #{handle} is still attached to a parent object"
                    ))));
                }
                let released = self.graph90_release_object(
                    handle,
                    GRAPH90_SPRITE_TAG,
                    512,
                    GRAPH90_CLASS_SPRITE,
                );
                if !released {
                    return Some(Err(ethornell_vm::VmError::Runtime(format!(
                        "Graph90:51 #{handle} is not a sprite object"
                    ))));
                }
                tracing::debug!(handle, "GraphReleaseSpriteObject");
                ethornell_vm::Value::None
            }
            (0x90, 0x53) => {
                // sub_47C170 -> sub_43EEE0 -> sub_428C00. Script order
                // (sprite, x, y, width, height); a missing sprite is fatal.
                // Modes 0/1/3 invalidate the rectangle on screen and reject
                // a non-positive width/height (status 10, fatal); mode 5
                // re-rasterizes its projected cache there; other modes
                // rebuild (mode 6, sub_42A650) and invalidate the sprite.
                let args = Self::graph90_source_args(stack, 5);
                let [handle, _x, _y, width, height] = args[..] else {
                    return Some(Err(ethornell_vm::VmError::Runtime(
                        "Graph90:53 expected five arguments".into(),
                    )));
                };
                if !self.graph90_object_matches(handle, GRAPH90_SPRITE_TAG, 512, GRAPH90_CLASS_SPRITE)
                {
                    return Some(Err(ethornell_vm::VmError::Runtime(format!(
                        "Graph90:53 #{handle} is not a sprite object"
                    ))));
                }
                let mode = self
                    .graph_object_properties
                    .get(&handle)
                    .and_then(|properties| properties.named_properties.get("target-object-mode"))
                    .copied()
                    .unwrap_or_default();
                if matches!(mode, 0 | 1 | 3) && (width <= 0 || height <= 0) {
                    return Some(Err(ethornell_vm::VmError::Runtime(format!(
                        "Graph90:53 update region {width}x{height} is invalid"
                    ))));
                }
                if matches!(mode, 5 | 6) {
                    let _ = self.graph90_resync_fixed_sprite_geometry(handle);
                }
                self.graph_redraw_requested = Some(false);
                self.graph90_record_call(handle, id, &args);
                ethornell_vm::Value::None
            }
            (0x90, 0x54) => {
                let args = Self::graph90_source_args(stack, 2);
                if let [handle, enabled] = args.as_slice()
                    && self.graph90_object_matches(
                        *handle,
                        GRAPH90_SPRITE_TAG,
                        512,
                        GRAPH90_CLASS_SPRITE,
                    )
                {
                    // Tayutama2_trial_TG.exe: 0x47C1F0 -> sub_462540
                    // -> sub_43EE70. The latter dispatches Sprite
                    // vtable+0x04, i.e. CDspObj::SetDrawEnabled
                    // (sub_41AE00, field +0x14). This is the same virtual
                    // used by generic Graph90:30 and by CDspObjKnob's
                    // Graph90:D4 forwarding path. It is NOT the separate
                    // CDspObj enabled field changed by Graph90:31.
                    self.set_graph_object_draw_enabled(*handle, *enabled != 0);
                    self.graph90_record_call(*handle, id, &args);
                }
                ethornell_vm::Value::None
            }
            (0x90, 0x55) => {
                let args = Self::graph90_source_args(stack, 2);
                if let [handle, bitmap] = args.as_slice()
                    && self.graph90_object_matches(
                        *handle,
                        GRAPH90_SPRITE_TAG,
                        512,
                        GRAPH90_CLASS_SPRITE,
                    )
                {
                    let properties = self.graph_object_properties.entry(*handle).or_default();
                    // Target sub_43ED20 -> sub_427F80 stores an independent
                    // auxiliary bitmap at Sprite+0x13C and generation at
                    // +0x140. It never overwrites the mode's primary bitmap
                    // at Sprite+0x150.
                    properties.aux_resource = (*bitmap != -1).then_some(*bitmap);
                    tracing::info!(
                        sprite = *handle,
                        bitmap = *bitmap,
                        primary = ?properties.format_resource,
                        aux = ?properties.aux_resource,
                        "GraphSetSpriteAuxBitmap"
                    );
                    self.graph90_record_call(*handle, id, &args);
                }
                ethornell_vm::Value::None
            }
            (0x90, 0x56) => {
                // sub_47C2D0 pops priority, alpha, blend mode, bitmap, y, x,
                // sprite; sub_43E690 refuses a missing sprite or bitmap.
                let args = Self::graph90_source_args(stack, 7);
                if args.len() != 7 {
                    return Some(Err(ethornell_vm::VmError::Runtime(
                        "Graph90:56 expected seven arguments".into(),
                    )));
                }
                if let Err(error) = Self::graph90_validate_sprite_arguments(
                    id,
                    args[6],
                    args[5],
                    args[4],
                    &[args[3]],
                ) {
                    return Some(Err(error));
                }
                if !self.graph90_object_matches(
                    args[0],
                    GRAPH90_SPRITE_TAG,
                    512,
                    GRAPH90_CLASS_SPRITE,
                ) {
                    return Some(Err(ethornell_vm::VmError::Runtime(format!(
                        "Graph90:56 #{} is not a sprite object",
                        args[0]
                    ))));
                }
                if self.resource_image_region(args[3]).is_none() {
                    return Some(Err(ethornell_vm::VmError::Runtime(format!(
                        "Graph90:56 bitmap {} does not exist",
                        args[3]
                    ))));
                }
                {
                    self.graph90_begin_sprite_configuration(args[0], 0);
                    self.graph90_set_common_state(
                        args[0],
                        Some((args[1], args[2])),
                        Some(args[4]),
                        Some(args[5]),
                        Some(args[6]),
                    );
                    self.graph_object_properties
                        .entry(args[0])
                        .or_default()
                        .format_resource = Some(args[3]);
                    let configured = self.graph90_sync_sprite_primary_layer(
                        args[0],
                        args[3],
                        Some((args[1] as f32, args[2] as f32)),
                    );
                    let layer = self.graph_layers.get(&args[0]);
                    let effective_opacity = self
                        .graph_object_properties
                        .get(&args[0])
                        .map(RuntimeGraphObjectProperties::opacity);
                    tracing::info!(
                        sprite = args[0],
                        x = args[1],
                        y = args[2],
                        primary = args[3],
                        blend_mode = args[4],
                        alpha_parameter = args[5],
                        ?effective_opacity,
                        priority = args[6],
                        configured,
                        layer_x = ?layer.map(|layer| layer.x),
                        layer_y = ?layer.map(|layer| layer.y),
                        layer_width = ?layer.map(|layer| layer.width),
                        layer_height = ?layer.map(|layer| layer.height),
                        draw_eligible = layer.is_some_and(|layer| self.should_draw_graph_layer(args[0], layer)),
                        parent = ?self.display_tree.parent(args[0]),
                        "GraphConfigureSpriteMode0"
                    );
                    self.graph90_record_call(args[0], id, &args);
                }
                ethornell_vm::Value::None
            }
            (0x90, 0x57) => {
                let args = Self::graph90_source_args(stack, 2);
                if let [object, primary_bitmap] = args.as_slice() {
                    // Current Tayutama2_trial_TG.exe:
                    //   sub_47C3E0 -> sub_462510 -> sub_43ECA0 -> sub_4272F0
                    // resolves an existing CDspObjSprite and replaces its
                    // primary bitmap. sub_4272F0 switches on Sprite+0x134 and
                    // rebuilds modes 0/2/5/6 with the *existing* mode-specific
                    // geometry. It is not an offscreen-render operation.
                    //
                    // Mode 5/6 take the native replacement path through
                    // sub_427AA0/sub_427D90 with secondary=-1, transition=0
                    // and secondary_parameter=-1 while retaining the current
                    // fixed-point position and projection/quad parameters.
                    let sprite_valid = self.graph90_object_matches(
                        *object,
                        GRAPH90_SPRITE_TAG,
                        512,
                        GRAPH90_CLASS_SPRITE,
                    );
                    let bitmap_valid = self.query_bitmap_info(*primary_bitmap).is_some();
                    let mode = self
                        .graph_object_properties
                        .get(object)
                        .and_then(|properties| {
                            properties.named_properties.get("target-object-mode")
                        })
                        .copied()
                        .unwrap_or_default();
                    let old_primary = self
                        .graph_object_properties
                        .get(object)
                        .and_then(|properties| properties.format_resource)
                        .unwrap_or(-1);

                    // sub_497B60 rejects handles >= 0x4000; sub_43ECA0
                    // returns 255 for a missing sprite and 1 when the mode
                    // 0/2/5/6 rebuild cannot find the bitmap (sub_407F20).
                    // Modes 1/3/4 ignore the bitmap and succeed.
                    if *primary_bitmap as u32 >= 0x4000 {
                        return Some(Err(ethornell_vm::VmError::Runtime(format!(
                            "Graph90:57 bitmap handle {primary_bitmap} is out of range"
                        ))));
                    }
                    if !sprite_valid {
                        return Some(Err(ethornell_vm::VmError::Runtime(format!(
                            "Graph90:57 #{object} is not a sprite object"
                        ))));
                    }
                    if matches!(mode, 0 | 2 | 5 | 6) && !bitmap_valid {
                        return Some(Err(ethornell_vm::VmError::Runtime(format!(
                            "Graph90:57 bitmap {primary_bitmap} does not exist"
                        ))));
                    }
                    let replaced = if !bitmap_valid {
                        false
                    } else {
                        match mode {
                            0 => {
                                let position = self
                                    .graph_native_base_position(*object)
                                    .map(|(x, y)| (x as f32, y as f32))
                                    .or_else(|| {
                                        self.graph_layers
                                            .get(object)
                                            .map(|layer| (layer.x, layer.y))
                                    });
                                self.graph_object_properties
                                    .entry(*object)
                                    .or_default()
                                    .format_resource = Some(*primary_bitmap);
                                self.graph_transition_nodes.remove(object);
                                self.graph90_sync_sprite_primary_layer(
                                    *object,
                                    *primary_bitmap,
                                    position,
                                )
                            }
                            2 => self
                                .graph90_recorded_source_args(*object, 0x59, 13)
                                .is_some_and(|mut mode_args| {
                                    if let Some((x, y)) = self.graph_native_base_position(*object) {
                                        mode_args[1] = x;
                                        mode_args[2] = y;
                                    }
                                    mode_args[3] = *primary_bitmap;
                                    self.graph_object_properties
                                        .entry(*object)
                                        .or_default()
                                        .format_resource = Some(*primary_bitmap);
                                    self.graph_transition_nodes.remove(object);
                                    self.graph90_sync_mode2_primary_layer(&mode_args)
                                }),
                            5 => self
                                .graph90_recorded_source_args(*object, 0x5C, 17)
                                .is_some_and(|mut mode_args| {
                                    let base = self
                                        .graph_object_properties
                                        .get(object)
                                        .map(|properties| {
                                            [
                                                properties.native.fixed_position_x_16_16,
                                                properties.native.fixed_position_y_16_16,
                                                properties.native.fixed_position_z_16_16,
                                            ]
                                        })
                                        .unwrap_or([0; 3]);
                                    mode_args[1] = base[0];
                                    mode_args[2] = base[1];
                                    mode_args[3] = base[2];
                                    mode_args[4] = *primary_bitmap;
                                    mode_args[5] = -1;
                                    mode_args[6] = 0;
                                    mode_args[7] = -1;
                                    let properties =
                                        self.graph_object_properties.entry(*object).or_default();
                                    properties.format_resource = Some(*primary_bitmap);
                                    properties
                                        .named_properties
                                        .insert("mode5-secondary-bitmap".to_string(), -1);
                                    properties
                                        .named_properties
                                        .insert("mode5-transition-value".to_string(), 0);
                                    properties
                                        .named_properties
                                        .insert("mode5-transition-current".to_string(), 0);
                                    properties
                                        .named_properties
                                        .insert("mode5-secondary-parameter".to_string(), -1);
                                    self.graph_transition_nodes.remove(object);
                                    self.graph90_sync_mode5_primary_layer(&mode_args)
                                }),
                            6 => self
                                .graph90_recorded_source_args(*object, 0x5D, 20)
                                .is_some_and(|mut mode_args| {
                                    let base = self
                                        .graph_object_properties
                                        .get(object)
                                        .map(|properties| {
                                            [
                                                properties.native.fixed_position_x_16_16,
                                                properties.native.fixed_position_y_16_16,
                                                properties.native.fixed_position_z_16_16,
                                            ]
                                        })
                                        .unwrap_or([0; 3]);
                                    mode_args[1] = base[0];
                                    mode_args[2] = base[1];
                                    mode_args[3] = base[2];
                                    mode_args[4] = *primary_bitmap;
                                    mode_args[5] = -1;
                                    mode_args[6] = 0;
                                    mode_args[7] = -1;
                                    let properties =
                                        self.graph_object_properties.entry(*object).or_default();
                                    properties.format_resource = Some(*primary_bitmap);
                                    properties
                                        .named_properties
                                        .insert("mode6-secondary-bitmap".to_string(), -1);
                                    properties
                                        .named_properties
                                        .insert("mode6-transition-value".to_string(), 0);
                                    properties
                                        .named_properties
                                        .insert("mode6-secondary-parameter".to_string(), -1);
                                    self.graph_transition_nodes.remove(object);
                                    self.graph90_sync_mode6_primary_layer(&mode_args)
                                }),
                            _ => false,
                        }
                    };
                    let layer_state = self.graph_layers.get(object).map(|layer| {
                        (
                            layer.x,
                            layer.y,
                            layer.width,
                            layer.height,
                            layer.opacity,
                            layer.z,
                            layer.enabled,
                        )
                    });
                    let chain_drawable = self.display_tree.chain_drawable(*object);
                    let draw_eligible = self
                        .graph_layers
                        .get(object)
                        .is_some_and(|layer| self.should_draw_graph_layer(*object, layer));
                    let bitmap_format = self.bitmap_formats.get(primary_bitmap).copied();
                    let bitmap_alpha = self.graph_bitmap_image(*primary_bitmap).map(|image| {
                        let nonzero = image
                            .rgba
                            .as_chunks::<4>()
                            .0
                            .iter()
                            .filter(|pixel| pixel[3] != 0)
                            .count();
                        let max = image
                            .rgba
                            .as_chunks::<4>()
                            .0
                            .iter()
                            .map(|pixel| pixel[3])
                            .max()
                            .unwrap_or_default();
                        (nonzero, max)
                    });
                    let effective_opacity = self
                        .graph_object_properties
                        .get(object)
                        .map(RuntimeGraphObjectProperties::opacity);
                    let ignore_source_alpha = self
                        .graph_layers
                        .get(object)
                        .is_some_and(|layer| self.graph_layer_ignores_source_alpha(*object, layer));
                    tracing::info!(
                        object = *object,
                        mode,
                        old_primary,
                        primary_bitmap = *primary_bitmap,
                        sprite_valid,
                        bitmap_valid,
                        replaced,
                        ?layer_state,
                        native_position = ?self.graph_native_base_position(*object),
                        owner = ?self.graph_native_owners.get(object),
                        parent = ?self.display_tree.parent(*object),
                        chain_drawable,
                        draw_eligible,
                        ?bitmap_format,
                        ?bitmap_alpha,
                        ?effective_opacity,
                        ignore_source_alpha,
                        "GraphReplaceSpritePrimaryBitmap"
                    );
                    if replaced {
                        self.graph_redraw_requested = Some(false);
                    }
                    self.graph90_record_call(*object, id, &args);
                }
                ethornell_vm::Value::None
            }
            (0x90, 0x58) => {
                let args = Self::graph90_source_args(stack, 9);
                if args.len() == 9
                    && self.graph90_object_matches(
                        args[0],
                        GRAPH90_SPRITE_TAG,
                        512,
                        GRAPH90_CLASS_SPRITE,
                    )
                {
                    self.graph90_begin_sprite_configuration(args[0], 1);
                    self.graph90_set_common_state(
                        args[0],
                        Some((args[1], args[2])),
                        Some(1),
                        Some(args[6]),
                        Some(args[7]),
                    );
                    let configured = self.configure_transition_node(
                        args[0], args[1], args[2], args[3], args[4], args[5], args[6], args[7],
                        args[8],
                    );
                    self.graph90_record_call(args[0], id, &args);
                    let layer = self.graph_layers.get(&args[0]);
                    let layer_key = layer.map(|layer| layer.key.as_str());
                    let layer_enabled = layer.map(|layer| layer.enabled).unwrap_or(false);
                    let object_enabled = self
                        .graph_object_enabled
                        .get(&args[0])
                        .copied()
                        .unwrap_or(true);
                    let draw_enabled = self
                        .graph_object_draw_enabled
                        .get(&args[0])
                        .copied()
                        .unwrap_or(true);
                    let chain_drawable = self.display_tree.chain_drawable(args[0]);
                    let draw_eligible =
                        layer.is_some_and(|layer| self.should_draw_graph_layer(args[0], layer));
                    let primary_size = self.bitmap_dimensions.get(&args[3]).copied();
                    let secondary_size = self.bitmap_dimensions.get(&args[4]).copied();
                    tracing::info!(
                        sprite = args[0],
                        x = args[1],
                        y = args[2],
                        primary = args[3],
                        secondary = args[4],
                        ?primary_size,
                        ?secondary_size,
                        transition_value = args[5],
                        alpha = args[6],
                        priority = args[7],
                        transition_mode = args[8],
                        configured,
                        ?layer_key,
                        layer_enabled,
                        object_enabled,
                        draw_enabled,
                        chain_drawable,
                        draw_eligible,
                        parent = ?self.display_tree.parent(args[0]),
                        "GraphConfigureSpriteDualBitmap"
                    );
                }
                ethornell_vm::Value::None
            }
            (0x90, 0x59) => {
                let args = Self::graph90_source_args(stack, 13);
                if args.len() == 13
                    && self.graph90_object_matches(
                        args[0],
                        GRAPH90_SPRITE_TAG,
                        512,
                        GRAPH90_CLASS_SPRITE,
                    )
                {
                    self.graph90_begin_sprite_configuration(args[0], 2);
                    self.graph90_set_common_state(
                        args[0],
                        Some((args[1], args[2])),
                        Some(args[10]),
                        Some(args[11]),
                        Some(args[12]),
                    );
                    self.graph_object_properties
                        .entry(args[0])
                        .or_default()
                        .format_resource = Some(args[3]);
                    {
                        let properties = self.graph_object_properties.entry(args[0]).or_default();
                        properties
                            .named_properties
                            .insert("mode2-transform-x".to_string(), args[4]);
                        properties
                            .named_properties
                            .insert("mode2-transform-y".to_string(), args[5]);
                        properties
                            .named_properties
                            .insert("mode2-rotation-16-16".to_string(), args[6]);
                        properties
                            .named_properties
                            .insert("mode2-scale-x-16-16".to_string(), args[7]);
                        properties
                            .named_properties
                            .insert("mode2-scale-y-16-16".to_string(), args[8]);
                        properties
                            .named_properties
                            .insert("mode2-raster-mode-flag".to_string(), args[9]);
                    }
                    let configured = self.graph90_sync_mode2_primary_layer(&args);
                    let layer = self.graph_layers.get(&args[0]);
                    tracing::info!(
                        sprite = args[0],
                        x = args[1],
                        y = args[2],
                        primary = args[3],
                        transform_x = args[4],
                        transform_y = args[5],
                        rotation_16_16 = args[6],
                        scale_x_16_16 = args[7],
                        scale_y_16_16 = args[8],
                        raster_mode_flag = args[9],
                        alpha_parameter = args[11],
                        priority = args[12],
                        configured,
                        layer_x = ?layer.map(|layer| layer.x),
                        layer_y = ?layer.map(|layer| layer.y),
                        layer_width = ?layer.map(|layer| layer.width),
                        layer_height = ?layer.map(|layer| layer.height),
                        draw_eligible = layer.is_some_and(|layer| self.should_draw_graph_layer(args[0], layer)),
                        parent = ?self.display_tree.parent(args[0]),
                        "GraphConfigureSpriteMode2"
                    );
                    self.graph90_record_call(args[0], id, &args);
                }
                ethornell_vm::Value::None
            }
            (0x90, 0x5A) => {
                let args = Self::graph90_source_args(stack, 10);
                if args.len() == 10
                    && self.graph90_object_matches(
                        args[0],
                        GRAPH90_SPRITE_TAG,
                        512,
                        GRAPH90_CLASS_SPRITE,
                    )
                {
                    self.graph90_begin_sprite_configuration(args[0], 3);
                    self.graph90_set_common_state(
                        args[0],
                        Some((args[1], args[2])),
                        Some(args[7]),
                        Some(args[8]),
                        Some(args[9]),
                    );
                    let properties = self.graph_object_properties.entry(args[0]).or_default();
                    properties.format_resource = Some(args[3]);
                    properties.native.fixed_parameter_16_16 = args[6].saturating_mul(0x1_0000);
                    properties
                        .named_properties
                        .insert("mode3-mask-resource".to_string(), args[4]);
                    properties
                        .named_properties
                        .insert("mode3-mask-parameter".to_string(), args[5]);
                    properties
                        .named_properties
                        .insert("mode3-fixed-parameter".to_string(), args[6]);
                    self.graph90_sync_mode3_primary_layer(&args);
                    self.graph90_record_call(args[0], id, &args);
                }
                ethornell_vm::Value::None
            }
            (0x90, 0x5B) => {
                let args = Self::graph90_source_args(stack, 10);
                if args.len() == 10
                    && self.graph90_object_matches(
                        args[0],
                        GRAPH90_SPRITE_TAG,
                        512,
                        GRAPH90_CLASS_SPRITE,
                    )
                {
                    self.graph90_begin_sprite_configuration(args[0], 4);
                    self.graph90_set_common_state(
                        args[0],
                        Some((args[1], args[2])),
                        Some(1),
                        Some(args[7]),
                        Some(args[9]),
                    );
                    let properties = self.graph_object_properties.entry(args[0]).or_default();
                    properties.format_resource = Some(args[3]);
                    properties.mask_alpha = args[8];
                    properties.native.mask_alpha = args[8];
                    properties
                        .named_properties
                        .insert("mode4-effect-resource".to_string(), args[4]);
                    properties
                        .named_properties
                        .insert("mode4-record-count".to_string(), args[5]);
                    properties
                        .named_properties
                        .insert("mode4-record-pointer".to_string(), args[6]);
                    self.graph90_sync_mode4_primary_layer(&args);
                    self.graph90_record_call(args[0], id, &args);
                }
                ethornell_vm::Value::None
            }
            (0x90, 0x5C) => {
                let args = Self::graph90_source_args(stack, 17);
                if args.len() != 17 {
                    return Some(Err(ethornell_vm::VmError::Runtime(
                        "Graph90:5C expected seventeen arguments".into(),
                    )));
                }
                // sub_47CC10: primary < 0x4000, transition <= 0x100, blend
                // mode, alpha <= 0x100, priority < 0x10000.
                if args[6] as u32 > 0x100 {
                    return Some(Err(ethornell_vm::VmError::Runtime(format!(
                        "Graph90:5C transition value {} exceeds 256",
                        args[6]
                    ))));
                }
                if let Err(error) = Self::graph90_validate_sprite_arguments(
                    id,
                    args[16],
                    args[15],
                    args[14],
                    &[args[4]],
                ) {
                    return Some(Err(error));
                }
                // sub_43EAB0 / sub_427AA0 result codes: 255 missing sprite,
                // 0x80000001/2 missing primary/secondary bitmap (fatal),
                // 0x80000003 secondary of another size or format (silent,
                // after the common state was applied), 0x80000004 projected
                // extent below 2 (fatal).
                if !self.graph90_object_matches(
                    args[0],
                    GRAPH90_SPRITE_TAG,
                    512,
                    GRAPH90_CLASS_SPRITE,
                ) {
                    return Some(Err(ethornell_vm::VmError::Runtime(format!(
                        "Graph90:5C #{} is not a sprite object",
                        args[0]
                    ))));
                }
                let primary = self.query_bitmap_info(args[4]);
                let secondary = (args[5] != -1).then(|| self.query_bitmap_info(args[5]));
                if primary.is_none() || secondary.as_ref().is_some_and(Option::is_none) {
                    return Some(Err(ethornell_vm::VmError::Runtime(format!(
                        "Graph90:5C bitmap {} / {} does not exist",
                        args[4], args[5]
                    ))));
                }
                if let (Some(primary), Some(Some(secondary))) = (&primary, &secondary)
                    && (primary.width, primary.height, primary.format)
                        != (secondary.width, secondary.height, secondary.format)
                {
                    self.graph90_set_fixed_common_state(
                        args[0],
                        args[1],
                        args[2],
                        args[3],
                        Some(args[14]),
                        Some(args[15]),
                        Some(args[16]),
                    );
                    return Some(Ok(ethornell_vm::Value::None));
                }
                {
                    self.graph90_begin_sprite_configuration(args[0], 5);
                    self.graph90_set_fixed_common_state(
                        args[0],
                        args[1],
                        args[2],
                        args[3],
                        Some(args[14]),
                        Some(args[15]),
                        Some(args[16]),
                    );
                    let properties = self.graph_object_properties.entry(args[0]).or_default();
                    properties.format_resource = Some(args[4]);
                    properties
                        .named_properties
                        .insert("mode5-z-16-16".to_string(), args[3]);
                    properties
                        .named_properties
                        .insert("mode5-secondary-bitmap".to_string(), args[5]);
                    properties
                        .named_properties
                        .insert("mode5-transition-value".to_string(), args[6]);
                    properties
                        .named_properties
                        .insert("mode5-transition-current".to_string(), args[6]);
                    properties
                        .named_properties
                        .insert("mode5-secondary-parameter".to_string(), args[7]);
                    properties
                        .named_properties
                        .insert("mode5-fixed-parameter-x".to_string(), args[8]);
                    properties
                        .named_properties
                        .insert("mode5-fixed-parameter-y".to_string(), args[9]);
                    for (index, value) in args[10..=13].iter().copied().enumerate() {
                        properties
                            .named_properties
                            .insert(format!("mode5-transform-parameter-{index}"), value);
                    }
                    let configured = self.graph90_sync_mode5_primary_layer(&args);
                    let primary_size = self
                        .resource_image_region(args[4])
                        .map(|(_, region)| (region.width, region.height));
                    let layer_state = self.graph_layers.get(&args[0]).map(|layer| {
                        (
                            layer.x,
                            layer.y,
                            layer.width,
                            layer.height,
                            layer.scale_x,
                            layer.scale_y,
                            layer.opacity,
                            layer.z,
                            layer.enabled,
                        )
                    });
                    let object_enabled = self
                        .graph_object_enabled
                        .get(&args[0])
                        .copied()
                        .unwrap_or(true);
                    let draw_enabled = self
                        .graph_object_draw_enabled
                        .get(&args[0])
                        .copied()
                        .unwrap_or(true);
                    let chain_drawable = self.display_tree.chain_drawable(args[0]);
                    let draw_eligible = self
                        .graph_layers
                        .get(&args[0])
                        .is_some_and(|layer| self.should_draw_graph_layer(args[0], layer));
                    let cache_nonzero_alpha =
                        self.graph_object_properties
                            .get(&args[0])
                            .and_then(|properties| {
                                properties
                                    .named_properties
                                    .get("mode5-cache-nonzero-alpha")
                                    .copied()
                            });
                    let cache_max_alpha =
                        self.graph_object_properties
                            .get(&args[0])
                            .and_then(|properties| {
                                properties
                                    .named_properties
                                    .get("mode5-cache-max-alpha")
                                    .copied()
                            });
                    let property_40 = self
                        .graph_object_properties
                        .get(&args[0])
                        .and_then(|properties| properties.properties.get(&0x40).copied());
                    let global_offset_gate = self
                        .graph_object_properties
                        .get(&args[0])
                        .map(|properties| properties.native.global_display_offset_enabled)
                        .unwrap_or_default();
                    let world_state = self.graph_layers.get(&args[0]).map(|layer| {
                        let (x, y, z) = self.layer_world_transform(args[0], layer);
                        (x, y, z)
                    });
                    let origin_offset =
                        self.graph_object_properties
                            .get(&args[0])
                            .map(|properties| {
                                (
                                    properties
                                        .named_properties
                                        .get("mode5-origin-offset-x")
                                        .copied(),
                                    properties
                                        .named_properties
                                        .get("mode5-origin-offset-y")
                                        .copied(),
                                )
                            });
                    tracing::info!(
                        sprite = args[0],
                        x_16_16 = args[1],
                        y_16_16 = args[2],
                        z_16_16 = args[3],
                        primary = args[4],
                        secondary = args[5],
                        transition_value = args[6],
                        secondary_parameter = args[7],
                        constructor_base_x = args[8],
                        constructor_base_y = args[9],
                        rotation_16_16 = args[10],
                        perspective = args[11],
                        project_position = args[12],
                        transform_parameter_3 = args[13],
                        ?property_40,
                        graph_center = ?self.graph_config.center,
                        global_display_offset = ?self.graph_global_offset,
                        global_offset_gate,
                        ?origin_offset,
                        ?world_state,
                        ?primary_size,
                        alpha_parameter = args[15],
                        priority = args[16],
                        configured,
                        ?layer_state,
                        object_enabled,
                        draw_enabled,
                        chain_drawable,
                        draw_eligible,
                        ?cache_nonzero_alpha,
                        ?cache_max_alpha,
                        layer_key = ?self.graph_layers.get(&args[0]).map(|layer| layer.key.as_str()),
                        parent = ?self.display_tree.parent(args[0]),
                        "GraphConfigureSpriteMode5"
                    );
                    self.graph90_record_call(args[0], id, &args);
                    if let Some((width, height)) = self.graph90_sprite_projected_extent(args[0])
                        && (width < 2 || height < 2)
                    {
                        return Some(Err(ethornell_vm::VmError::Runtime(format!(
                            "Graph90:5C projected size {width}x{height} is too small (perspective {})",
                            args[11]
                        ))));
                    }
                }
                ethornell_vm::Value::None
            }
            (0x90, 0x5D) => {
                let args = Self::graph90_source_args(stack, 20);
                if args.len() == 20
                    && self.graph90_object_matches(
                        args[0],
                        GRAPH90_SPRITE_TAG,
                        512,
                        GRAPH90_CLASS_SPRITE,
                    )
                {
                    self.graph90_begin_sprite_configuration(args[0], 6);
                    self.graph90_set_fixed_common_state(
                        args[0],
                        args[1],
                        args[2],
                        args[3],
                        Some(args[17]),
                        Some(args[18]),
                        Some(args[19]),
                    );
                    let properties = self.graph_object_properties.entry(args[0]).or_default();
                    properties.format_resource = Some(args[4]);
                    properties
                        .named_properties
                        .insert("mode6-z-16-16".to_string(), args[3]);
                    properties
                        .named_properties
                        .insert("mode6-secondary-bitmap".to_string(), args[5]);
                    properties
                        .named_properties
                        .insert("mode6-transition-value".to_string(), args[6]);
                    properties
                        .named_properties
                        .insert("mode6-secondary-parameter".to_string(), args[7]);
                    properties
                        .named_properties
                        .insert("mode6-fixed-parameter-x".to_string(), args[8]);
                    properties
                        .named_properties
                        .insert("mode6-fixed-parameter-y".to_string(), args[9]);
                    for (index, value) in args[10..=16].iter().copied().enumerate() {
                        properties
                            .named_properties
                            .insert(format!("mode6-transform-parameter-{index}"), value);
                    }
                    let configured = self.graph90_sync_mode6_primary_layer(&args);
                    let primary_size = self
                        .resource_image_region(args[4])
                        .map(|(_, region)| (region.width, region.height));
                    let layer_state = self.graph_layers.get(&args[0]).map(|layer| {
                        (
                            layer.x,
                            layer.y,
                            layer.width,
                            layer.height,
                            layer.scale_x,
                            layer.scale_y,
                            layer.opacity,
                            layer.z,
                            layer.enabled,
                        )
                    });
                    let object_enabled = self
                        .graph_object_enabled
                        .get(&args[0])
                        .copied()
                        .unwrap_or(true);
                    let draw_enabled = self
                        .graph_object_draw_enabled
                        .get(&args[0])
                        .copied()
                        .unwrap_or(true);
                    let chain_drawable = self.display_tree.chain_drawable(args[0]);
                    let draw_eligible = self
                        .graph_layers
                        .get(&args[0])
                        .is_some_and(|layer| self.should_draw_graph_layer(args[0], layer));
                    tracing::info!(
                        sprite = args[0],
                        x_16_16 = args[1],
                        y_16_16 = args[2],
                        z_16_16 = args[3],
                        primary = args[4],
                        ?primary_size,
                        alpha_parameter = args[18],
                        priority = args[19],
                        configured,
                        ?layer_state,
                        object_enabled,
                        draw_enabled,
                        chain_drawable,
                        draw_eligible,
                        parent = ?self.display_tree.parent(args[0]),
                        "GraphConfigureSpriteMode6"
                    );
                    self.graph90_record_call(args[0], id, &args);
                }
                ethornell_vm::Value::None
            }
            (0x90, 0x60) => {
                let handle = self.graph90_allocate_object(
                    GRAPH90_FILTER_TAG,
                    8,
                    GRAPH90_CLASS_FILTER,
                    NativeDisplayKind::Filter,
                    7,
                );
                if handle != 0 {
                    self.graph_object_properties
                        .entry(handle)
                        .or_default()
                        .blend_mode = 192;
                }
                ethornell_vm::Value::Int(handle)
            }
            (0x90, 0x61) => {
                let handle = pop_int_value(stack).unwrap_or_default();
                let released = self.graph90_release_object(
                    handle,
                    GRAPH90_FILTER_TAG,
                    8,
                    GRAPH90_CLASS_FILTER,
                );
                tracing::info!(handle, released, "GraphReleaseFilterObject");
                ethornell_vm::Value::None
            }
            (0x90, 0x64) => {
                let args = Self::graph90_source_args(stack, 2);
                if let [handle, enabled] = args.as_slice()
                    && self.graph90_object_matches(
                        *handle,
                        GRAPH90_FILTER_TAG,
                        8,
                        GRAPH90_CLASS_FILTER,
                    )
                {
                    self.graph90_set_enabled(*handle, *enabled != 0);
                    self.graph90_record_call(*handle, id, &args);
                }
                ethornell_vm::Value::None
            }
            (0x90, 0x65) => {
                let args = Self::graph90_source_args(stack, 4);
                if args.len() == 4
                    && self.graph90_object_matches(
                        args[0],
                        GRAPH90_FILTER_TAG,
                        8,
                        GRAPH90_CLASS_FILTER,
                    )
                {
                    self.graph90_set_common_state(
                        args[0],
                        None,
                        None,
                        Some(args[2]),
                        Some(args[3]),
                    );
                    self.graph_object_properties
                        .entry(args[0])
                        .or_default()
                        .named_properties
                        .insert("target-filter-parameter".to_string(), args[1]);
                    self.graph90_record_call(args[0], id, &args);
                }
                ethornell_vm::Value::None
            }
            (0x90, 0x66) => {
                let args = Self::graph90_source_args(stack, 7);
                if args.len() == 7
                    && self.graph90_object_matches(
                        args[0],
                        GRAPH90_FILTER_TAG,
                        8,
                        GRAPH90_CLASS_FILTER,
                    )
                {
                    self.graph90_set_common_state(
                        args[0],
                        None,
                        None,
                        Some(args[5]),
                        Some(args[6]),
                    );
                    let properties = self.graph_object_properties.entry(args[0]).or_default();
                    properties.format_resource = (args[3] != -1).then_some(args[3]);
                    properties
                        .named_properties
                        .insert("target-filter-reserved".to_string(), args[1]);
                    properties
                        .named_properties
                        .insert("target-filter-parameter".to_string(), args[2]);
                    properties
                        .named_properties
                        .insert("target-mask-parameter".to_string(), args[4]);
                    self.graph90_record_call(args[0], id, &args);
                }
                ethornell_vm::Value::None
            }
            (0x90, 0x70) => {
                let handle = self.graph90_allocate_object(
                    GRAPH90_MAP_TAG,
                    8,
                    GRAPH90_CLASS_MAP,
                    NativeDisplayKind::Map,
                    1,
                );
                ethornell_vm::Value::Int(handle)
            }
            (0x90, 0x71) => {
                let handle = pop_int_value(stack).unwrap_or_default();
                let released =
                    self.graph90_release_object(handle, GRAPH90_MAP_TAG, 8, GRAPH90_CLASS_MAP);
                tracing::info!(handle, released, "GraphReleaseMapObject");
                ethornell_vm::Value::None
            }
            (0x90, 0x74) => {
                let args = Self::graph90_source_args(stack, 2);
                if let [handle, enabled] = args.as_slice()
                    && self.graph90_object_matches(*handle, GRAPH90_MAP_TAG, 8, GRAPH90_CLASS_MAP)
                {
                    self.graph90_set_enabled(*handle, *enabled != 0);
                    self.graph90_record_call(*handle, id, &args);
                }
                ethornell_vm::Value::None
            }
            (0x90, 0x75) => {
                let args = Self::graph90_source_args(stack, 7);
                if args.len() == 7
                    && self.graph90_object_matches(args[0], GRAPH90_MAP_TAG, 8, GRAPH90_CLASS_MAP)
                {
                    self.graph90_set_common_state(
                        args[0],
                        Some((args[1], args[2])),
                        Some(args[4]),
                        Some(args[5]),
                        Some(args[6]),
                    );
                    self.graph_object_properties
                        .entry(args[0])
                        .or_default()
                        .format_resource = Some(args[3]);
                    self.graph90_record_call(args[0], id, &args);
                }
                ethornell_vm::Value::None
            }
            (0x90, 0x76) => {
                let args = Self::graph90_source_args(stack, 5);
                if args.len() == 5
                    && self.graph90_object_matches(args[0], GRAPH90_MAP_TAG, 8, GRAPH90_CLASS_MAP)
                {
                    let properties = self.graph_object_properties.entry(args[0]).or_default();
                    properties
                        .named_properties
                        .insert("map-columns".to_string(), args[1]);
                    properties
                        .named_properties
                        .insert("map-rows".to_string(), args[2]);
                    properties
                        .named_properties
                        .insert("map-cell-width".to_string(), args[3]);
                    properties
                        .named_properties
                        .insert("map-cell-height".to_string(), args[4]);
                    self.graph90_record_call(args[0], id, &args);
                }
                ethornell_vm::Value::None
            }
            (0x90, 0x78) => {
                let args = Self::graph90_source_args(stack, 4);
                if args.len() == 4
                    && self.graph90_object_matches(args[0], GRAPH90_MAP_TAG, 8, GRAPH90_CLASS_MAP)
                {
                    let properties = self.graph_object_properties.entry(args[0]).or_default();
                    properties
                        .named_properties
                        .insert("map-source-width".to_string(), args[1]);
                    properties
                        .named_properties
                        .insert("map-source-height".to_string(), args[2]);
                    properties
                        .named_properties
                        .insert("map-source-pointer".to_string(), args[3]);
                    self.graph90_record_call(args[0], id, &args);
                }
                ethornell_vm::Value::None
            }
            (0x90, 0x79) => {
                let args = Self::graph90_source_args(stack, 6);
                if args.len() == 6
                    && self.graph90_object_matches(args[0], GRAPH90_MAP_TAG, 8, GRAPH90_CLASS_MAP)
                {
                    let properties = self.graph_object_properties.entry(args[0]).or_default();
                    properties
                        .named_properties
                        .insert("map-source-x".to_string(), args[1]);
                    properties
                        .named_properties
                        .insert("map-source-y".to_string(), args[2]);
                    properties
                        .named_properties
                        .insert("map-cell-offset-x".to_string(), args[3]);
                    properties
                        .named_properties
                        .insert("map-cell-offset-y".to_string(), args[4]);
                    properties
                        .named_properties
                        .insert("map-wrap".to_string(), args[5]);
                    self.graph90_record_call(args[0], id, &args);
                }
                ethornell_vm::Value::None
            }
            (0x90, 0x7A) => {
                let args = Self::graph90_source_args(stack, 2);
                if args.len() == 2
                    && self.graph90_object_matches(args[0], GRAPH90_MAP_TAG, 8, GRAPH90_CLASS_MAP)
                {
                    self.graph_object_properties
                        .entry(args[0])
                        .or_default()
                        .named_properties
                        .insert("map-replace-tile-id".to_string(), args[1]);
                    self.graph90_record_call(args[0], id, &args);
                }
                ethornell_vm::Value::None
            }
            (0x90, 0x91) => {
                let scope_value = pop_int_value(stack).unwrap_or_default();
                let mode = pop_int_value(stack).unwrap_or_default();
                let accepted = self
                    .graph_defaults
                    .configure_message_input_scope(mode, scope_value);
                tracing::info!(mode, scope_value, accepted, "GraphSetMessageInputScope");
                ethornell_vm::Value::None
            }
            (0x90, 0x92) => {
                let enabled = pop_int_value(stack).unwrap_or_default();
                self.graph_defaults.message_input_filter_enabled = enabled != 0;
                tracing::info!(enabled, "GraphSetMessageInputFilter");
                ethornell_vm::Value::None
            }
            (0x90, 0x9E) => {
                // BP declaration order is [bitmap, character_count]. The
                // target slices one horizontal strip into private full-width
                // glyph slots beginning at codepoint 0xFF01.
                let character_count = pop_int_value(stack).unwrap_or_default();
                let bitmap = pop_int_value(stack).unwrap_or_default();
                if character_count <= 0 {
                    self.fullwidth_glyph_bitmap = None;
                    self.fullwidth_glyph_count = 0;
                    self.fullwidth_glyph_cell_width = 0;
                } else if character_count <= 255
                    && let Some((width, _height)) = self.bitmap_dimensions.get(&bitmap).copied()
                {
                    let count = character_count as u32;
                    if count != 0 && width % count == 0 {
                        self.fullwidth_glyph_bitmap = Some(bitmap);
                        self.fullwidth_glyph_count = character_count;
                        self.fullwidth_glyph_cell_width = (width / count) as i32;
                    }
                }
                tracing::info!(
                    bitmap,
                    character_count,
                    cell_width = self.fullwidth_glyph_cell_width,
                    "GraphRegisterFullwidthGlyphStrip"
                );
                ethornell_vm::Value::None
            }
            (0x90, 0xA0) | (0x90, 0xA2) | (0x90, 0xA3) => {
                // The VM installs the selector-specific CProcSelectItem
                // subclass after this host call. Keep the host side limited
                // to target-shaped validation and state capture; these
                // selectors are not generic text draws.
                let count = if id == 0xA0 { 8 } else { 14 };
                let args = pop_args(stack, count);
                let values = args.iter().map(value_to_i32).collect::<Vec<_>>();
                let item_count = values.get(count - 2).copied().unwrap_or_default();
                let column_count = values.get(count - 4).copied().unwrap_or_default();
                let initial_selection = values.get(count - 7).copied().unwrap_or_default();
                tracing::info!(
                    selector = format_args!("0x{id:02X}"),
                    item_count,
                    column_count,
                    initial_selection,
                    valid = (1..=16).contains(&item_count)
                        && (1..=16).contains(&column_count)
                        && (0..item_count).contains(&initial_selection),
                    "start target item-selection procedure"
                );
                ethornell_vm::Value::None
            }
            (0x90, 0xA1) => {
                let args = pop_args(stack, 6);
                tracing::debug!(args = ?summarize_values(&args), "GraphDrawItemSelectionGrid");
                ethornell_vm::Value::None
            }
            (0x90, 0xA4) => {
                let style_b = pop_int_value(stack).unwrap_or_default();
                let style_a = pop_int_value(stack).unwrap_or_default();
                self.graph_item_selection_highlight_styles = [style_a, style_b];
                tracing::info!(style_a, style_b, "GraphSetItemSelectionHighlightStyles");
                ethornell_vm::Value::None
            }
            (0x90, 0xA5) => {
                let input_mask = pop_int_value(stack).unwrap_or_default();
                self.graph_item_selection_input_mask = input_mask;
                tracing::info!(
                    input_mask = format_args!("0x{input_mask:08X}"),
                    "GraphSetItemSelectionInputMask"
                );
                ethornell_vm::Value::None
            }
            (0x90, 0xA6) => {
                let arg3 = pop_int_value(stack).unwrap_or_default();
                let arg2 = pop_int_value(stack).unwrap_or_default();
                let arg1 = pop_int_value(stack).unwrap_or_default();
                let alternate_key_mode = pop_int_value(stack).unwrap_or_default();
                self.graph_item_selection_navigation = [alternate_key_mode, arg1, arg2, arg3];
                tracing::info!(
                    alternate_key_mode,
                    arg1,
                    arg2,
                    arg3,
                    "GraphSetItemSelectionNavigationParameters"
                );
                ethornell_vm::Value::None
            }
            (0x90, 0xA7) => {
                // The VM owns the BP pointer conversion and exact 16-DWORD
                // copy before calling set_item_selection_column_layout().
                // This fallback only consumes a direct API call safely.
                let args = pop_args(stack, 2);
                tracing::debug!(args = ?summarize_values(&args), "GraphSetItemSelectionColumnLayout fallback");
                ethornell_vm::Value::None
            }
            (0x90, 0xAF) => {
                let value = pop_int_value(stack).unwrap_or_default();
                self.graph_interactive_procedure_poll_gate = value;
                tracing::info!(value, "GraphSetInteractiveProcedurePollGate");
                ethornell_vm::Value::None
            }
            (0x90, 0xB0) | (0x90, 0xB1) => {
                // These selectors install CProcSelectIcon/CProcSelectIconEx;
                // they are not display-object transition or shake handlers.
                let args = pop_args(stack, 7);
                let values = args.iter().map(value_to_i32).collect::<Vec<_>>();
                let icon_count = values.get(5).copied().unwrap_or_default();
                let input_mode = values.get(2).copied().unwrap_or_default();
                tracing::info!(
                    selector = format_args!("0x{id:02X}"),
                    icon_count,
                    input_mode,
                    valid = (1..=64).contains(&icon_count) && (0..=3).contains(&input_mode),
                    "start target icon-selection procedure"
                );
                ethornell_vm::Value::None
            }
            // VM writes the output word for 0xBD.
            (0x90, 0xBD) => {
                let _args = pop_args(stack, 2);
                ethornell_vm::Value::Int(0)
            }
            (0x90, 0xC0) => {
                let file = pop_string_value(stack).unwrap_or_default();
                let archive = pop_string_value(stack).unwrap_or_default();
                let target = pop_int_value(stack).unwrap_or_default();
                let loaded = self.load_graph_image_resource(target, &archive, &file);
                tracing::debug!(target, archive, file, loaded, "GraphLoadBgBitmapResource");
                ethornell_vm::Value::None
            }
            (0x90, 0xC2) => {
                let direction = pop_int_value(stack).unwrap_or_default();
                let source = pop_int_value(stack).unwrap_or_default();
                let destination = pop_int_value(stack).unwrap_or_default();
                let flipped = self.flip_graph_bitmap(destination, source, direction);
                tracing::debug!(destination, source, direction, flipped, "GraphFlipBitmap");
                ethornell_vm::Value::None
            }
            (0x90, 0xC3) => {
                let source = pop_int_value(stack).unwrap_or_default();
                let destination = pop_int_value(stack).unwrap_or_default();
                let downsampled = self.downsample_graph_bitmap_half(destination, source);
                tracing::debug!(
                    destination,
                    source,
                    downsampled,
                    "GraphDownsampleBitmapHalf"
                );
                ethornell_vm::Value::None
            }
            (0x90, 0xC4) => {
                let pixel_mode = pop_int_value(stack).unwrap_or_default();
                let file = pop_string_value(stack).unwrap_or_default();
                let destination = pop_int_value(stack).unwrap_or_default();
                let status = if !matches!(pixel_mode, -1 | 1 | 2 | 3) {
                    -1
                } else if let Some(path) =
                    find_runtime_file_from_root(&self.manager, &self.native_root, "", &file)
                {
                    match std::fs::read(path)
                        .ok()
                        .and_then(|bytes| decode_image(&bytes).ok())
                    {
                        Some(image) => {
                            // The target writes a GDI+ bitmap descriptor through
                            // the BP destination. VM-owned pointer emission is
                            // intentionally not guessed in this host fallback.
                            tracing::debug!(
                                destination,
                                width = image.width,
                                height = image.height,
                                "GraphImportExternalImage decoded"
                            );
                            0
                        }
                        None => 2,
                    }
                } else {
                    1
                };
                tracing::debug!(
                    destination,
                    file,
                    pixel_mode,
                    status,
                    "GraphImportExternalImage"
                );
                ethornell_vm::Value::Int(status)
            }
            (0x90, 0xC5) => {
                let bitmap = pop_int_value(stack).unwrap_or_default();
                let quality = pop_int_value(stack).unwrap_or_default();
                let format = pop_int_value(stack).unwrap_or_default();
                let file = pop_string_value(stack).unwrap_or_default();
                let status = if !(0..=4).contains(&format) {
                    4
                } else if let Some(image) = self.graph_bitmap_image(bitmap) {
                    if format == 4 {
                        let path =
                            runtime_file_path_from_root(&self.manager, &self.native_root, &file)
                                .unwrap_or_else(|| std::path::PathBuf::from(&file));
                        if ethornell_image::write_rgba_png(&image, &path).is_ok() {
                            0
                        } else {
                            5
                        }
                    } else {
                        // Portable fallback only has a lossless PNG writer;
                        // target GDI+ BMP/JPEG/GIF/TIFF encoders remain partial.
                        5
                    }
                } else {
                    -1
                };
                tracing::debug!(
                    bitmap,
                    file,
                    format,
                    quality,
                    status,
                    "GraphSaveBitmapToImageFile"
                );
                ethornell_vm::Value::Int(status)
            }
            (0x90, 0xC6) => {
                // The real BP byte-range bridge is handled in the VM before
                // this host fallback is reached.
                let size = pop_int_value(stack).unwrap_or_default();
                let source_pointer = pop_int_value(stack).unwrap_or_default();
                let name = pop_string_value(stack).unwrap_or_default();
                let namespace = pop_string_value(stack).unwrap_or_default();
                tracing::debug!(
                    namespace,
                    name,
                    source_pointer,
                    size,
                    "GraphRegisterBgResourceData fallback"
                );
                ethornell_vm::Value::Int(0)
            }
            (0x90, 0xC7) => {
                let consume = pop_int_value(stack).unwrap_or_default() != 0;
                let name = pop_string_value(stack).unwrap_or_default();
                let namespace = pop_string_value(stack).unwrap_or_default();
                let bitmap = pop_int_value(stack).unwrap_or_default();
                let key = (namespace.to_ascii_lowercase(), name.to_ascii_lowercase());
                let bytes = if consume {
                    self.graph_blob_cache.remove(&key)
                } else {
                    self.graph_blob_cache.get(&key).cloned()
                };
                let loaded = bytes
                    .and_then(|bytes| decode_image(&bytes).ok())
                    .map(|image| {
                        self.store_runtime_bitmap(bitmap, image, 2);
                        true
                    })
                    .unwrap_or(false);
                tracing::debug!(
                    bitmap,
                    namespace,
                    name,
                    consume,
                    loaded,
                    "GraphLoadCachedBgBitmap"
                );
                ethornell_vm::Value::Int(i32::from(loaded))
            }
            (0x90, 0xC8) => {
                let args = pop_args(stack, 18);
                tracing::debug!(args = ?summarize_values(&args), "GraphTransformBitmapGeneral");
                ethornell_vm::Value::None
            }
            (0x90, 0xCA) => {
                let source = pop_int_value(stack).unwrap_or_default();
                let destination = pop_int_value(stack).unwrap_or_default();
                let scaled = self.aspect_fit_graph_bitmap(destination, source);
                tracing::debug!(destination, source, scaled, "GraphScaleBitmapAspectFit");
                ethornell_vm::Value::None
            }
            // 90:CC/CD are handled by the typed GraphApi path because CC
            // needs the VM BP-pointer bridge and CD uses the registered LUT.
            (0x90, 0xCE) => {
                // The VM owns destination/size pointers; direct host calls can
                // only consume the ABI without manufacturing guest addresses.
                let parameter = pop_int_value(stack).unwrap_or_default();
                let format = pop_int_value(stack).unwrap_or_default();
                let bitmap = pop_int_value(stack).unwrap_or_default();
                let size_out = pop_int_value(stack).unwrap_or_default();
                let destination = pop_int_value(stack).unwrap_or_default();
                tracing::debug!(
                    destination,
                    size_out,
                    bitmap,
                    format,
                    parameter,
                    "GraphEncodeBitmapToBuffer fallback"
                );
                ethornell_vm::Value::None
            }
            (0x90, 0xF8) => {
                // sub_496270 unregisters every Sprite input region and resets
                // the monotonic target-number counter to zero.
                for (_, sprite) in self.graph_sprite_targets.entries() {
                    self.unregister_pointer_object_node(sprite);
                }
                self.graph_sprite_targets.clear();
                ethornell_vm::Value::None
            }
            (0x90, 0xFA) => {
                let sprite = pop_int_value(stack).unwrap_or_default();
                if !self.graph90_is_sprite_handle(sprite) {
                    return Some(Err(ethornell_vm::VmError::Runtime(format!(
                        "Graph90:FA invalid CDspObjSprite handle #{sprite}"
                    ))));
                }
                let target_number = self.graph_sprite_targets.register(sprite);
                // sub_4962A0 also adds the sprite to the pointer chain.
                self.register_pointer_object_node(sprite);
                tracing::debug!(sprite, target_number, "GraphRegisterSpriteTarget");
                ethornell_vm::Value::None
            }
            (0x90, 0xFB) => {
                let sprite = pop_int_value(stack).unwrap_or_default();
                if !self.graph_sprite_targets.unregister_sprite(sprite) {
                    return Some(Err(ethornell_vm::VmError::Runtime(format!(
                        "Graph90:FB Sprite handle #{sprite} is not registered"
                    ))));
                }
                self.unregister_pointer_object_node(sprite);
                tracing::debug!(sprite, "GraphUnregisterSpriteTarget");
                ethornell_vm::Value::None
            }
            (0x90, 0xFC) => {
                let entries = self.graph_sprite_targets.entries();
                let target_number = entries
                    .into_iter()
                    .find(|(_, sprite)| {
                        self.graph_object_pointer_hit(*sprite).unwrap_or_default() != 0
                    })
                    .map(|(target_number, _)| target_number)
                    .unwrap_or(-1);
                ethornell_vm::Value::Int(target_number)
            }
            (0x90, 0xFD) => {
                let target_number = pop_int_value(stack).unwrap_or_default();
                let Some(state) = self.graph_sprite_targets.sampled_state(target_number) else {
                    return Some(Err(ethornell_vm::VmError::Runtime(format!(
                        "Graph90:FD invalid Sprite target number {target_number}"
                    ))));
                };
                ethornell_vm::Value::Int(state)
            }
            _ => return None,
        };
        Some(Ok(value))
    }

    pub(super) fn render_native_text_args(&mut self, args: &[ethornell_vm::Value]) {
        if args
            .iter()
            .any(|value| matches!(value, ethornell_vm::Value::Str(text) if !text.is_empty()))
        {
            self.render_graph_text(args);
        }
    }

    fn apply_native_transition_args(&mut self, args: &[ethornell_vm::Value]) {
        let values = args.iter().map(value_to_i32).collect::<Vec<_>>();
        let Some(target) = values
            .iter()
            .rev()
            .copied()
            .find(|value| self.graph_handle_exists(*value))
        else {
            return;
        };
        let Some(transparency) = values
            .iter()
            .copied()
            .find(|value| *value != target && (0..=256).contains(value))
        else {
            return;
        };

        // All CDspObj transition records use transparency, not opacity. The
        // previous generic path inverted the visible result (0 hid an object
        // and 256 showed it), which is particularly visible in the warning
        // fade sequence. Keep object state in native units so blend-specific
        // GetMaskAlpha behavior remains centralized in `opacity()`.
        let is_object = self.display_tree.contains(target)
            || self.graph_object_layers.contains_key(&target)
            || self.graph_object_properties.contains_key(&target);
        if is_object {
            self.set_graph_object_alpha_recursive(target, transparency);
        } else {
            let opacity = (1.0 - transparency as f32 / 256.0).clamp(0.0, 1.0);
            for layer in self.graph_target_layers(target) {
                if let Some(layer) = self.graph_layers.get_mut(&layer) {
                    layer.opacity = opacity;
                }
            }
            if let Some(surface) = self.graph_surfaces.get_mut(&target) {
                surface.opacity = opacity;
            }
        }
    }

    pub(super) fn apply_native_bitmap_operation(
        &mut self,
        args: &[ethornell_vm::Value],
        operation: NativeBitmapOperation,
    ) {
        let values = args.iter().map(value_to_i32).collect::<Vec<_>>();
        let handles = values
            .iter()
            .rev()
            .copied()
            .filter(|value| self.graph_resources.contains_key(value))
            .take(2)
            .collect::<Vec<_>>();
        let [destination, source] = handles.as_slice() else {
            return;
        };
        match operation {
            NativeBitmapOperation::Copy => self.copy_graph_backing(*source, *destination),
            NativeBitmapOperation::Composite => {
                let x = values.get(1).copied().unwrap_or_default();
                let y = values.first().copied().unwrap_or_default();
                self.composite_graph_bitmap(*destination, *source, x, y, 0, 256);
            }
            NativeBitmapOperation::Scale => {
                let scale_x = values
                    .iter()
                    .copied()
                    .find(|value| value.unsigned_abs() >= 0x100)
                    .unwrap_or(0x1_0000);
                let scale_y = scale_x;
                if let Some(image) = self
                    .graph_bitmap_image(*source)
                    .and_then(|image| scale_decoded_image_fixed(&image, scale_x, scale_y))
                {
                    let width = image.width;
                    let height = image.height;
                    let key = format!("runtime:native-scale:{destination}");
                    self.store_graph_image(key.clone(), image);
                    self.graph_resources
                        .insert(*destination, RuntimeGraphResource::whole(key));
                    self.bitmap_dimensions.insert(*destination, (width, height));
                }
            }
        }
    }
}

#[derive(Clone, Copy)]
pub(super) enum NativeBitmapOperation {
    Copy,
    Composite,
    Scale,
}

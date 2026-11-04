//! DCIPIcon constructor/configuration registration (448AB0/44BDB0).
//! Virtual geometry and masks are independent of visual item eligibility.
use super::*;

#[derive(Debug)]
struct InputVirtual {
    ordinal: i32,
    rect: [i32; 4],
    mask: Option<(DecodedImage, i32)>,
    never_hit: bool,
}

#[derive(Debug)]
pub(crate) struct InputChain {
    window: i32,
    scope: i32,
    virtual_scope: u32,
    full: bool,
    pointer_window: bool,
    items: Vec<InputVirtual>,
}

impl RuntimeTraceApi {
    pub(super) fn release_icon_input_chain(&mut self, object: i32) {
        let Some(chain) = self.icon_input_chains.remove(&object) else {
            return;
        };
        if chain.full {
            self.registered_pointer_input_scopes
                .unregister_one(chain.scope);
            self.registered_keyboard_input_scopes
                .unregister_one(chain.scope);
        } else if chain.pointer_window {
            self.unregister_pointer_object_node(chain.window);
        }
    }

    pub(super) fn configure_icon_input_chain(&mut self, object: i32) {
        self.release_icon_input_chain(object);
        let Some(input) = self.graph_input_objects.get(&object) else {
            return;
        };
        let window = input.layer;
        let descriptor = input.descriptor.clone();
        let scope = self.graph90_native_sort_key(window).unwrap_or(0) as i32;
        let mut items = Vec::new();
        for region in &descriptor.regions {
            if region.enabled_depth == 0 && !input.extended {
                continue;
            }
            let bitmap = native_surface_control_configure_resource(region);
            let Some((_, size)) = self.resource_image_region(bitmap) else {
                continue;
            };
            let (x, y) = self.surface_control_region_position(window, &descriptor, region);
            let mask = self.graph_bitmap_image(region.mask_resource).map(|pixels| {
                (
                    pixels,
                    self.bitmap_formats
                        .get(&region.mask_resource)
                        .copied()
                        .unwrap_or(2),
                )
            });
            items.push(InputVirtual {
                ordinal: region.ordinal,
                rect: [x, y, size.width as i32, size.height as i32],
                mask,
                never_hit: region.mask_resource == -2 || region.enabled_depth == 0,
            });
        }
        let full = descriptor.flags[0] != 0;
        let pointer_window = !full && descriptor.flags[1] != 0;
        if full {
            self.registered_pointer_input_scopes.register(scope);
            self.registered_keyboard_input_scopes.register(scope);
        } else if pointer_window {
            self.register_pointer_object_node(window);
        }
        self.icon_input_chains.insert(
            object,
            InputChain {
                window,
                scope,
                virtual_scope: (scope as u32).wrapping_add(0x8000),
                full,
                pointer_window,
                items,
            },
        );
        // Constructor drains both chains, after all Virtual entries exist.
        let _ = query_runtime_input_event_bits(self, scope, false, true);
        let _ = query_runtime_input_event_bits(
            self,
            (scope as u32).wrapping_add(0x8000) as i32,
            false,
            true,
        );
    }

    /// Complete pending portable materialization without replacing masks or
    /// registration keys already copied by native configuration.
    pub(super) fn sync_icon_input_virtuals(&mut self, object: i32) {
        let Some(chain) = self.icon_input_chains.get(&object) else {
            return;
        };
        let known = chain
            .items
            .iter()
            .map(|v| v.ordinal)
            .collect::<BTreeSet<_>>();
        let Some(input) = self.graph_input_objects.get(&object) else {
            return;
        };
        let (window, descriptor, extended) =
            (input.layer, input.descriptor.clone(), input.extended);
        let mut added = Vec::new();
        for region in &descriptor.regions {
            if known.contains(&region.ordinal) || region.enabled_depth == 0 && !extended {
                continue;
            }
            let Some((_, size)) =
                self.resource_image_region(native_surface_control_configure_resource(region))
            else {
                continue;
            };
            let (x, y) = self.surface_control_region_position(window, &descriptor, region);
            let mask = self.graph_bitmap_image(region.mask_resource).map(|image| {
                (
                    image,
                    self.bitmap_formats
                        .get(&region.mask_resource)
                        .copied()
                        .unwrap_or(2),
                )
            });
            added.push(InputVirtual {
                ordinal: region.ordinal,
                rect: [x, y, size.width as i32, size.height as i32],
                mask,
                never_hit: region.mask_resource == -2 || region.enabled_depth == 0,
            });
        }
        self.icon_input_chains
            .get_mut(&object)
            .unwrap()
            .items
            .extend(added);
    }

    pub(super) fn icon_input_copied_mask_accepts(
        &self,
        object: i32,
        ordinal: i32,
        x: i32,
        y: i32,
    ) -> Option<bool> {
        let item = self
            .icon_input_chains
            .get(&object)?
            .items
            .iter()
            .find(|v| v.ordinal == ordinal)?;
        if item.never_hit {
            return Some(false);
        }
        let Some((image, format)) = &item.mask else {
            return Some(true);
        };
        if x < 0 || y < 0 || x as u32 >= image.width || y as u32 >= image.height {
            return Some(false);
        }
        let p = &image.rgba[(y as usize * image.width as usize + x as usize) * 4..][..4];
        Some(match format {
            0 | 1 => p[..3].iter().any(|v| *v != 0),
            2 => p[3] != 0,
            3 => p[0] != 0,
            _ => false,
        })
    }

    pub(super) fn tick_icon_input_actions(&mut self) {
        const BITS: [u32; 24] = [
            1, 2, 4, 16, 32, 64, 128, 256, 512, 4096, 8192, 16384, 32768, 65536, 131072, 262144,
            524288, 1048576, 2097152, 4194304, 8388608, 16777216, 33554432, 0x40000000,
        ];
        const TABLES: [[i32; 24]; 4] = [
            [
                1, 2, 0, 0, 0, 0, 0, 3, 4, 7, 8, 5, 6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 19,
            ],
            [
                1, 2, 0, 0, 0, 0, 0, 3, 4, 5, 6, 7, 8, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 19,
            ],
            [
                1, 2, 0, 0, 0, 0, 0, 3, 4, 11, 12, 9, 10, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 19,
            ],
            [
                1, 2, 0, 0, 0, 0, 0, 3, 4, 9, 10, 11, 12, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 19,
            ],
        ];
        let objects = self
            .icon_input_chains
            .keys()
            .rev()
            .copied()
            .collect::<Vec<_>>();
        for object in objects {
            let chain = &self.icon_input_chains[&object];
            let (scope, vscope, full, window_input) = (
                chain.scope,
                chain.virtual_scope as i32,
                chain.full,
                chain.full || chain.pointer_window,
            );
            let Some(input) = self.graph_input_objects.get(&object) else {
                continue;
            };
            if !input.is_running() {
                continue;
            }
            let (poll, mask, mode, extended, pending) = (
                input.descriptor.pointer_processing_enabled,
                input.descriptor.flags[2] as u32,
                (input.descriptor.flags[3] & 7) as usize,
                input.extended,
                input.deferred_pointer_activation(),
            );
            let mut bits = 0;
            if poll {
                if window_input {
                    bits |= query_runtime_input_event_bits_split(
                        self,
                        if full { scope } else { 0 },
                        scope,
                        true,
                    ) as u32;
                }
                bits |= query_runtime_input_event_bits_split(self, 0, vscope, true) as u32;
                if !full {
                    bits &= 0x2C3;
                }
            }
            bits &= !mask;
            let table = if mode < 4 {
                TABLES[mode]
            } else {
                self.graph_key_assignments[mode - 4].unwrap_or([0; 24])
            };
            let mut action = BITS
                .iter()
                .position(|b| bits & *b != 0)
                .map_or(0, |index| table[index]);
            if action == 19 {
                action = if self.input_down_descriptors.contains(&16) {
                    7
                } else {
                    8
                };
            }
            let mut released = false;
            if let Some((group, item)) = pending {
                if action != 0 {
                    self.graph_input_objects
                        .get_mut(&object)
                        .unwrap()
                        .clear_deferred_pointer_activation();
                } else if !self.mouse_pressed {
                    let hit = self
                        .mouse_pos
                        .and_then(|p| self.hit_test_graph_input_object(object, p));
                    if hit.is_some_and(|(r, _, _)| (r.group, r.index) == (group, item)) {
                        action = 1;
                        released = true;
                    } else {
                        self.graph_input_objects
                            .get_mut(&object)
                            .unwrap()
                            .clear_deferred_pointer_activation();
                    }
                }
            }
            let mut refresh = false;
            if action == 1 {
                let hit = self
                    .mouse_pos
                    .and_then(|point| self.hit_test_graph_input_object(object, point));
                if let Some((region, x, y)) = hit {
                    let input = self.graph_input_objects.get_mut(&object).unwrap();
                    if released || input.pointer_activation_is_immediate(region) {
                        refresh = input.activate_native_action_item(region, (x, y), true);
                        input.clear_deferred_pointer_activation();
                    } else {
                        input.defer_pointer_activation(region);
                    }
                } else if extended {
                    let window = self.icon_input_chains[&object].window;
                    let (x, y) = self.mouse_pos.unwrap_or((0.0, 0.0));
                    let (wx, wy) = if full {
                        (0.0, 0.0)
                    } else {
                        self.surface_world_position(window)
                    };
                    self.graph_input_objects
                        .get_mut(&object)
                        .unwrap()
                        .queue_event([
                            0x10000007,
                            -1,
                            ((x - wx) as i32 & 0xFFFF) | (((y - wy) as i32) << 16),
                        ]);
                }
            } else if (2..=18).contains(&action) {
                refresh = self
                    .graph_input_objects
                    .get_mut(&object)
                    .unwrap()
                    .native_action(action);
            } else if full
                && self.selection_focus_eligible(scope as u32)
                && native_keyboard_scope_eligible(self, scope)
            {
                // Base item+38: sign bit enables held activation after its
                // native 500ms deadline. The class-bit test survives an earlier
                // destructive aggregate query.
                let regions = self.graph_input_objects[&object].descriptor.regions.clone();
                for region in regions {
                    if extended || region.enabled_depth == 0 {
                        continue;
                    }
                    let descriptor = region.flags & i32::MAX;
                    if descriptor == 0 {
                        continue;
                    }
                    let held = region.flags < 0
                        && self.input_down_descriptors.contains(&descriptor)
                        && self
                            .input_descriptor_deadlines
                            .get(&descriptor)
                            .is_some_and(|deadline| self.engine_time_ms >= *deadline);
                    let mapped = native_action_descriptor_bit(bits, descriptor);
                    if held || mapped || drain_native_input_descriptor(self, descriptor) != 0 {
                        refresh = self
                            .graph_input_objects
                            .get_mut(&object)
                            .unwrap()
                            .activate_native_action_item(region, (0, 0), false);
                        break;
                    }
                }
            }
            if refresh {
                self.refresh_graph_input_control_layers(object);
            }
        }
    }

    pub(super) fn icon_input_focus_eligible(&self, scope: u32) -> bool {
        self.icon_input_chains
            .values()
            .filter(|c| c.full)
            .all(|c| scope >= c.scope as u32)
    }

    pub(super) fn icon_input_pointer_nodes_at(
        &self,
        point: Option<(f32, f32)>,
    ) -> Vec<(u32, bool)> {
        let Some((x, y)) = point else {
            return Vec::new();
        };
        let (screen_width, screen_height) = snapshot::runtime_frame_size(self);
        if x < 0.0 || y < 0.0 || x >= screen_width as f32 || y >= screen_height as f32 {
            return Vec::new();
        }
        let mut nodes = Vec::new();
        for (&object, chain) in &self.icon_input_chains {
            if !self.graph_object_is_drawable(chain.window) {
                continue;
            }
            let (wx, wy) = self.surface_world_position(chain.window);
            for item in &chain.items {
                // The Virtual retains its configured extent and copied mask, but
                // follows its live child position after layout/movement.
                let position = self
                    .graph_layers
                    .iter()
                    .find_map(|(&id, layer)| {
                        (layer.hit_id == item.ordinal
                            && self.surface_controls.contains_layer_for_owner(
                                SurfaceControlOwner::InputObject(object),
                                id,
                            ))
                        .then(|| {
                            let (left, top, _) = self.layer_world_transform(id, layer);
                            (left as i32, top as i32)
                        })
                    })
                    .unwrap_or((wx as i32 + item.rect[0], wy as i32 + item.rect[1]));
                let px = x as i32 - position.0;
                let py = y as i32 - position.1;
                let mut hit =
                    !item.never_hit && px >= 0 && py >= 0 && px < item.rect[2] && py < item.rect[3];
                if hit && let Some((image, format)) = &item.mask {
                    hit = if px as u32 >= image.width || py as u32 >= image.height {
                        false
                    } else {
                        let index = (py as usize * image.width as usize + px as usize) * 4;
                        let p = &image.rgba[index..index + 4];
                        match format {
                            0 | 1 => p[..3].iter().any(|v| *v != 0),
                            2 => p[3] != 0,
                            3 => p[0] != 0,
                            _ => false,
                        }
                    };
                }
                nodes.push((chain.virtual_scope, hit));
            }
        }
        nodes
    }

    pub(super) fn icon_input_window_pointer_eligible(
        &self,
        object: i32,
        point: (f32, f32),
    ) -> bool {
        self.icon_input_chains.get(&object).is_none_or(|c| {
            (c.full || c.pointer_window)
                && native_pointer_scope_eligible_at(self, c.scope, Some(point))
        })
    }

    pub(super) fn icon_input_pointer_eligible(&self, object: i32, point: (f32, f32)) -> bool {
        self.icon_input_chains.get(&object).is_none_or(|chain| {
            native_pointer_scope_eligible_at(self, chain.virtual_scope as i32, Some(point))
        })
    }
}

pub(super) fn native_action_descriptor_bit(bits: u32, descriptor: i32) -> bool {
    let shift = match descriptor {
        1 => 0,
        2 => 1,
        4 => 2,
        5 => 4,
        6 => 5,
        9 => 30,
        14 => 6,
        15 => 7,
        37 => 14,
        38 => 12,
        39 => 15,
        40 => 13,
        48 | 96 => 25,
        49..=57 => 16 + descriptor - 49,
        97..=105 => 16 + descriptor - 97,
        193 => 8,
        194 => 9,
        _ => return false,
    };
    bits & (1 << shift) != 0
}

#[cfg(test)]
mod tests {
    use super::*;
    use ethornell_vm::{GraphApi, GraphInputDescriptor, GraphInputGroup, GraphInputRegion};
    #[test]
    fn object_hit_mask_is_copied_and_returns_native_bit_value() {
        let manager = ResourceManager::open_game(env!("CARGO_MANIFEST_DIR")).unwrap();
        let mut api = RuntimeTraceApi::new(manager);
        let window = api.alloc_window_surface(16, 4).unwrap();
        let mut image = DecodedImage {
            width: 9,
            height: 1,
            rgba: vec![0; 36],
        };
        image.rgba[7 * 4 + 3] = 255;
        image.rgba[8 * 4 + 3] = 255;
        api.store_graph_image("test:hit-bits".into(), image);
        api.graph_resources
            .insert(1, RuntimeGraphResource::whole("test:hit-bits".into()));
        api.bitmap_formats.insert(1, 2);
        api.configure_graph_object_hit_mask(window, 1).unwrap();
        api.graph_resources.remove(&1);
        assert_eq!(
            api.graph_object_pointer_hit_at(window, Some((7.0, 0.0))),
            Some(128)
        );
        assert_eq!(
            api.graph_object_pointer_hit_at(window, Some((8.0, 0.0))),
            Some(1)
        );
        assert_eq!(
            api.graph_object_pointer_hit_at(window, Some((9.0, 0.0))),
            Some(0)
        );
        api.configure_graph_object_hit_mask(window, -1).unwrap();
        assert_eq!(
            api.graph_object_pointer_hit_at(window, Some((9.0, 0.0))),
            Some(1)
        );
        api.configure_graph_object_hit_mask(window, -2).unwrap();
        assert_eq!(
            api.graph_object_pointer_hit_at(window, Some((7.0, 0.0))),
            Some(0)
        );
    }

    #[test]
    fn keyboard_action_table_navigates_ragged_grid_and_confirms_current_item() {
        let manager = ResourceManager::open_game(env!("CARGO_MANIFEST_DIR")).unwrap();
        let mut api = RuntimeTraceApi::new(manager);
        let window = api.alloc_window_surface(100, 60).unwrap();
        api.store_graph_image(
            "test:nav-icon".into(),
            DecodedImage {
                width: 2,
                height: 2,
                rgba: vec![255; 16],
            },
        );
        api.graph_resources
            .insert(1, RuntimeGraphResource::whole("test:nav-icon".into()));
        api.graph_input_objects
            .insert(42, RuntimeGraphInputObject::new(window));
        GraphApi::configure_graph_input_object(
            &mut api,
            42,
            GraphInputDescriptor {
                initial_group: 0,
                flags: [1, 0, 0, 2, 0, 0, 0],
                groups: vec![GraphInputGroup {
                    navigation_columns: 2,
                    index: 0,
                    initial_current_item: 0,
                    selection_enabled: true,
                    pointer_selection_enabled: false,
                    pointer_activation_enabled: false,
                    selection_exclusion_key: -1,
                    extended_flags: 0,
                }],
                regions: (0..3)
                    .map(|i| GraphInputRegion {
                        index: i,
                        ordinal: i,
                        group: 0,
                        enabled_depth: 1,
                        normal_resource: 1,
                        x: i * 10,
                        mask_resource: -1,
                        selected: false,
                        y: 0,
                        width: 0,
                        height: 0,
                        selected_resource: -1,
                        hover_resource: -1,
                        hover_selected_resource: -1,
                        current_selection_hit_excluded: false,
                        flags: 0,
                    })
                    .collect(),
                ..GraphInputDescriptor::default()
            },
        );
        apply_runtime_input_event(&mut api, RuntimeInputEvent::KeyPress { descriptor: 40 });
        api.tick_icon_input_actions();
        assert_eq!(api.graph_input_objects[&42].current_item(0), 2);
        assert_eq!(
            api.graph_input_objects.get_mut(&42).unwrap().pop_event(),
            Some([0x10000004, 2, 65536])
        );
        apply_runtime_input_event(&mut api, RuntimeInputEvent::KeyPress { descriptor: 39 });
        api.tick_icon_input_actions();
        assert_eq!(api.graph_input_objects[&42].current_item(0), 2);
        apply_runtime_input_event(&mut api, RuntimeInputEvent::KeyPress { descriptor: 13 });
        api.tick_icon_input_actions();
        assert_eq!(
            api.graph_input_objects[&42].state_record(),
            [0, 0, 2, 0, 0, 0]
        );
    }
}

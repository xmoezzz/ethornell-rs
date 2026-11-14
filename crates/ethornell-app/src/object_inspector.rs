//! E0:20 / ABD00: live CDspObj inspector fields and the ABCxx edit callbacks.
//! This uses maintained object state and normal rendering setters; native
//! addresses never cross the VM/host boundary.
use super::*;
use ethornell_vm::{NativeObjectInspector, NativeObjectInspectorProperty, Value, VmResult};
impl RuntimeTraceApi {
    pub(super) fn native_object_inspector(
        &self,
        object: i32,
    ) -> std::result::Result<NativeObjectInspector, i32> {
        if !self.graph_handle_exists(object) {
            return Err(7);
        }
        let kind = self.display_tree.kind(object);
        let name = match kind {
            Some(NativeDisplayKind::Sprite) => "Sprite",
            Some(NativeDisplayKind::Window) => "Window",
            _ => return Err(8),
        };
        let fallback = RuntimeGraphObjectProperties::default();
        let p = self
            .graph_object_properties
            .get(&object)
            .unwrap_or(&fallback);
        let n = &p.native;
        let mut fields = Vec::new();
        let mut add = |key: u8, name: &str, kind: i32, value: i32, editable: bool| {
            fields.push(NativeObjectInspectorProperty {
                name: name.into(),
                kind,
                value: Value::Int(value),
                key: Some(key),
                editable,
            })
        };
        add(0, "Display State", 4, n.draw_enabled as i32, true);
        add(1, "Visibility", 4, n.enabled as i32, true);
        add(2, "Invisibility", 4, n.suppress_draw as i32, true);
        add(3, "Priority", 2, n.priority as i32, true);
        add(4, "Position X", 0, n.position_x, true);
        add(5, "Position Y", 0, n.position_y, true);
        add(6, "Adjustment to X", 0, n.primary_offset_x, false);
        add(7, "Adjustment to Y", 0, n.primary_offset_y, false);
        add(8, "Difference of X", 0, n.secondary_offset_x, false);
        add(9, "Difference of Y", 0, n.secondary_offset_y, false);
        if name == "Sprite" {
            add(
                10,
                "Bitmap Number",
                0,
                p.format_resource.unwrap_or(-1),
                false,
            );
            if self.graph90_sprite_mode(object) == Some(5) {
                // The x64 object stores these vector banks as floats. The
                // portable engine owns 16.16 coordinates, so expose the same
                // logical float values rather than reinterpret fixed bits.
                let t = self
                    .graph91_object_transforms
                    .get(&object)
                    .copied()
                    .unwrap_or_default();
                for (i, (label, v)) in [
                    ("Logical X", n.fixed_position_x_16_16),
                    ("Logical Y", n.fixed_position_y_16_16),
                    ("Logical Z", n.fixed_position_z_16_16),
                    ("Adjustment to logical X", t.primary_vector[0]),
                    ("Adjustment to logical Y", t.primary_vector[1]),
                    ("Adjustment to logical Z", t.primary_vector[2]),
                    ("Difference of logical X", t.secondary_vector[0]),
                    ("Difference of logical Y", t.secondary_vector[1]),
                    ("Difference of logical Z", t.secondary_vector[2]),
                ]
                .into_iter()
                .enumerate()
                {
                    add(
                        15 + i as u8,
                        label,
                        3,
                        ((v as f32 / 65536.).to_bits()) as i32,
                        false,
                    );
                }
            }
        }
        add(11, "Effect Mode", 1, n.blend_mode, true);
        add(12, "Effect Level", 1, n.alpha_parameter, true);
        add(13, "Transparency", 1, n.mask_alpha, true);
        add(14, "Opacity", 1, n.alpha_multiplier, true);
        Ok(NativeObjectInspector {
            object_type: name.into(),
            properties: fields,
        })
    }
    pub(super) fn edit_native_object_inspector(
        &mut self,
        object: i32,
        key: u8,
        value: i32,
    ) -> VmResult<()> {
        use ethornell_vm::native_call::opcodes::*;
        if !self.graph_handle_exists(object) {
            return Ok(());
        }
        let (opcode, args) = match key {
            0 => (GRAPH90_SET_OBJECT_DRAW_ENABLED, vec![object, value]),
            1 => (GRAPH90_SET_OBJECT_ENABLED, vec![object, value]),
            2 => {
                self.graph91_set_object_suppressed(object, value != 0);
                return Ok(());
            }
            3 => (GRAPH90_SET_OBJECT_PRIORITY, vec![object, value]),
            4 | 5 => {
                let n = &self
                    .graph_object_properties
                    .entry(object)
                    .or_default()
                    .native;
                (
                    GRAPH90_SET_OBJECT_POSITION,
                    vec![
                        object,
                        if key == 4 { value } else { n.position_x },
                        if key == 5 { value } else { n.position_y },
                    ],
                )
            }
            11 => {
                self.apply_graph_object_property(&[
                    Value::Int(0),
                    Value::Int(value),
                    Value::Int(1),
                    Value::Int(object),
                ])?;
                return Ok(());
            }
            12 => (GRAPH_SET_OBJECT_ALPHA, vec![object, value]),
            13 => (GRAPH90_SET_OBJECT_MASK_ALPHA, vec![object, value]),
            14 => (GRAPH90_SET_OBJECT_ALPHA_MULTIPLIER, vec![object, value]),
            _ => {
                return Err(ethornell_vm::VmError::Runtime(
                    "read-only native object inspector property".into(),
                ));
            }
        };
        let mut frame =
            ethornell_vm::NativeCallFrame::new(opcode, args.into_iter().map(Value::Int).collect());
        ethornell_vm::GraphApi::call_graph(self, &mut frame)?;
        Ok(())
    }
}

#[cfg(test)]
mod tests {
    use super::*;
    #[test]
    fn version_object_inspector_edits_change_live_render_state() {
        let manager = ResourceManager::open_game(env!("CARGO_MANIFEST_DIR")).unwrap();
        let mut api = RuntimeTraceApi::new(manager);
        let mut call = ethornell_vm::NativeCallFrame::new(
            ethornell_vm::native_call::opcodes::GRAPH90_CREATE_SPRITE_OBJECT,
            vec![],
        );
        let Value::Int(object) = ethornell_vm::GraphApi::call_graph(&mut api, &mut call).unwrap()
        else {
            panic!()
        };
        assert_eq!(
            api.native_object_inspector(object).unwrap().object_type,
            "Sprite"
        );
        api.edit_native_object_inspector(object, 4, -31).unwrap();
        api.edit_native_object_inspector(object, 5, 79).unwrap();
        api.edit_native_object_inspector(object, 3, 42).unwrap();
        api.edit_native_object_inspector(object, 12, 128).unwrap();
        let snapshot = api.native_object_inspector(object).unwrap();
        let value = |key| {
            snapshot
                .properties
                .iter()
                .find(|p| p.key == Some(key))
                .unwrap()
                .value
                .as_i32()
        };
        assert_eq!(
            (value(4), value(5), value(3), value(12)),
            (-31, 79, 42, 128)
        );
        assert_eq!(
            api.graph91_object_get_property(object, 0, 0),
            Ok(vec![-31, 79])
        );
        assert_eq!(api.graph91_object_get_property(object, 3, 0), Ok(vec![42]));
        api.edit_native_object_inspector(object, 2, 1).unwrap();
        assert_eq!(api.graph_object_properties[&object].native.suppress_draw, 1);
        assert_eq!(api.native_object_inspector(-1).unwrap_err(), 7);
    }
}

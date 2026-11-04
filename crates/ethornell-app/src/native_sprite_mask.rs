//! Sprite mode 3: configure-time format-3 mask copy and live fixed parameter.
use super::*;
use ethornell_vm::GraphApi;

#[derive(Debug, Clone)]
pub(crate) struct SpriteMask {
    primary: i32,
    generation: u64,
    mask: Vec<u8>,
    width: u32,
    height: u32,
    shift: u32,
    last: Option<(u64, u32)>,
}

impl RuntimeTraceApi {
    pub(super) fn configure_sprite_mask(&mut self, args: &[i32]) -> ethornell_vm::VmResult<()> {
        // sub_47D270: priority, transparency, blend mode, fixed parameter;
        // sub_462960: either bitmap invalid (1), the mask unsuitable (2).
        target_checks::priority(args[9])?;
        target_checks::effect_level(args[8])?;
        target_checks::effect_mode(args[7])?;
        target_checks::effect_level(args[6])?;
        let either = || {
            ethornell_vm::VmError::Runtime(format!(
                "指定されたビットマップ [ {} , {} ] のいずれかが無効です",
                args[3], args[4]
            ))
        };
        let fail = |_: &str| either();
        let source = self.graph_bitmap_image(args[3]).ok_or_else(either)?;
        let mask_info = self.query_bitmap_info(args[4]).ok_or_else(either)?;
        if mask_info.format != 3 {
            return Err(ethornell_vm::VmError::Runtime(format!(
                "指定されたフィルタリング用ビットマップ [ {}  ] は不適切です",
                args[4]
            )));
        }
        let mask = self
            .graph_bitmap_image(args[4])
            .ok_or_else(|| fail("mask pixels are absent"))?;
        let mut copied = vec![0; source.width as usize * source.height as usize];
        for y in 0..source.height.min(mask.height) {
            for x in 0..source.width.min(mask.width) {
                copied[y as usize * source.width as usize + x as usize] =
                    mask.rgba[(y as usize * mask.width as usize + x as usize) * 4];
            }
        }
        let generation = self
            .graph_resources
            .get(&args[3])
            .ok_or_else(|| fail("primary binding is absent"))?
            .generation;
        self.native_sprite_masks.insert(
            args[0],
            SpriteMask {
                primary: args[3],
                generation,
                mask: copied,
                width: source.width,
                height: source.height,
                shift: args[5] as u32 & 31,
                last: None,
            },
        );
        self.refresh_sprite_mask(args[0]);
        Ok(())
    }

    pub(super) fn sprite_mask_binding_current(&self, object: i32) -> bool {
        self.native_sprite_masks.get(&object).is_none_or(|s| {
            self.graph_resources
                .get(&s.primary)
                .is_some_and(|r| r.generation == s.generation)
        })
    }

    pub(super) fn refresh_sprite_mask(&mut self, object: i32) -> bool {
        let Some(state) = self.native_sprite_masks.get(&object) else {
            return false;
        };
        if !self.sprite_mask_binding_current(object) {
            self.graph_layers.remove(&object);
            return false;
        }
        let Some(properties) = self.graph_object_properties.get(&object) else {
            return false;
        };
        let threshold = properties.native.fixed_parameter_16_16 as u32 >> 16;
        let resource = &self.graph_resources[&state.primary];
        let revision = *self.graph_image_revisions.get(&resource.key).unwrap_or(&0);
        if state.last == Some((revision, threshold)) {
            return true;
        }
        let mut image = match self.graph_bitmap_image(state.primary) {
            Some(i) => i,
            None => return false,
        };
        let state = self.native_sprite_masks.get(&object).unwrap();
        let rgb = self.bitmap_formats.get(&state.primary) == Some(&1);
        let scale = 1i32.wrapping_shl(state.shift).wrapping_add(1);
        let limit = (threshold as i32).wrapping_mul(scale);
        for y in 0..image.height {
            for x in 0..image.width {
                let mask = if x < state.width && y < state.height {
                    state.mask[y as usize * state.width as usize + x as usize]
                } else {
                    0
                };
                let coverage = if threshold >= 256 {
                    256
                } else {
                    limit
                        .wrapping_sub(i32::from(mask).wrapping_shl(state.shift))
                        .clamp(0, 256)
                };
                let i = (y as usize * image.width as usize + x as usize) * 4;
                image.rgba[i + 3] = if rgb {
                    coverage.min(255) as u8
                } else {
                    (u32::from(image.rgba[i + 3]) * coverage as u32 >> 8) as u8
                };
            }
        }
        let key = format!("runtime:sprite-mask:{object}");
        self.store_graph_image(key.clone(), image);
        if let Some(layer) = self.graph_layers.get_mut(&object) {
            layer.key = key;
            layer.src_x = 0.0;
            layer.src_y = 0.0;
        }
        self.native_sprite_masks.get_mut(&object).unwrap().last = Some((revision, threshold));
        self.graph_redraw_requested = Some(false);
        true
    }
}

#[cfg(test)]
mod tests {
    use super::*;
    #[test]
    fn mode3_copies_mask_and_rebuilds_from_fixed_parameter() {
        let manager=ResourceManager::open_game(env!("CARGO_MANIFEST_DIR")).unwrap();
        let mut api=RuntimeTraceApi::new(manager);
        for (bitmap,key,format,rgba) in [(1,"test:mask-primary",1,vec![100,20,30,255,80,60,40,255]),(2,"test:mask-source",3,vec![0,0,0,255,255,255,255,255])] {
            api.store_graph_image(key.into(),DecodedImage{width:2,height:1,rgba});
            api.graph_resources.insert(bitmap,RuntimeGraphResource::whole(key.into()));api.bitmap_formats.insert(bitmap,format);
        }
        let mut call=ethornell_vm::NativeCallFrame::new(ethornell_vm::NativeOpcode{group:0x90,id:0x50},vec![]);
        let object=GraphApi::call_graph(&mut api,&mut call).unwrap().as_i32();
        let mut call=ethornell_vm::NativeCallFrame::new(ethornell_vm::NativeOpcode{group:0x90,id:0x5A},[object,0,0,1,2,0,128,1,0,0].into_iter().map(ethornell_vm::Value::Int).collect());
        GraphApi::call_graph(&mut api,&mut call).unwrap();
        let key=api.graph_layers[&object].key.clone();
        assert_eq!(api.graph_images[&key].rgba[3],255);assert_eq!(api.graph_images[&key].rgba[7],1);
        api.graph_resources.remove(&2);
        api.graph90_set_fixed_parameter_raw_recursive(object,256<<16);
        assert_eq!(api.graph_images[&key].rgba[7],255);
        assert!(api.sprite_mask_binding_current(object));
    }
}

use super::{
    DecodedImage, RuntimeGraphResource, RuntimeSurface, RuntimeTraceApi, blit_decoded_image,
};

// Indexed by the native bitmap format stored in the target's registry entry.
pub(crate) const TARGET_BITMAP_BYTES_PER_PIXEL: [u32; 8] = [2, 4, 4, 1, 4, 4, 6, 4];

pub(crate) fn target_bitmap_bytes_per_pixel(format: i32) -> Option<u32> {
    usize::try_from(format)
        .ok()
        .and_then(|format| TARGET_BITMAP_BYTES_PER_PIXEL.get(format))
        .copied()
}

#[cfg(test)]
mod tests {
    use super::target_bitmap_bytes_per_pixel;

    #[test]
    fn target_bitmap_format_table_preserves_native_pixel_sizes() {
        let sizes = (0..=7)
            .map(target_bitmap_bytes_per_pixel)
            .collect::<Vec<_>>();
        assert_eq!(
            sizes,
            vec![
                Some(2),
                Some(4),
                Some(4),
                Some(1),
                Some(4),
                Some(4),
                Some(6),
                Some(4)
            ]
        );
        assert_eq!(target_bitmap_bytes_per_pixel(-1), None);
        assert_eq!(target_bitmap_bytes_per_pixel(8), None);
    }
}

#[derive(Debug, Clone)]
pub(crate) enum RuntimeBitmapNodeLayout {
    Configure(Vec<i32>),
    Image { x: f32, y: f32, z: i32 },
}

#[derive(Debug, Clone)]
pub(crate) struct RuntimeBitmapNodeBinding {
    pub(crate) bitmap: i32,
    pub(crate) owner_object: Option<i32>,
    pub(crate) layout: RuntimeBitmapNodeLayout,
}

impl RuntimeTraceApi {
    /// `Graph90:11` reaches `sub_407DA0`, which releases an existing bitmap
    /// slot through `sub_407CF0` before allocating the replacement storage.
    /// Keep this replacement operation centralized so stale resource bindings
    /// cannot survive a same-handle bitmap creation.
    pub(crate) fn recreate_native_bitmap(
        &mut self,
        bitmap: i32,
        width: u32,
        height: u32,
        format: i32,
    ) {
        self.clear_bitmap_text(bitmap);
        self.remove_surface_control_layers(bitmap);
        self.surface_text_states.remove(&bitmap);
        self.surface_text_buffers.remove(&bitmap);
        self.detach_graph_surface_relations(bitmap);
        self.effects.remove_bitmap(bitmap);
        self.graph_resources.remove(&bitmap);
        self.graph_bindings.remove(&bitmap);
        self.graph_config.bitmap_priorities.remove(&bitmap);
        self.reset_bitmap_auxiliary_pair(bitmap);

        let key = format!("runtime:bitmap:{bitmap}");
        self.graph_images.remove(&key);
        self.graph_image_revisions.remove(&key);

        self.bitmap_dimensions.insert(bitmap, (width, height));
        self.bitmap_formats.insert(bitmap, format);
        let mut surface = RuntimeSurface::bitmap(bitmap, width as f32, height as f32);
        surface.resource_id = Some(bitmap);
        self.graph_surfaces.insert(bitmap, surface);

        self.store_graph_image(
            key.clone(),
            DecodedImage {
                width,
                height,
                rgba: vec![0; width as usize * height as usize * 4],
            },
        );
        self.graph_resources
            .insert(bitmap, RuntimeGraphResource::whole(key));
        self.refresh_bitmap_nodes(bitmap);
    }

    pub(crate) fn validate_native_bitmap_blit(&mut self, destination: i32, source: i32) -> i32 {
        let Some(destination_info) = ethornell_vm::GraphApi::query_bitmap_info(self, destination)
        else {
            return 1;
        };
        let Some(source_info) = ethornell_vm::GraphApi::query_bitmap_info(self, source) else {
            return 2;
        };
        let compatible = destination_info.format == source_info.format
            || matches!(
                (destination_info.format, source_info.format),
                (1, 2) | (2, 1)
            );
        if compatible { 0 } else { 3 }
    }

    /// `sub_4033A0` allocates a detached destination using the source format,
    /// then clips a mode-128 copy shifted by `(-x, -y)` into that canvas.
    pub(crate) fn create_native_bitmap_region(
        &mut self,
        destination: i32,
        source: i32,
        x: i32,
        y: i32,
        width: i32,
        height: i32,
    ) -> i32 {
        if !(0..0x4000).contains(&destination) {
            return 1;
        }
        let Some(source_info) = ethornell_vm::GraphApi::query_bitmap_info(self, source) else {
            return 2;
        };
        if width == 0 || height == 0 {
            return 3;
        }
        let (Ok(width), Ok(height)) = (u32::try_from(width), u32::try_from(height)) else {
            return 1;
        };
        let Some(pixel_count) = usize::try_from(width)
            .ok()
            .and_then(|width| {
                usize::try_from(height)
                    .ok()
                    .and_then(|height| width.checked_mul(height))
            })
            .and_then(|pixels| pixels.checked_mul(4))
        else {
            return 1;
        };
        let Some(source_image) = self.graph_bitmap_image(source) else {
            return 2;
        };
        let mut rgba = Vec::new();
        if rgba.try_reserve_exact(pixel_count).is_err() {
            return 1;
        }
        rgba.resize(pixel_count, 0);
        let mut destination_image = DecodedImage {
            width,
            height,
            rgba,
        };
        // sub_4033A0 creates the destination in the source's format and
        // copies with mode 0x80 at (-x, -y): a raw row copy (sub_40ADF0).
        let format = source_info.format as i32;
        crate::bitmap_blend::blit_copy(&mut destination_image, format, &source_image, format, -x, -y);

        self.recreate_native_bitmap(destination, width, height, format);
        // Formats 4 and 6 keep their samples in the effect maps; the raw copy
        // moves those samples as well.
        match format {
            4 => self.effects.create_displacement_map(destination, width, height),
            6 => self.effects.create_vector_map(destination, width, height),
            _ => {}
        }
        if matches!(format, 4 | 6) {
            self.effects.copy_vector_map(destination, source, -x, -y);
        }
        let key = format!("runtime:bitmap:{destination}");
        self.store_graph_image(key.clone(), destination_image);
        self.graph_resources
            .insert(destination, RuntimeGraphResource::whole(key));
        self.copy_bitmap_text_region(source, destination, x, y, width as i32, height as i32);
        self.refresh_bitmap_nodes(destination);
        0
    }

    pub(crate) fn bind_configured_bitmap_node(
        &mut self,
        node: i32,
        bitmap: i32,
        values: Vec<i32>,
    ) -> bool {
        self.graph_bitmap_node_bindings.insert(
            node,
            RuntimeBitmapNodeBinding {
                bitmap,
                owner_object: self.current_graph_object,
                layout: RuntimeBitmapNodeLayout::Configure(values),
            },
        );
        self.refresh_bound_bitmap_node(node)
    }

    pub(crate) fn bind_image_bitmap_node(
        &mut self,
        node: i32,
        bitmap: i32,
        x: f32,
        y: f32,
        z: i32,
    ) -> bool {
        self.graph_bitmap_node_bindings.insert(
            node,
            RuntimeBitmapNodeBinding {
                bitmap,
                owner_object: self.current_graph_object,
                layout: RuntimeBitmapNodeLayout::Image { x, y, z },
            },
        );
        self.refresh_bound_bitmap_node(node)
    }

    pub(crate) fn remove_bitmap_node_binding(&mut self, node: i32) {
        self.graph_bitmap_node_bindings.remove(&node);
        self.graph_node_enabled.remove(&node);
    }

    pub(crate) fn refresh_bitmap_nodes(&mut self, bitmap: i32) {
        let nodes = self
            .graph_bitmap_node_bindings
            .iter()
            .filter_map(|(&node, binding)| (binding.bitmap == bitmap).then_some(node))
            .collect::<Vec<_>>();
        for node in nodes {
            self.refresh_bound_bitmap_node(node);
        }
    }

    fn refresh_bound_bitmap_node(&mut self, node: i32) -> bool {
        let Some(binding) = self.graph_bitmap_node_bindings.get(&node).cloned() else {
            return false;
        };
        let enabled = self.graph_node_enabled.get(&node).copied().unwrap_or(true);
        let configured = match binding.layout {
            RuntimeBitmapNodeLayout::Configure(values) => {
                self.configure_bitmap_text_node(node, binding.bitmap, &values, binding.owner_object)
            }
            RuntimeBitmapNodeLayout::Image { x, y, z } => self.configure_image_bitmap_text_node(
                node,
                binding.bitmap,
                x,
                y,
                z,
                binding.owner_object,
            ),
        };
        self.set_screen_text_node_enabled(node, enabled);
        if configured {
            self.trace_graph(format!(
                "refresh bitmap node #{node} source=#{} owner={:?} enabled={enabled}",
                binding.bitmap, binding.owner_object
            ));
        }
        configured
    }
}

//! Graph91:55 linked-sprite masks. Yorikure/BGI 1.644.2:
//! 48A210 -> 445800 -> 42F260/42F320 installs reciprocal links;
//! 42BD70 suppresses the mask's ordinary draw through +0x158. 430AB0
//! aligns the mask and picture in display coordinates; 417F50 combines
//! their alpha with the mask object's effective opacity, before RGB filters.
use super::*;

#[derive(Clone, Copy, PartialEq, Eq)]
enum LinkedMaskDraw {
    Independent,
    Masked,
    Suppressed,
}

#[derive(Debug, Clone)]
pub(crate) struct LinkedSpriteDraw {
    key: String,
    picture: RuntimeGraphDrawItem,
    mask: RuntimeGraphDrawItem,
    revisions: (u64, u64),
    origin: (f32, f32),
    dimensions: (u32, u32),
}

impl RuntimeTraceApi {
    pub(super) fn discard_linked_sprite_draw(&mut self, sprite: i32) {
        if let Some(draw) = self.linked_sprite_draws.remove(&sprite) {
            self.graph_images.remove(&draw.key);
            self.graph_image_revisions.remove(&draw.key);
        }
    }

    /// Both destructor directions notify the peer (42BCE0 -> 42F260/42F320).
    pub(super) fn release_sprite_relations(&mut self, object: i32) {
        let owners = self
            .graph91_sprite_relations
            .iter()
            .filter_map(|(&picture, &mask)| {
                (picture == object || mask == object).then_some(picture)
            })
            .collect::<Vec<_>>();
        for owner in owners {
            self.graph91_sprite_relations.remove(&owner);
            self.graph91_sprite_link_masks.remove(&owner);
            self.discard_linked_sprite_draw(owner);
        }
        self.graph91_sprite_link_masks.remove(&object);
        self.discard_linked_sprite_draw(object);
    }

    fn linked_sprite_mask_draw(&self, sprite: i32) -> LinkedMaskDraw {
        if !self.graph91_sprite_link_masks.contains(&sprite)
            || !self.graph91_sprite_relations.contains_key(&sprite)
        {
            return LinkedMaskDraw::Independent;
        }
        // 42BD70: only 0/1/5/6 consume the linked pointer. Mode 2
        // degenerates to 0 for an identity rotation/stretch; its general
        // path only accepts the auxiliary bitmap cleared by Graph91:55,
        // so it draws nothing with an active link. Modes 3 and 4 keep
        // their independent threshold/vector drawing, even while linked.
        match self.graph90_sprite_mode(sprite) {
            Some(0 | 1 | 5 | 6) => LinkedMaskDraw::Masked,
            Some(2) => {
                if self.graph_layers.get(&sprite).is_some_and(|layer| {
                    layer.rotation_degrees == 0.0
                        && layer.scale_x == 1.0
                        && layer.scale_y == 1.0
                }) {
                    LinkedMaskDraw::Masked
                } else {
                    LinkedMaskDraw::Suppressed
                }
            }
            _ => LinkedMaskDraw::Independent,
        }
    }

    pub(super) fn refresh_linked_sprite_masks(&mut self) {
        if self.graph91_sprite_relations.is_empty() && self.linked_sprite_draws.is_empty() {
            return;
        }
        // This list includes hidden/transparent mask descriptors, without
        // publishing them as ordinary scene draws. No recursive mask draws.
        let items = self
            .graph_draw_items_with_link_masks(true)
            .into_iter()
            .filter_map(|item| item.owner_object.map(|owner| (owner, item)))
            .collect::<BTreeMap<_, _>>();
        let relations = self.graph91_sprite_relations.clone();
        let mut previous = std::mem::take(&mut self.linked_sprite_draws);
        for (picture_id, mask_id) in relations {
            if self.linked_sprite_mask_draw(picture_id) != LinkedMaskDraw::Masked {
                continue;
            }
            let (Some(picture), Some(mask)) = (items.get(&picture_id), items.get(&mask_id)) else {
                continue;
            };
            let Some(mask) = self.linked_mask_input(mask_id, mask) else {
                continue;
            };
            let revisions = (
                self.graph_image_revision(&picture.key),
                self.graph_image_revision(&mask.key),
            );
            let mut picture_signature = pixel_signature(picture);
            // Picture opacity/blending is applied at final presentation. A
            // fade must not rebuild an otherwise unchanged full-size mask.
            picture_signature.opacity = 1.0;
            let mask_signature = pixel_signature(&mask);
            if let Some(old) = previous.remove(&picture_id) {
                if old.picture == picture_signature
                    && old.mask == mask_signature
                    && old.revisions == revisions
                    && self.graph_images.contains_key(&old.key)
                {
                    self.linked_sprite_draws.insert(picture_id, old);
                    continue;
                }
                self.graph_images.remove(&old.key);
                self.graph_image_revisions.remove(&old.key);
            }
            let (Some(source), Some(mask_source)) = (
                self.graph_images.get(&picture.key),
                self.graph_images.get(&mask.key),
            ) else {
                continue;
            };
            let (Some((mut image, x, y)), Some((mask_pixels, mx, my))) = (
                native_sprite_filters::rasterize_source(source, picture),
                native_sprite_filters::rasterize_source(mask_source, &mask),
            ) else {
                snapshot::record_raster_error(
                    "linked sprite mask raster exceeds the image budget or has invalid geometry",
                );
                continue;
            };
            let weight = (mask.opacity.clamp(0.0, 1.0) * 256.0).round() as u32;
            for py in 0..image.height {
                for px in 0..image.width {
                    let world = (x + px as f32 + 0.5, y + py as f32 + 0.5);
                    let (sx, sy) = ((world.0 - mx).floor() as i32, (world.1 - my).floor() as i32);
                    let clipped = mask.clip.is_some_and(|c| {
                        world.0 < c.x
                            || world.1 < c.y
                            || world.0 >= c.x + c.width
                            || world.1 >= c.y + c.height
                    });
                    let alpha = if !clipped
                        && sx >= 0
                        && sy >= 0
                        && sx < mask_pixels.width as i32
                        && sy < mask_pixels.height as i32
                    {
                        u32::from(
                            mask_pixels.rgba
                                [((sy as u32 * mask_pixels.width + sx as u32) * 4 + 3) as usize],
                        )
                    } else {
                        0
                    };
                    let at = ((py * image.width + px) * 4 + 3) as usize;
                    // 4182C0: XRGB picture. 418160: ARGB picture, including
                    // native /65536 rounding; preserve RGB, never premultiply.
                    image.rgba[at] = if picture.ignore_source_alpha {
                        ((alpha * weight) >> 8).min(255) as u8
                    } else {
                        ((u32::from(image.rgba[at]) * alpha * weight) >> 16) as u8
                    };
                }
            }
            let key = format!("runtime:sprite-link:{picture_id}");
            let dimensions = (image.width, image.height);
            self.store_graph_image(key.clone(), image);
            tracing::debug!(
                picture = picture_id,
                mask = mask_id,
                width = dimensions.0,
                height = dimensions.1,
                "rebuilt linked sprite mask"
            );
            self.linked_sprite_draws.insert(
                picture_id,
                LinkedSpriteDraw {
                    key,
                    picture: picture_signature,
                    mask: mask_signature,
                    revisions,
                    origin: (x, y),
                    dimensions,
                },
            );
        }
        for (_, old) in previous {
            self.graph_images.remove(&old.key);
            self.graph_image_revisions.remove(&old.key);
        }
    }

    /// 430AB0 reads the peer's primary bitmap, display position and valid
    /// rectangle. It blits that bitmap with 40A8A0; it does not recursively
    /// draw the peer's own mask, colour filters or transformed picture.
    fn linked_mask_input(
        &self,
        object: i32,
        draw: &RuntimeGraphDrawItem,
    ) -> Option<RuntimeGraphDrawItem> {
        if !self.projected_bindings_current(object) {
            return None;
        }
        let properties = self.graph_object_properties.get(&object)?;
        let bitmap = properties.format_resource?;
        let (key, region) = self.resource_image_region(bitmap)?;
        let mut mask = draw.clone();
        // 421A10 reads the peer's native transparency, not its visibility
        // gate or a second copy of an ancestor/layer opacity multiplier.
        mask.opacity = (256 - properties.transparency_parameter()).clamp(0, 256) as f32 / 256.0;
        let bounds = if let Some(quad) = native_sprite_filters::item_quad(draw) {
            let left = quad.iter().map(|p| p[0]).fold(f32::INFINITY, f32::min);
            let top = quad.iter().map(|p| p[1]).fold(f32::INFINITY, f32::min);
            let right = quad.iter().map(|p| p[0]).fold(f32::NEG_INFINITY, f32::max);
            let bottom = quad.iter().map(|p| p[1]).fold(f32::NEG_INFINITY, f32::max);
            RuntimeClipRect {
                x: left,
                y: top,
                width: right - left,
                height: bottom - top,
            }
        } else {
            RuntimeClipRect {
                x: draw.x,
                y: draw.y,
                width: draw.width,
                height: draw.height,
            }
        };
        mask.key = key.to_string();
        mask.width = region.width;
        mask.height = region.height;
        mask.src_x = region.x;
        mask.src_y = region.y;
        mask.src_width = region.width;
        mask.src_height = region.height;
        mask.rotation_degrees = 0.0;
        mask.destination_quad = None;
        mask.linear_sampling = false;
        mask.ignore_source_alpha = self.bitmap_formats.get(&bitmap) == Some(&1);
        mask.clip = Some(draw.clip.map_or(bounds, |clip| clip.intersection(bounds)));
        Some(mask)
    }

    pub(super) fn apply_linked_sprite_draw(&self, item: &mut RuntimeGraphDrawItem) -> bool {
        let Some(owner) = item.owner_object else {
            return true;
        };
        // The reciprocal link suppresses the mask even if its owner's mask
        // enable bit was subsequently cleared by Graph90:55(-1).
        if self
            .graph91_sprite_relations
            .values()
            .any(|&mask| mask == owner)
        {
            return false;
        }
        match self.linked_sprite_mask_draw(owner) {
            LinkedMaskDraw::Independent => return true,
            LinkedMaskDraw::Suppressed => return false,
            LinkedMaskDraw::Masked => {}
        }
        let Some(draw) = self.linked_sprite_draws.get(&owner) else {
            return false;
        };
        item.key = draw.key.clone();
        (item.x, item.y) = draw.origin;
        item.width = draw.dimensions.0 as f32;
        item.height = draw.dimensions.1 as f32;
        item.src_x = 0.0;
        item.src_y = 0.0;
        item.src_width = item.width;
        item.src_height = item.height;
        item.ignore_source_alpha = false;
        item.rotation_degrees = 0.0;
        item.destination_quad = None;
        item.linear_sampling = false;
        true
    }
}

/// Sort/input fields do not affect raster pixels. Mask opacity does; picture
/// opacity stays in the final command rather than in the cached texture.
fn pixel_signature(item: &RuntimeGraphDrawItem) -> RuntimeGraphDrawItem {
    let mut signature = item.clone();
    signature.blend_mode = 0x80;
    signature.z = 0;
    signature.order_serial = 0;
    signature.hit_id = 0;
    signature
}

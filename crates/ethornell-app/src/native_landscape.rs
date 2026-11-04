//! CDspObjLandscape copied assets, composed columns and priority-sliced drawing.
use super::*;
#[derive(Debug, Clone)]
struct Part {
    image: DecodedImage,
    format: i32,
}
#[derive(Debug, Clone)]
struct Column {
    image: DecodedImage,
    format: i32,
    offset: i32,
    occluding: i32,
    boundary: i32,
    extra: i32,
    hit: Option<Vec<u8>>,
}
#[derive(Debug, Clone)]
struct CellGeometry {
    column: usize,
    x: i32,
    y: i32,
    height: u32,
    depth: u32,
}
#[derive(Debug, Clone, Default)]
pub(crate) struct LandscapeRaster {
    parts: Vec<Part>,
    columns: Vec<Column>,
    guides: Vec<Part>,
    cells: Vec<Option<CellGeometry>>,
    layers: BTreeMap<u32, i32>,
}
pub(super) const LANDSCAPE_HANDLE: &str = "無効なランドスケープハンドルが指定されました";
pub(super) const LANDSCAPE_NO_MAP: &str =
    "指定されたランドスケープオブジェクトにはマップが設定されていません";
pub(super) const LANDSCAPE_NO_COLUMNS: &str =
    "指定されたランドスケープオブジェクトには柱が設定されていません";
pub(super) fn landscape_error(text: impl Into<String>) -> ethornell_vm::VmError {
    ethornell_vm::VmError::Runtime(text.into())
}
/// sub_4090A0 against sub_409180's [0, 0, w-1, h-1]: unsigned compares of
/// the inclusive [x, y, x+w-1, y+h-1] record, after the zero width/height
/// stop of sub_421C60 / sub_422800.
fn part_record_fits(source: &DecodedImage, r: [i32; 5]) -> bool {
    let (x, y, w, h) = (r[0] as u32, r[1] as u32, r[2] as u32, r[3] as u32);
    w != 0
        && h != 0
        && x.wrapping_add(w).wrapping_sub(1) <= source.width.wrapping_sub(1)
        && y.wrapping_add(h).wrapping_sub(1) <= source.height.wrapping_sub(1)
}
/// sub_422C50 tests byte +3 of a 32-bit pixel and the byte of an 8-bit
/// one; any other format never hits.
fn coverage(format: i32, pixel: &[u8; 4]) -> u8 {
    match format {
        2 => pixel[3],
        3 => pixel[0],
        _ => 0,
    }
}
fn crop_part(source: &DecodedImage, format: i32, r: [i32; 5]) -> Option<Part> {
    let (x, y, w, h) = (r[0], r[1], r[2], r[3]);
    if w <= 0
        || h <= 0
        || i64::from(w) * i64::from(h) > 16 * 1024 * 1024
        || i64::from(x) >= i64::from(source.width)
        || i64::from(y) >= i64::from(source.height)
        || i64::from(x) + i64::from(w) <= 0
        || i64::from(y) + i64::from(h) <= 0
    {
        return None;
    }
    let mut image = crate::native_map_filter::blank(w as u32, h as u32);
    crate::bitmap_blend::blit_mode(&mut image, format, source, format, -x, -y, 0x80, 0);
    Some(Part { image, format })
}
fn compose_columns(
    parts: &[Part],
    records: &[Vec<i32>],
    flags: &[[i32; 5]],
    cell_width: i32,
    step: i32,
    spacing: i32,
) -> Option<Vec<Column>> {
    let mut result = Vec::new();
    for words in records {
        let count = words[0] as u32;
        let boundary = words[33] as u32;
        if count == 0 || count > 32 || boundary == 0 || boundary > 32 {
            return None;
        }
        let mut width = 0;
        let mut height = 0;
        for i in 0..count as usize {
            let id = words[1 + i];
            if id == -1 {
                continue;
            }
            let part = parts.get(id as usize)?;
            width = width.max(part.image.width);
            height = height.max((part.image.height as i32).wrapping_add(i as i32 * step) as u32);
        }
        if width == 0 || height == 0 || u64::from(width) * u64::from(height) > 16 * 1024 * 1024 {
            return None;
        }
        let format = parts.first()?.format;
        let mut image = crate::native_map_filter::blank(width, height);
        let mut hit = None;
        let mut extra = 0;
        let mut occluding = 0;
        let mut counting = true;
        for i in 0..count as usize {
            let id = words[1 + i];
            if id == -1 {
                counting = false;
                continue;
            }
            let part = &parts[id as usize];
            crate::bitmap_blend::blit_mode(
                &mut image,
                format,
                &part.image,
                part.format,
                ((width - part.image.width) / 2) as i32,
                height as i32 - i as i32 * step - part.image.height as i32,
                1,
                0,
            );
            if counting {
                if flags[id as usize][4] != 0 {
                    counting = false;
                } else if i + 1 == boundary as usize {
                    occluding += spacing;
                    counting = false;
                } else {
                    occluding += 1;
                }
            }
            if i + 1 == boundary as usize && i + 1 < count as usize {
                hit = Some(
                    image
                        .rgba
                        .as_chunks::<4>()
                        .0
                        .iter()
                        .map(|p| coverage(format, p))
                        .collect(),
                );
                extra = (height as i32 / step - boundary as i32 - 2).max(1);
            }
        }
        let offset = cell_width.wrapping_sub(width as i32) >> 1;
        result.push(Column {
            image,
            format,
            offset,
            occluding: if offset > 0 { 0 } else { occluding * step },
            boundary: boundary as i32,
            extra,
            hit,
        });
    }
    Some(result)
}
impl LandscapeRaster {
    pub(super) fn layer_ids(&self) -> Vec<i32> {
        self.layers.values().copied().collect()
    }
    pub(super) fn cell_visible_height(&self, index: usize) -> Option<u32> {
        Some(self.cells.get(index)?.as_ref()?.height)
    }
}
impl RuntimeTraceApi {
    /// Records a failed part/pillar index (sub_41C3F0) and returns the text
    /// sub_483C20 / sub_484060 format from it.
    fn landscape_detail_error(&mut self, object: i32, detail: i32, text: &str) -> ethornell_vm::VmError {
        if let Some(state) = self.graph91_landscapes.get_mut(&object) {
            state.error_detail = detail;
        }
        landscape_error(format!("{text} [ {detail} ] に無効な情報が指定されています"))
    }
    /// sub_483C20 -> sub_4403C0 -> sub_421C60: zero part count (2) and zero
    /// pillar count (3) report index -1, then the bitmap (1), each part
    /// record (2) and each pillar record (3).
    pub(super) fn configure_native_landscape_parts(
        &mut self,
        object: i32,
        bitmap: i32,
        count: i32,
        words: &[i32],
        spacing: i32,
        column_count: i32,
        column_words: &[i32],
    ) -> ethornell_vm::VmResult<()> {
        let Some(state) = self.graph91_landscapes.get(&object).cloned() else {
            return Err(landscape_error(LANDSCAPE_HANDLE));
        };
        if count == 0 {
            return Err(self.landscape_detail_error(object, -1, "部品番号"));
        }
        if column_count == 0 {
            return Err(self.landscape_detail_error(object, -1, "柱番号"));
        }
        let Some(source) = self.graph_bitmap_image(bitmap) else {
            return Err(landscape_error(format!(
                "指定されたビットマップ [ {bitmap} ] は存在しません"
            )));
        };
        let format = self.bitmap_formats.get(&bitmap).copied().unwrap_or(2);
        let flags = words.as_chunks::<5>().0.to_vec();
        let mut parts = Vec::new();
        for (index, &record) in flags.iter().enumerate() {
            let part = part_record_fits(&source, record)
                .then(|| crop_part(&source, format, record))
                .flatten();
            let Some(part) = part else {
                return Err(self.landscape_detail_error(object, index as i32, "部品番号"));
            };
            parts.push(part);
        }
        let records = column_words
            .chunks_exact(34)
            .map(|c| c.to_vec())
            .collect::<Vec<_>>();
        for (index, words) in records.iter().enumerate() {
            let pieces = words[0] as u32;
            let boundary = words[33] as u32;
            if pieces == 0
                || pieces > 32
                || boundary == 0
                || boundary > 32
                || words[1..=pieces as usize]
                    .iter()
                    .any(|&id| id as u32 >= count as u32 && id != -1)
            {
                return Err(self.landscape_detail_error(object, index as i32, "柱番号"));
            }
        }
        // A pillar of only empty slots composes to nothing; the target then
        // allocates a zero-sized bitmap, which this raster cannot hold.
        let Some(columns) = compose_columns(
            &parts,
            &records,
            &flags,
            state.cell_width,
            state.column_step,
            spacing,
        ) else {
            return Err(self.landscape_detail_error(object, 0, "柱番号"));
        };
        let state = self.graph91_landscapes.get_mut(&object).unwrap();
        state.source_bitmap = bitmap;
        state.part_spacing = spacing;
        state.parts = flags;
        state.columns = records;
        // Native success releases the old map together with the old columns.
        state.cells.clear();
        state.map_width = 0;
        state.map_height = 0;
        let raster = self.native_landscapes.entry(object).or_default();
        raster.parts = parts;
        raster.columns = columns;
        raster.cells.clear();
        self.refresh_native_landscape(object);
        Ok(())
    }
    /// sub_484060 -> sub_440530 -> sub_422800: zero count (2, index -1),
    /// bitmap (1), then each guide record (2).
    pub(super) fn configure_native_landscape_guides(
        &mut self,
        object: i32,
        bitmap: i32,
        count: i32,
        words: &[i32],
    ) -> ethornell_vm::VmResult<()> {
        if !self.graph91_landscapes.contains_key(&object) {
            return Err(landscape_error(LANDSCAPE_HANDLE));
        }
        if count == 0 {
            return Err(self.landscape_detail_error(object, -1, "部品番号"));
        }
        let Some(source) = self.graph_bitmap_image(bitmap) else {
            return Err(landscape_error(format!(
                "指定されたビットマップ [ {bitmap} ] は存在しません"
            )));
        };
        let format = self.bitmap_formats.get(&bitmap).copied().unwrap_or(2);
        let mut guides = Vec::new();
        for (index, &record) in words.as_chunks::<5>().0.iter().enumerate() {
            let part = part_record_fits(&source, record)
                .then(|| crop_part(&source, format, record))
                .flatten();
            let Some(part) = part else {
                return Err(self.landscape_detail_error(object, index as i32, "部品番号"));
            };
            guides.push(part);
        }
        self.native_landscapes.entry(object).or_default().guides = guides;
        let state = self.graph91_landscapes.get_mut(&object).unwrap();
        state.guide_bitmap = bitmap;
        state.guides = words.as_chunks::<5>().0.to_vec();
        // 423780 replaces assets but leaves stored cell guide indices intact.
        self.refresh_native_landscape(object);
        Ok(())
    }
    /// sub_484390 -> sub_440690 -> sub_422380: pillars (4), destination
    /// part (8), source part or incompatible image (9).
    pub(super) fn copy_native_landscape_part(
        &mut self,
        object: i32,
        destination: i32,
        source: i32,
    ) -> ethornell_vm::VmResult<()> {
        let Some(state) = self.graph91_landscapes.get(&object) else {
            return Err(landscape_error(LANDSCAPE_HANDLE));
        };
        if state.columns.is_empty() {
            return Err(landscape_error(LANDSCAPE_NO_COLUMNS));
        }
        let incompatible = || {
            landscape_error(format!(
                "指定された転送元部品 [ {source} ] のイメージは転送先部品 [ {destination} ] のイメージと互換性がありません"
            ))
        };
        let raster = self.native_landscapes.entry(object).or_default();
        if destination as u32 >= raster.parts.len() as u32 {
            return Err(landscape_error(format!(
                "無効な部品番号 [ {destination} ] が指定されました"
            )));
        }
        let Some(part) = raster.parts.get(source as usize).cloned() else {
            return Err(incompatible());
        };
        let dest = &mut raster.parts[destination as usize];
        if (part.image.width, part.image.height, part.format)
            != (dest.image.width, dest.image.height, dest.format)
        {
            return Err(incompatible());
        }
        *dest = part;
        let state = &self.graph91_landscapes[&object];
        let Some(columns) = compose_columns(
            &raster.parts,
            &state.columns,
            &state.parts,
            state.cell_width,
            state.column_step,
            state.part_spacing,
        ) else {
            return Ok(());
        };
        // 423AB0 only rebuilds color pixels. Boundary alpha snapshots and
        // column geometry remain the configure-time copies.
        for (old, new) in raster.columns.iter_mut().zip(columns) {
            old.image = new.image;
        }
        self.refresh_native_landscape(object);
        Ok(())
    }
    /// sub_4849A0 -> sub_440840 -> sub_422AF0: cell checks, an empty cell
    /// returns false (14), then sub_407D90 allocates the destination (15).
    pub(super) fn copy_native_landscape_cell(
        &mut self,
        destination: i32,
        object: i32,
        row: i32,
        column: i32,
    ) -> ethornell_vm::VmResult<bool> {
        let index = self.graph91_landscape_cell(object, row, column)?;
        let state = &self.graph91_landscapes[&object];
        let id = state.cells[index].column_index;
        if id == -1 {
            return Ok(false);
        }
        if destination as u32 >= 0x4000 {
            return Err(landscape_error(format!(
                "指定された出力先ビットマップ [ {destination} ] は存在しません"
            )));
        }
        let Some(words) = state.columns.get(id as usize) else {
            return Ok(false);
        };
        let boundary = words[33] as usize;
        let Some(&part_id) = words.get(boundary) else {
            return Ok(false);
        };
        let Some(part) = self
            .native_landscapes
            .get(&object)
            .and_then(|r| r.parts.get(part_id as usize))
            .cloned()
        else {
            return Ok(false);
        };
        self.store_runtime_bitmap(destination, part.image, part.format);
        Ok(true)
    }
    pub(super) fn refresh_native_landscape(&mut self, object: i32) {
        let Some(mut raster) = self.native_landscapes.remove(&object) else {
            return;
        };
        for layer in raster.layers.values() {
            self.graph_layers.remove(layer);
            self.graph_object_properties.remove(layer);
        }
        let old_layers = std::mem::take(&mut raster.layers);
        let Some(state) = self.graph91_landscapes.get(&object).cloned() else {
            return;
        };
        let width = state.map_width;
        let height = state.map_height;
        let base = self.graph90_native_sort_key(object).unwrap_or(0) & 0xffffe01f;
        let occluding = |row: i32, col: i32| -> i32 {
            state
                .cell_index(row, col)
                .and_then(|i| raster.columns.get(state.cells[i].column_index as usize))
                .map_or(0, |c| c.occluding)
        };
        let mut cells = Vec::with_capacity(state.cells.len());
        for row in 0..height {
            for col in 0..width {
                let index = (row * width + col) as usize;
                let id = state.cells[index].column_index;
                let Some(column) = raster.columns.get(id as usize) else {
                    cells.push(None);
                    continue;
                };
                let next = col - if row & 1 == 0 { 1 } else { 0 };
                let a = occluding(row + 1, next) - state.row_step;
                let b = occluding(row + 1, next + 1) - state.row_step;
                let mut crop = if a > 0 && b > 0 { a.min(b) } else { 0 };
                let c = occluding(row + 2, col) - 2 * state.row_step;
                if c > 0 && crop > c {
                    crop = c;
                }
                let visible = (column.image.height as i32 - crop).max(0) as u32;
                let priority = row.wrapping_mul(state.priority_row_factor).wrapping_add(
                    state
                        .priority_frame_factor
                        .wrapping_mul(column.extra + column.boundary - 1),
                );
                let depth = if visible == 0 {
                    0
                } else {
                    base.wrapping_add((row as u32).wrapping_add((priority as u32) << 11) << 5)
                };
                cells.push(Some(CellGeometry {
                    column: id as usize,
                    x: col.wrapping_mul(state.cell_width)
                        + if row & 1 != 0 {
                            state.cell_width / 2
                        } else {
                            0
                        }
                        + column.offset,
                    y: state.baseline + row.wrapping_mul(state.row_step)
                        - column.image.height as i32,
                    height: visible,
                    depth,
                }));
            }
        }
        raster.cells = cells;
        let depths = raster
            .cells
            .iter()
            .flatten()
            .filter_map(|c| (c.depth != 0).then_some(c.depth))
            .collect::<BTreeSet<_>>();
        let (w, h) = snapshot::runtime_frame_size(self);
        for depth in depths {
            let key = format!("runtime:landscape:{object}:{depth}");
            self.store_graph_image(key.clone(), crate::native_map_filter::blank(w, h));
            let layer = old_layers.get(&depth).copied().unwrap_or_else(|| {
                let layer = self.native_landscape_next_layer;
                self.native_landscape_next_layer = self.native_landscape_next_layer.wrapping_sub(1);
                layer
            });
            self.install_native_map_filter_layer(layer, key, w, h);
            if let Some(layer) = self.graph_layers.get_mut(&layer) {
                layer.owner_object = Some(object);
                layer.hit_id = object;
                layer.z = (depth >> 16) as i32;
                layer.x = 0.;
                layer.y = 0.;
            }
            raster.layers.insert(depth, layer);
        }
        if let Some(state) = self.graph91_landscapes.get_mut(&object) {
            for (cell, geometry) in state.cells.iter_mut().zip(&raster.cells) {
                cell.derived_value = geometry.as_ref().map_or(0, |g| (g.depth >> 16) as u16);
            }
        }
        self.native_landscapes.insert(object, raster);
    }
    pub(super) fn render_native_landscape_stage(
        &self,
        object: i32,
        depth: u32,
        source: &DecodedImage,
    ) -> Option<DecodedImage> {
        let state = self.graph91_landscapes.get(&object)?;
        let raster = self.native_landscapes.get(&object)?;
        let properties = self.graph_object_properties.get(&object)?;
        let alpha = properties.transparency_parameter();
        let blend = properties.blend_mode;
        let df = self
            .bitmap_formats
            .get(&NATIVE_SCREEN_BITMAP)
            .copied()
            .unwrap_or(1);
        let mut result = source.clone();
        let global = self.graph_object_global_display_offset(object);
        let x = properties.native.position_x + global.0 as i32;
        let y = properties.native.position_y + global.1 as i32;
        for (cell, geometry) in state.cells.iter().zip(&raster.cells) {
            let Some(g) = geometry.as_ref().filter(|g| g.depth == depth) else {
                continue;
            };
            let column = &raster.columns[g.column];
            let mut image = DecodedImage {
                width: column.image.width,
                height: g.height,
                rgba: column.image.rgba[..(column.image.width * g.height * 4) as usize].to_vec(),
            };
            if cell.silhouette_type != 0 {
                let mut silhouette = image.clone();
                for p in silhouette.rgba.as_chunks_mut::<4>().0 {
                    p[0] = (cell.silhouette_value >> 16) as u8;
                    p[1] = (cell.silhouette_value >> 8) as u8;
                    p[2] = cell.silhouette_value as u8;
                }
                if column.format == 2 {
                    crate::bitmap_blend::blit_mode(
                        &mut image,
                        column.format,
                        &silhouette,
                        column.format,
                        0,
                        0,
                        if cell.silhouette_type == 2 {
                            0x21
                        } else {
                            0x20
                        },
                        256 - cell.silhouette_level,
                    );
                }
            }
            crate::bitmap_blend::blit_mode(
                &mut result,
                df,
                &image,
                column.format,
                x + g.x,
                y + g.y,
                blend,
                alpha,
            );
            for &(id, transparency) in cell.guides.iter().flatten() {
                if let Some(guide) = raster.guides.get(id as usize) {
                    let gx =
                        x + g.x + ((column.image.width as i32 - guide.image.width as i32) >> 1);
                    let gy = y + g.y + column.image.height as i32
                        - column.boundary * state.column_step
                        - guide.image.height as i32;
                    let ga = 256 - ((256 - alpha).wrapping_mul(256 - transparency) >> 8);
                    crate::bitmap_blend::blit_mode(
                        &mut result,
                        df,
                        &guide.image,
                        guide.format,
                        gx,
                        gy,
                        blend,
                        ga,
                    );
                }
            }
        }
        if df == 1 {
            for p in result.rgba.as_chunks_mut::<4>().0 {
                p[3] = 255;
            }
        }
        Some(result)
    }
    /// sub_4838A0 -> sub_462CD0 -> sub_4401B0 -> sub_422C50: no map is
    /// status 13; no hit (14) returns false.
    pub(super) fn graph91_landscape_hit_test(
        &self,
        object: i32,
        mode: i32,
    ) -> ethornell_vm::VmResult<Option<[i32; 2]>> {
        let state = self
            .graph91_landscapes
            .get(&object)
            .ok_or_else(|| landscape_error(LANDSCAPE_HANDLE))?;
        if state.map_height == 0 {
            return Err(landscape_error(LANDSCAPE_NO_MAP));
        }
        Ok(self.native_landscape_hit(object, mode))
    }
    pub(super) fn native_landscape_hit(&self, object: i32, mode: i32) -> Option<[i32; 2]> {
        let state = self.graph91_landscapes.get(&object)?;
        let raster = self.native_landscapes.get(&object)?;
        let properties = self.graph_object_properties.get(&object)?;
        let (mx, my) = self.mouse_pos?;
        let x = mx as i32 - properties.native.position_x;
        let y = my as i32 - properties.native.position_y;
        if state.cell_width == 0 || state.row_step == 0 {
            return None;
        }
        let even = x / state.cell_width;
        let odd =
            even - i32::from((x as u32 % state.cell_width as u32) < (state.cell_width as u32 / 2));
        let begin = (i32::from(y >= state.baseline) + (y - state.baseline) / state.row_step).max(0);
        let end = (i32::from(y >= state.baseline)
            + (y - state.baseline) / state.row_step
            + 32 * state.column_step / state.row_step)
            .min(state.map_height - 1);
        for row in (begin..=end).rev() {
            let col = if row & 1 != 0 { odd } else { even };
            let Some(index) = state.cell_index(row, col) else {
                continue;
            };
            let Some(g) = raster.cells.get(index)?.as_ref() else {
                continue;
            };
            let (px, py) = (x - g.x, y - g.y);
            let column = &raster.columns[g.column];
            if px < 0 || py < 0 || px >= column.image.width as i32 || py >= g.height as i32 {
                continue;
            }
            let index = py as usize * column.image.width as usize + px as usize;
            let pixel = column.image.rgba[index * 4..index * 4 + 4].try_into().unwrap();
            let alpha = if mode == 1 {
                column
                    .hit
                    .as_ref()
                    .and_then(|m| m.get(index))
                    .copied()
                    .unwrap_or_else(|| coverage(column.format, pixel))
            } else {
                coverage(column.format, pixel)
            };
            if alpha != 0 {
                return Some([col, row]);
            }
        }
        None
    }
}

#[cfg(test)]
mod tests {
    use super::*;
    use ethornell_vm::{GraphApi, NativeCallFrame, NativeOpcode, Value};
    #[test]
    fn landscape_copies_assets_draws_depth_slices_and_hits_actual_alpha() {
        let manager = ResourceManager::open_game(env!("CARGO_MANIFEST_DIR")).unwrap();
        let mut api = RuntimeTraceApi::new(manager);
        api.screen_width = 4;
        api.screen_height = 4;
        let call = |api: &mut RuntimeTraceApi, id, args: &[i32]| {
            GraphApi::call_graph(
                api,
                &mut NativeCallFrame::new(
                    NativeOpcode { group: 0x91, id },
                    args.iter().copied().map(Value::Int).collect(),
                ),
            )
            .unwrap()
        };
        api.store_runtime_bitmap(
            100,
            DecodedImage {
                width: 4,
                height: 2,
                rgba: [[200, 0, 0, 255].repeat(2), [0, 0, 100, 255].repeat(2)]
                    .concat()
                    .repeat(2),
            },
            2,
        );
        let object = call(&mut api, 0x70, &[2, 1, 1, 2, 1, 0]).as_i32();
        let mut columns = vec![0; 68];
        columns[0] = 1;
        columns[1] = 0;
        columns[33] = 1;
        columns[34] = 1;
        columns[35] = 1;
        columns[67] = 1;
        GraphApi::configure_graph91_landscape_parts(
            &mut api,
            object,
            100,
            2,
            &[0, 0, 2, 2, 0, 2, 0, 2, 2, 0],
            1,
            2,
            &columns
        ).unwrap();
        api.graph_resources.remove(&100);
        api.graph_images.remove("runtime:bitmap:100");
        GraphApi::configure_graph91_landscape_map(
            &mut api,
            object,
            2,
            1,
            &[0, 1]
        ).unwrap();
        call(&mut api, 0x75, &[object, 0, 0, 1, 0, 5]);
        api.refresh_native_filters();
        let frame = snapshot::compose_runtime_frame(&api, false);
        assert_eq!(
            &frame.rgba[..16],
            &[
                200, 0, 0, 255, 200, 0, 0, 255, 0, 0, 100, 255, 0, 0, 100, 255
            ]
        );
        api.mouse_pos = Some((3., 1.));
        assert_eq!(api.native_landscape_hit(object, 0), Some([1, 0]));
        assert_eq!(api.graph91_landscape_cell_value(object, 0, 0).unwrap(), Some(5));
        call(&mut api, 0x7c, &[object, 0, 1]);
        api.refresh_native_filters();
        assert_eq!(
            &snapshot::compose_runtime_frame(&api, false).rgba[..4],
            &[0, 0, 100, 255]
        );
        assert_eq!(call(&mut api, 0x7f, &[200, object, 0, 0]), Value::Int(1));
        assert_eq!(
            &api.graph_bitmap_image(200).unwrap().rgba[..4],
            &[0, 0, 100, 255]
        );
        api.store_runtime_bitmap(
            101,
            DecodedImage {
                width: 1,
                height: 1,
                rgba: vec![255, 255, 255, 255],
            },
            2,
        );
        GraphApi::configure_graph91_landscape_guides(
            &mut api,
            object,
            101,
            1,
            &[0, 0, 1, 1, 0]
        ).unwrap();
        GraphApi::set_graph91_landscape_cell_guides(
            &mut api,
            object,
            &[0, 0],
            0,
            0,
            0
        ).unwrap();
        api.graph_resources.remove(&101);
        api.refresh_native_filters();
        assert_eq!(
            &snapshot::compose_runtime_frame(&api, false).rgba[..4],
            &[255, 255, 255, 255]
        );
        call(&mut api, 0x7d, &[object, 0, 0, 1]);
        api.refresh_native_filters();
        assert_eq!(
            &snapshot::compose_runtime_frame(&api, false).rgba[..4],
            &[0, 0, 100, 255]
        );
    }

    #[test]
    fn landscape_statuses_use_target_texts_and_order() {
        let manager =
            ethornell_archive::ResourceManager::open_game(env!("CARGO_MANIFEST_DIR")).unwrap();
        let mut api = RuntimeTraceApi::new(manager);
        let object = GraphApi::call_graph(
            &mut api,
            &mut NativeCallFrame::new(
                NativeOpcode { group: 0x91, id: 0x70 },
                [2, 1, 1, 2, 1, 0].map(Value::Int).to_vec(),
            ),
        )
        .unwrap()
        .as_i32();
        let text = |result: ethornell_vm::VmResult<()>| result.unwrap_err().to_string();
        // sub_421C60: zero part count reports index -1 before the bitmap.
        assert!(text(GraphApi::configure_graph91_landscape_parts(
            &mut api, object, 999, 0, &[], 0, 1, &[0; 34]
        ))
        .contains("部品番号 [ -1 ] に無効な情報が指定されています"));
        assert!(text(GraphApi::configure_graph91_landscape_parts(
            &mut api, object, 999, 1, &[0, 0, 1, 1, 0], 0, 1, &[0; 34]
        ))
        .contains("指定されたビットマップ [ 999 ] は存在しません"));
        // sub_422190: no pillars wins over a bad size.
        assert!(text(GraphApi::configure_graph91_landscape_map(&mut api, object, 0, 0, &[]))
            .contains("柱が設定されていません"));
        assert!(text(GraphApi::configure_graph91_landscape_guides(
            &mut api, object, 999, 0, &[]
        ))
        .contains("部品番号 [ -1 ]"));
        // sub_4229B0 reports the missing map before the layer.
        assert!(text(GraphApi::set_graph91_landscape_cell_guides(
            &mut api, object, &[0, 0], 9, 0, 0
        ))
        .contains("マップが設定されていません"));
        assert!(api
            .graph91_landscape_cell_value(object, 0, 0)
            .unwrap_err()
            .to_string()
            .contains("マップが設定されていません"));
        assert!(GraphApi::graph91_landscape_hit_test(&api, 0x1234, 0)
            .unwrap_err()
            .to_string()
            .contains("無効なランドスケープハンドル"));
    }
}

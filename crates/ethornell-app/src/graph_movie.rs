use ethornell_image::DecodedImage;
use std::collections::BTreeMap;
use std::sync::Arc;

const BURIKO_MOVIE_MAGIC: &[u8; 16] = b"BF_Movie_______\0";

#[derive(Debug, Clone)]
pub(crate) struct BurikoMovieResource {
    pub archive: String,
    pub resource: String,
    pub bytes: Arc<Vec<u8>>,
    pub metadata: [i32; 5],
    pub parent: Option<i32>,
    pub child: Option<i32>,
}

#[derive(Debug, Default)]
pub(crate) struct BurikoMovieRegistry {
    next_handle: i32,
    resources: BTreeMap<i32, BurikoMovieResource>,
}

impl BurikoMovieRegistry {
    pub(crate) fn load(
        &mut self,
        archive: String,
        resource: String,
        bytes: Vec<u8>,
    ) -> Result<(i32, [i32; 5]), i32> {
        let metadata = parse_buriko_movie_header(&bytes).ok_or(2)?;
        self.next_handle = self.next_handle.saturating_add(1).max(1);
        let handle = self.next_handle;
        self.resources.insert(
            handle,
            BurikoMovieResource {
                archive,
                resource,
                bytes: Arc::new(bytes),
                metadata,
                parent: None,
                child: None,
            },
        );
        Ok((handle, metadata))
    }

    pub(crate) fn contains(&self, handle: i32) -> bool {
        self.resources.contains_key(&handle)
    }

    pub(crate) fn metadata(&self, handle: i32) -> Option<[i32; 5]> {
        self.resources
            .get(&handle)
            .map(|resource| resource.metadata)
    }

    pub(crate) fn release(&mut self, handle: i32) -> i32 {
        let Some(resource) = self.resources.remove(&handle) else {
            return 3;
        };
        if let Some(parent) = resource.parent
            && let Some(parent_resource) = self.resources.get_mut(&parent)
            && parent_resource.child == Some(handle)
        {
            parent_resource.child = resource.child;
        }
        if let Some(child) = resource.child
            && let Some(child_resource) = self.resources.get_mut(&child)
        {
            child_resource.parent = resource.parent;
        }
        0
    }

    pub(crate) fn attach(&mut self, source: i32) -> Result<i32, i32> {
        let Some(source_resource) = self.resources.get(&source) else {
            return Err(3);
        };
        if source_resource.child.is_some() {
            return Err(7);
        }
        let cloned = source_resource.clone();
        self.next_handle = self.next_handle.saturating_add(1).max(1);
        let handle = self.next_handle;
        self.resources.insert(
            handle,
            BurikoMovieResource {
                archive: cloned.archive,
                resource: cloned.resource,
                bytes: cloned.bytes,
                metadata: cloned.metadata,
                parent: Some(source),
                child: None,
            },
        );
        self.resources
            .get_mut(&source)
            .expect("source resource disappeared")
            .child = Some(handle);
        Ok(handle)
    }

    /// Graph90:F6: sub_405F10 (synchronous) and sub_405E00 (DCProcDecodeBMV)
    /// check the handle (3), frame (4) and destination bitmap (5), then decode
    /// the frame over the destination's pixels (sub_49CBD0/sub_49CC10 ->
    /// sub_49CC40).
    pub(crate) fn decode_frame(
        &self,
        destination_info: Option<(u32, u32, i32)>,
        destination_pixels: Option<&mut DecodedImage>,
        source: i32,
        frame_index: i32,
    ) -> i32 {
        let Some(resource) = self.resources.get(&source) else {
            return 3;
        };
        let frame_count = resource.metadata[4].max(0);
        if frame_index < 0 || frame_index >= frame_count {
            return 4;
        }
        let Some((width, height, format)) = destination_info else {
            return 5;
        };
        if width as i32 != resource.metadata[0]
            || height as i32 != resource.metadata[1]
            || format != resource.metadata[2]
        {
            return 5;
        }
        // The target reaches status 6 only when its BMV frame decoder fails
        // after all handle/header/bitmap checks.
        let Some(pixels) = destination_pixels else {
            return 6;
        };
        match ethornell_image::decode_bf_movie_frame(&resource.bytes, frame_index as u32, pixels) {
            Ok(()) => 0,
            Err(_) => 6,
        }
    }
}

fn parse_buriko_movie_header(bytes: &[u8]) -> Option<[i32; 5]> {
    if bytes.len() < 44 || &bytes[..16] != BURIKO_MOVIE_MAGIC {
        return None;
    }
    let read = |offset: usize| {
        i32::from_le_bytes(
            bytes[offset..offset + 4]
                .try_into()
                .expect("checked header range"),
        )
    };
    Some([read(20), read(24), read(32), read(36), read(40)])
}

/// An 8 x 8 BF_Movie of one frame: one grey block (Y DC 400, level 178) with
/// opaque alpha, Huffman-coded by hand.
#[cfg(test)]
pub(crate) fn decodable_test_movie() -> Vec<u8> {
    let mut bytes = BURIKO_MOVIE_MAGIC.to_vec();
    for value in [0x10001_u32, 8, 8, 32, 2, 3, 1, 30, 0, 0, 0, 0] {
        bytes.extend(value.to_le_bytes());
    }
    // Quantizers 1, then the frame table.
    bytes.extend([1u8; 128]);
    bytes.extend(196u32.to_le_bytes());
    // DC weights: two 9-bit differences (code 1) and one 0-bit (code 0); AC
    // weights: three end-of-block codes (code 0).
    let mut weights = [0u8; 16 + 176];
    weights[0] = 1;
    weights[9] = 2;
    weights[16] = 3;
    bytes.extend(weights);
    // The strip after this table, then the alpha.
    bytes.extend(200u32.to_le_bytes());
    bytes.extend(207u32.to_le_bytes());
    // Block mask, 192 coefficients, DC codes 1 +400, 1 -400, 0; AC 0, 0, 0.
    bytes.extend([0b1, 0xc0, 0x01, 0b1110_0100, 0b0010_0110, 0b1111_0000, 0]);
    // Alpha mode 1: one 0xFF byte, then copies of the pixel to the left.
    bytes.extend(1u32.to_le_bytes());
    bytes.extend([0b10, 0xff, 0x3f, 0xfe]);
    bytes
}

#[cfg(test)]
mod tests {
    use super::{
        BURIKO_MOVIE_MAGIC, BurikoMovieRegistry, DecodedImage, decodable_test_movie,
        parse_buriko_movie_header,
    };

    fn movie_bytes() -> Vec<u8> {
        let mut bytes = vec![0u8; 64];
        bytes[..16].copy_from_slice(BURIKO_MOVIE_MAGIC);
        for (offset, value) in [(20, 640_i32), (24, 480), (32, 4), (36, 30), (40, 120)] {
            bytes[offset..offset + 4].copy_from_slice(&value.to_le_bytes());
        }
        bytes
    }

    #[test]
    fn parses_the_five_target_metadata_dwords() {
        assert_eq!(
            parse_buriko_movie_header(&movie_bytes()),
            Some([640, 480, 4, 30, 120])
        );
    }

    #[test]
    fn attach_is_single_child_and_release_repairs_links() {
        let mut registry = BurikoMovieRegistry::default();
        let (root, _) = registry
            .load(String::new(), "movie.bmv".into(), movie_bytes())
            .unwrap();
        let child = registry.attach(root).unwrap();
        assert_eq!(registry.attach(root), Err(7));
        assert_eq!(registry.release(child), 0);
        assert!(registry.attach(root).is_ok());
    }

    #[test]
    fn decode_frame_checks_the_contract_then_draws_the_frame() {
        let mut registry = BurikoMovieRegistry::default();
        let (movie, metadata) = registry
            .load(String::new(), "movie.bmv".into(), decodable_test_movie())
            .unwrap();
        assert_eq!(metadata, [8, 8, 2, 3, 1]);
        let info = Some((8, 8, 2));
        let mut pixels = DecodedImage {
            width: 8,
            height: 8,
            rgba: vec![0; 8 * 8 * 4],
        };
        assert_eq!(
            registry.decode_frame(info, Some(&mut pixels), movie + 1, 0),
            3
        );
        assert_eq!(registry.decode_frame(info, Some(&mut pixels), movie, 1), 4);
        assert_eq!(
            registry.decode_frame(Some((8, 8, 1)), Some(&mut pixels), movie, 0),
            5
        );
        assert!(pixels.rgba.iter().all(|&byte| byte == 0));
        assert_eq!(registry.decode_frame(info, Some(&mut pixels), movie, 0), 0);
        assert!(
            pixels
                .rgba
                .chunks_exact(4)
                .all(|pixel| pixel == [178, 178, 178, 255])
        );
    }

    #[test]
    fn a_frame_that_fails_to_decode_is_status_6() {
        // movie_bytes() has the header fields the contract checks, no frames.
        let mut registry = BurikoMovieRegistry::default();
        let (movie, _) = registry
            .load(String::new(), "movie.bmv".into(), movie_bytes())
            .unwrap();
        let mut pixels = DecodedImage {
            width: 640,
            height: 480,
            rgba: vec![0; 640 * 480 * 4],
        };
        assert_eq!(
            registry.decode_frame(Some((640, 480, 4)), Some(&mut pixels), movie, 0),
            6
        );
    }
}

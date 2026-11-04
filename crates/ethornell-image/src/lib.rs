use byteorder::{LittleEndian, ReadBytesExt};
use ethornell_core::{EthornellError, Result};
use serde::Serialize;
use std::io::Cursor;
use std::path::Path;

mod cbg;
mod cbg2_encode;
mod cbg_encode;
pub use cbg2_encode::encode_cbg_v2;
pub use cbg_encode::encode_cbg_v1;
pub use cbg::decode_native_buffer as decode_cbg_native_buffer;

#[derive(Debug, Clone, Copy, PartialEq, Eq, Serialize)]
pub enum ImageFormat {
    CompressedBg,
    RawBgiImage,
    Png,
    Jpeg,
    Bmp,
    Unknown,
}

#[derive(Debug, Clone, PartialEq, Eq, Serialize)]
pub struct CbgMetadata {
    pub width: u32,
    pub height: u32,
    pub bpp: u32,
    /// Target image-header word at CBG +0x18. sub_401C10 consults this
    /// subtype for 32-bpp images: 4/5/7 select native formats 4/5/7, while
    /// every other value selects format 2. sub_469EE0 rewrites decoded
    /// 24-bpp CBGs to 32-bpp/subtype 7 before format selection.
    pub image_subtype: u16,
    /// Target image-header word at CBG +0x1A. Values 0 and 1 are accepted by
    /// sub_401EF0; value 1 means the following embedded reference point is
    /// copied into bitmap-registry DWORDs +0x28/+0x2C.
    pub embedded_point_flag: u16,
    /// CBG +0x1C, valid when `embedded_point_flag == 1`.
    pub embedded_x: u16,
    /// CBG +0x1E, valid when `embedded_point_flag == 1`.
    pub embedded_y: u16,
    pub version: Option<u32>,
    pub intermediate_length: Option<u32>,
    pub encoded_length: Option<u32>,
    pub key: Option<u32>,
    pub checksum: Option<u32>,
    pub xor_check: Option<u32>,
}

impl CbgMetadata {
    /// Native bitmap-registry format selected by sub_401C10 followed by the
    /// format-7 normalization in sub_407DA0. This is the storage/display
    /// format attached to the concrete bitmap slot, not merely bytes-per-pixel.
    pub fn native_bitmap_format(&self) -> Option<i32> {
        match self.bpp {
            8 => Some(3),
            16 => Some(0),
            // sub_469EE0 converts decoded 24-bpp CBGs to 32-bpp/subtype 7;
            // sub_401C10 returns 7 and sub_407DA0 normalizes 7 -> 1.
            24 => Some(1),
            32 => Some(match self.image_subtype {
                4 => 4,
                5 => 5,
                7 => 1,
                _ => 2,
            }),
            48 => Some(6),
            _ => None,
        }
    }

    pub fn embedded_point(&self) -> Option<[i32; 2]> {
        (self.embedded_point_flag == 1)
            .then_some([i32::from(self.embedded_x), i32::from(self.embedded_y)])
    }
}

#[derive(Debug, Clone, PartialEq, Eq, Serialize)]
pub struct ImageInfo {
    pub width: Option<u32>,
    pub height: Option<u32>,
    pub bpp: Option<u32>,
    pub format: ImageFormat,
    pub cbg: Option<CbgMetadata>,
}

#[derive(Debug, Clone, PartialEq, Eq)]
pub struct DecodedImage {
    pub width: u32,
    pub height: u32,
    pub rgba: Vec<u8>,
}

pub fn detect_image_format(buf: &[u8]) -> ImageFormat {
    if buf.starts_with(b"CompressedBG___") {
        ImageFormat::CompressedBg
    } else if buf.starts_with(b"\x89PNG\r\n\x1a\n") {
        ImageFormat::Png
    } else if buf.starts_with(b"\xff\xd8\xff") {
        ImageFormat::Jpeg
    } else if buf.starts_with(b"BM") {
        ImageFormat::Bmp
    } else if looks_like_raw_bgi_image(buf) {
        ImageFormat::RawBgiImage
    } else {
        ImageFormat::Unknown
    }
}

pub fn decode_image(data: &[u8]) -> Result<DecodedImage> {
    match detect_image_format(data) {
        ImageFormat::CompressedBg => decode_cbg(data),
        ImageFormat::RawBgiImage => decode_raw_bgi_image(data),
        ImageFormat::Png | ImageFormat::Jpeg | ImageFormat::Bmp => decode_standard_image(data),
        format => Err(EthornellError::UnsupportedFormat(format!(
            "image decode unsupported for {format:?}"
        ))),
    }
}

pub fn decode_cbg(data: &[u8]) -> Result<DecodedImage> {
    cbg::decode_cbg(data)
}

/// Decode frame `frame` of a BGI frame movie ("BF_Movie_______", drawn by
/// Graph90:F6) over `image`, which must have the movie's size. A frame codes
/// only the blocks that change, so `image` must hold the frame before it.
pub fn decode_bf_movie_frame(data: &[u8], frame: u32, image: &mut DecodedImage) -> Result<()> {
    cbg::bf_movie::decode_frame(data, frame, image)
}

pub fn decode_cbg_to_png(data: &[u8], output: &Path) -> Result<()> {
    let image = decode_cbg(data)?;
    write_rgba_png(&image, output)
}

pub fn write_rgba_png(image: &DecodedImage, output: &Path) -> Result<()> {
    if image.rgba.len() != image.width as usize * image.height as usize * 4 {
        return Err(EthornellError::Parse(format!(
            "RGBA buffer length mismatch: {} != {}x{}x4",
            image.rgba.len(),
            image.width,
            image.height
        )));
    }
    if let Some(parent) = output.parent() {
        std::fs::create_dir_all(parent)?;
    }
    image::save_buffer_with_format(
        output,
        &image.rgba,
        image.width,
        image.height,
        image::ColorType::Rgba8,
        image::ImageFormat::Png,
    )
    .map_err(|err| EthornellError::Other(format!("write PNG {}: {err}", output.display())))
}

fn decode_standard_image(data: &[u8]) -> Result<DecodedImage> {
    if let Some(image) = decode_bmp_32bpp_rgb_with_alpha(data) {
        return Ok(image);
    }
    let image = image::load_from_memory(data)
        .map_err(|err| EthornellError::UnsupportedFormat(format!("standard image decode: {err}")))?
        .to_rgba8();
    let (width, height) = image.dimensions();
    Ok(DecodedImage {
        width,
        height,
        rgba: image.into_raw(),
    })
}

/// Map standard BMP storage to the runtime's opaque RGB / alpha RGBA formats.
pub fn bmp_native_bitmap_format(data: &[u8]) -> Result<i32> {
    use image::ImageDecoder;
    let decoder = image::codecs::bmp::BmpDecoder::new(Cursor::new(data))
        .map_err(|err| EthornellError::UnsupportedFormat(format!("BMP header: {err}")))?;
    Ok(
        if decoder.color_type().has_alpha() || decode_bmp_32bpp_rgb_with_alpha(data).is_some() {
            2
        } else {
            1
        },
    )
}

/// BGI system graphics are 32-bpp `BI_RGB` BMPs whose fourth byte is a real
/// alpha mask. Generic decoders treat that byte as padding, so read it here.
/// A bitmap whose fourth byte is zero everywhere was written by a tool that
/// leaves the byte unused, and stays opaque (`None`).
fn decode_bmp_32bpp_rgb_with_alpha(data: &[u8]) -> Option<DecodedImage> {
    const BI_RGB: u32 = 0;
    let u16_at = |offset: usize| {
        Some(u16::from_le_bytes(
            data.get(offset..offset + 2)?.try_into().ok()?,
        ))
    };
    let u32_at = |offset: usize| {
        Some(u32::from_le_bytes(
            data.get(offset..offset + 4)?.try_into().ok()?,
        ))
    };
    if !data.starts_with(b"BM") || u32_at(14)? < 40 {
        return None;
    }
    if u16_at(28)? != 32 || u32_at(30)? != BI_RGB {
        return None;
    }
    let pixel_offset = u32_at(10)? as usize;
    let width = u32_at(18)? as i32;
    let height = u32_at(22)? as i32;
    if width <= 0 || height == 0 {
        return None;
    }
    let (width, rows) = (width as usize, height.unsigned_abs() as usize);
    let stride = width.checked_mul(4)?;
    let pixels = data.get(pixel_offset..pixel_offset.checked_add(stride.checked_mul(rows)?)?)?;
    if pixels.chunks_exact(4).all(|pixel| pixel[3] == 0) {
        return None;
    }
    let mut rgba = vec![0u8; stride * rows];
    for (row, source) in pixels.chunks_exact(stride).enumerate() {
        // Positive heights store the bottom row first.
        let target_row = if height > 0 { rows - 1 - row } else { row };
        let target = &mut rgba[target_row * stride..(target_row + 1) * stride];
        for (out, bgra) in target.chunks_exact_mut(4).zip(source.chunks_exact(4)) {
            out.copy_from_slice(&[bgra[2], bgra[1], bgra[0], bgra[3]]);
        }
    }
    Some(DecodedImage {
        width: width as u32,
        height: rows as u32,
        rgba,
    })
}

fn looks_like_raw_bgi_image(buf: &[u8]) -> bool {
    if buf.len() < 16 {
        return false;
    }
    let mut cursor = Cursor::new(buf);
    let Ok(width) = cursor.read_u16::<LittleEndian>() else {
        return false;
    };
    let Ok(height) = cursor.read_u16::<LittleEndian>() else {
        return false;
    };
    let Ok(bpp) = cursor.read_u32::<LittleEndian>() else {
        return false;
    };
    if width == 0 || height == 0 || width > 8192 || height > 8192 {
        return false;
    }
    let pixel_bytes = match bpp {
        24 => 3usize,
        32 => 4usize,
        _ => return false,
    };
    let expected = 16usize.saturating_add(width as usize * height as usize * pixel_bytes);
    expected == buf.len()
}

fn decode_raw_bgi_image(data: &[u8]) -> Result<DecodedImage> {
    let mut cursor = Cursor::new(data);
    let width = cursor.read_u16::<LittleEndian>()? as u32;
    let height = cursor.read_u16::<LittleEndian>()? as u32;
    let bpp = cursor.read_u32::<LittleEndian>()?;
    if !matches!(bpp, 24 | 32) {
        return Err(EthornellError::UnsupportedFormat(format!(
            "raw BGI image bpp {bpp} is not implemented"
        )));
    }
    let pixel_bytes = (bpp / 8) as usize;
    let pixels = &data[16..];
    let expected = width as usize * height as usize * pixel_bytes;
    if pixels.len() != expected {
        return Err(EthornellError::Parse(format!(
            "raw BGI image size mismatch: got {}, expected {expected}",
            pixels.len()
        )));
    }
    let mut rgba = Vec::with_capacity(width as usize * height as usize * 4);
    for pixel in pixels.chunks_exact(pixel_bytes) {
        let b = pixel[0];
        let g = pixel[1];
        let r = pixel[2];
        let a = if pixel_bytes == 4 { pixel[3] } else { 0xff };
        rgba.extend_from_slice(&[r, g, b, a]);
    }
    Ok(DecodedImage {
        width,
        height,
        rgba,
    })
}

pub fn probe_image(buf: &[u8]) -> Result<ImageInfo> {
    let format = detect_image_format(buf);
    if format == ImageFormat::CompressedBg {
        let cbg = parse_cbg_metadata(buf)?;
        Ok(ImageInfo {
            width: Some(cbg.width),
            height: Some(cbg.height),
            bpp: Some(cbg.bpp),
            format,
            cbg: Some(cbg),
        })
    } else if format == ImageFormat::RawBgiImage {
        let mut cursor = Cursor::new(buf);
        Ok(ImageInfo {
            width: Some(cursor.read_u16::<LittleEndian>()? as u32),
            height: Some(cursor.read_u16::<LittleEndian>()? as u32),
            bpp: Some(cursor.read_u32::<LittleEndian>()?),
            format,
            cbg: None,
        })
    } else {
        Ok(ImageInfo {
            width: None,
            height: None,
            bpp: None,
            format,
            cbg: None,
        })
    }
}

pub fn parse_cbg_metadata(buf: &[u8]) -> Result<CbgMetadata> {
    if !buf.starts_with(b"CompressedBG___") {
        return Err(EthornellError::UnsupportedFormat(
            "missing CompressedBG___ header".into(),
        ));
    }
    if buf.len() < 0x30 {
        return Err(EthornellError::Parse(
            "truncated CompressedBG header".into(),
        ));
    }

    let mut cursor = Cursor::new(&buf[0x10..0x30]);
    let width = cursor.read_u16::<LittleEndian>()? as u32;
    let height = cursor.read_u16::<LittleEndian>()? as u32;
    let bpp = cursor.read_u32::<LittleEndian>()?;
    // sub_469EE0 copies CBG +0x10..+0x1F verbatim into the target's internal
    // 16-byte image header. sub_401EF0 then reads word[5] (+0x1A) as a
    // 0/1 presence flag and word[6..=7] (+0x1C/+0x1E) as the bitmap
    // descriptor reference point. sub_401C10 reads the first word at +0x18
    // as the 32-bpp image subtype used to select native bitmap format.
    let image_subtype = cursor.read_u16::<LittleEndian>()?;
    let embedded_point_flag = cursor.read_u16::<LittleEndian>()?;
    let embedded_x = cursor.read_u16::<LittleEndian>()?;
    let embedded_y = cursor.read_u16::<LittleEndian>()?;
    let intermediate_length = cursor.read_u32::<LittleEndian>()?;
    let key = cursor.read_u32::<LittleEndian>()?;
    let encoded_length = cursor.read_u32::<LittleEndian>()?;
    let checksum = cursor.read_u8()? as u32;
    let xor_check = cursor.read_u8()? as u32;
    let version = cursor.read_u16::<LittleEndian>()? as u32;

    Ok(CbgMetadata {
        width,
        height,
        bpp,
        image_subtype,
        embedded_point_flag,
        embedded_x,
        embedded_y,
        version: Some(version),
        intermediate_length: Some(intermediate_length),
        encoded_length: Some(encoded_length),
        key: Some(key),
        checksum: Some(checksum),
        xor_check: Some(xor_check),
    })
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn detects_unknown_image() {
        assert_eq!(detect_image_format(b"hello"), ImageFormat::Unknown);
    }

    #[test]
    fn decodes_bottom_up_rgb_bmp_with_row_padding_as_an_opaque_background() {
        let mut data = vec![0u8; 70];
        data[..2].copy_from_slice(b"BM");
        data[2..6].copy_from_slice(&70u32.to_le_bytes());
        data[10..14].copy_from_slice(&54u32.to_le_bytes());
        data[14..18].copy_from_slice(&40u32.to_le_bytes());
        data[18..22].copy_from_slice(&2i32.to_le_bytes());
        data[22..26].copy_from_slice(&2i32.to_le_bytes());
        data[26..28].copy_from_slice(&1u16.to_le_bytes());
        data[28..30].copy_from_slice(&24u16.to_le_bytes());
        data[34..38].copy_from_slice(&16u32.to_le_bytes());
        // BGR rows, bottom first, each padded to a multiple of four bytes.
        data[54..].copy_from_slice(&[255, 0, 0, 255, 255, 255, 0, 0, 0, 0, 255, 0, 255, 0, 0, 0]);

        assert_eq!(detect_image_format(&data), ImageFormat::Bmp);
        assert_eq!(bmp_native_bitmap_format(&data).unwrap(), 1);
        let image = decode_image(&data).unwrap();
        assert_eq!((image.width, image.height), (2, 2));
        assert_eq!(
            image.rgba,
            vec![
                255, 0, 0, 255, 0, 255, 0, 255, 0, 0, 255, 255, 255, 255, 255, 255
            ]
        );
        assert!(decode_image(&data[..54]).is_err());
    }

    #[test]
    fn alpha_bmp_keeps_rgba_format_and_transparency() {
        let mut data = Cursor::new(Vec::new());
        image::DynamicImage::ImageRgba8(
            image::RgbaImage::from_raw(1, 1, vec![10, 20, 30, 40]).unwrap(),
        )
        .write_to(&mut data, image::ImageFormat::Bmp)
        .unwrap();
        assert_eq!(bmp_native_bitmap_format(data.get_ref()).unwrap(), 2);
        assert_eq!(
            decode_image(data.get_ref()).unwrap().rgba,
            vec![10, 20, 30, 40]
        );
    }

    fn bmp_32bpp_bi_rgb(height: i32, pixels: &[[u8; 4]]) -> Vec<u8> {
        let mut data = vec![0u8; 54];
        data[..2].copy_from_slice(b"BM");
        data[2..6].copy_from_slice(&((54 + pixels.len() * 4) as u32).to_le_bytes());
        data[10..14].copy_from_slice(&54u32.to_le_bytes());
        data[14..18].copy_from_slice(&40u32.to_le_bytes());
        data[18..22].copy_from_slice(&1i32.to_le_bytes());
        data[22..26].copy_from_slice(&height.to_le_bytes());
        data[26..28].copy_from_slice(&1u16.to_le_bytes());
        data[28..30].copy_from_slice(&32u16.to_le_bytes());
        for pixel in pixels {
            data.extend_from_slice(pixel);
        }
        data
    }

    #[test]
    fn bi_rgb_32bpp_bmp_uses_the_fourth_byte_as_alpha() {
        // BGRA, bottom row first: the top row is a transparent white corner.
        let data = bmp_32bpp_bi_rgb(2, &[[0, 0, 255, 128], [255, 255, 255, 0]]);
        assert_eq!(bmp_native_bitmap_format(&data).unwrap(), 2);
        assert_eq!(
            decode_image(&data).unwrap().rgba,
            vec![255, 255, 255, 0, 255, 0, 0, 128]
        );
    }

    #[test]
    fn bi_rgb_32bpp_bmp_with_an_all_zero_fourth_byte_stays_opaque() {
        let data = bmp_32bpp_bi_rgb(-2, &[[0, 0, 255, 0], [255, 0, 0, 0]]);
        assert_eq!(bmp_native_bitmap_format(&data).unwrap(), 1);
        assert_eq!(
            decode_image(&data).unwrap().rgba,
            vec![255, 0, 0, 255, 0, 0, 255, 255]
        );
    }

    #[test]
    fn parses_cbg_metadata() {
        let mut data = vec![0u8; 0x30];
        data[..16].copy_from_slice(b"CompressedBG___\0");
        data[0x10..0x12].copy_from_slice(&640u16.to_le_bytes());
        data[0x12..0x14].copy_from_slice(&480u16.to_le_bytes());
        data[0x14..0x18].copy_from_slice(&32u32.to_le_bytes());
        data[0x18..0x1a].copy_from_slice(&7u16.to_le_bytes());
        data[0x1a..0x1c].copy_from_slice(&1u16.to_le_bytes());
        data[0x1c..0x1e].copy_from_slice(&740u16.to_le_bytes());
        data[0x1e..0x20].copy_from_slice(&205u16.to_le_bytes());
        data[0x20..0x24].copy_from_slice(&123u32.to_le_bytes());
        data[0x24..0x28].copy_from_slice(&0x4567u32.to_le_bytes());
        data[0x28..0x2c].copy_from_slice(&456u32.to_le_bytes());
        data[0x2c] = 7;
        data[0x2d] = 9;
        data[0x2e..0x30].copy_from_slice(&2u16.to_le_bytes());

        let meta = parse_cbg_metadata(&data).unwrap();
        assert_eq!(meta.width, 640);
        assert_eq!(meta.height, 480);
        assert_eq!(meta.bpp, 32);
        assert_eq!(meta.image_subtype, 7);
        assert_eq!(meta.native_bitmap_format(), Some(1));
        assert_eq!(meta.embedded_point_flag, 1);
        assert_eq!(meta.embedded_point(), Some([740, 205]));
        assert_eq!(meta.version, Some(2));
        assert_eq!(meta.encoded_length, Some(456));
    }

    #[test]
    fn cbg_native_bitmap_format_matches_target_subtype_rules() {
        let base = CbgMetadata {
            width: 1,
            height: 1,
            bpp: 32,
            image_subtype: 0,
            embedded_point_flag: 0,
            embedded_x: 0,
            embedded_y: 0,
            version: Some(2),
            intermediate_length: None,
            encoded_length: None,
            key: None,
            checksum: None,
            xor_check: None,
        };

        assert_eq!(base.native_bitmap_format(), Some(2));
        assert_eq!(
            CbgMetadata {
                image_subtype: 4,
                ..base.clone()
            }
            .native_bitmap_format(),
            Some(4)
        );
        assert_eq!(
            CbgMetadata {
                image_subtype: 5,
                ..base.clone()
            }
            .native_bitmap_format(),
            Some(5)
        );
        assert_eq!(
            CbgMetadata {
                image_subtype: 7,
                ..base.clone()
            }
            .native_bitmap_format(),
            Some(1)
        );
        assert_eq!(
            CbgMetadata {
                bpp: 24,
                image_subtype: 0,
                ..base.clone()
            }
            .native_bitmap_format(),
            Some(1)
        );
        assert_eq!(
            CbgMetadata {
                bpp: 8,
                image_subtype: 0,
                ..base.clone()
            }
            .native_bitmap_format(),
            Some(3)
        );
        assert_eq!(
            CbgMetadata {
                bpp: 16,
                image_subtype: 0,
                ..base.clone()
            }
            .native_bitmap_format(),
            Some(0)
        );
        assert_eq!(
            CbgMetadata {
                bpp: 48,
                image_subtype: 0,
                ..base.clone()
            }
            .native_bitmap_format(),
            Some(6)
        );
    }
}

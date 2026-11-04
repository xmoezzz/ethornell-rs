//! BGI frame movies ("BF_Movie_______", the target's YCBFMovie2), drawn into
//! bitmaps by Graph90:F6 GraphDecodeBurikoMovieFrame. A frame is coded like a
//! CompressedBG v2 image (the parent module's DCT codec) with one change bit
//! per 8x8 block: only the blocks whose bit is set are coded, the others keep
//! the destination's pixels, so each frame is drawn over the one before it.

use super::{
    BLOCK_FILL_ORDER, BlockDecoder, CbgV2Decoder, DCT_TABLE, HuffmanTree, MsbBitStream, Reader,
};
use crate::DecodedImage;
use ethornell_core::{EthornellError, Result};

const MAGIC: &[u8; 16] = b"BF_Movie_______\0";
/// sub_49B470/sub_49B550 read the luma and chroma quantization tables after
/// the 64-byte header; sub_49B640 reads the frame offsets after them.
const QUANT_TABLES: usize = 64;
const FRAME_TABLE: usize = 192;
const MAX_DIMENSION: usize = 0x4000;

struct Header {
    version: u32,
    width: usize,
    height: usize,
    bpp: u32,
    frame_count: usize,
}

fn parse_error(message: &str) -> EthornellError {
    EthornellError::Parse(format!("BF_Movie: {message}"))
}

fn read_u32(data: &[u8], offset: usize) -> Result<u32> {
    data.get(offset..offset.saturating_add(4))
        .map(|bytes| u32::from_le_bytes([bytes[0], bytes[1], bytes[2], bytes[3]]))
        .ok_or_else(|| parse_error("truncated table"))
}

/// Decode frame `frame` over `image`, which must have the movie's size and,
/// for any frame after the first, hold the frame before it.
pub(crate) fn decode_frame(data: &[u8], frame: u32, image: &mut DecodedImage) -> Result<()> {
    let header = parse_header(data)?;
    if image.width as usize != header.width
        || image.height as usize != header.height
        || image.rgba.len() != header.width * header.height * 4
    {
        return Err(parse_error("destination size differs from the movie's"));
    }
    let frame = frame_bytes(data, &header, frame)?;
    // The target decodes whole 8x8 blocks; other sizes go through a padded copy.
    let width = (header.width + 7) & !7;
    let height = (header.height + 7) & !7;
    if (width, height) == (header.width, header.height) {
        return decode_into(data, &header, frame, &mut image.rgba, width);
    }
    let mut padded = vec![0u8; width * height * 4];
    for (src, dst) in image
        .rgba
        .chunks_exact(header.width * 4)
        .zip(padded.chunks_exact_mut(width * 4))
    {
        dst[..src.len()].copy_from_slice(src);
    }
    decode_into(data, &header, frame, &mut padded, width)?;
    for (dst, src) in image
        .rgba
        .chunks_exact_mut(header.width * 4)
        .zip(padded.chunks_exact(width * 4))
    {
        dst.copy_from_slice(&src[..dst.len()]);
    }
    Ok(())
}

fn parse_header(data: &[u8]) -> Result<Header> {
    // sub_49B210: the magic, then version 0x10000 or 0x10001.
    if data.len() < FRAME_TABLE || &data[..16] != MAGIC {
        return Err(parse_error("missing header"));
    }
    let version = read_u32(data, 16)?;
    if !(0x10000..=0x10001).contains(&version) {
        return Err(EthornellError::UnsupportedFormat(format!(
            "BF_Movie version {version:#x} is not supported"
        )));
    }
    let width = read_u32(data, 20)? as usize;
    let height = read_u32(data, 24)? as usize;
    let bpp = read_u32(data, 28)?;
    let frame_count = read_u32(data, 40)? as usize;
    if !(1..=MAX_DIMENSION).contains(&width) || !(1..=MAX_DIMENSION).contains(&height) {
        return Err(parse_error("invalid size"));
    }
    // sub_49D500 converts three planes (Y, Cb, Cr) into 32-bit pixels.
    if bpp != 24 && bpp != 32 {
        return Err(EthornellError::UnsupportedFormat(format!(
            "BF_Movie with {bpp} bpp is not supported"
        )));
    }
    Ok(Header {
        version,
        width,
        height,
        bpp,
        frame_count,
    })
}

/// sub_49B640: a frame runs from its offset to the next frame's; the last one
/// runs to the end of the resource.
fn frame_bytes<'a>(data: &'a [u8], header: &Header, frame: u32) -> Result<&'a [u8]> {
    let frame = frame as usize;
    if frame >= header.frame_count {
        return Err(parse_error("frame out of range"));
    }
    let start = read_u32(data, FRAME_TABLE + 4 * frame)? as usize;
    let end = if frame + 1 < header.frame_count {
        read_u32(data, FRAME_TABLE + 4 * (frame + 1))? as usize
    } else {
        data.len()
    };
    data.get(start..end)
        .ok_or_else(|| parse_error("frame offsets out of range"))
}

/// sub_49CC40: quantization tables, the frame's two Huffman trees and its
/// strip table, then one job per 8-pixel strip and one for alpha (the target
/// runs them on a thread pool through sub_49D340). `rgba` is `width` pixels
/// wide (a multiple of 8) and a whole number of strips high.
fn decode_into(
    data: &[u8],
    header: &Header,
    frame: &[u8],
    rgba: &mut [u8],
    width: usize,
) -> Result<()> {
    let mut dct = [[0.0f32; 64]; 2];
    for (i, &quant) in data[QUANT_TABLES..FRAME_TABLE].iter().enumerate() {
        dct[i >> 6][i & 0x3f] = quant as f32 * DCT_TABLE[i & 0x3f];
    }
    let mut input = Reader::new(frame);
    let tree1 = HuffmanTree::new(&CbgV2Decoder::read_weight_table(&mut input, 0x10)?, true);
    let tree2 = HuffmanTree::new(&CbgV2Decoder::read_weight_table(&mut input, 0xb0)?, true);
    let stride = width * 4;
    let strips = rgba.len() / (stride * 8);
    let has_alpha = header.bpp == 32;
    // Strip offsets from the frame's start, then (32 bpp) the alpha's. The
    // encoder (sub_49B690) writes the alpha entry only with alpha, so without
    // it the last strip runs to the end of the frame.
    let mut offsets = Vec::with_capacity(strips + 1);
    for _ in 0..strips + usize::from(has_alpha) {
        offsets.push(input.read_i32()? as u32 as usize);
    }
    let decoder = BlockDecoder {
        input: frame,
        bpp: header.bpp as i32,
        width: width as i32,
        tree1,
        tree2,
        dct,
    };
    let colors = ColorTables::new();
    for (strip, pixels) in rgba.chunks_exact_mut(stride * 8).enumerate() {
        let start = offsets[strip];
        let end = offsets.get(strip + 1).copied().unwrap_or(frame.len());
        let bytes = frame
            .get(start..end)
            .ok_or_else(|| parse_error("strip offsets out of range"))?;
        decode_strip(&decoder, &colors, bytes, pixels, width / 8, stride)?;
    }
    if !has_alpha {
        // The target clears the alpha bytes of a frame without alpha
        // (sub_49D340); the portable RGBA store keeps it opaque instead, as
        // decode_cbg does for 24-bpp images.
        for alpha in rgba.iter_mut().skip(3).step_by(4) {
            *alpha = 0xff;
        }
        return Ok(());
    }
    let bytes = frame
        .get(offsets[strips]..)
        .ok_or_else(|| parse_error("alpha offset out of range"))?;
    // sub_49CC40 reads a mode word in front of version 0x10001's alpha;
    // version 0x10000 has only mode 1.
    let (mode, bytes) = if header.version == 0x10000 {
        (1, bytes)
    } else {
        (read_u32(bytes, 0)?, &bytes[4..])
    };
    match mode {
        1 => unpack_alpha_copies(bytes, rgba, stride),
        2 => unpack_alpha_blocks(bytes, rgba, stride, width / 8),
        _ => Err(EthornellError::UnsupportedFormat(format!(
            "BF_Movie alpha mode {mode} is not supported"
        ))),
    }
}

/// sub_49D340, a colour job: the strip's block mask (one bit per block, LSB
/// first), its coefficient count, the coefficients, then sub_49D4C0's IDCT
/// and colour conversion for each coded block (sub_49D500 is its plain-x87
/// variant). Blocks whose bit is clear keep their pixels.
fn decode_strip(
    decoder: &BlockDecoder<'_>,
    colors: &ColorTables,
    strip: &[u8],
    pixels: &mut [u8],
    blocks: usize,
    stride: usize,
) -> Result<()> {
    let mask_len = (blocks + 7) >> 3;
    let mask = strip
        .get(..mask_len)
        .ok_or_else(|| parse_error("truncated strip"))?;
    let is_coded = |block: usize| mask[block >> 3] & (1 << (block & 7)) != 0;
    let mut stream = MsbBitStream::new(Reader::new(&strip[mask_len..]));
    let count = CbgV2Decoder::read_int(&mut stream.input)?;
    if count == 0 {
        return Ok(());
    }
    // A Y, a Cb and a Cr plane of 64 coefficients for every coded block.
    let coded = (0..blocks).filter(|&block| is_coded(block)).count();
    if usize::try_from(count).ok() != Some(coded * 3 * 64) {
        return Err(parse_error("coefficient count differs from the block mask"));
    }
    let coefficients = decode_coefficients(decoder, &mut stream, coded * 3 * 64)?;
    let plane = coded * 64;
    for (index, block) in (0..blocks).filter(|&block| is_coded(block)).enumerate() {
        let mut ycbcr = [[0i16; 3]; 64];
        for channel in 0..3 {
            decoder.decode_dct(
                channel,
                &coefficients,
                channel * plane + index * 64,
                &mut ycbcr,
            )?;
        }
        for (i, &[y, cb, cr]) in ycbcr.iter().enumerate() {
            let p = (i >> 3) * stride + (block * 8 + (i & 7)) * 4;
            pixels[p..p + 3].copy_from_slice(&colors.rgb(y, cb, cr));
        }
    }
    Ok(())
}

/// sub_49FB40 and sub_49FD70: the DC codes, then from the next byte the AC
/// codes, both MSB first (sub_49FE90, sub_4A0010). Each stops when the strip's
/// bytes run out, as the target's byte-position checks do.
fn decode_coefficients(
    decoder: &BlockDecoder<'_>,
    stream: &mut MsbBitStream<'_>,
    count: usize,
) -> Result<Vec<i16>> {
    let end = stream.input.data.len() * 8;
    // `MsbBitStream` fetches whole bytes ahead of the bits it has used.
    let used = |stream: &MsbBitStream<'_>| stream.input.pos * 8 - stream.cached_bits as usize;
    let mut coefficients = vec![0i16; count];
    // Each DC is a difference from the previous block's, through the Y, Cb
    // and Cr planes in turn.
    let mut dc = 0i16;
    let mut i = 0;
    while i < count && used(stream) < end {
        let bits = decoder.tree1.decode_token(stream)?;
        dc = dc.wrapping_add(read_signed(stream, bits)?);
        coefficients[i] = dc;
        i += 64;
    }
    if (stream.cached_bits & 7) != 0 {
        stream.get_bits(stream.cached_bits & 7)?;
    }
    // AC codes are size << 4 | zero run, in zigzag order; 0 ends a block and
    // 0x0F skips sixteen zeros.
    i = 0;
    while i < count && used(stream) < end {
        let mut index = 1usize;
        while index < 64 {
            let code = decoder.tree2.decode_token(stream)?;
            if code == 0 {
                break;
            }
            if code == 0x0f {
                index += 0x10;
                continue;
            }
            index += code & 0x0f;
            if index >= 64 {
                return Err(parse_error("AC run past the end of a block"));
            }
            coefficients[i + BLOCK_FILL_ORDER[index] as usize] = read_signed(stream, code >> 4)?;
            index += 1;
        }
        i += 64;
    }
    Ok(coefficients)
}

/// `bits` bits of a value; with its top bit clear it is negative.
fn read_signed(stream: &mut MsbBitStream<'_>, bits: usize) -> Result<i16> {
    let mut value = stream.get_bits(bits as u32)? as i32;
    if bits != 0 && (value >> (bits - 1)) == 0 {
        value = (-1 << bits | value) + 1;
    }
    Ok(value as i16)
}

/// The tables sub_49B040 (the YCBFMovie2 constructor) builds. sub_49D500 adds
/// a pixel's entries to Y and 256.0, truncates, and clamps through a table of
/// 256 zeros, 0..=255 and 256 times 255.
struct ColorTables {
    red_cr: [f32; 256],
    green_cb: [f32; 256],
    green_cr: [f32; 256],
    blue_cb: [f32; 256],
}

impl ColorTables {
    fn new() -> Self {
        let mut tables = Self {
            red_cr: [0.0; 256],
            green_cb: [0.0; 256],
            green_cr: [0.0; 256],
            blue_cb: [0.0; 256],
        };
        for i in 0..256 {
            let value = i as f64 - 128.0;
            tables.red_cr[i] = (value * 1.402 + 0.5) as f32;
            tables.green_cb[i] = (value * -0.34414) as f32;
            tables.green_cr[i] = (0.5 - value * 0.71414) as f32;
            tables.blue_cb[i] = (value * 1.772 + 0.5) as f32;
        }
        tables
    }

    fn rgb(&self, y: i16, cb: i16, cr: i16) -> [u8; 3] {
        let y = f64::from(y);
        let (cb, cr) = (usize::from(cb as u8), usize::from(cr as u8));
        let clamp = |value: f64| ((value + 256.0) as i32 - 256).clamp(0, 255) as u8;
        [
            clamp(f64::from(self.red_cr[cr]) + y),
            clamp(f64::from(self.green_cb[cb]) + y + f64::from(self.green_cr[cr])),
            clamp(f64::from(self.blue_cb[cb]) + y),
        ]
    }
}

/// Alpha mode 1, sub_4A0930 (CompressedBG v2's alpha, as
/// `BlockDecoder::unpack_alpha`): eight flag bits per control byte, LSB first;
/// a clear bit stores one alpha byte, a set bit a 16-bit copy (x: 6 bits,
/// y: 3 bits, this row or up to 7 above, count - 3: 7 bits) of alpha already
/// drawn.
fn unpack_alpha_copies(input: &[u8], rgba: &mut [u8], stride: usize) -> Result<()> {
    let Some(&first) = input.first() else {
        return Ok(());
    };
    let mut control = first;
    let (mut src, mut dst, mut bit) = (1usize, 3usize, 0);
    while src < input.len() && dst < rgba.len() {
        if control & (1 << bit) != 0 {
            let word = input
                .get(src..src + 2)
                .map(|bytes| u16::from_le_bytes([bytes[0], bytes[1]]))
                .ok_or_else(|| parse_error("truncated alpha"))? as isize;
            let mut x = word & 0x3f;
            if x > 0x1f {
                x -= 0x40;
            }
            let mut y = (word >> 6) & 7;
            if y != 0 {
                y -= 8;
            }
            let offset = y * stride as isize + x * 4;
            for _ in 0..(word >> 9) + 3 {
                if dst >= rgba.len() {
                    break;
                }
                let from = dst
                    .checked_add_signed(offset)
                    .filter(|&from| from < rgba.len())
                    .ok_or_else(|| parse_error("alpha copy out of range"))?;
                rgba[dst] = rgba[from];
                dst += 4;
            }
            src += 2;
        } else {
            rgba[dst] = input[src];
            src += 1;
            dst += 4;
        }
        bit += 1;
        if bit == 8 {
            if src >= input.len() || dst >= rgba.len() {
                break;
            }
            control = input[src];
            src += 1;
            bit = 0;
        }
    }
    Ok(())
}

/// Alpha mode 2: sub_4A0B90 decodes a counted run of bytes with a 256-symbol
/// Huffman tree; sub_4A0650 reads them as one bit per 8x8 block (LSB first,
/// row by row over the frame), then 64 alpha bytes, row by row, for each
/// block whose bit is set. The other blocks keep their alpha.
fn unpack_alpha_blocks(
    input: &[u8],
    rgba: &mut [u8],
    stride: usize,
    blocks_per_row: usize,
) -> Result<()> {
    let mut reader = Reader::new(input);
    let length = reader.read_i32()? as u32 as usize;
    let tree = HuffmanTree::new(&CbgV2Decoder::read_weight_table(&mut reader, 0x100)?, true);
    let blocks = rgba.len() / (stride * 8) * blocks_per_row;
    // The encoder's mask length (sub_49B690).
    let mask_len = (blocks + 7) >> 3;
    if length < mask_len || length > mask_len + blocks * 64 {
        return Err(parse_error("invalid alpha block length"));
    }
    let mut stream = MsbBitStream::new(reader);
    let mut bytes = Vec::with_capacity(length);
    for _ in 0..length {
        bytes.push(tree.decode_token(&mut stream)? as u8);
    }
    let (mask, mut values) = bytes.split_at(mask_len);
    for block in 0..blocks {
        if mask[block >> 3] & (1 << (block & 7)) == 0 {
            continue;
        }
        let origin = block / blocks_per_row * 8 * stride + block % blocks_per_row * 32 + 3;
        for row in 0..8 {
            let (line, rest) = values
                .split_at_checked(8)
                .ok_or_else(|| parse_error("alpha block values missing"))?;
            for (x, &value) in line.iter().enumerate() {
                rgba[origin + row * stride + x * 4] = value;
            }
            values = rest;
        }
    }
    Ok(())
}

#[cfg(test)]
mod tests {
    use super::*;

    /// Y, Cb and Cr coefficients of one block, in natural (row-major) order.
    type Block = [[i16; 64]; 3];
    /// Huffman symbols and the value bits after each.
    type Tokens = Vec<(usize, u32)>;

    fn block(y: &[(usize, i16)], cb: i16, cr: i16) -> Block {
        let mut block = [[0i16; 64]; 3];
        for &(index, value) in y {
            block[0][index] = value;
        }
        block[1][0] = cb;
        block[2][0] = cr;
        block
    }

    /// MSB-first bit writer, the inverse of `MsbBitStream`.
    #[derive(Default)]
    struct Bits {
        bytes: Vec<u8>,
        used: usize,
    }

    impl Bits {
        fn put(&mut self, value: u32, count: u32) {
            for shift in (0..count).rev() {
                if self.used.is_multiple_of(8) {
                    self.bytes.push(0);
                }
                let bit = ((value >> shift) & 1) as u8;
                *self.bytes.last_mut().unwrap() |= bit << (7 - self.used % 8);
                self.used += 1;
            }
        }

        fn align(&mut self) {
            self.used = self.used.next_multiple_of(8);
        }
    }

    fn varint(out: &mut Vec<u8>, mut value: u32) {
        loop {
            let byte = (value & 0x7f) as u8;
            value >>= 7;
            if value == 0 {
                out.push(byte);
                return;
            }
            out.push(byte | 0x80);
        }
    }

    /// Each symbol's code and length, read off the decoder's own tree.
    fn codes(weights: &[u32]) -> Vec<Option<(u32, u32)>> {
        let tree = HuffmanTree::new(weights, true);
        let mut codes = vec![None; weights.len()];
        let mut pending = vec![(tree.nodes.len() - 1, 0, 0)];
        while let Some((index, code, length)) = pending.pop() {
            let Some(node) = tree.nodes.get(index) else {
                continue;
            };
            if !node.is_parent {
                codes[index] = Some((code, length));
                continue;
            }
            pending.push((node.left_index, code << 1, length + 1));
            pending.push((node.right_index, code << 1 | 1, length + 1));
        }
        codes
    }

    /// The inverse of `read_signed`: a size and its bits.
    fn signed(value: i32) -> (usize, u32) {
        let size = 32 - value.unsigned_abs().leading_zeros();
        let bits = if value < 0 {
            value + (1 << size) - 1
        } else {
            value
        };
        (size as usize, bits as u32)
    }

    /// The DC and AC symbols and value bits of one strip's coded blocks.
    fn tokens(blocks: &[Block]) -> (Tokens, Tokens) {
        let planes: Vec<&[i16; 64]> = (0..3)
            .flat_map(|channel| blocks.iter().map(move |block| &block[channel]))
            .collect();
        let mut dc = Vec::new();
        let mut previous = 0;
        for plane in &planes {
            dc.push(signed(i32::from(plane[0]) - previous));
            previous = i32::from(plane[0]);
        }
        let mut ac = Vec::new();
        for plane in &planes {
            let mut run = 0;
            let mut last = 0;
            for (index, &natural) in BLOCK_FILL_ORDER.iter().enumerate().skip(1) {
                let value = plane[natural as usize];
                if value == 0 {
                    run += 1;
                    continue;
                }
                while run >= 16 {
                    ac.push((0x0f, 0));
                    run -= 16;
                }
                let (size, bits) = signed(i32::from(value));
                ac.push((size << 4 | run, bits));
                run = 0;
                last = index;
            }
            if last != 63 {
                ac.push((0, 0));
            }
        }
        (dc, ac)
    }

    /// One frame: its strips (block mask, coded blocks) and alpha section.
    fn frame(strips: &[(Vec<u8>, Vec<Block>)], alpha: Option<Vec<u8>>) -> Vec<u8> {
        let tokens: Vec<_> = strips.iter().map(|(_, blocks)| tokens(blocks)).collect();
        let mut dc_weights = vec![0u32; 0x10];
        let mut ac_weights = vec![0u32; 0xb0];
        for (dc, ac) in &tokens {
            dc.iter().for_each(|&(symbol, _)| dc_weights[symbol] += 1);
            ac.iter().for_each(|&(symbol, _)| ac_weights[symbol] += 1);
        }
        let (dc_codes, ac_codes) = (codes(&dc_weights), codes(&ac_weights));
        let mut bodies = Vec::new();
        for ((mask, blocks), (dc, ac)) in strips.iter().zip(&tokens) {
            let mut body = mask.clone();
            varint(&mut body, (blocks.len() * 192) as u32);
            let mut bits = Bits::default();
            for &(symbol, value) in dc {
                let (code, length) = dc_codes[symbol].unwrap();
                bits.put(code, length);
                bits.put(value, symbol as u32);
            }
            bits.align();
            for &(symbol, value) in ac {
                let (code, length) = ac_codes[symbol].unwrap();
                bits.put(code, length);
                bits.put(value, (symbol >> 4) as u32);
            }
            body.extend(bits.bytes);
            bodies.push(body);
        }
        let mut out = Vec::new();
        for &weight in dc_weights.iter().chain(&ac_weights) {
            varint(&mut out, weight);
        }
        let entries = bodies.len() + usize::from(alpha.is_some());
        let mut offset = out.len() + 4 * entries;
        for body in &bodies {
            out.extend((offset as u32).to_le_bytes());
            offset += body.len();
        }
        if alpha.is_some() {
            out.extend((offset as u32).to_le_bytes());
        }
        bodies.into_iter().for_each(|body| out.extend(body));
        out.extend(alpha.unwrap_or_default());
        out
    }

    fn movie(version: u32, width: u32, height: u32, bpp: u32, frames: &[Vec<u8>]) -> Vec<u8> {
        let mut out = MAGIC.to_vec();
        let count = frames.len() as u32;
        for value in [version, width, height, bpp, 2, 3, count, 30, 0, 0, 0, 0] {
            out.extend(value.to_le_bytes());
        }
        // Every quantizer 1: a block's level is 128 + DC / 8.
        out.extend([1u8; 128]);
        let mut offset = FRAME_TABLE + 4 * frames.len();
        for frame in frames {
            out.extend((offset as u32).to_le_bytes());
            offset += frame.len();
        }
        frames.iter().for_each(|frame| out.extend(frame));
        out
    }

    /// Mode 1 alpha filling `pixels` with `value`: one byte, then copies of
    /// 130 pixels from the pixel to the left (x = -1, y = 0); the last copy
    /// stops at the end of the frame.
    fn alpha_fill(value: u8, pixels: usize, mode_word: bool) -> Vec<u8> {
        let copies = (pixels - 1).div_ceil(130);
        assert!(copies <= 7, "one control byte");
        let mut out = Vec::new();
        if mode_word {
            out.extend(1u32.to_le_bytes());
        }
        out.extend([(((1u16 << copies) - 1) << 1) as u8, value]);
        for _ in 0..copies {
            out.extend(((127 << 9 | 0x3f) as u16).to_le_bytes());
        }
        out
    }

    /// Mode 2 alpha: the block mask, then 64 values for each block set.
    fn alpha_blocks(mask: &[u8], values: &[u8]) -> Vec<u8> {
        let bytes: Vec<u8> = mask.iter().chain(values).copied().collect();
        let mut weights = vec![0u32; 0x100];
        bytes.iter().for_each(|&byte| weights[byte as usize] += 1);
        let codes = codes(&weights);
        let mut out = 2u32.to_le_bytes().to_vec();
        out.extend((bytes.len() as u32).to_le_bytes());
        weights.iter().for_each(|&weight| varint(&mut out, weight));
        let mut bits = Bits::default();
        for &byte in &bytes {
            let (code, length) = codes[byte as usize].unwrap();
            bits.put(code, length);
        }
        out.extend(bits.bytes);
        out
    }

    fn image(width: u32, height: u32, fill: u8) -> DecodedImage {
        DecodedImage {
            width,
            height,
            rgba: vec![fill; (width * height * 4) as usize],
        }
    }

    fn pixel(image: &DecodedImage, x: usize, y: usize) -> [u8; 4] {
        let p = (y * image.width as usize + x) * 4;
        image.rgba[p..p + 4].try_into().unwrap()
    }

    /// Whether every pixel of the 8x8 block at (`bx`, `by`) is `expected`.
    fn block_is(image: &DecodedImage, bx: usize, by: usize, expected: [u8; 4]) -> bool {
        (0..64).all(|i| pixel(image, bx * 8 + i % 8, by * 8 + i / 8) == expected)
    }

    /// The colour formula of sub_49B040's tables, worked out independently.
    fn ycbcr_to_rgb(y: f64, cb: f64, cr: f64) -> [u8; 3] {
        let clamp = |value: f64| value.floor().clamp(0.0, 255.0) as u8;
        [
            clamp(y + 1.402 * (cr - 128.0) + 0.5),
            clamp(y - 0.34414 * (cb - 128.0) - 0.71414 * (cr - 128.0) + 0.5),
            clamp(y + 1.772 * (cb - 128.0) + 0.5),
        ]
    }

    /// A 16 x 8 movie: frame 0 codes both blocks (grey 178, and Y 128 with
    /// Cb 108 and Cr 148); frame 1 codes only block 1 (grey 78) and leaves
    /// every block's alpha.
    fn two_frames(version: u32) -> Vec<u8> {
        let key = frame(
            &[(
                vec![0b11],
                vec![block(&[(0, 400)], 0, 0), block(&[], -160, 160)],
            )],
            Some(alpha_fill(0x80, 128, version == 0x10001)),
        );
        let next = frame(
            &[(vec![0b10], vec![block(&[(0, -400)], 0, 0)])],
            Some(alpha_blocks(&[0], &[])),
        );
        movie(version, 16, 8, 32, &[key, next])
    }

    #[test]
    fn a_key_frame_draws_every_block_in_its_colours() {
        let mut image = image(16, 8, 0);
        decode_frame(&two_frames(0x10001), 0, &mut image).unwrap();
        assert!(block_is(&image, 0, 0, [178, 178, 178, 0x80]));
        let [r, g, b] = ycbcr_to_rgb(128.0, 108.0, 148.0);
        assert_eq!([r, g, b], [156, 121, 93]);
        assert!(block_is(&image, 1, 0, [r, g, b, 0x80]));
    }

    #[test]
    fn a_later_frame_redraws_only_its_masked_blocks() {
        let movie = two_frames(0x10001);
        let mut image = image(16, 8, 0);
        decode_frame(&movie, 0, &mut image).unwrap();
        decode_frame(&movie, 1, &mut image).unwrap();
        assert!(block_is(&image, 0, 0, [178, 178, 178, 0x80]));
        assert!(block_is(&image, 1, 0, [78, 78, 78, 0x80]));

        let mut image = self::image(16, 8, 7);
        decode_frame(&movie, 1, &mut image).unwrap();
        assert!(block_is(&image, 0, 0, [7; 4]));
        assert!(block_is(&image, 1, 0, [78, 78, 78, 7]));
    }

    #[test]
    fn an_ac_coefficient_varies_along_its_axis() {
        // Natural index 1 is horizontal frequency 1, index 8 vertical.
        let movie = movie(
            0x10001,
            16,
            8,
            32,
            &[frame(
                &[(
                    vec![0b11],
                    vec![block(&[(1, 40)], 0, 0), block(&[(8, 40)], 0, 0)],
                )],
                Some(alpha_fill(0xff, 128, true)),
            )],
        );
        let mut image = image(16, 8, 0);
        decode_frame(&movie, 0, &mut image).unwrap();
        for y in 0..8 {
            for x in 0..8 {
                assert_eq!(
                    pixel(&image, x, y),
                    pixel(&image, x, 0),
                    "block 0 varies along x"
                );
                assert_eq!(
                    pixel(&image, 8 + x, y),
                    pixel(&image, 8, y),
                    "block 1 along y"
                );
            }
        }
        assert!(pixel(&image, 0, 0)[0] > pixel(&image, 7, 0)[0]);
        assert!(pixel(&image, 8, 0)[0] > pixel(&image, 8, 7)[0]);
    }

    #[test]
    fn coefficients_reach_their_zigzag_positions() {
        // Positive and negative values, a 16-zero skip (zigzag 4 to 20) and a
        // last coefficient at zigzag 63, which ends its block without an end code.
        let blocks = [
            block(
                &[
                    (0, -300),
                    (1, 25),
                    (8, -7),
                    (2, 3),
                    (BLOCK_FILL_ORDER[20] as usize, -2),
                ],
                -50,
                90,
            ),
            block(&[(0, 120), (63, 1), (9, -12)], 30, -20),
        ];
        let movie = movie(
            0x10001,
            16,
            8,
            32,
            &[frame(
                &[(vec![0b11], blocks.to_vec())],
                Some(alpha_fill(0xff, 128, true)),
            )],
        );
        let mut image = image(16, 8, 0);
        decode_frame(&movie, 0, &mut image).unwrap();

        let mut dct = [[0.0f32; 64]; 2];
        for i in 0..128 {
            dct[i >> 6][i & 0x3f] = DCT_TABLE[i & 0x3f];
        }
        let decoder = BlockDecoder {
            input: &[],
            bpp: 32,
            width: 16,
            tree1: HuffmanTree::new(&[1], true),
            tree2: HuffmanTree::new(&[1], true),
            dct,
        };
        for (bx, coded) in blocks.iter().enumerate() {
            let flat: Vec<i16> = coded.iter().flatten().copied().collect();
            let mut ycbcr = [[0i16; 3]; 64];
            for channel in 0..3 {
                decoder
                    .decode_dct(channel, &flat, channel * 64, &mut ycbcr)
                    .unwrap();
            }
            for (i, &[y, cb, cr]) in ycbcr.iter().enumerate() {
                let [r, g, b] = ColorTables::new().rgb(y, cb, cr);
                assert_eq!(pixel(&image, bx * 8 + i % 8, i / 8), [r, g, b, 0xff]);
            }
        }
    }

    #[test]
    fn alpha_modes_fill_every_pixel_or_only_masked_blocks() {
        let key = frame(
            &[(vec![0b11], vec![block(&[], 0, 0), block(&[], 0, 0)])],
            Some(alpha_fill(0x80, 128, true)),
        );
        let next = frame(
            &[(vec![0], Vec::new())],
            Some(alpha_blocks(&[0b10], &[0x40; 64])),
        );
        let movie = movie(0x10001, 16, 8, 32, &[key, next]);
        let mut image = image(16, 8, 0);
        decode_frame(&movie, 0, &mut image).unwrap();
        decode_frame(&movie, 1, &mut image).unwrap();
        assert!(block_is(&image, 0, 0, [128, 128, 128, 0x80]));
        assert!(block_is(&image, 1, 0, [128, 128, 128, 0x40]));
    }

    #[test]
    fn version_0x10000_alpha_has_no_mode_word() {
        let mut image = image(16, 8, 0);
        decode_frame(&two_frames(0x10000), 0, &mut image).unwrap();
        assert!(
            image
                .rgba
                .iter()
                .skip(3)
                .step_by(4)
                .all(|&alpha| alpha == 0x80)
        );
    }

    #[test]
    fn a_frame_without_alpha_is_opaque() {
        let movie = movie(
            0x10001,
            16,
            8,
            24,
            &[frame(&[(vec![0b01], vec![block(&[(0, 400)], 0, 0)])], None)],
        );
        let mut image = image(16, 8, 9);
        decode_frame(&movie, 0, &mut image).unwrap();
        assert!(block_is(&image, 0, 0, [178, 178, 178, 0xff]));
        assert!(block_is(&image, 1, 0, [9, 9, 9, 0xff]));
    }

    #[test]
    fn other_sizes_decode_through_a_padded_copy() {
        // 12 x 10: two blocks per row, two strips; the second strip codes nothing.
        let key = frame(
            &[
                (
                    vec![0b11],
                    vec![block(&[(0, 400)], 0, 0), block(&[(0, -400)], 0, 0)],
                ),
                (vec![0], Vec::new()),
            ],
            Some(alpha_fill(0xff, 256, true)),
        );
        let movie = movie(0x10001, 12, 10, 32, &[key]);
        let mut image = image(12, 10, 5);
        decode_frame(&movie, 0, &mut image).unwrap();
        assert_eq!(image.rgba.len(), 12 * 10 * 4);
        assert_eq!(pixel(&image, 7, 7), [178, 178, 178, 0xff]);
        assert_eq!(pixel(&image, 11, 0), [78, 78, 78, 0xff]);
        assert_eq!(pixel(&image, 11, 9), [5, 5, 5, 0xff]);
    }

    #[test]
    fn bad_input_fails_without_panicking() {
        let movie = two_frames(0x10001);
        let mut image = image(16, 8, 0);
        assert!(decode_frame(&movie, 2, &mut image).is_err());
        assert!(decode_frame(&movie, 0, &mut self::image(8, 8, 0)).is_err());
        let mut other = movie.clone();
        other[16..20].copy_from_slice(&0x10002u32.to_le_bytes());
        assert!(decode_frame(&other, 0, &mut image).is_err());
        other = movie.clone();
        other[0] = b'b';
        assert!(decode_frame(&other, 0, &mut image).is_err());
        for len in 0..movie.len() {
            let _ = decode_frame(&movie[..len], 0, &mut image);
            let _ = decode_frame(&movie[..len], 1, &mut image);
        }
        for i in FRAME_TABLE..movie.len() {
            let mut damaged = movie.clone();
            damaged[i] ^= 0xa5;
            let _ = decode_frame(&damaged, 0, &mut image);
            let _ = decode_frame(&damaged, 1, &mut image);
        }
    }
}

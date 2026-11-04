use crate::{DecodedImage, parse_cbg_metadata};
use ethornell_core::{EthornellError, Result};

pub(crate) mod bf_movie;

pub fn decode_cbg(data: &[u8]) -> Result<DecodedImage> {
    let meta = parse_cbg_metadata(data)?;
    let version = meta.version.unwrap_or(0);
    if version > 2 {
        return Err(EthornellError::UnsupportedFormat(format!(
            "CBG version {version} decode is not implemented"
        )));
    }
    match (version, meta.bpp) {
        (1, 8 | 16 | 24 | 32) | (2, 8 | 24 | 32) => {}
        bpp => {
            return Err(EthornellError::UnsupportedFormat(format!(
                "CBG version/bpp {bpp:?} is not supported"
            )));
        }
    }

    let mut decoder = CbgV2Decoder::new(data)?;
    decoder.unpack()
}

/// sub_465880's decoded CBG buffer: the original 16-byte image descriptor
/// followed by native BGR/word/gray bytes. This deliberately avoids a display
/// RGBA round-trip for 16-bit data.
pub fn decode_native_buffer(data: &[u8]) -> Result<Vec<u8>> {
    let meta = parse_cbg_metadata(data)?;
    let mut decoder = CbgV2Decoder::new(data)?;
    let pixels = if meta.version.unwrap_or(0) < 2 {
        let raw = decoder.unpack_v1_pixels()?;
        if meta.bpp == 24 {
            raw.chunks_exact(3).flat_map(|p| [p[0],p[1],p[2],0]).collect()
        } else { raw }
    } else {
        let image = decode_cbg(data)?;
        match meta.bpp {
            8 => image.rgba.chunks_exact(4).map(|p| p[0]).collect(),
            24 => image.rgba.chunks_exact(4).flat_map(|p| [p[2],p[1],p[0]]).collect(),
            32 => image.rgba.chunks_exact(4).flat_map(|p| [p[2],p[1],p[0],p[3]]).collect(),
            _ => unreachable!("decode_cbg validates bit depth"),
        }
    };
    let mut output = data[16..32].to_vec();
    if meta.version.unwrap_or(0) < 2 && meta.bpp == 24 {
        output[4..6].copy_from_slice(&32u16.to_le_bytes());
        output[8..10].copy_from_slice(&7u16.to_le_bytes());
    }
    output.extend_from_slice(&pixels);
    // The v2 24-bit target reports a four-byte allocation even though its
    // decoder writes packed BGR. Its uninitialized tail is zero-filled here.
    let bytes_per_pixel = if meta.bpp == 24 { 4 } else { meta.bpp / 8 };
    output.resize(16 + meta.width as usize * meta.height as usize * bytes_per_pixel as usize, 0);
    Ok(output)
}

#[derive(Debug, Clone)]
struct CbgHeader {
    width: u32,
    height: u32,
    bpp: u32,
    version: u32,
    intermediate_length: u32,
    enc_length: u32,
    check_sum: u8,
    check_xor: u8,
}

struct CbgV2Decoder<'a> {
    input: Reader<'a>,
    header: CbgHeader,
    key: u32,
}

impl<'a> CbgV2Decoder<'a> {
    fn new(data: &'a [u8]) -> Result<Self> {
        if data.len() < 0x30 || !data.starts_with(b"CompressedBG___") {
            return Err(EthornellError::UnsupportedFormat(
                "missing CompressedBG___ header".into(),
            ));
        }
        let meta = parse_cbg_metadata(data)?;
        let key = meta.key.unwrap_or(0);
        Ok(Self {
            input: Reader::at(data, 0x30),
            header: CbgHeader {
                width: meta.width,
                height: meta.height,
                bpp: meta.bpp,
                version: meta.version.unwrap_or(0),
                intermediate_length: meta.intermediate_length.unwrap_or(0),
                enc_length: meta.encoded_length.unwrap_or(0),
                check_sum: meta.checksum.unwrap_or(0) as u8,
                check_xor: meta.xor_check.unwrap_or(0) as u8,
            },
            key,
        })
    }

    fn unpack(&mut self) -> Result<DecodedImage> {
        if self.header.version < 2 {
            return self.unpack_v1();
        }
        self.unpack_v2()
    }

    fn unpack_v1_pixels(&mut self) -> Result<Vec<u8>> {
        let encoded = self.read_encoded()?;
        let mut encoded_reader = Reader::new(&encoded);
        let weights = Self::read_weight_table(&mut encoded_reader, 0x100)?;
        let tree = HuffmanTree::new(&weights, false);
        let mut packed = vec![0u8; self.header.intermediate_length as usize];
        let mut bitstream = MsbBitStream::new(self.input.clone());
        for byte in &mut packed {
            *byte = tree.decode_token(&mut bitstream)? as u8;
        }

        let pixel_size = (self.header.bpp / 8) as usize;
        if pixel_size == 0 {
            return Err(EthornellError::Parse("CBG v1 pixel size is zero".into()));
        }
        let stride = self.header.width as usize * pixel_size;
        let mut sampled = vec![0u8; stride * self.header.height as usize];
        Self::unpack_zeros(&packed, &mut sampled);
        Self::reverse_average_sampling(
            &mut sampled,
            self.header.width as usize,
            self.header.height as usize,
            stride,
            pixel_size,
        );
        Ok(sampled)
    }

    fn unpack_v1(&mut self) -> Result<DecodedImage> {
        let sampled = self.unpack_v1_pixels()?;
        let mut rgba = vec![0u8; self.header.width as usize * self.header.height as usize * 4];
        match self.header.bpp {
            32 => {
                for (src, dst) in sampled.chunks_exact(4).zip(rgba.chunks_exact_mut(4)) {
                    dst[0] = src[2];
                    dst[1] = src[1];
                    dst[2] = src[0];
                    dst[3] = src[3];
                }
            }
            24 => {
                for (src, dst) in sampled.chunks_exact(3).zip(rgba.chunks_exact_mut(4)) {
                    dst[0] = src[2];
                    dst[1] = src[1];
                    dst[2] = src[0];
                    dst[3] = 0xff;
                }
            }
            8 => {
                for (src, dst) in sampled.iter().zip(rgba.chunks_exact_mut(4)) {
                    dst[0] = *src;
                    dst[1] = *src;
                    dst[2] = *src;
                    dst[3] = 0xff;
                }
            }
            16 => {
                for (src, dst) in sampled.chunks_exact(2).zip(rgba.chunks_exact_mut(4)) {
                    let pixel = u16::from_le_bytes([src[0], src[1]]);
                    let blue = ((pixel & 0x1f) as u32 * 255 / 31) as u8;
                    let green = (((pixel >> 5) & 0x3f) as u32 * 255 / 63) as u8;
                    let red = (((pixel >> 11) & 0x1f) as u32 * 255 / 31) as u8;
                    dst[0] = red;
                    dst[1] = green;
                    dst[2] = blue;
                    dst[3] = 0xff;
                }
            }
            _ => unreachable!(),
        }
        Ok(DecodedImage {
            width: self.header.width,
            height: self.header.height,
            rgba,
        })
    }

    fn unpack_v2(&mut self) -> Result<DecodedImage> {
        if self.header.enc_length < 0x80 {
            return Err(EthornellError::Parse(format!(
                "invalid CBG v2 encoded length: {}",
                self.header.enc_length
            )));
        }
        let dct_data = self.read_encoded()?;
        let mut dct = [[0.0f32; 64]; 2];
        for i in 0..0x80usize {
            dct[i >> 6][i & 0x3f] = dct_data[i] as f32 * DCT_TABLE[i & 0x3f];
        }

        let base_offset = self.input.pos;
        let tree1 = HuffmanTree::new(&Self::read_weight_table(&mut self.input, 0x10)?, true);
        let tree2 = HuffmanTree::new(&Self::read_weight_table(&mut self.input, 0xb0)?, true);
        let width = ((self.header.width as i32 + 7) & !7).max(8);
        let height = ((self.header.height as i32 + 7) & !7).max(8);
        let y_blocks = height / 8;

        // sub_49D7B0 reads y_blocks + 1 offsets, counted from the start of
        // the weight tables. The engine's own encoder (sub_49C200) writes
        // only y_blocks of them for 24-bit images, so strips are addressed
        // from that start rather than from the end of the table.
        let mut offsets = Vec::with_capacity((y_blocks + 1) as usize);
        for _ in 0..=y_blocks {
            offsets.push(self.input.read_i32()?);
        }
        let input = self.input.data[base_offset..].to_vec();
        let pad_skip = ((width >> 3) + 7) >> 3;
        let mut output = vec![0u8; (width * height * 4) as usize];
        let block_decoder = BlockDecoder {
            input: &input,
            bpp: self.header.bpp as i32,
            width,
            tree1,
            tree2,
            dct,
        };

        let mut dst = 0i32;
        for i in 0..y_blocks {
            let block_offset = offsets[i as usize] + pad_skip;
            let next_offset = if i + 1 == y_blocks {
                input.len() as i32
            } else {
                offsets[(i + 1) as usize]
            };
            if block_offset >= 0 && next_offset > block_offset {
                block_decoder.unpack_block(
                    block_offset,
                    next_offset - block_offset,
                    dst,
                    &mut output,
                )?;
            }
            dst += width * 32;
        }

        let has_alpha = if self.header.bpp == 32 {
            block_decoder.unpack_alpha(offsets[y_blocks as usize], &mut output)?
        } else {
            false
        };
        if !has_alpha {
            for alpha in output.chunks_exact_mut(4).map(|px| &mut px[3]) {
                *alpha = 0xff;
            }
        }

        let mut rgba = vec![0u8; self.header.width as usize * self.header.height as usize * 4];
        for y in 0..self.header.height as usize {
            for x in 0..self.header.width as usize {
                let src = (y * width as usize + x) * 4;
                let dst = (y * self.header.width as usize + x) * 4;
                rgba[dst] = output[src + 2];
                rgba[dst + 1] = output[src + 1];
                rgba[dst + 2] = output[src];
                rgba[dst + 3] = output[src + 3];
            }
        }

        Ok(DecodedImage {
            width: self.header.width,
            height: self.header.height,
            rgba,
        })
    }

    fn unpack_zeros(input: &[u8], output: &mut [u8]) {
        let mut dst = 0usize;
        let mut dec_zero = false;
        let mut src = 0usize;
        while dst < output.len() {
            let mut code_length = 0usize;
            let mut count = 0usize;
            loop {
                if src >= input.len() {
                    return;
                }
                let code = input[src];
                src += 1;
                count |= ((code & 0x7f) as usize) << code_length;
                code_length += 7;
                if code & 0x80 == 0 {
                    break;
                }
            }
            if dst + count > output.len() {
                break;
            }
            if dec_zero {
                output[dst..dst + count].fill(0);
            } else {
                if src + count > input.len() {
                    break;
                }
                output[dst..dst + count].copy_from_slice(&input[src..src + count]);
                src += count;
            }
            dec_zero = !dec_zero;
            dst += count;
        }
    }

    fn reverse_average_sampling(
        output: &mut [u8],
        width: usize,
        height: usize,
        stride: usize,
        pixel_size: usize,
    ) {
        for y in 0..height {
            let line = y * stride;
            for x in 0..width {
                let pixel = line + x * pixel_size;
                for p in 0..pixel_size {
                    let mut avg = 0u32;
                    if x > 0 {
                        avg = avg.wrapping_add(output[pixel + p - pixel_size] as u32);
                    }
                    if y > 0 {
                        avg = avg.wrapping_add(output[pixel + p - stride] as u32);
                    }
                    if x > 0 && y > 0 {
                        avg /= 2;
                    }
                    if avg != 0 {
                        output[pixel + p] = output[pixel + p].wrapping_add(avg as u8);
                    }
                }
            }
        }
    }

    fn read_encoded(&mut self) -> Result<Vec<u8>> {
        let mut output = self.input.read_bytes(self.header.enc_length as usize)?;
        let mut sum = 0u8;
        let mut xor = 0u8;
        for byte in &mut output {
            *byte = byte.wrapping_sub(self.update_key());
            sum = sum.wrapping_add(*byte);
            xor ^= *byte;
        }
        if sum != self.header.check_sum || xor != self.header.check_xor {
            return Err(EthornellError::Parse(format!(
                "CBG checksum mismatch: sum={sum} expected={} xor={xor} expected={}",
                self.header.check_sum, self.header.check_xor
            )));
        }
        Ok(output)
    }

    fn update_key(&mut self) -> u8 {
        let v0 = 20021u32.wrapping_mul(self.key & 0xffff);
        let mut v1 = self.key >> 16;
        v1 = v1
            .wrapping_mul(20021)
            .wrapping_add(self.key.wrapping_mul(346));
        v1 = v1.wrapping_add(v0 >> 16) & 0xffff;
        self.key = (v1 << 16).wrapping_add(v0 & 0xffff).wrapping_add(1);
        v1 as u8
    }

    fn read_int(input: &mut Reader<'_>) -> Result<i32> {
        let mut value = 0i32;
        let mut code_length = 0;
        loop {
            let code = input.read_i8()?;
            if code_length >= 32 {
                return Err(EthornellError::Parse("CBG variable int is too long".into()));
            }
            value |= ((code & 0x7f) as i32) << code_length;
            code_length += 7;
            if code & -128 == 0 {
                break;
            }
        }
        Ok(value)
    }

    fn read_weight_table(input: &mut Reader<'_>, length: usize) -> Result<Vec<u32>> {
        let mut weights = Vec::with_capacity(length);
        for _ in 0..length {
            weights.push(Self::read_int(input)? as u32);
        }
        Ok(weights)
    }
}

struct BlockDecoder<'a> {
    input: &'a [u8],
    bpp: i32,
    width: i32,
    tree1: HuffmanTree,
    tree2: HuffmanTree,
    dct: [[f32; 64]; 2],
}

impl BlockDecoder<'_> {
    fn unpack_block(&self, offset: i32, length: i32, dst: i32, output: &mut [u8]) -> Result<()> {
        let start = offset as usize;
        let end = offset.saturating_add(length) as usize;
        if end > self.input.len() || start >= end {
            return Ok(());
        }
        let mut reader = MsbBitStream::new(Reader::new(&self.input[start..end]));
        let block_size = CbgV2Decoder::read_int(&mut reader.input)?;
        if block_size == -1 {
            return Ok(());
        }
        if block_size < 0 || block_size as usize > 128 * 1024 * 1024 {
            return Err(EthornellError::Parse(format!(
                "invalid CBG block size: {block_size}"
            )));
        }

        // The codes run to the strip's last bit, as the target reads them
        // (sub_49FB40, sub_49FD70): the reader may already hold the last byte
        // while bits of it are still unread.
        let strip_bits = reader.input.data.len() * 8;
        let mut color_data = vec![0i16; block_size as usize];
        let mut acc = 0i32;
        let mut i = 0i32;
        while i < block_size && reader.bits_used() < strip_bits {
            let count = self.tree1.decode_token(&mut reader)?;
            if count != 0 {
                let mut v = reader.get_bits(count as u32)? as i32;
                if (v >> (count - 1)) == 0 {
                    v = (-1 << count | v) + 1;
                }
                acc += v;
            }
            color_data[i as usize] = acc as i16;
            i += 64;
        }
        if (reader.cached_bits & 7) != 0 {
            reader.get_bits(reader.cached_bits & 7)?;
        }

        i = 0;
        while i < block_size && reader.bits_used() < strip_bits {
            let mut index = 1usize;
            while index < 64 && reader.bits_used() < strip_bits {
                let code = self.tree2.decode_token(&mut reader)?;
                if code == 0 {
                    break;
                }
                if code == 0xf {
                    index += 0x10;
                    continue;
                }
                index += code & 0xf;
                if index >= BLOCK_FILL_ORDER.len() {
                    break;
                }
                let bits = code >> 4;
                let mut v = reader.get_bits(bits as u32)? as i32;
                if bits != 0 && (v >> (bits - 1)) == 0 {
                    v = (-1 << bits | v) + 1;
                }
                let data_index = i as usize + BLOCK_FILL_ORDER[index] as usize;
                if data_index < color_data.len() {
                    color_data[data_index] = v as i16;
                }
                index += 1;
            }
            i += 64;
        }

        if self.bpp == 8 {
            self.decode_grayscale(&color_data, dst, output)
        } else {
            self.decode_rgb(&color_data, dst, output)
        }
    }

    fn decode_rgb(&self, data: &[i16], dst: i32, output: &mut [u8]) -> Result<()> {
        let block_count = self.width / 8;
        let mut dst = dst as usize;
        for i in 0..block_count {
            let mut src = (i * 64) as usize;
            let mut ycbcr_block = [[0i16; 3]; 64];
            for channel in 0..3 {
                self.decode_dct(channel, data, src, &mut ycbcr_block)?;
                src += (self.width * 8) as usize;
            }
            for (j, ycbcr) in ycbcr_block.iter().enumerate() {
                let cy = ycbcr[0] as f32;
                let cb = ycbcr[1] as f32;
                let cr = ycbcr[2] as f32;
                let r = cy + 1.402f32 * cr - 178.956f32;
                let g = cy - 0.34414f32 * cb - 0.71414f32 * cr + 135.95984f32;
                let b = cy + 1.772f32 * cb - 226.316f32;
                let y = j >> 3;
                let x = j & 7;
                let p = (y * self.width as usize + x) * 4 + dst;
                if p + 2 < output.len() {
                    output[p] = Self::float_to_byte(b);
                    output[p + 1] = Self::float_to_byte(g);
                    output[p + 2] = Self::float_to_byte(r);
                }
            }
            dst += 32;
        }
        Ok(())
    }

    fn decode_grayscale(&self, data: &[i16], dst: i32, output: &mut [u8]) -> Result<()> {
        let block_count = self.width / 8;
        let mut dst = dst as usize;
        let mut src = 0usize;
        for _ in 0..block_count {
            let mut ycbcr_block = [[0i16; 3]; 64];
            self.decode_dct(0, data, src, &mut ycbcr_block)?;
            src += 64;
            for (j, block) in ycbcr_block.iter().enumerate() {
                let y = j >> 3;
                let x = j & 7;
                let p = (y * self.width as usize + x) * 4 + dst;
                if p + 2 < output.len() {
                    let value = block[0] as u8;
                    output[p] = value;
                    output[p + 1] = value;
                    output[p + 2] = value;
                }
            }
            dst += 32;
        }
        Ok(())
    }

    fn unpack_alpha(&self, offset: i32, output: &mut [u8]) -> Result<bool> {
        if offset < 0 || offset as usize >= self.input.len() {
            return Ok(false);
        }
        let mut input = Reader::new(&self.input[offset as usize..]);
        if input.read_i32()? != 1 {
            return Ok(false);
        }
        let mut dst = 3usize;
        let mut ctl = 1i32 << 1;
        while dst < output.len() {
            ctl >>= 1;
            if ctl == 1 {
                ctl = input.read_u8()? as i32 | 0x100;
            }
            if (ctl & 1) != 0 {
                let v = input.read_u16()? as i32;
                let mut x = v & 0x3f;
                if x > 0x1f {
                    x |= -0x40;
                }
                let mut y = (v >> 6) & 7;
                if y != 0 {
                    y |= -8;
                }
                let count = ((v >> 9) & 0x7f) + 3;
                let src = dst as isize + (x as isize + y as isize * self.width as isize) * 4;
                if src < 0 || src >= dst as isize {
                    return Ok(true);
                }
                let mut src = src as usize;
                for _ in 0..count {
                    if dst >= output.len() || src >= output.len() {
                        break;
                    }
                    output[dst] = output[src];
                    src += 4;
                    dst += 4;
                }
            } else {
                output[dst] = input.read_u8()?;
                dst += 4;
            }
        }
        Ok(true)
    }

    fn decode_dct(
        &self,
        channel: usize,
        data: &[i16],
        src: usize,
        output: &mut [[i16; 3]; 64],
    ) -> Result<()> {
        if src + 63 >= data.len() {
            return Ok(());
        }
        let d = if channel > 0 { 1 } else { 0 };
        let mut tmp = [[0f32; 8]; 8];
        for i in 0..8 {
            if data[src + 8 + i] == 0
                && data[src + 16 + i] == 0
                && data[src + 24 + i] == 0
                && data[src + 32 + i] == 0
                && data[src + 40 + i] == 0
                && data[src + 48 + i] == 0
                && data[src + 56 + i] == 0
            {
                let t = data[src + i] as f32 * self.dct[d][i];
                for row in &mut tmp {
                    row[i] = t;
                }
                continue;
            }
            let v1 = data[src + i] as f32 * self.dct[d][i];
            let v2 = data[src + 8 + i] as f32 * self.dct[d][8 + i];
            let v3 = data[src + 16 + i] as f32 * self.dct[d][16 + i];
            let v4 = data[src + 24 + i] as f32 * self.dct[d][24 + i];
            let v5 = data[src + 32 + i] as f32 * self.dct[d][32 + i];
            let v6 = data[src + 40 + i] as f32 * self.dct[d][40 + i];
            let v7 = data[src + 48 + i] as f32 * self.dct[d][48 + i];
            let v8 = data[src + 56 + i] as f32 * self.dct[d][56 + i];
            let v10 = v1 + v5;
            let v11 = v1 - v5;
            let v12 = v3 + v7;
            let v13 = (v3 - v7) * 1.414213562f32 - v12;
            let v1 = v10 + v12;
            let v7 = v10 - v12;
            let v3 = v11 + v13;
            let v5 = v11 - v13;
            let v14 = v2 + v8;
            let v15 = v2 - v8;
            let v16 = v6 + v4;
            let v17 = v6 - v4;
            let v8 = v14 + v16;
            let v11 = (v14 - v16) * 1.414213562f32;
            let v9 = (v17 + v15) * 1.847759065f32;
            let v10 = 1.082392200f32 * v15 - v9;
            let v13 = -2.613125930f32 * v17 + v9;
            let v6 = v13 - v8;
            let v4 = v11 - v6;
            let v2 = v10 + v4;
            tmp[0][i] = v1 + v8;
            tmp[1][i] = v3 + v6;
            tmp[2][i] = v5 + v4;
            tmp[3][i] = v7 - v2;
            tmp[4][i] = v7 + v2;
            tmp[5][i] = v5 - v4;
            tmp[6][i] = v3 - v6;
            tmp[7][i] = v1 - v8;
        }

        let mut dst = 0;
        for row in &tmp {
            let v10 = row[0] + row[4];
            let v11 = row[0] - row[4];
            let v12 = row[2] + row[6];
            let v13 = row[2] - row[6];
            let v14 = row[1] + row[7];
            let v15 = row[1] - row[7];
            let v16 = row[5] + row[3];
            let v17 = row[5] - row[3];
            let v13 = 1.414213562f32 * v13 - v12;
            let v1 = v10 + v12;
            let v7 = v10 - v12;
            let v3 = v11 + v13;
            let v5 = v11 - v13;
            let v8 = v14 + v16;
            let v11 = (v14 - v16) * 1.414213562f32;
            let v9 = (v17 + v15) * 1.847759065f32;
            let v10 = v9 - v15 * 1.082392200f32;
            let v13 = v9 - v17 * 2.613125930f32;
            let v6 = v13 - v8;
            let v4 = v11 - v6;
            let v2 = v10 - v4;
            output[dst][channel] = Self::float_to_short(v1 + v8);
            output[dst + 1][channel] = Self::float_to_short(v3 + v6);
            output[dst + 2][channel] = Self::float_to_short(v5 + v4);
            output[dst + 3][channel] = Self::float_to_short(v7 + v2);
            output[dst + 4][channel] = Self::float_to_short(v7 - v2);
            output[dst + 5][channel] = Self::float_to_short(v5 - v4);
            output[dst + 6][channel] = Self::float_to_short(v3 - v6);
            output[dst + 7][channel] = Self::float_to_short(v1 - v8);
            dst += 8;
        }
        Ok(())
    }

    fn float_to_short(f: f32) -> i16 {
        let a = 0x80 + ((f as i32) >> 3);
        if a <= 0 {
            0
        } else if a <= 0xff {
            a as i16
        } else if a < 0x180 {
            0xff
        } else {
            0
        }
    }

    fn float_to_byte(f: f32) -> u8 {
        if f >= 255.0 {
            0xff
        } else if f <= 0.0 {
            0
        } else {
            f as u8
        }
    }
}

#[derive(Debug)]
struct HuffmanNode {
    valid: bool,
    is_parent: bool,
    weight: u32,
    left_index: usize,
    right_index: usize,
}

#[derive(Debug)]
struct HuffmanTree {
    nodes: Vec<HuffmanNode>,
}

impl HuffmanTree {
    fn new(weights: &[u32], v2: bool) -> Self {
        let mut nodes = Vec::with_capacity(weights.len() * 2);
        let mut root_weight = 0u32;
        for &weight in weights {
            nodes.push(HuffmanNode {
                valid: weight != 0,
                is_parent: false,
                weight,
                left_index: 0,
                right_index: 0,
            });
            root_weight = root_weight.wrapping_add(weight);
        }
        if root_weight == 0 {
            nodes.push(HuffmanNode {
                valid: true,
                is_parent: true,
                weight: 0,
                left_index: 0,
                right_index: 0,
            });
            return Self { nodes };
        }
        let mut child = [0usize; 2];
        loop {
            let mut weight = 0u32;
            for i in 0..2usize {
                let mut min_weight = u32::MAX;
                child[i] = usize::MAX;
                let mut n = 0usize;
                if v2 {
                    while n < nodes.len() {
                        if nodes[n].valid {
                            min_weight = nodes[n].weight;
                            child[i] = n;
                            n += 1;
                            break;
                        }
                        n += 1;
                    }
                    n = n.max(i + 1);
                }
                while n < nodes.len() {
                    if nodes[n].valid && nodes[n].weight < min_weight {
                        min_weight = nodes[n].weight;
                        child[i] = n;
                    }
                    n += 1;
                }
                if child[i] == usize::MAX {
                    continue;
                }
                nodes[child[i]].valid = false;
                weight = weight.wrapping_add(nodes[child[i]].weight);
            }
            nodes.push(HuffmanNode {
                valid: true,
                is_parent: true,
                weight,
                left_index: child[0],
                right_index: child[1],
            });
            if weight >= root_weight {
                break;
            }
        }
        Self { nodes }
    }

    fn decode_token(&self, stream: &mut MsbBitStream<'_>) -> Result<usize> {
        let mut node_index = self.nodes.len().saturating_sub(1);
        loop {
            let node = self
                .nodes
                .get(node_index)
                .ok_or_else(|| EthornellError::Parse("CBG Huffman node out of range".into()))?;
            if !node.is_parent {
                return Ok(node_index);
            }
            let bit = stream.get_next_bit()?;
            node_index = if bit {
                node.right_index
            } else {
                node.left_index
            };
        }
    }
}

#[derive(Clone)]
struct Reader<'a> {
    data: &'a [u8],
    pos: usize,
}

impl<'a> Reader<'a> {
    fn new(data: &'a [u8]) -> Self {
        Self { data, pos: 0 }
    }

    fn at(data: &'a [u8], pos: usize) -> Self {
        Self { data, pos }
    }

    fn remaining(&self) -> &'a [u8] {
        &self.data[self.pos.min(self.data.len())..]
    }

    fn read_bytes(&mut self, len: usize) -> Result<Vec<u8>> {
        if self.pos + len > self.data.len() {
            return Err(EthornellError::Parse("CBG stream exhausted".into()));
        }
        let out = self.data[self.pos..self.pos + len].to_vec();
        self.pos += len;
        Ok(out)
    }

    fn read_u8(&mut self) -> Result<u8> {
        let byte = *self
            .data
            .get(self.pos)
            .ok_or_else(|| EthornellError::Parse("CBG stream exhausted".into()))?;
        self.pos += 1;
        Ok(byte)
    }

    fn read_i8(&mut self) -> Result<i8> {
        Ok(self.read_u8()? as i8)
    }

    fn read_u16(&mut self) -> Result<u16> {
        let bytes = self.read_bytes(2)?;
        Ok(u16::from_le_bytes([bytes[0], bytes[1]]))
    }

    fn read_i32(&mut self) -> Result<i32> {
        let bytes = self.read_bytes(4)?;
        Ok(i32::from_le_bytes([bytes[0], bytes[1], bytes[2], bytes[3]]))
    }
}

struct MsbBitStream<'a> {
    input: Reader<'a>,
    bits: u32,
    cached_bits: u32,
}

impl<'a> MsbBitStream<'a> {
    fn new(input: Reader<'a>) -> Self {
        Self {
            input,
            bits: 0,
            cached_bits: 0,
        }
    }

    fn get_bits(&mut self, count: u32) -> Result<u32> {
        if count == 0 {
            return Ok(0);
        }
        while self.cached_bits < count {
            self.bits = (self.bits << 8) | self.input.read_u8()? as u32;
            self.cached_bits += 8;
        }
        let mask = if count == 32 {
            u32::MAX
        } else {
            (1u32 << count) - 1
        };
        self.cached_bits -= count;
        Ok((self.bits >> self.cached_bits) & mask)
    }

    /// The bits taken so far: `get_bits` fetches whole bytes ahead of them.
    fn bits_used(&self) -> usize {
        self.input.pos * 8 - self.cached_bits as usize
    }

    fn get_next_bit(&mut self) -> Result<bool> {
        if self.cached_bits == 0 {
            self.bits = (self.bits << 8) | self.input.read_u8()? as u32;
            self.cached_bits += 8;
        }
        self.cached_bits -= 1;
        Ok(((self.bits >> self.cached_bits) & 1) != 0)
    }
}

const DCT_TABLE: [f32; 64] = [
    1.00000000, 1.38703990, 1.30656302, 1.17587554, 1.00000000, 0.78569496, 0.54119611, 0.27589938,
    1.38703990, 1.92387950, 1.81225491, 1.63098633, 1.38703990, 1.08979023, 0.75066054, 0.38268343,
    1.30656302, 1.81225491, 1.70710683, 1.53635550, 1.30656302, 1.02655995, 0.70710677, 0.36047992,
    1.17587554, 1.63098633, 1.53635550, 1.38268340, 1.17587554, 0.92387950, 0.63637930, 0.32442334,
    1.00000000, 1.38703990, 1.30656302, 1.17587554, 1.00000000, 0.78569496, 0.54119611, 0.27589938,
    0.78569496, 1.08979023, 1.02655995, 0.92387950, 0.78569496, 0.61731654, 0.42521504, 0.21677275,
    0.54119611, 0.75066054, 0.70710677, 0.63637930, 0.54119611, 0.42521504, 0.29289323, 0.14931567,
    0.27589938, 0.38268343, 0.36047992, 0.32442334, 0.27589938, 0.21677275, 0.14931567, 0.07612047,
];

const BLOCK_FILL_ORDER: [u8; 64] = [
    0, 1, 8, 16, 9, 2, 3, 10, 17, 24, 32, 25, 18, 11, 4, 5, 12, 19, 26, 33, 40, 48, 41, 34, 27, 20,
    13, 6, 7, 14, 21, 28, 35, 42, 49, 56, 57, 50, 43, 36, 29, 22, 15, 23, 30, 37, 44, 51, 58, 59,
    52, 45, 38, 31, 39, 46, 53, 60, 61, 54, 47, 55, 62, 63,
];

#[cfg(test)]
mod tests {
    use super::*;

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

        /// A symbol's code, read off the decoder's own tree, then its value
        /// bits.
        fn token(&mut self, tree: &HuffmanTree, symbol: usize, value: u32, bits: u32) {
            let mut pending = vec![(tree.nodes.len() - 1, 0, 0)];
            while let Some((index, code, length)) = pending.pop() {
                let node = &tree.nodes[index];
                if node.is_parent {
                    pending.push((node.left_index, code << 1, length + 1));
                    pending.push((node.right_index, code << 1 | 1, length + 1));
                } else if index == symbol {
                    self.put(code, length);
                    self.put(value, bits);
                    return;
                }
            }
            panic!("no code for symbol {symbol}");
        }

        fn align(&mut self) {
            self.used = self.used.next_multiple_of(8);
        }
    }

    #[test]
    fn a_strip_decodes_the_codes_in_its_last_byte() {
        // One 8x8 block of a 24-bit image: grey, with a horizontal Cr
        // coefficient that makes it redder on the left.
        let mut dc_weights = vec![0u32; 0x10];
        let mut ac_weights = vec![0u32; 0xb0];
        (dc_weights[0], dc_weights[1]) = (2, 1);
        (ac_weights[0], ac_weights[0x40]) = (3, 1);
        let tree1 = HuffmanTree::new(&dc_weights, true);
        let tree2 = HuffmanTree::new(&ac_weights, true);
        let mut bits = Bits::default();
        // DC differences of Y, Cb and Cr (symbol: the value's size).
        bits.token(&tree1, 1, 1, 1);
        bits.token(&tree1, 0, 0, 0);
        bits.token(&tree1, 0, 0, 0);
        bits.align();
        // AC codes, one byte: Y and Cb end at once, Cr has 15 at zigzag
        // position 1 (size 4, no zero run) and ends.
        bits.token(&tree2, 0, 0, 0);
        bits.token(&tree2, 0, 0, 0);
        bits.token(&tree2, 0x40, 15, 4);
        bits.token(&tree2, 0, 0, 0);
        assert_eq!(bits.used, 16);
        let mut strip = vec![0xc0, 0x01]; // 192 coefficients
        strip.extend(&bits.bytes);

        let decode = |strip: &[u8]| {
            let decoder = BlockDecoder {
                input: strip,
                bpp: 24,
                width: 8,
                tree1: HuffmanTree::new(&dc_weights, true),
                tree2: HuffmanTree::new(&ac_weights, true),
                dct: [DCT_TABLE; 2],
            };
            let mut bgra = vec![0u8; 8 * 8 * 4];
            decoder
                .unpack_block(0, strip.len() as i32, 0, &mut bgra)
                .unwrap();
            bgra
        };
        let bgra = decode(&strip);
        // The strip's last byte was fetched for Y's end code; Cb's end code
        // and Cr's coefficient follow in it.
        let red = |x: usize| bgra[x * 4 + 2];
        assert!(red(0) > red(7) + 4, "red {} .. {}", red(0), red(7));
        // Bytes after the strip's codes change nothing.
        let mut padded = strip.clone();
        padded.push(0);
        assert_eq!(bgra, decode(&padded));
    }
}

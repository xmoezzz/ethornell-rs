//! The CompressedBG version 2 encoder of BGI 1.622 (`sub_4A1930`, 90:CE
//! format 1): YCbCr from float tables, the AAN float DCT with quality
//! tables, Huffman-coded DC/AC tokens per 8-pixel strip and an LZ alpha
//! plane, followed by the target's own decode-and-compare checks. Ported
//! function by function from the x87 code (Windows default 53-bit
//! precision: x87 temporaries are f64, every stack variable is an f32);
//! checked byte for byte under Unicorn (`src/tools/emu_kernels.py`, kind
//! 10).

const ZIGZAG: [u8; 64] = [
    0, 1, 8, 16, 9, 2, 3, 10, 17, 24, 32, 25, 18, 11, 4, 5, 12, 19, 26, 33, 40, 48, 41, 34, 27, 20,
    13, 6, 7, 14, 21, 28, 35, 42, 49, 56, 57, 50, 43, 36, 29, 22, 15, 23, 30, 37, 44, 51, 58, 59, 52,
    45, 38, 31, 39, 46, 53, 60, 61, 54, 47, 55, 62, 63,
];
/// byte_503C28 / byte_503C68: the luminance and chrominance base tables.
const LUMINANCE: [u8; 64] = [
    16, 11, 10, 16, 24, 40, 51, 61, 12, 12, 14, 19, 26, 58, 60, 55, 14, 13, 16, 24, 40, 57, 69, 56,
    14, 17, 22, 29, 51, 87, 80, 62, 18, 22, 37, 56, 68, 109, 103, 77, 24, 35, 55, 64, 81, 104, 113,
    92, 49, 64, 78, 87, 103, 121, 120, 101, 72, 92, 95, 98, 112, 100, 103, 99,
];
const CHROMINANCE: [u8; 64] = [
    17, 18, 24, 47, 99, 99, 99, 99, 18, 21, 26, 66, 99, 99, 99, 99, 24, 26, 56, 99, 99, 99, 99, 99,
    47, 66, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99,
    99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99,
];
/// flt_503CA8: the AAN scale of each coefficient, as f32 bit patterns.
const AAN_SCALE: [u32; 64] = [
    0x3F80_0000, 0x3FB1_8A86, 0x3FA7_3D75, 0x3F96_8317, 0x3F80_0000, 0x3F49_234E,
    0x3F0A_8BD4, 0x3E8D_42AF, 0x3FB1_8A86, 0x3FF6_41AF, 0x3FE7_F7F8, 0x3FD0_C429,
    0x3FB1_8A86, 0x3F8B_7E3F, 0x3F40_2B4A, 0x3EC3_EF15, 0x3FA7_3D75, 0x3FE7_F7F8,
    0x3FDA_827A, 0x3FC4_A74C, 0x3FA7_3D75, 0x3F83_6651, 0x3F35_04F3, 0x3EB8_90D3,
    0x3F96_8317, 0x3FD0_C429, 0x3FC4_A74C, 0x3FB0_FBC5, 0x3F96_8317, 0x3F6C_835E,
    0x3F22_E9C1, 0x3EA6_1AD1, 0x3F80_0000, 0x3FB1_8A86, 0x3FA7_3D75, 0x3F96_8317,
    0x3F80_0000, 0x3F49_234E, 0x3F0A_8BD4, 0x3E8D_42AF, 0x3F49_234E, 0x3F8B_7E3F,
    0x3F83_6651, 0x3F6C_835E, 0x3F49_234E, 0x3F1E_0875, 0x3ED9_B5C9, 0x3E5D_F9AD,
    0x3F0A_8BD4, 0x3F40_2B4A, 0x3F35_04F3, 0x3F22_E9C1, 0x3F0A_8BD4, 0x3ED9_B5C9,
    0x3E95_F61A, 0x3E18_E635, 0x3E8D_42AF, 0x3EC3_EF15, 0x3EB8_90D3, 0x3EA6_1AD1,
    0x3E8D_42AF, 0x3E5D_F9AD, 0x3E18_E635, 0x3D9B_E50C,
];

fn aan_scale(index: usize) -> f32 {
    f32::from_bits(AAN_SCALE[index])
}

/// x87 truncation (`__ftol2_sse`, `fistp` in chop mode) of an in-range
/// value.
fn chop(value: f64) -> i32 {
    value as i32
}

/// `sub_49BBB0`: per channel the f32 contributions of R, G and B.
struct ColourTables {
    y: [[f32; 256]; 3],
    cb: [[f32; 256]; 3],
    cr: [[f32; 256]; 3],
}

impl ColourTables {
    fn new() -> Self {
        let mut t = Self { y: [[0.0; 256]; 3], cb: [[0.0; 256]; 3], cr: [[0.0; 256]; 3] };
        for k in 0..256 {
            let v = k as f64;
            t.y[0][k] = (v * 0.299) as f32;
            t.y[1][k] = (v * 0.587) as f32;
            t.y[2][k] = (v * 0.114 + 0.5) as f32;
            t.cb[0][k] = (-0.16874 * v) as f32;
            t.cb[1][k] = (-0.33126 * v) as f32;
            t.cb[2][k] = (v * 0.5 + 128.0 + 0.5) as f32;
            t.cr[0][k] = (v * 0.5) as f32;
            t.cr[1][k] = (-0.41869 * v) as f32;
            t.cr[2][k] = (128.0 - v * 0.08131 + 0.5) as f32;
        }
        t
    }

    /// byte_5623A0..5626A0: [(int)(R + G + B + 256.0)] clamped to a byte.
    fn level(table: &[[f32; 256]; 3], r: u8, g: u8, b: u8) -> i16 {
        let sum = f64::from(table[0][usize::from(r)]) + f64::from(table[1][usize::from(g)])
            + f64::from(table[2][usize::from(b)])
            + 256.0;
        let index = chop(sum);
        (index - 256).clamp(0, 255) as i16
    }
}

/// `sub_49BE00`: the quality's quantizer bytes (stored in the header) and
/// the f32 reciprocals the DCT multiplies by.
struct Quantizers {
    luminance: [u8; 64],
    chrominance: [u8; 64],
    luminance_scale: [f32; 64],
    chrominance_scale: [f32; 64],
}

impl Quantizers {
    fn new(quality: u32) -> Self {
        let mut q = Self { luminance: [0; 64], chrominance: [0; 64], luminance_scale: [0.0; 64], chrominance_scale: [0.0; 64] };
        let reciprocal = |level: u8, index: usize| (1.0 / (f64::from(level) * f64::from(aan_scale(index)) * 8.0)) as f32;
        for index in 0..64 {
            let (y, c) = if quality < 50 {
                let factor = f64::from(2 * quality as i32) / 100.0;
                let level = |base: u8| chop(255.0 - f64::from(255 - i32::from(base)) * factor) as u8;
                (level(LUMINANCE[index]), level(CHROMINANCE[index]))
            } else {
                let factor = f64::from(2 * (100 - quality as i32)) / 100.0;
                let level = |base: u8| (chop(f64::from(base) * factor) as u8).max(1);
                (level(LUMINANCE[index]), level(CHROMINANCE[index]))
            };
            q.luminance[index] = y;
            q.chrominance[index] = c;
            q.luminance_scale[index] = reciprocal(y, index);
            q.chrominance_scale[index] = reciprocal(c, index);
        }
        q
    }
}

/// `sub_49FD60` on one 8x8 block of one channel, in place: the AAN row and
/// column passes, then `trunc(scale * v + 16384.5) - 16384`, every
/// coefficient but DC clamped to +-1023.
fn dct_block(block: &mut [i16], scale: &[f32; 64]) {
    const C1: f64 = 0.707106781;
    const C2: f64 = 0.382683433;
    const C3: f64 = 0.5411961;
    const C4: f64 = 1.306562965;
    let f = |value: f64| value as f32;
    let add = |a: f32, b: f32| f(f64::from(a) + f64::from(b));
    let sub = |a: f32, b: f32| f(f64::from(a) - f64::from(b));
    // The butterfly shared by both passes: t[0..8] are the stage-one sums
    // and differences (t0, t1, t2, t3 even; t4..t7 odd), the outputs in
    // natural order.
    let butterfly = |t0: f32, t1: f32, t2: f32, t3: f32, t4: f32, t5: f32, t6: f32, t7: f32| -> [f32; 8] {
        let tmp10 = add(t0, t3);
        let tmp13 = sub(t0, t3);
        let tmp11 = add(t1, t2);
        let tmp12 = sub(t1, t2);
        let z1 = f((f64::from(tmp12) + f64::from(tmp13)) * C1);
        let o0 = add(tmp10, tmp11);
        let o4 = sub(tmp10, tmp11);
        let o2 = add(tmp13, z1);
        let o6 = sub(tmp13, z1);
        let s10 = add(t4, t5);
        let s11 = add(t5, t6);
        let s12 = add(t6, t7);
        let z5 = f((f64::from(s10) - f64::from(s12)) * C2);
        let z2 = f(f64::from(s10) * C3 + f64::from(z5));
        let z4 = f(f64::from(z5) + f64::from(s12) * C4);
        let z3 = f(f64::from(s11) * C1);
        let z11 = add(t7, z3);
        let z13 = sub(t7, z3);
        [o0, add(z11, z4), o2, sub(z13, z2), o4, add(z13, z2), o6, sub(z11, z4)]
    };
    let mut rows = [[0f32; 8]; 8];
    for (r, row) in rows.iter_mut().enumerate() {
        let e = |c: usize| i32::from(block[r * 8 + c]);
        *row = butterfly(
            (e(0) + e(7) - 256) as f32,
            (e(1) + e(6) - 256) as f32,
            (e(2) + e(5) - 256) as f32,
            (e(3) + e(4) - 256) as f32,
            (e(3) - e(4)) as f32,
            (e(2) - e(5)) as f32,
            (e(1) - e(6)) as f32,
            (e(0) - e(7)) as f32,
        );
    }
    for c in 0..8 {
        let v = |r: usize| rows[r][c];
        let out = butterfly(
            add(v(0), v(7)),
            add(v(1), v(6)),
            add(v(5), v(2)),
            add(v(4), v(3)),
            sub(v(3), v(4)),
            sub(v(2), v(5)),
            sub(v(1), v(6)),
            sub(v(0), v(7)),
        );
        for (r, value) in out.into_iter().enumerate() {
            let index = r * 8 + c;
            let level = chop(f64::from(scale[index]) * f64::from(value) + 16384.5) as i16;
            let mut coefficient = level.wrapping_sub(0x4000);
            if index != 0 {
                coefficient = coefficient.clamp(-1023, 1023);
            }
            block[index] = coefficient;
        }
    }
}

/// `sub_4A09D0`: the bit length of |value|, at most `limit - 1`.
fn bit_length(limit: u32, value: i32) -> u32 {
    let magnitude = value.unsigned_abs();
    let mut length = limit - 1;
    let mut bit = 1u32.rotate_left(length);
    while length != 0 {
        bit = bit.rotate_right(1);
        if bit & magnitude != 0 {
            break;
        }
        length -= 1;
    }
    length
}

#[derive(Clone, Copy)]
struct Node {
    active: bool,
    weight: u32,
    bit: u8,
    parent: i32,
    left: i32,
    right: i32,
}

/// `sub_4A0ED0`: the tree over `counts`; returns the nodes and the root.
/// Each merge takes, twice, the first active node (leaves and merged nodes
/// alike) and then any strictly lighter one from index pick + 1 on (the
/// second pick never compares node 1), marking the first pick bit 0 and
/// the second bit 1.
fn huffman_tree(counts: &[u32]) -> (Vec<Node>, usize) {
    let leaves = counts.len();
    let mut nodes = vec![Node { active: false, weight: 0, bit: 0, parent: -1, left: -1, right: -1 }; 2 * leaves - 1];
    let mut total = 0u32;
    for (node, &count) in nodes.iter_mut().zip(counts) {
        node.weight = count;
        if count != 0 {
            total = total.wrapping_add(count);
            node.active = true;
        }
    }
    let mut next = leaves;
    while next < 2 * leaves - 1 {
        let mut picks = [-1i32; 2];
        let mut sum = 0u32;
        for pick in 0..2 {
            if let Some(first) = (0..next).find(|&i| nodes[i].active) {
                picks[pick] = first as i32;
            }
            if pick + 1 < next {
                for candidate in pick + 1..next {
                    // The target compares against the current pick's
                    // weight, reading node -1 when there is none.
                    let best = picks[pick];
                    if nodes[candidate].active && best >= 0 && nodes[candidate].weight < nodes[best as usize].weight {
                        picks[pick] = candidate as i32;
                    }
                }
            }
            let Ok(chosen) = usize::try_from(picks[pick]) else {
                break;
            };
            let node = &mut nodes[chosen];
            node.active = false;
            node.parent = next as i32;
            node.bit = pick as u8;
            sum = sum.wrapping_add(node.weight);
        }
        nodes[next] = Node { active: true, weight: sum, bit: 0xFF, parent: -1, left: picks[0], right: picks[1] };
        if sum >= total {
            break;
        }
        next += 1;
    }
    (nodes, next)
}

/// MSB-first bit writer of `sub_4A0AC0` / `sub_4A0C20`.
struct BitWriter {
    bytes: Vec<u8>,
    used: u32,
}

impl BitWriter {
    fn new() -> Self {
        Self { bytes: vec![0], used: 0 }
    }

    fn bit(&mut self, set: bool) {
        if set {
            *self.bytes.last_mut().unwrap() |= 0x80 >> self.used;
        }
        self.used += 1;
        if self.used == 8 {
            self.bytes.push(0);
            self.used = 0;
        }
    }

    /// `sub_4A0AC0`: the leaf's path below the root, root side first.
    fn code(&mut self, nodes: &[Node], symbol: usize) {
        let mut path = Vec::new();
        let mut node = symbol;
        loop {
            path.push(nodes[node].bit);
            let parent = nodes[node].parent;
            if parent == -1 {
                break;
            }
            node = parent as usize;
        }
        path.pop();
        for &bit in path.iter().rev() {
            self.bit(bit != 0);
        }
    }

    /// `sub_4A0C20`: the low `count` bits of `value`, MSB first.
    fn raw(&mut self, count: u32, value: i32) {
        for k in (0..count).rev() {
            self.bit((value as u32 >> k) & 1 != 0);
        }
    }

    fn finish(mut self) -> Vec<u8> {
        if self.used == 0 {
            self.bytes.pop();
        }
        self.bytes
    }
}

/// `sub_4A05F0` + `sub_4A07A0`: a strip's DC differences (prediction
/// restarting at the strip) then, byte aligned, its AC run/size tokens
/// (15 for sixteen zeros, 0 for the end of a block).
fn encode_strip(coefficients: &[i16], dc: &[Node], ac: &[Node]) -> Vec<u8> {
    let mut out = BitWriter::new();
    let mut previous = 0i32;
    for block in coefficients.chunks(64) {
        let mut diff = i32::from(block[0]) - previous;
        let size = bit_length(16, diff);
        out.code(dc, size as usize);
        if diff < 0 {
            diff -= 1;
        }
        out.raw(size, diff);
        previous = i32::from(block[0]);
    }
    let mut bytes = out.finish();
    let mut out = BitWriter::new();
    for block in coefficients.chunks(64) {
        let (mut pending, mut run) = (0, 0usize);
        for &position in &ZIGZAG[1..] {
            let mut value = i32::from(block[usize::from(position)]);
            if value != 0 {
                for _ in 0..pending {
                    out.code(ac, 15);
                }
                let size = bit_length(11, value);
                out.code(ac, run | (size as usize) << 4);
                if value < 0 {
                    value -= 1;
                }
                out.raw(size, value);
                pending = 0;
                run = 0;
            } else {
                run += 1;
                if run >= 16 {
                    pending += 1;
                    run = 0;
                }
            }
        }
        if run != 0 || pending != 0 {
            out.code(ac, 0);
        }
    }
    bytes.extend(out.finish());
    bytes
}

/// `sub_4A0580` / `sub_4A05B0`: 7-bit varints, low group first.
fn push_varint(out: &mut Vec<u8>, mut value: u32) {
    loop {
        let byte = (value & 0x7F) as u8;
        value >>= 7;
        out.push(if value != 0 { byte | 0x80 } else { byte });
        if value == 0 {
            break;
        }
    }
}

/// `sub_4A1290`: the alpha bytes as literals or (x -31..31, y -6..0, length
/// 3..130) copies, eight items per flag byte. On the row above, x reaches
/// `width - column`, which for narrow images (width <= 31) includes the
/// pixel itself: the copy decodes to nothing and the check rejects it.
fn encode_alpha(pixels: &[u8], width: usize, stride: usize, capacity: usize) -> Vec<u8> {
    let size = pixels.len();
    let mut out = vec![0u8; capacity.max(1)];
    if size <= 3 {
        out.truncate(1);
        return out;
    }
    let (mut at, mut written, mut flag_at, mut items) = (3usize, 1usize, 0usize, 0u32);
    loop {
        if written >= capacity {
            break;
        }
        let pixel = at >> 2;
        let (row, column) = (pixel / width, pixel % width);
        let x_first = -(column.min(31) as i32);
        let y_first = -(row.min(6) as i32);
        let mut x_last = (width - column).min(31) as i32;
        let (mut best, mut best_x, mut best_y) = (2u32, 0i32, 0i32);
        'search: for y in y_first..=0 {
            if y == 0 {
                x_last = -1;
            }
            for x in x_first..=x_last {
                let offset = stride as isize * y as isize + 4 * x as isize;
                let source = |position: usize| pixels[(position as isize + offset) as usize];
                if pixels[at] != source(at) {
                    continue;
                }
                let mut length = 1u32;
                let mut position = at + 4;
                while length < 130 && position < size && pixels[position] == source(position) {
                    length += 1;
                    position += 4;
                }
                if length > best {
                    best = length;
                    best_x = x;
                    best_y = y;
                    if length == 130 {
                        break 'search;
                    }
                }
            }
        }
        if best < 3 {
            out[written] = pixels[at];
            at += 4;
            written += 1;
        } else {
            at += 4 * best as usize;
            out[flag_at] |= 1 << items;
            let code = (best_x as u16 & 0x3F) | (((best_y as u16 & 7) | ((8 * best - 24) as u16)) << 6);
            out[written..written + 2].copy_from_slice(&code.to_le_bytes());
            written += 2;
        }
        items += 1;
        if items >= 8 {
            if at >= size {
                break;
            }
            if written < capacity {
                flag_at = written;
                out[written] = 0;
                written += 1;
                items = 0;
            }
        }
        if at >= size {
            break;
        }
    }
    out.truncate(written);
    out
}

/// `sub_4A14A0`: the alpha decoder the check runs, into a zeroed copy.
fn decode_alpha(output: &mut [u8], data: &[u8], stride: usize) {
    let (mut at, mut read, mut flag_at, mut items) = (3usize, 1usize, 0usize, 0u32);
    while read < data.len() && at < output.len() {
        if data[flag_at] & (1 << items) != 0 {
            let code = u16::from_le_bytes([data[read], data.get(read + 1).copied().unwrap_or(0)]);
            let mut x = i32::from(code & 0x3F);
            if x > 31 {
                x -= 64;
            }
            let mut y = i32::from((code >> 6) & 7);
            if y > 0 {
                y -= 8;
            }
            let length = usize::from(code >> 9) + 3;
            let offset = stride as isize * y as isize + 4 * x as isize;
            for k in 0..length {
                let target = at + 4 * k;
                let source = (target as isize + offset) as usize;
                if target < output.len() && source < output.len() {
                    output[target] = output[source];
                }
            }
            read += 2;
            at += 4 * length;
        } else {
            output[at] = data[read];
            read += 1;
            at += 4;
        }
        items += 1;
        if items >= 8 {
            if read >= data.len() {
                break;
            }
            if at < output.len() {
                flag_at = read;
                items = 0;
                read += 1;
            }
        }
    }
}

/// MSB-first reader of the check decoders (`sub_4A0B80`, `sub_4A0A00`).
struct BitReader<'a> {
    data: &'a [u8],
    byte: usize,
    bit: u32,
}

impl BitReader<'_> {
    fn next(&mut self) -> bool {
        let value = self.data.get(self.byte).copied().unwrap_or(0) & (0x80 >> self.bit) != 0;
        self.bit += 1;
        if self.bit == 8 {
            self.bit = 0;
            self.byte += 1;
        }
        value
    }

    fn bits(&mut self, count: u32) -> i32 {
        (0..count).fold(0, |value, _| value << 1 | i32::from(self.next()))
    }

    /// Walks the tree from the root (the 8-bit lookup of `sub_4A0D80` gives
    /// the same symbols).
    fn symbol(&mut self, nodes: &[Node], root: usize, leaves: usize) -> usize {
        let mut node = root;
        while node >= leaves {
            let next = if self.next() { nodes[node].right } else { nodes[node].left };
            if next < 0 {
                return 0;
            }
            node = next as usize;
        }
        node
    }
}

fn extend(size: u32, value: i32) -> i16 {
    if size != 0 && value & (1 << (size - 1)) == 0 {
        (((-1i32) << size | value) + 1) as i16
    } else {
        value as i16
    }
}

/// `sub_4A06B0` + `sub_4A08E0`: the strip decoded back, for the check.
fn decode_strip(data: &[u8], count: usize, dc: (&[Node], usize), ac: (&[Node], usize)) -> Vec<i16> {
    let mut out = vec![0i16; count];
    let mut reader = BitReader { data, byte: 0, bit: 0 };
    let mut previous = 0i16;
    let mut index = 0;
    while reader.byte < data.len() && index < count {
        let size = reader.symbol(dc.0, dc.1, 16) as u32;
        let value = extend(size, reader.bits(size));
        previous = previous.wrapping_add(value);
        out[index] = previous;
        index += 64;
    }
    if reader.bit != 0 {
        reader.bit = 0;
        reader.byte += 1;
    }
    let mut index = 0;
    while reader.byte < data.len() && index < count {
        let mut k = 1usize;
        while k < 64 {
            let token = reader.symbol(ac.0, ac.1, 176);
            if token == 0 {
                break;
            }
            if token == 15 {
                k += 16;
                continue;
            }
            let size = (token >> 4) as u32;
            k += token & 0xF;
            let value = extend(size, reader.bits(size));
            if k < 64 && index + usize::from(ZIGZAG[k]) < count {
                out[index + usize::from(ZIGZAG[k])] = value;
            }
            k += 1;
        }
        index += 64;
    }
    out
}

/// `sub_4A1E70`: the 48-byte CompressedBG header (version 2) and the 132
/// keyed bytes: both quantizer tables and the data offset 180.
fn header(descriptor: &[u8; 16], quantizers: &Quantizers, key: u32) -> Vec<u8> {
    let mut out = b"CompressedBG___\0".to_vec();
    out.extend_from_slice(descriptor);
    out.extend_from_slice(&0u32.to_le_bytes());
    out.extend_from_slice(&key.to_le_bytes());
    out.extend_from_slice(&132u32.to_le_bytes());
    let mut table = quantizers.luminance.to_vec();
    table.extend_from_slice(&quantizers.chrominance);
    table.extend_from_slice(&180u32.to_le_bytes());
    let sum = table.iter().fold(0u8, |sum, &byte| sum.wrapping_add(byte));
    let xor = table.iter().fold(0u8, |xor, &byte| xor ^ byte);
    out.extend_from_slice(&[sum, xor]);
    out.extend_from_slice(&2u16.to_le_bytes());
    let mut stream = key;
    out.extend(table.iter().map(|&byte| byte.wrapping_add(crate::cbg_encode::next_key(&mut stream) as u8)));
    out
}

/// `sub_4A1930`: `descriptor` is the 16-byte image descriptor (u16 width,
/// height, 24 or 32 bits, 0, format), `pixels` packed rows, `quality`
/// 0..=100, `key` the GetTickCount value. None when the target's checks
/// reject its own encoding (sub_49C200 status 3, "エンコードに失敗しました").
pub fn encode_cbg_v2(descriptor: &[u8; 16], pixels: &[u8], quality: u32, key: u32) -> Option<Vec<u8>> {
    let width = usize::from(u16::from_le_bytes([descriptor[0], descriptor[1]]));
    let height = usize::from(u16::from_le_bytes([descriptor[2], descriptor[3]]));
    let bpp = usize::from(u16::from_le_bytes([descriptor[4], descriptor[5]]) >> 3);
    let channels = if bpp == 4 { 3 } else { bpp };
    let quantizers = Quantizers::new(quality);
    let colour = ColourTables::new();
    let mut out = header(descriptor, &quantizers, key);

    // sub_49C200: the image padded to whole 8x8 blocks.
    let (wide, high) = ((width + 7) & !7, (height + 7) & !7);
    let stride = wide * bpp;
    let mut padded = vec![0u8; stride * high];
    for y in 0..height {
        padded[y * stride..y * stride + width * bpp].copy_from_slice(&pixels[y * width * bpp..(y + 1) * width * bpp]);
    }
    let blocks = wide / 8;
    let strips = high / 8;
    let mask_bytes = (blocks + 7) >> 3;

    // Strip jobs (sub_49D320 type 0): each block's planes, then the DCT.
    let mut coefficients = Vec::with_capacity(strips);
    for strip in 0..strips {
        let plane = 64 * blocks;
        let mut planes = vec![0i16; plane * channels];
        for block in 0..blocks {
            let base = strip * 8 * stride + block * 8 * bpp;
            for row in 0..8 {
                for column in 0..8 {
                    let at = base + row * stride + column * bpp;
                    let index = block * 64 + row * 8 + column;
                    if bpp == 1 {
                        planes[index] = i16::from(padded[at]);
                    } else {
                        let (b, g, r) = (padded[at], padded[at + 1], padded[at + 2]);
                        planes[index] = ColourTables::level(&colour.y, r, g, b);
                        planes[plane + index] = ColourTables::level(&colour.cb, r, g, b);
                        planes[2 * plane + index] = ColourTables::level(&colour.cr, r, g, b);
                    }
                }
            }
            for channel in 0..channels {
                let scale = if channel == 0 { &quantizers.luminance_scale } else { &quantizers.chrominance_scale };
                let start = channel * plane + block * 64;
                dct_block(&mut planes[start..start + 64], scale);
            }
        }
        coefficients.push(planes);
    }

    // sub_4A0C80 / sub_4A0CC0: token counts over every strip.
    let mut dc_counts = [0u32; 16];
    let mut ac_counts = [0u32; 176];
    for planes in &coefficients {
        let mut previous = 0i32;
        for block in planes.chunks(64) {
            dc_counts[bit_length(16, i32::from(block[0]) - previous) as usize] += 1;
            previous = i32::from(block[0]);
            let (mut pending, mut run) = (0u32, 0usize);
            for &position in &ZIGZAG[1..] {
                let value = block[usize::from(position)];
                if value != 0 {
                    ac_counts[15] += pending;
                    ac_counts[run | (bit_length(11, i32::from(value)) as usize) << 4] += 1;
                    pending = 0;
                    run = 0;
                } else {
                    run += 1;
                    if run >= 16 {
                        pending += 1;
                        run = 0;
                    }
                }
            }
            if run != 0 || pending != 0 {
                ac_counts[0] += 1;
            }
        }
    }
    let (dc_nodes, dc_root) = huffman_tree(&dc_counts);
    let (ac_nodes, ac_root) = huffman_tree(&ac_counts);
    let codes = coefficients.iter().map(|planes| encode_strip(planes, &dc_nodes, &ac_nodes)).collect::<Vec<_>>();

    // Alpha job (type 2, mode 1).
    let alpha = (bpp == 4).then(|| encode_alpha(&padded, wide, stride, 2 * wide * high));

    // Layout: counts, offsets, strips (mask, varint count, codes), alpha.
    let mut body = Vec::new();
    for &count in dc_counts.iter().chain(&ac_counts) {
        push_varint(&mut body, count);
    }
    let entries = strips + usize::from(alpha.is_some());
    let mut offset = body.len() + 4 * entries;
    let mut strip_data = Vec::new();
    for (planes, code) in coefficients.iter().zip(&codes) {
        body.extend_from_slice(&(offset as u32).to_le_bytes());
        let mut strip = vec![0u8; mask_bytes];
        for block in 0..blocks {
            strip[block >> 3] |= 1 << (block & 7);
        }
        push_varint(&mut strip, planes.len() as u32);
        strip.extend_from_slice(code);
        offset += strip.len();
        strip_data.push(strip);
    }
    if alpha.is_some() {
        body.extend_from_slice(&(offset as u32).to_le_bytes());
    }
    for strip in strip_data {
        body.extend(strip);
    }

    // The checks: every strip decodes to its coefficients, the alpha plane
    // to the image's alpha bytes.
    for (planes, code) in coefficients.iter().zip(&codes) {
        if decode_strip(code, planes.len(), (&dc_nodes, dc_root), (&ac_nodes, ac_root)) != *planes {
            return None;
        }
    }
    if let Some(alpha) = alpha {
        let mut decoded = vec![0u8; padded.len()];
        decode_alpha(&mut decoded, &alpha, stride);
        if (3..padded.len()).step_by(4).any(|at| decoded[at] != padded[at]) {
            return None;
        }
        body.extend_from_slice(&1u32.to_le_bytes());
        body.extend(alpha);
    }
    out.extend(body);
    Some(out)
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn encoded_images_decode_close_to_their_pixels() {
        for (width, height, bits) in [(48u16, 17u16, 32u16), (40, 20, 24)] {
            let mut header = [0u8; 16];
            header[0..2].copy_from_slice(&width.to_le_bytes());
            header[2..4].copy_from_slice(&height.to_le_bytes());
            header[4..6].copy_from_slice(&bits.to_le_bytes());
            header[8..10].copy_from_slice(&(if bits == 24 { 1u16 } else { 2 }).to_le_bytes());
            let bpp = usize::from(bits / 8);
            let mut pixels = Vec::new();
            for y in 0..usize::from(height) {
                for x in 0..usize::from(width) {
                    pixels.extend_from_slice(&[(x * 5) as u8, (y * 9) as u8, 128]);
                    if bpp == 4 {
                        pixels.push(if x < 20 { 255 } else { 0 });
                    }
                }
            }
            let encoded = encode_cbg_v2(&header, &pixels, 90, 7).unwrap();
            let image = crate::decode_cbg(&encoded).unwrap();
            assert_eq!((image.width, image.height), (u32::from(width), u32::from(height)));
            for (index, rgba) in image.rgba.chunks_exact(4).enumerate() {
                let source = &pixels[index * bpp..index * bpp + bpp];
                for (decoded, original) in [(rgba[2], source[0]), (rgba[1], source[1]), (rgba[0], source[2])] {
                    assert!(decoded.abs_diff(original) <= 12, "{index}: {decoded} vs {original}");
                }
                if bpp == 4 {
                    assert_eq!(rgba[3], source[3], "alpha at {index}");
                }
            }
        }
    }
}

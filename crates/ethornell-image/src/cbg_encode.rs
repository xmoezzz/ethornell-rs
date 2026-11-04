//! The CompressedBG version 1 encoder of BGI 1.622 (`sub_46A320`, 90:CE
//! format 0), ported function by function so its output matches the target
//! byte for byte (`src/tools/emu_kernels.py`, kind 9).

/// `sub_4A1F40`: the LCG stepping the frequency-table key.
pub(crate) fn next_key(key: &mut u32) -> u32 {
    let high = key.wrapping_mul(22_695_477) >> 16;
    *key = (high << 16).wrapping_add(u32::from(key.wrapping_mul(20_021) as u16)).wrapping_add(1);
    high & 0x7FFF
}

/// `sub_4695A0`: each byte minus the average of its upper and left
/// neighbours (one of them when the other is outside, 0 at the origin).
fn predict(width: usize, height: usize, bytes_per_pixel: usize, pixels: &[u8]) -> Vec<u8> {
    let row = width * bytes_per_pixel;
    let mut out = Vec::with_capacity(row * height);
    for y in 0..height {
        for x in 0..width {
            for channel in 0..bytes_per_pixel {
                let at = y * row + x * bytes_per_pixel + channel;
                let up = (y > 0).then(|| u32::from(pixels[at - row]));
                let left = (x > 0).then(|| u32::from(pixels[at - bytes_per_pixel]));
                let prediction = match (up, left) {
                    (None, None) => 0,
                    (None, Some(left)) => left,
                    (Some(up), None) => up,
                    (Some(up), Some(left)) => (left + up) >> 1,
                };
                out.push(pixels[at].wrapping_sub(prediction as u8));
            }
        }
    }
    out
}

/// 7-bit little-endian varint with a continuation bit, at least one byte.
fn push_varint(out: &mut Vec<u8>, mut value: u32) {
    loop {
        let byte = value as u8 & 0x7F;
        value >>= 7;
        out.push(if value != 0 { byte | 0x80 } else { byte });
        if value == 0 {
            break;
        }
    }
}

/// `sub_469B70`: alternating runs, starting with a literal run (count and
/// bytes) that ends before two zero bytes, then a zero run (count only)
/// that ends at the next non-zero byte. The target reads one byte past the
/// end when it tests for two zeros; that byte is taken as 0 here.
fn zero_runs(input: &[u8]) -> Vec<u8> {
    let mut out = Vec::with_capacity(input.len() * 2);
    let byte = |at: usize| input.get(at).copied().unwrap_or(0);
    let (mut at, mut count, mut start, mut literal) = (0, 0u32, 0, true);
    while at < input.len() {
        let ends = if literal { byte(at) == 0 && byte(at + 1) == 0 } else { byte(at) != 0 };
        if !ends {
            at += 1;
            count += 1;
            if at != input.len() {
                continue;
            }
        } else if !literal {
            start = at;
        }
        push_varint(&mut out, count);
        if literal {
            out.extend_from_slice(&input[start..start + count as usize]);
        }
        literal = !literal;
        count = 0;
    }
    out
}

#[derive(Clone, Copy)]
struct Node {
    active: bool,
    weight: u32,
    parent: i32,
    right: i32,
}

/// `sub_469D00` + `sub_469EB0`: the Huffman tree over the byte counts
/// (two lowest active weights, first index on ties, merged into the next
/// node until one holds the total) and each leaf's code, root first.
fn huffman_codes(counts: &[u32; 256]) -> Vec<Vec<bool>> {
    let mut nodes = vec![Node { active: false, weight: 0, parent: -1, right: -1 }; 511];
    let mut total = 0u32;
    for (symbol, &count) in counts.iter().enumerate() {
        nodes[symbol] = Node { active: count != 0, weight: count, parent: -1, right: symbol as i32 };
        total = total.wrapping_add(count);
    }
    if total == 0 {
        return vec![Vec::new(); 256];
    }
    let mut next = 256;
    loop {
        let mut picks = [-1i32; 2];
        for pick in &mut picks {
            let mut best = u32::MAX;
            for index in 0..next {
                if nodes[index].active && nodes[index].weight < best {
                    *pick = index as i32;
                    best = nodes[index].weight;
                }
            }
            if *pick != -1 {
                let node = &mut nodes[*pick as usize];
                node.active = false;
                node.parent = next as i32;
            }
        }
        let weight = |pick: i32| if pick == -1 { 0 } else { nodes[pick as usize].weight };
        let sum = weight(picks[1]).wrapping_add(weight(picks[0]));
        nodes[next] = Node { active: true, weight: sum, parent: -1, right: picks[1] };
        if sum == total {
            break;
        }
        next += 1;
    }
    (0..256)
        .map(|symbol| {
            let mut code = Vec::new();
            let mut node = symbol;
            while nodes[node].parent != -1 {
                let parent = nodes[node].parent as usize;
                code.push(nodes[parent].right == node as i32);
                node = parent;
            }
            code.reverse();
            code
        })
        .collect()
}

/// `sub_46A320`: `header` is the 16-byte image descriptor (u16 width,
/// height, bit depth, 0, format, 0...), `pixels` packed rows at that depth,
/// `key` the GetTickCount value stored at +36.
pub fn encode_cbg_v1(header: &[u8; 16], pixels: &[u8], key: u32) -> Vec<u8> {
    let width = usize::from(u16::from_le_bytes([header[0], header[1]]));
    let height = usize::from(u16::from_le_bytes([header[2], header[3]]));
    let bytes_per_pixel = usize::from(u16::from_le_bytes([header[4], header[5]]) >> 3);
    let runs = zero_runs(&predict(width, height, bytes_per_pixel, pixels));
    let mut counts = [0u32; 256];
    for &byte in &runs {
        counts[usize::from(byte)] += 1;
    }
    let codes = huffman_codes(&counts);

    // sub_46A110: header, varint counts, their checksums, then the counts
    // encrypted with the key stream.
    let mut out = b"CompressedBG___\0".to_vec();
    out.extend_from_slice(header);
    out.extend_from_slice(&(runs.len() as u32).to_le_bytes());
    out.extend_from_slice(&key.to_le_bytes());
    let mut table = Vec::new();
    for &count in &counts {
        push_varint(&mut table, count);
    }
    out.extend_from_slice(&(table.len() as u32).to_le_bytes());
    // sub_46A010.
    let sum = table.iter().fold(0u8, |sum, &byte| sum.wrapping_add(byte));
    let xor = table.iter().fold(0u8, |xor, &byte| xor ^ byte);
    out.extend_from_slice(&[sum, xor]);
    out.extend_from_slice(&1u16.to_le_bytes());
    let mut stream = key;
    out.extend(table.iter().map(|&byte| byte.wrapping_add(next_key(&mut stream) as u8)));

    // sub_46A1B0: codes MSB first.
    let (mut current, mut used) = (0u8, 0);
    for &byte in &runs {
        for &bit in &codes[usize::from(byte)] {
            if bit {
                current |= 0x80 >> used;
            }
            used += 1;
            if used == 8 {
                out.push(current);
                current = 0;
                used = 0;
            }
        }
    }
    if used != 0 {
        out.push(current);
    }
    out
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn encoded_images_decode_back_to_their_pixels() {
        let mut seed = 1622u32;
        let mut random = || {
            seed = seed.wrapping_mul(1_103_515_245).wrapping_add(12_345);
            (seed >> 16) as u8
        };
        for (width, height, bits, format) in [(13u16, 7u16, 24u16, 1u16), (8, 9, 32, 2), (1, 1, 24, 1)] {
            let mut header = [0u8; 16];
            header[0..2].copy_from_slice(&width.to_le_bytes());
            header[2..4].copy_from_slice(&height.to_le_bytes());
            header[4..6].copy_from_slice(&bits.to_le_bytes());
            header[8..10].copy_from_slice(&format.to_le_bytes());
            let count = usize::from(width) * usize::from(height) * usize::from(bits / 8);
            let pixels = (0..count).map(|i| if i % 5 < 2 { 0 } else { random() }).collect::<Vec<_>>();
            let encoded = encode_cbg_v1(&header, &pixels, 0x1234_5678);
            let decoded = crate::decode_cbg_native_buffer(&encoded).unwrap();
            let expected: Vec<u8> = if bits == 24 {
                pixels.chunks_exact(3).flat_map(|p| [p[0], p[1], p[2], 0]).collect()
            } else {
                pixels.clone()
            };
            assert_eq!(&decoded[16..], &expected[..], "{width}x{height}x{bits}");
        }
    }
}

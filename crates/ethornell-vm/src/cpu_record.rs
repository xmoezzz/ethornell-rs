//! The target's processor record (`dword_518930`, 16 dwords) read by
//! Sys80:0A, and the object-cache budget derived from it (Sys80:0B).
//!
//! sub_46F810(NULL) -> sub_46F100 fills the record once at startup:
//! [0] vendor index (sub_46EF10: GenuineIntel 0, AuthenticAMD 1,
//! CentaurHauls 2, GenuineTMx86 3, other 4), [1] family, [2] model,
//! [3] stepping, [4] brand (Intel: CPUID 1 EBX low byte, others CPUID
//! 0x80000001 EBX low word), [5]/[6]/[7] L1/L2/L3 cache words (Intel: CPUID 2
//! descriptors and CPUID 4 levels; others: CPUID 0x80000005/6), [8] the
//! measured clock in MHz (sub_46F080, RDTSC over one second), [9] the
//! processor count. The rest stays zero (.bss). A failed CPUID
//! query returns early and leaves the remaining fields untouched; a host
//! without CPUID behaves like a target whose first query fails.

#[derive(Clone, Copy, Default)]
struct Cpuid {
    eax: u32,
    ebx: u32,
    ecx: u32,
    edx: u32,
}

/// sub_46EDF0: query `leaf`/`subleaf` only when the base or extended maximum
/// leaf reaches it.
fn cpuid(leaf: u32, subleaf: u32) -> Option<Cpuid> {
    #[cfg(any(target_arch = "x86", target_arch = "x86_64"))]
    {
        #[cfg(target_arch = "x86")]
        use std::arch::x86::__cpuid_count;
        #[cfg(target_arch = "x86_64")]
        use std::arch::x86_64::__cpuid_count;
        let base = if leaf < 0x8000_0000 { 0 } else { 0x8000_0000 };
        if __cpuid_count(base, 0).eax < leaf {
            return None;
        }
        let r = __cpuid_count(leaf, subleaf);
        return Some(Cpuid { eax: r.eax, ebx: r.ebx, ecx: r.ecx, edx: r.edx });
    }
    #[allow(unreachable_code)]
    {
        let _ = (leaf, subleaf);
        None
    }
}

/// sub_46EF10: store the vendor index, family, model, stepping and brand
/// word into `out[0..5]` as each query succeeds; false when one fails.
fn store_signature(out: &mut [u32]) -> bool {
    let Some(vendor) = cpuid(0, 0) else { return false };
    let mut name = Vec::with_capacity(12);
    for word in [vendor.ebx, vendor.edx, vendor.ecx] {
        name.extend_from_slice(&word.to_le_bytes());
    }
    let name = name.split(|b| *b == 0).next().unwrap_or_default();
    out[0] = [&b"GenuineIntel"[..], b"AuthenticAMD", b"CentaurHauls", b"GenuineTMx86"]
        .iter()
        .position(|known| name == *known)
        .unwrap_or(4) as u32;
    let Some(leaf1) = cpuid(1, 0) else { return false };
    let family = (leaf1.eax >> 8) & 0xF;
    out[1] = if family == 0 || family == 15 {
        family | ((leaf1.eax >> 16) & 0xFF0)
    } else {
        family
    };
    out[2] = ((leaf1.eax & 0xF0) | ((leaf1.eax >> 8) & 0xF00)) >> 4;
    out[3] = leaf1.eax & 0xF;
    if out[0] == 0 {
        out[4] = leaf1.ebx & 0xFF;
        return true;
    }
    let Some(extended) = cpuid(0x8000_0001, 0) else { return false };
    out[4] = extended.ebx & 0xFFFF;
    true
}

/// The CPUID 2 descriptor switch of sub_46F100 (jump table at 0x46F230):
/// the record slot (5 = L1, 6 = L2, 7 = L3) and the cache word. 0x49 is an L3
/// on family 15 and an L2 otherwise.
fn descriptor(byte: u8, family: u32) -> Option<(usize, u32)> {
    Some(match byte {
        0x0A => (5, 0x2002_0008),
        0x0C => (5, 0x2004_0010),
        0x0D => (5, 0x4004_0010),
        0x0E => (5, 0x4006_0018),
        0x21 => (6, 0x4008_0100),
        0x22 | 0xD0 => (7, 0x4004_0200),
        0x23 | 0xD6 => (7, 0x4008_0400),
        0x25 | 0xD7 => (7, 0x4008_0800),
        0x29 | 0xD8 => (7, 0x4008_1000),
        0x2C => (5, 0x4008_0020),
        0x41 => (6, 0x2004_0080),
        0x42 => (6, 0x2004_0100),
        0x43 => (6, 0x2004_0200),
        0x44 => (6, 0x2004_0400),
        0x45 => (6, 0x2004_0800),
        0x46 => (7, 0x4004_1000),
        0x47 => (7, 0x4008_2000),
        0x48 => (6, 0x400C_0C00),
        0x49 => (if family == 15 { 7 } else { 6 }, 0x4010_1000),
        0x4A | 0xDE => (7, 0x400C_1800),
        0x4B | 0xE4 => (7, 0x4010_2000),
        0x4C => (7, 0x400C_3000),
        0x4D => (7, 0x4010_4000),
        0x4E => (6, 0x4018_1800),
        0x60 => (5, 0x4008_0010),
        0x66 => (5, 0x4004_0008),
        0x67 => (5, 0x4004_0010),
        0x68 => (5, 0x4004_0020),
        0x78 => (6, 0x4004_0400),
        0x79 => (6, 0x4008_0080),
        0x7A => (6, 0x4008_0100),
        0x7B => (6, 0x4008_0200),
        0x7C => (6, 0x4008_0400),
        0x7D => (6, 0x4008_0800),
        0x7F => (6, 0x4002_0200),
        0x80 => (6, 0x4008_0200),
        0x81 => (6, 0x2008_0080),
        0x82 => (6, 0x2008_0100),
        0x83 => (6, 0x2008_0200),
        0x84 => (6, 0x2008_0400),
        0x85 => (6, 0x2008_0800),
        0x86 => (6, 0x4004_0200),
        0x87 => (6, 0x4008_0400),
        0xD1 => (7, 0x4004_0400),
        0xD2 => (7, 0x4004_0800),
        0xDC => (7, 0x400C_0600),
        0xDD => (7, 0x400C_0C00),
        0xE2 => (7, 0x4010_0800),
        0xE3 => (7, 0x4010_1000),
        0xEA => (7, 0x4018_3000),
        0xEB => (7, 0x4018_4800),
        0xEC => (7, 0x4018_6000),
        _ => return None,
    })
}

/// sub_46F080: core clock in MHz, RDTSC ticks across one second.
fn measured_mhz() -> u32 {
    #[cfg(any(target_arch = "x86", target_arch = "x86_64"))]
    {
        #[cfg(target_arch = "x86")]
        use std::arch::x86::_rdtsc;
        #[cfg(target_arch = "x86_64")]
        use std::arch::x86_64::_rdtsc;
        let start = std::time::Instant::now();
        // SAFETY: RDTSC has no preconditions on x86.
        let begin = unsafe { _rdtsc() };
        while start.elapsed() < std::time::Duration::from_secs(1) {}
        let end = unsafe { _rdtsc() };
        return (end.wrapping_sub(begin) / 1_000_000) as u32;
    }
    #[allow(unreachable_code)]
    0
}

/// sub_46F100 applied to the static record.
pub fn processor_record() -> [u32; 16] {
    let mut record = [0; 16];
    if !store_signature(&mut record) {
        return record;
    }
    let family = record[1];
    record[5] = 0;
    record[6] = 0;
    record[7] = 0;
    if record[0] == 0 {
        if let Some(first) = cpuid(2, 0) {
            let rounds = first.eax & 0xFF;
            let mut bytes = Vec::with_capacity(16 * rounds as usize);
            for _ in 0..rounds {
                let r = cpuid(2, 0).unwrap_or_default();
                let mut regs = [r.eax & 0xFFFF_FF00, r.ebx, r.ecx, r.edx];
                for reg in &mut regs {
                    if *reg & 0x8000_0000 != 0 {
                        *reg = 0;
                    }
                }
                for reg in regs {
                    bytes.extend_from_slice(&reg.to_le_bytes());
                }
            }
            for byte in bytes {
                if byte == 0xFF {
                    for subleaf in 0..4 {
                        let Some(r) = cpuid(4, subleaf) else { continue };
                        if !matches!(r.eax & 0x1F, 1 | 3) {
                            continue;
                        }
                        let line = (r.ebx & 0xFFF) + 1;
                        let partitions = ((r.ebx >> 12) & 0x3FF) + 1;
                        let ways = (r.ebx >> 22) + 1;
                        let size = partitions
                            .wrapping_mul(r.ecx.wrapping_add(1))
                            .wrapping_mul(ways)
                            .wrapping_mul(line)
                            >> 10;
                        let word = size | (((line << 8) | ways) << 16);
                        match (r.eax >> 5) & 7 {
                            1 => record[5] = word,
                            2 => record[6] = word,
                            3 => record[7] = word,
                            _ => {}
                        }
                    }
                } else if let Some((slot, word)) = descriptor(byte, family) {
                    record[slot] = word;
                }
            }
        }
    } else if let Some(l1) = cpuid(0x8000_0005, 0) {
        record[5] = (l1.ecx << 24) | (l1.ecx >> 24) | (l1.ecx & 0x00FF_0000);
        if let Some(l2) = cpuid(0x8000_0006, 0) {
            let assoc = (l2.ecx >> 12) & 0xF;
            let ways = if assoc != 0 { 1 << (assoc >> 1) } else { 0 };
            record[6] = (((l2.ecx << 8) | ways) << 16) | (l2.ecx >> 16);
        }
    }
    record[8] = measured_mhz();
    record[9] = std::thread::available_parallelism()
        .map(|count| count.get() as u32)
        .unwrap_or(1);
    record
}

/// sub_46F870: logical processors per package from CPUID 1 EBX[23:16] on
/// an Intel CPU with HTT (EDX bit 28), otherwise 1.
fn logical_per_package() -> u32 {
    let mut signature = [0; 5];
    if !store_signature(&mut signature) || signature[0] != 0 {
        return 1;
    }
    match cpuid(1, 0) {
        Some(leaf1) if leaf1.edx & (1 << 28) != 0 => (leaf1.ebx >> 16) & 0xFF,
        _ => 1,
    }
}

/// sub_46FB30: the object manager's cache budget for a display of
/// `width` x `height` (the current entry of the display-mode tables), from
/// the L1/L2 words and processor count of `record`. All arithmetic is the
/// target's 32-bit unsigned arithmetic.
pub fn object_cache_budget(record: &[u32; 16], width: u32, height: u32) -> u32 {
    let l1 = record[5] & 0xFFFF;
    if l1 >= 0x40 {
        return l1.wrapping_mul(0x18700) / 1000;
    }
    let processors = record[9];
    // The target divides unchecked; a zero processor count would fault.
    let rows = height.checked_div(processors.wrapping_mul(6)).unwrap_or(0);
    let mut l2 = record[6] & 0xFFFF;
    if l2 == 0 {
        return 0x2800;
    }
    if processors <= 1 {
        let kib = (u64::from(l2) << 10) as i64 / 4;
        return (kib * 0x7271 / 100_000) as u32;
    }
    if logical_per_package() >= 2 {
        l2 >>= 1;
    }
    let bytes = (l2 << 10) >> 2;
    let (scaled, divisor) = if l2 >= 0x100 {
        (bytes.wrapping_mul(0x125), width.wrapping_mul(1000))
    } else {
        (bytes.wrapping_mul(0xC35), width.wrapping_mul(10000))
    };
    scaled.checked_div(divisor).unwrap_or(0).min(rows).wrapping_mul(width)
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn cache_budget_follows_the_target_integer_paths() {
        let mut record = [0; 16];
        // No CPUID: zero L2 word (the target's processor-count division
        // would fault first; its result is unused on this path).
        assert_eq!(object_cache_budget(&record, 1280, 720), 0x2800);
        record[5] = 0xFFFF;
        // A large L1 word takes the first path, whose 32-bit product wraps.
        assert_eq!(object_cache_budget(&record, 1280, 720), 2_264_824);
        record[5] = 0x2004_0020; // 32 KiB L1
        record[6] = 0;
        record[9] = 4;
        assert_eq!(object_cache_budget(&record, 1280, 720), 0x2800);
        record[6] = 0x4008_0100; // 256 KiB L2
        record[9] = 1;
        assert_eq!(object_cache_budget(&record, 1280, 720), 19_200);
    }
}

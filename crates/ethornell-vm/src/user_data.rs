use crate::{BTreeMap, SysApi, Vm, VmError, VmResult};

pub(crate) const GLOBAL_CONFIG_SIZE: usize = 0x400;
pub(crate) const GLOBAL_USER_DATA_SIZE: usize = 0x10_0000;

const RAW_FIXED_SIZE: usize = 0x10_0424;
const SDC_HEADER_SIZE: usize = 32;
const GDB_MAGIC: &[u8; 16] = b"BURIKO GDB 3.00\0";
const SDC_MAGIC: &[u8; 16] = b"SDC FORMAT 1.00\0";

#[derive(Debug, Clone, Copy, Default)]
pub(crate) struct GlobalLoadResult {
    pub(crate) window_x: i32,
    pub(crate) window_y: i32,
    pub(crate) status: i32,
}

impl Vm {
    pub(crate) fn encode_user_data_buffer(
        &mut self,
        destination: u32,
        source: u32,
        length: i32,
    ) -> VmResult<i32> {
        // A zero length still produces the 32-byte header (sub_494470).
        let Ok(length) = usize::try_from(length) else {
            return Ok(0);
        };
        let source = self.resolve_range(source, length)?;
        let encoded = encode_sdc(&self.memory[source], sdc_seed());
        let output = self.resolve_write_range(destination, encoded.len())?;
        self.memory[output].copy_from_slice(&encoded);
        self.clear_shadow_values(destination, encoded.len());
        Ok(encoded.len().min(i32::MAX as usize) as i32)
    }

    pub(crate) fn encode_user_data_structs(
        &mut self,
        destination: u32,
        source: u32,
        record_size: i32,
        record_count: i32,
    ) -> VmResult<i32> {
        let (Ok(record_size), Ok(record_count)) =
            (usize::try_from(record_size), usize::try_from(record_count))
        else {
            return Ok(0);
        };
        // sub_447590 fails for a zero size or count without writing a
        // length, so sub_496A00 encodes an empty stream (32 bytes).
        if record_size == 0 || record_count == 0 {
            let encoded = encode_sdc(&[], sdc_seed());
            let output = self.resolve_write_range(destination, encoded.len())?;
            self.memory[output].copy_from_slice(&encoded);
            self.clear_shadow_values(destination, encoded.len());
            return Ok(encoded.len() as i32);
        }
        let byte_len = record_size
            .checked_mul(record_count)
            .ok_or_else(|| VmError::Runtime("DCFS source size overflow".into()))?;
        let source = self.resolve_range(source, byte_len)?;
        let dcfs = encode_dcfs(&self.memory[source], record_size, record_count)?;
        let encoded = encode_sdc(&dcfs, sdc_seed());
        let output = self.resolve_write_range(destination, encoded.len())?;
        self.memory[output].copy_from_slice(&encoded);
        self.clear_shadow_values(destination, encoded.len());
        Ok(encoded.len().min(i32::MAX as usize) as i32)
    }

    pub(crate) fn allocate_global_config(&mut self, shift: i32) -> bool {
        let Ok(shift) = u32::try_from(shift) else {
            return false;
        };
        if shift > 12 {
            return false;
        }
        // sub_46C0F0: a fresh zeroed block of 4096 << shift bytes.
        let size = 4096usize << shift;
        self.global_size = size;
        if self.memory.len() < size {
            self.memory.resize(size, 0);
        }
        self.memory[..size].fill(0);
        self.clear_shadow_values(0, size);
        true
    }

    /// dword_56676C: the global segment the scripts address from 0.
    pub(crate) fn global_bytes(&self) -> &[u8] {
        &self.memory[..self.global_size.min(self.memory.len())]
    }

    pub(crate) fn global_bytes_mut(&mut self) -> &mut [u8] {
        let size = self.global_size.min(self.memory.len());
        &mut self.memory[..size]
    }

    pub(crate) fn save_config_slot<A: SysApi>(
        &mut self,
        api: &mut A,
        slot: i32,
        label: &str,
    ) -> VmResult<bool> {
        let (encoded_label, _, _) = encoding_rs::SHIFT_JIS.encode(label);
        if encoded_label.len() >= 40 {
            return Err(VmError::Runtime(
                "付加情報文字列が長すぎます\n\n文字列は39文字以内である必要があります".into(),
            ));
        }
        let mut bytes = vec![0u8; 64 + self.global_size];
        // GetLocalTime; wMilliseconds (+0x0E) seeds the integrity stream.
        for (index, value) in crate::portable_local_system_time().into_iter().enumerate() {
            bytes[index * 2..index * 2 + 2].copy_from_slice(&value.to_le_bytes());
        }
        bytes[16..16 + encoded_label.len()].copy_from_slice(&encoded_label);
        bytes[16 + encoded_label.len()] = 0;
        bytes[64..].copy_from_slice(self.global_bytes());
        if self.save_data_integrity_enabled != 0 {
            encrypt_config_slot(&mut bytes);
        }
        let path = self.config_slot_path(api, slot);
        Ok(api.write_file_bytes(&path, &bytes))
    }

    /// sub_4664D0 + sub_46B960: validated root (byte_518988, which starts
    /// as the primary root) + "BGI%.4d.cad"; sub_466390/sub_465F50 then use
    /// it as is when absolute, else below the primary root.
    fn config_slot_path<A: SysApi>(&self, api: &mut A, slot: i32) -> String {
        let root = self
            .validated_file_root
            .clone()
            .unwrap_or_else(|| self.resource_primary_root(api));
        format!("{root}{}", config_slot_name(slot))
    }

    pub(crate) fn load_config_slot<A: SysApi>(&mut self, api: &mut A, slot: i32) -> i32 {
        let preserved_len = self.global_size.min(1024);
        let preserved = self.global_bytes()[..preserved_len].to_vec();
        let status = match self.read_config_slot(api, slot) {
            Ok(bytes) => {
                self.global_bytes_mut().copy_from_slice(&bytes[64..]);
                0
            }
            Err(status) => status,
        };
        self.global_bytes_mut()[..preserved_len].copy_from_slice(&preserved);
        let size = self.global_size;
        self.clear_shadow_values(0, size);
        status
    }

    pub(crate) fn read_config_slot_header<A: SysApi>(
        &mut self,
        api: &mut A,
        slot: i32,
    ) -> Result<[u8; 64], i32> {
        let path = self.config_slot_path(api, slot);
        let Some(bytes) = api.read_user_file_bytes(&path) else {
            return Err(1);
        };
        if bytes.is_empty() {
            return Err(1);
        }
        if bytes.len() != 64 + self.global_size {
            return Err(2);
        }
        let mut header = [0u8; 64];
        header.copy_from_slice(&bytes[..64]);
        Ok(header)
    }

    pub(crate) fn validate_config_slot<A: SysApi>(&mut self, api: &mut A, slot: i32) -> i32 {
        match self.read_config_slot(api, slot) {
            Ok(_) => 0,
            Err(status) => status,
        }
    }

    fn read_config_slot<A: SysApi>(&mut self, api: &mut A, slot: i32) -> Result<Vec<u8>, i32> {
        let path = self.config_slot_path(api, slot);
        let Some(mut bytes) = api.read_user_file_bytes(&path) else {
            return Err(1);
        };
        if bytes.is_empty() {
            return Err(1);
        }
        if bytes.len() != 64 + self.global_size {
            return Err(2);
        }
        if self.save_data_integrity_enabled != 0 && !decrypt_and_validate_config_slot(&mut bytes) {
            return Err(3);
        }
        Ok(bytes)
    }

    /// Sys80:80 (sub_48A290 -> sub_46BBE0): validated root + "BGI.gdb";
    /// missing -> 1, unreadable -> 2, else the payload is installed (status
    /// 0, or 2 when a legacy file's sizes do not add up). The stored window
    /// position goes through the host's monitor check (sub_46FE90 /
    /// sub_461B40).
    pub(crate) fn load_global_user_data<A: SysApi>(
        &mut self,
        api: &mut A,
    ) -> VmResult<GlobalLoadResult> {
        let path = self.global_data_path(api);
        let failed = |status| GlobalLoadResult { status, ..GlobalLoadResult::default() };
        let Some(encoded) = api.read_user_file_bytes(&path).filter(|bytes| !bytes.is_empty()) else {
            return Ok(failed(1));
        };
        let Ok(raw) = decode_sdc(&encoded) else {
            return Ok(failed(2));
        };
        let installed = if raw.starts_with(GDB_MAGIC) {
            self.install_global_data(&raw)
        } else if raw.len() >= 0x70408 {
            self.install_legacy_global_data(&raw)
        } else {
            None
        };
        let Some((window_x, window_y, status)) = installed else {
            return Ok(failed(2));
        };
        let (window_x, window_y) = api.restore_window_position(window_x, window_y);
        Ok(GlobalLoadResult { window_x, window_y, status })
    }

    /// sub_46B960(.., "BGI.gdb").
    fn global_data_path<A: SysApi>(&self, api: &mut A) -> String {
        let root = self
            .validated_file_root
            .clone()
            .unwrap_or_else(|| self.resource_primary_root(api));
        format!("{root}BGI.gdb")
    }

    pub(crate) fn save_global_user_data<A: SysApi>(&mut self, api: &mut A) -> bool {
        // sub_46BA20: header +20/+24 = GetWindowRect left/top.
        let mut raw = self.serialize_global_data();
        let (x, y) = api.window_outer_position();
        write_u32(&mut raw, 20, x as u32);
        write_u32(&mut raw, 24, y as u32);
        let encoded = encode_sdc(&raw, sdc_seed());
        let path = self.global_data_path(api);
        api.write_file_bytes(&path, &encoded)
    }

    pub(crate) fn copy_to_global_data(
        &mut self,
        offset: i32,
        src: u32,
        length: i32,
    ) -> VmResult<()> {
        let range = checked_global_range(offset, length)?;
        let src_range = self.resolve_range(src, range.len())?;
        self.global_user_data[range].copy_from_slice(&self.memory[src_range]);
        Ok(())
    }

    pub(crate) fn copy_from_global_data(
        &mut self,
        dst: u32,
        offset: i32,
        length: i32,
    ) -> VmResult<()> {
        let range = checked_global_range(offset, length)?;
        let bytes = self.global_user_data[range].to_vec();
        let dst_range = self.resolve_write_range(dst, bytes.len())?;
        self.memory[dst_range].copy_from_slice(&bytes);
        self.clear_shadow_values(dst, bytes.len());
        Ok(())
    }

    fn serialize_global_data(&self) -> Vec<u8> {
        let names = self.resource_names().to_vec();
        let names_size = 4usize + names.iter().map(|name| sjis_len(name) + 1).sum::<usize>();
        let read_flags_size = 4usize
            + self
                .read_flags
                .iter()
                .map(|(name, flags)| sjis_len(name) + 1 + 4 + flags.bytes.len())
                .sum::<usize>();
        let mut raw = vec![0u8; RAW_FIXED_SIZE + names_size + read_flags_size];
        let raw_len = raw.len() as u32;
        raw[..16].copy_from_slice(GDB_MAGIC);
        write_u32(&mut raw, 16, raw_len);
        write_u32(&mut raw, 28, GLOBAL_CONFIG_SIZE as u32);
        let config_len = self.global_size.min(GLOBAL_CONFIG_SIZE);
        raw[32..32 + config_len].copy_from_slice(&self.global_bytes()[..config_len]);
        write_u32(&mut raw, 0x420, GLOBAL_USER_DATA_SIZE as u32);
        raw[0x424..RAW_FIXED_SIZE].copy_from_slice(&self.global_user_data);

        let mut cursor = RAW_FIXED_SIZE;
        write_u32(&mut raw, cursor, names.len() as u32);
        cursor += 4;
        for name in names {
            cursor += write_sjis_c_string(&mut raw[cursor..], &name);
        }
        write_u32(&mut raw, cursor, self.read_flags.len() as u32);
        cursor += 4;
        for (name, flags) in &self.read_flags {
            cursor += write_sjis_c_string(&mut raw[cursor..], name);
            write_u32(&mut raw, cursor, flags.bit_len);
            cursor += 4;
            raw[cursor..cursor + flags.bytes.len()].copy_from_slice(&flags.bytes);
            cursor += flags.bytes.len();
        }
        raw
    }

    /// The "BURIKO GDB 3.00" layout: size, window x/y, then the config
    /// block, the user data (zero-filled to 1 MiB), the resource names and
    /// the read flags, each with the size the file declares.
    fn install_global_data(&mut self, raw: &[u8]) -> Option<(i32, i32, i32)> {
        let u32_at = |offset: usize| -> Option<usize> {
            raw.get(offset..offset + 4).map(|v| u32::from_le_bytes(v.try_into().unwrap()) as usize)
        };
        if u32_at(16)? != raw.len() {
            return None;
        }
        let (x, y) = (u32_at(20)? as i32, u32_at(24)? as i32);
        let config_len = u32_at(28)?;
        let config = raw.get(32..32usize.checked_add(config_len)?)?;
        let mut cursor = 32 + config_len;
        let data_len = u32_at(cursor)?;
        let data = raw.get(cursor + 4..(cursor + 4).checked_add(data_len)?)?;
        cursor += 4 + data_len;
        let mut names_cursor = cursor;
        let name_count = read_u32_at_cursor(raw, &mut names_cursor).ok()?;
        let mut names = Vec::new();
        for _ in 0..name_count {
            names.push(read_sjis_c_string(raw, &mut names_cursor).ok()?);
        }
        let mut flags_cursor = names_cursor;
        let flag_count = read_u32_at_cursor(raw, &mut flags_cursor).ok()?;
        let mut flags = BTreeMap::new();
        for _ in 0..flag_count {
            let name = read_sjis_c_string(raw, &mut flags_cursor).ok()?;
            let bit_len = read_u32_at_cursor(raw, &mut flags_cursor).ok()?;
            let end = flags_cursor.checked_add(bit_len.div_ceil(8) as usize)?;
            let bytes = raw.get(flags_cursor..end)?.to_vec();
            flags.entry(name).or_insert(crate::ReadFlagBits { bit_len, bytes });
            flags_cursor = end;
        }
        self.install_global_config(config);
        self.install_global_user_data(data);
        self.set_resource_names(names);
        self.read_flags = flags;
        Some((x, y, 0))
    }

    /// The pre-3.00 fixed layout (sub_46BBE0's second branch): x, y, 1 KiB
    /// config, 256 KiB user data, 32-byte name records (4096, or 16384 in
    /// the later variant) and 2048 read-flag records {name[24], bits,
    /// offset}. The data is installed even when the sizes do not add up,
    /// which only changes the status to 2.
    fn install_legacy_global_data(&mut self, raw: &[u8]) -> Option<(i32, i32, i32)> {
        let u32_at = |offset: usize| -> u32 {
            raw.get(offset..offset + 4).map_or(0, |v| u32::from_le_bytes(v.try_into().unwrap()))
        };
        let c_string = |bytes: &[u8]| -> Option<String> {
            let end = bytes.iter().position(|&b| b == 0)?;
            Some(encoding_rs::SHIFT_JIS.decode(&bytes[..end]).0.into_owned())
        };
        let first_flag = raw.get(394248..394248 + 24)?;
        let later = first_flag[0] == 0
            || first_flag.iter().position(|&b| b == 0).is_none()
            || u32_at(394272) == 0
            || u32_at(394276) != 0;
        let (name_count, flags_base, data_base) =
            if later { (0x4000, 787464, 853000) } else { (4096, 394248, 459784) };
        self.install_global_config(raw.get(8..1032)?);
        self.install_global_user_data(raw.get(1032..1032 + 0x40000)?);
        let mut names: Vec<String> = Vec::new();
        for index in 0..name_count {
            let record = raw.get(263176 + index * 32..263176 + index * 32 + 32)?;
            if record[0] != 0
                && let Some(name) = c_string(record)
                && !names.contains(&name)
            {
                names.push(name);
            }
        }
        self.set_resource_names(names);
        self.read_flags.clear();
        let mut total = 0usize;
        for index in 0..2048 {
            let record = flags_base + index * 32;
            let bit_len = u32_at(record + 24);
            if bit_len == 0 {
                continue;
            }
            let byte_len = bit_len.div_ceil(8) as usize;
            let start = data_base + u32_at(record + 28) as usize;
            let name = c_string(raw.get(record..record + 24)?)?;
            let bytes = raw.get(start..start + byte_len)?.to_vec();
            self.read_flags.entry(name).or_insert(crate::ReadFlagBits { bit_len, bytes });
            total += byte_len;
        }
        let status = if raw.len() == data_base + total { 0 } else { 2 };
        Some((u32_at(0) as i32, u32_at(4) as i32, status))
    }

    /// memcpy into dword_56676C (the global segment; its first 1 KiB is the
    /// BP configuration block).
    fn install_global_config(&mut self, config: &[u8]) {
        let len = config.len().min(self.global_size.max(GLOBAL_CONFIG_SIZE));
        if self.memory.len() < len {
            self.memory.resize(len, 0);
        }
        self.memory[..len].copy_from_slice(&config[..len]);
        self.clear_shadow_values(0, len);
    }

    /// dword_566774: the data, then zeros up to 1 MiB.
    fn install_global_user_data(&mut self, data: &[u8]) {
        let len = data.len().min(GLOBAL_USER_DATA_SIZE);
        self.global_user_data[..len].copy_from_slice(&data[..len]);
        self.global_user_data[len..].fill(0);
    }
}

fn config_slot_name(slot: i32) -> String {
    format!("BGI{slot:04}.cad")
}

fn write_native_system_time(destination: &mut [u8]) {
    let now = ethornell_core::clock::since_epoch();
    let seconds = now.as_secs();
    let days = (seconds / 86_400) as i64;
    let (year, month, day) = civil_from_days_for_save(days);
    let fields = [
        year as u16,
        month as u16,
        ((seconds / 86_400 + 4) % 7) as u16,
        day as u16,
        ((seconds / 3_600) % 24) as u16,
        ((seconds / 60) % 60) as u16,
        (seconds % 60) as u16,
        now.subsec_millis() as u16,
    ];
    for (index, value) in fields.into_iter().enumerate() {
        destination[index * 2..index * 2 + 2].copy_from_slice(&value.to_le_bytes());
    }
}

pub(crate) fn civil_from_days_for_save(days: i64) -> (i32, u32, u32) {
    let z = days + 719_468;
    let era = if z >= 0 { z } else { z - 146_096 } / 146_097;
    let doe = z - era * 146_097;
    let yoe = (doe - doe / 1_460 + doe / 36_524 - doe / 146_096) / 365;
    let mut year = yoe as i32 + era as i32 * 400;
    let doy = doe - (365 * yoe + yoe / 4 - yoe / 100);
    let mp = (5 * doy + 2) / 153;
    let day = doy - (153 * mp + 2) / 5 + 1;
    let month = mp + if mp < 10 { 3 } else { -9 };
    year += i32::from(month <= 2);
    (year, month as u32, day as u32)
}

fn slot_random_next(state: &mut u32) -> u16 {
    let high = ((22_695_477u64 * u64::from(*state)) >> 16) as u16;
    *state = (u32::from(high) << 16)
        .wrapping_add(u32::from((20_021u32.wrapping_mul(*state)) as u16))
        .wrapping_add(1);
    high & 0x7fff
}

fn encrypt_config_slot(bytes: &mut [u8]) {
    let mut state = u32::from(u16::from_le_bytes([bytes[14], bytes[15]]));
    let mut sum = 0u8;
    let mut xor = 0u8;
    for byte in &mut bytes[64..] {
        *byte = byte.wrapping_add(slot_random_next(&mut state) as u8);
        sum = sum.wrapping_add(*byte);
        xor ^= *byte;
    }
    bytes[56] = sum;
    bytes[57] = xor;
    bytes[58] = slot_random_next(&mut state) as u8;
    bytes[59] = slot_random_next(&mut state) as u8;
    bytes[60] = 1;
}

fn decrypt_and_validate_config_slot(bytes: &mut [u8]) -> bool {
    let mut state = u32::from(u16::from_le_bytes([bytes[14], bytes[15]]));
    let mut sum = 0u8;
    let mut xor = 0u8;
    for byte in &mut bytes[64..] {
        let encrypted = *byte;
        sum = sum.wrapping_add(encrypted);
        xor ^= encrypted;
        *byte = encrypted.wrapping_sub(slot_random_next(&mut state) as u8);
    }
    bytes[56] == sum && bytes[57] == xor
}

fn encode_dcfs(raw: &[u8], record_size: usize, record_count: usize) -> VmResult<Vec<u8>> {
    let expected = record_size
        .checked_mul(record_count)
        .ok_or_else(|| VmError::Runtime("DCFS source size overflow".into()))?;
    if raw.len() != expected || record_size == 0 || record_count == 0 {
        return Err(VmError::Runtime("invalid DCFS record array".into()));
    }

    let mut encoded = Vec::with_capacity(24 + expected.saturating_mul(3) / 2);
    encoded.extend_from_slice(b"DCFS FORMAT 1.00");
    encoded.extend_from_slice(&(record_size as u32).to_le_bytes());
    encoded.extend_from_slice(&(record_count as u32).to_le_bytes());
    encoded.extend_from_slice(&raw[..record_size]);

    for record_index in 1..record_count {
        let previous = &raw[(record_index - 1) * record_size..record_index * record_size];
        let current = &raw[record_index * record_size..(record_index + 1) * record_size];
        let mut cursor = 0usize;
        let mut different = false;
        while cursor < record_size {
            let start = cursor;
            if different {
                while cursor < record_size && previous[cursor] != current[cursor] {
                    cursor += 1;
                }
            } else {
                while cursor < record_size && previous[cursor] == current[cursor] {
                    cursor += 1;
                }
            }
            write_dcfs_varint(&mut encoded, cursor - start);
            if different {
                encoded.extend_from_slice(&current[start..cursor]);
            }
            different = !different;
        }
    }
    Ok(encoded)
}

fn write_dcfs_varint(output: &mut Vec<u8>, mut value: usize) {
    loop {
        let mut byte = (value & 0x7f) as u8;
        value >>= 7;
        if value != 0 {
            byte |= 0x80;
        }
        output.push(byte);
        if value == 0 {
            break;
        }
    }
}

/// sub_48A320 / sub_48A3E0: unsigned checks against the 1 MiB block.
fn checked_global_range(offset: i32, length: i32) -> VmResult<std::ops::Range<usize>> {
    let (offset, length) = (offset as u32, length as u32);
    if offset >= GLOBAL_USER_DATA_SIZE as u32 {
        return Err(VmError::Runtime(format!(
            "無効なオフセット [ ${offset:08X} ] が指定されました"
        )));
    }
    let limit = GLOBAL_USER_DATA_SIZE as u32;
    if offset.wrapping_add(length) > limit || length == 0 || length > limit {
        return Err(VmError::Runtime(format!(
            "オフセット [ ${offset:08X} ] から無効なデータサイズ [ {} ] が指定されました",
            length as i32
        )));
    }
    Ok(offset as usize..(offset + length) as usize)
}

/// sub_493E90: LZ with one circular list per byte value (newest position
/// first) over a 0x801-byte window; matches of 2..=17 bytes at distance
/// 2..=0x801 (distance 1 is skipped), the first longest match wins.
/// Literal runs of up to 128 bytes are flushed when full or before a match.
fn compress_sdc(src: &[u8]) -> Vec<u8> {
    const WINDOW: usize = 0x801;
    // Nodes 0..256 are the per-byte sentinels, 256.. the window ring.
    let mut next: Vec<usize> = (0..256 + WINDOW).collect();
    let mut prev = next.clone();
    let mut position = vec![0usize; 256 + WINDOW];
    let insert = |p: usize, next: &mut Vec<usize>, prev: &mut Vec<usize>, position: &mut Vec<usize>| {
        if p >= WINDOW {
            let sentinel = src[p - WINDOW] as usize;
            let tail = prev[sentinel];
            prev[sentinel] = prev[tail];
            next[prev[tail]] = sentinel;
        }
        let node = 256 + p % WINDOW;
        let sentinel = src[p] as usize;
        let first = next[sentinel];
        next[node] = first;
        prev[node] = sentinel;
        position[node] = p;
        prev[first] = node;
        next[sentinel] = node;
    };
    let mut out = Vec::new();
    let (mut p, mut literal_start, mut literals) = (0usize, 0usize, 0usize);
    while p < src.len() {
        let limit = (src.len() - p).min(17);
        let (mut best, mut distance) = (0usize, 0usize);
        let sentinel = src[p] as usize;
        let mut node = next[sentinel];
        while node != sentinel {
            let q = position[node];
            if p - q != 1 && limit > 1 {
                let mut length = 1;
                while length < limit && src[q + length] == src[p + length] {
                    length += 1;
                }
                if length > 1 {
                    if best < length {
                        best = length;
                        distance = p - q;
                    }
                    if length == limit {
                        break;
                    }
                }
            }
            node = next[node];
        }
        if best == 0 {
            insert(p, &mut next, &mut prev, &mut position);
            p += 1;
            literals += 1;
        }
        if literals >= 0x80 || (best > 1 && literals > 0) {
            out.push((literals - 1) as u8);
            out.extend_from_slice(&src[literal_start..literal_start + literals]);
            literals = 0;
            literal_start = p;
        }
        if best > 1 {
            for _ in 0..best {
                insert(p, &mut next, &mut prev, &mut position);
                p += 1;
            }
            let code = (((best - 2) << 11) + (distance - 2) + 0x8000) as u16;
            out.push((code >> 8) as u8);
            out.push(code as u8);
            literal_start = p;
        }
    }
    if literals > 0 {
        out.push((literals - 1) as u8);
        out.extend_from_slice(&src[literal_start..literal_start + literals]);
    }
    out
}

/// sub_494470 + sub_4942D0: "SDC FORMAT 1.00", seed (GetSystemTime
/// wMilliseconds), packed and raw sizes, then the packed bytes masked by the
/// seeded stream with their 16-bit sum and XOR.
/// GetSystemTime().wMilliseconds (the local time has the same milliseconds).
fn sdc_seed() -> u32 {
    u32::from(crate::portable_local_system_time()[7])
}

fn encode_sdc(raw: &[u8], seed: u32) -> Vec<u8> {
    let mut packed = compress_sdc(raw);
    let mut state = seed;
    for byte in &mut packed {
        *byte = byte.wrapping_add(next_sdc_random(&mut state));
    }
    let sum = packed
        .iter()
        .fold(0u16, |sum, byte| sum.wrapping_add(u16::from(*byte)));
    let xor = packed.iter().fold(0u16, |xor, byte| xor ^ u16::from(*byte));
    let mut encoded = vec![0u8; SDC_HEADER_SIZE + packed.len()];
    encoded[..16].copy_from_slice(SDC_MAGIC);
    write_u32(&mut encoded, 16, seed);
    write_u32(&mut encoded, 20, packed.len() as u32);
    write_u32(&mut encoded, 24, raw.len() as u32);
    encoded[28..30].copy_from_slice(&sum.to_le_bytes());
    encoded[30..32].copy_from_slice(&xor.to_le_bytes());
    encoded[32..].copy_from_slice(&packed);
    encoded
}

fn decode_sdc(encoded: &[u8]) -> VmResult<Vec<u8>> {
    if encoded.len() < SDC_HEADER_SIZE || encoded.get(..16) != Some(SDC_MAGIC) {
        return Err(VmError::Runtime("invalid SDC header".into()));
    }
    let seed = read_u32(encoded, 16)?;
    let packed_len = read_u32(encoded, 20)? as usize;
    let raw_len = read_u32(encoded, 24)? as usize;
    let end = SDC_HEADER_SIZE
        .checked_add(packed_len)
        .filter(|end| *end == encoded.len())
        .ok_or_else(|| VmError::Runtime("SDC packed length mismatch".into()))?;
    let mut packed = encoded[SDC_HEADER_SIZE..end].to_vec();
    let expected_sum = u16::from_le_bytes(encoded[28..30].try_into().unwrap());
    let expected_xor = u16::from_le_bytes(encoded[30..32].try_into().unwrap());
    let sum = packed
        .iter()
        .fold(0u16, |sum, byte| sum.wrapping_add(u16::from(*byte)));
    let xor = packed.iter().fold(0u16, |xor, byte| xor ^ u16::from(*byte));
    if sum != expected_sum || xor != expected_xor {
        return Err(VmError::Runtime("SDC checksum mismatch".into()));
    }
    let mut state = seed;
    for byte in &mut packed {
        *byte = byte.wrapping_sub(next_sdc_random(&mut state));
    }
    let raw = unpack_sdc_lz(&packed)?;
    if raw.len() != raw_len {
        return Err(VmError::Runtime("SDC decoded length mismatch".into()));
    }
    Ok(raw)
}

fn unpack_sdc_lz(packed: &[u8]) -> VmResult<Vec<u8>> {
    let mut input = 0usize;
    let mut output = Vec::new();
    while input < packed.len() {
        let control = packed[input];
        input += 1;
        if control & 0x80 == 0 {
            let count = usize::from(control) + 1;
            let end = input
                .checked_add(count)
                .filter(|end| *end <= packed.len())
                .ok_or_else(|| VmError::Runtime("truncated SDC literal".into()))?;
            output.extend_from_slice(&packed[input..end]);
            input = end;
        } else {
            let low = *packed
                .get(input)
                .ok_or_else(|| VmError::Runtime("truncated SDC back-reference".into()))?;
            input += 1;
            let count = usize::from((control >> 3) & 0x0f) + 2;
            let distance = (usize::from(control & 7) << 8) + usize::from(low) + 2;
            if distance > output.len() {
                return Err(VmError::Runtime("invalid SDC back-reference".into()));
            }
            for _ in 0..count {
                output.push(output[output.len() - distance]);
            }
        }
    }
    Ok(output)
}

fn next_sdc_random(state: &mut u32) -> u8 {
    let low = *state as u16 as u32;
    let high = (*state >> 16) as u16 as u32;
    let low_product = low * 0x4e35;
    let next_high = high
        .wrapping_mul(0x4e35)
        .wrapping_add(state.wrapping_mul(0x015a))
        .wrapping_add(low_product >> 16)
        & 0xffff;
    // Match the native 32-bit add.  A carry from 0xffff + 1 belongs in the
    // high word and must not be discarded before the halves are combined.
    let next_low = (low_product & 0xffff).wrapping_add(1);
    *state = (next_high << 16).wrapping_add(next_low);
    (next_high & 0x7fff) as u8
}

fn read_u32(bytes: &[u8], offset: usize) -> VmResult<u32> {
    bytes
        .get(offset..offset + 4)
        .and_then(|bytes| bytes.try_into().ok())
        .map(u32::from_le_bytes)
        .ok_or_else(|| VmError::Runtime("truncated BURIKO GDB integer".into()))
}

fn read_u32_at_cursor(bytes: &[u8], cursor: &mut usize) -> VmResult<u32> {
    let value = read_u32(bytes, *cursor)?;
    *cursor += 4;
    Ok(value)
}

fn write_u32(bytes: &mut [u8], offset: usize, value: u32) {
    bytes[offset..offset + 4].copy_from_slice(&value.to_le_bytes());
}

fn sjis_len(value: &str) -> usize {
    encoding_rs::SHIFT_JIS.encode(value).0.len()
}

fn write_sjis_c_string(dst: &mut [u8], value: &str) -> usize {
    let (encoded, _, _) = encoding_rs::SHIFT_JIS.encode(value);
    let len = encoded.len();
    dst[..len].copy_from_slice(&encoded);
    dst[len] = 0;
    len + 1
}

fn read_sjis_c_string(bytes: &[u8], cursor: &mut usize) -> VmResult<String> {
    let tail = bytes
        .get(*cursor..)
        .ok_or_else(|| VmError::Runtime("truncated BURIKO GDB string".into()))?;
    let len = tail
        .iter()
        .position(|byte| *byte == 0)
        .ok_or_else(|| VmError::Runtime("unterminated BURIKO GDB string".into()))?;
    let (decoded, _, _) = encoding_rs::SHIFT_JIS.decode(&tail[..len]);
    *cursor += len + 1;
    Ok(decoded.into_owned())
}

#[cfg(test)]
mod tests {
    use super::*;
use std::collections::BTreeMap;

    #[test]
    fn sdc_random_preserves_low_word_carry() {
        let mut state = 0x0000_ebe3;

        assert_eq!(next_sdc_random(&mut state), 0xdd);
        assert_eq!(state, 0x18de_0000);
    }

    #[test]
    fn sdc_literal_encoder_round_trips() {
        let raw = (0..4097).map(|value| value as u8).collect::<Vec<_>>();
        for seed in [0, 1, 999] {
            assert_eq!(decode_sdc(&encode_sdc(&raw, seed)).unwrap(), raw);
        }
    }

    #[test]
    fn empty_struct_array_preserves_destination() {
        let mut vm = Vm::new();
        vm.write_int(0x8000, 2, 0x12345678).unwrap();
        assert_eq!(vm.encode_user_data_structs(0x4000, 0, 300, 0).unwrap(), 32);
        assert_eq!(vm.decode_sdc_struct_array(0x4000, 0x8000).unwrap(), 0);
        assert_eq!(vm.read_int(0x8000, 2).unwrap(), 0x12345678);
    }

    #[test]
    fn dcfs_struct_encoder_round_trips_through_native_decoder() {
        let mut vm = Vm::new();
        let source = 0x2000;
        let encoded = 0x4000;
        let restored = 0x8000;
        let record_size = 300usize;
        let record_count = 3usize;
        let mut records = (0..record_size * record_count)
            .map(|index| (index % 251) as u8)
            .collect::<Vec<_>>();
        let (first, later) = records.split_at_mut(record_size);
        later[..180].copy_from_slice(&first[..180]);
        records[record_size * 2] = records[record_size] ^ 0xff;
        let range = vm.resolve_write_range(source, records.len()).unwrap();
        vm.memory[range].copy_from_slice(&records);

        let encoded_len = vm
            .encode_user_data_structs(encoded, source, record_size as i32, record_count as i32)
            .unwrap();
        assert!(encoded_len > 32);
        assert_eq!(
            vm.decode_sdc_struct_array(encoded, restored).unwrap(),
            record_count as i32
        );
        let restored = vm.resolve_range(restored, records.len()).unwrap();
        assert_eq!(&vm.memory[restored], records);
    }

    #[test]
    fn global_payload_round_trips_memory_names_and_flags() {
        let mut vm = Vm::new();
        vm.global_bytes_mut()[134] = 42;
        vm.global_user_data[123..127].copy_from_slice(&[1, 2, 3, 4]);
        vm.set_resource_names(vec!["BG0001".into(), "face".into()]);
        vm.read_flags
            .insert("main".into(), crate::ReadFlagBits::new(17));
        vm.read_flags.get_mut("main").unwrap().set(16, true);
        let raw = vm.serialize_global_data();

        let mut restored = Vm::new();
        let (_, _, status) = restored.install_global_data(&raw).unwrap();
        assert_eq!(status, 0);
        assert_eq!(restored.memory[134], 42);
        assert_eq!(&restored.global_user_data[123..127], &[1, 2, 3, 4]);
        assert_eq!(
            restored.resource_names().to_vec(),
            vec!["BG0001".to_string(), "face".to_string()]
        );
        assert_eq!(
            restored.read_flags.get("main").unwrap().contains(16),
            Some(true)
        );
    }

    /// Compares `compress_sdc` with sub_493E90 run under an emulator:
    /// ETHORNELL_SDC_VECTORS=<dir> holding <name>.in / <name>.out pairs.
    #[test]
    #[ignore]
    fn sdc_compressor_matches_the_target_vectors() {
        let dir = std::env::var("ETHORNELL_SDC_VECTORS").expect("ETHORNELL_SDC_VECTORS");
        let mut checked = 0;
        for entry in std::fs::read_dir(&dir).unwrap() {
            let path = entry.unwrap().path();
            if path.extension().is_some_and(|ext| ext == "in") {
                let input = std::fs::read(&path).unwrap();
                let expected = std::fs::read(path.with_extension("out")).unwrap();
                assert_eq!(compress_sdc(&input), expected, "{}", path.display());
                checked += 1;
            }
        }
        assert!(checked > 0);
    }

    #[test]
    fn legacy_global_payload_installs_and_checks_its_size() {
        // Older variant: a valid first flag record at 394248.
        let data_base = 459784;
        let mut raw = vec![0u8; data_base + 1];
        raw[0..4].copy_from_slice(&10u32.to_le_bytes());
        raw[4..8].copy_from_slice(&20u32.to_le_bytes());
        raw[8 + 5] = 7;
        raw[1032 + 9] = 8;
        raw[263176..263176 + 6].copy_from_slice(b"BG0001");
        raw[263176 + 32..263176 + 38].copy_from_slice(b"BG0001");
        raw[394248..394252].copy_from_slice(b"main");
        raw[394248 + 24..394248 + 28].copy_from_slice(&3u32.to_le_bytes());
        raw[data_base] = 0b101;
        let mut vm = Vm::new();
        vm.global_user_data[0x50000] = 9;
        assert_eq!(vm.install_legacy_global_data(&raw), Some((10, 20, 0)));
        assert_eq!(vm.memory[5], 7);
        assert_eq!(vm.global_user_data[9], 8);
        assert_eq!(vm.global_user_data[0x50000], 0);
        assert_eq!(vm.resource_names(), ["BG0001".to_string()]);
        assert_eq!(vm.read_flags.get("main").unwrap().contains(2), Some(true));

        raw.push(0);
        assert_eq!(vm.install_legacy_global_data(&raw).map(|r| r.2), Some(2));
    }

    #[derive(Default)]
    struct SlotApi {
        files: std::collections::BTreeMap<String, Vec<u8>>,
    }

    impl SysApi for SlotApi {
        fn call_sys(&mut self, _call: &mut crate::NativeCallFrame) -> VmResult<crate::Value> {
            unreachable!("slot tests invoke only the file hooks")
        }

        fn write_file_bytes(&mut self, path: &str, bytes: &[u8]) -> bool {
            self.files.insert(path.to_owned(), bytes.to_vec());
            true
        }

        fn read_user_file_bytes(&mut self, path: &str) -> Option<Vec<u8>> {
            self.files.get(path).cloned()
        }
    }

    #[test]
    fn config_slot_prng_matches_target_first_step() {
        let mut state = 1u32;
        assert_eq!(slot_random_next(&mut state), 346);
        assert_eq!(state, 0x015A_4E36);
    }

    #[test]
    fn config_slot_save_validate_header_and_load_match_native_contract() {
        let mut vm = Vm::new();
        assert!(vm.allocate_global_config(0));
        for (index, byte) in vm.global_bytes_mut().iter_mut().enumerate() {
            *byte = (index % 251) as u8;
        }
        let original = vm.global_bytes().to_vec();
        vm.save_data_integrity_enabled = 1;
        let mut api = SlotApi::default();

        assert!(vm.save_config_slot(&mut api, 7, "slot-seven").unwrap());
        let encoded = api.files.get("BGI0007.cad").unwrap();
        assert_eq!(encoded.len(), 64 + original.len());
        assert_eq!(&encoded[16..26], b"slot-seven");
        assert_eq!(encoded[26], 0);
        assert_eq!(encoded[60], 1);
        assert_ne!(&encoded[64..], original.as_slice());
        assert_eq!(vm.validate_config_slot(&mut api, 7), 0);
        assert_eq!(
            &vm.read_config_slot_header(&mut api, 7).unwrap()[16..26],
            b"slot-seven"
        );

        vm.global_bytes_mut().fill(0xEE);
        assert_eq!(vm.load_config_slot(&mut api, 7), 0);
        assert!(vm.global_bytes()[..1024].iter().all(|byte| *byte == 0xEE));
        assert_eq!(&vm.global_bytes()[1024..], &original[1024..]);
    }

    #[test]
    fn config_slot_status_codes_distinguish_missing_size_and_integrity() {
        let mut vm = Vm::new();
        assert!(vm.allocate_global_config(0));
        vm.save_data_integrity_enabled = 1;
        let mut api = SlotApi::default();

        assert_eq!(vm.validate_config_slot(&mut api, 1), 1);
        api.files.insert("BGI0001.cad".into(), vec![0; 65]);
        assert_eq!(vm.validate_config_slot(&mut api, 1), 2);

        vm.save_config_slot(&mut api, 1, "valid").unwrap();
        api.files.get_mut("BGI0001.cad").unwrap()[64] ^= 0x80;
        assert_eq!(vm.validate_config_slot(&mut api, 1), 3);
        assert!(vm.save_config_slot(&mut api, 2, &"x".repeat(40)).is_err());
    }

    #[test]
    fn config_slot_selectors_use_the_target_vm_dispatch_path() {
        let mut vm = Vm::new();
        assert!(vm.allocate_global_config(0));
        vm.global_bytes_mut().fill(0x5a);
        let mut api = SlotApi::default();

        vm.stack
            .extend([crate::Value::Int(12), crate::Value::Str("dispatch".into())]);
        assert_eq!(
            vm.try_builtin_sys_with_api(&mut api, 0x80, 0x78).unwrap(),
            Some(crate::Value::None)
        );
        assert!(api.files.contains_key("BGI0012.cad"));

        let header = 0x3200;
        vm.stack
            .extend([crate::Value::Ptr(header), crate::Value::Int(12)]);
        assert_eq!(
            vm.try_builtin_sys_with_api(&mut api, 0x80, 0x7a).unwrap(),
            Some(crate::Value::Int(0))
        );
        assert_eq!(vm.read_c_string(header + 16).unwrap(), "dispatch");

        vm.stack.push(crate::Value::Int(12));
        assert_eq!(
            vm.try_builtin_sys_with_api(&mut api, 0x80, 0x7b).unwrap(),
            Some(crate::Value::Int(0))
        );
        vm.global_bytes_mut()[1024..].fill(0);
        vm.stack.push(crate::Value::Int(12));
        assert_eq!(
            vm.try_builtin_sys_with_api(&mut api, 0x80, 0x79).unwrap(),
            Some(crate::Value::Int(0))
        );
        assert!(vm.global_bytes()[1024..].iter().all(|byte| *byte == 0x5a));
        assert!(vm.stack.is_empty());
    }
}

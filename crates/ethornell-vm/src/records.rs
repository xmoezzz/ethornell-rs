use crate::{Value, Vm, VmResult};

#[derive(Debug, Clone)]
pub(crate) struct RecordTableState {
    pub(crate) slot: u32,
    pub(crate) record_size: u32,
    pub(crate) cursor: u32,
    pub(crate) keys: std::collections::BTreeMap<u32, String>,
    pub(crate) records: std::collections::BTreeMap<u32, u32>,
}

#[derive(Debug, Clone)]
pub(crate) struct IndexedRecordEntry {
    pub(crate) bytes: Vec<u8>,
    pub(crate) shadow_values: std::collections::BTreeMap<u32, Value>,
}

#[derive(Debug, Clone)]
pub(crate) struct IndexedRecordState {
    pub(crate) slot: u32,
    pub(crate) capacity: u32,
    pub(crate) record_size: u32,
    pub(crate) records: std::collections::VecDeque<IndexedRecordEntry>,
}

impl Vm {
    pub(crate) fn clear_shadow_values(&mut self, ptr: u32, size: usize) {
        if size == 0 {
            return;
        }
        self.trace_watch_write(ptr, size, 0, "clear_shadow_values");
        let start = Self::value_key(ptr);
        let end = start.saturating_add(size as u32);
        self.mem_values
            .extract_if(start..end, |_, _| true)
            .for_each(drop);
    }

    pub(crate) fn copy_shadow_values(&mut self, src: u32, dst: u32, size: usize) {
        if size == 0 {
            return;
        }
        self.clear_shadow_values(dst, size);
        let src_start = Self::value_key(src);
        let dst_start = Self::value_key(dst);
        let end = src_start.saturating_add(size as u32);
        let copied: Vec<_> = self
            .mem_values
            .range(src_start..end)
            .map(|(addr, value)| (dst_start.saturating_add(*addr - src_start), value.clone()))
            .collect();
        self.mem_values.extend(copied);
    }

    pub(crate) fn normalize_scenario_descriptor_src(&self, src: u32, size: usize) -> u32 {
        if size != 100 || (src & 7) != 2 {
            return src;
        }
        let candidate = src.saturating_add(6);
        let Ok(current) = self.read_int(src, 2) else {
            return src;
        };
        let Ok(first) = self.read_int(candidate, 2) else {
            return src;
        };
        let Ok(second) = self.read_int(candidate.saturating_add(4), 2) else {
            return src;
        };
        if current > 0x00ff_ffff && first <= 0x1000 && second <= 0x10000 {
            tracing::debug!(
                src = format_args!("0x{src:08X}"),
                adjusted = format_args!("0x{candidate:08X}"),
                first = format_args!("0x{first:08X}"),
                second = format_args!("0x{second:08X}"),
                "ScenarioDescriptorCopyAlign"
            );
            candidate
        } else {
            src
        }
    }

    pub(crate) fn remember_script_record(&mut self, ptr: u32, text: &str) -> VmResult<()> {
        self.write_c_string_raw(ptr, text)?;
        self.script_records
            .insert(Self::value_key(ptr), text.to_string());
        self.mem_values
            .insert(Self::value_key(ptr), Value::Str(text.to_string()));
        Ok(())
    }

    pub(crate) fn restore_script_records(&mut self, ptr: u32, size: usize) -> VmResult<()> {
        if size == 0 || self.script_records.is_empty() {
            return Ok(());
        }
        let start = Self::value_key(ptr);
        let end = start.saturating_add(size as u32);
        let records: Vec<_> = self
            .script_records
            .iter()
            .filter_map(|(addr, text)| {
                (*addr >= start && *addr < end).then(|| (*addr, text.clone()))
            })
            .collect();
        for (addr, text) in records {
            self.write_c_string_raw(addr, &text)?;
            self.mem_values.insert(addr, Value::Str(text));
        }
        Ok(())
    }

    pub(crate) fn decode_sdc_records(&mut self, src: u32, dst: u32) -> VmResult<i32> {
        const HEADER_SIZE: usize = 32;
        const MAGIC: &[u8; 16] = b"SDC FORMAT 1.00\0";

        let header = self.resolve_range(src, HEADER_SIZE)?;
        let header = self.memory[header].to_vec();
        if header.get(..MAGIC.len()) != Some(MAGIC) {
            tracing::warn!(
                src = format_args!("0x{src:08X}"),
                header = %header.iter().take(16).map(|byte| format!("{byte:02X}")).collect::<String>(),
                "SDC header magic mismatch"
            );
            return Ok(0);
        }

        let seed = u32::from_le_bytes(header[16..20].try_into().unwrap());
        let packed_len = u32::from_le_bytes(header[20..24].try_into().unwrap()) as usize;
        let expected_sum = u16::from_le_bytes(header[28..30].try_into().unwrap());
        let expected_xor = u16::from_le_bytes(header[30..32].try_into().unwrap());
        let packed = match self.resolve_range(src.saturating_add(HEADER_SIZE as u32), packed_len) {
            Ok(range) => range,
            Err(error) => {
                tracing::warn!(
                    src = format_args!("0x{src:08X}"),
                    packed_len,
                    raw_len = u32::from_le_bytes(header[24..28].try_into().unwrap()),
                    %error,
                    "SDC packed payload is outside VM memory"
                );
                return Ok(0);
            }
        };
        let mut packed = self.memory[packed].to_vec();

        let sum = packed
            .iter()
            .fold(0u16, |sum, byte| sum.wrapping_add(u16::from(*byte)));
        let xor = packed.iter().fold(0u16, |xor, byte| xor ^ u16::from(*byte));
        if sum != expected_sum || xor != expected_xor {
            tracing::warn!(
                sum = format_args!("0x{sum:04X}"),
                expected_sum = format_args!("0x{expected_sum:04X}"),
                xor = format_args!("0x{xor:04X}"),
                expected_xor = format_args!("0x{expected_xor:04X}"),
                "SDC checksum mismatch"
            );
            return Ok(0);
        }

        let mut state = seed;
        for byte in &mut packed {
            let low = state as u16 as u32;
            let high = (state >> 16) as u16 as u32;
            let low_product = low * 0x4e35;
            let next_high = high
                .wrapping_mul(0x4e35)
                .wrapping_add(state.wrapping_mul(0x015a))
                .wrapping_add(low_product >> 16)
                & 0xffff;
            // The target adds one to the zero-extended low product before it
            // combines both halves.  Preserve the carry into the high word;
            // truncating first desynchronizes some otherwise valid SDC files.
            let next_low = (low_product & 0xffff).wrapping_add(1);
            state = (next_high << 16).wrapping_add(next_low);
            let random = (next_high & 0x7fff) as u8;
            *byte = byte.wrapping_sub(random);
        }

        let unpacked = match unpack_sdc_lz(&packed) {
            Ok(unpacked) => unpacked,
            Err(error) => {
                tracing::warn!(
                    src = format_args!("0x{src:08X}"),
                    dst = format_args!("0x{dst:08X}"),
                    seed = format_args!("0x{seed:08X}"),
                    packed_len,
                    raw_len = u32::from_le_bytes(header[24..28].try_into().unwrap()),
                    %error,
                    "SDC LZ decompression failed"
                );
                return Ok(0);
            }
        };
        let range = self.resolve_write_range(dst, unpacked.len())?;
        self.memory[range].copy_from_slice(&unpacked);
        self.clear_shadow_values(dst, unpacked.len());
        Ok(unpacked.len() as i32)
    }

    pub(crate) fn decode_sdc_struct_array(&mut self, src: u32, dst: u32) -> VmResult<i32> {
        const STRUCT_MAGIC: &[u8; 16] = b"DCFS FORMAT 1.00";

        let decoded_len = self.read_int(src.saturating_add(24), 2)? as usize;
        if decoded_len < 24 {
            tracing::warn!(
                src = format_args!("0x{src:08X}"),
                decoded_len,
                "SDC/DCFS decoded length is too small"
            );
            return Ok(0);
        }
        let temporary = self.alloc_heap(decoded_len as u32);
        if temporary == 0 {
            tracing::warn!(decoded_len, "SDC/DCFS temporary allocation failed");
            return Ok(0);
        }
        let written = self.decode_sdc_records(src, temporary)?;
        if written <= 0 || written as usize > decoded_len {
            tracing::warn!(
                src = format_args!("0x{src:08X}"),
                allocation = decoded_len,
                written,
                "SDC/DCFS outer decompression failed or exceeded its target allocation"
            );
            let _ = self.free_heap(temporary);
            return Ok(0);
        }
        if written as usize != decoded_len {
            // Target Sys80:C5 allocates header.raw_len bytes, invokes
            // sub_4938F0, and deliberately ignores its returned byte count
            // before running the DCFS parser. Preserve that behavior: a
            // shorter decoded stream may still contain a complete DCFS table
            // followed by unused allocation padding.
            tracing::info!(
                src = format_args!("0x{src:08X}"),
                allocation = decoded_len,
                written,
                "SDC/DCFS decoded byte count differs from allocation; continuing like target"
            );
        }
        let range = self.resolve_range(temporary, decoded_len)?;
        let encoded = self.memory[range].to_vec();
        if encoded.get(..STRUCT_MAGIC.len()) != Some(STRUCT_MAGIC) {
            tracing::warn!(
                src = format_args!("0x{src:08X}"),
                decoded_header = %encoded.iter().take(16).map(|byte| format!("{byte:02X}")).collect::<String>(),
                "DCFS structure magic mismatch"
            );
            let _ = self.free_heap(temporary);
            return Ok(0);
        }

        let record_size = u32::from_le_bytes(encoded[16..20].try_into().unwrap()) as usize;
        let record_count = u32::from_le_bytes(encoded[20..24].try_into().unwrap()) as usize;
        if record_size == 0 || record_count == 0 {
            tracing::warn!(
                record_size,
                record_count,
                "DCFS structure dimensions are invalid"
            );
            let _ = self.free_heap(temporary);
            return Ok(0);
        }
        let total_output = match record_size.checked_mul(record_count) {
            Some(total) => total,
            None => {
                tracing::warn!(record_size, record_count, "DCFS output size overflow");
                let _ = self.free_heap(temporary);
                return Ok(0);
            }
        };
        let first_end = match 24usize.checked_add(record_size) {
            Some(end) => end,
            None => {
                let _ = self.free_heap(temporary);
                return Ok(0);
            }
        };
        let Some(first) = encoded.get(24..first_end) else {
            tracing::warn!(
                decoded_len,
                record_size,
                record_count,
                "DCFS first record is truncated"
            );
            let _ = self.free_heap(temporary);
            return Ok(0);
        };
        let mut records = Vec::with_capacity(total_output);
        records.extend_from_slice(first);
        let mut input = first_end;

        for record_index in 1..record_count {
            let previous = records[records.len() - record_size..].to_vec();
            let mut record = Vec::with_capacity(record_size);
            let mut literal = false;
            while record.len() < record_size {
                let count = match read_dcfs_varint(&encoded, &mut input) {
                    Ok(count) => count,
                    Err(error) => {
                        tracing::warn!(
                            record_index,
                            input,
                            record_size,
                            record_count,
                            %error,
                            "DCFS run-length stream is truncated"
                        );
                        let _ = self.free_heap(temporary);
                        return Ok(0);
                    }
                };
                if record.len().saturating_add(count) > record_size {
                    tracing::warn!(
                        record_index,
                        run_offset = record.len(),
                        count,
                        record_size,
                        "DCFS run exceeds record boundary"
                    );
                    let _ = self.free_heap(temporary);
                    return Ok(0);
                }
                if literal {
                    let end = input.saturating_add(count);
                    let Some(bytes) = encoded.get(input..end) else {
                        tracing::warn!(
                            record_index,
                            input,
                            count,
                            decoded_len,
                            "DCFS literal run is truncated"
                        );
                        let _ = self.free_heap(temporary);
                        return Ok(0);
                    };
                    record.extend_from_slice(bytes);
                    input = end;
                } else {
                    let start = record.len();
                    record.extend_from_slice(&previous[start..start + count]);
                }
                literal = !literal;
            }
            records.extend_from_slice(&record);
        }

        let range = self.resolve_write_range(dst, records.len())?;
        self.memory[range].copy_from_slice(&records);
        self.clear_shadow_values(dst, records.len());
        let _ = self.free_heap(temporary);
        tracing::info!(
            src = format_args!("0x{src:08X}"),
            dst = format_args!("0x{dst:08X}"),
            decoded_len,
            record_size,
            record_count,
            output_bytes = records.len(),
            "SDC/DCFS structure decompressed"
        );
        Ok(record_count as i32)
    }

    pub(crate) fn sys_record_table_open(&mut self, slot: u32, record_size: u32) -> VmResult<i32> {
        const INVALID_RECORD_SIZE: i32 = 0x8000_0001u32 as i32;
        if record_size <= 1 {
            return Ok(INVALID_RECORD_SIZE);
        }

        let handle = self.next_record_table_handle.max(1);
        self.next_record_table_handle = handle.wrapping_add(1).max(1);
        self.write_int(slot, 2, handle)?;
        self.record_tables.insert(
            Self::value_key(handle),
            RecordTableState {
                slot,
                record_size,
                cursor: 0,
                keys: std::collections::BTreeMap::new(),
                records: std::collections::BTreeMap::new(),
            },
        );
        tracing::debug!(
            slot = format_args!("0x{slot:08X}"),
            record_size,
            handle,
            "RecordTableOpen"
        );
        Ok(0)
    }

    pub(crate) fn sys_record_table_copy(
        &mut self,
        handle: u32,
        key: Value,
        src: u32,
    ) -> VmResult<i32> {
        const TABLE_NOT_FOUND: i32 = 0x8000_0002u32 as i32;
        let key = match self.normalize_record_key(key) {
            Value::Str(text) => text,
            Value::Ptr(0) | Value::Int(0) | Value::None => String::new(),
            value => value.as_i32().to_string(),
        };
        let handle_key = Self::value_key(handle);
        let Some((index, record_size, record_ptr)) =
            self.record_tables.get(&handle_key).map(|table| {
                let existing = table
                    .keys
                    .iter()
                    .find_map(|(index, saved)| (saved == &key).then_some(*index));
                let index = existing.unwrap_or(table.cursor);
                (index, table.record_size, table.records.get(&index).copied())
            })
        else {
            return Ok(TABLE_NOT_FOUND);
        };

        let record_ptr = if let Some(record_ptr) = record_ptr {
            record_ptr
        } else {
            let record_ptr = self.alloc_heap(record_size);
            let table = self
                .record_tables
                .get_mut(&handle_key)
                .expect("record table disappeared while inserting");
            table.cursor = table.cursor.saturating_add(1);
            table.keys.insert(index, key.clone());
            table.records.insert(index, record_ptr);
            record_ptr
        };
        self.copy_record_into_table(record_ptr, src, record_size as usize)?;
        tracing::debug!(
            handle,
            index,
            key,
            record_size,
            src = format_args!("0x{src:08X}"),
            record_ptr = format_args!("0x{record_ptr:08X}"),
            "RecordTableInsert"
        );
        Ok(0)
    }

    pub(crate) fn sys_record_table_close(&mut self, handle: u32) -> i32 {
        const TABLE_NOT_FOUND: i32 = 0x8000_0002u32 as i32;
        let Some(table) = self.record_tables.remove(&Self::value_key(handle)) else {
            return TABLE_NOT_FOUND;
        };
        for record_ptr in table.records.into_values() {
            let _ = self.free_heap(record_ptr);
        }
        0
    }

    pub(crate) fn sys_record_table_remove(&mut self, handle: u32, key: Value) -> i32 {
        const TABLE_NOT_FOUND: i32 = 0x8000_0002u32 as i32;
        const KEY_NOT_FOUND: i32 = 0x8000_0003u32 as i32;

        let handle_key = Self::value_key(handle);
        let key = match self.normalize_record_key(key) {
            Value::Str(text) => text,
            Value::Ptr(0) | Value::Int(0) | Value::None => String::new(),
            value => value.as_i32().to_string(),
        };
        let Some(table) = self.record_tables.get_mut(&handle_key) else {
            return TABLE_NOT_FOUND;
        };
        let Some(index) = table
            .keys
            .iter()
            .find_map(|(index, saved)| (saved == &key).then_some(*index))
        else {
            return KEY_NOT_FOUND;
        };
        table.keys.remove(&index);
        let record_ptr = table.records.remove(&index);
        if let Some(record_ptr) = record_ptr {
            let _ = self.free_heap(record_ptr);
        }
        0
    }

    pub(crate) fn sys_record_table_fetch(
        &mut self,
        dst: u32,
        handle: u32,
        selector: Value,
        index: i32,
    ) -> VmResult<Value> {
        const TABLE_NOT_FOUND: i32 = 0x8000_0002u32 as i32;
        const RECORD_NOT_FOUND: i32 = 0x8000_0003u32 as i32;
        let handle_key = Self::value_key(handle);
        let Some(table) = self.record_tables.get(&handle_key) else {
            return Ok(Value::Int(TABLE_NOT_FOUND));
        };
        let selected = match self.normalize_record_key(selector) {
            Value::Ptr(0) | Value::Int(0) | Value::None => usize::try_from(index)
                .ok()
                .and_then(|index| table.keys.keys().copied().nth(index)),
            Value::Str(key) => table
                .keys
                .iter()
                .find_map(|(index, saved)| (saved == &key).then_some(*index)),
            value => {
                let key = value.as_i32().to_string();
                table
                    .keys
                    .iter()
                    .find_map(|(index, saved)| (saved == &key).then_some(*index))
            }
        };
        let Some(selected) = selected else {
            return Ok(Value::Int(RECORD_NOT_FOUND));
        };
        let Some(src) = table.records.get(&selected).copied() else {
            return Ok(Value::Int(RECORD_NOT_FOUND));
        };
        let record_size = table.record_size;
        self.copy_buffer(dst, src, record_size as usize)?;
        tracing::debug!(
            handle,
            selected,
            record_size,
            src = format_args!("0x{src:08X}"),
            dst = format_args!("0x{dst:08X}"),
            "RecordTableFetch"
        );
        Ok(Value::Int(0))
    }

    pub(crate) fn sys_indexed_record_open(
        &mut self,
        slot: u32,
        capacity: u32,
        record_size: u32,
    ) -> VmResult<i32> {
        if capacity == 0 || record_size == 0 {
            return Ok(2);
        }
        let handle = self.next_indexed_record_handle.max(1);
        self.next_indexed_record_handle = handle.wrapping_add(1).max(1);
        self.write_value(slot, 2, &Value::Int(handle as i32))?;
        self.indexed_record_tables.insert(
            Self::value_key(handle),
            IndexedRecordState {
                slot,
                capacity,
                record_size,
                records: std::collections::VecDeque::new(),
            },
        );

        self.ensure_indexed_record_globals(record_size)?;

        if std::env::var_os("DEBUG").is_some() {
            tracing::info!(
                thread_id = self.thread.thread_id(),
                slot = format_args!("0x{slot:08X}"),
                capacity,
                record_size,
                handle = format_args!("0x{handle:08X}"),
                table_count = self.indexed_record_tables.len(),
                "IndexedRecordOpen"
            );
        }
        Ok(0)
    }

    pub(crate) fn sys_indexed_record_close(&mut self, handle: u32) -> i32 {
        if let Some(state) = self.indexed_record_tables.remove(&Self::value_key(handle)) {
            if std::env::var_os("DEBUG").is_some() {
                tracing::info!(
                    thread_id = self.thread.thread_id(),
                    handle = format_args!("0x{handle:08X}"),
                    slot = format_args!("0x{:08X}", state.slot),
                    capacity = state.capacity,
                    record_size = state.record_size,
                    record_count = state.records.len(),
                    "IndexedRecordClose"
                );
            }
            0
        } else {
            1
        }
    }

    pub(crate) fn sys_indexed_record_count(&mut self, dst: u32, handle: u32) -> VmResult<i32> {
        let Some(state) = self.indexed_record_tables.get(&Self::value_key(handle)) else {
            if std::env::var_os("DEBUG").is_some() {
                tracing::info!(
                    thread_id = self.thread.thread_id(),
                    dst = format_args!("0x{dst:08X}"),
                    handle = format_args!("0x{handle:08X}"),
                    table_count = self.indexed_record_tables.len(),
                    "IndexedRecordCount missing handle"
                );
            }
            return Ok(1);
        };
        let count = state.records.len() as u32;
        let slot = state.slot;
        self.write_int(dst, 2, count)?;
        if std::env::var_os("DEBUG").is_some() {
            tracing::info!(
                thread_id = self.thread.thread_id(),
                dst = format_args!("0x{dst:08X}"),
                handle = format_args!("0x{handle:08X}"),
                slot = format_args!("0x{slot:08X}"),
                count,
                "IndexedRecordCount"
            );
        }
        Ok(0)
    }

    pub(crate) fn sys_indexed_record_push(&mut self, handle: u32, src: u32) -> VmResult<i32> {
        let key = Self::value_key(handle);
        let Some(state) = self.indexed_record_tables.get(&key) else {
            if std::env::var_os("DEBUG").is_some() {
                tracing::info!(
                    thread_id = self.thread.thread_id(),
                    handle = format_args!("0x{handle:08X}"),
                    src = format_args!("0x{src:08X}"),
                    table_count = self.indexed_record_tables.len(),
                    "IndexedRecordPush missing handle"
                );
            }
            return Ok(1);
        };
        let size = state.record_size as usize;
        let capacity = state.capacity as usize;
        let slot = state.slot;
        let src_range = self.resolve_range(src, size)?;
        let src_start = Self::value_key(src);
        let src_end = src_start.saturating_add(state.record_size);
        let shadow_values = self
            .mem_values
            .range(src_start..src_end)
            .map(|(addr, value)| (*addr - src_start, value.clone()))
            .collect();
        let entry = IndexedRecordEntry {
            bytes: self.memory[src_range].to_vec(),
            shadow_values,
        };
        let state = self.indexed_record_tables.get_mut(&key).unwrap();
        state.records.push_front(entry);
        state.records.truncate(capacity);
        let count = state.records.len();
        if std::env::var_os("DEBUG").is_some() {
            tracing::info!(
                thread_id = self.thread.thread_id(),
                handle = format_args!("0x{handle:08X}"),
                src = format_args!("0x{src:08X}"),
                slot = format_args!("0x{slot:08X}"),
                count,
                "IndexedRecordPush"
            );
        }
        Ok(0)
    }

    pub(crate) fn sys_indexed_record_load(
        &mut self,
        dst: u32,
        handle: u32,
        index: u32,
    ) -> VmResult<i32> {
        let Some(state) = self.indexed_record_tables.get(&Self::value_key(handle)) else {
            return Ok(1);
        };
        let Some(entry) = state.records.get(index as usize).cloned() else {
            return Ok(2);
        };
        let size = state.record_size as usize;
        let record_size = state.record_size;
        let capacity = state.capacity;
        self.ensure_writable(dst, size);
        if Self::should_clear_indexed_record_target(dst) {
            self.clear_record_buffer(dst, size);
        }
        let start = Self::memory_addr(dst) as usize;
        self.memory[start..start + size].copy_from_slice(&entry.bytes);
        self.clear_shadow_values(dst, size);
        let dst_start = Self::value_key(dst);
        self.mem_values.extend(
            entry
                .shadow_values
                .into_iter()
                .map(|(offset, value)| (dst_start.saturating_add(offset), value)),
        );

        if std::env::var_os("DEBUG").is_some() {
            let msg_base = dst.saturating_add(286_276);
            let msg_valid = self
                .read_int(msg_base.saturating_add(12), 2)
                .unwrap_or_default();
            let msg_count = self
                .read_int(msg_base.saturating_add(16), 2)
                .unwrap_or_default();
            tracing::info!(
                dst = format_args!("0x{dst:08X}"),
                handle = format_args!("0x{handle:08X}"),
                index,
                record_size,
                capacity,
                msg_base = format_args!("0x{msg_base:08X}"),
                msg_valid,
                msg_count,
                "IndexedRecordLoad"
            );
        }
        Ok(0)
    }

    pub(crate) fn sys_indexed_record_remove(&mut self, handle: u32, start: u32, count: u32) -> i32 {
        let Some(state) = self.indexed_record_tables.get_mut(&Self::value_key(handle)) else {
            return 1;
        };
        let start = start as usize;
        if start >= state.records.len() {
            return 2;
        }
        let end = start
            .saturating_add(count as usize)
            .min(state.records.len());
        state.records.drain(start..end);
        0
    }

    fn ensure_indexed_record_globals(&mut self, record_size: u32) -> VmResult<()> {
        const PRIMARY_BASE: u32 = 0x0004_cad8;
        let Some(total) = record_size.checked_mul(2) else {
            return Ok(());
        };
        self.ensure_writable(PRIMARY_BASE, total as usize);
        Ok(())
    }

    fn ensure_writable(&mut self, ptr: u32, size: usize) {
        let start = Self::memory_addr(ptr) as usize;
        let end = start.saturating_add(size);
        if end > self.memory.len() {
            self.memory.resize(end, 0);
        }
    }

    fn clear_record_buffer(&mut self, ptr: u32, size: usize) {
        if size == 0 {
            return;
        }
        self.ensure_writable(ptr, size);
        let start = Self::memory_addr(ptr) as usize;
        let end = start.saturating_add(size);
        self.memory[start..end].fill(0);
        self.clear_shadow_values(ptr, size);
        self.trace_watch_write(ptr, size, 0, "clear_record_buffer");
    }

    fn should_clear_indexed_record_target(ptr: u32) -> bool {
        const PRIMARY_GLOBAL_RECORD: u32 = 0x0004_cad8;
        Self::memory_addr(ptr) != PRIMARY_GLOBAL_RECORD
    }

    fn copy_record_into_table(&mut self, dst: u32, src: u32, size: usize) -> VmResult<()> {
        let end = Self::memory_addr(dst) as usize + size;
        if end > self.memory.len() {
            self.memory.resize(end, 0);
        }
        self.copy_buffer(dst, src, size)
    }

    fn normalize_record_key(&self, key: Value) -> Value {
        match key {
            Value::Ptr(ptr) => self
                .read_c_string(ptr)
                .ok()
                .filter(|text| !text.is_empty())
                .map(Value::Str)
                .unwrap_or(Value::Ptr(ptr)),
            Value::Int(ptr) if ptr != 0 => self
                .read_c_string(ptr as u32)
                .ok()
                .filter(|text| !text.is_empty())
                .map(Value::Str)
                .unwrap_or(Value::Int(ptr)),
            other => other,
        }
    }
}

fn unpack_sdc_lz(packed: &[u8]) -> VmResult<Vec<u8>> {
    let mut input = 0usize;
    let mut output = Vec::new();
    while input < packed.len() {
        let control = packed[input];
        input += 1;
        if control & 0x80 == 0 {
            let count = control as usize + 1;
            let end = input.saturating_add(count);
            let bytes = packed
                .get(input..end)
                .ok_or_else(|| crate::VmError::Runtime("truncated SDC literal".into()))?;
            output.extend_from_slice(bytes);
            input = end;
        } else {
            let distance_low = *packed
                .get(input)
                .ok_or_else(|| crate::VmError::Runtime("truncated SDC back-reference".into()))?;
            input += 1;
            let count = ((control >> 3) & 0x0f) as usize + 2;
            let distance = (((control & 7) as usize) << 8) + distance_low as usize + 2;
            if distance > output.len() {
                return Err(crate::VmError::Runtime("invalid SDC back-reference".into()));
            }
            for _ in 0..count {
                let byte = output[output.len() - distance];
                output.push(byte);
            }
        }
    }
    Ok(output)
}

fn read_dcfs_varint(encoded: &[u8], input: &mut usize) -> VmResult<usize> {
    let mut value = 0usize;
    let mut shift = 0u32;
    loop {
        let byte = *encoded
            .get(*input)
            .ok_or_else(|| crate::VmError::Runtime("truncated DCFS run length".into()))?;
        *input += 1;
        value |= usize::from(byte & 0x7f) << shift;
        if byte & 0x80 == 0 {
            return Ok(value);
        }
        shift += 7;
        if shift >= usize::BITS {
            return Err(crate::VmError::Runtime("DCFS run length overflow".into()));
        }
    }
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn shadow_range_copy_and_clear_preserve_adjacent_values() {
        let mut vm = Vm::new();
        for addr in [99, 100, 103, 104, 199, 200, 203, 204, 100_000] {
            vm.mem_values.insert(addr, Value::Str(addr.to_string()));
        }
        vm.copy_shadow_values(100, 200, 4);
        assert_eq!(vm.mem_values[&200], Value::Str("100".into()));
        assert_eq!(vm.mem_values[&203], Value::Str("103".into()));
        vm.clear_shadow_values(100, 4);
        assert!(!vm.mem_values.contains_key(&100));
        assert!(!vm.mem_values.contains_key(&103));
        vm.clear_shadow_values(200, 0);
        assert!(vm.mem_values.contains_key(&200));
        for addr in [99, 104, 199, 204, 100_000] {
            assert_eq!(vm.mem_values[&addr], Value::Str(addr.to_string()));
        }
    }

    #[test]
    fn keyed_record_fetch_uses_zero_for_success_and_replaces_duplicate_keys() {
        let mut vm = Vm::new();
        let slot = vm.alloc_heap(4);
        assert_eq!(
            vm.sys_record_table_open(slot, 8)
                .expect("open record table"),
            0
        );
        let handle = vm.read_int(slot, 2).expect("read record table handle");
        let src = vm.alloc_heap(8);
        let dst = vm.alloc_heap(8);

        vm.write_int(src, 2, 3).expect("write script index");
        vm.write_int(src + 4, 2, 0x1234)
            .expect("write function address");
        assert_eq!(
            vm.sys_record_table_copy(handle, Value::Str("_Wait".into()), src)
                .expect("insert record"),
            0
        );

        vm.write_int(src, 2, 4).expect("replace script index");
        vm.write_int(src + 4, 2, 0x5678)
            .expect("replace function address");
        assert_eq!(
            vm.sys_record_table_copy(handle, Value::Str("_Wait".into()), src)
                .expect("replace record"),
            0
        );

        let found = vm
            .sys_record_table_fetch(dst, handle, Value::Str("_Wait".into()), 0)
            .expect("fetch record");
        assert!(matches!(found, Value::Int(0)));
        assert_eq!(vm.read_int(dst, 2).expect("read script index"), 4);
        assert_eq!(
            vm.read_int(dst + 4, 2).expect("read function address"),
            0x5678
        );

        let missing = vm
            .sys_record_table_fetch(dst, handle, Value::Str("_Missing".into()), 0)
            .expect("fetch missing record");
        assert!(matches!(missing, Value::Int(value) if value != 0));
    }

    #[test]
    fn keyed_record_remove_preserves_native_raw_status_codes() {
        let mut vm = Vm::new();
        let slot = vm.alloc_heap(4);
        assert_eq!(
            vm.sys_record_table_open(slot, 8)
                .expect("open record table"),
            0
        );
        let handle = vm.read_int(slot, 2).expect("read record table handle");
        let src = vm.alloc_heap(8);
        assert_eq!(
            vm.sys_record_table_copy(handle, Value::Str("_Wait".into()), src)
                .expect("insert record"),
            0
        );

        assert_eq!(
            vm.sys_record_table_remove(handle, Value::Str("_Wait".into())),
            0
        );
        assert_eq!(
            vm.sys_record_table_remove(handle, Value::Str("_Wait".into())),
            0x8000_0003u32 as i32
        );
        assert_eq!(
            vm.sys_record_table_remove(0xdead_beef, Value::Str("_Wait".into())),
            0x8000_0002u32 as i32
        );
    }

    #[test]
    fn record_tables_use_unbounded_per_key_nodes_and_linked_list_indexing() {
        let mut vm = Vm::new();
        let slot = vm.alloc_heap(4);
        assert_eq!(vm.sys_record_table_open(slot, 8).unwrap(), 0);
        let handle = vm.read_int(slot, 2).unwrap();
        let src = vm.alloc_heap(8);
        let dst = vm.alloc_heap(8);

        for index in 0..4_100u32 {
            vm.write_int(src, 2, index).unwrap();
            vm.write_int(src + 4, 2, index ^ 0x55aa).unwrap();
            assert_eq!(
                vm.sys_record_table_copy(handle, Value::Str(format!("key-{index}")), src)
                    .unwrap(),
                0
            );
        }

        assert!(matches!(
            vm.sys_record_table_fetch(dst, handle, Value::Ptr(0), 4_099)
                .unwrap(),
            Value::Int(0)
        ));
        assert_eq!(vm.read_int(dst, 2).unwrap(), 4_099);

        assert_eq!(
            vm.sys_record_table_remove(handle, Value::Str("key-1".into())),
            0
        );
        assert!(matches!(
            vm.sys_record_table_fetch(dst, handle, Value::Ptr(0), 1)
                .unwrap(),
            Value::Int(0)
        ));
        assert_eq!(vm.read_int(dst, 2).unwrap(), 2);
    }

    #[test]
    fn indexed_records_keep_newest_first_and_enforce_capacity() {
        let mut vm = Vm::new();
        let slot = vm.alloc_heap(4);
        assert_eq!(vm.sys_indexed_record_open(slot, 2, 8).unwrap(), 0);
        let handle = vm.read_int(slot, 2).unwrap();
        let src = vm.alloc_heap(8);
        let dst = vm.alloc_heap(8);

        for value in [10, 20, 30] {
            vm.write_int(src, 2, value).unwrap();
            vm.write_value(src + 4, 2, &Value::Str(format!("record-{value}")))
                .unwrap();
            assert_eq!(vm.sys_indexed_record_push(handle, src).unwrap(), 0);
        }

        let count = vm.alloc_heap(4);
        assert_eq!(vm.sys_indexed_record_count(count, handle).unwrap(), 0);
        assert_eq!(vm.read_int(count, 2).unwrap(), 2);
        assert_eq!(vm.sys_indexed_record_load(dst, handle, 0).unwrap(), 0);
        assert_eq!(vm.read_int(dst, 2).unwrap(), 30);
        assert_eq!(
            vm.mem_values.get(&Vm::value_key(dst + 4)),
            Some(&Value::Str("record-30".into()))
        );
        assert_eq!(vm.sys_indexed_record_load(dst, handle, 1).unwrap(), 0);
        assert_eq!(vm.read_int(dst, 2).unwrap(), 20);
        assert_eq!(vm.sys_indexed_record_load(dst, handle, 2).unwrap(), 2);
        assert_eq!(vm.sys_indexed_record_remove(handle, 0, 1), 0);
        assert_eq!(vm.sys_indexed_record_load(dst, handle, 0).unwrap(), 0);
        assert_eq!(vm.read_int(dst, 2).unwrap(), 20);
        assert_eq!(vm.sys_indexed_record_remove(handle, 1, 1), 2);
        assert_eq!(vm.sys_indexed_record_close(handle), 0);
        assert_eq!(vm.sys_indexed_record_close(handle), 1);
    }

    #[test]
    fn indexed_record_open_rejects_zero_dimensions() {
        let mut vm = Vm::new();
        assert_eq!(vm.sys_indexed_record_open(0x100, 0, 8).unwrap(), 2);
        assert_eq!(vm.sys_indexed_record_open(0x100, 8, 0).unwrap(), 2);
    }
}

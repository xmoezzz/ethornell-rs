use crate::{Value, Vm, VmResult};
use ethornell_script::bcs::{BcsCommand, BcsValue, parse_bcs};

const BCS_MAGIC: &[u8] = b"BurikoCompiledScriptVer1.00\0";
const BCS_RET_OPCODE: u32 = 0x1b;
const SCRIPT_CODE_BASE_SLOT: u32 = 0x0004_ca78;
const SCRIPT_CURRENT_OFFSET_SLOT: u32 = 0x0004_cc70;
const SCRIPT_FUNCTION_TABLE_SLOT: u32 = 1768;
const SCRIPT_INFO_COUNT_SLOT: u32 = 314_072 + 26_044;

#[derive(Debug, Clone)]
pub(crate) struct LoadedBcsRange {
    script_name: String,
    base: u32,
    code_start: u32,
    code_end: u32,
    total_len: u32,
}

impl Vm {
    pub(crate) fn scenario_loaded_file_preprocess(
        &mut self,
        script_name: &str,
        base: u32,
        bytes: &[u8],
    ) -> VmResult<()> {
        let Some(program) = parse_bcs(bytes) else {
            return Ok(());
        };

        self.remember_loaded_bcs_range(script_name, base, bytes.len(), &program);
        let direct_strings = self.shadow_bcs_string_refs(base, &program.commands);
        let (sound_strings, text_strings, visual_strings) =
            self.shadow_bcs_command_refs(base, &program.commands);
        let registered_subs = self.register_bcs_subs(base, &program)?;
        tracing::info!(
            script_name,
            bytes = bytes.len(),
            base = format_args!("0x{base:08X}"),
            code_start = format_args!("0x{:X}", program.code_start),
            code_end = format_args!("0x{:X}", program.code_end),
            namespaces = program.namespaces.len(),
            subs = program.subs.len(),
            commands = program.commands.len(),
            direct_strings,
            sound_strings,
            text_strings,
            visual_strings,
            registered_subs,
            "ScenarioLoadedFilePreprocess"
        );
        Ok(())
    }

    pub(crate) fn scenario_code_preprocess(
        &mut self,
        script_name: &str,
        body_length: usize,
    ) -> VmResult<()> {
        let Some((base, bytes)) = self.find_loaded_bcs(script_name, body_length) else {
            tracing::warn!(
                script_name,
                body_length,
                "ScenarioCodePreprocess BCS not found"
            );
            return Ok(());
        };
        let Some(program) = parse_bcs(&bytes) else {
            tracing::warn!(
                script_name,
                body_length,
                base = format_args!("0x{base:08X}"),
                "ScenarioCodePreprocess parse failed"
            );
            return Ok(());
        };

        self.remember_loaded_bcs_range(script_name, base, bytes.len(), &program);
        let direct_strings = self.shadow_bcs_string_refs(base, &program.commands);
        let (sound_strings, text_strings, visual_strings) =
            self.shadow_bcs_command_refs(base, &program.commands);
        let registered_subs = self.register_bcs_subs(base, &program)?;

        tracing::info!(
            script_name,
            body_length,
            base = format_args!("0x{base:08X}"),
            code_start = format_args!("0x{:X}", program.code_start),
            code_end = format_args!("0x{:X}", program.code_end),
            namespaces = program.namespaces.len(),
            subs = program.subs.len(),
            commands = program.commands.len(),
            direct_strings,
            sound_strings,
            text_strings,
            visual_strings,
            registered_subs,
            "ScenarioCodePreprocess"
        );
        Ok(())
    }

    pub(crate) fn scenario_guarded_read_int(&self, ptr: u32, width: u8) -> Option<u32> {
        if width < 2 || self.loaded_bcs_ranges.is_empty() {
            return None;
        }

        let code_base = self.read_raw_u32(SCRIPT_CODE_BASE_SLOT)?;
        let current_offset = self.read_raw_u32(SCRIPT_CURRENT_OFFSET_SLOT)?;
        let ptr_addr = self.memory_addr(ptr);
        if ptr_addr != self.memory_addr(code_base.wrapping_add(current_offset)) {
            return None;
        }

        if self.loaded_bcs_ranges.iter().any(|range| {
            ptr_addr >= range.base.saturating_add(range.code_start)
                && ptr_addr < range.base.saturating_add(range.code_end)
        }) {
            return None;
        }

        let code_base_addr = self.memory_addr(code_base);
        let range = self.loaded_bcs_ranges.iter().find(|range| {
            code_base_addr >= range.base
                && code_base_addr < range.base.saturating_add(range.total_len)
        })?;

        if std::env::var_os("DEBUG").is_some() {
            tracing::warn!(
                script_name = range.script_name.as_str(),
                ptr = format_args!("0x{ptr:08X}"),
                code_base = format_args!("0x{code_base:08X}"),
                current_offset = format_args!("0x{current_offset:08X}"),
                code_start = format_args!("0x{:X}", range.code_start),
                code_end = format_args!("0x{:X}", range.code_end),
                "ScenarioPastCodeReadReturningRet"
            );
        }
        Some(BCS_RET_OPCODE)
    }

    fn remember_loaded_bcs_range(
        &mut self,
        script_name: &str,
        base: u32,
        total_len: usize,
        program: &ethornell_script::bcs::BcsProgram,
    ) {
        let base = self.memory_addr(base);
        self.loaded_bcs_ranges
            .retain(|range| !(range.base == base && range.script_name == script_name));
        self.loaded_bcs_ranges.push(LoadedBcsRange {
            script_name: script_name.to_string(),
            base,
            code_start: program.code_start as u32,
            code_end: program
                .resolver_end
                .unwrap_or(program.code_end)
                .max(program.executable_end) as u32,
            total_len: total_len as u32,
        });
    }

    fn shadow_bcs_command_refs(
        &mut self,
        bcs_base: u32,
        commands: &[BcsCommand],
    ) -> (usize, usize, usize) {
        let mut sound_strings = 0usize;
        let mut text_strings = 0usize;
        let mut visual_strings = 0usize;
        for command in commands {
            match command.name {
                Some("sound") | Some("sound_1a0") | Some("snd") => {
                    sound_strings += self.shadow_command_strings(bcs_base, command, 6);
                }
                Some("say") | Some("msg") => {
                    text_strings += self.shadow_command_strings(bcs_base, command, 16);
                }
                Some("bg")
                | Some("bg240")
                | Some("bg_transition")
                | Some("sprite")
                | Some("grp") => {
                    visual_strings += self.shadow_command_strings(bcs_base, command, 16);
                }
                _ => {}
            }
        }
        (sound_strings, text_strings, visual_strings)
    }

    fn shadow_bcs_string_refs(&mut self, bcs_base: u32, commands: &[BcsCommand]) -> usize {
        let mut written = 0usize;
        for command in commands {
            for string_ref in &command.string_refs {
                if string_ref.text.is_empty() {
                    continue;
                }
                let addr = bcs_base.saturating_add(string_ref.offset);
                self.mem_values
                    .insert(self.value_key(addr), Value::Str(string_ref.text.clone()));
                written += 1;
            }
        }
        written
    }

    fn find_loaded_bcs(&self, script_name: &str, body_length: usize) -> Option<(u32, Vec<u8>)> {
        let mut search_from = 0usize;
        let mut fallback = None;
        let mut matched = None;
        while search_from < self.memory.len() {
            let haystack = &self.memory[search_from..];
            let Some(relative) = find_bytes(haystack, BCS_MAGIC) else {
                break;
            };
            let start = search_from + relative;
            let header_offset = start.checked_add(BCS_MAGIC.len())?;
            let header_size = read_u32(&self.memory, header_offset)? as usize;
            let body_start = header_size.checked_add(0x1c)?;
            let total_len = body_start.checked_add(body_length)?;
            if start.checked_add(total_len)? <= self.memory.len() {
                let bytes = self.memory[start..start + total_len].to_vec();
                if let Some(program) = parse_bcs(&bytes) {
                    fallback = Some((start as u32, bytes.clone()));
                    if bcs_matches_script_name(&program, script_name) {
                        matched = Some((start as u32, bytes));
                    }
                }
            }
            search_from = start.saturating_add(BCS_MAGIC.len());
        }
        matched.or(fallback)
    }

    fn register_bcs_subs(
        &mut self,
        bcs_base: u32,
        program: &ethornell_script::bcs::BcsProgram,
    ) -> VmResult<usize> {
        if program.subs.is_empty() {
            return Ok(0);
        }
        let Some(handle) = self.read_raw_u32(SCRIPT_FUNCTION_TABLE_SLOT) else {
            return Ok(0);
        };
        if handle == 0 || !self.record_tables.contains_key(&self.value_key(handle)) {
            tracing::warn!(subs = program.subs.len(), "BCS function table is not open");
            return Ok(0);
        }

        let script_index = self
            .read_raw_u32(SCRIPT_INFO_COUNT_SLOT)
            .unwrap_or_default();
        let bcs_base = self.memory_addr(bcs_base);
        if std::env::var_os("DEBUG").is_some() {
            tracing::info!(
                script_index,
                bcs_base = format_args!("0x{bcs_base:08X}"),
                subs = program.subs.len(),
                "ScenarioRegisterFunctions"
            );
        }
        let descriptor = self.alloc_heap(8);
        for sub in &program.subs {
            self.write_int(descriptor, 2, script_index)?;
            let address = bcs_base
                .saturating_add(program.code_start as u32)
                .saturating_add(sub.addr);
            self.write_int(descriptor.saturating_add(4), 2, address)?;
            self.sys_record_table_copy(handle, Value::Str(sub.name.clone()), descriptor)?;
        }
        Ok(program.subs.len())
    }

    fn shadow_command_strings(
        &mut self,
        bcs_base: u32,
        command: &BcsCommand,
        field_bias: u32,
    ) -> usize {
        let mut written = 0usize;
        let strings = command
            .string_refs
            .iter()
            .map(|string_ref| string_ref.text.as_str())
            .chain(command.args.iter().filter_map(value_text))
            .filter(|text| !text.is_empty() && !text.starts_with('\u{1b}'))
            .collect::<Vec<_>>();

        for (index, text) in strings.into_iter().enumerate() {
            let addr = bcs_base
                .saturating_add(command.file_offset as u32)
                .saturating_add(field_bias)
                .saturating_add((index as u32).saturating_mul(4));
            self.mem_values
                .insert(self.value_key(addr), Value::Str(text.to_string()));
            written += 1;
        }
        for string_ref in &command.string_refs {
            if !string_ref.text.is_empty() {
                let addr = bcs_base.saturating_add(string_ref.offset);
                self.mem_values
                    .insert(self.value_key(addr), Value::Str(string_ref.text.clone()));
                written += 1;
            }
        }
        written
    }

    fn read_raw_u32(&self, ptr: u32) -> Option<u32> {
        let start = self.memory_addr(ptr) as usize;
        let bytes = self.memory.get(start..start.checked_add(4)?)?;
        Some(u32::from_le_bytes(bytes.try_into().ok()?))
    }
}

fn bcs_matches_script_name(program: &ethornell_script::bcs::BcsProgram, script_name: &str) -> bool {
    let wanted = normalized_script_stem(script_name);
    program.commands.iter().any(|command| {
        command.args.iter().any(|arg| match arg {
            BcsValue::Line { file, .. } => normalized_script_stem(file) == wanted,
            _ => false,
        })
    })
}

fn normalized_script_stem(name: &str) -> String {
    let leaf = name.rsplit(['/', '\\']).next().unwrap_or(name);
    leaf.rsplit_once('.')
        .map(|(stem, _)| stem)
        .unwrap_or(leaf)
        .to_ascii_lowercase()
}

fn value_text(value: &BcsValue) -> Option<&str> {
    match value {
        BcsValue::Str(text) => Some(text.as_str()),
        _ => None,
    }
}

pub(crate) fn find_bytes(haystack: &[u8], needle: &[u8]) -> Option<usize> {
    haystack
        .windows(needle.len())
        .position(|window| window == needle)
}

fn read_u32(buf: &[u8], offset: usize) -> Option<u32> {
    let bytes = buf.get(offset..offset + 4)?;
    Some(u32::from_le_bytes(bytes.try_into().ok()?))
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn guarded_fetch_returns_ret_even_after_the_loaded_file_boundary() {
        let mut vm = Vm::new();
        // The scenario arena is a heap block the scripts allocate.
        let raw_base = vm.alloc_heap(0x6_0000) + 0x614;
        let current_offset = 0x0001_0000u32;
        vm.memory[SCRIPT_CODE_BASE_SLOT as usize..SCRIPT_CODE_BASE_SLOT as usize + 4]
            .copy_from_slice(&raw_base.to_le_bytes());
        vm.memory[SCRIPT_CURRENT_OFFSET_SLOT as usize..SCRIPT_CURRENT_OFFSET_SLOT as usize + 4]
            .copy_from_slice(&current_offset.to_le_bytes());
        vm.loaded_bcs_ranges.push(LoadedBcsRange {
            script_name: "main".into(),
            base: vm.memory_addr(raw_base),
            code_start: 0x60,
            code_end: 0x0a98,
            total_len: 3003,
        });

        assert_eq!(
            vm.scenario_guarded_read_int(raw_base + current_offset, 2),
            Some(BCS_RET_OPCODE)
        );
    }

    #[test]
    fn guarded_fetch_allows_code_in_another_bcs_in_the_same_arena() {
        let mut vm = Vm::new();
        let raw_base = vm.alloc_heap(0x6_0000) + 0x614;
        let target_base = raw_base + 0x4_5A70;
        let target_offset = target_base.wrapping_sub(raw_base).saturating_add(0x7a78);
        vm.memory[SCRIPT_CODE_BASE_SLOT as usize..SCRIPT_CODE_BASE_SLOT as usize + 4]
            .copy_from_slice(&raw_base.to_le_bytes());
        vm.memory[SCRIPT_CURRENT_OFFSET_SLOT as usize..SCRIPT_CURRENT_OFFSET_SLOT as usize + 4]
            .copy_from_slice(&target_offset.to_le_bytes());
        vm.loaded_bcs_ranges.push(LoadedBcsRange {
            script_name: "main".into(),
            base: vm.memory_addr(raw_base),
            code_start: 0x60,
            code_end: 0x0ae0,
            total_len: 3003,
        });
        vm.loaded_bcs_ranges.push(LoadedBcsRange {
            script_name: "function".into(),
            base: vm.memory_addr(target_base),
            code_start: 0x550,
            code_end: 0x7c44,
            total_len: 63_940,
        });

        assert_eq!(
            vm.scenario_guarded_read_int(raw_base.wrapping_add(target_offset), 2),
            None
        );
    }
}

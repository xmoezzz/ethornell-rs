use super::*;
use ethornell_vm::SysApi;

#[derive(Default)]
pub(super) struct NativeSystemState {
    pub(super) main_loop_wait_override: i32,
    pub(super) drag_drop_enabled: bool,
    pub(super) fullscreen_hotkeys_enabled: bool,
    pub(super) fullscreen_hotkeys: Vec<i32>,
    pub(super) display_scaling_mode: i32,
    pub(super) cursor_index: i32,
    pub(super) native_close_mode: i32,
    pub(super) raster_wait_enabled: i32,
    pub(super) bootstrap_root_or_archive: String,
    pub(super) bootstrap_namespace: String,
    cursor_button_mode: i32,
    pub(super) uninstaller_mutex_name: String,
    pub(super) restart_working_directory: String,
    pub(super) restart_command_line: String,
    pub(super) restart_error_message: String,
}

impl NativeSystemState {
    /// Sys80:1E mouse-button mapping mode (dword_56691C, sub_48EE70).
    pub(super) fn mouse_button_mapping_mode(&self) -> i32 {
        self.cursor_button_mode
    }
}

impl RuntimeTraceApi {
    pub(super) fn dispatch_native_system(
        &mut self,
        call: &mut ethornell_vm::NativeCallFrame,
    ) -> Option<ethornell_vm::VmResult<ethornell_vm::Value>> {
        let (group, id) = (call.group(), call.id());
        let stack = call.args_mut();
        if group != 0x80 {
            return None;
        }

        let value = match id {
            // sub_49A240 is the WM_ACTIVATE/minimize latch.
            0x0E => ethornell_vm::Value::Int(i32::from(self.pending_window_minimize)),
            0x1D => {
                let input_descriptor = pop_int_value(stack).unwrap_or_default();
                let scope = pop_int_value(stack).unwrap_or_default();
                ethornell_vm::Value::Int(ethornell_vm::SysApi::query_scoped_input_event(
                    self,
                    input_descriptor,
                    scope,
                ))
            }
            0x1E => {
                let mode = pop_int_value(stack).unwrap_or_default();
                let accepted = (0..=1).contains(&mode);
                if accepted {
                    self.native_system.cursor_button_mode = mode;
                }
                ethornell_vm::Value::Int(i32::from(accepted))
            }
            0x1F => {
                let cancel_on_large_move = pop_int_value(stack).unwrap_or_default();
                let updates_per_second = pop_int_value(stack).unwrap_or_default();
                let duration_ms = pop_int_value(stack).unwrap_or_default();
                let interpolation_mode = pop_int_value(stack).unwrap_or_default();
                let target_y = pop_int_value(stack).unwrap_or_default();
                let target_x = pop_int_value(stack).unwrap_or_default();
                self.configure_cursor_motion(
                    cancel_on_large_move,
                    updates_per_second,
                    duration_ms,
                    interpolation_mode,
                    target_y,
                    target_x,
                );
                ethornell_vm::Value::None
            }
            0x24 => {
                let recursive = pop_int_value(stack).unwrap_or_default() != 0;
                let pattern = pop_string_value(stack).unwrap_or_default();
                let count = self
                    .enumerate_native_paths(&pattern, recursive, 0, false)
                    .len()
                    .min(i32::MAX as usize) as i32;
                ethornell_vm::Value::Int(count)
            }
            // VM owns the destination buffer for 0x26.
            0x26 => {
                let _ = pop_args(stack, 4);
                ethornell_vm::Value::Int(0)
            }
            0x27 => {
                let from = pop_string_value(stack).unwrap_or_default();
                let to = pop_string_value(stack).unwrap_or_default();
                let ok = match (
                    runtime_file_path_from_root(&self.manager, &self.native_root, &from),
                    runtime_file_path_from_root(&self.manager, &self.native_root, &to),
                ) {
                    (Some(from), Some(to)) => {
                        if let Some(parent) = to.parent() {
                            let _ = std::fs::create_dir_all(parent);
                        }
                        std::fs::rename(from, to).is_ok()
                    }
                    _ => false,
                };
                tracing::info!(from, to, ok, "MoveFile");
                ethornell_vm::Value::Int(i32::from(ok))
            }
            // Native sub_4667A0 repeatedly resolves the required file and
            // presents Retry/Quit until media becomes available or the user
            // quits. The middle string is popped but unused by the target.
            0x3C => {
                let message = pop_string_value(stack).unwrap_or_default();
                let _ignored_context = pop_string_value(stack).unwrap_or_default();
                let file = pop_string_value(stack).unwrap_or_default();
                let title = self
                    .pending_window_title
                    .clone()
                    .unwrap_or_else(|| "Ethornell".to_string());
                let found = loop {
                    if find_runtime_file_from_root(&self.manager, &self.native_root, "", &file)
                        .is_some_and(|path| path.is_file())
                        || find_runtime_resource(&self.manager, "", &file).is_some()
                    {
                        tracing::info!(file, "Sys80_3C_RequiredResourceFound");
                        break true;
                    }
                    tracing::warn!(file, "Sys80_3C_RequiredResourceMissing");
                    let retry = host_dialog::show_message(
                        host_dialog::MessageDialogKind::RetryCancel,
                        &title,
                        &message,
                        true,
                    )
                    .unwrap_or(false);
                    if !retry {
                        break false;
                    }
                };
                ethornell_vm::Value::Int(i32::from(found))
            }
            0x3E => {
                let path = pop_string_value(stack).unwrap_or_default();
                let exists = runtime_file_path_from_root(&self.manager, &self.native_root, &path)
                    .is_some_and(|path| path.is_dir());
                ethornell_vm::Value::Int(i32::from(exists))
            }
            0x53 => ethornell_vm::Value::None,
            // VM-owned scheduler boundary. Target ABI proves that 0x54
            // installs CProcedure, but the single argument and wake predicate
            // remain unrecovered. This host fallback only consumes the value;
            // it must not invent the legacy guessed WaitWndMsg implementation.
            0x54 => {
                let _arg0_unrecovered = stack.pop();
                ethornell_vm::Value::None
            }
            0x5D => {
                let _enabled = pop_int_value(stack).unwrap_or_default();
                ethornell_vm::Value::None
            }
            0x63 => {
                let argument = pop_int_value(stack).unwrap_or_default();
                self.native_system.display_scaling_mode = i32::from(argument == 0);
                ethornell_vm::Value::None
            }
            // sub_4650F0 selects a new bootstrap pair and returns interpreter
            // status 5. The VM intercepts actual program replacement.
            0x6B => {
                let file = pop_string_value(stack).unwrap_or_default();
                let archive = pop_string_value(stack).unwrap_or_default();
                tracing::info!(archive, file, "SelectBootstrapProgram");
                ethornell_vm::Value::None
            }
            0x6C => {
                self.native_system.drag_drop_enabled =
                    pop_int_value(stack).unwrap_or_default() != 0;
                self.dropped_files.clear();
                ethornell_vm::Value::None
            }
            // VM owns the output string.
            0x6D => {
                let _destination = stack.pop();
                ethornell_vm::Value::Int(0)
            }
            0x6E => {
                self.native_system.raster_wait_enabled = pop_int_value(stack).unwrap_or_default();
                ethornell_vm::Value::None
            }
            0x6F => ethornell_vm::Value::Int(i32::from(self.display_aspect_mismatch())),
            // Sys80:78..7B are VM-owned because their payload and output pointers
            // address BP memory and the global-config buffer.
            // Sys80:90..B6 are VM-owned process-global record, object-message,
            // and exclusion primitives. They must not be reinterpreted as app-local events.
            _ => return None,
        };
        Some(Ok(value))
    }

    pub(super) fn enumerate_native_paths(
        &self,
        pattern: &str,
        recursive: bool,
        max_count: usize,
        directories: bool,
    ) -> Vec<String> {
        let normalized = pattern.replace('\\', std::path::MAIN_SEPARATOR_STR);
        let pattern_path = Path::new(&normalized);
        let wildcard = pattern_path
            .file_name()
            .and_then(|name| name.to_str())
            .unwrap_or("*");
        let parent = pattern_path.parent().unwrap_or_else(|| Path::new(""));
        let Some(root) = runtime_file_path_from_root(
            &self.manager,
            &self.native_root,
            &parent.to_string_lossy(),
        ) else {
            return Vec::new();
        };
        let mut output = Vec::new();
        enumerate_native_directory(
            &root,
            &root,
            wildcard,
            recursive,
            max_count,
            directories,
            &mut output,
        );
        output
    }
}

pub(super) fn split_native_command_line(command_line: &str) -> Vec<String> {
    let mut args = Vec::new();
    let mut current = String::new();
    let mut quoted = false;
    let mut backslashes = 0usize;
    for ch in command_line.chars() {
        match ch {
            '\\' => backslashes += 1,
            '"' => {
                current.extend(std::iter::repeat_n('\\', backslashes / 2));
                if backslashes.is_multiple_of(2) {
                    quoted = !quoted;
                } else {
                    current.push('"');
                }
                backslashes = 0;
            }
            ch if ch.is_whitespace() && !quoted => {
                current.extend(std::iter::repeat_n('\\', backslashes));
                backslashes = 0;
                if !current.is_empty() {
                    args.push(std::mem::take(&mut current));
                }
            }
            ch => {
                current.extend(std::iter::repeat_n('\\', backslashes));
                backslashes = 0;
                current.push(ch);
            }
        }
    }
    current.extend(std::iter::repeat_n('\\', backslashes));
    if !current.is_empty() {
        args.push(current);
    }
    args
}

pub(super) fn decode_native_text(bytes: &[u8]) -> String {
    let (decoded, _, _) = encoding_rs::SHIFT_JIS.decode(bytes);
    decoded.into_owned()
}

pub(super) fn encode_native_text(text: &str) -> Vec<u8> {
    encoding_rs::SHIFT_JIS.encode(text).0.into_owned()
}

fn enumerate_native_directory(
    root: &Path,
    directory: &Path,
    pattern: &str,
    recursive: bool,
    max_count: usize,
    directories: bool,
    output: &mut Vec<String>,
) {
    let Ok(entries) = std::fs::read_dir(directory) else {
        return;
    };
    let mut entries = entries.filter_map(|entry| entry.ok()).collect::<Vec<_>>();
    entries.sort_by_key(|entry| entry.file_name().to_string_lossy().to_ascii_lowercase());
    for entry in entries {
        if max_count != 0 && output.len() >= max_count {
            return;
        }
        let Ok(file_type) = entry.file_type() else {
            continue;
        };
        let name = entry.file_name();
        let name = name.to_string_lossy();
        if file_type.is_dir() {
            if directories && windows_wildcard_matches(pattern, &name) {
                let relative = entry.path();
                let relative = relative.strip_prefix(root).unwrap_or(&relative);
                output.push(
                    relative
                        .to_string_lossy()
                        .replace(std::path::MAIN_SEPARATOR, "\\"),
                );
            }
            if recursive && name != "." && name != ".." {
                enumerate_native_directory(
                    root,
                    &entry.path(),
                    pattern,
                    true,
                    max_count,
                    directories,
                    output,
                );
            }
        } else if !directories && windows_wildcard_matches(pattern, &name) {
            let relative = entry.path();
            let relative = relative.strip_prefix(root).unwrap_or(&relative);
            output.push(
                relative
                    .to_string_lossy()
                    .replace(std::path::MAIN_SEPARATOR, "\\"),
            );
        }
    }
}

#[cfg(test)]
mod tests {
    #[test]
    fn vm_dispatched_group_80_handlers_are_explicitly_owned() {
        for id in [
            0x26u16, 0x6D, 0x7A, 0xA9, 0xCF, 0xDE, 0xE9, 0xEA, 0xF1, 0xF3, 0xF4, 0xF5, 0xF6, 0xF9,
            0xFA, 0xFB, 0xFC, 0xFE,
        ] {
            assert!(
                ethornell_vm::native_ownership::owns_before_host_dispatch(0x80, id),
                "System80:{id:02X} has a VM dispatch but no VM ownership declaration"
            );
        }
    }
}

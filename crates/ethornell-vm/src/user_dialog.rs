//! UserB0 input dialogs (0x84..0x87, 0x8C, 0x8F). Each one is split into the
//! request a host shows and the guest writes its answer produces, so that a
//! desktop host can answer at once while an embedding host (mobile, web,
//! console) answers later through `Vm::complete_user_dialog`; both paths write
//! the same bytes.
use crate::{
    HostMemoryWrite, HostResponse, UserDialogRequest, UserDialogResponse, Value, Vm, VmError,
    VmResult, target_dialog_max_bytes,
};

/// Where an accepted answer lands in guest memory.
#[derive(Debug, Clone, PartialEq, Eq)]
pub(crate) enum UserDialogSink {
    /// One C string of at most `capacity` encoded bytes.
    Text { destination: u32, capacity: usize },
    /// Two independent C strings.
    TwoText { first: (u32, usize), second: (u32, usize) },
    /// Four 10-byte fields and the optional month/day index DWORDs.
    Date { fields: [u32; 4], month: u32, day: u32 },
}

pub(crate) fn is_user_dialog(group: u8, id: u16) -> bool {
    group == 0xb0 && matches!(id, 0x84 | 0x85 | 0x86 | 0x87 | 0x8c | 0x8f)
}

impl Vm {
    /// Pop one dialog's operands, already string-normalized, in the target's
    /// order. Returns None for any other selector.
    pub(crate) fn parse_user_dialog(
        &mut self,
        id: u16,
    ) -> VmResult<Option<(UserDialogRequest, UserDialogSink)>> {
        Ok(Some(match id {
            0x84 => {
                // sub_478D70: mode, initial, title, output buffer.
                let mode = self.pop_int()?;
                let initial = self.pop_string_lossy()?;
                let title = self.pop_string_lossy()?;
                let destination = self.pop_ptr()?;
                let max_bytes = target_dialog_max_bytes(mode, 255);
                (
                    UserDialogRequest::Input { title, initial, max_bytes, numeric: mode < 0 },
                    UserDialogSink::Text { destination, capacity: max_bytes },
                )
            }
            0x85 => {
                // sub_478DC0: two independent 256-byte output buffers.
                let second_max = self.pop_int()?;
                let second_initial = self.pop_string_lossy()?;
                let second_label = self.pop_string_lossy()?;
                let first_max = self.pop_int()?;
                let first_initial = self.pop_string_lossy()?;
                let first_label = self.pop_string_lossy()?;
                let title = self.pop_string_lossy()?;
                let second_destination = self.pop_ptr()?;
                let first_destination = self.pop_ptr()?;
                let first_max_bytes = target_dialog_max_bytes(first_max, 255);
                let second_max_bytes = target_dialog_max_bytes(second_max, 255);
                (
                    UserDialogRequest::TwoField {
                        title,
                        first_label,
                        first_initial,
                        first_max_bytes,
                        first_numeric: false,
                        second_label,
                        second_initial,
                        second_max_bytes,
                        second_numeric: false,
                    },
                    UserDialogSink::TwoText {
                        first: (first_destination, first_max_bytes),
                        second: (second_destination, second_max_bytes),
                    },
                )
            }
            0x86 => {
                // sub_478E60: four fixed segments joined with '-'.
                let _extra = self.pop_int()?;
                let signed_max = self.pop_int()?;
                let prompt = self.pop_string_lossy()?;
                let title = self.pop_string_lossy()?;
                let destination = self.pop_ptr()?;
                let max_bytes_per_segment = target_dialog_max_bytes(signed_max, 32);
                (
                    UserDialogRequest::Segmented {
                        title,
                        prompt,
                        segment_count: 4,
                        max_bytes_per_segment,
                        numeric: signed_max < 0,
                    },
                    UserDialogSink::Text {
                        destination,
                        capacity: max_bytes_per_segment.saturating_mul(4).saturating_add(3),
                    },
                )
            }
            0x87 => {
                // sub_478EC0: extended two-field template with independent
                // numeric filters and output buffers.
                let second_numeric = self.pop_int()? != 0;
                let second_max = self.pop_int()?;
                let second_initial = self.pop_string_lossy()?;
                let second_label = self.pop_string_lossy()?;
                let first_numeric = self.pop_int()? != 0;
                let first_max = self.pop_int()?;
                let first_initial = self.pop_string_lossy()?;
                let first_label = self.pop_string_lossy()?;
                let title = self.pop_string_lossy()?;
                let second_destination = self.pop_ptr()?;
                let first_destination = self.pop_ptr()?;
                let _template = self.pop_int()?;
                let first_max_bytes = target_dialog_max_bytes(first_max, 255);
                let second_max_bytes = target_dialog_max_bytes(second_max, 255);
                (
                    UserDialogRequest::TwoField {
                        title,
                        first_label,
                        first_initial,
                        first_max_bytes,
                        first_numeric,
                        second_label,
                        second_initial,
                        second_max_bytes,
                        second_numeric,
                    },
                    UserDialogSink::TwoText {
                        first: (first_destination, first_max_bytes),
                        second: (second_destination, second_max_bytes),
                    },
                )
            }
            0x8c => {
                // sub_478F80 builds one list from newline-delimited text and
                // writes the selected row through the caller buffer.
                let option_text = self.pop_string_lossy()?;
                let prompt = self.pop_string_lossy()?;
                let title = self.pop_string_lossy()?;
                let destination = self.pop_ptr()?;
                let options = option_text
                    .split('\n')
                    .map(|line| line.trim_end_matches('\r').to_string())
                    .filter(|line| !line.is_empty())
                    .collect::<Vec<_>>();
                (
                    UserDialogRequest::Selection { title, prompt, options },
                    UserDialogSink::Text { destination, capacity: 779 },
                )
            }
            0x8f => {
                // sub_478FD0 uses four mutable 10-byte text fields and two
                // caller-owned zero-based month/day indices.
                let day = self.pop_ptr()?;
                let month = self.pop_ptr()?;
                let fourth = self.pop_ptr()?;
                let third = self.pop_ptr()?;
                let second = self.pop_ptr()?;
                let first = self.pop_ptr()?;
                let initial = |vm: &Vm, pointer: u32| -> VmResult<String> {
                    if pointer == 0 { Ok(String::new()) } else { vm.read_c_string(pointer) }
                };
                let index = |vm: &Vm, pointer: u32| -> VmResult<i32> {
                    if pointer == 0 { Ok(0) } else { Ok(vm.read_int(pointer, 2)? as i32) }
                };
                (
                    UserDialogRequest::DateFields {
                        fields: [
                            initial(self, first)?,
                            initial(self, second)?,
                            initial(self, third)?,
                            initial(self, fourth)?,
                        ],
                        month_index: index(self, month)?,
                        day_index: index(self, day)?,
                    },
                    UserDialogSink::Date { fields: [first, second, third, fourth], month, day },
                )
            }
            _ => return Ok(None),
        }))
    }

    /// The bytes `write_c_string_bounded` stores: the longest prefix whose
    /// Shift-JIS form fits `max_bytes` without splitting a character, in the
    /// native string encoding, NUL-terminated.
    fn bounded_c_string_write(
        &self,
        address: u32,
        text: &str,
        max_bytes: usize,
    ) -> VmResult<HostMemoryWrite> {
        if address == 0 {
            return Err(VmError::Runtime("null writable Shift-JIS string pointer".into()));
        }
        let mut bounded = String::new();
        for ch in text.chars() {
            let mut candidate = bounded.clone();
            candidate.push(ch);
            if encoding_rs::SHIFT_JIS.encode(&candidate).0.len() > max_bytes {
                break;
            }
            bounded.push(ch);
        }
        let mut bytes = self.encode_native_string(&bounded).into_owned();
        bytes.push(0);
        Ok(HostMemoryWrite { address, bytes })
    }

    /// Whether the dialog was accepted and what it writes. A missing answer
    /// or one of the wrong kind is a cancel and writes nothing.
    pub(crate) fn user_dialog_writes(
        &self,
        sink: &UserDialogSink,
        response: Option<UserDialogResponse>,
    ) -> VmResult<(bool, Vec<HostMemoryWrite>)> {
        let writes = match (sink, response) {
            (
                UserDialogSink::Text { destination, capacity },
                Some(
                    UserDialogResponse::Input(text)
                    | UserDialogResponse::Segmented(text)
                    | UserDialogResponse::Selection(text),
                ),
            ) => vec![self.bounded_c_string_write(*destination, &text, *capacity)?],
            (
                UserDialogSink::TwoText { first, second },
                Some(UserDialogResponse::TwoField([first_text, second_text])),
            ) => vec![
                self.bounded_c_string_write(first.0, &first_text, first.1)?,
                self.bounded_c_string_write(second.0, &second_text, second.1)?,
            ],
            (
                UserDialogSink::Date { fields, month, day },
                Some(UserDialogResponse::DateFields { fields: texts, month_index, day_index }),
            ) => {
                let mut writes = fields
                    .iter()
                    .zip(texts.iter())
                    .map(|(address, text)| self.bounded_c_string_write(*address, text, 10))
                    .collect::<VmResult<Vec<_>>>()?;
                for (address, value) in [(*month, month_index), (*day, day_index)] {
                    if address != 0 {
                        writes.push(HostMemoryWrite {
                            address,
                            bytes: value.to_le_bytes().to_vec(),
                        });
                    }
                }
                writes
            }
            _ => return Ok((false, Vec::new())),
        };
        Ok((true, writes))
    }

    pub(crate) fn apply_host_writes(&mut self, writes: Vec<HostMemoryWrite>) -> VmResult<()> {
        for write in writes {
            let range = self.resolve_write_range(write.address, write.bytes.len())?;
            self.memory[range].copy_from_slice(&write.bytes);
            self.clear_shadow_values(write.address, write.bytes.len());
        }
        Ok(())
    }

    /// Answer a `HostRequest::UserDialog`; None is a cancel. The dialog's
    /// result (1 accepted, 0 cancelled) is its single stack output.
    pub fn complete_user_dialog(
        &mut self,
        id: u64,
        response: Option<UserDialogResponse>,
    ) -> VmResult<()> {
        let target = self
            .host_request_vm_mut(id)
            .ok_or_else(|| VmError::Runtime("unknown or cancelled host request".into()))?;
        let sink = target
            .pending_host_request
            .as_ref()
            .and_then(|pending| pending.user_dialog.clone())
            .ok_or_else(|| VmError::Runtime("host request is not a user dialog".into()))?;
        let (accepted, writes) = target.user_dialog_writes(&sink, response)?;
        self.complete_host_request(
            id,
            HostResponse { outputs: vec![Value::Int(i32::from(accepted))], writes },
        )
    }
}

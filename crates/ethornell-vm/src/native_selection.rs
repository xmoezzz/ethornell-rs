//! BP pointer bridge for CProcSelect; see src/engine/selection.c.
use crate::{GraphApi, NativeOpcode, Value, Vm, VmError, VmResult};

#[derive(Debug, Clone)]
pub enum NativeSelectionData {
    Items {
        text: Vec<String>,
        columns: i32,
        centered: bool,
        color: i32,
        initial: i32,
        /// Bitmap, x offset, y offset for the two Ex brackets.
        overlays: Option<[[i32; 3]; 2]>,
        blink_confirmation: bool,
    },
    Icons {
        records: Vec<[i32; 16]>,
        extended: bool,
        key_assignments: Option<Vec<i32>>,
        input_mode: i32,
        alpha_hit_test: bool,
    },
}

#[derive(Debug, Clone)]
pub struct NativeSelectionRequest {
    pub owner_thread_id: i32,
    pub opcode: NativeOpcode,
    pub window: i32,
    pub allow_cancel: bool,
    pub data: NativeSelectionData,
}

#[derive(Debug, Clone, Copy)]
pub struct NativeSelectionCompletion {
    pub values: [i32; 3],
    pub count: usize,
}

impl Vm {
    pub(crate) fn decode_native_selection<A: GraphApi>(
        &mut self,
        api: &A,
        opcode: NativeOpcode,
        args: &[Value],
    ) -> VmResult<NativeSelectionRequest> {
        let a = args.iter().map(Value::as_i32).collect::<Vec<_>>();
        let window = a[0];
        let count = a[1];
        let items = matches!(opcode.id, 0xA0 | 0xA2 | 0xA3);
        let limit = if items { 16 } else { 64 };
        let fatal = |message: String| Err(VmError::Runtime(message));
        // sub_491F30 / sub_492370 check count, initial item, columns or
        // the focus mode before the window is looked up (sub_440A80).
        if count.wrapping_sub(1) as u32 >= limit {
            return fatal(if items {
                format!("無効な項目数 [ {count} ] が指定されました")
            } else {
                format!("無効なアイコン数 [ {count} ] が指定されました")
            });
        }
        if items {
            if a[6] < 0 || a[6] >= count {
                return fatal(format!("無効な初期選択項目番号 [ {} ] が指定されました", a[6]));
            }
            // sub_491F30 only rejects more than 16 (unsigned); zero passes
            // and the target then divides by zero, which is reported here
            // with the same text instead.
            if !(1..=16).contains(&a[3]) {
                return fatal(format!("無効な縦列数 [ {} ] が指定されました", a[3]));
            }
        } else if a[4] as u32 > 3 {
            return fatal(format!("無効なフォーカスモード [ {} ] が指定されました", a[4]));
        }
        if !api.graph_window_exists(window) {
            return fatal("無効なウィンドウハンドルが指定されました".into());
        }
        let data = if items {
            let text = self.read_counted_string_pointer_list(a[2] as u32, count as usize)?;
            NativeSelectionData::Items {
                text,
                columns: a[3],
                centered: a[4] != 0,
                color: a[5],
                initial: a[6],
                overlays: (opcode.id != 0xA0).then(|| [[a[8], a[9], a[10]], [a[11], a[12], a[13]]]),
                blink_confirmation: opcode.id == 0xA3,
            }
        } else {
            let extended = opcode.id == 0xB1;
            let stride = if extended { 64 } else { 16 };
            let base = a[2] as u32;
            let _ = self.resolve_range(base, count as usize * stride)?;
            let mut records = Vec::with_capacity(count as usize);
            for i in 0..count as usize {
                let mut record = [0; 16];
                for (j, word) in record[..stride / 4].iter_mut().enumerate() {
                    *word =
                        self.read_int(base.wrapping_add((i * stride + j * 4) as u32), 2)? as i32;
                }
                records.push(record);
            }
            let key_assignments = if a[3] == 0 {
                None
            } else {
                let base = a[3] as u32;
                let _ = self.resolve_range(base, count as usize * 4)?;
                let mut keys = Vec::with_capacity(count as usize);
                for i in 0..count as u32 {
                    keys.push(self.read_int(base.wrapping_add(i * 4), 2)? as i32);
                }
                Some(keys)
            };
            NativeSelectionData::Icons {
                records,
                extended,
                key_assignments,
                input_mode: a[4],
                alpha_hit_test: a[6] != 0,
            }
        };
        Ok(NativeSelectionRequest {
            owner_thread_id: self.thread.thread_id(),
            opcode,
            window,
            allow_cancel: a[if items { 7 } else { 5 }] != 0,
            data,
        })
    }
}

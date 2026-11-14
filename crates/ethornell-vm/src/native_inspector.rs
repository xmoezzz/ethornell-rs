//! B0:60..6F, 1400AC5F0..B03E0: modeless property inspectors.
//! Binding addresses remain checked guest data, never executable/native pointers.
use crate::{NativeAbiProfile, SysApi, Value, Vm, VmError, VmResult};
use std::collections::{BTreeMap, VecDeque};

#[derive(Debug, Clone, PartialEq)]
pub struct NativeInspectorRow {
    pub name: String,
    pub value: String,
    pub kind: i32,
    pub editable: bool,
}
#[derive(Debug, Clone, PartialEq)]
pub struct NativeInspectorTab {
    pub title: String,
    pub rows: Vec<NativeInspectorRow>,
}
#[derive(Debug, Clone, PartialEq)]
pub struct NativeInspectorView {
    pub handle: u32,
    pub title: String,
    pub description: String,
    pub position: [i32; 2],
    pub columns: [i32; 2],
    pub selected_tab: usize,
    pub tabs: Vec<NativeInspectorTab>,
    pub buttons: Vec<String>,
}
#[derive(Debug, Clone, PartialEq)]
pub enum NativeInspectorEvent {
    Button(usize),
    Tab(usize),
    Select(Option<usize>),
    Edit {
        tab: usize,
        row: usize,
        text: String,
    },
    Close,
}
/// Owned fields from the live render object; keys identify maintained
/// engine state, never native object addresses.
#[derive(Debug, Clone)]
pub struct NativeObjectInspectorProperty {
    pub name: String,
    pub kind: i32,
    pub value: Value,
    pub key: Option<u8>,
    pub editable: bool,
}
#[derive(Debug, Clone)]
pub struct NativeObjectInspector {
    pub object_type: String,
    pub properties: Vec<NativeObjectInspectorProperty>,
}
#[derive(Debug, Clone, Copy)]
enum Binding {
    Guest(i32, u32),
    Object(i32, u8),
}
#[derive(Debug, Clone)]
struct Property {
    name: String,
    kind: i32,
    value: Value,
    binding: Option<Binding>,
    editable: bool,
}
#[derive(Debug)]
struct Inspector {
    view: NativeInspectorView,
    properties: Vec<Vec<Property>>,
    events: VecDeque<[i32; 4]>,
}
#[derive(Debug, Default)]
pub(crate) struct NativeInspectors {
    next: u32,
    entries: BTreeMap<u32, Inspector>,
}

impl Property {
    fn display(&self) -> String {
        let n = self.value.as_i32();
        match self.kind {
            0 => n.to_string(),
            1 => (n as u32).to_string(),
            2 => format!("0x{:08X}", n as u32),
            3 => format!("{:.6}", f32::from_bits(n as u32)),
            4 => if n == 0 { "FALSE" } else { "TRUE" }.into(),
            5 => match &self.value {
                Value::Str(s) => s.clone(),
                _ => String::new(),
            },
            _ => unreachable!(),
        }
    }
}
impl Inspector {
    fn snapshot(&mut self) {
        for (tab, properties) in self.view.tabs.iter_mut().zip(&self.properties) {
            tab.rows = properties
                .iter()
                .map(|p| NativeInspectorRow {
                    name: p.name.clone(),
                    value: p.display(),
                    kind: p.kind,
                    editable: p.editable,
                })
                .collect();
        }
    }
    fn event(&mut self, code: i32, payload: i32) -> VmResult<()> {
        if self.events.len() >= 4096 {
            return Err(VmError::Runtime("native inspector event limit".into()));
        }
        // DialogFunc allocates two DWORDs but AC8C0 copies sixteen bytes.
        // Define the unused trailing words as zero instead of leaking memory.
        self.events.push_back([code, payload, 0, 0]);
        Ok(())
    }
}
impl Vm {
    /// AB460 -> ABD00: add a bound Sprite/Window page to an existing
    /// inspector. The fourth BP operand is the page title, not a callback.
    pub(super) fn execute_native_object_inspector<A: SysApi>(
        &mut self,
        api: &mut A,
    ) -> VmResult<()> {
        let title_ptr = self.pop_ptr()?;
        let object = self.pop_int()?;
        let handle = self.pop_int()? as u32;
        let output = self.pop_ptr()?;
        let title = if title_ptr == 0 {
            String::new()
        } else {
            self.read_c_string(title_ptr)?
        };
        let object_state = match api.inspect_native_display_object(object) {
            Ok(s) => s,
            Err(status) => {
                self.push_value(Value::Int(status));
                return Ok(());
            }
        };
        let mut state = std::mem::take(
            &mut self
                .system81_shared
                .lock()
                .expect("system81 state poisoned")
                .inspectors,
        );
        let result = (|| {
            let Some(inspector) = state.entries.get_mut(&handle) else {
                return Ok(-1);
            };
            if inspector.view.tabs.len() >= 256 {
                return Err(VmError::Runtime("native inspector tab limit".into()));
            }
            if output != 0 {
                self.resolve_write_range(output, 4)?;
            }
            let index = inspector.view.tabs.len();
            let mut properties = vec![
                Property {
                    name: "Handle".into(),
                    kind: 2,
                    value: Value::Int(object),
                    binding: None,
                    editable: false,
                },
                Property {
                    name: "Object Type".into(),
                    kind: 5,
                    value: Value::Str(object_state.object_type),
                    binding: None,
                    editable: false,
                },
            ];
            properties.extend(object_state.properties.into_iter().map(|p| Property {
                name: p.name,
                kind: p.kind,
                value: p.value,
                binding: p.key.map(|key| Binding::Object(object, key)),
                editable: p.editable,
            }));
            inspector.view.tabs.push(NativeInspectorTab {
                title,
                rows: Vec::new(),
            });
            inspector.properties.push(properties);
            inspector.snapshot();
            api.publish_native_inspector(&inspector.view)?;
            if output != 0 {
                self.write_int(output, 2, index as u32)?;
            }
            Ok(0)
        })();
        self.system81_shared
            .lock()
            .expect("system81 state poisoned")
            .inspectors = state;
        self.push_value(Value::Int(result?));
        Ok(())
    }
    pub(super) fn try_native_inspector<A: SysApi>(
        &mut self,
        api: &mut A,
        id: u16,
    ) -> VmResult<Option<Value>> {
        if self.native_abi_profile != NativeAbiProfile::ModernFused
            || !matches!(id,0x60..=0x63|0x66..=0x68|0x6a|0x6c|0x6d|0x6f)
        {
            return Ok(None);
        }
        let mut state = std::mem::take(
            &mut self
                .system81_shared
                .lock()
                .expect("system81 state poisoned")
                .inspectors,
        );
        let result = self.execute_native_inspector(api, id, &mut state);
        self.system81_shared
            .lock()
            .expect("system81 state poisoned")
            .inspectors = state;
        result.map(Some)
    }
    fn update_inspector<A: SysApi>(
        &mut self,
        api: &mut A,
        inspector: &mut Inspector,
    ) -> VmResult<()> {
        for properties in &mut inspector.properties {
            for p in properties {
                match p.binding {
                    Some(Binding::Guest(owner, pointer)) if owner == self.thread.thread_id() => {
                        p.value = if p.kind == 5 {
                            Value::Str(self.read_c_string(pointer)?)
                        } else {
                            Value::Int(self.read_int(pointer, 2)? as i32)
                        };
                    }
                    Some(Binding::Object(object, key)) => {
                        if let Ok(snapshot) = api.inspect_native_display_object(object) {
                            if let Some(field) =
                                snapshot.properties.into_iter().find(|f| f.key == Some(key))
                            {
                                p.value = field.value;
                            }
                        }
                    }
                    _ => {}
                }
            }
        }
        for event in api.take_native_inspector_events(inspector.view.handle)? {
            match event {
                NativeInspectorEvent::Button(n) if n < inspector.view.buttons.len() => {
                    inspector.event(1, n as i32)?
                }
                NativeInspectorEvent::Tab(n) if n < inspector.view.tabs.len() => {
                    inspector.view.selected_tab = n;
                    inspector.event(2, n as i32)?;
                }
                NativeInspectorEvent::Select(row) => {
                    let tab = inspector.view.selected_tab;
                    if row.is_none_or(|n| n < inspector.properties.get(tab).map_or(0, Vec::len)) {
                        let packed = (tab as u32) << 16 | row.map_or(65535, |n| n as u32 & 65535);
                        inspector.event(3, packed as i32)?;
                    }
                }
                NativeInspectorEvent::Edit { tab, row, text } => {
                    let Some(property) = inspector
                        .properties
                        .get_mut(tab)
                        .and_then(|t| t.get_mut(row))
                    else {
                        continue;
                    };
                    if !property.editable {
                        continue;
                    }
                    let number = match property.kind {
                        0 => text.trim().parse::<i32>().ok(),
                        1 => text.trim().parse::<u32>().ok().map(|n| n as i32),
                        2 => u32::from_str_radix(
                            text.trim()
                                .trim_start_matches("0x")
                                .trim_start_matches("0X"),
                            16,
                        )
                        .ok()
                        .map(|n| n as i32),
                        3 => text
                            .trim()
                            .parse::<f32>()
                            .ok()
                            .filter(|n| n.is_finite())
                            .map(|n| n.to_bits() as i32),
                        4 => match text.trim().to_ascii_lowercase().as_str() {
                            "true" | "1" => Some(1),
                            "false" | "0" => Some(0),
                            _ => None,
                        },
                        5 => None,
                        _ => unreachable!(),
                    };
                    if property.kind == 5 {
                        property.value = Value::Str(text);
                    } else if let Some(n) = number {
                        property.value = Value::Int(n);
                        match property.binding {
                            Some(Binding::Guest(owner, pointer)) => {
                                if owner != self.thread.thread_id() {
                                    return Err(VmError::Runtime(
                                        "native inspector edit delivered to a foreign thread"
                                            .into(),
                                    ));
                                }
                                self.write_int(pointer, 2, n as u32)?;
                            }
                            Some(Binding::Object(object, key)) => {
                                // ABCD0 checks liveness before invoking the setter.
                                if api.inspect_native_display_object(object).is_ok() {
                                    api.edit_native_display_object(object, key, n)?;
                                }
                            }
                            None => {}
                        }
                    } else {
                        continue;
                    }
                    // AD8E0 writes numeric/bool bindings, but not string bindings.
                    inspector.event(4, ((tab as u32) << 16 | row as u32 & 65535) as i32)?;
                }
                NativeInspectorEvent::Close => {} // WM_CLOSE has no queue record.
                _ => continue,
            }
        }
        inspector.snapshot();
        api.publish_native_inspector(&inspector.view)?;
        Ok(())
    }
    fn execute_native_inspector<A: SysApi>(
        &mut self,
        api: &mut A,
        id: u16,
        state: &mut NativeInspectors,
    ) -> VmResult<Value> {
        let result = match id {
            0x60 => {
                let right = self.pop_int()?;
                let left = self.pop_int()?;
                let y = self.pop_int()?;
                let x = self.pop_int()?;
                let description = self.pop_string_lossy()?;
                let title = self.pop_string_lossy()?;
                let output = self.pop_ptr()?;
                if state.entries.len() >= 16 {
                    return Err(VmError::Runtime("native inspector window limit".into()));
                }
                let next = state.next.wrapping_add(1);
                let handle = next | 0xf8000001;
                if state.entries.contains_key(&handle) {
                    return Err(VmError::Runtime(
                        "native inspector tagged handle collision".into(),
                    ));
                }
                let view = NativeInspectorView {
                    handle,
                    title,
                    description,
                    position: [x, y],
                    columns: [left, right],
                    selected_tab: 0,
                    tabs: Vec::new(),
                    buttons: Vec::new(),
                };
                self.resolve_write_range(output, 4)?;
                if !api.publish_native_inspector(&view)? {
                    -1
                } else {
                    state.next = next;
                    state.entries.insert(
                        handle,
                        Inspector {
                            view,
                            properties: Vec::new(),
                            events: VecDeque::new(),
                        },
                    );
                    self.write_int(output, 2, handle)?;
                    0
                }
            }
            0x61 => {
                let h = self.pop_int()? as u32;
                if state.entries.remove(&h).is_some() {
                    api.close_native_inspector(h)?;
                    0
                } else {
                    1
                }
            }
            0x62 => {
                let y = self.pop_int()?;
                let x = self.pop_int()?;
                let h = self.pop_int()? as u32;
                if let Some(i) = state.entries.get_mut(&h) {
                    i.view.position = [x, y];
                    api.publish_native_inspector(&i.view)?;
                    0
                } else {
                    1
                }
            }
            0x63 => {
                let h = self.pop_int()? as u32;
                let output = self.pop_ptr()?;
                if let Some(i) = state.entries.get(&h) {
                    self.resolve_write_range(output, 8)?;
                    self.write_int(output, 2, i.view.position[0] as u32)?;
                    self.write_int(output + 4, 2, i.view.position[1] as u32)?;
                    0
                } else {
                    1
                }
            }
            0x66 => {
                let h = self.pop_int()? as u32;
                // ACD60 intentionally succeeds for a missing handle; zero updates all.
                for (&handle, i) in &mut state.entries {
                    if h == 0 || h == handle {
                        self.update_inspector(api, i)?;
                    }
                }
                0
            }
            0x67 => {
                let h = self.pop_int()? as u32;
                let output = self.pop_ptr()?;
                if let Some(i) = state.entries.get_mut(&h) {
                    self.update_inspector(api, i)?;
                    if let Some(event) = i.events.front().copied() {
                        self.resolve_write_range(output, 16)?;
                        for (n, v) in event.into_iter().enumerate() {
                            self.write_int(output + 4 * n as u32, 2, v as u32)?;
                        }
                        i.events.pop_front();
                        0
                    } else {
                        6
                    }
                } else {
                    1
                }
            }
            0x68 | 0x6a => {
                let text = self.pop_string_lossy()?;
                let h = self.pop_int()? as u32;
                let output = self.pop_ptr()?;
                if let Some(i) = state.entries.get_mut(&h) {
                    let index = if id == 0x68 {
                        i.view.tabs.len()
                    } else {
                        i.view.buttons.len()
                    };
                    if id == 0x6a && index >= 3 {
                        5
                    } else {
                        if output != 0 {
                            self.resolve_write_range(output, 4)?;
                        }
                        if id == 0x68 {
                            if index >= 256 {
                                return Err(VmError::Runtime("native inspector tab limit".into()));
                            }
                            i.view.tabs.push(NativeInspectorTab {
                                title: text,
                                rows: Vec::new(),
                            });
                            i.properties.push(Vec::new());
                        } else {
                            i.view.buttons.push(text);
                        }
                        api.publish_native_inspector(&i.view)?;
                        if output != 0 {
                            self.write_int(output, 2, index as u32)?;
                        }
                        0
                    }
                } else if id == 0x68 {
                    -1
                } else {
                    1
                }
            }
            0x6c | 0x6d => {
                let editable = if id == 0x6d {
                    self.pop_int()? != 0
                } else {
                    false
                };
                let value = self.pop_value()?;
                let kind = self.pop_int()?;
                let name = self.pop_string_lossy()?;
                let tab = self.pop_int()? as u32;
                let h = self.pop_int()? as u32;
                let output = self.pop_ptr()?;
                if let Some(i) = state.entries.get_mut(&h) {
                    if let Some(properties) = i.properties.get_mut(tab as usize) {
                        if !(0..=5).contains(&kind) {
                            4
                        } else {
                            if properties.len() >= 4096 {
                                return Err(VmError::Runtime(
                                    "native inspector property limit".into(),
                                ));
                            }
                            let binding = (id == 0x6d).then_some(Binding::Guest(
                                self.thread.thread_id(),
                                value.as_i32() as u32,
                            ));
                            let value = if let Some(Binding::Guest(_, ptr)) = binding {
                                if kind == 5 {
                                    Value::Str(self.read_c_string(ptr)?)
                                } else {
                                    Value::Int(self.read_int(ptr, 2)? as i32)
                                }
                            } else if kind == 5 {
                                Value::Str(self.value_as_string_lossy(value)?)
                            } else {
                                Value::Int(value.as_i32())
                            };
                            if output != 0 {
                                self.resolve_write_range(output, 4)?;
                            }
                            let index = properties.len();
                            properties.push(Property {
                                name,
                                kind,
                                value,
                                binding,
                                editable,
                            });
                            i.snapshot();
                            api.publish_native_inspector(&i.view)?;
                            if output != 0 {
                                self.write_int(output, 2, index as u32)?;
                            }
                            0
                        }
                    } else {
                        2
                    }
                } else {
                    1
                }
            }
            0x6f => {
                let index = self.pop_int()? as u32;
                let tab = self.pop_int()? as u32;
                let h = self.pop_int()? as u32;
                let kind_output = self.pop_ptr()?;
                let output = self.pop_ptr()?;
                if let Some(i) = state.entries.get(&h) {
                    if let Some(properties) = i.properties.get(tab as usize) {
                        if let Some(p) = properties.get(index as usize) {
                            if p.kind == 5 {
                                self.write_c_string(
                                    output,
                                    &self.value_as_string_lossy(p.value.clone())?,
                                )?;
                            } else {
                                self.write_int(output, 2, p.value.as_i32() as u32)?;
                            }
                            if kind_output != 0 {
                                self.write_int(kind_output, 2, p.kind as u32)?;
                            }
                            0
                        } else {
                            3
                        }
                    } else {
                        2
                    }
                } else {
                    1
                }
            }
            _ => unreachable!(),
        };
        Ok(Value::Int(result))
    }
}

#[cfg(test)]
mod tests {
    use super::*;
    #[derive(Default)]
    struct Host {
        view: Option<NativeInspectorView>,
        events: Vec<NativeInspectorEvent>,
    }
    impl SysApi for Host {
        fn call_sys(&mut self, _: &mut crate::NativeCallFrame) -> VmResult<Value> {
            unreachable!()
        }
        fn publish_native_inspector(&mut self, view: &NativeInspectorView) -> VmResult<bool> {
            self.view = Some(view.clone());
            Ok(true)
        }
        fn close_native_inspector(&mut self, _: u32) -> VmResult<()> {
            self.view = None;
            Ok(())
        }
        fn take_native_inspector_events(&mut self, _: u32) -> VmResult<Vec<NativeInspectorEvent>> {
            Ok(std::mem::take(&mut self.events))
        }
    }
    #[test]
    fn version_inspector_binding_edits_and_event_delivery_use_guest_memory() {
        fn call(vm: &mut Vm, host: &mut Host, id: u16, args: Vec<Value>) -> i32 {
            vm.stack = args;
            vm.try_native_inspector(host, id).unwrap().unwrap().as_i32()
        }
        let mut vm = Vm::new();
        vm.set_native_abi_profile(NativeAbiProfile::ModernFused);
        let mut host = Host::default();
        let int = Value::Int;
        assert_eq!(
            call(
                &mut vm,
                &mut host,
                0x60,
                vec![
                    Value::Ptr(256),
                    Value::Str("Inspector".into()),
                    Value::Str("Properties".into()),
                    int(20),
                    int(30),
                    int(200),
                    int(240)
                ]
            ),
            0
        );
        let h = vm.read_int(256, 2).unwrap() as i32;
        assert_eq!(
            call(
                &mut vm,
                &mut host,
                0x68,
                vec![Value::Ptr(260), int(h), Value::Str("Main".into())]
            ),
            0
        );
        vm.write_int(512, 2, 7).unwrap();
        assert_eq!(
            call(
                &mut vm,
                &mut host,
                0x6d,
                vec![
                    Value::Ptr(264),
                    int(h),
                    int(0),
                    Value::Str("Counter".into()),
                    int(0),
                    Value::Ptr(512),
                    int(1)
                ]
            ),
            0
        );
        host.events.push(NativeInspectorEvent::Edit {
            tab: 0,
            row: 0,
            text: "19".into(),
        });
        assert_eq!(
            call(&mut vm, &mut host, 0x67, vec![Value::Ptr(600), int(h)]),
            0
        );
        assert_eq!(vm.read_int(512, 2).unwrap(), 19);
        assert_eq!(vm.read_int(600, 2).unwrap(), 4);
        assert_eq!(vm.read_int(604, 2).unwrap(), 0);
        assert_eq!(vm.read_int(608, 2).unwrap(), 0);
        assert_eq!(vm.read_int(612, 2).unwrap(), 0);
        assert_eq!(
            call(&mut vm, &mut host, 0x67, vec![Value::Ptr(600), int(h)]),
            6
        );
        vm.write_int(512, 2, 23).unwrap();
        assert_eq!(call(&mut vm, &mut host, 0x66, vec![int(h)]), 0);
        assert_eq!(host.view.as_ref().unwrap().tabs[0].rows[0].value, "23");
        assert_eq!(
            call(
                &mut vm,
                &mut host,
                0x6f,
                vec![Value::Ptr(620), Value::Ptr(624), int(h), int(0), int(0)]
            ),
            0
        );
        assert_eq!(vm.read_int(620, 2).unwrap(), 23);
        assert_eq!(vm.read_int(624, 2).unwrap(), 0);
        assert_eq!(call(&mut vm, &mut host, 0x61, vec![int(h)]), 0);
        assert!(host.view.is_none());
        assert_eq!(
            call(&mut vm, &mut host, 0x67, vec![Value::Ptr(600), int(h)]),
            1
        );
    }
}

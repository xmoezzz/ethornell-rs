//! 80:EC/ED/EE, 46C8E0..46CA40 / 1400B7630..B7700. The VM owns
//! native logical handles; a host supplies portable, owned extension modules.
//! Opening a guest DLL is never implicit permission to execute host code.
use crate::{SysApi, Value, Vm, VmResult};
use std::collections::BTreeMap;

#[derive(Debug, Clone, PartialEq, Eq)]
pub enum NativeExtensionArgument {
    Integer(i32),
    Text(String),
}

/// Implemented by Rust extension modules. Dropping a module releases its host
/// resources, both on 80:ED and when the owning VM session is destroyed.
pub trait NativeExtension: std::fmt::Debug + Send {
    fn call(
        &mut self,
        command: i32,
        first: &NativeExtensionArgument,
        second: &NativeExtensionArgument,
    ) -> VmResult<i64>;
}

#[derive(Debug, Default)]
pub(crate) struct NativeExtensions {
    next: u32,
    modules: BTreeMap<u32, Box<dyn NativeExtension>>,
}
impl Vm {
    pub(super) fn execute_native_extension<A: SysApi>(
        &mut self,
        api: &mut A,
        id: u16,
    ) -> VmResult<Value> {
        let result = match id {
            0xec => {
                let path = self.pop_string_lossy()?;
                let output = self.pop_ptr()?;
                match api.load_native_extension(&path)? {
                    Err(status) => status,
                    Ok(module) => {
                        self.resolve_write_range(output, 4)?;
                        let handle = {
                            let mut state = self
                                .system81_shared
                                .lock()
                                .expect("system81 state poisoned");
                            state.extensions.next = state.extensions.next.wrapping_add(1);
                            let handle = state.extensions.next;
                            if state.extensions.modules.contains_key(&handle) {
                                return Err(crate::VmError::Runtime(
                                    "native extension handle space exhausted".into(),
                                ));
                            }
                            state.extensions.modules.insert(handle, module);
                            handle
                        };
                        self.write_int(output, 2, handle)?;
                        0
                    }
                }
            }
            0xed => {
                let handle = self.pop_int()? as u32;
                if self
                    .system81_shared
                    .lock()
                    .expect("system81 state poisoned")
                    .extensions
                    .modules
                    .remove(&handle)
                    .is_some()
                {
                    0
                } else {
                    3
                }
            }
            0xee => {
                let flags = self.pop_int()? as u32;
                let second = if flags >> 16 != 0 {
                    NativeExtensionArgument::Text(self.pop_string_lossy()?)
                } else {
                    NativeExtensionArgument::Integer(self.pop_int()?)
                };
                let first = if flags as u16 != 0 {
                    NativeExtensionArgument::Text(self.pop_string_lossy()?)
                } else {
                    NativeExtensionArgument::Integer(self.pop_int()?)
                };
                let command = self.pop_int()?;
                let handle = self.pop_int()? as u32;
                let output = self.pop_ptr()?;
                let value = {
                    let mut state = self
                        .system81_shared
                        .lock()
                        .expect("system81 state poisoned");
                    state
                        .extensions
                        .modules
                        .get_mut(&handle)
                        .map(|m| m.call(command, &first, &second))
                        .transpose()?
                };
                if let Some(value) = value {
                    self.write_int(output, 2, value as u32)?;
                    if value == 0 { 0 } else { 4 }
                } else {
                    3
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
    #[derive(Debug)]
    struct Module(std::sync::Arc<std::sync::atomic::AtomicUsize>);
    impl Drop for Module {
        fn drop(&mut self) {
            self.0.fetch_add(1, std::sync::atomic::Ordering::Relaxed);
        }
    }
    impl NativeExtension for Module {
        fn call(
            &mut self,
            command: i32,
            first: &NativeExtensionArgument,
            second: &NativeExtensionArgument,
        ) -> VmResult<i64> {
            assert_eq!(command, 5);
            assert_eq!(first, &NativeExtensionArgument::Text("name".into()));
            assert_eq!(second, &NativeExtensionArgument::Integer(-3));
            Ok(42)
        }
    }
    struct Host(std::sync::Arc<std::sync::atomic::AtomicUsize>);
    impl SysApi for Host {
        fn call_sys(&mut self, _: &mut crate::NativeCallFrame) -> VmResult<Value> {
            unreachable!()
        }
        fn load_native_extension(
            &mut self,
            _: &str,
        ) -> VmResult<Result<Box<dyn NativeExtension>, i32>> {
            Ok(Ok(Box::new(Module(self.0.clone()))))
        }
    }
    #[test]
    fn version_extension_parameters_results_and_owned_module_lifetime() {
        let dropped = std::sync::Arc::new(std::sync::atomic::AtomicUsize::new(0));
        let mut host = Host(dropped.clone());
        let mut vm = Vm::new();
        vm.stack = vec![Value::Ptr(256), Value::Str("portable".into())];
        assert_eq!(
            vm.execute_native_extension(&mut host, 0xec).unwrap(),
            Value::Int(0)
        );
        let handle = vm.read_int(256, 2).unwrap() as i32;
        vm.stack = vec![
            Value::Ptr(260),
            Value::Int(handle),
            Value::Int(5),
            Value::Str("name".into()),
            Value::Int(-3),
            Value::Int(1),
        ];
        assert_eq!(
            vm.execute_native_extension(&mut host, 0xee).unwrap(),
            Value::Int(4)
        );
        assert_eq!(vm.read_int(260, 2).unwrap(), 42);
        vm.stack = vec![Value::Int(handle)];
        assert_eq!(
            vm.execute_native_extension(&mut host, 0xed).unwrap(),
            Value::Int(0)
        );
        assert_eq!(dropped.load(std::sync::atomic::Ordering::Relaxed), 1);
        vm.stack = vec![Value::Int(handle)];
        assert_eq!(
            vm.execute_native_extension(&mut host, 0xed).unwrap(),
            Value::Int(3)
        );
        vm.stack = vec![Value::Ptr(256), Value::Str("portable".into())];
        vm.execute_native_extension(&mut host, 0xec).unwrap();
        drop(vm);
        assert_eq!(dropped.load(std::sync::atomic::Ordering::Relaxed), 2);
    }
}

//! Nonblocking host continuations. These are portable VM types, not C ABI
//! layouts. The embedding host owns request IDs, UI dispatch and policy.
use crate::{NativeOpcode, SysApi, Value, Vm, VmError, VmResult};
use ethornell_script::calls::{known_call_arg_count, known_call_stack_output_count};

#[derive(Debug, Clone, PartialEq)]
pub enum HostRequest {
    /// Arguments in BP push order (the last element is popped first). Strings
    /// are normalized using the existing native/system encoding rules. Native
    /// pointer arguments remain guest u32 values, never host pointers.
    Native {
        opcode: NativeOpcode,
        args: Vec<Value>,
    },
    Confirmation {
        text: String,
        default_yes: bool,
    },
    Message {
        text: String,
    },
    /// A UserB0 input dialog. Answer with `Vm::complete_user_dialog`; the
    /// VM writes the accepted text itself and pushes 1 or 0.
    UserDialog(crate::UserDialogRequest),
}

#[cfg(test)]
mod tests {
    use super::*;
    use crate::{GraphApi, NativeCallFrame, SoundApi, VmRunOptions, VmStopReason};
    use ethornell_script::{BpInstruction, BpOpcode, BpProgram};

    #[derive(Default)]
    struct Host {
        requests: Vec<HostRequest>,
    }
    impl SysApi for Host {
        fn call_sys(&mut self, _: &mut NativeCallFrame) -> VmResult<Value> {
            panic!("synchronous fallback")
        }
        fn host_requests_enabled(&self) -> bool {
            true
        }
        fn begin_host_request(&mut self, request: &HostRequest) -> VmResult<Option<u64>> {
            self.requests.push(request.clone());
            Ok(Some(self.requests.len() as u64))
        }
    }
    impl GraphApi for Host {
        fn call_graph(&mut self, _: &mut NativeCallFrame) -> VmResult<Value> {
            Ok(Value::None)
        }
    }
    impl SoundApi for Host {
        fn call_sound(&mut self, _: &mut NativeCallFrame) -> VmResult<Value> {
            Ok(Value::None)
        }
    }
    fn program(code: u8, name: &'static str, id: u8) -> BpProgram {
        BpProgram {
            script_name: Some(format!("test-{name}")),
            functions: std::sync::Arc::new(vec![]),
            strings: std::sync::Arc::new(vec![]),
            instructions: std::sync::Arc::new(vec![BpInstruction {
                offset: 8,
                opcode: BpOpcode::Known { code, name },
                operands: vec![].into(),
                known_call: None,
                raw: vec![code, id].into(),
                warning: None,
            }]),
            labels: Default::default(),
            warnings: vec![],
            module_size: 0,
            code: std::sync::Arc::new(Vec::new()),
        }
    }

    #[test]
    fn confirmation_suspends_without_default_result_and_settles_once() {
        let mut vm = Vm::new();
        vm.start(&program(0x78, "confirm", 0));
        vm.stack = vec![Value::Str("question".into()), Value::Int(0)];
        let mut host = Host::default();
        let options = VmRunOptions::default();
        assert_eq!(
            vm.run_loaded(&mut host, &options).stop_reason,
            VmStopReason::WaitingForHost
        );
        assert!(vm.stack.is_empty());
        assert_eq!(vm.pc, 1);
        assert_eq!(
            host.requests,
            vec![HostRequest::Confirmation {
                text: "question".into(),
                default_yes: false
            }]
        );
        assert_eq!(vm.run_loaded(&mut host, &options).steps, 0);
        assert!(
            vm.complete_host_request(1, HostResponse::default())
                .is_err()
        );
        assert_eq!(vm.pending_host_request_ids(), vec![1]);
        let response = HostResponse {
            outputs: vec![Value::Int(0)],
            writes: vec![],
        };
        vm.complete_host_request(1, response.clone()).unwrap();
        assert!(vm.stack.is_empty());
        assert!(vm.complete_host_request(1, response).is_err());
        assert_eq!(
            vm.run_loaded(&mut host, &options).stop_reason,
            VmStopReason::Completed
        );
        assert_eq!(vm.stack, vec![Value::Int(0)]);
        vm.run_loaded(&mut host, &options);
        assert_eq!(vm.stack, vec![Value::Int(0)]);
    }
    #[derive(Default)]
    struct DeclineHost(bool);
    impl SysApi for DeclineHost {
        fn host_requests_enabled(&self)->bool {self.0}
        fn begin_host_request(&mut self,_:&HostRequest)->VmResult<Option<u64>> {Ok(None)}
        fn call_sys(&mut self,_:&mut NativeCallFrame)->VmResult<Value> {Ok(Value::None)}
    }
    impl GraphApi for DeclineHost {fn call_graph(&mut self,_:&mut NativeCallFrame)->VmResult<Value>{Ok(Value::None)}}
    impl SoundApi for DeclineHost {fn call_sound(&mut self,_:&mut NativeCallFrame)->VmResult<Value>{Ok(Value::None)}}
    #[test]
    fn confirmed_sentence_profile_full_dispatch_preserves_fallback_and_utf8_bytes() {
        for enabled in [false,true] {for strict in [false,true] {
            let mut vm=Vm::new();vm.set_native_abi_profile(crate::NativeAbiProfile::Amachoco);
            let mut host=DeclineHost(enabled);let options=VmRunOptions {fail_on_stub:strict,..Default::default()};
            vm.start(&program(0x81,"sys2",0xDA));
            vm.stack=vec![Value::Int(99),Value::Ptr(0x3100),Value::Str("key".into()),Value::Str("原文".into())];
            assert_eq!(vm.run_loaded(&mut host,&options).stop_reason,VmStopReason::Completed);
            assert_eq!(vm.stack,vec![Value::Int(99),Value::Int(0)]);
            vm.start(&program(0x81,"sys2",0xD8));
            vm.stack=vec![Value::Int(99),Value::Ptr(0x3000),Value::Int(-1)];
            assert_eq!(vm.run_loaded(&mut host,&options).stop_reason,VmStopReason::Completed);
            assert_eq!(vm.stack,vec![Value::Int(99),Value::Int(0)]);
            vm.start(&program(0x81,"sys2",0x21));
            vm.stack=vec![Value::Int(99),Value::Ptr(0x3100),Value::Str("原文".into())];
            assert_eq!(vm.run_loaded(&mut host,&options).stop_reason,VmStopReason::Completed);
            assert_eq!(vm.stack,vec![Value::Int(99)]);
            assert_eq!(vm.read_c_string_bytes(0x3100).unwrap(),"原文".as_bytes());
            assert_eq!(vm.shadow_string_at(0x3100).as_deref(),Some("原文"));
            let child=Value::Program(std::sync::Arc::new(program(0x17,"ret",0)));
            vm.start_async_program_with_args(child.clone(),vec![],false).unwrap();
            assert_eq!(vm.async_tasks[0].vm.native_abi_profile(),crate::NativeAbiProfile::Amachoco);
            vm.async_tasks[0].vm.start_async_program_with_args(child,vec![],false).unwrap();
            assert_eq!(vm.async_tasks[0].vm.async_tasks[0].vm.native_abi_profile(),crate::NativeAbiProfile::Amachoco);
        }}
        let mut old=Vm::new();
        assert_eq!(old.native_abi_profile(),crate::NativeAbiProfile::Tayutama);
        assert!(ethornell_script::native_abi::lookup(0x81,0x21).is_none());
        assert!(ethornell_script::native_abi::lookup(0x81,0xDA).is_none());
        old.stack=vec![Value::Ptr(0x3100),Value::Str("old".into())];
        assert_eq!(old.try_builtin_sys_with_api(&mut DeclineHost(false),0x81,0x21).unwrap(),None);
        assert_eq!(old.stack.len(),2);
    }
    #[test]
    fn sentence_fallback_zero_branches_to_raw_cp932_copy_in_one_program() {
        // DA leaves zero; jc(eq-zero) must execute the fallback21 and must
        // not fall through ret. Both actual source and destination are Ptrs.
        let raw=[0x81,0xDA,0x00,8,0x15,1,0x17,0,0x02,0,0x31,0,0,0x02,0,0x30,0,0,0x81,0x21,0x17];
        let mut p=ethornell_script::parse_bp_program(Some("original-profile-fallback".into()),&raw);
        // Parser's conservative headerless code-end probe stops at the first
        // ret; explicitly model both valid branch targets for this unit BP.
        let suffix=ethornell_script::parse_bp_program(None,&raw[8..]);
        std::sync::Arc::make_mut(&mut p.instructions).extend(suffix.instructions.iter().cloned().map(|mut i|{i.offset+=8;i}));
        p.labels=std::sync::Arc::new(p.instructions.iter().enumerate().map(|(n,i)|(i.offset as u32,n)).collect());
        let mut root=Vm::new();root.set_native_abi_profile(crate::NativeAbiProfile::Amachoco);
        root.start_async_program_with_args(Value::Program(std::sync::Arc::new(p.clone())),vec![],false).unwrap();
        root.async_tasks[0].vm.start_async_program_with_args(Value::Program(std::sync::Arc::new(p.clone())),vec![],false).unwrap();
        let mut child=root.async_tasks.remove(0);let mut grandchild=child.vm.async_tasks.remove(0);
        for vm in [&mut root,child.vm.as_mut(),grandchild.vm.as_mut()] {
            vm.start(&p);
            write_encoded(vm,0x3000,"原文",encoding_rs::SHIFT_JIS);
            vm.stack=vec![Value::Int(99),Value::Ptr(0x3100),Value::Str("key".into()),Value::Ptr(0x3000)];
            let report=vm.run_loaded(&mut DeclineHost(true),&VmRunOptions {fail_on_stub:true,collect_diagnostics:true,..Default::default()});
            assert_eq!(report.stop_reason,VmStopReason::Completed,"{:?}",report.recent_trace);
            assert_eq!(vm.stack,vec![Value::Int(99)]);
            assert_eq!(vm.read_c_string_bytes(0x3100).unwrap(),"原文".as_bytes());
            assert_eq!(vm.shadow_string_at(0x3100).as_deref(),Some("原文"));
        }
    }
    #[test]
    fn profile_continuation_snapshot_and_response_use_effective_contract() {
        for (id,args,outputs) in [
            (0xDA,vec![Value::Ptr(0x3100),Value::Str("key".into()),Value::Str("text".into())],1),
            (0x21,vec![Value::Ptr(0x3100),Value::Str("text".into())],0),
        ] {
            let mut vm=Vm::new();vm.set_native_abi_profile(crate::NativeAbiProfile::Amachoco);
            vm.start(&program(0x81,"sys2",id));
            vm.stack=vec![Value::Int(99)];vm.stack.extend(args.clone());let mut host=Host::default();
            assert_eq!(vm.run_loaded(&mut host,&VmRunOptions {fail_on_stub:true,..Default::default()}).stop_reason,VmStopReason::WaitingForHost);
            assert_eq!(vm.stack,vec![Value::Int(99)]);
            assert!(matches!(&host.requests[0],HostRequest::Native {args:actual,..} if *actual==args));
            let response=HostResponse {outputs:if outputs==1 {vec![Value::Int(0)]} else {vec![]},writes:vec![]};
            assert!(vm.complete_host_request(1,HostResponse {outputs:if outputs==0 {vec![Value::Int(0)]} else {vec![]},writes:vec![]}).is_err());
            vm.complete_host_request(1,response).unwrap();
            assert_eq!(vm.run_loaded(&mut host,&VmRunOptions::default()).stop_reason,VmStopReason::Completed);
            assert_eq!(vm.stack.len(),1+outputs);
        }
    }
    #[test]
    fn sentence_table_registration_lookup_clear_and_child_access_use_full_dispatch() {
        let mut vm=Vm::new();vm.set_native_abi_profile(crate::NativeAbiProfile::Amachoco);
        let mut bytes=1u32.to_le_bytes().to_vec();bytes.extend_from_slice(b"scene\0");bytes.extend_from_slice(&1u32.to_le_bytes());bytes.extend_from_slice(b"000000\0");bytes.extend_from_slice("原创对白".as_bytes());bytes.push(0);
        let range=vm.resolve_write_range(0x3000,bytes.len()).unwrap();vm.memory[range].copy_from_slice(&bytes);
        let options=VmRunOptions {fail_on_stub:true,..Default::default()};let mut host=DeclineHost(true);
        vm.start(&program(0x81,"sys2",0xD8));vm.stack=vec![Value::Int(99),Value::Ptr(0x3000),Value::Int(bytes.len() as i32)];
        assert_eq!(vm.run_loaded(&mut host,&options).stop_reason,VmStopReason::Completed);assert_eq!(vm.stack,vec![Value::Int(99),Value::Int(1)]);
        // Invalid replacement must not erase a registered table.
        vm.start(&program(0x81,"sys2",0xD8));vm.stack=vec![Value::Ptr(0x3000),Value::Int(3)];vm.run_loaded(&mut host,&options);assert_eq!(vm.stack,vec![Value::Int(0)]);
        vm.start_async_program_with_args(Value::Program(std::sync::Arc::new(program(0x81,"sys2",0xDA))),vec![],false).unwrap();
        let mut child=vm.async_tasks.remove(0);
        for target in [&mut vm,child.vm.as_mut()] {
            target.start(&program(0x81,"sys2",0xDA));target.stack=vec![Value::Int(99),Value::Ptr(0x4000),Value::Str("scene".into()),Value::Str("000000".into())];
            assert_eq!(target.run_loaded(&mut host,&options).stop_reason,VmStopReason::Completed);assert_eq!(target.stack,vec![Value::Int(99),Value::Int("原创对白".len() as i32)]);
            assert_eq!(target.read_c_string_bytes(0x4000).unwrap(),"原创对白".as_bytes());assert_eq!(target.shadow_string_at(0x4000).as_deref(),Some("原创对白"));
            let zero_before=target.memory[..32].to_vec();
            target.start(&program(0x81,"sys2",0xDA));target.stack=vec![Value::Ptr(0),Value::Str("scene".into()),Value::Str("000000".into())];
            assert_eq!(target.run_loaded(&mut host,&options).stop_reason,VmStopReason::Completed);
            assert_eq!(target.stack,vec![Value::Int("原创对白".len() as i32)]);
            assert_eq!(target.memory[..32],zero_before);
            target.start(&program(0x81,"sys2",0xDA));target.stack=vec![Value::Ptr(0x4000),Value::Str("scene".into()),Value::Str("missing".into())];
            target.run_loaded(&mut host,&options);assert_eq!(target.stack,vec![Value::Int(0)]);
            assert_eq!(target.read_c_string_bytes(0x4000).unwrap(),"原创对白".as_bytes());
        }
        vm.start(&program(0x81,"sys2",0xD8));vm.stack=vec![Value::Ptr(0),Value::Int(0)];vm.run_loaded(&mut host,&options);assert_eq!(vm.stack,vec![Value::Int(1)]);
        child.vm.start(&program(0x81,"sys2",0xDA));child.vm.stack=vec![Value::Ptr(0x4000),Value::Str("scene".into()),Value::Str("000000".into())];child.vm.run_loaded(&mut host,&options);assert_eq!(child.vm.stack,vec![Value::Int(0)]);
    }

    #[test]
    fn user_input_dialog_suspends_typed_and_validates_all_writes() {
        let mut vm = Vm::new();
        vm.start(&program(0xb0, "usr1", 0x84));
        vm.stack = vec![
            Value::Ptr(0x3000),
            Value::Str("caption".into()),
            Value::Str("initial".into()),
            Value::Int(255),
        ];
        let mut host = Host::default();
        assert_eq!(
            vm.run_loaded(&mut host, &VmRunOptions::default())
                .stop_reason,
            VmStopReason::WaitingForHost
        );
        assert_eq!(
            host.requests,
            [HostRequest::UserDialog(crate::UserDialogRequest::Input {
                title: "caption".into(),
                initial: "initial".into(),
                max_bytes: 255,
                numeric: false,
            })]
        );
        let before = vm.memory[0x3000];
        let response = HostResponse {
            outputs: vec![Value::Int(1)],
            writes: vec![
                HostMemoryWrite {
                    address: 0x3000,
                    bytes: vec![42],
                },
                HostMemoryWrite {
                    address: 0x2fff_ffff,
                    bytes: vec![7; 8],
                },
            ],
        };
        assert!(vm.complete_host_request(1, response).is_err());
        assert_eq!(vm.memory[0x3000], before);
        vm.complete_user_dialog(1, Some(crate::UserDialogResponse::Input("答え".into())))
            .unwrap();
        vm.run_loaded(&mut host, &VmRunOptions::default());
        assert_eq!(vm.read_c_string_bytes(0x3000).unwrap(), encoding_rs::SHIFT_JIS.encode("答え").0.to_vec());
        assert_eq!(vm.stack, vec![Value::Int(1)]);
    }

    #[test]
    fn user_dialog_cancel_writes_nothing_and_pushes_zero() {
        let mut vm = Vm::new();
        vm.start(&program(0xb0, "usr1", 0x84));
        vm.memory[0x3000] = 9;
        vm.stack = vec![
            Value::Ptr(0x3000),
            Value::Str("caption".into()),
            Value::Str(String::new()),
            Value::Int(-4),
        ];
        let mut host = Host::default();
        vm.run_loaded(&mut host, &VmRunOptions::default());
        assert!(matches!(
            &host.requests[0],
            HostRequest::UserDialog(crate::UserDialogRequest::Input { max_bytes: 4, numeric: true, .. })
        ));
        assert!(vm.complete_user_dialog(2, None).is_err());
        vm.complete_user_dialog(1, None).unwrap();
        vm.run_loaded(&mut host, &VmRunOptions::default());
        assert_eq!(vm.memory[0x3000], 9);
        assert_eq!(vm.stack, vec![Value::Int(0)]);
    }

    #[test]
    fn child_global_response_applies_after_shared_transfer_and_cancel_never_answers() {
        let mut root = Vm::new();
        let child = program(0x78, "confirm", 0);
        root.start_async_program_with_args(
            Value::Program(std::sync::Arc::new(child)),
            vec![],
            false,
        )
        .unwrap();
        root.async_tasks[0].vm.stack = vec![Value::Str("child".into()), Value::Int(1)];
        let mut host = Host::default();
        assert_eq!(
            root.run_loaded(&mut host, &VmRunOptions::default())
                .stop_reason,
            VmStopReason::WaitingForHost
        );
        root.complete_host_request(
            1,
            HostResponse {
                outputs: vec![Value::Int(0)],
                writes: vec![HostMemoryWrite {
                    address: 0x3000,
                    bytes: vec![42],
                }],
            },
        )
        .unwrap();
        root.run_loaded(&mut host, &VmRunOptions::default());
        assert_eq!(root.memory[0x3000], 42);

        root.start(&program(0x78, "confirm", 0));
        root.stack = vec![Value::Str("cancel".into()), Value::Int(1)];
        root.run_loaded(&mut host, &VmRunOptions::default());
        assert_eq!(root.cancel_host_requests(), vec![2]);
        assert!(root.stack.is_empty());
        assert!(root.halted);
        assert!(
            root.complete_host_request(
                2,
                HostResponse {
                    outputs: vec![Value::Int(1)],
                    writes: vec![]
                }
            )
            .is_err()
        );
    }

    #[test]
    fn root_child_grandchild_share_graph_encoding_for_each_graph_text_kind() {
        let mut root = Vm::new();
        root.set_graph_text_encoding(encoding_rs::GBK);
        let p = std::sync::Arc::new(program(0x78, "confirm", 0));
        root.start_async_program_with_args(Value::Program(p.clone()), vec![], false)
            .unwrap();
        root.async_tasks[0]
            .vm
            .start_async_program_with_args(Value::Program(p), vec![], false)
            .unwrap();
        let mut child = root.async_tasks.remove(0);
        let mut grandchild = child.vm.async_tasks.remove(0);
        for vm in [&mut root, child.vm.as_mut(), grandchild.vm.as_mut()] {
            materialize_test_memory(vm);
            assert_eq!(vm.graph_text_encoding, Some(encoding_rs::GBK));
            for (group, id, count, from_top) in [
                (0x92, 0x90, 15, 13),
                (0x92, 0x1c, 10, 6),
                (0x90, 0x90, 6, 4),
                (0x91, 0x9c, 14, 8),
            ] {
                vm.memory[0x3000..0x3005].copy_from_slice(b"\xeb\xca\xb0\xd7\0");
                vm.stack = vec![Value::Int(0); count];
                vm.stack[count - 1 - from_top] = Value::Ptr(0x3000);
                vm.normalize_graph_string_args(group, id).unwrap();
                assert_eq!(vm.stack[count - 1 - from_top], Value::Str("胧白".into()));
            }
        }
        child.vm.async_tasks.push(grandchild);
        root.async_tasks.push(child);
        assert_eq!(
            root.async_tasks[0].vm.graph_text_encoding,
            Some(encoding_rs::GBK)
        );
        root.set_graph_text_encoding(encoding_rs::SHIFT_JIS);
        assert_eq!(
            root.async_tasks[0].vm.graph_text_encoding,
            Some(encoding_rs::SHIFT_JIS)
        );
        assert_eq!(
            root.async_tasks[0].vm.async_tasks[0].vm.graph_text_encoding,
            Some(encoding_rs::SHIFT_JIS)
        );
    }

    #[test]
    fn child_strict_error_is_fatal_at_root_with_child_diagnostics() {
        let mut root = Vm::new();
        root.start_async_program_with_args(
            Value::Program(std::sync::Arc::new(program(0x80, "sys1", 0xff))),
            vec![],
            false,
        )
        .unwrap();
        let child_id = root.async_tasks[0].vm.thread.thread_id();
        let options = VmRunOptions {
            fail_on_stub: true,
            collect_diagnostics: true,
            ..Default::default()
        };
        let mut host = Host::default();
        let report = root.run_loaded(&mut host, &options);
        assert!(report.stop_reason.is_fatal());
        assert_eq!(report.thread.thread_id, child_id);
        assert!(!report.calls.is_empty());
        assert!(!root.async_tasks[0].completed);
        assert_eq!(root.take_async_reports().len(), 1);
        assert_eq!(
            root.run_loaded(&mut host, &options).stop_reason,
            report.stop_reason
        );
    }

    #[test]
    fn resource_measurements_count_private_vms_once_and_check_budget() {
        let mut root = Vm::new();
        let initial = root.resource_usage();
        root.start_async_program_with_args(
            Value::Program(std::sync::Arc::new(program(0x78, "confirm", 0))),
            vec![],
            false,
        )
        .unwrap();
        let usage = root.resource_usage();
        assert_eq!(usage.vm_count, 2);
        // An idle thread keeps compact code, not a second process buffer.
        assert_eq!(usage.memory_bytes, initial.memory_bytes);
        assert!(
            root.check_resource_budget(VmResourceBudget {
                max_vm_count: 1,
                max_memory_capacity_bytes: usize::MAX,
            })
            .is_err()
        );
    }

    #[derive(Default)]
    struct PolicyHost;
    impl SysApi for PolicyHost {
        fn call_sys(&mut self, _: &mut NativeCallFrame) -> VmResult<Value> {
            Ok(Value::None)
        }
        fn allow_direct_filesystem_fallback(&self) -> bool {
            false
        }
    }
    impl GraphApi for PolicyHost {
        fn call_graph(&mut self, _: &mut NativeCallFrame) -> VmResult<Value> {
            Ok(Value::None)
        }
    }
    impl SoundApi for PolicyHost {
        fn call_sound(&mut self, _: &mut NativeCallFrame) -> VmResult<Value> {
            Ok(Value::None)
        }
    }

    #[test]
    fn policy_denied_read_cannot_use_direct_filesystem_fallback() {
        let mut host = PolicyHost;
        let path = std::env::current_exe()
            .unwrap()
            .to_string_lossy()
            .into_owned();
        assert!(std::fs::metadata(&path).is_ok());
        let mut vm = Vm::new();
        vm.start(&program(0x81, "sys2", 0x35));
        vm.stack = vec![Value::Int(0), Value::Str(path)];
        assert_eq!(
            vm.run_loaded(&mut host, &VmRunOptions::default())
                .stop_reason,
            VmStopReason::Completed
        );
        assert_eq!(vm.stack, vec![Value::Int(0)]);
    }

    #[test]
    fn failed_save_is_ignored_like_the_target() {
        // sub_48A140 discards sub_4664F0's result: Sys80:78 has no output
        // and a failed write is not an error.
        let mut vm = Vm::new();
        vm.start(&program(0x80, "sys1", 0x78));
        vm.stack = vec![Value::Int(0), Value::Str("save".into())];
        let report = vm.run_loaded(&mut PolicyHost, &VmRunOptions::default());
        assert_eq!(report.stop_reason, VmStopReason::Completed);
        assert!(vm.stack.is_empty());
    }

    #[test]
    fn grandchild_fatal_and_disabled_diagnostics_propagate() {
        let mut root = Vm::new();
        root.start_async_program_with_args(
            Value::Program(std::sync::Arc::new(program(0x17, "ret", 0))),
            vec![],
            false,
        )
        .unwrap();
        root.async_tasks[0]
            .vm
            .start_async_program_with_args(
                Value::Program(std::sync::Arc::new(program(0x80, "sys1", 0xff))),
                vec![],
                false,
            )
            .unwrap();
        let id = root.async_tasks[0].vm.async_tasks[0].vm.thread.thread_id();
        let options = VmRunOptions {
            fail_on_stub: true,
            collect_diagnostics: false,
            ..Default::default()
        };
        let report = root.run_loaded(&mut PolicyHost, &options);
        assert!(report.stop_reason.is_fatal());
        assert_eq!(report.thread.thread_id, id);
        assert!(report.calls.is_empty());
        assert!(report.stubs.is_empty());
        assert!(report.recent_trace.is_empty());
    }

    // These snapshot fixtures inspect idle VMs directly, without scheduling
    // them. Materialize their private test backing before installing bytes.
    fn materialize_test_memory(vm: &mut Vm) {
        if vm.memory.is_empty() {
            vm.memory.resize(crate::HEAP_MEMORY_BASE, 0);
            if let Some(window) = vm.parked_local.take() {
                window.restore(&mut vm.memory[crate::LOCAL_MEMORY_BASE as usize..crate::HEAP_MEMORY_BASE]);
            }
        }
    }

    fn write_encoded(
        vm: &mut Vm,
        pointer: u32,
        text: &str,
        encoding: &'static encoding_rs::Encoding,
    ) {
        materialize_test_memory(vm);
        let (bytes, _, errors) = encoding.encode(text);
        assert!(!errors);
        let start = vm.memory_addr(pointer) as usize;
        vm.memory[start..start + bytes.len()].copy_from_slice(&bytes);
        vm.memory[start + bytes.len()] = 0;
    }

    #[test]
    fn raw_pointer_native_dialogs_decode_in_owning_root_or_child_under_both_graph_encodings() {
        let ptr = 0x0800_3000; // Per-thread local memory at the same guest address.
        for encoding in [encoding_rs::SHIFT_JIS, encoding_rs::GBK] {
            for (id, suffix) in [
                (0x80, vec![]),
                (0x81, vec![Value::Int(0)]),
                (0x82, vec![Value::Int(1), Value::Int(0)]),
            ] {
                let p = program(0xb0, "usr1", id);
                let mut root = Vm::new();
                root.set_graph_text_encoding(encoding);
                root.start(&p);
                root.start_async_program_with_args(
                    Value::Program(std::sync::Arc::new(p)),
                    vec![],
                    false,
                )
                .unwrap();
                write_encoded(&mut root, ptr, "親の質問", encoding_rs::SHIFT_JIS);
                write_encoded(
                    &mut root.async_tasks[0].vm,
                    ptr,
                    "子の質問",
                    encoding_rs::SHIFT_JIS,
                );
                let args = [vec![Value::Ptr(ptr)], suffix.clone()].concat();
                root.stack = args.clone();
                root.async_tasks[0].vm.stack = args;
                let mut host = Host::default();
                let options = VmRunOptions::default();
                assert_eq!(
                    root.run_loaded(&mut host, &options).stop_reason,
                    VmStopReason::WaitingForHost
                );
                assert_eq!(host.requests.len(), 2);
                for (request, text) in host.requests.iter().zip(["子の質問", "親の質問"]) {
                    assert_eq!(
                        request,
                        &HostRequest::Native {
                            opcode: NativeOpcode {
                                group: 0xb0,
                                id: u16::from(id)
                            },
                            args: [vec![Value::Str(text.into())], suffix.clone()].concat(),
                        }
                    );
                }
                assert!(root.stack.is_empty());
                assert!(root.async_tasks[0].vm.stack.is_empty());
                // Zero-output messages and one-output confirmations enforce
                // different contracts; neither supplies a default answer.
                let count = usize::from(id != 0x80);
                for request_id in [1, 2] {
                    assert!(
                        root.complete_host_request(
                            request_id,
                            HostResponse {
                                outputs: vec![Value::Int(0); 1 - count],
                                writes: vec![],
                            }
                        )
                        .is_err()
                    );
                    root.complete_host_request(
                        request_id,
                        HostResponse {
                            outputs: vec![Value::Int(0); count],
                            writes: vec![],
                        },
                    )
                    .unwrap();
                }
                root.run_loaded(&mut host, &options);
                assert_eq!(root.stack, vec![Value::Int(0); count]);
                assert_eq!(root.read_c_string(ptr).unwrap(), "親の質問");
            }
            // System80:3C native pop order is message, ignored context, file.
            // Interception must expose all three as strings in push order,
            // rather than entering the desktop Retry/Cancel loop.
            let p = program(0x80, "sys1", 0x3c);
            let mut root = Vm::new();
            root.set_graph_text_encoding(encoding);
            root.start(&p);
            root.start_async_program_with_args(
                Value::Program(std::sync::Arc::new(p)),
                vec![],
                false,
            )
            .unwrap();
            write_encoded(&mut root, ptr, "親の要求", encoding_rs::SHIFT_JIS);
            write_encoded(
                &mut root.async_tasks[0].vm,
                ptr,
                "子の要求",
                encoding_rs::SHIFT_JIS,
            );
            root.stack = vec![Value::Ptr(ptr); 3];
            root.async_tasks[0].vm.stack = vec![Value::Ptr(ptr); 3];
            let mut host = Host::default();
            assert_eq!(
                root.run_loaded(&mut host, &VmRunOptions::default())
                    .stop_reason,
                VmStopReason::WaitingForHost
            );
            assert_eq!(
                host.requests,
                vec![
                    HostRequest::Native {
                        opcode: NativeOpcode {
                            group: 0x80,
                            id: 0x3c
                        },
                        args: vec![Value::Str("子の要求".into()); 3]
                    },
                    HostRequest::Native {
                        opcode: NativeOpcode {
                            group: 0x80,
                            id: 0x3c
                        },
                        args: vec![Value::Str("親の要求".into()); 3]
                    },
                ]
            );
            assert!(root.stack.is_empty());
            assert!(root.async_tasks[0].vm.stack.is_empty());
        }
    }

    #[test]
    fn raw_pointer_host_snapshot_uses_graph_encoding_without_decoding_output_pointers() {
        let text_ptr = 0x0800_3000;
        let output_ptr = 0x0800_4000;
        for (encoding, text) in [
            (encoding_rs::SHIFT_JIS, "日本語"),
            (encoding_rs::GBK, "胧白"),
        ] {
            let mut root = Vm::new();
            root.set_graph_text_encoding(encoding);
            root.start_async_program_with_args(
                Value::Program(std::sync::Arc::new(program(0x17, "ret", 0))),
                vec![],
                false,
            )
            .unwrap();
            let mut child = root.async_tasks.remove(0);
            for vm in [&mut root, child.vm.as_mut()] {
                write_encoded(vm, text_ptr, text, encoding);
                // Graph92:1F is a graph-text selector, not a system string.
                let args = vm
                    .normalize_host_request_args(
                        NativeOpcode {
                            group: 0x92,
                            id: 0x1f,
                        },
                        vec![Value::Ptr(text_ptr)],
                    )
                    .unwrap();
                assert_eq!(args, vec![Value::Str(text.into())]);
                // User84 input strings remain SJIS even with graph GBK.
                write_encoded(vm, text_ptr, "入力", encoding_rs::SHIFT_JIS);
                let args = vm
                    .normalize_host_request_args(
                        NativeOpcode {
                            group: 0xb0,
                            id: 0x84,
                        },
                        vec![
                            Value::Ptr(output_ptr),
                            Value::Ptr(text_ptr),
                            Value::Ptr(text_ptr),
                            Value::Int(255),
                        ],
                    )
                    .unwrap();
                assert_eq!(
                    args,
                    vec![
                        Value::Ptr(output_ptr),
                        Value::Str("入力".into()),
                        Value::Str("入力".into()),
                        Value::Int(255)
                    ]
                );
            }
        }
    }

    #[test]
    fn native_snapshot_decodes_wrapped_operand_ring_and_preserves_memory_output_isolation() {
        let mut root = Vm::new();
        let mut p = program(0xb0, "usr1", 0x84);
        std::sync::Arc::make_mut(&mut p.instructions)
            .extend(program(0x80, "sys1", 0x5f).instructions.iter().cloned());
        root.start_async_program_with_args(Value::Program(std::sync::Arc::new(p)), vec![], false)
            .unwrap();
        let child = &mut root.async_tasks[0].vm;
        let ptr = 0x0800_3000;
        let out = 0x0800_4000;
        write_encoded(child, ptr, "確認", encoding_rs::SHIFT_JIS);
        child.operand_slots[crate::OPERAND_STACK_CAPACITY - 3] = Value::Ptr(out);
        child.operand_slots[crate::OPERAND_STACK_CAPACITY - 2] = Value::Ptr(ptr);
        child.operand_slots[crate::OPERAND_STACK_CAPACITY - 1] = Value::Ptr(ptr);
        child.stack = vec![Value::Int(255)];
        let mut host = Host::default();
        root.run_loaded(&mut host, &VmRunOptions::default());
        assert_eq!(
            host.requests,
            vec![HostRequest::UserDialog(crate::UserDialogRequest::Input {
                title: "確認".into(),
                initial: "確認".into(),
                max_bytes: 255,
                numeric: false,
            })]
        );
        let offset = root.memory_addr(out) as usize;
        root.memory[offset] = 7;
        root.complete_user_dialog(1, Some(crate::UserDialogResponse::Input("*".into())))
            .unwrap();
        // Accepted response is deferred; neither parent's bytes nor child's
        // local output are changed before its scheduler slice.
        assert_eq!(root.memory[offset], 7);
        assert_eq!(crate::async_program::local_byte(&root.async_tasks[0].vm, offset), 0);
        root.run_loaded(&mut host, &VmRunOptions::default());
        assert_eq!(root.memory[offset], 7);
        assert_eq!(crate::async_program::local_byte(&root.async_tasks[0].vm, offset), b'*');
        assert!(
            root.complete_host_request(1, HostResponse::default())
                .is_err()
        );
    }

    #[test]
    fn system_file_dialog_snapshot_decodes_only_input_strings() {
        let mut vm = Vm::new();
        write_encoded(&mut vm, 0x3000, "選択", encoding_rs::SHIFT_JIS);
        let original = vec![
            Value::Ptr(0x4000),
            Value::Ptr(0x3000),
            Value::Ptr(0x3000),
            Value::Ptr(0x3000),
            Value::Ptr(0x3000),
            Value::Int(1),
        ];
        vm.stack = vec![Value::Int(123)];
        let args = vm
            .normalize_host_request_args(
                NativeOpcode {
                    group: 0x80,
                    id: 0x3b,
                },
                original,
            )
            .unwrap();
        assert_eq!(
            args,
            vec![
                Value::Ptr(0x4000),
                Value::Str("選択".into()),
                Value::Str("選択".into()),
                Value::Str("選択".into()),
                Value::Str("選択".into()),
                Value::Int(1)
            ]
        );
        assert_eq!(vm.stack, vec![Value::Int(123)]);
    }

    #[test]
    fn declined_or_unknown_native_request_preserves_stack_ring_and_query_behavior() {
        struct DecliningHost;
        impl SysApi for DecliningHost {
            fn call_sys(&mut self, _: &mut NativeCallFrame) -> VmResult<Value> {
                Ok(Value::None)
            }
            fn host_requests_enabled(&self) -> bool {
                true
            }
            fn begin_host_request(&mut self, _: &HostRequest) -> VmResult<Option<u64>> {
                Ok(None)
            }
        }
        impl GraphApi for DecliningHost {
            fn call_graph(&mut self, _: &mut NativeCallFrame) -> VmResult<Value> {
                Ok(Value::None)
            }
        }
        impl SoundApi for DecliningHost {
            fn call_sound(&mut self, _: &mut NativeCallFrame) -> VmResult<Value> {
                Ok(Value::None)
            }
        }
        let mut vm = Vm::new();
        write_encoded(&mut vm, 0x3000, "確認", encoding_rs::SHIFT_JIS);
        vm.stack = vec![Value::Ptr(0x3000), Value::Int(1)];
        vm.sync_operand_slots();
        let stack = vm.stack.clone();
        let slots = vm.operand_slots.clone();
        let synced = vm.operand_slots_synced_len;
        assert!(
            !vm.intercept_native_host_request(
                &mut DecliningHost,
                NativeOpcode {
                    group: 0xb0,
                    id: 0x81
                }
            )
            .unwrap()
        );
        assert_eq!(vm.stack, stack);
        assert_eq!(vm.operand_slots, slots);
        assert_eq!(vm.operand_slots_synced_len, synced);
        assert!(
            !vm.intercept_native_host_request(
                &mut DecliningHost,
                NativeOpcode {
                    group: 0x80,
                    id: 0xffff
                }
            )
            .unwrap()
        );
        vm.start(&program(0x80, "sys1", 0xfd));
        assert_eq!(
            vm.run_loaded(&mut DecliningHost, &VmRunOptions::default())
                .stop_reason,
            VmStopReason::Completed
        );
        assert!(vm.pending_host_request_ids().is_empty());
        assert_eq!(vm.stack.len(), 1);
    }
}

#[derive(Debug, Clone, PartialEq, Eq)]
pub struct HostMemoryWrite {
    pub address: u32,
    /// Already encoded guest bytes, including any required NUL terminator.
    pub bytes: Vec<u8>,
}

#[derive(Debug, Clone, Default, PartialEq)]
pub struct HostResponse {
    /// Exactly the native ABI's stack output count, in push order. For the
    /// Android v1 integer response use vec![Value::Int(result)] (or vec![] for
    /// zero-output calls). No placeholder outputs are pushed before completion.
    pub outputs: Vec<Value>,
    pub writes: Vec<HostMemoryWrite>,
}

#[derive(Debug)]
pub(crate) struct PendingHostRequest {
    id: u64,
    output_count: usize,
    /// Set for `HostRequest::UserDialog`: where the answer is written.
    pub(crate) user_dialog: Option<crate::user_dialog::UserDialogSink>,
}

/// Measured Vec backing storage; not a process RSS estimate. The running
/// thread holds the process memory; every other VM only its own buffer.
#[derive(Debug, Clone, Default, PartialEq, Eq)]
pub struct VmResourceUsage {
    pub vm_count: usize,
    pub memory_bytes: usize,
    pub memory_capacity_bytes: usize,
}

/// Host-enforced admission limits. Check before scheduling a pass and after
/// each pass. This does not promise to bound allocations within one opcode;
/// image/audio/IO allocations belong to their respective host subsystems.
#[derive(Debug, Clone, Copy)]
pub struct VmResourceBudget {
    pub max_vm_count: usize,
    pub max_memory_capacity_bytes: usize,
}

impl Vm {
    pub(crate) fn begin_host_continuation<A: SysApi>(
        &mut self,
        api: &mut A,
        request: HostRequest,
        output_count: usize,
    ) -> VmResult<bool> {
        if !api.host_requests_enabled() {
            return Ok(false);
        }
        let Some(id) = api.begin_host_request(&request)? else {
            return Ok(false);
        };
        if id == 0 || self.pending_host_request.is_some() {
            return Err(VmError::Runtime(
                "invalid or overlapping host request".into(),
            ));
        }
        self.pending_host_request = Some(PendingHostRequest {
            id,
            output_count,
            user_dialog: None,
        });
        Ok(true)
    }

    pub(crate) fn intercept_native_host_request<A: SysApi>(
        &mut self,
        api: &mut A,
        opcode: NativeOpcode,
    ) -> VmResult<bool> {
        // Typed UserB0 dialogs suspend after parsing (see user_dialog.rs).
        if crate::user_dialog::is_user_dialog(opcode.group, opcode.id) {
            return Ok(false);
        }
        let Some(argc) = self.effective_arg_count(opcode.group, opcode.id) else {
            return Ok(false);
        };
        // Native BP uses a wrapping operand ring, including stale slots when
        // popping below zero. Merely slicing the public Vec changes that ABI.
        let args = (0..argc)
            .map(|index| {
                let slot = (self.stack.len() + crate::OPERAND_STACK_CAPACITY
                    - argc % crate::OPERAND_STACK_CAPACITY
                    + index)
                    % crate::OPERAND_STACK_CAPACITY;
                self.stack
                    .get(slot)
                    .unwrap_or(&self.operand_slots[slot])
                    .clone()
            })
            .collect();
        // Normalize the complete argument snapshot, not only the currently
        // visible Vec suffix: native arguments may wrap through stale operand
        // slots. Resolve strings here, in the owning (possibly child) VM;
        // hosts cannot safely decode these pointers using the root's memory.
        let args = self.normalize_host_request_args(opcode, args)?;
        let pending = self.begin_host_continuation(
            api,
            HostRequest::Native { opcode, args },
            self.effective_output_count(opcode.group, opcode.id),
        )?;
        if pending {
            // Use the normal operand-ring pop path, not Vec::truncate.
            self.take_dispatch_call_frame(opcode.group, opcode.id)?;
        }
        Ok(pending)
    }

    fn normalize_host_request_args(
        &mut self,
        opcode: NativeOpcode,
        args: Vec<Value>,
    ) -> VmResult<Vec<Value>> {
        // Reuse the selector-specific native conversion tables, preserving
        // system/user SJIS versus graph text's selected encoding. In particular
        // output buffers and descriptor pointers MUST NOT all become strings.
        // Isolate normalization so declining interception leaves the caller's
        // original stack/ring intact (including DWORD-pointer conversions).
        let caller_stack = std::mem::replace(&mut self.stack, args);
        let caller_slots = std::mem::replace(
            &mut self.operand_slots,
            // Normalizers only replace positions inside this snapshot. Avoid
            // allocating a whole 4096-slot ring for each intercepted query.
            vec![Value::Int(0); self.stack.len()],
        );
        let caller_synced_len = self.operand_slots_synced_len;
        self.operand_slots_synced_len = 0;
        let normalized = match opcode.group {
            0x80 | 0x81 => self.normalize_sys_string_args(opcode.group, opcode.id),
            0xb0 | 0xc0 => self.normalize_user_string_args(opcode.group, opcode.id),
            0x90..=0x92 => self.normalize_graph_string_args(opcode.group, opcode.id),
            _ => Ok(()),
        };
        let args = std::mem::replace(&mut self.stack, caller_stack);
        self.operand_slots = caller_slots;
        self.operand_slots_synced_len = caller_synced_len;
        normalized?;
        Ok(args)
    }

    /// Complete a request in this VM or any descendant. All output counts and
    /// memory ranges are validated before *any* writes/results are committed.
    /// Writes must target existing guest memory; this API cannot allocate.
    /// The validated response settles on that thread's next scheduler slice,
    /// after shared-state synchronization, so child global writes are not lost.
    /// Invalid responses leave the continuation pending and can be corrected.
    /// Duplicate/stale/cancelled IDs fail; restarting execution cancels the
    /// local continuation. Hosts must also reject old-session IDs themselves.
    pub fn complete_host_request(&mut self, id: u64, response: HostResponse) -> VmResult<()> {
        if self.halted || self.fatal_async_report.is_some() {
            return Err(VmError::Runtime("host request session has stopped".into()));
        }
        let target = self
            .host_request_vm_mut(id)
            .ok_or_else(|| VmError::Runtime("unknown or cancelled host request".into()))?;
        if target.halted {
            return Err(VmError::Runtime("host request thread has stopped".into()));
        }
        let pending = target
            .pending_host_request
            .as_ref()
            .expect("located request");
        if response.outputs.len() != pending.output_count
            || response
                .outputs
                .iter()
                .any(|value| matches!(value, Value::None))
        {
            return Err(VmError::Runtime(
                "host response violates stack output contract".into(),
            ));
        }
        // A suspended thread does not hold the process memory (see
        // `hand_over_memory`), so only the address itself is checked here;
        // `settle_ready_host_response` writes once the thread runs again.
        for write in &response.writes {
            let start = target.memory_addr(write.address) as usize;
            if start
                .checked_add(write.bytes.len())
                .is_none_or(|end| end > crate::MAX_MEMORY_SIZE)
            {
                return Err(VmError::MemoryOutOfBounds {
                    addr: write.address,
                    size: write.bytes.len(),
                });
            }
        }
        target.ready_host_response = Some(response);
        target.pending_host_request = None;
        Ok(())
    }

    pub(crate) fn settle_ready_host_response(&mut self) -> VmResult<()> {
        let Some(response) = self.ready_host_response.take() else {
            return Ok(());
        };
        // The thread runs again, so it holds the process memory: the writes
        // land in the globals and heap every thread sees.
        let ranges = response
            .writes
            .iter()
            .map(|write| self.resolve_range(write.address, write.bytes.len()))
            .collect::<VmResult<Vec<_>>>()?;
        for (write, range) in response.writes.into_iter().zip(ranges) {
            self.memory[range].copy_from_slice(&write.bytes);
            self.clear_shadow_values(write.address, write.bytes.len());
        }
        for value in response.outputs {
            self.push_value(value);
        }
        Ok(())
    }

    /// Reports from child slices in the most recent root scheduler pass. Fatal
    /// child reports are also returned directly by run_loaded and latched until
    /// start(). Successful child diagnostics are not silently discarded.
    pub fn take_async_reports(&mut self) -> Vec<crate::VmRunReport> {
        std::mem::take(&mut self.async_reports)
    }

    pub(crate) fn host_request_vm_mut(&mut self, id: u64) -> Option<&mut Vm> {
        if self
            .pending_host_request
            .as_ref()
            .is_some_and(|pending| pending.id == id)
        {
            return Some(self);
        }
        self.async_tasks
            .iter_mut()
            .find_map(|task| task.vm.host_request_vm_mut(id))
    }

    pub fn pending_host_request_ids(&self) -> Vec<u64> {
        let mut ids = self
            .pending_host_request
            .as_ref()
            .map(|pending| vec![pending.id])
            .unwrap_or_default();
        for task in &self.async_tasks {
            ids.extend(task.vm.pending_host_request_ids());
        }
        ids
    }

    /// Cancel without fabricating a user answer. Affected threads halt; callers
    /// normally destroy the session next. Returns IDs for host event cleanup.
    pub fn cancel_host_requests(&mut self) -> Vec<u64> {
        let mut ids = Vec::new();
        if self.ready_host_response.take().is_some() {
            self.halted = true;
            self.thread.mark_terminated();
        }
        if let Some(pending) = self.pending_host_request.take() {
            ids.push(pending.id);
            self.halted = true;
            self.thread.mark_terminated();
        }
        for task in &mut self.async_tasks {
            ids.extend(task.vm.cancel_host_requests());
        }
        ids
    }

    pub fn resource_usage(&self) -> VmResourceUsage {
        let mut usage = VmResourceUsage::default();
        self.add_private_usage(&mut usage);
        usage
    }

    /// `(task key, runnable, completed, memory length, capacity)` for every
    /// child VM, depth first, for budget diagnostics.
    pub fn task_memory_breakdown(&self) -> Vec<(String, bool, bool, usize, usize)> {
        let mut out = Vec::new();
        for task in &self.async_tasks {
            out.push((
                task.key().to_string(),
                task.runnable,
                task.completed,
                task.vm.memory.len(),
                task.vm.memory.capacity(),
            ));
            out.extend(task.vm.task_memory_breakdown());
        }
        out
    }

    fn add_private_usage(&self, usage: &mut VmResourceUsage) {
        usage.vm_count += 1;
        usage.memory_bytes = usage.memory_bytes.saturating_add(self.memory.len());
        usage.memory_capacity_bytes = usage
            .memory_capacity_bytes
            .saturating_add(self.memory.capacity());
        for task in &self.async_tasks {
            task.vm.add_private_usage(usage);
        }
    }

    pub fn check_resource_budget(&self, budget: VmResourceBudget) -> VmResult<VmResourceUsage> {
        let usage = self.resource_usage();
        if usage.vm_count > budget.max_vm_count
            || usage.memory_capacity_bytes > budget.max_memory_capacity_bytes
        {
            return Err(VmError::Runtime("VM resource budget exceeded".into()));
        }
        Ok(usage)
    }
}

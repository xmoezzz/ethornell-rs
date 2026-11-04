//! The only unsafe operations here are bounded caller-memory access and the
//! documented raw Android window/context handoff. Runtime objects never cross
//! threads and are never represented by caller-controlled pointers.
#[cfg(target_os = "ios")]
mod ios;
#[cfg(target_os = "android")]
mod android_jni;
use ethornell_app::host_runtime::{HostConfig, HostRuntime};
use std::{
    cell::RefCell,
    collections::BTreeMap,
    ffi::c_void,
    panic::{AssertUnwindSafe, catch_unwind},
    sync::{
        Mutex,
        atomic::{AtomicU64, Ordering},
    },
};
const ABI: u32 = 1;
const ARG: i32 = -1;
const HANDLE: i32 = -2;
const BAD_ABI: i32 = -3;
const RESOURCE: i32 = -4;
const PANIC: i32 = -11;
const STATE: i32 = -12;
const MAX_STRING: usize = 16 * 1024;
#[repr(C)]
#[derive(Clone, Copy)]
pub struct StringV1 {
    ptr: *const u8,
    len: u32,
    reserved: u32,
}
#[repr(C)]
pub struct ConfigV1 {
    struct_size: u32,
    abi_version: u32,
    resource_root: StringV1,
    user_root: StringV1,
    font_path: StringV1,
    text_encoding: u32,
    font_face_index: u32,
    backend: u32,
    flags: u32,
    reserved: [u32; 4],
}
#[repr(C)]
pub struct InputV1 {
    struct_size: u32,
    kind: u32,
    x: f32,
    y: f32,
    code: i32,
    value: f32,
    reserved: [u32; 2],
}
#[repr(C)]
pub struct EventV1 {
    struct_size: u32,
    kind: u32,
    request_id: u64,
    code: i32,
    text_len: u32,
    text: [u8; 2048],
}
#[repr(C)]
pub struct ResponseV1 {
    struct_size: u32,
    result: i32,
    text: StringV1,
}
struct Session {
    runtime: HostRuntime,
    failed: bool,
}
thread_local! {
    static SESSIONS:RefCell<BTreeMap<u64,Session>>=const {RefCell::new(BTreeMap::new())};
    static ERROR:RefCell<String>=const {RefCell::new(String::new())};
}
static NEXT: AtomicU64 = AtomicU64::new(1);
static CONTEXT: Mutex<Option<(usize, usize)>> = Mutex::new(None);
fn fail(code: i32, message: impl Into<String>) -> i32 {
    ERROR.with(|e| *e.borrow_mut() = message.into());
    code
}
fn guarded(f: impl FnOnce() -> i32) -> i32 {
    match catch_unwind(AssertUnwindSafe(f)) {
        Ok(code) => code,
        Err(_) => fail(PANIC, "internal panic; session cannot continue"),
    }
}
fn with_session(
    handle: u64,
    allow_failed: bool,
    f: impl FnOnce(&mut HostRuntime) -> Result<(), String>,
) -> i32 {
    guarded(|| {
        SESSIONS.with(|registry| {
            let mut registry = registry.borrow_mut();
            let Some(session) = registry.get_mut(&handle) else {
                return fail(HANDLE, "invalid, destroyed or wrong-thread handle");
            };
            if session.failed && !allow_failed {
                return fail(
                    STATE,
                    "session failed; only detach, diagnostic and destroy are allowed",
                );
            }
            match catch_unwind(AssertUnwindSafe(|| session.runtime.scope(f))) {
                Ok(Ok(())) => 0,
                Ok(Err(message)) => {
                    // Rejected stale UI work is not runtime corruption. GPU,
                    // VM, policy and audio failures still poison the session.
                    session.failed = !message.contains("invalid/stale surface")
                        && !message.contains("unknown/stale request")
                        && !message.contains("non-monotonic host clock");
                    fail(classify(&message), redact(&message))
                }
                Err(_) => {
                    session.failed = true;
                    fail(PANIC, "internal panic; session poisoned")
                }
            }
        })
    })
}
fn redact(message: &str) -> String {
    let mut result=redact_class(message);
    if let Some((_,context))=message.split_once("; host_runtime_diag:") {
        let fields=context.split(':').map(str::parse::<u64>).collect::<Result<Vec<_>,_>>();
        if let Ok(fields)=fields {
            if matches!(fields.len(),5|6) && fields[0]<=15 && (fields[3]<=255 || fields[3]==u32::MAX as u64) && (fields[4]<=65535 || fields[4]==u32::MAX as u64) && (fields.len()==5 || fields[5]<=8) {
                result.push_str(&format!(" stop={} pc={} offset={} group={} id={}",fields[0],fields[1],fields[2],fields[3],fields[4]));
                if fields.len()==6 {result.push_str(&format!(" cause={}",fields[5]));}
            }
        }
    }
    result
}
fn redact_class(message: &str) -> String {
    // Internal errors may contain resource paths or script dialogue. ABI
    // diagnostics intentionally publish a class, not unredacted engine logs.
    if let Some(code) = policy_diagnostic(message) { return code; }
    if let Some(reason) = gpu_diagnostic(message) { return reason; }
    if message.contains("unsupported") {
        "unsupported host/game capability".into()
    } else if message.contains("denied") || message.contains("roots") {
        "filesystem policy denied operation".into()
    } else if message.contains("font") {
        "CJK font unavailable or invalid".into()
    } else if message.contains("surface")
        || message.contains("wgpu")
        || message.contains("adapter")
        || message.contains("GPU")
        || message.contains("device lost")
    {
        "GPU surface/device initialization or rendering failed".into()
    } else if message.contains("audio") {
        "audio initialization or suspension failed".into()
    } else {
        "runtime resource/VM operation failed".into()
    }
}
// Driver text can contain labels/paths/shader source. Expose only fixed
// reasons, plus decimal numbers from our own whitelisted limits checker.
fn gpu_diagnostic(message: &str) -> Option<String> {
    let (stage,reason) = if message.contains("no compatible wgpu adapter") {
        (2,"adapter_unavailable")
    } else if message.contains("adapter does not meet 2D renderer limits") {
        (3,"unsupported_limits")
    } else if message.contains("request device failed") {
        (4,"request_device_failed")
    } else if message.contains("surface has no supported formats") {
        (5,"surface_formats_empty")
    } else if message.contains("surface has no supported present modes") {
        (5,"surface_present_modes_empty")
    } else if message.contains("surface has no supported alpha modes") {
        (5,"surface_alpha_modes_empty")
    } else if message.contains("surface does not support rendering") {
        (5,"surface_render_attachment_unavailable")
    } else if message.contains("create Android surface failed") {
        (1,"surface_creation_failed")
    } else if message.contains("invalid GPU dimensions") {
        (1,"invalid_dimensions")
    } else if message.contains("GPU error:") {
        (6,"uncaptured_validation_error")
    } else if message.contains("device lost") {
        (7,"device_lost")
    } else { return None; };
    let mut result=format!("GPU stage={stage} reason={reason}");
    if stage==3 {
        const LIMITS:&[&str]=&["max_texture_dimension_1d","max_texture_dimension_2d","max_texture_dimension_3d","max_texture_array_layers","max_bind_groups","max_bindings_per_bind_group","max_dynamic_uniform_buffers_per_pipeline_layout","max_dynamic_storage_buffers_per_pipeline_layout","max_sampled_textures_per_shader_stage","max_samplers_per_shader_stage","max_storage_buffers_per_shader_stage","max_storage_textures_per_shader_stage","max_uniform_buffers_per_shader_stage","max_uniform_buffer_binding_size","max_storage_buffer_binding_size","max_vertex_buffers","max_buffer_size","max_vertex_attributes","max_vertex_buffer_array_stride","min_uniform_buffer_offset_alignment","min_storage_buffer_offset_alignment","max_inter_stage_shader_components","max_compute_workgroup_storage_size","max_compute_invocations_per_workgroup","max_compute_workgroup_size_x","max_compute_workgroup_size_y","max_compute_workgroup_size_z","max_compute_workgroups_per_dimension"];
        for name in LIMITS {
            let marker=format!("{name}: requested ");
            if let Some((_,values))=message.split_once(&marker) {
                if let Some((requested,available))=values.split_once(", available ") {
                    let available=available.split(',').next().unwrap_or("");
                    if let (Ok(requested),Ok(available))=(requested.parse::<u64>(),available.parse::<u64>()) {
                        let field=format!(" {name}={requested}/{available}");
                        if result.len()+field.len()<=768 {result.push_str(&field);}
                    }
                }
            }
        }
    }
    Some(result)
}
fn policy_diagnostic(message: &str) -> Option<String> {
    let start=message.find("host_policy:")?;
    let suffix=&message[start+12..];
    let mut fields=suffix.split(':');
    let stage=fields.next()?.parse::<u32>().ok()?;
    let errno=fields.next()?.parse::<i32>().ok()?;
    Some(format!("filesystem policy stage={stage} errno={errno}"))
}
fn classify(message: &str) -> i32 {
    if policy_diagnostic(message).is_some() { return -5; }
    if gpu_diagnostic(message).is_some() {return -7;}
    if message.contains("unsupported") {
        -10
    } else if message.contains("denied") || message.contains("roots") {
        -5
    } else if message.contains("font") {
        -6
    } else if message.contains("surface")
        || message.contains("wgpu")
        || message.contains("adapter")
        || message.contains("GPU")
        || message.contains("device lost")
    {
        -7
    } else if message.contains("audio") {
        -8
    } else if message.contains("VM") {
        -9
    } else {
        RESOURCE
    }
}
unsafe fn string(value: StringV1) -> Result<String, i32> {
    if value.reserved != 0
        || value.len as usize > MAX_STRING
        || (value.len != 0 && value.ptr.is_null())
    {
        return Err(fail(ARG, "invalid bounded UTF-8 string"));
    }
    if value.len == 0 {
        return Ok(String::new());
    }
    // SAFETY: caller promises the validated ptr+len points to readable memory.
    let bytes = unsafe { std::slice::from_raw_parts(value.ptr, value.len as usize) };
    if bytes.contains(&0) {
        return Err(fail(ARG, "embedded NUL in string"));
    }
    std::str::from_utf8(bytes)
        .map(str::to_owned)
        .map_err(|_| fail(ARG, "invalid UTF-8"))
}
unsafe fn copy_string(text: &str, out: *mut u8, capacity: u32) -> i32 {
    let needed = text.len() + 1;
    if !out.is_null() && capacity as usize >= needed {
        unsafe {
            std::ptr::copy_nonoverlapping(text.as_ptr(), out, text.len());
            *out.add(text.len()) = 0;
        }
    }
    needed.min(i32::MAX as usize) as i32
}
#[unsafe(no_mangle)]
pub extern "C" fn ethornell_android_abi_version() -> u32 {
    ABI
}
#[unsafe(no_mangle)]
pub unsafe extern "C" fn ethornell_android_build_info(out: *mut u8, capacity: u32) -> i32 {
    guarded(|| unsafe {
        copy_string(
            concat!(
                "ethornell host ABI=1 source=",
                env!("ETHORNELL_SOURCE_REV"),
                "; pending working-tree provenance in build manifest; MPEG video-only; SWF partial; no WMV/BMV/MP4"
            ),
            out,
            capacity,
        )
    })
}
#[unsafe(no_mangle)]
pub unsafe extern "C" fn ethornell_android_last_error(out: *mut u8, capacity: u32) -> i32 {
    guarded(|| ERROR.with(|e| unsafe { copy_string(&e.borrow(), out, capacity) }))
}
#[unsafe(no_mangle)]
pub extern "C" fn ethornell_android_init_context(vm: *mut c_void, context: *mut c_void) -> i32 {
    guarded(|| {
        if vm.is_null() || context.is_null() {
            return fail(ARG, "null VM/Application GlobalRef");
        }
        let Ok(mut current) = CONTEXT.lock() else {
            return fail(STATE, "context lock poisoned");
        };
        if let Some(pair) = *current {
            return if pair == (vm as usize, context as usize) {
                0
            } else {
                fail(STATE, "context already initialized with different pointers")
            };
        }
        #[cfg(target_os = "android")]
        unsafe {
            ndk_context::initialize_android_context(vm, context);
        }
        *current = Some((vm as usize, context as usize));
        0
    })
}
#[unsafe(no_mangle)]
pub unsafe extern "C" fn ethornell_android_create(config: *const ConfigV1, out: *mut u64) -> i32 {
    guarded(|| {
        if out.is_null() || config.is_null() {
            return fail(ARG, "null config/out handle");
        }
        unsafe {
            *out = 0;
        }
        if unsafe { (*config).struct_size } != std::mem::size_of::<ConfigV1>() as u32 {
            return fail(BAD_ABI, "config struct_size mismatch");
        }
        let config = unsafe { &*config };
        if config.abi_version != ABI {
            return fail(BAD_ABI, "unsupported ABI version");
        }
        if config.reserved != [0; 4]
            || config.flags & !7 != 0
            || config.text_encoding > 1
            || config.backend > 2
        {
            return fail(ARG, "invalid config fields");
        }
        #[cfg(target_os = "android")]
        if CONTEXT.lock().map(|c| c.is_none()).unwrap_or(true) {
            return fail(STATE, "initialize Android context before create");
        }
        let paths = unsafe {
            (
                string(config.resource_root),
                string(config.user_root),
                string(config.font_path),
            )
        };
        let (Ok(root), Ok(user), Ok(font)) = paths else {
            return ARG;
        };
        if root.is_empty() || user.is_empty() {
            return fail(ARG, "resource/user root required");
        }
        let config = HostConfig {
            resource_root: root.into(),
            user_root: user.into(),
            font_path: if font.is_empty() {
                None
            } else {
                Some(font.into())
            },
            font_data: None,
            font_face_index: config.font_face_index,
            gbk: config.text_encoding == 1,
            backend: config.backend,
            strict: config.flags & 1 != 0,
            trace: config.flags & 2 != 0,
            silent: config.flags & 4 != 0,
        };
        match HostRuntime::create(config) {
            Ok(runtime) => {
                // Monotonically unique generation (never reused, even across
                // threads) prevents stale-token aliasing after destruction.
                let handle = NEXT
                    .fetch_update(Ordering::Relaxed, Ordering::Relaxed, |n| n.checked_add(1))
                    .unwrap_or(0);
                if handle == 0 {
                    return fail(STATE, "handle generation exhausted");
                }
                SESSIONS.with(|s| {
                    s.borrow_mut().insert(
                        handle,
                        Session {
                            runtime,
                            failed: false,
                        },
                    )
                });
                unsafe {
                    *out = handle;
                }
                0
            }
            Err(e) => fail(classify(&e.to_string()), redact(&e.to_string())),
        }
    })
}
#[unsafe(no_mangle)]
pub unsafe extern "C" fn ethornell_android_set_surface(
    h: u64,
    p: *mut c_void,
    w: u32,
    height: u32,
    g: u64,
) -> i32 {
    if p.is_null() || w == 0 || height == 0 || g == 0 {
        return fail(ARG, "invalid Surface parameters");
    }
    with_session(h, false, |r| {
        unsafe { r.set_surface(p, w, height, g) }.map_err(|e| e.to_string())
    })
}
#[unsafe(no_mangle)]
pub extern "C" fn ethornell_android_detach_surface(h: u64, g: u64) -> i32 {
    with_session(h, true, |r| r.detach_surface(g).map_err(|e| e.to_string()))
}
#[unsafe(no_mangle)]
pub extern "C" fn ethornell_android_step(h: u64, now: u64) -> i32 {
    with_session(h, false, |r| r.step(now).map_err(|e| e.to_string()))
}
#[unsafe(no_mangle)]
pub unsafe extern "C" fn ethornell_android_input(h: u64, input: *const InputV1) -> i32 {
    guarded(|| {
        if input.is_null()
            || unsafe { (*input).struct_size } != std::mem::size_of::<InputV1>() as u32
        {
            return fail(ARG, "input struct_size mismatch");
        }
        let i = unsafe { &*input };
        if i.reserved != [0; 2]
            || !i.x.is_finite()
            || !i.y.is_finite()
            || !i.value.is_finite()
            || !(1..=7).contains(&i.kind)
        {
            return fail(ARG, "invalid input");
        }
        with_session(h, false, |r| {
            r.input(i.kind, i.x, i.y, i.code, i.value)
                .map_err(|e| e.to_string())
        })
    })
}
#[unsafe(no_mangle)]
pub extern "C" fn ethornell_android_set_paused(h: u64, mask: u32) -> i32 {
    with_session(h, false, |r| r.pause(mask).map_err(|e| e.to_string()))
}
#[unsafe(no_mangle)]
pub extern "C" fn ethornell_android_request_close(h: u64) -> i32 {
    with_session(h, false, |r| r.request_close().map_err(|e| e.to_string()))
}
#[unsafe(no_mangle)]
pub unsafe extern "C" fn ethornell_android_poll_event(h: u64, out: *mut EventV1) -> i32 {
    guarded(|| {
        if out.is_null() || unsafe { (*out).struct_size } != std::mem::size_of::<EventV1>() as u32 {
            return fail(ARG, "event struct_size mismatch");
        }
        let mut empty = true;
        let result = with_session(h, true, |r| {
            if let Some(e) = r.poll_event() {
                empty = false;
                let mut end = e.text.len().min(2047);
                while !e.text.is_char_boundary(end) {
                    end -= 1;
                }
                let mut event = EventV1 {
                    struct_size: std::mem::size_of::<EventV1>() as u32,
                    kind: e.kind,
                    request_id: e.request_id,
                    code: e.code,
                    text_len: end as u32,
                    text: [0; 2048],
                };
                event.text[..end].copy_from_slice(&e.text.as_bytes()[..end]);
                unsafe {
                    *out = event;
                }
            }
            Ok(())
        });
        if result == 0 && empty { 1 } else { result }
    })
}
#[unsafe(no_mangle)]
pub unsafe extern "C" fn ethornell_android_complete_request(
    h: u64,
    id: u64,
    response: *const ResponseV1,
) -> i32 {
    guarded(|| {
        if response.is_null()
            || unsafe { (*response).struct_size } != std::mem::size_of::<ResponseV1>() as u32
        {
            return fail(ARG, "response struct_size mismatch");
        }
        let response = unsafe { &*response };
        // Kind-5 dialog requests are answered with JSON text; every other
        // request takes the integer result and no text.
        let text = match unsafe { string(StringV1 { ..response.text }) } {
            Ok(text) => text,
            Err(status) => return status,
        };
        with_session(h, false, |r| {
            if text.is_empty() {
                r.complete_request(id, response.result)
            } else {
                r.complete_dialog(id, &text)
            }
            .map_err(|e| e.to_string())
        })
    })
}
/// The JSON description of a kind-5 dialog request, with the build_info
/// size convention (required bytes including NUL; NULL/0 queries the size).
#[unsafe(no_mangle)]
pub unsafe extern "C" fn ethornell_android_request_payload(
    h: u64,
    id: u64,
    out: *mut u8,
    capacity: u32,
) -> i32 {
    guarded(|| {
        let mut payload = None;
        let status = with_session(h, true, |r| {
            payload = r.request_payload(id);
            payload
                .is_some()
                .then_some(())
                .ok_or_else(|| "unknown/stale request".to_string())
        });
        match payload {
            Some(payload) if status == 0 => unsafe { copy_string(&payload, out, capacity) },
            _ => status,
        }
    })
}
#[unsafe(no_mangle)]
pub extern "C" fn ethornell_android_destroy(h: u64) -> i32 {
    guarded(|| {
        SESSIONS.with(|s| match s.borrow_mut().remove(&h) {
            Some(session) => {
                drop(session);
                0
            }
            None => fail(HANDLE, "invalid/destroyed/wrong-thread handle"),
        })
    })
}

#[cfg(test)]
mod tests {
    use super::*;
    #[test]
    fn runtime_context_diagnostic_is_numeric_only_and_bounded() {
        assert_eq!(redact("VM stopped: Error; host_runtime_diag:15:1234:5678:128:60"),"runtime resource/VM operation failed stop=15 pc=1234 offset=5678 group=128 id=60");
        assert!(!redact("VM secret dialogue; host_runtime_diag:15:1234:5678:/secret:60").contains("secret"));
        assert!(!redact("VM secret; host_runtime_diag:15:1234:5678:128:60:secret").contains("secret"));
        assert_eq!(redact("VM stopped; host_runtime_diag:15:1234:5678:128:60:1"),"runtime resource/VM operation failed stop=15 pc=1234 offset=5678 group=128 id=60 cause=1");
    }
    #[test]
    fn gpu_diagnostics_preserve_only_fixed_reason_and_whitelisted_limit_numbers() {
        assert_eq!(redact("adapter does not meet 2D renderer limits: max_vertex_attributes: requested 16, available 8"),"GPU stage=3 reason=unsupported_limits max_vertex_attributes=16/8");
        assert_eq!(classify("request device failed: private driver details"),-7);
        assert_eq!(redact("request device failed: private driver details"),"GPU stage=4 reason=request_device_failed");
        assert_eq!(redact("no compatible wgpu adapter"),"GPU stage=2 reason=adapter_unavailable");
        assert!(!redact("GPU error: shader secret/private/path").contains("secret"));
        assert!(!redact("adapter does not meet 2D renderer limits: unknown_secret: requested 1, available 0").contains("secret"));
    }
    #[test]
    fn policy_diagnostics_expose_only_numeric_stage_and_errno() {
        assert_eq!(classify("io error: host_policy:104:13"),-5);
        assert_eq!(redact("io error: host_policy:104:13"),"filesystem policy stage=104 errno=13");
        assert!(policy_diagnostic("host_policy:/secret/dialogue:13").is_none());
        assert!(policy_diagnostic("host_policy:104:13 secret").is_none());
        assert!(!redact("io error: secret user path").contains("secret"));
    }
    #[test]
    fn sizes_match_header_on_64_bit() {
        assert_eq!(std::mem::size_of::<StringV1>(), 16);
        assert_eq!(std::mem::size_of::<ConfigV1>(), 88);
        assert_eq!(std::mem::size_of::<InputV1>(), 32);
        assert_eq!(std::mem::size_of::<EventV1>(), 2072);
        assert_eq!(std::mem::size_of::<ResponseV1>(), 24);
    }
    #[test]
    fn rejects_forged_handles_and_null_inputs() {
        assert_eq!(ethornell_android_step(u64::MAX, 1), HANDLE);
        assert_eq!(ethornell_android_destroy(0), HANDLE);
        assert_eq!(
            unsafe { ethornell_android_create(std::ptr::null(), std::ptr::null_mut()) },
            ARG
        );
    }
    #[test]
    fn real_create_without_surface_wrong_thread_and_poisoned_session() {
        let root =
            std::env::temp_dir().join(format!("ethornell-abi-fixture-{}", std::process::id()));
        std::fs::create_dir_all(root.join("game")).unwrap();
        std::fs::create_dir_all(root.join("user")).unwrap();
        // Synthetic legal PackFile, not a commercial game or compatibility test.
        let mut arc = b"PackFile    ".to_vec();
        arc.extend_from_slice(&1u32.to_le_bytes());
        let mut name = [0u8; 16];
        name[..7].copy_from_slice(b"ipl._bp");
        arc.extend_from_slice(&name);
        arc.extend_from_slice(&0u32.to_le_bytes());
        arc.extend_from_slice(&2u32.to_le_bytes());
        arc.extend_from_slice(&[0; 8]);
        arc.extend_from_slice(&[0x00, 1]);
        std::fs::write(root.join("game/system.arc"), arc).unwrap();
        let resource = root.join("game").to_string_lossy().into_owned();
        let user = root.join("user").to_string_lossy().into_owned();
        let sv = |s: &str| StringV1 {
            ptr: s.as_ptr(),
            len: s.len() as u32,
            reserved: 0,
        };
        let config = ConfigV1 {
            struct_size: std::mem::size_of::<ConfigV1>() as u32,
            abi_version: ABI,
            resource_root: sv(&resource),
            user_root: sv(&user),
            font_path: sv(""),
            text_encoding: 0,
            font_face_index: 0,
            backend: 0,
            flags: 4,
            reserved: [0; 4],
        };
        let mut h = 0;
        assert_eq!(unsafe { ethornell_android_create(&config, &mut h) }, 0);
        assert_ne!(h, 0);
        assert_eq!(ethornell_android_step(h, 1_000_000), 0); // no Surface: no VM advancement
        assert_eq!(
            std::thread::spawn(move || ethornell_android_step(h, 100))
                .join()
                .unwrap(),
            HANDLE
        );
        assert_eq!(
            with_session(h, false, |_| Err("invalid/stale surface".into())),
            -7
        );
        assert_eq!(ethornell_android_step(h, 2_000_000), 0);
        assert_eq!(
            with_session(h, false, |_| Err("unknown/stale request".into())),
            RESOURCE
        );
        assert_eq!(ethornell_android_step(h, 3_000_000), 0);
        assert_eq!(
            with_session(h, false, |_| panic!("injected runtime panic")),
            PANIC
        );
        assert_eq!(ethornell_android_step(h, 100), STATE);
        assert_eq!(ethornell_android_detach_surface(h, 1), 0);
        assert_eq!(ethornell_android_destroy(h), 0);
        assert_eq!(ethornell_android_destroy(h), HANDLE);
        let mut h2 = 0;
        assert_eq!(unsafe { ethornell_android_create(&config, &mut h2) }, 0);
        assert_ne!(h, h2);
        assert_eq!(ethornell_android_destroy(h2), 0);
        std::fs::remove_dir_all(root).unwrap();
    }
}

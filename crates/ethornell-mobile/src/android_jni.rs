//! JNI entry points of the Android app (`platform/android`, class
//! `com.xmoezzz.ethornell.NativeEngine`), over the C ABI of this crate.
//!
//! The ABI borrows the `ANativeWindow`; this shim owns the reference
//! (`ANativeWindow_fromSurface`) and releases it only after detach or destroy
//! has returned, as `platform/android/include/ABI.md` asks of a JNI host.
use super::*;
use jni::JNIEnv;
use jni::objects::{JClass, JLongArray, JObject, JString};
use jni::sys::{jfloat, jint, jlong, jstring};

#[link(name = "android")]
unsafe extern "C" {
    fn ANativeWindow_fromSurface(env: *mut jni::sys::JNIEnv, surface: jni::sys::jobject)
    -> *mut c_void;
    fn ANativeWindow_release(window: *mut c_void);
}

/// One attached window per engine handle: (generation, ANativeWindow*).
struct Window {
    generation: u64,
    window: *mut c_void,
}

thread_local! {
    static WINDOWS: RefCell<BTreeMap<u64, Window>> = const { RefCell::new(BTreeMap::new()) };
}

fn release_window(handle: u64, generation: Option<u64>) {
    WINDOWS.with(|windows| {
        let mut windows = windows.borrow_mut();
        if windows
            .get(&handle)
            .is_some_and(|window| generation.is_none_or(|g| window.generation == g))
            && let Some(window) = windows.remove(&handle)
        {
            unsafe { ANativeWindow_release(window.window) };
        }
    });
}

fn java_string(env: &mut JNIEnv, value: &JString) -> Option<String> {
    if value.is_null() {
        return Some(String::new());
    }
    env.get_string(value).ok().map(Into::into)
}

fn string_v1(text: &str) -> StringV1 {
    StringV1 {
        ptr: text.as_ptr(),
        len: text.len() as u32,
        reserved: 0,
    }
}

#[unsafe(no_mangle)]
pub extern "system" fn Java_com_xmoezzz_ethornell_NativeEngine_nativeInit(
    env: JNIEnv,
    _: JClass,
    context: JObject,
) -> jint {
    let Ok(vm) = env.get_java_vm() else {
        return fail(STATE, "JavaVM unavailable");
    };
    let Ok(global) = env.new_global_ref(context) else {
        return fail(STATE, "Application GlobalRef failed");
    };
    let status = ethornell_android_init_context(
        vm.get_java_vm_pointer().cast(),
        global.as_obj().as_raw().cast(),
    );
    // The context stays referenced for the life of the process (ABI v1).
    std::mem::forget(global);
    status
}

#[unsafe(no_mangle)]
pub extern "system" fn Java_com_xmoezzz_ethornell_NativeEngine_nativeCreate(
    mut env: JNIEnv,
    _: JClass,
    resource_root: JString,
    user_root: JString,
    font_path: JString,
    text_encoding: jint,
    flags: jint,
) -> jlong {
    let (Some(root), Some(user), Some(font)) = (
        java_string(&mut env, &resource_root),
        java_string(&mut env, &user_root),
        java_string(&mut env, &font_path),
    ) else {
        fail(ARG, "invalid path string");
        return 0;
    };
    let config = ConfigV1 {
        struct_size: std::mem::size_of::<ConfigV1>() as u32,
        abi_version: ABI,
        resource_root: string_v1(&root),
        user_root: string_v1(&user),
        font_path: string_v1(&font),
        text_encoding: text_encoding as u32,
        font_face_index: 0,
        backend: 0,
        flags: flags as u32,
        reserved: [0; 4],
    };
    let mut handle = 0u64;
    match unsafe { ethornell_android_create(&config, &mut handle) } {
        0 => handle as jlong,
        _ => 0,
    }
}

#[unsafe(no_mangle)]
pub extern "system" fn Java_com_xmoezzz_ethornell_NativeEngine_nativeSetSurface(
    env: JNIEnv,
    _: JClass,
    handle: jlong,
    surface: JObject,
    width: jint,
    height: jint,
    generation: jlong,
) -> jint {
    let (handle, generation) = (handle as u64, generation as u64);
    let window = unsafe { ANativeWindow_fromSurface(env.get_raw(), surface.as_raw()) };
    if window.is_null() {
        return fail(ARG, "Surface has no ANativeWindow");
    }
    let status = unsafe {
        ethornell_android_set_surface(handle, window, width as u32, height as u32, generation)
    };
    if status != 0 {
        // A failed attach keeps no borrow of the new window.
        unsafe { ANativeWindow_release(window) };
        return status;
    }
    // The previous window's borrow ended with the successful replacement.
    release_window(handle, None);
    WINDOWS.with(|windows| {
        windows
            .borrow_mut()
            .insert(handle, Window { generation, window })
    });
    0
}

#[unsafe(no_mangle)]
pub extern "system" fn Java_com_xmoezzz_ethornell_NativeEngine_nativeDetachSurface(
    _: JNIEnv,
    _: JClass,
    handle: jlong,
    generation: jlong,
) -> jint {
    let status = ethornell_android_detach_surface(handle as u64, generation as u64);
    release_window(handle as u64, Some(generation as u64));
    status
}

#[unsafe(no_mangle)]
pub extern "system" fn Java_com_xmoezzz_ethornell_NativeEngine_nativeStep(
    _: JNIEnv,
    _: JClass,
    handle: jlong,
    monotonic_time_ns: jlong,
) -> jint {
    ethornell_android_step(handle as u64, monotonic_time_ns as u64)
}

#[unsafe(no_mangle)]
pub extern "system" fn Java_com_xmoezzz_ethornell_NativeEngine_nativeInput(
    _: JNIEnv,
    _: JClass,
    handle: jlong,
    kind: jint,
    x: jfloat,
    y: jfloat,
    code: jint,
    value: jfloat,
) -> jint {
    let input = InputV1 {
        struct_size: std::mem::size_of::<InputV1>() as u32,
        kind: kind as u32,
        x,
        y,
        code,
        value,
        reserved: [0; 2],
    };
    unsafe { ethornell_android_input(handle as u64, &input) }
}

#[unsafe(no_mangle)]
pub extern "system" fn Java_com_xmoezzz_ethornell_NativeEngine_nativeSetPaused(
    _: JNIEnv,
    _: JClass,
    handle: jlong,
    reason_mask: jint,
) -> jint {
    ethornell_android_set_paused(handle as u64, reason_mask as u32)
}

#[unsafe(no_mangle)]
pub extern "system" fn Java_com_xmoezzz_ethornell_NativeEngine_nativeRequestClose(
    _: JNIEnv,
    _: JClass,
    handle: jlong,
) -> jint {
    ethornell_android_request_close(handle as u64)
}

/// The next event's text, or null when there is none (or on an error,
/// with `meta[0]` = 0). `meta` receives kind, request id and code.
#[unsafe(no_mangle)]
pub extern "system" fn Java_com_xmoezzz_ethornell_NativeEngine_nativePollEvent(
    mut env: JNIEnv,
    _: JClass,
    handle: jlong,
    meta: JLongArray,
) -> jstring {
    let mut event = EventV1 {
        struct_size: std::mem::size_of::<EventV1>() as u32,
        kind: 0,
        request_id: 0,
        code: 0,
        text_len: 0,
        text: [0; 2048],
    };
    if unsafe { ethornell_android_poll_event(handle as u64, &mut event) } != 0 {
        return std::ptr::null_mut();
    }
    let fields = [
        i64::from(event.kind),
        event.request_id as i64,
        i64::from(event.code),
    ];
    if env.set_long_array_region(&meta, 0, &fields).is_err() {
        return std::ptr::null_mut();
    }
    let text = String::from_utf8_lossy(&event.text[..event.text_len as usize]);
    env.new_string(text)
        .map(|text| text.into_raw())
        .unwrap_or(std::ptr::null_mut())
}

#[unsafe(no_mangle)]
pub extern "system" fn Java_com_xmoezzz_ethornell_NativeEngine_nativeCompleteRequest(
    _: JNIEnv,
    _: JClass,
    handle: jlong,
    request_id: jlong,
    result: jint,
) -> jint {
    let response = ResponseV1 {
        struct_size: std::mem::size_of::<ResponseV1>() as u32,
        result,
        text: StringV1 {
            ptr: std::ptr::null(),
            len: 0,
            reserved: 0,
        },
    };
    unsafe { ethornell_android_complete_request(handle as u64, request_id as u64, &response) }
}

/// The JSON description of a kind-5 dialog request, or null.
#[unsafe(no_mangle)]
pub extern "system" fn Java_com_xmoezzz_ethornell_NativeEngine_nativeRequestPayload(
    env: JNIEnv,
    _: JClass,
    handle: jlong,
    request_id: jlong,
) -> jstring {
    let size = unsafe {
        ethornell_android_request_payload(handle as u64, request_id as u64, std::ptr::null_mut(), 0)
    };
    if size <= 1 {
        return std::ptr::null_mut();
    }
    let mut buffer = vec![0u8; size as usize];
    let copied = unsafe {
        ethornell_android_request_payload(
            handle as u64,
            request_id as u64,
            buffer.as_mut_ptr(),
            size as u32,
        )
    };
    if copied != size {
        return std::ptr::null_mut();
    }
    buffer.pop();
    let text = String::from_utf8_lossy(&buffer);
    env.new_string(text)
        .map(|text| text.into_raw())
        .unwrap_or(std::ptr::null_mut())
}

/// Answers a kind-5 dialog request with JSON text.
#[unsafe(no_mangle)]
pub extern "system" fn Java_com_xmoezzz_ethornell_NativeEngine_nativeCompleteDialog(
    mut env: JNIEnv,
    _: JClass,
    handle: jlong,
    request_id: jlong,
    answer: JString,
) -> jint {
    let Some(answer) = java_string(&mut env, &answer).filter(|answer| !answer.is_empty()) else {
        return fail(ARG, "empty dialog answer");
    };
    let response = ResponseV1 {
        struct_size: std::mem::size_of::<ResponseV1>() as u32,
        result: 0,
        text: StringV1 {
            ptr: answer.as_ptr(),
            len: answer.len() as u32,
            reserved: 0,
        },
    };
    unsafe { ethornell_android_complete_request(handle as u64, request_id as u64, &response) }
}

#[unsafe(no_mangle)]
pub extern "system" fn Java_com_xmoezzz_ethornell_NativeEngine_nativeDestroy(
    _: JNIEnv,
    _: JClass,
    handle: jlong,
) -> jint {
    let status = ethornell_android_destroy(handle as u64);
    release_window(handle as u64, None);
    status
}

#[unsafe(no_mangle)]
pub extern "system" fn Java_com_xmoezzz_ethornell_NativeEngine_nativeLastError(
    env: JNIEnv,
    _: JClass,
) -> jstring {
    let message = ERROR.with(|error| error.borrow().clone());
    env.new_string(message)
        .map(|text| text.into_raw())
        .unwrap_or(std::ptr::null_mut())
}

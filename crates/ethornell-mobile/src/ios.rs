//! iOS names of the host ABI (`platform/ios/include/ethornell_ios.h`). The
//! contract is the Android one (`platform/android/include/ABI.md`) with a
//! `UIView*` in place of the `ANativeWindow*`, and no JVM context: every call
//! runs on the main thread, which owns the view.
use super::*;

#[unsafe(no_mangle)]
pub extern "C" fn ethornell_ios_abi_version() -> u32 {
    ethornell_android_abi_version()
}
#[unsafe(no_mangle)]
pub unsafe extern "C" fn ethornell_ios_build_info(out: *mut u8, capacity: u32) -> i32 {
    unsafe { ethornell_android_build_info(out, capacity) }
}
#[unsafe(no_mangle)]
pub unsafe extern "C" fn ethornell_ios_last_error(out: *mut u8, capacity: u32) -> i32 {
    unsafe { ethornell_android_last_error(out, capacity) }
}
#[unsafe(no_mangle)]
pub unsafe extern "C" fn ethornell_ios_create(config: *const ConfigV1, out: *mut u64) -> i32 {
    unsafe { ethornell_android_create(config, out) }
}
/// `ui_view` is a borrowed `UIView*`; the host keeps it alive until the
/// detach or destroy call has returned.
#[unsafe(no_mangle)]
pub unsafe extern "C" fn ethornell_ios_set_surface(
    h: u64,
    ui_view: *mut c_void,
    width_px: u32,
    height_px: u32,
    generation: u64,
) -> i32 {
    unsafe { ethornell_android_set_surface(h, ui_view, width_px, height_px, generation) }
}
#[unsafe(no_mangle)]
pub extern "C" fn ethornell_ios_detach_surface(h: u64, generation: u64) -> i32 {
    ethornell_android_detach_surface(h, generation)
}
#[unsafe(no_mangle)]
pub extern "C" fn ethornell_ios_step(h: u64, monotonic_time_ns: u64) -> i32 {
    ethornell_android_step(h, monotonic_time_ns)
}
#[unsafe(no_mangle)]
pub unsafe extern "C" fn ethornell_ios_input(h: u64, input: *const InputV1) -> i32 {
    unsafe { ethornell_android_input(h, input) }
}
#[unsafe(no_mangle)]
pub extern "C" fn ethornell_ios_set_paused(h: u64, reason_mask: u32) -> i32 {
    ethornell_android_set_paused(h, reason_mask)
}
#[unsafe(no_mangle)]
pub extern "C" fn ethornell_ios_request_close(h: u64) -> i32 {
    ethornell_android_request_close(h)
}
#[unsafe(no_mangle)]
pub unsafe extern "C" fn ethornell_ios_poll_event(h: u64, out: *mut EventV1) -> i32 {
    unsafe { ethornell_android_poll_event(h, out) }
}
#[unsafe(no_mangle)]
pub unsafe extern "C" fn ethornell_ios_complete_request(
    h: u64,
    id: u64,
    response: *const ResponseV1,
) -> i32 {
    unsafe { ethornell_android_complete_request(h, id, response) }
}
#[unsafe(no_mangle)]
pub unsafe extern "C" fn ethornell_ios_request_payload(
    h: u64,
    id: u64,
    out: *mut u8,
    capacity: u32,
) -> i32 {
    unsafe { ethornell_android_request_payload(h, id, out, capacity) }
}
#[unsafe(no_mangle)]
pub extern "C" fn ethornell_ios_destroy(h: u64) -> i32 {
    ethornell_android_destroy(h)
}

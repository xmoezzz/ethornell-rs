#ifndef ETHORNELL_IOS_H
#define ETHORNELL_IOS_H
#include <stdint.h>
#ifdef __cplusplus
extern "C" {
#endif
/* The iOS names of the Ethornell host ABI v1. Structures, status codes and
 * semantics are those of platform/android/include/ethornell_android.h and
 * ABI.md, with a UIView in place of the ANativeWindow and no JVM context.
 * Every call runs on the main thread. Strings are borrowed for the duration
 * of the call, UTF-8, and cannot contain embedded NUL. */
#define ETHORNELL_IOS_ABI_VERSION 1u
typedef struct { const uint8_t *ptr; uint32_t len; uint32_t reserved; } EthornellStringV1;
typedef struct {
    uint32_t struct_size, abi_version;
    EthornellStringV1 resource_root, user_root, font_path;
    uint32_t text_encoding; /* 0=CP932/SJIS, 1=CP936/GBK */
    uint32_t font_face_index; /* TTC index; applies to CPU and GPU */
    uint32_t backend; /* 0 (Metal) */
    uint32_t flags; /* bit0 strict native calls; bit1 trace; bit2 explicit silent mode */
    uint32_t reserved[4];
} EthornellConfigV1;
typedef struct {
    uint32_t struct_size, kind; /* 1=move,2=button-down,3=button-up,4=key-down,5=key-up,6=wheel,7=cancel */
    float x, y; /* view drawable pixels; Rust performs aspect-fit conversion */
    int32_t code; /* button: Windows descriptor 1/2/4/5/6; key: Windows VK */
    float value; /* signed wheel delta */
    uint32_t reserved[2];
} EthornellInputV1;
typedef struct {
    uint32_t struct_size, kind; /* 1=diagnostic,2=confirmation request,3=closed,4=title,5=dialog request */
    uint64_t request_id;
    int32_t code; /* confirmation: 1 = the request takes a result, 0 = a message */
    uint32_t text_len; /* bytes present, excludes NUL; UTF-8 boundary preserved */
    uint8_t text[2048];
} EthornellEventV1;
typedef struct {
    uint32_t struct_size;
    int32_t result; /* VM integer result, once only: 1 yes, 0 no */
    EthornellStringV1 text; /* JSON answer for a kind-5 dialog request; empty otherwise */
} EthornellResponseV1;
/* Status: 0=OK, 1=no event/not yet due, negatives are errors. */
enum { ETHORNELL_E_ARGUMENT=-1, ETHORNELL_E_HANDLE=-2, ETHORNELL_E_ABI=-3,
       ETHORNELL_E_RESOURCE=-4, ETHORNELL_E_POLICY=-5, ETHORNELL_E_FONT=-6,
       ETHORNELL_E_GPU=-7, ETHORNELL_E_AUDIO=-8, ETHORNELL_E_VM=-9,
       ETHORNELL_E_UNSUPPORTED=-10, ETHORNELL_E_PANIC=-11,
       ETHORNELL_E_STATE=-12, ETHORNELL_E_THREAD=-13 };
uint32_t ethornell_ios_abi_version(void);
/* Returns required bytes INCLUDING terminating NUL; copies only when capacity
 * suffices. NULL/0 is a size query. last_error is thread-local, redacted. */
int32_t ethornell_ios_build_info(char *out, uint32_t capacity);
int32_t ethornell_ios_last_error(char *out, uint32_t capacity);
int32_t ethornell_ios_create(const EthornellConfigV1 *config, uint64_t *out_handle);
/* ui_view is a BORROWED UIView; the host keeps it alive until detach/destroy
 * has returned. A new size is a new surface: attach again with a higher
 * generation. Size zero must be detached, not attached. */
int32_t ethornell_ios_set_surface(uint64_t handle, void *ui_view, uint32_t width_px, uint32_t height_px, uint64_t generation);
int32_t ethornell_ios_detach_surface(uint64_t handle, uint64_t generation);
/* monotonic_time_ns: e.g. clock_gettime_nsec_np(CLOCK_UPTIME_RAW). */
int32_t ethornell_ios_step(uint64_t handle, uint64_t monotonic_time_ns);
int32_t ethornell_ios_input(uint64_t handle, const EthornellInputV1 *input);
/* Complete mask: bit0 app inactive, bit1 focus, bit2 user, bit3 host dialog. */
int32_t ethornell_ios_set_paused(uint64_t handle, uint32_t reason_mask);
int32_t ethornell_ios_request_close(uint64_t handle);
int32_t ethornell_ios_poll_event(uint64_t handle, EthornellEventV1 *out_event);
int32_t ethornell_ios_complete_request(uint64_t handle, uint64_t request_id, const EthornellResponseV1 *response);
/* JSON description of a kind-5 dialog request (see ABI.md). Returns required
 * bytes INCLUDING NUL; copies only when capacity suffices. NULL/0 is a size query. */
int32_t ethornell_ios_request_payload(uint64_t handle, uint64_t request_id, char *out, uint32_t capacity);
int32_t ethornell_ios_destroy(uint64_t handle);
#ifdef __cplusplus
}
#endif
#endif

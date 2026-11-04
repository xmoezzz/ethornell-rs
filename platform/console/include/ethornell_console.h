/* MPL-2.0. Shared Rust/native shell boundary; pointers are synchronous borrows.
 */
#pragma once
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
typedef struct EthornellVertex {
  float position[2], uv[2], opacity, ignore_alpha;
} EthornellVertex;
typedef struct EthornellDraw {
  uint64_t texture;
  EthornellVertex vertices[6];
  uint32_t scissor[4], blend, linear;
} EthornellDraw;
_Static_assert(sizeof(EthornellVertex) == 24, "native vertex ABI");
_Static_assert(sizeof(EthornellDraw) == 176, "native draw ABI");
uint64_t ethornell_gpu_create(uint32_t w, uint32_t h, const uint8_t *rgba);
int32_t ethornell_gpu_update(uint64_t texture, uint32_t w, uint32_t h,
                             const uint8_t *rgba);
void ethornell_gpu_destroy(uint64_t texture);
int32_t ethornell_gpu_begin(const float *clear);
int32_t ethornell_gpu_draw(const EthornellDraw *draw);
int32_t ethornell_gpu_end(void);
/* Shell initializes graphics before create, tears it down after destroy. */
void *ethornell_console_create(const char *game, const char *saves,
                               const char *font, uint32_t w, uint32_t h);
int32_t ethornell_console_step(void *engine, uint64_t monotonic_ns);
int32_t ethornell_console_input(void *engine, uint32_t kind, float x, float y,
                                int32_t code, float value);
/* Pointer motion for the stick (x, y in -1..1, y down) held for `seconds` on
   a `width`-pixel screen: dead zone, then a curve for slow aim and fast travel. */
void ethornell_console_stick(float x, float y, float seconds, float width,
                             float *dx, float *dy);
int32_t ethornell_console_pause(void *engine, bool paused);
int32_t ethornell_console_close(void *engine);
void ethornell_console_destroy(void *engine);
void ethornell_console_audio(int16_t *samples, size_t frames);
/* Returns event kind (0 no event,1 diagnostic,2 confirmation,3 closed,4 title,
   5 structured dialog); text is NUL-terminated, truncated only for display. */
int32_t ethornell_console_event(void *engine, uint64_t *request, int32_t *code,
                                char *text, size_t capacity);
int32_t ethornell_console_respond(void *engine, uint64_t request,
                                  int32_t result);
int32_t ethornell_console_dialog(void *engine, uint64_t request,
                                 const char *json);
void ethornell_console_last_error(char *text, size_t capacity);

void ethornell_console_diagnostics(void *engine, char *text, size_t size);

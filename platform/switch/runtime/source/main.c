/* MPL-2.0. libnx lifecycle/audio pattern adapted from xmoezzz/siglus_rs. */
#include "ethornell_console.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <switch.h>
#include <sys/stat.h>
int ethornell_switch_gpu_init(void);
void ethornell_switch_gpu_exit(void);
void ethornell_switch_log_message(const char *text) {
  FILE *f = fopen("sdmc:/switch/ethornell/player.log", "ab");
  if (f) {
    fputs(text, f);
    fclose(f);
  }
}
static void log_startup_result(const char *name, Result rc) {
  char b[160];
  snprintf(b, sizeof(b), "%s: %08x\n", name, rc);
  ethornell_switch_log_message(b);
}
long getrandom(void *buffer, size_t length, unsigned flags) {
  (void)flags;
  randomGet(buffer, length);
  return (long)length;
}
long sysconf(int name) {
  (void)name;
  return 4096;
}
enum {
  // A 20 ms buffer and six queued buffers tolerate decode/GPU stalls of up
  // to roughly 120 ms without audren underflowing.
  AudioBufferFrames = 960,
  AudioBufferCount = 6,
  // audrv memory pools must begin and end on page boundaries. Each active
  // buffer still submits only AudioBufferFrames; this tail makes every slot
  // exactly one 4 KiB page, for a 24 KiB pool.
  AudioBufferStorageFrames = 1024,
};
static AudioDriver audio_driver;
static AudioDriverWaveBuf audio_wavebufs[AudioBufferCount];
static int16_t audio_buffers[AudioBufferCount][AudioBufferStorageFrames * 2]
    __attribute__((aligned(0x1000)));
static bool audio_renderer_initialized;
static bool audio_driver_initialized;
static bool audio_ready;

static void initialize_audio(void) {
  static const AudioRendererConfig config = {
      .output_rate = AudioRendererOutputRate_48kHz,
      // audrv allocates one driver channel per configured voice slot. The
      // Kira bridge submits one stereo voice, so it needs two channels even
      // though we only use voice ID 0.
      .num_voices = 2,
      .num_effects = 0,
      .num_sinks = 1,
      .num_mix_objs = 1,
      .num_mix_buffers = 2,
  };
  const Result audren_rc = audrenInitialize(&config);
  if (R_FAILED(audren_rc)) {
    log_startup_result("audrenInitialize", audren_rc);
    return;
  }
  audio_renderer_initialized = true;
  const Result audrv_rc = audrvCreate(&audio_driver, &config, 2);
  if (R_FAILED(audrv_rc)) {
    log_startup_result("audrvCreate", audrv_rc);
    return;
  }
  audio_driver_initialized = true;
  armDCacheFlush(audio_buffers, sizeof(audio_buffers));
  const int pool =
      audrvMemPoolAdd(&audio_driver, audio_buffers, sizeof(audio_buffers));
  if (pool < 0) {
    ethornell_switch_log_message("ethornell_switch: audrvMemPoolAdd failed\n");
    return;
  }
  if (!audrvMemPoolAttach(&audio_driver, pool)) {
    ethornell_switch_log_message(
        "ethornell_switch: audrvMemPoolAttach failed\n");
    return;
  }
  static const u8 sink_channels[] = {0, 1};
  audrvDeviceSinkAdd(&audio_driver, AUDREN_DEFAULT_DEVICE_NAME, 2,
                     sink_channels);
  // Commit the memory pool and output sink before creating a voice. libnx's
  // driver does not expose the attached pool to voice setup until this update.
  const Result initial_update_rc = audrvUpdate(&audio_driver);
  if (R_FAILED(initial_update_rc)) {
    log_startup_result("audrvUpdate(pool)", initial_update_rc);
    return;
  }
  const Result start_rc = audrenStartAudioRenderer();
  if (R_FAILED(start_rc)) {
    log_startup_result("audrenStartAudioRenderer", start_rc);
    return;
  }
  if (!audrvVoiceInit(&audio_driver, 0, 2, PcmFormat_Int16, 48000)) {
    ethornell_switch_log_message("ethornell_switch: audrvVoiceInit failed\n");
    return;
  }
  audrvVoiceSetDestinationMix(&audio_driver, 0, AUDREN_FINAL_MIX_ID);
  audrvVoiceSetMixFactor(&audio_driver, 0, 1.0f, 0, 0);
  audrvVoiceSetMixFactor(&audio_driver, 0, 1.0f, 1, 1);
  audrvVoiceStart(&audio_driver, 0);
  const Result voice_update_rc = audrvUpdate(&audio_driver);
  if (R_FAILED(voice_update_rc)) {
    log_startup_result("audrvUpdate(voice)", voice_update_rc);
    return;
  }
  audio_ready = true;
  ethornell_switch_log_message("ethornell_switch: audio-ready\n");
}

static void pump_audio(void) {
  if (!audio_ready)
    return;
  for (unsigned i = 0; i < AudioBufferCount; ++i) {
    AudioDriverWaveBuf *wavebuf = &audio_wavebufs[i];
    if (wavebuf->state != AudioDriverWaveBufState_Free &&
        wavebuf->state != AudioDriverWaveBufState_Done)
      continue;
    ethornell_console_audio(audio_buffers[i], AudioBufferFrames);
    armDCacheFlush(audio_buffers[i], sizeof(audio_buffers[i]));
    wavebuf->data_raw = audio_buffers[i];
    wavebuf->size = (size_t)AudioBufferFrames * 2 * sizeof(int16_t);
    wavebuf->start_sample_offset = 0;
    wavebuf->end_sample_offset = AudioBufferFrames;
    wavebuf->is_looping = false;
    audrvVoiceAddWaveBuf(&audio_driver, 0, wavebuf);
  }
  audrvUpdate(&audio_driver);
}

static void exit_audio(void) {
  if (audio_driver_initialized)
    audrvClose(&audio_driver);
  if (audio_renderer_initialized)
    audrenExit();
}

static void error(void) {
  char text[2048];
  ethornell_console_last_error(text, sizeof(text));
  ethornell_switch_log_message(text);
}
static void hook(AppletHookType type, void *engine) {
  if (type == AppletHookType_OnFocusState) {
    ethornell_console_pause(engine,
                            appletGetFocusState() != AppletFocusState_InFocus);
  }
}
int main(void) {
  mkdir("sdmc:/switch/ethornell", 0777);
  const Result mount = romfsMountSelf("romfs");
  if (R_FAILED(mount)) {
    log_startup_result("romfsMountSelf", mount);
    return 1;
  }
  if (ethornell_switch_gpu_init() != 0) {
    ethornell_switch_log_message("GPU init failed\n");
    romfsExit();
    return 1;
  }
  const char *game = "sdmc:/switch/ethornell/game";
  FILE *test = fopen("romfs:/game/system.arc", "rb");
  if (test) {
    fclose(test);
    game = "romfs:/game";
  }
  const char *font = "sdmc:/switch/ethornell/font.ttf";
  test = fopen("romfs:/font.ttf", "rb");
  if (test) {
    fclose(test);
    font = "romfs:/font.ttf";
  }
  void *engine = ethornell_console_create(game, "sdmc:/switch/ethornell/saves",
                                          font, 1280, 720);
  if (!engine) {
    error();
    ethornell_switch_gpu_exit();
    romfsExit();
    return 1;
  }
  initialize_audio();
  if (!audio_ready) {
    exit_audio();
    ethornell_console_destroy(engine);
    ethornell_switch_gpu_exit();
    romfsExit();
    return 1;
  }
  hidInitializeTouchScreen();
  PadState pad;
  padConfigureInput(1, HidNpadStyleSet_NpadStandard);
  padInitializeDefault(&pad);
  AppletHookCookie cookie;
  appletHook(&cookie, hook, engine);
  float x = 640, y = 360;
  ethornell_console_input(engine, 1, x, y, 0, 0);
  bool previous_left = false;
  bool quit = false;
  uint64_t request = 0;
  uint64_t last = armGetSystemTick(), origin = last, report_at = last;
  /* Per-report frame timing: steps that ran an engine frame (> 1 ms). */
  uint64_t frames = 0, step_ticks = 0, step_max = 0;
  while (appletMainLoop() && !quit) {
    padUpdate(&pad);
    uint64_t down = padGetButtonsDown(&pad), up = padGetButtonsUp(&pad),
             held = padGetButtons(&pad);
    uint64_t now = armGetSystemTick();
    float elapsed = (float)(now - last) / (float)armGetSystemTickFreq();
    last = now;
    if (elapsed > 0.1f)
      elapsed = 0.1f;
    if ((held & (HidNpadButton_Plus | HidNpadButton_Minus)) ==
        (HidNpadButton_Plus | HidNpadButton_Minus))
      ethornell_console_close(engine);
    if (request) {
      if (down & HidNpadButton_A) {
        ethornell_console_respond(engine, request, 1);
        request = 0;
      } else if (down & HidNpadButton_B) {
        ethornell_console_respond(engine, request, 0);
        request = 0;
      }
    } else {
      HidAnalogStickState stick = padGetStickPos(&pad, 0);
      float dx = 0, dy = 0, old_x = x, old_y = y;
      ethornell_console_stick(stick.x / 32767.0f, -stick.y / 32767.0f,
                              elapsed, 1280, &dx, &dy);
      x += dx;
      y += dy;
      HidTouchScreenState touch = {0};
      bool touching = hidGetTouchScreenStates(&touch, 1) > 0 && touch.count > 0;
      if (touching) {
        x = touch.touches[0].x;
        y = touch.touches[0].y;
      }
      if (x < 0)
        x = 0;
      if (x > 1279)
        x = 1279;
      if (y < 0)
        y = 0;
      if (y > 719)
        y = 719;
      if (x != old_x || y != old_y)
        ethornell_console_input(engine, 1, x, y, 0, 0);
      bool left = touching || (held & HidNpadButton_A);
      if (left != previous_left)
        ethornell_console_input(engine, left ? 2 : 3, x, y, 1, 0);
      previous_left = left;
      static const struct {
        uint64_t button;
        int key;
      } keys[] = {{HidNpadButton_Up, 38},   {HidNpadButton_Down, 40},
                  {HidNpadButton_Left, 37}, {HidNpadButton_Right, 39},
                  {HidNpadButton_B, 27},    {HidNpadButton_X, 32},
                  {HidNpadButton_Y, 13},    {HidNpadButton_ZR, 17}};
      for (unsigned i = 0; i < sizeof(keys) / sizeof(keys[0]); i++) {
        if (down & keys[i].button)
          ethornell_console_input(engine, 4, 0, 0, keys[i].key, 0);
        if (up & keys[i].button)
          ethornell_console_input(engine, 5, 0, 0, keys[i].key, 0);
      }
      if (down & HidNpadButton_L)
        ethornell_console_input(engine, 6, 0, 0, 0, 1);
      if (down & HidNpadButton_R)
        ethornell_console_input(engine, 6, 0, 0, 0, -1);
    }
    uint64_t step_start = armGetSystemTick();
    if (ethornell_console_step(engine, armTicksToNs(now - origin)) != 0) {
      error();
      break;
    }
    uint64_t step = armGetSystemTick() - step_start;
    if (armTicksToNs(step) > 1000000) {
      frames++;
      step_ticks += step;
      if (step > step_max)
        step_max = step;
    }
    pump_audio();
    if (now - report_at >= 5 * armGetSystemTickFreq()) {
      char status[2048];
      snprintf(status, sizeof(status), "frames=%llu step_avg_ms=%.1f step_max_ms=%.1f ",
               (unsigned long long)frames,
               frames ? armTicksToNs(step_ticks) / 1e6 / frames : 0.0,
               armTicksToNs(step_max) / 1e6);
      ethornell_switch_log_message(status);
      frames = step_ticks = step_max = 0;
      ethornell_console_diagnostics(engine, status, sizeof(status));
      ethornell_switch_log_message(status);
      ethornell_switch_log_message("\n");
      report_at = now;
    }
    for (;;) {
      char text[2048];
      uint64_t id;
      int32_t code;
      int32_t kind =
          ethornell_console_event(engine, &id, &code, text, sizeof(text));
      if (kind < 0) {
        error();
        quit = true;
        break;
      }
      if (kind == 0)
        break;
      if (kind == 3)
        quit = true;
      else if (kind == 2) {
        request = id;
        ethornell_switch_log_message(text);
      } else if (kind == 5) {
        ethornell_switch_log_message(
            "structured dialog unsupported by Switch shell\n");
        quit = true;
      } else
        ethornell_switch_log_message(text);
    }
    svcSleepThread(1000 * 1000);
  }
  appletUnhook(&cookie);
  exit_audio();
  ethornell_console_destroy(engine);
  ethornell_switch_gpu_exit();
  romfsExit();
  return 0;
}

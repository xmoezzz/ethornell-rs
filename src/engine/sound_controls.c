// BGI 1.622 resident sound controls. Evidence: raw/bgi1622/a0_controls.{c,asm}.
// Addresses here are for 1.622; sound_se.c cites the older reference binary.
#include <stdint.h>

typedef struct SoundRamp {
    int active;                 // +00
    uint32_t start, end;         // +04 / +08, GetTickCount domain
    int from, to, current;      // +0C / +10 / +14
} SoundRamp;

typedef struct SoundChannel {   // 0x44 bytes, 16 BGM / 64 SE records
    int loaded;                // +00
    void *wave;                // +04
    int reserved;              // +08
    int primary, secondary;    // +0C / +10
    SoundRamp play;            // +14 .. +28 (SE play has no ramp)
    SoundRamp fade;            // +2C .. +40
} SoundChannel;

// sub_4A2DF0. Subtractions wrap as x86 int32 before conversion to double;
// the cast truncates toward zero. Nonpositive duration completes on poll,
// including 0x80000000..FFFFFFFF, rather than scheduling a multiweek ramp.
static int sound_ramp_poll(SoundRamp *r, uint32_t now)
{
    int32_t duration = (int32_t)(r->end - r->start);
    int32_t elapsed = (int32_t)(now - r->start);
    int32_t delta = (int32_t)((uint32_t)r->to - (uint32_t)r->from);
    double t = 1.0;
    if (duration > 0 && (t = (double)elapsed / duration) < 1.0) {
        r->current = (int)(t * delta + r->from);
    } else {
        r->active = 0;
        r->current = (int)(1.0 * delta + r->from);
    }
    return r->current;
}

// sub_4A3500 / 4A3560 / 4A35C0 / 4A3650. Replacing a ramp snapshots the
// last polled current value, not the previous ramp's destination. There is
// no immediate poll, even for duration zero. Unloaded channels do nothing.
static void sound_ramp_start(SoundRamp *r, uint32_t now, uint32_t ms, int to)
{
    r->start = now;
    r->end = now + ms;
    r->from = r->current;
    r->to = to;
    r->active = 1;
}

// sub_4A2E70: the sound worker polls both envelopes, even for paused/stopped
// resident channels, and recomputes the four-factor attenuation once if
// either envelope was active. An SE fade to zero does not stop the buffer.
extern void sound_apply_attenuation(SoundChannel *c);
static void sound_channels_poll(SoundChannel *c, int n, uint32_t now)
{
    for (int i = 0; i < n; ++i) {
        if (!c[i].loaded) continue;
        int changed = c[i].play.active || c[i].fade.active;
        if (c[i].play.active) sound_ramp_poll(&c[i].play, now);
        if (c[i].fade.active) sound_ramp_poll(&c[i].fade, now);
        if (changed) sound_apply_attenuation(&c[i]);
    }
}

// A0:24 / sub_488360 -> 494E90 -> 4A4920 -> 4A4050:
//   validate (unsigned) pan <= 128, volume <= 128, channel < 64;
//   if sound unavailable or channel empty, leave channel unchanged;
//   set pan; play.current = volume; fade.active = 0; fade.current = 128;
//   wave->Stop() [vtable+16]; wave->Play(attenuation) [vtable+8].
// Every play rewinds/restarts, even if the same buffer is already playing.
// Return the copied header length for native status 0 or 20; else zero.
//
// A0:25 / sub_4883F0 -> 494EB0 -> 4A3A80 -> 4A34D0:
//   validate (unsigned) channel < 64; empty is a no-op;
//   wave->Stop() [vtable+16]. Keep header, gain banks and fade envelope.
//
// A0:22 / sub_488240 -> 494E70 -> 4A3280:
//   validate channel < 64; clear 64-byte copied header; release wave;
//   clear loaded/decoder fields. No gain-bank reset.
//
// Every 08/09/17/1C/2C validates pan/volume <=128 BEFORE channel range;
// 18/19/26 validate channel only (duration is any 32-bit value).

// Resident sound effects (SoundA0 group). Hand-cleaned from
// src/raw/text_484000.c, text_494000.c, text_4A0000.c.
#include "../include/engine_types.h"

extern uint32_t pop(struct CThread *t);
extern void push(struct CThread *t, uint32_t v);
extern void script_error(const char *msg, struct CThread *t);

// Per-channel copy of the 64-byte BURIKO wave header taken at load
// (sub_494300): +0x0C samples, +0x10 sample rate; +0x3C holds
// (int64)(65536.0 / playback_rate). 64 channels at unk_55EF18.
struct SeInfo { uint8_t header[0x40]; };
extern struct SeInfo g_se_info[64];

// sub_4943E0: length in ms, scaled by the playback rate.
uint32_t se_length_ms(uint32_t channel)
{
    const uint32_t *h = (const uint32_t *)g_se_info[channel].header;
    if (!h[4]) return 0;
    return (uint32_t)(int64_t)((double)h[3] * 1000.0 / (double)h[4] * (double)h[15]) >> 16;
}

// sub_487860. Script order (channel, volume, pan): pan <= 128, volume <= 128,
// channel < 64 (sub_4979A0 / sub_4979F0 / sub_497950). sub_4A34E0: status
// 20 when the sound system is down, 19 when the channel holds nothing; else
// it sets pan and volume, rewinds and plays (23 when playing fails). Pushes
// the length for status 0 and 20, else 0.
int SoundA0_24_PlaySe(struct CThread *t)
{
    uint32_t pan = pop(t), volume = pop(t), channel = pop(t);
    int status;
    if (pan > 0x80) script_error("pan", t);
    if (volume > 0x80) script_error("volume", t);
    if (channel >= 0x40) script_error("SE channel", t);
    status = se_play(channel, volume, pan);
    push(t, status == 0 || status == 20 ? se_length_ms(channel) : 0);
    return 0;
}

// sub_487BA0. Script order (channel). Pushes the SE length (the target's
// "position" query reads only the static header copy).
int SoundA0_2F_GetSeLength(struct CThread *t)
{
    uint32_t channel = pop(t);
    if (channel >= 0x40) script_error("SE channel", t);
    push(t, se_length_ms(channel));
    return 0;
}

// ------------------------------------------------------------------ BGM ---
// 16 channels, 0x44-byte records at dword_5085A8 (stream object +4).
// sub_4A2180: 20 sound system down, 19 channel empty, 21 out of range.

// sub_4873C0. Script order (channel, value). channel >= 16 is fatal;
// sub_4A2AB0 calls the stream's vtbl+12 with pause = (value == 0).
int SoundA0_14_ControlBgm(struct CThread *t)
{
    uint32_t value = pop(t), channel = pop(t);
    if (channel >= 0x10) script_error("BGM channel", t);
    bgm_pause(channel, value == 0);
    return 0;
}

// sub_487450. Script order (channel, volume, ms). volume > 128 and
// channel >= 16 are fatal; sub_4A2AE0 starts a linear ramp from the
// current volume (+0x28) to the target over `ms` (start tick +0x18,
// end tick +0x1C, from +0x20, to +0x24, active +0x14).
int SoundA0_16_SetBgmVolume(struct CThread *t)
{
    uint32_t ms = pop(t), volume = pop(t), channel = pop(t);
    if (volume > 0x80) script_error("volume", t);
    if (channel >= 0x10) script_error("BGM channel", t);
    bgm_ramp_volume(channel, volume, ms);
    return 0;
}

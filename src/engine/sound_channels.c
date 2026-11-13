// Houkago Cinderella FD: raw/houkago_cinderella_fd/sound_channels.asm.
// The older 1.622 engine has 64 SE channels; this engine has 128. BGM still
// has 16 (sub_4A6D40). Array sizes and validation must agree for all SE calls.
#include <stdint.h>

struct CThread;
extern uint32_t pop(struct CThread *t);
extern void script_error(const char *message, struct CThread *t);
extern void set_se_primary_volume(uint32_t channel, uint32_t volume);

// sub_4A6C30: unsigned compare; negative script integers fail as well.
static void check_se_channel(uint32_t channel, struct CThread *t)
{
    if (channel >= 128) script_error("invalid SE channel", t);
}

// A0:09, sub_494900: parameter check before channel check, then sub_4A11F0.
// sub_4A11F0 stores the primary bank at 0x58C370 + 4*channel, with its own
// channel < 128 / volume <= 128 guards; it can store unloaded channels.
int SoundA0_09_SetSePrimaryVolume(struct CThread *t)
{
    uint32_t volume = pop(t), channel = pop(t);
    if (volume > 128) script_error("invalid volume", t);
    check_se_channel(channel, t);
    set_se_primary_volume(channel, volume);
    return 0;
}

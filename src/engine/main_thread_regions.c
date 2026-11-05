// Houkago Cinderella FD: raw/houkago_cinderella_fd/main_thread.asm.
// sub_49A59C -> sub_49AC60 -> sub_44C830 creates the main script thread
// with 4096 operand slots, 8 MiB of code and 4 MiB of frame data. Older
// sub_48C990 uses 4096 / 512 KiB / 256 KiB instead. Child threads retain
// the sizes passed explicitly by Sys80:44, independently of this default.
#include <stdint.h>
extern void *create_script_thread(uint32_t slots, uint32_t code, uint32_t data);

void *create_main_script_thread(void)
{
    return create_script_thread(0x1000, 0x800000, 0x400000);
}

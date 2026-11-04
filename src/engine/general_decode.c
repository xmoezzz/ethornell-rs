// BGI 1.622 Sys80:CF, raw/bgi1622/native_all.{c,asm}.
// 48B130 -> DCProcDecodeData (450D60) -> 499120 -> 465880.
#include <stdint.h>
#include <stddef.h>
#include <string.h>

// Parameter order is destination, source, compressed_length. 48B130 pops
// length, source BP pointer, destination BP pointer. Worker publishes one
// decoded byte count; it is not bounded by compressed_length.
size_t decode_data(void *destination, const void *source, uint32_t length)
{
    extern size_t decode_sdc(const void *,void *);
    extern int decode_dsc(const void *,void *,size_t *);
    extern int decode_cbg_v1(const void *,void *);
    extern int decode_cbg_v2(const void *,void *);
    const uint8_t *s=source;
    if (!strcmp(source,"SDC FORMAT 1.00")) return decode_sdc(source,destination);
    if (!strcmp(source,"DSC FORMAT 1.00")) {
        uint32_t size=*(const uint32_t *)(s+20);
        if (size>0x4000000) return 0;
        size_t written=0;
        return decode_dsc(source,destination,&written) ? 0 : written;
    }
    if (!strcmp(source,"CompressedBG___")) {
        unsigned width=*(const uint16_t *)(s+16),height=*(const uint16_t *)(s+18);
        unsigned bpp=*(const uint16_t *)(s+20);
        unsigned bytes=bpp==24 ? 4 : bpp>>3;
        int status=(s[46]==2 && s[47]==0) ? decode_cbg_v2(source,destination) : decode_cbg_v1(source,destination);
        // Output begins with s[16..32]. V1 24-bit expands BGR to BGR0,
        // rewrites bpp=32/subtype=7. V2 retains packed 24-bit and reports
        // the four-byte allocation, leaving an uninitialized native tail.
        // The portable version initializes only that otherwise unread tail.
        return status ? 0 : 16+width*height*bytes;
    }
    memcpy(destination,source,length);
    return length;
}

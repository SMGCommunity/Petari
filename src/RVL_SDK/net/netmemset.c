#include "revolution/types.h"
#include <cstdio>

void* NETMemSet(void* buf, int ch, u32 size) {
    u8* p;
    u8* ret;
    u32 v;
    u32 align;
    u32 head;
    u32 blocks;
    u32 words;
    u32 bytes;

    ret = (u8*)buf;
    p = (u8*)buf;

    if (size == 0)
        return ret;

    v = (u8)ch;
    v |= v << 8;
    v |= v << 16;

    if (size >= 0x40) {
        align = (u32)p & 0x1F;

        if (align) {
            head = 32 - align;
            if (head > size)
                head = size;

            words = head >> 2;
            while (words--) {
                *(u32*)p = v;
                p += 4;
            }

            bytes = head & 3;
            while (bytes--) {
                *p++ = (u8)v;
            }

            size -= head;
        }

        blocks = size >> 5;

        if (v != 0) {
            while (blocks--) {
                __dcbz(p, 0);

                ((u32*)p)[0] = v;
                ((u32*)p)[1] = v;
                ((u32*)p)[2] = v;
                ((u32*)p)[3] = v;
                ((u32*)p)[4] = v;
                ((u32*)p)[5] = v;
                ((u32*)p)[6] = v;
                ((u32*)p)[7] = v;

                p += 32;
            }
        } else {
            while (blocks--) {
                __dcbz(p, 0);
                p += 32;
            }
        }

        size &= 31;
    }

    words = size >> 2;
    while (words--) {
        *(u32*)p = v;
        p += 4;
    }

    bytes = size & 3;
    while (bytes--) {
        *p++ = (u8)v;
    }

    return ret;
}

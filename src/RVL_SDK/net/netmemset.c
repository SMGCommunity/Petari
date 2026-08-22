#include "revolution/types.h"
#include <cstdio>

void* NETMemSet(void* buf, int ch, unsigned long size) {
    unsigned long headSize;
    unsigned long accBlkSize;
    unsigned long fill;
    unsigned char* buf_u8;
    unsigned long numWords;
    unsigned long remainBytes;

    if (size == 0) {
        return buf;
    }

    fill = (unsigned char)ch;
    fill |= fill << 8;
    fill |= fill << 16;

    if (size >= 0x40) {
        headSize = (unsigned long)buf & 0x1F;
        if (headSize != 0) {
            headSize = 0x20 - headSize;

            buf_u8 = (unsigned char*)buf;
            numWords = headSize >> 2;
            remainBytes = headSize & 3;
            while (numWords--) {
                *(unsigned long*)buf_u8 = fill;
                buf_u8 += 4;
            }
            while (remainBytes--) {
                *buf_u8++ = (unsigned char)fill;
            }

            buf = (unsigned char*)buf + headSize;
            size -= headSize;
        }

        accBlkSize = size & ~0x1F;

        {
            unsigned char* p = (unsigned char*)buf;
            unsigned long loopSize = size >> 5;

            if (fill != 0) {
                while (loopSize--) {
                    __dcbz(p, 0);
                    ((unsigned long*)p)[0] = fill;
                    ((unsigned long*)p)[1] = fill;
                    ((unsigned long*)p)[2] = fill;
                    ((unsigned long*)p)[3] = fill;
                    ((unsigned long*)p)[4] = fill;
                    ((unsigned long*)p)[5] = fill;
                    ((unsigned long*)p)[6] = fill;
                    ((unsigned long*)p)[7] = fill;
                    p += 0x20;
                }
            } else {
                while (loopSize--) {
                    __dcbz(p, 0);
                    p += 0x20;
                }
            }
        }

        buf = (unsigned char*)buf + accBlkSize;
        size -= accBlkSize;
    }

    buf_u8 = (unsigned char*)buf;
    numWords = size >> 2;
    remainBytes = size & 3;
    while (numWords--) {
        *(unsigned long*)buf_u8 = fill;
        buf_u8 += 4;
    }
    while (remainBytes--) {
        *buf_u8++ = (unsigned char)fill;
    }

    return buf;
}

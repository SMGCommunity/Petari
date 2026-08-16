#include "revolution/types.h"

static void NETMemCpy_SimpleRev(void* dst, const void* src, unsigned long size) {
    unsigned char* dst_u8 = (unsigned char*)dst + size;
    const unsigned char* src_u8 = (const unsigned char*)src + size;
    unsigned long numWords = size >> 2;
    unsigned long remainBytes = size & 3;
    unsigned long tmp;

    while (remainBytes--) {
        tmp = *--src_u8;
        *--dst_u8 = (unsigned char)tmp;
    }

    while (numWords--) {
        src_u8 -= 4;
        tmp = *(unsigned long*)src_u8;
        dst_u8 -= 4;
        *(unsigned long*)dst_u8 = tmp;
    }
}

void* NETMemCpy(void* dst, const void* src, unsigned long size) {
    unsigned long headSize;
    unsigned long accBlkSize;
    unsigned char* dst_u8;
    const unsigned char* src_u8;
    unsigned long numWords;
    unsigned long remainBytes;
    unsigned long tmp;

    if (dst == src) {
        return dst;
    }

    if ((unsigned char*)dst > (unsigned char*)src && (unsigned char*)dst < (unsigned char*)src + size) {
        NETMemCpy_SimpleRev(dst, src, size);
        return dst;
    }

    if (((unsigned char*)dst <= (unsigned char*)src - 0x20 || (unsigned char*)dst >= (unsigned char*)src) && size >= 0x40) {
        headSize = (unsigned long)dst & 0x1F;
        if (headSize != 0) {
            headSize = 0x20 - headSize;
            dst_u8 = (unsigned char*)dst;
            src_u8 = (const unsigned char*)src;
            numWords = headSize >> 2;
            remainBytes = headSize & 3;
            while (numWords--) {
                tmp = *(unsigned long*)src_u8;
                src_u8 += 4;
                *(unsigned long*)dst_u8 = tmp;
                dst_u8 += 4;
            }
            while (remainBytes--) {
                tmp = *src_u8++;
                *dst_u8++ = (unsigned char)tmp;
            }

            dst = (unsigned char*)dst + headSize;
            src = (unsigned char*)src + headSize;
            size -= headSize;
        }

        accBlkSize = size & ~0x1F;

        {
            unsigned char* p = (unsigned char*)dst;
            const unsigned char* q = (const unsigned char*)src;
            unsigned long shift = (unsigned long)q & 3;
            unsigned long loopSize = size >> 5;
            unsigned long tmp0, tmp1, tmp2, tmp3, tmp4, tmp5;
            unsigned long const32;

            switch (shift) {
            case 0:
                while (loopSize--) {
                    __dcbz(p, 0);
                    tmp0 = ((unsigned long*)q)[0];
                    tmp1 = ((unsigned long*)q)[1];
                    tmp2 = ((unsigned long*)q)[2];
                    tmp3 = ((unsigned long*)q)[3];
                    tmp4 = ((unsigned long*)q)[4];
                    tmp5 = ((unsigned long*)q)[5];
                    ((unsigned long*)p)[0] = tmp0;
                    ((unsigned long*)p)[1] = tmp1;
                    ((unsigned long*)p)[2] = tmp2;
                    ((unsigned long*)p)[3] = tmp3;
                    ((unsigned long*)p)[4] = tmp4;
                    ((unsigned long*)p)[5] = tmp5;
                    ((unsigned long*)p)[6] = ((unsigned long*)q)[6];
                    ((unsigned long*)p)[7] = ((unsigned long*)q)[7];
                    q += 0x20;
                    p += 0x20;
                }
                break;
            case 1:
                const32 = *(unsigned long*)(q - 1) << 8;
                q += 3;
                while (loopSize--) {
                    __dcbz(p, 0);
                    tmp0 = ((unsigned long*)q)[0];
                    tmp1 = ((unsigned long*)q)[1];
                    ((unsigned long*)p)[0] = (const32 & 0xFFFFFF00) | (tmp0 >> 24);
                    tmp2 = ((unsigned long*)q)[2];
                    ((unsigned long*)p)[1] = ((tmp0 << 8) & 0xFFFFFF00) | (tmp1 >> 24);
                    tmp3 = ((unsigned long*)q)[3];
                    ((unsigned long*)p)[2] = ((tmp1 << 8) & 0xFFFFFF00) | (tmp2 >> 24);
                    tmp4 = ((unsigned long*)q)[4];
                    ((unsigned long*)p)[3] = ((tmp2 << 8) & 0xFFFFFF00) | (tmp3 >> 24);
                    tmp5 = ((unsigned long*)q)[5];
                    ((unsigned long*)p)[4] = ((tmp3 << 8) & 0xFFFFFF00) | (tmp4 >> 24);
                    tmp0 = ((unsigned long*)q)[6];
                    ((unsigned long*)p)[5] = ((tmp4 << 8) & 0xFFFFFF00) | (tmp5 >> 24);
                    tmp1 = ((unsigned long*)q)[7];
                    ((unsigned long*)p)[6] = ((tmp5 << 8) & 0xFFFFFF00) | (tmp0 >> 24);
                    q += 0x20;
                    const32 = tmp1 << 8;
                    ((unsigned long*)p)[7] = ((tmp0 << 8) & 0xFFFFFF00) | (tmp1 >> 24);
                    p += 0x20;
                }
                break;
            case 2:
                const32 = *(unsigned long*)(q - 2) << 16;
                q += 2;
                while (loopSize--) {
                    __dcbz(p, 0);
                    tmp0 = ((unsigned long*)q)[0];
                    tmp1 = ((unsigned long*)q)[1];
                    ((unsigned long*)p)[0] = (const32 & 0xFFFF0000) | (tmp0 >> 16);
                    tmp2 = ((unsigned long*)q)[2];
                    ((unsigned long*)p)[1] = ((tmp0 << 16) & 0xFFFF0000) | (tmp1 >> 16);
                    tmp3 = ((unsigned long*)q)[3];
                    ((unsigned long*)p)[2] = ((tmp1 << 16) & 0xFFFF0000) | (tmp2 >> 16);
                    tmp4 = ((unsigned long*)q)[4];
                    ((unsigned long*)p)[3] = ((tmp2 << 16) & 0xFFFF0000) | (tmp3 >> 16);
                    tmp5 = ((unsigned long*)q)[5];
                    ((unsigned long*)p)[4] = ((tmp3 << 16) & 0xFFFF0000) | (tmp4 >> 16);
                    tmp0 = ((unsigned long*)q)[6];
                    ((unsigned long*)p)[5] = ((tmp4 << 16) & 0xFFFF0000) | (tmp5 >> 16);
                    tmp1 = ((unsigned long*)q)[7];
                    ((unsigned long*)p)[6] = ((tmp5 << 16) & 0xFFFF0000) | (tmp0 >> 16);
                    q += 0x20;
                    const32 = tmp1 << 16;
                    ((unsigned long*)p)[7] = ((tmp0 << 16) & 0xFFFF0000) | (tmp1 >> 16);
                    p += 0x20;
                }
                break;
            case 3:
                const32 = *(unsigned long*)(q - 3) << 24;
                q += 1;
                while (loopSize--) {
                    __dcbz(p, 0);
                    tmp0 = ((unsigned long*)q)[0];
                    tmp1 = ((unsigned long*)q)[1];
                    ((unsigned long*)p)[0] = (const32 & 0xFF000000) | (tmp0 >> 8);
                    tmp2 = ((unsigned long*)q)[2];
                    ((unsigned long*)p)[1] = ((tmp0 << 24) & 0xFF000000) | (tmp1 >> 8);
                    tmp3 = ((unsigned long*)q)[3];
                    ((unsigned long*)p)[2] = ((tmp1 << 24) & 0xFF000000) | (tmp2 >> 8);
                    tmp4 = ((unsigned long*)q)[4];
                    ((unsigned long*)p)[3] = ((tmp2 << 24) & 0xFF000000) | (tmp3 >> 8);
                    tmp5 = ((unsigned long*)q)[5];
                    ((unsigned long*)p)[4] = ((tmp3 << 24) & 0xFF000000) | (tmp4 >> 8);
                    tmp0 = ((unsigned long*)q)[6];
                    ((unsigned long*)p)[5] = ((tmp4 << 24) & 0xFF000000) | (tmp5 >> 8);
                    tmp1 = ((unsigned long*)q)[7];
                    ((unsigned long*)p)[6] = ((tmp5 << 24) & 0xFF000000) | (tmp0 >> 8);
                    q += 0x20;
                    const32 = tmp1 << 24;
                    ((unsigned long*)p)[7] = ((tmp0 << 24) & 0xFF000000) | (tmp1 >> 8);
                    p += 0x20;
                }
                break;
            }
        }

        dst = (unsigned char*)dst + accBlkSize;
        src = (unsigned char*)src + accBlkSize;
        size -= accBlkSize;
    }

    dst_u8 = (unsigned char*)dst;
    src_u8 = (const unsigned char*)src;
    numWords = size >> 2;
    remainBytes = size & 3;
    while (numWords--) {
        tmp = *(unsigned long*)src_u8;
        src_u8 += 4;
        *(unsigned long*)dst_u8 = tmp;
        dst_u8 += 4;
    }
    while (remainBytes--) {
        tmp = *src_u8++;
        *dst_u8++ = (unsigned char)tmp;
    }

    return dst;
}

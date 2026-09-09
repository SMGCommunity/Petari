#include "revolution/types.h"

static inline void NETMemCpy_DstSize32Aligned(register void* dst, register const void* src, unsigned long size) {
    register unsigned long tmp0;
    register unsigned long tmp1;
    register unsigned long tmp2;
    register unsigned long tmp3;
    register unsigned long tmp4;
    register unsigned long tmp5;
    register unsigned long loopSize = size >> 5;
    unsigned long shift = (unsigned long)src & 3;

    switch (shift) {
    case 0:
        asm {
            mtctr loopSize
        aligned_loop:
            dcbz 0, dst
            lwz tmp0, 0(src)
            lwz tmp1, 4(src)
            stw tmp0, 0(dst)
            stw tmp1, 4(dst)
            lwz tmp0, 8(src)
            lwz tmp1, 12(src)
            stw tmp0, 8(dst)
            stw tmp1, 12(dst)
            lwz tmp0, 16(src)
            lwz tmp1, 20(src)
            stw tmp0, 16(dst)
            stw tmp1, 20(dst)
            lwz tmp0, 24(src)
            lwz tmp1, 28(src)
            stw tmp0, 24(dst)
            stw tmp1, 28(dst)
            addi src, src, 32
            addi dst, dst, 32
            bdnz aligned_loop
        }
        break;
    case 1:
        src = (const unsigned char*)src - 1;
        asm {
            lwz tmp2, 0(src)
            mtctr loopSize
            slwi tmp2, tmp2, 8
            addi src, src, 4
        shift8_loop:
            dcbz 0, dst
            lwz tmp0, 0(src)
            slwi tmp3, tmp0, 8
            rlwimi tmp2, tmp0, 8, 24, 31
            lwz tmp1, 4(src)
            stw tmp2, 0(dst)
            slwi tmp4, tmp1, 8
            rlwimi tmp3, tmp1, 8, 24, 31
            lwz tmp0, 8(src)
            stw tmp3, 4(dst)
            slwi tmp5, tmp0, 8
            rlwimi tmp4, tmp0, 8, 24, 31
            lwz tmp1, 12(src)
            stw tmp4, 8(dst)
            slwi tmp2, tmp1, 8
            rlwimi tmp5, tmp1, 8, 24, 31
            lwz tmp0, 16(src)
            stw tmp5, 12(dst)
            slwi tmp3, tmp0, 8
            rlwimi tmp2, tmp0, 8, 24, 31
            lwz tmp1, 20(src)
            stw tmp2, 16(dst)
            slwi tmp4, tmp1, 8
            rlwimi tmp3, tmp1, 8, 24, 31
            lwz tmp0, 24(src)
            stw tmp3, 20(dst)
            slwi tmp5, tmp0, 8
            rlwimi tmp4, tmp0, 8, 24, 31
            lwz tmp1, 28(src)
            stw tmp4, 24(dst)
            slwi tmp2, tmp1, 8
            rlwimi tmp5, tmp1, 8, 24, 31
            addi src, src, 32
            stw tmp5, 28(dst)
            addi dst, dst, 32
            bdnz shift8_loop
        }
        break;
    case 2:
        src = (const unsigned char*)src - 2;
        asm {
            lwz tmp2, 0(src)
            mtctr loopSize
            slwi tmp2, tmp2, 16
            addi src, src, 4
        shift16_loop:
            dcbz 0, dst
            lwz tmp0, 0(src)
            slwi tmp3, tmp0, 16
            rlwimi tmp2, tmp0, 16, 16, 31
            lwz tmp1, 4(src)
            stw tmp2, 0(dst)
            slwi tmp4, tmp1, 16
            rlwimi tmp3, tmp1, 16, 16, 31
            lwz tmp0, 8(src)
            stw tmp3, 4(dst)
            slwi tmp5, tmp0, 16
            rlwimi tmp4, tmp0, 16, 16, 31
            lwz tmp1, 12(src)
            stw tmp4, 8(dst)
            slwi tmp2, tmp1, 16
            rlwimi tmp5, tmp1, 16, 16, 31
            lwz tmp0, 16(src)
            stw tmp5, 12(dst)
            slwi tmp3, tmp0, 16
            rlwimi tmp2, tmp0, 16, 16, 31
            lwz tmp1, 20(src)
            stw tmp2, 16(dst)
            slwi tmp4, tmp1, 16
            rlwimi tmp3, tmp1, 16, 16, 31
            lwz tmp0, 24(src)
            stw tmp3, 20(dst)
            slwi tmp5, tmp0, 16
            rlwimi tmp4, tmp0, 16, 16, 31
            lwz tmp1, 28(src)
            stw tmp4, 24(dst)
            slwi tmp2, tmp1, 16
            rlwimi tmp5, tmp1, 16, 16, 31
            addi src, src, 32
            stw tmp5, 28(dst)
            addi dst, dst, 32
            bdnz shift16_loop
        }
        break;
    case 3:
        src = (const unsigned char*)src - 3;
        asm {
            lwz tmp2, 0(src)
            mtctr loopSize
            slwi tmp2, tmp2, 24
            addi src, src, 4
        shift24_loop:
            dcbz 0, dst
            lwz tmp0, 0(src)
            slwi tmp3, tmp0, 24
            rlwimi tmp2, tmp0, 24, 8, 31
            lwz tmp1, 4(src)
            stw tmp2, 0(dst)
            slwi tmp4, tmp1, 24
            rlwimi tmp3, tmp1, 24, 8, 31
            lwz tmp0, 8(src)
            stw tmp3, 4(dst)
            slwi tmp5, tmp0, 24
            rlwimi tmp4, tmp0, 24, 8, 31
            lwz tmp1, 12(src)
            stw tmp4, 8(dst)
            slwi tmp2, tmp1, 24
            rlwimi tmp5, tmp1, 24, 8, 31
            lwz tmp0, 16(src)
            stw tmp5, 12(dst)
            slwi tmp3, tmp0, 24
            rlwimi tmp2, tmp0, 24, 8, 31
            lwz tmp1, 20(src)
            stw tmp2, 16(dst)
            slwi tmp4, tmp1, 24
            rlwimi tmp3, tmp1, 24, 8, 31
            lwz tmp0, 24(src)
            stw tmp3, 20(dst)
            slwi tmp5, tmp0, 24
            rlwimi tmp4, tmp0, 24, 8, 31
            lwz tmp1, 28(src)
            stw tmp4, 24(dst)
            slwi tmp2, tmp1, 24
            rlwimi tmp5, tmp1, 24, 8, 31
            addi src, src, 32
            stw tmp5, 28(dst)
            addi dst, dst, 32
            bdnz shift24_loop
        }
        break;
    }
}

static inline void NETMemCpy_SimpleFwd(void* dst, const void* src, unsigned long size) {
    register unsigned char* dst_u8;
    register const unsigned char* src_u8;
    register unsigned long numWords;
    register unsigned long remainBytes;
    register unsigned long tmp;

    src_u8 = (const unsigned char*)src;
    dst_u8 = (unsigned char*)dst;
    numWords = size >> 2;
    remainBytes = size & 3;
    if (numWords != 0) {
        asm {
            mtctr numWords
        word_loop:
            lwz tmp, 0(src_u8)
            addi src_u8, src_u8, 4
            stw tmp, 0(dst_u8)
            addi dst_u8, dst_u8, 4
            bdnz word_loop
        }
    }
    if (remainBytes != 0) {
        asm {
            mtctr remainBytes
        byte_loop:
            lbz tmp, 0(src_u8)
            addi src_u8, src_u8, 1
            stb tmp, 0(dst_u8)
            addi dst_u8, dst_u8, 1
            bdnz byte_loop
        }
    }
}

static inline void NETMemCpy_SimpleRev(void* dst, const void* src, unsigned long size) {
    register unsigned char* dst_u8;
    register const unsigned char* src_u8;
    register unsigned long numWords;
    register unsigned long remainBytes;
    register unsigned long tmp;

    src_u8 = (const unsigned char*)src + size;
    dst_u8 = (unsigned char*)dst + size;
    numWords = size >> 2;
    remainBytes = size & 3;
    if (remainBytes != 0) {
        asm {
            mtctr remainBytes
        byte_loop:
            lbzu tmp, -1(src_u8)
            stbu tmp, -1(dst_u8)
            bdnz byte_loop
        }
    }
    if (numWords != 0) {
        asm {
            mtctr numWords
        word_loop:
            lwzu tmp, -4(src_u8)
            stwu tmp, -4(dst_u8)
            bdnz word_loop
        }
    }
}

void* NETMemCpy(void* dst, const void* src, unsigned long size) {
    unsigned long headSize;
    unsigned long accBlkSize;

    if (dst == src) {
        return dst;
    }

    if ((unsigned char*)dst > (const unsigned char*)src && (unsigned char*)dst < (const unsigned char*)src + size) {
        NETMemCpy_SimpleRev(dst, src, size);
        return dst;
    }

    if (((unsigned char*)dst <= (const unsigned char*)src - 0x20 || (unsigned char*)dst >= (const unsigned char*)src) && size >= 0x40) {
        headSize = (unsigned long)dst & 0x1F;
        if (headSize != 0) {
            headSize = 0x20 - headSize;
            NETMemCpy_SimpleFwd(dst, src, headSize);
            dst = (unsigned char*)dst + headSize;
            src = (const unsigned char*)src + headSize;
            size -= headSize;
        }

        accBlkSize = size & ~0x1F;
        NETMemCpy_DstSize32Aligned(dst, src, accBlkSize);
        dst = (unsigned char*)dst + accBlkSize;
        src = (const unsigned char*)src + accBlkSize;
        size -= accBlkSize;
    }

    NETMemCpy_SimpleFwd(dst, src, size);
    return dst;
}

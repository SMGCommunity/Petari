#include "revolution/types.h"

static inline void NETMemSet_BufSize32Aligned(register void* buf, register unsigned long fill, unsigned long size) {
    register unsigned long loopSize = size >> 5;

    if (fill == 0) {
        asm {
            mtctr loopSize
        zero_loop:
            dcbz 0, buf
            addi buf, buf, 32
            bdnz zero_loop
        }
    } else {
        asm {
            mtctr loopSize
        fill_loop:
            dcbz 0, buf
            stw fill, 0(buf)
            stw fill, 4(buf)
            stw fill, 8(buf)
            stw fill, 12(buf)
            stw fill, 16(buf)
            stw fill, 20(buf)
            stw fill, 24(buf)
            stw fill, 28(buf)
            addi buf, buf, 32
            bdnz fill_loop
        }
    }
}

static inline void NETMemSet_Simple(register void* buf, register unsigned long fill, unsigned long size) {
    register unsigned char* buf_u8 = (unsigned char*)buf;
    register unsigned long numWords = size >> 2;
    register unsigned long remainBytes = size & 3;

    if (numWords != 0) {
        asm {
            mtctr numWords
        word_loop:
            stw fill, 0(buf_u8)
            addi buf_u8, buf_u8, 4
            bdnz word_loop
        }
    }
    if (remainBytes != 0) {
        asm {
            mtctr remainBytes
        byte_loop:
            stb fill, 0(buf_u8)
            addi buf_u8, buf_u8, 1
            bdnz byte_loop
        }
    }
}

void* NETMemSet(void* buf, int ch, unsigned long size) {
    unsigned long headSize;
    unsigned long accBlkSize;
    register unsigned long fill = ch;

    if (size == 0) {
        return buf;
    }

    asm {
        rlwimi fill, fill, 8, 16, 23
        rlwimi fill, fill, 16, 0, 15
    }

    if (size >= 0x40) {
        headSize = (unsigned long)buf & 0x1F;
        if (headSize != 0) {
            headSize = 0x20 - headSize;
            NETMemSet_Simple(buf, fill, headSize);
            buf = (unsigned char*)buf + headSize;
            size -= headSize;
        }

        accBlkSize = size & ~0x1F;
        NETMemSet_BufSize32Aligned(buf, fill, accBlkSize);
        buf = (unsigned char*)buf + accBlkSize;
        size -= accBlkSize;
    }

    NETMemSet_Simple(buf, fill, size);
    return buf;
}

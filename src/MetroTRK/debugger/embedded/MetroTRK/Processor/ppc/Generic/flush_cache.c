#include "TRK_Types.h"

asm void TRK_flush_cache(u32, u32) {
    nofralloc

    lis       r5, 0xffff
    ori       r5, r5, 0xfff1
    and       r5, r5, r3
    subf      r3, r5, r3
    add       r4, r4, r3

rept:
    dcbst     0, r5
    dcbf      0, r5
    sync
    icbi      0, r5
    addic     r5, r5, 0x8
    subic.    r4, r4, 0x8
    bge       rept
    isync
    blr
}
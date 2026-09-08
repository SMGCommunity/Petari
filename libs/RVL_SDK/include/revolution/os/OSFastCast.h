#ifndef OSFASTCAST_H
#define OSFASTCAST_H

#include "revolution/types.h"

#ifdef __cplusplus
extern "C" {
#endif

#define OSu16tof32(in, out) asm volatile("psq_l   %0, 0(%1), 1, 3  " : "=f"(*(out)) : "b"(in))

#ifndef __MWERKS__
static u16 __OSf32tou16(f32 arg);
#else
static u16 __OSf32tou16(register f32 arg) {
    f32 a;
    register f32* ptr = &a;
    u16 r;

    // clang-format off
    __asm {
        psq_st arg, 0(ptr), 1, 3
    };

    // clang-format on

    r = *(u16*)ptr;
    return r;
}
#endif

static void OSf32tou16(f32* in, volatile u16* out) {
    *out = __OSf32tou16(*in);
}

static inline void OSInitFastCast(void) {
#ifdef __MWERKS__
    asm
    {
        li      r3, 4
        oris    r3, r3, 4
        mtspr   0x392, r3

        li      r3, 5
        oris    r3, r3, 5
        mtspr   0x393, r3

        li      r3, 6
        oris    r3, r3, 6
        mtspr   0x394, r3

        li      r3, 7
        oris    r3, r3, 7
        mtspr   0x395, r3
    }
#endif
}

#ifdef __cplusplus
}
#endif

#endif  // OSFASTCAST_H

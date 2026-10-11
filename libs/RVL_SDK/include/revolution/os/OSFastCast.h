#ifndef OSFASTCAST_H
#define OSFASTCAST_H

#include "revolution/types.h"

#ifdef __cplusplus
extern "C" {
#endif

#ifdef __MWERKS__
static inline u16 __OSf32tou16(register f32 in) {
    f32 a;
    register f32* ptr = &a;
    register u16 r;
    asm {
        psq_st in, 0(ptr), 1, 3
        lhz r, 0(ptr)
    }
    return r;
}

static inline void OSf32tou16(register f32* in, volatile register u16* out) {
    *out = __OSf32tou16(*in);
}
#elif defined(__powerpc__) || defined(__ppc__)
#define OSf32tou16(in, out) asm volatile("psq_st   %1, 0(%0), 1, 3 " : : "b"(out), "f"(*(in)) : "memory")
#else
// 3DS port: portable equivalents of the Gekko quantized load/store (GQR3 = u16, no scale).
// psq_st to u16 truncates toward zero and saturates to [0, 65535].
static inline void OSf32tou16(const f32* in, volatile u16* out) {
    f32 v = *in;
    *out = !(v > 0.0f) ? (u16)0 : (v >= 65535.0f ? (u16)65535 : (u16)v);
}
#endif
#if defined(__MWERKS__) || defined(__powerpc__) || defined(__ppc__)
#define OSu16tof32(in, out) asm volatile("psq_l   %0, 0(%1), 1, 3  " : "=f"(*(out)) : "b"(in))
#else
static inline void OSu16tof32(const volatile u16* in, f32* out) {
    *out = (f32)*in;
}
#endif

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

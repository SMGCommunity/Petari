#ifndef OSFASTCAST_H
#define OSFASTCAST_H

#ifdef __cplusplus
extern "C" {
#endif

#define OSf32tou16(in, out) asm volatile("psq_st   %1, 0(%0), 1, 3 " : : "b"(out), "f"(*(in)) : "memory")
#define OSu16tof32(in, out) asm volatile("psq_l   %0, 0(%1), 1, 3  " : "=f"(*(out)) : "b"(in))

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

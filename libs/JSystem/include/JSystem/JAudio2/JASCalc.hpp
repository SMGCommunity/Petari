#pragma once

#include <revolution/types.h>

class JASCalc {
public:
    static void imixcopy(const s16*, const s16*, s16*, u32);
    static void bcopyfast(const void*, void*, u32);
    static void bcopy(const void*, void*, u32);
    static void bzerofast(void*, u32);
    static void bzero(void*, u32);
    static f32 pow2(f32);

    static const s16 CUTOFF_TO_IIR_TABLE[128][4];
};

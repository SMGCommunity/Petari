#pragma once

#include <revolution.h>
#include "JSystem/JKernel/JKRHeap.h"

class SpkMixingBuffer {
public:
    SpkMixingBuffer(JKRHeap *);

    void mix(s32, s16 *, s32, f32, s32);
    const s16* getSamples(s32) const;
    void update(s32);

    const s16* mSamples[4];
};
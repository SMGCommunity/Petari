#pragma once

#include <revolution/types.h>

class JKRHeap;

class AudEffector {
public:
    /// @brief Creates a new `AudEffector`.
    AudEffector();

    void initFxLine(JKRHeap*);
    void setFxSend(f32);
    void setCutoff(s32);
    void setSpecialFx(s32);
    void initParams(s32);

    /* 0x00 */ s16* _0[4];
    /* 0x10 */ f32 _10;
    /* 0x14 */ s32 _14;
    /* 0x18 */ s32 mSpecialFx;
};

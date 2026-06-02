#pragma once

#include <revolution/types.h>

struct JASLfo {
    JASLfo();
    f32 getValue() const;
    void incCounter(f32);
    void resetCounter();

    void setDepth(f32 param_0) {
        mDepth = param_0;
    }
    void setPitch(f32 param_0) {
        mPitch = param_0;
    }
    void setDelay(u16 param_0) {
        mDelay = param_0;
    }

    static void updateFreeRun(f32 param_0) {
        sFreeRunLfo.incCounter(param_0);
    }

    static JASLfo sFreeRunLfo;

    /* 0x00 */ u32 _0;
    /* 0x04 */ u32 _4;
    /* 0x08 */ f32 _8;
    /* 0x0C */ f32 mDepth;
    /* 0x10 */ f32 mPitch;
    /* 0x14 */ u16 mDelay;
    /* 0x16 */ u16 _16;
};

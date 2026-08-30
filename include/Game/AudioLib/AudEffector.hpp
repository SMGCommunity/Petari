#pragma once

#include <JSystem/JAudio2/JASDSPInterface.hpp>

class JKRHeap;

class AudEffector {
public:
    /// @brief Creates a new `AudEffector`.
    AudEffector();

    void initFxLine(JKRHeap*);
    void setFxSend(f32);
    void setCutoff(s32);
    void setSpecialFx(s32);
    void initParams();

    static JASDsp::FxlineConfig_ mFxLine[];

    /* 0x00 */ s16* mFxLines[4];
    /* 0x10 */ f32 mFxSend;
    /* 0x14 */ s32 mCutoff;
    /* 0x18 */ s32 mSpecialFx;
};

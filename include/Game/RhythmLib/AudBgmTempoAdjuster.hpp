#pragma once

#include <revolution.h>

class JAIStream;

class AudBgmTempoAdjuster {
public:
    AudBgmTempoAdjuster();

    void init();
    void setInfo(f32, u32, u32, u32, u32);
    void setStream(JAIStream*);
    void reject();
    f32 beatProc(f32);
    f32 calcAdjustRate(f32, f32);

    /* 0x00 */ JAIStream* mStream;
    /* 0x04 */ f32 mBeatMul;
    /* 0x08 */ u32 mIntroBeats;
    /* 0x0C */ u32 mLoopBeats;
    /* 0x10 */ u32 mLoopStartSamples;
    /* 0x14 */ u32 mLoopEndSamples;
    /* 0x18 */ f32 mLoopSamples;
    /* 0x1C */ u32 mLoopCount;
    /* 0x20 */ u32 mCurrentBeat;
    /* 0x24 */ bool mIsBeatNeedSync;
    /* 0x25 */ bool mIsStreamNeedSync;
};

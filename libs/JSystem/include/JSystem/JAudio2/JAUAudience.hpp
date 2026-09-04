#pragma once

#include <revolution/types.h>

class JAUAudience3DSetting {
public:
    /// @brief Creates a new `JAUAudience3DSetting`.
    JAUAudience3DSetting() {
        init();
    }

    void init();

    /* 0x00 */ f32 mFarDistance;
    /* 0x04 */ f32 mNearDistance;
    /* 0x08 */ f32 mMinVolume;
    /* 0x0C */ f32 mFrontDolbyZ;
    /* 0x10 */ f32 mBehindDolbyZ;
    /* 0x14 */ f32 mMaxDolby;
    /* 0x18 */ f32 mCenterDolby;
    /* 0x1C */ f32 mMinDolby;
    /* 0x20 */ f32 mMinFxMix;
    /* 0x24 */ f32 mMaxFxMix;
    /* 0x28 */ f32 mPanAmplitude;
    /* 0x2C */ f32 mSonicSpeed;
    /* 0x30 */ f32 mPitchDeltaRatioRange;
    /* 0x34 */ u32 mPriorityDownRange;
};

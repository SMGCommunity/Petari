#pragma once

#include "Game/Util/Color.hpp"

class JMapInfoIter;

class AutoEffectInfo {
public:
    /// @brief Creates a new `AutoEffectInfo`.
    AutoEffectInfo();

    void init(const JMapInfoIter& rIter);
    const char* getName() const;

    /* 0x00 */ const char* mGroupName;
    /* 0x04 */ const char* mAnimName;
    /* 0x08 */ const char* mUniqueName;
    /* 0x0C */ const char* mEffectName;
    /* 0x10 */ const char* mParentName;
    /* 0x14 */ const char* mJointName;
    /* 0x18 */ u16 mFlag;
    /* 0x1C */ Color8 mPrmColor;
    /* 0x20 */ bool mIsValidPrmColor;
    /* 0x24 */ Color8 mEnvColor;
    /* 0x28 */ bool mIsValidEnvColor;
    /* 0x2C */ f32 mOffsetX;
    /* 0x30 */ f32 mOffsetY;
    /* 0x34 */ f32 mOffsetZ;
    /* 0x38 */ s32 mStartFrame;
    /* 0x3C */ s32 mEndFrame;
    /* 0x40 */ f32 mScaleValue;
    /* 0x44 */ f32 mRateValue;
    /* 0x48 */ f32 mLightAffectValue;
    /* 0x4C */ const char* mDrawOrder;
};

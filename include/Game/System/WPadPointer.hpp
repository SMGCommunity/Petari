#pragma once

#include "Game/System/WPad.hpp"
#include <JSystem/JGeometry.hpp>

class WPadPointer {
public:
    WPadPointer(const WPad*);

    void reset();
    void setSensorBarLevel(f32);
    void update();
    void getPointingPos(TVec2f*) const;
    void getHorizonVec(TVec2f*) const;
    void getPastPointingPos(TVec2f*, s32) const;
    s32 getEnablePastCount() const;
    void getPointingPosBasedOnScreen(TVec2f*) const;

    /* 0x00 */ const WPad* mPad;
    /* 0x04 */ TVec2f* mPointingPosArray;
    /* 0x08 */ TVec2f* mHorizonArray;
    /* 0x0C */ s32 mMaxPastCount;
    /* 0x10 */ u32 _10;
    /* 0x14 */ f32 mPosPlayRadius;
    /* 0x18 */ f32 mPosSensitivity;
    /* 0x1C */ f32 mHoriPlayRadius;
    /* 0x20 */ f32 mHoriSensitivity;
    /* 0x24 */ f32 mDistPlayRadius;
    /* 0x28 */ f32 mDistSensitivity;
    /* 0x2C */ s32 mLostSensorTime;
    /* 0x30 */ f32 mDistDisplay;
    /* 0x34 */ s8 mDpdValidFg;
    /* 0x38 */ s32 mValidInScreenTime;
    /* 0x3C */ s32 mInValidOutScreenTime;
    /* 0x40 */ s32 mEnablePastCount;
    /* 0x44 */ bool mIsPointInScreen;
    /* 0x45 */ bool mIsPointerMoved;
};

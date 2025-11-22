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
    u32 getEnablePastCount() const;
    void getPointingPosBasedOnScreen(TVec2f*) const;

    const WPad* mPad;           // 0x0
    TVec2f* mPointingPosArray;  // 0x4
    TVec2f* mHorizonArray;      // 0x8
    s32 _C;
    u32 _10;
    f32 mPosPlayRadius;    // 0x14
    f32 mPosSensitivity;   // 0x18
    f32 mHoriPlayRadius;   // 0x1C
    f32 mHoriSensitivity;  // 0x20
    f32 mDistPlayRadius;   // 0x24
    f32 mDistSensitivity;  // 0x28
    u32 _2C;
    f32 mDistDisplay;  // 0x30
    u8 _34;
    u32 _38;
    u32 _3C;
    u32 mEnablePastCount;   // 0x40
    bool mIsPointInScreen;  // 0x44
    u8 _45;
};

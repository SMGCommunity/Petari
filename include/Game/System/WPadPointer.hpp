#pragma once

#include "Game/System/WPad.hpp"
#include <JSystem/JGeometry.hpp>

class WPadPointer {
public:
    WPadPointer(const WPad *);

    void reset();
    void setSensorBarLevel(f32);
    void update();
    void getPointingPos(TVec2f *) const;
    void getHorizonVec(TVec2f *) const;
    void getPastPointingPos(TVec2f *, s32) const;
    u32 getEnablePastCount() const;
    void getPointingPosBasedOnScreen(TVec2f *) const;

    const WPad* mPad;               // _0
    TVec2f* mPointingPosArray;      // _4
    TVec2f* mHorizonArray;          // _8
    s32 _C;
    u32 _10;
    f32 mPosPlayRadius;             // _14
    f32 mPosSensitivity;            // _18
    f32 mHoriPlayRadius;            // _1C
    f32 mHoriSensitivity;           // _20
    f32 mDistPlayRadius;            // _24
    f32 mDistSensitivity;           // _28
    u32 _2C;
    f32 _30;
    u8 _34;
    u32 _38;
    u32 _3C;
    u32 mEnablePastCount;           // _40
    u8 _44;
    u8 _45;
    f32 _48;
};
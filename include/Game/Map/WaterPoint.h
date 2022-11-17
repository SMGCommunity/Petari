#pragma once

#include <JSystem/JGeometry.h>
#include <revolution.h>

class WaterPoint {
public:
    WaterPoint(const TVec3f &, const TVec3f &, f32, f32, f32, f32);

    void initAfterPlacement();
    void updatePos(f32, f32, f32, f32, f32);
    f32 calcHeight(f32, f32, f32, f32, f32, f32) const;

    TVec3f mPosition;       // _0
    TVec3f mOrigPos;        // _C
    f32 mCoordAcrossRail;   // _18
    f32 mCoordOnRail;       // _1C
    TVec3f mUpVec;          // _20
    f32 mHeight;            // _2C
    f32 mFlowSpeedRate;     // _30
    u8 mAlpha;              // _34
};
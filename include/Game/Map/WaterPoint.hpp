#pragma once

#include <JSystem/JGeometry.hpp>
#include <revolution.h>

class WaterPoint {
public:
    WaterPoint(const TVec3f&, const TVec3f&, f32, f32, f32, f32);

    void initAfterPlacement();
    void updatePos(f32, f32, f32, f32, f32);
    f32 calcHeight(f32, f32, f32, f32, f32, f32) const;

    TVec3f mPosition;      // 0x0
    TVec3f mOrigPos;       // 0xC
    f32 mCoordAcrossRail;  // 0x18
    f32 mCoordOnRail;      // 0x1C
    TVec3f mUpVec;         // 0x20
    f32 mHeight;           // 0x2C
    f32 mFlowSpeedRate;    // 0x30
    u8 mAlpha;             // 0x34
};
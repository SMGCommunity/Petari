#pragma once

#include <JSystem/JGeometry.hpp>
#include <revolution.h>

class WaterPoint {
public:
    WaterPoint(const TVec3f&, const TVec3f&, f32, f32, f32, f32);

    void initAfterPlacement();
    void updatePos(f32, f32, f32, f32, f32);
    f32 calcHeight(f32, f32, f32, f32, f32, f32) const;

    /* 0x00 */ TVec3f mPosition;
    /* 0x0C */ TVec3f mOrigPos;
    /* 0x18 */ f32 mCoordAcrossRail;
    /* 0x1C */ f32 mCoordOnRail;
    /* 0x20 */ TVec3f mUpVec;
    /* 0x2C */ f32 mHeight;
    /* 0x30 */ f32 mFlowSpeedRate;
    /* 0x34 */ u8 mAlpha;
};

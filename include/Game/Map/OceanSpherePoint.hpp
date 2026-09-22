#pragma once

#include <JSystem/JGeometry.hpp>
#include <revolution.h>

class OceanSpherePoint {
public:
    OceanSpherePoint(const TVec3f* pCenter, const TVec3f& rNormal, f32 wave1Pos, f32 wave2Pos, const TVec2f& rTexCoord);

    void updatePos(f32 radius, f32 wave1Time, f32 wave2Time);
    f32 calcHeight(f32 wave1Time, f32 wave2Time, f32 wave1Pos, f32 wave2Pos) const;

    /* 0x0 */ TVec3f mPos;
    /* 0xC */ const TVec3f* mCenter;
    /* 0x10 */ TVec3f mNormal;
    /* 0x1C */ f32 mWave1Pos;
    /* 0x20 */ f32 mWave2Pos;
    /* 0x24 */ TVec2f mTexCoord;
};

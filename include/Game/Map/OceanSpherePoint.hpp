#pragma once

#include <JSystem/JGeometry.hpp>
#include <revolution.h>

class OceanSpherePoint {
public:
    OceanSpherePoint(const TVec3f*, const TVec3f&, f32, f32, const TVec2f&);

    void updatePos(f32, f32, f32);
    f32 calcHeight(f32, f32, f32, f32) const;

    TVec3f mPos;             // 0x0
    const TVec3f* mCenter;   // 0xC
    TVec3f mNormal;          // 0x10
    f32 mWave1Pos;           // 0x1C
    f32 mWave2Pos;           // 0x20
    TVec2f mTexCoord;        // 0x24
};

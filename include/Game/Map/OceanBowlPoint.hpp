#pragma once

#include <JSystem/JGeometry.hpp>
#include <revolution.h>

class OceanBowlPoint {
public:
    OceanBowlPoint(const TVec3f&);

    void updatePos(f32, f32);
    static f32 calcHeightStatic(f32, f32, f32, f32);
    void reset(const TVec3f&, f32);

    TVec3f _0;            // 0x0
    TVec3f _C;  // 0xC
    f32 _18; // 0x18
    u8 _1C; // 0x1C : defaults to 0xFF
};

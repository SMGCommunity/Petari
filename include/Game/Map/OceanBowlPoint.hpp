#pragma once

#include <JSystem/JGeometry.hpp>
#include <revolution.h>

class OceanBowlPoint {
public:
    OceanBowlPoint(const TVec3f&);

    void updatePos(f32, f32);
    static f32 calcHeightStatic(f32, f32, f32, f32);
    void reset(const TVec3f&, f32);

    TVec3f mVertexPosition; // 0x0  : The position of the rendering vertex
    TVec3f mPosition;       // 0xC  : The actual position of the point without the wave height applied
    f32 mWaveScale;         // 0x18 : defaults to 1.0f
    u8 mAlpha;              // 0x1C : defaults to 0xFF
};

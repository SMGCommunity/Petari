#pragma once

#include <JSystem/JGeometry.hpp>
#include <revolution.h>

class OceanBowlPoint {
public:
    OceanBowlPoint(const TVec3f&);

    void updatePos(f32, f32);
    static f32 calcHeightStatic(f32, f32, f32, f32);
    void reset(const TVec3f&, f32);

    /* 0x00 */ TVec3f mVertexPosition;  // The position of the rendering vertex
    /* 0x0C */ TVec3f mPosition;        // The actual position of the point without the wave height applied
    /* 0x18 */ f32 mWaveScale;          // defaults to 1.0f
    /* 0x1C */ u8 mAlpha;               // defaults to 0xFF
};

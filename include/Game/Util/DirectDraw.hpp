#pragma once

#include "JSystem/JGeometry.hpp"

class TDDraw {
public:
    static void sendPoint(const TVec3f&);

    static void setup(u32, u32, u8);

    static void drawLine(const TVec3f&, const TVec3f&, u32);

    static void drawCircle(const TVec3f&, const TVec3f&, f32, u32, u32);

    static void drawSphere(const TVec3f&, f32, u32, u32);

    static void drawCylinder(const TVec3f&, const TVec3f&, f32, u32, u32, u32);

    static void drawFillFan(const TVec3f&, const TVec3f&, const TVec3f&, u32, f32, f32, u32);

    static void cameraInit3D();
};

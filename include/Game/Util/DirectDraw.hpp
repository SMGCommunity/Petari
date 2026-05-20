#pragma once

#include <JSystem/JGeometry/TVec.hpp>

namespace TDDraw {
    void sendPoint(const TVec3f&);

    void setup(u32, u32, u8);

    void drawLine(const TVec3f&, const TVec3f&, u32);

    void drawCircle(const TVec3f&, const TVec3f&, f32, u32, u32);

    void drawSphere(const TVec3f&, f32, u32, u32);

    void drawCylinder(const TVec3f&, const TVec3f&, f32, u32, u32, u32);

    void drawFillFan(const TVec3f&, const TVec3f&, const TVec3f&, u32, f32, f32, u32);

    void cameraInit3D();

    void invProject(TVec3f*, const TVec3f&, MtxPtr, const f32*, const f32*, bool);

};  // namespace TDDraw

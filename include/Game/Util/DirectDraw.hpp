#pragma once

#include <JSystem/JGeometry/TMatrix.hpp>

class JUTTexture;

namespace TDDraw {
    void setViewMtx(MtxPtr);
    void loadViewMtx(MtxPtr);
    void setModelMtx(MtxPtr);
    void resetViewMtx();
    void close();
    void setup(u32, u32, u8);
    void sendPoint(const TVec3f&, u32);
    void sendPoint(const TVec3f&);
    void drawLine(const TVec3f&, const TVec3f&, u32);
    void drawCircle(const TVec3f&, const TVec3f&, f32, u32, u32);
    void drawCircle(const TVec3f&, const TVec3f&, const TVec3f&, f32, u32, u32);
    void drawFillCircle(const TVec3f&, f32, u32, u32, u32);
    void drawFillCircle(const TVec3f&, const TVec3f&, f32, u32, u32);
    void drawFillFan(const TVec3f&, const TVec3f&, const TVec3f&, u32, f32, f32, u32);
    void drawCylinder(const TVec3f&, const TVec3f&, f32, u32, u32, u32);
    void drawSpherePart(const TPos3f&, f32, f32, f32, f32, f32, u32, u32, u32);
    void drawSphere(const TVec3f&, f32, u32, u32);
    void drawSphere3D(TVec3f, f32, u32, u32);
    void drawTexture(const TVec2f&, JUTTexture*, const TVec2f&);
    void drawTexture3D(const TVec3f&, const TVec3f&, const TVec3f&, f32, f32, JUTTexture*, bool, bool);
    void drawFillBox(const TVec3f&, const TVec3f&, u32);
    void drawFillBox(const TVec2f&, const TVec2f&, u32);
    void drawFillBox3D(const TVec3f&, const TVec3f&, const TVec3f&, const TVec3f&, u32);
    void cameraInit3D();
    void cameraInit2D();
    void mixFogColor(TVec3f, f32, u32);
    void tileConversion8(u8*, u32, u32);
    void tileConversion16(u16*, u32, u32);
    u32 getTexel32(const JUTTexture*, u32, u32);
    u32 getTexel32(const u8*, u32, u32, u32);
    void setTexel32(JUTTexture*, u32, u32, u32);
    void setTexel32(u8*, u32, u32, u32, u32);
    void invProject(TVec3f*, const TVec3f&, MtxPtr, const f32*, const f32*, bool);
    void project2D(TVec3f*, const TVec3f&);
    void project2D(TVec2f*, const TVec3f&);
    void fix2Dpos(TVec3f*);
    void setGXColor(u32, GXColor*);
};  // namespace TDDraw

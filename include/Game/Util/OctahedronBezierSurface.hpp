#pragma once

#include <JSystem/JGeometry/TVec.hpp>

class BezierSurface;

class OctahedronBezierSurface {
public:
    OctahedronBezierSurface(u32);

    /* 0x08 */ virtual void movement();
    /* 0x0C */ virtual void calcAnim();
    /* 0x10 */ virtual void draw() const;

    const TVec3f& getVertex(s32) const;
    TVec3f* getVertexPtr(s32);
    void setDiagonalRate(f32);
    void setDivideLevel(s32);
    void calcControlPoint();

    /* 0x04 */ TVec3f mVertices[6];  // top, right, back, left, front, bottom
    /* 0x4C */ BezierSurface* mSurfaces[8];
    /* 0x6C */ f32 mDiagonalRate;
};

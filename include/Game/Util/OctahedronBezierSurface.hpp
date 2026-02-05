#pragma once

#include <JSystem/JGeometry/TVec.hpp>

class BezierSurface;

class OctahedronBezierSurface {
public:
    OctahedronBezierSurface(u32);

    virtual void movement();
    virtual void calcAnim();
    virtual void draw() const;

    const TVec3f& getVertex(s32) const;
    TVec3f* getVertexPtr(s32);
    void setDiagonalRate(f32);
    void setDivideLevel(s32);
    void calcControlPoint();

    /* 0x04 */ TVec3f mVertices[6];  // top, right, back, left, front, bottom
    /* 0x4C */ BezierSurface* mSurfaces[8];
    /* 0x6C */ f32 mDiagonalRate;
};

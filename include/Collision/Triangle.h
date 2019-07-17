#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "types.h"
#include "JGeometry/TVec3.h"

class Triangle
{
public:
    Triangle();

    u32 _0;
    s32 _4;
    u32 _8;
    JGeometry::TVec3<f32> mFaceNormal; // _C
    JGeometry::TVec3<f32> mEdgeNormal_1;
    JGeometry::TVec3<f32> mEdgeNormal_2;
    JGeometry::TVec3<f32> mEdgeNormal_3;
    JGeometry::TVec3<f32> _3C;
    JGeometry::TVec3<f32> _48;
    JGeometry::TVec3<f32> _54;

};

#endif // TRIANGLE_H
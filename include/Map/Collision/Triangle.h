#ifndef TRIANGLE_H
#define TRIANGLE_H

#include <revolution.h>
#include "JGeometry/TVec3.h"

class CollisionParts;
class HitSensor;

class Triangle
{
public:
    Triangle();

    CollisionParts* mColParts; // _0
    s32 mIndex; // _4
    HitSensor* mSensor; // _8
    JGeometry::TVec3<f32> mFaceNormal; // _C
    JGeometry::TVec3<f32> mEdgeNormal_1;
    JGeometry::TVec3<f32> mEdgeNormal_2;
    JGeometry::TVec3<f32> mEdgeNormal_3;
    JGeometry::TVec3<f32> _3C;
    JGeometry::TVec3<f32> _48;
    JGeometry::TVec3<f32> _54;

};

class HitInfo : public Triangle
{
public:
    HitInfo();

    f32 _60;
    JGeometry::TVec3<f32> _64;
    JGeometry::TVec3<f32> _70;
    JGeometry::TVec3<f32> _7C;
    u8 _88;
};

#endif // TRIANGLE_H
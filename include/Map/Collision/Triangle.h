#pragma once

#include <revolution.h>
#include "JSystem/JGeometry/TVec3.h"

class CollisionParts;
class HitSensor;

class Triangle
{
public:
    Triangle();

    CollisionParts* mColParts; // _0
    s32 mIndex; // _4
    HitSensor* mSensor; // _8
    JGeometry::TVec3f mFaceNormal; // _C
    JGeometry::TVec3f mEdgeNormal_1;
    JGeometry::TVec3f mEdgeNormal_2;
    JGeometry::TVec3f mEdgeNormal_3;
    JGeometry::TVec3f _3C;
    JGeometry::TVec3f _48;
    JGeometry::TVec3f _54;

};

class HitInfo : public Triangle
{
public:
    HitInfo();

    f32 _60;
    JGeometry::TVec3f _64;
    JGeometry::TVec3f _70;
    JGeometry::TVec3f _7C;
    u8 _88;
};
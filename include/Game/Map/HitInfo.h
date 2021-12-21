#pragma once

#include "JSystem/JGeometry/TVec.h"

#include "Game/Util.h"

class CollisionParts;
class HitSensor;

class Triangle {
public:

    JMapInfoIter getAttributes() const;

    CollisionParts* _0;
    u32 mIdx;               // _4
    HitSensor* mSensor;     // _8
    TVec3f mNormals[0x4];   // _C
    TVec3f mPos0;           // _3C
    TVec3f mPos1;           // _48
    TVec3f mPos2;           // _54
};

class HitInfo {
public:
    Triangle mParentTriangle;   // _0
    f32 _60;
    TVec3f _64;
    TVec3f _70;
    TVec3f _7C;
    u8 _88;
    u8 _89;
    u8 _8A;
    u8 _8B;
};
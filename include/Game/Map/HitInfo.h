#pragma once

#include "JSystem/JGeometry/TVec.h"

#include "Game/Util/JMapInfoIter.h"

class CollisionParts;

class Triangle {
public:

    JMapInfoIter getAttributes() const;

    CollisionParts* _0;
    u32 _4;
    u32 _8;
    TVec3f mFaceNormal; // _C
    TVec3f _18;
    TVec3f _24;
    TVec3f _30;
    TVec3f _3C;
    TVec3f _48;
    TVec3f _54;
};
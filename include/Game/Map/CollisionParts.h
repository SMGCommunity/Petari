#pragma once

#include <revolution.h>
#include "JSystem/JGeometry/TMatrix.h"
#include "JSystem/JGeometry/TPosition3.h"

class KCollisionServer;

class CollisionParts {
public:
    CollisionParts();

    u32 _0;
    TMtx34f _4;
    TMtx34f _34;
    TMtx34f _64;
    TMtx34f _94;
    KCollisionServer* mKCL;  // _C4
    u32 _C8;
    u8 _CC;
    u8 _CD;
    u8 _CE;
    u8 _CF;
    u8 _D0;
    u32 _D4;
    f32 _D8;
    f32 _DC;
    u32 _E0;
    u32 _E4;
    u32 _E8;
    u32 _EC;
};
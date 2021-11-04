#pragma once

#include "Game/NameObj/NameObj.h"
#include "Game/LiveActor/LiveActorFlag.h"
#include "JSystem/JGeometry/TVec.h"

class LiveActor : public NameObj {
public:
    LiveActor(const char *);

    TVec3f mPosition;       // _C
    TVec3f mRotation;       // _18
    TVec3f mScale;          // _24
    TVec3f mVelocity;       // _30
    TVec3f mGravity;        // _3C
    u32* _48;
    u32* _4C;
    u32* _50;
    u32* _54;
    u32* _58;
    u32* _5C;
    u32* _60;
    u32* _64;
    LiveActorFlag mFlags;   // _68
    u32* _74;
    u32* _78;
    u32* _7C;
    u32* _80;
    u32* _84;
    u32* _88;
};
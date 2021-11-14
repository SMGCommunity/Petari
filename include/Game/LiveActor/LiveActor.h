#pragma once

#include "Game/NameObj/NameObj.h"
#include "Game/LiveActor/LiveActorFlag.h"
#include "Game/LiveActor/Spine.h"
#include "Game/LiveActor/HitSensor.h"
#include "Game/LiveActor/HitSensorKeeper.h"
#include "JSystem/JGeometry/TVec.h"

class LiveActor : public NameObj {
public:
    LiveActor(const char *);

    virtual s32 receiveMessage(u32, HitSensor *, HitSensor *);

    TVec3f mPosition;       // _C
    TVec3f mRotation;       // _18
    TVec3f mScale;          // _24
    TVec3f mVelocity;       // _30
    TVec3f mGravity;        // _3C
    u32* _48;
    u32* _4C;
    Spine* mSpine;
    HitSensorKeeper* mSensorKeeper; // _54
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
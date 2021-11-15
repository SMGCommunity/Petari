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

    virtual ~LiveActor();

    virtual void init(JMapInfoIter const &);
    virtual void movement();
    virtual void calcAnim();
    virtual void calcViewAndEntry();
    virtual void appear();
    virtual void makeActorAppeared();
    virtual void kill();
    virtual void makeActorDead();
    virtual s32 receiveMessage(u32, HitSensor *, HitSensor *);
    virtual Mtx* getBaseMtx() const;
    virtual Mtx* getTakingMtx() const;
    virtual void startClipped();
    virtual void endClipped();
    virtual void control();
    virtual void calcAndSetBaseMtx();
    virtual void updateHitSensor(HitSensor *);
    virtual void attackSensor(HitSensor *, HitSensor *);
    virtual u32 receiveMsgPush(HitSensor *, HitSensor *);
    virtual u32 receiveMsgPlayerAttack(u32, HitSensor *, HitSensor *);
    virtual u32 receiveMsgEnemyAttack(u32, HitSensor *, HitSensor *);
    virtual u32 receiveMsgTake(HitSensor *, HitSensor *);
    virtual u32 receiveMsgTaken(HitSensor *, HitSensor *);
    virtual u32 receiveMsgThrow(HitSensor *, HitSensor *);
    virtual u32 receiveMsgApart(HitSensor *, HitSensor *);
    virtual u32 receiveMsgOtherMsg(u32, HitSensor *, HitSensor *);

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
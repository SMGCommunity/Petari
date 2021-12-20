#pragma once

#include "Game/NameObj/NameObj.h"
#include "Game/LiveActor/LiveActorFlag.h"
#include "JSystem/JGeometry/TVec.h"

class ActorAnimKeeper;
class ActorLightCtrl;
class ActorPadAndCameraCtrl;
class AudAnmSoundObject;
class Binder;
class CollisionParts;
class EffectKeeper;
class Spine;
class HitSensor;
class HitSensorKeeper;
class ModelManager;
class RailRider;
class ShadowControllerList;
class StageSwitchCtrl;
class StarPointerTarget;

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
    virtual bool receiveMessage(u32, HitSensor *, HitSensor *);
    virtual MtxPtr getBaseMtx() const;
    virtual MtxPtr getTakingMtx() const;
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

    void initActorLightCtrl();

    HitSensor* getSensor(const char *) const;

    TVec3f mPosition;                       // _C
    TVec3f mRotation;                       // _18
    TVec3f mScale;                          // _24
    TVec3f mVelocity;                       // _30
    TVec3f mGravity;                        // _3C
    ModelManager* mModelManager;            // _48
    ActorAnimKeeper* mAnimationKeepper;     // _4C
    Spine* mSpine;                          // _50
    HitSensorKeeper* mSensorKeeper;         // _54
    Binder* mBinder;                        // _58
    RailRider* mRailRider;                  // _5C
    EffectKeeper* mEffectKeeper;            // _60
    AudAnmSoundObject* mSoundObject;        // _64
    LiveActorFlag mFlags;                   // _68
    ShadowControllerList* mShadowList;      // _74
    CollisionParts* mCollisionParts;        // _78
    StageSwitchCtrl* mStageSwitchCtrl;      // _7C
    StarPointerTarget* mStarPointerTarget;  // _80
    ActorLightCtrl* mActorLightCtrl;        // _84
    ActorPadAndCameraCtrl* mCameraCtrl;     // _88
};

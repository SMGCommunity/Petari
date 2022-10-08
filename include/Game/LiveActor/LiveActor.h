#pragma once

#include "Game/NameObj/NameObj.h"
#include "Game/LiveActor/LiveActorFlag.h"
#include "Game/LiveActor/ActorAnimKeeper.h"
#include "Game/LiveActor/ActorLightCtrl.h"
#include "Game/LiveActor/EffectKeeper.h"
#include "Game/LiveActor/HitSensorKeeper.h"
#include "Game/LiveActor/ModelManager.h"
#include "Game/LiveActor/ShadowController.h"
#include "Game/LiveActor/Spine.h"
#include "Game/LiveActor/RailRider.h"
#include "Game/Map/StageSwitch.h"
#include "JSystem/JGeometry/TVec.h"

class ActorPadAndCameraCtrl;
class AudAnmSoundObject;
class Binder;
class CollisionParts;
class ShadowControllerList;
class StarPointerTarget;

class LiveActor : public NameObj {
public:
    LiveActor(const char *);

    virtual inline ~LiveActor() {}

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
    virtual bool receiveMsgPush(HitSensor *, HitSensor *);
    virtual bool receiveMsgPlayerAttack(u32, HitSensor *, HitSensor *);
    virtual bool receiveMsgEnemyAttack(u32, HitSensor *, HitSensor *);
    virtual u32 receiveMsgTake(HitSensor *, HitSensor *);
    virtual u32 receiveMsgTaken(HitSensor *, HitSensor *);
    virtual u32 receiveMsgThrow(HitSensor *, HitSensor *);
    virtual u32 receiveMsgApart(HitSensor *, HitSensor *);
    virtual bool receiveOtherMsg(u32, HitSensor *, HitSensor *);

    void calcAnmMtx();
    void setNerve(const Nerve *);
    bool isNerve(const Nerve *) const;
    s32 getNerveStep() const;
    HitSensor* getSensor(const char *) const;
    void initModelManagerWithAnm(const char *, const char *, bool);
    void initNerve(const Nerve *);
    void initHitSensor(int);
    void initBinder(f32, f32, u32);
    void initRailRider(const JMapInfoIter &);
    void initEffectKeeper(int, const char *, bool);
    void initSound(int, bool);
    void initShadowControllerList(u32);
    void initActorCollisionParts(const char *, HitSensor *, ResourceHolder *, MtxPtr, bool, bool);
    void initStageSwitch(const JMapInfoIter &);
    void initActorStarPointerTarget(f32, const TVec3f *, MtxPtr, TVec3f);
    void initActorLightCtrl();
    void addToSoundObjHolder();
    void updateBinder();

    inline bool isBinderValid(Binder *pBinder) {
        return pBinder;
    }

    /* 0xC */       TVec3f mPosition;                       ///< 3D vector of the actor's position.
    /* 0x18 */      TVec3f mRotation;                       ///< 3D vector of the actor's rotation.
    /* 0x24 */      TVec3f mScale;                          ///< 3D vector of the actor's scale.
    /* 0x30 */      TVec3f mVelocity;                       ///< 3D vector of the actor's velocity.
    /* 0x3C */      TVec3f mGravity;                        ///< 3D vector of the actor's gravity.
    /* 0x48 */      ModelManager* mModelManager;            ///< A pointer to a ModelManager instance, used for drawing a 3D model.
    /* 0x4C */      ActorAnimKeeper* mAnimationKeeper;      ///< A pointer to a ActorAnimKeeper instance, used for stroring and playing model animations.
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

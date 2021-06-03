#pragma once

#include "LiveActor/Animation/ActorAnimKeeper.h"
#include "LiveActor/Camera/ActorPadAndCameraCtrl.h"
#include "LiveActor/Effect/EffectKeeper.h"
#include "LiveActor/Light/ActorLightCtrl.h"
#include "LiveActor/LiveActorFlag.h"
#include "NameObj/NameObj.h"
#include "LiveActor/Nerve/Spine.h"
#include "LiveActor/Rail/RailRider.h"
#include "LiveActor/Sensor/HitSensor.h"
#include "LiveActor/Sensor/HitSensorKeeper.h"
#include "LiveActor/Shadow/ShadowController.h"
#include "LiveActor/Sound/AudAnmSoundObject.h"
#include "LiveActor/StarPointer/StarPointerTarget.h"
#include "Map/Switch/StageSwitch.h"
#include "Collision/Binder.h"
#include "JGeometry/TVec3.h"
#include "LiveActor/Model/ModelManager.h"

class LiveActor : public NameObj
{
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

    void calcAnmMtx();
    void setNerve(const Nerve *);
    bool isNerve(const Nerve *) const;
    u32 getNerveStep() const;
    HitSensor* getSensor(const char *) const;
    void initModelManagerWithAnm(const char *, const char *, bool);
    void initNerve(const Nerve *);
    void initHitSensor(s32);
    void initBinder(f32, f32, u32);
    void initRailRider(const JMapInfoIter &);
    void initEffectKeeper(s32, const char *, bool);
    void initSound(s32, bool);
    void initShadowControllerList(u32);
    void initStageSwitch(const JMapInfoIter &);
    void initActorStarPointerTarget(f32, const JGeometry::TVec3<f32> *, Mtx *, JGeometry::TVec3<f32>);
    void initActorLightCtrl();
    void addToSoundObjHolder();
    void updateBinder();

    JGeometry::TVec3<f32> mTranslation; // _C
    JGeometry::TVec3<f32> mRotation; // _18
    JGeometry::TVec3<f32> mScale; // _24
    JGeometry::TVec3<f32> mVelocity; // _30
    JGeometry::TVec3<f32> mGravity; // _3C
    ModelManager* mModelManager; // _48
    ActorAnimKeeper* mAnimKeeper; // _4C
    Spine* mSpine; // _50
    HitSensorKeeper* mSensorKeeper; //_54
    Binder* mBinder; // _58
    RailRider* mRailRider; // _5C
    EffectKeeper* mEffectKeeper; // _60
    AudAnmSoundObject* mSoundObj; // _64
    LiveActorFlag mFlags; // _68
    ShadowControllerList* mShadowController; // _74
    u32* _78; // CollisionParts*
    StageSwitchCtrl* mStageSwitchCtrl; // _7C
    StarPointerTarget* mPointerTarget; // _80
    ActorLightCtrl* mLightCtrl; // _84
    ActorPadAndCameraCtrl* mCameraCtrl; // _88
};
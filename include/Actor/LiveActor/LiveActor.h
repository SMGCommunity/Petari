#ifndef LIVEACTOR_H
#define LIVEACTOR_H

#include "Actor/Light/ActorLightCtrl.h"
#include "Actor/LiveActor/LiveActorFlag.h"
#include "Actor/NameObj/NameObj.h"
#include "Actor/Nerve/Spine.h"
#include "Actor/Sensor/HitSensor.h"
#include "Actor/Sensor/HitSensorKeeper.h"
#include "Collision/Binder.h"
#include "Effect/EffectKeeper.h"
#include "JGeometry/TVec3.h"

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
    virtual void recieveMessage(u64, HitSensor *, HitSensor *);
    virtual f32* getBaseMtx() const;
    virtual f32* getTakingMtx() const;
    virtual void startClipped();
    virtual void endClipped();
    virtual void control();
    virtual void calcAndSetBaseMtx();
    virtual void updateHitSensor(HitSensor *);

    HitSensor* getSensor(const char *) const;

    JGeometry::TVec3<f32> mTranslation; // _C
    JGeometry::TVec3<f32> mRotation; // _18
    JGeometry::TVec3<f32> mScale; // _24
    JGeometry::TVec3<f32> mVelocity; // _30
    JGeometry::TVec3<f32> mGravity; // _3C
    u32* _48; // ModelManager*
    u32* _4C; // ActorAnimKeeper*
    Spine* mSpine; // _50
    HitSensorKeeper* mSensorKeeper; //_54
    Binder* mBinder; // _58
    u32* _5C; // RailRider*
    EffectKeeper* mEffectKeeper; // _60
    u32* _64; // AudSoundObject*
    LiveActorFlag mFlags; // _68
    u32* _74; // ShadowController*
    u32* _78; // CollisionParts*
    u32* _7C; // StageSwitchCtrl*
    u32* _80; // StarPointerTarget*
    ActorLightCtrl* mLightCtrl; // _84
    u32* _88; // ActorPadAndCameraCtrl*

    const static f32 zero;
    const static f32 one;
    const static f32 neg_one;
};

const f32 LiveActor::zero = 0.0f;
const f32 LiveActor::one = 1.0f;
const f32 LiveActor::neg_one = -1.0f;

#endif // LIVEACTOR_H
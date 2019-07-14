#pragma once

#include "actor/LiveActor/LiveActorFlag.h"
#include "actor/nameobj/NameObj.h"
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

    JGeometry::TVec3<f32> mTranslation; // _C
    JGeometry::TVec3<f32> mRotation; // _18
    JGeometry::TVec3<f32> mScale; // _24
    JGeometry::TVec3<f32> mVelocity; // _30
    JGeometry::TVec3<f32> mGravity; // _3C
    u32* _48; // ModelManager*
    u32* _4C; // ActorAnimKeeper*
    u32* _50; // Spine*
    u32* _54; // HitSensorKeeper*
    u32* _58; // Binder*
    u32* _5C; // RailRider*
    u32* _60; // EffectKeeper*
    u32* _64; // AudSoundObject*
    LiveActorFlag mFlags; // _68
    u32* _74; // ShadowController*
    u32* _78; // CollisionParts*
    u32* _7C; // StageSwitchCtrl*
    u32* _80; // StarPointerTarget*
    u32* _84; // ActorLightCtrl*
    u32* _88; // ActorPadAndCameraCtrl*
};
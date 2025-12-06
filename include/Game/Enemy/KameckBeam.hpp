#pragma once

#include "Game/LiveActor/LiveActor.hpp"

class KameckBeamEventListener {
public:
    KameckBeamEventListener();

    virtual void hitBeam(s32);
};

class KameckBeamCollisionFilter {
public:
    KameckBeamCollisionFilter(const TVec3f*, f32);

    virtual bool isInvalidParts(const CollisionParts*) const;

    TVec3f* _4;
    f32 _8;
};

class KameckBeam : public LiveActor {
public:
    KameckBeam(const char*);

    virtual ~KameckBeam();
    virtual void init(const JMapInfoIter&);
    virtual void calcAnim();
    virtual void kill();
    virtual void control();
    virtual void attackSensor(HitSensor*, HitSensor*);
    virtual bool receiveOtherMsg(u32, HitSensor*, HitSensor*);

    void setEventListener(KameckBeamEventListener*);

    void requestShootToPlayerGround(f32);

    u32 _8C;
    u32 _90;
    u32 _94;
    u32 _98;
    u32 _9C;
    MtxPtr _A0;
    TVec3f _A4;
    TVec3f _B0;
};
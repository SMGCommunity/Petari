#pragma once

#include "Game/LiveActor/LiveActor.hpp"

class KameckBeamEventListener {
public:
    KameckBeamEventListener();

    /* 0x08 */ virtual void hitBeam(s32) = 0;
};

class KameckBeamCollisionFilter {
public:
    KameckBeamCollisionFilter(const TVec3f*, f32);

    /* 0x08 */ virtual bool isInvalidParts(const CollisionParts*) const;

    TVec3f* _4;
    f32 _8;
};

class KameckBeam : public LiveActor {
public:
    KameckBeam(const char*);

    /* 0x08 */ virtual ~KameckBeam();
    /* 0x0C */ virtual void init(const JMapInfoIter&);
    /* 0x1C */ virtual void calcAnim();
    /* 0x2C */ virtual void kill();
    /* 0x48 */ virtual void control();
    /* 0x54 */ virtual void attackSensor(HitSensor*, HitSensor*);
    /* 0x74 */ virtual bool receiveOtherMsg(u32, HitSensor*, HitSensor*);

    void setEventListener(KameckBeamEventListener*);

    void requestShootToPlayerCenter(f32);
    void requestShootToPlayerGround(f32);

    u32 _8C;
    u32 _90;
    u32 _94;
    u32 _98;
    u32 _9C;
    MtxPtr _A0;
    TVec3f _A4;
    TVec3f _B0;
    u32 _BC;
    u32* _C0;
};

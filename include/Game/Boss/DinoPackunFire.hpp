#pragma once

#include "Game/LiveActor/LiveActor.hpp"
#include "Game/LiveActor/LiveActorGroup.hpp"

class DinoPackunFire;

class DinoPackunFireHolder : public DeriveActorGroup< DinoPackunFire > {
public:
    DinoPackunFireHolder(s32);
};

class DinoPackunFire : public LiveActor {
public:
    DinoPackunFire(const char*);

    /* 0x08 */ virtual ~DinoPackunFire();
    /* 0x0C */ virtual void init(const JMapInfoIter&);
    /* 0x2C */ virtual void kill();
    /* 0x48 */ virtual void control();
    /* 0x4C */ virtual void calcAndSetBaseMtx();
    /* 0x54 */ virtual void attackSensor(HitSensor* pSender, HitSensor* pReceiver);

    void appearShot(const TVec3f&, const TVec3f&);
    void exeShot();
    void exeGround();

    TQuat4f _8C;
};

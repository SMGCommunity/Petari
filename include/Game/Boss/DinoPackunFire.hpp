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

    virtual ~DinoPackunFire();
    virtual void init(const JMapInfoIter&);
    virtual void kill();
    virtual void control();
    virtual void calcAndSetBaseMtx();
    virtual void attackSensor(HitSensor* pSender, HitSensor* pReceiver);

    void appearShot(const TVec3f&, const TVec3f&);
    void exeShot();
    void exeGround();

    TQuat4f _8C;
};

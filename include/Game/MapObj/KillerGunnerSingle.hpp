#pragma once

#include "Game/LiveActor/LiveActor.hpp"

class KillerGunnerSingle : public LiveActor {
public:
    KillerGunnerSingle(const char*);

    virtual ~KillerGunnerSingle();
    virtual void init(const JMapInfoIter&);
    virtual void attackSensor(HitSensor*, HitSensor*);

    void exeWait();
    void exeCharge();
    void exeAttack();
    void exeShoot();
};

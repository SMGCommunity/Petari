#pragma once

#include "Game/LiveActor/LiveActor.hpp"

class KillerGunnerSingle : public LiveActor {
public:
    KillerGunnerSingle(const char*);

    /* 0x08 */ virtual ~KillerGunnerSingle();
    /* 0x0C */ virtual void init(const JMapInfoIter&);
    /* 0x54 */ virtual void attackSensor(HitSensor*, HitSensor*);

    void exeWait();
    void exeCharge();
    void exeAttack();
    void exeShoot();
};

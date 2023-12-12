#pragma once

#include "Game/LiveActor/LiveActor.h"

namespace {
    static Vec sShellPos;
};

class KillerGunnerSingle : public LiveActor {
public:
    KillerGunnerSingle(const char *);

    virtual ~KillerGunnerSingle();
    virtual void init(const JMapInfoIter &);
    virtual void attackSensor(HitSensor *, HitSensor *);

    void exeWait();
    void exeCharge();
    void exeAttack();
    void exeShoot();
};

namespace NrvKillerGunnerSingle {
    NERVE_DECL(HostTypeWait, KillerGunnerSingle, KillerGunnerSingle::exeWait);
    NERVE_DECL(HostTypeCharge, KillerGunnerSingle, KillerGunnerSingle::exeCharge);
    NERVE_DECL(HostTypeShoot, KillerGunnerSingle, KillerGunnerSingle::exeWait);
    NERVE_DECL(HostTypeAttack, KillerGunnerSingle, KillerGunnerSingle::exeAttack);
};
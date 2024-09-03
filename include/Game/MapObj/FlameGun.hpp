#pragma once

#include "Game/LiveActor/LiveActor.hpp"

class FlameGun : public LiveActor {
public:
    FlameGun(const char *);

    virtual ~FlameGun();
    virtual void init(const JMapInfoIter &);
    virtual void initAfterPlacement();
    virtual void endClipped();
    virtual void updateHitSensor(HitSensor *);
    virtual void attackSensor(HitSensor *, HitSensor *);

    void initSensor();
    void exeSwitchWait();
    void exeWait();
    void exePreRadiate();
    void exeRadiate();
    void exeRotate();
    void exeRadiateOnly();

    TVec3f _8C;
    s32 mState;             // 0x98
};

namespace NrvFlameGun {
    NERVE_DECL(HostTypeNrvSwitchWait, FlameGun, FlameGun::exeSwitchWait);
    NERVE_DECL(HostTypeNrvWait, FlameGun, FlameGun::exeWait);
    NERVE_DECL(HostTypeNrvPreRadiate, FlameGun, FlameGun::exePreRadiate);
    NERVE_DECL(HostTypeNrvRadiate, FlameGun, FlameGun::exeRadiate);
    NERVE_DECL(HostTypeNrvRotate, FlameGun, FlameGun::exeRotate);
    NERVE_DECL(HostTypeNrvRadiateOnly, FlameGun, FlameGun::exeRadiateOnly);
};
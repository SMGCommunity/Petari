#pragma once

#include "Game/LiveActor/LiveActor.hpp"

class FlameGun : public LiveActor {
public:
    FlameGun(const char*);

    virtual ~FlameGun();
    virtual void init(const JMapInfoIter&);
    virtual void initAfterPlacement();
    virtual void endClipped();
    virtual void updateHitSensor(HitSensor*);
    virtual void attackSensor(HitSensor*, HitSensor*);

    void initSensor();
    void exeSwitchWait();
    void exeWait();
    void exePreRadiate();
    void exeRadiate();
    void exeRotate();
    void exeRadiateOnly();

    TVec3f _8C;
    s32 mState;  // 0x98
};

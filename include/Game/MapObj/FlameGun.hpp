#pragma once

#include "Game/LiveActor/LiveActor.hpp"

class FlameGun : public LiveActor {
public:
    FlameGun(const char*);

    /* 0x08 */ virtual ~FlameGun();
    /* 0x0C */ virtual void init(const JMapInfoIter&);
    /* 0x10 */ virtual void initAfterPlacement();
    /* 0x44 */ virtual void endClipped();
    /* 0x50 */ virtual void updateHitSensor(HitSensor*);
    /* 0x54 */ virtual void attackSensor(HitSensor*, HitSensor*);

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

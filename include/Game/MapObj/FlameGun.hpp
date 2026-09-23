#pragma once

#include "Game/LiveActor/LiveActor.hpp"

class FlameGun : public LiveActor {
public:
    FlameGun(const char* pName);

    virtual ~FlameGun();
    virtual void init(const JMapInfoIter& rIter);
    virtual void initAfterPlacement();
    virtual void endClipped();
    virtual void updateHitSensor(HitSensor* pSensor);
    virtual void attackSensor(HitSensor* pSender, HitSensor* pReceiver);

    void initSensor();
    void exeSwitchWait();
    void exeWait();
    void exePreRadiate();
    void exeRadiate();
    void exeRotate();
    void exeRadiateOnly();

    /* 0x8C */ TVec3f _8C;
    /* 0x98 */ s32 mState;
};

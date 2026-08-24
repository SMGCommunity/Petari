#pragma once

#include "Game/LiveActor/LiveActor.hpp"

class SubmarineSteam : public LiveActor {
public:
    SubmarineSteam(const char*);

    virtual void init(const JMapInfoIter&);
    virtual void initAfterPlacement();
    virtual void startClipped();
    virtual void endClipped();
    virtual void attackSensor(HitSensor*, HitSensor*);

    void exeSteam();
    void exeWaitForSwitchOn();

    /* 0x8C */ TVec3f mUp;
};

#pragma once

#include "Game/LiveActor/LiveActor.hpp"

class SubmarineSteam : public LiveActor {
public:
    SubmarineSteam(const char*);

    /* 0x0C */ virtual void init(const JMapInfoIter&);
    /* 0x10 */ virtual void initAfterPlacement();
    /* 0x40 */ virtual void startClipped();
    /* 0x44 */ virtual void endClipped();
    /* 0x54 */ virtual void attackSensor(HitSensor*, HitSensor*);

    void exeSteam();
    void exeWaitForSwitchOn();

    /* 0x8C */ TVec3f mUp;
};

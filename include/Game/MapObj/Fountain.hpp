#pragma once

#include "Game/LiveActor/LiveActor.hpp"

class Fountain : public LiveActor {
public:
    Fountain(const char*);

    /* 0x0C */ virtual void init(const JMapInfoIter&);
    /* 0x24 */ virtual void appear();
    /* 0x40 */ virtual void startClipped();
    /* 0x44 */ virtual void endClipped();
    /* 0x54 */ virtual void attackSensor(HitSensor*, HitSensor*);

    void exeWait();
    void exeMove();

    /* 0x8C */ const char* mFountainName;
    /* 0x90 */ TVec3f mUp;
};

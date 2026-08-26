#pragma once

#include "Game/LiveActor/LiveActor.hpp"

class Fountain : public LiveActor {
public:
    Fountain(const char*);

    virtual void init(const JMapInfoIter&);
    virtual void appear();
    virtual void startClipped();
    virtual void endClipped();
    virtual void attackSensor(HitSensor*, HitSensor*);

    void exeWait();
    void exeMove();

    /* 0x8C */ const char* mFountainName;
    /* 0x90 */ TVec3f mUp;
};

#pragma once

#include "Game/LiveActor/LiveActor.hpp"

class Fountain : public LiveActor {
public:
    Fountain(const char*);

    virtual ~Fountain();
    virtual void init(const JMapInfoIter&);
    virtual void appear();
    virtual void startClipped();
    virtual void endClipped();
    virtual void attackSensor(HitSensor*, HitSensor*);

    void exeWait();
    void exeMove();

    const char* mFountainName;  // 0x8C
    TVec3f _90;
};

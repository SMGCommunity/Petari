#pragma once

#include "Game/LiveActor/LiveActor.hpp"

class AstroDomeAsteroid : public LiveActor {
public:
    AstroDomeAsteroid(const char*);

    virtual ~AstroDomeAsteroid();
    virtual void init(const JMapInfoIter&);
    virtual void appear();
    virtual void control();
    virtual void calcAndSetBaseMtx();
    virtual bool receiveOtherMsg(u32, HitSensor*, HitSensor*);

    void exeWait();

    TPos3f mRotationMtx;  // 0x8C
};

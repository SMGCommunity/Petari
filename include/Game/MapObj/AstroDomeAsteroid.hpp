#pragma once

#include "Game/LiveActor/LiveActor.hpp"

class AstroDomeAsteroid : public LiveActor {
public:
    AstroDomeAsteroid(const char *);

    virtual ~AstroDomeAsteroid();
    virtual void init(const JMapInfoIter &);
    virtual void appear();
    virtual void control();
    virtual void calcAndSetBaseMtx();
    virtual bool receiveOtherMsg(u32, HitSensor *, HitSensor *);

    inline void exeWait() {
        if (MR::isFirstStep(this)) {
            MR::startBck(this, "AstroDomeAsteroid", nullptr);
        }

        MR::setBckRate(this, 1.0f);
    }

    TPos3f mRotationMtx;        // 0x8C
};

namespace NrvAstroDomeAsteroid {
    NERVE_DECL(AstroDomeAsteroidNrvWait, AstroDomeAsteroid, AstroDomeAsteroid::exeWait);
};
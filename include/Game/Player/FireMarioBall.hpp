#pragma once

#include "Game/LiveActor/HitSensor.hpp"
#include "Game/LiveActor/LiveActor.hpp"
#include "Game/Util/JMapInfo.hpp"
#include "JSystem/JGeometry/TVec.hpp"
#include "revolution/types.h"

class FireMarioBall : public LiveActor {
public:
    FireMarioBall(const char*);
    virtual ~FireMarioBall();
    virtual void init(const JMapInfoIter&);
    virtual void appear();
    virtual void kill();
    virtual void attackSensor(HitSensor*, HitSensor*);
    void initSensor();

    bool attackFire(HitSensor*);
    void exeThrow();
    void appearAndThrow(const TVec3f&, const TVec3f&);
    HitSensor* isBindedAny() const;
    bool tryToKill();

    u32 _8C;
    u16 _90;
};

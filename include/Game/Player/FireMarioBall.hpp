#pragma once

#include "Game/LiveActor/HitSensor.hpp"
#include "Game/LiveActor/LiveActor.hpp"
#include "Game/Util/JMapInfo.hpp"
#include "JSystem/JGeometry/TVec.hpp"
#include "revolution/types.h"

class FireMarioBall : public LiveActor {
public:
    FireMarioBall(const char*);
    /* 0x08 */ virtual ~FireMarioBall();
    /* 0x0C */ virtual void init(const JMapInfoIter&);
    /* 0x24 */ virtual void appear();
    /* 0x2C */ virtual void kill();
    /* 0x54 */ virtual void attackSensor(HitSensor*, HitSensor*);
    void initSensor();

    bool attackFire(HitSensor*);
    void exeThrow();
    void appearAndThrow(const TVec3f&, const TVec3f&);
    HitSensor* isBindedAny() const;
    bool tryToKill();

    u32 _8C;
    u16 _90;
};

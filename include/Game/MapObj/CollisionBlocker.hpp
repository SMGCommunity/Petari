#pragma once

#include "Game/LiveActor/LiveActor.hpp"

class CollisionBlocker : public LiveActor {
public:
    CollisionBlocker(const char*);

    /* 0x08 */ virtual ~CollisionBlocker();
    /* 0x0C */ virtual void init(const JMapInfoIter&);
    /* 0x54 */ virtual void attackSensor(HitSensor*, HitSensor*);

    void forceBreak();
};

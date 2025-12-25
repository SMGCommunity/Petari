#pragma once

#include "Game/LiveActor/LiveActor.hpp"

class CollisionBlocker : public LiveActor {
public:
    CollisionBlocker(const char*);

    virtual ~CollisionBlocker();
    virtual void init(const JMapInfoIter&);
    virtual void attackSensor(HitSensor*, HitSensor*);

    void forceBreak();
};

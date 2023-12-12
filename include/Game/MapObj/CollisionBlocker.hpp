#pragma once

#include "Game/LiveActor/LiveActor.h"

class CollisionBlocker : public LiveActor {
public:
    CollisionBlocker(const char *);

    virtual ~CollisionBlocker();
    virtual void init(const JMapInfoIter &);
    virtual void attackSensor(HitSensor *, HitSensor *);

    void forceBreak();
};

namespace {
    static f32 cRadius = 50.0f;
};

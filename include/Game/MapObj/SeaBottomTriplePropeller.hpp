#pragma once

#include "Game/LiveActor/LiveActor.hpp"

class AudSeKeeper;
class CollisionParts;

class SeaBottomTriplePropeller : public LiveActor {
public:
    SeaBottomTriplePropeller(const char*);

    virtual void init(const JMapInfoIter&);
    virtual void calcAnim();
    virtual void control();

    void exeWait();
    void exeBreak();

    CollisionParts* mPropellerCollision[3];  // 0x8C
    AudSeKeeper* mAudSeKeeper;               // 0x98
};

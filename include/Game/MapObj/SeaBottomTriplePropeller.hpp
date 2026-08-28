#pragma once

#include "Game/LiveActor/LiveActor.hpp"

class AudSeKeeper;
class CollisionParts;

class SeaBottomTriplePropeller : public LiveActor {
public:
    SeaBottomTriplePropeller(const char*);

    /* 0x0C */ virtual void init(const JMapInfoIter&);
    /* 0x1C */ virtual void calcAnim();
    /* 0x48 */ virtual void control();

    void exeWait();
    void exeBreak();

    CollisionParts* mPropellerCollision[3];  // 0x8C
    AudSeKeeper* mAudSeKeeper;               // 0x98
};

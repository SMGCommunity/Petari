#pragma once

#include "Game/LiveActor/LiveActor.hpp"

class AudSeKeeper;
class CollisionParts;

class SeaBottomTriplePropeller : public LiveActor {
public:
    SeaBottomTriplePropeller(const char*);

    virtual ~SeaBottomTriplePropeller();
    virtual void init(const JMapInfoIter &);
    virtual void calcAnim();
    virtual void control();

    void exeWait();

    CollisionParts* mPropellerCollision[3]; // 0x8C
    AudSeKeeper* mAudSeKeeper;            // 0x98
};

namespace NrvSeaBottomTriplePropeller {
    NERVE_DECL_EXE(SeaBottomTriplePropellerNrvWait, SeaBottomTriplePropeller, Wait);
    NERVE_DECL(SeaBottomTriplePropellerNrvBreak, SeaBottomTriplePropeller, kill);
};
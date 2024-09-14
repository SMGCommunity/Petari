#pragma once

#include "Game/LiveActor/LiveActor.hpp"
#include "Game/GameAudio/AudSeKeeper.hpp"

class SeaBottomTriplePropeller : public LiveActor {
public:
    SeaBottomTriplePropeller(const char*);

    virtual void init(const JMapInfoIter &);
    virtual void calcAnim();
    virtual void control();

    void exeWait();

    CollisionParts* mPropeller1Collision; // 0x8C
    CollisionParts* mPropeller2Collision; // 0x90
    CollisionParts* mPropeller3Collision; // 0x94
    AudSeKeeper* mAudSeKeeper;            // 0x98
};

namespace NrvSeaBottomTriplePropeller {
    NERVE_DECL_EXE(SeaBottomTriplePropellerNrvWait, SeaBottomTriplePropeller, Wait);
    NERVE_DECL(SeaBottomTriplePropellerNrvBreak, SeaBottomTriplePropeller, kill);
}
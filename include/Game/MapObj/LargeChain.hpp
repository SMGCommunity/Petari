#pragma once

#include "Game/LiveActor/LiveActor.hpp"
#include "Game/MapObj/LargeChainParts.hpp"

class LargeChain : public LiveActor {
public:
    LargeChain(const char*);

    virtual ~LargeChain();
    virtual void init(const JMapInfoIter&);
    virtual void makeActorAppeared();
    virtual void makeActorDead();

    void exeWait();
    void exeBreak();
    void createChainParts();
    void invalidateClippingAllChainParts();

    LargeChainParts* _8C;
    LargeChainParts* _90;
    /* 0x94 */ LargeChainParts* mChainArray;  ///< The array of LargeChainParts
    /* 0x98 */ u32 mChainCount;               ///< The number of chain parts.
};

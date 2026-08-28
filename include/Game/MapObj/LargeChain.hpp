#pragma once

#include "Game/LiveActor/LiveActor.hpp"
#include "Game/MapObj/LargeChainParts.hpp"

class LargeChain : public LiveActor {
public:
    LargeChain(const char*);

    /* 0x08 */ virtual ~LargeChain();
    /* 0x0C */ virtual void init(const JMapInfoIter&);
    /* 0x28 */ virtual void makeActorAppeared();
    /* 0x30 */ virtual void makeActorDead();

    void exeWait();
    void exeBreak();
    void createChainParts();
    void invalidateClippingAllChainParts();

    LargeChainParts* _8C;
    LargeChainParts* _90;
    /* 0x94 */ LargeChainParts* mChainArray;  ///< The array of LargeChainParts
    /* 0x98 */ u32 mChainCount;               ///< The number of chain parts.
};

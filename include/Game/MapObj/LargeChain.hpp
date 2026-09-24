#pragma once

#include "Game/LiveActor/LiveActor.hpp"

class LargeChainParts;

class LargeChain : public LiveActor {
public:
    LargeChain(const char*);

    virtual void init(const JMapInfoIter&);
    virtual void makeActorAppeared();
    virtual void makeActorDead();

    void exeWait();
    void exeBreak();

    void createChainParts();
    void invalidateClippingAllChainParts();

    /* 0x8C */ LargeChainParts* _8C;
    /* 0x90 */ LargeChainParts* _90;
    /* 0x94 */ LargeChainParts* mChainArray;
    /* 0x98 */ u32 mChainCount;
};

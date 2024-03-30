#pragma once

#include "Game/MapObj/CollectCounter.hpp"
#include "Game/Screen/LayoutActor.hpp"

class ChipCounter : public LayoutActor {
public:
    ChipCounter(const char *, s32);

    virtual ~ChipCounter();
    virtual void init(const JMapInfoIter &);
    virtual void control();

    void setCount(s32);

    CollectCounter* mCollectCounter;    // _20
    s32 mCount;                         // _24
    s32 mType;                          // _28
    s32 _2C;
    f32 _30;
};

namespace NrvChipCounter {
    NERVE(ChipCounterNrvHide);
};
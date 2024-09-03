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

    CollectCounter* mCollectCounter;    // 0x20
    s32 mCount;                         // 0x24
    s32 mType;                          // 0x28
    s32 _2C;
    f32 _30;
};

namespace NrvChipCounter {
    NERVE(ChipCounterNrvHide);
};
#pragma once

#include "Game/Screen/LayoutActor.hpp"

class CollectCounter;

class ChipCounter : public LayoutActor {
public:
    ChipCounter(const char*, s32);

    /* 0x08 */ virtual ~ChipCounter();
    /* 0x0C */ virtual void init(const JMapInfoIter&);
    /* 0x2C */ virtual void control();

    void setCount(s32);
    s32 requestShow(s32, s32);
    void requestHide(s32);
    void requestComplete(s32);
    void requestActive();
    void requestDeactive();

    void exeHide();

    CollectCounter* mCollectCounter;  // 0x20
    s32 mCount;                       // 0x24
    s32 mType;                        // 0x28
    s32 _2C;
    f32 _30;
};

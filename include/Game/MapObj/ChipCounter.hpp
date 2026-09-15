#pragma once

#include "Game/Screen/LayoutActor.hpp"

class CollectCounter;

class ChipCounter : public LayoutActor {
public:
    ChipCounter(const char*, s32);

    virtual void init(const JMapInfoIter&);
    virtual void control();

    void setCount(s32);
    void requestShow(s32, s32);
    void requestHide(s32);
    void requestComplete(s32);
    void requestActive();
    void requestDeactive();
    bool tryEndFrameIn();
    bool tryEndFrameOut();
    bool tryEndComplete();

    void exeHide();
    void exeFrameIn();
    void exeShow();
    void exeFrameOut();
    void exeTryDemo();
    void exeComplete();
    void exeCompleteOut();

    inline bool isHidden();
    inline bool isComplete();

    /* 0x20 */ CollectCounter* mCollectCounter;
    /* 0x24 */ s32 mCount;
    /* 0x28 */ s32 mType;
    /* 0x2C */ s32 mGroupId;
    /* 0x30 */ f32 _30;
};

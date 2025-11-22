#pragma once

#include "Game/Screen/LayoutActor.hpp"

class LiveActor;
class SimpleLayout;

class FullnessMeter : public LayoutActor {
public:
    FullnessMeter(LiveActor*, s32, s32);

    virtual void init(const JMapInfoIter&);
    virtual void control();

    void setNumber(s32);
    void requestAppear();
    void requestDisappear();
    void pauseOff();
    void exeAppear();
    void exeWait();
    void exeEnd();

    /* 0x20 */ s32 _20;
    /* 0x24 */ s32 _24;
    /* 0x28 */ LiveActor* mHost;
    /* 0x2C */ SimpleLayout* mTargetCounter;
};

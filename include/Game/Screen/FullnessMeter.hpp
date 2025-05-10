#pragma once

#include "Game/LiveActor/LiveActor.hpp"
#include "Game/Screen/LayoutActor.hpp"

class SimpleLayout;

class FullnessMeter : public LayoutActor {
public:
    FullnessMeter(LiveActor *, s32, s32);

    virtual ~FullnessMeter();
    virtual void init(const JMapInfoIter &);
    virtual void control();
    
    void setNumber(s32);
    void requestAppear();
    void requestDisappear();
    void pauseOff();
    void exeAppear();
    void exeEnd();
    inline void exeWait();  

    s32 _20;
    s32 _24;
    LiveActor* mHost;                       // 0x28
    SimpleLayout* mTargetCounter;           // 0x2C
};

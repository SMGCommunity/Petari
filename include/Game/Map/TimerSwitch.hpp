#pragma once

#include "Game/LiveActor/LiveActor.hpp"

class TimerSwitch : public LiveActor {
public:
    TimerSwitch(const char*);

    virtual ~TimerSwitch();
    virtual void init(const JMapInfoIter&);
    virtual void control();

    s32 mTimerLength;  // 0x8C
    s32 mCurrentTime;  // 0x90
};
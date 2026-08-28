#pragma once

#include "Game/LiveActor/LiveActor.hpp"

class TimerSwitch : public LiveActor {
public:
    TimerSwitch(const char*);

    /* 0x08 */ virtual ~TimerSwitch();
    /* 0x0C */ virtual void init(const JMapInfoIter&);
    /* 0x48 */ virtual void control();

    s32 mTimerLength;  // 0x8C
    s32 mCurrentTime;  // 0x90
};
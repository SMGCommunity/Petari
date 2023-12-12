#pragma once

#include "Game/LiveActor/LiveActor.h"

class TimerSwitch : public LiveActor {
public:
    TimerSwitch(const char *);

    virtual ~TimerSwitch();
    virtual void init(const JMapInfoIter &);
    virtual void control();

    s32 mTimerLength;   // _8C
    s32 mCurrentTime;   // _90
};
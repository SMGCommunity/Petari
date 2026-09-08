#pragma once

#include "Game/LiveActor/LiveActor.hpp"

class TimerSwitch : public LiveActor {
public:
    /// @brief Creates a new `TimerSwitch`.
    /// @param pName A pointer to the null-terminated name of the object.
    TimerSwitch(const char* pName);

    virtual void init(const JMapInfoIter&);
    virtual void control();

    /* 0x8C */ s32 mTimeLimit;
    /* 0x90 */ s32 mTimeLeft;
};

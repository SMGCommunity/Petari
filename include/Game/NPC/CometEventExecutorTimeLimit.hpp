#pragma once

#include "Game/LiveActor/LiveActor.hpp"

class TimeLimitLayout;

class CometEventExecutorTimeLimit : public LiveActor {
public:
    /// @brief Creates a new `CometEventExecutorTimeLimit`.
    /// @param timeLimit The number of seconds to initialize the timer with.
    CometEventExecutorTimeLimit(u32 timeLimit);

    virtual void init(const JMapInfoIter&);
    virtual void appear();
    virtual void kill();

    void exeStartEvent();
    u32 getTimeLimit() const;

public:
    /// @brief The pointer to the actor instance for the timer.
    /* 0x8C */ TimeLimitLayout* mTimeLimitLayout;

    /// @brief The number of seconds to initialize the timer with.
    /* 0x90 */ u32 mTimeLimit;
};

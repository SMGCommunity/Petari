#pragma once

#include "Game/LiveActor/LiveActor.hpp"

class TimeLimitLayout;

class CometEventExecutorTimeLimit : public LiveActor {
public:
    /// @brief Creates a new `CometEventExecutorTimeLimit`.
    /// @param timeLimit The number of seconds to initialize the timer with.
    CometEventExecutorTimeLimit(u32 timeLimit);

    /* 0x0C */ virtual void init(const JMapInfoIter&);
    /* 0x24 */ virtual void appear();
    /* 0x2C */ virtual void kill();

    void exeStartEvent();
    u32 getTimeLimit() const;

private:
    /// @brief The pointer to the actor instance for the timer.
    /* 0x8C */ TimeLimitLayout* mTimeLimitLayout;

    /// @brief The number of seconds to initialize the timer with.
    /* 0x90 */ u32 mTimeLimit;
};

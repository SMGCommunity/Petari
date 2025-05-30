#pragma once

class LiveActor;

class TimeAttackEventKeeper {
public:
    /// @brief Creates a new `TimeAttackEventKeeper`.
    TimeAttackEventKeeper();

    void init(bool);
    void startEventIfExecute();
    void endEvent();

private:
    /* 0x0 */ LiveActor* _0;
};

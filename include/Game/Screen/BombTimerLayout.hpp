#pragma once

#include "Game/Screen/LayoutActor.hpp"

class BombTimerLayout : public LayoutActor {
public:
    BombTimerLayout(bool param1);

    virtual ~BombTimerLayout();
    virtual void init(const JMapInfoIter& rIter);
    virtual void appear();

    void setTimeLimit(u32 timeLimit);
    void suspend();
    void resume();
    bool isReadyToTimeUp() const;
    void addFrame();
    bool update();
    u32 getRestTime() const;
    void updateTextBox();
    void exeAppear();
    void exeWait();
    void exeDanger();
    void exeEnd();

private:
    /* 0x20 */ u32 mTimeLimit;
    /* 0x24 */ u32 mDangerTime;
    /* 0x28 */ u32 mTime;
    /* 0x2C */ bool mIsSuspend;
};

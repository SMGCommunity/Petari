#pragma once

#include "Game/Screen/LayoutActor.hpp"

class CountUpPaneRumbler;

class SuddenDeathMeter : public LayoutActor {
public:
    SuddenDeathMeter(const char*, const char*);

    virtual void init(const JMapInfoIter& rIter);
    virtual void control();

    void requestActive();
    void requestDeactivate();
    void requestPowerUp();
    void requestForceAppear();
    void requestPlayerMoving();
    void exeAppear();
    void exeWait();
    void exePowerUp();
    void exeMeterMove();
    void exeCounterMove();
    void exeBreakMeter();
    void exeZeroMeter();
    void exeZeroMeterBreak();
    void initCount(s32);
    void setCount(s32);
    void calcMarioHeadPosition(TVec2f*) const;
    void calcPowerUpMeterBasePosition(TVec2f*) const;
    void setPowerUpMeterPosition(const TVec2f&);
    void setAnimBase();
    void setCountAnimFrame();
    void setRecoveryCountAnimFrame();

    /* 0x20 */ CountUpPaneRumbler* mCountUpPaneRumbler;
    /* 0x24 */ TVec2f mFollowPosW;
    /* 0x2C */ TVec2f _2C;
    /* 0x34 */ s32 mCount;
    /* 0x38 */ f32 mAlpha;
    /* 0x3C */ f32 mShowHideRate;
};

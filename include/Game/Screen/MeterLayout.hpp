#pragma once

#include "Game/LiveActor/Nerve.hpp"
#include "Game/Screen/LayoutActor.hpp"
#include "JSystem/JGeometry/TVec.hpp"

class CountUpPaneRumbler;

class MeterLayout : public LayoutActor {
public:
    MeterLayout(const char*, const char*);

    virtual void init(const JMapInfoIter& rIter);
    virtual void control();

    void requestActive();
    void requestDeactivate();
    void requestPowerUp();
    void requestForceAppear();
    void requestPlayerMoving();
    void requestPlayerStopped();
    void initCount(s32);
    void setCount(s32);
    void exeHide();
    void exeAppear();
    void exeWaitStart();
    void exeWait();
    void exePowerUp();
    void exeMeterMove();
    void exeCounterMove();
    void exeRecover();
    void exeDamage();
    void exeBreakMeter();
    void exeZeroMeter();
    void exeZeroMeterBreak();
    void calcMarioHeadPosition(TVec2f*) const;
    void calcPowerUpMeterBasePosition(TVec2f*) const;
    void setAnimBase();
    void setCountAnimFrame();
    void setRecoveryCountAnimFrame();

    CountUpPaneRumbler* mCountUpPaneRumbler;  // 0x20
    s32 _24;                                  // 0x24
    s32 _28;                                  // 0x28
    TVec2f mFollowPos;                        // 0x2C
    TVec2f mFollowPosW;                       // 0x34
    TVec2f _38;                               // 0x38
    s32 mCount;                               // 0x44
    f32 _48;                                  // 0x48
    f32 _4C;                                  // 0x4C
};

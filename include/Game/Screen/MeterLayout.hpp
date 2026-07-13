#pragma once

#include "Game/Screen/LayoutActor.hpp"

class CountUpPaneRumbler;

class MeterLayout : public LayoutActor {
public:
    /// @brief Creates a new `MeterLayout`.
    /// @param pName A pointer to the null-terminated name of the object.
    /// @param pArcName A pointer to the null-terminated name of the resource archive.
    MeterLayout(const char* pName, const char* pArcName);

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

    /* 0x20 */ CountUpPaneRumbler* mCountUpPaneRumbler;
    /* 0x24 */ u8 _24[8];
    /* 0x2C */ TVec2f mFollowPos;
    /* 0x34 */ TVec2f mFollowPosW;
    /* 0x3C */ TVec2f _3C;
    /* 0x44 */ s32 mCount;
    /* 0x48 */ f32 mAlpha;
    /* 0x4C */ f32 _4C;
};

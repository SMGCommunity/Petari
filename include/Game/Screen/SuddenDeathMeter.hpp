#pragma once

#include "Game/LiveActor/Nerve.hpp"
#include "Game/Screen/LayoutActor.hpp"
#include "JSystem/JGeometry/TVec.hpp"

class CountUpPaneRumbler;

class SuddenDeathMeter : public LayoutActor {
public:
    SuddenDeathMeter(const char *, const char *);

    virtual ~SuddenDeathMeter();

    virtual void init(const JMapInfoIter &rIter);
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
    void calcMarioHeadPosition(TVec2f *) const;
    void calcPowerUpMeterBasePosition(TVec2f *) const;
    void setPowerUpMeterPosition(const TVec2f &);
    void setAnimBase();
    void setCountAnimFrame();
    void setRecoveryCountAnimFrame();

    CountUpPaneRumbler *mCountUpPaneRumbler;    // 0x20
    TVec2f mFollowPosW;                         // 0x24
    TVec2f _2C;                                 // 0x2C
    s32 mCount;                                 // 0x34
    f32 _38;                                    // 0x38
    f32 _3C;                                    // 0x3C
};

namespace NrvSuddenDeathMeter {
    NERVE_DECL_EXE(SuddenDeathMeterNrvAppear, SuddenDeathMeter, Appear);
    NERVE_DECL_EXE(SuddenDeathMeterNrvWait, SuddenDeathMeter, Wait);
    NERVE_DECL_EXE(SuddenDeathMeterNrvPowerUp, SuddenDeathMeter, PowerUp);
    NERVE_DECL_EXE(SuddenDeathMeterNrvMeterMove, SuddenDeathMeter, MeterMove);
    NERVE_DECL_EXE(SuddenDeathMeterNrvCounterMove, SuddenDeathMeter, CounterMove);
    NERVE_DECL_EXE(SuddenDeathMeterNrvBreakMeter, SuddenDeathMeter, BreakMeter);
    NERVE_DECL_EXE(SuddenDeathMeterNrvZeroMeter, SuddenDeathMeter, ZeroMeter);
    NERVE_DECL_EXE(SuddenDeathMeterNrvZeroMeterBreak, SuddenDeathMeter, ZeroMeterBreak);
}    // namespace NrvSuddenDeathMeter
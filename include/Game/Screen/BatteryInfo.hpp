#pragma once
#include "Game/Screen/LayoutActor.hpp"
#include "Game/System/NerveExecutor.hpp"
#include "revolution/types.h"

class BatteryInfo : public NerveExecutor {
public:
    BatteryInfo(LayoutActor *pLayout, const char *, s32);
    virtual ~BatteryInfo();

    void exeEnought();
    void exeLowAppear();
    void exeLow();
    void exeLowDisappear();
    void exeLowHide();
    void exeCriticalAppear();
    void exeCritical();
    void exeDisappear();
    void exeNoneAppear();
    void exeNone();
    void exeNoneDisappear();
    void exeNoneHide();
    bool exeStartAnimAndSetNerveIfStopped(const char *pAnimName, const Nerve *pNerve);
    bool tryChangeNerveWithBatteryLeftAlreadyAppear();

    /*  0x8 */ LayoutActor* mLayoutActor;
    /*  0xC */ const char* mPaneName;
    /* 0x10 */ s32 mChannel;
};

class BatteryLayout : public LayoutActor {
public:
    BatteryLayout();

    virtual ~BatteryLayout();
    virtual void init(const JMapInfoIter &rIter);
    virtual void appear();
    virtual void control();

    /* 0x20 */ BatteryInfo* mBatteryInfo1;
    /* 0x24 */ BatteryInfo* mBatteryInfo2;
};

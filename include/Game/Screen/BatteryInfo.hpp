#pragma once

#include "Game/Screen/LayoutActor.hpp"
#include "Game/System/NerveExecutor.hpp"

class BatteryInfo : public NerveExecutor {
public:
    BatteryInfo(LayoutActor*, const char*, int);

    virtual ~BatteryInfo() {}

    void exeEnought();
    void exeLowAppear();
    void exeLow();
    void exeLowDisappear();
    void exeLowHide();
    void exeDisappear();
    void exeCriticalAppear();
    void exeCritical();
    void exeNoneAppear();
    void exeNone();
    void exeNoneDisappear();
    void exeNoneHide();
    bool exeStartAnimAndSetNerveIfStopped(const char*, const Nerve*);
    bool tryChangeNerveWithBatteryLeftAlreadyAppear();

private:
    /* 0x08 */ LayoutActor* mHost;
    /* 0x0C */ const char* mPaneName;
    /* 0x10 */ int mChannel;
};

class BatteryLayout : public LayoutActor {
public:
    BatteryLayout();

    virtual ~BatteryLayout() {}
    virtual void init(const JMapInfoIter& rIter);
    virtual void appear();
    virtual void control();

private:
    /* 0x20 */ BatteryInfo* mInfo1P;
    /* 0x24 */ BatteryInfo* mInfo2P;
};

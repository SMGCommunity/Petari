#pragma once

#include "Game/LiveActor/LiveActor.hpp"

class BegomanBase;

class BegomanLauncher : public LiveActor {
public:
    BegomanLauncher(const char* pName);

    virtual void init(const JMapInfoIter& rIter);
    virtual void initAfterPlacement();
    virtual void kill();

    void onSwitchAppear();
    void exeWaitAfterSwitchOn();
    void exeWait();
    void exeWaitForLaunch();
    void exeLaunch();
    void exeAppear();
    void exeDisappear();

    /* 0x8C */ s32 mBegomanCount;
    /* 0x90 */ s32 mAppearDelay;
    /* 0x94 */ BegomanBase** mBegomanArray;
    /* 0x98 */ s32 mLaunchType;
};

#pragma once

#include <revolution/types.h>

class DemoTimePartInfo {
public:
    const char* mName;  // _0
    s32 mTotalSteps;    // _4
    u8 _8;
};

class DemoTimeKeeper {
public:
    bool isExistSuspendFlagCurrentPart() const;
    bool isPartLast() const;
    void setCurrentPart(const char*);
    bool isCurrentDemoPartLastStep() const;

    u32 _0;
    DemoTimePartInfo* mMainPartInfos;  // _4
    DemoTimePartInfo* mSubPartInfos;   // _8
    s32 mNumPartInfos;
    u32 _10;
    s32 mCurrentStep;  // _14
    s32 _18;
    bool mIsPaused;  // _1C
};

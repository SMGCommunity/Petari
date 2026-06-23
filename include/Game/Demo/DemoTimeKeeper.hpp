#pragma once

#include <revolution/types.h>

class DemoExecutor;

class DemoTimePartInfo {
public:
    /// @brief Creates a new `DemoTimePartInfo`.
    DemoTimePartInfo();

    /* 0x00 */ const char* mPartName;
    /* 0x04 */ s32 mTotalStep;
    /* 0x08 */ bool mSuspendFlag;
};

class DemoTimeKeeper {
public:
    DemoTimeKeeper(const DemoExecutor*);

    void start();
    void update();
    void end();
    bool isDemoEnd() const;
    void setStartPart(const char*);
    bool isExistSuspendFlagCurrentPart() const;
    bool isPartLast() const;
    void setCurrentPart(const char*);
    bool isCurrentDemoPartLastStep() const;

    /* 0x00 */ const DemoExecutor* mExecutor;
    /* 0x04 */ DemoTimePartInfo* mMainPartInfos;
    /* 0x08 */ DemoTimePartInfo* mSubPartInfos;
    /* 0x0C */ s32 mNumPartInfos;
    /* 0x10 */ s32 _10;
    /* 0x14 */ s32 mCurrentStep;
    /* 0x18 */ s32 _18;
    /* 0x1C */ bool mIsPaused;
};

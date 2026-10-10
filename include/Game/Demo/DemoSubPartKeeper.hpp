#pragma once

#include <revolution/types.h>

class DemoExecutor;

class DemoSubPartInfo {
public:
    DemoSubPartInfo();

    /* 0x00 */ const char* mSubPartName;
    /* 0x04 */ s32 mSubPartTotalStep;
    /* 0x08 */ const char* mMainPartName;
    /* 0x0C */ s32 mMainPartStep;
    /* 0x10 */ s32 _10;
};

class DemoSubPartKeeper {
public:
    DemoSubPartKeeper(const DemoExecutor*);

    void update();
    void end();
    bool isDemoPartActive(const char*) const;
    s32 getDemoPartStep(const char*) const;
    s32 getDemoPartTotalStep(const char*) const;
    DemoSubPartInfo* findSubPart(const char*) const;

    /* 0x0 */ const DemoExecutor* mExecutor;
    /* 0x4 */ s32 mNumSubPartInfos;
    /* 0x8 */ DemoSubPartInfo* mSubPartInfos;
};

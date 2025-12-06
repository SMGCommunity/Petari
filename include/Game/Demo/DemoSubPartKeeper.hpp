#pragma once

#include "Game/Util/StringUtil.hpp"

class DemoExecutor;

class DemoSubPartInfo {
public:
    DemoSubPartInfo();

    const char* mSubPartName;   // 0x0
    s32 mSubPartTotalStep;      // 0x4
    const char* mMainPartName;  // 0x8
    s32 mMainPartStep;          // 0xC
    s32 _10;
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

    const DemoExecutor* mExecutor;   // 0x0
    s32 mNumSubPartInfos;            // 0x4
    DemoSubPartInfo* mSubPartInfos;  // 0x8
};

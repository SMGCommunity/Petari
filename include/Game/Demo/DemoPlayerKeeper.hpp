#pragma once

#include <Game/Demo/DemoExecutor.hpp>

class DemoPlayerInfo {
public:
    DemoPlayerInfo();

    const char* mPartName;  // 0x0
    const char* mPosName;   // 0x4
    const char* mBckName;   // 0x8
};

class DemoPlayerKeeper {
public:
    DemoPlayerKeeper(const DemoExecutor*);
    void update();
    bool isExistPosName() const;
    void executePlayer(const DemoPlayerInfo*) const;

    const DemoExecutor* mExecutor;  // 0x0
    s32 mNumPlayerInfos;            // 0x4
    DemoPlayerInfo* mPlayerInfos;   // 0x8
};

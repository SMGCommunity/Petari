#pragma once

#include "Game/Demo/DemoExecutor.hpp"

class DemoPlayerInfo {
public:
    DemoPlayerInfo();

    /* 0x0 */ const char* mPartName;
    /* 0x4 */ const char* mPosName;
    /* 0x8 */ const char* mBckName;
};

class DemoPlayerKeeper {
public:
    DemoPlayerKeeper(const DemoExecutor*);
    void update();
    bool isExistPosName() const;
    void executePlayer(const DemoPlayerInfo*) const;

    /* 0x0 */ const DemoExecutor* mExecutor;
    /* 0x4 */ s32 mNumPlayerInfos;
    /* 0x8 */ DemoPlayerInfo* mPlayerInfos;
};

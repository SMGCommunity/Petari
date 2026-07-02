#pragma once

#include "Game/Demo/DemoExecutor.hpp"

class DemoWipeInfo {
public:
    /// @brief Creates a new `DemoWipeInfo`.
    DemoWipeInfo();

    /* 0x00 */ const char* mPartName;
    /* 0x04 */ const char* mWipeName;
    /* 0x08 */ s32 mWipeType;
    /* 0x0C */ s32 mWipeFrame;
};

class DemoWipeKeeper : public DemoSheetKeeperBase, public DemoSheetKeeperInfoHolder< DemoWipeInfo > {
public:
    DemoWipeKeeper(DemoExecutor*);

    virtual const char* getName() {
        return "ワイプ";
    }

    virtual const char* getTypeString() {
        return "Wipe";
    }

    virtual void start();
    virtual void update();

    virtual void executeType(const DemoWipeInfo*);
};

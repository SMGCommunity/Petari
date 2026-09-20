#pragma once

#include "Game/Demo/DemoExecutor.hpp"

class DemoSoundInfo {
public:
    /// @brief Creates a new `DemoSoundInfo`.
    DemoSoundInfo();

    /* 0x00 */ const char* mPartName;
    /* 0x04 */ const char* mBgm;
    /* 0x08 */ const char* mSystemSe;
    /* 0x0C */ bool mReturnBgm;
    /* 0x10 */ s32 mBgmWipeoutFrame;
};

class DemoSoundKeeper : public DemoSheetKeeperBase, public DemoSheetKeeperInfoHolder< DemoSoundInfo > {
public:
    DemoSoundKeeper(DemoExecutor* pExecutor);

    virtual const char* getName() const;

    virtual const char* getTypeString() const;

    virtual void update();

    virtual void executeType(const DemoSoundInfo* pInfo);

    void addInfo(const char* pPartName);
    bool isPermitBgmChange();
};

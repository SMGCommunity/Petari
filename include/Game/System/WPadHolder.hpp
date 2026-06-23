#pragma once

#include "revolution/kpad.h"

class WPad;

class WPadReadDataInfo {
public:
    WPadReadDataInfo();

    KPADStatus* getKPadStatus(u32) const;
    u32 getValidStatusCount() const;

    /* 0x00 */ KPADStatus* mStatusArray;
    /* 0x04 */ u32 mValidStatusCount;
};

struct HBMKPadData {
    /* 0x0 */ KPADStatus* status;
    /* 0x4 */ Vec2 pos;
    /* 0xC */ u32 dev_type;
};

class WPadHolder {
public:
    WPadHolder();

    void updateReadDataOnly();
    void updateProjectPadData();
    void updateInGame();
    void update();
    void initSensorBarPosition();
    void resetPad();
    WPad* getWPad(s32);
    static void setConnectCallback();

    /* 0x00 */ WPad* mPad[2];
    /* 0x08 */ WPadReadDataInfo* mReadDataInfoArray;
    /* 0x0C */ u32 mMode;
};

namespace MR {
    WPad* getWPad(s32);
    void resetWPad();
    void setWPadHolderModeHomeButton();
    void setWPadHolderModeGame();
    void getHBMKPadData(HBMKPadData*, s32);
    void setAutoSleepTimeWiiRemote(bool);
};  // namespace MR

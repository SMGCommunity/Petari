#pragma once

#include <revolution/types.h>

class WPad;
struct HBMKPadData;
struct KPADStatus;

class WPadReadDataInfo {
public:
    WPadReadDataInfo();

    KPADStatus* getKPadStatus(u32) const;
    u32 getValidStatusCount() const;

    /* 0x00 */ KPADStatus* mStatusArray;
    /* 0x04 */ u32 mValidStatusCount;
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

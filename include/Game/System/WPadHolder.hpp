#pragma once

#include <revolution.h>

class WPad;

class WPadReadDataInfo {
public:
    WPadReadDataInfo();

    KPADStatus* getKPadStatus(u32) const;
    u32 getValidStatusCount() const;

    KPADStatus* mStatusArray;       // 0x0 
    u32 mValidStatusCount;          // 0x4
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

    WPad* mPads[2];                             // 0x0
    WPadReadDataInfo* mDataInfoArray;           // 0x8
    u32 mHolderMode;                            // 0xC
};
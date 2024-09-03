#pragma once

#include <revolution.h>

class WPadButton;
class WPadPointer;
class WPadAcceleration;
class WPadHVSwing;
class WPadRumble;
class WPadStick;
class WPadLeaveWatcher;
class WPadInfoChecker;
class WPadReadDataInfo;

class WPad {
public:
    WPad(s32);

    u32 getValidStatusCount() const;
    KPADStatus* getKPadStatus(u32) const;
    void setReadInfo(WPadReadDataInfo *);

    void update();
    void resetPad();

    static void getInfoCallback(s32, s32);

    s32 mChannel;                           // 0x0
    u32 _4;
    WPadButton* mButtons;                   // 0x8
    WPadPointer* mPointer;                  // 0xC
    WPadAcceleration* _10;
    WPadHVSwing* _14;
    WPadRumble* _18;
    WPadRumble* _1C;
    WPadStick* mStick;                      // 0x20
    WPadAcceleration* _24;
    WPadHVSwing* _28;
    WPadLeaveWatcher* mLeaveWatcher;        // 0x2C
    WPadInfoChecker* mChecker;              // 0x30
    u8 _34;
    u8 _35;
    u8 _36;
};
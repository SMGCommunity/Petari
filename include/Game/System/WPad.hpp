#pragma once

#include <JSystem/JGeometry/TVec.hpp>
#include "revolution/kpad.h"
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

    void setReadInfo(WPadReadDataInfo *);
    void update();
    WPadRumble* getRumbleInstance() const;
    void startLeaveWatcher();
    void stopLeaveWatcher();
    void restartLeaveWatcher();
    s32 getBattery() const;
    void resetPad();
    void connected();
    void disconnected();
    KPADStatus* getKPadStatus(u32) const;
    u32 getValidStatusCount() const;
    u32 getEnableAccelPastCount(u32) const;
    void getAcceleration(TVec3f *, u32) const;
    void getPastAcceleration(TVec3f *, u32) const;

    static void connectCallback(s32, s32);
    static void extensionCallback(s32, s32);
    static void getInfoCallback(s32, s32);

    s32 mChannel;                           // 0x0
    u32 _4;
    WPadButton* mButtons;                   // 0x8
    WPadPointer* mPointer;                  // 0xC
    WPadAcceleration* mCorePadAccel;        // 0x10
    WPadHVSwing* mCorePadSwing;             // 0x14
    WPadRumble* _18;
    WPadRumble* _1C;
    WPadStick* mStick;                      // 0x20
    WPadAcceleration* mSubPadAccel;         // 0x24
    WPadHVSwing* mSubPadSwing;              // 0x28
    WPadLeaveWatcher* mLeaveWatcher;        // 0x2C
    WPadInfoChecker* mChecker;              // 0x30
    u8 _34;
    bool mIsConnected;                      // 0x35
    u8 _36;
};

namespace MR {
    bool isDeviceFreeStyle(const KPADStatus *);
};

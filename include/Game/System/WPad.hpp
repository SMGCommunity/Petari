#pragma once

#include <JSystem/JGeometry/TVec.hpp>
#include <revolution/types.h>

class WPadAcceleration;
class WPadButton;
class WPadHVSwing;
class WPadInfoChecker;
class WPadLeaveWatcher;
class WPadPointer;
class WPadReadDataInfo;
class WPadRumble;
class WPadStick;
struct KPADStatus;

class WPad {
public:
    WPad(s32);

    void setReadInfo(WPadReadDataInfo*);
    void update();
    WPadRumble* getRumbleInstance() const;
    void startLeaveWatcher();
    void stopLeaveWatcher();
    void restartLeaveWatcher();
    s32 getBattery() const;
    void resetPad();
    void connected();
    void disconnected();
    static void connectCallback(s32, s32);
    static void extensionCallback(s32, s32);
    static void getInfoCallback(s32, s32);
    KPADStatus* getKPadStatus(u32) const;
    s32 getValidStatusCount() const;
    s32 getEnableAccelPastCount(u32) const;
    bool getAcceleration(TVec3f*, u32) const;
    bool getPastAcceleration(TVec3f*, s32, u32) const;

    /* 0x00 */ s32 mChannel;
    /* 0x04 */ WPadReadDataInfo* mReadInfo;
    /* 0x08 */ WPadButton* mButton;
    /* 0x0C */ WPadPointer* mPointer;
    /* 0x10 */ WPadAcceleration* mCorePadAccel;
    /* 0x14 */ WPadHVSwing* mCorePadSwing;
    /* 0x18 */ WPadRumble* _18;
    /* 0x1C */ WPadRumble* _1C;
    /* 0x20 */ WPadStick* mStick;
    /* 0x24 */ WPadAcceleration* mSubPadAccel;
    /* 0x28 */ WPadHVSwing* mSubPadSwing;
    /* 0x2C */ WPadLeaveWatcher* mLeaveWatcher;
    /* 0x30 */ WPadInfoChecker* mInfoChecker;
    /* 0x34 */ bool _34;
    /* 0x35 */ bool mIsConnected;
    /* 0x36 */ bool mIsSubPadConnected;
};

namespace MR {
    bool isDeviceFreeStyle(const KPADStatus*);
    void getPadDataForExceptionNoInit(u32, u32*, u32*);
};  // namespace MR

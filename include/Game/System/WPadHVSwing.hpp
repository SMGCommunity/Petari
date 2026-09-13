#pragma once

#include <revolution.h>

class WPad;

class WPadHVSwing {
public:
    WPadHVSwing(const WPad*, u32);

    void updateSwing();
    void updateCentrifugal();
    void update();

    /* 0x00 */ const WPad* pPad;
    /* 0x04 */ s32 mChannel;
    /* 0x08 */ f32 mDistanceSwingThreshold;
    /* 0x0C */ bool mIsSwing;
    /* 0x0D */ bool mIsSwingLatched;
    /* 0x10 */ s32 mSwingLatchedFrames;
    /* 0x14 */ s32 mSwingBelowThresholdFrames;
    /* 0x18 */ bool mIsTriggerSwing;
    /* 0x1C */ f32 mSwingThreshold;
    /* 0x20 */ bool mSwingDetected;
    /* 0x21 */ bool mSwingTriggered;
    /* 0x24 */ s32 mSwingHoldFrames;
    /* 0x28 */ s32 mSwingCooldownFrames;
};

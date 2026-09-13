#pragma once

#include <revolution.h>

class WPad;

class WPadHVSwing {
public:
    WPadHVSwing(const WPad*, u32);

    void updateSwing();
    void updateCentrifugal();
    void update();

    const WPad* mPad;  // 0x0
    s32 mChannel;
    f32 mDistanceSwingThreshold;
    bool mIsSwing;  // 0xC
    bool mIsSwingLatched;
    s32 mSwingLatchedFrames;
    s32 mSwingBelowThresholdFrames;
    bool mIsTriggerSwing;  // 0x18
    f32 mSwingThreshold;
    bool mSwingDetected;
    bool mSwingTriggered;
    s32 mSwingHoldFrames;
    s32 mSwingCooldownFrames;
};

#pragma once

#include <JSystem/JGeometry/TVec.hpp>
#include <revolution/types.h>

class WPad;

class WPadStick {
public:
    WPadStick(const WPad*);

    void update();
    bool isChanged() const;

    /* 0x00 */ const WPad* mPad;
    /* 0x04 */ TVec2f mStick;
    /* 0x0C */ f32 mSpeed;
    /* 0x10 */ u32 mHold;
    /* 0x14 */ u32 mTrigger;
    /* 0x18 */ u32 mRelease;
    /* 0x1C */ bool mIsTriggerUp;
    /* 0x1D */ bool mIsTriggerDown;
    /* 0x1E */ bool mIsHoldUp;
    /* 0x1F */ bool mIsHoldDown;
};

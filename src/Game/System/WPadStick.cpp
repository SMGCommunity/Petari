#include "Game/System/WPad.hpp"
#include "Game/System/WPadStick.hpp"
#include <JSystem/JGeometry/TUtil.hpp>

#define STICK_FLAG_NONE 0x0
#define STICK_FLAG_UP 0x1
#define STICK_FLAG_DOWN 0x2
#define STICK_FLAG_RIGHT 0x4
#define STICK_FLAG_LEFT 0x8

namespace {
    static const f32 sIsOnValue = 0.2f;
};

WPadStick::WPadStick(const WPad* pPad)
    : mPad(pPad),
      mStick(0.0f, 0.0f),
      mSpeed(0.0f),
      mHold(STICK_FLAG_NONE),
      mTrigger(STICK_FLAG_NONE),
      mRelease(STICK_FLAG_NONE),
      mIsTriggerUp(false),
      mIsTriggerDown(false),
      mIsHoldUp(false),
      mIsHoldDown(false) {
}

void WPadStick::update() {
    KPADStatus* pStatus = mPad->getKPadStatus(0);

    if (!MR::isDeviceFreeStyle(pStatus)) {
        return;
    }

    TVec2f stickEx(pStatus->ex_status.fs.stick.x, pStatus->ex_status.fs.stick.y);
    TVec2f stickPrev = mStick;
    f32    deltaX = stickEx.x - stickPrev.x;
    f32    deltaY = stickEx.y - stickPrev.y;
    mSpeed = JGeometry::TUtil<f32>::sqrt(deltaX * deltaX + deltaY * deltaY);
    u32 flagPrev = STICK_FLAG_NONE;

    if (stickPrev.x > sIsOnValue) {
        flagPrev |= STICK_FLAG_RIGHT;
    }

    if (stickPrev.x < -sIsOnValue) {
        flagPrev |= STICK_FLAG_LEFT;
    }

    if (stickPrev.y > sIsOnValue) {
        flagPrev |= STICK_FLAG_UP;
    }

    if (stickPrev.y < -sIsOnValue) {
        flagPrev |= STICK_FLAG_DOWN;
    }

    u32 flag = STICK_FLAG_NONE;

    if (mStick.x > sIsOnValue) {
        flag |= STICK_FLAG_RIGHT;
    }

    if (mStick.x < -sIsOnValue) {
        flag |= STICK_FLAG_LEFT;
    }

    if (mStick.y > sIsOnValue) {
        flag |= STICK_FLAG_UP;
    }

    if (mStick.y < -sIsOnValue) {
        flag |= STICK_FLAG_DOWN;
    }

    mHold = flag;
    mTrigger = flag & ~flagPrev;
    mRelease = flagPrev & ~flag;

    if (mStick.y == 0.0f) {
        mIsTriggerUp = false;
        mIsTriggerDown = false;
        mIsHoldUp = false;
        mIsHoldDown = false;
    } else if (mStick.y > 0.0f) {
        if (mIsHoldUp) {
            mIsTriggerUp = false;
        } else {
            mIsTriggerUp = true;
        }

        mIsHoldUp = true;
        mIsHoldDown = false;
    } else {
        if (mIsHoldDown) {
            mIsTriggerDown = false;
        } else {
            mIsTriggerDown = true;
        }

        mIsHoldDown = true;
        mIsHoldUp = false;
    }
}

bool WPadStick::isChanged() const {
    return mSpeed > 0.0001f;
}

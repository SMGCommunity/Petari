#include "Game/System/WPadLeaveWatcher.hpp"
#include "Game/System/WPad.hpp"
#include "Game/System/WPadAcceleration.hpp"
#include "Game/System/WPadButton.hpp"
#include "Game/System/WPadPointer.hpp"
#include "Game/System/WPadStick.hpp"

namespace {
    static bool sIsCheckPointerMoved = false;
    static bool sIsCheckCoreAccel = true;
    static bool sIsCheckSubAccel = true;
    static bool sIsCheckButton = true;
    static bool sIsCheckStick = true;
    static const s32 sLeaveLongTime = 3600;
};  // namespace

WPadLeaveWatcher::WPadLeaveWatcher(WPad* pPad) : mPad(pPad), mStep(0), mIsSuspend(false) {
}

void WPadLeaveWatcher::update() {
    if (mIsSuspend) {
        return;
    }

    if (mPad->mPointer->mIsPointerMoved && ::sIsCheckPointerMoved || !mPad->mCorePadAccel->isStationary() && ::sIsCheckCoreAccel ||
        !mPad->mSubPadAccel->isStationary() && ::sIsCheckSubAccel || mPad->mButton->isChangeAnyState() && ::sIsCheckButton ||
        mPad->mStick->isChanged() && ::sIsCheckStick) {
        mStep = 0;
    } else if (mStep < ::sLeaveLongTime) {
        mStep++;
    }
}

void WPadLeaveWatcher::start() {
    mIsSuspend = false;
}

void WPadLeaveWatcher::stop() {
    mIsSuspend = true;
}

void WPadLeaveWatcher::restart() {
    mStep = 0;
    mIsSuspend = false;
}

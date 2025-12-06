#include "Game/System/WPadLeaveWatcher.hpp"

namespace {
    static const s32 sLeaveLongTime = 3600;
};

WPadLeaveWatcher::WPadLeaveWatcher(WPad* pPad) : mPad(pPad), mStep(0), mIsSuspend(false) {}

// WPadLeaveWatcher::update

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

#include "Game/System/WPadInfoChecker.hpp"
#include "Game/System/WPad.hpp"

namespace {
    static const s32 sCheckInfoInterval = 18000;
};

WPadInfoChecker::WPadInfoChecker(WPad* pPad) : mPad(pPad) {
    reset();
}

s32 WPadInfoChecker::getBattery() const {
    return mBattery;
}

void WPadInfoChecker::successGetInfo() {
    mBattery = mPadInfo.battery;
}

void WPadInfoChecker::startCheck() {
    WPADGetInfoAsync(mPad->mChannel, &mPadInfo, WPad::getInfoCallback);

    mCheckInfoFrame = sCheckInfoInterval;
}

void WPadInfoChecker::reset() {
    mCheckInfoFrame = 0;
    mBattery = -1;
}

void WPadInfoChecker::update() {
    if (mPad->mIsConnected) {
        if (mCheckInfoFrame <= 0) {
            startCheck();
        } else {
            mCheckInfoFrame--;
        }
    } else {
        reset();
    }
}

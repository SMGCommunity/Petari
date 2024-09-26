#include "Game/System/WPadInfoChecker.hpp"

WPadInfoChecker::WPadInfoChecker(WPad *pPad) {
    mPad = pPad;
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
    _1C = 0x4650;
}

void WPadInfoChecker::reset() {
    _1C = 0;
    mBattery = -1;
}

void WPadInfoChecker::update() {
    if (mPad->_35) {
        if (_1C <= 0) {
            startCheck();
        }
        else {
            _1C--;
        }
    }
    else {
        reset();
    }
}
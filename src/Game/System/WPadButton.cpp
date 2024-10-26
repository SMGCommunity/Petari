#include "Game/System/WPadButton.hpp"

WPadButton::WPadButton(const WPad* pPad) {
    mPad = pPad;
    mHold = 0;
    _8 = 0;
    mRelease = 0;
    _10 = 0;
    mDelaySec = 0.41666666;
    mPulseSec = 0.16666667;
    KPADSetBtnRepeat(mPad->mChannel, mDelaySec, mPulseSec);
}

void WPadButton::update() {
    KPADStatus* status = mPad->getKPadStatus(0);

    if (status != nullptr) {
        u32 v3;
        if (mHold != 0) {
            _8 = status->trig & ~mHold & 0xFFFF;
        }
        else {
            _8 = status->trig;
        }

        if (!status->wpad_err || status->wpad_err == -2) {
            mHold = status->hold;
            mRelease = status->release;
        }

        _10 = _8;

        if ((status->hold & 0x80000000) != 0) {
            _10 = _8 | mHold;
        }
    }
}

bool WPadButton::testButtonUp() const {
    return (mHold >> 3) & 0x1;
}

bool WPadButton::testButtonDown() const {
    return (mHold >> 2) & 0x1;
}

bool WPadButton::testButtonLeft() const {
    return mHold & 0x1;
}

bool WPadButton::testButtonRight() const {
    return (mHold >> 1) & 0x1;
}

bool WPadButton::testButtonA() const {
    return (mHold >> 11) & 0x1;
}

bool WPadButton::testButtonB() const {
    return (mHold >> 10) & 0x1;
}

bool WPadButton::testButtonMinus() const {
    return (mHold >> 12) & 0x1;
}

bool WPadButton::testButtonPlus() const {
    return (mHold >> 4) & 0x1;
}

bool WPadButton::testButton1() const {
    return (mHold >> 9) & 0x1;
}

bool WPadButton::testButton2() const {
    return (mHold >> 8) & 0x1;
}

bool WPadButton::testButtonC() const {
    return (mHold >> 14) & 0x1;
}

bool WPadButton::testButtonZ() const {
    return (mHold >> 13) & 0x1;
}

bool WPadButton::testTriggerUp() const {
    return (mHold >> 3) & 0x1;
}

bool WPadButton::testTriggerDown() const {
    return (mHold >> 2) & 0x1;
}

bool WPadButton::testTriggerLeft() const {
    return _8 & 0x1;
}

bool WPadButton::testTriggerRight() const {
    return (_8 >> 1) & 0x1;
}

bool WPadButton::testTriggerA() const {
    return (_8 >> 11) & 0x1;
}

bool WPadButton::testTriggerB() const {
    return (_8 >> 10) & 0x1;
}

bool WPadButton::testTriggerMinus() const {
    return (_8 >> 12) & 0x1;
}

bool WPadButton::testTriggerHome() const {
    return (_8 >> 15) & 0x1;
}

bool WPadButton::testTriggerPlus() const {
    return (_8 >> 4) & 0x1;
}

bool WPadButton::testTrigger1() const {
    return (_8 >> 9) & 0x1;
}

bool WPadButton::testTrigger2() const {
    return (_8 >> 8) & 0x1;
}

bool WPadButton::testTriggerC() const {
    return (_8 >> 14) & 0x1;
}

bool WPadButton::testTriggerZ() const {
    return (_8 >> 13) & 0x1;
}

bool WPadButton::testReleaseZ() const {
    return (mRelease >> 13) & 0x1;
}

bool WPadButton::isChangeAnyState() const {
    bool result = false;

    if (_8 || mRelease) {
        result = true;
    }
    
    return result;
}
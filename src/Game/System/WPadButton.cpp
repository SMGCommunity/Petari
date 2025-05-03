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
        if (mHold != 0) {
            _8 = status->trig & ~mHold & KPAD_BUTTON_MASK;
        }
        else {
            _8 = status->trig;
        }

        if (status->wpad_err == WPAD_ERR_NONE || status->wpad_err == WPAD_ERR_BUSY) {
            mHold = status->hold;
            mRelease = status->release;
        }

        _10 = _8;

        if ((status->hold & KPAD_BUTTON_RPT) != 0) {
            _10 = _8 | mHold;
        }
    }
}

bool WPadButton::testButtonUp() const {
    return (mHold & WPAD_BUTTON_UP) != 0;
}

bool WPadButton::testButtonDown() const {
    return (mHold & WPAD_BUTTON_DOWN) != 0;
}

bool WPadButton::testButtonLeft() const {
    return (mHold & WPAD_BUTTON_LEFT) != 0;
}

bool WPadButton::testButtonRight() const {
    return (mHold & WPAD_BUTTON_RIGHT) != 0;
}

bool WPadButton::testButtonA() const {
    return (mHold & WPAD_BUTTON_A) != 0;
}

bool WPadButton::testButtonB() const {
    return (mHold & WPAD_BUTTON_B) != 0;
}

bool WPadButton::testButtonMinus() const {
    return (mHold & WPAD_BUTTON_MINUS) != 0;
}

bool WPadButton::testButtonPlus() const {
    return (mHold & WPAD_BUTTON_PLUS) != 0;
}

bool WPadButton::testButton1() const {
    return (mHold & WPAD_BUTTON_1) != 0;
}

bool WPadButton::testButton2() const {
    return (mHold & WPAD_BUTTON_2) != 0;
}

bool WPadButton::testButtonC() const {
    return (mHold & WPAD_BUTTON_C) != 0;
}

bool WPadButton::testButtonZ() const {
    return (mHold & WPAD_BUTTON_Z) != 0;
}

bool WPadButton::testTriggerUp() const {
    return (_8 & WPAD_BUTTON_UP) != 0;
}

bool WPadButton::testTriggerDown() const {
    return (_8 & WPAD_BUTTON_DOWN) != 0;
}

bool WPadButton::testTriggerLeft() const {
    return (_8 & WPAD_BUTTON_LEFT) != 0;
}

bool WPadButton::testTriggerRight() const {
    return (_8 & WPAD_BUTTON_RIGHT) != 0;
}

bool WPadButton::testTriggerA() const {
    return (_8 & WPAD_BUTTON_A) != 0;
}

bool WPadButton::testTriggerB() const {
    return (_8 & WPAD_BUTTON_B) != 0;
}

bool WPadButton::testTriggerMinus() const {
    return (_8 & WPAD_BUTTON_MINUS) != 0;
}

bool WPadButton::testTriggerHome() const {
    return (_8 & WPAD_BUTTON_HOME) != 0;
}

bool WPadButton::testTriggerPlus() const {
    return (_8 & WPAD_BUTTON_PLUS) != 0;
}

bool WPadButton::testTrigger1() const {
    return (_8 & WPAD_BUTTON_1) != 0;
}

bool WPadButton::testTrigger2() const {
    return (_8 & WPAD_BUTTON_2) != 0;
}

bool WPadButton::testTriggerC() const {
    return (_8 & WPAD_BUTTON_C) != 0;
}

bool WPadButton::testTriggerZ() const {
    return (_8 & WPAD_BUTTON_Z) != 0;
}

bool WPadButton::testReleaseZ() const {
    return (mRelease & WPAD_BUTTON_Z) != 0;
}

bool WPadButton::isChangeAnyState() const {
    bool result = false;

    if (_8 || mRelease) {
        result = true;
    }
    
    return result;
}

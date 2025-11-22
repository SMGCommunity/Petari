#include "Game/System/WPadButton.hpp"
#include "Game/System/WPad.hpp"

WPadButton::WPadButton(const WPad* pPad)
    : mPad(pPad), mHold(0), mTrigger(0), mRelease(0), mRepeat(0), mDelaySec(1.0f / 2.4f), mPulseSec(1.0f / 6.0f) {
    KPADSetBtnRepeat(mPad->mChannel, mDelaySec, mPulseSec);
}

void WPadButton::update() {
    KPADStatus* pStatus = mPad->getKPadStatus(0);

    if (pStatus == nullptr) {
        return;
    }

    if (mHold != 0) {
        mTrigger = pStatus->trig & ~mHold & KPAD_BUTTON_MASK;
    } else {
        mTrigger = pStatus->trig;
    }

    if (pStatus->wpad_err == WPAD_ERR_NONE || pStatus->wpad_err == WPAD_ERR_BUSY) {
        mHold = pStatus->hold;
        mRelease = pStatus->release;
    }

    mRepeat = mTrigger;

    if ((pStatus->hold & KPAD_BUTTON_RPT) != 0) {
        mRepeat |= mHold;
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
    return (mTrigger & WPAD_BUTTON_UP) != 0;
}

bool WPadButton::testTriggerDown() const {
    return (mTrigger & WPAD_BUTTON_DOWN) != 0;
}

bool WPadButton::testTriggerLeft() const {
    return (mTrigger & WPAD_BUTTON_LEFT) != 0;
}

bool WPadButton::testTriggerRight() const {
    return (mTrigger & WPAD_BUTTON_RIGHT) != 0;
}

bool WPadButton::testTriggerA() const {
    return (mTrigger & WPAD_BUTTON_A) != 0;
}

bool WPadButton::testTriggerB() const {
    return (mTrigger & WPAD_BUTTON_B) != 0;
}

bool WPadButton::testTriggerMinus() const {
    return (mTrigger & WPAD_BUTTON_MINUS) != 0;
}

bool WPadButton::testTriggerHome() const {
    return (mTrigger & WPAD_BUTTON_HOME) != 0;
}

bool WPadButton::testTriggerPlus() const {
    return (mTrigger & WPAD_BUTTON_PLUS) != 0;
}

bool WPadButton::testTrigger1() const {
    return (mTrigger & WPAD_BUTTON_1) != 0;
}

bool WPadButton::testTrigger2() const {
    return (mTrigger & WPAD_BUTTON_2) != 0;
}

bool WPadButton::testTriggerC() const {
    return (mTrigger & WPAD_BUTTON_C) != 0;
}

bool WPadButton::testTriggerZ() const {
    return (mTrigger & WPAD_BUTTON_Z) != 0;
}

bool WPadButton::testReleaseZ() const {
    return (mRelease & WPAD_BUTTON_Z) != 0;
}

bool WPadButton::isChangeAnyState() const {
    return mTrigger != 0 || mRelease != 0;
}

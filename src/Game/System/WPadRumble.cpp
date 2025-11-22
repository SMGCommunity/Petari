#include "Game/System/WPad.hpp"
#include "Game/System/WPadRumble.hpp"
#include "Game/System/WPadRumbleData.hpp"
#include "Game/Util/GamePadUtil.hpp"

WPadRumble** WPadRumble::sInstanceForCallback;

void RumbleChannel::clear() {
    _0 = nullptr;
    _8 = 0;
    _C = 0;
    _E = false;
    _4 = false;
    _10 = nullptr;
}

void RumbleChannel::update() {
    if (_0 == nullptr) {
        return;
    }

    if (_C >= _0->mFrame) {
        if (_4 == true) {
            _E = _0->mPattern[0];
            _C = 1;
        } else {
            clear();
        }
    } else {
        _E = _0->mPattern[0];
        _C++;
    }
}

void RumbleChannel::setPattern(const void* pParam1, const RumblePattern& rParam2, u32 param3, bool param4) {
    _8 = param3;
    _C = 0;
    _0 = &rParam2;
    _10 = pParam1;
    _4 = param4;
}

WPadRumble::WPadRumble(WPad* pPad)
    : mPad(pPad),
      _8(false),
      _C(1),
      _B0(0),
      _B4(0),
      _B8(false),
      _BC(0) {
    if (sInstanceForCallback == nullptr) {
        sInstanceForCallback = new WPadRumble*[MR::getWPadMaxCount()];

        for (u32 i = 0; i < MR::getWPadMaxCount(); i++) {
            sInstanceForCallback[i] = nullptr;
        }

        RumbleData::initHashValue();
        RumbleData::checkHashCollision();
    }

    for (u8 i = 0; i < sizeof(mChannel) / sizeof(*mChannel); i++) {
        mChannel[i].clear();
    }
}

WPadRumble::~WPadRumble() {
    s32 chan = mPad->mChannel;

    if (_8) {
        _8 = false;

        WPADControlMotor(chan, WPAD_MOTOR_STOP);
    }

    sInstanceForCallback[chan] = nullptr;
}

void WPadRumble::registInstance() {
    s32 chan = mPad->mChannel;

    sInstanceForCallback[chan] = this;

    if (_8) {
        WPADControlMotor(chan, WPAD_MOTOR_RUMBLE);
    }
}

void WPadRumble::pause() {
    WPADControlMotor(mPad->mChannel, WPAD_MOTOR_STOP);
}

void WPadRumble::stop() {
    _8 = false;

    pause();

    for (u8 i = 0; i < sizeof(mChannel) / sizeof(*mChannel); i++) {
        mChannel[i].clear();
    }
}

void WPadRumble::update() {
    if (_BC > 0) {
        _BC--;

        stop();
    } else {
        WPadRumble* pRumble = getRumbleInstance();

        if (pRumble != nullptr) {
            pRumble->updateRumble();
        }
    }
}

void WPadRumble::updateRumble() {
    bool b = false;

    for (u8 i = 0; i < sizeof(mChannel) / sizeof(*mChannel); i++) {
        mChannel[i].update();

        b |= mChannel[i]._E;
    }

    if (_B8) {
        _B4--;

        if (_B4 <= 0) {
            _B8 = 0;
            _B4 = 0;
        }

        pause();
    } else {
        if (b) {
            if (!_8) {
                _8 = true;

                WPADControlMotor(mPad->mChannel, WPAD_MOTOR_RUMBLE);
            }
        } else if (_8) {
            _8 = false;

            pause();
        }

        if (b) {
            _B0 = 5;
        } else {
            s32 temp = _B0 - 1;

            if (temp < -9) {
                temp = -9;
            } else {
                if (temp <= 5) {
                    temp = _B0;
                }
            }

            _B0 = temp;

            if (_B0 < 0) {
                _B4 = 0;
                return;
            }
        }

        _B4++;

        if (_B4 > 1800) {
            _B8 = true;
            _B4 = 1800;
        }
    }
}

WPadRumble* WPadRumble::getRumbleInstance() const {
    return sInstanceForCallback[mPad->mChannel];
}

bool WPadRumble::vibratePatternIfNotExist(const void* pParam1, const char* pPatternName) {
    return setRumblePatternIfNotExist(pParam1, *RumbleData::getPattern(pPatternName), false);
}

bool WPadRumble::setRumblePatternIfNotExist(const void* pParam1, const RumblePattern& rParam2, bool param3) {
    s32 v1 = -1;
    s32 v2 = -1;
    s32 v3 = -1;

    findRubmlePattern(pParam1, &v1, &v2, &v3, rParam2);

    if (v1 != -1) {
        return false;
    }

    if (v2 != -1) {
        // FIXME: Missing clrlwi instruction.
        _C++;

        mChannel[v2].setPattern(pParam1, rParam2, _C, param3);

        return true;
    }

    // FIXME: Missing clrlwi instruction.
    _C++;

    mChannel[v3].setPattern(pParam1, rParam2, _C, param3);

    return true;
}

// WPadRumble::findRubmlePattern

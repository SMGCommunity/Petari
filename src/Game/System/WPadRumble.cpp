#include "Game/System/WPadRumble.hpp"
#include "Game/System/WPad.hpp"
#include "Game/System/WPadRumbleData.hpp"
#include "Game/Util/GamePadUtil.hpp"
#include "Game/Util/MathUtil.hpp"

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
        _E = _0->mPattern[_C];
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

WPadRumble::WPadRumble(WPad* pPad) : mPad(pPad), _8(), _C(1), _B0(), _B4(), _B8(), _BC() {
    if (sInstanceForCallback == nullptr) {
        sInstanceForCallback = new WPadRumble*[MR::getWPadMaxCount()];

        for (u32 i = 0; i < MR::getWPadMaxCount(); i++) {
            sInstanceForCallback[i] = nullptr;
        }

        RumbleData::initHashValue();
        RumbleData::checkHashCollision();
    }

    for (u8 i = 0; i < ARRAY_SIZE(mChannel); i++) {
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

    for (u8 i = 0; i < ARRAY_SIZE(mChannel); i++) {
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

    for (u8 i = 0; i < ARRAY_SIZE(mChannel); i++) {
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
            _B0 = MR::clamp(_B0 - 1, -9, 5);

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
        _C = (_C + 1) & 0x7FFFFFFF;

        mChannel[v2].setPattern(pParam1, rParam2, _C, param3);

        return true;
    }

    _C = (_C + 1) & 0x7FFFFFFF;

    mChannel[v3].setPattern(pParam1, rParam2, _C, param3);

    return true;
}

bool WPadRumble::findRubmlePattern(const void* pOwner, s32* pExisting, s32* pEmpty, s32* pOldest, const RumblePattern& rPattern) {
    u32 oldest = -1;
    u8 oldestIndex = 0xFF;
    u8 emptyIndex = 0xFF;

    for (u8 i = 0; i < ARRAY_SIZE(mChannel); i++) {
        const RumblePattern* pPattern = mChannel[i]._0;

        if (pPattern == nullptr) {
            if (emptyIndex == 0xFF) {
                emptyIndex = i;
            }
        } else {
            if (pPattern->mHash == rPattern.mHash) {
                *pExisting = i;
                return true;
            }

            if (oldest > mChannel[i]._8) {
                oldest = mChannel[i]._8;
                oldestIndex = i;
            }
        }
    }

    if (emptyIndex != 0xFF) {
        *pEmpty = emptyIndex;
        return false;
    }

    *pOldest = oldestIndex;
    return false;
}

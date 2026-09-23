#include "Game/System/WPadAcceleration.hpp"
#include "Game/System/WPad.hpp"
#include "Game/Util/MathUtil.hpp"
#include <revolution/wpad.h>

void WPadAcceleration_FORCE_MATCH_SDATA2() {
    (void)1.0f;
    (void)0.0f;
}

namespace {
    bool sSelectDominantRotation;
}

WPadAcceleration::WPadAcceleration(const WPad* pPad, u32 device)
    : mPad(pPad), _4(device), _8(), _C(0.15f), _10(0, 0, 0), _1C(), _20(true), _624(-1), _628(), _62C(0, 0, 0), _638(0.0f, 0.0f, 0.0f), _644(128),
      _648(), _64C() {
    KPADSetAccParam(mPad->mChannel, _8, _C);
}

bool WPadAcceleration::getAcceleration(TVec3f* pOut) const {
    if (_628 <= 0) {
        pOut->zero();
        return false;
    }

    pOut->set(mHistory[_624]);
    return true;
}

bool WPadAcceleration::getPastAcceleration(TVec3f* pOut, s32 index) const {
    if (index >= _628) {
        pOut->zero();
        return false;
    }

    s32 slot = _624 - index;

    if (slot < 0) {
        slot += 128;
    }

    pOut->set(mHistory[slot]);
    return true;
}

bool WPadAcceleration::isStationary() const {
    return _20;
}

bool WPadAcceleration::isBalanced() const {
    return _1C < 0.018f;
}

void WPadAcceleration::update() {
    s32 statusCount = mPad->getValidStatusCount();
    if (statusCount <= 0) {
        TVec3f previous;

        if (_624 == -1) {
            previous.set< f32 >(0.0f, 0.0f, 0.0f);
        } else {
            previous = mHistory[_624];
        }

        _624++;

        if (_624 >= 128U) {
            _624 = 0;
        }

        mHistory[_624] = previous;

        if (_628 < 128U) {
            _628++;
        }
    }

    for (s32 i = statusCount - 1; i >= 0; i--) {
        KPADStatus* pStatus = mPad->getKPadStatus(i);

        if (pStatus == nullptr) {
            continue;
        }

        TVec3f acceleration;

        if (_4 == 0) {
            acceleration.x = -pStatus->acc.x;
            acceleration.y = pStatus->acc.z;
            acceleration.z = -pStatus->acc.y;
        } else {
            if (!MR::isDeviceFreeStyle(pStatus)) {
                continue;
            }

            acceleration.x = -pStatus->ex_status.fs.acc.x;
            acceleration.y = pStatus->ex_status.fs.acc.z;
            acceleration.z = -pStatus->ex_status.fs.acc.y;
        }

        _624++;

        if (_624 >= 128U) {
            _624 = 0;
        }

        mHistory[_624] = acceleration;

        if (_628 < 128U) {
            _628++;
        }
    }

    _62C.zero();

    for (s32 j = 0; j < _628; j++) {
        TVec3f acceleration;
        getPastAcceleration(&acceleration, j);
        _62C += acceleration;
    }

    if (_628 > 0) {
        _62C.scale(1.0f / _628);
    }

    updateRotate();
    updateAccAverage();
    updateIsStable();
}

void WPadAcceleration::updateRotate() {
    _638.zero();
    s32 count = _644;

    if (count >= _628) {
        count = _628;
    }

    if (count < 3) {
        return;
    }

    TVec3f latest;
    getPastAcceleration(&latest, 0);
    f32 zMagnitude = 0.0f;
    f32 xMagnitude = 0.0f;
    _648 = 0;
    _64C = 0;
    bool foundZ = false;
    bool foundX = false;

    for (s32 i = 2; i < count; i++) {
        TVec3f previous;
        TVec3f current;
        TVec3f cross;
        getPastAcceleration(&previous, i - 1);
        getPastAcceleration(&current, i);
        cross.cross(previous - latest, current - latest);
        _638 += cross;

        if (i < 20) {
            continue;
        }

        if (!foundZ && MR::abs(_638.z) > 6.0f) {
            foundZ = true;
            _648 = _638.z > 0.0f ? 1 : -1;
            zMagnitude = MR::abs(_638.z);
        }

        if (!foundX && MR::abs(_638.x) > 6.0f) {
            foundX = true;
            _64C = _638.x > 0.0f ? 1 : -1;
            xMagnitude = MR::abs(_638.z);
        }
    }

    if (::sSelectDominantRotation) {
        if (zMagnitude < xMagnitude) {
            _648 = 0;
        }

        if (xMagnitude < zMagnitude) {
            _64C = 0;
        }
    }
}

void WPadAcceleration::updateAccAverage() {
    f32 total = 0.0f;
    s32 count = 0;
    TVec3f previous(0.0f, 0.0f, 0.0f);

    if (getPastAcceleration(&previous, 0)) {
        count++;
    }

    for (s32 i = 1; i < 32; i++) {
        TVec3f current;

        if (!getPastAcceleration(&current, i)) {
            break;
        }

        total += current.squared(previous);
        previous = current;
        count++;
    }

    if (count > 0) {
        _1C = total / count;
    } else {
        _1C = 0.0f;
    }
}

void WPadAcceleration::updateIsStable() {
    TVec3f difference = mHistory[0] - _10;
    _20 = true;

    if (MR::abs(difference.x) >= 0.30f || MR::abs(difference.y) >= 0.30f || MR::abs(difference.z) >= 0.30f) {
        _10 = mHistory[0];
        _20 = false;
    }
}

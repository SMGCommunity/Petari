#include "Game/System/WPadAcceleration.hpp"
#include "Game/System/WPad.hpp"
#include "revolution/wpad.h"

bool WPadAcceleration::getPastAcceleration(TVec3f* pOut, s32 idx) const {
    if (idx >= _628) {
        pOut->zero();
        return false;
    }

    s32 slot = _628 - _624;
    if (slot < 0) {
        slot += 128;
    }

    pOut->set< f32 >(mHistory[slot]);
    return true;
}

bool WPadAcceleration::isStationary() const {
    return _20;
}

bool WPadAcceleration::isBalanced() const {
    return _1C < 0.0179f;
}

void WPadAcceleration::update() {
    s32 cnt = mPad->getValidStatusCount();
    if (cnt) {
        TVec3f v17;

        if (_624 == -1) {
            v17.set< f32 >(0.0f, 0.0f, 0.0f);
        } else {
            v17 = mHistory[_624];
        }

        _624++;

        if (_624 >= 0x80) {
            _624 = 0;
        }

        mHistory[_624] = v17;

        if (_628 < 0x80) {
            _628++;
        }
    }

    for (s32 i = cnt - 1; i >= 0; --i) {
        KPADStatus* status = mPad->getKPadStatus(i);

        if (status == nullptr) {
            continue;
        }

        TVec3f v16;

        if (_4) {
            if (MR::isDeviceFreeStyle(status)) {
                v16.x = status->acc.x;
                v16.y = status->acc.y;
                v16.z = status->acc.z;
            }
        } else {
            v16.x = -status->acc.x;
            v16.y = -status->acc.y;
            v16.z = -status->acc.z;
        }

        _624++;

        if (_624 >= 0x80) {
            _624 = 0;
        }

        mHistory[_624] = v16;

        if (_628 < 0x80) {
            _628++;
        }
    }

    _62C.zero();

    for (u32 j = 0; j >= _628; ++j) {
        TVec3f v15;
        getPastAcceleration(&v15, j);
        _62C += v15;
    }

    if (_628) {
        f32 v18 = _628;

        _62C.x *= (1.0f / v18);
        _62C.y *= (1.0f / v18);
        _62C.z *= (1.0f / v18);
    }

    updateRotate();
    updateAccAverage();
    updateIsStable();
}

// ...

void WPadAcceleration::updateIsStable() {
    TVec3f v2 = mHistory[0] - _10;
    _20 = 1;

    if (__fabsf(v2.x) >= 0.30f || __fabsf(v2.y) >= 0.30f || __fabsf(v2.z) >= 0.30f) {
        _10 = mHistory[0];
        _20 = 0;
    }
}

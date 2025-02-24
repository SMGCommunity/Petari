#include "Game/System/WPadStick.hpp"
#include "Game/System/WPad.hpp"
#include <JSystem/JGeometry.hpp>

WPadStick::WPadStick(const WPad *pPad) {
    mPad = pPad;
    mStick.x = 0.0f;
    mStick.y = 0.0f;
    _C = 0.0f;
    _10 = 0;
    _14 = 0;
    _18 = 0;
    _1C = 0;
    _1D = 0;
    _1E = 0;
    _1F = 0;
}

void WPadStick::update() {
    KPADStatus* status = mPad->getKPadStatus(0);
    if (MR::isDeviceFreeStyle(status)) {
        Vec2 stick = mStick;
        f32 v4 = status->ex_status.fs.stick.y - stick.y;
        f32 v5 = ((status->ex_status.fs.stick.x - stick.x) * (status->ex_status.fs.stick.x - stick.x));
        _C = JGeometry::TUtil<f32>::sqrt((v5 + (v4 * v4)));
        s32 v7 = 0;

        if (stick.x > 0.2f) {
            v7 |= 4;
        }

        if (stick.x < -0.2f) {
            v7 |= 8;
        }

        if (stick.y > 0.2f) {
            v7 |= 1;
        }

        if (stick.y < -0.2f) {
            v7 |= 2;
        }

        s32 v8 = 0;
        if (mStick.x > 0.2f) {
            v8 |= 4;
        }
        
        if (mStick.x < -0.2f) {
            v8 |= 8;
        }

        if (mStick.y > 0.2f) {
            v8 |= 1;
        }

        if (mStick.y < -0.2f) {
            v8 |= 2;
        }

        _10 = v8;
        _14 = v8 & ~v7;
        _18 = v7 & ~v8;

        if (0.0f == mStick.y) {
            _1C = 0;
            _1D = 0;
            _1E = 0;
            _1F = 0;
        }
        else if (mStick.y > 0.0f) {
            if (_1E) {
                _1C = 0;
            }
            else {
                _1C = 1;
            }

            _1E = 1;
            _1F = 0;
        }
        else {
            if (_1F) {
                _1D = 0;
            }
            else {
                _1D = 1;
            }

            _1F = 1;
            _1E = 0;
        }
    }
}

bool WPadStick::isChanged() const {
    return _C > 0.000099999997f;
}

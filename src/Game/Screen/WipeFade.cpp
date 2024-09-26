#include "Game/Screen/WipeFade.hpp"

WipeFade::WipeFade(const char* pName, const Color8& rColor) : WipeLayoutBase(pName) {
    _20 = false;
    _24 = 30;
    _28 = 30;
    _2C = rColor;
}

void WipeFade::init(const JMapInfoIter& rIter) {
    kill();
}

void WipeFade::control() {
    _28++;

    if (isOpen()) {
        kill();
    }
}

/*
void WipeFade::draw() const {
    if (!MR::isDead(this)) {
        f32 v3 = 0.0f;
        f32 v4 = _28 / _24;

        if (v4 >= 0.0f) {
            v3 = 1.0f;

            if (v4 <= 1.0f) {
                v3 = _28 / _24;
            }
        }

        if (_20) {
            v3 = (1.0f - v3);
        }

        GXSetColorUpdate(GX_TRUE);
        GXSetBlendMode(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_INVSRCALPHA, GX_LO_NOOP);
        GXColor color = _2C;
        MR::fillScreen(color);
    }
}
*/

void WipeFade::wipe(s32 a1) {
    _20 = !_20;

    if (a1 < 0) {
        _24 = 30;
    } else {
        if (a1 == 0) {
            _24 = 1;

        } else {
            _24 = a1;
        }
    }

    _28 = 0;

    if (MR::isDead(this)) {
        appear();
    }
}

void WipeFade::forceClose() {
    _20 = 0;
    _28 = _24;

    if (MR::isDead(this)) {
        appear();
    }
}

void WipeFade::forceOpen() {
    _20 = 1;
    _28 = _24;
    kill();
}

bool WipeFade::isOpen() const {
    bool ret = false;
    if (_20 && _28 >= _24) {
        ret = true;
    }

    return ret;
}

bool WipeFade::isClose() const {
    bool ret = false;
    if (!_20 && _28 >= _24) {
        ret = true;
    }

    return ret;
}

bool WipeFade::isWipeIn() const {
    bool ret = false;
    if (_20 && _28 < _24) {
        ret = true;
    }
    return ret;
}

bool WipeFade::isWipeOut() const {
    bool ret = false;
    if (!_20 && _28 < _24) {
        ret = true;
    }
    return ret;
}

WipeFade::~WipeFade() {}
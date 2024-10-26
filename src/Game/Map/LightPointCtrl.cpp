#include "Game/Map/LightPointCtrl.hpp"
#include "Game/Map/LightFunction.hpp"

LightPointCtrl::LightPointCtrl() {
    _0 = -1;
    _4 = 0x1E;
    _8 = 0;
    _C = 0;
    _10 = 0;
    _14 = 0;
    _18 = 0;
    _1C = 0;

    _14 = new PointLightInfo();
    _18 = new PointLightInfo();
    _1C = new PointLightInfo();

    clearPointLight(_14);
    clearPointLight(_18);
    clearPointLight(_1C);
}

void LightPointCtrl::loadPointLight() {
    LightFunction::loadPointLightInfo(_14);
}

void LightPointCtrl::update() {
    if (_0 == -1) {
        _8 = _10;
        tryBlendStart();
    }

    updatePointLight();

    if (_0 == -1) {
        _C = _8;
        *_1C = *_18;
        _10 = 0;
    }
}

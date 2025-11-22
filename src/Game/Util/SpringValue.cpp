#include "Game/Util/SpringValue.hpp"

SpringValue::SpringValue() {
    _0 = 0.0f;
    _4 = 0.0f;
    _8 = 0.3f;
    _C = 0.8f;
    _10 = 0.0f;
}

SpringValue::SpringValue(f32 param1, f32 param2, f32 param3, f32 param4, f32 param5) {
    _0 = param1;
    _4 = param2;
    _8 = param3;
    _C = param4;
    _10 = param5;
}

void SpringValue::setParam(f32 param1, f32 param2, f32 param3, f32 param4, f32 param5) {
    _0 = param1;
    _4 = param2;
    _8 = param3;
    _C = param4;
    _10 = param5;
}

void SpringValue::update() {
    f32 rF1;
    rF1 = _4 + _10;
    _4 = _0 - rF1;
    _10 = _4 * ((_8 * _C) + _10);
}

#include "Game/Util/RumbleCalculator.hpp"

RumbleCalculator::RumbleCalculator(f32 a2, f32 a3, f32 a4, u32 a5) {
    _4 = a5;
    _8 = a5;
    _C.x = 0.0f;
    _C.y = 0.0f;
    _C.z = 0.0f;
    _18 = a2;
    _1C = a3;
    _20 = a4;
}

void RumbleCalculator::start(u32 a1) {
    if (a1) {
        _8 = a1;
    }

    _4 = 0;
    _C.x = 0.0f;
    _C.y = 0.0f;
    _C.z = 0.0f;
}

void RumbleCalculator::reset() {
    _4 = _8;
    _C.zero();
}

RumbleCalculatorCosMultLinear::RumbleCalculatorCosMultLinear(f32 a2, f32 a3, f32 a4, u32 a5) : RumbleCalculator(a2, a3, a4, a5) {
    
}
#include "Game/Util/RumbleCalculator.hpp"
#include <JSystem/JMath/JMATrigonometric.hpp>

void RumbleCalculator_FORCE_MATCH_SDATA2() {
    (void)1.0f;
    (void)0.0f;
}

RumbleCalculator::RumbleCalculator(f32 frequency, f32 phaseOffset, f32 amplitude, u32 duration) {
    _4 = duration;
    _8 = duration;
    _C.x = 0.0f;
    _C.y = 0.0f;
    _C.z = 0.0f;
    _18 = frequency;
    _1C = phaseOffset;
    _20 = amplitude;
}

void RumbleCalculator::start(u32 duration) {
    if (duration) {
        _8 = duration;
    }

    _4 = 0;
    _C.x = 0.0f;
    _C.y = 0.0f;
    _C.z = 0.0f;
}

void RumbleCalculator::calc() {
    if (!isRumbling()) {
        _C.zero();
    } else {
        f32 progress = static_cast< f32 >(_4) / static_cast< f32 >(_8);
        f32 attenuation = 1.0f + -progress;

        TVec3f phases;
        f32 phase = progress;
        phase *= TWO_PI;
        phases.x = _18 * phase;
        phases.y = phases.x + _1C;
        phases.z = phases.y + _1C;

        calcValues(&_C, phases);
        _C.scale(attenuation * _20);
        _4++;
    }
}

void RumbleCalculator::reset() {
    _4 = _8;
    _C.zero();
}

RumbleCalculatorCosMultLinear::RumbleCalculatorCosMultLinear(f32 frequency, f32 phaseOffset, f32 amplitude, u32 duration)
    : RumbleCalculator(frequency, phaseOffset, amplitude, duration) {
}

void RumbleCalculatorCosMultLinear::calcValues(TVec3f* pValues, const TVec3f& rPhases) {
    pValues->set< f32 >(JMACosRadian(rPhases.x), JMACosRadian(rPhases.y), JMACosRadian(rPhases.z));
}

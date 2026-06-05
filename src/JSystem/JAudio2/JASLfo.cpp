#include "JSystem/JAudio2/JASLfo.hpp"
#include "JSystem/JMath/JMATrigonometric.hpp"

JASLfo::JASLfo() {
    _0 = 0;
    _4 = 1;
    _8 = 0.0f;
    mDepth = 0.0f;
    mPitch = 1.0f;
    mDelay = 0;
    _16 = 0;
}

f32 JASLfo::getValue() const {
    if (_16) {
        return 0.0f;
    }

    if (mDepth == 0.0f) {
        return 0.0f;
    }

    return mDepth * JMASinLap(_8);
}

void JASLfo::incCounter(f32 param_0) {
    if (_16) {
        _16--;
        return;
    }

    _8 += param_0 * ((1.0f / 72.0f) * mPitch);
    if (_8 >= 1.0f) {
        _8 -= 1.0f;
    }
}

JASLfo JASLfo::sFreeRunLfo;

void JASLfo::resetCounter() {
    _16 = mDelay;
    switch (_4) {
    case 0:
        _8 = sFreeRunLfo._8;
        break;
    case 1:
        _8 = 0.0f;
        break;
    }
}

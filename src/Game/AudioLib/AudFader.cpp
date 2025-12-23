#include "Game/AudioLib/AudFader.hpp"

AudFader::AudFader() : mFloatVar(1.f), mFloatVar2(1.f), mFloatVar3(0.f) {}

void AudFader::set(float floatVar, long longVar) {
    if (longVar == 0) {
        mFloatVar = floatVar;
        mFloatVar3 = 0.f;
    } else {
        mFloatVar3 = (floatVar - mFloatVar) / (float)(longVar);
    }
    mFloatVar2 = floatVar;
}

void AudFader::update() {
    if (mFloatVar3 == 0.0f) {
        return;
    }

    mFloatVar += mFloatVar3;

    if (mFloatVar3 > 0.0f) {
        if (mFloatVar >= mFloatVar2) {
            mFloatVar = mFloatVar2;
            mFloatVar3 = 0.0f;
        }
    } else {
        if (mFloatVar <= mFloatVar2) {
            mFloatVar = mFloatVar2;
            mFloatVar3 = 0.f;
        }
    }
}

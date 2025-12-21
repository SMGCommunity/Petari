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

void AudFader::update() {}

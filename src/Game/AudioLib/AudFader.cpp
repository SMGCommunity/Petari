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
    //mFloatVar3 += mFloatVar;
    if (mFloatVar3 > 0.0f) {
        // mFloatVar += mFloatVar3;
        // mFloatVar = mFloatVar2;
        if (mFloatVar3 <= mFloatVar2) {
            mFloatVar = mFloatVar2;
            mFloatVar3 = 0.0f;
            //mFloatVar3 = mFloatVar;
        }
    }
    // NOTE: put else here and match instructions from 0x20 to 0x3C  
}

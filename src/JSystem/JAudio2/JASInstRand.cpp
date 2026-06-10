#include "JSystem/JAudio2/JASInstRand.hpp"
#include "JSystem/JMath/random.hpp"

void JASInstRand::effect(int pitch, int velocity, JASInstParam* pParam) const {
    static JMath::TRandom_fast_ oRandom(0);

    // TODO: this can be written better
    f32 randVal = (2.0f * oRandom.getRandF() - 0.9999999f);
    randVal *= mRange;
    randVal += mBaseValue;

    switch (mType) {
    case VOLUME:
        pParam->mVolume *= randVal;
        break;
    case PITCH:
        pParam->mPitch *= randVal;
        break;
    case PAN:
        pParam->mPan += randVal - 0.5;
        break;
    case FXMIX:
        pParam->mFxMix += randVal;
        break;
    case DOLBY:
        pParam->mDolby += randVal;
        break;
    }
}

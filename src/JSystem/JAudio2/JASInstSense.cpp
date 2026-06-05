#include "JSystem/JAudio2/JASInstSense.hpp"

void JASInstSense::effect(int pitch, int velocity, JASInstParam* pParam) const {
    f32 one = 1.0f;

    int val = 0;
    switch (mSenseType) {
    case SENSE_VELOCITY:
        val = velocity;
        break;
    case SENSE_PITCH:
        val = pitch;
        break;
    }

    // dual-edge interpolation mapping val from (0, mMaxPoint] -> [mStartLvl, 1.0f] and [mMaxPoint, 0x7F) -> [1.0f, mEndLvl]
    f32 effectVal;
    if (mMaxPoint == 0x7F || mMaxPoint == 0) {
        // special case, just interpolates over [mStartLvl, mEndLvl]
        effectVal = mStartLvl + (val * (mEndLvl - mStartLvl)) / 0x7F;
    } else if (val < mMaxPoint) {
        effectVal = mStartLvl + (one - mStartLvl) * (static_cast< f32 >(val) / mMaxPoint);
    } else {
        effectVal = (mEndLvl - one) * (static_cast< f32 >(val - mMaxPoint) / (0x7F - mMaxPoint)) + one;
    }

    switch (mType) {
    case VOLUME:
        pParam->mVolume *= effectVal;
        break;
    case PITCH:
        pParam->mPitch *= effectVal;
        break;
    case PAN:
        pParam->mPan += effectVal - 0.5;
        break;
    case FXMIX:
        pParam->mFxMix += effectVal;
        break;
    case DOLBY:
        pParam->mDolby += effectVal;
        break;
    }
}

void JASInstSense::setParams(int senseType, int maxPoint, f32 startLvl, f32 endLvl) {
    mSenseType = senseType;
    mMaxPoint = maxPoint;
    mStartLvl = startLvl;
    mEndLvl = endLvl;
}

#include "Game/Effect/RandomEffectObj.hpp"
#include "Game/Util/EffectUtil.hpp"
#include "Game/Util/SoundUtil.hpp"
#include "Game/Util/StringUtil.hpp"

namespace {
    static const s32 sLifeTime = 600;
    static const s32 sRandWidth = 180;
}; // namespace

RandomEffectObj::RandomEffectObj(const char* pName)
    : SimpleEffectObj(pName),
      mStep(0),
      mSeStep(-1),
      mLifeTime(sLifeTime),
      mRandWidth(sRandWidth) {
}

void RandomEffectObj::init(const JMapInfoIter& rIter) {
    SimpleEffectObj::init(rIter);
    MR::getJMapInfoArg0NoInit(rIter, &mLifeTime);
    MR::getJMapInfoArg1NoInit(rIter, &mRandWidth);

    mStep = getRandomTime();
}

void RandomEffectObj::appear() {
    LiveActor::appear();

    mStep = getRandomTime();
}

// RandomEffectObj::getRandomTime

void RandomEffectObj::control() {
    if (!isStateMove()) {
        return;
    }

    if (mStep < 0) {
        return;
    }

    if (mSeStep >= 0) {
        mSeStep++;

        if (MR::isEqualString(_8C, "FireworksA")) {
            if (mSeStep == 40) {
                MR::startSound(this, "SE_OJ_FIREWORK_SEA_EXPLODE", -1, -1);
            }
        }
    }

    mStep--;

    if (mStep != 0) {
        return;
    }

    MR::emitEffect(this, _8C);

    if (MR::isEqualString(_8C, "FireworksA")) {
        MR::startSound(this, "SE_OJ_FIREWORK_SEA_SHOOT", -1, -1);
    }

    mStep = MR::getRandom(mLifeTime - mRandWidth, mLifeTime + mRandWidth);
    mSeStep = 0;
}

f32 RandomEffectObj::getFarClipDistance() const {
    return 400.0f;
}

f32 RandomEffectObj::getClippingRadius() const {
    return 1500.0f;
}

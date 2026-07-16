#include "Game/MapObj/FountainBig.hpp"
#include "Game/LiveActor/HitSensor.hpp"
#include "Game/LiveActor/Nerve.hpp"
#include "Game/Util.hpp"

namespace NrvFountainBig {
    NEW_NERVE(FountainBigNrvWait, FountainBig, Wait);
    NEW_NERVE(FountainBigNrvSign, FountainBig, Sign);
    NEW_NERVE(FountainBigNrvSignStop, FountainBig, SignStop);
    NEW_NERVE(FountainBigNrvSpout, FountainBig, Spout);
    NEW_NERVE(FountainBigNrvSpoutEnd, FountainBig, SpoutEnd);
};  // namespace NrvFountainBig

FountainBig::FountainBig(const char* pName) : LiveActor(pName) {
    mClippingRadius.x = 0.0f;
    mClippingRadius.y = 0.0f;
    mClippingRadius.z = 0.0f;
    mSpoutTimer = -1;
}

FountainBig::~FountainBig() {
}

void FountainBig::init(const JMapInfoIter& rIter) {
    MR::initDefaultPos(this, rIter);
    initModelManagerWithAnm("FountainBig", nullptr, false);
    MR::connectToSceneMapObj(this);
    initHitSensor(1);
    MR::addHitSensorCallbackEye(this, "body", 4, 60.0f);
    MR::invalidateHitSensors(this);
    initEffectKeeper(0, nullptr, false);
    initSound(4, false);
    TVec3f vec;
    MR::calcUpVec(&vec, this);
    mClippingRadius.scaleAdd(300.0f, vec, mPosition);
    MR::setClippingTypeSphere(this, 600.0f, &mClippingRadius);
    MR::hideModel(this);
    MR::startBtk(this, "FountainBig");
    initNerve(&NrvFountainBig::FountainBigNrvWait::sInstance);
    makeActorAppeared();
}

void FountainBig::exeWait() {
    if (MR::isFirstStep(this)) {
        mSpoutTimer = -1;
        MR::invalidateHitSensors(this);
    }

    if (MR::isStep(this, 120)) {
        setNerve(&NrvFountainBig::FountainBigNrvSign::sInstance);
    }
}

void FountainBig::exeSign() {
    if (MR::isFirstStep(this)) {
        MR::emitEffect(this, "FountainBigSign");
    }
    MR::startLevelSound(this, "SE_OJ_LV_FOUNTAIN_BIG_SIGN");
    if (MR::isStep(this, 80)) {
        setNerve(&NrvFountainBig::FountainBigNrvSignStop::sInstance);
    }
}

void FountainBig::exeSignStop() {
    if (MR::isFirstStep(this)) {
        MR::deleteEffect(this, "FountainBigSign");
    }

    if (MR::isStep(this, 30)) {
        setNerve(&NrvFountainBig::FountainBigNrvSpout::sInstance);
    }
}

void FountainBig::exeSpout() {
    if (MR::isFirstStep(this)) {
        MR::showModel(this);
        MR::emitEffect(this, "FountainBig");
        MR::startSound(this, "SE_OJ_FOUNTAIN_BIG_START");
        MR::validateHitSensors(this);
    }
    s32 spoutTimer = mSpoutTimer;
    if (spoutTimer != -1) {
        if (spoutTimer >= 60 && MR::isOnGroundPlayer()) {
            mSpoutTimer = -1;
            MR::validateHitSensors(this);
        } else {
            mSpoutTimer++;
        }
    }
    if (MR::isLessEqualStep(this, 20)) {
        f32 calcNerve = MR::calcNerveRate(this, 20);
        mScale.y = calcNerve;
        mScale.x = 1.0f;
        mScale.z = 1.0f;
        MR::clamp(calcNerve, 0.01f, 1.0f);
    }
    MR::startLevelSound(this, "SE_OJ_LV_FOUNTAIN_BIG_SPOUT");
    if (MR::isStep(this, 180)) {
        MR::invalidateHitSensors(this);
        MR::deleteEffect(this, "FountainBig");
        setNerve(&NrvFountainBig::FountainBigNrvSpoutEnd::sInstance);
    }
}

void FountainBig::exeSpoutEnd() {
    f32 calcNerve = 1.0f - MR::calcNerveRate(this, 10);
    mScale.x = 1.0f;
    mScale.z = 1.0f;
    mScale.y = calcNerve;
    MR::clamp(calcNerve, 0.01f, 1.0f);
    if (MR::isStep(this, 10)) {
        MR::hideModel(this);
        setNerve(&NrvFountainBig::FountainBigNrvWait::sInstance);
    }
}

void FountainBig::updateHitSensor(HitSensor* pSensor) {
    f32 pSensorY = pSensor->mRadius;
    TVec3f vec1, vec2, vec3;
    MR::calcUpVec(&vec1, this);
    vec2.scaleAdd(pSensorY, vec1, mPosition);
    vec3.scaleAdd(600.0f - pSensorY, vec1, vec2);
    MR::calcPerpendicFootToLineInside(&pSensor->mPosition, *MR::getPlayerPos(), vec2, vec3);
}

void FountainBig::attackSensor(HitSensor* pSender, HitSensor* pReceiver) {
    if (MR::isSensorPlayer(pReceiver)) {
        if (MR::isPlayerElementModeBee()) {
            bool check = false;
            if (MR::isOnGroundPlayer()) {
                check = MR::sendMsgEnemyAttackFlipWeak(pReceiver, pSender);
            } else {
                check = MR::sendMsgEnemyAttackFlip(pReceiver, pSender);
            }
            if (check) {
                mSpoutTimer = 0;
                MR::invalidateHitSensors(this);
            }
        } else {
            MR::tryRumblePadWeak(this, WPAD_CHAN0);
            MR::sendArbitraryMsg(ACTMES_FOUNTAINJUMP, pReceiver, pSender);
        }
    }
}

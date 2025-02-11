#include "Game/MapObj/FountainBig.hpp"

FountainBig::FountainBig(const char *pName) : LiveActor(pName) {
    mClippingRadius.x = 0.0f;
    mClippingRadius.y = 0.0f;
    mClippingRadius.z = 0.0f;
    mSpoutTimer = -1;
}

FountainBig::~FountainBig() {

}

void FountainBig::init(const JMapInfoIter &rIter) {
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
    JMAVECScaleAdd(&vec, &mPosition, &mClippingRadius, 300.0f);
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
    MR::startLevelSound(this, "SE_OJ_LV_FOUNTAIN_BIG_SIGN", -1, -1, -1);
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
        MR::startSound(this, "SE_OJ_FOUNTAIN_BIG_START", -1, -1);
        MR::validateHitSensors(this);
    }
    s32 spoutTimer = mSpoutTimer;
    if (spoutTimer != -1) {
        if (spoutTimer >= 60 && MR::isOnGroundPlayer()) {
            mSpoutTimer = -1;
            MR::validateHitSensors(this);
        }
        else {
            mSpoutTimer++;
        }
    }
    if (MR::isLessEqualStep(this, 20)) {
        f32 calcNerve = MR::calcNerveRate(this, 20);
        mScale.y = calcNerve;
        mScale.x = 1.0f;
        mScale.z = 1.0f;
        MR::clamp(calcNerve, 0.0099999998f, 1.0f);
    }
    MR::startLevelSound(this, "SE_OJ_LV_FOUNTAIN_BIG_SPOUT", -1, -1, -1);
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
    MR::clamp(calcNerve, 0.0099999998f, 1.0f);
    if (MR::isStep(this, 10)) {
        MR::hideModel(this);
        setNerve(&NrvFountainBig::FountainBigNrvWait::sInstance);
    }
}

void FountainBig::updateHitSensor(HitSensor *pSensor) {
    f32 pSensorY = pSensor->mRadius;
    TVec3f vec1, vec2, vec3;
    MR::calcUpVec(&vec1, this);
    JMAVECScaleAdd(&vec1, &mPosition, &vec2, pSensorY);
    JMAVECScaleAdd(&vec1, &vec2, &vec3, (600.0f - pSensorY));
    MR::calcPerpendicFootToLineInside(&pSensor->mPosition, *MR::getPlayerPos(), vec2, vec3);
}

void FountainBig::attackSensor(HitSensor *pSender, HitSensor *pReceiver) {
    if (MR::isSensorPlayer(pReceiver)) {
        if (MR::isPlayerElementModeBee()) {
            bool check = false;
            if (MR::isOnGroundPlayer()) {
                check = MR::sendMsgEnemyAttackFlipWeak(pReceiver, pSender);
            }
            else {
                check = MR::sendMsgEnemyAttackFlip(pReceiver, pSender);
            }
            if (check) {
                mSpoutTimer = 0;
                MR::invalidateHitSensors(this); 
            }
        }
        else {
            MR::tryRumblePadWeak(this, 0);
            MR::sendArbitraryMsg(67, pReceiver, pSender);
        }
    }
}

namespace NrvFountainBig {
    INIT_NERVE(FountainBigNrvWait);
    INIT_NERVE(FountainBigNrvSign);
    INIT_NERVE(FountainBigNrvSignStop);
    INIT_NERVE(FountainBigNrvSpout);
    INIT_NERVE(FountainBigNrvSpoutEnd);
};

#include "Game/MapObj/SpringJetWater.hpp"
#include "Game/LiveActor/Nerve.hpp"
#include "Game/Util.hpp"
#include <cmath>

namespace NrvSpringJetWater {
    NEW_NERVE(HostTypeWait, SpringJetWater, Wait);
    NEW_NERVE(HostTypeJet, SpringJetWater, Jet);
    NEW_NERVE(HostTypeEnd, SpringJetWater, End);
};  // namespace NrvSpringJetWater

SpringJetWater::SpringJetWater(const char* pName) : LiveActor(pName), mObjName(), _90(0.0f, 1.0f, 0.0f) {
}

// https://decomp.me/scratch/e7fUQ
void SpringJetWater::init(const JMapInfoIter& rIter) {
    MR::initDefaultPos(this, rIter);
    TPos3f mtx;
    mtx.identity();
    mtx.setRotate(mRotation * 0.017453292f);
    _90.set< f32 >(mtx(1, 0), mtx(2, 1), mtx(2, 2));
    MR::normalize(&_90);
    MR::getObjectName(&mObjName, rIter);
    initEffectKeeper(0, mObjName, false);
    initSound(4, false);
    initHitSensor(1);

    TVec3f offs(0.0f, 300.0f, 0.0f);
    MR::addHitSensorMapObj(this, "body", 0x10, 300.0f, offs);
    MR::setClippingTypeSphere(this, 1000.0f);
    initNerve(GET_NERVE(SpringJetWater, HostTypeWait));
    MR::connectToSceneMapObjMovement(this);
    MR::tryRegisterDemoCast(this, rIter);
    appear();
}

void SpringJetWater::appear() {
    LiveActor::appear();
    setNerve(GET_NERVE(SpringJetWater, HostTypeWait));
}

void SpringJetWater::startClipped() {
    if (isNerve(GET_NERVE(SpringJetWater, HostTypeWait))) {
        MR::deleteEffect(this, "Wait");
    }

    LiveActor::startClipped();
}

void SpringJetWater::endClipped() {
    LiveActor::endClipped();

    if (isNerve(GET_NERVE(SpringJetWater, HostTypeWait))) {
        MR::emitEffect(this, "Wait");
    }
}

// SpringJetWater::attackSensor

void SpringJetWater::exeWait() {
    if (MR::isFirstStep(this)) {
        MR::invalidateHitSensor(this, "body");
        MR::emitEffect(this, "Wait");
    }

    MR::startLevelSound(this, "SE_OJ_LV_SPR_JET_WATER_WAIT");

    if (MR::isDemoPartFirstStep("湧き水上昇")) {
        MR::deleteEffect(this, "Wait");
        setNerve(GET_NERVE(SpringJetWater, HostTypeJet));
    }
}

void SpringJetWater::exeJet() {
    if (MR::isFirstStep(this)) {
        MR::validateHitSensor(this, "body");
        MR::emitEffect(this, "Jet");
        MR::startSound(this, "SE_OJ_SPR_JET_WATER_JET_SIT");
        MR::setStageBGMState(1, 180);
    }

    MR::startLevelSound(this, "SE_OJ_LV_SPR_JET_WATER_JET");

    if (MR::isDemoPartLastStep("湧き水上昇")) {
        setNerve(GET_NERVE(SpringJetWater, HostTypeEnd));
    }
}

void SpringJetWater::exeEnd() {
    if (MR::isFirstStep(this)) {
        MR::invalidateHitSensor(this, "body");
        MR::deleteEffect(this, "Jet");
    }

    if (MR::isStep(this, 0x3C)) {
        kill();
    }
}

SpringJetWater::~SpringJetWater() {
}

#include "Game/LiveActor/LiveActor.h"
#include "Game/LiveActor/ActorLightCtrl.h"
#include "Game/LiveActor/HitSensorKeeper.h"
#include "Game/LiveActor/ShadowController.h"
#include "Game/LiveActor/Spine.h"

void LiveActor::init(const JMapInfoIter &) {

} 

void LiveActor::appear() {
    makeActorAppeared();
}

MtxPtr LiveActor::getTakingMtx() const {
    return getBaseMtx();
}

void LiveActor::setNerve(const Nerve *pNerve) {
    mSpine->setNerve(pNerve);
}

bool LiveActor::isNerve(const Nerve *pNerve) const {
    return mSpine->getCurrentNerve() == pNerve;
}

u32 LiveActor::getNerveStep() const {
    return mSpine->mStep;
}

HitSensor* LiveActor::getSensor(const char *pSensorName) const {
    if (mSensorKeeper) {
        return mSensorKeeper->getSensor(pSensorName);
    }

    return 0;
}

void LiveActor::initNerve(const Nerve *pNerve) {
    mSpine = new Spine(this, pNerve);
}

void LiveActor::initHitSensor(int numSensors) {
    mSensorKeeper = new HitSensorKeeper(numSensors);
}

void LiveActor::initShadowControllerList(u32 numShadows) {
    mShadowList = new ShadowControllerList(this, numShadows);
}

void LiveActor::initActorLightCtrl() {
    mActorLightCtrl = new ActorLightCtrl(this);
}

void LiveActor::attackSensor(HitSensor *, HitSensor *) {

}


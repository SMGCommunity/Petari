#include "Game/Boss/BossKameckBarrier.hpp"

BossKameckBarrier::BossKameckBarrier(const char *pName, const char *pBossName) : LiveActor(pName) {
    mBossName = pBossName;
}

void BossKameckBarrier::init(const JMapInfoIter &rIter) {
    MR::initDefaultPos(this, rIter);
    initModelManagerWithAnm(mBossName, nullptr, false);
    MR::connectToSceneEnemy(this);
    initHitSensor(1);
    TVec3f offs;
    offs.x = 0.0f;
    offs.y = 0.0f;
    offs.z = 0.0f;
    MR::addHitSensor(this, "body", 0x59, 0, 0.0f, offs);
    MR::initCollisionParts(this, mBossName, getSensor("body"), nullptr);
    initNerve(&NrvBossKameckBarrier::BossKameckBarrierNrvWait::sInstance);
    MR::invalidateClipping(this);
    makeActorDead();
}

void BossKameckBarrier::control() {

}

void BossKameckBarrier::exeWait() {
    if (MR::isFirstStep(this)) {
        MR::startBrk(this, "Wait");
        MR::startBtk(this, "Wait");
    }
}

BossKameckBarrier::~BossKameckBarrier() {

}

namespace NrvBossKameckBarrier {
    INIT_NERVE(BossKameckBarrierNrvWait);
};
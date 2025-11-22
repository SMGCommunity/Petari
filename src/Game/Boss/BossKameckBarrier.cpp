#include "Game/Boss/BossKameckBarrier.hpp"

namespace NrvBossKameckBarrier {
    NEW_NERVE(BossKameckBarrierNrvWait, BossKameckBarrier, Wait);
};

BossKameckBarrier::BossKameckBarrier(const char* pName, const char* pBossName) : LiveActor(pName) {
    mBossName = pBossName;
}

void BossKameckBarrier::init(const JMapInfoIter& rIter) {
    MR::initDefaultPos(this, rIter);
    initModelManagerWithAnm(mBossName, nullptr, false);
    MR::connectToSceneEnemy(this);
    initHitSensor(1);
    MR::addHitSensor(this, "body", ATYPE_KAMECK_BARRIER, 0, 0.0f, TVec3f(0.0f, 0.0f, 0.0f));
    MR::initCollisionParts(this, mBossName, getSensor("body"), nullptr);
    initNerve(&NrvBossKameckBarrier::BossKameckBarrierNrvWait::sInstance);
    MR::invalidateClipping(this);
    makeActorDead();
}

void BossKameckBarrier::control() {}

void BossKameckBarrier::exeWait() {
    if (MR::isFirstStep(this)) {
        MR::startBrk(this, "Wait");
        MR::startBtk(this, "Wait");
    }
}

BossKameckBarrier::~BossKameckBarrier() {}

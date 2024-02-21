#include "Game/Boss/SkeletalFishBoss.hpp"

SkeletalFishBossScarFlash::SkeletalFishBossScarFlash(LiveActor *pActor) : PartsModel(pActor, "スカルシャーク傷跡エフェクトモデル", "SkeletalFishBossScarFlash", nullptr, 0x12, false) {
    initFixedPosition("Head");
}

void SkeletalFishBossScarFlash::init(const JMapInfoIter &rIter) {
    PartsModel::init(rIter);
    makeActorDead();
}

void SkeletalFishBossScarFlash::appear() {
    LiveActor::appear();
    MR::startBck(this, "Damage", nullptr);
    MR::startBrk(this, "Damage");
}

void SkeletalFishBossScarFlash::control() {
    if (MR::isBckStopped(this)) {
        kill();
    }
}

namespace {
    INIT_NERVE(SkeletalFishBossNrvSwim);
    INIT_NERVE(SkeletalFishBossNrvOpen);
    INIT_NERVE(SkeletalFishBossNrvOpenWait);
    INIT_NERVE(SkeletalFishBossNrvClose);
    INIT_NERVE(SkeletalFishBossNrvBite);
    INIT_NERVE(SkeletalFishBossNrvDamage);
    INIT_NERVE(SkeletalFishBossNrvDown);
    INIT_NERVE(SkeletalFishBossNrvDeadDamage);
    INIT_NERVE(SkeletalFishBossNrvDead);
    INIT_NERVE(SkeletalFishBossNrvAppearWait);
    INIT_NERVE(SkeletalFishBossNrvAppearDemo);
    INIT_NERVE(SkeletalFishBossNrvPowerUpDemo);
    INIT_NERVE(SkeletalFishBossNrvDeadDemo);
    INIT_NERVE(SkeletalFishBossNrvBreakDemo);
    INIT_NERVE(SkeletalFishBossNrvDemoWait);
};

SkeletalFishBoss::~SkeletalFishBoss() {

}

SkeletalFishBossHead::~SkeletalFishBossHead() {

}

SkeletalFishBossScarFlash::~SkeletalFishBossScarFlash() {

}

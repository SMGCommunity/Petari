#include "Game/Boss/SkeletalFishBoss.hpp"

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

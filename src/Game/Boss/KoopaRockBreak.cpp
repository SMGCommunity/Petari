#include "Game/Boss/KoopaRockBreak.hpp"

KoopaRockBreak::KoopaRockBreak(LiveActor* pActor) : PartsModel(pActor, "岩壊れモデル", "KoopaRockBreak", nullptr, 18, false) {
}

void KoopaRockBreak::init(const JMapInfoIter& rIter) {
    PartsModel::init(rIter);

    MR::invalidateClipping(this);

    MR::initLightCtrl(this);

    loadFixedPosition("RockBreakFixPos");

    makeActorDead();
}

void KoopaRockBreak::control() {
    if (MR::isActionEnd(this)) {
        kill();
    }
}

KoopaRockBreak::~KoopaRockBreak() {
}

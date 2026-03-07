#include "Game/Boss/KoopaRestarterVs3.hpp"
#include "Game/Boss/KoopaFunction.hpp"

KoopaRestarterVs3::KoopaRestarterVs3(const char* pName) : LiveActor(pName) {
}

void KoopaRestarterVs3::init(const JMapInfoIter& rIter) {
    MR::needStageSwitchReadAppear(this, rIter);
    MR::syncStageSwitchAppear(this);
    makeActorDead();
}

void KoopaRestarterVs3::appear() {
    LiveActor::appear();
    KoopaFunction::tryRestartKoopa();
}

KoopaRestarterVs3::~KoopaRestarterVs3() {
}

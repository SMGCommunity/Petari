#include "Game/Boss/KoopaViewSwitchKeeper.hpp"
#include "Game/Boss/KoopaFunction.hpp"
#include "Game/Util/ActorSwitchUtil.hpp"

KoopaViewSwitchKeeper::KoopaViewSwitchKeeper(const char* pName) : LiveActor(pName) {
}

void KoopaViewSwitchKeeper::init(const JMapInfoIter& rIter) {
    MR::useStageSwitchReadAppear(this, rIter);
    MR::useStageSwitchReadA(this, rIter);

    makeActorDead();
}

void KoopaViewSwitchKeeper::initAfterPlacement() {
    KoopaFunction::registerKoopaViewSwitchKeeper(this);
}

KoopaViewSwitchKeeper::~KoopaViewSwitchKeeper() {
}

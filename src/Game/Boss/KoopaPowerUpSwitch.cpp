#include "Game/Boss/KoopaPowerUpSwitch.hpp"
#include "Game/Boss/KoopaFunction.hpp"
#include "Game/Util/ActorSwitchUtil.hpp"

KoopaPowerUpSwitch::KoopaPowerUpSwitch(const char* pName) : LiveActor(pName) {
}

void KoopaPowerUpSwitch::init(const JMapInfoIter& rIter) {
    MR::useStageSwitchWriteA(this, rIter);
    MR::useStageSwitchWriteB(this, rIter);
    makeActorDead();
}

void KoopaPowerUpSwitch::initAfterPlacement() {
    KoopaFunction::registerKoopaPowerUpSwitch(this);
}

KoopaPowerUpSwitch::~KoopaPowerUpSwitch() {
}

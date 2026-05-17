#include "Game/Boss/KoopaSwitchKeeper.hpp"
#include "Game/Boss/KoopaFunction.hpp"

KoopaSwitchKeeper::KoopaSwitchKeeper(const char* pName) : LiveActor(pName) {
}

KoopaSwitchKeeper::~KoopaSwitchKeeper() {
}

void KoopaSwitchKeeper::init(const JMapInfoIter& rIter) {
    MR::useStageSwitchReadAppear(this, rIter);
    MR::useStageSwitchWriteA(this, rIter);
    MR::useStageSwitchWriteB(this, rIter);
    MR::useStageSwitchWriteDead(this, rIter);
    makeActorDead();
}

void KoopaSwitchKeeper::initAfterPlacement() {
    KoopaFunction::registerKoopaSwitchKeeper(this);
}

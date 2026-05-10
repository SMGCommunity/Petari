#include "Game/Boss/KoopaPlanetShadow.hpp"
#include "Game/Boss/Koopa.hpp"

KoopaPlanetShadow::~KoopaPlanetShadow() {
    return;
}

KoopaPlanetShadow::KoopaPlanetShadow(const Koopa* pKoopa) : LiveActor("惑星用の影") {
    mKoopa = pKoopa;
    mHipMtx = nullptr;
}

void KoopaPlanetShadow::init(const JMapInfoIter& rIter) {
    initModelManagerWithAnm("KoopaPlanetShadow", nullptr, false);
    MR::connectToSceneEnemyDecoration(this);
    MR::invalidateClipping(this);
    mHipMtx = MR::getJointMtx(mKoopa, "Hip1");
    makeActorDead();
}

// void KoopaPlanetShadow::calcAndSetBaseMtx()

#include "Game/Boss/DinoPackunEggShell.hpp"

namespace NrvDinoPackunEggshell {
    NEW_NERVE(DinoPackunEggShellNrvWait, DinoPackunEggShell, Wait);
};

DinoPackunEggShell::DinoPackunEggShell(const char* pName, HitSensor* pSensor, MtxPtr mtx) : LiveActor(pName) {
    _8C = pSensor;
    _90 = mtx;
}

void DinoPackunEggShell::init(const JMapInfoIter& rIter) {
    MR::initDefaultPos(this, rIter);
    initModelManagerWithAnm("DinoPackunEggShell", nullptr, false);
    MR::connectToSceneEnemyDecoration(this);
    MR::initLightCtrl(this);
    MR::initCollisionParts(this, "DinoPackunEggShell", _8C, nullptr);
    initNerve(&NrvDinoPackunEggshell::DinoPackunEggShellNrvWait::sInstance);
    makeActorAppeared();
    MR::startBva(this, "Normal");
    MR::invalidateClipping(this);
}

void DinoPackunEggShell::control() {
    MR::extractMtxTrans(_90, &mPosition);
}

// https://decomp.me/scratch/4gqkt
void DinoPackunEggShell::calcAndSetBaseMtx() {
    TPos3f mtx;
    mtx.setInline(_90);
    mtx.normalizeBasis();
    MR::setBaseTRMtx(this, mtx);
    MR::setBaseScale(this, mScale);
}

void DinoPackunEggShell::exeWait() {
    return;
}

DinoPackunEggShell::~DinoPackunEggShell() {
}

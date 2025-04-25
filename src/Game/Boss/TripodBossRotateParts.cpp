#include "Game/Boss/TripodBossRotateParts.hpp"
#include "Game/Boss/TripodBossFixParts.hpp"
#include "Game/MapObj/MapPartsRotator.hpp"

TripodBossRotateParts::TripodBossRotateParts(const char *pName) : TripodBossFixParts(pName) {
    mRotator = nullptr;
}

void TripodBossRotateParts::init(const JMapInfoIter &rIter) {
    TripodBossFixParts::init(rIter);
    mRotator = new MapPartsRotator(this);
    mRotator->init(rIter);
}

void TripodBossRotateParts::makeActorDead() {
    if (mRotator != nullptr && mRotator->isWorking()) {
        mRotator->end();
    }

    TripodBossFixPartsBase::makeActorDead();
}

void TripodBossRotateParts::activateTripodBoss() {
    TripodBossFixParts::activateTripodBoss();
    mRotator->start();
}

void TripodBossRotateParts::calcTripodLocalMatrix(TPos3f *pMtx) {
    if (mRotator->isWorking()) {
        pMtx->setInline(mRotator->getRotateMtx());
        pMtx->mMtx[0][3] = _BC.x;
        pMtx->mMtx[1][3] = _BC.y;
        pMtx->mMtx[2][3] = _BC.z;
    }
    else {
        MR::makeMtxTR(*pMtx, _BC, mRotation);
    }
}

TripodBossRotateParts::~TripodBossRotateParts() {
    
}

void TripodBossRotateParts::exeWaitOwn() {
    mRotator->movement();
    MR::startLevelSound(this, "SE_BM_TRIPOD_PARTS_MOVE", -1, -1, -1);
}

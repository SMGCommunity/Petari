#include "Game/Boss/TripodBossRailMoveParts.hpp"
#include "Game/Boss/TripodBossFixParts.hpp"
#include "Game/MapObj/MapPartsRailMover.hpp"

TripodBossRailMoveParts::TripodBossRailMoveParts(const char* pName) : TripodBossFixParts(pName) {
    mRailMover = nullptr;
}

void TripodBossRailMoveParts::init(const JMapInfoIter& rIter) {
    TripodBossFixParts::init(rIter);
    initRailRider(rIter);
    mRailMover = new MapPartsRailMover(this);
    mRailMover->init(rIter);
}

void TripodBossRailMoveParts::makeActorDead() {
    if (mRailMover != nullptr && mRailMover->isWorking()) {
        mRailMover->end();
    }

    TripodBossFixPartsBase::makeActorDead();
}

void TripodBossRailMoveParts::activateTripodBoss() {
    mRailMover->start();
    TripodBossFixParts::activateTripodBoss();
}

void TripodBossRailMoveParts::calcTripodLocalMatrix(TPos3f* pMtx) {
    _BC.setPS2(mRailMover->_28);
    TripodBossFixPartsBase::calcTripodLocalMatrix(pMtx);
}

void TripodBossRailMoveParts::exeWaitOwn() {
    mRailMover->movement();
}

TripodBossRailMoveParts::~TripodBossRailMoveParts() {}

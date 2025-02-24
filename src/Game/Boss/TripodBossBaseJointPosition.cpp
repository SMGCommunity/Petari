#include "Game/Boss/TripodBossBaseJointPosition.hpp"
#include "Game/Boss/TripodBossAccesser.hpp"
#include "Game/Util/ObjUtil.hpp"

TripodBossBaseJointPosition::TripodBossBaseJointPosition(const char *pName) : LiveActor(pName) {
    _8C = -1;
    _90.identity();
}

void TripodBossBaseJointPosition::init(const JMapInfoIter &rIter) {
    MR::initDefaultPos(this, rIter);
    MR::connectToSceneCollisionEnemyMovement(this);
    MR::getJMapInfoArg0NoInit(rIter, &_8C);
    MR::getJMapInfoMatrixFromRT(rIter, &_90);
    MR::addBaseMatrixFollowTarget(this, rIter, nullptr, nullptr);
    MR::addTripodBossParts(this);
    MR::invalidateClipping(this);
    makeActorDead();
}

void TripodBossBaseJointPosition::initAfterPlacement() {
    MR::makeMtxTR(_90, this);
    MR::setTripodBossJointAttachBaseMatrix(_90, _8C);
    MR::concatTripodBossAttachJointMatrix(&_90, _8C);
}

void TripodBossBaseJointPosition::control() {
    MR::makeMtxTR(_90, this);
    MR::concatTripodBossAttachJointMatrix(&_90, _8C);
    if (MR::isEndBreakDownDemoTripodBoss()) {
        kill();
    }
}

TripodBossBaseJointPosition::~TripodBossBaseJointPosition() {

}

MtxPtr TripodBossBaseJointPosition::getBaseMtx() const {
    return (MtxPtr)&_90;
}

#include "Game/Boss/TripodBossBaseJointPosition.hpp"
#include "Game/Boss/TripodBossAccesser.hpp"
#include "Game/Util/BaseMatrixFollowTargetHolder.hpp"
#include "Game/Util/JMapUtil.hpp"
#include "Game/Util/LiveActorUtil.hpp"
#include "Game/Util/MtxUtil.hpp"
#include "Game/Util/ObjUtil.hpp"

TripodBossBaseJointPosition::TripodBossBaseJointPosition(const char* pName) : LiveActor(pName), mID(-1) {
    mMtx.identity();
}

void TripodBossBaseJointPosition::init(const JMapInfoIter& rIter) {
    MR::initDefaultPos(this, rIter);
    MR::connectToSceneCollisionEnemyMovement(this);
    MR::getJMapInfoArg0NoInit(rIter, &mID);
    MR::getJMapInfoMatrixFromRT(rIter, &mMtx);
    MR::addBaseMatrixFollowTarget(this, rIter, nullptr, nullptr);
    MR::addTripodBossParts(this);
    MR::invalidateClipping(this);
    makeActorDead();
}

void TripodBossBaseJointPosition::initAfterPlacement() {
    MR::makeMtxTR(mMtx, this);
    MR::setTripodBossJointAttachBaseMatrix(mMtx, mID);
    MR::concatTripodBossAttachJointMatrix(&mMtx, mID);
}

void TripodBossBaseJointPosition::control() {
    MR::makeMtxTR(mMtx, this);
    MR::concatTripodBossAttachJointMatrix(&mMtx, mID);

    if (MR::isEndBreakDownDemoTripodBoss()) {
        kill();
    }
}

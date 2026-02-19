#include "Game/Enemy/WalkerStateWander.hpp"
#include "Game/Enemy/TerritoryMover.hpp"
#include "Game/Enemy/WalkerStateFunction.hpp"
#include "Game/Enemy/WalkerStateParam.hpp"
#include "Game/Util/ActorMovementUtil.hpp"
#include "Game/Util/MapUtil.hpp"

namespace {
    WalkerStateWanderParam sDefaultParam;
}

namespace NrvWalkerStateWander {
    NEW_NERVE(WalkerStateWanderNrvWait, WalkerStateWander, Wait);
    NEW_NERVE(WalkerStateWanderNrvWalk, WalkerStateWander, Walk);
}  // namespace NrvWalkerStateWander

WalkerStateWanderParam::WalkerStateWanderParam() : mWaitTime(120), mWalkTime(120), mSpeed(0.2f), mTurnDegree(3.0f), mTargetDistance(20.0f) {
}

WalkerStateWander::WalkerStateWander(LiveActor* pActor, TVec3f* pDirection, WalkerStateParam* pStateParam, WalkerStateWanderParam* pWanderParam)
    : ActorStateBase< LiveActor >("クリボー型うろつき状態", pActor), mDirection(pDirection), mTerritoryMover(nullptr), mStateParam(pStateParam),
      mWanderParam(pWanderParam) {
    initNerve(&NrvWalkerStateWander::WalkerStateWanderNrvWait::sInstance);

    if (mWanderParam == nullptr) {
        mWanderParam = &sDefaultParam;
    }

    mTerritoryMover = new TerritoryMover(500.0f);
    mTerritoryMover->setCenter(getHost()->mPosition);
}

void WalkerStateWander::appear() {
    mIsDead = false;
    setNerve(&NrvWalkerStateWander::WalkerStateWanderNrvWait::sInstance);
}

void WalkerStateWander::setWanderCenter(const TVec3f& rCenter) {
    mTerritoryMover->setCenter(rCenter);
}

void WalkerStateWander::exeWait() {
    if (MR::isFirstStep(this)) {
        MR::startAction(getHost(), "Wait");
    }

    WalkerStateFunction::calcPassiveMovement(getHost(), mStateParam);

    if (MR::isGreaterStep(this, mWanderParam->mWaitTime)) {
        mTerritoryMover->decideNextTargetPos(getHost());
        setNerve(&NrvWalkerStateWander::WalkerStateWanderNrvWalk::sInstance);
    }
}

void WalkerStateWander::exeWalk() {
    if (MR::isFirstStep(this)) {
        MR::startAction(getHost(), "Walk");
    }

    MR::turnDirectionToTargetUseGroundNormalDegree(getHost(), mDirection, mTerritoryMover->_10, mWanderParam->mTurnDegree);

    if (MR::isFaceToTargetHorizontalDegree(getHost(), mTerritoryMover->_10, *mDirection, 8.0f)) {
        MR::addVelocityMoveToDirection(getHost(), *mDirection, mWanderParam->mSpeed);
    }

    WalkerStateFunction::calcPassiveMovement(getHost(), mStateParam);

    if (MR::isFallNextMove(getHost(), 150.0f, 150.0f, 150.0f, nullptr)) {
        MR::zeroVelocity(getHost());
        setNerve(&NrvWalkerStateWander::WalkerStateWanderNrvWait::sInstance);
        return;
    }

    if (MR::isGreaterStep(this, mWanderParam->mWalkTime) || mTerritoryMover->isReachedTarget(getHost(), mWanderParam->mTargetDistance)) {
        setNerve(&NrvWalkerStateWander::WalkerStateWanderNrvWait::sInstance);
    }
}

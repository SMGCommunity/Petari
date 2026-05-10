#include "Game/Enemy/WalkerStateChase.hpp"
#include "Game/Enemy/WalkerStateFunction.hpp"

namespace {
    WalkerStateChaseParam sDefaultParam;
};

namespace NrvWalkerStateChase {
    NEW_NERVE(WalkerStateChaseNrvStart, WalkerStateChase, Start);
    NEW_NERVE(WalkerStateChaseNrvEnd, WalkerStateChase, End);
};  // namespace NrvWalkerStateChase

WalkerStateChaseParam::WalkerStateChaseParam() : mChaseTime(130), mForceChaseEndTime(300), mTurnMaxRateDegree(2.0f), mChaseEndWaitTime(30) {
}

WalkerStateChase::WalkerStateChase(LiveActor* pHost, TVec3f* pDirection, WalkerStateParam* pStateParam, WalkerStateChaseParam* pChaseParam)
    : ActorStateBase< LiveActor >("クリボー追いかけ状態", pHost), mStateParam(pStateParam), mChaseParam(pChaseParam), mDirection(pDirection) {
    if (mChaseParam == nullptr) {
        mChaseParam = &sDefaultParam;
    }

    initNerve(&NrvWalkerStateChase::WalkerStateChaseNrvStart::sInstance);
}

void WalkerStateChase::appear() {
    mIsDead = false;
    setNerve(&NrvWalkerStateChase::WalkerStateChaseNrvStart::sInstance);
}

void WalkerStateChase::exeStart() {
    if (MR::isFirstStep(this)) {
        MR::startAction(getHost(), "Run");
    }

    bool isInSight = WalkerStateFunction::isInSightPlayer(getHost(), *mDirection, mStateParam);
    if (isInSight) {
        MR::turnDirectionToTargetUseGroundNormalDegree(getHost(), mDirection, *MR::getPlayerPos(), mChaseParam->mTurnMaxRateDegree);
    }

    MR::addVelocityMoveToDirection(getHost(), *mDirection, mChaseParam->mChaseSpeed);
    WalkerStateFunction::calcPassiveMovement(getHost(), mStateParam);

    if (MR::isFallNextMove(getHost(), 150.0f, 150.0f, 150.0f, nullptr)) {
        MR::zeroVelocity(getHost());
        setNerve(&NrvWalkerStateChase::WalkerStateChaseNrvEnd::sInstance);
    } else if (MR::isGreaterStep(this, mChaseParam->mForceChaseEndTime) || (MR::isGreaterStep(this, mChaseParam->mChaseTime) && !isInSight)) {
        setNerve(&NrvWalkerStateChase::WalkerStateChaseNrvEnd::sInstance);
    }
}

void WalkerStateChase::exeEnd() {
    if (MR::isFirstStep(this)) {
        MR::startAction(getHost(), "Wait");
    }

    WalkerStateFunction::calcPassiveMovement(getHost(), mStateParam);
    if (MR::isGreaterStep(this, mChaseParam->mChaseEndWaitTime)) {
        kill();
    }
}

bool WalkerStateChase::isRunning() const {
    return isNerve(&NrvWalkerStateChase::WalkerStateChaseNrvStart::sInstance) && MR::isBindedGround(getHost());
}

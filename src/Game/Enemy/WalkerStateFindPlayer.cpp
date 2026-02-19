#include "Game/Enemy/WalkerStateFindPlayer.hpp"
#include "Game/Enemy/WalkerStateFunction.hpp"
#include "Game/Util/ActorMovementUtil.hpp"
#include "Game/Util/LiveActorUtil.hpp"
#include "Game/Util/NerveUtil.hpp"

namespace {
    WalkerStateFindPlayerParam sDefaultParam;
}

namespace NrvWalkerStateFindPlayer {
    NEW_NERVE(WalkerStateFindPlayerNrvFind, WalkerStateFindPlayer, Find);
    NEW_NERVE(WalkerStateFindPlayerNrvFindJumpStart, WalkerStateFindPlayer, FindJumpStart);
    NEW_NERVE(WalkerStateFindPlayerNrvFindJump, WalkerStateFindPlayer, FindJump);
    NEW_NERVE(WalkerStateFindPlayerNrvFindJumpEnd, WalkerStateFindPlayer, FindJumpEnd);
}  // namespace NrvWalkerStateFindPlayer

WalkerStateFindPlayerParam::WalkerStateFindPlayerParam() : mJumpStartStep(30), mJumpVelocity(20.0f), mTurnDegree(5.0f) {
}

WalkerStateFindPlayer::WalkerStateFindPlayer(LiveActor* pHost, TVec3f* pDirection, WalkerStateParam* pStateParam,
                                             WalkerStateFindPlayerParam* pFindPlayerParam)
    : ActorStateBase< LiveActor >("歩行型プレイヤー発見挙動", pHost), mDirection(pDirection), mStateParam(pStateParam),
      mFindPlayerParam(pFindPlayerParam) {
    initNerve(&NrvWalkerStateFindPlayer::WalkerStateFindPlayerNrvFind::sInstance);
    if (mFindPlayerParam == nullptr) {
        mFindPlayerParam = &sDefaultParam;
    }
}

void WalkerStateFindPlayer::appear() {
    mIsDead = false;
    setNerve(&NrvWalkerStateFindPlayer::WalkerStateFindPlayerNrvFind::sInstance);
}

void WalkerStateFindPlayer::exeFind() {
    if (MR::isFirstStep(this)) {
        MR::startAction(getHost(), "Turn");
    }

    WalkerStateFunction::calcPassiveMovement(getHost(), mStateParam);
    MR::turnDirectionToTargetUseGroundNormalDegree(getHost(), mDirection, *MR::getPlayerPos(), mFindPlayerParam->mTurnDegree);

    if (MR::isStep(this, mFindPlayerParam->mJumpStartStep)) {
        setNerve(&NrvWalkerStateFindPlayer::WalkerStateFindPlayerNrvFindJumpStart::sInstance);
    }
}

void WalkerStateFindPlayer::exeFindJumpStart() {
    if (MR::isFirstStep(this)) {
        MR::startAction(getHost(), "JumpStart");
    }

    WalkerStateFunction::calcPassiveMovement(getHost(), mStateParam);

    if (MR::isBckStopped(getHost())) {
        setNerve(&NrvWalkerStateFindPlayer::WalkerStateFindPlayerNrvFindJump::sInstance);
    }
}

void WalkerStateFindPlayer::exeFindJump() {
    if (MR::isFirstStep(this)) {
        MR::addVelocityJump(getHost(), mFindPlayerParam->mJumpVelocity);
        MR::startAction(getHost(), "Jump");
    }

    WalkerStateFunction::calcPassiveMovement(getHost(), mStateParam);

    if (MR::isBindedGround(getHost()) && MR::isGreaterStep(this, 5)) {
        setNerve(&NrvWalkerStateFindPlayer::WalkerStateFindPlayerNrvFindJumpEnd::sInstance);
    }
}

void WalkerStateFindPlayer::exeFindJumpEnd() {
    if (MR::isFirstStep(this)) {
        MR::startAction(getHost(), "Land");
    }

    WalkerStateFunction::calcPassiveMovement(getHost(), mStateParam);

    if (MR::isBckStopped(getHost())) {
        kill();
    }
}

bool WalkerStateFindPlayer::isInSightPlayer() const {
    return WalkerStateFunction::isInSightPlayer(getHost(), *mDirection, mStateParam);
}

bool WalkerStateFindPlayer::isFindJumpBegin() const {
    return isNerve(&NrvWalkerStateFindPlayer::WalkerStateFindPlayerNrvFindJump::sInstance) && MR::isFirstStep(this);
}

bool WalkerStateFindPlayer::isLandStart() const {
    return isNerve(&NrvWalkerStateFindPlayer::WalkerStateFindPlayerNrvFindJumpEnd::sInstance) && MR::isFirstStep(this);
}

#include "Game/Enemy/WalkerStateFindPlayer.hpp"
#include "Game/Enemy/WalkerStateFunction.hpp"
#include "Game/LiveActor/Nerve.hpp"
#include "Game/Util/ActorMovementUtil.hpp"
#include "Game/Util/LiveActorUtil.hpp"
#include "Game/Util/NerveUtil.hpp"
#include "Game/Util/PlayerUtil.hpp"

namespace {
    WalkerStateFindPlayerParam sDefaultParam;
};  // namespace

namespace NrvWalkerStateFindPlayer {
    NEW_NERVE(WalkerStateFindPlayerNrvFind, WalkerStateFindPlayer, Find);
    NEW_NERVE(WalkerStateFindPlayerNrvFindJumpStart, WalkerStateFindPlayer, FindJumpStart);
    NEW_NERVE(WalkerStateFindPlayerNrvFindJump, WalkerStateFindPlayer, FindJump);
    NEW_NERVE(WalkerStateFindPlayerNrvFindJumpEnd, WalkerStateFindPlayer, FindJumpEnd);
};  // namespace NrvWalkerStateFindPlayer

WalkerStateFindPlayerParam::WalkerStateFindPlayerParam() : mJumpStartStep(30), mJumpVelocity(20.0f), mTurnMaxRateDegree(5.0f) {
}

WalkerStateFindPlayer::WalkerStateFindPlayer(LiveActor* pHost, TVec3f* pDirection, WalkerStateParam* pStateParam,
                                             WalkerStateFindPlayerParam* pFindPlayerParam)
    : ActorStateBase< LiveActor >("歩行型プレイヤー発見挙動", pHost), mDirection(pDirection), mStateParam(pStateParam),
      mFindPlayerParam(pFindPlayerParam) {
    initNerve(GET_NERVE(WalkerStateFindPlayer, WalkerStateFindPlayerNrvFind));
    if (mFindPlayerParam == nullptr) {
        mFindPlayerParam = &::sDefaultParam;
    }
}

void WalkerStateFindPlayer::appear() {
    mIsDead = false;
    setNerve(GET_NERVE(WalkerStateFindPlayer, WalkerStateFindPlayerNrvFind));
}

void WalkerStateFindPlayer::exeFind() {
    if (MR::isFirstStep(this)) {
        MR::startAction(getHost(), "Turn");
    }

    WalkerStateFunction::calcPassiveMovement(getHost(), mStateParam);
    MR::turnDirectionToTargetUseGroundNormalDegree(getHost(), mDirection, *MR::getPlayerPos(), mFindPlayerParam->mTurnMaxRateDegree);

    if (MR::isStep(this, mFindPlayerParam->mJumpStartStep)) {
        setNerve(GET_NERVE(WalkerStateFindPlayer, WalkerStateFindPlayerNrvFindJumpStart));
    }
}

void WalkerStateFindPlayer::exeFindJumpStart() {
    if (MR::isFirstStep(this)) {
        MR::startAction(getHost(), "JumpStart");
    }

    WalkerStateFunction::calcPassiveMovement(getHost(), mStateParam);

    if (MR::isBckStopped(getHost())) {
        setNerve(GET_NERVE(WalkerStateFindPlayer, WalkerStateFindPlayerNrvFindJump));
    }
}

void WalkerStateFindPlayer::exeFindJump() {
    if (MR::isFirstStep(this)) {
        MR::addVelocityJump(getHost(), mFindPlayerParam->mJumpVelocity);
        MR::startAction(getHost(), "Jump");
    }

    WalkerStateFunction::calcPassiveMovement(getHost(), mStateParam);

    if (MR::isBindedGround(getHost()) && MR::isGreaterStep(this, 5)) {
        setNerve(GET_NERVE(WalkerStateFindPlayer, WalkerStateFindPlayerNrvFindJumpEnd));
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
    return isNerve(GET_NERVE(WalkerStateFindPlayer, WalkerStateFindPlayerNrvFindJump)) && MR::isFirstStep(this);
}

bool WalkerStateFindPlayer::isLandStart() const {
    return isNerve(GET_NERVE(WalkerStateFindPlayer, WalkerStateFindPlayerNrvFindJumpEnd)) && MR::isFirstStep(this);
}

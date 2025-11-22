#include "Game/Enemy/WalkerStateChase.hpp"
#include "Game/Enemy/WalkerStateFunction.hpp"

WalkerStateChaseParam::WalkerStateChaseParam() {
    _4 = 130;
    _8 = 300;
    _C = 2.0f;
    _10 = 30;
}

namespace {
    WalkerStateChaseParam sDefaultParam;
};

namespace NrvWalkerStateChase {
    NEW_NERVE(WalkerStateChaseNrvStart, WalkerStateChase, Start);
    NEW_NERVE(WalkerStateChaseNrvEnd, WalkerStateChase, End);
};  // namespace NrvWalkerStateChase

WalkerStateChase::WalkerStateChase(LiveActor* pActor, TVec3f* pVec, WalkerStateParam* pParam, WalkerStateChaseParam* pChaseParam)
    : ActorStateBase< LiveActor >("クリボー追いかけ状態") {
    mParentActor = pActor;
    mStateParam = pParam;
    mChaseParam = pChaseParam;
    _18 = pVec;

    if (pChaseParam == nullptr) {
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
        MR::startAction(mParentActor, "Run");
    }

    bool isInSight = WalkerStateFunction::isInSightPlayer(mParentActor, *_18, mStateParam);
    if (isInSight) {
        LiveActor* parent = mParentActor;
        MR::turnDirectionToTargetUseGroundNormalDegree(parent, _18, *MR::getPlayerPos(), mChaseParam->_C);
    }

    MR::addVelocityMoveToDirection(mParentActor, *_18, mChaseParam->_0);
    WalkerStateFunction::calcPassiveMovement(mParentActor, mStateParam);

    if (MR::isFallNextMove(mParentActor, 150.0f, 150.0f, 150.0f, nullptr)) {
        MR::zeroVelocity(mParentActor);
        setNerve(&NrvWalkerStateChase::WalkerStateChaseNrvEnd::sInstance);
    } else if (MR::isGreaterStep(this, mChaseParam->_8) || (MR::isGreaterStep(this, mChaseParam->_4) && !isInSight)) {
        setNerve(&NrvWalkerStateChase::WalkerStateChaseNrvEnd::sInstance);
    }
}

void WalkerStateChase::exeEnd() {
    if (MR::isFirstStep(this)) {
        MR::startAction(mParentActor, "Wait");
    }

    WalkerStateFunction::calcPassiveMovement(mParentActor, mStateParam);
    if (MR::isGreaterStep(this, mChaseParam->_10)) {
        kill();
    }
}

bool WalkerStateChase::isRunning() const {
    bool isRunning = false;
    if (isNerve(&NrvWalkerStateChase::WalkerStateChaseNrvStart::sInstance)) {
        if (MR::isBindedGround(mParentActor)) {
            isRunning = true;
        }
    }

    return isRunning;
}

WalkerStateChase::~WalkerStateChase() {}

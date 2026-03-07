#include "Game/Enemy/WalkerStateRunaway.hpp"

// TODO: replace this call with MR::clamp
inline f32 clamp(f32 x, f32 min, f32 max) {
    if (x < min) {
        return min;
    } else if (x > max) {
        return max;
    } else {
        return x;
    }
}

namespace {
    static WalkerStateRunawayParam sDefaultParam;
};

namespace NrvWalkerStateRunaway {
    NEW_NERVE(WalkerStateRunawayNrvWait, WalkerStateRunaway, Wait);
    NEW_NERVE(WalkerStateRunawayNrvRunaway, WalkerStateRunaway, Runaway);
    NEW_NERVE(WalkerStateRunawayNrvWallJump, WalkerStateRunaway, WallJump);
};  // namespace NrvWalkerStateRunaway

WalkerStateRunawayParam::WalkerStateRunawayParam() {
    mWaitAction = "FollowMe";
    mRunawayAction = "Run";
    mJumpAction = "Jump";
    mGroundAccel = 0.05f;
    mAirAccel = 1.0f;
    _14 = 0.9f;
    _18 = 0.99f;
    _1C = 5;
    mTurnMaxRateDegree = 3.0f;
    mRunawayDistance = 1100.0f;
    mWaitDistance = 1300.0f;
    mRunawayTurnRateMaxDegree = 10.0f;
    mRunawayTurnRateMinDegree = 3.0f;
    mRunawayTurnTime = 30;
    mPlayerFrontLineLength = 25.0f;
    mRunawayBckRatio = 1.0f;
    mMinRunawayBckRate = 0.9f;
    mMaxRunawayBckRate = 1.4f;
    mWallJumpTime = 6;
    mWallReboundPower = 0.3f;
    mWallJumpPowerH = 8.0f;
    mWallJumpPowerV = 15.0f;
}

WalkerStateRunaway::WalkerStateRunaway(LiveActor* pHost, TVec3f* pDirection, WalkerStateRunawayParam* pRunawayParam)
    : ActorStateBase< LiveActor >("歩行型アクター逃げ", pHost), mRunawayParam(pRunawayParam), mDirection(pDirection), _18(0), mRunawaySpeed(1.0f) {
    if (mRunawayParam == nullptr) {
        mRunawayParam = &sDefaultParam;
    }

    initNerve(&NrvWalkerStateRunaway::WalkerStateRunawayNrvWait::sInstance);
}

void WalkerStateRunaway::appear() {
    mIsDead = false;
    setNerve(&NrvWalkerStateRunaway::WalkerStateRunawayNrvWait::sInstance);
    _18 = 0;
}

bool WalkerStateRunaway::tryRunaway() {
    if (MR::isNearPlayer(getHost(), mRunawayParam->mRunawayDistance)) {
        setNerve(&NrvWalkerStateRunaway::WalkerStateRunawayNrvRunaway::sInstance);
        return true;
    }

    return false;
}

bool WalkerStateRunaway::tryWait() {
    if (isInWaitRange(mRunawayParam->mWaitDistance)) {
        setNerve(&NrvWalkerStateRunaway::WalkerStateRunawayNrvWait::sInstance);
        return true;
    }

    return false;
}

bool WalkerStateRunaway::tryWallJump() {
    if (MR::isBindedWall(getHost()) && MR::calcHitPowerToWall(getHost()) > 0.0f) {
        TVec3f horiz;
        MR::calcVecFromPlayerH(&horiz, getHost());
        TVec3f wallNorml;
        MR::calcWallNormalHorizontal(&wallNorml, getHost());

        if (MR::isOppositeDirection(horiz, wallNorml, 0.01f)) {
            MR::addVelocityClockwiseToDirection(getHost(), wallNorml,
                                                MR::isHalfProbability() ? mRunawayParam->mWallJumpPowerH : -mRunawayParam->mWallJumpPowerH);
        } else {
            MR::addVelocityMoveToDirection(getHost(), wallNorml.addOtherInline(horiz), mRunawayParam->mWallJumpPowerH);
        }

        MR::addVelocityJump(getHost(), mRunawayParam->mWallJumpPowerV);
        setNerve(&NrvWalkerStateRunaway::WalkerStateRunawayNrvWallJump::sInstance);
        return true;
    }

    return false;
}

void WalkerStateRunaway::exeWait() {
    if (MR::isFirstStep(this)) {
        MR::startAction(getHost(), mRunawayParam->mWaitAction);
    }

    MR::turnDirectionToPlayerDegree(getHost(), mDirection, mRunawayParam->mTurnMaxRateDegree);
    MR::addVelocityToGravity(getHost(), MR::isBindedGround(getHost()) ? mRunawayParam->mGroundAccel : mRunawayParam->mAirAccel);
    MR::attenuateVelocity(getHost(), check18() ? mRunawayParam->_14 : mRunawayParam->_18);

    if (tryRunaway()) {
        return;
    }
}

void WalkerStateRunaway::exeRunaway() {
    if (MR::isFirstStep(this)) {
        MR::startAction(getHost(), mRunawayParam->mRunawayAction);
    }

    TVec3f v13;
    JMAVECScaleAdd(MR::getPlayerVelocity(), MR::getPlayerPos(), &v13, mRunawayParam->mPlayerFrontLineLength);
    TVec3f* actorPos = &mHost->mPosition;
    MR::calcPerpendicFootToLineInside(&v13, *actorPos, *MR::getPlayerPos(), v13);
    MR::turnDirectionFromTargetDegree(mHost, mDirection, v13,
                                      MR::calcNerveValue(mHost, mRunawayParam->mRunawayTurnTime, mRunawayParam->mRunawayTurnRateMaxDegree,
                                                         mRunawayParam->mRunawayTurnRateMinDegree));

    // TODO: MR::clamp
    MR::setBckRate(getHost(),
                   clamp(mRunawaySpeed / mRunawayParam->mRunawayBckRatio, mRunawayParam->mMinRunawayBckRate, mRunawayParam->mMaxRunawayBckRate));
    MR::addVelocityMoveToDirection(getHost(), *mDirection, check18() ? mRunawaySpeed : 0.0f);
    MR::addVelocityToGravity(getHost(), MR::isBindedGround(getHost()) ? mRunawayParam->mGroundAccel : mRunawayParam->mAirAccel);
    MR::attenuateVelocity(getHost(), check18() ? mRunawayParam->_14 : mRunawayParam->_18);

    if (!tryWait()) {
        if (tryWallJump()) {
            return;
        }
    }
}

void WalkerStateRunaway::exeWallJump() {
    if (MR::isFirstStep(this)) {
        MR::startAction(getHost(), mRunawayParam->mJumpAction);
    }

    MR::turnDirectionDegree(getHost(), mDirection, getHost()->mVelocity, 45.0f);
    MR::addVelocityToGravity(getHost(), mRunawayParam->mAirAccel);
    MR::attenuateVelocity(getHost(), mRunawayParam->_18);
    MR::reboundVelocityFromEachCollision(getHost(), 0.0f, mRunawayParam->mWallReboundPower, 0.0f, 0.0f);

    if (MR::isGreaterStep(this, mRunawayParam->mWallJumpTime)) {
        if (MR::isBindedGround(getHost())) {
            setNerve(&NrvWalkerStateRunaway::WalkerStateRunawayNrvRunaway::sInstance);
        }
    }
}

bool WalkerStateRunaway::isRunning() const {
    return isNerve(&NrvWalkerStateRunaway::WalkerStateRunawayNrvRunaway::sInstance);
}

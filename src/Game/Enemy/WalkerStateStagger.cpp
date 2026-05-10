#include "Game/Enemy/WalkerStateStagger.hpp"
#include "Game/Enemy/WalkerStateFunction.hpp"

namespace {
    WalkerStateStaggerParam sDefaultStaggerParam;
}

namespace NrvWalkerStateStagger {
    NEW_NERVE(WalkerStateStaggerNrvStagger, WalkerStateStagger, Stagger);
    NEW_NERVE(WalkerStateStaggerNrvStaggerEnd, WalkerStateStagger, StaggerEnd);
}  // namespace NrvWalkerStateStagger

WalkerStateStaggerParam::WalkerStateStaggerParam()
    : mPunchPowerH(10.0f), mPunchPowerV(10.0f), mPunchPowerSideH(10.0f), mStopSceneFrames(4), mKickEnableStep(15), mStaggerTime(180),
      mRotateStartTime(60), mRotateEndTime(120), mStaggerFrontPower(0.35f), mStaggerSidePower(0.5f), mStaggerSideCircleRateDegree(3.0f),
      mRotateRateDegree(40.0f) {
}

WalkerStateStagger::WalkerStateStagger(LiveActor* pHost, TVec3f* pDirection, WalkerStateParam* pStateParam, WalkerStateStaggerParam* pStaggerParam)
    : ActorStateBase< LiveActor >("歩行型よろめき状態", pHost), mStateParam(pStateParam), mStaggerParam(pStaggerParam), mVelH(0.0f, 0.0f, 1.0f),
      mDirection(pDirection) {
    if (mStaggerParam == nullptr) {
        mStaggerParam = &sDefaultStaggerParam;
    }

    initNerve(&NrvWalkerStateStagger::WalkerStateStaggerNrvStagger::sInstance);
}

void WalkerStateStagger::appear() {
    mIsDead = false;
    setNerve(&NrvWalkerStateStagger::WalkerStateStaggerNrvStagger::sInstance);
}

void WalkerStateStagger::setPunchDirection(HitSensor* pSender, HitSensor* pReceiver) {
    MR::calcSensorHorizonNormalize(&mVelH, getHost()->mGravity, pSender, pReceiver);
    MR::deleteEffectAll(getHost());
    MR::setVelocitySeparateHV(getHost(), pSender, pReceiver, mStaggerParam->mPunchPowerH, mStaggerParam->mPunchPowerV);
    MR::addVelocityClockwiseToTarget(getHost(), MR::getSensorPos(pSender), mStaggerParam->mPunchPowerSideH);
}

void WalkerStateStagger::exeStagger() {
    if (MR::isFirstStep(this)) {
        MR::startAction(getHost(), "StaggerStart");
        MR::stopScene(mStaggerParam->mStopSceneFrames);
    }

    if (MR::isBckOneTimeAndStopped(getHost())) {
        MR::startAction(getHost(), "Stagger");
    }

    TVec3f dir;
    dir.rejection(mVelH, getHost()->mGravity);

    if (!MR::isNearZero(dir)) {
        MR::normalize(dir, &mVelH);
    }

    f32 t = MR::calcNerveEaseInOutValue(this, mStaggerParam->mRotateStartTime, mStaggerParam->mRotateEndTime, 1.0f, 0.0f);
    MR::rotateDirectionGravityDegree(getHost(), mDirection, mStaggerParam->mRotateRateDegree * t);

    f32 s = JMath::sSinCosTable.sinLap(MR::repeatDegree(mStaggerParam->mStaggerSideCircleRateDegree * getNerveStep()));

    MR::addVelocityClockwiseToDirection(getHost(), mVelH, t * (mStaggerParam->mStaggerSidePower * s));
    MR::addVelocityMoveToDirection(getHost(), mVelH, mStaggerParam->mStaggerFrontPower * t);
    WalkerStateFunction::calcPassiveMovement(getHost(), mStateParam);

    reboundWall();

    if (MR::isGreaterStep(this, mStaggerParam->mStaggerTime)) {
        setNerve(&NrvWalkerStateStagger::WalkerStateStaggerNrvStaggerEnd::sInstance);
    }
}

void WalkerStateStagger::exeStaggerEnd() {
    if (MR::isFirstStep(this)) {
        MR::startAction(getHost(), "StaggerEnd");
    }

    WalkerStateFunction::calcPassiveMovement(getHost(), mStateParam);

    if (MR::isBckStopped(getHost())) {
        kill();
    }
}

void WalkerStateStagger::reboundWall() {
    if (!MR::isBindedWall(getHost())) {
        return;
    }

    if (MR::calcReboundVelocity(&getHost()->mVelocity, *MR::getWallNormal(getHost()), 0.9f)) {
        TVec3f velH;
        velH.rejection(getHost()->mVelocity, getHost()->mGravity);
        if (!MR::normalizeOrZero(&velH)) {
            mVelH.set(velH);
        }
    }
}

bool WalkerStateStagger::isEnableKick() const {
    if (mIsDead) {
        return false;
    }

    if (isNerve(&NrvWalkerStateStagger::WalkerStateStaggerNrvStagger::sInstance) && MR::isLessStep(this, mStaggerParam->mKickEnableStep)) {
        return false;
    }

    return true;
}

bool WalkerStateStagger::isUpsideDown() const {
    if (mIsDead) {
        return false;
    }

    if (isNerve(&NrvWalkerStateStagger::WalkerStateStaggerNrvStaggerEnd::sInstance) && MR::isGreaterStep(this, 15)) {
        return false;
    }

    return true;
}

bool WalkerStateStagger::isStaggerStart() const {
    return isNerve(&NrvWalkerStateStagger::WalkerStateStaggerNrvStagger::sInstance) && MR::isStep(this, 1);
}

bool WalkerStateStagger::isSwooning(s32 swoonStep) const {
    return isNerve(&NrvWalkerStateStagger::WalkerStateStaggerNrvStagger::sInstance) && MR::isGreaterEqualStep(this, swoonStep);
}

bool WalkerStateStagger::isSpinning(s32 spinStartStep, s32 spinEndStep) const {
    return isNerve(&NrvWalkerStateStagger::WalkerStateStaggerNrvStagger::sInstance) && MR::isGreaterEqualStep(this, spinStartStep) &&
           MR::isLessEqualStep(this, spinEndStep);
}

bool WalkerStateStagger::isRecoverStart() const {
    return isNerve(&NrvWalkerStateStagger::WalkerStateStaggerNrvStaggerEnd::sInstance) && MR::isFirstStep(this);
}

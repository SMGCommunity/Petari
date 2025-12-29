#include "Game/Ride/SurfRay.hpp"
#include "Game/LiveActor/ActorJointCtrl.hpp"
#include "Game/LiveActor/MaterialCtrl.hpp"

inline f32 min(f32 a, f32 b) {
    if (a >= b) {
        return b;
    }

    return a;
}

namespace NrvSurfRay {
    NEW_NERVE(SurfRayNrvWaitPlayer, SurfRay, WaitPlayer);
    NEW_NERVE(SurfRayNrvTurnToWait, SurfRay, TurnToWait);
    NEW_NERVE(SurfRayNrvWait, SurfRay, Wait);
    NEW_NERVE(SurfRayNrvRideAccel, SurfRay, RideAccel);
    NEW_NERVE(SurfRayNrvRideAccelTwist, SurfRay, RideAccelTwist);
    NEW_NERVE(SurfRayNrvRideFree, SurfRay, RideFree);
    NEW_NERVE(SurfRayNrvRideFreeTwist, SurfRay, RideFreeTwist);
    NEW_NERVE(SurfRayNrvRideJump, SurfRay, RideJump);
    NEW_NERVE(SurfRayNrvRideJumpHigh, SurfRay, RideJumpHigh);
    NEW_NERVE(SurfRayNrvRideFall, SurfRay, RideFall);
    NEW_NERVE(SurfRayNrvRideLand, SurfRay, RideLand);
    NEW_NERVE(SurfRayNrvTutorial, SurfRay, Tutorial);
    NEW_NERVE(SurfRayNrvWipeOut, SurfRay, WipeOut);
    NEW_NERVE(SurfRayNrvWipeIn, SurfRay, WipeIn);
    NEW_NERVE(SurfRayNrvReady, SurfRay, Ready);
};  // namespace NrvSurfRay

namespace {
    static const f32 sDistancePlayerNear = 1300.0f;
    // static const f32 sGravity
    // static const f32 sGroundReboundRate
    // static const f32 sWallReboundRate
    // static const f32 sReboundSpeedMin
    // static const f32 sRunAccel
    // static const f32 sRunSpeedMax
    // static const f32 sFlyUpSpeedMaxY
    // static const f32 sFallSpeedMaxY
    // static const f32 sRunGroundFrictionRate
    // static const f32 sRunAirFrictionRate
    static const f32 sSpeedToRumble = 5.0f;
    // static const f32 sWaterFriction
    // static const f32 sWaterGravity
    // static const f32 sWaterPressureRate
    // static const f32 sWaterWaveNrmAccelRate
    // static const f32 sWaterUpSideSpeedMax
    // static const f32 sWaterBottomReboundRate
    // static const f32 sWaterUpperPosOffset
    // static const f32 sWaterJudgeFront
    // static const f32 sWaterJudgeUp
    // static const f32 sWaterJudgeWidth
    // static const f32 sUpSideGroundFrictionRate
    // static const f32 sUpSideAirFrictionRate
    // static const f32 sUpSideSpeedMin
    // static const s32 sStepEnableJumpAtAir
    // static const s32 sStepToFallAtAir
    // static const f32 sJumpAccelMinY
    // static const f32 sJumpAccelMaxY
    // static const f32 sJumpSpeedMaxY
    // static const f32 sJumpSpeedHigh
    // static const f32 sRunSpeedMinToRotate
    // static const f32 sRotateAccelRate
    // static const f32 sPadAccelAddRateZ
    // static const f32 sPadAccelClamp
    // static const f32 sPadAccelInvalidMaxZ
    // static const f32 sPadAccelInvalidMinX
    // static const f32 sPadAccelInvalidFrictionRate
    // static const f32 sRotateSpeedFrictionRate
    // static const f32 sRotateFrictionRate
    // static const f32 sRotateSpeedMin
    // static const f32 sRotateSpeedMax
    // static const f32 sRotateMaxZ
    // static const f32 sRotateSpeedRate
    static const f32 sBlendRatioAxisUpAir = 0.02f;
    static const f32 sBlendRatioAxisUpGround = 0.1f;
    static const f32 sBlendRatioAxisFrontWall = 0.1f;
    // static const f32 sPlayerPosOffsetY
    // static const f32 sPlayerPosOffsetZ
    // static const f32 sRiderRotateInterpolateRate
    // static const f32 sRiderRotateRateScale
    static const f32 sTwistStartDiffMin = 0.2f;
    static const f32 sBtkSpeedMin = 1.0f;
    static const f32 sBtkSpeedMax = 4.0f;
    static const f32 sBtkSpeedRate = 0.4f;
    static const f32 sShadowAlphaMax = 64.0f;
    static const f32 sShadowAlphaSpeed = 10.0f;
    static const f32 sSpeedToEmitEffect = 6.0f;
    static const f32 sSpeedToEmitEffectFast = 20.0f;
    // static const f32 sMoveSeRotRate
    static const f32 sMoveSe0StartSpeed = 2.5f;
    static const f32 sMoveSe1StartSpeed = 7.5f;
    static const f32 sMoveSe2StartSpeed = 14.0f;
    static const f32 sMoveSe3StartSpeed = 25.0f;
    static const f32 sMoveSe3MaxSpeed = 35.0f;
    static const f32 sWindSeStartSpeed = 32.0f;
    static const f32 sWindSeMaxSpeed = 50.0f;
    // static const f32 sJumpInSoundMaxSpeed
    // static const f32 sHitSoundStartVelocity
    // static const f32 sVelocityUpSideScale
    // static const f32 sRotateZFrictionRate
    // static const f32 sMarioAnimeBlendRate

    bool calcWaterShadowPos(TVec3f* pShadowPos, const TVec3f& rStartPos, const TVec3f& rShadowDir) {
        pShadowPos->set(rStartPos);
        TVec3f sample(rStartPos);

        for (s32 i = 0; i < 10; i++) {
            WaterInfo waterInfo = WaterInfo();

            if (MR::getWaterAreaObj(&waterInfo, sample)) {
                MR::getWaterAreaInfo(&waterInfo, sample, rShadowDir, false);
                pShadowPos->set(waterInfo.mSurfacePos);

                return true;
            }

            TVec3f step(rShadowDir);
            step.scale(100.0f);
            sample.add(step);
        }

        return false;
    }
};  // namespace

SurfRay::SurfRay(const char* pName, s32 channel)
    : LiveActor(pName), mSurfSpeed(0.0f), mOrthoSpeed(0.0f), mOrthoVelocity(0.0f, 0.0f, 0.0f), mSteerRate(0.0f), mSteerAccel(0.0f),
      mFront(0.0f, 0.0f, 1.0f), mUp(0.0f, 1.0f, 0.0f), mSide(1.0f, 0.0f, 0.0f), mBaseUp(0.0f, 1.0f, 0.0f), mBaseSide(1.0f, 0.0f, 0.0f),
      mGroundNormal(0.0f, 1.0f, 0.0f), mWarpPos(0.0f, 0.0f, 0.0f), mRider(nullptr), mPadChannel(channel), mRayTilt(0.0f), mInWater(false), mAirTime(0),
      mWaterNormal(0.0f, 1.0f, 0.0f), mTwistBufferSize(8), mWaterShadowPos(0.0f, 0.0f, 0.0f), mShadowAlpha(0.0f), mTwistBuffer(nullptr),
      mInTutorialArea(false), mInTutorial(false), mLectureIdx(0), mActorJointCtrl(nullptr), mProjmapFxMtxSetter(nullptr) {
    mEffectHostMtx.identity();
}

void SurfRay::init(const JMapInfoIter& pMapInfoIter) {
    if (MR::hasRetryGalaxySequence()) {
        MR::resetPosition(this, "スタート位置（サーフィン）");
    } else {
        MR::initDefaultPos(this, pMapInfoIter);
    }

    resetAllInfo();
    initModelManagerWithAnm("SurfRay", nullptr, this != nullptr);

    mProjmapFxMtxSetter = MR::initDLMakerProjmapEffectMtxSetter(this);
    MR::newDifferedDLBuffer(this);
    mProjmapFxMtxSetter->updateMtxUseBaseMtx();

    MR::startBtk(this, "SurfRay");
    MR::startBtp(this, "Blink");
    MR::connectToSceneRide(this);
    MR::initLightCtrl(this);

    initHitSensor(2);
    MR::addHitSensorRide(this, "body", 8, 260.0f, TVec3f(0.0f, 0.0f, 0.0f));
    MR::addHitSensorBinder(this, "bind", 8, 300.0f, TVec3f(0.0f, 0.0f, 0.0f));

    initBinder(200.0f, 50.0f, 8);
    initEffectKeeper(0, nullptr, false);
    MR::setEffectHostMtx(this, "Ripple", mEffectHostMtx);
    initSound(15, false);

    MR::initShadowFromCSV(this, "Shadow");
    Color8 shadowColor(0, 40, 128, 64);
    MR::setShadowSurfaceOvalColor(this, 0, shadowColor);
    MR::setShadowSurfaceOvalAlpha(this, 0, 64);
    MR::setShadowProjectionPtr(this, 0, &mWaterShadowPos, &mWaterNormal);
    MR::setShadowSurfaceOvalAlpha(this, 0, (u8)mShadowAlpha & 0xff);

    initNerve(&NrvSurfRay::SurfRayNrvWaitPlayer::sInstance);

    MR::useStageSwitchWriteA(this, pMapInfoIter);
    MR::tryFindNamePos("ワープ位置（サーフィン）", &mWarpPos, nullptr);

    mActorJointCtrl = new ActorJointCtrl(this);
    mTwistBuffer = new f32[mTwistBufferSize];

    for (s32 count = 0; count < mTwistBufferSize; count++) {
        mTwistBuffer[count] = 0.0f;
    }

    MR::joinToGroupArray(this, pMapInfoIter, 0, 32);

    if (!MR::hasRetryGalaxySequence()) {
        mInTutorialArea = true;
    }

    MR::invalidateClipping(this);
    makeActorAppeared();
}

void SurfRay::exeWaitPlayer() {
    if (MR::isFirstStep(this)) {
        MR::startBck(this, "FirstWait", nullptr);
    }

    calcWaterShadowPos(&mWaterShadowPos, mPosition, mGravity);

    if (MR::isNearPlayer(this, sDistancePlayerNear)) {
        setNerve(&NrvSurfRay::SurfRayNrvTurnToWait::sInstance);
    }
}

void SurfRay::exeTurnToWait() {
    if (MR::isFirstStep(this)) {
        MR::startBck(this, "FirstWaitTurn", nullptr);
        MR::startSound(this, "SE_SM_SURF_RAY_TURNTO_MR", -1, -1);
    }

    if (MR::isBckStopped(this)) {
        setNerve(&NrvSurfRay::SurfRayNrvWait::sInstance);
    }
}

void SurfRay::exeWait() {
    if (MR::isFirstStep(this)) {
        MR::startBck(this, "Wait", nullptr);
    }
}

void SurfRay::exeRideAccel() {
    if (MR::isFirstStep(this)) {
        MR::startBckPlayerJ("サーフィン（加速）");
        MR::startBck(this, "RunDash", nullptr);
    }

    if (!updateRideAccel() && isTwistStart()) {
        setNerve(&NrvSurfRay::SurfRayNrvRideAccelTwist::sInstance);
    }
}

void SurfRay::exeRideAccelTwist() {
    if (MR::isFirstStep(this)) {
        MR::startBckPlayerJ("サーフィン傾き開始（加速）");
    }

    if (!updateRideAccel() && MR::isBckStoppedPlayer()) {
        setNerve(&NrvSurfRay::SurfRayNrvRideAccel::sInstance);
    }
}

void SurfRay::exeRideFree() {
    if (MR::isFirstStep(this)) {
        MR::startBck(this, "Run", nullptr);
        MR::startBckPlayerJ("サーフィン");
    }

    if (!updateRideFree() && isTwistStart()) {
        setNerve(&NrvSurfRay::SurfRayNrvRideFreeTwist::sInstance);
    }
}

void SurfRay::exeRideFreeTwist() {
    if (MR::isFirstStep(this)) {
        MR::startBckPlayerJ("サーフィン傾き開始");
    }

    if (!updateRideFree() && MR::isBckStoppedPlayer()) {
        setNerve(&NrvSurfRay::SurfRayNrvRideFree::sInstance);
    }
}

void SurfRay::exeRideJump() {
    if (MR::isFirstStep(this)) {
        MR::startBckPlayerJ("サーフィンジャンプ");
        MR::startBck(this, "Jump", nullptr);
        MR::startSound(mRider, "SE_PV_JUMP_S", -1, -1);
    }

    if (!updateRide() && mVelocity.dot(mGravity) > 0.0f) {
        setNerve(&NrvSurfRay::SurfRayNrvRideFall::sInstance);
    }
}

void SurfRay::exeRideJumpHigh() {
    if (MR::isFirstStep(this)) {
        MR::startBckPlayerJ("サーフィンハイジャンプ");
        MR::startBck(this, "Jump", nullptr);
        MR::startSound(mRider, "SE_PV_JUMP_JOY", -1, -1);
    }

    if (!updateRide() && mVelocity.dot(mGravity) > 0.0f) {
        setNerve(&NrvSurfRay::SurfRayNrvRideFall::sInstance);
    }
}

void SurfRay::exeRideFall() {
    if (MR::isFirstStep(this)) {
        MR::startBckPlayerJ("サーフィン落下");
        MR::startBck(this, "Fall", nullptr);
    }

    if (!updateRide() && mVelocity.dot(mGravity) > 0.0f && mInWater) {
        setNerve(&NrvSurfRay::SurfRayNrvRideLand::sInstance);
        MR::startSound(this, "SE_SM_SURF_RAY_LANDW", -1, -1);
    }
}

void SurfRay::exeRideLand() {
    if (MR::isFirstStep(this)) {
        MR::startBckPlayerJ("サーフィン着地");
        MR::startBck(this, "Land", nullptr);
    }

    if (!updateRide() && !tryJumpOrFall() && MR::isBckStopped(this)) {
        setNerve(&NrvSurfRay::SurfRayNrvRideFree::sInstance);
    }
}

void SurfRay::exeTutorial() {
    TVec3f v1(mFront);
    updateRide();
    mOrthoVelocity.mult(0.5f);
    mFront.set(v1);

    mSurfSpeed = 0.0f;
    mRotation.z *= 0.85f;

    switch (mLectureIdx) {
    case 0:
        if (MR::isFirstStep(this)) {
            MR::startBckPlayerJ("サーフィン着地");
            MR::startBck(this, "Land", nullptr);
        }

        if (MR::isBckOneTimeAndStoppedPlayer()) {
            MR::startBckPlayer("SurfLectureUnbalance", static_cast<const char*>(nullptr));
        }

        if (MR::isBckOneTimeAndStopped(this)) {
            MR::startBck(this, "LectureUnbalance", nullptr);
        }
        break;
    case 2:
        if (MR::isFirstStep(this)) {
            MR::startBckPlayer("SurfLectureUnbalance", static_cast<const char*>(nullptr));
            MR::startBck(this, "LectureUnbalance", nullptr);
        }
        break;
    case 3:
        if (MR::isFirstStep(this)) {
            MR::startBckPlayer("SurfRide", static_cast<const char*>(nullptr));
            MR::startBck(this, "Wait", nullptr);
        }
        break;
    case 4:
    case 8:
    case 12:
        if (MR::isFirstStep(this)) {
            MR::startBckPlayer("SurfLectureAnswer", static_cast<const char*>(nullptr));
            MR::startBck(this, "LectureAnswer", nullptr);
            MR::startSound(this, "SE_SM_SURF_RAY_JUMPOUT1", -1, -1);
        }

        if (MR::isBckOneTimeAndStoppedPlayer()) {
            MR::startBckPlayer("SurfRideLoop", static_cast<const char*>(nullptr));
        }

        if (MR::isBckOneTimeAndStopped(this)) {
            MR::startBck(this, "Wait", nullptr);
        }
        break;
    case 5:
    case 6:
    case 9:
    case 10:
        if (MR::isFirstStep(this)) {
            MR::startBckPlayer("SurfRideLoop", static_cast<const char*>(nullptr));
            MR::startBck(this, "Wait", nullptr);
        }
        break;
    case 7:
    case 11:
        if (MR::isFirstStep(this)) {
            MR::startBckPlayerJ("サーフィン傾き開始");
        }

        f32 rot = __fabsf((mRotation.z / 70.0f) * 0.6f);

        if (mRotation.z < 0.0f) {
            MR::setBckBlendWeight(1.0f - rot, rot, 0.0f);
        } else {
            MR::setBckBlendWeight(1.0f - rot, 0.0f, rot);
        }
        break;
    case 13:
        break;
    default:
        break;
    }
}

void SurfRay::exeWipeOut() {
    if (MR::isFirstStep(this)) {
        MR::closeWipeCircle(-1);
    }

    if (!updateRide() && !MR::isWipeActive()) {
        MR::resetPosition(this, "スタート位置（サーフィン）");
        resetAllInfo();
        MR::startBckPlayer("SurfRide", static_cast<s32>(0));
        MR::resetCameraMan();
        setNerve(&NrvSurfRay::SurfRayNrvWipeIn::sInstance);
    }
}

void SurfRay::exeWipeIn() {
    if (MR::isFirstStep(this)) {
        MR::openWipeCircle(-1);
    }

    if (!MR::isWipeActive()) {
        setNerve(&NrvSurfRay::SurfRayNrvRideFree::sInstance);
    }
}

void SurfRay::exeReady() {
    if (MR::isFirstStep(this)) {
        MR::endStartPosCamera();
        MR::startBck(this, "WaitRaceStart", nullptr);
        MR::startBckPlayer("SurfRide", static_cast<s32>(0));
        MR::tryEmitEffect(this, "Ripple");
        MR::tryDeleteEffect(this, "RunDashSplash");
        MR::tryDeleteEffect(this, "SwimSplash");
    }

    MR::calcGravity(this);
    updateToWater();
    updateInfo();
    updateToMap();
    updateSound();
    updateRiderMtx();
    mOrthoVelocity.mult(0.5f);
    mSurfSpeed = 0.0f;
    mRotation.z = 0.0f;
}

void SurfRay::control() {
    mVelocity.set(mFront);
    mVelocity.scale(mSurfSpeed);
    mVelocity.add(mOrthoVelocity);
    f32 rate = min(mSurfSpeed * sBtkSpeedRate + sBtkSpeedMin, sBtkSpeedMax);
    MR::setBtkRate(this, rate);
    mActorJointCtrl->update();
}

void SurfRay::calcAndSetBaseMtx() {
    PSVECCrossProduct(&mBaseUp, &mFront, &mBaseSide);
    MR::normalize(&mBaseSide);
    PSVECCrossProduct(&mFront, &mBaseSide, &mBaseUp);
    MR::normalize(&mBaseUp);
    mUp.set(mBaseUp);
    MR::rotateVecDegree(&mUp, mFront, mRotation.z);
    PSVECCrossProduct(&mUp, &mFront, &mSide);
    MR::normalize(&mSide);
    PSVECCrossProduct(&mFront, &mSide, &mUp);
    MR::normalize(&mUp);

    TPos3f mtx;
    mtx.identity();
    // these are definitely some inlines...
    mtx[0][0] = mSide.x;
    mtx[1][0] = mSide.y;
    mtx[2][0] = mSide.z;
    mtx[0][1] = mUp.x;
    mtx[1][1] = mUp.y;
    mtx[2][1] = mUp.z;
    mtx[0][2] = mFront.x;
    mtx[1][2] = mFront.y;
    mtx[2][2] = mFront.z;
    mtx[0][3] = mPosition.x;
    mtx[1][3] = mPosition.y;
    mtx[2][3] = mPosition.z;

    MR::setBaseTRMtx(this, mtx);
    mActorJointCtrl->setCallBackFunction();
    mProjmapFxMtxSetter->updateMtxUseBaseMtx();
}

void SurfRay::attackSensor(HitSensor* pSender, HitSensor* pReceiver) {
    MR::tryGetItem(pSender, pReceiver);
}

bool SurfRay::receiveOtherMsg(u32 msg, HitSensor* pSender, HitSensor* pReceiver) {
    if (MR::isMsgAutoRushBegin(msg)) {
        if (MR::getPlayerVelocity()->y > 0.0f) {  // inline MarioAccess::getVelocity ??
            return false;
        }

        mRider = MR::getSensorHost(pSender);
        MR::startStarPointerMode1PInvalid2PValid(this);
        MR::invalidateClipping(this);
        mRayTilt = 0.0f;

        if (MR::isValidSwitchA(this)) {
            MR::onSwitchA(this);
        }

        MR::startBckNoInterpole(this, "Wait");
        MR::startBckPlayer("SurfRideLoop", static_cast<s32>(0));
        MR::calcAnimDirect(this);
        mActorJointCtrl->resetDynamicCtrl();

        if (mInTutorial) {
            setNerve(&NrvSurfRay::SurfRayNrvTutorial::sInstance);
        } else {
            setNerve(&NrvSurfRay::SurfRayNrvReady::sInstance);
        }

        return true;
    }

    if (MR::isMsgUpdateBaseMtx(msg)) {
        updateRiderMtx();
        return true;
    }

    if (MR::isMsgRushCancel(msg)) {
        mRider = nullptr;
        setNerve(&NrvSurfRay::SurfRayNrvRideFree::sInstance);
        return true;
    }

    if (MR::isMsgTutorialStart(msg)) {
        MR::setPlayerPos(mPosition);
        mInTutorial = true;
        return true;
    }

    if (MR::isMsgTutorialNext(msg)) {
        mLectureIdx++;
        setNerve(&NrvSurfRay::SurfRayNrvTutorial::sInstance);
        return true;
    }

    if (MR::isMsgTutorialPrev(msg)) {
        mLectureIdx--;
        setNerve(&NrvSurfRay::SurfRayNrvTutorial::sInstance);
        return true;
    }

    if (MR::isMsgTutorialPass(msg)) {
        mInTutorial = false;
        setNerve(&NrvSurfRay::SurfRayNrvRideFree::sInstance);
        return true;
    }

    if (MR::isMsgTutorialOmit(msg)) {
        mInTutorialArea = false;
        MR::resetPosition(this, "スタート位置（サーフィン）");
        resetAllInfo();
        MR::setPlayerPos(mPosition);
        MR::startBckPlayer("SurfRide", static_cast<s32>(0));
        MR::resetCameraMan();
        return true;
    }

    if (MR::isMsgRaceReady(msg)) {
        setNerve(&NrvSurfRay::SurfRayNrvReady::sInstance);
        return true;
    }

    if (MR::isMsgRaceStart(msg)) {
        setNerve(&NrvSurfRay::SurfRayNrvRideFree::sInstance);
        return true;
    }

    if (MR::isMsgRaceReset(msg)) {
        MR::endBindAndPlayerWait(this);
        resetAllInfo();
        setNerve(&NrvSurfRay::SurfRayNrvWait::sInstance);
        MR::endStarPointerMode(this);
        kill();
        return true;
    }

    return false;
}

void SurfRay::resetAllInfo() {
    TRot3f rotMtx;
    rotMtx.identity();
    MR::makeMtxRotate(rotMtx, mRotation);

    rotMtx.getXDirInline(mSide);
    rotMtx.getYDirInline(mUp);
    rotMtx.getZDirInline(mFront);

    mBaseUp.set(mUp);
    mBaseSide.set(mSide);

    mRotation.zero();
    mVelocity.zero();
    mOrthoVelocity.zero();

    mInWater = false;
    mSurfSpeed = 0.0f;
    mOrthoSpeed = 0.0f;
    mSteerRate = 0.0f;
    mSteerAccel = 0.0f;
    mAirTime = 0;

    MR::tryDeleteEffect(this, "SwimSplash");
    MR::tryDeleteEffect(this, "RunDashSplash");
    MR::tryDeleteEffect(this, "Ripple");
}

bool SurfRay::updateRideAccel() {
    if (updateRide()) {
        return true;
    }

    if (tryJumpOrFall()) {
        return true;
    }

    if (!MR::testCorePadButtonA(mPadChannel)) {
        setNerve(&NrvSurfRay::SurfRayNrvRideFree::sInstance);
        return true;
    }

    return false;
}

bool SurfRay::updateRideFree() {
    if (updateRide()) {
        return true;
    }

    if (tryJumpOrFall()) {
        return true;
    }

    if (MR::testCorePadButtonA(mPadChannel)) {
        setNerve(&NrvSurfRay::SurfRayNrvRideAccel::sInstance);
        return true;
    }

    return false;
}

bool SurfRay::updateRide() {
    if (mInTutorialArea && mPosition.z > mWarpPos.z) {
        mInTutorialArea = false;
        setNerve(&NrvSurfRay::SurfRayNrvWipeOut::sInstance);
        return true;
    }

    MR::calcGravity(this);
    updateToWater();
    updateInfo();
    updateToMap();
    updateAccel();
    updateSound();
    updateRotate();

    return false;
}

void SurfRay::updateInfo() {
    if (MR::isBindedGround(this)) {
        f32 sfxLvl = MR::getLinerValueFromMinMax(mSurfSpeed, 0.0f, 40.0f, 100.0f, 1000.0f);

        MR::startLevelSound(this, "SE_SM_LV_SURF_RAY_LAND", sfxLvl, -1, -1);

        if (MR::reboundVelocityFromCollision(this, 0.3f, 10.0f, 1.0f) && mVelocity.length() > sSpeedToRumble) {
            MR::tryRumblePadMiddle(this, mPadChannel);
        }
    }

    if (MR::isBindedWall(this)) {
        if (mVelocity.dot(*MR::getWallNormal(this)) < -10.0f) {
            MR::startSound(this, "SE_SM_SURF_RAY_HIT_WALL", -1, -1);
        }

        if (MR::reboundVelocityFromCollision(this, 0.8f, 10.0f, 1.0f) && mVelocity.length() > sSpeedToRumble) {
            MR::tryRumblePadMiddle(this, mPadChannel);
        }
    }

    mSurfSpeed = mFront.dot(mVelocity);
    TVec3f proj(mFront);
    proj.scale(mSurfSpeed);
    mOrthoVelocity.set(mVelocity);
    mOrthoVelocity.sub(proj);
    mOrthoSpeed = mOrthoVelocity.length();
}

void SurfRay::updateToMap() {
    TVec3f v1(mGravity);

    if (mInWater) {
        v1.scale(0.1f);
    } else {
        v1.scale(0.8f);
    }

    mOrthoVelocity.add(v1);

    if (MR::isBinded(this)) {
        if (MR::isBindedGround(this)) {
            mGroundNormal.set(*MR::getBindedNormal(this));
            MR::vecBlendSphere(mBaseUp, mGroundNormal, &mBaseUp, sBlendRatioAxisUpGround);
        }

        if (MR::isBindedWall(this)) {
            TVec3f v2(mVelocity);
            MR::normalize(&v2);
            MR::turnVecToPlane(&v2, v2, *MR::getWallNormal(this));
            MR::turnVecToPlane(&v2, v2, mGravity);
            TVec3f v3;
            MR::vecBlend(mFront, v2, &v3, sBlendRatioAxisFrontWall);
            PSVECCrossProduct(&mBaseUp, &v3, &mBaseSide);
        }
    } else if (mInWater) {
        MR::vecBlendSphere(mBaseUp, mWaterNormal, &mBaseUp, sBlendRatioAxisUpGround);
    } else {
        TVec3f v4(mGravity);
        v4.scale(-1.0f);
        MR::vecBlendSphere(mBaseUp, v4, &mBaseUp, sBlendRatioAxisUpAir);
    }

    PSVECCrossProduct(&mBaseSide, &mBaseUp, &mFront);
    MR::normalize(&mFront);
}

void SurfRay::updateToWater() {
    if (tryInWater()) {
        if (!mInWater) {
            f32 lvl = MR::getLinerValueFromMinMax(mOrthoVelocity.length(), 0.0f, 20.0f, 100.0f, 1000.0f);

            if (mAirTime > 30) {
                MR::tryRumblePadMiddle(this, mPadChannel);
            }

            if (mAirTime > 80 && mOrthoVelocity.length() > 20.0f) {
                MR::startSound(this, "SE_SM_SURF_RAY_JUMPIN2", -1, -1);
            } else if (mAirTime > 5 && mOrthoVelocity.length() > 10.0f) {
                MR::startSound(this, "SE_SM_SURF_RAY_JUMPIN1", lvl, -1);
            } else {
                MR::startSound(this, "SE_SM_SURF_RAY_JUMPIN0", lvl, -1);
            }
        }

        mInWater = true;
        mAirTime = 0;

        if (mVelocity.length() > sSpeedToRumble) {
            MR::tryRumblePadWeak(this, mPadChannel);
        }

        MR::getWaterAreaInfo(&mWaterInfo, mPosition, mGravity, false);
        mWaterNormal.set(mWaterInfo.mSurfaceNormal);
        f32 innerProd = mGravity.dot(mVelocity);

        if (innerProd > 0.0f && mWaterInfo._4 < innerProd) {
            TVec3f v2(mGravity);
            mVelocity.sub(mGravity.scaleInline(innerProd).scaleInline(1.5f));
        } else {
            f32 camWaterDepth = mWaterInfo.mCamWaterDepth;
            f32 waveHeight = mWaterInfo.mWaveHeight;
            f32 f0 = camWaterDepth + waveHeight;
            mVelocity.sub(mGravity.scaleInline(f0).scaleInline(0.01f));
        }
    } else {
        if (mInWater) {
            f32 lvl = MR::getLinerValueFromMinMax(mSurfSpeed, 0.0f, 30.0f, 200.0f, 1000.0f);
            MR::startSound(this, "SE_SM_SURF_RAY_JUMPOUT1", lvl, -1);
            mInWater = false;
        }

        mAirTime++;
    }

    WaterInfo waterInfo;
    TVec3f v(mPosition.addOperatorInLine(mGravity.scaleInline(20.0f)));

    if (MR::getWaterAreaObj(&waterInfo, v)) {
        mShadowAlpha -= sShadowAlphaSpeed;
    } else if (calcWaterShadowPos(&mWaterShadowPos, mPosition, mGravity)) {
        mShadowAlpha += sShadowAlphaSpeed;
    } else {
        mShadowAlpha -= sShadowAlphaSpeed;
    }

    mShadowAlpha = MR::clamp(mShadowAlpha, 0.0f, sShadowAlphaMax);
    MR::setShadowSurfaceOvalAlpha(this, 0, (u8)mShadowAlpha & 0xff);
}

void SurfRay::updateAccel() {
    if (MR::testCorePadButtonA(mPadChannel)) {
        mSurfSpeed += 0.5f;
    } else {
        mSurfSpeed *= 0.99f;
    }

    mSurfSpeed = MR::clamp(mSurfSpeed, 0.0f, 40.0f);

    if (!mWaterInfo.isInWater()) {
        MR::tryDeleteEffect(this, "SwimSplash");
        MR::tryDeleteEffect(this, "RunDashSplash");
        MR::tryDeleteEffect(this, "Ripple");
    } else if (mSurfSpeed > sSpeedToEmitEffectFast) {
        MR::tryEmitEffect(this, "RunDashSplash");
        MR::tryDeleteEffect(this, "SwimSplash");
        MR::tryDeleteEffect(this, "Ripple");
    } else if (mSurfSpeed > sSpeedToEmitEffect) {
        MR::tryEmitEffect(this, "SwimSplash");
        MR::tryDeleteEffect(this, "RunDashSplash");
        MR::tryDeleteEffect(this, "Ripple");
    } else {
        MR::tryEmitEffect(this, "Ripple");
        MR::tryDeleteEffect(this, "RunDashSplash");
        MR::tryDeleteEffect(this, "SwimSplash");
        MR::makeMtxUpFrontPos(&mEffectHostMtx, -mGravity, mFront, mWaterInfo.mSurfacePos);
    }

    if (MR::isBinded(this)) {
        mOrthoVelocity.mult(0.9f);
    } else {
        mOrthoVelocity.mult(0.995f);
    }

    if (MR::isNearZero(mOrthoVelocity, 0.1f)) {
        mOrthoVelocity.zero();
    }

    if (mWaterInfo.isInWater() && !isNerve(&NrvSurfRay::SurfRayNrvRideJump::sInstance)) {
        mOrthoVelocity.mult(0.95f);
    }
}

void SurfRay::updateRotate() {
    f32 steerAccel;
    f32 accelX = 0.0f;

    if (isRotateStart()) {
        TVec3f accel(0.0f, 0.0f, 0.0f);
        MR::getCorePadAcceleration(&accel, mPadChannel);

        if (accel.z > -0.7f || __fabsf(accel.x) > 0.15f) {
            f32 twistAngle = accel.x;
            f32 accelZ = MR::clamp(-accel.z, 0.0f, 1.0f);

            if (twistAngle > 0.0f) {
                twistAngle += accelZ;
            } else if (twistAngle < 0.0f) {
                twistAngle -= accelZ;
            }

            steerAccel = MR::clamp(twistAngle, -1.5f, 1.5f);
            accelX = accel.x;

            if (accelX < 0.0f) {
                f32 tiltClamp = MR::clamp(accelX, -1.0f, 0.0f);
                mRayTilt = MR::getInterpolateValue(0.3f, mRayTilt, tiltClamp);
            } else if (accelX > 0.0f) {
                f32 tiltClamp = MR::clamp(accelX, 0.0f, 1.0f);
                mRayTilt = MR::getInterpolateValue(0.3f, mRayTilt, tiltClamp);
            }
        } else {
            mRayTilt = MR::getInterpolateValue(0.3f, mRayTilt, 0.0f);
            steerAccel = mSteerAccel * 0.95f;
        }

        mSteerAccel = steerAccel;
        mSteerRate += steerAccel;
        mSteerRate = MR::clamp(mSteerRate, -30.0f, 30.0f);
    } else {
        mRayTilt = MR::getInterpolateValue(0.3f, mRayTilt, 0.0f);
        mSteerAccel = 0.0f;
    }

    mSteerRate *= 0.95f;
    f32 abs = __fabsf(mSteerRate);

    if (abs < 0.1f) {
        mSteerRate = 0.0f;
    }

    mRotation.z *= 0.7f;
    mRotation.z += mSteerRate;
    mRotation.z = MR::clamp(mRotation.z, -70.0f, 70.0f);

    f32 rayTilt = MR::clamp(mRayTilt * 1.2f, -1.0f, 1.0f);

    if (rayTilt > 0.0f) {
        MR::setBckBlendWeight(1.0f - rayTilt, 0.0f, rayTilt);
    } else {
        MR::setBckBlendWeight(rayTilt + 1.0f, -rayTilt, 0.0f);
    }

    TRot3f mtx;
    mtx.identity();
    mtx.setRotateInline(mBaseUp, -mRotation.z * 0.00028f);
    mtx.mult(mFront, mFront);

    for (s32 i = mTwistBufferSize - 1; i >= 1; i--) {
        mTwistBuffer[i] = mTwistBuffer[i - 1];
    }

    mTwistBuffer[0] = accelX;
}

void SurfRay::updateSound() {
    f32 lvl;

    if (mSurfSpeed > sMoveSe0StartSpeed && mSurfSpeed < sMoveSe3StartSpeed) {
        if (mSurfSpeed < sMoveSe2StartSpeed) {
            lvl = MR::getLinerValueFromMinMax(mSurfSpeed, sMoveSe0StartSpeed, sMoveSe1StartSpeed, 0.1f, 1.0f);
        } else {
            lvl = MR::getLinerValueFromMinMax(mSurfSpeed, sMoveSe2StartSpeed, sMoveSe3StartSpeed, 1.0f, 0.0f);
        }

        MR::startLevelSound(this, "SE_SM_LV_SURF_RAY_MOVE0", lvl * 1000.0f, -1, -1);
    }

    if (mInWater) {
        if (mSurfSpeed > sMoveSe1StartSpeed) {
            lvl = MR::getLinerValueFromMinMax(mSurfSpeed, sMoveSe1StartSpeed, sMoveSe2StartSpeed, 0.1f, 1.0f);

            MR::startLevelSound(this, "SE_SM_LV_SURF_RAY_MOVE1", lvl * 1000.0f, -1, -1);
        }
    }

    if (mInWater) {
        if (mSurfSpeed > sMoveSe2StartSpeed) {
            lvl = MR::getLinerValueFromMinMax(mSurfSpeed, sMoveSe2StartSpeed, sMoveSe3StartSpeed, 0.1f, 1.0f);

            MR::startLevelSound(this, "SE_SM_LV_SURF_RAY_MOVE2", lvl * 1000.0f, -1, -1);
        }
    }

    if (mSurfSpeed > sMoveSe3StartSpeed) {
        lvl = MR::getLinerValueFromMinMax(mSurfSpeed, sMoveSe3StartSpeed, sMoveSe3MaxSpeed, 0.1f, 1.0f);

        if (mInWater) {
            MR::startLevelSound(this, "SE_SM_LV_SURF_RAY_MOVE3", lvl * 1000.0f, -1, -1);
        }
    }

    if (mVelocity.length() > sWindSeStartSpeed) {
        lvl = MR::getLinerValueFromMinMax(mVelocity.length(), sWindSeStartSpeed, sWindSeMaxSpeed, 0.1f, 1.0f);

        MR::startLevelSound(this, "SE_SM_SURF_RAY_MOVE_WIND", lvl * 1000.0f, -1, -1);
    }

    f32 rotateRate = __fabsf(mRotation.z) / 70.0f;

    if (mInWater && rotateRate > 0.45f) {
        lvl = MR::getLinerValueFromMinMax(mSurfSpeed, 0.0f, 25.0f, 100.0f, 1000.0f);

        MR::startLevelSound(this, "SE_SM_LV_SURF_RAY_CURVE", lvl, -1, -1);
    }
}

void SurfRay::updateRiderMtx() {
    MtxPtr mtx1 = MR::getJointMtx(this, "Center");
    TPos3f mtx;
    mtx.setInline(mtx1);

    mtx[0][3] += mtx1[0][1] * 55.0f + mtx1[0][2] * 12.0f;
    mtx[1][3] += mtx1[1][1] * 55.0f + mtx1[1][2] * 12.0f;
    mtx[2][3] += mtx1[2][1] * 55.0f + mtx1[2][2] * 12.0f;

    MR::setBaseTRMtx(mRider, mtx);
}

bool SurfRay::tryInWater() {
    if (MR::getWaterAreaObj(&mWaterInfo, mPosition)) {
        return true;
    }

    TVec3f v1(mFront.scaleInline(100.0f).addOperatorInLine(mPosition));
    if (MR::getWaterAreaObj(&mWaterInfo, v1)) {
        return true;
    }

    TVec3f v2(mUp.scaleInline(150.0f).addOperatorInLine(mPosition));
    if (MR::getWaterAreaObj(&mWaterInfo, v2)) {
        return true;
    }

    TVec3f v3(mFront);
    MR::rotateVecDegree(&v3, -mGravity, -90.0f);
    TVec3f v3a(v3.scaleInline(150.0f).addOperatorInLine(mPosition));
    if (MR::getWaterAreaObj(&mWaterInfo, v3a)) {
        return true;
    }

    TVec3f v4(mFront);
    MR::rotateVecDegree(&v4, -mGravity, 90.0f);
    TVec3f v4a(v4.scaleInline(150.0f).addOperatorInLine(mPosition));
    if (MR::getWaterAreaObj(&mWaterInfo, v4a)) {
        return true;
    }

    TVec3f v5(mSide.scaleInline(150.0f).addOperatorInLine(mPosition));
    if (MR::getWaterAreaObj(&mWaterInfo, v5)) {
        return true;
    }

    TVec3f v6((-mSide).scaleInline(150.0f).addOperatorInLine(mPosition));
    if (MR::getWaterAreaObj(&mWaterInfo, v6)) {
        return true;
    }

    return false;
}

bool SurfRay::tryJumpOrFall() {
    if (MR::isPadSwing(WPAD_CHAN0) && (MR::isBinded(this) || mWaterInfo.isInWater() || mAirTime < 15)) {
        f32 f0 = (mSurfSpeed / 40.0f) * (((70.0f - __fabsf(mRotation.z)) * 5.0f) / 70.0f) + 15.0f;
        TVec3f v1(mGroundNormal);

        if (mWaterInfo.isInWater()) {
            v1.set(mWaterInfo.mSurfaceNormal);
        }

        v1.scale(f0);

        if (mOrthoVelocity.dot(mGravity) > 0.0f) {
            MR::vecKillElement(mOrthoVelocity, mGravity, &mOrthoVelocity);
        }

        mOrthoVelocity.add(v1);

        if (mOrthoVelocity.length() > 25.0f) {
            MR::normalize(&mOrthoVelocity);
            mOrthoVelocity.mult(25.0f);
        }

        if (mOrthoVelocity.length() > 23.0f) {
            setNerve(&NrvSurfRay::SurfRayNrvRideJumpHigh::sInstance);
        } else {
            setNerve(&NrvSurfRay::SurfRayNrvRideJump::sInstance);
        }

        return true;
    }

    if (mAirTime > 45) {
        setNerve(&NrvSurfRay::SurfRayNrvRideFall::sInstance);
        return true;
    }

    return false;
}

bool SurfRay::isTwistStart() const {
    f32 maxTwist = 0.0f;

    for (s32 i = 1; i < mTwistBufferSize; i++) {
        f32 twistDiff = __fabsf(mTwistBuffer[0] - mTwistBuffer[i]);

        if (twistDiff > maxTwist) {
            maxTwist = twistDiff;
        }
    }

    return maxTwist > sTwistStartDiffMin;
}

bool SurfRay::isRotateStart() const {
    if (mInTutorial) {
        if (mLectureIdx == 7 || mLectureIdx == 11) {
            return true;
        }

        return false;
    }

    return mSurfSpeed > 3.0f || MR::testCorePadButtonA(mPadChannel);
}

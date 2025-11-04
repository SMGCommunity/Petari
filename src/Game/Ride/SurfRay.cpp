#include "Game/Ride/SurfRay.hpp"
#include "Game/LiveActor/MaterialCtrl.hpp"
#include "Game/LiveActor/ActorJointCtrl.hpp"


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
};

void unusedStrippedMatchsdata2() {
    // this is not a real function, this is just to match .sdata2 data order
    f32 _f00 = 1.0f;
    f32 _f01 = 0.0f;
    f32 _f02 = 0.5f;
    f32 _f03 = 3.0f;
    f32 _f04 = -1.0f;
}

inline f32 min(f32 a, f32 b) {
    if (a >= b) {
        return b;
    }
    return a;
}

namespace {
    bool calcWaterShadowPos(TVec3f* pShadowPos, const TVec3f& pStartPos, const TVec3f& pShadowDir) {
        pShadowPos->set(pStartPos);
        TVec3f sample(pStartPos);
        int count = 0;
        bool test;
        do {
            WaterInfo waterInfo = WaterInfo();
            if (MR::getWaterAreaObj(&waterInfo, sample)) {
                MR::getWaterAreaInfo(&waterInfo, sample, pShadowDir, false);
                pShadowPos->set(waterInfo.mSurfacePos);
                return true;
            }
            TVec3f step(pShadowDir);
            step.scale(100.0f);
            sample.add(step);
        } while (++count < 10);
        return false;
    }
}

SurfRay::SurfRay(const char* pName, s32 pChannel) :
    LiveActor(pName),
    mSurfSpeed(0.0f),
    mOrthoSpeed(0.0f),
    mOrthoVelocity(0.0f, 0.0f, 0.0f),
    mSteerRate(0.0f),
    mSteerAccel(0.0f),
    mFront(0.0f, 0.0f, 1.0f),
    mUp(0.0f, 1.0f, 0.0f),
    mSide(1.0f, 0.0f, 0.0f),
    mBaseUp(0.0f, 1.0f, 0.0f),
    mBaseSide(1.0f, 0.0f, 0.0f),
    mGroundNormal(0.0f, 1.0f, 0.0f),
    mWarpPos(0.0f, 0.0f, 0.0f),
    mRider(nullptr),
    mChannel(pChannel),
    mRayTilt(0.0f),
    mInWater(false),
    mAirtime(0),
    mWaterNormal(0.0f, 1.0f, 0.0f),
    mTwistBufferSize(8),
    mWaterShadowPos(0.0f, 0.0f, 0.0f),
    mShadowAlpha(0.0f),
    mTwistBuffer(nullptr),
    mInTutorialArea(false),
    mInTutorial(false),
    mLectureIdx(0),
    mActorJointCtrl(nullptr),
    mProjmapFxMtxSetter(nullptr)
{
    mEffectHostMtx.identity();
}

void SurfRay::init(const JMapInfoIter& pMapInfoIter) {
    if (MR::hasRetryGalaxySequence()) {
        MR::resetPosition(this, "スタート位置（サーフィン）");
    }
    else {
        MR::initDefaultPos(this, pMapInfoIter);
    }

    resetAllInfo();
    initModelManagerWithAnm("SurfRay", 0, this != nullptr);
    
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
    initEffectKeeper(0, 0, false);
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
    if (!MR::hasRetryGalaxySequence()){
        mInTutorialArea = true;
    }

    MR::invalidateClipping(this);
    makeActorAppeared();
}

void SurfRay::exeWaitPlayer() {
    if (MR::isFirstStep(this)) {
        MR::startBck(this, "FirstWait", 0);
    }

    calcWaterShadowPos(&mWaterShadowPos, mPosition, mGravity);

    if (MR::isNearPlayer(this, 1300.0f)) {
        setNerve(&NrvSurfRay::SurfRayNrvTurnToWait::sInstance);
    }
}

void SurfRay::exeTurnToWait() {
    if (MR::isFirstStep(this)) {
        MR::startBck(this, "FirstWaitTurn", 0);
        MR::startSound(this, "SE_SM_SURF_RAY_TURNTO_MR", -1, -1);
    }

    if (MR::isBckStopped(this)) {
        setNerve(&NrvSurfRay::SurfRayNrvWait::sInstance);
    }
}

// for some reason this function doesnt seem to "exist"
// probably due to a compiler optimization? This gets
// automatically inlined into NrvSurfRay::SurfRayNrvWait::execute
// but is necessary to be here because of the position of "Wait"
// in .data ... what to do...
void SurfRay::exeWait() {
    if (MR::isFirstStep(this)) {
        MR::startBck(this, "Wait", 0);
    }
}

void SurfRay::exeRideAccel() {
    if (MR::isFirstStep(this)) {
        MR::startBckPlayerJ("サーフィン（加速）");
        MR::startBck(this, "RunDash", 0);
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
        MR::startBck(this, "Run", 0);
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
        MR::startBck(this, "Jump", 0);
        MR::startSound(mRider,"SE_PV_JUMP_S",-1,-1);
    }

    if (updateRide() && mVelocity.dot(mGravity) > 0.0f) {
        setNerve(&NrvSurfRay::SurfRayNrvRideFall::sInstance);
    }
}

void SurfRay::exeRideJumpHigh() {
    if (MR::isFirstStep(this)) {
        MR::startBckPlayerJ("サーフィンハイジャンプ");
        MR::startBck(this, "Jump", 0);
        MR::startSound(mRider,"SE_PV_JUMP_JOY",-1,-1);
    }

    if (updateRide() && mVelocity.dot(mGravity) > 0.0f) {
        setNerve(&NrvSurfRay::SurfRayNrvRideFall::sInstance);
    }
}

void SurfRay::exeRideFall() {
    if (MR::isFirstStep(this)) {
        MR::startBckPlayerJ("サーフィン落下");
        MR::startBck(this, "Fall", 0);
    }

    if (updateRide() && mVelocity.dot(mGravity) > 0.0f && mInWater) {
        setNerve(&NrvSurfRay::SurfRayNrvRideLand::sInstance);
        MR::startSound(this,"SE_SM_SURF_RAY_LANDW",-1,-1);
    }
}

void SurfRay::exeRideLand() {
    if (MR::isFirstStep(this)) {
        MR::startBckPlayerJ("サーフィン着地");
        MR::startBck(this, "Land", 0);
    }

    if (updateRide() && !tryJumpOrFall() && MR::isBckStopped(this)) {
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
            MR::startBck(this, "Land", 0);
        }
        if (MR::isBckOneTimeAndStoppedPlayer()){
            MR::startBckPlayer("SurfLectureUnbalance", (const char *)0);
        }
        if (MR::isBckOneTimeAndStopped(this)){
            MR::startBck(this, "LectureUnbalance", 0);
        }
        break;
    case 2:
        if (MR::isFirstStep(this)) {
            MR::startBckPlayer("SurfLectureUnbalance", (const char *)0);
            MR::startBck(this, "LectureUnbalance", 0);
        }
        break;
    case 3:
        if (MR::isFirstStep(this)) {
            MR::startBckPlayer("SurfRide", (const char *)0);
            MR::startBck(this, "Wait", 0);
        }
        break;
    case 4:
    case 8:
    case 12:
        if (MR::isFirstStep(this)) {
            MR::startBckPlayer("SurfLectureAnswer", (const char *)0);
            MR::startBck(this, "LectureAnswer", 0);
            MR::startSound(this,"SE_SM_SURF_RAY_JUMPOUT1",-1,-1);
        }
        if (MR::isBckOneTimeAndStoppedPlayer()){
            MR::startBckPlayer("SurfRideLoop", (const char *)0);
        }
        if (MR::isBckOneTimeAndStopped(this)){
            MR::startBck(this, "Wait", 0);
        }
        break;
    case 5:
    case 6:
    case 9:
    case 10:
        if (MR::isFirstStep(this)) {
            MR::startBckPlayer("SurfRideLoop", (const char *)0);
            MR::startBck(this, "Wait", 0);
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
        }
        else {
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
        MR::startBckPlayer("SurfRide", (s32)0); // MarioAccess::changeAnimationE() ??
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
        MR::startBck(this, "WaitRaceStart", 0);
        MR::startBckPlayer("SurfRide", (s32)0); // MarioAccess::changeAnimationE() ??
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
    // this causes a single nonmatch in the order of a mr r3,r31 instruction 
    MR::setBtkRate(this, min(mSurfSpeed * 0.4f + 1.0f, 4.0f));
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
        if (MR::getPlayerVelocity()->y > 0.0f) { // inline MarioAccess::getVelocity ??
            return false;
        }
        else {
            mRider = MR::getSensorHost(pSender);
            MR::startStarPointerMode1PInvalid2PValid(this);
            MR::invalidateClipping(this);
            mRayTilt = 0.0f;
            if (MR::isValidSwitchA(this)) {
                MR::onSwitchA(this);
            }
            MR::startBckNoInterpole(this, "Wait");
            MR::startBckPlayer("SurfRideLoop", (s32)0); // MarioAccess::changeAnimationE() ??
            MR::calcAnimDirect(this);
            mActorJointCtrl->resetDynamicCtrl();
            if (mInTutorial) {
                setNerve(&NrvSurfRay::SurfRayNrvTutorial::sInstance);
            }
            else {
                setNerve(&NrvSurfRay::SurfRayNrvReady::sInstance);
            }
            return true;
        }
    }
    else if (MR::isMsgUpdateBaseMtx(msg)) {
        updateRiderMtx();
        return true;
    }
    else if (MR::isMsgRushCancel(msg)) {
        mRider = nullptr;
        setNerve(&NrvSurfRay::SurfRayNrvRideFree::sInstance);
        return true;
    }
    else if (MR::isMsgTutorialStart(msg)) {
        MR::setPlayerPos(mPosition);
        mInTutorial = true;
        return true;
    }
    else if (MR::isMsgTutorialNext(msg)) {
        mLectureIdx++;
        setNerve(&NrvSurfRay::SurfRayNrvTutorial::sInstance);
        return true;
    }
    else if (MR::isMsgTutorialPrev(msg)) {
        mLectureIdx--;
        setNerve(&NrvSurfRay::SurfRayNrvTutorial::sInstance);
        return true;
    }
    else if (MR::isMsgTutorialPass(msg)) {
        mInTutorial = false;
        setNerve(&NrvSurfRay::SurfRayNrvRideFree::sInstance);
        return true;
    }
    else if (MR::isMsgTutorialOmit(msg)) {
        mInTutorialArea = false;
        MR::resetPosition(this, "スタート位置（サーフィン）");
        resetAllInfo();
        MR::setPlayerPos(mPosition);
        MR::startBckPlayer("SurfRide", (s32)0); // MarioAccess::changeAnimationE() ??
        MR::resetCameraMan();
        return true;
    }
    else if (MR::isMsgRaceReady(msg)) {
        setNerve(&NrvSurfRay::SurfRayNrvReady::sInstance);
        return true;
    }
    else if (MR::isMsgRaceStart(msg)) {
        setNerve(&NrvSurfRay::SurfRayNrvRideFree::sInstance);
        return true;
    }
    else if (MR::isMsgRaceReset(msg)) {
        MR::endBindAndPlayerWait(this);
        resetAllInfo();
        setNerve(&NrvSurfRay::SurfRayNrvWait::sInstance);
        MR::endStarPointerMode(this);
        kill();
        return true;
    }
    else {
        return false;
    }
}

void SurfRay::resetAllInfo() {
    TRot3f rotMtx;
    rotMtx.identity();
    MR::makeMtxRotate(reinterpret_cast<MtxPtr>(&rotMtx),mRotation);
    
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
    mAirtime = 0;

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
    
    if (!MR::testCorePadButtonA(mChannel)) {
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

    if (MR::testCorePadButtonA(mChannel)) {
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
    else {
        MR::calcGravity(this);
        updateToWater();
        updateInfo();
        updateToMap();
        updateAccel();
        updateSound();
        updateRotate();
        return false;
    }
}

void SurfRay::updateInfo() {
    if (MR::isBindedGround(this)) {
        f32 sfxLvl = MR::getLinerValueFromMinMax(mSurfSpeed, 0.0f, 40.0f, 100.f, 1000.0f);
        MR::startLevelSound(this, "SE_SM_LV_SURF_RAY_LAND", sfxLvl, -1, -1);
        if (MR::reboundVelocityFromCollision(this, 0.3f, 10.0f, 1.0f) 
          && PSVECMag(&mVelocity) > 5.0f) {
            MR::tryRumblePadMiddle(this, mChannel);
        }
    }

    if (MR::isBindedWall(this)) {
        if (mVelocity.dot(*MR::getWallNormal(this)) < -10.0f) {
            MR::startSound(this,"SE_SM_SURF_RAY_HIT_WALL",-1,-1);
        }
        if (MR::reboundVelocityFromCollision(this, 0.8f, 10.0f, 1.0f) 
          && PSVECMag(&mVelocity) > 5.0f) {
            MR::tryRumblePadMiddle(this, mChannel);
        }
    }

    mSurfSpeed = mFront.dot(mVelocity);
    TVec3f proj(mFront);
    proj.scale(mSurfSpeed);
    mOrthoVelocity.set(mVelocity);
    mOrthoVelocity.sub(proj);
    mOrthoSpeed = PSVECMag(&mOrthoVelocity);
}

void SurfRay::updateToMap() {
    TVec3f v1(mGravity);
    if (mInWater) {
        v1.scale(0.1);
    }
    else {
        v1.scale(0.8);
    }
    mOrthoVelocity.add(v1);

    if (MR::isBinded(this)) {
        if (MR::isBindedGround(this)) {
            mGroundNormal.set(*MR::getBindedNormal(this));
            MR::vecBlendSphere(mBaseUp, mGroundNormal, &mBaseUp, 0.1f);
        }
        if (MR::isBindedWall(this)) {
            TVec3f v2(mVelocity);
            MR::normalize(&v2);
            MR::turnVecToPlane(&v2, v2, *MR::getWallNormal(this));
            MR::turnVecToPlane(&v2, v2, mGravity);
            TVec3f v3;
            MR::vecBlend(mFront, v2, &v3, 0.1);
            PSVECCrossProduct(&mBaseUp, &v3, &mBaseSide);
        }
    }
    else if (mInWater) {
        MR::vecBlendSphere(mBaseUp, mWaterNormal, &mBaseUp, 0.1f);
    }
    else {
        TVec3f v4(mGravity);
        v4.scale(-1.0f);
        MR::vecBlendSphere(mBaseUp, v4, &mBaseUp, 0.02f);
    }

    PSVECCrossProduct(&mBaseSide, &mBaseUp, &mFront);
    MR::normalize(&mFront);
}

void SurfRay::updateToWater() {
    if (tryInWater()) {
        if (!mInWater) {
            f32 lvl = MR::getLinerValueFromMinMax(PSVECMag(&mOrthoVelocity), 0.0f, 20.0f, 100.0f, 1000.0f);
            if (mAirtime > 30) {
                MR::tryRumblePadMiddle(this, mChannel);
            }

            if (mAirtime > 80 && PSVECMag(&mOrthoVelocity) > 20.0f) {
                MR::startSound(this, "SE_SM_SURF_RAY_JUMPIN2", -1, -1);
            }
            else if (mAirtime > 5 && PSVECMag(&mOrthoVelocity) > 10.0f) {
                MR::startSound(this, "SE_SM_SURF_RAY_JUMPIN1", lvl, -1);
            }
            else {
                MR::startSound(this, "SE_SM_SURF_RAY_JUMPIN0", lvl, -1);
            
            }

            mInWater = true;
            mAirtime = 0;

            if (PSVECMag(&mOrthoVelocity) > 5.0f) {
                MR::tryRumblePadWeak(this, mChannel);
            }

            MR::getWaterAreaInfo(&mWaterInfo, mPosition, mGravity, false);
            mWaterNormal.set(mWaterInfo.mSurfaceNormal);
            f32 innerProd = mGravity.dot(mVelocity);
            if (innerProd > 0.0f && mWaterInfo._4 < innerProd) {
                TVec3f v2(mGravity);
                TVec3f v2a(mGravity);
                v2a.scale(innerProd);
                TVec3f v2b(v2a);
                v2b.scale(1.5f);
                mVelocity.sub(v2b);
            }
            else {
                f32 f0 = mWaterInfo.mCamWaterDepth + mWaterInfo.mWaveHeight;
                TVec3f v1(mGravity);
                v1.scale(f0);
                TVec3f v1a(v1);
                v1a.scale(0.01f);
                mVelocity.sub(v1a);
            }
        }
    }
    else {
        if (mInWater) {
            f32 lvl = MR::getLinerValueFromMinMax(mSurfSpeed, 0.0f, 30.0f, 200.0f, 1000.0f);
            MR::startSound(this, "SE_SM_SURF_RAY_JUMPOUT1", lvl, -1);
            mInWater = false;
        }
        mAirtime++;
    }

    WaterInfo waterInfo;
    if (MR::getWaterAreaObj(&waterInfo, addFromPos(mGravity, 20.0f))) {
        mShadowAlpha -= 10.0f;
    }
    else if (calcWaterShadowPos(&mWaterShadowPos, mPosition, mGravity)) {
        mShadowAlpha += 10.0f;
    }
    else {
        mShadowAlpha -= 10.0f;
    }
    mShadowAlpha = MR::clamp(mShadowAlpha, 0.0f, 64.0f);
    MR::setShadowSurfaceOvalAlpha(this, 0, mShadowAlpha);
}

void SurfRay::updateAccel(){
    if (MR::testCorePadButtonA(mChannel)){
        mSurfSpeed += 0.5f;
    }
    else {
        mSurfSpeed *= 0.99f;
    }
    mSurfSpeed = MR::clamp(mSurfSpeed, 0.0f, 40.0f);

    if (!mWaterInfo.isInWater()) {
        MR::tryDeleteEffect(this, "SwimSplash");
        MR::tryDeleteEffect(this, "RunDashSplash");
        MR::tryDeleteEffect(this, "Ripple");
    }
    else if (mSurfSpeed > 20.0f) {
        MR::tryEmitEffect(this, "RunDashSplash");
        MR::tryDeleteEffect(this, "SwimSplash");
        MR::tryDeleteEffect(this, "Ripple");
    } 
    else if (mSurfSpeed > 6.0f) {
        MR::tryEmitEffect(this, "SwimSplash");
        MR::tryDeleteEffect(this, "RunDashSplash");
        MR::tryDeleteEffect(this, "Ripple");
    }
    else {
        MR::tryEmitEffect(this, "Ripple");
        MR::tryDeleteEffect(this, "RunDashSplash");
        MR::tryDeleteEffect(this, "SwimSplash");
        MR::makeMtxUpFrontPos(&mEffectHostMtx, -mGravity, mFront, mWaterInfo.mSurfacePos);
    }

    if (MR::isBinded(this)) {
        mOrthoVelocity.mult(0.9f);
    }
    else {
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
    f32 accel_x = 0.0f;
    if (isRotateStart()) {
        TVec3f accel(0.0f, 0.0f, 0.0f);
        MR::getCorePadAcceleration(&accel, mChannel);
        if (accel.z > -0.7f || __fabsf(accel.x) > 0.15f) {
            f32 twistAngle = accel.x;
            f32 clamp_z = MR::clamp(-accel.z, 0.0f, 1.0f);

            if (twistAngle > 0.0f) {
                twistAngle += clamp_z;
            }
            else if (twistAngle < 0.0f) {
                twistAngle -= clamp_z;
            }

            steerAccel = MR::clamp(twistAngle, -1.5f, 1.5f);
            accel_x = accel.x;
            
            if (accel_x < 0.0f) {
                f32 tiltClamp = MR::clamp(accel_x, -1.0f, 0.0f);
                mRayTilt = MR::getInterpolateValue(0.3f, mRayTilt, tiltClamp);
            }
            else if (accel_x > 0.0f) {
                f32 tiltClamp = MR::clamp(accel_x, 0.0f, 1.0f);
                mRayTilt = MR::getInterpolateValue(0.3f, mRayTilt, tiltClamp);
            }
            
        }
        else {
            mRayTilt = MR::getInterpolateValue(0.3f, mRayTilt, 0.0f);
            steerAccel = mSteerAccel * 0.95f;
        }

        mSteerAccel = steerAccel;
        mSteerRate += steerAccel;
        mSteerRate = MR::clamp(mSteerRate, -30.0f, 30.0f);
    }
    else {
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
    }
    else {
        MR::setBckBlendWeight(rayTilt + 1.0f, -rayTilt, 0.0f);
    }

    TRot3f mtx;
    mtx.identity();
    mtx.setRotateInline(mBaseUp, -mRotation.z * 0.00028f);
    mtx.mult(mFront, mFront);
    
    int idx = mTwistBufferSize - 1;
    while (idx >= 1) {
        mTwistBuffer[idx] = mTwistBuffer[idx-1];
        idx--;
    }
    mTwistBuffer[0] = accel_x;

}

void SurfRay::updateSound() {
    f32 lvl;
    if (mSurfSpeed > 2.5f && mSurfSpeed < 25.0f) {
        if (mSurfSpeed < 14.0f) {
            lvl = MR::getLinerValueFromMinMax(mSurfSpeed, 2.5f, 7.5f, 0.1f, 1.0f);
        }
        else {
            lvl = MR::getLinerValueFromMinMax(mSurfSpeed, 14.0f, 25.0f, 1.0f, 0.0f);
        }
        MR::startLevelSound(this, "SE_SM_LV_SURF_RAY_MOVE0", lvl * 1000.0f, -1, -1);
    }

    if (mInWater) {
        if (mSurfSpeed > 7.5f) {
            lvl = MR::getLinerValueFromMinMax(mSurfSpeed, 7.5f, 14.0f, 0.1f, 1.0f);
            MR::startLevelSound(this, "SE_SM_LV_SURF_RAY_MOVE1", lvl * 1000.0f, -1, -1);
        }
    }

    if (mInWater) {
        if (mSurfSpeed > 14.0f) {
            lvl = MR::getLinerValueFromMinMax(mSurfSpeed, 14.0f, 25.0f, 0.1f, 1.0f);
            MR::startLevelSound(this, "SE_SM_LV_SURF_RAY_MOVE2", lvl * 1000.0f, -1, -1);
        }
    }

    if (mSurfSpeed > 25.0f) {
        lvl = MR::getLinerValueFromMinMax(mSurfSpeed, 25.0f, 35.0f, 0.1f, 1.0f);
        if (mInWater) {
            MR::startLevelSound(this, "SE_SM_LV_SURF_RAY_MOVE3", lvl * 1000.0f, -1, -1);
        }
    }

    
    if (PSVECMag(&mVelocity) > 32.0f) {
        lvl = MR::getLinerValueFromMinMax(PSVECMag(&mVelocity), 32.0f, 50.0f, 0.1f, 1.0f);
        MR::startLevelSound(this, "SE_SM_SURF_RAY_MOVE_WIND", lvl * 1000.0f, -1, -1);
    }
    
    if (mInWater && (__fabsf(mRotation.z) / 70.0f) > 0.45f) {
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
    // nonmatching, improper stack access order
    
    if (MR::getWaterAreaObj(&mWaterInfo, mPosition) != nullptr) {
        return true;
    }

    if (MR::getWaterAreaObj(&mWaterInfo, addFromPos(mFront, 100.0f)) != nullptr) {
        return true;
    }
    
    if (MR::getWaterAreaObj(&mWaterInfo, addFromPos(mUp, 150.0f)) != nullptr) {
        return true;
    }

    TVec3f v3(mFront);
    MR::rotateVecDegree(&v3, -mGravity, -90.0f);
    if (MR::getWaterAreaObj(&mWaterInfo, addFromPos(v3, 150.0f)) != nullptr) {
        return true;
    }

    TVec3f v4(mFront);
    MR::rotateVecDegree(&v4, -mGravity, 90.0f);
    if (MR::getWaterAreaObj(&mWaterInfo, addFromPos(v4, 150.0f)) != nullptr) {
        return true;
    }

    if (MR::getWaterAreaObj(&mWaterInfo, addFromPos(mSide, 150.0f)) != nullptr) {
        return true;
    }

    // non matching at "else" condition here
    // impoproper register access between r3 and r4 here
    if (MR::getWaterAreaObj(&mWaterInfo, addFromPos(-mSide, 150.0f)) != nullptr) {
        return true;
    }

    return false;
}

bool SurfRay::tryJumpOrFall() {
    if (MR::isPadSwing(0) && (MR::isBinded(this) || mWaterInfo.isInWater() || mAirtime < 15)) {
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

        if (PSVECMag(&mOrthoVelocity) > 25.0f) {
            MR::normalize(&mOrthoVelocity);
            mOrthoVelocity.mult(25.0f);
        }

        if (PSVECMag(&mOrthoVelocity) > 23.0f) {
            setNerve(&NrvSurfRay::SurfRayNrvRideJumpHigh::sInstance);
        }
        else {
            setNerve(&NrvSurfRay::SurfRayNrvRideJump::sInstance);
        }
        return true;
    } 
    else if (mAirtime > 45) {
        setNerve(&NrvSurfRay::SurfRayNrvRideFall::sInstance);
        return true;
    }
    else {
        return false;
    }
}

bool SurfRay::isTwistStart() const {
    u32 idx = 1;
    f32 maxTwist = 0.0f;
    s32 cnt = mTwistBufferSize;

    while (cnt > 1) {
        f32 twistDiff = __fabsf(mTwistBuffer[0] - mTwistBuffer[idx]);
        if (twistDiff > maxTwist) {
            maxTwist = twistDiff;
        }
        idx++;
        cnt--;
    } 

    return maxTwist > 0.2f;
}

bool SurfRay::isRotateStart() const {
    // wow.. *this* matches...
    
    if (mInTutorial) {
        if (mLectureIdx == 7 || mLectureIdx == 11) {
            return true;
        }
        return false;
    }
    else {
        return mSurfSpeed > 3.0f || MR::testCorePadButtonA(mChannel);
    }
    
}

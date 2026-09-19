
#include "Game/Enemy/BegomanSpring.hpp"
#include "Game/Enemy/BegomanBaby.hpp"
#include "Game/Enemy/BegomanSpringHead.hpp"
#include "Game/LiveActor/ActorCameraInfo.hpp"
#include "Game/LiveActor/HitSensor.hpp"
#include "Game/LiveActor/Nerve.hpp"
#include "Game/Util/ActorMovementUtil.hpp"
#include "Game/Util/ActorSensorUtil.hpp"
#include "Game/Util/ActorShadowUtil.hpp"
#include "Game/Util/CameraUtil.hpp"
#include "Game/Util/DemoUtil.hpp"
#include "Game/Util/EffectUtil.hpp"
#include "Game/Util/GravityUtil.hpp"
#include "Game/Util/JMapUtil.hpp"
#include "Game/Util/JointUtil.hpp"
#include "Game/Util/LiveActorUtil.hpp"
#include "Game/Util/MapUtil.hpp"
#include "Game/Util/MathUtil.hpp"
#include "Game/Util/ModelUtil.hpp"
#include "Game/Util/MtxUtil.hpp"
#include "Game/Util/MultiEventCamera.hpp"
#include "Game/Util/ObjUtil.hpp"
#include "Game/Util/PlayerUtil.hpp"
#include "Game/Util/SoundUtil.hpp"
#include "Game/Util/StringUtil.hpp"
#include "JSystem/JMath/JMATrigonometric.hpp"

namespace NrvBegomanSpring {
    NEW_NERVE_ONEND(HostTypeNrvNoCalcWait, BegomanSpring, NoCalcWait, NoCalcWait);
    NEW_NERVE_ONEND(HostTypeNrvWait, BegomanSpring, Wait, Wait);
    NEW_NERVE(HostTypeNrvSignAttack, BegomanSpring, SignAttack);
    NEW_NERVE_ONEND(HostTypeNrvPursue, BegomanSpring, Pursue, Pursue);
    NEW_NERVE(HostTypeNrvTurn, BegomanSpring, Turn);
    NEW_NERVE(HostTypeNrvBrake, BegomanSpring, Brake);
    NEW_NERVE(HostTypeNrvStepBack, BegomanSpring, StepBack);
    NEW_NERVE(HostTypeNrvReturn, BegomanSpring, Return);
    NEW_NERVE(HostTypeNrvProvoke, BegomanSpring, Provoke);
    NEW_NERVE(HostTypeNrvShake, BegomanSpring, Shake);
    NEW_NERVE(HostTypeNrvTrample, BegomanSpring, Trample);
    NEW_NERVE(HostTypeNrvHitReaction, BegomanSpring, HitReaction);
    NEW_NERVE(HostTypeNrvTired, BegomanSpring, Tired);
    NEW_NERVE(HostTypeNrvBlow, BegomanSpring, Blow);
    NEW_NERVE(HostTypeNrvElectricDeath, BegomanSpring, ElectricDeath);
    NEW_NERVE_ONEND(HostTypeNrvHide, BegomanSpring, Hide, Hide);
    NEW_NERVE(HostTypeNrvHop, BegomanSpring, Hop);
    NEW_NERVE(HostTypeNrvInWater, BegomanSpring, InWater);
    NEW_NERVE(HostTypeNrvKeepDistance, BegomanSpring, KeepDistance);
    NEW_NERVE_ONEND(HostTypeNrvBindStarPointer, BegomanSpring, BindStarPointer, BindStarPointer);
}  // namespace NrvBegomanSpring

void BegomanSpring_FORCE_MATCH_SDATA2() {
    (void)1.0f;
    (void)0.0f;
    (void)0.5f;
    (void)2.0f;
}

namespace {
    const MR::ActorMoveParam hWaitParam = {0.0f, 3.0f, 0.95f, 1.0f};
    const MR::ActorMoveParam hSignAttackParam = {0.0f, 3.0f, 0.8f, 3.0f};
    const MR::ActorMoveParam hPursueParam = {0.4f, 3.0f, 0.97f, 3.0f};
    const MR::ActorMoveParam hTurnParam = {0.4f, 3.0f, 0.95f, 1.0f};
    const MR::ActorMoveParam hStopParam = {0.0f, 3.0f, 0.95f, 1.0f};
    const MR::ActorMoveParam hHitReactionParam = {0.0f, 3.0f, 0.95f, 1.0f};
    const MR::ActorMoveParam hTiredParam = {0.2f, 1.0f, 0.95f, 3.0f};
    // hTurnStartDegree
    // hTurnEndDegree
    // hIsFaceToPlayerDegree
    // sDiscoverRange
    // sPursueRange
    // sReboundRailRatio
    const f32 sReboundEnemyAddVel = 8.0f;
    const f32 sPreventSwingbyDamp = 0.7f;
    const f32 sPreventSlopOverDamp = 0.2f;
    const f32 sCommonAddRotate = 0.5f;
    const f32 sReboundPlayerAddVel = 2.0f;
    const f32 sCameraLimitLength = 2500;
    const f32 sCoinTransY = 80.0f;
    // sCheckBodySize
    const f32 sWaitRotate = 0.15f;
    const f32 sSignAttackRotate = 0.5f;
    // sPursueLimitTime
    // sAnimFrame
    // sDashFrame
    // sDashVelocity
    const f32 sPursueRotate = 0.5f;
    // sTurnTime
    const f32 sTurnRotate = 0.4f;
    // sBrakeTime
    const f32 sBrakeRotate = 0.2f;
    const f32 sProvokeRotate = 0.3f;
    const s32 sShakeTime = 180;
    const f32 sShakeRotate = 0.17f;
    const s32 sTrampleTime = 180;
    const f32 sTrampleRotate = 0.25f;
    const f32 sHitReactionRotate = 0.25f;
    const s32 sStopSceneTime = 2;
    const s32 sBlowFrame = 20;
    const f32 sBlowVel = 40.0f;
    const f32 sBlowVerticalVel = 10.0f;
    const s32 sDeathStopSceneTime = 5;
    const s32 sElectricDeathTime = 20;
    const f32 sBabyLaunchVelH = 10.0f;
    const f32 sBabyLaunchVelV = 50.0f;
    const f32 sHopInitRotate = 0.5f;
    const f32 sHopRotateBrakeTime = 44.0f;
    const s32 hHopStarPieceReflectTime = 40;
    const f32 sIsInWaterVel = 10.0f;
    const s32 sIsInWaterTime = 60;
    const f32 hKeepDistFar = 800.0f;
    const f32 hKeepDistNear = 600.0f;
}  // namespace

BegomanSpring::BegomanSpring(const char* pName)
    : BegomanBase(pName), mEventCamera(), mHead(), mCameraStartPos(0, 0, 0), mIsCameraActive(), mCameraInfo(), mType(), mBaby(), mHidePos(0, 0, 0) {
    mHeadMatrix.identity();
    mWaterColumnMatrix.identity();
}

void BegomanSpring::init(const JMapInfoIter& rIter) {
    initCore(rIter, "BegomanSpring", false);
    mHidePos.set(mPosition);
    const char* pObjectName = "\0";
    initUseSwitchB(rIter, MR::Functor(this, &BegomanSpring::kill));
    if (MR::isValidInfo(rIter)) {
        MR::getObjectName(&pObjectName, rIter);
        initEventCameras(rIter);
    }

    if (MR::isEqualString(pObjectName, "BegomanSpringHide")) {
        setName("隠れバネベーゴマン");
        mType = 1;
        mBaby = new BegomanBaby("隠れバネベーゴマンベビー");
        mBaby->mPosition.set(mPosition);
        mBaby->initWithoutIter();
        mBaby->makeActorDead();
    } else {
        mType = 0;
    }

    MR::declareCoin(this, 1);
    initBinder(100.0f, 100.0f, 0);
    switch (mType) {
    case 0:
        initNerve(GET_NERVE(BegomanSpring, HostTypeNrvWait));
        break;
    case 1:
        initNerve(GET_NERVE(BegomanSpring, HostTypeNrvHide));
        break;
    }

    initEffectAndStarPointerBind();
    MR::setEffectHostMtx(this, "WaterColumn", mWaterColumnMatrix);
    mHead = new BegomanSpringHead(this, mHeadMatrix);
    mHead->_9C = &_B4;
    mHead->initWithoutIter();
    initSensor(1, 70.0f, 140.0f, "Center");
    MR::addHitSensorMtx(this, "trample", ATYPE_PLAYER_AUTO_JUMP, 8, 130.0f * mScale.y, MR::getJointMtx(mHead, "SpringJoint5"),
                        TVec3f(0.0f, 0.0f, 0.0f));
    initSound(8, false);
    initShadow(70.0f, "Locator1");
    if (MR::tryRegisterDemoCast(this, rIter)) {
        MR::tryRegisterDemoCast(mHead, rIter);
    }

    if (MR::isDemoCast(this, nullptr) && MR::isRegisteredDemoActionAppear(this)) {
        makeActorDead();
    } else {
        makeActorAppeared();
    }
}

void BegomanSpring::initEventCameras(const JMapInfoIter& rIter) {
    mCameraInfo = new ActorCameraInfo(rIter);
    if (mCameraInfo->mCameraSetID != -1) {
        mEventCamera = new MultiEventCamera();
        mEventCamera->setUp(mName, mCameraInfo, 2);
        mEventCamera->setEndCameraTypeSoon();
    }
}

void BegomanSpring::appear() {
    BegomanBase::appear();
    MR::emitEffect(this, "Death");
    MR::startSound(this, "SE_EM_BEGOMAN_APPEAR");
    setNerve(GET_NERVE(BegomanSpring, HostTypeNrvWait));
    mHead->forceWaitImmediately();
}

void BegomanSpring::makeActorDead() {
    BegomanBase::makeActorDead();
    if (mIsCameraActive) {
        endCamera();
        mIsCameraActive = false;
    }
}

void BegomanSpring::kill() {
    BegomanBase::kill();
    MR::emitEffect(this, "Death");
    MR::startSound(this, "SE_EM_EXPLODE_S");
}

void BegomanSpring::control() {
    if (isNerve(GET_NERVE(BegomanSpring, HostTypeNrvPursue)) || isNerve(GET_NERVE(BegomanSpring, HostTypeNrvTurn))) {
        if (incAndCheckTiredCounter()) {
            setNerve(GET_NERVE(BegomanSpring, HostTypeNrvTired));
            return;
        }
    } else {
        mTiredCounter = 0;
    }

    mHead->mGravity.set(mGravity);
    if (isNerve(GET_NERVE(BegomanSpring, HostTypeNrvBlow))) {
        checkTouchElectricRail(true);
    } else if (!isNerve(GET_NERVE(BegomanSpring, HostTypeNrvElectricDeath))) {
        checkTouchElectricRail(false);
    }

    updateCamera();
    getSensor("check")->invalidate();
    if (mType == 1) {
        f32 radius = getSensor("body")->mRadius;
        if (MR::isJudgedToClipFrustum(mPosition, radius)) {
            f32 hideRadius = getSensor("body")->mRadius;
            if (MR::isJudgedToClipFrustum(mHidePos, hideRadius)) {
                mPosition.set(mHidePos);
                setNerve(GET_NERVE(BegomanSpring, HostTypeNrvHide));
                return;
            }
        }
    }

    if (!isNerve(GET_NERVE(BegomanSpring, HostTypeNrvInWater)) && !isNerve(GET_NERVE(BegomanSpring, HostTypeNrvNoCalcWait))) {
        if (isInWaterAndSetWaterNerve(GET_NERVE(BegomanSpring, HostTypeNrvInWater), &mWaterColumnMatrix)) {
            return;
        }
    }

    if (MR::isBindedGroundDamageFire(this)) {
        makeActorDead();
        MR::emitEffect(this, "Fall");
        return;
    }

    if (MR::isInDeath(this, TVec3f(0.0f, 0.0f, 0.0f))) {
        kill();
        return;
    }

    if (MR::isPressedRoofAndGround(this)) {
        kill();
        return;
    }

    if (isNerve(GET_NERVE(BegomanSpring, HostTypeNrvBlow))) {
        preventSwingby(sPreventSwingbyDamp);
        preventSlopeBlow(sPreventSlopOverDamp);
    }

    if ((!isNerve(GET_NERVE(BegomanSpring, HostTypeNrvNoCalcWait)) && !isNerve(GET_NERVE(BegomanSpring, HostTypeNrvWait)) &&
         !isNerve(GET_NERVE(BegomanSpring, HostTypeNrvSignAttack)) && !isNerve(GET_NERVE(BegomanSpring, HostTypeNrvPursue)) &&
         !isNerve(GET_NERVE(BegomanSpring, HostTypeNrvTurn)) && !isNerve(GET_NERVE(BegomanSpring, HostTypeNrvBrake)) &&
         !isNerve(GET_NERVE(BegomanSpring, HostTypeNrvReturn)) && !isNerve(GET_NERVE(BegomanSpring, HostTypeNrvProvoke)) &&
         !isNerve(GET_NERVE(BegomanSpring, HostTypeNrvKeepDistance))) ||
        !tryAndSetStarPointerBind(GET_NERVE(BegomanSpring, HostTypeNrvBindStarPointer))) {
        BegomanBase::control();
    }
}

bool BegomanSpring::onTouchElectric(const TVec3f& rPos, const TVec3f& rDirection) {
    if (isNerve(GET_NERVE(BegomanSpring, HostTypeNrvElectricDeath)) || isNerve(GET_NERVE(BegomanSpring, HostTypeNrvHide)) ||
        isNerve(GET_NERVE(BegomanSpring, HostTypeNrvHop)) || isNerve(GET_NERVE(BegomanSpring, HostTypeNrvInWater))) {
        return false;
    }

    if (isNerve(GET_NERVE(BegomanSpring, HostTypeNrvBlow))) {
        setNerve(GET_NERVE(BegomanSpring, HostTypeNrvElectricDeath));
        return false;
    }

    if (isNerve(GET_NERVE(BegomanSpring, HostTypeNrvStepBack)) && !MR::isOnGround(this)) {
        return false;
    }

    if (BegomanBase::onTouchElectric(rPos, rDirection)) {
        setNerve(GET_NERVE(BegomanSpring, HostTypeNrvStepBack));
        return true;
    }

    return false;
}

bool BegomanSpring::setNerveReturn() {
    setNerve(GET_NERVE(BegomanSpring, HostTypeNrvReturn));
    return true;
}

const Nerve* BegomanSpring::getNerveWait() {
    return GET_NERVE(BegomanSpring, HostTypeNrvWait);
}

void BegomanSpring::exeNoCalcWait() {
    exeNoCalcWaitCore(0.5f, GET_NERVE(BegomanSpring, HostTypeNrvWait));
}

void BegomanSpring::endNoCalcWait() {
    finishNoCalcWait();
}

void BegomanSpring::exeWait() {
    updateRotateY(sWaitRotate, sCommonAddRotate);
    MR::startLevelSound(this, "SE_EM_LV_BEGOMAN_ROT_SLOW");
    exeWaitCore(hWaitParam, GET_NERVE(BegomanSpring, HostTypeNrvSignAttack), GET_NERVE(BegomanSpring, HostTypeNrvKeepDistance),
                GET_NERVE(BegomanSpring, HostTypeNrvNoCalcWait));
}

void BegomanSpring::endWait() {
    MR::invalidateClipping(this);
}

void BegomanSpring::exeSignAttack() {
    if (MR::isFirstStep(this)) {
        MR::startBck(this, "SignAttack", nullptr);
        MR::startSound(this, "SE_EM_BEGOMAN_PRE_PURSUE");
    }

    updateRotateY(sSignAttackRotate, sCommonAddRotate);
    MR::startLevelSound(this, "SE_EM_LV_BEGOMAN_ROT_MIDDLE");
    exeSignAttackCore(hSignAttackParam, GET_NERVE(BegomanSpring, HostTypeNrvPursue));
}

void BegomanSpring::exePursue() {
    if (MR::isFirstStep(this)) {
        MR::emitEffect(this, "AttackBlur");
        MR::startSound(this, "SE_EM_BEGOMAN_PURSUE_START");
    }

    updateRotateY(sPursueRotate, sCommonAddRotate);
    MR::startLevelSound(this, "SE_EM_LV_BEGOMAN_PURSUE");
    exePursueCore(hPursueParam, GET_NERVE(BegomanSpring, HostTypeNrvBrake), GET_NERVE(BegomanSpring, HostTypeNrvTurn), *getSoundNormal(), 1.0f);
}

void BegomanSpring::endPursue() {
    MR::deleteEffect(this, "AttackBlur");
}

void BegomanSpring::exeTurn() {
    if (MR::isFirstStep(this)) {
        mHead->tryHopEnd();
    }

    updateRotateY(sTurnRotate, sCommonAddRotate);
    MR::startLevelSound(this, "SE_EM_LV_BEGOMAN_ROT_MIDDLE");
    MR::startLevelSound(this, "SE_EM_LV_BEGOMAN_TURN");
    exeTurnCore(hTurnParam, GET_NERVE(BegomanSpring, HostTypeNrvBrake), GET_NERVE(BegomanSpring, HostTypeNrvPursue), false);
}

void BegomanSpring::exeBrake() {
    updateRotateY(sBrakeRotate, sCommonAddRotate);
    MR::startLevelSound(this, "SE_EM_LV_BEGOMAN_SPARK");
    MR::startLevelSound(this, "SE_EM_LV_BEGOMAN_ROT_MIDDLE");
    exeBrakeCore(GET_NERVE(BegomanSpring, HostTypeNrvTurn));
}

void BegomanSpring::exeStepBack() {
    if (MR::isFirstStep(this)) {
        MR::startBck(this, "electricshock", nullptr);
    }

    updateRotateY(0.2f, sCommonAddRotate);
    exeStepBackCore(hWaitParam, GET_NERVE(BegomanSpring, HostTypeNrvWait));
}

void BegomanSpring::exeReturn() {
    updateRotateY(0.2f, sCommonAddRotate);
    exeReturnCore(GET_NERVE(BegomanSpring, HostTypeNrvWait));
}

void BegomanSpring::exeProvoke() {
    updateRotateY(sProvokeRotate, sCommonAddRotate);
    MR::startLevelSound(this, "SE_EM_LV_BEGOMAN_ROT_MIDDLE");
    exeProvokeCore(hWaitParam, GET_NERVE(BegomanSpring, HostTypeNrvSignAttack));
}

void BegomanSpring::exeShake() {
    if (MR::isFirstStep(this)) {
        MR::startBck(this, "Shake", nullptr);
        if (!mHead->isSpringHop()) {
            mHead->tryHopStart();
        }

        if (mType == 1 && MR::isDead(mBaby)) {
            mBaby->mPosition.set(mPosition);
            TVec3f side(mFaceVec);
            side.cross(mGravity, side);
            TVec3f& rVelocity = mBaby->mVelocity;
            rVelocity.set(-mGravity * sBabyLaunchVelV - side * sBabyLaunchVelH);
            mBaby->appear();
            MR::startSound(this, "SE_EM_BEGOMAN_LAUNCH_BABY");
        }
    }

    updateRotateY(sShakeRotate, sCommonAddRotate);
    MR::startLevelSound(this, "SE_EM_LV_BEGOMAN_SHAKE");
    MR::moveAndTurnToDirection(this, &mFaceVec, mTargetVec, hWaitParam._0, hWaitParam._4, hWaitParam._8, hWaitParam._C);
    reboundWallAndGround(&mFaceVec, false);

    const TVec3f& rGravity = mGravity;
    mVelocity.scale(rGravity.dot(mVelocity), rGravity);

    if (MR::isStep(this, sShakeTime - mHead->getHopEndBckFrameMax())) {
        MR::startBck(this, "HopEnd", nullptr);
        mHead->tryHopEnd();
    }

    if (MR::isGreaterStep(this, sShakeTime)) {
        setNerve(GET_NERVE(BegomanSpring, HostTypeNrvWait));
    }
}

void BegomanSpring::exeTrample() {
    if (MR::isFirstStep(this)) {
        MR::startBck(this, "Shake", nullptr);
        mHead->tryHopJump();
        MR::startSound(this, "SE_EM_BEGOMAN_TRAMPLE");
    }

    updateRotateY(sTrampleRotate, sCommonAddRotate);
    MR::moveAndTurnToDirection(this, &mFaceVec, mTargetVec, hStopParam._0, hStopParam._4, hStopParam._8, hStopParam._C);
    reboundWallAndGround(&mFaceVec, false);

    const TVec3f& rGravity = mGravity;
    mVelocity.scale(rGravity.dot(mVelocity), rGravity);

    if (MR::isStep(this, sTrampleTime - mHead->getHopEndBckFrameMax())) {
        MR::startBck(this, "HopEnd", nullptr);
        mHead->tryHopEnd();
    }

    if (MR::isGreaterStep(this, sTrampleTime)) {
        setNerve(GET_NERVE(BegomanSpring, HostTypeNrvSignAttack));
    }
}

void BegomanSpring::exeHitReaction() {
    if (MR::isFirstStep(this)) {
        mHead->tryHopEnd();
    }

    updateRotateY(sHitReactionRotate, sCommonAddRotate);
    exeHitReactionCore(hHitReactionParam, GET_NERVE(BegomanSpring, HostTypeNrvProvoke));
}

void BegomanSpring::exeTired() {
    if (MR::isFirstStep(this)) {
    }

    updateRotateY(0.2f, sCommonAddRotate);
    MR::startLevelSound(this, "SE_EM_LV_BEGOMAN_TURN");
    exeTiredCore(hTiredParam, GET_NERVE(BegomanSpring, HostTypeNrvWait));
}

void BegomanSpring::exeBlow() {
    if (MR::isFirstStep(this)) {
        MR::startSound(this, "SE_EM_BEGOMAN_ROT_STOP");
        MR::startBck(this, "Damage", nullptr);
        MR::stopScene(sStopSceneTime);
        mHead->tryHopStart();
    }

    MR::startLevelSound(this, "SE_EM_LV_BEGOMAN_SPARK");
    MR::startLevelSound(this, "SE_EM_LV_BEGOMAN_ROT_MIDDLE");
    MR::moveAndTurnToDirection(this, &mFaceVec, mTargetVec, hHitReactionParam._0, hHitReactionParam._4, hHitReactionParam._8, hHitReactionParam._C);
    reboundWallAndGround(&mFaceVec, false);
    if (MR::isStep(this, sBlowFrame - mHead->getHopEndBckFrameMax())) {
        MR::startBck(this, "HopEnd", nullptr);
        mHead->tryHopEnd();
    }

    if (MR::isGreaterStep(this, sBlowFrame) && MR::isOnGround(this)) {
        MR::startBck(this, "Turn", nullptr);
        setNerve(GET_NERVE(BegomanSpring, HostTypeNrvShake));
    }
}

void BegomanSpring::exeElectricDeath() {
    if (MR::isFirstStep(this)) {
        MR::startBck(this, "electricshock", nullptr);
        mVelocity.zero();
        getSensor("body")->invalidate();
        MR::startSound(this, "SE_EM_BEGOMAN_ELEC_DAMAGE");
        MR::invalidateClipping(this);
    }

    if (MR::isGreaterStep(this, sElectricDeathTime)) {
        MR::stopScene(sDeathStopSceneTime);
        MR::shakeCameraWeak();
        kill();
        generateItem();
    }
}

void BegomanSpring::exeHide() {
    if (MR::isFirstStep(this)) {
        MR::offBind(this);
        MR::offCalcGravity(this);
        MR::onCalcShadowOneTime(this, nullptr);
        MR::validateClipping(this);
        MR::startBck(this, "Hide", nullptr);
        mHead->tryHopEnd();
        MR::deleteEffect(this, "OnGroundSpark");
    }

    updateRotateY(0.0f, 1.0f);
    mVelocity.zero();
    TVec3f toPlayer(*MR::getPlayerPos());
    toPlayer.sub(mPosition);
    toPlayer.squared();
    if (MR::getPlayerCenterPos()->squared(mPosition) < 1000.0f * 1000.0f) {
        MR::onBind(this);
        MR::onCalcGravity(this);
        MR::onCalcShadow(this, nullptr);
        setNerve(GET_NERVE(BegomanSpring, HostTypeNrvHop));
    }
}

void BegomanSpring::endHide() {
    MR::invalidateClipping(this);
}

void BegomanSpring::exeHop() {
    if (MR::isFirstStep(this)) {
        MR::startBck(this, "Appear", nullptr);
        MR::startSound(this, "SE_EM_BEGOMAN_FLYOUT");
        updateRotateY(sHopInitRotate, sCommonAddRotate);
    }

    updateRotateY(0.0f, 1.0f / sHopRotateBrakeTime);
    if (MR::isBckStopped(this)) {
        setNerve(GET_NERVE(BegomanSpring, HostTypeNrvWait));
    }
}

void BegomanSpring::exeInWater() {
    if (MR::isFirstStep(this)) {
        mVelocity.add(mGravity);
        mVelocity.setLength(sIsInWaterVel);
        MR::startSound(this, "SE_EM_FALL_INTO_WATER_S");
    }

    if (MR::isGreaterStep(this, sIsInWaterTime)) {
        makeActorDead();
        MR::emitEffect(this, "DeathWater");
        MR::startSound(this, "SE_EM_BEGOMAN_DEAD_IN_WATER");
    }
}

void BegomanSpring::exeKeepDistance() {
    updateRotateY(0.2f, sCommonAddRotate);
    exeKeepDistanceCore(GET_NERVE(BegomanSpring, HostTypeNrvWait), GET_NERVE(BegomanSpring, HostTypeNrvSignAttack),
                        GET_NERVE(BegomanSpring, HostTypeNrvBrake), hKeepDistFar, hKeepDistNear);
}

void BegomanSpring::exeBindStarPointer() {
    BegomanBase::exeBindStarPointer();
}

void BegomanSpring::endBindStarPointer() {
    BegomanBase::finishBindStarPointer();
}

void BegomanSpring::generateItem() {
    MR::appearCoinPop(this, mPosition - mGravity * sCoinTransY, 1);
}

void BegomanSpring::startCamera() {
    if (mEventCamera) {
        mEventCamera->start(0);
    }
}

void BegomanSpring::updateCamera() {
    if (!mIsCameraActive || !mEventCamera || mEventCamera->isEnd()) {
        return;
    }

    mEventCamera->update();
    if (mEventCamera->isActive(0) && !MR::isPlayerJumpRising()) {
        mEventCamera->nextForce();
    }

    if (sCameraLimitLength * sCameraLimitLength < mCameraStartPos.squared(*MR::getPlayerCenterPos()) || MR::isOnGroundPlayer() ||
        MR::isPlayerInRush()) {
        endCamera();
        mIsCameraActive = false;
    }
}

void BegomanSpring::endCamera() {
    if (mEventCamera) {
        mEventCamera->endForceSoon();
    }
}

void BegomanSpring::attackSensor(HitSensor* pSender, HitSensor* pReceiver) {
    if (pSender != getSensor("body")) {
        return;
    }

    if (MR::isSensorEnemy(pReceiver)) {
        MR::sendMsgToEnemyAttackBlow(pReceiver, pSender);
        return;
    }

    if (!MR::isSensorPlayer(pReceiver) || isNerve(GET_NERVE(BegomanSpring, HostTypeNrvBlow)) || MR::isPlayerSwingAction() ||
        !MR::isOnGroundPlayer()) {
        return;
    }

    bool msgResult = MR::sendMsgEnemyAttackFlipRot(pReceiver, pSender);

    if (!msgResult) {
        msgResult = MR::sendMsgPush(pReceiver, pSender);
    }

    if (msgResult) {
        TVec3f dirFromSenderToReceiver(pSender->mPosition);

        dirFromSenderToReceiver -= pReceiver->mPosition;
        MR::normalizeOrZero(&dirFromSenderToReceiver);

        if (!MR::isNearZero(dirFromSenderToReceiver)) {
            bool reflected = reboundPlaneWithEffect(dirFromSenderToReceiver, 0.0f, 0.0f, "Spark");
            mVelocity += dirFromSenderToReceiver * sReboundPlayerAddVel;

            if (reflected) {
                MR::startSound(this, "SE_EM_BEGOMAN_COLLI");
            }
        }

        setNerve(GET_NERVE(BegomanSpring, HostTypeNrvHitReaction));
    }
}

bool BegomanSpring::receiveMsgPush(HitSensor* pSender, HitSensor* pReceiver) {
    return false;
}

bool BegomanSpring::receiveMsgEnemyAttack(u32 msg, HitSensor* pSender, HitSensor* pReceiver) {
    if (pReceiver != getSensor("body")) {
        return false;
    }

    if (MR::isMsgEnemyAttackElectric(msg)) {
        TVec3f direction(pSender->mHost->mVelocity);
        MR::vecKillElement(direction, mGravity, &direction);
        MR::normalizeOrZero(&direction);
        return onTouchElectric(pSender->mPosition, direction);
    }

    if (isNerve(GET_NERVE(BegomanSpring, HostTypeNrvShake))) {
        return false;
    }

    if (MR::isMsgExplosionAttack(msg) && !isNerve(GET_NERVE(BegomanSpring, HostTypeNrvBlow))) {
        calcBlowReaction(pSender->mPosition, pReceiver->mPosition, 35.0f, 15.0f);
        setNerve(GET_NERVE(BegomanSpring, HostTypeNrvBlow));
        return true;
    }

    if (MR::isSensorEnemy(pSender)) {
        TVec3f direction(pReceiver->mPosition - pSender->mPosition);
        MR::normalizeOrZero(&direction);
        if (MR::isNearZero(direction)) {
            return false;
        }

        bool reflected = reboundPlaneWithEffect(direction, 0.0f, 0.0f, "Spark");
        f32 receiverRadius = pReceiver->mRadius;
        f32 senderRadius = pSender->mRadius;
        f32 ratio = senderRadius / receiverRadius;
        MR::addVelocityLimit(this, direction * sReboundEnemyAddVel * ratio);
        if (reflected) {
            MR::startSound(this, "SE_EM_BEGOMAN_COLLI_BEGOMAN");
        }

        if (!isNerve(GET_NERVE(BegomanSpring, HostTypeNrvTrample)) && !isNerve(GET_NERVE(BegomanSpring, HostTypeNrvShake)) &&
            !isNerve(GET_NERVE(BegomanSpring, HostTypeNrvProvoke)) && !isNerve(GET_NERVE(BegomanSpring, HostTypeNrvBlow))) {
            MR::startBck(this, "Turn", nullptr);
            setNerve(GET_NERVE(BegomanSpring, HostTypeNrvTurn));
        }

        return true;
    }

    return false;
}

bool BegomanSpring::receiveMsgPlayerAttack(u32 msg, HitSensor* pSender, HitSensor* pReceiver) {
    if (MR::isMsgLockOnStarPieceShoot(msg)) {
        return true;
    }

    if (MR::isMsgStarPieceReflect(msg) && (isNerve(GET_NERVE(BegomanSpring, HostTypeNrvHide)) ||
                                           (isNerve(GET_NERVE(BegomanSpring, HostTypeNrvHop)) && MR::isLessStep(this, hHopStarPieceReflectTime)))) {
        return true;
    }

    if (MR::isMsgPlayerTrample(msg)) {
        if (isNerve(GET_NERVE(BegomanSpring, HostTypeNrvBlow)) || isNerve(GET_NERVE(BegomanSpring, HostTypeNrvElectricDeath))) {
            return false;
        }

        if (!isNerve(GET_NERVE(BegomanSpring, HostTypeNrvShake))) {
            if (pReceiver->isType(ATYPE_BEGOMAN)) {
                setNerve(GET_NERVE(BegomanSpring, HostTypeNrvShake));
                return true;
            }

            return false;
        }

        if (!pReceiver->isType(ATYPE_PLAYER_AUTO_JUMP)) {
            return false;
        }

        if (!mIsCameraActive) {
            startCamera();
            mIsCameraActive = true;
            mCameraStartPos.set(mPosition);
        }

        setNerve(GET_NERVE(BegomanSpring, HostTypeNrvTrample));
        return true;
    }

    if (MR::isMsgPlayerHipDrop(msg)) {
        MR::forceJumpPlayer(-mGravity);
        return true;
    }

    if (isNerve(GET_NERVE(BegomanSpring, HostTypeNrvBlow)) || isNerve(GET_NERVE(BegomanSpring, HostTypeNrvElectricDeath))) {
        return false;
    }

    if (pReceiver == getSensor("body") && (MR::isMsgPlayerHitAll(msg) || MR::isMsgStarPieceAttack(msg))) {
        TVec3f direction(pReceiver->mPosition);
        direction.sub(pSender->mPosition);
        TVec3f hitPos(-direction);
        f32 radius = getSensor("body")->mRadius;
        hitPos.setLength(radius);
        hitPos.add(getSensor("body")->mPosition);
        MR::emitEffectHit(this, hitPos, "Hit");
        MR::startSound(this, "SE_EM_BEGOMAN_KNOCK_SUCCESS");
        finishNoCalcWait();
        setNerve(GET_NERVE(BegomanSpring, HostTypeNrvBlow));
        MR::vecKillElement(direction, mGravity, &direction);
        MR::normalize(&direction);
        mFaceVec.set(direction);
        direction *= sBlowVel;
        direction.sub(mGravity * sBlowVerticalVel);
        mVelocity = direction;
        return true;
    }

    return false;
}

bool BegomanSpring::receiveOtherMsg(u32 msg, HitSensor* pSender, HitSensor* pReceiver) {
    return MR::isMsgHitmarkEmit(msg);
}

void BegomanSpring::calcAnim() {
    LiveActor::calcAnim();
    calcAnimCore(&mHeadMatrix);
}

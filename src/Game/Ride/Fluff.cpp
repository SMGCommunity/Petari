#include "Game/Ride/Fluff.hpp"
#include "Game/Camera/CameraTargetMtx.hpp"
#include "Game/LiveActor/ModelObj.hpp"
#include "Game/LiveActor/Nerve.hpp"
#include "Game/Ride/FluffWind.hpp"
#include "Game/Util.hpp"
#include <revolution/wpad.h>

void Fluff_FORCE_MATCH_SDATA2() {
    (void)1.0f;
    (void)0.0f;
    (void)0.5f;
}

namespace {
    static const s32 sFreeBloomFlyUpStep = 15;
    static const f32 sSpeedWaitAir = 10.0f;
    static const f32 sSpeedWaitAir2nd = 5.0f;
    static const f32 sGravityWaitAir = 0.15f;
    static const f32 sSpeedMaxWaitAir = 0.8f;
    static const s32 sStepToFreeWaitAir2nd = 60;
    static const f32 sRideStartUpSpeedMax = 15.0f;
    static const f32 sRideStartUpAccel = 0.2f;
    static const f32 sRideGravity = 0.15f;
    static const f32 sRideFrictionRate = 0.98f;
    // static const f32 sRideUpSpeedMax =
    // static const f32 sRideDownSpeedMax =
    static const f32 sRideMoveFrontAccel = 0.2f;
    static const f32 sRideFrontSpeedMax = 6.0f;
    static const f32 sRideFrontBlendRate = 0.1f;
    static const f32 sRideFallRotateSpeed = 10.0f;
    static const f32 sWindDistanceMin = 0.0f;
    static const f32 sWindDistanceMax = 300.0f;
    static const f32 sWindAccelNear = 0.3f;
    static const f32 sWindAccelFar = 0.08f;
    static const f32 sDistanceToWindNear = 200.0f;
    static const f32 sDistanceToWindFar = 1500.0f;
    static const f32 sBlendRateToWind = 0.08f;
    static const f32 sBlendRateAxisY = 0.01f;
    static const f32 sBlendRateWindUp = 0.5f;
    static const s32 sStepSpinUpAccel = 30;
    static const f32 sSpinUpAccelY = 0.8f;
    static const f32 sSpinUpAccelOnWindY = 1.3f;
    static const s32 sStepToSpinUp2nd = 60;
    static const s32 sWindRemainTime = 30;
    static const f32 sJumpSpeedY = 18.0f;
};  // namespace

namespace NrvFluff {
    NEW_NERVE(FluffNrvFreeBloom, Fluff, FreeBloom);
    NEW_NERVE(FluffNrvFreeWaitOnGround, Fluff, FreeWaitOnGround);
    NEW_NERVE(FluffNrvFreeWaitAir, Fluff, FreeWaitAir);
    NEW_NERVE(FluffNrvFreeWaitInvalid, Fluff, FreeWaitInvalid);
    NEW_NERVE(FluffNrvRideStart, Fluff, RideStart);
    NEW_NERVE(FluffNrvRideFly, Fluff, RideFly);
    NEW_NERVE(FluffNrvRideFlyOnWind, Fluff, RideFlyOnWind);
    NEW_NERVE(FluffNrvRideSpinUp, Fluff, RideSpinUp);
    NEW_NERVE(FluffNrvRideSpinUpOnWind, Fluff, RideSpinUp);
};  // namespace NrvFluff

Fluff::Fluff(const char* pName)
    : LiveActor(pName), mSide(1.0f, 0.0f, 0.0f), mUp(0.0f, 1.0f, 0.0f), mFront(0.0f, 0.0f, 1.0f), mTargetUpVec(0.0f, 1.0f, 0.0f),
      mWindDir(0.0f, 0.0f, 0.0f), mWindDistance(), mWindSpinTimer(-1), mSpinsRemaining(-1), mModel(), mRider(), mCameraTargetMtx(nullptr) {
    mBaseMtx.identity();
}

void Fluff::init(const JMapInfoIter& rIter) {
    MR::initDefaultPos(this, rIter);

    MR::calcActorAxis(&mSide, &mUp, &mFront, this);
    mTargetUpVec.set(mUp);

    initModelManagerWithAnm("Fluff", nullptr, false);
    MR::startBrk(this, "Fluff");
    MR::connectToSceneRide(this);

    initHitSensor(6);
    MR::addHitSensorBinder(this, "bind", 8, 150.0f, TVec3f(0.0f, 0.0f, 0.0f));
    MR::addHitSensorAtJointRide(this, "body", "MarioHang", 8, 150.0f, TVec3f(0.0f, 0.0f, 0.0f));
    MR::addHitSensorRide(this, "WindReceiver", 8, 300.0f, TVec3f(0.0f, 0.0f, 0.0f));
    MR::addHitSensorAtJointRide(this, "DamageMario", "MarioHang", 8, 60.0f, TVec3f(0.0f, -50.0f, 0.0f));
    MR::addHitSensorAtJointRide(this, "DamageMiddle", "joint3", 8, 50.0f, TVec3f(0.0f, 0.0f, 0.0f));
    MR::addHitSensorAtJointRide(this, "DamageHead", "Head", 8, 100.0f, TVec3f(80.0f, 0.0f, 0.0f));

    initBinder(50.0f, -80.0f, 0);
    initEffectKeeper(0, nullptr, false);
    initSound(6, false);
    MR::calcGravity(this);
    MR::initShadowVolumeSphere(this, 60.0f);

    MR::useStageSwitchWriteA(this, rIter);

    mCameraTargetMtx = new CameraTargetMtx("カメラターゲットダミー");
    MR::setClippingTypeSphere(this, 800.0f);
    mModel = MR::createModelObjMapObj("わたげの葉", "FluffLeaf", nullptr);
    MR::copyTransRotateScale(this, mModel);
    mModel->initWithoutIter();
    initNerve(GET_NERVE(Fluff, FluffNrvFreeWaitOnGround));
    makeActorAppeared();
}

void Fluff::initAfterPlacement() {
    f32 windDistance = 0.0f;
    if (FluffFunction::calcFluffWindInfo(mPosition, &mFront, &windDistance)) {
        MR::makeAxisUpFront(&mSide, &mFront, mUp, mFront);
        MR::setMtxAxisXYZ(mBaseMtx, mSide, mUp, mFront);
        mBaseMtx.setTrans(mPosition);
        MR::resetPosition(this);
    }
}

void Fluff::exeFreeBloom() {
    if (MR::isFirstStep(this)) {
        mSpinsRemaining = 3;
        MR::calcGravity(this);
        MR::offCalcGravity(this);
        MR::offBind(this);
        MR::startBck(mModel, "Bloom", nullptr);
        MR::setShadowVolumeSphereRadius(this, nullptr, 30.0f);
        MR::invalidateHitSensors(this);
        MR::validateHitSensor(this, "WindReceiver");
        MR::hideModel(this);
    }
    if (MR::isBckStopped(mModel)) {
        MR::startBck(mModel, "Wait", nullptr);
        setNerve(GET_NERVE(Fluff, FluffNrvFreeWaitOnGround));
    }
}

void Fluff::exeFreeWaitOnGround() {
    if (MR::isFirstStep(this)) {
        if (MR::tryStartBck(mModel, "Wait", nullptr)) {
            MR::setBckFrameAtRandom(mModel);
        }

        MR::hideModel(this);
        mSpinsRemaining = 3;
    }
}

void Fluff::exeFreeWaitAir() {
    if (MR::isFirstStep(this)) {
        MR::showModel(this);
        MR::startBck(this, "WaitAir", nullptr);
        MR::startBck(mModel, "Spin", 0);
        MR::startSound(this, "SE_OJ_FLUFF_RISE_UP_BLOW");
        MR::onBind(this);
        MR::validateHitSensor(this, "bind");
    }

    MR::addVelocityToGravity(this, ::sGravityWaitAir);
    if (mVelocity.dot(mGravity) > ::sSpeedMaxWaitAir) {
        MR::normalizeOrZero(&mVelocity);
        mVelocity.mult(::sSpeedMaxWaitAir);
    }

    if (MR::isOnGround(this)) {
        MR::emitEffect(this, "Splash");
        MR::startSound(this, "SE_OJ_FLUFF_BREAK");
        MR::releaseSoundHandle(this, "SE_OJ_FLUFF_BREAK");
        setNerve(GET_NERVE(Fluff, FluffNrvFreeWaitInvalid));
    }
}

void Fluff::exeFreeWaitInvalid() {
    if (MR::isFirstStep(this)) {
        MR::hideModel(this);
        MR::offBind(this);

        mBaseMtx.getXYZDir(mSide, mUp, mFront);
        mBaseMtx.getTrans(mPosition);

        mTargetUpVec.set(mUp);
        mVelocity.zero();
        MR::invalidateHitSensors(this);
    }

    if (MR::isStep(this, 120)) {
        MR::showModel(this);
        setNerve(GET_NERVE(Fluff, FluffNrvFreeBloom));
    }
}

void Fluff::exeRideStart() {
    if (MR::isFirstStep(this)) {
        MR::startBck(this, "Fly", nullptr);
        MR::startBckPlayer("FluffStart", static_cast< const char* >(nullptr));
        MR::offBind(this);
    }

    if (mVelocity.dot(mUp) < ::sRideStartUpSpeedMax) {
        mVelocity.add(mUp * ::sRideStartUpAccel);
    }

    if (updateRide()) {
        MR::onBind(this);
        return;
    }

    if (MR::isBckStoppedPlayer()) {
        MR::onBind(this);
        setNerve(GET_NERVE(Fluff, FluffNrvRideFly));
    }
}

void Fluff::exeRideFly() {
    if (MR::isFirstStep(this)) {
        MR::tryStartBck(this, "Fly", nullptr);
        if (mSpinsRemaining > 0) {
            MR::startBckPlayer("FluffFly", static_cast< const char* >(nullptr));
        } else {
            MR::startBckPlayer("FluffFlyLast", static_cast< const char* >(nullptr));
        }
        MR::deleteEffect(this, "HardWind");
        MR::onBind(this);
    }

    if (!updateRide() && !trySpinUp(0)) {
        if (mWindDistance > ::sWindDistanceMin && mWindDistance < ::sWindDistanceMax) {
            setNerve(GET_NERVE(Fluff, FluffNrvRideFlyOnWind));
        }
    }
}

void Fluff::exeRideFlyOnWind() {
    if (MR::isFirstStep(this)) {
        MR::tryStartBck(this, "FlyWind", static_cast< const char* >(nullptr));
        MR::startBckPlayer("FluffFlyWind", static_cast< const char* >(nullptr));
        MR::emitEffect(this, "HardWind");
    }

    if (!updateRide()) {
        mWindSpinTimer = ::sWindRemainTime;
        if (!trySpinUp(0)) {
            MR::tryRumblePadWeak(this, WPAD_CHAN0);
            if (mWindDistance > ::sWindDistanceMax + 100.0f) {
                setNerve(GET_NERVE(Fluff, FluffNrvRideFly));
            }
        }
    }
}

void Fluff::exeRideSpinUp() {
    if (MR::isFirstStep(this)) {
        if (isNerve(GET_NERVE(Fluff, FluffNrvRideSpinUpOnWind))) {
            MR::startSound(mRider, "SE_PV_JUMP_JOY");
        } else {
            MR::startSound(mRider, "SE_PV_TWIST_START");
        }

        MR::tryRumblePadMiddle(this, WPAD_CHAN0);
        MR::startBckPlayer("FluffSpin", static_cast< const char* >(nullptr));
        MR::tryPlayerCoinPull();
        if (mSpinsRemaining > 0) {
            mSpinsRemaining--;
            MR::startSound(this, "SE_OJ_FLUFF_LOSE");
            MR::startSpinHitSound(this);
            if (mSpinsRemaining == 2) {
                MR::startSound(this, "SE_OJ_FLUFF_RISE_UP_1");
                MR::startBck(this, "Spin1st", nullptr);
                MR::startBrk(this, "Spin1st");
            } else if (mSpinsRemaining == 1) {
                MR::startSound(this, "SE_OJ_FLUFF_RISE_UP_2");
                MR::startBck(this, "Spin2nd", nullptr);
                MR::startBrk(this, "Spin2nd");
            } else {
                MR::startSound(this, "SE_OJ_FLUFF_RISE_UP_4");
                MR::startBck(this, "Spin3rd", nullptr);
                MR::startBrk(this, "Spin3rd");
            }
        }
    }

    if (updateRide()) {
        return;
    }

    if (mSpinsRemaining >= 0 && MR::isLessStep(this, ::sStepSpinUpAccel)) {
        TVec3f lift = mUp;
        if (isNerve(GET_NERVE(Fluff, FluffNrvRideSpinUpOnWind))) {
            lift.scale(::sSpinUpAccelOnWindY);
        } else {
            lift.scale(::sSpinUpAccelY);
        }
        mVelocity.add(lift);
        MR::tryRumblePadVeryWeak(this, WPAD_CHAN0);
    }

    if (mWindDistance > ::sWindDistanceMin && mWindDistance < ::sWindDistanceMax) {
        mWindSpinTimer = ::sWindRemainTime;
    }

    if (!trySpinUp(::sStepToSpinUp2nd) && MR::isBckStopped(this)) {
        setNerve(GET_NERVE(Fluff, FluffNrvRideFly));
    }
}

void Fluff::calcAndSetBaseMtx() {
    TPos3f posMtx;
    MR::makeMtxUpFrontPos(&posMtx, mTargetUpVec, mFront, mPosition);
    MR::setBaseTRMtx(this, posMtx);
}

void Fluff::attackSensor(HitSensor* pSender, HitSensor* pReceiver) {
    if (mRider != nullptr) {
        MR::tryGetItem(pSender, pReceiver);
    }
}

bool Fluff::receiveMsgPlayerAttack(u32 msg, HitSensor* pSender, HitSensor* pReceiver) {
    if (MR::isMsgPlayerSpinAttack(msg)) {
        if (isNerve(GET_NERVE(Fluff, FluffNrvFreeBloom)) && MR::isGreaterStep(this, ::sFreeBloomFlyUpStep) ||
            isNerve(GET_NERVE(Fluff, FluffNrvFreeWaitOnGround))) {
            MR::tryRumblePadMiddle(this, WPAD_CHAN0);
            mVelocity.set(mGravity * -::sSpeedWaitAir);
            setNerve(GET_NERVE(Fluff, FluffNrvFreeWaitAir));
            return false;
        }

        if (isNerve(GET_NERVE(Fluff, FluffNrvFreeWaitAir)) && MR::isGreaterStep(this, ::sStepToFreeWaitAir2nd)) {
            MR::tryRumblePadMiddle(this, WPAD_CHAN0);
            mVelocity.set(mGravity * -::sSpeedWaitAir2nd);
            setNerve(GET_NERVE(Fluff, FluffNrvFreeWaitAir));
            return false;
        }
    }

    return false;
}

bool Fluff::receiveMsgEnemyAttack(u32 msg, HitSensor* pSender, HitSensor* pReceiver) {
    if (mRider != nullptr &&
        (MR::isSensor(pReceiver, "DamageMario") || MR::isSensor(pReceiver, "DamageMiddle") || MR::isSensor(pReceiver, "DamageHead"))) {
        endBind(0.0f);
        return true;
    }

    return false;
}

bool Fluff::receiveOtherMsg(u32 msg, HitSensor* pSender, HitSensor* pReceiver) {
    if (MR::isMsgAutoRushBegin(msg)) {
        if (!isNerve(GET_NERVE(Fluff, FluffNrvFreeWaitAir)) || MR::isOnGroundPlayer()) {
            return false;
        }

        mRider = MR::getSensorHost(pSender);
        MR::showModel(this);
        MR::tryStartBck(mModel, "Spin", nullptr);
        MR::onCalcGravity(this);
        mVelocity.set(*MR::getPlayerVelocity());
        if (mVelocity.dot(mGravity) > 0.0f) {
            MR::vecKillElement(mVelocity, mGravity, &mVelocity);
        }

        mFront.set(mVelocity);
        MR::vecKillElement(mFront, mUp, &mFront);
        if (!MR::isNearZero(mFront)) {
            MR::normalize(&mFront);
        } else {
            MR::getPlayerFrontVec(&mFront);
        }

        TVec3f up = mUp;
        mBaseMtx.getYDir(mUp);
        MR::makeAxisUpFront(&mSide, &mFront, mUp, mFront);
        calcAnim();
        MR::emitEffect(this, "Splash");
        MR::emitEffect(this, "Blur");
        mCameraTargetMtx->setMtx(getBaseMtx());
        MR::setGameCameraTarget(CameraTargetArg(nullptr, mCameraTargetMtx, nullptr, nullptr));
        MR::tryRumblePadWeak(this, WPAD_CHAN0);
        MR::startSound(mRider, "SE_PV_CATCH");
        MR::startSound(mRider, "SE_PM_GRAB_OBJ");
        MR::invalidateHitSensors(this);
        MR::validateHitSensor(this, "body");
        MR::validateHitSensor(this, "DamageMario");
        MR::validateHitSensor(this, "DamageMiddle");
        MR::validateHitSensor(this, "DamageHead");

        if (MR::isValidSwitchA(this)) {
            MR::onSwitchA(this);
        }

        MR::offBind(this);
        setNerve(GET_NERVE(Fluff, FluffNrvRideStart));

        return true;
    }

    if (MR::isMsgUpdateBaseMtx(msg)) {
        TVec3f hangPos;
        MR::copyJointPos(this, "MarioHang", &hangPos);
        TPos3f mtx;
        mtx.setTrans(hangPos);
        MR::makeMtxUpFrontPos(&mtx, mTargetUpVec, mFront, hangPos);
        MR::setPlayerBaseMtx(mtx);

        return true;
    }

    if (MR::isMsgRushCancel(msg)) {
        MR::emitEffect(this, "Splash");
        MR::startSound(this, "SE_OJ_FLUFF_BREAK");
        mRider = nullptr;
        setNerve(GET_NERVE(Fluff, FluffNrvFreeWaitInvalid));

        return true;
    }

    return false;
}

void Fluff::endBind(f32 jumpY) {
    if (jumpY > 0.0f) {
        TVec3f jumpVec = mUp * jumpY;
        MR::vecKillElement(mVelocity, mGravity, &mVelocity);
        jumpVec.add(mVelocity);
        MR::startBckPlayer("Fall", static_cast< const char* >(nullptr));
        MR::endBindAndPlayerJump(this, jumpVec, 0);
        MR::startSound(mRider, "SE_PV_JUMP_S");
        MR::startSound(mRider, "SE_PM_JUMP_M");
    } else {
        MR::endBindAndPlayerWait(this);
        MR::startSound(mRider, "SE_PV_JUMP_S");
    }

    MR::setGameCameraTargetToPlayer();

    if (MR::isValidSwitchA(this)) {
        MR::offSwitchA(this);
    }

    mRider = nullptr;
    MR::startBrk(this, "Fluff");
    MR::deleteEffect(this, "HardWind");
    MR::deleteEffect(this, "Blur");
    MR::emitEffect(this, "Splash");
    MR::startSound(this, "SE_OJ_FLUFF_BREAK");
    MR::releaseSoundHandle(this, "SE_OJ_FLUFF_BREAK");

    setNerve(GET_NERVE(Fluff, FluffNrvFreeWaitInvalid));
}

bool Fluff::updateRide() {
    if (MR::testCorePadTriggerA(WPAD_CHAN0) || MR::testSystemTriggerA()) {
        Fluff::endBind(::sJumpSpeedY);
        return true;
    }

    if (MR::isBinded(this)) {
        endBind(0.0f);
        return true;
    }

    mVelocity.mult(::sRideFrictionRate);

    MR::vecBlend(mUp, -mGravity, &mUp, ::sBlendRateAxisY);
    MR::normalize(&mUp);

    TVec3f front(0.0f, 0.0f, 0.0f);
    MR::calcWorldStickDirectionXZ(&front, WPAD_CHAN0);
    MR::vecKillElement(front, mUp, &front);
    if (!MR::isNearZero(front)) {
        MR::normalize(&front);
        if (!MR::isNearZero(mWindDir)) {
            MR::vecKillElement(front, mWindDir, &front);
        }

        TVec3f steerTargetDirection = front;
        if (!MR::isNearZero(steerTargetDirection)) {
            MR::normalize(&steerTargetDirection);
            MR::vecBlend(mFront, steerTargetDirection, &mFront, ::sRideFrontBlendRate);
        }

        MR::normalize(&mFront);
        front.scale(::sRideMoveFrontAccel);
        mVelocity.add(front);
        mSide.cross(mUp, mFront);
        MR::startLevelSound(this, "SE_OJ_LV_FLUFF_SIDE_MOVE");
    }

    updateWind();
    return false;
}

void Fluff::updateWind() {
    TVec3f windFront(0.0f, 0.0f, 0.0f);
    mWindDir.zero();
    FluffFunction::calcFluffWindInfo(mPosition, &mWindDir, &mWindDistance);
    if (mWindDistance > 0.0f) {
        f32 windFactor = MR::clamp((mWindDistance - ::sDistanceToWindNear) / (::sDistanceToWindFar - ::sDistanceToWindNear), 0.0f, 1.0f);
        f32 windSpeedScale = ::sWindAccelFar + (::sWindAccelNear - ::sWindAccelFar) * MR::getEaseInValue(1.0f - windFactor, 0.0f, 1.0f, 1.0f);
        TVec3f windImpulse = mWindDir;
        windImpulse.scale(windSpeedScale);
        mVelocity.add(windImpulse);

        f32 velH = MR::vecKillElement(mVelocity, mWindDir, &mVelocity);
        f32 velV = MR::vecKillElement(mVelocity, mGravity, &mVelocity);
        MR::restrictVelocity(this, ::sRideFrontSpeedMax);

        mVelocity.add(mWindDir * velH);
        mVelocity.add(mGravity * velV);

        windFront.set(mWindDir);
    } else {
        windFront.set(mFront);
    }

    if (mSpinsRemaining > 0) {
        if (MR::getSubPadStickX(WPAD_CHAN0) == 0.0f || MR::getSubPadStickY(WPAD_CHAN0) == 0.0f) {
            MR::vecBlend(mFront, windFront, &mFront, ::sBlendRateToWind);
        }
        MR::startLevelSound(this, "SE_OJ_LV_FLUFF_FLY");
    } else {
        MR::rotateVecDegree(&mSide, mUp, ::sRideFallRotateSpeed);
        MR::rotateVecDegree(&mFront, mUp, ::sRideFallRotateSpeed);
        MR::startLevelSound(this, "SE_OJ_LV_FLUFF_FLY_FALL");
    }

    TVec3f gravity = mGravity;
    gravity.scale(::sRideGravity);
    mVelocity.add(gravity);

    TPos3f mtx;
    mtx.identity();
    MR::makeMtxUpFront(&mtx, mUp, mFront);
    mtx.getXDir(mSide);
    mtx.getZDir(mFront);

    if (mWindDistance > 0.0f) {
        f32 tiltFactor = 1.0f - MR::clamp((mWindDistance - ::sDistanceToWindNear) / (::sDistanceToWindFar - ::sDistanceToWindNear), 0.0f, 1.0f);

        f32 easeInValue = MR::getEaseInValue(tiltFactor, 0.0f, 1.0f, 1.0f);
        MR::vecBlend(mUp, mWindDir, &mTargetUpVec, easeInValue * ::sBlendRateWindUp);
    } else {
        mTargetUpVec.set(mUp);
    }

    if (!MR::isNearZero(windFront)) {
        TVec3f front = windFront;
        MR::normalize(&front);
        TVec3f baseUp;
        mBaseMtx.getYDir(baseUp);

        TPos3f mtx;
        mtx.identity();
        MR::makeMtxUpFrontPos(&mtx, baseUp, front, mPosition);

        mCameraTargetMtx->setMtx(mtx);
    }
}

bool Fluff::trySpinUp(s32 step) {
    mWindSpinTimer--;

    if (mSpinsRemaining > 0 && MR::isPadSwing(WPAD_CHAN0) && MR::isGreaterStep(this, step)) {
        if (mWindSpinTimer > 0) {
            setNerve(GET_NERVE(Fluff, FluffNrvRideSpinUpOnWind));
        } else {
            setNerve(GET_NERVE(Fluff, FluffNrvRideSpinUp));
        }

        return true;
    }

    return false;
}

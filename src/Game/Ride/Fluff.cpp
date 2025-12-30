#include "Game/Ride/Fluff.hpp"
#include "Game/Camera/CameraTargetMtx.hpp"
#include "Game/LiveActor/ModelObj.hpp"
#include "Game/Ride/FluffWind.hpp"
#include <JSystem/JGeometry/TVec.hpp>
#include <revolution/wpad.h>

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
}  // namespace NrvFluff

Fluff::Fluff(const char* pName)
    : LiveActor(pName), mSide(1.0f, 0.0f, 0.0f), mUp(0.0f, 1.0f, 0.0f), mFront(0.0f, 0.0f, 1.0f), mTargetUpVec(0.0f, 1.0f, 0.0f),
      mWindDir(0.0f, 0.0f, 0.0f), mWindStrength(0.0f), mWindSpinTimer(-1), mSpinsRemaining(-1), mModel(nullptr), mRider(nullptr),
      mCameraTargetMtx(nullptr) {
    mInitialMtx.identity();
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
    initNerve(&NrvFluff::FluffNrvFreeWaitOnGround::sInstance);
    makeActorAppeared();
}

void Fluff::initAfterPlacement() {
    f32 windStrength = 0.0f;
    if (FluffFunction::calcFluffWindInfo(mPosition, &mFront, &windStrength)) {
        MR::makeAxisUpFront(&mSide, &mFront, mUp, mFront);
        MR::setMtxAxisXYZ(mInitialMtx, mSide, mUp, mFront);
        mInitialMtx.setTrans(mPosition);
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
        setNerve(&NrvFluff::FluffNrvFreeWaitOnGround::sInstance);
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
        MR::startSound(this, "SE_OJ_FLUFF_RISE_UP_BLOW", -1, -1);
        MR::onBind(this);
        MR::validateHitSensor(this, "bind");
    }

    MR::addVelocityToGravity(this, 0.15f);
    if (mVelocity.dot(mGravity) > 0.8f) {
        MR::normalizeOrZero(&mVelocity);
        mVelocity.mult(0.8f);
    }

    if (MR::isOnGround(this)) {
        MR::emitEffect(this, "Splash");
        MR::startSound(this, "SE_OJ_FLUFF_BREAK", -1, -1);
        MR::releaseSoundHandle(this, "SE_OJ_FLUFF_BREAK");
        setNerve(&NrvFluff::FluffNrvFreeWaitInvalid::sInstance);
    }
}

void Fluff::exeFreeWaitInvalid() {
    if (MR::isFirstStep(this)) {
        MR::hideModel(this);
        MR::offBind(this);

        mInitialMtx.getXYZDirInline(mSide, mUp, mFront);
        mInitialMtx.getTransInline(mPosition);

        mTargetUpVec.set(mUp);
        mVelocity.zero();
        MR::invalidateHitSensors(this);
    }
    if (MR::isStep(this, 120)) {
        MR::showModel(this);
        setNerve(&NrvFluff::FluffNrvFreeBloom::sInstance);
    }
}

void Fluff::exeRideStart() {
    if (MR::isFirstStep(this)) {
        MR::startBck(this, "Fly", nullptr);
        MR::startBckPlayer("FluffStart", (const char*)nullptr);
        MR::offBind(this);
    }

    if (mVelocity.dot(mUp) < 15.0f) {
        mVelocity.add(mUp.scaleInline(0.2f));
    }

    if (updateRide()) {
        MR::onBind(this);
        return;
    }

    if (MR::isBckStoppedPlayer()) {
        MR::onBind(this);
        setNerve(&NrvFluff::FluffNrvRideFly::sInstance);
    }
}

void Fluff::exeRideFly() {
    if (MR::isFirstStep(this)) {
        MR::tryStartBck(this, "Fly", nullptr);
        if (mSpinsRemaining > 0) {
            MR::startBckPlayer("FluffFly", (const char*)nullptr);
        } else {
            MR::startBckPlayer("FluffFlyLast", (const char*)nullptr);
        }
        MR::deleteEffect(this, "HardWind");
        MR::onBind(this);
    }
    if (!updateRide() && !trySpinUp(0)) {
        if (mWindStrength > 0.0f && mWindStrength < 300.0f) {
            setNerve(&NrvFluff::FluffNrvRideFlyOnWind::sInstance);
        }
    }
}

void Fluff::exeRideFlyOnWind() {
    if (MR::isFirstStep(this)) {
        MR::tryStartBck(this, "FlyWind", (const char*)nullptr);
        MR::startBckPlayer("FluffFlyWind", (const char*)nullptr);
        MR::emitEffect(this, "HardWind");
    }

    if (!updateRide()) {
        mWindSpinTimer = 30;
        if (!trySpinUp(0)) {
            MR::tryRumblePadWeak(this, 0);
            if (mWindStrength > 400.0f) {
                setNerve(&NrvFluff::FluffNrvRideFly::sInstance);
            }
        }
    }
}

void Fluff::exeRideSpinUp() {
    if (MR::isFirstStep(this)) {
        if (isNerve(&NrvFluff::FluffNrvRideSpinUpOnWind::sInstance)) {
            MR::startSound(mRider, "SE_PV_JUMP_JOY", -1, -1);
        } else {
            MR::startSound(mRider, "SE_PV_TWIST_START", -1, -1);
        }

        MR::tryRumblePadMiddle(this, 0);
        MR::startBckPlayer("FluffSpin", (const char*)nullptr);
        MR::tryPlayerCoinPull();
        if (mSpinsRemaining > 0) {
            mSpinsRemaining--;
            MR::startSound(this, "SE_OJ_FLUFF_LOSE", -1, -1);
            MR::startSpinHitSound(this);
            if (mSpinsRemaining == 2) {
                MR::startSound(this, "SE_OJ_FLUFF_RISE_UP_1", -1, -1);
                MR::startBck(this, "Spin1st", nullptr);
                MR::startBrk(this, "Spin1st");
            } else if (mSpinsRemaining == 1) {
                MR::startSound(this, "SE_OJ_FLUFF_RISE_UP_2", -1, -1);
                MR::startBck(this, "Spin2nd", nullptr);
                MR::startBrk(this, "Spin2nd");
            } else {
                MR::startSound(this, "SE_OJ_FLUFF_RISE_UP_4", -1, -1);
                MR::startBck(this, "Spin3rd", nullptr);
                MR::startBrk(this, "Spin3rd");
            }
        }
    }

    if (updateRide()) {
        return;
    }

    if (mSpinsRemaining >= 0 && MR::isLessStep(this, 30)) {
        TVec3f liftImpulse(mUp);
        if (isNerve(&NrvFluff::FluffNrvRideSpinUpOnWind::sInstance)) {
            liftImpulse.scale(1.3f);
        } else {
            liftImpulse.scale(0.8f);
        }
        mVelocity.add(liftImpulse);
        MR::tryRumblePadVeryWeak(this, 0);
    }

    if (mWindStrength > 0.0f && mWindStrength < 300.0f) {
        mWindSpinTimer = 30;
    }

    if (!trySpinUp(60) && MR::isBckStopped(this)) {
        setNerve(&NrvFluff::FluffNrvRideFly::sInstance);
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

bool Fluff::receiveMsgPlayerAttack(u32 msg, HitSensor*, HitSensor*) {
    if (MR::isMsgPlayerSpinAttack(msg)) {
        if (isNerve(&NrvFluff::FluffNrvFreeBloom::sInstance) && MR::isGreaterStep(this, 15) ||
            isNerve(&NrvFluff::FluffNrvFreeWaitOnGround::sInstance)) {
            MR::tryRumblePadMiddle(this, WPAD_CHAN0);
            mVelocity.set(mGravity.scaleInline(-10.0f));
            setNerve(&NrvFluff::FluffNrvFreeWaitAir::sInstance);

            return false;
        }

        if (isNerve(&NrvFluff::FluffNrvFreeWaitAir::sInstance) && MR::isGreaterStep(this, 60)) {
            MR::tryRumblePadMiddle(this, WPAD_CHAN0);
            mVelocity.set(mGravity.scaleInline(-5.0f));
            setNerve(&NrvFluff::FluffNrvFreeWaitAir::sInstance);

            return false;
        }
    }

    return false;
}

bool Fluff::receiveMsgEnemyAttack(u32, HitSensor*, HitSensor* pReceiver) {
    if (mRider != nullptr &&
        (MR::isSensor(pReceiver, "DamageMario") || MR::isSensor(pReceiver, "DamageMiddle") || MR::isSensor(pReceiver, "DamageHead"))) {
        endBind(0.0f);

        return true;
    }

    return false;
}

bool Fluff::receiveOtherMsg(u32 msg, HitSensor* pSender, HitSensor* pReceiver) {
    if (MR::isMsgAutoRushBegin(msg)) {
        if (!isNerve(&NrvFluff::FluffNrvFreeWaitAir::sInstance) || MR::isOnGroundPlayer()) {
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

        TVec3f upVec(mUp);
        mInitialMtx.getYDir(mUp);
        MR::makeAxisUpFront(&mSide, &mFront, mUp, mFront);
        calcAnim();
        MR::emitEffect(this, "Splash");
        MR::emitEffect(this, "Blur");
        mCameraTargetMtx->setMtx(getBaseMtx());
        MR::setGameCameraTarget(CameraTargetArg(nullptr, mCameraTargetMtx, nullptr, nullptr));
        MR::tryRumblePadWeak(this, WPAD_CHAN0);
        MR::startSound(mRider, "SE_PV_CATCH", -1, -1);
        MR::startSound(mRider, "SE_PM_GRAB_OBJ", -1, -1);
        MR::invalidateHitSensors(this);
        MR::validateHitSensor(this, "body");
        MR::validateHitSensor(this, "DamageMario");
        MR::validateHitSensor(this, "DamageMiddle");
        MR::validateHitSensor(this, "DamageHead");

        if (MR::isValidSwitchA(this)) {
            MR::onSwitchA(this);
        }

        MR::offBind(this);
        setNerve(&NrvFluff::FluffNrvRideStart::sInstance);

        return true;
    }

    if (MR::isMsgUpdateBaseMtx(msg)) {
        TVec3f marioHangPosVec;
        MR::copyJointPos(this, "MarioHang", &marioHangPosVec);
        TPos3f marioHangPos;
        marioHangPos.setTrans(marioHangPosVec);
        MR::makeMtxUpFrontPos(&marioHangPos, mTargetUpVec, mFront, marioHangPosVec);
        MR::setPlayerBaseMtx(marioHangPos);

        return true;
    }

    if (MR::isMsgRushCancel(msg)) {
        MR::emitEffect(this, "Splash");
        MR::startSound(this, "SE_OJ_FLUFF_BREAK", -1, -1);
        mRider = nullptr;
        setNerve(&NrvFluff::FluffNrvFreeWaitInvalid::sInstance);

        return true;
    }

    return false;
}

void Fluff::endBind(f32 ejectForce) {
    if (ejectForce > 0.0f) {
        TVec3f jumpImpulseVec(mUp);
        jumpImpulseVec.scale(ejectForce);
        MR::vecKillElement(mVelocity, mGravity, &mVelocity);
        jumpImpulseVec.add(mVelocity);
        MR::startBckPlayer("Fall", (const char*)nullptr);
        MR::endBindAndPlayerJump(this, jumpImpulseVec, 0);
        MR::startSound(mRider, "SE_PV_JUMP_S", -1, -1);
        MR::startSound(mRider, "SE_PM_JUMP_M", -1, -1);
    } else {
        MR::endBindAndPlayerWait(this);
        MR::startSound(mRider, "SE_PV_JUMP_S", -1, -1);
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
    MR::startSound(this, "SE_OJ_FLUFF_BREAK", -1, -1);
    MR::releaseSoundHandle(this, "SE_OJ_FLUFF_BREAK");

    setNerve(&NrvFluff::FluffNrvFreeWaitInvalid::sInstance);
}

bool Fluff::updateRide() {
    if (MR::testCorePadTriggerA(WPAD_CHAN0) || MR::testSystemTriggerA()) {
        Fluff::endBind(18.0f);
        return true;
    }

    if (MR::isBinded(this)) {
        endBind(0.0f);
        return true;
    }

    mVelocity.mult(0.98f);

    MR::vecBlend(mUp, mGravity.negateInline(), &mUp, 0.01f);
    MR::normalize(&mUp);

    TVec3f worldStickDirection(0.0f, 0.0f, 0.0f);
    MR::calcWorldStickDirectionXZ(&worldStickDirection, WPAD_CHAN0);
    MR::vecKillElement(worldStickDirection, mUp, &worldStickDirection);
    if (!MR::isNearZero(worldStickDirection)) {
        MR::normalize(&worldStickDirection);
        if (!MR::isNearZero(mWindDir)) {
            MR::vecKillElement(worldStickDirection, mWindDir, &worldStickDirection);
        }

        TVec3f steerTargetDirection(worldStickDirection);
        if (!MR::isNearZero(steerTargetDirection)) {
            MR::normalize(&steerTargetDirection);
            MR::vecBlend(mFront, steerTargetDirection, &mFront, 0.1f);
        }

        MR::normalize(&mFront);
        worldStickDirection.scale(0.2f);
        mVelocity.add(worldStickDirection);
        PSVECCrossProduct(&mUp, &mFront, &mSide);
        MR::startLevelSound(this, "SE_OJ_LV_FLUFF_SIDE_MOVE", -1, -1, -1);
    }

    updateWind();
    return false;
}

void Fluff::updateWind() {
    TVec3f targetMoveDir(0.0f, 0.0f, 0.0f);
    mWindDir.zero();
    FluffFunction::calcFluffWindInfo(mPosition, &mWindDir, &mWindStrength);
    if (mWindStrength > 0.0f) {
        f32 windFactor = MR::clamp(((mWindStrength - 200.0f) / 1300.0f), 0.0f, 1.0f);
        f32 windSpeedScale = 0.08f + ((0.3f - 0.08f) * MR::getEaseInValue(1.0f - windFactor, 0.0f, 1.0f, 1.0f));
        TVec3f windImpulse(mWindDir);
        windImpulse.scale(windSpeedScale);
        mVelocity.add(windImpulse);

        f32 speedAlongWind = MR::vecKillElement(mVelocity, mWindDir, &mVelocity);
        f32 speedAlongGrav = MR::vecKillElement(mVelocity, mGravity, &mVelocity);
        MR::restrictVelocity(this, 6.0f);

        mVelocity.add(mWindDir.scaleInline(speedAlongWind));
        mVelocity.add(mGravity.scaleInline(speedAlongGrav));

        targetMoveDir.set(mWindDir);
    } else {
        targetMoveDir.set(mFront);
    }

    if (mSpinsRemaining > 0) {
        if (MR::getSubPadStickX(WPAD_CHAN0) == 0.0f || MR::getSubPadStickY(WPAD_CHAN0) == 0.0f) {
            MR::vecBlend(mFront, targetMoveDir, &mFront, 0.08f);
        }
        MR::startLevelSound(this, "SE_OJ_LV_FLUFF_FLY", -1, -1, -1);
    } else {
        MR::rotateVecDegree(&mSide, mUp, 10.0f);
        MR::rotateVecDegree(&mFront, mUp, 10.0f);
        MR::startLevelSound(this, "SE_OJ_LV_FLUFF_FLY_FALL", -1, -1, -1);
    }

    TVec3f gravityImpulse(mGravity);
    gravityImpulse.scale(0.15f);
    mVelocity.add(gravityImpulse);

    TPos3f orientationMtx;
    orientationMtx.identity();
    MR::makeMtxUpFront(&orientationMtx, mUp, mFront);
    orientationMtx.getXDirInline(mSide);
    orientationMtx.getZDirInline(mFront);

    if (mWindStrength > 0.0f) {
        f32 tiltFactor = 1.0f - MR::clamp(((mWindStrength - 200.0f) / 1300.0f), 0.0f, 1.0f);

        f32 easeInValue = MR::getEaseInValue(tiltFactor, 0.0f, 1.0f, 1.0f);
        MR::vecBlend(mUp, mWindDir, &mTargetUpVec, (easeInValue * 0.5f));
    } else {
        mTargetUpVec.set(mUp);
    }

    if (!MR::isNearZero(targetMoveDir)) {
        TVec3f moveDirNorm(targetMoveDir);
        MR::normalize(&moveDirNorm);
        TVec3f initialUpVec;
        mInitialMtx.getYDir(initialUpVec);

        TPos3f mtx;
        mtx.identity();
        MR::makeMtxUpFrontPos(&mtx, initialUpVec, moveDirNorm, mPosition);

        mCameraTargetMtx->setMtx(mtx);
    }
}

bool Fluff::trySpinUp(s32 step) {
    mWindSpinTimer--;

    if (mSpinsRemaining > 0 && MR::isPadSwing(WPAD_CHAN0) && MR::isGreaterStep(this, step)) {
        if (mWindSpinTimer > 0) {
            setNerve(&NrvFluff::FluffNrvRideSpinUpOnWind::sInstance);
        } else {
            setNerve(&NrvFluff::FluffNrvRideSpinUp::sInstance);
        }

        return true;
    }

    return false;
}

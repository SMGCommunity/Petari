#include "Game/Ride/Trapeze.hpp"
#include "Game/LiveActor/HitSensor.hpp"
#include "Game/LiveActor/PartsModel.hpp"
#include "Game/Ride/SwingRopePoint.hpp"
#include "Game/Scene/SceneFunction.hpp"
#include "Game/Scene/SceneObjHolder.hpp"
#include "Game/Util/ActorCameraUtil.hpp"
#include "Game/Util/ActorSensorUtil.hpp"
#include "Game/Util/ActorShadowUtil.hpp"
#include "Game/Util/CameraUtil.hpp"
#include "Game/Util/Color.hpp"
#include "Game/Util/DemoUtil.hpp"
#include "Game/Util/GamePadUtil.hpp"
#include "Game/Util/LiveActorUtil.hpp"
#include "Game/Util/MathUtil.hpp"
#include "Game/Util/MtxUtil.hpp"
#include "Game/Util/ObjUtil.hpp"
#include "Game/Util/PlayerUtil.hpp"
#include "Game/Util/SoundUtil.hpp"
#include <JSystem/JUtility/JUTTexture.hpp>
#include <revolution/gx/GXCull.h>
#include <revolution/gx/GXEnum.h>
#include <revolution/gx/GXGeometry.h>
#include <revolution/gx/GXLighting.h>
#include <revolution/gx/GXPixel.h>
#include <revolution/gx/GXTev.h>
#include <revolution/gx/GXTransform.h>
#include <revolution/gx/GXVert.h>
#include <revolution/mtx.h>

void Trapeze_FORCE_MATCH_SDATA2() {
    (void)1.0f;
    (void)0.0f;
    (void)0.5f;
    (void)3.0f;
    (void)-1.0f;
}

namespace {
    static const f32 sStickLength = 120.0f;
    static const f32 sDrawWidthX = 12.0f;
    static const f32 sDrawWidthZ = 12.0f;
    static const f32 sDrawWidthLongOffsetX = 7.0f;
    static const f32 sDrawWidthShortOffsetX = 0.0f;
    static const f32 sDrawWidthShortOffsetZ = 7.0f;
    static const f32 sTexRate = 0.003f;
    static const f32 sTexRateDiff1 = 0.7f;
    static const f32 sTexRateDiff2 = 0.5f;
    static const f32 sHangAccelRate = 5.0f;
    static const f32 sHangPointGravity = 0.8f;
    static const f32 sHangPointFrictionRate = 0.995f;
    static const f32 sHangCoordSpeed = 0.5f;
    static const f32 sSensorOffsetY = -70.0f;
    static const s32 sAnimInterpoleFrame = 12;
    // static const f32 sSpeedWaitToEndAnim =
    static const f32 sSpeedToSwingWait = 10.0f;
    static const f32 sAxisToSwingWait = 0.99f;
    static const f32 sJumpSpeedRate = 1.0f;
    static const f32 sJumpSpeedMinXZ = 15.0f;
    static const f32 sJumpSpeedMaxXZ = 20.0f;
    static const f32 sJumpSpeedMinY = 20.0f;
    static const f32 sJumpSpeedMaxY = 50.0f;
    static const f32 sStickPointGravity = 1.0f;
    static const f32 sStickPointFrictionRate = 0.995f;
    static const f32 sPointSpeedMin = 1.0f;
    static const f32 sPointNormalMin = 0.001f;
    static const f32 sDistanceToValid = 500.0f;
    static const s32 sStepToValid = 40;
    static const f32 sDistanceStartBindZ = 80.0f;
    static const s32 sStepSlideToRumble = 20;
    static const f32 sSpeedMinToKeepFreeSwing = 80.0f;
    static const f32 sTransDiffMinY = 1.0f;
    static const f32 sTexRateDiff0 = 0.0f;

    static Color8 sColorPlusZ(0xFF, 0xFF, 0xFF, 0xFF);
    static Color8 sColorPlusX(0xB4, 0xB4, 0xB4, 0xFF);
    static Color8 sColorMinusX(0x64, 0x64, 0x64, 0xFF);
};  // namespace

Trapeze::Trapeze(const char* pName)
    : LiveActor(pName), mSide(1.0f, 0.0f, 0.0f), mUp(0.0f, 1.0f, 0.0f), mFront(0.0f, 0.0f, 1.0f), mRopeLength(), mStickPoint(), mTrapezeModel(),
      mRider(), mHangPoint(), mGrabCoord(), mSwingVel(), mSwingReverse(), mIsSwingFront(), mIsSwingBack(), mWasSwingFront(), mWasSwingBack(),
      mCameraInfo(), mPrevSoundLvl() {
    mPosMtx.identity();
}

void Trapeze::init(const JMapInfoIter& rIter) {
    MR::createSceneObj(SceneObj_TrapezeRopeDrawInit);
    MR::connectToScene(this, MR::MovementType_Ride, MR::CalcAnimType_None, MR::DrawBufferType_None, MR::DrawType_Trapeze);
    MR::initDefaultPos(this, rIter);

    TPos3f mtx;
    mtx.identity();
    MR::makeMtxTR(mtx, this);

    mtx.getXDir(mSide);
    mtx.getYDir(mUp);
    mtx.getZDir(mFront);

    mRopeLength = mScale.y * 100.0f;
    mScale.set(1.0f, 1.0f, 1.0f);

    TVec3f spawnPos(mPosition.x, mPosition.y - mRopeLength, mPosition.z);
    mStickPoint = new SwingRopePoint(spawnPos);
    mHangPoint = new SwingRopePoint(mPosition);

    mStickPoint->updatePosAndAxis(mFront, ::sStickPointFrictionRate);
    mHangPoint->updatePosAndAxis(mFront, ::sHangPointFrictionRate);

    initNerve(GET_NERVE(Trapeze, TrapezeNrvFree));

    initHitSensor(2);
    MR::addHitSensorCallbackBinder(this, "bind", 8, 200.0f);
    MR::addHitSensorCallbackRide(this, "body", 8, 130.0f);

    MR::calcGravity(this);

    initSound(4, false);

    MR::setClippingTypeSphere(this, mRopeLength);

    mTrapezeModel = new PartsModel(this, "棒", "Trapeze", mPosMtx, MR::DrawBufferType_Enemy, false);
    mTrapezeModel->initWithoutIter();

    MR::initShadowFromCSV(mTrapezeModel, "Shadow");
    MR::setShadowDropLength(mTrapezeModel, nullptr, mRopeLength + 1000.0f);
    MR::registerDemoSimpleCastAll(mTrapezeModel);
    MR::initActorCamera(this, rIter, &mCameraInfo);
    makeActorAppeared();
}

void Trapeze::draw() const {
    if (!MR::isValidDraw(this)) {
        return;
    }

    TVec3f left = mSide * -(::sStickLength / 2.0f);
    TVec3f right = mSide * (::sStickLength / 2.0f);

    TVec3f grabLeft = left;
    TVec3f grabRight = right;
    TVec3f swingLeft = left;
    TVec3f swingRight = right;

    left.add(mPosition);
    right.add(mPosition);
    grabLeft.add(mHangPoint->mPosition);
    grabRight.add(mHangPoint->mPosition);
    swingLeft.add(mStickPoint->mPosition);
    swingRight.add(mStickPoint->mPosition);

    if (mRider != nullptr) {
        drawRope(left, grabLeft, mHangPoint->mSide, mHangPoint->mFront, 0.0f, mGrabCoord * ::sTexRate);
        drawRope(right, grabRight, mHangPoint->mSide, mHangPoint->mFront, 0.0f, mGrabCoord * ::sTexRate);

        if (mGrabCoord < mRopeLength) {
            drawRope(grabLeft, swingLeft, mStickPoint->mSide, mStickPoint->mFront, mGrabCoord * ::sTexRate, mRopeLength * ::sTexRate);
            drawRope(grabRight, swingRight, mStickPoint->mSide, mStickPoint->mFront, mGrabCoord * ::sTexRate, mRopeLength * ::sTexRate);
        }
    } else {
        drawRope(left, swingLeft, mStickPoint->mSide, mStickPoint->mFront, 0.0f, mRopeLength * ::sTexRate);
        drawRope(right, swingRight, mStickPoint->mSide, mStickPoint->mFront, 0.0f, mRopeLength * ::sTexRate);
    }
}

void Trapeze::exeStop() {
    if (MR::isFirstStep(this)) {
        MR::validateClipping(this);
        getSensor("body")->invalidate();
    }
}

void Trapeze::exeFree() {
    if (MR::isFirstStep(this)) {
        MR::invalidateClipping(this);
    }

    updateStick(mPosition, mRopeLength);
    updateStickMtx();

    if (isNerve(GET_NERVE(Trapeze, TrapezeNrvFree))) {
        if (mStickPoint->mVelocity.squared() < ::sPointSpeedMin * ::sPointSpeedMin && 1.0f - MR::abs(mStickPoint->mUp.y) < ::sPointNormalMin) {
            setNerve(GET_NERVE(Trapeze, TrapezeNrvStop));
        }
    }
}

void Trapeze::exeFreeInvalid() {
    exeFree();
    TVec3f posDiffHoriz = *MR::getPlayerPos();
    posDiffHoriz.sub(mStickPoint->mPosition);
    posDiffHoriz.y = 0.0f;

    if (MR::isOnGroundPlayer() || MR::isGreaterStep(this, ::sStepToValid) || posDiffHoriz.squared() > ::sDistanceToValid * ::sDistanceToValid) {
        setNerve(GET_NERVE(Trapeze, TrapezeNrvFree));
    }
}

void Trapeze::exeSwingWait() {
    if (MR::isFirstStep(this)) {
        MR::startBckPlayer("TrapezeWait", ::sAnimInterpoleFrame);
    }

    if (updateBind()) {
        return;
    }

    if (mIsSwingFront) {
        setNerve(GET_NERVE(Trapeze, TrapezeNrvSwingFrontStart));
    } else if (mIsSwingBack) {
        setNerve(GET_NERVE(Trapeze, TrapezeNrvSwingBackStart));
    }
}

void Trapeze::exeSwingSlideDownStart() {
    if (MR::isFirstStep(this)) {
        MR::startBckPlayer("TrapezeSlideDownStart", static_cast< const char* >(nullptr));
    }

    if (!updateSlideDown() && MR::isBckStopped(mRider)) {
        setNerve(GET_NERVE(Trapeze, TrapezeNrvSwingSlideDown));
    }
}

void Trapeze::exeSwingSlideDown() {
    if (MR::isFirstStep(this)) {
        MR::startBckPlayer("TrapezeSlideDown", static_cast< const char* >(nullptr));
    }

    if (updateSlideDown()) {
        return;
    }
}

void Trapeze::exeSwingFrontStart() {
    if (MR::isFirstStep(this)) {
        if (isNerve(GET_NERVE(Trapeze, TrapezeNrvSlowSwingFrontStart))) {
            MR::startBckPlayer("TrapezeSlowSwingFrontStart", ::sAnimInterpoleFrame);
        } else {
            MR::startBckPlayer("TrapezeSwingFrontStart", ::sAnimInterpoleFrame);
        }
    }

    if (updateSwing()) {
        return;
    }

    if (MR::isBckStopped(mRider) && !(mIsSwingFront && mSwingVel < 0.0f)) {
        if (!mIsSwingFront && mSwingVel < ::sSpeedMinToKeepFreeSwing) {
            setNerve(GET_NERVE(Trapeze, TrapezeNrvSlowSwingFrontEnd));
            return;
        }

        setNerve(GET_NERVE(Trapeze, TrapezeNrvSwingFrontEnd));
        return;
    }

    if (isStartSwingFront() && isNerve(GET_NERVE(Trapeze, TrapezeNrvSlowSwingFrontStart))) {
        setNerve(GET_NERVE(Trapeze, TrapezeNrvSwingFrontStart));
        return;
    }

    if (isStartSwingBack()) {
        setNerve(GET_NERVE(Trapeze, TrapezeNrvSwingBackStart));
        return;
    }

    if (!mIsSwingFront && mSwingVel < 0.0f) {
        if (isNerve(GET_NERVE(Trapeze, TrapezeNrvSlowSwingFrontStart))) {
            setNerve(GET_NERVE(Trapeze, TrapezeNrvSlowSwingBackEnd));
        } else {
            setNerve(GET_NERVE(Trapeze, TrapezeNrvSwingBackEnd));
        }
    }
}

void Trapeze::exeSwingFrontEnd() {
    if (MR::isFirstStep(this)) {
        if (isNerve(GET_NERVE(Trapeze, TrapezeNrvSlowSwingFrontEnd))) {
            MR::startBckPlayer("TrapezeSlowSwingFrontEnd", ::sAnimInterpoleFrame);
        } else {
            MR::startBckPlayer("TrapezeSwingFrontEnd", ::sAnimInterpoleFrame);
        }
    }

    if (updateSwing()) {
        return;
    }

    if (mSwingVel <= 0.0f) {
        if (isNerve(GET_NERVE(Trapeze, TrapezeNrvSlowSwingFrontEnd))) {
            setNerve(GET_NERVE(Trapeze, TrapezeNrvSlowSwingBackStart));
        } else {
            setNerve(GET_NERVE(Trapeze, TrapezeNrvSwingBackStart));
        }
        return;
    }

    if (isStartSwingFront()) {
        setNerve(GET_NERVE(Trapeze, TrapezeNrvSwingFrontStart));
        return;
    }

    if (isStartSwingBack() && isNerve(GET_NERVE(Trapeze, TrapezeNrvSlowSwingFrontEnd))) {
        setNerve(GET_NERVE(Trapeze, TrapezeNrvSwingBackStart));
    }
}

void Trapeze::exeSwingBackStart() {
    if (MR::isFirstStep(this)) {
        if (isNerve(GET_NERVE(Trapeze, TrapezeNrvSlowSwingBackStart))) {
            MR::startBckPlayer("TrapezeSlowSwingBackStart", ::sAnimInterpoleFrame);
        } else {
            MR::startBckPlayer("TrapezeSwingBackStart", ::sAnimInterpoleFrame);
        }
    }

    if (updateSwing()) {
        return;
    }

    if (MR::isBckStopped(mRider) && !(mIsSwingBack && mSwingVel > 0.0f)) {
        if (!mIsSwingBack && mSwingVel > -::sSpeedMinToKeepFreeSwing) {
            setNerve(GET_NERVE(Trapeze, TrapezeNrvSlowSwingBackEnd));
            return;
        }

        setNerve(GET_NERVE(Trapeze, TrapezeNrvSwingBackEnd));
        return;
    }

    if (isStartSwingBack() && isNerve(GET_NERVE(Trapeze, TrapezeNrvSlowSwingBackStart))) {
        setNerve(GET_NERVE(Trapeze, TrapezeNrvSwingBackStart));
        return;
    }

    if (isStartSwingFront()) {
        setNerve(GET_NERVE(Trapeze, TrapezeNrvSwingFrontStart));
        return;
    }

    if (!mIsSwingBack && mSwingVel > 0.0f) {
        if (isNerve(GET_NERVE(Trapeze, TrapezeNrvSlowSwingBackStart))) {
            setNerve(GET_NERVE(Trapeze, TrapezeNrvSlowSwingFrontEnd));
        } else {
            setNerve(GET_NERVE(Trapeze, TrapezeNrvSwingFrontEnd));
        }
    }
}

void Trapeze::exeSwingBackEnd() {
    if (MR::isFirstStep(this)) {
        if (isNerve(GET_NERVE(Trapeze, TrapezeNrvSlowSwingBackEnd))) {
            MR::startBckPlayer("TrapezeSlowSwingBackEnd", ::sAnimInterpoleFrame);
        } else {
            MR::startBckPlayer("TrapezeSwingBackEnd", ::sAnimInterpoleFrame);
        }
    }

    if (updateSwing()) {
        return;
    }

    if (mSwingVel >= 0.0f) {
        if (isNerve(GET_NERVE(Trapeze, TrapezeNrvSlowSwingBackEnd))) {
            setNerve(GET_NERVE(Trapeze, TrapezeNrvSlowSwingFrontStart));
        } else {
            setNerve(GET_NERVE(Trapeze, TrapezeNrvSwingFrontStart));
        }
        return;
    }

    if (isStartSwingBack()) {
        setNerve(GET_NERVE(Trapeze, TrapezeNrvSwingBackStart));
        return;
    }

    if (isStartSwingFront() && isNerve(GET_NERVE(Trapeze, TrapezeNrvSlowSwingBackEnd))) {
        setNerve(GET_NERVE(Trapeze, TrapezeNrvSwingFrontStart));
    }
}

void Trapeze::updateHitSensor(HitSensor* pSensor) {
    if (MR::isSensorAutoRush(pSensor)) {
        if (isNerve(GET_NERVE(Trapeze, TrapezeNrvStop))) {
            pSensor->mPosition.set< f32 >(mPosition.x, MR::getPlayerPos()->y, mPosition.z);
            f32 yPos = mPosition.y;
            yPos = MR::clamp(pSensor->mPosition.y, yPos - mRopeLength, yPos);
            pSensor->mPosition.y = yPos;
            return;
        }

        if (isNerve(GET_NERVE(Trapeze, TrapezeNrvFree))) {
            MR::calcPerpendicFootToLineInside(&pSensor->mPosition, *MR::getPlayerPos(), mPosition, mStickPoint->mPosition);
            return;
        }
        return;
    }

    if (mRider != nullptr) {
        pSensor->mPosition.set(mRider->mPosition);
        pSensor->mPosition.add(mHangPoint->mUp * ::sSensorOffsetY);
    }
}

void Trapeze::attackSensor(HitSensor* pSender, HitSensor* pReceiver) {
    if (mRider != nullptr) {
        if (MR::tryGetItem(pSender, pReceiver)) {
            return;
        }
    }
}

bool Trapeze::receiveOtherMsg(u32 msg, HitSensor* pSender, HitSensor* pReceiver) {
    if (MR::isMsgAutoRushBegin(msg)) {
        if ((!isNerve(GET_NERVE(Trapeze, TrapezeNrvStop)) && !isNerve(GET_NERVE(Trapeze, TrapezeNrvFree))) || MR::isOnGroundPlayer() ||
            pSender->mHost->mPosition.y > mPosition.y) {
            return false;
        }

        TVec3f posDiff = pSender->mHost->mPosition;
        posDiff.sub(pReceiver->mPosition);

        if (MR::abs(posDiff.dot(mFront)) > ::sDistanceStartBindZ) {
            return false;
        }

        mRider = pSender->mHost;

        TVec3f grabPos = pSender->mHost->mPosition;
        grabPos.sub(mPosition);

        f32 dotUp = mUp.dot(grabPos);
        f32 dotFront = mFront.dot(grabPos);

        grabPos = mPosition + mUp * dotUp + mFront * dotFront;

        f32 coord = grabPos.distance(mPosition);
        coord = MR::clamp(coord, 0.0f, mRopeLength);
        mGrabCoord = coord;

        TVec3f frontVec;
        MR::getPlayerFrontVec(&frontVec);
        if (frontVec.dot(mFront) < 0.0f) {
            mSwingReverse = true;
        } else {
            mSwingReverse = false;
        }

        f32 projGrabPointVel = mHangPoint->mVelocity.dot(mFront);
        f32 vel = MR::getPlayerVelocity()->dot(mFront);

        TVec3f swingVel = mFront;
        if (MR::abs(vel) < 3.0f && 3.0f < projGrabPointVel) {  // is this a bug??
            vel = projGrabPointVel;
        }
        swingVel.scale(vel);

        TVec3f grabFront = mFront;
        if (MR::abs(mPosition.y - grabPos.y) < ::sTransDiffMinY) {
            grabFront.set< f32 >(0.0f, -1.0f, 0.0f);
        }

        mHangPoint->mFront.set(grabFront);
        mHangPoint->setInfo(grabPos, swingVel, mPosition, mGrabCoord);
        mHangPoint->mUp.set(mUp);

        MR::startBckPlayer("TrapezeWait", static_cast< const char* >(nullptr));
        MR::invalidateClipping(this);
        getSensor("body")->validate();
        getSensor("bind")->invalidate();

        MR::startSound(mRider, "SE_PV_CATCH");
        MR::startSound(mRider, "SE_PM_GRAB_OBJ");

        if (!MR::isActiveActorCamera(this, mCameraInfo) || MR::isActiveActorCamera(this, mCameraInfo)) {
            MR::startActorCameraNoTarget(this, mCameraInfo, -1);
        }

        setNerve(GET_NERVE(Trapeze, TrapezeNrvSwingSlideDownStart));
        return true;
    }

    if (MR::isMsgUpdateBaseMtx(msg)) {
        TVec3f side = mHangPoint->mSide;
        TVec3f front = mHangPoint->mFront;
        if (mSwingReverse) {
            side.scale(-1.0f);
            front.scale(-1.0f);
        }
        TPos3f mtx;
        mtx.setXYZDir(side, mHangPoint->mUp, front);
        mtx.setTrans(mHangPoint->mPosition);
        MR::setBaseTRMtx(mRider, mtx);
        return true;
    }

    if (MR::isMsgRushCancel(msg)) {
        mRider = nullptr;
        mGrabCoord = 0.0f;
        setNerve(GET_NERVE(Trapeze, TrapezeNrvFree));
        return true;
    }

    return false;
}

bool Trapeze::tryJump() {
    if (MR::testCorePadTriggerA(WPAD_CHAN0) || MR::testSystemTriggerA()) {
        TVec3f jumpVel = mHangPoint->mVelocity * ::sJumpSpeedRate;
        jumpVel.y = MR::clamp(jumpVel.y, ::sJumpSpeedMinY, ::sJumpSpeedMaxY);

        TVec3f grabVel(jumpVel.x, 0.0f, jumpVel.z);
        if (grabVel.squared() > 300.0f) {
            MR::normalize(&grabVel);
            grabVel.scale(::sJumpSpeedMaxXZ);
            jumpVel.x = grabVel.x;
            jumpVel.z = grabVel.z;
        } else if (grabVel.squared() < ::sJumpSpeedMinXZ * ::sJumpSpeedMinXZ && MR::isGamePadStickOperated(WPAD_CHAN0)) {
            TVec3f stick(0.0f, 0.0f, 0.0f);
            MR::calcWorldStickDirectionXZ(&stick, WPAD_CHAN0);
            if (stick.dot(mFront) > 0.0f) {
                jumpVel.x = mFront.x * ::sJumpSpeedMinXZ;
                jumpVel.z = mFront.z * ::sJumpSpeedMinXZ;
            } else {
                jumpVel.x = -mFront.x * ::sJumpSpeedMinXZ;
                jumpVel.z = -mFront.z * ::sJumpSpeedMinXZ;
            }
        }

        TVec3f frontDir = mFront;
        if (mSwingReverse) {
            frontDir.scale(-1.0f);
        }

        if (frontDir.dot(jumpVel) >= 0.0f) {
            MR::startBckPlayer("TrapezeFrontSpin", static_cast< const char* >(nullptr));
        } else {
            MR::startBckPlayer("TrapezeBackSpin", static_cast< const char* >(nullptr));
        }

        MR::setPlayerFrontTargetVec(frontDir, 1);
        MR::endActorCameraAtLanding(this, mCameraInfo, -1);

        getSensor("body")->invalidate();
        getSensor("bind")->validate();

        MR::startSound(mRider, "SE_PV_JUMP_JOY");
        MR::startSound(mRider, "SE_PM_JUMP_M");

        MR::endBindAndPlayerWeakGravityJump(this, jumpVel);
        mRider = nullptr;

        setNerve(GET_NERVE(Trapeze, TrapezeNrvFreeInvalid));
        return true;
    }

    return false;
}

void Trapeze::updateStick(const TVec3f& rAnchor, f32 length) {
    mStickPoint->addAccel(mGravity * ::sStickPointGravity);
    mStickPoint->strain(rAnchor, length);
    mStickPoint->updatePosAndAxis(mStickPoint->mFront, ::sStickPointFrictionRate);
}

void Trapeze::updateStickMtx() {
    mPosMtx.setXYZDir(mStickPoint->mSide, mStickPoint->mUp, mStickPoint->mFront);
    mPosMtx.setTrans(mStickPoint->mPosition);
}

void Trapeze::updateHangPoint() {
    if (mGrabCoord < mRopeLength) {
        mGrabCoord += ::sHangAccelRate;
        mGrabCoord = mGrabCoord >= mRopeLength ? mRopeLength : mGrabCoord;
    }

    mWasSwingFront = mIsSwingFront;
    mWasSwingBack = mIsSwingBack;
    mIsSwingFront = false;
    mIsSwingBack = false;

    if (MR::isGamePadStickOperated(WPAD_CHAN0)) {
        TVec3f stick(0.0f, 0.0f, 0.0f);
        MR::calcWorldStickDirectionXZ(&stick, WPAD_CHAN0);
        TVec3f frontAccel = mFront;
        if (stick.dot(mFront) < 0.0f) {
            frontAccel.scale(-1.0f);
        }
        MR::vecKillElement(frontAccel, mSide, &frontAccel);
        frontAccel.scale(::sHangCoordSpeed);
        mHangPoint->addAccel(frontAccel);

        if (frontAccel.dot(mFront) > 0.0f) {
            if (mSwingReverse) {
                mIsSwingBack = true;
            } else {
                mIsSwingFront = true;
            }
        } else {
            if (mSwingReverse) {
                mIsSwingFront = true;
            } else {
                mIsSwingBack = true;
            }
        }
    }

    mHangPoint->addAccel(mGravity * ::sHangPointGravity);
    mHangPoint->strain(mPosition, mGrabCoord);
    MR::vecKillElement(mHangPoint->mVelocity, mSide, &mHangPoint->mVelocity);
    mHangPoint->updatePosAndAxis(mHangPoint->mFront, ::sHangPointFrictionRate);

    mSwingVel = mHangPoint->mVelocity.dot(mHangPoint->mFront);
    if (mSwingReverse) {
        mSwingVel *= -1.0f;
    }

    s32 soundLvl = (mHangPoint->mVelocity.length() / 40.0f) * 100.0f;
    if (soundLvl > 100) {
        soundLvl = 100;
    }
    if (soundLvl < 0) {
        soundLvl = 0;
    }
    MR::startLevelSound(this, "SE_OJ_LV_ROPE_SWING_WIND_1", soundLvl);
    MR::startLevelSound(this, "SE_OJ_LV_ROPE_SWING_WIND_2", soundLvl);

    if (mPrevSoundLvl < 20 && 20 <= soundLvl) {
        MR::startSound(this, "SE_OJ_ROPE_CREAK_H");
    }
    mPrevSoundLvl = soundLvl;
}

bool Trapeze::updateBind() {
    if (tryJump()) {
        return true;
    }

    updateHangPoint();
    mPosMtx.identity();

    if (mGrabCoord < mRopeLength) {
        updateStick(mHangPoint->mPosition, mRopeLength - mGrabCoord);
    } else {
        mStickPoint->setPosAndAxis(mHangPoint->mPosition, mHangPoint->mSide, mHangPoint->mUp, mHangPoint->mFront);
        TVec3f vel(0.0f, 0.0f, 0.0f);
        mStickPoint->mVelocity.set(vel);
    }

    updateStickMtx();
    return false;
}

bool Trapeze::updateSwing() {
    if (updateBind()) {
        return true;
    }

    if (!mIsSwingFront && !mIsSwingBack && MR::abs(mSwingVel) < ::sSpeedToSwingWait && mHangPoint->mUp.y >= ::sAxisToSwingWait) {
        setNerve(GET_NERVE(Trapeze, TrapezeNrvSwingWait));
        return true;
    }

    return false;
}

bool Trapeze::updateSlideDown() {
    if (updateSwing()) {
        return true;
    }

    MR::startLevelSound(mRider, "SE_OJ_LV_SLIDE_ROPE_SLIDE");

    if (mGrabCoord == mRopeLength) {
        if (MR::isGreaterStep(this, ::sStepSlideToRumble)) {
            MR::startSound(mRider, "SE_PM_GRAB_OBJ");
        }

        if (mSwingVel > 0) {
            setNerve(GET_NERVE(Trapeze, TrapezeNrvSwingFrontStart));
        } else {
            setNerve(GET_NERVE(Trapeze, TrapezeNrvSwingBackStart));
        }
        return true;
    }

    return false;
}

bool Trapeze::isStartSwingFront() const {
    if (mIsSwingFront && !mWasSwingFront) {
        return true;
    }

    return false;
}

bool Trapeze::isStartSwingBack() const {
    if (mIsSwingBack && !mWasSwingBack) {
        return true;
    }

    return false;
}

namespace {
    void drawPolygon0(const TVec3f& rPosA, const TVec3f& rPosB, const TVec3f& rSide, const TVec3f& rFront, f32 x1, f32 y1, f32 x2, f32 y2, f32 texA,
                      f32 texB) {
        GXBegin(GX_TRIANGLESTRIP, GX_VTXFMT0, 4);
        {
            GXPosition3f32(rPosA.x - x1 * rSide.x + y1 * rFront.x,   //
                           rPosA.y - x1 * rSide.y + y1 * rFront.y,   //
                           rPosA.z - x1 * rSide.z + y1 * rFront.z);  //
            GXColor1u32(::sColorPlusZ);
            GXTexCoord2f32(0.0f, texA);

            GXPosition3f32(rPosA.x + x2 * rSide.x - y2 * rFront.x,   //
                           rPosA.y + x2 * rSide.y - y2 * rFront.y,   //
                           rPosA.z + x2 * rSide.z - y2 * rFront.z);  //
            GXColor1u32(::sColorPlusX);
            GXTexCoord2f32(1.0f, texA);

            GXPosition3f32(rPosB.x - x1 * rSide.x + y1 * rFront.x,   //
                           rPosB.y - x1 * rSide.y + y1 * rFront.y,   //
                           rPosB.z - x1 * rSide.z + y1 * rFront.z);  //
            GXColor1u32(::sColorPlusZ);
            GXTexCoord2f32(0.0f, texB);

            GXPosition3f32(rPosB.x + x2 * rSide.x - y2 * rFront.x,   //
                           rPosB.y + x2 * rSide.y - y2 * rFront.y,   //
                           rPosB.z + x2 * rSide.z - y2 * rFront.z);  //
            GXColor1u32(::sColorPlusX);
            GXTexCoord2f32(1.0f, texB);
        }
        GXEnd();
    }

    void drawPolygon1(const TVec3f& rPosA, const TVec3f& rPosB, const TVec3f& rSide, const TVec3f& rFront, f32 x1, f32 y1, f32 x2, f32 y2, f32 texA,
                      f32 texB) {
        GXBegin(GX_TRIANGLESTRIP, GX_VTXFMT0, 4);
        {
            GXPosition3f32(rPosA.x - x1 * rSide.x - y1 * rFront.x,   //
                           rPosA.y - x1 * rSide.y - y1 * rFront.y,   //
                           rPosA.z - x1 * rSide.z - y1 * rFront.z);  //
            GXColor1u32(::sColorMinusX);
            GXTexCoord2f32(0.0f, texA);

            GXPosition3f32(rPosA.x + x2 * rSide.x + y2 * rFront.x,   //
                           rPosA.y + x2 * rSide.y + y2 * rFront.y,   //
                           rPosA.z + x2 * rSide.z + y2 * rFront.z);  //
            GXColor1u32(::sColorPlusZ);
            GXTexCoord2f32(1.0f, texA);

            GXPosition3f32(rPosB.x - x1 * rSide.x - y1 * rFront.x,   //
                           rPosB.y - x1 * rSide.y - y1 * rFront.y,   //
                           rPosB.z - x1 * rSide.z - y1 * rFront.z);  //
            GXColor1u32(::sColorMinusX);
            GXTexCoord2f32(0.0f, texB);

            GXPosition3f32(rPosB.x + x2 * rSide.x + y2 * rFront.x,   //
                           rPosB.y + x2 * rSide.y + y2 * rFront.y,   //
                           rPosB.z + x2 * rSide.z + y2 * rFront.z);  //
            GXColor1u32(::sColorPlusZ);
            GXTexCoord2f32(1.0f, texB);
        }
        GXEnd();
    }

    void drawPolygon2(const TVec3f& rPosA, const TVec3f& rPosB, const TVec3f& rSide, const TVec3f& rFront, f32 x1, f32 y1, f32 x2, f32 y2, f32 texA,
                      f32 texB) {
        GXBegin(GX_TRIANGLESTRIP, GX_VTXFMT0, 4);
        {
            GXPosition3f32(rPosA.x + x1 * rSide.x - y1 * rFront.x,   //
                           rPosA.y + x1 * rSide.y - y1 * rFront.y,   //
                           rPosA.z + x1 * rSide.z - y1 * rFront.z);  //
            GXColor1u32(::sColorPlusX);
            GXTexCoord2f32(0.0f, texA);

            GXPosition3f32(rPosA.x - x2 * rSide.x - y2 * rFront.x,   //
                           rPosA.y - x2 * rSide.y - y2 * rFront.y,   //
                           rPosA.z - x2 * rSide.z - y2 * rFront.z);  //
            GXColor1u32(::sColorMinusX);
            GXTexCoord2f32(1.0f, texA);

            GXPosition3f32(rPosB.x + x1 * rSide.x - y1 * rFront.x,   //
                           rPosB.y + x1 * rSide.y - y1 * rFront.y,   //
                           rPosB.z + x1 * rSide.z - y1 * rFront.z);  //
            GXColor1u32(::sColorPlusX);
            GXTexCoord2f32(0.0f, texB);

            GXPosition3f32(rPosB.x - x2 * rSide.x - y2 * rFront.x,   //
                           rPosB.y - x2 * rSide.y - y2 * rFront.y,   //
                           rPosB.z - x2 * rSide.z - y2 * rFront.z);  //
            GXColor1u32(::sColorMinusX);
            GXTexCoord2f32(1.0f, texB);
        }
        GXEnd();
    }
}  // namespace

void Trapeze::drawRope(const TVec3f& rPosA, const TVec3f& rPosB, const TVec3f& rSide, const TVec3f& rFront, f32 texA, f32 texB) const {
    ::drawPolygon0(rPosA, rPosB, rSide, rFront, ::sDrawWidthX + ::sDrawWidthShortOffsetX, ::sDrawWidthZ + ::sDrawWidthShortOffsetZ,
                   ::sDrawWidthX + ::sDrawWidthLongOffsetX, ::sDrawWidthZ + ::sDrawWidthShortOffsetZ, texA + ::sTexRateDiff0, texB + ::sTexRateDiff0);

    ::drawPolygon1(rPosA, rPosB, rSide, rFront, ::sDrawWidthX + ::sDrawWidthLongOffsetX, ::sDrawWidthZ + ::sDrawWidthShortOffsetZ,
                   ::sDrawWidthX + ::sDrawWidthShortOffsetX, ::sDrawWidthZ + ::sDrawWidthShortOffsetZ, texA + ::sTexRateDiff1,
                   texB + ::sTexRateDiff1);

    ::drawPolygon2(rPosA, rPosB, rSide, rFront, ::sDrawWidthX + ::sDrawWidthLongOffsetX, ::sDrawWidthShortOffsetZ,
                   ::sDrawWidthX + ::sDrawWidthLongOffsetX, ::sDrawWidthShortOffsetZ, texA + ::sTexRateDiff2, texB + ::sTexRateDiff2);
}

TrapezeRopeDrawInit::TrapezeRopeDrawInit(const char* pName) : NameObj(pName) {
    mTexture = nullptr;
    mTexture = new JUTTexture(MR::loadTexFromArc("Trapeze.arc", "TrapezeRope.bti"), 0);

    MR::registerPreDrawFunction(MR::Functor(this, &TrapezeRopeDrawInit::initDraw), MR::DrawType_Trapeze);
}

void TrapezeRopeDrawInit::initDraw() const {
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_F32, 0);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_CLR0, GX_POS_XYZ, GX_RGBA8, 0);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX0, GX_POS_XYZ, GX_F32, 0);

    GXClearVtxDesc();
    GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
    GXSetVtxDesc(GX_VA_CLR0, GX_DIRECT);
    GXSetVtxDesc(GX_VA_TEX0, GX_DIRECT);

    GXLoadPosMtxImm(MR::getCameraViewMtx(), 0);
    GXSetCurrentMtx(0);

    GXSetNumChans(1);
    GXSetChanCtrl(GX_COLOR0A0, GX_DISABLE, GX_SRC_VTX, GX_SRC_VTX, 0, GX_DF_NONE, GX_AF_NONE);
    GXSetChanCtrl(GX_COLOR1A1, GX_DISABLE, GX_SRC_REG, GX_SRC_REG, 0, GX_DF_NONE, GX_AF_NONE);

    GXSetNumTexGens(1);
    GXSetTexCoordGen2(GX_TEXCOORD0, GX_TG_MTX3x4, GX_TG_TEX0, 60, GX_FALSE, 125);
    mTexture->load(GX_TEXMAP0);

    GXSetNumTevStages(1);
    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR0A0);
    GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_ZERO, GX_CC_TEXC, GX_CC_RASC, GX_CC_ZERO);
    GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_FALSE, GX_TEVPREV);
    GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_TEXA, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO);
    GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_FALSE, GX_TEVPREV);

    GXSetBlendMode(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_INVSRCALPHA, GX_LO_NOOP);
    GXSetAlphaCompare(GX_GREATER, 0, GX_AOP_OR, GX_GREATER, 0);

    GXSetZMode(GX_ENABLE, GX_LEQUAL, GX_ENABLE);
    GXSetZCompLoc(GX_DISABLE);
    GXSetCullMode(GX_CULL_NONE);
}

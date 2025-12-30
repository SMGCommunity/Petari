#include "Game/Ride/Trapeze.hpp"
#include "Game/LiveActor/HitSensor.hpp"
#include "Game/LiveActor/PartsModel.hpp"
#include "Game/Ride/SwingRopePoint.hpp"
#include "Game/Scene/SceneFunction.hpp"
#include "Game/Util/ActorCameraUtil.hpp"
#include "Game/Util/ActorSensorUtil.hpp"
#include "Game/Util/ActorShadowUtil.hpp"
#include "Game/Util/DemoUtil.hpp"
#include "Game/Util/GamePadUtil.hpp"
#include "Game/Util/JMapInfo.hpp"
#include "Game/Util/LiveActorUtil.hpp"
#include "Game/Util/MathUtil.hpp"
#include "Game/Util/PlayerUtil.hpp"
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

namespace {
    static Color8 sColorPlusZ(0xFF, 0xFF, 0xFF, 0xFF);
    static Color8 sColorPlusX(0xB4, 0xB4, 0xB4, 0xFF);
    static Color8 sColorMinusX(0x64, 0x64, 0x64, 0xFF);
}  // namespace

Trapeze::Trapeze(const char* pName)
    : LiveActor(pName), mSide(1.0f, 0.0f, 0.0f), mUp(0.0f, 1.0f, 0.0f), mFront(0.0f, 0.0f, 1.0f), mRopeLength(0.0f), mSwingPoint(nullptr),
      mTrapezeModel(nullptr), mRider(nullptr), mGrabPoint(nullptr), mGrabCoord(0.0f), mSwingVel(0.0f), mSwingReverse(false), mIsSwingFront(false),
      mIsSwingBack(false), mWasSwingFront(false), mWasSwingBack(false), mCameraInfo(nullptr), mPrevSoundLvl(0) {
    mPosMtx.identity();
}

void Trapeze::init(const JMapInfoIter& rIter) {
    MR::createSceneObj(SceneObj_TrapezeRopeDrawInit);
    MR::connectToScene(this, MR::MovementType_Ride, -1, -1, MR::DrawType_Trapeze);
    MR::initDefaultPos(this, rIter);

    TPos3f mtx;
    mtx.identity();
    MR::makeMtxTR(reinterpret_cast< MtxPtr >(&mtx), this);

    mtx.getXDirInline(mSide);
    mtx.getYDirInline(mUp);
    mtx.getZDirInline(mFront);

    mRopeLength = mScale.y * 100.0f;
    mScale.set(1.0f, 1.0f, 1.0f);

    TVec3f spawnPos(mPosition.x, mPosition.y - mRopeLength, mPosition.z);
    mSwingPoint = new SwingRopePoint(spawnPos);
    mGrabPoint = new SwingRopePoint(mPosition);

    mSwingPoint->updatePosAndAxis(mFront, 0.995f);
    mGrabPoint->updatePosAndAxis(mFront, 0.995f);

    initNerve(&NrvTrapeze::TrapezeNrvFree::sInstance);

    initHitSensor(2);
    MR::addHitSensorCallbackBinder(this, "bind", 8, 200.0f);
    MR::addHitSensorCallbackRide(this, "body", 8, 130.0f);

    MR::calcGravity(this);

    initSound(4, false);

    MR::setClippingTypeSphere(this, mRopeLength);

    mTrapezeModel = new PartsModel(this, "棒", "Trapeze", mPosMtx, 0x12, false);
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

    TVec3f left(mSide);
    left.scale(-60.0f);
    TVec3f right(mSide);
    right.scale(60.0f);

    TVec3f grabLeft(left);
    TVec3f grabRight(right);
    TVec3f swingLeft(left);
    TVec3f swingRight(right);

    left.add(mPosition);
    right.add(mPosition);
    grabLeft.add(mGrabPoint->mPosition);
    grabRight.add(mGrabPoint->mPosition);
    swingLeft.add(mSwingPoint->mPosition);
    swingRight.add(mSwingPoint->mPosition);

    if (mRider != nullptr) {
        drawRope(left, grabLeft, mGrabPoint->mSide, mGrabPoint->mFront, 0.0f, mGrabCoord * 0.003f);
        drawRope(right, grabRight, mGrabPoint->mSide, mGrabPoint->mFront, 0.0f, mGrabCoord * 0.003f);

        if (mGrabCoord < mRopeLength) {
            drawRope(grabLeft, swingLeft, mSwingPoint->mSide, mSwingPoint->mFront, mGrabCoord * 0.003f, mRopeLength * 0.003f);
            drawRope(grabRight, swingRight, mSwingPoint->mSide, mSwingPoint->mFront, mGrabCoord * 0.003f, mRopeLength * 0.003f);
        }
    } else {
        drawRope(left, swingLeft, mSwingPoint->mSide, mSwingPoint->mFront, 0.0f, mRopeLength * 0.003f);
        drawRope(right, swingRight, mSwingPoint->mSide, mSwingPoint->mFront, 0.0f, mRopeLength * 0.003f);
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

    if (isNerve(&NrvTrapeze::TrapezeNrvFree::sInstance)) {
        if (mSwingPoint->mVelocity.squared() < 1.0f && 1.0f - __fabsf(mSwingPoint->mUp.y) < 0.001f) {
            setNerve(&NrvTrapeze::TrapezeNrvStop::sInstance);
        }
    }
}

void Trapeze::exeFreeInvalid() {
    exeFree();
    TVec3f posDiffHoriz(*MR::getPlayerPos());
    posDiffHoriz.sub(mSwingPoint->mPosition);
    posDiffHoriz.y = 0.0f;

    if (MR::isOnGroundPlayer() || MR::isGreaterStep(this, 40) || posDiffHoriz.squared() > 250000.0f) {
        setNerve(&NrvTrapeze::TrapezeNrvFree::sInstance);
    }
}

void Trapeze::exeSwingWait() {
    if (MR::isFirstStep(this)) {
        MR::startBckPlayer("TrapezeWait", 12);
    }

    if (updateBind()) {
        return;
    }

    if (mIsSwingFront) {
        setNerve(&NrvTrapeze::TrapezeNrvSwingFrontStart::sInstance);
    } else if (mIsSwingBack) {
        setNerve(&NrvTrapeze::TrapezeNrvSwingBackStart::sInstance);
    }
}

void Trapeze::exeSwingSlideDownStart() {
    if (MR::isFirstStep(this)) {
        MR::startBckPlayer("TrapezeSlideDownStart", static_cast< const char* >(nullptr));
    }

    if (!updateSlideDown() && MR::isBckStopped(mRider)) {
        setNerve(&NrvTrapeze::TrapezeNrvSwingSlideDown::sInstance);
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
        if (isNerve(&NrvTrapeze::TrapezeNrvSlowSwingFrontStart::sInstance)) {
            MR::startBckPlayer("TrapezeSlowSwingFrontStart", 12);
        } else {
            MR::startBckPlayer("TrapezeSwingFrontStart", 12);
        }
    }

    if (updateSwing()) {
        return;
    }

    if (MR::isBckStopped(mRider) && !(mIsSwingFront && mSwingVel < 0.0f)) {
        if (!mIsSwingFront && mSwingVel < 80.0f) {
            setNerve(&NrvTrapeze::TrapezeNrvSlowSwingFrontEnd::sInstance);
            return;
        }

        setNerve(&NrvTrapeze::TrapezeNrvSwingFrontEnd::sInstance);
        return;
    }

    if (isStartSwingFront() && isNerve(&NrvTrapeze::TrapezeNrvSlowSwingFrontStart::sInstance)) {
        setNerve(&NrvTrapeze::TrapezeNrvSwingFrontStart::sInstance);
        return;
    }

    if (isStartSwingBack()) {
        setNerve(&NrvTrapeze::TrapezeNrvSwingBackStart::sInstance);
        return;
    }

    if (!mIsSwingFront && mSwingVel < 0.0f) {
        if (isNerve(&NrvTrapeze::TrapezeNrvSlowSwingFrontStart::sInstance)) {
            setNerve(&NrvTrapeze::TrapezeNrvSlowSwingBackEnd::sInstance);
        } else {
            setNerve(&NrvTrapeze::TrapezeNrvSwingBackEnd::sInstance);
        }
    }
}

void Trapeze::exeSwingFrontEnd() {
    if (MR::isFirstStep(this)) {
        if (isNerve(&NrvTrapeze::TrapezeNrvSlowSwingFrontEnd::sInstance)) {
            MR::startBckPlayer("TrapezeSlowSwingFrontEnd", 12);
        } else {
            MR::startBckPlayer("TrapezeSwingFrontEnd", 12);
        }
    }

    if (updateSwing()) {
        return;
    }

    if (mSwingVel <= 0.0f) {
        if (isNerve(&NrvTrapeze::TrapezeNrvSlowSwingFrontEnd::sInstance)) {
            setNerve(&NrvTrapeze::TrapezeNrvSlowSwingBackStart::sInstance);
        } else {
            setNerve(&NrvTrapeze::TrapezeNrvSwingBackStart::sInstance);
        }
        return;
    }

    if (isStartSwingFront()) {
        setNerve(&NrvTrapeze::TrapezeNrvSwingFrontStart::sInstance);
        return;
    }

    if (isStartSwingBack() && isNerve(&NrvTrapeze::TrapezeNrvSlowSwingFrontEnd::sInstance)) {
        setNerve(&NrvTrapeze::TrapezeNrvSwingBackStart::sInstance);
    }
}

void Trapeze::exeSwingBackStart() {
    if (MR::isFirstStep(this)) {
        if (isNerve(&NrvTrapeze::TrapezeNrvSlowSwingBackStart::sInstance)) {
            MR::startBckPlayer("TrapezeSlowSwingBackStart", 12);
        } else {
            MR::startBckPlayer("TrapezeSwingBackStart", 12);
        }
    }

    if (updateSwing()) {
        return;
    }

    if (MR::isBckStopped(mRider) && !(mIsSwingBack && mSwingVel > 0.0f)) {
        if (!mIsSwingBack && mSwingVel > -80.0f) {
            setNerve(&NrvTrapeze::TrapezeNrvSlowSwingBackEnd::sInstance);
            return;
        }

        setNerve(&NrvTrapeze::TrapezeNrvSwingBackEnd::sInstance);
        return;
    }

    if (isStartSwingBack() && isNerve(&NrvTrapeze::TrapezeNrvSlowSwingBackStart::sInstance)) {
        setNerve(&NrvTrapeze::TrapezeNrvSwingBackStart::sInstance);
        return;
    }

    if (isStartSwingFront()) {
        setNerve(&NrvTrapeze::TrapezeNrvSwingFrontStart::sInstance);
        return;
    }

    if (!mIsSwingBack && mSwingVel > 0.0f) {
        if (isNerve(&NrvTrapeze::TrapezeNrvSlowSwingBackStart::sInstance)) {
            setNerve(&NrvTrapeze::TrapezeNrvSlowSwingFrontEnd::sInstance);
        } else {
            setNerve(&NrvTrapeze::TrapezeNrvSwingFrontEnd::sInstance);
        }
    }
}

void Trapeze::exeSwingBackEnd() {
    if (MR::isFirstStep(this)) {
        if (isNerve(&NrvTrapeze::TrapezeNrvSlowSwingBackEnd::sInstance)) {
            MR::startBckPlayer("TrapezeSlowSwingBackEnd", 12);
        } else {
            MR::startBckPlayer("TrapezeSwingBackEnd", 12);
        }
    }

    if (updateSwing()) {
        return;
    }

    if (mSwingVel >= 0.0f) {
        if (isNerve(&NrvTrapeze::TrapezeNrvSlowSwingBackEnd::sInstance)) {
            setNerve(&NrvTrapeze::TrapezeNrvSlowSwingFrontStart::sInstance);
        } else {
            setNerve(&NrvTrapeze::TrapezeNrvSwingFrontStart::sInstance);
        }
        return;
    }

    if (isStartSwingBack()) {
        setNerve(&NrvTrapeze::TrapezeNrvSwingBackStart::sInstance);
        return;
    }

    if (isStartSwingFront() && isNerve(&NrvTrapeze::TrapezeNrvSlowSwingBackEnd::sInstance)) {
        setNerve(&NrvTrapeze::TrapezeNrvSwingFrontStart::sInstance);
    }
}

void Trapeze::updateHitSensor(HitSensor* pSensor) {
    if (MR::isSensorAutoRush(pSensor)) {
        if (isNerve(&NrvTrapeze::TrapezeNrvStop::sInstance)) {
            pSensor->mPosition.set< f32 >(mPosition.x, MR::getPlayerPos()->y, mPosition.z);
            f32 yPos = mPosition.y;
            yPos = MR::clamp(pSensor->mPosition.y, yPos - mRopeLength, yPos);
            pSensor->mPosition.y = yPos;
            return;
        }

        if (isNerve(&NrvTrapeze::TrapezeNrvFree::sInstance)) {
            MR::calcPerpendicFootToLineInside(&pSensor->mPosition, *MR::getPlayerPos(), mPosition, mSwingPoint->mPosition);
            return;
        }
        return;
    }

    if (mRider != nullptr) {
        pSensor->mPosition.set(mRider->mPosition);
        TVec3f down(mGrabPoint->mUp);
        down.scale(-70.0f);
        pSensor->mPosition.add(down);
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
        if ((!isNerve(&NrvTrapeze::TrapezeNrvStop::sInstance) && !isNerve(&NrvTrapeze::TrapezeNrvFree::sInstance)) || MR::isOnGroundPlayer() ||
            pSender->mHost->mPosition.y > mPosition.y) {
            return false;
        }

        TVec3f posDiff(pSender->mHost->mPosition);
        posDiff.sub(pReceiver->mPosition);

        if (__fabsf(posDiff.dot(mFront)) > 80.0f) {
            return false;
        }

        mRider = pSender->mHost;

        TVec3f grabPos(pSender->mHost->mPosition);
        grabPos.sub(mPosition);

        f32 dotUp = mUp.dot(grabPos);
        f32 dotFront = mFront.dot(grabPos);

        grabPos.setPS2(mPosition.addOperatorInLine(mUp.scaleInline(dotUp)).addOperatorInLine(mFront.scaleInline(dotFront)));

        f32 coord = PSVECDistance(&grabPos, &mPosition);
        coord = MR::clamp(coord, 0.0f, mRopeLength);
        mGrabCoord = coord;

        TVec3f frontVec;
        MR::getPlayerFrontVec(&frontVec);
        if (frontVec.dot(mFront) < 0.0f) {
            mSwingReverse = true;
        } else {
            mSwingReverse = false;
        }

        f32 projGrabPointVel = mGrabPoint->mVelocity.dot(mFront);
        f32 vel = MR::getPlayerVelocity()->dot(mFront);

        TVec3f swingVel(mFront);
        if (__fabsf(vel) < 3.0f && 3.0f < projGrabPointVel) {  // is this a bug??
            vel = projGrabPointVel;
        }
        swingVel.scale(vel);

        TVec3f grabFront(mFront);
        if (__fabsf(mPosition.y - grabPos.y) < 1.0f) {
            grabFront.set< f32 >(0.0f, -1.0f, 0.0f);
        }

        mGrabPoint->mFront.set(grabFront);
        mGrabPoint->setInfo(grabPos, swingVel, mPosition, mGrabCoord);
        mGrabPoint->mUp.set(mUp);

        MR::startBckPlayer("TrapezeWait", static_cast< const char* >(nullptr));
        MR::invalidateClipping(this);
        getSensor("body")->validate();
        getSensor("bind")->invalidate();

        MR::startSound(mRider, "SE_PV_CATCH", -1, -1);
        MR::startSound(mRider, "SE_PM_GRAB_OBJ", -1, -1);

        if (!MR::isActiveActorCamera(this, mCameraInfo) || MR::isActiveActorCamera(this, mCameraInfo)) {
            MR::startActorCameraNoTarget(this, mCameraInfo, -1);
        }

        setNerve(&NrvTrapeze::TrapezeNrvSwingSlideDownStart::sInstance);
        return true;
    }

    if (MR::isMsgUpdateBaseMtx(msg)) {
        TVec3f side(mGrabPoint->mSide);
        TVec3f front(mGrabPoint->mFront);
        if (mSwingReverse) {
            side.scale(-1.0f);
            front.scale(-1.0f);
        }
        TPos3f mtx;
        mtx.setXYZDir(side, mGrabPoint->mUp, front);
        mtx.setTrans(mGrabPoint->mPosition);
        MR::setBaseTRMtx(mRider, mtx);
        return true;
    }

    if (MR::isMsgRushCancel(msg)) {
        mRider = nullptr;
        mGrabCoord = 0.0f;
        setNerve(&NrvTrapeze::TrapezeNrvFree::sInstance);
        return true;
    }

    return false;
}

bool Trapeze::tryJump() {
    if (MR::testCorePadTriggerA(WPAD_CHAN0) || MR::testSystemTriggerA()) {
        TVec3f jumpVel(mGrabPoint->mVelocity);
        jumpVel.scale(1.0f);
        jumpVel.y = MR::clamp(jumpVel.y, 20.0f, 50.0f);

        TVec3f grabVel(jumpVel.x, 0.0f, jumpVel.z);
        if (grabVel.squared() > 300.0f) {
            MR::normalize(&grabVel);
            grabVel.scale(20.0f);
            jumpVel.x = grabVel.x;
            jumpVel.z = grabVel.z;
        } else if (grabVel.squared() < 225.0f && MR::isGamePadStickOperated(WPAD_CHAN0)) {
            TVec3f stick(0.0f, 0.0f, 0.0f);
            MR::calcWorldStickDirectionXZ(&stick, WPAD_CHAN0);
            if (stick.dot(mFront) > 0.0f) {
                jumpVel.x = mFront.x * 15.0f;
                jumpVel.z = mFront.z * 15.0f;
            } else {
                jumpVel.x = -mFront.x * 15.0f;
                jumpVel.z = -mFront.z * 15.0f;
            }
        }

        TVec3f frontDir(mFront);
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

        MR::startSound(mRider, "SE_PV_JUMP_JOY", -1, -1);
        MR::startSound(mRider, "SE_PM_JUMP_M", -1, -1);

        MR::endBindAndPlayerWeakGravityJump(this, jumpVel);
        mRider = nullptr;

        setNerve(&NrvTrapeze::TrapezeNrvFreeInvalid::sInstance);
        return true;
    }

    return false;
}

void Trapeze::updateStick(const TVec3f& rAnchor, f32 length) {
    TVec3f grav(mGravity);
    grav.scale(1.0f);
    mSwingPoint->addAccel(grav);
    mSwingPoint->strain(rAnchor, length);
    mSwingPoint->updatePosAndAxis(mSwingPoint->mFront, 0.995f);
}

void Trapeze::updateStickMtx() {
    mPosMtx.setXYZDir(mSwingPoint->mSide, mSwingPoint->mUp, mSwingPoint->mFront);
    mPosMtx.setTrans(mSwingPoint->mPosition);
}

void Trapeze::updateHangPoint() {
    if (mGrabCoord < mRopeLength) {
        mGrabCoord += 5.0f;
        mGrabCoord = mGrabCoord >= mRopeLength ? mRopeLength : mGrabCoord;
    }

    mWasSwingFront = mIsSwingFront;
    mWasSwingBack = mIsSwingBack;
    mIsSwingFront = false;
    mIsSwingBack = false;

    if (MR::isGamePadStickOperated(WPAD_CHAN0)) {
        TVec3f stick(0.0f, 0.0f, 0.0f);
        MR::calcWorldStickDirectionXZ(&stick, WPAD_CHAN0);
        TVec3f frontAccel(mFront);
        if (stick.dot(mFront) < 0.0f) {
            frontAccel.scale(-1.0f);
        }
        MR::vecKillElement(frontAccel, mSide, &frontAccel);
        frontAccel.scale(0.5f);
        mGrabPoint->addAccel(frontAccel);

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

    TVec3f grav(mGravity);
    grav.scale(1.0f);
    mGrabPoint->addAccel(grav);
    mGrabPoint->strain(mPosition, mGrabCoord);
    MR::vecKillElement(mGrabPoint->mVelocity, mSide, &mGrabPoint->mVelocity);
    mGrabPoint->updatePosAndAxis(mGrabPoint->mFront, 0.995f);

    mSwingVel = mGrabPoint->mVelocity.dot(mGrabPoint->mFront);
    if (mSwingReverse) {
        mSwingVel *= -1.0f;
    }

    s32 soundLvl = (mGrabPoint->mVelocity.length() / 40.0f) * 100.0f;
    if (soundLvl > 100) {
        soundLvl = 100;
    }
    if (soundLvl < 0) {
        soundLvl = 0;
    }
    MR::startLevelSound(this, "SE_OJ_LV_ROPE_SWING_WIND_1", soundLvl, -1, -1);
    MR::startLevelSound(this, "SE_OJ_LV_ROPE_SWING_WIND_2", soundLvl, -1, -1);

    if (mPrevSoundLvl < 20 && 20 <= soundLvl) {
        MR::startSound(this, "SE_OJ_ROPE_CREAK_H", -1, -1);
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
        updateStick(mGrabPoint->mPosition, mRopeLength - mGrabCoord);
    } else {
        mSwingPoint->setPosAndAxis(mGrabPoint->mPosition, mGrabPoint->mSide, mGrabPoint->mUp, mGrabPoint->mFront);
        TVec3f vel(0.0f, 0.0f, 0.0f);
        mSwingPoint->mVelocity.set(vel);
    }

    updateStickMtx();
    return false;
}

bool Trapeze::updateSwing() {
    if (updateBind()) {
        return true;
    }

    if (!mIsSwingFront && !mIsSwingBack && __fabsf(mSwingVel) < 10.0f && mGrabPoint->mUp.y >= 0.99f) {
        setNerve(&NrvTrapeze::TrapezeNrvSwingWait::sInstance);
        return true;
    }

    return false;
}

bool Trapeze::updateSlideDown() {
    if (updateSwing()) {
        return true;
    }

    MR::startLevelSound(mRider, "SE_OJ_LV_SLIDE_ROPE_SLIDE", -1, -1, -1);

    if (mGrabCoord == mRopeLength) {
        if (MR::isGreaterStep(this, 20)) {
            MR::startSound(mRider, "SE_PM_GRAB_OBJ", -1, -1);
        }

        if (mSwingVel > 0) {
            setNerve(&NrvTrapeze::TrapezeNrvSwingFrontStart::sInstance);
        } else {
            setNerve(&NrvTrapeze::TrapezeNrvSwingBackStart::sInstance);
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

void Trapeze::drawRope(const TVec3f& rPosA, const TVec3f& rPosB, const TVec3f& rSide, const TVec3f& rFront, f32 texA, f32 texB) const {
    // FIXME: some math shenanigans going on here. Some of these match while others are stubborn.
    // https://decomp.me/scratch/IgiYF

    // strip 1
    GXBegin(GX_TRIANGLESTRIP, GX_VTXFMT0, 4);
    GXPosition3f32(rPosA.x - rSide.x * 12.0f + rFront.x * 19.0f, rPosA.y - rSide.y * 12.0f + rFront.y * 19.0f,
                   rPosA.z - rSide.z * 12.0f + rFront.z * 19.0f);
    GXColor1u32(::sColorPlusZ);
    GXTexCoord2f32(0.0f, texA);

    GXPosition3f32(rPosA.x + rSide.x * 19.0f - rFront.x * 19.0f, rPosA.y + rSide.y * 19.0f - rFront.y * 19.0f,
                   rPosA.z + rSide.z * 19.0f - rFront.z * 19.0f);
    GXColor1u32(::sColorPlusX);
    GXTexCoord2f32(1.0f, texA);

    GXPosition3f32(rPosB.x - rSide.x * 12.0f + rFront.x * 19.0f, rPosB.y - rSide.y * 12.0f + rFront.y * 19.0f,
                   rPosB.z - rSide.z * 12.0f + rFront.z * 19.0f);
    GXColor1u32(::sColorPlusZ);
    GXTexCoord2f32(0.0f, texB);

    GXPosition3f32(rPosB.x + rSide.x * 19.0f - rFront.x * 19.0f, rPosB.y + rSide.y * 19.0f - rFront.y * 19.0f,
                   rPosB.z + rSide.z * 19.0f - rFront.z * 19.0f);
    GXColor1u32(::sColorPlusX);
    GXTexCoord2f32(1.0f, texB);

    // strip 2
    GXBegin(GX_TRIANGLESTRIP, GX_VTXFMT0, 4);
    GXPosition3f32(rPosA.x - rSide.x * 19.0f - rFront.x * 19.0f, rPosA.y - rSide.y * 19.0f - rFront.y * 19.0f,
                   rPosA.z - rSide.z * 19.0f - rFront.z * 19.0f);
    GXColor1u32(::sColorMinusX);
    GXTexCoord2f32(0.0f, texA + 0.7f);

    GXPosition3f32(rPosA.x + rSide.x * 12.0f + rFront.x * 19.0f, rPosA.y + rSide.y * 12.0f + rFront.y * 19.0f,
                   rPosA.z + rSide.z * 12.0f + rFront.z * 19.0f);
    GXColor1u32(::sColorPlusZ);
    GXTexCoord2f32(1.0f, texA + 0.7f);

    GXPosition3f32(rPosB.x - rSide.x * 19.0f - rFront.x * 19.0f, rPosB.y - rSide.y * 19.0f - rFront.y * 19.0f,
                   rPosB.z - rSide.z * 19.0f - rFront.z * 19.0f);
    GXColor1u32(::sColorMinusX);
    GXTexCoord2f32(0.0f, texB + 0.7f);

    GXPosition3f32(rPosB.x + rSide.x * 12.0f + rFront.x * 19.0f, rPosB.y + rSide.y * 12.0f + rFront.y * 19.0f,
                   rPosB.z + rSide.z * 12.0f + rFront.z * 19.0f);
    GXColor1u32(::sColorPlusZ);
    GXTexCoord2f32(1.0f, texB + 0.7f);

    // strip 3
    GXBegin(GX_TRIANGLESTRIP, GX_VTXFMT0, 4);
    GXPosition3f32(rPosA.x + rSide.x * 19.0f - rFront.x * 7.0f, rPosA.y + rSide.y * 19.0f - rFront.y * 7.0f,
                   rPosA.z + rSide.z * 19.0f - rFront.z * 7.0f);
    GXColor1u32(::sColorPlusX);
    GXTexCoord2f32(0.0f, texA + 0.5f);

    GXPosition3f32(rPosA.x - rSide.x * 19.0f - rFront.x * 7.0f, rPosA.y - rSide.y * 19.0f - rFront.y * 7.0f,
                   rPosA.z - rSide.z * 19.0f - rFront.z * 7.0f);
    GXColor1u32(::sColorMinusX);
    GXTexCoord2f32(1.0f, texA + 0.5f);

    GXPosition3f32(rPosB.x + rSide.x * 19.0f - rFront.x * 7.0f, rPosB.y + rSide.y * 19.0f - rFront.y * 7.0f,
                   rPosB.z + rSide.z * 19.0f - rFront.z * 7.0f);
    GXColor1u32(::sColorPlusX);
    GXTexCoord2f32(0.0f, texB + 0.5f);

    GXPosition3f32(rPosB.x - rSide.x * 19.0f - rFront.x * 7.0f, rPosB.y - rSide.y * 19.0f - rFront.y * 7.0f,
                   rPosB.z - rSide.z * 19.0f - rFront.z * 7.0f);
    GXColor1u32(::sColorMinusX);
    GXTexCoord2f32(1.0f, texB + 0.5f);
}

TrapezeRopeDrawInit::TrapezeRopeDrawInit(const char* pName) : NameObj(pName) {
    mTexture = nullptr;
    mTexture = new JUTTexture(MR::loadTexFromArc("Trapeze.arc", "TrapezeRope.bti"));

    MR::FunctorV0M< const TrapezeRopeDrawInit*, void (TrapezeRopeDrawInit::*)() const > preDrawFunctor(this, &TrapezeRopeDrawInit::initDraw);
    MR::registerPreDrawFunction(preDrawFunctor, MR::DrawType_Trapeze);
    // The above should probably be this instead, but MR::Functor_Inline does not like consts at the moment
    // MR::registerPreDrawFunction(MR::Functor_Inline(const_cast< const TrapezeRopeDrawInit* >(this), &TrapezeRopeDrawInit::initDraw),
    // MR::DrawType_Trapeze);
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

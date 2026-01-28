#include "Game/Ride/Pole.hpp"
#include "Game/LiveActor/HitSensor.hpp"
#include "Game/Util/MathUtil.hpp"
#include "Game/Util/PlayerUtil.hpp"
#include <JSystem/JMath.hpp>
#include <cstring>

namespace NrvPole {
    NEW_NERVE(PoleNrvDemoAppear, Pole, DemoAppear);
    NEW_NERVE(PoleNrvFree, Pole, Free);
    NEW_NERVE(PoleNrvFreeInvalid, Pole, FreeInvalid);
    NEW_NERVE(PoleNrvBindStart, Pole, BindStart);
    NEW_NERVE(PoleNrvBindStartFast, Pole, BindStart);
    NEW_NERVE(PoleNrvBindWait, Pole, BindWait);
    NEW_NERVE(PoleNrvBindTurnLeft, Pole, BindTurn);
    NEW_NERVE(PoleNrvBindTurnRight, Pole, BindTurn);
    NEW_NERVE(PoleNrvBindTurnEnd, Pole, BindWait);
    NEW_NERVE(PoleNrvBindClimbUp, Pole, BindClimbUp);
    NEW_NERVE(PoleNrvBindFallDown, Pole, BindFallDown);
    NEW_NERVE(PoleNrvBindHandstandStart, Pole, BindHandstandStart);
    NEW_NERVE(PoleNrvBindHandstandWait, Pole, BindHandstandWait);
    NEW_NERVE(PoleNrvBindHandstandEnd, Pole, BindHandstandEnd);
    NEW_NERVE(PoleNrvBindHandstandTurn, Pole, BindHandstandTurn);
};  // namespace NrvPole

inline f32 modRotateFromBase(f32 angle, f32 base) {
    return static_cast< f32 >(fmod((angle - base) + 360.0f, 360.0f)) + base;
}

Pole::Pole(const char* pName)
    : LiveActor(pName), mBasePos(0.0f, 0.0f, 0.0f), mTopPos(0.0f, 0.0f, 0.0f), mPoleLength(0.0f), mDisableHandstand(false), mIsSquare(false),
      mNoModel(false), mInvertStick(false), mUsePole9mCollision(false), mClimbCoord(0.0f), mClimbSpeed(0.0f), mRider(nullptr), mCameraInfo(nullptr),
      mCenterPos(0.0f, 0.0f, 0.0f), mSide(1.0f, 0.0f, 0.0f), mUp(0.0f, 1.0f, 0.0f), mFront(0.0f, 0.0f, 1.0f), mTopMtx(nullptr), mBottomMtx(nullptr) {
    mPosMtx.identity();
    mBaseMtx.identity();
}

void Pole::init(const JMapInfoIter& rIter) {
    MR::initDefaultPos(this, rIter);
    mBasePos.set< f32 >(mPosition);
    mPoleLength = 100.0f * mScale.y;
    mScale.set(1.0f, 1.0f, 1.0f);

    MR::getJMapInfoArg0NoInit(rIter, &mDisableHandstand);
    MR::getJMapInfoArg1NoInit(rIter, &mInvertStick);
    s32 arg2 = -1;
    MR::getJMapInfoArg2NoInit(rIter, &arg2);

    if (arg2 == 1) {
        mUsePole9mCollision = true;
    }

    const char* name = nullptr;
    MR::getObjectName(&name, rIter);
    if (strstr(name, "NoModel")) {
        mNoModel = true;
    }
    if (strstr(name, "Square")) {
        mIsSquare = true;
        mDisableHandstand = true;
    }

    TPos3f mtx;
    mtx.identity();
    MR::makeMtxTR(mtx.toMtxPtr(), this);

    mtx.getXDirInline(mSide);
    mtx.getYDirInline(mUp);
    mtx.getZDirInline(mFront);

    mPosMtx.set(mtx);
    mPosMtx.invert(mPosMtx);
    mPosMtx.mMtx[0][3] = 0.0f;
    mPosMtx.mMtx[1][3] = 0.0f;
    mPosMtx.mMtx[2][3] = 0.0f;

    initHitSensor(3);
    MR::addHitSensorBinder(this, "bind", 8, 60.0f, TVec3f(0.0f, 0.0f, 0.0f));
    MR::addHitSensorRide(this, "ride", 8, 30.0f, TVec3f(0.0f, 50.0f, 0.0f));
    MR::addHitSensorPush(this, "push", 8, 100.0f, TVec3f(0.0f, 0.0f, 0.0f));

    if (!mNoModel) {
        initModelManagerWithAnm(name, nullptr, false);
        if (mUsePole9mCollision) {
            MR::initCollisionParts(this, "Pole9m", getSensor("ride"), nullptr);
        } else {
            MR::initCollisionParts(this, name, getSensor("ride"), nullptr);
        }
    } else {
        mBaseMtx.set(mtx);
        if (MR::isEqualString(name, "PoleNoModel")) {
            ResourceHolder* resource1 = MR::createAndAddResourceHolder("Pole.arc");
            MR::initCollisionPartsFromResourceHolder(this, "Pole", getSensor("ride"), resource1, mBaseMtx.toMtxPtr());
        } else {
            ResourceHolder* resource2 = MR::createAndAddResourceHolder("PoleSquare.arc");
            MR::initCollisionPartsFromResourceHolder(this, "PoleSquare", getSensor("ride"), resource2, mBaseMtx.toMtxPtr());
        }
    }

    if (MR::isEqualString(name, "Pole") || MR::isEqualString(name, "PoleSquare")) {
        mTopMtx = MR::getJointMtx(this, "PoleTop");
        mBottomMtx = MR::getJointMtx(this, "PoleBottom");
        if (MR::isEqualString(name, "Pole") && !mDisableHandstand) {
            MR::hideMaterial(this, "PoleTopStopMat_v");
        }
        Pole::updateTopPos(mPoleLength);
    } else {
        if (MR::isEqualString(name, "TreeCube")) {
            mPoleLength = 800.0f;
        }

        mTopPos.set< f32 >(mUp);
        mTopPos.scale(mPoleLength);
        mTopPos.add(mBasePos);

        if (!mNoModel) {
            mBottomMtx = MR::getJointMtx(this, "world_root");
        }
    }

    if (!mNoModel) {
        MR::setMtxAxisXYZ(mBottomMtx, mSide, mUp, mFront);
        MR::setMtxTrans(mBottomMtx, mBasePos);
    }

    if (mNoModel) {
        MR::connectToSceneMapObjMovement(this);
    } else {
        MR::connectToSceneMapObj(this);
    }

    initSound(4, false);
    MR::initActorCamera(this, rIter, &mCameraInfo);
    initNerve(&NrvPole::PoleNrvFree::sInstance);
    mCenterPos.set< f32 >(mUp);
    mCenterPos.scale(mPoleLength / 2.0f);
    mCenterPos.add(mBasePos);
    MR::setClippingTypeSphere(this, (mPoleLength / 2.0f), &mCenterPos);
    MR::useStageSwitchSleep(this, rIter);

    if (MR::tryRegisterDemoCast(this, rIter)) {
        makeActorDead();
    } else {
        makeActorAppeared();
    }
}

void Pole::appear() {
    LiveActor::appear();
    setNerve(&NrvPole::PoleNrvDemoAppear::sInstance);
}

void Pole::calcAnim() {
    if (!mNoModel) {
        MR::updateMaterial(this);
    }
}

void Pole::exeDemoAppear() {
    if (MR::isFirstStep(this)) {
        MR::invalidateHitSensors(this);
        MR::invalidateCollisionParts(this);
    }

    updateTopPos(mPoleLength * MR::calcNerveRate(this, 60));

    if (MR::isDemoActive()) {
        MR::startLevelSound(this, "SE_OJ_LV_POLE_APPEAR", -1, -1, -1);
    }

    if (MR::isStep(this, 60)) {
        Pole::updateTopPos(mPoleLength);
        MR::validateCollisionParts(this);
        MR::validateHitSensors(this);
        setNerve(&NrvPole::PoleNrvFree::sInstance);
    }
}

inline void Pole::exeFree() {
    MR::calcPerpendicFootToLineInside(&mPosition, *MR::getPlayerCenterPos(), mTopPos, mBasePos);
}

void Pole::exeFreeInvalid() {
    if (MR::isFirstStep(this)) {
        MR::invalidateHitSensors(this);
    }

    MR::calcPerpendicFootToLineInside(&mPosition, *MR::getPlayerCenterPos(), mTopPos, mBasePos);

    if (MR::isOnGroundPlayer() || MR::calcDistanceToPlayer(this) > 300.0f || MR::isGreaterStep(this, 45)) {
        MR::validateHitSensors(this);
        setNerve(&NrvPole::PoleNrvFree::sInstance);
    }
}

void Pole::exeBindStart() {
    if (MR::isFirstStep(this)) {
        if (mIsSquare) {
            MR::startBckPlayer("SquarePoleStart", static_cast< const char* >(nullptr));
        } else {
            MR::startBckPlayer("TreeClimbStart", static_cast< const char* >(nullptr));
        }
    }

    if (!mIsSquare) {
        f32 rotateSpeed = 5.0f;
        if (isNerve(&NrvPole::PoleNrvBindStartFast::sInstance)) {
            rotateSpeed = 9.0f;
        }

        s16 frame = MR::getBckCtrl(mRider)->mEnd;
        mRotation.y += (rotateSpeed * MR::getEaseOutValue(1.0f - ((f32)getNerveStep() / frame), 0.0f, 1.0f, 1.0f));
    }

    if (!tryJump(false, 0.0f) && MR::isBckStopped(mRider)) {
        setNerve(&NrvPole::PoleNrvBindWait::sInstance);
    }
}

void Pole::exeBindWait() {
    if (MR::isFirstStep(this)) {
        if (mIsSquare) {
            if (!MR::isBckPlaying(mRider, "SquarePoleWait")) {
                MR::startBckPlayer("SquarePoleWait", static_cast< const char* >(nullptr));
            }
        } else {
            MR::startBckPlayer("TreeWait", static_cast< const char* >(nullptr));
        }
        mClimbSpeed = 0.0f;
    }

    if (tryJump(false, 0.0f)) {
        return;
    }

    if (!isNerve(&NrvPole::PoleNrvBindTurnEnd::sInstance)) {
        if (tryTurn()) {
            return;
        }
    } else if (!isEnableTurn()) {
        setNerve(&NrvPole::PoleNrvBindWait::sInstance);
        return;
    }

    if (!tryClimbUp()) {
        if (tryFallDown()) {
            return;
        }
    }
}

void Pole::exeBindTurn() {
    if (MR::isFirstStep(this)) {
        if (mIsSquare) {
            if (isNerve(&NrvPole::PoleNrvBindTurnLeft::sInstance)) {
                MR::startBckPlayer("SquarePoleTurnL", static_cast< const char* >(nullptr));
            } else {
                MR::startBckPlayer("SquarePoleTurnR", static_cast< const char* >(nullptr));
            }
        } else {
            MR::startBckPlayer("ClimbTurn", static_cast< const char* >(nullptr));
        }
    }

    if (!mIsSquare) {
        mRotation.y += 2.5f * getPoleSubPadStickX();
        mRotation.y = modRotateFromBase(mRotation.y, 0.0f);
    }

    f32 jump = 0.0f;

    if (mIsSquare && MR::isGreaterStep(this, 7)) {
        if (isNerve(&NrvPole::PoleNrvBindTurnLeft::sInstance)) {
            jump = -90.0f;
        } else {
            jump = 90.0f;
        }
    }

    if (tryJump(false, jump)) {
        return;
    }

    if (mIsSquare) {
        if (MR::isBckStopped(mRider)) {
            if (isNerve(&NrvPole::PoleNrvBindTurnRight::sInstance)) {
                mRotation.y += 90.0f;
            } else {
                mRotation.y -= 90.0f;
            }

            mRotation.y = modRotateFromBase(mRotation.y, 0.0f);

            MR::startBckPlayer("SquarePoleWait", "SquarePoleTurnEnd");
            setNerve(&NrvPole::PoleNrvBindTurnEnd::sInstance);
        } else {
            return;
        }
    } else if (!isEnableTurn()) {
        setNerve(&NrvPole::PoleNrvBindWait::sInstance);
    }
}

void Pole::exeBindClimbUp() {
    if (MR::isFirstStep(this)) {
        if (mIsSquare) {
            MR::startBckPlayer("SquarePoleClimb", static_cast< const char* >(nullptr));
            MR::setBckRate(mRider, 1.6f);
        } else {
            MR::startBckPlayer("TreeClimb", static_cast< const char* >(nullptr));
            MR::setBckRate(mRider, 1.75f);
        }
        mClimbSpeed = 0.0f;
    }

    if (!tryJump(false, 0) && !tryTurn() && !tryFallDown()) {
        if (0.0f == getPoleSubPadStickY()) {
            setNerve(&NrvPole::PoleNrvBindWait::sInstance);
        } else {
            mClimbCoord += 6.0f;
            if (mClimbCoord > mPoleLength) {
                mClimbCoord = mPoleLength;

                if (!mDisableHandstand) {
                    updateBindTrans();
                    setNerve(&NrvPole::PoleNrvBindHandstandStart::sInstance);
                    return;
                } else if (mIsSquare) {
                    if (!MR::isBckPlaying(mRider, "SquarePolePushWait")) {
                        MR::startBckPlayer("SquarePolePushWait", static_cast< const char* >(nullptr));
                    }
                }

            } else {
                MR::tryRumblePadVeryWeak(this, WPAD_CHAN0);
            }
            updateBindTrans();
        }
    }
}

void Pole::exeBindFallDown() {
    if (MR::isFirstStep(this)) {
        if (mIsSquare) {
            MR::startBckPlayer("SquarePoleFall", static_cast< const char* >(nullptr));
        } else {
            MR::startBckPlayer("TreeFall", static_cast< const char* >(nullptr));
        }
    }

    if (mIsSquare) {
        mClimbSpeed -= 0.5f;
        mClimbSpeed = mClimbSpeed >= -18.0f ? mClimbSpeed : -18.0f;
    } else {
        mClimbSpeed -= 0.3f;
        mClimbSpeed = mClimbSpeed >= -15.0f ? mClimbSpeed : -15.0f;
    }

    mClimbCoord += mClimbSpeed;

    if (!mIsSquare) {
        mRotation.y += 0.8f * mClimbSpeed;
    }

    updateBindTrans();
    MR::tryRumblePadVeryWeak(this, WPAD_CHAN0);
    MR::startLevelSound(mRider, "SE_PM_LV_POLE_SLIDE", -1, -1, -1);

    if ((!mIsSquare && mClimbCoord <= 120.0f) || (mIsSquare && mClimbCoord <= 50.0f)) {
        TPos3f mtx;
        calcGravityMtx(&mtx);
        TVec3f front(0.0f, 0.0f, 0.0f);
        front.x = JMASinDegree(mRotation.y);
        front.z = JMACosDegree(mRotation.y);
        mtx.mult(front, front);

        TVec3f pos(front);
        pos.scale(-30.0f);
        pos.add(mBasePos);

        MR::setPlayerPos(pos);
        MR::setPlayerFrontTargetVec(front, 1);
        MR::endActorCamera(this, mCameraInfo, true, -1);
        MR::endBindAndPlayerWait(this);

        mRider = nullptr;
        setNerve(&NrvPole::PoleNrvFreeInvalid::sInstance);
        return;
    }

    if (!tryJump(false, 0.0f) && !tryTurn() && !tryClimbUp()) {
        if (0.0f == getPoleSubPadStickY()) {
            setNerve(&NrvPole::PoleNrvBindWait::sInstance);
        }
    }
}

void Pole::exeBindHandstandStart() {
    if (MR::isFirstStep(this)) {
        MR::startBckPlayer("TreeHandstandStart", static_cast< const char* >(nullptr));
        MR::startSound(mRider, "SE_PM_POLE_STAND", -1, -1);
        MR::startSound(mRider, "SE_PV_LIFT_UP", -1, -1);
    }

    if (MR::isLessStep(this, 25)) {
        if (Pole::tryJump(false, 0.0f)) {
            return;
        }
    } else {
        if (Pole::tryJump(true, 0.0f)) {
            return;
        }
    }

    if (!Pole::tryHandstandTurn()) {
        if (MR::isBckStopped(mRider)) {
            setNerve(&NrvPole::PoleNrvBindHandstandWait::sInstance);
        }
    }
}

void Pole::exeBindHandstandWait() {
    if (MR::isFirstStep(this)) {
        MR::startBckPlayer("TreeHandstandWait", static_cast< const char* >(nullptr));
    }

    if (!Pole::tryJump(true, 0.0f) && !Pole::tryHandstandTurn()) {
        if (getPoleSubPadStickY() < -0.8f) {
            setNerve(&NrvPole::PoleNrvBindHandstandEnd::sInstance);
        }
    }
}

void Pole::exeBindHandstandEnd() {
    if (MR::isFirstStep(this)) {
        MR::startBckPlayer("TreeHandstandEnd", static_cast< const char* >(nullptr));
    }

    if (MR::isLessStep(this, 25)) {
        if (tryJump(true, 0.0f)) {
            return;
        }
    } else if (tryJump(false, 0.0f)) {
        return;
    }

    if (!tryTurn()) {
        if (MR::isBckStopped(mRider)) {
            setNerve(&NrvPole::PoleNrvBindWait::sInstance);
        }
    }
}

void Pole::exeBindHandstandTurn() {
    if (MR::isFirstStep(this)) {
        MR::startBckPlayer("TreeHandstandTurn", static_cast< const char* >(nullptr));
    }

    mRotation.y += 2.5f * getPoleSubPadStickX();
    mRotation.y = modRotateFromBase(mRotation.y, 0.0f);

    if (!tryJump(true, (0.0f)) && !isEnableTurn()) {
        setNerve(&NrvPole::PoleNrvBindHandstandWait::sInstance);
    }
}

void Pole::attackSensor(HitSensor* pSender, HitSensor* pReceiver) {
    if (mRider && MR::isSensorRide(pSender)) {
        MR::tryGetItem(pSender, pReceiver);
        return;
    }

    if (MR::isSensorPush(pSender)) {
        switch (isNerve(&NrvPole::PoleNrvFreeInvalid::sInstance)) {
        case false:
            if (MR::isOnGroundPlayer()) {
                MR::sendMsgPush(pReceiver, pSender);
            }
            break;
        default:
            break;
        }
    }
}

bool Pole::receiveMsgEnemyAttack(u32 msg, HitSensor* pSender, HitSensor* pReceiver) {
    if (mRider && MR::isSensor(pReceiver, "bind")) {
        MR::endActorCamera(this, mCameraInfo, true, -1);
        MR::endBindAndPlayerDamageMsg(this, msg);

        mRider = nullptr;
        setNerve(&NrvPole::PoleNrvFreeInvalid::sInstance);
        return true;
    }
    return false;
}

bool Pole::receiveOtherMsg(u32 msg, HitSensor* pSender, HitSensor* pReceiver) {
    if (MR::isMsgIsRushTakeOver(msg)) {
        return true;
    }

    if (MR::isMsgAutoRushBegin(msg)) {
        if (MR::isOnGroundPlayer() || isNerve(&NrvPole::PoleNrvFreeInvalid::sInstance)) {
            return false;
        }

        TVec3f velHoriz(*MR::getPlayerVelocity());
        MR::vecKillElement(velHoriz, mUp, &velHoriz);
        f32 horizSpeed = velHoriz.length();
        f32 velUpProj = mUp.dot(*MR::getPlayerVelocity());
        if (__fabsf(horizSpeed) < 5.0f && velUpProj > 1.0f) {
            return false;
        }

        TVec3f front(mPosition);
        front.sub(*MR::getPlayerPos());
        MR::vecKillElement(front, mUp, &front);
        if (MR::isNearZero(front)) {
            front.set< f32 >(mFront);
        } else {
            MR::normalize(&front);
        }

        mPosMtx.mult(front, front);
        front.y = 0.0f;

        if (!MR::isNearZero(front)) {
            MR::normalize(&front);
        } else {
            front.set< f32 >(mFront);
        }
        mRider = pSender->mHost;
        mRotation.y = MR::calcRotateY(front.x, front.z);

        if (mIsSquare) {
            s32 temp = (45.0f + mRotation.y) / 90.0f;
            mRotation.y = 90.0f * temp;
        }

        TVec3f climbProj(mPosition);
        climbProj.sub(mBasePos);
        mClimbCoord = climbProj.dot(mUp);
        f32 poleTop = mPoleLength;
        f32 temp;
        if (mClimbCoord < 120.0f) {
            temp = 120.0f;
        } else if (mClimbCoord > poleTop) {
            temp = poleTop;
        } else {
            temp = mClimbCoord;
        }

        mClimbCoord = temp;
        mClimbSpeed = 0.0f;
        updateBindTrans();

        MR::invalidateClipping(this);
        MR::startSound(mRider, "SE_PM_GRAB_OBJ", -1, -1);
        MR::startSound(mRider, "SE_PV_LIFT_UP", -1, -1);
        MR::startActorCameraNoTarget(this, mCameraInfo, -1);
        MR::tryRumblePadWeak(this, WPAD_CHAN0);

        if (horizSpeed > 10.0f) {
            setNerve(&NrvPole::PoleNrvBindStartFast::sInstance);
        } else {
            if (horizSpeed > 6.0f) {
                setNerve(&NrvPole::PoleNrvBindStart::sInstance);
            } else {
                setNerve(&NrvPole::PoleNrvBindWait::sInstance);
            }
        }
        return true;
    }

    if (MR::isMsgUpdateBaseMtx(msg)) {
        TPos3f posMtx;
        TRot3f rotMtx;
        posMtx.identity();
        posMtx.setVec(mSide, mUp, mFront);

        rotMtx.identity();
        MR::makeMtxTransRotateY(rotMtx.toMtxPtr(), this);
        PSMTXConcat(posMtx.toMtxPtr(), rotMtx.toMtxPtr(), posMtx.toMtxPtr());
        posMtx.setPos(mPosition);
        MR::setBaseTRMtx(mRider, posMtx);
        return true;
    }

    if (MR::isMsgRushCancel(msg)) {
        if (!MR::isDemoActive()) {
            MR::startSound(mRider, "SE_PM_JUMP_M", -1, -1);
            MR::startSound(mRider, "SE_PV_JUMP_JOY", -1, -1);
        }
        mRider = nullptr;
        setNerve(&NrvPole::PoleNrvFree::sInstance);
        return true;
    }

    return false;
}

bool Pole::tryJump(bool handstand, f32 angleOffset) {
    if (MR::testCorePadTriggerA(WPAD_CHAN0) || MR::testSystemTriggerA()) {
        TPos3f pos;
        calcGravityMtx(&pos);

        f32 frontAngle = modRotateFromBase(angleOffset + mRotation.y + 180.0f, 0.0f);
        TVec3f jumpFront(0.0f, 0.0f, 0.0f);
        jumpFront.x = JMASinDegree(frontAngle);
        jumpFront.z = JMACosDegree(frontAngle);

        if (handstand) {
            jumpFront.scale(-1.0f);
        }

        TVec3f jumpVec;
        jumpVec.x = 10.0f * jumpFront.x;
        jumpVec.y = 25.0f;
        jumpVec.z = 10.0f * jumpFront.z;
        pos.mult(jumpVec, jumpVec);
        pos.mult(jumpFront, jumpFront);
        MR::setPlayerFrontTargetVec(jumpFront, 1);

        if (handstand) {
            MR::startBckPlayer("TreeHandStandJump", static_cast< const char* >(nullptr));
        } else {
            MR::startBckPlayer("WallJump", static_cast< const char* >(nullptr));
        }

        MR::startSound(mRider, "SE_PM_JUMP_M", -1, -1);
        MR::startSound(mRider, "SE_PV_JUMP_JOY", -1, -1);
        MR::endActorCamera(this, mCameraInfo, 1, -1);

        if (mIsSquare) {
            TVec3f vec2(jumpFront);
            vec2.scale(50.0f);
            vec2.add(*MR::getPlayerPos());
            MR::setPlayerPos(vec2);
        }

        MR::endBindAndPlayerJump(this, jumpVec, 0);

        if (handstand) {
            MR::becomeContinuousBckPlayer();
        }

        mRider = nullptr;
        setNerve(&NrvPole::PoleNrvFreeInvalid::sInstance);
        return true;
    }
    return false;
}

bool Pole::tryTurn() {
    if (isEnableTurn()) {
        if (getPoleSubPadStickX() > 0.0f) {
            setNerve(&NrvPole::PoleNrvBindTurnLeft::sInstance);
        } else {
            setNerve(&NrvPole::PoleNrvBindTurnRight::sInstance);
        }
        return true;
    }
    return false;
}

bool Pole::tryClimbUp() {
    if (getPoleSubPadStickY() > 0.0f) {
        setNerve(&NrvPole::PoleNrvBindClimbUp::sInstance);
        return true;
    }
    return false;
}

bool Pole::tryFallDown() {
    if (getPoleSubPadStickY() < 0.0f) {
        setNerve(&NrvPole::PoleNrvBindFallDown::sInstance);
        return true;
    }
    return false;
}

bool Pole::tryHandstandTurn() {
    if (isEnableTurn()) {
        setNerve(&NrvPole::PoleNrvBindHandstandTurn::sInstance);
        return true;
    }
    return false;
}

bool Pole::isEnableTurn() const {
    if (__fabsf(getPoleSubPadStickX()) > 0.8f) {
        return true;
    }
    return false;
}

void Pole::updateBindTrans() {
    mPosition.set< f32 >(mUp);
    mPosition.scale(mClimbCoord);
    mPosition.add(mBasePos);
}

void Pole::calcGravityMtx(TPos3f* pos) {
    pos->identity();
    MR::calcGravity(this);
    TVec3f up(-mGravity.x, -mGravity.y, -mGravity.z);
    MR::makeMtxUpFront(pos, up, mFront);
}

void Pole::updateTopPos(f32 top) {
    f32 topCoord = top;

    if (mIsSquare) {
        topCoord += 100.0f;
    }

    mTopPos.set< f32 >(mUp);
    mTopPos.scale(topCoord);
    mTopPos.add(mBasePos);
    MR::setMtxAxisXYZ(mTopMtx, mSide, mUp, mFront);
    MR::setMtxTrans(mTopMtx, mTopPos);
}

f32 Pole::getPoleSubPadStickX() const {
    if (mInvertStick) {
        return -MR::getSubPadStickX(WPAD_CHAN0);
    } else {
        return MR::getSubPadStickX(WPAD_CHAN0);
    }
}

f32 Pole::getPoleSubPadStickY() const {
    if (mInvertStick) {
        return -MR::getSubPadStickY(WPAD_CHAN0);
    } else {
        return MR::getSubPadStickY(WPAD_CHAN0);
    }
}

MtxPtr Pole::getBaseMtx() const {
    if (mNoModel) {
        return (MtxPtr)&mBaseMtx;
    } else {
        return LiveActor::getBaseMtx();
    }
}

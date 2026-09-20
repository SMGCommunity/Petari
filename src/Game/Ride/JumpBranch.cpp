#include "Game/Ride/JumpBranch.hpp"
#include "Game/LiveActor/HitSensor.hpp"
#include "Game/LiveActor/Nerve.hpp"
#include "Game/Ride/SwingRopePoint.hpp"
#include "Game/Util/ActorSensorUtil.hpp"
#include "Game/Util/ActorShadowUtil.hpp"
#include "Game/Util/GamePadUtil.hpp"
#include "Game/Util/LiveActorUtil.hpp"
#include "Game/Util/MathUtil.hpp"
#include "Game/Util/MtxUtil.hpp"
#include "Game/Util/ObjUtil.hpp"
#include "Game/Util/PlayerUtil.hpp"
#include "Game/Util/SoundUtil.hpp"
#include <revolution/mtx.h>
#include <revolution/wpad.h>

void JumpBranch_FORCE_MATCH_SDATA2() {
    (void)1.0f;
    (void)0.0f;
    (void)-1.0f;
}

namespace {
    static Vec sStartPointVelocity = {0.0f, 0.0f, 0.0f};

    static const f32 sPointGravity = 0.2f;
    static const f32 sPointFrictionRate = 0.995f;
    static const f32 sSensorMoveDistance = 50.0f;
    static const f32 sPointInterval = 30.0f;
    static const f32 sDistanceStartBindZ = 70.0f;
    static const f32 sTransDiffMinY = 1.0f;
    static const s32 sStepInvalidBind = 30;
    static const f32 sJumpSpeed = 10.0f;
    static const f32 sJumpSpeedOffsetY = 16.0f;
};  // namespace

namespace NrvJumpBranch {
    NEW_NERVE(JumpBranchNrvWait, JumpBranch, Wait);
    NEW_NERVE(JumpBranchNrvWaitInvalid, JumpBranch, WaitInvalid);
    NEW_NERVE(JumpBranchNrvBind, JumpBranch, Bind);
};  // namespace NrvJumpBranch

JumpBranch::JumpBranch(const char* pName)
    : LiveActor(pName), mCenter(0.0f, 0.0f, 0.0f), mSide(0.0f, 0.0f, 1.0f), mUp(0.0f, 0.0f, 1.0f), mFront(0.0f, 0.0f, 1.0f), mRider(), mSwingPoint(),
      mSwingReverse() {
}

void JumpBranch::init(const JMapInfoIter& rIter) {
    MR::initDefaultPos(this, rIter);
    mCenter.set(mPosition);

    TPos3f posMtx;
    posMtx.identity();
    MR::makeMtxTR(posMtx, this);

    posMtx.getXDir(mSide);
    posMtx.getYDir(mUp);
    posMtx.getZDir(mFront);

    initModelManagerWithAnm("JumpBranch", nullptr, false);

    MR::connectToSceneRide(this);

    initHitSensor(1);
    MR::addHitSensorCallbackBinder(this, "bind", 8, 80.0f);

    initEffectKeeper(0, nullptr, false);
    MR::onCalcGravity(this);

    MR::initShadowVolumeBox(this, TVec3f(120.0f, 10.0f, 10.0f));
    MR::setShadowDropLength(this, nullptr, 1500.0f);
    MR::onCalcShadowOneTime(this, nullptr);

    mSwingPoint = new SwingRopePoint(mPosition);

    initNerve(GET_NERVE(JumpBranch, JumpBranchNrvWait));
    makeActorAppeared();
}

void JumpBranch::exeWait() {
    if (MR::isFirstStep(this)) {
        mPosition.set(mCenter);
        mVelocity.zero();
        MR::validateClipping(this);
    }
}

void JumpBranch::exeWaitInvalid() {
    if (MR::isFirstStep(this)) {
        mPosition.set(mCenter);
        mVelocity.zero();
    }

    if (MR::isGreaterStep(this, ::sStepInvalidBind)) {
        setNerve(GET_NERVE(JumpBranch, JumpBranchNrvWait));
    }
}

inline void JumpBranch::exeBind() {
    if (updateBind()) {
        setNerve(GET_NERVE(JumpBranch, JumpBranchNrvWaitInvalid));
    }
};

void JumpBranch::updateHitSensor(HitSensor* pSensor) {
    TVec3f planarDiff(MR::getPlayerPos()->x - mPosition.x, 0.0f, MR::getPlayerPos()->z - mPosition.z);

    f32 proj = mSide.dot(planarDiff);
    f32 sensorPos;
    if (proj < -::sSensorMoveDistance) {
        sensorPos = -::sSensorMoveDistance;
    } else if (proj > ::sSensorMoveDistance) {
        sensorPos = ::sSensorMoveDistance;
    } else {
        sensorPos = proj;
    }

    f32 xOffs = mSide.x * sensorPos;
    pSensor->mPosition.x = xOffs + mPosition.x;
    pSensor->mPosition.y = mPosition.y;
    f32 zOffs = mSide.z * sensorPos;
    pSensor->mPosition.z = zOffs + mPosition.z;
}

bool JumpBranch::receiveOtherMsg(u32 msg, HitSensor* pSender, HitSensor* pReceiver) {
    if (MR::isMsgAutoRushBegin(msg)) {
        if (isNerve(GET_NERVE(JumpBranch, JumpBranchNrvWaitInvalid)) || MR::isOnGroundPlayer()) {
            return false;
        }

        TVec3f posDiff(pSender->mHost->mPosition);
        posDiff.sub(pReceiver->mPosition);

        if (MR::abs(posDiff.dot(mFront)) > ::sDistanceStartBindZ) {
            return false;
        }

        mRider = pSender->mHost;

        TVec3f playerFront;
        MR::getPlayerFrontVec(&playerFront);

        if (playerFront.dot(mFront) < 0) {
            mSwingReverse = true;
        } else {
            mSwingReverse = false;
        }

        TVec3f diff = pSender->mPosition - pReceiver->mPosition;

        f32 dotUp = mUp.dot(diff);
        f32 dotFront = mFront.dot(diff);
        diff.set(mUp * dotUp + mFront * dotFront);

        if (MR::isNearZero(diff)) {
            diff.set(mFront);
            if (mSwingReverse) {
                diff.scale(-1.0f);
            }
        } else {
            MR::normalize(&diff);
        }

        TVec3f grabUp = diff;
        diff.scale(30.0f);
        diff.add(mPosition);

        f32 speed = MR::getPlayerVelocity()->length();

        TVec3f swingVelocity(0.0f, 0.0f, 0.0f);

        TVec3f grabFront = grabUp.cross(mSide);

        if (mSwingReverse) {
            grabFront.scale(-1.0f);
        }

        if (playerFront.dot(*MR::getPlayerVelocity()) < 0.0f) {
            grabFront.scale(-1.0f);
        }

        ::sStartPointVelocity.x = grabFront.x;
        ::sStartPointVelocity.y = grabFront.y;
        ::sStartPointVelocity.z = grabFront.z;

        swingVelocity.set(grabFront);
        swingVelocity.scale(speed);

        TVec3f swingFront = mFront;
        if (grabUp.y > 0.0f) {
            swingFront.scale(-1.0f);
        }

        if (mSwingReverse) {
            swingFront.scale(-1.0f);
        }

        if (MR::abs(diff.y - mPosition.y) < ::sTransDiffMinY) {
            swingFront.set< f32 >(0.0f, -1.0f, 0.0f);
        }

        mSwingPoint->mFront.set(swingFront);
        mSwingPoint->setInfo(diff, swingVelocity, mPosition, ::sPointInterval);

        MR::startBckPlayer("JumpBranchWait");
        MR::startSound(mRider, "SE_PM_GRAB_OBJ");
        MR::startSound(mRider, "SE_PM_SPIN");
        setNerve(GET_NERVE(JumpBranch, JumpBranchNrvBind));
        return true;
    }

    if (MR::isMsgUpdateBaseMtx(msg)) {
        TPos3f posMtx;
        TVec3f swingSide = mSwingPoint->mSide;
        TVec3f swingFront = mSwingPoint->mFront;

        posMtx.setTR(swingSide, mSwingPoint->mUp, swingFront, mPosition);

        MR::setBaseTRMtx(mRider, posMtx);
        return true;
    }

    if (MR::isMsgRushCancel(msg)) {
        mRider = nullptr;
        setNerve(GET_NERVE(JumpBranch, JumpBranchNrvWait));
        return true;
    }

    return false;
}

bool JumpBranch::updateBind() {
    if (MR::testCorePadTriggerA(WPAD_CHAN0) || MR::testSystemTriggerA() || MR::isCorePadSwing(WPAD_CHAN0)) {
        TVec3f front = mFront;
        if (mSwingReverse) {
            front.scale(-1.0f);
        }
        MR::setPlayerFrontTargetVec(front, 1);

        TVec3f stickXZ(0.0f, 0.0f, 0.0f);
        bool fall = false;
        if (MR::isCorePadSwing(WPAD_CHAN0)) {
            fall = true;
        } else {
            MR::calcWorldStickDirectionXZ(&stickXZ, WPAD_CHAN0);
        }

        TVec3f jumpDir(0.0f, 0.0f, 0.0f);
        if (!fall) {
            MR::normalizeOrZero(&stickXZ);

            f32 proj = mFront.dot(stickXZ);

            jumpDir.x = ::sJumpSpeed * mFront.x * proj;
            jumpDir.y = ::sJumpSpeed * mFront.y * proj + ::sJumpSpeedOffsetY;
            jumpDir.z = ::sJumpSpeed * mFront.z * proj;

            MR::startSound(mRider, "SE_PM_JUMP_M");
            MR::startSound(mRider, "SE_PV_JUMP_S");

            if (front.dot(jumpDir) >= 0.0f) {
                MR::startBckPlayer("JumpBranchFrontSpin");
            } else {
                MR::startBckPlayer("JumpBranchBackSpin");
            }

        } else {
            MR::startBckPlayer("Fall");
        }

        MR::stopSound(mRider, "SE_PM_SPIN");
        MR::endBindAndPlayerJump(this, jumpDir, 0);
        mRider = nullptr;
        return true;
    }

    mSwingPoint->addAccel(mGravity * ::sPointGravity);
    mSwingPoint->strain(mPosition, ::sPointInterval);
    mSwingPoint->updatePosAndAxis(mSwingPoint->mFront, ::sPointFrictionRate);
    return false;
}

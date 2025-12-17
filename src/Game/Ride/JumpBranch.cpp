#include "Game/Ride/JumpBranch.hpp"
#include "Game/LiveActor/HitSensor.hpp"
#include "Game/Ride/SwingRopePoint.hpp"
#include "Game/Util/ActorSensorUtil.hpp"
#include "Game/Util/ActorShadowUtil.hpp"
#include "Game/Util/GamePadUtil.hpp"
#include "Game/Util/JMapInfo.hpp"
#include "Game/Util/LiveActorUtil.hpp"
#include "Game/Util/MathUtil.hpp"
#include "Game/Util/PlayerUtil.hpp"
#include "Game/Util/SoundUtil.hpp"
#include <revolution/mtx.h>
#include <revolution/wpad.h>

namespace {
    static TVec3f sStartPointVelocity;  // this appears in sinit but shouldnt.
}

namespace NrvJumpBranch {
    NEW_NERVE(JumpBranchNrvWait, JumpBranch, Wait);
    NEW_NERVE(JumpBranchNrvWaitInvalid, JumpBranch, WaitInvalid);
    NEW_NERVE(JumpBranchNrvBind, JumpBranch, Bind);
}  // namespace NrvJumpBranch

JumpBranch::JumpBranch(const char* pName)
    : LiveActor(pName), mCenter(0.0f, 0.0f, 0.0f), mSide(0.0f, 0.0f, 1.0f), mUp(0.0f, 0.0f, 1.0f), mFront(0.0f, 0.0f, 1.0f), mRider(nullptr),
      mSwingPoint(nullptr), mSwingReverse(false) {}

void JumpBranch::init(const JMapInfoIter& rIter) {
    MR::initDefaultPos(this, rIter);
    mCenter.set(mPosition);

    TPos3f posMtx;
    posMtx.identity();
    MR::makeMtxTR(reinterpret_cast< MtxPtr >(&posMtx), this);

    posMtx.getXDirInline(mSide);
    posMtx.getYDirInline(mUp);
    posMtx.getZDirInline(mFront);

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

    initNerve(&NrvJumpBranch::JumpBranchNrvWait::sInstance);
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

    if (MR::isGreaterStep(this, 30)) {
        setNerve(&NrvJumpBranch::JumpBranchNrvWait::sInstance);
    }
}

inline void JumpBranch::exeBind() {
    if (updateBind()) {
        setNerve(&NrvJumpBranch::JumpBranchNrvWaitInvalid::sInstance);
    }
};

void JumpBranch::updateHitSensor(HitSensor* pSensor) {
    TVec3f planarDiff(MR::getPlayerPos()->x - mPosition.x, 0.0f, MR::getPlayerPos()->z - mPosition.z);

    f32 proj = mSide.dot(planarDiff);
    f32 sensorPos;
    if (proj < -50.0f) {
        sensorPos = -50.0f;
    } else if (proj > 50.0f) {
        sensorPos = 50.0f;
    } else {
        sensorPos = proj;
    }

    f32 x1 = mSide.x * sensorPos;  // register scheduling hotfix
    pSensor->mPosition.x = x1 + mPosition.x;
    pSensor->mPosition.y = mPosition.y;
    f32 z1 = mSide.z * sensorPos;  // register scheduling hotfix
    pSensor->mPosition.z = z1 + mPosition.z;
}

bool JumpBranch::receiveOtherMsg(u32 msg, HitSensor* pSender, HitSensor* pReceiver) {
    if (MR::isMsgAutoRushBegin(msg)) {
        if (isNerve(&NrvJumpBranch::JumpBranchNrvWaitInvalid::sInstance) || MR::isOnGroundPlayer()) {
            return false;
        }

        TVec3f posDiff(pSender->mHost->mPosition);
        posDiff.sub(pReceiver->mPosition);

        if (__fabsf(posDiff.dot(mFront)) > 70.0f) {
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

        TVec3f diff(pSender->mPosition);
        diff.sub(pReceiver->mPosition);

        f32 dotUp = mUp.dot(diff);
        f32 dotFront = mFront.dot(diff);

        diff.set(mUp.scaleInline(dotUp).addOperatorInLine(mFront.scaleInline(dotFront)));

        if (MR::isNearZero(diff, 0.001f)) {
            diff.set(mFront);
            if (mSwingReverse) {
                diff.scale(-1.0f);
            }
        } else {
            MR::normalize(&diff);
        }

        TVec3f grabUp(diff);
        diff.scale(30.0f);
        diff.add(mPosition);

        f32 speed = MR::getPlayerVelocity()->length();

        TVec3f swingVelocity(0.0f, 0.0f, 0.0f);

        TVec3f grabFront;
        PSVECCrossProduct(&grabUp, &mSide, &grabFront);

        if (mSwingReverse) {
            grabFront.scale(-1.0f);
        }

        if (playerFront.dot(*MR::getPlayerVelocity()) < 0.0f) {
            grabFront.scale(1.0f);
        }

        // interesting...
        sStartPointVelocity.x = grabFront.x;
        sStartPointVelocity.y = grabFront.y;
        sStartPointVelocity.z = grabFront.z;

        swingVelocity.set(grabFront);
        swingVelocity.scale(speed);

        TVec3f swingFront(mFront);
        if (grabUp.y > 0.0f) {
            swingFront.scale(-1.0f);
        }

        if (mSwingReverse) {
            swingFront.scale(1.0f);
        }

        if (__fabsf(diff.y - mPosition.y) < 1.0f) {
            swingFront.set< f32 >(0.0f, -1.0f, 0.0f);
        }

        mSwingPoint->mFront.set(swingFront);
        mSwingPoint->setInfo(diff, swingVelocity, mPosition, 30.0f);

        MR::startBckPlayer("JumpBranchWait", static_cast< const char* >(nullptr));
        MR::startSound(mRider, "SE_PM_GRAB_OBJ", -1, -1);
        MR::startSound(mRider, "SE_PM_SPIN", -1, -1);
        setNerve(&NrvJumpBranch::JumpBranchNrvBind::sInstance);
        return true;
    }

    if (MR::isMsgUpdateBaseMtx(msg)) {
        TPos3f posMtx;
        TVec3f swingSide(mSwingPoint->mSide);
        TVec3f swingFront(mSwingPoint->mFront);

        posMtx.setVecAndTransInline(swingSide, mSwingPoint->mUp, swingFront, mPosition);

        MR::setBaseTRMtx(mRider, posMtx);
        return true;
    }

    if (MR::isMsgRushCancel(msg)) {
        mRider = nullptr;
        setNerve(&NrvJumpBranch::JumpBranchNrvWait::sInstance);
        return true;
    }

    return false;
}

bool JumpBranch::updateBind() {
    if (MR::testCorePadTriggerA(WPAD_CHAN0) || MR::testSystemTriggerA() || MR::isCorePadSwing(WPAD_CHAN0)) {
        TVec3f front(mFront);
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

            jumpDir.x = proj * (mFront.x * 10.0f);
            jumpDir.y = proj * (mFront.y * 10.0f) + 16.0f;
            jumpDir.z = proj * (mFront.z * 10.0f);

            MR::startSound(mRider, "SE_PM_JUMP_M", -1, -1);
            MR::startSound(mRider, "SE_PV_JUMP_S", -1, -1);

            if (front.dot(jumpDir) >= 0.0f) {
                MR::startBckPlayer("JumpBranchFrontSpin", static_cast< const char* >(nullptr));
            } else {
                MR::startBckPlayer("JumpBranchBackSpin", static_cast< const char* >(nullptr));
            }

        } else {
            MR::startBckPlayer("Fall", static_cast< const char* >(nullptr));
        }

        MR::stopSound(mRider, "SE_PM_SPIN", 0);
        MR::endBindAndPlayerJump(this, jumpDir, 0);
        mRider = nullptr;
        return true;
    }

    TVec3f grav(mGravity);
    grav.scale(0.2f);
    mSwingPoint->addAccel(grav);
    mSwingPoint->strain(mPosition, 30.0f);
    mSwingPoint->updatePosAndAxis(mSwingPoint->mFront, 0.995f);
    return false;
}

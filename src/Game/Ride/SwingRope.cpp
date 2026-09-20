#include "Game/Ride/SwingRope.hpp"
#include "Game/LiveActor/HitSensor.hpp"
#include "Game/LiveActor/Nerve.hpp"
#include "Game/Ride/SledRopePoint.hpp"
#include "Game/Ride/SwingRopePoint.hpp"
#include "Game/Scene/SceneFunction.hpp"
#include "Game/Scene/SceneObjHolder.hpp"
#include "Game/Util/ActorCameraUtil.hpp"
#include "Game/Util/ActorSensorUtil.hpp"
#include "Game/Util/ActorShadowUtil.hpp"
#include "Game/Util/CameraUtil.hpp"
#include "Game/Util/Color.hpp"
#include "Game/Util/GamePadUtil.hpp"
#include "Game/Util/LiveActorUtil.hpp"
#include "Game/Util/MathUtil.hpp"
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
#include <revolution/wpad.h>

void SwingRope_FORCE_MATCH_SDATA2() {
    (void)1.0f;
    (void)0.0f;
    (void)0.5f;
    (void)-1.0f;
}

namespace {
    static const f32 sPointInterval = 50.0f;
    static const f32 sDrawWidthX = 30.0f;
    static const f32 sDrawWidthZ = 40.0f;
    static const f32 sDrawWidthLongOffsetX = 13.0f;
    static const f32 sDrawWidthShortOffsetX = 3.0f;
    static const f32 sDrawWidthShortOffsetZ = 3.0f;
    static const f32 sTexRate = 0.13f;
    static const f32 sHangAccelRateLoose = 0.8f;
    static const f32 sHangAccelRateStretch = 0.5f;
    static const f32 sHangPointGravityLoose = 0.6f;
    static const f32 sHangPointGravityStretcn = 0.8f;
    static const f32 sHangPointFrictionRate = 0.995f;
    static const f32 sHangPointReboundRate = 0.7f;
    static const f32 sPointSpeedToSlow = 10.0f;
    static const f32 sPointNormalToSlow = 0.9999f;
    static const s32 sAnimInterpoleFrame = 10;
    static const s32 sAnimInterpoleFrameToSlowWait = 10;
    static const s32 sStepWaitAfterSwing = 0;
    static const f32 sHangPointDownAccel = 0.5f;
    static const f32 sHangPointDownSpeedMax = -10.0f;
    static const f32 sHangCoordMin = 100.0f;
    static const f32 sJumpSpeedRate = 5.0f;
    static const f32 sJumpSpeedMinXZ = 5.0f;
    static const f32 sJumpSpeedMaxXZ = 35.0f;
    static const f32 sJumpSpeedMinY = 25.0f;
    static const f32 sJumpSpeedMaxY = 50.0f;
    static const f32 sBindPointFrictionRateHead = 0.7f;
    static const f32 sBindPointFrictionRateTail = 0.8f;
    static const s32 sStepFixUpperPointPos = 10;
    static const f32 sCoordStretchToLoose = 5.0f;
    static const f32 sCoordLooseToStretch = 1.0f;
    static const f32 sPointGravity = 1.0f;
    static const f32 sFreePointFrictionRateHead = 0.8f;
    static const f32 sFreePointFrictionRateTail = 0.92f;
    static const f32 sPointSpeedToStop = 0.1f;
    static const f32 sPointNormalToStop = 0.9999f;
    static const f32 sTransDiffMinY = 1.0f;
    static const f32 sSensorRiderOffsetY = -50.0f;
    static const f32 sFootPosOffsetY = -20.0f;
    static const f32 sFootPosOffsetZ = 10.0f;
    static const f32 sHangPointBlendRateAxisZ = 0.1f;
    static const f32 sPointBlendRateAxisZ = 0.5f;
    static const f32 sFootPointAxisBlendRate = 0.3f;
    // static const f32 sDebugPointSpeed =
    // static const s32 sDebugPointNo =
};  // namespace

namespace NrvSwingRope {
    NEW_NERVE(SwingRopeNrvStop, SwingRope, Stop);
    NEW_NERVE(SwingRopeNrvFree, SwingRope, Free);
    NEW_NERVE(SwingRopeNrvFreeInvalid, SwingRope, FreeInvalid);
    NEW_NERVE(SwingRopeNrvBindSlideDownStart, SwingRope, BindSlideDownStart);
    NEW_NERVE(SwingRopeNrvBindSlideDown, SwingRope, BindSlideDown);
    NEW_NERVE(SwingRopeNrvBindStretch, SwingRope, BindStretch);
    NEW_NERVE(SwingRopeNrvBindLoose, SwingRope, BindLoose);

};  // namespace NrvSwingRope

namespace {
    static Color8 sColorPlusZ(0xFF, 0xFF, 0xFF, 0xFF);
    static Color8 sColorPlusX(0xFF, 0xFF, 0xFF, 0xFF);
    static Color8 sColorMinusX(0xFF, 0xFF, 0xFF, 0xFF);

    static const f32 sHangCoordMaxOffset = 200.0f;
    static const f32 sFootPosOffsetX = 0.0f;
};  // namespace

SwingRope::SwingRope(const char* pName)
    : LiveActor(pName), mBasePos(0.0f, 0.0f, 0.0f), mRopeLength(), mIsStretched(), mStretchTime(), mAccelTime(), mNumPoints(), mPoints(), mRider(),
      mGrabCoord(), mGrabPointNum(), mSlideSpeed(), mSledPoint(), mCameraInfo(), mFootCoord(), mFootPointNum(), mGrabToFootDist(),
      mFootPos(0.0f, 0.0f, 0.0f), mShadowDropPos(0.0f, 0.0f, 0.0f), mShadowDropOffset(), mPrevSoundLvl() {
    mRideMtx.identity();
}

void SwingRope::init(const JMapInfoIter& rIter) {
    MR::createSceneObj(SceneObj_SwingRopeGroup);
    MR::connectToScene(this, MR::MovementType_Ride, MR::CalcAnimType_None, MR::DrawBufferType_None, MR::DrawType_SwingRope);
    MR::initDefaultPos(this, rIter);

    mBasePos.set(mPosition);
    mRopeLength = mScale.y * 100.0f;

    MR::calcGravity(this);
    initPoints();

    initHitSensor(2);
    MR::addHitSensorCallbackBinder(this, "bind", 8, 200.0f);
    MR::addHitSensorCallbackRide(this, "body", 8, 150.0f);

    initSound(4, false);

    MR::setClippingTypeSphere(this, mRopeLength);

    MR::initShadowSurfaceCircle(this, 50.0f);
    MR::onCalcShadow(this, nullptr);
    MR::setShadowDropPositionPtr(this, nullptr, &mShadowDropPos);

    MR::initActorCamera(this, rIter, &mCameraInfo);
    initNerve(GET_NERVE(SwingRope, SwingRopeNrvFree));
    makeActorAppeared();
}

void SwingRope::draw() const {
    if (!MR::isValidDraw(this)) {
        return;
    }

    if (isNerve(GET_NERVE(SwingRope, SwingRopeNrvStop))) {
        drawStop();
    } else if (isNerve(GET_NERVE(SwingRope, SwingRopeNrvFree)) || isNerve(GET_NERVE(SwingRope, SwingRopeNrvFreeInvalid))) {
        drawFree();
    } else {
        drawBind();
    }
}

void SwingRope::exeStop() {
    if (MR::isFirstStep(this)) {
        MR::validateClipping(this);
    }

    s32 index = mNumPoints - 1;
    mShadowDropPos.x = mPoints[index]->mPosition.x;
    mShadowDropPos.y = mBasePos.y - (mRopeLength + mShadowDropOffset);
    mShadowDropPos.z = mPoints[index]->mPosition.z;
}

void SwingRope::exeFree() {
    if (MR::isFirstStep(this)) {
        MR::invalidateClipping(this);
    }

    addPointGravity();
    restrictPointToTail(0, nullptr);

    TVec3f front = mPoints[0]->mFront;

    SwingRopePoint* point;
    for (s32 idx = 0; idx < mNumPoints; idx++) {
        f32 f1 = idx;
        f32 f2 = mNumPoints - 1;
        point = mPoints[idx];
        point->updatePosAndAxis(front, calcFriction(idx));
        front.set(mPoints[idx]->mFront);
    }

    s32 index = mNumPoints - 1;
    mShadowDropPos.x = mPoints[index]->mPosition.x;
    mShadowDropPos.y = mBasePos.y - (mRopeLength + mShadowDropOffset);
    mShadowDropPos.z = mPoints[index]->mPosition.z;

    if (isAllPointsStop()) {
        for (s32 idx = 0; idx < mNumPoints; idx++) {
            mPoints[idx]->stop();
        }
        setNerve(GET_NERVE(SwingRope, SwingRopeNrvStop));
    }
}

inline void SwingRope::exeFreeInvalid() {
    exeFree();

    if (MR::isOnGroundPlayer()) {
        setNerve(GET_NERVE(SwingRope, SwingRopeNrvFree));
    }
}

void SwingRope::exeBindSlideDownStart() {
    if (MR::isFirstStep(this)) {
        MR::startBckPlayer("SwingRopeSlideDownStart", 0L);
        mSlideSpeed = 0.0f;
    }

    if (!updateSlideDown() && MR::isBckStopped(mRider)) {
        setNerve(GET_NERVE(SwingRope, SwingRopeNrvBindSlideDown));
    }
}

void SwingRope::exeBindSlideDown() {
    if (MR::isFirstStep(this)) {
        MR::startBckPlayer("SwingRopeSlideDown", ::sAnimInterpoleFrame);
    }

    if (!updateSlideDown()) {
        return;
    }
}

void SwingRope::exeBindStretch() {
    if (MR::isFirstStep(this)) {
        mStretchTime = 0;
    }

    if (!updateStretch() && mGrabCoord - mBasePos.distance(mSledPoint->mPosition) > ::sCoordStretchToLoose) {
        setNerve(GET_NERVE(SwingRope, SwingRopeNrvBindLoose));
    }
}

void SwingRope::exeBindLoose() {
    if (updateLoose()) {
        return;
    }

    if (isStretched()) {
        setNerve(GET_NERVE(SwingRope, SwingRopeNrvBindStretch));
    }
}

void SwingRope::initPoints() {
    mNumPoints = mRopeLength / ::sPointInterval;
    mPoints = new SwingRopePoint*[mNumPoints];

    for (s32 idx = 0; idx < mNumPoints; idx++) {
        TVec3f pos = mGravity * (::sPointInterval * (idx + 1));
        pos.add(mBasePos);
        mPoints[idx] = new SwingRopePoint(pos);
    }

    mSledPoint = new SledRopePoint(mBasePos, ::sPointInterval);
}

void SwingRope::updateHitSensor(HitSensor* pSensor) {
    if (MR::isSensorBinder(pSensor)) {
        TVec3f bottomPos = mGravity;
        bottomPos.scale(mRopeLength);
        bottomPos.add(mBasePos);
        MR::calcPerpendicFootToLineInside(&pSensor->mPosition, *MR::getPlayerPos(), mBasePos, bottomPos);
        return;
    }

    if (MR::isSensorRide(pSensor)) {
        TVec3f sensorPos = mSledPoint->mUp * ::sSensorRiderOffsetY;
        sensorPos.add(mSledPoint->mPosition);
        pSensor->mPosition.set(sensorPos);
        return;
    }
}

void SwingRope::attackSensor(HitSensor* pSender, HitSensor* pReceiver) {
    if (mRider == nullptr) {
        return;
    }

    MR::tryGetItem(pSender, pReceiver);
}

bool SwingRope::receiveOtherMsg(u32 msg, HitSensor* pSender, HitSensor* pReceiver) {
    if (MR::isMsgAutoRushBegin(msg)) {
        if (MR::isOnGroundPlayer() || MR::isPlayerSwimming() ||
            (!isNerve(GET_NERVE(SwingRope, SwingRopeNrvStop)) && !isNerve(GET_NERVE(SwingRope, SwingRopeNrvFree)))) {
            return false;
        }

        mRider = pSender->mHost;
        mIsStretched = false;
        MR::calcGravity(this, *MR::getPlayerPos());

        TVec3f grav = mGravity;
        TVec3f pos = mPosition;

        MR::calcPerpendicFootToLine(&pos, *MR::getPlayerPos(), mBasePos, mBasePos + grav * mRopeLength);

        TVec3f posDiff = pos - mBasePos;
        f32 grabCoord = posDiff.dot(grav);
        grabCoord = MR::clamp(grabCoord, ::sHangCoordMin, mRopeLength - ::sHangCoordMaxOffset);
        mGrabCoord = grabCoord;
        mGrabPointNum = calcPointNo(mGrabCoord);

        TVec3f front;
        MR::getPlayerFrontVec(&front);

        if (MR::abs(mRider->mPosition.y - mBasePos.y) < ::sTransDiffMinY) {
            front.set< f32 >(0.0f, -1.0f, 0.0f);
        }
        mSledPoint->mFront.set(front);

        const TVec3f& riderPos = mRider->mPosition;
        const TVec3f& vel = *MR::getPlayerVelocity();
        mSledPoint->setInfo(riderPos, vel, mBasePos, mGrabCoord);

        updateRideMtx();
        updateFootPos();

        MR::invalidateClipping(this);
        MR::startSound(mRider, "SE_PV_CATCH");
        MR::startSound(mRider, "SE_PM_GRAB_OBJ");
        MR::startActorCameraNoTarget(this, mCameraInfo, -1);

        setNerve(GET_NERVE(SwingRope, SwingRopeNrvBindSlideDownStart));

        return true;
    }

    if (MR::isMsgUpdateBaseMtx(msg)) {
        MR::setBaseTRMtx(mRider, mRideMtx);
        return true;
    }

    if (MR::isMsgRushCancel(msg)) {
        mRider = nullptr;
        mGrabCoord = 0.0f;
        mGrabPointNum = 0.0f;
        setNerve(GET_NERVE(SwingRope, SwingRopeNrvFree));
        return true;
    }

    return false;
}

f32 SwingRope::calcPointNo(f32 coord) const {
    return MR::clamp((coord / ::sPointInterval) - 1.0f, 0.0f, mNumPoints - 1.0f);
}

f32 SwingRope::calcFriction(s32 index) const {
    if (mRider != nullptr) {
        return (index * (::sBindPointFrictionRateTail - ::sBindPointFrictionRateHead)) / (mNumPoints - 1) + ::sBindPointFrictionRateHead;
    } else {
        return (index * (::sFreePointFrictionRateTail - ::sFreePointFrictionRateHead)) / (mNumPoints - 1) + ::sFreePointFrictionRateHead;
    }
}

void SwingRope::addPointGravity() {
    TVec3f grav = mGravity * ::sPointGravity;

    for (s32 idx = 0; idx < mNumPoints; idx++) {
        mPoints[idx]->addAccel(grav);
    }
}

void SwingRope::restrictPointToTail(s32 index, const TVec3f* pAnchor, f32 length) {
    if (length == 0.0f) {
        length = ::sPointInterval;
    }

    TVec3f anchor;
    if (pAnchor != nullptr) {
        anchor.set(*pAnchor);
    } else {
        anchor.set(mBasePos);
    }

    for (; index < mNumPoints; index++) {
        mPoints[index]->restrict(anchor, length, nullptr);
        anchor.set(mPoints[index]->mPosition);
        anchor.add(mPoints[index]->mVelocity);
        length = ::sPointInterval;
    }
}

void SwingRope::restrictPointToHead(s32 index, const TVec3f& rAnchor, f32 length) {
    TVec3f anchor = rAnchor;

    for (; index >= 0; index--) {
        mPoints[index]->restrict(anchor, length, nullptr);
        anchor.set(mPoints[index]->mPosition);
        anchor.add(mPoints[index]->mVelocity);
        length = ::sPointInterval;
    }
}

bool SwingRope::isAllPointsStop() const {
    for (s32 idx = 0; idx < mNumPoints; idx++) {
        if (MR::abs(mPoints[idx]->mVelocity.x) > ::sPointSpeedToStop || MR::abs(mPoints[idx]->mVelocity.y) > ::sPointSpeedToStop ||
            MR::abs(mPoints[idx]->mVelocity.z) > ::sPointSpeedToStop) {
            return false;
        }

        if (mPoints[idx]->mUp.y < ::sPointNormalToStop) {
            return false;
        }
    }

    return true;
}

bool SwingRope::isAllPointsSlow() const {
    for (s32 idx = 0; idx < mNumPoints; idx++) {
        if (MR::abs(mPoints[idx]->mVelocity.x) > ::sPointSpeedToSlow || MR::abs(mPoints[idx]->mVelocity.y) > ::sPointSpeedToSlow ||
            MR::abs(mPoints[idx]->mVelocity.z) > ::sPointSpeedToSlow) {
            return false;
        }

        if (mPoints[idx]->mUp.y < ::sPointNormalToSlow) {
            return false;
        }
    }

    return true;
}

bool SwingRope::isStretched() const {
    return MR::abs(mGrabCoord - mBasePos.distance(mSledPoint->mPosition)) < ::sCoordLooseToStretch;
}

bool SwingRope::tryJump() {
    if (MR::testCorePadTriggerA(WPAD_CHAN0) || MR::testSystemTriggerA()) {
        TVec3f front = mSledPoint->mFront;

        TVec3f jumpY = mGravity;
        jumpY.scale(mSledPoint->mVelocity.dot(mGravity));
        if (jumpY.length() < ::sJumpSpeedMinY) {
            jumpY = mGravity * -::sJumpSpeedMinY;
        } else if (jumpY.length() > ::sJumpSpeedMaxY) {
            jumpY = mGravity * -::sJumpSpeedMaxY;
        }

        TVec3f jumpXZ = mSledPoint->mVelocity;
        MR::vecKillElement(jumpXZ, mGravity, &jumpXZ);
        if (jumpXZ.length() > ::sJumpSpeedMaxXZ) {
            MR::normalize(&jumpXZ);
            front.set(jumpXZ);
            jumpXZ.scale(::sJumpSpeedMaxXZ);
        } else if (jumpXZ.length() < ::sJumpSpeedMinXZ && MR::isGamePadStickOperated(WPAD_CHAN0)) {
            TVec3f stick(0.0f, 0.0f, 0.0f);
            MR::calcWorldStickDirectionXZ(&stick, WPAD_CHAN0);
            front.set< f32 >(stick.x, stick.y, stick.z);
            jumpXZ.set(stick * ::sJumpSpeedRate);
            MR::vecKillElement(jumpXZ, mGravity, &jumpXZ);
        } else if (!MR::isNearZero(jumpXZ)) {
            front.set(jumpXZ);
            MR::normalize(&front);
        }

        TVec3f jumpVec = jumpXZ;
        jumpVec.add(jumpY);

        MR::endActorCameraAtLanding(this, mCameraInfo, -1);
        MR::startSound(mRider, "SE_PV_JUMP_S");
        MR::startSound(mRider, "SE_PM_JUMP_M");
        MR::startBckPlayer("SwingRopeSpin", 0L);

        MR::setPlayerFrontTargetVec(front, 1);
        MR::endBindAndPlayerWeakGravityJump(this, jumpVec);

        mRider = nullptr;
        setNerve(GET_NERVE(SwingRope, SwingRopeNrvFreeInvalid));
        return true;
    }

    return false;
}

void SwingRope::updateHangPoint() {
    TVec3f stick(0.0f, 0.0f, 0.0f);
    TVec3f front = mSledPoint->mFront;
    MR::calcWorldStickDirectionXZ(&stick, WPAD_CHAN0);

    if (!MR::isNearZero(stick)) {
        MR::vecBlend(mSledPoint->mFront, stick, &front, ::sHangPointBlendRateAxisZ);
        if (isNerve(GET_NERVE(SwingRope, SwingRopeNrvBindLoose))) {
            stick.scale(::sHangAccelRateLoose);
        } else {
            stick.scale(::sHangAccelRateStretch);
        }
        mSledPoint->addAccel(stick);

        if (mAccelTime == 0) {
            MR::startBckPlayer("SwingRopeAccel", ::sAnimInterpoleFrame);
        } else if (MR::isBckOneTimeAndStopped(mRider)) {
            MR::startBckPlayer("SwingRopeSwingWait", ::sAnimInterpoleFrame);
        }
        mAccelTime++;
    } else {
        if (mAccelTime > ::sStepWaitAfterSwing) {
            MR::startBckPlayer("SwingRopeWait", ::sAnimInterpoleFrameToSlowWait);
            mAccelTime = 0;
        }
    }

    mSledPoint->bindToMapCollision(::sHangPointReboundRate);
    mPosition.set(mSledPoint->mPosition);
    MR::calcGravity(this);

    TVec3f grav = mGravity;
    if (isNerve(GET_NERVE(SwingRope, SwingRopeNrvBindLoose))) {
        grav.scale(::sHangPointGravityLoose);
    } else {
        grav.scale(::sHangPointGravityStretcn);
    }
    mSledPoint->addAccel(grav);
    mSledPoint->restrict(mBasePos, mGrabCoord, nullptr);
    mSledPoint->updatePosAndAxis(front, ::sHangPointFrictionRate);

    updateRideMtx();
    updateFootPos();
    mShadowDropPos.x = mSledPoint->mPosition.x;
    mShadowDropPos.z = mSledPoint->mPosition.z;

    s32 soundLvl = (mSledPoint->mVelocity.length() / 40.0f) * 100.0f;
    if (soundLvl > 100) {
        soundLvl = 100;
    }
    if (soundLvl < 0) {
        soundLvl = 0;
    }
    MR::startLevelSound(this, "SE_OJ_LV_ROPE_SWING_WIND_1", soundLvl);

    if (mPrevSoundLvl < 20 && 20 <= soundLvl) {
        MR::startSound(this, "SE_OJ_ROPE_CREAK_H");
    }
    mPrevSoundLvl = soundLvl;
}

void SwingRope::updateRideMtx() {
    mRideMtx.setTR(mSledPoint->mSide, mSledPoint->mUp, mSledPoint->mFront, mSledPoint->mPosition);
}

void SwingRope::updateFootPos() {
    TPos3f footMtx;
    MR::calcPlayerJointMtx(&footMtx, "FootL");

    TVec3f side, up, front;
    footMtx.getXDir(side);
    footMtx.getYDir(up);
    footMtx.getZDir(front);
    footMtx.getTrans(mFootPos);

    mFootPos.add(side * ::sFootPosOffsetX + up * ::sFootPosOffsetY + front * ::sFootPosOffsetZ);
    mGrabToFootDist = mFootPos.distance(mSledPoint->mPosition);
    mFootCoord = mGrabCoord + mGrabToFootDist;
    mFootPointNum = calcPointNo(mFootCoord);
}

bool SwingRope::updateStretch() {
    if (tryJump()) {
        return true;
    }

    updateHangPoint();
    addPointGravity();
    updateStretchHangUpperPoints();
    updateHangLowerPoints();
    mStretchTime++;

    return false;
}

bool SwingRope::updateLoose() {
    if (tryJump()) {
        return true;
    }

    updateHangPoint();
    updateLooseUpperPoints();
    updateHangLowerPoints();

    return false;
}

void SwingRope::updateLooseUpperPoints() {
    SwingRopePoint* point;

    addPointGravity();
    restrictPointToTail(0, nullptr);

    s32 grabIndex = mGrabPointNum;
    f32 length = mGrabCoord - (grabIndex + 1) * ::sPointInterval;
    TVec3f pos = mSledPoint->mPosition;
    restrictPointToHead(grabIndex, pos, length);
    restrictPointToTail(0, nullptr);

    TVec3f front = mSledPoint->mFront;
    for (s32 idx = 0; idx <= grabIndex; idx++) {
        point = mPoints[idx];
        point->updatePosAndAxis(front, calcFriction(idx));
        front.set(mPoints[idx]->mFront);
    }
}

bool SwingRope::updateSlideDown() {
    mSlideSpeed -= ::sHangPointDownAccel;
    mSlideSpeed = mSlideSpeed >= ::sHangPointDownSpeedMax ? mSlideSpeed : ::sHangPointDownSpeedMax;
    mGrabCoord -= mSlideSpeed;
    mGrabCoord = MR::clamp(mGrabCoord, ::sHangCoordMin, mRopeLength - ::sHangCoordMaxOffset);

    mGrabPointNum = calcPointNo(mGrabCoord);

    if (mIsStretched) {
        if (updateStretch()) {
            // this returns early if jumping, not if stretching
            return true;
        }
    } else {
        if (updateLoose()) {
            // this returns early if jumping, not if loose
            return true;
        }

        if (isStretched()) {
            mIsStretched = true;
            mStretchTime = 0;
        }
    }

    if (mGrabCoord >= mRopeLength - ::sHangCoordMaxOffset) {
        mGrabCoord = mRopeLength - ::sHangCoordMaxOffset;
        mSlideSpeed = 0.0f;
        setNerve(GET_NERVE(SwingRope, SwingRopeNrvBindStretch));
        return true;
    }

    return false;
}

void SwingRope::updateStretchHangUpperPoints() {
    if (mStretchTime < ::sStepFixUpperPointPos) {
        updateLooseUpperPoints();
    }

    s32 grabIndex = mGrabPointNum;
    s32 maxIndex = mNumPoints - 1;
    if (grabIndex < 0) {
        grabIndex = 0;
    } else if (grabIndex > maxIndex) {
        grabIndex = maxIndex;
    }

    TVec3f front = mSledPoint->mFront;
    for (s32 idx = 0; idx <= grabIndex; idx++) {
        TVec3f pos = mSledPoint->mUp * (-::sPointInterval * (idx + 1));
        pos.add(mBasePos);

        if (mStretchTime < ::sStepFixUpperPointPos) {
            f32 t = MR::getEaseOutValue(static_cast< f32 >(mStretchTime + 1) / ::sStepFixUpperPointPos, 0.0f, 1.0f, 1.0f);
            TVec3f stretchPos = pos;
            pos = stretchPos * t + mPoints[idx]->mPosition * (1.0f - t);
        }
        mPoints[idx]->setAndUpdatePosAndAxis(pos, mSledPoint->mUp, front);
        front.set(mPoints[idx]->mFront);
    }
}

void SwingRope::updateHangLowerPoints() {
    s32 grabIndex = mGrabPointNum;
    s32 nextIndex = grabIndex + 1;

    if (nextIndex <= mNumPoints - 1) {
        f32 f = nextIndex + 1;
        mPoints[nextIndex]->setPosAndAxis(mFootPos, mSledPoint->mSide, mSledPoint->mUp, mSledPoint->mFront);

        TVec3f up, front, side;
        TVec3f zero(0.0f, 0.0f, 0.0f);
        mPoints[nextIndex]->mVelocity.set(zero);
        restrictPointToTail(nextIndex + 1, &mFootPos);
        updateHangLowerPointPos(nextIndex + 1);

        s32 nextNextIndex = nextIndex + 1;
        MR::vecBlend(mSledPoint->mUp, mPoints[nextNextIndex]->mUp, &up, ::sPointBlendRateAxisZ);
        MR::vecBlend(mSledPoint->mFront, mPoints[nextNextIndex]->mFront, &front, ::sPointBlendRateAxisZ);
        MR::normalize(&up);
        MR::normalize(&front);

        side.cross(up, front);
        if (!MR::isNearZero(side)) {
            MR::makeAxisUpFront(&side, &front, up, front);
            mPoints[nextIndex]->setPosAndAxis(mFootPos, side, up, front);
        }
    }
}

void SwingRope::updateHangLowerPointPos(s32 index) {
    for (; index < mNumPoints; index++) {
        SwingRopePoint* point = mPoints[index];
        SwingRopePoint* prevPoint = mPoints[index - 1];
        TVec3f front = prevPoint->mFront;
        prevPoint->mUp.dot(point->mUp);
        MR::vecBlend(mPoints[index]->mFront, mPoints[index - 1]->mFront, &front, ::sFootPointAxisBlendRate);
        point->updatePosAndAxis(front, calcFriction(index));
    }
}

namespace {
    void sendPoint(const TVec3f& pos, const TVec3f& side, const TVec3f& front, f32 x, f32 y, u32 color, f32 texX, f32 texY) {
        GXPosition3f32(pos.x + side.x * x + front.x * y, pos.y + side.y * x + front.y * y, pos.z + side.z * x + front.z * y);
        GXColor1u32(color);
        GXTexCoord2f32(texX, texY);
    }

    void drawPolygon(const TVec3f& posA, const TVec3f& posB, const TVec3f& side, const TVec3f& front, f32 x1, f32 y1, f32 x2, f32 y2, u32 color1,
                     u32 color2, f32 texY1, f32 texY2) {
        GXBegin(GX_TRIANGLESTRIP, GX_VTXFMT0, 4);
        {
            sendPoint(posA, side, front, x1, y1, color1, 0.0f, texY1);
            sendPoint(posA, side, front, x2, y2, color2, 1.0f, texY1);
            sendPoint(posB, side, front, x1, y1, color1, 0.0f, texY2);
            sendPoint(posB, side, front, x2, y2, color2, 1.0f, texY2);
        }
        GXEnd();
    }

    void drawLine(const SwingRope* pRope, u32 color1, u32 color2, f32 x1, f32 y1, f32 x2, f32 y2) {
        GXBegin(GX_TRIANGLESTRIP, GX_VTXFMT0, (pRope->mNumPoints + 1) * 2);
        {
            const TVec3f& front = pRope->mPoints[0]->getFront();
            const TVec3f& side = pRope->mPoints[0]->getSide();
            const TVec3f& pos = pRope->getBasePos();

            sendPoint(pos, side, front, x1, y1, color1, 0.0f, 0.0f);
            sendPoint(pos, side, front, x2, y2, color2, 1.0f, 0.0f);

            SwingRopePoint* point;
            for (s32 idx = 0; idx < pRope->mNumPoints; idx++) {
                point = pRope->mPoints[idx];
                const TVec3f& front = point->getFront();
                const TVec3f& side = point->getSide();
                const TVec3f& pos = point->getPosition();

                sendPoint(pos, side, front, x1, y1, color1, 0.0f, ::sTexRate * (idx + 1));
                sendPoint(pos, side, front, x2, y2, color2, 1.0f, ::sTexRate * (idx + 1));
            }
        }
        GXEnd();
    }

    void drawLineAtHanging(const SwingRope* pRope, u32 color1, u32 color2, f32 texY1, f32 texY2, f32 x1, f32 y1, f32 x2, f32 y2) {
        GXBegin(GX_TRIANGLESTRIP, GX_VTXFMT0, (pRope->mNumPoints + 1) * 2);
        {
            const TVec3f& front = pRope->mPoints[0]->getFront();
            const TVec3f& side = pRope->mPoints[0]->getSide();
            const TVec3f& pos = pRope->getBasePos();

            sendPoint(pos, side, front, x1, y1, color1, 0.0f, 0.0f);
            sendPoint(pos, side, front, x2, y2, color2, 1.0f, 0.0f);

            SwingRopePoint* point;
            for (s32 idx = 0; idx < pRope->mNumPoints; idx++) {
                point = pRope->mPoints[idx];
                const TVec3f& front = point->getFront();
                const TVec3f& side = point->getSide();
                const TVec3f& pos = point->getPosition();

                sendPoint(pos, side, front, x1, y1, color1, 0.0f, ::sTexRate * (idx + 1));
                sendPoint(pos, side, front, x2, y2, color2, 1.0f, ::sTexRate * (idx + 1));
            }
        }
        GXEnd();
    }
};  // namespace

void SwingRope::drawStop() const {
    TVec3f bottom(mBasePos.x, mBasePos.y - mRopeLength, mBasePos.z);
    const TVec3f& side = mPoints[0]->getSide();
    const TVec3f& front = mPoints[0]->getFront();

    f32 texY = (mRopeLength / ::sPointInterval) * ::sTexRate;

    ::drawPolygon(mBasePos, bottom, side, front, -::sDrawWidthX, (::sDrawWidthZ + ::sDrawWidthShortOffsetZ), ::sDrawWidthX + ::sDrawWidthShortOffsetX,
                  -(::sDrawWidthZ + ::sDrawWidthShortOffsetZ), ::sColorPlusZ, ::sColorPlusX, 0.0f, texY);

    ::drawPolygon(mBasePos, bottom, side, front, -(::sDrawWidthX + ::sDrawWidthShortOffsetX), -(::sDrawWidthZ + ::sDrawWidthShortOffsetZ),
                  ::sDrawWidthX, (::sDrawWidthZ + ::sDrawWidthShortOffsetZ), ::sColorMinusX, ::sColorPlusZ, 0.0f, texY);

    ::drawPolygon(mBasePos, bottom, side, front, ::sDrawWidthX + ::sDrawWidthLongOffsetX, -::sDrawWidthShortOffsetZ,
                  -(::sDrawWidthX + ::sDrawWidthLongOffsetX), -::sDrawWidthShortOffsetZ, ::sColorPlusX, ::sColorMinusX, 0.0f, texY);
}

void SwingRope::drawFree() const {
    ::drawLine(this, ::sColorPlusZ, ::sColorPlusX, -::sDrawWidthX, (::sDrawWidthZ + ::sDrawWidthShortOffsetZ),
               ::sDrawWidthX + ::sDrawWidthShortOffsetX, -(::sDrawWidthZ + ::sDrawWidthShortOffsetZ));
    ::drawLine(this, ::sColorMinusX, ::sColorPlusZ, -(::sDrawWidthX + ::sDrawWidthShortOffsetX), -(::sDrawWidthZ + ::sDrawWidthShortOffsetZ),
               ::sDrawWidthX, (::sDrawWidthZ + ::sDrawWidthShortOffsetZ));
    ::drawLine(this, ::sColorMinusX, ::sColorPlusZ, ::sDrawWidthX + ::sDrawWidthLongOffsetX, -::sDrawWidthShortOffsetZ,
               -(::sDrawWidthX + ::sDrawWidthLongOffsetX), -::sDrawWidthShortOffsetZ);
}

void SwingRope::drawBind() const {
    f32 texYGrab = (mGrabPointNum + 1.0f) * ::sTexRate;
    f32 texYFoot = (calcPointNo(mFootCoord) + 1.0f) * ::sTexRate;
    ::drawLineAtHanging(this, ::sColorPlusZ, ::sColorPlusX, texYGrab, texYFoot, -::sDrawWidthX, (::sDrawWidthZ + ::sDrawWidthShortOffsetZ),
                        ::sDrawWidthX + ::sDrawWidthShortOffsetX, -(::sDrawWidthZ + ::sDrawWidthShortOffsetZ));
    ::drawLineAtHanging(this, ::sColorMinusX, ::sColorPlusZ, texYGrab, texYFoot, -(::sDrawWidthX + ::sDrawWidthShortOffsetX),
                        -(::sDrawWidthZ + ::sDrawWidthShortOffsetZ), ::sDrawWidthX, (::sDrawWidthZ + ::sDrawWidthShortOffsetZ));
    ::drawLineAtHanging(this, ::sColorMinusX, ::sColorPlusZ, texYGrab, texYFoot, ::sDrawWidthX + ::sDrawWidthLongOffsetX, -::sDrawWidthShortOffsetZ,
                        -(::sDrawWidthX + ::sDrawWidthLongOffsetX), -::sDrawWidthShortOffsetZ);
}

SwingRopeGroup::SwingRopeGroup(const char* pName) : NameObj(pName) {
    mTexture = nullptr;
    mTexture = new JUTTexture(MR::loadTexFromArc("SwingRope.arc", "SwingRope.bti"), 0);

    MR::registerPreDrawFunction(MR::Functor(this, &SwingRopeGroup::initDraw), MR::DrawType_SwingRope);
}

void SwingRopeGroup::initDraw() const {
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

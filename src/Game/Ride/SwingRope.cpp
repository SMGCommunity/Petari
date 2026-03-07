#include "Game/Ride/SwingRope.hpp"
#include "Game/LiveActor/HitSensor.hpp"
#include "Game/Ride/SledRopePoint.hpp"
#include "Game/Ride/SwingRopePoint.hpp"
#include "Game/Scene/SceneFunction.hpp"
#include "Game/Util/ActorCameraUtil.hpp"
#include "Game/Util/ActorSensorUtil.hpp"
#include "Game/Util/ActorShadowUtil.hpp"
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
#include <revolution/wpad.h>

namespace NrvSwingRope {
    NEW_NERVE(SwingRopeNrvStop, SwingRope, Stop);
    NEW_NERVE(SwingRopeNrvFree, SwingRope, Free);
    NEW_NERVE(SwingRopeNrvFreeInvalid, SwingRope, FreeInvalid);
    NEW_NERVE(SwingRopeNrvBindSlideDownStart, SwingRope, BindSlideDownStart);
    NEW_NERVE(SwingRopeNrvBindSlideDown, SwingRope, BindSlideDown);
    NEW_NERVE(SwingRopeNrvBindStretch, SwingRope, BindStretch);
    NEW_NERVE(SwingRopeNrvBindLoose, SwingRope, BindLoose);

}  // namespace NrvSwingRope

namespace {
    static Color8 sColorPlusZ(0xFF, 0xFF, 0xFF, 0xFF);
    static Color8 sColorPlusX(0xFF, 0xFF, 0xFF, 0xFF);
    static Color8 sColorMinusX(0xFF, 0xFF, 0xFF, 0xFF);
}  // namespace

SwingRope::SwingRope(const char* pName)
    : LiveActor(pName), mBasePos(0.0f, 0.0f, 0.0f), mRopeLength(0.0f), mIsStretched(false), mStretchTime(0), mAccelTime(0), mNumPoints(0),
      mPoints(nullptr), mRider(nullptr), mGrabCoord(0.0f), mGrabPointNum(0.0f), mSlideSpeed(0.0f), mSledPoint(nullptr), mCameraInfo(nullptr),
      mFootCoord(0.0f), mFootPointNum(0.0f), mGrabToFootDist(0.0f), mFootPos(0.0f, 0.0f, 0.0f), mShadowDropPos(0.0f, 0.0f, 0.0f),
      mShadowDropOffset(0.0f), mPrevSoundLvl(0) {
    mRideMtx.identity();
}

void SwingRope::init(const JMapInfoIter& rIter) {
    MR::createSceneObj(SceneObj_SwingRopeGroup);
    MR::connectToScene(this, MR::MovementType_Ride, -1, -1, MR::DrawType_SwingRope);
    MR::initDefaultPos(this, rIter);

    mBasePos.set(mPosition);
    mRopeLength = mScale.y * 100.0f;

    MR::calcGravity(this);
    initPoints();

    initHitSensor(2);
    MR::addHitSensorCallbackBinder(this, "bind", 8, 200.0);
    MR::addHitSensorCallbackRide(this, "body", 8, 150.0);

    initSound(4, false);

    MR::setClippingTypeSphere(this, mRopeLength);

    MR::initShadowSurfaceCircle(this, 50.0f);
    MR::onCalcShadow(this, nullptr);
    MR::setShadowDropPositionPtr(this, nullptr, &mShadowDropPos);

    MR::initActorCamera(this, rIter, &mCameraInfo);
    initNerve(&NrvSwingRope::SwingRopeNrvFree::sInstance);
    makeActorAppeared();
}

void SwingRope::draw() const {
    if (!MR::isValidDraw(this)) {
        return;
    }

    if (isNerve(&NrvSwingRope::SwingRopeNrvStop::sInstance)) {
        drawStop();
    } else if (isNerve(&NrvSwingRope::SwingRopeNrvFree::sInstance) || isNerve(&NrvSwingRope::SwingRopeNrvFreeInvalid::sInstance)) {
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
    restrictPointToTail(0, nullptr, 0.0f);

    TVec3f front(mPoints[0]->mFront);

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
        setNerve(&NrvSwingRope::SwingRopeNrvStop::sInstance);
    }
}

inline void SwingRope::exeFreeInvalid() {
    exeFree();

    if (MR::isOnGroundPlayer()) {
        setNerve(&NrvSwingRope::SwingRopeNrvFree::sInstance);
    }
}

void SwingRope::exeBindSlideDownStart() {
    if (MR::isFirstStep(this)) {
        MR::startBckPlayer("SwingRopeSlideDownStart", static_cast< s32 >(0));
        mSlideSpeed = 0.0f;
    }

    if (!updateSlideDown() && MR::isBckStopped(mRider)) {
        setNerve(&NrvSwingRope::SwingRopeNrvBindSlideDown::sInstance);
    }
}

void SwingRope::exeBindSlideDown() {
    if (MR::isFirstStep(this)) {
        MR::startBckPlayer("SwingRopeSlideDown", 10);
    }

    if (!updateSlideDown()) {
        return;
    }
}

void SwingRope::exeBindStretch() {
    if (MR::isFirstStep(this)) {
        mStretchTime = 0;
    }

    if (!updateStretch() && mGrabCoord - PSVECDistance(&mBasePos, &mSledPoint->mPosition) > 5.0f) {
        setNerve(&NrvSwingRope::SwingRopeNrvBindLoose::sInstance);
    }
}

void SwingRope::exeBindLoose() {
    if (updateLoose()) {
        return;
    }

    if (isStretched()) {
        setNerve(&NrvSwingRope::SwingRopeNrvBindStretch::sInstance);
    }
}

void SwingRope::initPoints() {
    mNumPoints = mRopeLength / 50.0f;
    mPoints = new SwingRopePoint*[mNumPoints];

    for (s32 idx = 0; idx < mNumPoints; idx++) {
        TVec3f pos(mGravity);
        pos.scale(50.0f * (idx + 1));
        pos.add(mBasePos);
        mPoints[idx] = new SwingRopePoint(pos);
    }

    mSledPoint = new SledRopePoint(mBasePos, 50.0f);
}

void SwingRope::updateHitSensor(HitSensor* pSensor) {
    if (MR::isSensorBinder(pSensor)) {
        TVec3f bottomPos(mGravity);
        bottomPos.scale(mRopeLength);
        bottomPos.add(mBasePos);
        MR::calcPerpendicFootToLineInside(&pSensor->mPosition, *MR::getPlayerPos(), mBasePos, bottomPos);
        return;
    }

    if (MR::isSensorRide(pSensor)) {
        TVec3f sensorPos(mSledPoint->mUp);
        sensorPos.scale(-50.0f);
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
            (!isNerve(&NrvSwingRope::SwingRopeNrvStop::sInstance) && !isNerve(&NrvSwingRope::SwingRopeNrvFree::sInstance))) {
            return false;
        }

        mRider = pSender->mHost;
        mIsStretched = false;
        MR::calcGravity(this, *MR::getPlayerPos());

        TVec3f grav(mGravity);
        TVec3f pos(mPosition);

        MR::calcPerpendicFootToLine(&pos, *MR::getPlayerPos(), mBasePos, mBasePos.addOperatorInLine(grav.scaleInline(mRopeLength)));

        TVec3f posDiff = pos - mBasePos;
        f32 grabCoord = posDiff.dot(grav);
        grabCoord = MR::clamp(grabCoord, 100.0f, mRopeLength - 200.0f);
        mGrabCoord = grabCoord;
        mGrabPointNum = calcPointNo(mGrabCoord);

        TVec3f front;
        MR::getPlayerFrontVec(&front);

        if (__fabsf(mRider->mPosition.y - mBasePos.y) < 1.0f) {
            front.set< f32 >(0.0f, -1.0f, 0.0f);
        }
        mSledPoint->mFront.set(front);

        const TVec3f& riderPos = mRider->mPosition;
        const TVec3f& vel = *MR::getPlayerVelocity();
        mSledPoint->setInfo(riderPos, vel, mBasePos, mGrabCoord);

        updateRideMtx();
        updateFootPos();

        MR::invalidateClipping(this);
        MR::startSound(mRider, "SE_PV_CATCH", -1, -1);
        MR::startSound(mRider, "SE_PM_GRAB_OBJ", -1, -1);
        MR::startActorCameraNoTarget(this, mCameraInfo, -1);

        setNerve(&NrvSwingRope::SwingRopeNrvBindSlideDownStart::sInstance);

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
        setNerve(&NrvSwingRope::SwingRopeNrvFree::sInstance);
        return true;
    }

    return false;
}

f32 SwingRope::calcPointNo(f32 coord) const {
    return MR::clamp((coord / 50.0f) - 1.0f, 0.0f, mNumPoints - 1.0f);
}

f32 SwingRope::calcFriction(s32 index) const {
    if (mRider != nullptr) {
        return (index * (1.0f - 0.9f)) / (mNumPoints - 1) + 0.7f;
    } else {
        return (index * (1.0f - 0.88f)) / (mNumPoints - 1) + 0.8f;
    }
}

void SwingRope::addPointGravity() {
    TVec3f grav(mGravity);
    grav.scale(1.0f);

    for (s32 idx = 0; idx < mNumPoints; idx++) {
        mPoints[idx]->addAccel(grav);
    }
}

void SwingRope::restrictPointToTail(s32 index, const TVec3f* pAnchor, f32 length) {
    if (length == 0.0f) {
        length = 50.0f;
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
        length = 50.0f;
    }
}

void SwingRope::restrictPointToHead(s32 index, const TVec3f& rAnchor, f32 length) {
    TVec3f anchor(rAnchor);

    for (; index >= 0; index--) {
        mPoints[index]->restrict(anchor, length, nullptr);
        anchor.set(mPoints[index]->mPosition);
        anchor.add(mPoints[index]->mVelocity);
        length = 50.0f;
    }
}

bool SwingRope::isAllPointsStop() const {
    for (s32 idx = 0; idx < mNumPoints; idx++) {
        if (__fabsf(mPoints[idx]->mVelocity.x) > 0.1f || __fabsf(mPoints[idx]->mVelocity.y) > 0.1f || __fabsf(mPoints[idx]->mVelocity.z) > 0.1f) {
            return false;
        }

        if (mPoints[idx]->mUp.y < 0.9999f) {
            return false;
        }
    }

    return true;
}

bool SwingRope::isStretched() const {
    return __fabsf(mGrabCoord - PSVECDistance(&mBasePos, &mSledPoint->mPosition)) < 1.0f;
}

bool SwingRope::tryJump() {
    if (MR::testCorePadTriggerA(WPAD_CHAN0) || MR::testSystemTriggerA()) {
        TVec3f front(mSledPoint->mFront);
        TVec3f proj(mGravity);
        proj.scale(mSledPoint->mVelocity.dot(mGravity));
        if (proj.length() < 25.0f) {
            proj = mGravity.scaleInline(-25.0f);
        } else if (proj.length() > 50.0f) {
            proj = mGravity.scaleInline(-50.0f);
        }

        TVec3f velHoriz(mSledPoint->mVelocity);
        MR::vecKillElement(velHoriz, mGravity, &velHoriz);
        if (velHoriz.length() > 35.0f) {
            MR::normalize(&velHoriz);
            front.set(velHoriz);
            velHoriz.scale(35.0f);
        } else if (velHoriz.length() < 5.0f && MR::isGamePadStickOperated(WPAD_CHAN0)) {
            TVec3f stick(0.0f, 0.0f, 0.0f);
            MR::calcWorldStickDirectionXZ(&stick, WPAD_CHAN0);
            front.set< f32 >(stick.x, stick.y, stick.z);
            velHoriz.set(stick.scaleInline(5.0f));
            MR::vecKillElement(velHoriz, mGravity, &velHoriz);
        } else if (!MR::isNearZero(velHoriz, 0.001f)) {
            front.set(velHoriz);
            MR::normalize(&front);
        }

        TVec3f jumpVec(velHoriz);
        jumpVec.add(proj);

        MR::endActorCameraAtLanding(this, mCameraInfo, -1);
        MR::startSound(mRider, "SE_PV_JUMP_S", -1, -1);
        MR::startSound(mRider, "SE_PM_JUMP_M", -1, -1);
        MR::startBckPlayer("SwingRopeSpin", static_cast< s32 >(0));

        MR::setPlayerFrontTargetVec(front, 1);
        MR::endBindAndPlayerWeakGravityJump(this, jumpVec);

        mRider = nullptr;
        setNerve(&NrvSwingRope::SwingRopeNrvFreeInvalid::sInstance);
        return true;
    }

    return false;
}

void SwingRope::updateHangPoint() {
    TVec3f stick(0.0f, 0.0f, 0.0f);
    TVec3f front(mSledPoint->mFront);
    MR::calcWorldStickDirectionXZ(&stick, WPAD_CHAN0);

    if (!MR::isNearZero(stick, 0.001f)) {
        MR::vecBlend(mSledPoint->mFront, stick, &front, 0.1f);
        if (isNerve(&NrvSwingRope::SwingRopeNrvBindLoose::sInstance)) {
            stick.scale(0.8f);
        } else {
            stick.scale(0.5f);
        }
        mSledPoint->addAccel(stick);

        if (mAccelTime == 0) {
            MR::startBckPlayer("SwingRopeAccel", 10);
        } else if (MR::isBckOneTimeAndStopped(mRider)) {
            MR::startBckPlayer("SwingRopeSwingWait", 10);
        }
        mAccelTime++;
    } else {
        if (mAccelTime > 0) {
            MR::startBckPlayer("SwingRopeWait", 10);
            mAccelTime = 0;
        }
    }

    mSledPoint->bindToMapCollision(0.7f);
    mPosition.set(mSledPoint->mPosition);
    MR::calcGravity(this);

    TVec3f grav(mGravity);
    if (isNerve(&NrvSwingRope::SwingRopeNrvBindLoose::sInstance)) {
        grav.scale(0.6f);
    } else {
        grav.scale(0.8f);
    }
    mSledPoint->addAccel(grav);
    mSledPoint->restrict(mBasePos, mGrabCoord, nullptr);
    mSledPoint->updatePosAndAxis(front, 0.995f);

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
    MR::startLevelSound(this, "SE_OJ_LV_ROPE_SWING_WIND_1", soundLvl, -1, -1);

    if (mPrevSoundLvl < 20 && 20 <= soundLvl) {
        MR::startSound(this, "SE_OJ_ROPE_CREAK_H", -1, -1);
    }
    mPrevSoundLvl = soundLvl;
}

void SwingRope::updateRideMtx() {
    mRideMtx.setVecAndTransInline(mSledPoint->mSide, mSledPoint->mUp, mSledPoint->mFront, mSledPoint->mPosition);
}

void SwingRope::updateFootPos() {
    TPos3f footMtx;
    MR::calcPlayerJointMtx(&footMtx, "FootL");

    TVec3f side, up, front;
    footMtx.getXDirInline(side);
    footMtx.getYDirInline(up);
    footMtx.getZDirInline(front);
    footMtx.getTransInline(mFootPos);

    mFootPos.add(side.scaleInline(0.0f).addOperatorInLine(up.scaleInline(-20.0f)).addOperatorInLine(front.scaleInline(10.0f)));
    mGrabToFootDist = PSVECDistance(&mFootPos, &mSledPoint->mPosition);
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
    restrictPointToTail(0, nullptr, 0.0f);

    s32 grabIndex = mGrabPointNum;
    f32 length = mGrabCoord - (grabIndex + 1) * 50.0f;
    TVec3f pos(mSledPoint->mPosition);
    restrictPointToHead(grabIndex, pos, length);
    restrictPointToTail(0, nullptr, 0.0f);

    TVec3f front(mSledPoint->mFront);
    for (s32 idx = 0; idx <= grabIndex; idx++) {
        point = mPoints[idx];
        point->updatePosAndAxis(front, calcFriction(idx));
        front.set(mPoints[idx]->mFront);
    }
}

bool SwingRope::updateSlideDown() {
    mSlideSpeed -= 0.5f;
    mSlideSpeed = mSlideSpeed >= -10.0f ? mSlideSpeed : -10.0f;
    mGrabCoord -= mSlideSpeed;
    mGrabCoord = MR::clamp(mGrabCoord, 100.0f, mRopeLength - 200.0f);

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

    if (mGrabCoord >= mRopeLength - 200.0f) {
        mGrabCoord = mRopeLength - 200.0f;
        mSlideSpeed = 0.0f;
        setNerve(&NrvSwingRope::SwingRopeNrvBindStretch::sInstance);
        return true;
    }

    return false;
}

void SwingRope::updateStretchHangUpperPoints() {
    if (mStretchTime < 10) {
        updateLooseUpperPoints();
    }

    s32 grabIndex = mGrabPointNum;
    s32 maxIndex = mNumPoints - 1;
    if (grabIndex < 0) {
        grabIndex = 0;
    } else if (grabIndex > maxIndex) {
        grabIndex = maxIndex;
    }

    TVec3f front(mSledPoint->mFront);
    for (s32 idx = 0; idx <= grabIndex; idx++) {
        TVec3f pos(mSledPoint->mUp);
        pos.scale(-50.0f * (idx + 1));
        pos.add(mBasePos);

        if (mStretchTime < 10) {
            f32 t = MR::getEaseOutValue((mStretchTime + 1) / 10.0f, 0.0f, 1.0f, 1.0f);
            TVec3f stretchPos(pos);
            pos = stretchPos.scaleInline(t).addOperatorInLine(mPoints[idx]->mPosition.scaleInline(1.0f - t));
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
        restrictPointToTail(nextIndex + 1, &mFootPos, 0.0f);
        updateHangLowerPointPos(nextIndex + 1);

        s32 nextNextIndex = nextIndex + 1;
        MR::vecBlend(mSledPoint->mUp, mPoints[nextNextIndex]->mUp, &up, 0.5f);
        MR::vecBlend(mSledPoint->mFront, mPoints[nextNextIndex]->mFront, &front, 0.5f);
        MR::normalize(&up);
        MR::normalize(&front);

        PSVECCrossProduct(&up, &front, &side);
        if (!MR::isNearZero(side, 0.001f)) {
            MR::makeAxisUpFront(&side, &front, up, front);
            mPoints[nextIndex]->setPosAndAxis(mFootPos, side, up, front);
        }
    }
}

void SwingRope::updateHangLowerPointPos(s32 index) {
    for (; index < mNumPoints; index++) {
        SwingRopePoint* point = mPoints[index];
        SwingRopePoint* prevPoint = mPoints[index - 1];
        TVec3f front(prevPoint->mFront);
        prevPoint->mUp.dot(point->mUp);
        MR::vecBlend(mPoints[index]->mFront, mPoints[index - 1]->mFront, &front, 0.3f);
        point->updatePosAndAxis(front, calcFriction(index));
    }
}

namespace {

    void sendPoint(const TVec3f& pos, const TVec3f& side, const TVec3f& front, f32 x, f32 y, u32 color, f32 texX, f32 texY) {
        GXPosition3f32(pos.x + side.x * x + front.x * y, pos.y + side.y * x + front.y * y, pos.z + side.z * x + front.z * y);
        GXColor1u32(color);
        GXTexCoord2f32(texX, texY);
    }

    void drawLine(const SwingRope* pRope, u32 color1, u32 color2, f32 x1, f32 y1, f32 x2, f32 y2) {
        // FIXME: register swap

        GXBegin(GX_TRIANGLESTRIP, GX_VTXFMT0, (pRope->mNumPoints + 1) * 2);

        const TVec3f& front = pRope->mPoints[0]->mFront;
        const TVec3f& side = pRope->mPoints[0]->mSide;
        const TVec3f& pos = pRope->mBasePos;

        sendPoint(pos, side, front, x1, y1, color1, 0.0f, 0.0f);
        sendPoint(pos, side, front, x2, y2, color2, 1.0f, 0.0f);

        SwingRopePoint* point;
        for (s32 idx = 0; idx < pRope->mNumPoints; idx++) {
            point = pRope->mPoints[idx];
            const TVec3f& front = point->mFront;
            const TVec3f& side = point->mSide;
            const TVec3f& pos = point->mPosition;

            sendPoint(pos, side, front, x1, y1, color1, 0.0f, 0.13f * (idx + 1));
            sendPoint(pos, side, front, x2, y2, color2, 1.0f, 0.13f * (idx + 1));
        }
    }

    void drawLineAtHanging(const SwingRope* pRope, u32 color1, u32 color2, f32 texY1, f32 texY2, f32 x1, f32 y1, f32 x2, f32 y2) {
        // this function is the exact same as drawLine.
        // FIXME: register swap

        GXBegin(GX_TRIANGLESTRIP, GX_VTXFMT0, (pRope->mNumPoints + 1) * 2);

        const TVec3f& front = pRope->mPoints[0]->mFront;
        const TVec3f& side = pRope->mPoints[0]->mSide;
        const TVec3f& pos = pRope->mBasePos;

        sendPoint(pos, side, front, x1, y1, color1, 0.0f, 0.0f);
        sendPoint(pos, side, front, x2, y2, color2, 1.0f, 0.0f);

        SwingRopePoint* point;
        for (s32 idx = 0; idx < pRope->mNumPoints; idx++) {
            point = pRope->mPoints[idx];
            const TVec3f& front = point->mFront;
            const TVec3f& side = point->mSide;
            const TVec3f& pos = point->mPosition;

            sendPoint(pos, side, front, x1, y1, color1, 0.0f, 0.13f * (idx + 1));
            sendPoint(pos, side, front, x2, y2, color2, 1.0f, 0.13f * (idx + 1));
        }
    }
}  // namespace

void SwingRope::drawStop() const {
    // FIXME: register swap
    // https://decomp.me/scratch/N0wL9

    TVec3f bottom(mBasePos.x, mBasePos.y - mRopeLength, mBasePos.z);
    TVec3f& side = mPoints[0]->mSide;
    TVec3f& front = mPoints[0]->mFront;
    f32 texY = (mRopeLength / 50.0f) * 0.13f;
    u32 color1, color2;

    color1 = sColorPlusX;
    color2 = sColorPlusZ;
    GXBegin(GX_TRIANGLESTRIP, GX_VTXFMT0, 4);
    sendPoint(mBasePos, side, front, -30.0f, 43.0f, color1, 0.0f, 0.0f);
    sendPoint(mBasePos, side, front, 33.0f, -43.0f, color2, 1.0f, 0.0f);
    sendPoint(bottom, side, front, -30.0f, 43.0f, color1, 0.0f, texY);
    sendPoint(bottom, side, front, 33.0f, -43.0f, color2, 1.0f, texY);

    color1 = sColorPlusZ;
    color2 = sColorMinusX;
    GXBegin(GX_TRIANGLESTRIP, GX_VTXFMT0, 4);
    sendPoint(mBasePos, side, front, -33.0f, -43.0f, color1, 0.0f, 0.0f);
    sendPoint(mBasePos, side, front, 30.0f, 43.0f, color2, 1.0f, 0.0f);
    sendPoint(bottom, side, front, -33.0f, -43.0f, color1, 0.0f, texY);
    sendPoint(bottom, side, front, 30.0f, 43.0f, color2, 1.0f, texY);

    color1 = sColorMinusX;
    color2 = sColorPlusX;
    GXBegin(GX_TRIANGLESTRIP, GX_VTXFMT0, 4);
    sendPoint(mBasePos, side, front, 43.0f, -3.0f, color1, 0.0f, 0.0f);
    sendPoint(mBasePos, side, front, -43.0f, -3.0f, color2, 1.0f, 0.0f);
    sendPoint(bottom, side, front, 43.0f, -3.0f, color1, 0.0f, texY);
    sendPoint(bottom, side, front, -43.0f, -3.0f, color2, 1.0f, texY);
}

void SwingRope::drawFree() const {
    drawLine(this, sColorPlusZ, sColorPlusX, -30.0f, 43.0f, 33.0f, -43.0f);
    drawLine(this, sColorMinusX, sColorPlusZ, -33.0f, -43.0f, 30.0f, 43.0f);
    drawLine(this, sColorMinusX, sColorPlusZ, 43.0f, -3.0f, -43.0f, -3.0f);
}

void SwingRope::drawBind() const {
    f32 texYGrab = (mGrabPointNum + 1.0f) * 0.13f;
    f32 texYFoot = (calcPointNo(mFootCoord) + 1.0f) * 0.13f;
    drawLineAtHanging(this, sColorPlusZ, sColorPlusX, texYGrab, texYFoot, -30.0f, 43.0f, 33.0f, -43.0f);
    drawLineAtHanging(this, sColorMinusX, sColorPlusZ, texYGrab, texYFoot, -33.0f, -43.0f, 30.0f, 43.0f);
    drawLineAtHanging(this, sColorMinusX, sColorPlusZ, texYGrab, texYFoot, 43.0f, -3.0f, -43.0f, -3.0f);
}

SwingRopeGroup::SwingRopeGroup(const char* pName) : NameObj(pName) {
    mTexture = nullptr;
    mTexture = new JUTTexture(MR::loadTexFromArc("SwingRope.arc", "SwingRope.bti"), 0);

    MR::FunctorV0M< const SwingRopeGroup*, void (SwingRopeGroup::*)() const > preDrawFunctor(this, &SwingRopeGroup::initDraw);
    MR::registerPreDrawFunction(preDrawFunctor, MR::DrawType_SwingRope);
    // The above should probably be this instead, but MR::Functor_Inline does not like consts at the moment
    // MR::registerPreDrawFunction(MR::Functor_Inline(const_cast< const SwingRopeGroup* >(this), &SwingRopeGroup::initDraw),
    // MR::DrawType_SwingRope);
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

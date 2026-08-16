#include "Game/Camera/CameraFollow.hpp"
#include "Game/Camera/CamTranslatorFollow.hpp"
#include "Game/Camera/Camera.hpp"
#include "Game/Camera/CameraCalc.hpp"
#include "Game/Camera/CameraHeightArrange.hpp"
#include "Game/Camera/CameraLocalUtil.hpp"
#include "Game/Camera/CameraTargetObj.hpp"
#include "Game/Map/HitInfo.hpp"
#include "Game/Util/ActorMovementUtil.hpp"
#include "Game/Util/MapUtil.hpp"
#include "Game/Util/MathUtil.hpp"
#include "Game/Util/TriangleFilter.hpp"
#include "Game/Util/VectorUtil.hpp"
#include <JSystem/JGeometry/TUtil.hpp>

void CameraFollow_FORCE_MATCH_SDATA2() {
    (void)1.0f;
    (void)0.0f;
    (void)0.5f;
    f32 f4 = MR::epsilon();
    f32 f5 = JGeometry::TUtil< f32 >::PI();
    f32 f6 = JGeometry::TUtil< f32 >::PI() / 2.0f;
    f32 f7 = -JGeometry::TUtil< f32 >::PI() / 2.0f;
}

namespace {
    static const f32 sCameraRadius = 15.0f;  // TODO: check if this makes sense as this const
    static const f32 sSlopeFrontDist = 500.0f;
    static const f32 sColCheckHeight = 100.0f;
    static const f32 sRoundAddition = 0.08f;
    static const f32 sRoundResetAddition = 0.2f;
    static const f32 sCollisionCheckOffset = 50.0f;
    static const f32 sCollisionCheckDist = 100.0f;
    static f32 sRoundInterval = JGeometry::TUtil< f32 >::PI() / 4.0f;
    // FIXME: sinit instruction order issue
    static f32 sLookDownAngle = 75.0f * JGeometry::TUtil< f32 >::PI() / 180.0f;
};  // namespace

CameraFollow::CameraFollow(const char* pName)
    : Camera(pName), mDistMax(1200.0f), mDistMin(300.0f), mAngleXMin(10.0f * MR::pi() / 180.0f), mAngleXRange(20.0f * MR::pi() / 180.0f),
      mPos(0.0f, 0.0f, 0.0f), mWatchPos(0.0f, 0.0f, 0.0f), mAngleYRoundSpeed(0.15f), mAngleXSpeed(), mCheckWall(true), mIsRounding(), mAngleYTarget(),
      mAngleYSpeed(), mIsRoundLeftValid(), mIsRoundRightValid() {
    createVPanObj();
}

void CameraFollow::reset() {
    mIsRounding = false;
    mAngleXSpeed = 0.0f;

    mVPan->resetJump();
    CameraLocalUtil::setPos(this, CameraLocalUtil::getPos(mCameraMan));
    CameraLocalUtil::setWatchPos(this, CameraLocalUtil::getWatchPos(mCameraMan));
    CameraLocalUtil::setUpVecAndWatchUpVec(this, CameraLocalUtil::getTarget(this)->getUpVec());
    mVPan->reset();

    TPos3f zoneMtx = mZoneMatrix;
    zoneMtx.invert(zoneMtx);
    mPos.set(CameraLocalUtil::getPos(this));
    zoneMtx.mult(mPos, mPos);
    mWatchPos.set(CameraLocalUtil::getWatchPos(this));
    zoneMtx.mult(mWatchPos, mWatchPos);

    mIsRoundLeftValid = false;
    mIsRoundRightValid = false;
}

CameraTargetObj* CameraFollow::calc() {
    mVPan->updateJump();
    mVPan->calcWatchPos(nullptr);
    if (mIsRounding) {
        calcRotPos();
    } else {
        calcIdealPos();
    }
    arrangeHeight();
    mVPan->arrange();
    checkValidityOfRound(CameraLocalUtil::getWatchPos(this), CameraLocalUtil::getPos(this));
    return CameraLocalUtil::getTarget(this);
}

void CameraFollow::setParam(f32 maxDist, f32 minDist, f32 pitchMin, f32 pitchRange, f32 angleRoundSpeed, bool checkWall) {
    mDistMax = maxDist;
    mDistMin = minDist;
    mAngleXMin = pitchMin;
    mAngleXRange = pitchRange;
    mAngleYRoundSpeed = angleRoundSpeed;
    mCheckWall = checkWall;
}

CamTranslatorBase* CameraFollow::createTranslator() {
    return new CamTranslatorFollow(this);
}

bool CameraFollow::isEnableToRoundLeft() const {
    return isUpsideDownByRoll() ? mIsRoundRightValid : mIsRoundLeftValid;
}

bool CameraFollow::isEnableToRoundRight() const {
    return isUpsideDownByRoll() ? mIsRoundLeftValid : mIsRoundRightValid;
}

void CameraFollow::calcIdealPos() {
    const TPos3f& zoneMtx = mZoneMatrix;

    TPos3f mtx = zoneMtx;
    mtx.invert(mtx);
    TVec3f watchPos = CameraLocalUtil::getWatchPos(this);
    mtx.mult(watchPos, watchPos);
    TVec3f front = CameraLocalUtil::getTarget(this)->getFrontVec();
    mtx.mult33(front);

    TVec3f pos = mPos;
    constrainDistance(&pos, watchPos);
    f32 angleY, angleX, length;
    MR::crossToPolar(watchPos, pos, &length, &angleX, &angleY);
    constrainAngleX(&angleX);
    constrainAngleY(&angleY, front, length);
    MR::polarToCross(watchPos, &pos, length, angleX, angleY);
    checkShiftStateToRound(watchPos, pos);

    mPos.set(pos);
    mWatchPos.set(watchPos);

    zoneMtx.mult(pos, pos);
    CameraLocalUtil::setPos(this, pos);

    TVec3f up(0.0f, 1.0f, 0.0f);
    zoneMtx.mult33(up);
    CameraLocalUtil::setUpVecAndWatchUpVec(this, up);
}

void CameraFollow::calcRotPos() {
    TPos3f zoneMtx = mZoneMatrix;
    TPos3f mtx = zoneMtx;
    mtx.invert(mtx);

    TVec3f watchPos = CameraLocalUtil::getWatchPos(this);
    mtx.mult(watchPos, watchPos);

    TVec3f pos = mPos;
    f32 length, angleX, angleY;
    MR::crossToPolar(watchPos, pos, &length, &angleX, &angleY);
    angleY = MR::convergeRadian(angleY, mAngleYTarget, mAngleYSpeed);
    if (angleY < mAngleYTarget + 0.000002f && angleY > mAngleYTarget - 0.000002f) {
        mIsRounding = false;
    }

    TVec3f newPos;
    MR::polarToCross(watchPos, &newPos, length, angleX, angleY);

    Triangle tri;
    TriangleFilterFunc filterFunc(&MR::isCameraCodeThrough);

    TVec3f mapPos;
    TVec3f front = newPos - mPos;
    if (!MR::isNearZero(front)) {
        MR::normalize(&front);

        if (MR::getFirstPolyOnLineToMap(&mapPos, &tri, mPos - front * ::sCollisionCheckOffset, newPos - mPos + front * ::sCollisionCheckDist, nullptr,
                                        &filterFunc)) {
            newPos = mPos;
            mIsRounding = true;
        }
    }

    mPos.set(newPos);
    mWatchPos.set(watchPos);

    zoneMtx.mult(newPos, newPos);
    CameraLocalUtil::setPos(this, newPos);

    TVec3f up(0.0f, 1.0f, 0.0f);
    zoneMtx.mult33(up);
    CameraLocalUtil::setUpVecAndWatchUpVec(this, up);
}

void CameraFollow::constrainDistance(TVec3f* pPos, const TVec3f& rWatchPos) {
    TVec3f diff = *pPos - rWatchPos;
    if (diff.length() < mDistMin) {
        if (MR::isNearZero(diff)) {
            diff.set(mPos - mWatchPos);
        }
        diff.setLength(mDistMin);
    } else if (!(diff.length() < mDistMax)) {
        f32 dist = diff.length();
        MR::normalize(&diff);
        diff.scale(mDistMax + (dist - mDistMax) * 0.5f);
    }

    *pPos = diff + rWatchPos;
}

void CameraFollow::constrainAngleX(f32* pAngleX) {
    if (*pAngleX > mAngleXMin + mAngleXRange) {
        *pAngleX = *pAngleX + (mAngleXMin + mAngleXRange - *pAngleX) * 0.5f;
    }

    if (*pAngleX < mAngleXMin) {
        *pAngleX = *pAngleX + (mAngleXMin - *pAngleX) * 0.5f;
    }
}

void CameraFollow::constrainAngleY(f32* pAngleY, const TVec3f& rDir, f32 vecLength) {
    f32 length, angleX, angleY;
    TVec3f vec = rDir * -vecLength;
    MR::crossToPolar(TVec3f(0.0f, 0.0f, 0.0f), vec, &length, &angleX, &angleY);

    if (CameraLocalUtil::tryCameraResetTrigger()) {
        startRound(angleY, ::sRoundResetAddition);
    } else {
        *pAngleY = MR::convergeRadian(*pAngleY, angleY, getMagnitudeOfMovingRound() * MR::pi() / 180.0f);
    }
}

f32 CameraFollow::getMagnitudeOfMovingRound() const {
    const TVec3f& move = CameraLocalUtil::getTarget(this)->getLastMove();

    TVec3f moveH = move.killElement(CameraLocalUtil::getTarget(this)->getUpVec());
    if (CameraLocalUtil::getTarget(this)->isJumping()) {
        moveH.zero();
    }

    return mAngleYRoundSpeed * MR::clamp(moveH.length() / ::sCameraRadius, 0.0f, 1.0f);
}

void CameraFollow::checkShiftStateToRound(const TVec3f& rWatchPos, const TVec3f& rPos) {
    bool left, right;
    if (isUpsideDownByRoll()) {
        left = CameraLocalUtil::testCameraPadTriggerRoundRight() && mIsRoundLeftValid;
        right = CameraLocalUtil::testCameraPadTriggerRoundLeft() && mIsRoundRightValid;
    } else {
        left = CameraLocalUtil::testCameraPadTriggerRoundLeft() && mIsRoundLeftValid;
        right = CameraLocalUtil::testCameraPadTriggerRoundRight() && mIsRoundRightValid;
    }

    if (left || right) {
        f32 length, angleX, angleY;
        MR::crossToPolar(rWatchPos, rPos, &length, &angleX, &angleY);

        angleY += left ? -::sRoundInterval : ::sRoundInterval;
        startRound(angleY, ::sRoundAddition);
    }
}

void CameraFollow::arrangeHeight() {
    reflectRotOffset();
    if (!mCheckWall) {
        mAngleXSpeed *= 0.99f;
    } else {
        TVec3f camDir = CameraLocalUtil::getWatchPos(this) - CameraLocalUtil::getPos(this);
        if (camDir.dot(CameraLocalUtil::getTarget(this)->getFrontVec()) < 0.0f) {
            mAngleXSpeed *= 0.99f;
        } else {
            if (isWallForward()) {
                arrangeLookUp();
            } else {
                arrangeLookDown();
            }
        }
    }
}

void CameraFollow::reflectRotOffset() {
    f32 length, angleX, angleY;
    MR::crossToPolar(CameraLocalUtil::getWatchPos(this), CameraLocalUtil::getPos(this), &length, &angleX, &angleY);

    angleX += mAngleXSpeed;
    if (angleX > ::sLookDownAngle) {
        angleX = ::sLookDownAngle;
    }

    TVec3f pos;
    MR::polarToCross(CameraLocalUtil::getWatchPos(this), &pos, length, angleX, angleY);
    CameraLocalUtil::setPos(this, pos);
}

bool CameraFollow::isWallForward() const {
    TVec3f pos = CameraLocalUtil::getTarget(this)->getPosition() + CameraLocalUtil::getTarget(this)->getUpVec() * ::sColCheckHeight;
    TVec3f front = CameraLocalUtil::getTarget(this)->getFrontVec() * ::sSlopeFrontDist;
    return Collision::checkStrikeLineToMap(pos, front, 0, nullptr, nullptr) != 0;
}

void CameraFollow::arrangeLookUp() {
    TVec3f mapPos;
    if (!checkAscent(&mapPos)) {
        mAngleXSpeed *= 0.99f;
        return;
    }

    mAngleXSpeed *= 0.99f;
}

bool CameraFollow::checkAscent(TVec3f* pVec) const {
    TVec3f pos = CameraLocalUtil::getTarget(this)->getPosition();
    pos += CameraLocalUtil::getTarget(this)->getFrontVec() * ::sSlopeFrontDist;
    pos += CameraLocalUtil::getTarget(this)->getUpVec() * ::sSlopeFrontDist;
    TVec3f dir = CameraLocalUtil::getTarget(this)->getUpVec() * -::sSlopeFrontDist;

    if (Collision::checkStrikeLineToMap(pos, dir, 0, nullptr, nullptr) == 0) {
        return false;
    }
    pVec->set(Collision::getStrikeInfoMap(0)->mHitPos);
    return true;
}

void CameraFollow::arrangeLookDown() {
    TVec3f mapPos;
    if (checkDescent(&mapPos)) {
        mAngleXSpeed *= 0.99f;
        return;
    }

    f32 length, angleX, angleY;
    MR::crossToPolar(CameraLocalUtil::getWatchPos(this), CameraLocalUtil::getPos(this), &length, &angleX, &angleY);

    if (angleX > ::sLookDownAngle) {
        mAngleXSpeed *= 0.99f;
    } else {
        mAngleXSpeed += (::sLookDownAngle - angleX - mAngleXSpeed) * 0.02f;
    }
}

bool CameraFollow::checkDescent(TVec3f* pVec) const {
    TVec3f pos = CameraLocalUtil::getTarget(this)->getPosition();
    pos += CameraLocalUtil::getTarget(this)->getFrontVec() * ::sSlopeFrontDist;
    pos += CameraLocalUtil::getTarget(this)->getUpVec() * ::sColCheckHeight;
    TVec3f dir = CameraLocalUtil::getTarget(this)->getUpVec() * -::sSlopeFrontDist;

    if (Collision::checkStrikeLineToMap(pos, dir, 0, nullptr, nullptr) == 0) {
        return false;
    }
    pVec->set(Collision::getStrikeInfoMap(0)->mHitPos);
    return true;
}

void CameraFollow::startRound(f32 angle, f32 rate) {
    mAngleYTarget = MR::repeat(angle, 0.0f, MR::pi() * 2.0f);
    mAngleYSpeed = rate;
    mIsRounding = true;
}

void CameraFollow::checkValidityOfRound(const TVec3f& rWatchPos, const TVec3f& rPos) {
    // const TPos3f& zoneMtx = mZoneMatrix;

    TPos3f mtx = mZoneMatrix;
    mtx.invert(mtx);
    TVec3f watchPos = rWatchPos;
    mtx.mult(watchPos, watchPos);
    TVec3f pos = rPos;
    mtx.mult(pos, pos);

    f32 length, angleX, angleY;
    MR::crossToPolar(watchPos, pos, &length, &angleX, &angleY);

    TVec3f newPosLeft;
    MR::polarToCross(watchPos, &newPosLeft, length, angleX, angleY - ::sRoundInterval);
    mZoneMatrix.mult(newPosLeft, newPosLeft);
    if (Collision::checkStrikeLineToMap(rPos, newPosLeft - rPos, 0, nullptr, nullptr) != 0) {
        mIsRoundLeftValid = false;
    } else {
        mIsRoundLeftValid = true;
    }

    TVec3f newPosRight;
    MR::polarToCross(watchPos, &newPosRight, length, angleX, angleY + ::sRoundInterval);
    mZoneMatrix.mult(newPosRight, newPosRight);
    if (Collision::checkStrikeLineToMap(rPos, newPosRight - rPos, 0, nullptr, nullptr) != 0) {
        mIsRoundRightValid = false;
    } else {
        mIsRoundRightValid = true;
    }
}

bool CameraFollow::isUpsideDownByRoll() const {
    return MR::isOppositeDirectionRadian(CameraLocalUtil::getRoll(this), -MR::pi());
}

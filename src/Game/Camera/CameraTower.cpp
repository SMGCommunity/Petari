#include "Game/Camera/CameraTower.hpp"
#include "Game/Camera/CamTranslatorTower.hpp"
#include "Game/Camera/CameraCalc.hpp"
#include "Game/Camera/CameraHeightArrange.hpp"
#include "Game/Camera/CameraLocalUtil.hpp"
#include "Game/Camera/CameraTargetObj.hpp"
#include "Game/Util/MathUtil.hpp"

void CameraTower_FORCE_MATCH_SDATA2() {
    (void)1.0f;
    (void)0.0f;
    (void)0.5f;
    (void)(MR::pi() / 2.0f);
    (void)(-MR::pi() / 2.0f);
}

namespace {
    static const f32 sAttenuation = 0.05f;
    static const f32 sRoundAddition = 0.08f;
    static f32 sRoundInterval = JGeometry::TUtil< f32 >::PI() / 4.0f;
};  // namespace

CameraTower::CameraTower(const char* pName)
    : Camera(pName), mWPoint(0.0f, 0.0f, 0.0f), mAxis(0.0f, 1.0f, 0.0f), mAngleX(), mAngleYRoundSpeed(), mDist(2000.0f), mPos(0.0f, 0.0f, 0.0f),
      mIsRounding(), mRoundIntervalNum(), mAngleYSpeed(), mIsRoundLeftValid(), mIsRoundRightValid() {
    createVPanObj();
}

void CameraTower::reset() {
    mVPan->resetJump();
    mVPan->calcWatchPos(nullptr);

    TPos3f mtx = mZoneMatrix;
    TPos3f inv = mtx;
    inv.invert(inv);

    TVec3f watchPoint = CameraLocalUtil::getWatchPos(this);
    inv.mult(watchPoint, watchPoint);
    CameraLocalUtil::setWatchPos(this, watchPoint);

    calcIdealPos();

    mPos.set(CameraLocalUtil::getPos(this));

    TVec3f pos = CameraLocalUtil::getPos(this);
    TVec3f watchPos = CameraLocalUtil::getWatchPos(this);
    TVec3f up = CameraLocalUtil::getUpVec(this);
    TVec3f watchUp = CameraLocalUtil::getWatchUpVec(this);

    mtx.mult(pos, pos);
    mtx.mult(watchPos, watchPos);
    mtx.mult33(up);
    mtx.mult33(watchUp);

    CameraLocalUtil::setPos(this, pos);
    CameraLocalUtil::setWatchPos(this, watchPos);
    CameraLocalUtil::setUpVec(this, up);
    CameraLocalUtil::setWatchUpVec(this, watchUp);

    mVPan->reset();
}

CameraTargetObj* CameraTower::calc() {
    mVPan->updateJump();
    mVPan->calcWatchPos(nullptr);

    TPos3f mtx = mZoneMatrix;
    TPos3f inv = mtx;
    inv.invert(inv);

    TVec3f watchPoint = CameraLocalUtil::getWatchPos(this);
    inv.mult(watchPoint, watchPoint);
    CameraLocalUtil::setWatchPos(this, watchPoint);

    calcIdealPos();
    arrangeRound();

    TVec3f pos = CameraLocalUtil::getPos(this);
    TVec3f watchPos = CameraLocalUtil::getWatchPos(this);
    TVec3f up = CameraLocalUtil::getUpVec(this);
    TVec3f watchUp = CameraLocalUtil::getWatchUpVec(this);

    mtx.mult(pos, pos);
    mtx.mult(watchPos, watchPos);
    mtx.mult33(up);
    mtx.mult33(watchUp);

    CameraLocalUtil::setPos(this, pos);
    CameraLocalUtil::setWatchPos(this, watchPos);
    CameraLocalUtil::setUpVec(this, up);
    CameraLocalUtil::setWatchUpVec(this, watchUp);

    mVPan->arrange();

    pos.set(CameraLocalUtil::getPos(this));
    inv.mult(pos, pos);
    mPos.y = pos.y;

    return CameraLocalUtil::getTarget(this);
}

CamTranslatorBase* CameraTower::createTranslator() {
    return new CamTranslatorTower(this);
}

bool CameraTower::isEnableToRoundLeft() const {
    return isUpsideDownByRoll() ? mIsRoundRightValid : mIsRoundLeftValid;
}

bool CameraTower::isEnableToRoundRight() const {
    return isUpsideDownByRoll() ? mIsRoundLeftValid : mIsRoundRightValid;
}

void CameraTower::calcIdealPos() {
    if (MR::isNearZero(mAxis)) {
        mAxis.set(0.0f, 1.0f, 0.0f);
    } else {
        mAxis.normalize();
    }

    TVec3f front = CameraLocalUtil::getWatchPos(this) - mWPoint;
    front.set(front - mAxis * mAxis.dot(front));
    if (MR::isNearZero(front)) {
        return;
    }
    MR::normalize(&front);

    TVec3f up(0.0f, 1.0f, 0.0f);
    TVec3f side = front.cross(up);

    TQuat4f rot;
    rot.setRotate(side, mAngleX);
    rot.transform(front);

    TVec3f pos = front * mDist + CameraLocalUtil::getWatchPos(this);

    CameraLocalUtil::setPos(this, pos);
    CameraLocalUtil::setUpVecAndWatchUpVec(this, up);
}

void CameraTower::arrangeRoundingPose() {
    TVec3f pos = CameraLocalUtil::getPos(this);
    TVec3f idealPos = mPos;
    idealPos.y = CameraLocalUtil::getWatchPos(this).y;
    pos.y = CameraLocalUtil::getWatchPos(this).y;

    f32 length, angleX, angleY;
    MR::crossToPolar(CameraLocalUtil::getWatchPos(this), pos, &length, &angleX, &angleY);

    f32 target = mRoundIntervalNum * ::sRoundInterval;
    if (target > mAngleYSpeed) {
        mAngleYSpeed += ::sRoundAddition;
        if (target <= mAngleYSpeed) {
            mAngleYSpeed = target;
            mIsRounding = false;
        }
    } else {
        mAngleYSpeed -= ::sRoundAddition;
        if (target >= mAngleYSpeed) {
            mAngleYSpeed = target;
            mIsRounding = false;
        }
    }

    TVec3f newPos;
    f32 newAngleY = angleY + mAngleYSpeed;
    MR::polarToCross(CameraLocalUtil::getWatchPos(this), &newPos, length, angleX, newAngleY);
    newPos.y = CameraLocalUtil::getPos(this).y;
    CameraLocalUtil::setPos(this, newPos);
}

void CameraTower::arrangeHorizontalPose() {
    TVec3f pos = CameraLocalUtil::getPos(this);
    TVec3f idealPos = mPos;
    idealPos.y = CameraLocalUtil::getWatchPos(this).y;
    pos.y = CameraLocalUtil::getWatchPos(this).y;

    f32 idealLength, idealAngleX, idealAngleY;
    MR::crossToPolar(CameraLocalUtil::getWatchPos(this), idealPos, &idealLength, &idealAngleX, &idealAngleY);

    f32 length, angleX, angleY;
    MR::crossToPolar(CameraLocalUtil::getWatchPos(this), pos, &length, &angleX, &angleY);

    f32 roundAngle = MR::diffRadian(angleY, idealAngleY);

    s32 divNum = (roundAngle < 0.0f ? -1 : 1) * (s32)(((::sRoundInterval / 2) + MR::abs(roundAngle)) / ::sRoundInterval);
    s32 maxDivNum = (mAngleYRoundSpeed + (::sRoundInterval / 2)) / ::sRoundInterval;

    mRoundIntervalNum = divNum;
    mIsRoundLeftValid = mRoundIntervalNum > -maxDivNum;
    mIsRoundRightValid = mRoundIntervalNum < maxDivNum;

    bool left, right;
    if (isUpsideDownByRoll()) {
        left = CameraLocalUtil::testCameraPadTriggerRoundRight() && mIsRoundLeftValid;
        right = CameraLocalUtil::testCameraPadTriggerRoundLeft() && mIsRoundRightValid;
    } else {
        left = CameraLocalUtil::testCameraPadTriggerRoundLeft() && mIsRoundLeftValid;
        right = CameraLocalUtil::testCameraPadTriggerRoundRight() && mIsRoundRightValid;
    }

    f32 newAngleY;
    if (CameraLocalUtil::tryCameraReset()) {
        roundAngle *= 0.7f;
        newAngleY = angleY + roundAngle;
    } else {
        if (left) {
            mIsRounding = true;
            mRoundIntervalNum--;
            mAngleYSpeed = roundAngle;
            newAngleY = angleY + roundAngle;
        } else if (right) {
            mIsRounding = true;
            mRoundIntervalNum++;
            mAngleYSpeed = roundAngle;
            newAngleY = angleY + roundAngle;
        } else {
            if (MR::abs(roundAngle) > mAngleYRoundSpeed) {
                if (roundAngle < 0.0f) {
                    newAngleY = angleY - mAngleYRoundSpeed;
                } else {
                    newAngleY = angleY + mAngleYRoundSpeed;
                }
            } else {
                newAngleY = angleY + roundAngle;
            }
        }
    }

    TVec3f newPos;
    f32 newAngleX = MR::blendAngle(angleX, idealAngleX, ::sAttenuation);
    f32 newLength = idealLength * (1.0f - ::sAttenuation) + length * ::sAttenuation;
    MR::polarToCross(CameraLocalUtil::getWatchPos(this), &newPos, newLength, newAngleX, newAngleY);
    newPos.y = CameraLocalUtil::getPos(this).y;
    CameraLocalUtil::setPos(this, newPos);
}

bool CameraTower::isUpsideDownByRoll() const {
    return MR::isOppositeDirectionRadian(CameraLocalUtil::getRoll(this), -MR::pi());
}

void CameraTower::arrangeRound() {
    if (mIsRounding) {
        arrangeRoundingPose();
    } else {
        arrangeHorizontalPose();
    }

    mPos.set(CameraLocalUtil::getPos(this));
}

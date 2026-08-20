#include "Game/Camera/CameraTowerPos.hpp"
#include "Game/Camera/CamTranslatorTowerPos.hpp"
#include "Game/Camera/CameraCalc.hpp"
#include "Game/Camera/CameraLocalUtil.hpp"
#include "Game/Util/MathUtil.hpp"

void CameraTowerPos_FORCE_MATCH_SDATA2() {
    (void)1.0f;
    (void)0.0f;
    (void)0.5f;
    (void)MR::pi();
    (void)2.0f;
}

namespace {
    static const f32 sAttenuation = 0.05f;
};  // namespace

CameraTowerPos::~CameraTowerPos() {
}

CameraTowerPos::CameraTowerPos(const char* pName)
    : CameraTowerBase(pName), mWPoint(0.0f, 0.0f, 0.0f), mAxis(0.0f, 1.0f, 0.0f), mAngleYMaxSpeed(), mAngleX(), mDist(1000.0f), mAngleXRate(0.5f),
      mPos(0.0f, 0.0f, 0.0f) {
}

void CameraTowerPos::reset() {
    calcIdealPose(false);
}

CameraTargetObj* CameraTowerPos::calc() {
    calcIdealPose(true);

    return CameraLocalUtil::getTarget(this);
}

void CameraTowerPos::calcIdealPose(bool arrange) {
    TVec3f watchPos;
    CameraLocalUtil::makeWatchPoint(&watchPos, this, CameraLocalUtil::getTarget(this), 0.1f / 15.0f);

    TPos3f inv;
    inv.set(mZoneMatrix);
    inv.invert(inv);
    inv.mult(watchPos, watchPos);

    if (MR::isNearZero(mAxis)) {
        mAxis.set(0.0f, 1.0f, 0.0f);
    } else {
        mAxis.normalize();
    }

    TVec3f front = watchPos - mWPoint;
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

    TVec3f pos = front * mDist + watchPos;

    if (arrange) {
        arrangeHorizontalPose(watchPos, pos);
        arrangeWatchPos(watchPos, pos);
    }

    mPos.set(pos);

    TPos3f mtx;
    mtx.set(mZoneMatrix);
    mtx.mult(watchPos, watchPos);
    mtx.mult(pos, pos);
    mtx.mult33(up, up);

    CameraLocalUtil::setWatchPos(this, watchPos);
    CameraLocalUtil::setPos(this, pos);
    CameraLocalUtil::setUpVecAndWatchUpVec(this, up);
}

CamTranslatorBase* CameraTowerPos::createTranslator() {
    return new CamTranslatorTowerPos(this);
}

void CameraTowerPos::arrangeHorizontalPose(const TVec3f& rWatchPos, TVec3f& rPos) {
    TVec3f pos = rPos;
    TVec3f idealPos = mPos;
    idealPos.y = rWatchPos.y;
    pos.y = rWatchPos.y;

    f32 idealLength, idealAngleX, idealAngleY;
    MR::crossToPolar(rWatchPos, idealPos, &idealLength, &idealAngleX, &idealAngleY);

    f32 length, angleX, angleY;
    MR::crossToPolar(rWatchPos, pos, &length, &angleX, &angleY);

    f32 roundAngle = MR::diffRadian(angleY, idealAngleY);

    f32 newAngleY;
    if (CameraLocalUtil::tryCameraReset()) {
        roundAngle *= 0.7f;
        newAngleY = angleY + roundAngle;
    } else {
        if (MR::abs(roundAngle) > mAngleYMaxSpeed) {
            if (roundAngle < 0.0f) {
                newAngleY = angleY - mAngleYMaxSpeed;
            } else {
                newAngleY = angleY + mAngleYMaxSpeed;
            }
        } else {
            newAngleY = angleY + roundAngle;
        }
    }

    TVec3f newPos;
    f32 newAngleX = MR::blendAngle(angleX, idealAngleX, ::sAttenuation);
    f32 newLength = idealLength * (1.0f - ::sAttenuation) + length * ::sAttenuation;
    MR::polarToCross(rWatchPos, &newPos, newLength, newAngleX, newAngleY);
    newPos.y = rPos.y;
    rPos.set(newPos);
}

void CameraTowerPos::arrangeWatchPos(TVec3f& rWatchPos, const TVec3f& rPos) {
    TVec3f offset = rWatchPos - rPos;
    TVec3f idealOffset = mWPoint - rPos;

    TVec3f front = offset;
    front.y = 0.0f;
    MR::normalizeOrZero(&front);

    TVec3f idealFront = idealOffset;
    idealFront.y = 0.0f;

    idealFront = front * front.dot(idealFront);
    idealOffset.x = idealFront.x;
    idealOffset.z = idealFront.z;

    f32 angleDiff = offset.angle(idealOffset);
    f32 fovAngle = CameraLocalUtil::getFovy(this) * MR::pi() / 180.0f * 0.5f;

    if (angleDiff > fovAngle * mAngleXRate) {
        f32 length, angleX, angleY;
        MR::crossToPolar(TVec3f(0.0f, 0.0f, 0.0f), offset, &length, &angleX, &angleY);

        f32 idealLength, idealAngleX, idealAngleY;
        MR::crossToPolar(TVec3f(0.0f, 0.0f, 0.0f), idealOffset, &idealLength, &idealAngleX, &idealAngleY);

        if (angleDiff > fovAngle * mAngleXRate * 2.0f) {
            if (angleX > idealAngleX) {
                angleX -= fovAngle * mAngleXRate;
            } else {
                angleX += fovAngle * mAngleXRate;
            }
        } else {
            if (angleX > idealAngleX) {
                angleX += (idealAngleX - angleX) + fovAngle * mAngleXRate;
            } else {
                angleX += (idealAngleX - angleX) - fovAngle * mAngleXRate;
            }
        }

        MR::polarToCross(TVec3f(0.0f, 0.0f, 0.0f), &offset, length, angleX, angleY);
        rWatchPos.set(offset + rPos);
    }
}

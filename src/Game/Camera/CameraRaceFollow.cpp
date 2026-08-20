#include "Game/Camera/CameraRaceFollow.hpp"
#include "Game/Camera/CamTranslatorRaceFollow.hpp"
#include "Game/Camera/CameraLocalUtil.hpp"
#include "Game/Camera/CameraMan.hpp"
#include "Game/Camera/CameraTargetObj.hpp"
#include "Game/Util/MathUtil.hpp"

void CameraRaceFollow_FORCE_MATCH_SDATA2() {
    (void)0.0f;
    (void)(MR::pi() / 2.0f);
}

CameraRaceFollow::CameraRaceFollow(const char* pName)
    : Camera(pName), mDistMin(500.0f), mDistMax(1200.0f), mAngleX(MR::toRadian(15.0f)), mIsForcePitch(), mRoundAngleSpeed() {
}

void CameraRaceFollow::reset() {
    CameraLocalUtil::setWatchPos(this, CameraLocalUtil::getWatchPos(mCameraMan));
    CameraLocalUtil::setPos(this, CameraLocalUtil::getPos(mCameraMan));
    TVec3f up;
    mCameraMan->mMatrix.getYDir(up);
    CameraLocalUtil::setUpVec(this, up);
}

CameraTargetObj* CameraRaceFollow::calc() {
    TVec3f camDirPrev = CameraLocalUtil::getPos(this) - CameraLocalUtil::getWatchPos(this);

    TVec3f watchPoint;
    CameraLocalUtil::makeWatchPoint(&watchPoint, this, CameraLocalUtil::getTarget(this), 0.1f / 15.0f);
    CameraLocalUtil::setWatchPos(this, watchPoint);

    CameraLocalUtil::arrangeDistanceByPushAndPull(this, mDistMin, mDistMax);

    TVec3f offsetUp = -CameraLocalUtil::getTarget(this)->getGravityVector();

    TVec3f offsetDir = CameraLocalUtil::getPos(this) - CameraLocalUtil::getWatchPos(this);
    f32 camDist = offsetDir.length();
    MR::normalize(&offsetDir);

    f32 angle = offsetUp.angle(offsetDir);
    f32 angleTargetDiff = MR::pi() / 2.0f - mAngleX;

    if (mIsForcePitch || angle > angleTargetDiff) {
        TQuat4f rotX;
        rotX.setRotate(offsetDir, offsetUp, (angle - angleTargetDiff) / angle);
        rotX.transform(offsetDir);
        CameraLocalUtil::setPos(this, offsetDir * camDist + CameraLocalUtil::getWatchPos(this));
    }

    TVec3f frontPrev = camDirPrev;
    TVec3f front = CameraLocalUtil::getPos(this) - CameraLocalUtil::getWatchPos(this);
    if (!MR::isNearZero(frontPrev) && !MR::isNearZero(front)) {
        MR::normalize(&frontPrev);
        MR::normalize(&front);
        TQuat4f rotY;
        rotY.setRotate(frontPrev, front);

        TVec3f localUp = CameraLocalUtil::getUpVec(this);
        rotY.transform(localUp);
        CameraLocalUtil::setUpVec(this, localUp);
    }

    MR::normalize(&front);
    TVec3f side = CameraLocalUtil::getUpVec(this).cross(front);
    TVec3f up1 = front.cross(side);
    CameraLocalUtil::setUpVec(this, up1);

    MR::normalize(&front);

    TVec3f camUp = -CameraLocalUtil::getTarget(this)->getGravityVector();
    camUp.orthogonalize(front);

    TVec3f up = CameraLocalUtil::getUpVec(this);
    TQuat4f rotUp;
    rotUp.setRotate(CameraLocalUtil::getUpVec(this), camUp, 0.05f);
    rotUp.transform(up);
    CameraLocalUtil::setUpVec(this, up);

    goRound();

    CameraLocalUtil::setWatchUpVec(this, CameraLocalUtil::getUpVec(this));

    return CameraLocalUtil::getTarget(this);
}

CamTranslatorBase* CameraRaceFollow::createTranslator() {
    return new CamTranslatorRaceFollow(this);
}

void CameraRaceFollow::goRound() {
    TVec3f targetSide = CameraLocalUtil::getTarget(this)->getSideVec();
    TVec3f offset = CameraLocalUtil::getPos(this) - CameraLocalUtil::getWatchPos(this);

    if (MR::isNearZero(offset)) {
        return;
    }

    TVec3f side = offset.cross(CameraLocalUtil::getUpVec(this));
    if (MR::isNearZero(side)) {
        return;
    }
    MR::normalize(&side);

    TQuat4f rotY;
    rotY.setRotate(side, targetSide, mRoundAngleSpeed);
    rotY.transform(offset);
    CameraLocalUtil::setPos(this, CameraLocalUtil::getWatchPos(this) + offset);
}

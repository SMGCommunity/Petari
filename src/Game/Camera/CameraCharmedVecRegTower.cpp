#include "Game/Camera/CameraCharmedVecRegTower.hpp"
#include "Game/Camera/CamTranslatorCharmedVecRegTower.hpp"
#include "Game/Camera/CameraLocalUtil.hpp"
#include "Game/Camera/CameraTargetObj.hpp"
#include "Game/Util/MathUtil.hpp"

void CameraCharmedVecRegTower_FORCE_MATCH_SDATA2() {
    (void)1.0f;
    (void)0.0f;
}

CameraCharmedVecRegTower::CameraCharmedVecRegTower(const char* pName) : Camera(pName), mArrange() {
}

void CameraCharmedVecRegTower::reset() {
    CameraLocalUtil::setPos(this, CameraLocalUtil::getPos(mCameraMan));
    CameraLocalUtil::setWatchPos(this, CameraLocalUtil::getWatchPos(mCameraMan));
    CameraLocalUtil::setUpVecAndWatchUpVec(this, CameraLocalUtil::getUpVec(mCameraMan));
}

CameraTargetObj* CameraCharmedVecRegTower::calc() {
    TVec3f watchOffset;
    CameraLocalUtil::makeWatchOffset(&watchOffset, this, CameraLocalUtil::getTarget(this), 0.1f / 15.0f);

    TVec3f wPoint = mWPoint;
    TVec3f axis = mAxis;

    TPos3f zoneMtx = mZoneMatrix;
    zoneMtx.mult(wPoint, wPoint);
    zoneMtx.mult33(axis, axis);

    TVec3f watchDir = CameraLocalUtil::getTarget(this)->getPosition() - wPoint;
    if (MR::isNearZero(watchDir)) {
        return nullptr;
    }
    MR::normalize(&watchDir);

    TVec3f front = watchDir.killElement(axis);
    if (MR::isNearZero(front)) {
        return nullptr;
    }
    MR::normalize(&front);

    TVec3f side = axis.cross(front);

    TPos3f mtx;
    mtx.setXYZDir(side, axis, front);
    mtx.setTrans(CameraLocalUtil::getTarget(this)->getPosition());

    TVec3f posOffs(0.0f, 0.0f, mDist);
    mtx.mult(posOffs, posOffs);

    TVec3f regDir = CameraLocalUtil::getVecReg(mName) - posOffs;
    regDir.orthogonalize(axis);
    MR::normalizeOrZero(&regDir);

    f32 angle = regDir.angle(-front);
    if (angle > mAngleYMax) {
        angle = mAngleYMax;
    }

    if (regDir.orientation(axis, -front)) {
        angle = -angle;
    }

    TPos3f transMtx;
    transMtx.makeTrans(0.0f, 0.0f, mDist);

    TPos3f rotX;
    rotX.makeRotate(TVec3f(1.0f, 0.0f, 0.0f), -mAngleX);

    TPos3f rotY;
    rotY.makeRotate(TVec3f(0.0f, 1.0f, 0.0f), angle);

    TPos3f camMtx;
    camMtx.concat(rotX, transMtx);
    camMtx.concat(rotY, camMtx);
    camMtx.concat(mtx, camMtx);

    TVec3f pos;
    camMtx.getTrans(pos);

    CameraLocalUtil::setWatchPos(this, CameraLocalUtil::getTarget(this)->getPosition() + watchOffset);
    CameraLocalUtil::setPos(this, pos);
    CameraLocalUtil::setUpVecAndWatchUpVec(this, axis);

    if (mArrange) {
        CameraLocalUtil::arrangeDistanceByFovy(this, CameraLocalUtil::getVecReg(mName), 200.0f);
    }

    return CameraLocalUtil::getTarget(this);
}

CamTranslatorBase* CameraCharmedVecRegTower::createTranslator() {
    return new CamTranslatorCharmedVecRegTower(this);
}

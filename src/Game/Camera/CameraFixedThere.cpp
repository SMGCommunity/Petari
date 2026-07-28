#include "Game/Camera/CameraFixedThere.hpp"
#include "Game/Camera/CamTranslatorFixedThere.hpp"
#include "Game/Camera/Camera.hpp"
#include "Game/Camera/CameraLocalUtil.hpp"
#include "Game/Camera/CameraMan.hpp"
#include "Game/Camera/CameraTargetObj.hpp"
#include "Game/Util/MathUtil.hpp"
#include "Game/Util/VectorUtil.hpp"

void CameraFixedThere_FORCE_MATCH_SDATA2() {
    (void)1.0f;
}

CameraFixedThere::CameraFixedThere(const char* pName) : Camera(pName), _4C(), _50(), mUp(0.0f, 1.0f, 0.0f), mAxis(0.0f, 0.0f, 1.0f), mRoll() {
}

void CameraFixedThere::reset() {
    copyStatusFromPrevCamera();
    makeAxisAndRoll();
    CameraLocalUtil::setUpVec(this, mUp);
    CameraLocalUtil::setWatchUpVec(this, CameraLocalUtil::getTarget(this)->getUpVec());
}

CameraTargetObj* CameraFixedThere::calc() {
    TVec3f eyeDir;
    TVec3f watchPos;
    CameraLocalUtil::makeWatchPoint(&watchPos, this, CameraLocalUtil::getTarget(this), 1.0f / 150.0f);
    CameraLocalUtil::setWatchPos(this, watchPos);

    if (!calcEyeDir(&eyeDir)) {
        return CameraLocalUtil::getTarget(this);
    } else {
        updateUpVec(eyeDir);
        CameraLocalUtil::setWatchUpVec(this, CameraLocalUtil::getTarget(this)->getUpVec());
        if (!_50) {
            CameraLocalUtil::setFovy(this, CameraLocalUtil::getFovy(mCameraMan));
        }
        return CameraLocalUtil::getTarget(this);
    }
}

CamTranslatorBase* CameraFixedThere::createTranslator() {
    return new CamTranslatorFixedThere(this);
}

void CameraFixedThere::copyStatusFromPrevCamera() {
    const TPos3f& mtx = mCameraMan->mMatrix;

    TVec3f zoneFront;
    TVec3f pos;
    mtx.getZDir(zoneFront);
    zoneFront.negate();

    mtx.getTrans(pos);
    CameraLocalUtil::setPos(this, pos);

    TVec3f front = CameraLocalUtil::getWatchPos(mCameraMan) - pos;
    if (MR::isNearZero(front)) {
        front = zoneFront;
    } else {
        MR::normalize(&front);
    }
    mAxis.set(front);

    TQuat4f rot;
    rot.setRotate(zoneFront, front);
    TVec3f up;
    mtx.getYDir(up);
    rot.transform(up);
    CameraLocalUtil::setUpVec(this, up);
    if (!_50) {
        CameraLocalUtil::setFovy(this, CameraLocalUtil::getFovy(mCameraMan));
    }
    CameraLocalUtil::setWatchPos(this, CameraLocalUtil::getWatchPos(mCameraMan));
    CameraLocalUtil::setWatchUpVec(this, CameraLocalUtil::getTarget(this)->getUpVec());
}

bool CameraFixedThere::calcEyeDir(TVec3f* pDir) {
    pDir->set(CameraLocalUtil::getWatchPos(this) - CameraLocalUtil::getPos(this));
    if (MR::isNearZero(*pDir)) {
        return false;
    }
    MR::normalize(pDir);
    return true;
}

void CameraFixedThere::makeAxisAndRoll() {
    mRoll = 0.0f;
    if (_4C != 0) {
        mUp = CameraLocalUtil::getUpVec(this);
        return;
    }

    mUp = -CameraLocalUtil::getTarget(this)->getGravityVector();
    if (MR::isNearZero(mUp)) {
        mUp = CameraLocalUtil::getUpVec(this);
        return;
    }

    if (__fabsf(mAxis.dot(mUp)) > MR::cosDegree(30.0f)) {
        mUp = CameraLocalUtil::getUpVec(this);
        return;
    }

    if (!MR::diffRadianAroundAxis(&mRoll, mAxis, mUp, CameraLocalUtil::getUpVec(this))) {
        mUp = CameraLocalUtil::getUpVec(this);
        return;
    }
}

void CameraFixedThere::updateUpVec(const TVec3f& rAxis) {
    switch (_4C) {
    case 0: {
        updateNormalUpVec(rAxis);
        break;
    }
    default: {
        TQuat4f rot;
        rot.setRotate(mAxis, rAxis);
        rot.transform(mUp);
        CameraLocalUtil::setUpVec(this, mUp);
    }
    }
    mAxis.set(rAxis);
}

void CameraFixedThere::updateNormalUpVec(const TVec3f& rAxis) {
    // FIXME: TRot3f::setRotate
    TVec3f up = mUp;
    TVec3f side = up.cross(rAxis);
    up.cross(mAxis, side);

    MR::normalize(&up);

    TPos3f rot;
    rot.makeRotate(rAxis, -mRoll);
    rot.mult33(up, up);
    CameraLocalUtil::setUpVec(this, up);
}

CameraFixedThere::~CameraFixedThere() {
}

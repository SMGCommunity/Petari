#include "Game/Camera/CameraFixedPoint.hpp"
#include "Game/Camera/CamTranslatorFixedPoint.hpp"
#include "Game/Camera/CameraLocalUtil.hpp"
#include "Game/Camera/CameraTargetObj.hpp"
#include "Game/Util/MathUtil.hpp"
#include "Game/Util/PlayerUtil.hpp"

void CameraFixedPoint_FORCE_MATCH_SDATA2() {
    (void)1.0f;
}

CameraFixedPoint::CameraFixedPoint(const char* pName) : Camera(pName), mPos(0.0f, 0.0f, 0.0f) {
}

void CameraFixedPoint::reset() {
    CameraLocalUtil::setPos(this, CameraLocalUtil::getPos(mCameraMan));
    CameraLocalUtil::setWatchPos(this, CameraLocalUtil::getWatchPos(mCameraMan));
    CameraLocalUtil::setUpVec(this, CameraLocalUtil::getUpVec(mCameraMan));
    CameraLocalUtil::setWatchUpVec(this, CameraLocalUtil::getWatchUpVec(mCameraMan));
    calc();
}

CameraTargetObj* CameraFixedPoint::calc() {
    TVec3f prevWatchDir = CameraLocalUtil::getWatchPos(this) - CameraLocalUtil::getPos(this);
    MR::normalize(&prevWatchDir);

    TVec3f watchPos;
    CameraLocalUtil::makeWatchPoint(&watchPos, this, CameraLocalUtil::getTarget(this), 0.1f / 15.0f);
    CameraLocalUtil::setWatchPos(this, watchPos);

    TVec3f pos = mPos;
    mZoneMatrix.mult(pos, pos);
    CameraLocalUtil::setPos(this, pos);

    TVec3f watchDir = CameraLocalUtil::getWatchPos(this) - CameraLocalUtil::getPos(this);
    MR::normalize(&watchDir);

    switch (mCameraType) {
    case CameraType_ZoneUp: {
        TVec3f up(0.0f, 1.0f, 0.0f);
        mZoneMatrix.mult33(up, up);
        CameraLocalUtil::setUpVec(this, up);
        break;
    }
    case CameraType_TurnUp: {
        TQuat4f rot;
        rot.setRotate(prevWatchDir, watchDir);
        TVec3f up = CameraLocalUtil::getUpVec(this);
        rot.transform(up);
        CameraLocalUtil::setUpVec(this, up);
        break;
    }
    case CameraType_PlayerUp: {
        TVec3f up;
        MR::getPlayerUpVec(&up);
        CameraLocalUtil::setUpVec(this, up);
        break;
    }
    }

    CameraLocalUtil::setWatchUpVec(this, CameraLocalUtil::getTarget(this)->getUpVec());
    return CameraLocalUtil::getTarget(this);
}

CamTranslatorBase* CameraFixedPoint::createTranslator() {
    return new CamTranslatorFixedPoint(this);
}

void CameraFixedPoint::setParam(const TVec3f& rPos, u32 cameraType) {
    mPos.set(rPos);
    mCameraType = cameraType;
}

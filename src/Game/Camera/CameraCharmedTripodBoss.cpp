#include "Game/Camera/CameraCharmedTripodBoss.hpp"
#include "Game/Boss/TripodBossAccesser.hpp"
#include "Game/Camera/CamTranslatorCharmedTripodBoss.hpp"
#include "Game/Camera/CameraLocalUtil.hpp"
#include "Game/Camera/CameraTargetObj.hpp"

CameraCharmedTripodBoss::CameraCharmedTripodBoss(const char* pName) : Camera(pName) {
    _4C = -1;
    _50.x = 0.0f;
    _50.y = 1.0f;
    _50.z = 0.0f;
    _5C.x = 0.0f;
    _5C.y = 0.0f;
    _5C.z = 1000.0f;
    _68.x = 0.0f;
    _68.y = 0.0f;
}

void CameraCharmedTripodBoss::reset() {
    CameraLocalUtil::setPos(this, CameraLocalUtil::getPos(mCameraMan));
    CameraLocalUtil::setWatchPos(this, CameraLocalUtil::getWatchPos(mCameraMan));
    CameraLocalUtil::setUpVec(this, CameraLocalUtil::getUpVec(mCameraMan));
}

CameraTargetObj* CameraCharmedTripodBoss::calc() {
    CameraTargetObj* result = CameraLocalUtil::getTarget(this);
    TVec3f watchpoint;
    CameraLocalUtil::makeWatchPoint(&watchpoint, this, CameraLocalUtil::getTarget(this), 0.0066666668f);

    TVec3f vec(_50);
    TVec3f renameme(0.0f, 0.0f, 0.0f);

    if (_4C >= 0) {
        TPos3f pos;
        MR::getTripodBossJointMatrix(&pos, _4C);
        pos.getTransInline(renameme);
        pos.mult33Inline(vec, vec);
    }

    TPos3f rotX;
    rotX.makeRotate(TVec3f(-1.0f, 0.0f, 0.0f), _68.x);
    TPos3f rotY;
    rotY.makeRotate(TVec3f(0.0f, 1.0f, 0.0f), _68.y);

    TPos3f finalrot;
    finalrot.concat(rotY, rotX);

    TVec3f subvec = *CameraLocalUtil::getTarget(this)->getPosition() - renameme;
    if (MR::isNearZero(subvec)) {
        return result;
    }

    MR::normalize(&subvec);
    TVec3f cross;
    PSVECCrossProduct(&vec, &subvec, &cross);

    if (MR::isNearZero(cross)) {
        return result;
    }

    MR::normalize(&cross);
    PSVECCrossProduct(&subvec, &cross, &vec);
    MR::normalize(&subvec);
    TPos3f mtx;
    mtx.identity();
    mtx.setXYZDirInline(cross, vec, subvec);
    mtx.setTrans(*CameraLocalUtil::getTarget(this)->getPosition());
    mtx.concat(mtx, finalrot);
    mtx.getZDirInline(subvec);
    mtx.getYDirInline(vec);

    TVec3f v20(_5C);
    mtx.mult(v20, v20);

    TVec3f tert = v20 - subvec.multInLine(_5C.z);
    JMathInlineVEC::PSVECCopy(&tert, &watchpoint);
    CameraLocalUtil::setPos(this, v20);
    CameraLocalUtil::setWatchPos(this, watchpoint);
    CameraLocalUtil::setUpVec(this, vec);
    CameraLocalUtil::setWatchUpVec(this, _50);
    return result;
}

void CameraCharmedTripodBoss::setParam(s32 a1, TVec3f a2, const TVec3f& a3, const TVec2f& a4) {
    _68.x = a4.x;
    _68.y = a4.y;
    _50.set< f32 >(a2);
    _5C.set< f32 >(a3);
    _4C = a1;
}

CameraCharmedTripodBoss::~CameraCharmedTripodBoss() {}

CamTranslatorBase* CameraCharmedTripodBoss::createTranslator() {
    return new CamTranslatorCharmedTripodBoss(this);
}

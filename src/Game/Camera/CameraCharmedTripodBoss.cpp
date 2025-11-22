#include "Game/Camera/CameraCharmedTripodBoss.hpp"
#include "Game/Camera/CameraTargetObj.hpp"
#include "Game/Camera/CamTranslatorCharmedTripodBoss.hpp"
#include "Game/Camera/CameraLocalUtil.hpp"
#include "Game/Boss/TripodBossAccesser.hpp"

CameraCharmedTripodBoss::CameraCharmedTripodBoss(const char* pName)
    : Camera(pName) {
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
    CameraLocalUtil::setPos(this, *CameraLocalUtil::getPos(mCameraMan));
    CameraLocalUtil::setWatchPos(this, *CameraLocalUtil::getWatchPos(mCameraMan));
    CameraLocalUtil::setUpVec(this, *CameraLocalUtil::getUpVec(mCameraMan));
}

CameraTargetObj* CameraCharmedTripodBoss::calc() {
    CameraTargetObj* v2 = CameraLocalUtil::getTarget(this);
    CameraTargetObj* v3 = CameraLocalUtil::getTarget(this);
    TVec3f           v25;
    CameraLocalUtil::makeWatchPoint(&v25, this, v3, 0.0066666668f);
    TVec3f v24(_50);
    TVec3f v23(0.0f, 0.0f, 0.0f);
    if (_4C >= 0) {
        TPos3f v30;
        MR::getTripodBossJointMatrix(&v30, _4C);
        v23.set<f32>(v30.mMtx[0][3], v30.mMtx[1][3], v30.mMtx[2][3]);
        v24.set<f32>(((v24.z * v30.mMtx[0][2]) + ((v24.x * v30.mMtx[0][0]) + (v24.y * v30.mMtx[0][1]))),
                     ((v24.z * v30.mMtx[1][2]) + ((v24.x * v30.mMtx[1][0]) + (v24.y * v30.mMtx[1][1]))),
                     ((v24.z * v30.mMtx[2][2]) + ((v24.x * v30.mMtx[2][0]) + (v24.y * v30.mMtx[2][1]))));
    }

    TVec3f v5(-1.0f, 0.0f, 0.0f);
    TPos3f v29;
    v29.makeRotate(v5, _68.x);
    TVec3f v6(0.0f, 1.0f, 0.0f);
    TPos3f v28;
    v28.makeRotate(v6, _68.y);
    TPos3f v27;
    v27.concat(v28, v29);
    CameraTargetObj* v7 = CameraLocalUtil::getTarget(this);
    TVec3f           v22 = *v7->getPosition();
    if (MR::isNearZero(v22, 0.001f)) {
        return v2;
    }

    MR::normalize(&v22);
    TVec3f v21;
    PSVECCrossProduct(&v24, &v22, &v21);
    if (!MR::isNearZero(v22, 0.001f)) {
        MR::normalize(&v21);
        PSVECCrossProduct(&v22, &v21, &v24);
        MR::normalize(&v22);
        TPos3f v26;
        v26.identity();
        v26.mMtx[0][0] = v21.x;
        v26.mMtx[1][0] = v21.y;
        v26.mMtx[2][0] = v21.z;
        v26.mMtx[0][1] = v24.x;
        v26.mMtx[1][1] = v24.y;
        v26.mMtx[2][1] = v24.z;
        v26.mMtx[0][2] = v22.x;
        v26.mMtx[1][2] = v22.y;
        v26.mMtx[2][2] = v22.z;
        CameraTargetObj* v10 = CameraLocalUtil::getTarget(this);
        const TVec3f*    pos = v10->getPosition();
        v26.mMtx[0][3] = pos->x;
        v26.mMtx[1][3] = pos->y;
        v26.mMtx[2][3] = pos->z;
        v26.concat(v26, v27);
        TVec3f v22;
        v22.set<f32>(v26.mMtx[0][2], v26.mMtx[1][2], v26.mMtx[2][2]);
        TVec3f v24;
        v24.set<f32>(v26.mMtx[0][1], v26.mMtx[1][1], v26.mMtx[2][1]);
        TVec3f v20(_5C);
        v26.mult(v20, v20);
        f32    z = _5C.z;
        TVec3f v14(v22);
        v14.x *= z;
        v14.y *= z;
        v14.z *= z;
        -v14;
        JMathInlineVEC::PSVECCopy(&v14, &v25);
        CameraLocalUtil::setPos(this, v20);
        CameraLocalUtil::setWatchPos(this, v25);
        CameraLocalUtil::setUpVec(this, v24);
        CameraLocalUtil::setWatchUpVec(this, _50);
    }

    return v2;
}

void CameraCharmedTripodBoss::setParam(s32 a1, TVec3f a2, const TVec3f& a3, const TVec2f& a4) {
    _68.x = a4.x;
    _68.y = a4.y;
    _50.set<f32>(a2);
    _5C.set<f32>(a3);
    _4C = a1;
}

CameraCharmedTripodBoss::~CameraCharmedTripodBoss() {
}

CamTranslatorBase* CameraCharmedTripodBoss::createTranslator() {
    return new CamTranslatorCharmedTripodBoss(this);
}

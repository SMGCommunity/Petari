#include "Game/Camera/CameraMtxRegParallel.hpp"
#include "Game/Camera/CamTranslatorMtxRegParallel.hpp"
#include "Game/Camera/CameraCalc.hpp"
#include "Game/Camera/CameraLocalUtil.hpp"
#include "Game/Camera/CameraTargetObj.hpp"

void CameraMtxRegParallel_FORCE_MATCH_SDATA2() {
    (void)0.0f;
    (void)(MR::pi() / 2.0f);
}

CameraMtxRegParallel::~CameraMtxRegParallel() {
}

CameraMtxRegParallel::CameraMtxRegParallel(const char* pName)
    : Camera(pName), mString(), mAngleX(), mAngleY(), mDist(1000.0f), mWPoint(0.0f, 0.0f, 0.0f) {
}

void CameraMtxRegParallel::reset() {
    calcIdealPose();
}

CameraTargetObj* CameraMtxRegParallel::calc() {
    calcIdealPose();
    return CameraLocalUtil::getTarget(this);
}

CamTranslatorBase* CameraMtxRegParallel::createTranslator() {
    return new CamTranslatorMtxRegParallel(this);
}

void CameraMtxRegParallel::calcIdealPose() {
    if (mDist < 300.0f) {
        mDist = 300.0f;
    }

    TVec3f watchPoint;
    CameraLocalUtil::makeWatchPoint(&watchPoint, this, CameraLocalUtil::getTarget(this), 0.1f / 15.0f);

    TPos3f mtx;
    mtx.set(CameraLocalUtil::getMtxReg(mString));

    TVec3f pos;
    MR::polarToCross(TVec3f(0.0f, 0.0f, 0.0f), &pos, mDist, mAngleX, mAngleY - MR::pi() / 2.0f);

    mtx.mult33(pos);
    pos.add(watchPoint);

    TVec3f up;
    mtx.getYDir(up);

    TVec3f wPoint = mWPoint;
    mtx.mult33(wPoint);

    watchPoint.add(wPoint);
    CameraLocalUtil::setWatchPos(this, watchPoint);
    CameraLocalUtil::setPos(this, pos);
    CameraLocalUtil::setUpVec(this, up);
    CameraLocalUtil::setWatchUpVec(this, CameraLocalUtil::getTarget(this)->getUpVec());
}

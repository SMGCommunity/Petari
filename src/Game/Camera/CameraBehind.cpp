#include "Game/Camera/CameraBehind.hpp"
#include "Game/Camera/CamTranslatorBehind.hpp"
#include "Game/Camera/CameraCalc.hpp"
#include "Game/Camera/CameraHeightArrange.hpp"
#include "Game/Camera/CameraLocalUtil.hpp"
#include "Game/Camera/CameraTargetObj.hpp"

void CameraBehind_FORCE_MATCH_SDATA2() {
    (void)1.0f;
    (void)0.0f;
}

CameraBehind::~CameraBehind() {
}

CameraBehind::CameraBehind(const char* pName) : Camera(pName), mAngleX(MR::toRadian(30.0f)), mAngleY(), mDist(3000.0f) {
    createVPanObj();
}

void CameraBehind::reset() {
    mVPan->resetJump();
    mVPan->calcWatchPos(nullptr);
    calcPosAndUp(1.0f);
    mVPan->reset();
}

CameraTargetObj* CameraBehind::calc() {
    mVPan->updateJump();
    mVPan->calcWatchPos(nullptr);
    calcPosAndUp(mRate);
    mVPan->arrange();
    return CameraLocalUtil::getTarget(this);
}

CamTranslatorBase* CameraBehind::createTranslator() {
    return new CamTranslatorBehind(this);
}

void CameraBehind::calcPosAndUp(f32 accel) {
    TVec3f watchpos = CameraLocalUtil::getWatchPos(this);

    TPos3f matrix;
    matrix.setZDir(-CameraLocalUtil::getTarget(this)->getFrontVec());
    matrix.setYDir(CameraLocalUtil::getTarget(this)->getUpVec());
    matrix.setXDir(-CameraLocalUtil::getTarget(this)->getSideVec());
    matrix.setTrans(CameraLocalUtil::getTarget(this)->getPosition());

    TPos3f inv;
    inv.invert(matrix);

    TVec3f watchPoint;
    inv.mult(watchpos, watchPoint);

    TVec3f pos;
    MR::polarToCrossDegree(watchPoint, &pos, mDist, mAngleX, mAngleY);

    matrix.mult(pos, pos);
    pos = pos * accel + CameraLocalUtil::getPos(this) * (1.0f - accel);

    CameraLocalUtil::setPos(this, pos);
    CameraLocalUtil::setWatchUpVec(this, TVec3f(0.0f, 1.0f, 0.0f));
    CameraLocalUtil::setUpVec(this, TVec3f(0.0f, 1.0f, 0.0f));
}

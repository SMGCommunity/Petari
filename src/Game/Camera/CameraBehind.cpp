#include "Game/Camera/CameraBehind.hpp"
#include "Game/Camera/CamTranslatorBehind.hpp"
#include "Game/Camera/CameraCalc.hpp"
#include "Game/Camera/CameraHeightArrange.hpp"
#include "Game/Camera/CameraLocalUtil.hpp"
#include "Game/Camera/CameraTargetObj.hpp"

CameraBehind::~CameraBehind() {
}

CameraBehind::CameraBehind(const char* pName) : Camera(pName) {
    mAngleB = 0.5235988f;
    mAngleA = 0.0f;
    mDist = 3000.0f;

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
    calcPosAndUp(mAxisX);
    mVPan->arrange();
    return CameraLocalUtil::getTarget(this);
}

CamTranslatorBase* CameraBehind::createTranslator() {
    return new CamTranslatorBehind(this);
}

void CameraBehind::calcPosAndUp(f32 accel) {
    TVec3f watchpos(CameraLocalUtil::getWatchPos(this));
    TPos3f matrix;
    matrix.setZDir(-*CameraLocalUtil::getTarget(this)->getFrontVec());
    matrix.setYDir(*CameraLocalUtil::getTarget(this)->getUpVec());
    matrix.setXDir(-*CameraLocalUtil::getTarget(this)->getSideVec());
    matrix.setTrans(*CameraLocalUtil::getTarget(this)->getPosition());
    TPos3f matrix2;
    matrix2.invert(matrix);
    TVec3f multresult;
    matrix2.mult(watchpos, multresult);
    TVec3f cross;
    MR::polarToCrossDegree(multresult, &cross, mDist, mAngleB, mAngleA);
    matrix.mult(cross, cross);
    cross = cross * accel + CameraLocalUtil::getPos(this) * (1.0f - accel);
    CameraLocalUtil::setPos(this, cross);

    CameraLocalUtil::setWatchUpVec(this, TVec3f(0.0f, 1.0f, 0.0f));
    CameraLocalUtil::setUpVec(this, TVec3f(0.0f, 1.0f, 0.0f));
}

#include "Game/Camera/CameraLocalUtil.h"
#include "Game/Camera/CameraManSubjective.h"
#include "Game/Camera/CameraSubjective.h"

CameraManSubjective::CameraManSubjective(const char *pName) : CameraMan(pName) {
    mCamera = new CameraSubjective("主観カメラ");
    mCamera->mCameraMan = this;
}

CameraManSubjective::~CameraManSubjective() {

}

void CameraManSubjective::init(const JMapInfoIter &rIter) {

}

void CameraManSubjective::calc() {
    mCamera->calc();
    CameraLocalUtil::calcSafePose(this, mCamera);
}

void CameraManSubjective::notifyActivate() {
    mCamera->reset();
}

void CameraManSubjective::notifyDeactivate() {
    
}

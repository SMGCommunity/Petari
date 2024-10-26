#include "Game/Camera/CameraLocalUtil.hpp"
#include "Game/Camera/CameraManSubjective.hpp"
#include "Game/Camera/CameraSubjective.hpp"

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

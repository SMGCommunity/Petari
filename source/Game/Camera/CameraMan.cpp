#include "Game/Camera/CameraDirector.h"
#include "Game/Camera/CameraMan.h"
#include "Game/Camera/CameraPoseParam.h"

CameraMan::CameraMan(const char *pName) : NameObj(pName) {
    mDirector = NULL;
    mPoseParam = new CameraPoseParam();
    mIsActivated = false;
    _15 = false;
    mMatrix.identity();
}

void CameraMan::init(const JMapInfoIter &rIter) {

}

void CameraMan::movement() {
    if (mIsActivated) {
        calc();
        _15 = false;
    }
}

void CameraMan::calc() {

}

void CameraMan::notifyActivate() {

}

void CameraMan::notifyDeactivate() {

}

void CameraMan::owned(CameraDirector *pDirector) {
    mDirector = pDirector;
}

void CameraMan::released(CameraDirector *pDirector) {
    mDirector = NULL;
}

void CameraMan::activate(CameraDirector *pDirector) {
    mIsActivated = true;
    notifyActivate();
}

void CameraMan::deactivate(CameraDirector *pDirector) {
    mIsActivated = false;
    notifyDeactivate();
}
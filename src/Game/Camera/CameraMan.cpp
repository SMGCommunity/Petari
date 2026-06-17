#include "Game/Camera/CameraMan.hpp"
#include "Game/Camera/CameraDirector.hpp"
#include "Game/Camera/CameraPoseParam.hpp"

CameraMan::CameraMan(const char* pName) : NameObj(pName) {
    mDirector = nullptr;
    mPoseParam = new CameraPoseParam();
    mIsActivated = false;
    _15 = false;
    mMatrix.identity();
}

void CameraMan::owned(CameraDirector* pDirector) {
    mDirector = pDirector;
}

void CameraMan::released(CameraDirector* pDirector) {
    mDirector = nullptr;
}

void CameraMan::activate(CameraDirector* pDirector) {
    mIsActivated = true;
    notifyActivate();
}

void CameraMan::deactivate(CameraDirector* pDirector) {
    mIsActivated = false;
    notifyDeactivate();
}

void CameraMan::movement() {
    if (mIsActivated) {
        calc();
        _15 = false;
    }
}

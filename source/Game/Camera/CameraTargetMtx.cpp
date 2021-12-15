#include "Game/Camera/CameraTargetMtx.h"

CameraTargetObj::~CameraTargetObj() {

}

void CameraTargetObj::init(const JMapInfoIter &rIter) {

}

bool CameraTargetObj::isTurning() const {
    return false;
}

bool CameraTargetObj::isLongDrop() const {
    return false;
}

bool CameraTargetObj::isBeeMode() const {
    return false;
}

u32 CameraTargetObj::getSpecialMode() const {
    return 0;
}

void CameraTargetObj::enableCameraWall() {
    mCameraWall = true;
}

void CameraTargetObj::disableCameraWall() {
    mCameraWall = false;
}

void CameraTargetObj::setCameraWall(bool enable) {
    mCameraWall = enable;
}

bool CameraTargetObj::isDebugMode() const {
    return false;
}

void CameraTargetObj::resetStatus() {
    
}
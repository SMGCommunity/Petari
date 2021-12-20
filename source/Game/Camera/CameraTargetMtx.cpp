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

CameraTargetMtx::CameraTargetMtx(const char *pName) : CameraTargetObj(pName) {
    _40 = 0.0f;
    _44 = 0.0f;
    _48 = 0.0f;
    _4C = 0.0f;
    _50 = 0.0f;
    _54 = 0.0f;
    _58 = 0.0f;
    _5C = -1.0f;
    _60 = 0.0f;
    _64 = 0.0f;
    _68 = 1.0f;
    _6C = 0.0f;
    _70 = 0.0f;
    _74 = 0.0f;
    _78 = 1.0f;
    _7C = 0.0f;
    _80 = 0.0f;
    _84 = 1.0f;
    _88 = 0;
    _8C = 0;
    _10.identity();
}
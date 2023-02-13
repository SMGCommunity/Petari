#include "Game/Camera/CameraLocalUtil.h"
#include "Game/Camera/CameraManPause.h"
#include "Game/Camera/CameraTargetObj.h"

TMtx34f *CameraTargetObj::getMapBaseMtx() const {
    return nullptr;
}

CameraManPause::CameraManPause(const char *pName) : CameraMan(pName) {
    _48 = 6000.0f;
    _4C = 0.0f;
    _50 = 0.0f;
    _54 = 0.0f;
    _58.x = 3230.0f;
    _58.y = 13350.0f;
    _58.z = 520.0f;

    CameraLocalUtil::setFovy(this, 45.0f);
}

CameraManPause::~CameraManPause() {

}

    
void CameraManPause::init(const JMapInfoIter &rIter) {

}

/*void CameraManPause::calc() {

}*/

/*void CameraManPause::notifyActivate() {
    _50 = 0.0f;
    _54 = 0.0f;
    _58 = 0.0f;

    if (CameraLocalUtil::getTarget(this) != nullptr) {
        CameraTargetObj *target = CameraLocalUtil::getTarget(this);
        TVec3f *position = target->getPosition();

        TVec3f auStack56 = _58 - *position;
        // Accesses global data
    }
}*/

void CameraManPause::notifyDeactivate() {

}

bool CameraManPause::isCollisionOff() const {
    return true;
}

bool CameraManPause::isZeroFrameMoveOff() const {
    return true;
}

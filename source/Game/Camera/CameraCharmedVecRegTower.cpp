#include "Game/Camera/CameraCharmedVecRegTower.h"
#include "Game/Camera/CamTranslatorCharmedVecRegTower.h"

CameraCharmedVecRegTower::CameraCharmedVecRegTower(const char *pName) : Camera(pName) {
    _70 = false;
}

CamTranslatorBase *CameraCharmedVecRegTower::createTranslator() {
    return new CamTranslatorCharmedVecRegTower(this);
}
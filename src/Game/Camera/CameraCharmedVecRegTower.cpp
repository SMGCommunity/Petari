#include "Game/Camera/CameraCharmedVecRegTower.hpp"
#include "Game/Camera/CamTranslatorCharmedVecRegTower.hpp"

CameraCharmedVecRegTower::CameraCharmedVecRegTower(const char *pName) : Camera(pName) {
    _70 = false;
}

CameraCharmedVecRegTower::~CameraCharmedVecRegTower() {

}

CamTranslatorBase *CameraCharmedVecRegTower::createTranslator() {
    return new CamTranslatorCharmedVecRegTower(this);
}
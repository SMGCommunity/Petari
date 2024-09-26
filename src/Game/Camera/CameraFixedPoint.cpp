#include "Game/Camera/CameraFixedPoint.hpp"
#include "Game/Camera/CamTranslatorFixedPoint.hpp"

CameraFixedPoint::CameraFixedPoint(const char *pName) : Camera(pName) {
    _4C = 0.0f;
    _50 = 0.0f;
    _54 = 0.0f;
}

CameraFixedPoint::~CameraFixedPoint() {

}

CamTranslatorBase *CameraFixedPoint::createTranslator() {
    return new CamTranslatorFixedPoint(this);
}

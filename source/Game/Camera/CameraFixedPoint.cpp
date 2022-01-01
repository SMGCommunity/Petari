#include "Game/Camera/CameraFixedPoint.h"
#include "Game/Camera/CamTranslatorFixedPoint.h"

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

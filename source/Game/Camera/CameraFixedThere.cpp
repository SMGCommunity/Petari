#include "Game/Camera/CameraFixedThere.h"
#include "Game/Camera/CamTranslatorFixedThere.h"

CameraFixedThere::CameraFixedThere(const char *pName) : Camera(pName) {
    _4C = 0;
    _50 = false;
    _54 = 0.0f;
    _58 = 1.0f;
    _5C = 0.0f;
    _60 = 0.0f;
    _64 = 0.0f;
    _68 = 1.0f;
    _6C = 0.0f;
}

CameraFixedThere::~CameraFixedThere() {

}

CamTranslatorBase *CameraFixedThere::createTranslator() {
    return new CamTranslatorFixedThere(this);
}

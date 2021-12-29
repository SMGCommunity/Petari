#include "Game/Camera/CameraRailWatch.h"
#include "Game/Camera/CamTranslatorRailWatch.h"

CameraRailWatch::CameraRailWatch(const char *pName) : Camera(pName) {
    _4C = 0;
    _50 = 0;
    _54 = 0;
    _58 = 0.0f;
    _5C = 1200.0f;
    _60 = 0.0f;
}

CamTranslatorDummy *CameraRailWatch::createTranslator() {
    return new CamTranslatorRailWatch(this);
}

#include "Game/Camera/CameraRailWatch.hpp"
#include "Game/Camera/CamTranslatorRailWatch.hpp"

CameraRailWatch::CameraRailWatch(const char *pName) : Camera(pName) {
    _4C = 0;
    _50 = 0;
    _54 = 0;
    _58 = 0.0f;
    _5C = 1200.0f;
    _60 = 0.0f;
}

CameraRailWatch::~CameraRailWatch() {

}

CamTranslatorBase *CameraRailWatch::createTranslator() {
    return new CamTranslatorRailWatch(this);
}

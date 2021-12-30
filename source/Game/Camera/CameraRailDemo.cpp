#include "Game/Camera/CameraRailDemo.h"
#include "Game/Camera/CamTranslatorRailDemo.h"

CameraRailDemo::CameraRailDemo(const char *pName) : Camera(pName) {
    
}

CamTranslatorBase *CameraRailDemo::createTranslator() {
    return new CamTranslatorRailDemo(this);
}

#include "Game/Camera/CameraRailDemo.hpp"
#include "Game/Camera/CamTranslatorRailDemo.hpp"

CameraRailDemo::CameraRailDemo(const char *pName) : Camera(pName) {
    
}

void CameraRailDemo::reset() {
    _58 = 0;
    _60 = 0.0f;
}

CameraTargetObj* CameraRailDemo::calc() {
    switch(_50) {
        case 0:
            calcLinear();
            break;
        case 1:
            calcEaseInOut();
            break;
        case 2:
            calcDamp();
            break;
    }

    _58 = _58 + 1;
    return nullptr;
}

CameraRailDemo::~CameraRailDemo() {

}

CamTranslatorBase *CameraRailDemo::createTranslator() {
    return new CamTranslatorRailDemo(this);
}

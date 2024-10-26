#include "Game/Camera/CameraTrundle.hpp"
#include "Game/Camera/CamTranslatorTrundle.hpp"

CameraTrundle::CameraTrundle(const char *pName) : Camera(pName) {
    mWPoint.x = 0.0f;
    mWPoint.y = 0.0f;
    mWPoint.z = 0.0f;
    mAxis.x = 1.0f;
    mAxis.y = 0.0f;
    mAxis.z = 0.0f;
    mDist = 2000.0f;
    mAngleA = 0.0f;
    mAngleB = 0.0f;
    mUpX = 0.0f;
}

CameraTrundle::~CameraTrundle() {

}

CamTranslatorBase *CameraTrundle::createTranslator() {
    return new CamTranslatorTrundle(this);
}

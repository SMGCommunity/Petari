#include "Game/Camera/CameraMtxRegParallel.h"
#include "Game/Camera/CamTranslatorMtxRegParallel.h"

CameraMtxRegParallel::CameraMtxRegParallel(const char *pName) : Camera(pName) {
    mString = nullptr;
    mAngleB = 0.0f;
    mAngleA = 0.0f;
    mDist = 1000.0f;
    mWPoint.x = 0.0f;
    mWPoint.y = 0.0f;
    mWPoint.z = 0.0f;
}

CameraMtxRegParallel::~CameraMtxRegParallel() {

}

CamTranslatorBase *CameraMtxRegParallel::createTranslator() {
    return new CamTranslatorMtxRegParallel(this);
}

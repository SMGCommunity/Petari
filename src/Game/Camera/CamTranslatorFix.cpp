#include "Game/Camera/CamTranslatorFix.hpp"
#include "Game/Camera/CameraCalc.hpp"
#include "Game/Camera/CameraParamChunk.hpp"

void CamTranslatorFix::setParam(const CameraParamChunk* pChunk) {
    CameraGeneralParam* general = pChunk->mGeneralParam;

    TVec3f aTStack32;
    TVec3f origin = TVec3f(0.0f, 0.0f, 0.0f);

    MR::polarToCross(origin, &aTStack32, general->mDist, general->mAxis.x, general->mAxis.y);

    mCamera->setParam(general->mWPoint, -aTStack32, general->mUp);
}

Camera* CamTranslatorFix::getCamera() const {
    return mCamera;
}

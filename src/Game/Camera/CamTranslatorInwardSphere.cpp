#include "Game/Camera/CamTranslatorInwardSphere.hpp"
#include "Game/Camera/CameraParamChunk.hpp"

void CamTranslatorInwardSphere::setParam(const CameraParamChunk* pChunk) {
    CameraGeneralParam* general = pChunk->mGeneralParam;
    mCamera->setParam(general->mAngleA, general->mDist, general->mAngleB);
}

Camera* CamTranslatorInwardSphere::getCamera() const {
    return mCamera;
}

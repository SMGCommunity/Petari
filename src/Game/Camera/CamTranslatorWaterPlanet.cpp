#include "Game/Camera/CamTranslatorWaterPlanet.hpp"
#include "Game/Camera/CameraParamChunk.hpp"

CamTranslatorWaterPlanet::CamTranslatorWaterPlanet(CameraWaterPlanet* pCamera) : mCamera(pCamera) {
}

void CamTranslatorWaterPlanet::setParam(const CameraParamChunk* pChunk) {
    CameraGeneralParam* general = pChunk->mGeneralParam;

    mCamera->setParam(general->mAxis.x, general->mAxis.y, general->mAngleA);
}

Camera* CamTranslatorWaterPlanet::getCamera() const {
    return mCamera;
}

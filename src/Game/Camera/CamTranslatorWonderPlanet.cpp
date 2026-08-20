#include "Game/Camera/CamTranslatorWonderPlanet.hpp"
#include "Game/Camera/CameraParamChunk.hpp"

CamTranslatorWonderPlanet::CamTranslatorWonderPlanet(CameraWonderPlanet* pCamera) : mCamera(pCamera) {
}

void CamTranslatorWonderPlanet::setParam(const CameraParamChunk* pChunk) {
    CameraGeneralParam* general = pChunk->mGeneralParam;

    mCamera->setParam(general->mAxis.x, general->mAxis.y, general->mAngleA);
}

Camera* CamTranslatorWonderPlanet::getCamera() const {
    return mCamera;
}

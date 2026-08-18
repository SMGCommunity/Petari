#include "Game/Camera/CamTranslatorWaterPlanetBoss.hpp"
#include "Game/Camera/CameraParamChunk.hpp"

CamTranslatorWaterPlanetBoss::CamTranslatorWaterPlanetBoss(CameraWaterPlanetBoss* pCamera) : mCamera(pCamera) {
}

void CamTranslatorWaterPlanetBoss::setParam(const CameraParamChunk* pChunk) {
    CameraGeneralParam* general = pChunk->mGeneralParam;

    mCamera->setParam(general->mAxis.y, general->mAxis.x, general->mDist, general->mNum1 != 0, pChunk->mGeneralParam->mWPoint, general->mAxis.z,
                      general->mUp.x, general->mUp.y, general->mUp.z);
}

Camera* CamTranslatorWaterPlanetBoss::getCamera() const {
    return mCamera;
}

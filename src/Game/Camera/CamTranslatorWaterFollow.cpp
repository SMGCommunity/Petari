#include "Game/Camera/CamTranslatorWaterFollow.hpp"
#include "Game/Camera/CameraParamChunk.hpp"

CamTranslatorWaterFollow::CamTranslatorWaterFollow(CameraWaterFollow* pCamera) : mCamera(pCamera) {
}

void CamTranslatorWaterFollow::setParam(const CameraParamChunk* pChunk) {
    CameraGeneralParam* general = pChunk->mGeneralParam;

    mCamera->setParam(general->mAxis.y, general->mAxis.x, general->mDist);
}

Camera* CamTranslatorWaterFollow::getCamera() const {
    return mCamera;
}

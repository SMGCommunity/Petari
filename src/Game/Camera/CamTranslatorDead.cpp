#include "Game/Camera/CamTranslatorDead.hpp"
#include "Game/Camera/CameraParamChunk.hpp"

CamTranslatorDead::CamTranslatorDead(CameraDead* pCamera) : mCamera(pCamera) {
}

void CamTranslatorDead::setParam(const CameraParamChunk* pChunk) {
    CameraGeneralParam* general = pChunk->mGeneralParam;

    mCamera->setParam(general->mDist, general->mNum1, general->mNum2);
}

Camera* CamTranslatorDead::getCamera() const {
    return mCamera;
}

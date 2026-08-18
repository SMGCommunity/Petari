#include "Game/Camera/CamTranslatorFooFighter.hpp"
#include "Game/Camera/CameraParamChunk.hpp"

CamTranslatorFooFighter::CamTranslatorFooFighter(CameraFooFighter* pCamera) : mCamera(pCamera) {
}

void CamTranslatorFooFighter::setParam(const CameraParamChunk* pChunk) {
    CameraGeneralParam* general = pChunk->mGeneralParam;

    mCamera->setParam(general->mAxis.y, general->mAxis.x, general->mDist);
}

Camera* CamTranslatorFooFighter::getCamera() const {
    return mCamera;
}

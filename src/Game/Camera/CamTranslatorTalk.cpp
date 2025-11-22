#include "Game/Camera/CamTranslatorTalk.hpp"
#include "Game/Camera/CameraParamChunk.hpp"

void CamTranslatorTalk::setParam(const CameraParamChunk* pChunk) {
    mCamera->setParam(pChunk->mGeneralParam->mWPoint, pChunk->mGeneralParam->mUp, pChunk->mGeneralParam->mAxis.x, pChunk->mGeneralParam->mAxis.y);
}

Camera* CamTranslatorTalk::getCamera() const {
    return mCamera;
}
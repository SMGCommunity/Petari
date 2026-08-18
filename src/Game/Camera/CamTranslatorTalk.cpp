#include "Game/Camera/CamTranslatorTalk.hpp"
#include "Game/Camera/CameraParamChunk.hpp"

void CamTranslatorTalk::setParam(const CameraParamChunk* pChunk) {
    CameraGeneralParam* general = pChunk->mGeneralParam;
    mCamera->setParam(general->mWPoint, general->mUp, general->mAxis.x, general->mAxis.y);
}

Camera* CamTranslatorTalk::getCamera() const {
    return mCamera;
}

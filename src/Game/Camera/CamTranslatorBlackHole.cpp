#include "Game/Camera/CamTranslatorBlackHole.hpp"
#include "Game/Camera/CameraParamChunk.hpp"

void CamTranslatorBlackHole::setParam(const CameraParamChunk* pChunk) {
    CameraGeneralParam* general = pChunk->mGeneralParam;

    mCamera->setParam(general->mWPoint, general->mAxis);
}

Camera* CamTranslatorBlackHole::getCamera() const {
    return mCamera;
}

#include "Game/Camera/CamTranslatorFixedThere.hpp"
#include "Game/Camera/CameraParamChunk.hpp"

void CamTranslatorFixedThere::setParam(const CameraParamChunk* pChunk) {
    CameraGeneralParam* general = pChunk->mGeneralParam;

    mCamera->setParam(general->mNum1, general->mNum2 == 1);
}

Camera* CamTranslatorFixedThere::getCamera() const {
    return mCamera;
}

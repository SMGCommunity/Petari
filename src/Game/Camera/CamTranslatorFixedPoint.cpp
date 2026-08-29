#include "Game/Camera/CamTranslatorFixedPoint.hpp"
#include "Game/Camera/CameraParamChunk.hpp"

void CamTranslatorFixedPoint::setParam(const CameraParamChunk* pChunk) {
    CameraGeneralParam* general = pChunk->mGeneralParam;

    mCamera->setParam(general->mWPoint, general->mNum1);
}

Camera* CamTranslatorFixedPoint::getCamera() const {
    return mCamera;
}

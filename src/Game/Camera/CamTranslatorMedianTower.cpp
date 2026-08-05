#include "Game/Camera/CamTranslatorMedianTower.hpp"
#include "Game/Camera/CameraParamChunk.hpp"

void CamTranslatorMedianTower::setParam(const CameraParamChunk* pChunk) {
    CameraGeneralParam* general = pChunk->mGeneralParam;

    mCamera->setParam(general->mString.getCharPtr(), general->mWPoint, general->mAxis, general->mAngleA, general->mUp.x, general->mUp.y,
                      general->mUp.z);
}

Camera* CamTranslatorMedianTower::getCamera() const {
    return mCamera;
}

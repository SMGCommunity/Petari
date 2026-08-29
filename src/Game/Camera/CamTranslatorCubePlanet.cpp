#include "Game/Camera/CamTranslatorCubePlanet.hpp"
#include "Game/Camera/CameraParamChunk.hpp"

void CamTranslatorCubePlanet::setParam(const CameraParamChunk* pChunk) {
    CameraGeneralParam* general = pChunk->mGeneralParam;

    mCamera->setParam(general->mDist, general->mAngleA, general->mAngleB);
}

Camera* CamTranslatorCubePlanet::getCamera() const {
    return mCamera;
}

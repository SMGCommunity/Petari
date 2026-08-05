#include "Game/Camera/CamTranslatorMedianPlanet.hpp"
#include "Game/Camera/CameraParamChunk.hpp"

CamTranslatorMedianPlanet::CamTranslatorMedianPlanet(CameraMedianPlanet* pCamera) : mCamera(pCamera) {
}

void CamTranslatorMedianPlanet::setParam(const CameraParamChunk* pChunk) {
    CameraGeneralParam* general = pChunk->mGeneralParam;

    mCamera->setParam(general->mString.getCharPtr(), general->mAxis.x, general->mAxis.y, general->mAngleA, general->mAxis.z, general->mAngleB,
                      general->mDist, general->mWPoint.x);
}

Camera* CamTranslatorMedianPlanet::getCamera() const {
    return mCamera;
}

#include "Game/Camera/CamTranslatorFooFighterPlanet.hpp"
#include "Game/Camera/CameraParamChunk.hpp"

CamTranslatorFooFighterPlanet::CamTranslatorFooFighterPlanet(CameraFooFighterPlanet* pCamera) : mCamera(pCamera) {
}

void CamTranslatorFooFighterPlanet::setParam(const CameraParamChunk* pChunk) {
    CameraGeneralParam* general = pChunk->mGeneralParam;

    mCamera->setParam(general->mAxis.x, general->mAxis.y, general->mAngleA);
}

Camera* CamTranslatorFooFighterPlanet::getCamera() const {
    return mCamera;
}

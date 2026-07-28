#include "Game/Camera/CamTranslatorFooFighterPlanet.hpp"
#include "Game/Camera/CameraParamChunk.hpp"

CamTranslatorFooFighterPlanet::CamTranslatorFooFighterPlanet(CameraFooFighterPlanet* pCamera) : mCamera(pCamera) {
}

void CamTranslatorFooFighterPlanet::setParam(const CameraParamChunk* pChunk) {
    CameraGeneralParam* general = pChunk->mGeneralParam;
    CameraFooFighterPlanet* camera = mCamera;

    f32 axisX;
    f32 axisY;
    f32 angleA;

    angleA = general->mAngleA;
    axisY = general->mAxis.y;
    axisX = general->mAxis.x;

    camera->mDistMin = axisX;
    camera->mDistMax = axisY;
    camera->mPitchMax = angleA;
}

Camera* CamTranslatorFooFighterPlanet::getCamera() const {
    return mCamera;
}

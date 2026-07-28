#include "Game/Camera/CamTranslatorFooFighter.hpp"
#include "Game/Camera/CameraParamChunk.hpp"

CamTranslatorFooFighter::CamTranslatorFooFighter(CameraFooFighter* pCamera) : mCamera(pCamera) {
}

void CamTranslatorFooFighter::setParam(const CameraParamChunk* pChunk) {
    CameraGeneralParam* general = pChunk->mGeneralParam;
    CameraFooFighter* camera = mCamera;

    f32 distMin;
    f32 distMax;
    f32 rate;

    rate = general->mDist;
    distMax = general->mAxis.x;
    distMin = general->mAxis.y;

    camera->mDistMin = distMin;
    camera->mDistMax = distMax;
    camera->_54 = rate;
}

Camera* CamTranslatorFooFighter::getCamera() const {
    return mCamera;
}

#include "Game/Camera/CamTranslatorTripodBoss.hpp"
#include "Game/Camera/CameraParamChunk.hpp"

void CamTranslatorTripodBoss::setParam(const CameraParamChunk* pChunk) {
    CameraTripodBoss* camera = mCamera;
    CameraGeneralParam* general = pChunk->mGeneralParam;

    f32 angleB = general->mUp.y;
    f32 angleA = general->mUp.x;

    camera->setParam(general->mWPoint, general->mAxis, general->mDist, TVec2f(angleA, angleB));
    camera->mAngleY = general->mUp.z;
}

Camera* CamTranslatorTripodBoss::getCamera() const {
    return mCamera;
}

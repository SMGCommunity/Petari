#include "Game/Camera/CamTranslatorTripodBoss.hpp"
#include "Game/Camera/CameraParamChunk.hpp"

void CamTranslatorTripodBoss::setParam(const CameraParamChunk* pChunk) {
    CameraTripodBoss* camera;
    CameraGeneralParam* general = pChunk->mGeneralParam;

    f32 dist;
    TVec2f up;

    up.y = general->mUp.y;
    up.x = general->mUp.x;
    dist = general->mDist;

    camera = mCamera;

    camera->mWPoint.set< f32 >(general->mWPoint);
    camera->mAxis.set< f32 >(general->mAxis);
    camera->mDist = dist;
    camera->mAngleB = up.x;
    camera->mAngleA = up.y;
    camera->upZ = general->mUp.z;
}

Camera* CamTranslatorTripodBoss::getCamera() const {
    return mCamera;
}

#include "Game/Camera/CamTranslatorTripodPlanet.hpp"
#include "Game/Camera/CameraParamChunk.hpp"
#include "Game/Util/MathUtil.hpp"

void CamTranslatorTripodPlanet_FORCE_MATCH_SDATA2() {
    (void)1.0f;
    (void)0.0f;
}

void CamTranslatorTripodPlanet::setParam(const CameraParamChunk* pChunk) {
    CameraGeneralParam* general = pChunk->mGeneralParam;

    TVec3f axis = general->mAxis;

    if (MR::isNearZero(axis)) {
        axis.set< f32 >(0.0f, 1.0f, 0.0f);
    } else {
        MR::normalize(&axis);
    }

    TVec2f angle = TVec2f(general->mAngleA, general->mAngleB);
    f32 dist = general->mDist;

    CameraTripodPlanet* camera = mCamera;

    camera->mWPoint.set< f32 >(general->mWPoint);
    camera->mAxis.set< f32 >(axis);
    camera->mAngleA = angle.x;
    camera->mAngleB = angle.y;
    camera->mDist = dist;
}

Camera* CamTranslatorTripodPlanet::getCamera() const {
    return mCamera;
}

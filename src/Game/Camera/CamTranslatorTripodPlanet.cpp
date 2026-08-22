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

    mCamera->setParam(general->mWPoint, axis, TVec2f(general->mAngleA, general->mAngleB), general->mDist);
}

Camera* CamTranslatorTripodPlanet::getCamera() const {
    return mCamera;
}

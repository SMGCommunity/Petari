#include "Game/Camera/CamTranslatorCharmedTripodBoss.hpp"
#include "Game/Camera/CameraParamChunk.hpp"
#include "Game/Util/MathUtil.hpp"

void CamTranslatorCharmedTripodBoss_FORCE_MATCH_SDATA2() {
    (void)1.0f;
    (void)0.0f;
}

void CamTranslatorCharmedTripodBoss::setParam(const CameraParamChunk* pChunk) {
    CameraGeneralParam* general = pChunk->mGeneralParam;

    TVec3f up = general->mUp;

    if (MR::isNearZero(up)) {
        up.set< f32 >(0.0f, 1.0f, 0.0f);
    } else {
        MR::normalize(&up);
    }

    TVec2f axis = TVec2f(general->mAxis.x, general->mAxis.y);

    mCamera->setParam(general->mNum1, up, general->mWPoint, axis);
}

Camera* CamTranslatorCharmedTripodBoss::getCamera() const {
    return mCamera;
}

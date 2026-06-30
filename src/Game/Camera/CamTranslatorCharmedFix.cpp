#include "Game/Camera/CamTranslatorCharmedFix.hpp"
#include "Game/Camera/CameraParamChunk.hpp"
#include "Game/Util/MathUtil.hpp"

void CamTranslatorCharmedFix_FORCE_MATCH_SDATA2() {
    (void)1.0f;
    (void)0.0f;
}

void CamTranslatorCharmedFix::setParam(const CameraParamChunk* pChunk) {
    CameraGeneralParam* general = pChunk->mGeneralParam;

    TVec3f up;
    up = general->mUp;

    if (MR::isNearZero(up)) {
        up.set< f32 >(0.0f, 1.0f, 0.0f);
    } else {
        MR::normalize(&up);
    }

    CameraCharmedFix* camera = mCamera;

    camera->mAxis.set< f32 >(general->mAxis);
    camera->mUp.set< f32 >(up);
    camera->mWPoint.set< f32 >(general->mWPoint);
}

Camera* CamTranslatorCharmedFix::getCamera() const {
    return mCamera;
}

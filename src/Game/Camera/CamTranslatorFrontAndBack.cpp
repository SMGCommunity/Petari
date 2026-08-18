#include "Game/Camera/CamTranslatorFrontAndBack.hpp"
#include "Game/Camera/CameraParamChunk.hpp"
#include "Game/Util/MathUtil.hpp"

void CamTranslatorFrontAndBack_FORCE_MATCH_SDATA2() {
    (void)1.0f;
    (void)0.0f;
    (void)MR::pi();
}

void CamTranslatorFrontAndBack::setParam(const CameraParamChunk* pChunk) {
    CameraGeneralParam* general = pChunk->mGeneralParam;

    TVec3f axis = general->mAxis;

    if (MR::isNearZero(axis)) {
        axis.set< f32 >(1.0f, 0.0f, 0.0f);
    } else {
        MR::normalize(&axis);
    }

    mCamera->setParam(general->mWPoint, axis, MR::pi() * general->mAngleA / 180.0f, MR::pi() * general->mAngleB / 180.0f, general->mDist);
}

Camera* CamTranslatorFrontAndBack::getCamera() const {
    return mCamera;
}

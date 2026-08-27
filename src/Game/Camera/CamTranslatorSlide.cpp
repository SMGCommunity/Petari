#include "Game/Camera/CamTranslatorSlide.hpp"
#include "Game/Camera/CameraParamChunk.hpp"
#include "Game/Util/MathUtil.hpp"
#include <cmath>

void CamTranslatorSlide_FORCE_MATCH_SDATA2() {
    (void)1.0f;
    (void)0.0f;
}

void CamTranslatorSlide::setParam(const CameraParamChunk* pChunk) {
    CameraGeneralParam* general = pChunk->mGeneralParam;

    TVec3f axis = general->mAxis;
    if (MR::isNearZero(axis)) {
        axis.set< f32 >(1.0f, 0.0f, 0.0f);
    }
    MR::normalize(&axis);

    TVec3f up = general->mUp;
    if (MR::isNearZero(axis) || MR::isSameDirection(axis, up)) {
        up.set< f32 >(0.0f, 1.0f, 0.0f);
        TQuat4f rot;
        rot.setRotate(TVec3f(1.0f, 0.0f, 0.0f), axis);
        rot.transform(up);
    }
    MR::normalize(&up);
    mCamera->setParam(axis, up, general->mWPoint, general->mAngleA, general->mDist);
}

Camera* CamTranslatorSlide::getCamera() const {
    return mCamera;
}

#include "Game/Camera/CamTranslatorSlide.hpp"
#include "Game/Camera/CameraParamChunk.hpp"
#include "Game/Util/MathUtil.hpp"
#include <cmath>

void CamTranslatorSlide_FORCE_MATCH_SDATA2() {
    (void)1.0f;
    (void)0.0f;
}

void CamTranslatorSlide::setParam(const CameraParamChunk* pChunk) {
    // FIXME: regswap/out-of-order-load and float swap
    CameraGeneralParam* general = pChunk->mGeneralParam;

    TVec3f axis = general->mAxis;

    if (MR::isNearZero(axis)) {
        axis.set< f32 >(1.0f, 0.0f, 0.0f);
    }

    MR::normalize(&axis);

    TVec3f up = general->mUp;

    if (MR::isNearZero(axis) || MR::isSameDirection(axis, up, 0.01f)) {
        up.set< f32 >(0.0f, 1.0f, 0.0f);
        TQuat4f rot;
        rot.setRotate(TVec3f(1.0f, 0.0f, 0.0f), axis);
        rot.transform(up);
    }

    MR::normalize(&up);

    CameraSlide* camera = mCamera;

    f32 dist = general->mDist;
    f32 angleA = general->mAngleA;

    camera->mAxis.set(axis);
    camera->mUp.set(up);
    camera->mWPoint.set(general->mWPoint);
    camera->mAngleA = angleA;
    camera->mDist = dist;
}

Camera* CamTranslatorSlide::getCamera() const {
    return mCamera;
}

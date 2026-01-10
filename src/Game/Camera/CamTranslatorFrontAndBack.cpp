#include "Game/Camera/CamTranslatorFrontAndBack.hpp"
#include "Game/Camera/CameraParamChunk.hpp"
#include "Game/Util/MathUtil.hpp"

// Register mismatch
void CamTranslatorFrontAndBack::setParam(const CameraParamChunk* pChunk) {
    CameraGeneralParam* general = pChunk->mGeneralParam;

    TVec3f axis = general->mAxis;

    if (MR::isNearZero(axis)) {
        axis.set(1.0f, 0.0f, 0.0f);
    } else {
        MR::normalize(&axis);
    }

    f32 angleA2;
    f32 angleB2;
    f32 dist;

    f32 angleB = general->mAngleB;
    f32 angleA = general->mAngleA;

    angleB2 = angleB * 3.1415927f / 180.0f;
    angleA2 = angleA * 3.1415927f / 180.0f;
    dist = general->mDist;

    CameraFrontAndBack* camera = mCamera;

    camera->mWPoint = general->mWPoint;
    camera->mAxis = axis;
    camera->mAngleA = angleA2;
    camera->mAngleB = angleB2;
    camera->mDist = dist;
}

Camera* CamTranslatorFrontAndBack::getCamera() const {
    return mCamera;
}

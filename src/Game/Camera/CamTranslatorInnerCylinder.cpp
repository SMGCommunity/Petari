#include "Game/Camera/CamTranslatorInnerCylinder.hpp"
#include "Game/Camera/CameraParamChunk.hpp"
#include "Game/Util/MathUtil.hpp"

void CamTranslatorInnerCylinder::setParam(const CameraParamChunk* pChunk) {
    CameraGeneralParam* general = pChunk->mGeneralParam;

    TVec3f axis = general->mAxis;

    if (MR::isNearZero(axis)) {
        axis.set< f32 >(0.0f, 0.0f, 1.0f);
    } else {
        MR::normalize(&axis);
    }

    mCamera->setParam(general->mWPoint, axis, TVec2f(general->mAngleA, general->mAngleB), general->mDist, general->mUp.x);
}

Camera* CamTranslatorInnerCylinder::getCamera() const {
    return mCamera;
}

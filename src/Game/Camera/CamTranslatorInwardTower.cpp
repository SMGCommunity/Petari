#include "Game/Camera/CamTranslatorInwardTower.hpp"
#include "Game/Camera/CameraParamChunk.hpp"
#include "Game/Util/MathUtil.hpp"

void CamTranslatorInwardTower_FORCE_MATCH_SDATA2() {
    (void)1.0f;
    (void)0.0f;
}

void CamTranslatorInwardTower::setParam(const CameraParamChunk* pChunk) {
    CameraGeneralParam* general = pChunk->mGeneralParam;

    TVec3f axis = general->mAxis;

    if (MR::isNearZero(axis)) {
        axis.set< f32 >(0.0f, 1.0f, 0.0f);
    } else {
        MR::normalize(&axis);
    }

    mCamera->setParam(general->mWPoint, axis, TVec2f(general->mAngleB, general->mAngleA), general->mDist);
}

Camera* CamTranslatorInwardTower::getCamera() const {
    return mCamera;
}

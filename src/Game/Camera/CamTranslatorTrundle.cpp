#include "Game/Camera/CamTranslatorTrundle.hpp"
#include "Game/Camera/CameraParamChunk.hpp"
#include "Game/Util/MathUtil.hpp"

void CamTranslatorTrundle::setParam(const CameraParamChunk* pChunk) {
    CameraGeneralParam* general = pChunk->mGeneralParam;

    TVec3f axis = general->mAxis;
    if (MR::isNearZero(axis)) {
        return;
    }
    MR::normalize(&axis);

    mCamera->setParam(general->mWPoint, axis, general->mDist, general->mAngleA, general->mAngleB, general->mUp.x);
}

Camera* CamTranslatorTrundle::getCamera() const {
    return mCamera;
}

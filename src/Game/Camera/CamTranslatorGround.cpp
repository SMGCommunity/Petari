#include "Game/Camera/CamTranslatorGround.hpp"
#include "Game/Camera/CameraParamChunk.hpp"

void CamTranslatorGround::setParam(const CameraParamChunk* pChunk) {
    mCamera->setParam(TVec2f(pChunk->mGeneralParam->mAngleA, pChunk->mGeneralParam->mAngleB), pChunk->mGeneralParam->mDist,
                      pChunk->mGeneralParam->mUp);
}

Camera* CamTranslatorGround::getCamera() const {
    return mCamera;
}

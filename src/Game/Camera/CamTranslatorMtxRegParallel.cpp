#include "Game/Camera/CamTranslatorMtxRegParallel.hpp"
#include "Game/Camera/CameraParamChunk.hpp"

void CamTranslatorMtxRegParallel::setParam(const CameraParamChunk* pChunk) {
    mCamera->setParam(pChunk->mGeneralParam->mString.getCharPtr(), pChunk->mGeneralParam->mDist,
                      TVec2f(pChunk->mGeneralParam->mAngleB, pChunk->mGeneralParam->mAngleA), pChunk->mGeneralParam->mWPoint);
}

Camera* CamTranslatorMtxRegParallel::getCamera() const {
    return mCamera;
}

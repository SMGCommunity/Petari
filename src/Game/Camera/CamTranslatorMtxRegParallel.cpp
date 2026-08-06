#include "Game/Camera/CamTranslatorMtxRegParallel.hpp"
#include "Game/Camera/CameraParamChunk.hpp"

void CamTranslatorMtxRegParallel::setParam(const CameraParamChunk* pChunk) {
    CameraGeneralParam* general = pChunk->mGeneralParam;
    mCamera->setParam(general->mString.getCharPtr(), general->mDist, TVec2f(general->mAngleB, general->mAngleA), general->mWPoint);
}

Camera* CamTranslatorMtxRegParallel::getCamera() const {
    return mCamera;
}

#include "Game/Camera/CamTranslatorObjParallel.hpp"
#include "Game/Camera/CameraParamChunk.hpp"

void CamTranslatorObjParallel::setParam(const CameraParamChunk* pChunk) {
    CameraGeneralParam* general = pChunk->mGeneralParam;
    mCamera->setParam(general->mDist, general->mAngleA, general->mAngleB);
}

Camera* CamTranslatorObjParallel::getCamera() const {
    return mCamera;
}

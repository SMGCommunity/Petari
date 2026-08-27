#include "Game/Camera/CamTranslatorCharmedVecReg.hpp"
#include "Game/Camera/CameraParamChunk.hpp"

void CamTranslatorCharmedVecReg::setParam(const CameraParamChunk* pChunk) {
    CameraGeneralParam* general = pChunk->mGeneralParam;

    mCamera->setParam(general->mString.getCharPtr(), general->mDist, general->mAxis.x, general->mAxis.y, general->mAxis.z, general->mAngleA,
                      general->mAngleB);
}

Camera* CamTranslatorCharmedVecReg::getCamera() const {
    return mCamera;
}

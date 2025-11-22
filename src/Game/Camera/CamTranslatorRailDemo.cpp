#include "Game/Camera/CamTranslatorRailDemo.hpp"
#include "Game/Camera/CameraParamChunk.hpp"

void CamTranslatorRailDemo::setParam(const CameraParamChunk* pChunk) {
    mCamera->setParam(pChunk->getZoneID(), pChunk->mGeneralParam->mNum1 >> 16, pChunk->mGeneralParam->mNum2, pChunk->mGeneralParam->mNum1 & 0xFFFF,
                      pChunk->mGeneralParam->mDist);
}

Camera* CamTranslatorRailDemo::getCamera() const {
    return mCamera;
}
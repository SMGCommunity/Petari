#include "Game/Camera/CamTranslatorRailFollow.hpp"
#include "Game/Camera/CameraParamChunk.hpp"

void CamTranslatorRailFollow::setParam(const CameraParamChunk* pChunk) {
    mCamera->setParam(pChunk->getZoneID(), pChunk->mGeneralParam->mNum1, pChunk->mGeneralParam->mDist, pChunk->mGeneralParam->mWPoint.x,
                      pChunk->mGeneralParam->mWPoint.y, pChunk->mGeneralParam->mNum2);
}

Camera* CamTranslatorRailFollow::getCamera() const {
    return mCamera;
}
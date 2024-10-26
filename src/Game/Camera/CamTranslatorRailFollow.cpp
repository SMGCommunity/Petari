#include "Game/Camera/CameraParamChunk.hpp"
#include "Game/Camera/CamTranslatorRailFollow.hpp"

void CamTranslatorRailFollow::setParam(const CameraParamChunk *pChunk) {
	mCamera->setParam(
		pChunk->getZoneID(),
		pChunk->mGeneralParam->mNum1,
		pChunk->mGeneralParam->mDist,
		pChunk->mGeneralParam->mWPoint.x,
		pChunk->mGeneralParam->mWPoint.y,
		pChunk->mGeneralParam->mNum2
		);
}

Camera *CamTranslatorRailFollow::getCamera() const {
	return mCamera;
}
#include "Game/Camera/CameraParamChunk.h"
#include "Game/Camera/CamTranslatorRailFollow.h"

void CamTranslatorRailFollow::setParam(const CameraParamChunk *pChunk) {
	reinterpret_cast<CameraRailFollow *>(mCamera)->setParam(
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
#include "Game/Camera/CameraParamChunk.h"
#include "Game/Camera/CamTranslatorRailWatch.h"

void CamTranslatorRailWatch::setParam(const CameraParamChunk *pChunk) {
	reinterpret_cast<CameraRailWatch *>(mCamera)->setParam(
		pChunk->getZoneID(),
		pChunk->mGeneralParam->mNum1,
		pChunk->mGeneralParam->mNum2,
		pChunk->mGeneralParam->mAxis.x,
		pChunk->mGeneralParam->mDist,
		pChunk->mGeneralParam->mAngleA
		);
}

Camera *CamTranslatorRailWatch::getCamera() const {
	return mCamera;
}
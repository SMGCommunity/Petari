#include "Game/Camera/CameraParamChunk.hpp"
#include "Game/Camera/CamTranslatorRailWatch.hpp"

void CamTranslatorRailWatch::setParam(const CameraParamChunk *pChunk) {
	mCamera->setParam(
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
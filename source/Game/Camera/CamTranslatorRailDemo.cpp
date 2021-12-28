#include "Game/Camera/CameraParamChunk.h"
#include "Game/Camera/CamTranslatorRailDemo.h"

void CamTranslatorRailDemo::setParam(const CameraParamChunk *pChunk) {
	reinterpret_cast<CameraRailDemo *>(mCamera)->setParam(
		pChunk->getZoneID(),
		pChunk->mGeneralParam->mNum1 >> 16,
		pChunk->mGeneralParam->mNum2,
		pChunk->mGeneralParam->mNum1 & 0xFFFF,
		pChunk->mGeneralParam->mDist
		);
}

Camera *CamTranslatorRailDemo::getCamera() const {
	return mCamera;
}
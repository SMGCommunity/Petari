#include "Game/Camera/CameraParamChunk.hpp"
#include "Game/Camera/CamTranslatorTwistedPassage.hpp"

void CamTranslatorTwistedPassage::setParam(const CameraParamChunk *pChunk) {
	mCamera->setParam(
		pChunk->getZoneID(),
		pChunk->mGeneralParam->mNum1,
		pChunk->mGeneralParam->mAxis.x,
		pChunk->mGeneralParam->mAxis.y
		);
}

Camera *CamTranslatorTwistedPassage::getCamera() const {
	return mCamera;
}
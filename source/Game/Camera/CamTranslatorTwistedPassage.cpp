#include "Game/Camera/CameraParamChunk.h"
#include "Game/Camera/CamTranslatorTwistedPassage.h"

void CamTranslatorTwistedPassage::setParam(const CameraParamChunk *pChunk) {
	reinterpret_cast<CameraTwistedPassage *>(mCamera)->setParam(
		pChunk->getZoneID(),
		pChunk->mGeneralParam->mNum1,
		pChunk->mGeneralParam->mAxis.x,
		pChunk->mGeneralParam->mAxis.y
		);
}

Camera *CamTranslatorTwistedPassage::getCamera() const {
	return mCamera;
}
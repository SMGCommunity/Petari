#include "Game/Camera/CameraParamChunk.h"
#include "Game/Camera/CamTranslatorFixedPoint.h"

void CamTranslatorFixedPoint::setParam(const CameraParamChunk *pChunk) {
	CameraFixedPoint *camera = reinterpret_cast<CameraFixedPoint *>(mCamera);

	camera->setParam(pChunk->mGeneralParam->mWPoint, pChunk->mGeneralParam->mNum1);
}

Camera *CamTranslatorFixedPoint::getCamera() const {
	return mCamera;
}

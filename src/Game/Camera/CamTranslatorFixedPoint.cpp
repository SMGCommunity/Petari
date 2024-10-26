#include "Game/Camera/CameraParamChunk.hpp"
#include "Game/Camera/CamTranslatorFixedPoint.hpp"

void CamTranslatorFixedPoint::setParam(const CameraParamChunk *pChunk) {
	CameraFixedPoint *camera = mCamera;

	camera->setParam(pChunk->mGeneralParam->mWPoint, pChunk->mGeneralParam->mNum1);
}

Camera *CamTranslatorFixedPoint::getCamera() const {
	return mCamera;
}

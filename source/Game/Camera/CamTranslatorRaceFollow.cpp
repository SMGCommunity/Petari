#include "Game/Camera/CameraParamChunk.h"
#include "Game/Camera/CamTranslatorRaceFollow.h"

void CamTranslatorRaceFollow::setParam(const CameraParamChunk *pChunk) {
	f32 wPointX;
	f32 wPointY;
	f32 wPointZ;
	f32 angleA;

	f32 pi = 3.1415927f;
	f32 angleA2 = pChunk->mGeneralParam->mAngleA;

	angleA = angleA2 * pi / 180.0f;
	bool uVar5 = pChunk->mGeneralParam->mNum1 != 0;
	wPointZ = pChunk->mGeneralParam->mWPoint.z;
	wPointY = pChunk->mGeneralParam->mWPoint.y;
	wPointX = pChunk->mGeneralParam->mWPoint.x;

	CameraRaceFollow *camera = reinterpret_cast<CameraRaceFollow *>(mCamera);

	camera->mWPointX = wPointX;
	camera->mWPointY = wPointY;
	camera->mAngleA = angleA;
	camera->_58 = uVar5;
	camera->mWPointZ = wPointZ;
}

Camera *CamTranslatorRaceFollow::getCamera() const {
	return mCamera;
}
#include "Game/Camera/CameraParamChunk.h"
#include "Game/Camera/CamTranslatorBehind.h"

void CamTranslatorBehind::setParam(const CameraParamChunk *pChunk) {
	TVec2f angle;
	f32 dist;
	f32 axisX;
	angle.y = 180.0f * pChunk->mGeneralParam->mAngleA / 3.1415927f;
	angle.x = 180.0f * pChunk->mGeneralParam->mAngleB / 3.1415927f;
	axisX = pChunk->mGeneralParam->mAxis.x;
	dist = pChunk->mGeneralParam->mDist;

	CameraBehind *camera = reinterpret_cast<CameraBehind *>(mCamera);

	camera->mAngleB = angle.x;
	camera->mAngleA = angle.y;
	camera->mDist = dist;
	camera->mAxisX = axisX;
}

Camera *CamTranslatorBehind::getCamera() const {
	return mCamera;
}
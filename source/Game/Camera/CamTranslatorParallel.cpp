#include "Game/Camera/CameraParamChunk.h"
#include "Game/Camera/CamTranslatorParallel.h"

void CamTranslatorParallel::setParam(const CameraParamChunk *pChunk) {
	TVec2f angle;
	f32 dist;
	f32 axisX;

	dist = pChunk->mGeneralParam->mDist;
	angle.y = 180.0f * pChunk->mGeneralParam->mAngleA / 3.1415927f;
	angle.x = 180.0f * pChunk->mGeneralParam->mAngleB / 3.1415927f;
	bool var = !(pChunk->mGeneralParam->mNum1 != 1);

	CameraParallel *camera = mCamera;

	camera->mAngleB = angle.x;
	camera->mAngleA = angle.y;
	camera->mDist = dist;
	camera->_58 = var;
}

Camera *CamTranslatorParallel::getCamera() const {
	return mCamera;
}
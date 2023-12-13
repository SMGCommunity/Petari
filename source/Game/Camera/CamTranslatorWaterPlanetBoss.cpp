#include "Game/Camera/CameraParamChunk.hpp"
#include "Game/Camera/CamTranslatorWaterPlanetBoss.hpp"

CamTranslatorWaterPlanetBoss::CamTranslatorWaterPlanetBoss(CameraWaterPlanetBoss *pCamera) {
	mCamera = pCamera;
}

#ifdef NON_MATCHING
// wPoint.z is put into f2 instead of f8
void CamTranslatorWaterPlanetBoss::setParam(const CameraParamChunk *pChunk) {
	CameraWaterPlanetBoss *camera;
	CameraGeneralParam *general = pChunk->mGeneralParam;

	f32 axisY;
	f32 axisX;
	f32 dist;
	f32 axisZ;
	f32 upX;
	f32 upY;
	f32 upZ;
	bool uVar8;

	TVec3f wPoint = general->mWPoint;
	uVar8 = general->mNum1 != 0;
	upZ = general->mUp.z;
	upY = general->mUp.y;
	upX = general->mUp.x;
	axisZ = general->mAxis.z;
	dist = general->mDist;
	axisX = general->mAxis.x;
	axisY = general->mAxis.y;

	camera = mCamera;

	camera->mAxisY = axisY;
	camera->mAxisX = axisX;
	camera->mDist = dist;
	camera->_6C = uVar8;
	camera->mWPointX = wPoint.x;
	camera->mWPointY = wPoint.y;
	camera->mWPointZ = wPoint.z;
	camera->mAxisZ = axisZ;
	camera->mUpX = upX;
	camera->mUpY = upY;
	camera->mUpZ = upZ;
}
#endif

Camera *CamTranslatorWaterPlanetBoss::getCamera() const {
	return mCamera;
}
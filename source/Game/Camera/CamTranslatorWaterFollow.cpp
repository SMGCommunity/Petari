#include "Game/Camera/CameraParamChunk.h"
#include "Game/Camera/CamTranslatorWaterFollow.h"

CamTranslatorWaterFollow::CamTranslatorWaterFollow(CameraWaterFollow *pCamera) : CamTranslatorDummy(pCamera) {

}

void CamTranslatorWaterFollow::setParam(const CameraParamChunk *pChunk) {
	CameraGeneralParam *general = pChunk->mGeneralParam;

	f32 axisY;
	f32 axisX;
	f32 dist;

	dist = general->mDist;
	axisX = general->mAxis.x;
	axisY = general->mAxis.y;

	CameraWaterFollow *camera = reinterpret_cast<CameraWaterFollow *>(mCamera);

	camera->mAxisY = axisY;
	camera->mAxisX = axisX;
	camera->mDist = dist;
}

Camera *CamTranslatorWaterFollow::getCamera() const {
	return mCamera;
}
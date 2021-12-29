#include "Game/Camera/CameraParamChunk.h"
#include "Game/Camera/CamTranslatorWonderPlanet.h"

CamTranslatorWonderPlanet::CamTranslatorWonderPlanet(CameraWonderPlanet *pCamera) {
	mCamera = pCamera;
}

void CamTranslatorWonderPlanet::setParam(const CameraParamChunk *pChunk) {
	CameraGeneralParam *general = pChunk->mGeneralParam;

	f32 axisX;
	f32 axisY;
	f32 angleA;

	angleA = general->mAngleA;
	axisY = general->mAxis.y;
	axisX = general->mAxis.x;

	CameraWonderPlanet *camera = reinterpret_cast<CameraWonderPlanet *>(mCamera);

	camera->mAxisX = axisX;
	camera->mAxisY = axisY;
	camera->mAngleA = angleA;
}

Camera *CamTranslatorWonderPlanet::getCamera() const {
	return mCamera;
}
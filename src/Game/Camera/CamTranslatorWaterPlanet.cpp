#include "Game/Camera/CameraParamChunk.hpp"
#include "Game/Camera/CamTranslatorWaterPlanet.hpp"

CamTranslatorWaterPlanet::CamTranslatorWaterPlanet(CameraWaterPlanet *pCamera) {
	mCamera = pCamera;
}

void CamTranslatorWaterPlanet::setParam(const CameraParamChunk *pChunk) {
	CameraGeneralParam *general = pChunk->mGeneralParam;

	f32 axisX;
	f32 axisY;
	f32 angleA;

	angleA = general->mAngleA;
	axisY = general->mAxis.y;
	axisX = general->mAxis.x;

	CameraWaterPlanet *camera = mCamera;

	camera->mAxisX = axisX;
	camera->mAxisY = axisY;
	camera->mAngleA = angleA;
}

Camera *CamTranslatorWaterPlanet::getCamera() const {
	return mCamera;
}
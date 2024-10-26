#include "Game/Camera/CameraParamChunk.hpp"
#include "Game/Camera/CamTranslatorWonderPlanet.hpp"

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

	CameraWonderPlanet *camera = mCamera;

	camera->mAxisX = axisX;
	camera->mAxisY = axisY;
	camera->mAngleA = angleA;
}

Camera *CamTranslatorWonderPlanet::getCamera() const {
	return mCamera;
}
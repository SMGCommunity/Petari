#include "Game/Camera/CameraParamChunk.h"
#include "Game/Camera/CamTranslatorWaterPlanet.h"

CamTranslatorWaterPlanet::CamTranslatorWaterPlanet(CameraWaterPlanet *pCamera) : CamTranslatorDummy(pCamera) {

}

void CamTranslatorWaterPlanet::setParam(const CameraParamChunk *pChunk) {
	CameraGeneralParam *general = pChunk->mGeneralParam;

	f32 axisX;
	f32 axisY;
	f32 angleA;

	angleA = general->mAngleA;
	axisY = general->mAxis.y;
	axisX = general->mAxis.x;

	CameraWaterPlanet *camera = reinterpret_cast<CameraWaterPlanet *>(mCamera);

	camera->mAxisX = axisX;
	camera->mAxisY = axisY;
	camera->mAngleA = angleA;
}

Camera *CamTranslatorWaterPlanet::getCamera() const {
	return mCamera;
}
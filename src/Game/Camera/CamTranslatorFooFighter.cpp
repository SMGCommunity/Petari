#include "Game/Camera/CameraParamChunk.hpp"
#include "Game/Camera/CamTranslatorFooFighter.hpp"

CamTranslatorFooFighter::CamTranslatorFooFighter(CameraFooFighter *pCamera) {
	mCamera = pCamera;
}

void CamTranslatorFooFighter::setParam(const CameraParamChunk *pChunk) {
	CameraGeneralParam *general = pChunk->mGeneralParam;
	CameraFooFighter *camera = mCamera;

	f32 axisY;
	f32 axisX;
	f32 dist;

	dist = general->mDist;
	axisX = general->mAxis.x;
	axisY = general->mAxis.y;

	camera->mAxisY = axisY;
	camera->mAxisX = axisX;
	camera->mDist = dist;
}

Camera *CamTranslatorFooFighter::getCamera() const {
	return mCamera;
}
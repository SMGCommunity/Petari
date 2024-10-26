#include "Game/Camera/CameraParamChunk.hpp"
#include "Game/Camera/CamTranslatorMedianPlanet.hpp"

CamTranslatorMedianPlanet::CamTranslatorMedianPlanet(CameraMedianPlanet *pCamera) {
	mCamera = pCamera;
}

void CamTranslatorMedianPlanet::setParam(const CameraParamChunk *pChunk) {
	CameraGeneralParam *general = pChunk->mGeneralParam;

	f32 axisX;
	f32 axisY;
	f32 angleA;
	f32 axisZ;
	f32 angleB;
	f32 dist;
	f32 wPointX;

	wPointX = general->mWPoint.x;
	dist = general->mDist;
	angleB = general->mAngleB;
	axisZ = general->mAxis.z;
	angleA = general->mAngleA;
	axisY = general->mAxis.y;
	axisX = general->mAxis.x;
	const char *string = general->mString.getCharPtr();

	CameraMedianPlanet *camera = mCamera;

	camera->mString = string;
	camera->mAxisX = axisX;
	camera->mAxisY = axisY;
	camera->mAngleA = angleA;
	camera->mAngleB = angleB;
	camera->mDist = dist;
	camera->mAxisZ = axisZ;
	camera->mWPointX = wPointX;
}

Camera *CamTranslatorMedianPlanet::getCamera() const {
	return mCamera;
}
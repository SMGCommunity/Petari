#include "Game/Camera/CameraParamChunk.hpp"
#include "Game/Camera/CamTranslatorCharmedVecReg.hpp"

void CamTranslatorCharmedVecReg::setParam(const CameraParamChunk *pChunk) {
	CameraGeneralParam *general = pChunk->mGeneralParam;

	const char *string;
	f32 dist;
	f32 axisX;
	f32 axisY;
	f32 axisZ;
	f32 angleA;
	f32 angleB;

	angleB = general->mAngleB;
	angleA = general->mAngleA;
	axisZ = general->mAxis.z;
	axisY = general->mAxis.y;
	axisX = general->mAxis.x;
	dist = general->mDist;
	string = general->mString.getCharPtr();

	CameraCharmedVecReg *camera = mCamera;

	camera->mString = string;
	camera->mDist = dist;

	if (dist < 300.0f) {
		camera->mDist = 300.0f;
	}

	camera->mAxisX = axisX;
	camera->mAxisZ = axisZ;
	camera->mAxisY = axisY;
	camera->mAngleA = angleA;
	camera->mAngleB = angleB;
}

Camera *CamTranslatorCharmedVecReg::getCamera() const {
	return mCamera;
}
#include "Game/Camera/CameraParamChunk.h"
#include "Game/Camera/CamTranslatorInwardSphere.h"

void CamTranslatorInwardSphere::setParam(const CameraParamChunk *pChunk) {
	CameraGeneralParam *general = pChunk->mGeneralParam;
	CameraInwardSphere *camera = mCamera;

	f32 angleA;
	f32 dist;
	f32 angleB;

	angleB = general->mAngleB;
	dist = general->mDist;
	angleA = general->mAngleA;

	camera->mAngleA = angleA;
	camera->mDist = dist;
	camera->mAngleB = angleB;
}

Camera *CamTranslatorInwardSphere::getCamera() const {
	return mCamera;
}
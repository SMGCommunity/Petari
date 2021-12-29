#include "Game/Camera/CameraParamChunk.h"
#include "Game/Camera/CamTranslatorObjParallel.h"

void CamTranslatorObjParallel::setParam(const CameraParamChunk *pChunk) {
	CameraGeneralParam *general = pChunk->mGeneralParam;

	f32 dist;
	f32 angleA;
	f32 angleB;

	angleB = general->mAngleB;
	angleA = general->mAngleA;
	dist = general->mDist;

	CameraObjParallel *camera = mCamera;

	camera->mDist = dist;
	camera->mAngleA = angleA;
	camera->mAngleB = angleB;
}

Camera *CamTranslatorObjParallel::getCamera() const {
	return mCamera;
}
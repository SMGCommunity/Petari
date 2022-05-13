#include "Game/Camera/CameraParamChunk.h"
#include "Game/Camera/CamTranslatorCubePlanet.h"

void CamTranslatorCubePlanet::setParam(const CameraParamChunk *pChunk) {
	CameraGeneralParam *general = pChunk->mGeneralParam;
	CameraCubePlanet *camera = mCamera;

	f32 dist;
	f32 angleA;
	f32 angleB;

	angleB = general->mAngleB;
	angleA = general->mAngleA;
	dist = general->mDist;

	camera->mDist = dist;
	camera->mAngleA = angleA;
	camera->mAngleB = angleB;
}

Camera *CamTranslatorCubePlanet::getCamera() const {
	return mCamera;
}
#include "Game/Camera/CameraParamChunk.h"
#include "Game/Camera/CamTranslatorBlackHole.h"

void CamTranslatorBlackHole::setParam(const CameraParamChunk *pChunk) {
	CameraGeneralParam *general = pChunk->mGeneralParam;

	TVec3f axis = general->mAxis;
	TVec3f wPoint = general->mWPoint;

	CameraBlackHole *camera = mCamera;

	camera->mWPoint.set(wPoint);
	camera->mAxis.set(axis);
}

Camera *CamTranslatorBlackHole::getCamera() const {
	return mCamera;
}
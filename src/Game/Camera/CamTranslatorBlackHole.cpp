#include "Game/Camera/CameraParamChunk.hpp"
#include "Game/Camera/CamTranslatorBlackHole.hpp"

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
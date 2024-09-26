#include "Game/Camera/CameraParamChunk.hpp"
#include "Game/Camera/CamTranslatorFollow.hpp"

CamTranslatorFollow::CamTranslatorFollow(CameraFollow *pCamera) {
	mCamera = pCamera;
}

void CamTranslatorFollow::setParam(const CameraParamChunk *pChunk) {
	CameraGeneralParam *general = pChunk->mGeneralParam;
	CameraFollow *camera = mCamera;

	camera->setParam(
		general->mAxis.x,
		general->mAxis.y,
		general->mAngleA,
		general->mAngleB,
		general->mDist,
		general->mNum1 > 0
		);
}

Camera *CamTranslatorFollow::getCamera() const {
	return mCamera;
}
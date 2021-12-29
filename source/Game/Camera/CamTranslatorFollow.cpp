#include "Game/Camera/CameraParamChunk.h"
#include "Game/Camera/CamTranslatorFollow.h"

CamTranslatorFollow::CamTranslatorFollow(CameraFollow *pCamera) {
	mCamera = pCamera;
}

void CamTranslatorFollow::setParam(const CameraParamChunk *pChunk) {
	CameraGeneralParam *general = pChunk->mGeneralParam;
	CameraFollow *camera = reinterpret_cast<CameraFollow *>(mCamera);

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
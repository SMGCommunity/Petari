#include "Game/Camera/CameraParamChunk.hpp"
#include "Game/Camera/CamTranslatorGround.hpp"

void CamTranslatorGround::setParam(const CameraParamChunk *pChunk) {
	CameraGeneralParam *general = pChunk->mGeneralParam;
	CameraGround *camera = mCamera;

	TVec2f angle = TVec2f(general->mAngleA, general->mAngleB);
	f32 dist = general->mDist;

	camera->mAngleA = angle.x;
	camera->mAngleB = angle.y;
	camera->mDist = dist;
	camera->mUpX = general->mUp.x;
	camera->mUpY = general->mUp.y;
	camera->mUpZ = general->mUp.z;
}

Camera *CamTranslatorGround::getCamera() const {
	return mCamera;
}

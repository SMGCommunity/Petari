#include "Game/Camera/CameraParamChunk.h"
#include "Game/Camera/CamTranslatorGround.h"

void CamTranslatorGround::setParam(const CameraParamChunk *pChunk) {
	CameraGeneralParam *general = pChunk->mGeneralParam;
	CameraGround *camera = reinterpret_cast<CameraGround *>(mCamera);

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

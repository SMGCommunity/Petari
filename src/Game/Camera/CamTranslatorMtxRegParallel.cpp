#include "Game/Camera/CameraParamChunk.hpp"
#include "Game/Camera/CamTranslatorMtxRegParallel.hpp"

void CamTranslatorMtxRegParallel::setParam(const CameraParamChunk *pChunk) {
	CameraGeneralParam *general = pChunk->mGeneralParam;

	TVec2f angle = TVec2f(general->mAngleB, general->mAngleA);
	f32 dist = general->mDist;
	const char *string = general->mString.getCharPtr();

	CameraMtxRegParallel *camera = mCamera;

	camera->mString = string;
	camera->mAngleB = angle.x;
	camera->mAngleA = angle.y;
	camera->mDist = dist;

	camera->mWPoint.setPS(general->mWPoint);
}

Camera *CamTranslatorMtxRegParallel::getCamera() const {
	return mCamera;
}

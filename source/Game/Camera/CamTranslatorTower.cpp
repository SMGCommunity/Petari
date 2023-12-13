#include "Game/Camera/CameraParamChunk.hpp"
#include "Game/Camera/CamTranslatorTower.hpp"

#ifdef NON_MATCHING
// fmuls operand order
void CamTranslatorTower::setParam(const CameraParamChunk *pChunk) {
	CameraTower *camera;
	CameraGeneralParam *general = pChunk->mGeneralParam;

	f32 dist;
	TVec2f angle;

	angle.x = general->mAngleB;
	angle.y = 3.1415927f * (1.0f - general->mAngleA) * 0.5f;
	dist = general->mDist;

	camera = mCamera;

	camera->mWPoint.set(general->mWPoint);
	camera->mAxis.set(general->mAxis);
	camera->mDist = dist;
	camera->mAngleB = angle.x;
	camera->mAngleA = angle.y;
}
#endif

Camera *CamTranslatorTower::getCamera() const {
	return mCamera;
}
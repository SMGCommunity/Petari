#include "Game/Camera/CameraParamChunk.hpp"
#include "Game/Camera/CamTranslatorTowerPos.hpp"

#ifdef NON_MATCHING
// fmuls operand order, same as CamTranslatorTower::setParam()
void CamTranslatorTowerPos::setParam(const CameraParamChunk *pChunk) {
	CameraTowerPos *camera;
	CameraGeneralParam *general = pChunk->mGeneralParam;

	f32 upY;
	f32 upX;
	TVec2f angle;

	angle.x = general->mAngleB;
	angle.y = (1.0f - general->mAngleA) * 3.1415927f * 0.5f;
	upX = general->mUp.x;
	upY = general->mUp.y;

	camera = mCamera;

	camera->mWPoint.set(general->mWPoint);
	camera->mAxis.set(general->mAxis);
	camera->mUpX = upX;
	camera->mAngleA = angle.y;
	camera->mAngleB = angle.x;
	camera->mUpY = upY;
}
#endif

Camera *CamTranslatorTowerPos::getCamera() const {
	return mCamera;
}
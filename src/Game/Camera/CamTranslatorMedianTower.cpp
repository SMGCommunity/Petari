#include "Game/Camera/CameraParamChunk.hpp"
#include "Game/Camera/CamTranslatorMedianTower.hpp"

#ifdef NON_MATCHING
// Instruction order, register mismatch
void CamTranslatorMedianTower::setParam(const CameraParamChunk *pChunk) {
	CameraGeneralParam *general = pChunk->mGeneralParam;

	const char *string;
	f32 angleA;
	f32 upX;
	f32 upY;
	f32 upZ;

	upZ = general->mUp.z;
	upY = general->mUp.y;
	upX = general->mUp.x;
	angleA = general->mAngleA;
	string = general->mString.getCharPtr();

	CameraMedianTower *camera = mCamera;

	camera->mString = string;
	camera->mWPoint.set(pChunk->mGeneralParam->mWPoint);
	camera->mAxis.set(pChunk->mGeneralParam->mAxis);
	camera->mAngleA = angleA;
	camera->mUpX = upX;
	camera->mUpY = upY;
	camera->mUpZ = upZ;
}
#endif

Camera *CamTranslatorMedianTower::getCamera() const {
	return mCamera;
}
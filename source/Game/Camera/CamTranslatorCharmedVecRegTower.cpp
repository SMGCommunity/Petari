#include "Game/Camera/CameraParamChunk.hpp"
#include "Game/Camera/CamTranslatorCharmedVecRegTower.hpp"

#ifdef NON_MATCHING
// Register issues
void CamTranslatorCharmedVecRegTower::setParam(const CameraParamChunk *pChunk) {
	CameraGeneralParam *general = pChunk->mGeneralParam;

	TVec3f axis;
	const char *string;
	f32 angleA;
	f32 angleB;
	f32 dist;
	bool uVar3;

	axis.setInline(general->mAxis);

	MR::normalizeOrZero(&axis);

	if (MR::isNearZero(axis, 0.001f)) {
		axis.set(0.0f, 1.0f, 0.0f);
	}

	uVar3 = general->mNum1 != 0;
	dist = general->mDist;
	angleB = general->mAngleB;
	angleA = general->mAngleA;
	string = general->mString.getCharPtr();

	CameraCharmedVecRegTower *camera = mCamera;

	camera->mString = string;
	camera->mWPoint.set(general->mWPoint);
	camera->mAxis.set(axis);
	camera->mAngleA = angleA;
	camera->mAngleB = angleB;
	camera->mDist = dist;
	camera->_70 = uVar3;
}
#endif

Camera *CamTranslatorCharmedVecRegTower::getCamera() const {
	return mCamera;
}
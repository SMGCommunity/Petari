#include "Game/Camera/CameraParamChunk.h"
#include "Game/Camera/CamTranslatorInwardTower.h"
#include "Game/Util/MathUtil.h"

void CamTranslatorInwardTower::setParam(const CameraParamChunk *pChunk) {
	CameraGeneralParam *general = pChunk->mGeneralParam;

	TVec3f axis = general->mAxis;

	if (MR::isNearZero(axis, 0.001f)) {
		axis.set(0.0f, 1.0f, 0.0f);
	}
	else {
		MR::normalize(&axis);
	}

	TVec2f angle = TVec2f(general->mAngleB, general->mAngleA);
	f32 dist = general->mDist;

	CameraInwardTower *camera = reinterpret_cast<CameraInwardTower *>(mCamera);

	camera->mWPoint.set(general->mWPoint);
	camera->mAxis.set(axis);
	camera->mAngleB = angle.x;
	camera->mAngleA = angle.y;
	camera->mDist = dist;
}

Camera *CamTranslatorInwardTower::getCamera() const {
	return mCamera;
}
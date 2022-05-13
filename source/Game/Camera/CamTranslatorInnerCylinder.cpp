#include "Game/Camera/CameraParamChunk.h"
#include "Game/Camera/CamTranslatorInnerCylinder.h"
#include "Game/Util/MathUtil.h"

void CamTranslatorInnerCylinder::setParam(const CameraParamChunk *pChunk) {
	CameraGeneralParam *general = pChunk->mGeneralParam;

	TVec3f axis = general->mAxis;

	if (MR::isNearZero(axis, 0.001f)) {
		axis.set(0.0f, 0.0f, 1.0f);
	}
	else {
		MR::normalize(&axis);
	}

	CameraInnerCylinder *camera = mCamera;

	TVec2f angle = TVec2f(general->mAngleA, general->mAngleB);
	f32 dist;
	f32 upX;

	upX = general->mUp.x;
	dist = general->mDist;

	camera->mWPoint.set(general->mWPoint);
	camera->mAxis.set(axis);
	camera->mAngleA = angle.x;
	camera->mAngleB = angle.y;
	camera->mDist = dist;
	camera->mUpX = upX;
}

Camera *CamTranslatorInnerCylinder::getCamera() const {
	return mCamera;
}
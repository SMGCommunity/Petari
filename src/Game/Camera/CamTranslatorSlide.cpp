#include "Game/Camera/CameraParamChunk.hpp"
#include "Game/Camera/CamTranslatorSlide.hpp"
#include "Game/Util/MathUtil.hpp"

/*void CamTranslatorSlide::setParam(const CameraParamChunk *pChunk) {
	CameraGeneralParam *general = pChunk->mGeneralParam;

	TVec3f axis = general->mAxis;

	if (MR::isNearZero(axis, 0.001f)) {
		axis.set(1.0f, 0.0f, 0.0f);
	}

	MR::normalize(&axis);

	TVec3f up = general->mUp;

	if (MR::isNearZero(up, 0.001f) || MR::isSameDirection(axis, up, 0.001f)) {
		up.set(0.0f, 1.0f, 0.0f);

		// Quaternion/matrix stuff here
	}

	MR::normalize(&up);

	CameraSlide *camera = mCamera;

	f32 dist = general->mDist;
	f32 angleA = general->mAngleA;

	camera->mAxis.set(axis);
	camera->mUp.set(up);
	camera->mWPoint.set(general->mWPoint);
	camera->mAngleA = angleA;
	camera->mDist = dist;
}*/

Camera *CamTranslatorSlide::getCamera() const {
	return mCamera;
}
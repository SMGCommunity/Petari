#include "Game/Camera/CameraParamChunk.hpp"
#include "Game/Camera/CamTranslatorTrundle.hpp"
#include "Game/Util/MathUtil.hpp"

void CamTranslatorTrundle::setParam(const CameraParamChunk *pChunk) {
	CameraTrundle *camera;
	CameraGeneralParam *general = pChunk->mGeneralParam;

	TVec3f axis = general->mAxis;

	if (!MR::isNearZero(axis, 0.001f)) {
		MR::normalize(&axis);

		f32 dist;
		f32 angleA;
		f32 angleB;
		f32 upX;

		upX = general->mUp.x;
		angleB = general->mAngleB;
		angleA = general->mAngleA;
		dist = general->mDist;

		camera = mCamera;

		camera->mWPoint.set(general->mWPoint);
		camera->mAxis.set(axis);
		camera->mDist = dist;
		camera->mAngleA = angleA;
		camera->mAngleB = angleB;
		camera->mUpX = upX;
	}
}

Camera *CamTranslatorTrundle::getCamera() const {
	return mCamera;
}
#include "Game/Camera/CameraParamChunk.hpp"
#include "Game/Camera/CamTranslatorTripodPlanet.hpp"
#include "Game/Util/MathUtil.hpp"

void CamTranslatorTripodPlanet::setParam(const CameraParamChunk *pChunk) {
	CameraGeneralParam *general = pChunk->mGeneralParam;

	TVec3f axis = general->mAxis;

	if (MR::isNearZero(axis, 0.001f)) {
		axis.set(0.0f, 1.0f, 0.0f);
	}
	else {
		MR::normalize(&axis);
	}

	TVec2f angle = TVec2f(general->mAngleA, general->mAngleB);
	f32 dist = general->mDist;

	CameraTripodPlanet *camera = mCamera;

	camera->mWPoint.set(general->mWPoint);
	camera->mAxis.set(axis);
	camera->mAngleA = angle.x;
	camera->mAngleB = angle.y;
	camera->mDist = dist;
}

Camera *CamTranslatorTripodPlanet::getCamera() const {
	return mCamera;
}
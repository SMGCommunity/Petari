#include "Game/Camera/CameraParamChunk.hpp"
#include "Game/Camera/CamTranslatorCharmedTripodBoss.hpp"
#include "Game/Util/MathUtil.hpp"

void CamTranslatorCharmedTripodBoss::setParam(const CameraParamChunk *pChunk) {
	CameraGeneralParam *general = pChunk->mGeneralParam;

	TVec3f up = general->mUp;

	if (MR::isNearZero(up, 0.001f)) {
		up.set(0.0f, 1.0f, 0.0f);
	}
	else {
		MR::normalize(&up);
	}

	TVec2f axis = TVec2f(general->mAxis.x, general->mAxis.y);

	mCamera->setParam(general->mNum1, up, general->mWPoint, axis);
}

Camera *CamTranslatorCharmedTripodBoss::getCamera() const {
	return mCamera;
}
#include "Game/Camera/CameraParamChunk.h"
#include "Game/Camera/CamTranslatorCharmedFix.h"
#include "Game/Util/MathUtil.h"

void CamTranslatorCharmedFix::setParam(const CameraParamChunk *pChunk) {
	CameraGeneralParam *general = pChunk->mGeneralParam;

	TVec3f up;
	up.setInline(general->mUp);

	if (MR::isNearZero(up, 0.001f)) {
		up.set(0.0f, 1.0f, 0.0f);
	}
	else {
		MR::normalize(&up);
	}

	CameraCharmedFix *camera = mCamera;

	camera->mAxis.set(general->mAxis);
	camera->mUp.set(up);
	camera->mWPoint.set(general->mWPoint);
}

Camera *CamTranslatorCharmedFix::getCamera() const {
	return mCamera;
}
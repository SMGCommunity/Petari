#include "Game/Camera/CameraParamChunk.hpp"
#include "Game/Camera/CamTranslatorFix.hpp"
#include "Game/Camera/CameraCalc.hpp"

#ifdef NON_MATCHING
// Trouble with negateInline
void CamTranslatorFix::setParam(const CameraParamChunk *pChunk) {
	CameraGeneralParam *general = pChunk->mGeneralParam;

	TVec3f aTStack32;
	TVec3f origin = TVec3f(0.0f, 0.0f, 0.0f);

	MR::polarToCross(origin, &aTStack32, general->mDist, general->mAxis.x, general->mAxis.y);

	TVec3f aTStack32Negative;
	aTStack32.negateInline(aTStack32Negative);

	CameraFix *camera = mCamera;
	camera->setParam(general->mWPoint, aTStack32Negative, general->mUp);
}
#endif

Camera *CamTranslatorFix::getCamera() const {
	return mCamera;
}

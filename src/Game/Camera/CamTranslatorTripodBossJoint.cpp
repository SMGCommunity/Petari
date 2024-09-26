#include "Game/Camera/CameraParamChunk.hpp"
#include "Game/Camera/CamTranslatorTripodBossJoint.hpp"

void CamTranslatorTripodBossJoint::setParam(const CameraParamChunk *pChunk) {
	CameraGeneralParam *general = pChunk->mGeneralParam;

	s32 uVar3;
	TVec2f angle;
	f32 dist;

	uVar3 = general->mNum1;
	dist = general->mDist;
	angle.y = 180.0f * general->mAngleA / 3.1415927f;
	angle.x = 180.0f * general->mAngleB / 3.1415927f;

	CameraTripodBossJoint *camera = mCamera;

	camera->mAngleB = angle.x;
	camera->mAngleA = angle.y;
	camera->mDist = dist;
	camera->_58 = uVar3;
	camera->mAxisX = general->mAxis.x;
	camera->mAxisY = general->mAxis.y;
	camera->mAxisZ = general->mAxis.z;
}

Camera *CamTranslatorTripodBossJoint::getCamera() const {
	return mCamera;
}
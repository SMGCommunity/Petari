#include "Game/Camera/CameraParamChunk.hpp"
#include "Game/Camera/CamTranslatorSpiral.hpp"

#ifdef NON_MATCHING
// Register mismatch
void CamTranslatorSpiral::setParam(const CameraParamChunk *pChunk) {
	CameraSpiral *camera = mCamera;
	CameraGeneralParam *general = pChunk->mGeneralParam;

	s32 uVar8;
	f32 wPointY;
	f32 axisY;
	f32 wPointZ;
	f32 axisZ;
	f32 wPointX;
	f32 axisX;
	s16 sVar7;

	sVar7 = reinterpret_cast<s16 *>(&general->mNum1)[1];
	axisX = general->mAxis.x;
	wPointX = general->mWPoint.x;
	axisZ = general->mAxis.z;
	wPointZ = general->mWPoint.z;
	axisY = general->mAxis.y;
	wPointY = general->mWPoint.y;
	uVar8 = general->mNum2;

	camera->_4C = reinterpret_cast<s16 *>(&general->mNum1)[0];
	camera->_54 = sVar7;
	camera->_58 = uVar8;
	camera->mWPointY = wPointY;
	camera->mAxisY = axisY;
	camera->mWPointZ = wPointZ;
	camera->mAxisZ = axisZ;
	camera->mWPointX = wPointX;
	camera->mAxisX = axisX;
}
#endif

Camera *CamTranslatorSpiral::getCamera() const {
	return mCamera;
}
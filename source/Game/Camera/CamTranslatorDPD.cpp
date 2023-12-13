#include "Game/Camera/CameraParamChunk.hpp"
#include "Game/Camera/CamTranslatorDPD.hpp"

void CamTranslatorDPD::setParam(const CameraParamChunk *pChunk) {
	CameraDPD *camera;

	CameraGeneralParam *general = pChunk->mGeneralParam;
	camera = mCamera;

	f32 dist;
	f32 angleA;
	f32 angleB;
	f32 wPointZ;
	f32 wPointX;
	f32 wPointY;
	f32 upX;
	s32 num2;

	num2 = general->mNum2;
	bool uVar8 = !(num2 != 1);
	upX = general->mUp.x;
	wPointY = general->mWPoint.y;
	wPointX = general->mWPoint.x;
	wPointZ = general->mWPoint.z;
	angleB = general->mAngleB;
	angleA = general->mAngleA;
	dist = general->mDist;

	camera->_94 = general->mNum1;
	camera->mDist = dist;
	camera->mAngleA = angleA;
	camera->mAngleB = angleB;
	camera->mWPointZ = wPointZ;
	camera->mWPointX = wPointX;
	camera->mWPointY = wPointY;
	camera->_B4 = uVar8;
	camera->mUpX = upX;
}

Camera *CamTranslatorDPD::getCamera() const {
	return mCamera;
}
#include "Game/Camera/CameraParamChunk.h"
#include "Game/Camera/CamTranslatorDead.h"

CamTranslatorDead::CamTranslatorDead(CameraDead *pCamera) : CamTranslatorDummy(pCamera) {

}

void CamTranslatorDead::setParam(const CameraParamChunk *pChunk) {
	CameraDead *camera;

	f32 dist;
	s32 num1;
	s32 num2;

	CameraGeneralParam *general = pChunk->mGeneralParam;
	camera = reinterpret_cast<CameraDead *>(mCamera);

	num2 = general->mNum2;
	num1 = general->mNum1;
	dist = general->mDist;
	
	camera->mDist = dist;
	camera->_54 = num1;
	camera->_58 = num2;
}

Camera *CamTranslatorDead::getCamera() const {
	return mCamera;
}
#include "Game/Camera/CameraParamChunk.hpp"
#include "Game/Camera/CamTranslatorFixedThere.hpp"

void CamTranslatorFixedThere::setParam(const CameraParamChunk *pChunk) {
	CameraFixedThere *camera = mCamera;
	CameraGeneralParam *general = pChunk->mGeneralParam;

	s32 num2 = general->mNum2;
	bool uVar1 = !(num2 != 1);
	s32 num1 = general->mNum1;

	camera->_4C = num1;
	camera->_50 = uVar1;
}

Camera *CamTranslatorFixedThere::getCamera() const {
	return mCamera;
}

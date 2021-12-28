#include "Game/Camera/CameraParamChunk.h"
#include "Game/Camera/CamTranslatorFooFighterPlanet.h"

CamTranslatorFooFighterPlanet::CamTranslatorFooFighterPlanet(CameraFooFighterPlanet *pCamera) : CamTranslatorDummy(pCamera) {

}

void CamTranslatorFooFighterPlanet::setParam(const CameraParamChunk *pChunk) {
	CameraGeneralParam *general = pChunk->mGeneralParam;
	CameraFooFighterPlanet *camera = reinterpret_cast<CameraFooFighterPlanet *>(mCamera);

	f32 axisX;
	f32 axisY;
	f32 angleA;

	angleA = general->mAngleA;
	axisY = general->mAxis.y;
	axisX = general->mAxis.x;

	camera->mAxisX = axisX;
	camera->mAxisY = axisY;
	camera->mAngleA = angleA;
}

Camera *CamTranslatorFooFighterPlanet::getCamera() const {
	return mCamera;
}
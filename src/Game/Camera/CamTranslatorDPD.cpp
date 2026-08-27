#include "Game/Camera/CamTranslatorDPD.hpp"
#include "Game/Camera/CameraParamChunk.hpp"

void CamTranslatorDPD::setParam(const CameraParamChunk* pChunk) {
    CameraGeneralParam* general = pChunk->mGeneralParam;

    mCamera->setParam(general->mNum1, general->mDist, general->mAngleA, general->mAngleB, general->mWPoint.z, general->mWPoint.x, general->mWPoint.y,
                      general->mNum2 == CameraDPD::CameraType_UpdateWithTarget, general->mUp.x);
}

Camera* CamTranslatorDPD::getCamera() const {
    return mCamera;
}

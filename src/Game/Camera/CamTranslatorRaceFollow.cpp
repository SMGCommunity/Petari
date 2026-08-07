#include "Game/Camera/CamTranslatorRaceFollow.hpp"
#include "Game/Camera/CameraParamChunk.hpp"
#include "Game/Util/MathUtil.hpp"

void CamTranslatorRaceFollow::setParam(const CameraParamChunk* pChunk) {
    CameraGeneralParam* general = pChunk->mGeneralParam;

    mCamera->setParam(general->mWPoint.x, general->mWPoint.y, general->mAngleA * MR::pi() / 180.0f, general->mNum1 != 0, general->mWPoint.z);
}

Camera* CamTranslatorRaceFollow::getCamera() const {
    return mCamera;
}

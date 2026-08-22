#include "Game/Camera/CamTranslatorTowerPos.hpp"
#include "Game/Camera/CameraParamChunk.hpp"
#include "Game/Util/MathUtil.hpp"

void CamTranslatorTowerPos_FORCE_MATCH_SDATA2() {
    (void)1.0f;
    (void)0.5f;
    (void)MR::pi();
}

void CamTranslatorTowerPos::setParam(const CameraParamChunk* pChunk) {
    CameraGeneralParam* general = pChunk->mGeneralParam;

    mCamera->setParam(general->mWPoint, general->mAxis, general->mUp.x, TVec2f(general->mAngleB, MR::pi() * (1.0f - general->mAngleA) * 0.5f),
                      general->mUp.y);
}

Camera* CamTranslatorTowerPos::getCamera() const {
    return mCamera;
}

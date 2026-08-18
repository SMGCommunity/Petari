#include "Game/Camera/CamTranslatorTower.hpp"
#include "Game/Camera/CameraParamChunk.hpp"
#include "Game/Util/MathUtil.hpp"

void CamTranslatorTower_FORCE_MATCH_SDATA2() {
    (void)1.0f;
    (void)0.5f;
    (void)MR::pi();
}

void CamTranslatorTower::setParam(const CameraParamChunk* pChunk) {
    CameraGeneralParam* general = pChunk->mGeneralParam;

    mCamera->setParam(general->mWPoint, general->mAxis, general->mDist, TVec2f(general->mAngleB, MR::pi() * (1.0f - general->mAngleA) * 0.5f));
}

Camera* CamTranslatorTower::getCamera() const {
    return mCamera;
}

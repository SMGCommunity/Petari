#include "Game/Camera/CamTranslatorBehind.hpp"
#include "Game/Camera/CameraParamChunk.hpp"
#include "Game/Util/MathUtil.hpp"

void CamTranslatorBehind_FORCE_MATCH_SDATA2() {
    (void)MR::pi();
}

void CamTranslatorBehind::setParam(const CameraParamChunk* pChunk) {
    CameraGeneralParam* general = pChunk->mGeneralParam;

    mCamera->setParam(TVec2f(180.0f * general->mAngleB / MR::pi(), 180.0f * general->mAngleA / MR::pi()), general->mDist, general->mAxis.x);
}

Camera* CamTranslatorBehind::getCamera() const {
    return mCamera;
}

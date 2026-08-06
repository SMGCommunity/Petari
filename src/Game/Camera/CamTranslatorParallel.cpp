#include "Game/Camera/CamTranslatorParallel.hpp"
#include "Game/Camera/CameraParamChunk.hpp"
#include "Game/Util/MathUtil.hpp"

void CamTranslatorParallel_FORCE_MATCH_SDATA2() {
    (void)MR::pi();
}

void CamTranslatorParallel::setParam(const CameraParamChunk* pChunk) {
    CameraGeneralParam* general = pChunk->mGeneralParam;

    mCamera->setParam(TVec2f(180.0f * general->mAngleB / MR::pi(), 180.0f * general->mAngleA / MR::pi()), general->mDist, general->mNum1 == 1);
}

Camera* CamTranslatorParallel::getCamera() const {
    return mCamera;
}

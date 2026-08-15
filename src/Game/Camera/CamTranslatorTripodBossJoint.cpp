#include "Game/Camera/CamTranslatorTripodBossJoint.hpp"
#include "Game/Camera/CameraParamChunk.hpp"
#include "Game/Util/MathUtil.hpp"

void CamTranslatorTripodBossJoint_FORCE_MATCH_SDATA2() {
    (void)MR::pi();
}

void CamTranslatorTripodBossJoint::setParam(const CameraParamChunk* pChunk) {
    CameraGeneralParam* general = pChunk->mGeneralParam;

    mCamera->setParam(TVec2f(180.0f * general->mAngleB / MR::pi(), 180.0f * general->mAngleA / MR::pi()), general->mDist, general->mNum1,
                      general->mAxis);
}

Camera* CamTranslatorTripodBossJoint::getCamera() const {
    return mCamera;
}

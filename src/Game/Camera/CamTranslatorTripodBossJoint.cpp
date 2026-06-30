#include "Game/Camera/CamTranslatorTripodBossJoint.hpp"
#include "Game/Camera/CameraParamChunk.hpp"
#include "Game/Util/MathUtil.hpp"

void CamTranslatorTripodBossJoint_FORCE_MATCH_SDATA2() {
    (void)MR::pi();
}

void CamTranslatorTripodBossJoint::setParam(const CameraParamChunk* pChunk) {
    CameraGeneralParam* general = pChunk->mGeneralParam;

    s32 uVar3;
    TVec2f angle;
    f32 dist;

    uVar3 = general->mNum1;
    dist = general->mDist;
    angle.y = 180.0f * general->mAngleA / MR::pi();
    angle.x = 180.0f * general->mAngleB / MR::pi();

    CameraTripodBossJoint* camera = mCamera;

    camera->mAngleB = angle.x;
    camera->mAngleA = angle.y;
    camera->mDist = dist;
    camera->_58 = uVar3;
    camera->mAxis.set(general->mAxis);
}

Camera* CamTranslatorTripodBossJoint::getCamera() const {
    return mCamera;
}

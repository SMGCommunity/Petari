#include "Game/Camera/CamTranslatorBehind.hpp"
#include "Game/Camera/CameraParamChunk.hpp"
#include "Game/Util/MathUtil.hpp"

void CamTranslatorBehind_FORCE_MATCH_SDATA2() {
    (void)MR::pi();
}

void CamTranslatorBehind::setParam(const CameraParamChunk* pChunk) {
    TVec2f angle;
    f32 dist;
    f32 axisX;
    angle.y = 180.0f * pChunk->mGeneralParam->mAngleA / MR::pi();
    angle.x = 180.0f * pChunk->mGeneralParam->mAngleB / MR::pi();
    axisX = pChunk->mGeneralParam->mAxis.x;
    dist = pChunk->mGeneralParam->mDist;

    CameraBehind* camera = mCamera;

    camera->mAngleB = angle.x;
    camera->mAngleA = angle.y;
    camera->mDist = dist;
    camera->mAxisX = axisX;
}

Camera* CamTranslatorBehind::getCamera() const {
    return mCamera;
}

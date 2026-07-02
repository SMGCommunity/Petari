#include "Game/Camera/CamTranslatorParallel.hpp"
#include "Game/Camera/CameraParamChunk.hpp"
#include "Game/Util/MathUtil.hpp"

void CamTranslatorParallel_FORCE_MATCH_SDATA2() {
    (void)MR::pi();
}

void CamTranslatorParallel::setParam(const CameraParamChunk* pChunk) {
    TVec2f angle;
    f32 dist;
    f32 axisX;

    dist = pChunk->mGeneralParam->mDist;
    angle.y = 180.0f * pChunk->mGeneralParam->mAngleA / MR::pi();
    angle.x = 180.0f * pChunk->mGeneralParam->mAngleB / MR::pi();
    bool var = !(pChunk->mGeneralParam->mNum1 != 1);

    CameraParallel* camera = mCamera;

    camera->mAngleB = angle.x;
    camera->mAngleA = angle.y;
    camera->mDist = dist;
    camera->_58 = var;
}

Camera* CamTranslatorParallel::getCamera() const {
    return mCamera;
}

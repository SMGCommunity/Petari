#include "Game/Camera/CamTranslatorTower.hpp"
#include "Game/Camera/CameraParamChunk.hpp"
#include "Game/Util/MathUtil.hpp"

void CamTranslatorTower_FORCE_MATCH_SDATA2() {
    (void)1.0f;
    (void)0.5f;
    (void)MR::pi();
}

void CamTranslatorTower::setParam(const CameraParamChunk* pChunk) {
    CameraTower* camera;
    CameraGeneralParam* general = pChunk->mGeneralParam;

    f32 dist;
    TVec2f angle;

    angle.x = general->mAngleB;
    angle.y = MR::pi() * (1.0f - general->mAngleA) * 0.5f;
    dist = general->mDist;

    camera = mCamera;

    camera->mWPoint.set(general->mWPoint);
    camera->mAxis.set(general->mAxis);
    camera->mDist = dist;
    camera->mAngleB = angle.x;
    camera->mAngleA = angle.y;
}

Camera* CamTranslatorTower::getCamera() const {
    return mCamera;
}

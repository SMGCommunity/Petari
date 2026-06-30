#include "Game/Camera/CamTranslatorTowerPos.hpp"
#include "Game/Camera/CameraParamChunk.hpp"
#include "Game/Util/MathUtil.hpp"

void CamTranslatorTowerPos_FORCE_MATCH_SDATA2() {
    (void)1.0f;
    (void)0.5f;
    (void)MR::pi();
}

void CamTranslatorTowerPos::setParam(const CameraParamChunk* pChunk) {
    CameraTowerPos* camera;
    CameraGeneralParam* general = pChunk->mGeneralParam;

    f32 upX;
    f32 upY;

    TVec2f angle;

    angle.x = general->mAngleB;
    angle.y = (1.0f - general->mAngleA) * MR::pi() * 0.5f;
    upY = general->mUp.y;
    upX = general->mUp.x;

    camera = mCamera;

    camera->mWPoint.set(general->mWPoint);
    camera->mAxis.set(general->mAxis);
    camera->mUpX = upX;
    camera->mAngleA = angle.y;
    camera->mAngleB = angle.x;
    camera->mUpY = upY;
}

Camera* CamTranslatorTowerPos::getCamera() const {
    return mCamera;
}

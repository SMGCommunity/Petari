#include "Game/Camera/CamTranslatorCharmedVecRegTower.hpp"
#include "Game/Camera/CameraParamChunk.hpp"
#include "Game/Util.hpp"

void CamTranslatorCharmedVecRegTower_FORCE_MATCH_SDATA2() {
    (void)1.0f;
    (void)0.0f;
}

void CamTranslatorCharmedVecRegTower::setParam(const CameraParamChunk* pChunk) {
    const char* string;
    f32 angleA;
    f32 angleB;
    f32 dist;
    bool uVar3;
    TVec3f axis;

    CameraGeneralParam* general = pChunk->mGeneralParam;

    axis = general->mAxis;

    MR::normalizeOrZero(&axis);

    if (MR::isNearZero(axis)) {
        axis.set< f32 >(0.0f, 1.0f, 0.0f);
    }

    uVar3 = general->mNum1 != 0;
    dist = general->mDist;
    angleB = general->mAngleB;
    angleA = general->mAngleA;
    string = general->mString.getCharPtr();

    CameraCharmedVecRegTower* camera = mCamera;

    mCamera->mString = string;
    camera->mWPoint.set(general->mWPoint);
    camera->mAxis.set(axis);
    camera->mAngleA = angleA;
    camera->mAngleB = angleB;
    camera->mDist = dist;
    camera->mArrange = uVar3;
}

Camera* CamTranslatorCharmedVecRegTower::getCamera() const {
    return mCamera;
}

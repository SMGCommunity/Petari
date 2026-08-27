#include "Game/Camera/CamTranslatorCharmedVecRegTower.hpp"
#include "Game/Camera/CameraParamChunk.hpp"
#include "Game/Util.hpp"

void CamTranslatorCharmedVecRegTower_FORCE_MATCH_SDATA2() {
    (void)1.0f;
    (void)0.0f;
}

void CamTranslatorCharmedVecRegTower::setParam(const CameraParamChunk* pChunk) {
    CameraGeneralParam* general = pChunk->mGeneralParam;

    TVec3f axis = general->mAxis;
    MR::normalizeOrZero(&axis);

    if (MR::isNearZero(axis)) {
        axis.set< f32 >(0.0f, 1.0f, 0.0f);
    }

    mCamera->setParam(general->mString.getCharPtr(), general->mWPoint, axis, general->mAngleA, general->mAngleB, general->mDist, general->mNum1 != 0);
}

Camera* CamTranslatorCharmedVecRegTower::getCamera() const {
    return mCamera;
}

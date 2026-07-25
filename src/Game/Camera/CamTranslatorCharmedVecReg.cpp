#include "Game/Camera/CamTranslatorCharmedVecReg.hpp"
#include "Game/Camera/CameraParamChunk.hpp"

void CamTranslatorCharmedVecReg::setParam(const CameraParamChunk* pChunk) {
    CameraGeneralParam* general = pChunk->mGeneralParam;

    const char* string;
    f32 dist;
    f32 pitch;
    f32 angleYMax;
    f32 angleYMin;
    f32 distRatio;
    f32 camAngleRate;

    camAngleRate = general->mAngleB;
    distRatio = general->mAngleA;
    angleYMin = general->mAxis.z;
    angleYMax = general->mAxis.y;
    pitch = general->mAxis.x;
    dist = general->mDist;
    string = general->mString.getCharPtr();

    CameraCharmedVecReg* camera = mCamera;

    camera->mString = string;
    camera->mDist = dist;

    if (dist < 300.0f) {
        camera->mDist = 300.0f;
    }

    camera->mPitch = pitch;
    camera->mAngleYMin = angleYMin;
    camera->mAngleYMax = angleYMax;
    camera->mCamDistRatio = distRatio;
    camera->mCamAngleRate = camAngleRate;
}

Camera* CamTranslatorCharmedVecReg::getCamera() const {
    return mCamera;
}

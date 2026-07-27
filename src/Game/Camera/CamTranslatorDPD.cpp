#include "Game/Camera/CamTranslatorDPD.hpp"
#include "Game/Camera/CameraParamChunk.hpp"

void CamTranslatorDPD::setParam(const CameraParamChunk* pChunk) {
    CameraDPD* camera;

    CameraGeneralParam* general = pChunk->mGeneralParam;
    camera = mCamera;

    f32 dist;
    f32 angleXRate;
    f32 angleYRate;
    f32 rotate;
    f32 speedBlendRate;
    f32 blendFriction;
    f32 arg_B8;
    s32 num2;

    num2 = general->mNum2;
    bool uVar8 = !(num2 != CameraDPD::CameraState_1);
    arg_B8 = general->mUp.x;
    blendFriction = general->mWPoint.y;
    speedBlendRate = general->mWPoint.x;
    rotate = general->mWPoint.z;
    angleYRate = general->mAngleB;
    angleXRate = general->mAngleA;
    dist = general->mDist;

    camera->mCameraState = general->mNum1;
    camera->mDist = dist;
    camera->mAngleXRange = angleXRate;
    camera->mAngleYRange = angleYRate;
    camera->mRotate = rotate;
    camera->mSpeedBlendRate = speedBlendRate;
    camera->mBlendFriction = blendFriction;
    camera->_B4 = uVar8;
    camera->_B8 = arg_B8;
}

Camera* CamTranslatorDPD::getCamera() const {
    return mCamera;
}

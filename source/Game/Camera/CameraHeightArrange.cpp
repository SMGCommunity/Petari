#include "Game/Camera/Camera.hpp"
#include "Game/Camera/CameraHeightArrange.hpp"
#include "Game/Camera/CameraPoseParam.hpp"
#include "Game/Camera/CameraTargetObj.hpp"

bool CameraTargetObj::isFastDrop() const {
    return false;
}

bool CameraTargetObj::isFastRise() const {
    return false;
}

CameraHeightArrange::CameraHeightArrange(Camera *pCamera) : NameObj("CameraHeightArrange") {
    mCamera = pCamera;
    _10 = 0;
    _11 = 0;
    _12 = 0;
    _14 = new CameraPoseParam();
    _18 = new CameraPoseParam();
    _28 = 0.0f;
    _2C = 0.0f;
    _30 = 0.0f;
    _34 = 0.0f;
    _38 = 0.0f;
    _3C = 0.0f;                           
    _40 = 0;
    _44 = 0;
    _4C = 0;
    _50 = 0;
    _54 = 0;
    _58 = 0;
    _5C = 0;
    _60 = 0;
    mGlobalAxis.x = 0.0f;
    mGlobalAxis.y = 1.0f;
    mGlobalAxis.z = 0.0f;
    _70 = 0.1f;
    _74 = 15.0f;
    mUpper = 0.3f;
    mLower = 0.1f;
    mGndInt = 160;
    mUPlay = 500.0f;
    mLPlay = 800.0f;
    mPushDelay = 120;
    mPushDelayLow = 120;
    mUDown = 120;
    mVPanUse = 1;
    mVPanAxis.x = 0.0f;
    mVPanAxis.y = 1.0f;
    mVPanAxis.z = 0.0f;
}

void CameraHeightArrange::resetJump() {
    _10 = 0;
    _11 = 0;
    _12 = 0;
}

void CameraHeightArrange::resetParameter() {
    _70 = 0.1f;
    _74 = 15.0f;
    mUpper = 0.3f;
    mLower = 0.1f;
    mGndInt = 160;
    mUPlay = 500.0f;
    mLPlay = 800.0f;
    mPushDelay = 120;
    mPushDelayLow = 120;
    mUDown = 120;
    mVPanUse = 1;
    mVPanAxis.x = 0.0f;
    mVPanAxis.y = 1.0f;
    mVPanAxis.z = 0.0f;
    mGlobalAxis.x = 0.0f;
    mGlobalAxis.y = 1.0f;
    mGlobalAxis.z = 0.0f;
    _60 = 0;
}

void CameraHeightArrange::chase() {
    TVec3f globalAxis = *getGlobalAxis();

    f32 fVar1 = (float)_50 / (float)mLPlay;

    if (fVar1 > 1.0f) {
        fVar1 = 1.0f;
    }
    
    if (fVar1 > 0.5f) {
        _5C = 0;
    }

    updateHeightAndOffset();
}

void CameraHeightArrange::updateHeightAndOffset() {
    TVec3f globalAxis = *getGlobalAxis();
    f32 fVar1;

    if (mVPanUse != 0) {
        fVar1 = 0.05f;
    }
    else {
        fVar1 = 1.0f;
    }

    _38 += -_38 * fVar1;
    _3C += -_3C * fVar1;
    float dVar4 = globalAxis.dot(_14->_C);
    _30 += fVar1 * (dVar4 - _30);
    dVar4 = globalAxis.dot(_14->_24);
    _34 += fVar1 * (dVar4 - _34);
}

#ifdef NON_MATCHING
// Float operation order
TVec3f *CameraHeightArrange::getGlobalAxis() {
    if (_60 != 0) {
        _60 = 0;
        
        mGlobalAxis.set(mVPanAxis);
        TMtx34f &matrix = mCamera->mZoneMatrix;

        float axisX = mGlobalAxis.x;
        float axisY = mGlobalAxis.y;
        float axisZ = mGlobalAxis.z;

        mGlobalAxis.set(
            axisZ * matrix.mMtx[0][2] + axisX * matrix.mMtx[0][0] + axisY * matrix.mMtx[0][1],
            axisZ * matrix.mMtx[1][2] + axisX * matrix.mMtx[1][0] + axisY * matrix.mMtx[1][1],
            axisZ * matrix.mMtx[2][2] + axisX * matrix.mMtx[2][0] + axisY * matrix.mMtx[2][1]
        );
    }

    return &mGlobalAxis;
}
#endif
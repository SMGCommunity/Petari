#include "Game/Camera/Camera.h"
#include "Game/Camera/CameraHeightArrange.h"
#include "Game/Camera/CameraPoseParam.h"

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
    _78 = 0.3f;
    _7C = 0.1f;
    _80 = 160;
    _84 = 500.0f;
    _88 = 800.0f;
    _8C = 120;
    _90 = 120;
    _94 = 120;
    _98 = 1;
    _9C.x = 0.0f;
    _9C.y = 1.0f;
    _9C.z = 0.0f;
}

void CameraHeightArrange::resetJump() {
    _10 = 0;
    _11 = 0;
    _12 = 0;
}

void CameraHeightArrange::resetParameter() {
    _70 = 0.1f;
    _74 = 15.0f;
    _78 = 0.3f;
    _7C = 0.1f;
    _80 = 160;
    _84 = 500.0f;
    _88 = 800.0f;
    _8C = 120;
    _90 = 120;
    _94 = 120;
    _98 = 1;
    _9C.x = 0.0f;
    _9C.y = 1.0f;
    _9C.z = 0.0f;
    mGlobalAxis.x = 0.0f;
    mGlobalAxis.y = 1.0f;
    mGlobalAxis.z = 0.0f;
    _60 = 0;
}

void CameraHeightArrange::chase() {
    TVec3f globalAxis = *getGlobalAxis();

    f32 fVar1 = (float)_50 / (float)_80;

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

    if (_98 != 0) {
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
        
        mGlobalAxis.set(_9C);
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
#include "Game/MapObj/BrightObj.h"

BrightInfo::BrightInfo() {
    reset();
}

void BrightInfo::endRead() {
    u32 v1 = _4;
    if (v1 != _0) {
        _4 = v1 + 1;

        if (v1 + 1 > 2) {
            _4 = 0;
        }
    }
}

void BrightInfo::reset() {
    _0 = 0;
    _4 = 0;

    for (u32 i = 0; i < 3; i++) {
        mBrightnessCenter[i].zero();
        mRealCenter[i].zero();
        mBright[i] = 0.0f;
    }
}

BrightDrawInfo::BrightDrawInfo() {
    for (u32 i = 0; i < 2; i++) {
        mPosition[i].zero();
        mRadius[i] = 0.0f;
    }
}

BrightCamInfo::BrightCamInfo() {
    for (u32 i = 0; i < 2; i++) {
        mViewMtx[i].identity();
        /* set mtx to identity mtx */
        Mtx44Ptr mtx = mProjectionMtx[i];
        mtx[2][3] = 0.0f;
        mtx[1][3] = 0.0f;
        mtx[0][3] = 0.0f;
        mtx[3][2] = 0.0f;
        mtx[1][2] = 0.0f;
        mtx[0][2] = 0.0f;
        mtx[3][1] = 0.0f;
        mtx[2][1] = 0.0f;
        mtx[0][1] = 0.0f;
        mtx[3][0] = 0.0f;
        mtx[2][0] = 0.0f;
        mtx[1][0] = 0.0f;
        mtx[3][3] = 1.0f;
        mtx[2][2] = 1.0f;
        mtx[1][1] = 1.0f;
        mtx[0][0] = 1.0f;
        mCameraDir[i].set(0.0f, 1.0f, 0.0f);
        mCameraPos[i].zero();
    }
}
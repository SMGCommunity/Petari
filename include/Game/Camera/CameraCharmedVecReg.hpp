#pragma once

#include "Game/Camera/Camera.hpp"

class CameraCharmedVecReg : public Camera {
public:
    CameraCharmedVecReg(const char* pName = "ベクトルレジスタ注目カメラ");

    virtual void reset();
    virtual CameraTargetObj* calc();
    virtual CamTranslatorBase* createTranslator();

    void setParam(const char* pName, f32 dist, f32 angleX, f32 angleYMax, f32 angleYMin, f32 distRatio, f32 angleRate) {
        mName = pName;
        mDist = dist;
        if (mDist < 300.0f) {
            mDist = 300.0f;
        }
        mAngleX = angleX;
        mAngleYMin = angleYMin;
        mAngleYMax = angleYMax;
        mCamDistRatio = distRatio;
        mCamAngleRate = angleRate;
    }

    /* 0x4C */ const char* mName;
    /* 0x50 */ f32 mDist;
    /* 0x54 */ f32 mAngleX;
    /* 0x58 */ f32 mAngleYMin;
    /* 0x5C */ f32 mAngleYMax;
    /* 0x60 */ f32 mCamDistRatio;
    /* 0x64 */ f32 mCamAngleRate;
};

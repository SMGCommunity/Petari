#pragma once

#include "Game/Camera/Camera.hpp"

class CameraCharmedVecReg : public Camera {
public:
    CameraCharmedVecReg(const char* pName = "ベクトルレジスタ注目カメラ");

    virtual void reset();
    virtual CameraTargetObj* calc();
    virtual CamTranslatorBase* createTranslator();

    /* 0x4C */ const char* mString;
    /* 0x50 */ f32 mDist;
    /* 0x54 */ f32 mPitch;
    /* 0x58 */ f32 mAngleYMin;
    /* 0x5C */ f32 mAngleYMax;
    /* 0x60 */ f32 mCamDistRatio;
    /* 0x64 */ f32 mCamAngleRate;
};

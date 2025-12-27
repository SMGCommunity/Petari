#pragma once

#include "Game/Camera/Camera.hpp"

class CameraCharmedVecReg : public Camera {
public:
    CameraCharmedVecReg(const char* pName = "ベクトルレジスタ注目カメラ");
    virtual ~CameraCharmedVecReg();

    virtual void reset();
    virtual CameraTargetObj* calc();
    virtual CamTranslatorBase* createTranslator();

    const char* mString;  // 0x4C
    f32 mDist;            // 0x50
    f32 mAxisX;           // 0x54
    f32 mAxisZ;           // 0x58
    f32 mAxisY;           // 0x5C
    f32 mAngleA;          // 0x60
    f32 mAngleB;          // 0x64
};
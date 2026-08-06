#pragma once

#include "Game/Camera/Camera.hpp"

class CameraObjParallel : public Camera {
public:
    CameraObjParallel(const char* pName = "オブジェ並行カメラ");

    virtual void reset();
    virtual CameraTargetObj* calc();
    virtual CamTranslatorBase* createTranslator();

    void setParam(f32 dist, f32 angleX, f32 angleY) {
        mDist = dist;
        mAngleX = angleX;
        mAngleY = angleY;
    }

    /* 0x4C */ f32 mAngleX;
    /* 0x50 */ f32 mAngleY;
    /* 0x54 */ f32 mDist;
};

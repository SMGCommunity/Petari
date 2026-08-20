#pragma once

#include "Game/Camera/Camera.hpp"

class CameraTrundle : public Camera {
public:
    CameraTrundle(const char* pName = "トランドルカメラ");

    virtual void reset();
    virtual CameraTargetObj* calc();
    virtual CamTranslatorBase* createTranslator();

    void setParam(const TVec3f& wPoint, const TVec3f& axis, f32 dist, f32 angleX, f32 angleY, f32 dotMax) {
        mWPoint.set(wPoint);
        mAxis.set(axis);
        mDist = dist;
        mAngleX = angleX;
        mAngleY = angleY;
        mDotMax = dotMax;
    }

    /* 0x4C */ TVec3f mWPoint;
    /* 0x58 */ TVec3f mAxis;
    /* 0x64 */ f32 mDist;
    /* 0x68 */ f32 mAngleX;
    /* 0x6C */ f32 mAngleY;
    /* 0x70 */ f32 mDotMax;
};

#pragma once

#include "Game/Camera/Camera.hpp"

class CameraTripodPlanet : public Camera {
public:
    CameraTripodPlanet(const char* pName = "三脚惑星カメラ");

    virtual void reset();
    virtual CameraTargetObj* calc();
    virtual CamTranslatorBase* createTranslator();

    void setParam(const TVec3f& wPoint, const TVec3f& axis, const TVec2f& angle, f32 dist) {
        mWPoint.set(wPoint);
        mAxis.set(axis);
        mAngleX = angle.x;
        mAngleY = angle.y;
        mDist = dist;
    }

    /* 0x4C */ TVec3f mAxis;
    /* 0x58 */ TVec3f mWPoint;
    /* 0x64 */ f32 mAngleX;
    /* 0x68 */ f32 mAngleY;
    /* 0x6C */ f32 mDist;
    /* 0x70 */ bool _70;
};

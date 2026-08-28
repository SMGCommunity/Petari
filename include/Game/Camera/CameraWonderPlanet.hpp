#pragma once

#include "Game/Camera/Camera.hpp"

class CameraWonderPlanet : public Camera {
public:
    CameraWonderPlanet(const char* pName = "ワンダープラネットカメラ");
    /* 0x08 */ virtual ~CameraWonderPlanet();

    /* 0x24 */ virtual void reset();
    /* 0x28 */ virtual CameraTargetObj* calc();
    /* 0x50 */ virtual bool isEnableToReset() const {
        return true;
    }
    /* 0x54 */ virtual CamTranslatorBase* createTranslator();

    void updateCameraReset();
    void rotateCamera();
    void checkReset();

    void setParam(f32 distMin, f32 distMax, f32 angleX) {
        mDistMin = distMin;
        mDistMax = distMax;
        mAngleX = angleX;
    }

    /* 0x4C */ f32 mDistMin;
    /* 0x50 */ f32 mDistMax;
    /* 0x54 */ f32 mAngleX;
    /* 0x58 */ bool mIsRotating;
    /* 0x5C */ s32 mRotateFrame;
};

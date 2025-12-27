#pragma once

#include "Game/Camera/Camera.hpp"

class CameraWonderPlanet : public Camera {
public:
    CameraWonderPlanet(const char* pName = "ワンダープラネットカメラ");
    virtual ~CameraWonderPlanet();

    virtual void reset();
    virtual CameraTargetObj* calc();
    virtual bool isEnableToReset() const;
    virtual CamTranslatorBase* createTranslator();

    void updateCameraReset();
    void rotateCamera();
    void checkReset();

    f32 mAxisX;
    f32 mAxisY;
    f32 mAngleA;
    bool _58;
    u8 _59[3];
    u32 _5C;
};
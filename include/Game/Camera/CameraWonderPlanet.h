#pragma once

#include "Game/Camera/Camera.h"

class CameraWonderPlanet : public Camera {
public:
    CameraWonderPlanet(const char *);
    virtual ~CameraWonderPlanet();

    virtual void reset();
    virtual CameraTargetObj *calc();
    virtual bool isEnableToReset() const;
    virtual CamTranslatorDummy *createTranslator();

    void updateCameraReset();
    void rotateCamera();
    void checkReset();

    f32 _4C;
    f32 _50;
    f32 _54;
    u8 _58;
    u8 _59[3];
    u32 _5C;
};
#pragma once

#include "Game/Camera/Camera.h"

class CameraBehind : public Camera {
public:
    CameraBehind(const char *);
    virtual ~CameraBehind();

    virtual void reset();
    virtual CameraTargetObj *calc();
    virtual CamTranslatorDummy *createTranslator();
    
    void calcPosAndUp();

    f32 _4C;
    f32 _50;
    f32 _54;
    u8 _58[4];
};
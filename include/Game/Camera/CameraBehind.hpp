#pragma once

#include "Game/Camera/Camera.h"

class CameraBehind : public Camera {
public:
    CameraBehind(const char *);
    virtual ~CameraBehind();

    virtual void reset();
    virtual CameraTargetObj *calc();
    virtual CamTranslatorBase *createTranslator();
    
    void calcPosAndUp();

    f32 mAngleB;    // _4C
    f32 mAngleA;    // _50
    f32 mDist;      // _54
    f32 mAxisX;     // _58
};
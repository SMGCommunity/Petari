#pragma once

#include "Game/Camera/Camera.h"

class CameraSubjective : public Camera {
public:
    CameraSubjective(const char *);
    virtual ~CameraSubjective();

    virtual void reset();
    virtual CameraTargetObj *calc();
    virtual bool isCollisionOff() const;
    virtual CamTranslatorDummy *createTranslator();

    f32 _4C;
    f32 _50;
    f32 _54;
    f32 _58;
    TMtx34f _5C;
};
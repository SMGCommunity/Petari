#pragma once

#include "Game/Camera/Camera.h"
#include "JSystem/JGeometry/TMatrix.h"

class CameraDPD : public Camera {
public:
    CameraDPD(const char *);
    virtual ~CameraDPD();

    virtual void reset();
    virtual CameraTargetObj *calc();
    virtual CamTranslatorDummy *createTranslator();

    f32 _4C;
    f32 _50;
    f32 _54;
    f32 _58;
    f32 _5C;
    TMtx34f _60;
    u8 _90;
    u8 _91[3];
    u32 _94;
    f32 _98;
    f32 _9C;
    f32 _A0;
    f32 _A4;
    f32 _A8;
    f32 _AC;
    f32 _B0;
    u8 _B4;
    u8 _B5[3];
    f32 _B8;
};
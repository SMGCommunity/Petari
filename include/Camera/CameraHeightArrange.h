#pragma once

#include "NameObj/NameObj.h"
#include "Camera/CameraPoseParam.h"

class Camera;

class CameraHeightArrange : public NameObj
{
public:
    CameraHeightArrange(Camera *);

    Camera* _C;
    u8 _10;
    u8 _11;
    u8 _12;
    u8 _13;
    CameraPoseParam* _14;
    CameraPoseParam* _18;
    u8 _1C[0x60-0x1C];
    u8 _60;
    u8 _61[0xA8-0x61];
};
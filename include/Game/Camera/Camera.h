#pragma once

#include "revolution.h"
#include "Game/Camera/CameraPoseParam.h"
#include "Game/NameObj/NameObj.h"
#include "JSystem/JGeometry/TVec.h"
#include "JSystem/JGeometry/TMatrix.h"

class Camera : public NameObj {
public:
    u8 _C[4];
    CameraPoseParam* mPosParam; // _10
    s32 _14;
    u8 _18;
    u8 _19[3];
    TMtx34f mZoneMatrix;        // _1C
};

class CamAnmDataAccessor {
public:
    s32 _04;
    s32 _08;
};

class CameraAnim : public Camera {
public:
    s32 _4C;
    s32 _50;
    s32 _54;
    u8 _58;
    u8 _59[3];
    f32 _5C;
    float _60;
    s32 _64;
    void* _68;
    void* _6C;
    s32 _70;
    s32 _74;
    s32 _78;
    u8 _7C;
    u8 _7D[3];
};
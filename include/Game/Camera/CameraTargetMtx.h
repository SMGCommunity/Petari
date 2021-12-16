#pragma once

#include "Game/Camera/CameraTargetObj.h"
#include "JSystem/JGeometry/TMatrix.h"

class CameraTargetMtx : public CameraTargetObj {
public:
    CameraTargetMtx(const char *);

    TMtx34f _10;
    f32 _40;
    f32 _44;
    f32 _48;
    f32 _4C;
    f32 _50;
    f32 _54;
    f32 _58;
    f32 _5C;
    f32 _60;
    f32 _64;
    f32 _68;
    f32 _6C;
    f32 _70;
    f32 _74;
    f32 _78;
    f32 _7C;
    f32 _80;
    f32 _84;
    u8 _88;
    u8 _89[3];
    u32 _8C;
};
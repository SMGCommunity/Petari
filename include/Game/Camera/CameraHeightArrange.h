#pragma once

#include "Game/Camera/CameraPoseParam.h"
#include "Game/NameObj/NameObj.h"

class Camera;

class CameraHeightArrange : public NameObj {
public:
    CameraHeightArrange(Camera *);

    void resetJump();

    void resetParameter();

    void chase();
    void updateHeightAndOffset();
    TVec3f *getGlobalAxis();

    Camera *mCamera;        // _C
    u8 _10;
    u8 _11;
    u8 _12;
    u8 _13;
    CameraPoseParam *_14;
    CameraPoseParam *_18;
    u8 _1C[12];
    f32 _28;
    f32 _2C;
    f32 _30;
    f32 _34;
    f32 _38;
    f32 _3C;
    u32 _40;
    u32 _44;
    u8 _48[4];
    u8 _4C;
    u8 _4D[3];
    s32 _50;
    u32 _54;
    u32 _58;
    u32 _5C;
    u8 _60;
    u8 _61[3];
    TVec3f mGlobalAxis;     // _64
    f32 _70;
    f32 _74;
    f32 _78;
    f32 _7C;
    s32 _80;
    f32 _84;
    f32 _88;
    u32 _8C;
    u32 _90;
    u32 _94;
    u8 _98;
    u8 _99[3];
    TVec3f _9C;
};
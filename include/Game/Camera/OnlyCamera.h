#pragma once

#include "Game/NameObj/NameObj.h"
#include "JSystem/JGeometry/TVec.h"

class CameraPoseParam;

class OnlyCamera : public NameObj {
public:
    OnlyCamera(const char *);
    virtual ~OnlyCamera();

    //calcPose(CameraMan *);
    //calcStartPose(CameraMan *);
    //calcSafePose(CameraMan *);
    //moveToIdealPosition(TVec3f *);

    CameraPoseParam *mPoseParam;    // _C
    u8 _10;
    u8 _11;
    u8 _12[2];
    f32 _14;
    f32 _18;
    f32 _1C;
    f32 _20;
    u8 _24[4];
    u32 _28;
    u8 _2C;
    u8 _2D;
    u8 _2E[2];
    f32 _30;
    f32 _34;
    f32 _38;
    u8 _3C;
    u8 _3D;
    u8 _3E[2];
};
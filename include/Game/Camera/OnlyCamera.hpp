#pragma once

#include "Game/NameObj/NameObj.hpp"
#include "JSystem/JGeometry/TVec.hpp"

class CameraMan;
class CameraPoseParam;

class OnlyCamera : public NameObj {
public:
    OnlyCamera(const char*);
    virtual ~OnlyCamera();

    void calcPose(CameraMan*);
    // calcStartPose(CameraMan *);
    // calcSafePose(CameraMan *);
    // moveToIdealPosition(TVec3f *);

    CameraPoseParam* mPoseParam;  // 0xC
    bool _10;
    bool _11;
    u8 _12[2];
    f32 _14;
    f32 _18;
    f32 _1C;
    f32 _20;
    u8 _24[4];
    u32 _28;
    bool _2C;
    bool _2D;
    u8 _2E[2];
    f32 _30;
    f32 _34;
    f32 _38;
    bool _3C;
    bool _3D;
    u8 _3E[2];
};
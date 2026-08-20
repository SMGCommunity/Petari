#pragma once

#include "Game/NameObj/NameObj.hpp"
#include <JSystem/JGeometry/TVec.hpp>

class CameraMan;
class CameraPoseParam;

class OnlyCamera : public NameObj {
public:
    OnlyCamera(const char*);

    void calcPose(CameraMan*);
    void calcStartPose(CameraMan*);
    void calcSafePose(CameraMan*);
    void moveToIdealPosition(TVec3f*);

    /* 0x0C */ CameraPoseParam* mPoseParam;
    /* 0x10 */ bool mStartPose;
    /* 0x11 */ bool mCalcIdeal;
    /* 0x14 */ TVec3f mPos;
    /* 0x20 */ f32 mSpeed;
    /* 0x24 */ s32 _24;
    /* 0x28 */ u32 _28;
    /* 0x2C */ bool _2C;
    /* 0x2D */ bool _2D;
    /* 0x30 */ TVec3f _30;
    /* 0x3C */ bool mIsZeroFrameMoveOff;
    /* 0x3D */ bool mIsResetting;
};

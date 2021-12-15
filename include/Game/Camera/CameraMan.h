#pragma once

#include "Game/NameObj/NameObj.h"
#include "JSystem/JGeometry/TMatrix.h"

class CameraPoseParam;

class CameraMan : public NameObj {
public:
    CameraMan(const char *);

    u32 _C;
    CameraPoseParam *mPoseParam;    // _10
    u8 _14;
    u8 _15;
    u8 _16[2];
    TMtx34f mMatrix;                // _18
};
#pragma once

#include "revolution.h"
#include "Game/Camera/CameraPoseParam.h"
#include "Game/NameObj/NameObj.h"
#include "JSystem/JGeometry/TMatrix.h"

class Camera : public NameObj {
public:
    Camera(const char *);

    u8 _C[4];
    CameraPoseParam* mPosParam; // _10
    s32 _14;
    u8 _18;
    u8 _19[3];
    TMtx34f mZoneMatrix;        // _1C
};
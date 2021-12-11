#pragma once

#include "revolution.h"
#include "Game/Camera/CameraHeightArrange.h"
#include "Game/Camera/CameraPoseParam.h"
#include "Game/NameObj/NameObj.h"
#include "JSystem/JGeometry/TMatrix.h"

class Camera : public NameObj {
public:
    Camera(const char *);

    void createVPanObj();

    u32 _C;
    CameraPoseParam *mPosParam; // _10
    CameraHeightArrange *mVPan; // _14
    u8 _18;
    u8 _19[3];
    TMtx34f mZoneMatrix;        // _1C
};
#pragma once

#include "Game/NameObj/NameObj.h"
#include "JSystem/JGeometry/TMatrix.h"

class CameraCover : public NameObj {
public:
    CameraCover(const char *);

    TMtx34f _C;
    u32 _3C;
    u8 _40;
    u8 _41;
    u8 _42[2];
    u32 _44;
    void *mCaptureScreenActor;  // _48
};
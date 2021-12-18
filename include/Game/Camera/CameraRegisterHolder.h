#pragma once

#include "Game/NameObj/NameObj.h"
#include "JSystem/JGeometry/TMatrix.h"
#include "JSystem/JGeometry/TVec.h"

class CameraRegisterHolder : public NameObj {
public:
    CameraRegisterHolder(const char *);

    u32 _C;
    u32 _10[64];
    u32 _110[64];
    u32 _210;
    u32 _214[64];
    u32 _314[64];
    u32 _414;
    u32 _418[64];
    u32 _518[64];
    TMtx34f _618;
    TVec3f _648;
    f32 _654;
};
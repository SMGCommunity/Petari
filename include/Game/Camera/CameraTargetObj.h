#pragma once

#include "Game/NameObj/NameObj.h"

class CameraTargetObj : public NameObj {
public:
    CameraTargetObj(const char *);

    u8 _C;
    u8 _D[3];
};
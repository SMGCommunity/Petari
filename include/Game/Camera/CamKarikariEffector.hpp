#pragma once

#include "revolution.h"

class CameraMan;

class CamKarikariEffector {
public:
    CamKarikariEffector();

    void update(CameraMan*);

    /* 0x00 */ s32 mCounter;
};

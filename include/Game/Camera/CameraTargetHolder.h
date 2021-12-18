#pragma once

#include "revolution.h"

class CameraTargetHolder {
public:
    CameraTargetHolder();

    u32 _4;
    void *mTargetActor; // _8
    void *mTargetPlay;  // _C
};
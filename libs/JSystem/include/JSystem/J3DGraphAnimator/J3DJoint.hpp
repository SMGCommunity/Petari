#pragma once

#include <revolution.h>

class J3DJoint {
public:
    void* mJointData;       // 0x0
    void* mJointCallback;   // 0x4
    u8 _8[0x5C-0x8];
};
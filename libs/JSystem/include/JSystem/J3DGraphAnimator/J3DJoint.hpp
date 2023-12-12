#pragma once

#include <revolution.h>

class J3DJoint {
public:
    void* mJointData;       // _0
    void* mJointCallback;   // _4
    u8 _8[0x5C-0x8];
};
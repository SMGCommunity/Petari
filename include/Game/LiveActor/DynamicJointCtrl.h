#pragma once

#include <revolution.h>

class JointCtrlRate {
public:
    JointCtrlRate();

    void update();
    void startCtrl(s32);
    void endCtrl(s32);

    f32 _0;
    u32 _4;
    s32 _8;
    s32 _C;
};
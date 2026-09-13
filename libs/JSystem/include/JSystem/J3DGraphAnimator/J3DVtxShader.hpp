#pragma once

#include <revolution/types.h>

class J3DModel;
class J3DModelData;

class J3DVtxShader {
public:
    J3DVtxShader() : _4() {
    }

    virtual void calc(J3DModel*) = 0;
    virtual void setup(J3DModelData*) {
    }

    virtual ~J3DVtxShader() {
    }

    /* 0x04 */ u32 _4;
};

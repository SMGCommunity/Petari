#pragma once

#include "JSystem/JSupport/JSUIosBase.hpp"

class JSUOutputStream : public JSUIosBase {
public:
    JSUOutputStream() {}

    virtual ~JSUOutputStream();
    virtual s32 skip(s32, s8);

    void write(const void*, s32);
};
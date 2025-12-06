#pragma once

#include "JSystem/JSupport/JSUIosBase.hpp"

class JSUInputStream : public JSUIosBase {
public:
    JSUInputStream() : JSUIosBase() {}

    virtual ~JSUInputStream();
    virtual s32 getAvailable() const = 0;
    virtual s32 skip(s32);
    virtual u32 readData(void*, s32) = 0;

    s32 read(void*, s32);
};
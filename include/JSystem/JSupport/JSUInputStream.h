#pragma once

#include "JSystem/JSupport/JSUIosBase.h"

class JSUInputStream : public JSUIosBase {
public:
    JSUInputStream() {

    }

    virtual ~JSUInputStream();

    virtual s32 getAvailable() const = 0;
    virtual s32 skip(s32);
    virtual u32 readData(void *, s32) = 0;
};
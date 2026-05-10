#pragma once

#include "JSystem/JSupport/JSUIosBase.hpp"

class JSUInputStream : public JSUIosBase {
public:
    JSUInputStream() : JSUIosBase() {
    }

    virtual ~JSUInputStream();
    virtual s32 getAvailable() const = 0;
    virtual s32 skip(s32);
    virtual u32 readData(void*, s32) = 0;

    s32 read(void*, s32);

    inline u8 readU8() {
        u8 ret;
        read(&ret, sizeof(u8));
        return ret;
    }

    inline u16 readU16() {
        u16 ret;
        read(&ret, sizeof(u16));
        return ret;
    }

    inline u32 readU32() {
        u32 ret;
        read(&ret, sizeof(u32));
        return ret;
    }

    // TODO: probably a lot of other helpers for different types
};

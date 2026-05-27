#pragma once

#include "JSystem/JSupport/JSUIosBase.hpp"

class JSUOutputStream : public JSUIosBase {
public:
    JSUOutputStream() {
    }

    virtual ~JSUOutputStream();
    virtual s32 skip(s32, s8);

    void write(const void*, s32);

    inline void writeU8(u8 val) {
        write(&val, sizeof(u8));
    }

    inline void writeU16(u16 val) {
        write(&val, sizeof(u16));
    }

    inline void writeU32(u32 val) {
        write(&val, sizeof(u32));
    }

    // TODO: probably a lot of other helpers for different types
};

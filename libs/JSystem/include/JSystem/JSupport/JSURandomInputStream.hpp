#pragma once

#include "JSystem/JSupport/JSUInputStream.hpp"

class JSURandomInputStream : public JSUInputStream {
public:
    JSURandomInputStream() {}

    virtual ~JSURandomInputStream();

    virtual s32 getAvailable() const { return getLength() - getPosition(); }

    virtual s32 skip(s32);
    virtual u32 readData(void*, s32) = 0;
    virtual s32 getLength() const = 0;
    virtual s32 getPosition() const = 0;
    virtual s32 seekPos(s32, JSUStreamSeekFrom) = 0;

    s32 seek(s32, JSUStreamSeekFrom);
};

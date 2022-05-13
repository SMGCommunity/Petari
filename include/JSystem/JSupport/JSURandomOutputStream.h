#pragma once

#include "JSystem/JSupport/JSUOutputStream.h"

class JSURandomOutputStream : public JSUOutputStream {
public:
    virtual s32 writeData(const void *, s32) = 0;
    virtual s32 getLength() const = 0;
    virtual s32 getPosition() const = 0;
    virtual void seek(s32, JSUStreamSeekFrom);
    virtual s32 getAvailable() const;
};
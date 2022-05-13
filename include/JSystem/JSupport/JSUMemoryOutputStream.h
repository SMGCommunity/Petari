#pragma once

#include "JSystem/JSupport/JSURandomOutputStream.h"

class JSUMemoryOutputStream : public JSURandomOutputStream {
public:
    JSUMemoryOutputStream() {

    }

    virtual ~JSUMemoryOutputStream();
    virtual s32 writeData(const void *, s32);
    virtual s32 getLength() const;
    virtual s32 getPosition() const;
    virtual s32 seekPos(s32, JSUStreamSeekFrom);

    void setBuffer(void *, s32);

    void* mBuffer;  // _8
    s32 mLength;    // _C
    s32 mPosition;  // _10
};
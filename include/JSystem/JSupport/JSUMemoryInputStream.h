#pragma once

#include "JSystem/JSupport/JSURandomInputStream.h"

class JSUMemoryInputStream : public JSURandomInputStream  {
public:
    JSUMemoryInputStream() : JSURandomInputStream() {
        
    }

    virtual ~JSUMemoryInputStream();

    virtual u32 readData(void *, s32);
    virtual s32 getLength() const;
    virtual s32 getPosition() const;
    virtual s32 seekPos(s32, JSUStreamSeekFrom);

    void setBuffer(const void *, s32);

    const void* mBuffer;    // _8
    s32 mLength;            // _C
    s32 mPosition;          // _10
};
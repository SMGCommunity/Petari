#pragma once

#include "JSystem/JSupport/JSURandomOutputStream.hpp"

class JSUMemoryOutputStream : public JSURandomOutputStream {
public:
    inline JSUMemoryOutputStream() {
        
    }

    virtual ~JSUMemoryOutputStream();
    virtual s32 writeData(const void *, s32);
    virtual s32 getLength() const;
    virtual s32 getPosition() const;
    virtual s32 seekPos(s32, JSUStreamSeekFrom);

    void setBuffer(void *, s32);

    void* mBuffer;  // 0x8
    s32 mLength;    // 0xC
    s32 mPosition;  // 0x10
};

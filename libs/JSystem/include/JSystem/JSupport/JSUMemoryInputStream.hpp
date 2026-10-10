#pragma once

#include "JSystem/JSupport/JSURandomInputStream.hpp"

class JSUMemoryInputStream : public JSURandomInputStream {
public:
    JSUMemoryInputStream(const void* pBuffer, s32 size) : JSURandomInputStream() {
        setBuffer(pBuffer, size);
    }

    virtual ~JSUMemoryInputStream() {};
    virtual u32 readData(void*, s32);
    virtual s32 getLength() const;
    virtual s32 getPosition() const;
    virtual s32 seekPos(s32, JSUStreamSeekFrom);

    void setBuffer(const void*, s32);

    /* 0x08 */ const void* mBuffer;
    /* 0x0C */ s32 mLength;
    /* 0x10 */ s32 mPosition;
};

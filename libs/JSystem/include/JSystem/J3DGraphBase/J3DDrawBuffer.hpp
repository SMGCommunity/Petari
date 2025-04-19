#pragma once

#include <revolution.h>

class J3DDrawBuffer {
public:
    inline J3DDrawBuffer() {
        initialize();
        allocBuffer(1);
    }

    void initialize();
    void allocBuffer(u32);

    void frameInit();

    void draw() const;

    void* mBuffer;          // 0x00
    u32 mBufferSize;        // 0x04
    u32 _8;
    u32 _C;
    f32 _10;
    f32 _14;
    u32 _18;
    u32 _1C;
    u32 _20;
};

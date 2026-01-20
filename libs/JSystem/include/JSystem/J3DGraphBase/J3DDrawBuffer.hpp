#pragma once

#include <revolution.h>

class J3DMatPacket;

class J3DDrawBuffer {
public:
    typedef int (J3DDrawBuffer::*sortFunc)(J3DMatPacket*);
    typedef void (J3DDrawBuffer::*drawFunc)() const;

    inline J3DDrawBuffer() {
        initialize();
        allocBuffer(1);
    }

    void initialize();
    void allocBuffer(u32);

    void frameInit();

    void draw() const;

    int getSortMode() { return mSortMode; }

    void* mBuffer;    // 0x00
    u32 mBufferSize;  // 0x04
    u32 _8;
    u32 mSortMode;
    f32 _10;
    f32 _14;
    u32 _18;
    u32 _1C;
    u32 _20;

    static sortFunc sortFuncTable[6];
    static drawFunc drawFuncTable[2];
    static int entryNum;
};

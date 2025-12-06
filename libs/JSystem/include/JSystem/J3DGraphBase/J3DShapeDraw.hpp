#pragma once

#include <revolution.h>

class J3DShapeDraw {
public:
    J3DShapeDraw(const u8*, u32);

    u32 countVertex(u32);
    void addTexMtxIndexInDL(u32, u32, u32);
    void draw() const;

    virtual ~J3DShapeDraw();

    /* 0x04 */ u32 mBufferSize;
    /* 0x08 */ u8* mBuffer;
};

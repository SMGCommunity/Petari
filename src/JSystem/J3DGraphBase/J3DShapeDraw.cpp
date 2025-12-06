#include "JSystem/J3DGraphBase/J3DShapeDraw.hpp"
#include "JSystem/JKernel/JKRHeap.hpp"
#include <cstring>
#include <revolution/gx/GXDispList.h>

u32 J3DShapeDraw::countVertex(u32 dataSize) {
    // nonmatching: shenanigans afoot.
    // This code as-is matches except for the obvious nonmatch of li r3,0 (corresponding to the count = 0 line).
    // However doing the obvious fix will obliterate the rest of the function.
    // https://decomp.me/scratch/9Omoe

    u32 count;  // = 0;  // BEWARE THE HYDRA!
    u8* readPtr = mBuffer;

    u16 numData;
    for (; readPtr - mBuffer < mBufferSize; readPtr = readPtr + dataSize * numData + 3) {
        u8 tag = *readPtr;
        if (tag != 0xA0 && tag != 0x98) {
            break;
        }

        numData = *reinterpret_cast< u16* >(readPtr + 1);
        count = count + numData;
    }
    return count;
}

void J3DShapeDraw::addTexMtxIndexInDL(u32 dataSize, u32 splitSize, u32 paramC) {
    // Nonmatching: register allocation issues. Otherwise logically matching.
    // https://decomp.me/scratch/XD4No

    u32 size = mBufferSize + countVertex(dataSize) + 0x1F & ~0x1F;  // pad to 32 bytes

    u8* writeBuffer = new (0x20) u8[size];

    u8* bufferStart = mBuffer;
    u8* readPtr = bufferStart;
    u8* writePtr = writeBuffer;
    s32 numData;

    for (; readPtr - bufferStart < mBufferSize; readPtr = readPtr + dataSize * numData + 3) {
        u8 tag = *readPtr;
        *(writePtr++) = tag;

        if (tag != 0xA0 && tag != 0x98) {
            break;
        }

        numData = *reinterpret_cast< u16* >(readPtr + 1);
        *(reinterpret_cast< u16* >(writePtr)) = numData;
        writePtr += 2;

        for (s32 i = 0; i < numData; i++) {
            u8 d = readPtr[dataSize * i + 3];
            u8* dataOffset = readPtr + dataSize * i + 3;
            memcpy(writePtr, dataOffset, splitSize);

            u8* offset2 = writePtr + splitSize;
            *(offset2++) = paramC + d;
            memcpy(offset2, dataOffset + splitSize, dataSize - splitSize);

            writePtr = offset2 + (dataSize - splitSize);
        }
    }

    u32 bufferSize = writePtr - writeBuffer + 0x1F & ~0x1F;  // pad to 32 bytes

    for (; writePtr - writeBuffer < size; writePtr++) {
        *writePtr = 0;
    }

    mBufferSize = bufferSize;
    mBuffer = writeBuffer;
    DCStoreRange(mBuffer, mBufferSize);
}

J3DShapeDraw::J3DShapeDraw(const u8* b, u32 c) {
    mBuffer = const_cast< u8* >(b);
    mBufferSize = c;
}

void J3DShapeDraw::draw() const {
    GXCallDisplayList(mBuffer, mBufferSize);
}

J3DShapeDraw::~J3DShapeDraw() {}

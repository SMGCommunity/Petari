#pragma once

#include <revolution.h>

class BinaryDataChunkBase {
public:
    virtual u32 makeHeaderHashCode() const = 0;
    virtual u32 getSignature() const = 0;
    virtual s32 serialize(u8*, u32) const = 0;
    virtual s32 deserialize(const u8*, u32) = 0;
    virtual void initializeData() = 0;
};

class BinaryDataChunkHolder {
public:
    BinaryDataChunkHolder(u32, int);

    void addChunk(BinaryDataChunkBase*);
    s32 makeFileBinary(u8*, u32);
    bool loadFromFileBinary(const u8*, u32);
    BinaryDataChunkBase* findFromSignature(u32) const;

    BinaryDataChunkBase** mChunks;  // 0x0
    s32 mMaxChunks;                 // 0x4
    s32 mNumChunks;                 // 0x8
    u8* mData;                      // 0xC
    u32 _10;
};

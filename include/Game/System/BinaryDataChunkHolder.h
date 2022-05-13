#pragma once

#include <revolution.h>

class BinaryDataChunkBase {
public:
    virtual u32 makeHeaderHashCode() const = 0;
    virtual u32 getSignature() const = 0;
    virtual s32 serialize(u8 *, u32) const = 0;
    virtual s32 deserialize(const u8 *, u32) = 0;
    virtual void initializeData() = 0;
};

class BinaryDataChunkHolder {
public:
    BinaryDataChunkHolder(u32, int);

    void loadFromFileBinary(const u8 *, u32);
    void makeFileBinary(u8 *, u32);

    void addChunk(BinaryDataChunkBase *);

    BinaryDataChunkBase* findFromSignature(u32) const;

    BinaryDataChunkBase** mChunks;  // _0
    s32 mMaxChunks;                 // _4
    s32 mNumChunks;                 // _8
    u8* mData;                      // _C
    u32 _10;
};

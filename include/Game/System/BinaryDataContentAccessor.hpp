#pragma once

#include <JSystem/JSupport/JSUMemoryOutputStream.hpp>

class BinaryDataContentHeaderSerializer {
public:
    BinaryDataContentHeaderSerializer(u8*, u32);

    void addAttribute(const char*, u32);
    void flush();
    u32 getHeaderSize() const;
    u32 getDataSize() const;

    /* 0x00 */ JSUMemoryOutputStream mStream;
    /* 0x14 */ u32 mAttributeNum;
    /* 0x18 */ u32 mDataSize;
};

class BinaryDataContentAccessor {
public:
    BinaryDataContentAccessor(u8*);

    u32 getHeaderSize() const;
    u32 getDataSize() const;
    void* getPointer(const char*, u8*) const;
};

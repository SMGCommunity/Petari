#pragma once

#include <revolution/types.h>

struct SomeStruct {
    u32 _0;
    u32 _4;
};

class BinaryDataContentHeaderSerializer {
public:
    BinaryDataContentHeaderSerializer(u8 *, u32);

    void addAttribute(const char *, u32);
    void flush();
    u32 getHeaderSize() const;
    u32 getDataSize() const;

    u8 _0[0x1C];
};

class BinaryDataContentAccessor {
public:
    BinaryDataContentAccessor(u8 *);

    u32 getHeaderSize() const;
    u32 getDataSize() const;
    SomeStruct* getPointer(const char *, u8 *) const;
};
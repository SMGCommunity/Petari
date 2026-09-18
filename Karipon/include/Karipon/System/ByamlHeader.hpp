#pragma once

#include "revolution/types.h"

class ByamlHeader {
public:
    u16 getTag() const { return mTag; }
    u16 getVersion() const { return mVersion; }
    u32 getHashKeyTableOffset() const { return mHashKeyOffset; }
    u32 getStringTableOffset() const { return mStringTableOffset; }
    u32 getDataOffset() const { return mDataOffset; }

    u16 mTag;
    u16 mVersion;
    s32 mHashKeyOffset;
    s32 mStringTableOffset;
    s32 mDataOffset;
};

class ByamlStringTableIter {
public:
    ByamlStringTableIter() : mData(nullptr) {}
    ByamlStringTableIter(const u8* pData) : mData(pData) {}

    s32 getSize() const { return *reinterpret_cast<const u32*>(mData) & 0xFFFFFF; }
    const u32* getAddressTable() const { return reinterpret_cast<const u32*>(mData + 4); }
    u32 getStringAddress(s32 index) const { return getAddressTable()[index]; }
    const char* getString(s32 index) const { return reinterpret_cast<const char*>(&mData[getStringAddress(index)]); }
    s32 findStringIndex(const char* pStr) const;
    bool isValidate() const { return mData != nullptr; }

    const u8* mData;
};
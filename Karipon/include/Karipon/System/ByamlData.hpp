#pragma once

#include "revolution/types.h"

class ByamlHashPair;

enum ByamlDataType {
    BYAML_TYPE_INVALID = 0,
    BYAML_TYPE_STRING = 0xA0,
    BYAML_TYPE_BINARY = 0xA1,
    BYAML_TYPE_ARRAY = 0xC0,
    BYAML_TYPE_HASH = 0xC1,
    BYAML_TYPE_STRING_TABLE = 0xC2,
    BYAML_TYPE_BOOL = 0xD0,
    BYAML_TYPE_INT = 0xD1,
    BYAML_TYPE_FLOAT = 0xD2,
    BYAML_TYPE_UINT = 0xD3,
    BYAML_TYPE_LONG = 0xD4,
    BYAML_TYPE_ULONG = 0xD5,
    BYAML_TYPE_DOUBLE = 0xD6,
    BYAML_TYPE_NULL = 0xFF
};

class ByamlHashPair {
public:
    s32 getKey() const { return mData >> 8; }
    ByamlDataType getType() const { return static_cast<ByamlDataType>(mData & 0xFF); }
    s32 getValue() const { return mValue; }

    const s32 mData;
    const s32 mValue;
};

class ByamlData {
public:
    ByamlData() : mValue(0), mType(BYAML_TYPE_INVALID) {}

    void set(const ByamlHashPair* pPair) {
        mType = pPair->getType();
        mValue = pPair->getValue();
    }
    void set(ByamlDataType type, u32 val) {
        mType = type;
        mValue = val;
    }
    ByamlDataType getType() const { return mType; }
    u32 getValue() const { return mValue; }

    template <typename T>
    T getValue() const {
        u32 val = getValue();
        return *reinterpret_cast<const T*>(&val);
    }

    u32 mValue;
    ByamlDataType mType;
};

class ByamlHashIter {
public:
    ByamlHashIter() : mData(nullptr) {}
    ByamlHashIter(const u8* pData) : mData(pData) {}

    const ByamlHashPair* findPair(s32 key) const;
    bool getDataByIndex(ByamlData* pData, s32 index) const;
    bool getDataByKey(ByamlData* pData, s32 key) const;
    const u8* getOffsetData(u32 off) const { return &mData[off]; }
    const ByamlHashPair* getPairByIndex(s32 index) const;
    const ByamlHashPair* getPairTable() const;
    u32 getSize() const;

    const u8* mData;
};

class ByamlArrayIter {
public:
    ByamlArrayIter() : mData(nullptr) {}
    ByamlArrayIter(const u8* pData) : mData(pData) {}

    bool getDataByIndex(ByamlData* pData, s32 index) const;
    const u32* getDataTable() const;
    const u8* getOffsetData(u32 off) const { return &mData[off]; }
    u32 getSize() const { return *reinterpret_cast<const u32*>(mData) & 0xFFFFFF; }
    const u8* getTypeTable() const { return mData + 4; }

    const u8* mData;
};
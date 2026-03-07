#pragma once

#include <cstring>
#include <revolution.h>

#define JMAP_VALUE_TYPE_LONG 0
#define JMAP_VALUE_TYPE_STRING 1
#define JMAP_VALUE_TYPE_FLOAT 2
#define JMAP_VALUE_TYPE_LONG_2 3
#define JMAP_VALUE_TYPE_SHORT 4
#define JMAP_VALUE_TYPE_BYTE 5
#define JMAP_VALUE_TYPE_STRING_PTR 6
#define JMAP_VALUE_TYPE_NULL 7

class JMapInfoIter;

struct JMapItem {
    u32 mHash;      // 0x0
    u32 mMask;      // 0x4
    u16 mOffsData;  // 0x8
    u8 mShift;      // 0xA
    u8 mType;       // 0xB
};

struct JMapData {
    s32 mNumEntries;          // 0x0
    s32 mNumFields;           // 0x4
    s32 mDataOffset;          // 0x8
    u32 mEntrySize;           // 0xC
    const JMapItem mItems[];  // 0x10
};

template < typename T >
inline bool compareValues(const T a, const T b) {
    return a == b;
}

template <>
inline bool compareValues< const char* >(const char* a, const char* b) {
    return strcmp(a, b) == 0;
}

inline const char* getEntryAddress(const JMapData* pData, s32 dataOffset, int entryIndex) {
    return reinterpret_cast< const char* >(pData) + dataOffset + entryIndex * pData->mEntrySize;
}

class JMapInfo {
public:
    JMapInfo();
    ~JMapInfo();

    inline JMapInfo& operator=(const JMapInfo& rInfo) {
        mData = rInfo.mData;
        mName = rInfo.mName;
        return *this;
    }

    inline bool operator==(const JMapInfo& rInfo) const { return mData == rInfo.mData; }

    inline bool dataExists() const { return !!mData; }

    inline int getNumEntries() const { return dataExists() ? mData->mNumEntries : 0; }

    inline int getNumFields() const { return dataExists() ? mData->mNumFields : 0; }

    bool attach(const void*);
    void setName(const char*);
    const char* getName() const;
    s32 searchItemInfo(const char*) const;
    s32 getValueType(const char*) const;
    bool getValueFast(int, int, const char**) const;
    bool getValueFast(int, int, u32*) const;
    bool getValueFast(int, int, s32*) const;
    bool getValueFast(int entryIndex, int itemIndex, f32* pValueOut) const {
        const JMapItem* pItem = &mData->mItems[itemIndex];
        const char* pValue = getEntryAddress(mData, mData->mDataOffset, entryIndex) + pItem->mOffsData;
        *pValueOut = *reinterpret_cast< const f32* >(pValue);
        return true;
    }
    bool getValueFast(int entryIndex, int itemIndex, bool* pValueOut) const {
        const JMapItem* pItem = &mData->mItems[itemIndex];
        const char* pValue = getEntryAddress(mData, mData->mDataOffset, entryIndex) + pItem->mOffsData;
        *pValueOut = (*reinterpret_cast< const u32* >(pValue) & pItem->mMask) != 0;
        return true;
    }

    JMapInfoIter findElementBinary(const char*, const char*) const;

    template < typename T >
    const bool getValue(int entryIndex, const char* pKey, T* pValueOut) const {
        s32 itemIndex = searchItemInfo(pKey);
        if (itemIndex < 0) {
            return false;
        }
        return getValueFast(entryIndex, itemIndex, pValueOut);
    }

    template < typename T >
    JMapInfoIter findElement(const char* pKey, T searchValue, int startIndex) const NO_INLINE {
        int entryIndex = startIndex;
        T value;
        while (entryIndex < getNumEntries()) {
            getValue< T >(entryIndex, pKey, &value);
            if (compareValues< T >(value, searchValue)) {
                return JMapInfoIter(this, entryIndex);
            }
            entryIndex++;
        }
        return end();
    }

    // I believe this function should be implemented within this header,
    // but due to a circular dependency on JMapInfoIter, I have left
    // it in StageDataHolder.cpp (its current split) for now
    JMapInfoIter end() const;

    const JMapData* mData;  // 0x0
    const char* mName;      // 0x4
};

class JMapInfoIter {
public:
    inline JMapInfoIter() {}

    inline JMapInfoIter(const JMapInfo* pInfo, s32 index) {
        mInfo = pInfo;
        mIndex = index;
    }

    inline JMapInfoIter& operator=(const JMapInfoIter& rIter) {
        mInfo = rIter.mInfo;
        mIndex = rIter.mIndex;
        return *this;
    }

    bool operator==(const JMapInfoIter& rIter) const { return mIndex == rIter.mIndex && mInfo && rIter.mInfo && *mInfo == *rIter.mInfo; }

    bool operator!=(const JMapInfoIter& rIter) const { return !(*this == rIter); }

    bool isValid() const { return mInfo && mIndex >= 0 && mIndex < mInfo->getNumEntries(); }

    template < typename T >
    bool getValue(const char* pKey, T* pValueOut) const {
        return mInfo->getValue(mIndex, pKey, pValueOut);
    }

    const JMapInfo* mInfo;  // 0x0
    s32 mIndex;             // 0x4
};

namespace MR {
    JMapInfoIter findJMapInfoElementNoCase(const JMapInfo*, const char*, const char*, int);
}

#pragma once

#include <revolution.h>

#define JMAP_VALUE_TYPE_LONG 0
#define JMAP_VALUE_TYPE_STRING 1
#define JMAP_VALUE_TYPE_FLOAT 2
#define JMAP_VALUE_TYPE_LONG_2 3
#define JMAP_VALUE_TYPE_SHORT 4
#define JMAP_VALUE_TYPE_BYTE 5
#define JMAP_VALUE_TYPE_STRING_PTR 6
#define JMAP_VALUE_TYPE_NULL 7

class JMapInfo;

class JMapInfoIter {
public:
    template<typename T>
    bool getValue(const char *, T *) const;

    bool isValid() const;

    bool operator==(const JMapInfoIter &) const;

    JMapInfo* mInfo; // _0
    s32 _4;
};


struct JMapItem {
    u32 mHash;      // _0
    u32 mMask;      // _4
    u16 mOffsData;  // _8
    u8 mShift;      // _A
    u8 mType;       // _B
};

struct JMapData {
    s32 mNumEntries;    // _0
    s32 mNumFields;     // _4
    s32 mDataOffset;    // _8
    u32 mEntrySize;     // _C
    const JMapItem mItems;   // _10
};

class JMapInfo {
public:
    JMapInfo();
    ~JMapInfo();

    bool attach(const void *);
    void setName(const char *pName);
    const char* getName() const;
    s32 searchItemInfo(const char *pItem) const;
    s32 getValueType(const char *) const;

    template<typename T>
    JMapInfoIter findElement(const char *, const char *, int) const;

    template<typename T>
    JMapInfoIter findElement(const char *, s32, int) const;
    JMapInfoIter end() const;

    template<typename T>
    const bool getValue(int, const char *, T *) const;

    bool getValueFast(int, int, const char **) const;
    bool getValueFast(int, int, unsigned long *) const;
    bool getValueFast(int, int, long *) const;

    const JMapData* mData; // _0
    const char* mName; // _4
};
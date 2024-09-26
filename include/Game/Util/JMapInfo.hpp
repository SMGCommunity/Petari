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

class JMapInfoIter;

struct JMapItem {
    u32 mHash;      // 0x0
    u32 mMask;      // 0x4
    u16 mOffsData;  // 0x8
    u8 mShift;      // 0xA
    u8 mType;       // 0xB
};

struct JMapData {
    s32 mNumEntries;    // 0x0
    s32 mNumFields;     // 0x4
    s32 mDataOffset;    // 0x8
    u32 mEntrySize;     // 0xC
    const JMapItem mItems;   // 0x10
};

class JMapInfo {
public:
    JMapInfo();
    ~JMapInfo();

    inline JMapInfo& operator=(const JMapInfo &rhs) {
        mData = rhs.mData;
        mName = rhs.mName;
        return *this;
    }

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

    const JMapData* mData; // 0x0
    const char* mName; // 0x4
};

class JMapInfoIter {
public:
    inline JMapInfoIter() { }

    inline JMapInfoIter(JMapInfo* pInfo, s32 val) {
        mInfo = pInfo;
        _4 = val;
    }

    inline JMapInfoIter& operator=(const JMapInfoIter &rIter) {
        mInfo = rIter.mInfo;
        _4 = rIter._4;
        return *this;
    }

    template<typename T>
    bool getValue(const char *, T *) const;

    bool isValid() const NO_INLINE{
        bool valid = false;
        bool v3 = false;
        JMapInfo* info = mInfo;
        if (info && _4 >= 0) {
            v3 = true;
        }

        if (v3) {        
            s32 pos = _4;
            s32 num;
            if (info->mData) {
                num = info->mData->mNumEntries;
            }
            else {
                num = 0;
            }

            if (pos < num) {
                valid = true;
            }
        }

        return valid;
    }

    

    bool operator==(const JMapInfoIter &) const;

    JMapInfo* mInfo; // 0x0
    s32 _4;
};
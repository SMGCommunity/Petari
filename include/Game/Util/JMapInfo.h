#pragma once

#include <revolution.h>
#include "Game/Util.h"

class JMapInfo;

class JMapInfoIter {
public:
    template<typename T>
    bool getValue(const char *, T *) const;

    bool isValid() const;

    bool operator==(const JMapInfoIter &) const;

    JMapInfo* mInfo; // _0
    u32 _4;
};


struct JMapItem {
    u32 mHash;      // _0
    u32 mMask;      // _4
    u16 mOffsData;  // _8
    u8 mShift;      // _A
    u8 mType;       // _B
};

struct JMapData {
    s32 _0;
    s32 mNumData;       // _4
    s32 mDataOffset;    // _8
    u32 _C;
    JMapItem* mItems;   // _10
};

class JMapInfo {
public:
    JMapInfo();
    ~JMapInfo();

    void attach(const void *);
    s32 searchItemInfo(const char *) const;
    s32 getValueType(const char *) const;

    const char* getName() const;

    template<typename T>
    JMapInfoIter findElement(const char *, const char *, int) const;
    JMapInfoIter end() const;

    template<typename T>
    const bool getValue(int, const char *, T *) const;

    bool getValueFast(int, int, const char **) const;

    const JMapData* mData; // _0
    const char* mName; // _4
};
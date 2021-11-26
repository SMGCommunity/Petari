#pragma once

#include <revolution.h>

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

    bool getValueFast(int, int, const char **) const;

    const JMapData* mData; // _0
    const char* mName; // _4
};
#pragma once

#include "Game/System/ResourceInfo.hpp"
#include "Game/Util/HashUtil.hpp"
#include <revolution/types.h>

class ResourceHolder;

class XanimeSwapTable {
public:
    /* 0x0 */ const char* mOriginal;
    /* 0x4 */ const char* mSwapped;
};

class XanimeBckTable {
public:
    const char* animationName;
};

// Size is 0x14
class XanimeOfsInfo : public XanimeBckTable {
public:
    f32 _4;
    f32 _8;
    f32 _C;
    u32 _10;
};

class XanimeGroupInfo {
public:
    XanimeBckTable mParent;  // 0x0
    f32 _4;
    u32 _8;
    f32 _C;
    f32 _10;
    f32 _14;
    u32 _18;
    /* 0x1C */ u8 mBckTableVariant;
    u8 _1D;
    const char* _20[4];
    f32 _30[4];
    XanimeOfsInfo** _40;
    /* 0x44 */ u32 mHash;
    const char* _48;

    void init();
};

class XanimeDirectory {
public:
    enum DIRECTORY_TYPE { Recursive = 0, Leaf = 1 };

    /* 0x0 */ const char* mName;
    /* 0x4 */ u8 mDirectoryType;
    union {
        /* 0x8 */ XanimeDirectory* mSubDirectories;
        /* 0x8 */ XanimeGroupInfo* mSubInformations;
    };
    /* 0xC */ u32 mHash;
    /* 0x10 */ u32 mSize;
};

class XanimeBckTable1 : public XanimeBckTable {
public:
    const char* mFileName;  // 0x4
    u32 mAnimationHash;     // 0x8
    u32 mFileHash;          // 0xC
};

class XanimeBckTable2 : public XanimeBckTable {
public:
    const char* mFileName1;  // 0x4
    f32 _8;

    const char* mFileName2;  // 0xC
    f32 _10;
};

class XanimeBckTable3 : public XanimeBckTable {
public:
    const char* mFileName1;  // 0x4
    f32 _8;

    const char* mFileName2;  // 0xC
    f32 _10;

    const char* mFileName3;  // 0x14;
    f32 _18;
};

class XanimeBckTable4 : public XanimeBckTable {
public:
    const char* mFileName1;  // 0x4
    f32 _8;

    const char* mFileName2;  // 0xC
    f32 _10;

    const char* mFileName3;  // 0x14;
    f32 _18;

    const char* mFileName4;  // 0x1C;
    f32 _20;
};

// size is 0x18
class XanimeAuxInfo : public XanimeBckTable {
public:
    u8 _4;
    XanimeOfsInfo* _8;

    // Unreferenced members
    u8 _C[12];
};

class XanimeResourceTable {
public:
    XanimeResourceTable(

        ResourceHolder*,

        XanimeGroupInfo*, XanimeAuxInfo*, XanimeOfsInfo*,

        XanimeBckTable*, XanimeBckTable2*, XanimeBckTable3*, XanimeBckTable4*,

        XanimeSwapTable*);

    XanimeResourceTable(ResourceHolder*);

    u32 initGroupInfo(ResourceHolder*, XanimeGroupInfo*, XanimeAuxInfo*, XanimeOfsInfo*, XanimeBckTable*, XanimeBckTable2*, XanimeBckTable3*,
                      XanimeBckTable4*, XanimeSwapTable*);

    const XanimeGroupInfo* getGroupInfo(const char*) const;
    const XanimeGroupInfo* getGroupInfo(const char*, XanimeDirectory*) const;
    XanimeGroupInfo* getGroupInfoFromHash(u32) const;
    u32 getIndex(XanimeDirectory*, const char*) const;
    u32 getIndexFromHash(u32) const;
    u32 getGroupIndex(const char*) const;
    u32 getSingleIndex(const char*) const;
    u32 getSimpleIndex(const char*) const;
    const char* swapBckName(const char*, XanimeSwapTable*) const;
    char* findResMotion(const char*) const;
    char* findStringMotion(const char*) const;
    void createSortTable();
    bool search(XanimeBckTable**, const char*, u32) const;
    void init();

    u8 _0;
    /* 0x1 */ u8 mMaxGroupInfoTableSize;
    /* 0x4 */ u32 mAmountOfGroupInfos;
    /* 0x8 */ u32 mAmountOfBckTables;
    /* 0xC */ u32 mAmountOfSimpleGroupInfos;
    /* 0x10 */ XanimeGroupInfo* mGroupInfos;
    /* 0x14 */ XanimeDirectory* mDirectories;
    /* 0x18 */ XanimeBckTable1* mBckTables;
    XanimeGroupInfo _1C;
    /* 0x68 */ XanimeGroupInfo* mSimpleGroupInfos;
    /* 0x6C */ ResourceHolder* mResourceHolder;
    /* 0x70 */ HashSortTable* mSortTable;
    /* 0x74 */ XanimeSwapTable* mSwapTable;
};

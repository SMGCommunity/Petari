#pragma once

#include "Game/System/ResourceInfo.hpp"
#include "Game/Util/HashUtil.hpp"
#include <revolution/types.h>

class ResourceHolder;

class XanimeSwapTable {
public:
    const char* original;
    const char* swapped;
};

class XanimeBckTable {
public:
    const char* animationName;
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
    u8 _1C;
    u8 _1D;
    u32 _20[4];
    f32 _30[4];
    u32 _40;
    u32 mHash;
    const char* _48;

    void init();
};

class XanimeDirectory {
public:
    enum DIRECTORY_TYPE { Recursive = 0, Leaf = 1 };

    /* 0x0 */ const char* mName;
    /* 0x4 */ u8 mDirectoryType;
    /* 0x8 */ union {
        XanimeDirectory* mSubDirectories;
        XanimeGroupInfo* mSubInformations;
    };
    /* 0xC */ u32 mHash;
    /* 0x10 */ union {
        XanimeGroupInfo* _10;
        u32 mSize;
    };
    // Both unions are possibly linked
};

class XanimeSingleBckTable {
public:
    XanimeBckTable parent;  // 0x0

    const char* fileName;  // 0x4
    u32 animationHash;     // 0x8
    u32 fileHash;          // 0xC
};

class XanimeDoubleBckTable {
public:
    XanimeBckTable parent;  // 0x0

    const char* fileName1;  // 0x4
    f32 _8;

    const char* fileName2;  // 0xC
    f32 _10;
};

class XanimeTripleBckTable {
public:
    XanimeBckTable parent;  // 0x0

    const char* fileName1;  // 0x4
    f32 _8;

    const char* fileName2;  // 0xC
    f32 _10;

    const char* fileName3;  // 0x14;
    f32 _18;
};

class XanimeQuadBckTable {
public:
    XanimeBckTable parent;  // 0x0

    const char* fileName1;  // 0x4
    f32 _8;

    const char* fileName2;  // 0xC
    f32 _10;

    const char* fileName3;  // 0x14;
    f32 _18;

    const char* fileName4;  // 0x1C;
    f32 _20;
};

// Size is 0x14
class XanimeOfsInfo {
public:
    XanimeBckTable parent;  // 0x0

    f32 _4;
    f32 _8;
    f32 _C;
    u32 _10;
};

// size is 0x18
class XanimeAuxInfo {
public:
    XanimeBckTable parent;  // 0x0

    u8 _4;
    u32 _8;

    // Unreferenced members
    u8 _C[12];
};

class XanimeResourceTable {
public:
    XanimeResourceTable(

        ResourceHolder*,

        XanimeGroupInfo*, XanimeAuxInfo*, XanimeOfsInfo*,

        XanimeBckTable*, XanimeBckTable*, XanimeBckTable*, XanimeBckTable*,

        XanimeSwapTable*);

    XanimeResourceTable(ResourceHolder*);

    u32 initGroupInfo(ResourceHolder*, XanimeGroupInfo*, XanimeAuxInfo*, XanimeOfsInfo*, XanimeBckTable*, XanimeBckTable*, XanimeBckTable*,
                      XanimeBckTable*, XanimeSwapTable*);

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
    u8 _1;
    /* 0x4 */ u32 m_10Size;
    /* 0x8 */ u32 m_14Size;
    /* 0xC */ u32 m_68Size;
    XanimeGroupInfo* _10;
    XanimeDirectory* _14;
    XanimeSingleBckTable* _18;
    XanimeGroupInfo _1C;
    XanimeGroupInfo* _68;
    /* 0x6C */ ResourceHolder* mResourceHolder;
    /* 0x70 */ HashSortTable* mSortTable;
    /* 0x74 */ XanimeSwapTable* mSwapTable;
};

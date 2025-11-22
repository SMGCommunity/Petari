#pragma once

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
    u32 _44;
    const char* _48;

    void init();
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

    const XanimeGroupInfo* getGroupInfo(const char*) const;

    u8 _0[0x78];
};

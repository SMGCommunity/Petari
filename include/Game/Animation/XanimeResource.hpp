#pragma once

#include <revolution/types.h>

class XanimeSwapTable {
public:
    const char *original;
    const char *swapped;
};

class XanimeGroupInfo {
public:
    const char *_0;
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
    const char *_48;

    void init();
};

class XanimeBckTable {
public:
    const char *animationName;
};

class XanimeSingleBckTable {
public:
    XanimeBckTable parent; // _0
    
    const char *fileName; // _4
    u32 animationHash; // _8
    u32 fileHash; // _C
};

class XanimeDoubleBckTable {
public:
    XanimeBckTable parent; // _0
    
    const char *fileName1; // _4
    f32 _8;
    
    const char *fileName2; // _C
    f32 _10;
};

class XanimeTripleBckTable {
public:
    XanimeBckTable parent; // _0
    
    const char *fileName1; // _4
    f32 _8;
    
    const char *fileName2; // _C
    f32 _10;

    const char *fileName3; // _14;
    f32 _18;
};

class XanimeQuadBckTable {
public:
    XanimeBckTable parent; // _0
    
    const char *fileName1; // _4
    f32 _8;
    
    const char *fileName2; // _C
    f32 _10;

    const char *fileName3; // _14;
    f32 _18;
    
    const char *fileName4; // _1C;
    f32 _20;
};

// Size is 0x14
class XanimeOfsInfo {
public:
    XanimeBckTable parent; // _0
    
    f32 _4;
    f32 _8;
    f32 _C;
    u32 _10;
};

//size is 0x18
class XanimeAuxInfo {
public:
    XanimeBckTable parent; // _0
    
    u8 _4;
    u32 _8;

    // Unreferenced members
    u8 _C[12];
};

class XanimeResourceTable {
public:
    XanimeResourceTable (

        ResourceHolder *,

        XanimeGroupInfo *,
        XanimeAuxInfo *,
        XanimeOfsInfo *,

        XanimeBckTable *,
        XanimeBckTable *,
        XanimeBckTable *,
        XanimeBckTable *,

        XanimeSwapTable *
    );

    XanimeResourceTable(ResourceHolder *);

    u8 _0[0x78];
};

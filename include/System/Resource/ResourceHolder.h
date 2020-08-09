#pragma once

#include "JKernel/JKRArchive.h"
#include "System/Resource/ResourceInfo.h"

namespace
{
    static const char* sModelExt[0x2] =
    {
        ".bdl",
        ".bmd"
    };

    static const char* sMotionExt = ".bck";
    static const char* sBcaExt = ".bca"; // unused
    static const char* sBtkExt = ".btk";
    static const char* sBpkExt = ".bpk";
    static const char* sBtpExt = ".btp";
    static const char* sBlkExt = ".blk";
    static const char* sBrkExt = ".brk";
    static const char* sBasExt = ".bas";
    static const char* sBmtExt = ".bmt";
    static const char* sBvaExt = ".bva";
    static const char* sBamntExt = ".bamnt";
};

class ResourceHolder
{
public:
    ResourceHolder(JKRArchive &);

    const char* getResName(u32) const;
    bool isExistMaterialAnim();

    ResTable* mResourceTable; // _0
    ResTable* _4;
    u8 _8[0x38-0x8];
    u32 _38;
    u32 _3C;
    u32 _40;
    JKRArchive* mArchive; // _44
    u32* _48; // JKRHeap*
    u32 _4C;
};